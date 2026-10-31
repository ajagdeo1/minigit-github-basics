#include "PathLab.hpp"

namespace minigit {
// Implement the focused behavior described by the active Issue.
std::filesystem::path metadataDirectory(const std::filesystem::path& root){
    return root / ".minigit";
}

std::filesystem::path HeadFile(const std::filesystem::path& repoRoot) {
    return metadataDirectory(repoRoot) / "HEAD";
}
} // namespace minigit
