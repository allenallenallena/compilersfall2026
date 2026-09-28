#ifndef UTILS
#define UTILS

void output_IR(std::vector<Token> tokens) {
    cout << '[';
    for (const Token each_token : tokens) {
        cout << "[\"" << each_token.lexeme << "\", "
              << '\"' << each_token.TOKEN_CLASS << "\"], "
    }
    cout << ']';
}

#endif