#pragma once
#include <filesystem>

namespace minigit {

class CommandLineApp {
public:
    explicit CommandLineApp(const std::filesystem::path& projectRoot);

private:
    std::filesystem::path projectRoot_;
};

}