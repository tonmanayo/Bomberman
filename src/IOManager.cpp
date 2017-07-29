//
// Created by Tony MACK on 2017/07/29.
//

#include <fstream>
#include "../inc/IOManager.hpp"
#include "../inc/ErrorHandle.hpp"

bool IOManager::readFileToBuffer(std::vector<unsigned char> &buff, std::string filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (file.fail()){
        perror(filePath.c_str());
        return false;
    }

    //get file size
    file.seekg(0, std::ios::end);
    long fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    //skip header
    fileSize -= file.tellg();

    buff.resize(static_cast<unsigned long>(fileSize));
    file.read((char *)&(buff[0]), fileSize);

    file.close();
    return true;
}
