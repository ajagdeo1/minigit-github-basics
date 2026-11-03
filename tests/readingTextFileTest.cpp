#include "readingTextFile.hpp"
#include "writingTextFile.hpp"
#include <filesystem>
#include <iostream>
#include <string>

bool check(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "readTextFile check failed: " << message << "\n";
        return false;
    }
    return true;
}

int main() {
    const std::filesystem::path testFolder = std::filesystem::temp_directory_path() / "issue4";
    std::filesystem::remove_all(testFolder);

    bool passed = true;

    // Read Back Test
    const std::filesystem::path notesFile = testFolder / "notes.txt";
    const std::string expected = "first line\nsecond line\n";
    passed = check(minigit::writeTextFile(notesFile, expected), "writeTextFile should create the test file") && passed;
    const minigit::ReadResult notes = minigit::readTextFile(notesFile);
    passed = check(notes.ok, "readTextFile should succeed for an existing file") && passed;
    passed = check(notes.text == expected, "readTextFile should return the exact contents") && passed;

    // Empty File Test
    const std::filesystem::path emptyFile = testFolder / "empty.txt";
    passed = check(minigit::writeTextFile(emptyFile, ""), "writeTextFile should create an empty file") && passed;
    const minigit::ReadResult empty = minigit::readTextFile(emptyFile);
    passed = check(empty.ok, "readTextFile should succeed for an empty file") && passed;
    passed = check(empty.text.empty(), "readTextFile should return no text for an empty file") && passed;

    // Missing File Test
    const std::filesystem::path missingFile = testFolder / "missing.txt";
    const minigit::ReadResult missing = minigit::readTextFile(missingFile);
    passed = check(!missing.ok, "readTextFile should fail for a missing file") && passed;
    passed = check(missing.error.find("does not exist") != std::string::npos,
                   "readTextFile should say the missing file does not exist") && passed;

    std::filesystem::remove_all(testFolder);

    if (!passed) {
        return 1;
    }
    std::cout << "readTextFile checks passed\n";
    return 0;
}
