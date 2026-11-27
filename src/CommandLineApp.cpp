#include "CommandLineApp.hpp"
#include "MiniGitRepository.hpp"
#include <stdexcept>

namespace minigit {
    CommandLineApp::CommandLineApp(const std::filesystem::path& projectRoot): projectRoot_(projectRoot) {
    }

    int CommandLineApp::run(const std::vector<std::string>& arguments,
                        std::ostream& out,
                        std::ostream& err) 
    {
        if (arguments.size() != 1 || arguments[0] != "init") {
        err << "Error: invalid command\n";
        return 1;
        }
        
        try {
        MiniGitRepository repository(projectRoot_);
        repository.initialize();
        return 0;
        }
        catch (const std::exception& e) {
        err << e.what() << '\n';
        return 1;
        }
    }
}

