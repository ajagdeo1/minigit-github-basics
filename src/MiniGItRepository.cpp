#include "MiniGitRepository.hpp"

namespace minigit {
    MiniGitRepository::MiniGitRepository(const std::filesystem::path& projectRoot): PathTests_(projectRoot), FileTests_() {
    }
}