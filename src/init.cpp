#include "init.h"

void init(){
    std::filesystem::create_directory("./.juniper");
    std::filesystem::create_directory("./.juniper/objects");
    std::filesystem::create_directories("./.juniper/refs/heads");

    std::ofstream file("./.juniper/HEAD", std::ios::out | std::ios::trunc);
    file << "ref: refs/heads/main";

}
