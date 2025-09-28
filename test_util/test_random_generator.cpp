#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <filesystem>

namespace fs = std::filesystem;

// --- Structures ---
struct Group { std::string name; };
struct Entity { std::string name; };
struct Model { std::string name; };
struct Program { std::string name; };

// --- Generate sequential names ---
std::vector<Group> generate_groups(int count) {
    std::vector<Group> v;
    for (int i = 1; i <= count; ++i)
        v.push_back(Group{ "group_" + std::to_string(i) });
    return v;
}

std::vector<Entity> generate_entities(int count) {
    std::vector<Entity> v;
    for (int i = 1; i <= count; ++i)
        v.push_back(Entity{ "entity_" + std::to_string(i) });
    return v;
}

std::vector<Model> generate_models(int count) {
    std::vector<Model> v;
    for (int i = 1; i <= count; ++i)
        v.push_back(Model{ "model_" + std::to_string(i) });
    return v;
}

std::vector<Program> generate_programs(int count) {
    std::vector<Program> v;
    for (int i = 1; i <= count; ++i)
        v.push_back(Program{ "program_" + std::to_string(i) });
    return v;
}

// --- Template filler ---
std::string fill_template(const std::string& tpl,
    const std::string& name,
    const std::string& model,
    const std::string& group) {
    std::string result = tpl;
    auto replace = [&](const std::string& key, const std::string& value) {
        size_t pos;
        while ((pos = result.find(key)) != std::string::npos)
            result.replace(pos, key.size(), value);
        };
    replace("{name}", name);
    replace("{model}", model);
    replace("{group}", group);
    return result;
}

int main() {
    // --- Ensure folder exists ---
    fs::path dir("../test/blackbox/random");
    fs::create_directories(dir);

    // --- Generate data ---
    auto groups = generate_groups(3);
    auto models = generate_models(3);
    auto entities = generate_entities(5);
    auto programs = generate_programs(2);

    std::string tpl = "<entity name=\"{name}\" model=\"{model}\" group=\"{group}\">";
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<> gdist(0, groups.size() - 1);
    std::uniform_int_distribution<> mdist(0, models.size() - 1);

    // --- Write 10 files ---
    for (int file_idx = 1; file_idx <= 10; ++file_idx) {
        std::ofstream file(dir / ("test_" + std::to_string(file_idx) + ".replicode"));

        for (const auto& e : entities) {
            const auto& g = groups[gdist(rng)];
            const auto& m = models[mdist(rng)];
            file << fill_template(tpl, e.name, m.name, g.name) << "\n";
        }

        for (const auto& p : programs) {
            file << "<program name=\"" << p.name << "\">\n";
        }

        file.close();
        std::cout << "Wrote " << dir / ("test_" + std::to_string(file_idx) + ".replicode") << "\n";
    }
}