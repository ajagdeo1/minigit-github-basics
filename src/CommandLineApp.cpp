#include "CommandLineApp.hpp"

#include <utility>

namespace minigit {

CommandLineApp::CommandLineApp(const std::filesystem::path& projectRoot)
    : projectRoot_(std::move(projectRoot)) {}

} // namespace minigit