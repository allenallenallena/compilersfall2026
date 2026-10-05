#ifndef UTILS
#define UTILS

using std::cout;
using std::endl;

void output_IR(std::vector<Token> tokens, std::ostream& streamout = cout) {
    streamout << '[';
    int number_of_tokens = tokens.size();
    for (int i = 0; i < number_of_tokens - 1; i++) { //all but the last tokens
        if (tokens.at(i).null_lexeme) {
        streamout << "[null, "
             << '\"' << tokens.at(i).TOKEN_CLASS << "\"], ";
        } else {
        streamout << "[\"" << tokens.at(i).lexeme << "\", "
             << '\"' << tokens.at(i).TOKEN_CLASS << "\"], ";
        }
    }

    streamout << "[\"" << tokens.at(number_of_tokens - 1).lexeme << "\", "
         << '\"' << tokens.at(number_of_tokens - 1).TOKEN_CLASS << "\"]";

    streamout << "]\n";
}

void debug_output_IR(std::vector<Token> tokens) {
    int number_of_tokens = tokens.size();
    for (int i = 0; i < number_of_tokens; i++) {
        cout << "token " << i << ": <" << tokens.at(i).lexeme << '>' << endl;
    }


}

#endif