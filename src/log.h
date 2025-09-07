#pragma once

#include <string>
#include "common.h"

void logHead();

void logCommitTree(std::string commitHash);

Commit *traverseParents(Commit &previous);

