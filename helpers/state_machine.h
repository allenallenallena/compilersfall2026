#ifndef STATE_MACHINE
#define STATE_MACHINE

//I think I just have to start goin

#include <stack>

using std::stack;

bool empty_stack(stack state_stack) {
    while (!state_stack.empty()) {state_stack.pop()}
}

/* I have discovered that I might have been doing extra work for no reason...
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


char get_next_char(ifstream& stream) {
    short int next_char = stream.get(); //tiny optimization
    if (stream.good()) return next_char;
    else if (stream.bad()) 
}
*/

void get_tokens(ifstream& stream, vector<Token>& tokens) { //referenced vector is return target
    Token current_token;
    char current_char;
    stack<int> state_stack;

s0: empty_stack(state_stack); //start state
    state_stack.push(-1); //-1 is the sentinal for "bad"
    current_char = stream.get(); //EOF is implementation-specific but I'm not sure what to replace it with
    current_token.lexeme += current_char;
    state_stack.push(0);
    switch (current_char) {
        case '}':
            goto s1;
        case '{':
            goto s2;
        case '(':
            goto s3;
        case ')':
            goto s4;
        case '#':
            goto s5;
        case 
        default: 
            goto: se
    }

//trivial states:
s1: //'}' brace-close
s2: //'{' brace-open
s3: //')' paren-close
s4: //'(' paren-open

//non-trivial states:
s5: //comment "#.*\n" '#'
s6: //any '.' (not literal period '\.', any)
s7: //newline '\n' 

se: //error state

}

#endif