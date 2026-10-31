#include "writingTextFile.hpp"
#include <system_error>
#include <filesystem>
#include <fstream>

namespace minigit {

bool writeTextFile(const std::filesystem::path& filepath, const std::string& text) {
  if (filepath.has_parent_path()) 
  {
  std::error_code 
  error;
    
  std::filesystem::create_directories(filepath.parent_path(), error);
  if (error) 
  {
  return false;
  
  }
  }
  std::ofstream file(filepath);
  if (!file)
  {
    return false;
  }
  file << text;

  if (!file)
  {
    return false;
  }
  return true;
  }
}
