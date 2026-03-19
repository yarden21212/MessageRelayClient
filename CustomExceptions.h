#pragma once

#include <iostream>

using std::exception;


class IllegalSizeException : public exception {
private:
    std::string message;
public:

    // Constructor accepting const char*
    IllegalSizeException();
    IllegalSizeException(const char* msg);

    // Override what() method, marked
    // noexcept for modern C++
    const char* what() const noexcept override;
};