#pragma once
#include <filesystem>

#include "FileSystem.hpp"
#include "RepositoryPaths.hpp"

namespace minigit {

class MiniGitRepository {
public:
    explicit MiniGitRepository(const std::filesystem::path& projectRoot);
    void requireRepository() const;
    void initialize();

private:
    RepositoryPaths PathTests_;
    FileSystem FileTests_;
};

}