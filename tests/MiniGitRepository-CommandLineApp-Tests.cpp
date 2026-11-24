#include <filesystem>
#include <iostream>
#include <string>
#include "MiniGitRepository.hpp"
#include "CommandLineApp.hpp"

namespace fs = std::filesystem;

static fs::path TempFolder(const std::string& name) {
    fs::path temp = fs::temp_directory_path() / ("minigit-tests-" + name);
    fs::remove_all(temp);
    fs::create_directories(temp);
    return temp;
}

static bool isEmpty(const fs::path& path) {
    return fs::directory_iterator(path) == fs::directory_iterator();
}

static bool RepositoryLeavesFolderEmpty(){
    fs::path temp = TempFolder("Repository");
    minigit::MiniGitRepository repo(temp);
    bool check = isEmpty(temp);
    fs::remove_all(temp);
    return check;
}

static bool CommandLineAppLeavesFolderEmpty(){
    fs::path temp = TempFolder("CommandLineApp");
    minigit::CommandLineApp app(temp);
    bool check = isEmpty(temp);
    fs::remove_all(temp);
    return check;
}

int main() {
    int failures = 0;
    if(!RepositoryLeavesFolderEmpty()){
        std::cerr << "Failure: MiniGitRepository leaves folder non-empty\n";
        failures++;
    }
    if(!CommandLineAppLeavesFolderEmpty()){
        std::cerr << "Failure: CommandLineApp leaves folder non-empty\n";
        failures++;
    }
    return failures == 0 ? 0 : 1;
}