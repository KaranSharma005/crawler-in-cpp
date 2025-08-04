#include <filesystem>
#include <iostream>
#include "../../strings/hepler.h"
using namespace std;
namespace fs = std::filesystem;

bool check_Path(char *path)
{
    if (fs::exists(path))
    {
        if (fs::is_directory(path))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

void create_Dir(char* name){
    char fullPath[30] = "../";
    my_strcat(fullPath, name);
    cout<<fullPath<<endl;
    if (fs::create_directory(fullPath)) {
        cout<<"Directory created successfully";
    } else {
        cout << "Directory creation failed or already exists." << endl;
    }
}