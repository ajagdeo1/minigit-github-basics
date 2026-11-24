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
    //FS3
    const fs::path file = temp / "written.txt";
    files.writeBytes(file, "1st line\n2nd line\n");
    const auto result = files.readBytes(file);
    check(result.has_value() && *result == "1st line\n2nd line\n",
      "writeBytes writes multiple lines");

    const fs::path replace = temp / "replace.txt";
    makeFile(replace, "Old text");
    files.writeBytes(replace, "New text");
    const auto newResult = files.readBytes(replace);
    check(newResult.has_value() && *newResult == "New text",
      "writeBytes replaces old text");

    const fs::path zero = temp / "zero-FS3.bin";
    const std::string data("A\0B", 3);
    files.writeBytes(zero, data);
    const auto zeroFS3Result = files.readBytes(zero);
    check(zeroFS3Result.has_value() && *zeroFS3Result == data,
      "writeBytes keeps zero bytes");

    const fs::path missingParentFile = temp / "missing" / "file.txt";
    bool writeError = false;
    try {
        files.writeBytes(missingParentFile, "test");
    }
    catch (const std::exception&) {
        writeError = true;
    }
    
    check(writeError, "writeBytes reports an error when the parent directory is missing");

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

    // FS-4 addDirectories()
    const fs::path nested = temp / "Demo" / "Dragon" / "Boo";
    files.addDirectories(nested);
    check(fs::is_directory(nested), "addDirectories() creates nested directories");

    //Existing directories: addDirectories() does not throw or change them.
    bool threw2 = false;
    try {
      files.addDirectories(nested);
      } catch (const std::exception& e) {
            threw2 = true;
    }
    check(!threw2, "addDirectories() does not throw when directories already exist");
    check(fs::is_directory(nested), "addDirectories() does not change existing directories");

    //Regular file exists
    const fs::path regularFile = temp / "regular.txt";
    makeFile(regularFile, "Regular file.");
    bool threw3 = false;
    try {
      files.addDirectories(regularFile);
      } catch (const std::exception& e) {
            threw3 = true;
    }
    check(threw3, "addDirectories() throws when a regular file exists at the path");
    check(fs::is_regular_file(regularFile), "addDirectories() does not change a regular file at the path");

    // FS-5 ReplaceFile() replaces existing targets contents
    const fs::path replaceTarget = temp / "replaceTarget.txt";
    const fs::path preparedText = temp / "preparedText.txt";
    makeFile(replaceTarget, "Old target bytes");
    makeFile(preparedText, "New prepared bytes");

    files.replaceFile(preparedText, replaceTarget);

    const auto replacedResult = files.readBytes(replaceTarget);
    check(replacedResult.has_value() && *replacedResult == "New prepared bytes", "replaceFile replaces the target's old bytes");

    // FS-5 replaceFile() preserves every prepared byte, including null bytes
    const fs::path binaryTarget = temp / "binaryTarget.bin";
    const fs::path preparedBinary = temp / "preparedBinary.bin";
    const std::string binaryBytes("A\0B", 3);

    makeFile(binaryTarget, "Old bytes");
    makeFile(preparedBinary, binaryBytes);

    files.replaceFile(preparedBinary, binaryTarget);

    const auto binaryResult = files.readBytes(binaryTarget);
    check(binaryResult.has_value() && *binaryResult == binaryBytes, "replaceFile preserves exact prepared file bytes");

    // FS-5 replaceFile() missing prepared file throws an exception without modifying target
    const fs::path unchangedTarget = temp / "unchangedTarget.txt";
    const fs::path missingPrepared = temp / "doesNotExist.txt";
    makeFile(unchangedTarget, "Keep these bytes");

    bool replaceThrew = false;
    try{
      files.replaceFile(missingPrepared, unchangedTarget);
    } catch(const std::exception& e){
      replaceThrew = true;
    }

    check(replaceThrew, "replaceFile throws an exception when the prepared file is missing");
    const auto unchangedResult = files.readBytes(unchangedTarget);
    check(unchangedResult.has_value() && *unchangedResult == "Keep these bytes", "replaceFile does not modify the target when the prepared file is missing");
    std::error_code cleanupError;
    fs::remove_all(temp, cleanupError);

    std::cout << '\n' << failures << " failed\n";
    return failures == 0 ? 0 : 1;
}

