#include <iostream>
#include "../../strings/hepler.h"
#include "./path.h"
#include <filesystem>
#include <ctime>
#include <cstdio>
using namespace std;

class Crawl
{
    char *seed_url;
    char *target_Dir;
    int crawling_Depth;
    char downloaded_Path[200];

public:
    Crawl(char *url, char *dir, int depth)
    {
        seed_url = new char[my_strlen(url) + 1];
        target_Dir = new char[my_strlen(dir) + 1];
        crawling_Depth = depth;
        my_strcpy(seed_url, url);
        my_strcpy(target_Dir, dir);
    }

    void downloadPage()
    {
        bool exists = check_Path(target_Dir);
        if (!exists)
        {
            create_Dir(target_Dir);
        }

        char downloadPath[200] = "../public/";
        clock_t timeStamp = time(0);
        char timeCharArray[30];
        sprintf(timeCharArray, "%ld", timeStamp);
        my_strcat(downloadPath, target_Dir);
        my_strcat(downloadPath, (char*)"/");
        my_strcat(downloadPath, timeCharArray);
        my_strcat(downloadPath, (char*)"downloaded_page.html");
        my_strcpy(downloaded_Path, downloadPath);

        char command[300] = "wget -O ";
        my_strcat(command, downloadPath);
        my_strcat(command, (char*)" ");
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

    void extractLinks(){
        getLinks(downloaded_Path, crawling_Depth);
    }

    ~Crawl()
    {
        delete seed_url;
        delete target_Dir;
    }
};
