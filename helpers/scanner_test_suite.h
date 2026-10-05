#ifndef SCANNER_TEST_SUITE
#define SCANNER_TEST_SUITE

#include <filesystem>
#include <sstream>

using std::cout;
using std::string;
using std::vector;
using std::ifstream;
using std::ostringstream;

void compare_tokens(string filename, vector<Token> tokens) {
    filename = std::filesystem::path(filename).replace_extension(".tokens").string();
    ifstream fin(filename, std::ios::binary);
    if (!fin) {
        cout << "failed to open " << filename << endl;
        return;
    }
    
    ostringstream scanner_output;
    ostringstream reference_output;

    reference_output << fin.rdbuf();
    fin.close();

    output_IR(tokens, scanner_output);


    //cout << "PRINTING SCANNER OUTPUT: \n" << scanner_output.str() << endl << endl;
    //cout << "PRINTING REFERENCE OUTPUT: \n" << reference_output.str() << endl << endl;

    //cout << scanner_output.str().size() << " vs " << reference_output.str().size() << "\n";

    if (scanner_output.str() == reference_output.str()) {cout << "PASS\n";} 
    else {cout << "FAIL\n";}
    return;
}

#endif