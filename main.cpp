#include <iostream>
#include "../strings/hepler.h"
#include "./library/crawl.h"
using namespace std;

int main(int argc, char* argv[]){
    if(argc < 4){
        cout<<"Missing necessary arguements\n";
        return 0;
    }
    int depth = convertToInt(argv[3]);
    if(depth == -1){
        cout<<"Invalid arguement for maximum crawling depth\n";
        return 0;
    }

    Crawl obj(argv[1], argv[2], depth);
    obj.downloadPage(argv[1], argv[2], depth);
    return 0;
}