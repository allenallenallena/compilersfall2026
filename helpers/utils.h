#ifndef UTILS
#define UTILS

using std::cout;

void output_IR(std::vector<Token> tokens) {
    cout << '[';

    for (const Token& each_token : tokens) {
        if (each_token.null_lexeme) {
        cout << "[null, "
             << '\"' << each_token.TOKEN_CLASS << "\"], ";
        } else {
        cout << "[\"" << each_token.lexeme << "\", "
             << '\"' << each_token.TOKEN_CLASS << "\"], ";
        }
    }

    cout << ']';
}

#endif