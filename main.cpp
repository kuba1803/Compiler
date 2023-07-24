#include <iostream>
#include "Lexer/Lexer.h"

int main() {
    Lexer lexer;
    Source source;
    source.read("Lexer/tests/test1.cpp");

    return 0;
}
