#include "add.h"

#include <openssl/sha.h>
#include <zconf.h>
#include <zlib.h>

#include <climits>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

CompressedFile compressFile(std::string filepath) {
    std::string fileStr, line;
    {
        std::ifstream file(filepath);

        while (std::getline(file, line)) {
            fileStr += line + "\n";
        }
    }
    unsigned long sizeDataCompressed = strlen(fileStr.c_str()) * 1.1 + 12;
    // unsigned char *dataCompressed = new unsigned char[sizeDataCompressed]();
    unsigned char *dataCompressed = (unsigned char *)malloc(sizeDataCompressed);

    // compress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong
    // sourceLen)
    int compressResult = compress(dataCompressed, &sizeDataCompressed,
                                  (Bytef *)fileStr.c_str(), fileStr.length());
    // 
    switch (compressResult) {
        case Z_OK:
            std::cout << "Compression Success\n";
            // printf("***** SUCCESS! *****\n");
            break;
        case Z_MEM_ERROR:
            std::cout << "Out of memory!\n";
            exit(1);  // quit.
            break;

        case Z_BUF_ERROR:
            printf("Not enough buffer space!\n");
            exit(1);  // quit.
            break;
    }
    return {.uncompressedSize = strlen(fileStr.c_str()),
            .compressedSize = sizeDataCompressed,
            .data = dataCompressed};
}

void addFile(std::string filepath) {
    std::string hash = randomHash(), line{}, fileStr;
    {
        std::ifstream file(filepath);

        while (std::getline(file, line)) {
            fileStr += line;
        }
    }
    // strlen(fileStr.c_sr());
    // std::cout << dataCompressed << "\n";
    std::ofstream blobFile(".juniper/objects/" + hash);
    CompressedFile compressedFile = compressFile(filepath);
    // write the size of the uncompressed data to the file followed by a new
    // line
    blobFile << compressedFile.uncompressedSize << "\n";
    blobFile << compressedFile.data;

    std::vector<std::string> indexVec;
    line = "";
    std::ifstream stagingFile(".juniper/index");
    bool brk = false;
    while (std::getline(stagingFile, line)) {
        indexVec.push_back(line);
    }
    int count = 0;
    for (int i = 0; i < indexVec.size(); i++) {
        std::vector<std::string> indexTmp = split(indexVec.at(i), " ");
        if (indexTmp.at(1) > filepath) {
            indexVec.insert(indexVec.begin() + i, hash + " " + filepath);
            break;
        }else{
            count++;
        }
    }
    if(count >= indexVec.size()){
        indexVec.push_back(hash + " " + filepath);
    }
    if (indexVec.size() == 0) {
        indexVec.push_back(hash + " " + filepath);
    }

    for(auto index : indexVec){
        std::cout << index << "\n";
    }
    {
        std::ofstream file(".juniper/index", std::ios::trunc);
        for (auto entry : indexVec) {
            file << entry << "\n";
        }
    }
}

void parseDir(std::string dirpath) {}
