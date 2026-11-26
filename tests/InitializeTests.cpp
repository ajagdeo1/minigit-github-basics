#include "MiniGitRepository.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace fs = std::filesystem;

namespace {

int failures = 0;

void check(bool condition, const std::string& message) {
    std::cout << (condition ? "  PASS  " : "  FAIL  ") << message << '\n';
    if (!condition) {
        ++failures;
    }
}

fs::path makeTempFolder(const std::string& name) {
    const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
    const fs::path folder = fs::temp_directory_path() / ("minigit-m2-4-" + name + "-" + std::to_string(stamp));
    fs::create_directories(folder);
    return folder;
}

void makeFile(const fs::path& file, const std::string& bytes) {
    std::ofstream output(file, std::ios::binary);
    output.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
}

std::string readFile(const fs::path& file) {
    std::ifstream input(file, std::ios::binary);
    std::ostringstream contents;
    contents << input.rdbuf();
    return contents.str();
}

} 

int main() {
    // Empty folder: creates every part of the storage format.
    const fs::path root = makeTempFolder("new");
    makeFile(root / "notes.txt", "project file");
    const fs::path metadata = root / ".minigit";

    minigit::MiniGitRepository repository(root);
    repository.initialize();
    check(fs::is_directory(metadata), "initialize() creates .minigit/");
    check(fs::is_directory(metadata / "stage"), "initialize() creates .minigit/stage/");
    check(fs::is_directory(metadata / "commits"), "initialize() creates .minigit/commits/");
    check(fs::is_regular_file(metadata / "HEAD") && fs::file_size(metadata / "HEAD") == 0,
          "initialize() creates an empty .minigit/HEAD");
    check(fs::is_regular_file(metadata / "index.txt") && fs::file_size(metadata / "index.txt") == 0,
          "initialize() creates an empty .minigit/index.txt");
    check(readFile(root / "notes.txt") == "project file", "initialize() does not change project files");

    bool valid = true;
    try {
        repository.requireRepository();
    } catch (const std::runtime_error&) {
        valid = false;
    }
    check(valid, "requireRepository() accepts the repository initialize() created");

    
    makeFile(metadata / "HEAD", "5");
    std::string message;
    try {
        repository.initialize();
    } catch (const std::runtime_error& e) {
        message = e.what();
    }
    check(message == "repository already exists", "a second initialize() reports \"repository already exists\"");
    check(readFile(metadata / "HEAD") == "5", "a second initialize() does not change HEAD");

    // if the head file is missing this throws and does not repair it.
    const fs::path damaged = makeTempFolder("damaged");
    fs::create_directories(damaged / ".minigit");
    minigit::MiniGitRepository damagedRepository(damaged);
    bool threw = false;
    try {
        damagedRepository.initialize();
    } catch (const std::runtime_error&) {
        threw = true;
    }
    check(threw, "initialize() throws when .minigit is damaged");
    check(!fs::exists(damaged / ".minigit" / "HEAD"), "initialize() changes nothing in a damaged .minigit");

    std::error_code cleanupError;
    fs::remove_all(root, cleanupError);
    fs::remove_all(damaged, cleanupError);

    std::cout << '\n' << failures << " failed\n";
    return failures == 0 ? 0 : 1;
}