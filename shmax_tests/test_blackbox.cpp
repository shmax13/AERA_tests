#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

const fs::path sln_path = fs::current_path().parent_path();
const fs::path aera_exe_path = sln_path / "Release" / "AERA.exe";
const fs::path prj_path = sln_path / "shmax_tests";
const fs::path blackbox_path = prj_path / "blackbox";
const fs::path settings_template_path = blackbox_path / "settings_template.xml";
const fs::path settings_filled_path = blackbox_path / "settings_filled.xml";
const fs::path output_log_path = blackbox_path / "AERA_output.log";
const fs::path decompiled_objects_path = sln_path / "Debug" / "decompiled_objects.txt";

namespace BlackBoxTests { 

    class BlackBoxTest : public ::testing::TestWithParam<fs::path> {
    protected:
        // optional setup/teardown
        void SetUp() override {}
        void TearDown() override {}
    };

    TEST_P(BlackBoxTest, BlackBox) {
        const fs::path inputFile = GetParam();
        GTEST_LOG_(INFO) << "Testing file: " << inputFile.filename();

        if (!fs::exists(settings_template_path)) {
            FAIL() << "Template XML file not found: " << settings_template_path;
        }

        // Load XML template (assuming template is stored as a string or file)
        std::ifstream templateFile(settings_template_path);
        std::string xmlContent((std::istreambuf_iterator<char>(templateFile)),
            std::istreambuf_iterator<char>());

        // Get path relative to blackbox_path
        fs::path relPath = fs::relative(inputFile, blackbox_path);

        // Convert to string with forward slashes (if needed for XML)
        std::string replacement = relPath.generic_string();

        // Replace {{SOURCE_FILE_NAME}} in XML template
        std::string toReplace = "{{SOURCE_FILE_NAME}}";
        size_t pos = xmlContent.find(toReplace);
        if (pos != std::string::npos) {
            xmlContent.replace(pos, toReplace.length(), replacement);
        }
        else {
            FAIL() << "Failed to find source_file_name in XML template";
        }

        // Write modified XML to temp file for this test run
        std::ofstream outFile(settings_filled_path);
        outFile << xmlContent;
        outFile.close();

        // Clear previous contents
        std::ofstream ofs(output_log_path, std::ofstream::trunc);
        ofs.close();

        // Check if this is a negative test
        bool expectDecompilation = inputFile.filename().string().rfind("testFail", 0) != 0;

        // Run AERA and redirect stdout/stderr to log file
        std::string command = "cmd /C \"\""
            + aera_exe_path.string() + "\" \""
            + settings_filled_path.string() + "\" > \""
            + output_log_path.string() + "\" 2>&1\"";
        std::string x = command.c_str();
        int result = std::system(command.c_str());

        // Read output log
        std::ifstream logFile(output_log_path);
        ASSERT_TRUE(logFile) << "Cannot open AERA output log: " << output_log_path;

        std::string logContent((std::istreambuf_iterator<char>(logFile)),
            std::istreambuf_iterator<char>());

        // Load expected lines
        fs::path expectedTxtPath = inputFile;
        expectedTxtPath.replace_extension(".txt");
        ASSERT_TRUE(fs::exists(expectedTxtPath)) << "Expected text file not found: " << expectedTxtPath;

        std::ifstream expectedFile(expectedTxtPath);
        ASSERT_TRUE(expectedFile) << "Cannot open expected text file: " << expectedTxtPath;

        // Collect expected lines
        std::vector<std::string> expectedLines;
        std::string line;
        while (std::getline(expectedFile, line)) {
            if (!line.empty()) expectedLines.push_back(line);
        }

        if (expectDecompilation) {
            // Positive test: DECOMPILATION must appear
            ASSERT_NE(logContent.find("DECOMPILATION"), std::string::npos)
                << "AERA call failed, 'DECOMPILATION' not found in output. See " << output_log_path;

            // Check expected lines in decompiled file
            std::ifstream decompiledFile(decompiled_objects_path);
            ASSERT_TRUE(decompiledFile) << "Cannot open decompiled objects file: " << decompiled_objects_path;

            std::string decompiledContent((std::istreambuf_iterator<char>(decompiledFile)),
                std::istreambuf_iterator<char>());

            for (const auto& expectedLine : expectedLines) {
                if (decompiledContent.find(expectedLine) == std::string::npos) {
                    FAIL() << "Expected line not found in decompiled output: " << expectedLine;
                }
            }
        }
        else {
            // Negative test: DECOMPILATION must NOT appear
            ASSERT_EQ(logContent.find("DECOMPILATION"), std::string::npos)
                << "Unexpected decompilation for negative test: " << inputFile;

            // Check expected lines in log instead
            for (const auto& expectedLine : expectedLines) {
                // Skip lines starting with ';'
                if (!expectedLine.empty() && expectedLine.front() == ';') {
                    continue;
                }

                if (logContent.find(expectedLine) == std::string::npos) {
                    FAIL() << "Expected line not found in log output: " << expectedLine;
                }
            }
        }
    }

    // TODO: some of the blackbox tests can fail seemingly at random. 
    // this makes sure they aren't executed.
    static const std::vector<std::string> blacklist = {
        "mk",  // marker classes
        "performance-counters" 
    };

    // Helper: recursively gather all .replicode files in a folder
    std::vector<fs::path> GetReplicodeFiles(const fs::path& folder) {
        std::vector<fs::path> files;
        if (!fs::exists(folder)) return files;
        for (const auto& entry : fs::recursive_directory_iterator(folder))
            if (entry.is_regular_file() && entry.path().extension() == ".replicode") {
                std::string name = entry.path().parent_path().filename().string() + "_" +
                    entry.path().stem().string();

                bool blacklisted = false;
                for (const auto& bad : blacklist) {
                    if (name.find(bad) != std::string::npos) {
                        blacklisted = true;
                        break;
                    }
                }

                if (!blacklisted)
                    files.push_back(entry.path());
            }
        return files;
    }

    // EXISTING
    /*INSTANTIATE_TEST_SUITE_P(
        EXISTING,
        BlackBoxTest,
        ::testing::ValuesIn(GetReplicodeFiles(blackbox_path / "existing")),
        [](const ::testing::TestParamInfo<fs::path>& info) {
            std::string name = info.param.parent_path().filename().string() + "_" +
                info.param.filename().stem().string();
            for (auto& c : name) if (!isalnum(c)) c = '_';
            return name;
        }
    );*/

    // TECHNICAL REPORT - MAIN
    INSTANTIATE_TEST_SUITE_P(
        MAIN,
        BlackBoxTest,
        ::testing::ValuesIn(GetReplicodeFiles(blackbox_path / "technical-report/main")),
        [](const ::testing::TestParamInfo<fs::path>& info) {
            std::string name = info.param.parent_path().filename().string() + "_" +
                info.param.filename().stem().string();
            for (auto& c : name) if (!isalnum(c)) c = '_';
            return name;
        }
    );

    // TECHNICAL REPORT - ANNEX 1
    INSTANTIATE_TEST_SUITE_P(
        ANNEX_1,
        BlackBoxTest,
        ::testing::ValuesIn(GetReplicodeFiles(blackbox_path / "technical-report/annex1")),
        [](const ::testing::TestParamInfo<fs::path>& info) {
            std::string name = info.param.parent_path().filename().string() + "_" +
                info.param.filename().stem().string();
            for (auto& c : name) if (!isalnum(c)) c = '_';
            return name;
        }
    );

    // TECHNICAL REPORT - ANNEX 2
    INSTANTIATE_TEST_SUITE_P(
        ANNEX_2,
        BlackBoxTest,
        ::testing::ValuesIn(GetReplicodeFiles(blackbox_path / "technical-report/annex2")),
        [](const ::testing::TestParamInfo<fs::path>& info) {
            std::string name = info.param.parent_path().filename().string() + "_" +
                info.param.filename().stem().string();
            for (auto& c : name) if (!isalnum(c)) c = '_';
            return name;
        }
    );

    // FREE TESTS
    INSTANTIATE_TEST_SUITE_P(
        FREE_TESTS,
        BlackBoxTest,
        ::testing::ValuesIn(GetReplicodeFiles(blackbox_path / "free-tests")),
        [](const ::testing::TestParamInfo<fs::path>& info) {
            std::string name = info.param.parent_path().filename().string() + "_" +
                info.param.filename().stem().string();
            for (auto& c : name) if (!isalnum(c)) c = '_';
            return name;
        }
    );
}