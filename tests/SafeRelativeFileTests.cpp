#include "RepositoryPaths.hpp"
#include <iostream>
#include <string>
#include <stdexcept>

int main(){
    int failures = 0;
    const minigit::RepositoryPaths paths("course/repo");

    const auto accepts = [&](const std::string& input, const std::string& expected){
        try{
            if(paths.safeRelativeFile(input) != expected){
                std::cerr << "Failure: incorrect result for " << input << '\n';
                failures++;
            }
        }catch(const std::runtime_error&){
            std::cerr << "Failure: unexpectedly rejected " << input << '\n';
            failures++;
        }
    };
    
    const auto rejects = [&](const std::string& input){
        try{
            paths.safeRelativeFile(input);
            std::cerr << "Failure: expected rejection for " << input << '\n';
            failures++;
        }catch(const std::runtime_error&){
            // Expected behavior
        }
    };

    // valid inputs prove ordinary paths are retained and harmless
    // "." and ".." components are normalized.
    accepts("notes.txt", "notes.txt");
    accepts("docs/notes.txt", "docs/notes.txt");
    accepts("docs/./notes.txt", "docs/notes.txt");
    accepts("docs/../notes.txt", "notes.txt");
    accepts("my..notes.txt", "my..notes.txt");

    // unsafe paths must throw an exception instead of allowing access outside the project
    // or inside MiniGit/Git metadata directories.
    rejects("");
    rejects(".");
    rejects("/notes.txt");
    rejects("../notes.txt");
    rejects("docs/../../notes.txt");
    rejects(".minigit/HEAD");
    rejects(".git/config");
    rejects("docs/.minigit/x");

    if(failures != 0){
        std::cerr << failures << " checks failed\n";
        return 1;
    }

    std::cout << "All checks passed\n";
    return 0;
}