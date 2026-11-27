cmake_minimum_required(VERSION 3.20)
project(MiniGitWeek1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(minigit_paths 
src/PathLab.cpp 
src/writingTextFile.cpp 
src/readingTextFile.cpp 
src/RepositoryPaths.cpp
src/SafeRelativeFile.cpp
src/FileSystem.cpp
src/CommandLineApp.cpp
src/MiniGitRepository.cpp
)

add_executable(minigit src/Main.cpp)
target_link_libraries(minigit PRIVATE minigit_paths)

target_include_directories(minigit_paths PUBLIC include)

include(CTest)
if(BUILD_TESTING)
    add_executable(path_lab_tests tests/PathLabTests.cpp)
    target_link_libraries(path_lab_tests PRIVATE minigit_paths)
    add_test(NAME path_lab_tests COMMAND path_lab_tests)
    add_executable(writing_text_file_test tests/writingTextFileTest.cpp)
    target_link_libraries(writing_text_file_test PRIVATE minigit_paths)
    add_test(NAME writing_text_file_test COMMAND writing_text_file_test)
    add_executable(reading_text_file_test tests/readingTextFileTest.cpp)
    target_link_libraries(reading_text_file_test PRIVATE minigit_paths)
    add_test(NAME reading_text_file_test COMMAND reading_text_file_test)
    add_executable(repository_paths_tests tests/RepositoryPathsTests.cpp)
    target_link_libraries(repository_paths_tests PRIVATE minigit_paths)
    add_test(NAME repository_paths_tests COMMAND repository_paths_tests)
    add_executable(safe_relative_file_tests tests/SafeRelativeFileTests.cpp)
    target_link_libraries(safe_relative_file_tests PRIVATE minigit_paths)
    add_test(NAME safe_relative_file_tests COMMAND safe_relative_file_tests)
    add_executable(file_system_tests tests/FileSystemTests.cpp)
    target_link_libraries(file_system_tests PRIVATE minigit_paths)
    add_test(NAME file_system_tests COMMAND file_system_tests)

    add_executable(MiniGitRepo-CommandLine_tests tests/MiniGitRepository-CommandLineApp-Tests.cpp)
    target_link_libraries(MiniGitRepo-CommandLine_tests PRIVATE minigit_paths)
    add_test(NAME MiniGitRepo-CommandLine_tests COMMAND MiniGitRepo-CommandLine_tests)
    add_executable(repository_validation_tests tests/RepositoryValidationTests.cpp)
    target_link_libraries(repository_validation_tests PRIVATE minigit_paths)
    add_test(NAME repository_validation_tests COMMAND repository_validation_tests)
    add_executable(initialize_tests tests/InitializeTests.cpp)
    target_link_libraries(initialize_tests PRIVATE minigit_paths)
    add_test(NAME initialize_tests COMMAND initialize_tests)
    endif()
