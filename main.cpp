#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>

using std::cout;
using std::endl;
using std::ifstream;

const int BUFFER_SIZE = 2049; //2KiB + null terminator

int fill_buffer(ifstream&, char*);

int main(int argc, char* argv[]) {

    char buff1[BUFFER_SIZE];
    char buff2[BUFFER_SIZE];

    for (int i = 1; i < argc; i++) { //loop over all of the argumets in argv, starting with the second one which will be our first argument
        ifstream fin(argv[i]);
        if (!fin) {
            cout << "failed to open " << argv[i] << endl;
            continue;
        }

        fill_buffer(fin, buff1);
        cout << buff1 << "PRINTED BUFFER WITH COUT!\n";
        
        int j = 0;
        while(buff1[j] != '\0') {
            cout << buff1[j];
            j++;
        }
        cout << "PRINTED BUFFER RAW!\n";
    }   
    

    return 0;
}

int fill_buffer(ifstream& stream, char* buffer) {
    int chars_read = BUFFER_SIZE - 1;
    stream.read(buffer, BUFFER_SIZE - 1); //read 2KiB into buffer
    if (stream.eof()) { //if it ended early...
        chars_read = stream.gcount(); //find out real number of chars read
        stream.clear(); //just to be safe, I'm not convinced that it's necessary
    }   
    buffer[chars_read] = '\n'; 
    return chars_read;
}