#include "pch.h"
#include <filesystem>
#include <iostream>

namespace TestBlackBox {

    namespace fs = std::filesystem;

    TEST(BlackBoxTests, ListInputFiles) {
        const fs::path inputDir = fs::current_path().parent_path() / "shmax_tests" / "blackbox";

        GTEST_LOG_(INFO) << "Checking folder: " << inputDir;

        if (!fs::exists(inputDir)) {
            FAIL() << "Input directory not found: " << inputDir;
            return;
        }

        int count = 0;
        for (const auto& entry : fs::directory_iterator(inputDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".replicode") {
                GTEST_LOG_(INFO) << "Found file: " << entry.path().filename();
                ++count;
            }
        }

        GTEST_LOG_(INFO) << "Total .replicode files found: " << count;
        SUCCEED(); // Keeps test green
    }

}