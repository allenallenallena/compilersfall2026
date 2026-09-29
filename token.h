#ifndef TOKEN
#define TOKEN

struct Token {
    std::string lexeme = "";
    std::string TOKEN_CLASS = "";
    bool null_lexeme = false;
};

#endif