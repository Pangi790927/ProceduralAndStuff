#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <sstream>
#include <string>
#include <initializer_list>

#include <vector>

#include <GL/gl.h>
#include <GL/glu.h>

#ifndef DEBUG
    #ifdef DEBUG_PRINT
        #include <iostream>
        #define DEBUG(x) std::cout << __FUNCTION__ << ": " << x << std::endl;
    #else
        #define DEBUG(x) ;
    #endif
#endif

namespace Util {
    //Text not by const reference
    //so that the function can be used with a character array as argument
    template <typename Type>
    Type stringToNumber ( const std::string &Text )
    {
        std::stringstream ss(Text);
        Type result;
        return ss >> result ? result : 0;
    }

    template <typename Type>
    std::string numberToString ( Type Number )
    {
        std::stringstream ss;
        ss << Number;
        return ss.str();
    }

    template <typename Type, typename... Args>
    bool eq (Type a, Type b, Args... args) {
        return a == b && eq(args...);
    }

    template <typename Type>
    bool eq (Type a, Type b, Type c) {
        return a == b && b == c;
    }

    template <typename Type>
    bool eq (Type a, Type b) {
        return a == b;
    }

    template <typename Type>
    bool isEqualToAny (Type val, const std::initializer_list<Type>& list) {
        for (const auto& i : list) {
            if (val == i) {
                return true;
            }
        }
        return false;
    }

    float PI = 3.141592653589;

    float toRadians (float angle) {
        return angle * PI / 180.0f;
    }

    std::string getOpenGLError() {
        GLenum errCode;
        const unsigned char *errString = NULL;
        
        if ((errCode = glGetError()) != GL_NO_ERROR) {
            errString = gluErrorString(errCode);
            return "OpenGL Error: " + std::string((const char*)errString) + " : " + Util::numberToString(errCode);
        }
        else {
            return "NO_ERROR";
        }
    }

    template <typename Out>
    void split(const std::string &s, char delim, Out result) {
        std::stringstream ss;
        ss.str(s);
        std::string item;
        
        while (getline(ss, item, delim)) {
            *(result++) = item;
        }

        if (ss.str() != "") {
            *(result++) = ss.str();   
        }
    }


    std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        split(s, delim, std::back_inserter(elems));
        return elems;
    }

    template< typename T >
    struct array_deleter
    {
      void operator ()( T const * p)
      { 
        delete[] p; 
      }
    };
}

#endif // UTIL_H_INCLUDED