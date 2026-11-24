#include "MiniGitRepository.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
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
    const fs::path folder = fs::temp_directory_path() / ("minigit-m2-3-" + name + "-" + std::to_string(stamp));
    fs::create_directories(folder);
    return folder;
}

// Builds a complete .minigit by hand, because initialize() is a separate issue (M2-4).
void makeValidRepository(const fs::path& root) {
    const fs::path metadata = root / ".minigit";
    fs::create_directories(metadata / "stage");
    fs::create_directories(metadata / "commits");
    std::ofstream(metadata / "HEAD", std::ios::binary);
    std::ofstream(metadata / "index.txt", std::ios::binary);
}

bool throwsOnCheck(const fs::path& root) {
    const minigit::MiniGitRepository repository(root);
    try {
        repository.requireRepository();
    } catch (const std::runtime_error&) {
        return true;
    }
    return false;
}

} 

int main() {
    
    const fs::path empty = makeTempFolder("empty");
    check(throwsOnCheck(empty), "requireRepository() throws for an empty folder");
    check(fs::is_empty(empty), "requireRepository() does not create anything");

    
    const fs::path valid = makeTempFolder("valid");
    makeValidRepository(valid);
    check(!throwsOnCheck(valid), "requireRepository() accepts a complete .minigit");

    
    const fs::path noHead = makeTempFolder("no-head");
    makeValidRepository(noHead);
    fs::remove(noHead / ".minigit" / "HEAD");
    check(throwsOnCheck(noHead), "requireRepository() throws when HEAD is missing");
    check(!fs::exists(noHead / ".minigit" / "HEAD"), "requireRepository() does not recreate HEAD");

  
    const fs::path noIndex = makeTempFolder("no-index");
    makeValidRepository(noIndex);
    fs::remove(noIndex / ".minigit" / "index.txt");
    check(throwsOnCheck(noIndex), "requireRepository() throws when index.txt is missing");

    
    const fs::path fileNotFolder = makeTempFolder("file");
    std::ofstream(fileNotFolder / ".minigit", std::ios::binary) << "not a folder";
    check(throwsOnCheck(fileNotFolder), "requireRepository() throws when .minigit is a regular file");

    std::error_code cleanupError;
    for (const fs::path& folder : {empty, valid, noHead, noIndex, fileNotFolder}) {
        fs::remove_all(folder, cleanupError);
    }

    std::cout << '\n' << failures << " failed\n";
    return failures == 0 ? 0 : 1;
}