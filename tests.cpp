#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <stdexcept>
#include <vector>
#include <iomanip>


//#include "./helpers/state_machine.h"
#include "token.h"
#include "./helpers/utils.h"
#include "./helpers/direct_coded_implementation.h"
#include "./helpers/scanner_test_suite.h"

int _dowildcard = -1;

using std::cout;
using std::endl;
using std::ifstream;
using std::vector;
using std::stringstream;

int main(int argc, char* argv[]) {
    size_t width = 0;
    for (int i = 1; i < argc; i++)
        width = std::max(width, std::strlen(argv[i]));

    for (int i = 1; i < argc; i++) { //loop over all of the argumets in argv, starting with the second one which will be our first argument
        ifstream fin(argv[i]);
        if (!fin) {
            cout << "failed to open " << argv[i] << endl;
            continue;
        }

        vector<Token> tokens; 
        get_tokens(fin, tokens);
        fin.close();

        //output_IR(tokens);
        //cout << endl << "END MAIN" << endl << endl;

        //debug_output_IR(tokens);
        cout << std::left << std::setw(width + 4) << (string(argv[i]) + ":");
        compare_tokens(argv[i], tokens);
    }   

    return 0;
}
