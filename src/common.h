#pragma once
#include <openssl/sha.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <zlib.h>

#define MAX_LENGTH 1024

template <typename T>
struct ReturnError {
    T value;
    std::optional<std::string> error;
};

inline std::vector<std::string> split(std::string str, std::string delim) {
    std::vector<std::string> output;
    while (str.find(delim) != std::string::npos) {
        output.push_back(str.substr(0, str.find(delim)));
        str.erase(0, str.find(delim) + delim.length());
    }
    output.push_back(str);
    return output;
}

inline std::string randomHash() {
    // srand(time(0));
    unsigned char outBuf[20];
    unsigned char inBuf[20];
    for (int i = 0; i < 20; i++) {
        inBuf[i] = (unsigned char)rand();
    }
    SHA1(inBuf, 20 * sizeof(unsigned char), outBuf);
    std::string str{};
    for (auto b : outBuf) {
        char buf[3];
        sprintf(buf, "%02x", b);
        str += buf;
    }
    // std::cout << str << "\n";
    return str;
}

struct CompressedFile{
    uLongf uncompressedSize, compressedSize;
    Bytef *data;
};

inline std::string getCurrentCommit(){
    std::ifstream head(".juniper/HEAD");
    std::string line;
    std::getline(head, line);
    std::string loc = line.substr(5, std::string::npos);
    std::cout << loc << "\n";
    std::ifstream ref(".juniper/" + loc);
    if(!ref.good()){
        //if there isn't currently a head commit, return ""
        return "";
    }
    std::getline(ref, line);
    return line.substr(4, std::string::npos);
}

inline void updateHead(std::string newHash){
    std::ifstream head(".juniper/HEAD");
    std::string line;
    std::getline(head, line);
    std::string loc = line.substr(5, std::string::npos);
    std::ofstream ref(".juniper/" + loc, std::ios::trunc);
    ref << "ref " << newHash;
}
