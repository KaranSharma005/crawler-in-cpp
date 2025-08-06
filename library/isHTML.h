#pragma once
#include <iostream>
using namespace std;
#include "../../strings/hepler.h"

bool substring(char* str,char* substr) {
    for (int i = 0; str[i] != '\0'; i++) {
        int j = 0;
        while (substr[j] != '\0' && str[i + j] == substr[j]) {
            j++;
        }
        if (substr[j] == '\0') {
            return true;  
        }
    }

    return false; 
}

bool isHtml(char* url) {
    if (endsWith(url, (char*)".css") || endsWith(url, (char*)".js") ||
        endsWith(url, (char*)".png") || endsWith(url, (char*)".jpg") ||
        endsWith(url, (char*)".jpeg") || substring(url, (char*)".svg") || 
        endsWith(url, (char*)".ico") || endsWith(url, (char*)".pdf") ||
        substring(url, (char*)"fonts.") || substring(url, (char*)"/css") ||
        substring(url, (char*)".woff") || substring(url, (char*)".ttf")) {
        return false; 
    }

    return true; 
}
