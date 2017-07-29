//
// Created by Tony MACK on 2017/07/29.
//

#ifndef BOMBERMAN_IOMANAGER_HPP
#define BOMBERMAN_IOMANAGER_HPP


#include <vector>
#include <string>

class IOManager {
public:
    static bool readFileToBuffer(std::vector<unsigned char> &buff, std::string filePath);
};


#endif //BOMBERMAN_IOMANAGER_HPP
