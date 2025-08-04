#include <iostream>
#include "../../strings/hepler.h"
#include "./path.h"
#include <filesystem>
#include <cstdlib>
using namespace std;

class Crawl
{
    char *seed_url;
    char *target_Dir;
    char *crawling_Depth;

public:
    Crawl(char *url, char *dir, char *depth)
    {
        seed_url = new char[my_strlen(url) + 1];
        target_Dir = new char[my_strlen(dir) + 1];
        crawling_Depth = new char[my_strlen(depth) + 1];
        my_strcpy(seed_url, url);
        my_strcpy(target_Dir, dir);
        my_strcpy(crawling_Depth, depth);
    }

    void downloadPage()
    {
        bool exists = check_Path(target_Dir);
        if (!exists)
        {
            create_Dir(target_Dir);
        }

        char downloadPath[200] = "../public/";
        my_strcat(downloadPath, target_Dir);
        my_strcat(downloadPath, "/downloaded_page.html");

        char command[300] = "wget -O ";
        my_strcat(command, downloadPath);
        my_strcat(command, " ");
        my_strcat(command, seed_url);

        int result = system(command);
        if (result == 0)
        {
            cout << "Web page downloaded successfully " << endl;
        }
        else
        {
            cout << "Error downloading web page exit code is " << result << endl;
        }
    }
    ~Crawl()
    {
        delete seed_url;
        delete target_Dir;
        delete crawling_Depth;
    }
};
