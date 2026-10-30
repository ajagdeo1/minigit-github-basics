#include "PathLab.hpp"
#include <iostream>
#include <string>

bool check (bool condition, const std::string& message){
    if(!condition){
        std::cerr << "PathLab check failed: " << message << "\n";
        return false;
    }
    return true;
}

int main() {
    // Baseline smoke check: the project and CTest wiring work before students
    // begin the path Issues. Add focused assertions with each Issue.
    std::filesystem::path sample = "course/repo";
    if (sample.filename() != "repo") {
        std::cerr << "path smoke check failed\n";
        return 1;
    }

    // ROOT TEST
    const std::filesystem::path root = "course/repo";
    const std::filesystem::path metadataPath = minigit::metadataDirectory(root);

    if(!check(metadataPath.filename() == ".minigit", "metadataDirectory should append .minigit to the root path")) {
        return 1;
    }
    if(!check(metadataPath.parent_path() == root, "metadataDirectory should place .minigit below the repo root path")) {
        return 1;
    }

    // NESTED ROOT TEST
    const std::filesystem::path nestedRoot = "demo/course/repo";
    const std::filesystem::path nestedMetadataPath = minigit::metadataDirectory(nestedRoot);

    if(!check(nestedMetadataPath.filename() == ".minigit", "metadataDirectory should append .minigit to a nested root path")) {
        return 1;
    }
    if(!check(nestedMetadataPath.parent_path() == nestedRoot, "metadataDirectory should place .minigit below a nested repo root path")) {
        return 1;
    }

    std::cout << "PathLab smoke check passed\n";
    return 0;
}
