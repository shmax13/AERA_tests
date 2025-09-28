#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <filesystem>
#include <chrono>
#include <ctime>
#include <iomanip>

namespace fs = std::filesystem;

// name generation
std::vector<std::string> generate_names(const std::string& prefix, int count) {
    std::vector<std::string> names;
    names.reserve(count);
    for (int i = 0; i <= count; ++i) {
        names.push_back(prefix + "_" + std::to_string(i));
    }
    return names;
}

// string manipulation
void replace_all(std::string& str, const std::string& key, const std::string& value) {
    size_t pos;
    while ((pos = str.find(key)) != std::string::npos) {
        str.replace(pos, key.size(), value);
    }
}

// random float generator 
float rand_float(float min, float max) {
    static thread_local std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng);
}

// random integer generator 
int rand_int(int min, int max) {
    static thread_local std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

std::string create_grp(const std::string& groupName) {
    std::string tpl = "grp_name:(std_grp upr sln_thr act_thr vis_thr ntf_grps) grp_view";

    replace_all(tpl, "grp_name", groupName);
    replace_all(tpl, "upr", "1");
    replace_all(tpl, "sln_thr", std::to_string(rand_float(0, 1)));
    replace_all(tpl, "act_thr", std::to_string(rand_float(0, 1)));
    replace_all(tpl, "vis_thr", std::to_string(rand_float(0, 1)));
    replace_all(tpl, "ntf_grps", "[nil]");
    replace_all(tpl, "grp_view", "[[SYNC_ONCE now 0 forever root nil COV_OFF 0]] ");

    return tpl;
}

std::string create_pgm(const std::string& pgmName) {
    std::string tpl = R"(pgm_name:(pgm
tpl_args
inputs
guards
prods
psln_thr) pgm_view)";

    replace_all(tpl, "pgm_name", pgmName);
    replace_all(tpl, "tpl_args", "|[]");
    replace_all(tpl, "inputs", "|[]");
    replace_all(tpl, "guards", "|[]");
    replace_all(tpl, "prods", "|[]");
    replace_all(tpl, "psln_thr", "1");
    replace_all(tpl, "pgm_view", "|[]");

    return tpl;
}

std::string create_ipgm(const std::string& pgmName) {
    std::string tpl = "ipgm_name:(ipgm pgm_name values run_type time_scope volatile_flag notify_flag psln_thr) ipgm_view";

    replace_all(tpl, "ipgm_name", "i" + pgmName);
    replace_all(tpl, "pgm_name", pgmName);
    replace_all(tpl, "values", "|[]");
    replace_all(tpl, "run_type", "RUN_ALWAYS");
    replace_all(tpl, "time_scope", "100ms");
    replace_all(tpl, "volatile_flag", "VOLATILE");
    replace_all(tpl, "notify_flag", "SILENT");
    replace_all(tpl, "psln_thr", "1");
    replace_all(tpl, "ipgm_view", "[[SYNC_ONCE now 0 1 stdin nil 1]]");

    return tpl;
}


int main() {
    // --- Ensure folder exists ---
    fs::path dir("../test/blackbox/random-tests");
    fs::create_directories(dir);

    // --- Generate data ---
    auto groups = generate_names("grp", 2);
    auto programs = generate_names("pgm", 2);

    // --- Write 10 files ---
    for (int file_idx = 1; file_idx <= 10; ++file_idx) {
        auto baseName = "test_" + std::to_string(file_idx);

        // .replicode file
        std::ofstream repFile(dir / (baseName + ".replicode"));
        
        // header 
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm localTime;
        localtime_s(&localTime, &t);
        repFile << "; Randomly generated in project test_util at "
            << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S")
            << "\n\n";

        // main content
        for (const auto& g : groups) {
            repFile << create_grp(g) << "\n";
        }
        repFile << "\n";
        for (const auto& p : programs) {
            repFile << create_pgm(p) << "\n";
            repFile << create_ipgm(p) << "\n\n";
        }
        repFile.close();

        // empty .txt file
        std::ofstream txtFile(dir / (baseName + ".txt"));
        txtFile.close();

        std::cout << "Wrote " << dir / (baseName + ".replicode")
            << " and " << dir / (baseName + ".txt") << "\n";
    }
}