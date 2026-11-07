#include "RepositoryPaths.hpp"
#include <filesystem>
#include <iostream>

int testCommitLocations(){
    namespace fs = std::filesystem;
    int failures = 0;

    const fs::path root = fs::path("course") / "repo";
    const minigit::RepositoryPaths paths(root);

    if(paths.commitDirectory("3") != root / ".minigit" / "commits" / "3"){
        std::cerr << "Failure: commitDirectory(\"3\")\n";
        failures++;
    }
    if(paths.committedFile("3", "docs/notes.txt") != root / ".minigit" / "commits" / "3" / "files" / "docs" / "notes.txt"){
        std::cerr << "Failure: committedFile(\"3\", \"docs/notes.txt\")\n";
        failures++;
    }
    if(paths.committedFile("3", "a.txt").parent_path() != paths.commitDirectory("3") / "files"){
        std::cerr << "Failure: committedFile(\"3\", \"a.txt\").parent_path()\n";
        failures++;
    }
    if(fs::exists(paths.metadataRoot())){
        std::cerr << "Failure: commit locations created something on disk\n";
        failures++;
    }

    return failures;
}

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
        
        //Issue 6 of Lab
        if(path.headFile() != root / ".minigit" / "HEAD"){
            std::cerr << "Failure: headFile() for " << root << '\n';
            failures++;
        }
        if(path.indexFile() != root / ".minigit" / "index.txt"){
            std::cerr << "Failure: indexFile() for " << root << '\n';
            failures++;
        }

        if(path.headFile().parent_path() != path.metadataRoot()){
            std::cerr << "Failure: headFile() parent path for " << root << '\n';
            failures++;
        }

        if(path.indexFile().filename() != "index.txt"){
            std::cerr << "Failure: indexFile() filename for " << root << '\n';
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
    failures += testCommitLocations();
    if(failures != 0){
        std::cerr << failures << "checks failed.\n";
        return 1;
    }

    std::cout << "All checks passed.\n";
    return 0;
}
