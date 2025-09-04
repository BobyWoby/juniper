#include "add.h"

#include <openssl/sha.h>
#include <zconf.h>
#include <zlib.h>

#include <climits>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

CompressedFile compressString(const std::string str) {
    CompressedFile out;
    uLongf uSize = str.length();
    uLongf size = compressBound(str.size());
    Bytef *data = new Bytef[size];

    if (compress(data, &size, (const Bytef *)str.c_str(), uSize) != Z_OK) {
        std::cerr << "Compression Failure!\n";
        exit(1);
    }

    return {.uncompressedSize = uSize, .compressedSize = size, .data = data};
}

CompressedFile compressFile(std::string filepath) {
    std::string fileStr, line;
    {
        std::ifstream file(filepath);

        while (std::getline(file, line)) {
            fileStr += line + "\n";
        }
    }
    return compressString(fileStr);
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
    std::ofstream blobFile(".juniper/objects/" + hash, std::ios::binary);
    std::filesystem::path fPath(filepath);
    if(!std::filesystem::is_regular_file(fPath)){
        std::cout << "Skipping non-regular file: " << filepath << "\n";
        return;
    }
    CompressedFile compressedFile = compressFile(filepath);

    // write the size of the uncompressed data to the file followed by a new
    // line
    blobFile << compressedFile.uncompressedSize << "\n";
    blobFile.write((char *)compressedFile.data, compressedFile.compressedSize);
    // blobFile << compressedFile.data;

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

    {
        std::ofstream file(".juniper/index", std::ios::trunc);
        for (auto entry : indexVec) {
            file << entry << "\n";
        }
    }
}

void parseDir(std::string dirpath) {
    std::filesystem::recursive_directory_iterator it(dirpath);
    for(auto entry : it){
        if(entry.is_regular_file()){
            addFile(entry.path().string());
        }
    }
}
