#include "MiniGitRepository.hpp"

#include <filesystem>
#include <iostream>
#include <string> 
#include <system_error>

namespace fs = std::filesystem;

namespace{
    class TempDir{
        public:
        TempDir(){
            const fs::path temp = fs::temp_directory_path();
            for (int i = 0;;i++){
                fs::path construct = temp / ("minigit_repositories_tests_" + std::to_string(i));
                if (fs::create_directory(construct)){
                    path_ = construct;
                    return;
                }
            }
        }
        ~TempDir(){
            std::error_code ec;
            fs::remove_all(path_, ec);
        }
        TempDir(const TempDir&) = delete;
        TempDir& operator=(const TempDir&) = delete;

        const fs::path& path() const{return path_;}

        private:
            fs::path path_;
    };
    
    bool isEmpty(const fs::path& directory){
        return fs::directory_iterator(directory) == fs::directory_iterator();
    }

    bool LeavesFolderEmpty(){
        TempDir temp;
        if (!isEmpty(temp.path())){
            std::cerr << "Constructor creates a non-empty folder: " << temp.path() << '\n';
            return false;
        }

        {
            minigit::MiniGitRepository repo(temp.path());
        }

        return isEmpty(temp.path());
    }
}

int main(){
    if(!LeavesFolderEmpty()){
        std::cerr << "MiniGitRepository constructor leaves the folder not empty\n";
        return 1;
    }
    std::cout << "MiniGitRepository constructor leaves the folder empty\n";
    return 0;
}