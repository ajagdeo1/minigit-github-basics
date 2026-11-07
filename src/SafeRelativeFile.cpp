#include "RepositoryPaths.hpp"
#include <stdexcept>

namespace minigit {
    std::string RepositoryPaths::safeRelativeFile(const std::string& argument) const{
        // an empty string is not a usable file path.
        if(argument.empty()){
            throw std::runtime_error("file path must not be empty");
        }
        
        const std::filesystem::path input(argument);
        
        // rejects rooted paths so callers cannot escape the repository root.
        if (input.has_root_path()){
            throw std::runtime_error("file path must be relative");
        }

        // collapse harmless path components
        const std::filesystem::path clean = input.lexically_normal();
        // "." names the current directory
        if(clean.empty() || clean == "."){
            throw std::runtime_error("file path must name a file.");
        }

        // path beginning with ".." would escape the repository root
        const auto first = clean.begin();
        if(first !=clean.end() && *first == ".."){
            throw std::runtime_error("file path must stay inside the project");
        }

        // Minigit must not stage its or gits metadata
        for(const auto& part : clean){
            if(part == ".git" || part == ".minigit"){
                throw std::runtime_error("file path must not access metadata.");
            }
        }

        // generic_string() always uses '/' which keeps stored paths portable
        return clean.generic_string();
    }
}
