#pragma once
#include <filesystem>
#include <string>
#include "common.h"



CompressedFile compressFile(std::string filepath);
void addFile(std::string filepath);
void parseDir(std::string dirpath);
