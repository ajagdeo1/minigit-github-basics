#include "MiniGitRepository.hpp"

#include <utility>

namespace minigit {

MiniGitRepository::MiniGitRepository(const std::filesystem::path& projectRoot)
    : PathTests_(std::move(projectRoot)), FileTests_() {}

} // namespace minigit