#pragma once

#include <string>
#include <utility>
#include <vector>

struct CommitEntry{
    std::string hash, name, type;
};

std::string commitIndex();

void createCommit(std::string treeHash, std::string parent, std::string msg);

std::string createTree(std::vector<CommitEntry> indexEntries, std::string dirname);
