//
// Created by Tony Mack on 2017/07/28.
//

#ifndef BOMBERMAN_ERRORHANDLE_HPP
#define BOMBERMAN_ERRORHANDLE_HPP

#include <exception>
#include <string>

class ErrorHandle {

class ErrorHandle : public std::exception {
public:
    explicit ErrorHandle(const char* message);
    explicit ErrorHandle(const std::string& message);
    virtual ~ErrorHandle();

    const char* what() const throw() override ;
private:
    std::string _message;
};
};


#endif //BOMBERMAN_ERRORHANDLE_HPP
