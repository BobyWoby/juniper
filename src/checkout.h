#pragma once

#include <string>
#include "common.h"

void loadHashedObject(std::string hash, std::string filepath);

std::string unblob(std::string hash);

void loadFile(std::string filepath, CompressedFile fileObj);

void loadCommit(std::string hash);

std::string decompressString(const CompressedFile file); 
