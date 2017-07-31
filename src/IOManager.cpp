//
// Created by Tony MACK on 2017/07/29.
//

#include <fstream>
#include <iostream>
#include "../inc/IOManager.hpp"
#include "../inc/ErrorHandle.hpp"

bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char> &buff) {
    try {
        std::ifstream file(filePath, std::ios::binary);
        if (file.fail()) {
            perror(filePath.c_str());
            throw ErrorHandle(filePath + ": Failed to open");
        }

        //get file size
        file.seekg(0, std::ios::end);
        long fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        //skip header
        fileSize -= file.tellg();

        buff.resize(fileSize);
        file.read((char *)&(buff[0]), fileSize);

        file.close();
        return true;
    } catch (ErrorHandle errorHandle) {
        std::cout << errorHandle.what() << std::endl;
        return false;
    }
}
