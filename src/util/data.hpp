#ifndef DATA_HPP
#define DATA_HPP

#include <string>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

class Data{
    private:
        std::string path;
        struct timespec lastMod;

        int UpdateLastModifiedTime();
        int IsFileModified();
        int StoreData();

    public:
        std::vector<float> data;

        Data(){};
        int LoadData(const std::string &path);
        int UpdateData();
};


#endif
