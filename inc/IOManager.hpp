//
// Created by Tony MACK on 2017/07/29.
//

#ifndef BOMBERMAN_IOMANAGER_HPP
#define BOMBERMAN_IOMANAGER_HPP


#include <vector>
#include <string>
namespace WTCEngine {


    class IOManager {
    public:
        static bool readFileToBuffer(std::string filePath, std::vector<unsigned char> &buff);
    };

}
#endif //BOMBERMAN_IOMANAGER_HPP
