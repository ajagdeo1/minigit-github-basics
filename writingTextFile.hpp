#pragma once
#include <string>
#include <filesystem>

namespace minigit
{
  bool writeTextFile(const std::filesystem::path& filepath, const std::string& text);
}
