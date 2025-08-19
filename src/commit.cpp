#include "commit.h"
#include <fstream>
#include "common.h"


std::string commitIndex(){
    std::ifstream index(".juniper/index");
    std::string line;
    std::vector<std::pair<std::string, std::string>> indexEntries;

    while(std::getline(index, line)){
        auto entryPair = split(line, " ");
        indexEntries.push_back({entryPair.at(0), entryPair.at(1)});
    }
    std::vector<std::pair<std::string, std::string>> commitObjects;
    for(int i = 0; i < indexEntries.size(); i++){
        auto entry = indexEntries.at(i);
        if(entry.second.find("/") != std::string::npos){
            // Parse the directory entry
            std::vector<std::pair<std::string, std::string>> tmpVec;
            std::string dirname = entry.second.substr(entry.second.find("/"));
            std::string filepath = indexEntries.at(i).second;
            while(filepath.substr(filepath.find("/")) == dirname){
                tmpVec.push_back(indexEntries.at(i));
                filepath = indexEntries.at(i).second;
                i++;
            }
            i--;
            std::string treeHash = createTree(tmpVec, dirname);
            commitObjects.push_back({treeHash, dirname});
        }else{
            commitObjects.push_back(indexEntries.at(i));
        }
    }

    // Create the tree object file
    std::string hash = randomHash();

    {
        std::ofstream file(".juniper/objects/" + hash);
        for(auto entry : commitObjects){
            file << entry.first << " " << entry.second << "\n";
        }
    }

    return hash;
}

std::string createTree(std::vector<std::pair<std::string, std::string>> indexEntries, std::string dirname){
    std::string hash = randomHash();
    std::vector<std::pair<std::string, std::string>> objects;
    for(int i = 0; i < indexEntries.size(); i++){
        auto entry = indexEntries.at(i);
        if(entry.second.find("/") != std::string::npos){
            // Parse the directory entry
            std::vector<std::pair<std::string, std::string>> tmpVec;
            std::string dirname = entry.second.substr(entry.second.find("/"));
            std::string filepath = indexEntries.at(i).second;
            while(filepath.substr(filepath.find("/")) == dirname){
                tmpVec.push_back(indexEntries.at(i));
                filepath = indexEntries.at(i).second;
                i++;
            }
            i--;
            std::string treeHash = createTree(tmpVec, dirname);
            objects.push_back({treeHash, dirname});
        }else{
            objects.push_back(indexEntries.at(i));
        }
    }
    std::ofstream file(".juniper/objects/" + hash);
    for(auto obj : objects){
        file << obj.first << " " << obj.second << "\n";
    }
    return hash;
}

void createCommit(std::string treeHash, std::string parent, std::string msg){
}
