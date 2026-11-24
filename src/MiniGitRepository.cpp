#include "MiniGitRepository.hpp"
#include <stdexcept>
#include <system_error>

namespace minigit {
    MiniGitRepository::MiniGitRepository(const std::filesystem::path& projectRoot): PathTests_(projectRoot), FileTests_() {
    }

    void MiniGitRepository::requireRepository() const {
    namespace fs = std::filesystem;
    std::error_code error;
    const fs::path metadata = PathTests_.metadataRoot();

    if (!fs::is_directory(metadata, error)) {
        throw std::runtime_error("not a MiniGit repository: " + PathTests_.projectRoot().string());
    }
    if (!fs::is_regular_file(PathTests_.headFile(), error)) {
        throw std::runtime_error("damaged repository: missing HEAD");
    }
    if (!fs::is_regular_file(PathTests_.indexFile(), error)) {
        throw std::runtime_error("damaged repository: missing index.txt");
    }
    if (!fs::is_directory(metadata / "stage", error)) {
        throw std::runtime_error("damaged repository: missing stage/");
    }
    if (!fs::is_directory(metadata / "commits", error)) {
        throw std::runtime_error("damaged repository: missing commits/");
    }
}

    void MiniGitRepository::initialize() {
        std::error_code error;
        if (std::filesystem::exists(PathTests_.metadataRoot(), error)) {
            requireRepository();
            throw std::runtime_error("repository already exists");
        }

        FileTests_.addDirectories(PathTests_.metadataRoot() / "stage");
        FileTests_.addDirectories(PathTests_.metadataRoot() / "commits");
        FileTests_.writeBytes(PathTests_.headFile(), "");
        FileTests_.writeBytes(PathTests_.indexFile(), "");
    }
}
