#include "log.h"
#include <fstream>
#include <string>
#include "commit.h"
#include "common.h"

void logHead(){
    std::ifstream headFile(".juniper/HEAD");
    std::string line;
    std::getline(headFile, line);
    std::string ref = line.substr(5, std::string::npos);

    std::ifstream refFile(".juniper/" + ref);
    std::getline(refFile, line);
    std::string commit = line.substr(4, std::string::npos);

    std::ifstream commitFile(commit);
    while(std::getline(commitFile, line)){
    }
}

void logCommitTree(Commit previous){
    std::ifstream file(previous.hash);
    std::string line;
    Commit commit;
    previous.parent = &commit;
    std::getline(file, line);
    if(line.substr(0, 6) == "commit"){
        commit.hash = line.substr(7, std::string::npos);
    }
    bool hasParent = false;

    std::getline(file, line);
    if(line.substr(0, 6) == "parent"){
        hasParent = true;
    }else{
        commit.msg += line;
    }

    while(std::getline(file, line)){
        commit.msg += line;
    }

    if(hasParent){
        logCommitTree(commit);
    }
}


/**
 * @brief 
 *
 * @param previous 
 * @return 
 */
Commit *traverseParents(Commit &previous){
    std::ifstream file(previous.hash);
    std::string line;
    Commit *commit;
    previous.parent = commit;
    std::getline(file, line);
    if(line.substr(0, 6) == "commit"){
        commit->hash = line.substr(7, std::string::npos);
    }
    bool hasParent = false;

    std::getline(file, line);
    if(line.substr(0, 6) == "parent"){
        commit->parent = traverseParents(*commit);
    }else{
        commit->msg += line;
    }

    while(std::getline(file, line)){
        commit->msg += line;
    }
    return commit;

}
