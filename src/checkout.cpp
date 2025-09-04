#include "checkout.h"

#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>

#include "common.h"
#include "zlib.h"


std::string decompressString(const CompressedFile file) {
    uLongf decompressedSize = file.uncompressedSize;
    Bytef *decompressedData = new Bytef[decompressedSize];

    if (uncompress(decompressedData, &decompressedSize,
                   (const Bytef *)file.data, file.compressedSize) != Z_OK) {
        std::cerr << "Decompression failed!" << std::endl;
        delete[] decompressedData;
        return "";
    }

    // std::cout << decompressedData << "\n";
    return std::string(reinterpret_cast<char *>(decompressedData), decompressedSize);
}


void loadFile(std::string filepath, CompressedFile fileObj) {
    unsigned char *file =
        static_cast<unsigned char *>(malloc(fileObj.uncompressedSize));

    int status =
        uncompress(static_cast<Bytef *>(file), &fileObj.uncompressedSize,
                   static_cast<Bytef *>(fileObj.data), fileObj.compressedSize);

    switch (status) {
        case Z_OK:
            std::cout << "Decompression success!\n";
        case Z_MEM_ERROR:
            std::cout << "Memory Error!\n";
        case Z_BUF_ERROR:
            std::cout << "Not enough buffer space!\n";
    }
    std::cout << file << "\n";

    // std::ofstream oFile(filepath);
    // oFile << file;
}

std::string unblob(std::string hash) {
    std::ifstream iFile(".juniper/objects/" + hash, std::ios::binary);
    std::string line = "";

    // Read the uncompressed size
    std::getline(iFile, line);
    unsigned long uncompressedSize = std::stoi(line);

    std::vector<unsigned char> dataBuf(std::istreambuf_iterator<char>(iFile), {});
    // std::cout << dataBuf.data() << "\n";

    CompressedFile fileObj = {
        .uncompressedSize = uncompressedSize,
        .compressedSize = dataBuf.size(),
        .data = dataBuf.data()};

    std::string decompressedStr = decompressString(fileObj);
    std::cout << decompressedStr << "\n";
    return decompressedStr;
}

/**
 * @brief Loads the compressed file blob object
 *
 * @param hash
 * @param filepath
 */
void loadHashedObject(std::string hash, std::string filepath) {
    std::ifstream iFile(hash);
    std::string line = "";
    std::getline(iFile, line);
    unsigned long uncompressedSize = std::stoi(line);
    std::string data = "";
    while (std::getline(iFile, line)) {
        data += line;
    }

    unsigned char *dataPtr = new unsigned char[uncompressedSize]();
    std::strcpy((char *)(dataPtr), data.c_str());
    // std::strncpy(dataPtr, data.c_str(), uncompressedSize);

    CompressedFile fileObj = {
        .uncompressedSize = uncompressedSize,
        .compressedSize = data.size(),
        .data = reinterpret_cast<unsigned char *>(dataPtr)};

    loadFile(filepath, fileObj);
}

void loadTree(std::string hash, std::string filepath) {}

void loadCommit(std::string hash) {
    // std::filesystem::path commitPath(".juniper/objects/" + hash);
    std::ifstream commitFile(".juniper/objects/" + hash);
    std::string line;
    std::getline(commitFile, line);
    std::string treeHash = line.substr(7, std::string::npos);

    std::ifstream treeFile(".juniper/objects/" + treeHash);
    std::getline(treeFile, line);

    std::ifstream blobFile(".juniper/objects/" + line.substr(5, 40));

    while (std::getline(blobFile, line)) {
    }
}
