#include <openssl/sha.h>
#include <zlib.h>

#include <cstring>
#include <iostream>
#include <string>

#include "add.h"
#include "init.h"
#include "commit.h"
#include "checkout.h"
#include "log.h"

int main(int argc, char* argv[]) {
    srand(time(0));
    // unsigned char * data; // byte pointer
    // std::cout << "Hello World!\n";
    // addFile("../src/main.cpp");
    if (argc < 1) return 1;
    // std::cout << argc << "\n";
    // std::cout << argv[1] << "\n";

    if(!strcmp(argv[1], "init")){
        std::cout << "initializing juniper\n";
        init();
    }
    if (!strcmp(argv[1], "add")){
        std::cout << "adding file\n";
        std::string filepath = argv[2];
        std::filesystem::path fsPath(filepath);
        std::error_code ec;
        if(std::filesystem::is_regular_file(fsPath, ec)){
            addFile(filepath);
        }else if(std::filesystem::is_directory(fsPath, ec)){
            parseDir(filepath);
        }
    }
    if(!strcmp(argv[1], "commit")){
        std::cout << "commiting staged files\n";
        std::string msg = "";
        if(argc >= 2){
            msg = argv[2];
        }
        std::string treeHash = commitIndex();
        std::string currentCommit = getCurrentCommit();
        createCommit(treeHash, currentCommit, msg);
    }
    if(!strcmp(argv[1], "checkout")){
        if(argc <= 2){
            std::cout << "Usage: juniper checkout commit-hash\n";
            exit(1);
        }
        std::string hash = argv[2];
        loadCommit(hash);
    }

    if(!strcmp(argv[1], "unblob")){
        if(argc <= 3){
            std::cout << "Usage: juniper unblob commit-hash filename\n";
            exit(1);
        }
        std::string hash = argv[2];
        unblob(hash);
    }
    if(!strcmp(argv[1], "log")){
        logHead();
    }
    return 0;
}
