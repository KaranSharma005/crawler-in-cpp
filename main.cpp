#include <iostream>
#include "../strings/hepler.h"
#include "./library/crawl.h"
using namespace std;

int main(int argc, char* argv[]){
    if(argc < 4){
        cout<<"Missing necessary arguements\n";
        return 0;
    }

    Crawl obj(argv[1], argv[2], argv[3]);
    obj.downloadPage();
    return 0;
}