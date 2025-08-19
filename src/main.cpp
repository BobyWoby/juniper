#include <openssl/sha.h>
#include <zlib.h>

#include <cstring>
#include <iostream>

#include "add.h"
#include "init.h"

int main(int argc, char* argv[]) {
    // unsigned char * data; // byte pointer
    // std::cout << "Hello World!\n";
    // addFile("../src/main.cpp");
    if (argc < 1) return 1;
    std::cout << argc << "\n";
    std::cout << argv[1] << "\n";

    if(!strcmp(argv[1], "init")){
        std::cout << "initializing juniper\n";
        init();
    }
    if (!strcmp(argv[1], "add")){
        std::cout << "adding file\n";
        addFile(argv[2]);
    }
    return 0;
}
