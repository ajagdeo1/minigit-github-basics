#include "RepositoryPaths.hpp"
#include <filesystem>
#include <iostream>

int main(){
    
    namespace fs = std::filesystem;
    int failures = 0;

    const fs::path roots[] = {
        fs::path("course")/ "repo",
        fs::path("coursre")/ "team"/ "repo"
    };

    for(const fs::path& root : roots){
        const minigit::RepositoryPaths path(root);

        if(path.projectRoot() != root){
            std::cerr << "Failure: projectRoot() for " << root << '\n';
            failures++;
        }
        if(path.metadataRoot() != root / ".minigit"){
            std::cerr << "Failure: metadataRoot() for " << root << '\n';
            failures++;
        }  
        if(path.stageFile("notes.txt") != root / ".minigit" / "stage" / "notes.txt"){
            std::cerr << "Failure: stageFile() for " << root << '\n';
            failures++;
        }
        if(path.stageFile("docs/notes.txt") != root / ".minigit" / "stage" /"docs"/ "notes.txt"){
            std::cerr << "Failure: stageFile() for  docs/notes.txt\n";
            failures++;
        }
        if(path.stageFile("docs/notes.txt").filename() != "notes.txt"){
            std::cerr << "Failure: stageFile() filename\n";
            failures++;
        }
    }

    if(failures != 0){
        std::cerr << failures << "checks failed.\n";
        return 1;
    }

    std::cout << "All checks passed.\n";
    return 0;
}
