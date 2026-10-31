cmake_minimum_required(VERSION 3.20)
project(MiniGitWeek1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(minigit_paths src/PathLab.cpp src/writingTextFile.cpp)
target_include_directories(minigit_paths PUBLIC include)

include(CTest)
if(BUILD_TESTING)
    add_executable(path_lab_tests tests/PathLabTests.cpp)
    target_link_libraries(path_lab_tests PRIVATE minigit_paths)
    add_test(NAME path_lab_tests COMMAND path_lab_tests)
    add_executable(writing_text_file_test tests/writingTextFileTest.cpp)
    target_link_libraries(writing_text_file_test PRIVATE minigit_paths)
    add_test(NAME writing_text_file_test COMMAND writing_text_file_test)
endif()
