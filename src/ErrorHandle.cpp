//
// Created by Tony Mack on 2017/07/28.
//

#include "../inc/ErrorHandle.hpp"

namespace WTCEngine {

    ErrorHandle::ErrorHandle(const char *message) : _message(message) {}

    ErrorHandle::ErrorHandle(const std::string &message) : _message(message) {}

    ErrorHandle::~ErrorHandle() {}

    const char *ErrorHandle::what() const throw() {
        return _message.c_str();
    }
}