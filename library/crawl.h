#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <ctime>
#include <cstdio>
#include <cstring>
#include "../../strings/hepler.h"
#include "../../HashMapGeneric/library/genericHash.h"
#include "./path.h"
#include "./isHTML.h"
#include "../../Genericlists/library/operations.h"

class Crawl
{
public:
    char *seed_url;
    char *target_Dir;
    int crawling_Depth;
    char downloaded_Path[200];
    List<char*> visited;
    HashMap<char *, char *> *htmlPagePath = new HashMap<char *, char *>(30);

    Crawl(char *url, char *dir, int depth)
    {
        seed_url = new char[my_strlen(url) + 1];
        target_Dir = new char[my_strlen(dir) + 1];
        crawling_Depth = depth;
        my_strcpy(seed_url, url);
        my_strcpy(target_Dir, dir);
    }

    char *normalizeURL(const char *url)
    {
        int len = my_strlen((char *)url);
        char *result = new char[len + 1];
        int i = 0, j = 0;
        if (my_strncmp((char *)url, (char *)"https://", 8) == 0)
        {
            for (int k = 0; k < 8; k++)
            {
                result[j++] = url[i++];
            }
        }

        char lastChar = '\0';
        while (url[i] != '\0')
        {
            if (url[i] == '/' && lastChar == '/')
            {
                i++;
                continue;
            }
            result[j++] = url[i];
            lastChar = url[i];
            i++;
        }

        if (j > 1 && result[j - 1] == '/')
            j--;

        result[j] = '\0';
        return result;
    }

    void downloadPage(char *url, char *dir, int depth)
    {
        if (!check_Path(target_Dir))
        {
            create_Dir(target_Dir);
        }

        char downloadPath[200] = "../public/";
        time_t timeStamp = time(0);
        char timeCharArray[30];
        sprintf(timeCharArray, "%ld", timeStamp);

        my_strcat(downloadPath, dir);
        my_strcat(downloadPath, (char *)"/");
        my_strcat(downloadPath, timeCharArray);
        my_strcat(downloadPath, (char *)"_downloaded_page.html");

        htmlPagePath->addInTable(url, downloadPath);
        my_strcpy(downloaded_Path, downloadPath);

        char command[300] = "wget -O ";
        my_strcat(command, downloadPath);
        my_strcat(command, (char *)" ");
        my_strcat(command, url);

        int result = system(command);
        if (result == 0)
        {
            cout << "Downloaded: " << url << "\n";
        }
        else
        {
            cout << "Failed: " << url << "\n";
        }
    }

    void extractLinks(char *currentURL, int depthLeft);
    
    void DFS(char *currentURL, int depthLeft);

    void startCrawling()
    {
        DFS(seed_url, crawling_Depth);
    }

    ~Crawl()
    {
        delete[] seed_url;
        delete[] target_Dir;
        delete htmlPagePath;
    }
};

void Crawl::DFS(char *currentURL, int depthLeft)
{
    if (depthLeft <= 0)
        return;

    char *normalizedURL = normalizeURL(currentURL);
    bool alreadyVisited = false;
    Node<char*> *cur = visited.head;
    while (cur)
    {
        if (strcmp(cur->val, normalizedURL) == 0)
        {
            alreadyVisited = true;
            break;
        }
        cur = cur->next;
    }
    if (alreadyVisited)
    {
        delete[] normalizedURL;
        return;
    }

    visited.addAtStart(normalizedURL); 
    downloadPage(normalizedURL, target_Dir, crawling_Depth - depthLeft + 1);
    Node<Pair<char*, char*>>* result = htmlPagePath -> search(normalizedURL);
    cout<<result -> val.val;
    extractLinks(normalizedURL, depthLeft - 1);
}

void Crawl::extractLinks(char *currentURL, int depthLeft)
{
    if (depthLeft <= 0) return;

    Node<Pair<char *, char *>> *result = htmlPagePath->search(currentURL);
    if (!result) return;

    char *fileName = result->val.val;
    std::ifstream in(fileName);
    if (!in.is_open())
        return;

    char c;
    const char startsWith[] = "href";
    int index = 0;
    char *arr[100];
    int ind = 0;

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

            char *url = new char[200];
            int urlIndex = 0;
            while (in.get(c))
            {
                if (c == '"' || c == '\'')
                    break;
                url[urlIndex++] = c;
            }
            url[urlIndex] = '\0';

            if (isHtml(url) && isStartsWith(url, "https"))
            {
                if (ind < 100)
                    arr[ind++] = url;
                else
                    delete[] url;
            }
            else if (isHtml(url) && url[0] != '#')
            {
                char *fullUrl = new char[300];
                fullUrl[0] = '\0';

                char *slashPos = strrchr(currentURL, '/');
                if (slashPos)
                {
                    int baseLen = slashPos - currentURL + 1;
                    strncpy(fullUrl, currentURL, baseLen);
                    fullUrl[baseLen] = '\0';
                }
                else
                {
                    my_strcpy(fullUrl, currentURL);
                    my_strcat(fullUrl, "/");
                }

                my_strcat(fullUrl, url);
                delete[] url;
                if (ind < 100)
                    arr[ind++] = fullUrl;
                else
                    delete[] fullUrl;
            }
            else
            {
                delete[] url;
            }

            index = 0;
            continue;
        }
        else
        {
            index = 0;
        }
    }

    for (int i = 0; i < ind; i++)
    {
        DFS(arr[i], depthLeft);
        delete[] arr[i];
    }
}

// #pragma once
// #include <iostream>
// #include <fstream>
// #include <filesystem>
// #include <ctime>
// #include <cstdio>
// #include "../../strings/hepler.h"
// #include "../../HashMapGeneric/library/genericHash.h"
// #include "./path.h"
// #include "./isHTML.h"
// #include "../../Genericlists/library/operations.h"

// using namespace std;

// class Crawl
// {
//     char *seed_url;
//     char *target_Dir;
//     int crawling_Depth;
//     char downloaded_Path[200];
//     HashMap<char *, char *> *allURL = new HashMap<char *, char *>(30);

// public:
//     Crawl(char *url, char *dir, int depth)
//     {
//         seed_url = new char[my_strlen(url) + 1];
//         target_Dir = new char[my_strlen(dir) + 1];
//         crawling_Depth = depth;
//         my_strcpy(seed_url, url);
//         my_strcpy(target_Dir, dir);
//     }

//     char *normalizeURL(const char *url)
//     {
//         int len = my_strlen((char *)url);
//         char *result = new char[len + 1];
//         int i = 0, j = 0;
//         if (my_strncmp((char *)url, (char *)"https://", 8) == 0)
//         {
//             for (int k = 0; k < 8; k++)
//             {
//                 result[j++] = url[i++];
//             }
//         }

//         char lastChar = '\0';
//         while (url[i] != '\0')
//         {
//             if (url[i] == '/' && lastChar == '/')
//             {
//                 i++;
//                 continue;
//             }

//             result[j++] = url[i];
//             lastChar = url[i];
//             i++;
//         }

//         result[j] = '\0';
//         return result;
//     }

//     void downloadPage(char *url, char *dir, int depth)
//     {
//         if (!check_Path(target_Dir))
//         {
//             create_Dir(target_Dir);
//         }

//         char downloadPath[200] = "../public/";
//         clock_t timeStamp = time(0);
//         char timeCharArray[30];
//         sprintf(timeCharArray, "%ld", timeStamp);

//         my_strcat(downloadPath, dir);
//         my_strcat(downloadPath, (char *)"/");
//         my_strcat(downloadPath, timeCharArray);
//         my_strcat(downloadPath, (char *)"downloaded_page.html");
//         my_strcpy(downloaded_Path, downloadPath);

//         char command[300] = "wget -O ";
//         my_strcat(command, downloadPath);
//         my_strcat(command, (char *)" ");
//         my_strcat(command, url);

//         int result = system(command);
//         if (result == 0)
//         {
//             cout << "Successfull" << "\n";
//         }
//         else
//         {
//             cout << "Failed" << url << "\n";
//         }
//     }

//     bool isRootRelative(const char *url)
//     {
//         return url[0] == '/';
//     }

//     void handleRootRelative(char *currentURL,char *relativeUrl, char *fullUrl)
//     {
//         int index = my_strstr(currentURL, (char*)"://");
//         cout<<index;
//         if (index == -1)
//         {
//             fullUrl[0] = '\0';
//             return;
//         }
//         int start = index + 3;
//         int slashPos = -1;
//         for (int i = start; currentURL[i] != '\0'; i++)
//         {
//             if (currentURL[i] == '/')
//             {
//                 slashPos = i;
//                 break;
//             }
//         }

//         if (slashPos != -1)
//         {
//             strncpy(fullUrl, currentURL, slashPos);
//             fullUrl[slashPos] = '\0';
//         }
//         else
//         {
//             my_strcpy(fullUrl, currentURL);
//         }
//         my_strcat(fullUrl, relativeUrl);
//     }

//     void extractLinks(const char *fileName, char *currentURL, int depthLeft)
//     {

//         cout<< "extractLinks\n\n\n\n";
//         ifstream in(fileName);
//         char c;
//         char startsWith[] = "href";
//         int index = 0;

//         while (in.get(c))
//         {
//             if (startsWith[index] != '\0' && startsWith[index] == c)
//             {
//                 index++;
//             }
//             else if (startsWith[index] == '\0')
//             {
//                 while (in.get(c))
//                 {
//                     if (c == '"' || c == '\'')
//                         break;
//                 }

//                 char *url = new char[200];
//                 int urlIndex = 0;

//                 while (in.get(c))
//                 {
//                     if (c == '"' || c == '\'')
//                         break;
//                     url[urlIndex++] = c;
//                 }
//                 url[urlIndex] = '\0';

//                 if (isHtml(url) && isStartsWith(url, "https"))
//                 {
//                     DFS(url, depthLeft - 1);
//                 }
//                 // else if (isHtml(url) && url[0] != '#')
//                 // {
//                 //     char *fullUrl = new char[150];
//                 //     fullUrl[0] = '\0';
//                 //     my_strcat(fullUrl, currentURL);
//                 //     my_strcat(fullUrl, url);
//                 //     DFS(fullUrl, depthLeft - 1);
//                 // }
//                 else if (isHtml(url) && url[0] != '#')
//                 {
//                     char fullUrl[300];
//                     if (isRootRelative(url))
//                     {
//                         handleRootRelative(currentURL, url, fullUrl);
//                     }
//                     else
//                     {
//                         strcpy(fullUrl, url);
//                     }
//                     DFS(fullUrl, depthLeft - 1);
//                 }

//                 index = 0;
//                 continue;
//             }
//             else
//             {
//                 index = 0;
//             }
//         }
//     }

//     void DFS(char *currentURL, int depthLeft)
//     {
//         if (depthLeft == 0)
//             return;
//         char *normalizedURL = normalizeURL(currentURL);
//         if (allURL->search(normalizedURL))
//             return;
//         allURL->addInTable(normalizedURL, (char *)"1");
//         char* tempPath;
//         my_strcpy(tempPath, downloaded_Path);
//         downloadPage(normalizedURL, target_Dir, crawling_Depth - depthLeft + 1);
//         extractWordsString(downloaded_Path);
//         cout<<"lknvu\n\n\n";
//         extractLinks(tempPath, normalizedURL, depthLeft);
//     }

//     void startCrawling()
//     {
//         DFS(seed_url, crawling_Depth);
//     }

//     ~Crawl()
//     {
//         delete seed_url;
//         delete target_Dir;
//     }
// };