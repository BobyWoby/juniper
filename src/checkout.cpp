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
    return std::string(reinterpret_cast<char *>(decompressedData),
                       decompressedSize);
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
    // std::cout << file << "\n";

    // std::ofstream oFile(filepath);
    // oFile << file;
}

/**
 * @brief Decompresses a given hashed object and returns the decompressed value
 * as a string
 *
 * @param hash the object hash that you are trying to unblob
 * @return decompressed string
 */
std::string unblob(std::string hash) {
    std::ifstream iFile(".juniper/objects/" + hash, std::ios::binary);
    std::string line = "";

    // Read the uncompressed size
    std::getline(iFile, line);
    unsigned long uncompressedSize = std::stoi(line);

    std::vector<unsigned char> dataBuf(std::istreambuf_iterator<char>(iFile),
                                       {});
    // std::cout << dataBuf.data() << "\n";

    CompressedFile fileObj = {.uncompressedSize = uncompressedSize,
                              .compressedSize = dataBuf.size(),
                              .data = dataBuf.data()};

    std::string decompressedStr = decompressString(fileObj);
    // std::cout << decompressedStr << "\n";
    return decompressedStr;
}

/**
 * @brief Loads the compressed file blob object into filepath
 *
 * @param hash the blob that you want to load
 * @param filepath where to write the blob contents to
 */
void loadHashedObject(std::string hash, std::string filepath) {
    std::string contents = unblob(hash);
    std::ofstream file(filepath, std::ios::trunc);
    file << contents;
}

void loadTree(std::string hash, std::string dirpath) {
    std::ifstream treeFile(".juniper/objects/" + hash);
    std::string line;
    while(std::getline(treeFile, line)){
        std::vector<std::string> entryPieces = split(line, " ");
        if(entryPieces.at(0) == "blob"){
            // load the blob
            loadHashedObject(entryPieces.at(1), dirpath + "/" + entryPieces.at(2));
        }else if(entryPieces.at(0) == "tree"){
            loadTree(entryPieces.at(1), dirpath + "/" + entryPieces.at(2));
        }
    }
}

void loadCommit(std::string hash) {
    // std::filesystem::path commitPath(".juniper/objects/" + hash);
    std::ifstream commitFile(".juniper/objects/" + hash);
    std::string line;
    std::getline(commitFile, line);
    std::string treeHash = line.substr(7, std::string::npos);

    std::ifstream treeFile(".juniper/objects/" + treeHash);
    std::getline(treeFile, line);

    std::vector<std::string> entryPieces = split(line, " ");
    loadTree(entryPieces.at(1), entryPieces.at(2));
    // while (std::getline(treeFile, line)) {
    //     std::vector<std::string> entryPieces = split(line, " ");
    //     if(entryPieces.at(0) == "blob" ){
    //
    //     }
    // }
}
