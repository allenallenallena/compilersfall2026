#ifndef UTILS
#define UTILS

using std::cout;
using std::endl;

void output_IR(std::vector<Token> tokens) {
    cout << '[';
    int number_of_tokens = tokens.size();
    for (int i = 0; i < number_of_tokens - 1; i++) { //all but the last tokens
        if (tokens.at(i).null_lexeme) {
        cout << "[null, "
             << '\"' << tokens.at(i).TOKEN_CLASS << "\"], ";
        } else {
        cout << "[\"" << tokens.at(i).lexeme << "\", "
             << '\"' << tokens.at(i).TOKEN_CLASS << "\"], ";
        }
    }

    cout << "[\"" << tokens.at(number_of_tokens - 1).lexeme << "\", "
         << '\"' << tokens.at(number_of_tokens - 1).TOKEN_CLASS << "\"]";

    cout << ']';
}

void debug_output_IR(std::vector<Token> tokens) {
    int number_of_tokens = tokens.size();
    for (int i = 0; i < number_of_tokens; i++) {
        cout << "token " << i << ": <" << tokens.at(i).lexeme << '>' << endl;
    }


}

#endif