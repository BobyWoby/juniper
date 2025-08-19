#pragma once

#include <string>
#include <utility>
#include <vector>

std::string commitIndex();

void createCommit(std::string treeHash, std::string parent, std::string msg);

std::string createTree(std::vector<std::pair<std::string, std::string>> indexEntries, std::string dirname);
