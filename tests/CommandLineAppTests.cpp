#include "CommandLineApp.hpp"

#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

namespace{
    class TempDir{
        public: 
        TempDir(){
            const fs::path temp = fs::temp_directory_path();
            for (int i = 0;;i++){
                fs::path test1 = temp / ("minigit_cl1_tests_" + std::to_string(i));
                if (fs::create_directory(test1)){
                    path_ = test1;
                    return;
                }
            }
        }
        ~TempDir(){
            std::error_code cleanupError;
            fs::remove_all(path_, cleanupError);
        }
        TempDir(const TempDir&) = delete;
        TempDir& operator=(const TempDir&) = delete;

        const fs::path& path() const{return path_;}

        private:
            fs::path path_;
    };

    bool isEmpty(const fs::path& path){
        return fs::directory_iterator(path) == fs::directory_iterator();
    }

    bool LeavesFolderEmpty(){
        TempDir temp;
        if (!isEmpty(temp.path())){
            std::cerr << "Constructor creates a non-empty folder: " << temp.path() << '\n';
            return false;
        }

        {
            minigit::CommandLineApp app(temp.path());
        }

        return isEmpty(temp.path());
    }

}
int main(){
    if(!LeavesFolderEmpty()){
        std::cerr << "CommandLineApp constructor leaves the folder not empty\n";
        return 1;
    }
    std::cout << "CommandlineApp constructor leaves the folder empty\n";
    return 0;
}
