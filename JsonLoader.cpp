#include "JsonLoader.h"
#include <fstream>
#include "include/json.hpp"
using json = nlohmann::json;

void JsonLoader::SaveJson(int score) {
    json data;
    data["score"] = score;
    std::ofstream file("data.json");
    file << data.dump(4); // Pretty-print with 4-space indent
}

int JsonLoader::LoadJson() {
    std::ifstream file("data.json");  // Open the file
    json data;                        // Create a json object
    file >> data;                     // Parse the contents of the file into the json object
    return data["score"];
}