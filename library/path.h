#include <filesystem>
#include <iostream> 
#include <fstream>
#include "../../strings/hepler.h"
#include "./isHTML.h"
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
    char fullPath[30] = "../public/";
    my_strcat(fullPath, name);
    cout<<fullPath<<endl;
    if (fs::create_directory(fullPath)) {
        cout<<"Directory created successfully";
    } else {
        cout << "Directory creation failed or already exists." << endl;
    }
}

void getLinks(char* fileName, int depth){
    fstream in(fileName);
    char c;
    char link[150];
    char startsWith[] = "href";
    int index = 0;

    while (in.get(c)) {
        if (startsWith[index] != '\0' && startsWith[index] == c) {
            index++;
        }
        else if (startsWith[index] == '\0') {
            while (in.get(c)) {
                if (c == '"' || c == '\'')
                    break;
            }

            char url[150];
            int urlIndex = 0;
            while (in.get(c)) {
                if (c == '"' || c == '\'')
                    break; 
                url[urlIndex++] = c;
            }
            url[urlIndex] = '\0';
            if(isHtml(url)){
                cout<<url<<endl;
            }
            index = 0; 
        }
        else {
            index = 0;
        }
    }
}