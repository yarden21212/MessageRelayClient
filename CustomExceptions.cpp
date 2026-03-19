#include "CustomExceptions.h"

/*
    Good explanation about how to create custom exceptions: https://www.geeksforgeeks.org/cpp/how-to-throw-custom-exception-in-cpp/
*/

// Constructor accepting const char*
IllegalSizeException::IllegalSizeException(){
}
IllegalSizeException::IllegalSizeException(const char* msg) :
    message(msg) {
}

// Override what() method, marked
// noexcept for modern C++
const char* IllegalSizeException::what() const noexcept{
    return message.c_str();
}





//int main() {
//    try {
//
//        // Throw custom exception with
//        // const char*
//        throw IllegalSizeException("This is a custom exception");
//    }
//
//    // Catch by const reference (good practice)
//    catch (const IllegalSizeException& e) {
//        printf("Caught an exception: %s\n",
//            e.what());
//    }
//
//
//    std::system("pause");
//    return 0;
//}
