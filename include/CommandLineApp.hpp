#pragma once
#include <filesystem>
#include <iosfwd>
#include <string>
#include <vector>

namespace minigit {

class CommandLineApp {
public:
    explicit CommandLineApp(const std::filesystem::path& projectRoot);
    int run(const std::vector<std::string>& arguments, std::ostream& out, std::ostream&err);

private:
    std::filesystem::path projectRoot_;
};

}