#include "writingTextFile.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
using namespace std;

int main()  
{
filesystem::path test_folder = filesystem::temp_directory_path() / "issue3";
filesystem::path nested_folder = test_folder / "Nested";
filesystem::path filepath = nested_folder / "txt.txt";
filesystem::remove_all(test_folder);

bool testingWriting = minigit::writeTextFile(filepath, "Created");

if(!testingWriting)
{
  cout << "Failed" << endl;
  filesystem::remove_all(test_folder);
  return 1;
}
string txt;
ifstream file(filepath);
file >> txt;
file.close();
  
if (txt == "Created")
{
  cout << "Passed" << endl;
  filesystem::remove_all(test_folder);
  return 0;
}
else
{
cout << "Failed" << endl;
  filesystem::remove_all(test_folder);
  return 1;
}
}






    
  
