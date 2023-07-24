//
// Created by Jakub on 22.06.2023.
//

#ifndef COMPILER_SOURCE_H
#define COMPILER_SOURCE_H

#include <fstream>
#include <sstream>
#include <unordered_map>
#include <iostream>

class Source {
private:
    std::wifstream wistream;
    std::wstring source;
public:

    bool read(std::string path) {
        std::locale::global(std::locale(""));
        wistream.clear();
        if (!translationPhase1(path)) {
            std::cout << "Fail to translation phase 1: read character" << path;
            return false;
        }
        if (!translationPhase2()) {
            std::cout << "Fail to translation phase 2: remove slice of line " << path;
            return false;
        }
        if (!translationPhase3()) {
            std::cout << "Fail to translation phase 3: remove comments and decomposition preprocessor " << path;
            return false;
        }
        std::wcout << source;
        return true;
    }

private:
    bool translationPhase1(std::string &path) {
        std::wstringstream buf;
        wchar_t c;
        wchar_t unicodeCharacter;
        wistream.open(path, std::ios_base::in);
        if (!wistream.is_open()) {
            std::cout << "File not founded: " << path;
            return false;
        }
        wistream.imbue(std::locale(""));
        while (!wistream.eof()) {
            c = wistream.get();
            std::wcout<<(int)c<<std::endl;
            if (c >= 11 && c <= 13)continue;
            if (c == '\t') {
                buf << " ";
                continue;
            }
            if (c == '<' && wistream.peek() == '%') {
                wistream.get();
                buf << "{";
                continue;
            }
            if (c == '%' && wistream.peek() == '>') {
                wistream.get();
                buf << "}";
                continue;
            }
            if (c == '<' && wistream.peek() == ':') {
                wistream.get();
                buf << "[";
                continue;
            }
            if (c == ':' && wistream.peek() == '>') {
                wistream.get();
                buf << "]";
                continue;
            }
            if (c == '%' && wistream.peek() == ':') {
                wistream.get();
                buf << "#";
                continue;
            }
            if (c == '\\' && wistream.peek() == 'u') {
                wistream.get();
                unicodeCharacter = 0;
                for (int i = 0; i < 4; ++i) {
                    unicodeCharacter *= 16;
                    c = wistream.get();
                    std::wcout<<c<<std::endl;
                    if (iswxdigit(c)) {
                        if(std::iswdigit(c)){
                            unicodeCharacter += c - '0';
                        }
                        else{
                            if(iswupper(c)){
                                unicodeCharacter += c - 'A' + 10;
                            }
                            else {
                                unicodeCharacter += c - 'a' + 10;
                            }
                        }
                    } else {
                        return false;
                    }
                }
                buf << unicodeCharacter;
                continue;
            }
            if (c == '\\' && wistream.peek() == 'U') {
                wistream.get();
                unicodeCharacter = 0;
                for (int i = 0; i < 8; ++i) {
                    unicodeCharacter *= 16;
                    c = wistream.get();
                    if (iswxdigit(c)) {
                        if(std::iswdigit(c)){
                            unicodeCharacter += c - '0';
                        }
                        else{
                            if(iswupper(c)){
                                unicodeCharacter += c - 'A' + 10;
                            }
                            else {
                                unicodeCharacter += c - 'a' + 10;
                            }
                        }
                    } else {
                        return false;
                    }
                }
                buf << unicodeCharacter;
                continue;
            }
            buf << c;
        }
        buf << '\n';
        wistream.close();
        source = buf.str();
        return true;
    }

    bool translationPhase2() {
        std::wstringstream buf;
        if (source.size() > 1 && source[source.size() - 2] == '\\') {
            std::cout << "File end with '\' character";
            return false;
        }
        for (int i = 0; i < source.size(); ++i) {
            if (source[i] == '\\' && source[i + 1] == '\n') continue;
            buf << source[i];
        }
        source = buf.str();
        return true;
    }

    bool translationPhase3() {
        std::wstringstream buf;
        for (int i = 0; i < source.size(); ++i) {
            if (source[i] == '/' && i + 1 < source.size() && source[i + 1] == '/') {
                while (source[i] != '\n')++i;
            }
            if (source[i] == '/' && i + 1 < source.size() && source[i + 1] == '*') {
                while (source[i] != '*' && source[i + 1] != '/') {
                    ++i;
                    if (i + 1 == source.size())return false;
                }
            }
            buf << source[i];
        }
        source = buf.str();
        return true;
    }


};

#endif //COMPILER_SOURCE_H
