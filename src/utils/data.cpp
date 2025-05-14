#ifndef DATA_CPP
#define DATA_CPP

#include <fstream>
#include <iostream>
#include <cstdio>

#include "data.hpp"

Data::Data() { }
Data::Data(Data const& dt){
    path = dt.path;
    lastMod = dt.lastMod;
    
    data = dt.data;
} 

Data::Data(Data &&dt){
    path = dt.path;
    lastMod = dt.lastMod;

    data = std::move(dt.data);
}

int Data::LoadData(const std::string &path){
    this->path = path;
    if(int status = UpdateLastModifiedTime()) return status;
    if (int status = StoreData()) return status;

    return 0;
}

int Data::UpdateLastModifiedTime(){
    // Get last modified time
    struct stat st;
    if (stat(path.c_str(), &st)){
        perror("UpdateLastModifiedTime: stat");
        return -1;
    }
    lastMod = st.st_mtim;
    return 0;
}

int Data::IsFileModified(){
    struct stat st;
    if (stat(path.c_str(), &st)){
        perror("IsFileModified: stat");
        return -1;
    }
    if (lastMod.tv_sec == st.st_mtim.tv_sec && lastMod.tv_nsec == st.st_mtim.tv_nsec) return 0;
    return 1;
}

int Data::StoreData(){
    // Get data
    std::ifstream file;
    file.open(path);
    if (!file.is_open()) {
        fprintf(stderr, "StoreData: file not found\n");
        return -2;
    }
    std::string line;
    while (std::getline(file, line)){
        if (line == "\n" || line[0] == 0)
            continue;
        float value;
        std::string numStr = line;
        for (int i = 0; i < (int)line.size(); ++i){
            if(line[i] == ':'){
                numStr = line.substr(i+1);
                break;
            }
        }
        try{
            value = std::stof(numStr);
        }
        catch (std::invalid_argument const &e){
            fprintf(stderr, "StoreData: stof: Invalid Argument\n");
            return -3;
        }
        data.push_back(value);
    }
    return 0;
}

int Data::UpdateData(){
    if (!IsFileModified()) return -1;
    Data newData;
    if (newData.LoadData(path)) return -2;
    data = newData.data;
    lastMod = newData.lastMod;
    return 0;
}


#endif
