#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <ctime>
#include <cstdio>
#include "../../strings/hepler.h"
#include "../../HashMapGeneric/library/genericHash.h"
#include "./path.h"
#include "./isHTML.h"
#include "../../Genericlists/library/operations.h"

using namespace std;

class Crawl
{
    char *seed_url;
    char *target_Dir;
    int crawling_Depth;
    char downloaded_Path[200];
    HashMap<char *, char *> *allURL = new HashMap<char *, char *>(30);

public:
    Crawl(char *url, char *dir, int depth)
    {
        seed_url = new char[my_strlen(url) + 1];
        target_Dir = new char[my_strlen(dir) + 1];
        crawling_Depth = depth;
        my_strcpy(seed_url, url);
        my_strcpy(target_Dir, dir);
    }

    void downloadPage(char *url, char *dir, int depth)
    {
        if (!check_Path(target_Dir))
        {
            create_Dir(target_Dir);
        }

        char downloadPath[200] = "../public/";
        clock_t timeStamp = time(0);
        char timeCharArray[30];
        sprintf(timeCharArray, "%ld", timeStamp);

        my_strcat(downloadPath, dir);
        my_strcat(downloadPath, (char *)"/");
        my_strcat(downloadPath, timeCharArray);
        my_strcat(downloadPath, (char *)"downloaded_page.html");
        my_strcpy(downloaded_Path, downloadPath);

        char command[300] = "wget -O ";
        my_strcat(command, downloadPath);
        my_strcat(command, (char *)" ");
        my_strcat(command, url);

        int result = system(command);
        if (result == 0) {
            cout << "Successfull" <<"\n";
        } else {
            cout << "Failed"<<url<<"\n";
        }
    }

    List<char *> extractLinks(char *fileName)
    {
        ifstream in(fileName);
        char c;
        char startsWith[] = "href";
        int index = 0;
        List<char *> myList;
        // cout<<"aagya\n";
        while (in.get(c))
        {
            if (startsWith[index] != '\0' && startsWith[index] == c)
            {
                index++;
            }
            else if (startsWith[index] == '\0')
            {
                while (in.get(c))
                {
                    if (c == '"' || c == '\'')
                        break;
                }

                char *url = new char[150];
                int urlIndex = 0;
                while (in.get(c))
                {
                    if (c == '"' || c == '\'')
                        break;
                    url[urlIndex++] = c;
                }
                url[urlIndex] = '\0';

                if (isHtml(url))
                {
                    cout<<url<<"\n\n\n";
                    myList.addAtLast(url);
                }
                index = 0;
            }
            else
            {
                index = 0;
            }
        }
        return myList;
    }

    void DFS(char *currentURL, int depthLeft)
    {
        if (depthLeft == 0)
            return;
        if (allURL->search(currentURL))
            return;
        allURL->addInTable(currentURL, (char *)"1");
        allURL->display();

        downloadPage(currentURL, target_Dir, crawling_Depth - depthLeft + 1);
        cout<<"After download\n";
        List<char *> links = extractLinks(downloaded_Path);
        // links.display();
        cout<<"Before DFS\n";
        Node<char *> *temp = links.head;
        while (temp)
        {
            DFS(temp->val, depthLeft - 1);
            temp = temp->next;
        }
    }

    void startCrawling()
    {
        DFS(seed_url, crawling_Depth);
    }

    ~Crawl()
    {
        delete seed_url;
        delete target_Dir;
    }
};
