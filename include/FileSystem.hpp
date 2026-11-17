#pragma once
#include <filesystem>
#include <optional>
#include <string>

namespace minigit {

// Byte-level file operations for MiniGit.
// FileSystem holds no state and knows nothing about MiniGit's rules.
class FileSystem {
public:
    // return every byte of the file exactly as stored, or std::nullopt when the file does not exist.
   
    std::optional<std::string> readBytes(const std::filesystem::path& path) const;


    void copy(const std::filesystem::path& source, const std::filesystem::path& target) const;

    void addDirectories(const std::filesystem::path& directory) const;
};

} 