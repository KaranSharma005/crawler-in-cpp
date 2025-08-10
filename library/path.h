#pragma once
#include <filesystem>
#include <iostream>
#include <fstream>
#include "../../strings/hepler.h"
#include "../../HashMapGeneric/library/genericHash.h"
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

void create_Dir(char *name)
{
    char fullPath[30] = "../public/";
    my_strcat(fullPath, name);
    if (fs::create_directory(fullPath))
    {
        cout << "Directory created successfully";
    }
    else
    {
        cout << "Directory creation failed or already exists." << endl;
    }
}
void extractWordsString(const char *filePath)
{
    HashMap<char *, int> *wordsFrequency = new HashMap<char *, int>(30);
    fstream sWord(filePath);
    if (!sWord.is_open())
    {
        cout << "Failed to open file" << endl;
        return;
    }

    char c;
    bool inTag = false;
    bool inStyle = false;
    int TAG_BUFFER_SIZE = 10;
    char tagBuffer[TAG_BUFFER_SIZE];
    int tagIndex = 0;

    char word[100];
    int wordIndex = 0;

    while (sWord.get(c))
    {
        if (c == '<')
        {
            inTag = true;
            tagIndex = 0;
            tagBuffer[tagIndex++] = c;
        }
        else if (inTag)
        {
            if (tagIndex < TAG_BUFFER_SIZE - 1)
            {
                tagBuffer[tagIndex++] = c;
            }

            if (c == '>')
            {
                tagBuffer[tagIndex] = '\0';
                if (isOpeningTag(tagBuffer))
                {
                    inStyle = true;
                }
                else if (isClosingTag(tagBuffer))
                {
                    inStyle = false;
                }
                inTag = false;
            }
        }
        else if (!inStyle && !inTag)
        {
            int count = 1;
            if (c == ' ' || c == '\n')
            {
                word[wordIndex] = '\0';
                lowerCase(word);
                wordIndex = 0;
                Node<Pair<char *, int>> *result = wordsFrequency->search(word);
                if (result)
                    count = result->val.val + 1;
                char *normalizedWord = normalizeWord(word);
                if ((normalizedWord[0] != '\0' && normalizedWord[1] != '\0') && !isGrammarWord(normalizedWord))
                {
                    my_strcpy(word, normalizedWord);
                    wordsFrequency->addInTable(word, count);
                }
            }
            else
            {
                word[wordIndex] = c;
                wordIndex++;
            }
        }
    }
    wordsFrequency->display();
    char *frequentWord = wordsFrequency->getFrequent();
    cout << "frequent word is -> " << frequentWord << "\n\n";
    // delete []wordsFrequency;
}
