#include "commit.h"

#include <fstream>

#include "common.h"

std::string commitIndex() {
    std::ifstream index(".juniper/index");
    std::string line;
    std::vector<CommitEntry> indexEntries;

    while (std::getline(index, line)) {
        auto entryPair = split(line, " ");
        indexEntries.push_back({entryPair.at(0), entryPair.at(1), "blob"});
    }
    std::vector<CommitEntry> commitObjects;
    std::cout << "index Entries size: " << indexEntries.size() << "\n";
    for (int i = 0; i < indexEntries.size(); i++) {
        auto entry = indexEntries.at(i);
        if (entry.name.find("/") != std::string::npos) {
            // Parse the directory entry
            std::vector<CommitEntry> tmpVec;
            std::string dirname = entry.name.substr(0, entry.name.find("/"));
            std::string filepath = indexEntries.at(i).name;

            std::cout << "Dir Name: " << dirname << "\n";
            while (filepath.substr(0, filepath.find("/")) == dirname) {
                std::string filename = indexEntries.at(i).name;
                std::string newStr =
                    filename.substr(filename.find("/") + 1, std::string::npos);

                tmpVec.push_back({.hash = indexEntries.at(i).hash,
                                  .name = newStr,
                                  .type = indexEntries.at(i).type});

                i++;
                if (i >= indexEntries.size()) {
                    break;
                }
                filepath = indexEntries.at(i).name;
            }
            i--;
            std::cout << "\n\nI: " << i << "\n";
            for (auto tmp : tmpVec) {
                std::cout << tmp.name << "\n";
            }
            std::string treeHash = createTree(tmpVec, dirname);
            commitObjects.push_back({treeHash, dirname, "tree"});
        } else {
            indexEntries.at(i).type = "blob";
            commitObjects.push_back(indexEntries.at(i));
        }
    }

    // Create the tree object file
    std::string hash = randomHash();

    {
        std::ofstream file(".juniper/objects/" + hash);
        for (auto entry : commitObjects) {
            file << entry.type << " " << entry.hash << " " << entry.name
                 << "\n";
        }
    }
    return hash;
}

std::string createTree(std::vector<CommitEntry> indexEntries,
                       std::string dirname) {
    std::string hash = randomHash();
    std::vector<CommitEntry> objects;
    for (int i = 0; i < indexEntries.size(); i++) {
        auto entry = indexEntries.at(i);
        if (entry.name.find("/") != std::string::npos) {
            // Parse the directory entry
            std::vector<CommitEntry> tmpVec;
            std::string dirname = entry.name.substr(0, entry.name.find("/"));
            std::string filepath = indexEntries.at(i).name;
            while (filepath.substr(0, filepath.find("/")) == dirname) {
                std::string filename = indexEntries.at(i).name;
                std::string newStr =
                    filename.substr(filename.find("/") + 1, std::string::npos);
                tmpVec.push_back({.hash = indexEntries.at(i).hash,
                                  .name = newStr,
                                  .type = indexEntries.at(i).type});
                i++;
                if (i >= indexEntries.size()) {
                    break;
                }
                filepath = indexEntries.at(i).name;
            }
            std::cout << "\nCreate Tree: " << i << "\n";
            for (auto tmp : tmpVec) {
                std::cout << tmp.name << "\n";
            }
            i--;
            std::string treeHash = createTree(tmpVec, dirname);
            objects.push_back({treeHash, dirname, "tree"});
        } else {
            indexEntries.at(i).type = "blob";
            objects.push_back(indexEntries.at(i));
        }
    }
    std::ofstream file(".juniper/objects/" + hash);
    for (auto obj : objects) {
        file << obj.type << " " << obj.hash << " " << obj.name << "\n";
    }
    return hash;
}

void createCommit(std::string treeHash, std::string parent, std::string msg) {
    std::string hash = randomHash();
    std::ofstream file(".juniper/objects/" + hash);
    // tree object
    file << "commit " << treeHash << "\n";
    if (parent != "") {
        // parent commit
        file << "parent " << parent << "\n";
    }
    // commit message
    file << msg;
    updateHead(hash);
}
