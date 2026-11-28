#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
// unix systems need this header to read process exit code, WIN does not provide it.
#ifndef _WIN32
#include <sys/wait.h>
#endif

namespace fs = std::filesystem;

// std::system returns process information differently on WIN and UNIX
// This converts the reult into the program's actual exit code.
int exitCode(int result){
    if(result == -1){
        // CMD could not start
        return -1;
    }
    #ifdef _WIN32
        return result;
    #else  
        if (WIFEXITED(result)){
            return WEXITSTATUS(result);
        }
        // Proc did not exit normally.
        return -1;
    #endif
}

// Read a file into a string. Used to check the error output from second run of init.
std::string readFile(const fs::path& path){
    std::ifstream input(path);
    std::ostringstream contents;
    contents << input.rdbuf();
    return contents.str();
}

int main(int argc, char* argv[]){
    //CTest passes path to compiled minigit exe.
    if(argc != 2){
        std::cerr << "Expected the path to the minigit executable\n";
        return 1;
    }

    // Saves an absolute path because the test changes its working directory
    const fs::path executable = fs::absolute(argv[1]);

    // Use a timestamp to give this test run its own temp folder
    const auto timestamp = std::chrono::steady_clock::now().time_since_epoch().count();

    const fs::path testDir = fs::temp_directory_path() / ("minigit-init-test-" + std::to_string(timestamp));

    // redirects minigit's standard error into this file
    const fs::path errorFile = testDir / "error.txt";

    const fs::path originalDir = fs::current_path();

    int failures = 0;
    
    // Print the result of each check and count any failures
    auto check = [&failures](bool condition, const std::string& message){
        std::cout << (condition ? "PASS: " : "FAIL: ") << message << '\n';

        if(!condition){
            ++failures;
        }
    };
    try{
        // Create a temp working folder outside the source repo
        fs::create_directories(testDir);

        // adds quotes to both paths so paths containing spaces can be handled
        const std::string command = "\"" + executable.string() + "\" init 2> \"" + errorFile.string() + "\"";

        // init uses current folder as project root and save the original folder so we can restore it afterward
        fs::current_path(testDir);

        // Run the compiled minigit program 
        const int firstResult = exitCode(std::system(command.c_str()));

        check(firstResult == 0, "first init exits with code 0");

        // check that init created every part of the expected layout
        const fs::path metadata = testDir / ".minigit";

        check(fs::is_directory(metadata), ".minigit directory exists");
        check(fs::is_regular_file(metadata / "HEAD"), "HEAD file exists");
        check(fs::is_regular_file(metadata / "index.txt"), "index.txt file exists");
        check(fs::is_directory(metadata / "stage"), "stage directory exists");
        check(fs::is_directory(metadata / "commits"), "commits directory exists");

        // Run init again in the same folder, expected to fail working on issue M2-5
        const int secondResult = exitCode(std::system(command.c_str()));

        check(secondResult == 1, "second init exits with code 1");
        check(!readFile(errorFile).empty(), "second init writes to stderr");

        // Leave the test folder before deleting it, the clean up
        fs::current_path(originalDir);
        fs::remove_all(testDir);
    } catch(const std::exception& error){
        std::cerr << "Test error: " << error.what() << '\n';

        // Use error_code so cleanup itself doesn't throw another exception
        std::error_code cleanupError;
        fs::current_path(originalDir, cleanupError);

        if(cleanupError){
            std::cerr << "Could not restore working directory: " << cleanupError.message() << '\n';
        }else {
            fs::remove_all(testDir, cleanupError);

            if(cleanupError){
                std::cerr << "Could not remove test folder: " << cleanupError.message() << '\n';
            }
        }

        return 1;
    }

    // CTest treats exit code 0 as a pass and a nonzero code as a fail
    return failures == 0 ? 0 : 1;
}



