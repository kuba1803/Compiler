//
// Created by Jakub on 05.06.2023.
//

#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H

#include "Tag.h"
#include <string>

class Token {
public:
    const Tag tag;
    Token( Tag tag ):tag(tag){};
    std::string toString(){
        return "" + tag;
    };
};


#endif //COMPILER_TOKEN_H
