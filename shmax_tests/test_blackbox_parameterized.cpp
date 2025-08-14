#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <Windows.h>

namespace fs = std::filesystem;

const fs::path sln_path = fs::current_path().parent_path();
const fs::path aera_exe_path = sln_path / "Release" / "AERA.exe";
const fs::path prj_path = sln_path / "shmax_tests";
const fs::path blackbox_path = prj_path / "blackbox";
const fs::path settings_template_path = blackbox_path / "settings_template.xml";
const fs::path settings_filled_path = blackbox_path / "settings_filled.xml";
const fs::path decompiled_objects_path = sln_path / "Debug" / "decompiled_objects.txt";

namespace TestBlackBoxParameterized {

    class BlackBoxTest : public ::testing::TestWithParam<fs::path> {};

    TEST_P(BlackBoxTest, RunAndCheckOutput) {
        const fs::path inputFile = GetParam();
        GTEST_LOG_(INFO) << "Testing file: " << inputFile.filename();

        if (!fs::exists(settings_template_path)) {
            FAIL() << "Template XML file not found: " << settings_template_path;
        }

        // Load XML template (assuming template is stored as a string or file)
        std::ifstream templateFile(settings_template_path);
        std::string xmlContent((std::istreambuf_iterator<char>(templateFile)),
            std::istreambuf_iterator<char>());

        // Replace source_file_name with current input file
        std::string toReplace = "{{SOURCE_FILE_NAME}}";
        std::string replacement = inputFile.filename().string();
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

        // run AERA
        std::string command = "\"" + aera_exe_path.string() + "\" " + settings_filled_path.string();
        int result = std::system(command.c_str());
        ASSERT_EQ(result, 0) << "Failed to run AERA.exe with settings: " << command;

        // Read decompiled output
        std::ifstream decompiledFile(decompiled_objects_path);
        ASSERT_TRUE(decompiledFile) << "Cannot open decompiled objects file: " << decompiled_objects_path;
        std::string decompiledContent((std::istreambuf_iterator<char>(decompiledFile)),
            std::istreambuf_iterator<char>());

        fs::path expectedTxtPath = inputFile;
        expectedTxtPath.replace_extension(".txt");
        ASSERT_TRUE(fs::exists(expectedTxtPath)) << "Expected text file not found: " << expectedTxtPath;

        std::ifstream expectedFile(expectedTxtPath);
        ASSERT_TRUE(expectedFile) << "Cannot open expected text file: " << expectedTxtPath;

        std::string line;
        while (std::getline(expectedFile, line)) {
            if (!line.empty() && decompiledContent.find(line) == std::string::npos) {
                FAIL() << "Expected line not found in decompiled output: " << line;
            }
        }
    }

    INSTANTIATE_TEST_SUITE_P(
        ReplicodeInputs,
        BlackBoxTest,
        ::testing::ValuesIn([]() {
            std::vector<fs::path> inputs;
            GTEST_LOG_(INFO) << "Scanning folder: " << blackbox_path;

            if (!fs::exists(blackbox_path)) {
                GTEST_LOG_(WARNING) << "Directory not found: " << blackbox_path;
                return inputs;
            }

            for (const auto& entry : fs::directory_iterator(blackbox_path)) {
                if (entry.is_regular_file() && entry.path().extension() == ".replicode") {
                    inputs.push_back(entry.path());
                }
            }
            GTEST_LOG_(INFO) << "Found " << inputs.size() << " .replicode files.";
            return inputs;
            }())
    );
}