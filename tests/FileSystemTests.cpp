#include "FileSystem.hpp"

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

// The tests create their files directly with std::ofstream,
// because writeBytes() is a separate issue (FS-3).
void makeFile(const fs::path& file, const std::string& bytes) {
    std::ofstream output(file, std::ios::binary);
    output.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
}

} // namespace

int main() {
    // A new folder for every run, removed again at the end.
    const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
    const fs::path temp = fs::temp_directory_path() / ("minigit-fs1-tests-" + std::to_string(stamp));
    fs::create_directories(temp);

    // A const object: this only compiles if readBytes() is a const member function.
    const minigit::FileSystem files{};

    // Multiple lines come back exactly, newlines included.
    const fs::path lines = temp / "lines.txt";
    makeFile(lines, "line1\nline2\n");
    const auto linesResult = files.readBytes(lines);
    check(linesResult.has_value() && *linesResult == "line1\nline2\n",
          "readBytes returns multiple lines exactly, newlines included");

    // Binary mode: a Windows line ending is not changed.
    const fs::path crlf = temp / "crlf.txt";
    makeFile(crlf, "a\r\nb");
    const auto crlfResult = files.readBytes(crlf);
    check(crlfResult.has_value() && *crlfResult == "a\r\nb",
          "readBytes keeps CR LF line endings unchanged (binary mode)");

    // A zero byte in the middle of the file is kept.
    const std::string withZero("a\0b", 3);
    const fs::path zeroFile = temp / "zero.bin";
    makeFile(zeroFile, withZero);
    const auto zeroResult = files.readBytes(zeroFile);
    check(zeroResult.has_value() && zeroResult->size() == 3 && *zeroResult == withZero,
          "readBytes keeps a zero byte (same size and bytes)");

    // An empty file has a value: the empty string.
    const fs::path empty = temp / "empty.txt";
    makeFile(empty, "");
    const auto emptyResult = files.readBytes(empty);
    check(emptyResult.has_value() && emptyResult->empty(),
          "readBytes of an empty file returns \"\", not std::nullopt");

    // Negative case: a missing file gives std::nullopt and creates nothing.
    const fs::path missing = temp / "missing.txt";
    check(!files.readBytes(missing).has_value(),
          "readBytes of a missing file returns std::nullopt");
    check(!fs::exists(missing),
          "readBytes of a missing file does not create it");

    // FS-2: copy() copies a file exactly, creating the target if needed.
    const fs::path source = temp / "source.txt";
    const fs::path target = temp / "target.txt";
    makeFile(source, "Testline1\nTestline2\n");
    files.copy(source, target);
    const std::optional<std::string> copyBin = files.readBytes(target);
    check(copyBin.has_value() && *copyBin == "Testline1\nTestline2\n",
          "copy() creates/copies the source file exactly to target file");
    const std::optional<std::string> copyBin2 = files.readBytes(source);
      check(copyBin2.has_value() && *copyBin2 == "Testline1\nTestline2\n",
          "copy() does not change the source file");

    //checks for exact byte content copy
    const std::string track("Testline1\nTestline2\n\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 30);
    const fs::path source2 = temp / "source2.txt";
    const fs::path target2 = temp / "target2.txt";
    makeFile(source2, track);
    files.copy(source2, target2);
    const std::optional<std::string> copyExactBin = files.readBytes(target2);
    check(copyExactBin.has_value() && *copyExactBin == track,
          "copy() keeps all byte contents the same including zero bytes");
    
    //Empty source file copy
    const fs::path source3 = temp / "source3.txt";
    const fs::path target3 = temp / "target3.txt";
    makeFile(source3, "");
    files.copy(source3, target3);
    check(fs::exists(target3) && fs::file_size(target3) == 0,
          "copy() creates an empty target file when source is empty");

    //Copy file into itself
    const fs::path source4 = temp / "source4.txt";
    makeFile(source4, "Testline14\nTestline24\n");
    files.copy(source4, source4);
    const std::optional<std::string> copySelfBin = files.readBytes(source4);
    check(copySelfBin.has_value() && *copySelfBin == "Testline14\nTestline24\n",
          "copy() copies a file into itself without changing its contents");

    //Negative case: missing source file
    const fs::path missingSource = temp / "missingSource.txt";
    const fs::path targetMissing = temp / "targetMissing.txt";
    bool threw = false;
    try {
      files.copy(missingSource, targetMissing);
      } catch (const std::exception& e) {
            threw = true;
    }
    check(threw, "copy of a missing source is thrown");
    check(!fs::exists(targetMissing), "copy of a missing source does not create target file");

    
    std::error_code cleanupError;
    fs::remove_all(temp, cleanupError);

    std::cout << '\n' << failures << " failed\n";
    return failures == 0 ? 0 : 1;

    

}