#ifndef DIRECT_CODED_IMPLEMENTATION
#define DIRECT_CODED_IMPLEMENTATION 

using std::vector;
using std::ifstream;
using std::string;

/* I think I may have goofed this one up a little again. I keep wasting so much time...

//I think right now we're not gonna touch the \r (carriage return)
//APPARENTLY commas are NOT used in this language at ALL
enum Char_classes { 
    ANY, //'.' i.e. "any" does NOT match ascii 10 or 13 \n or \r (newline or carriage return)
    SPACE, //ascii 9 \t (tab) & 32 (space)
    NEWLINE, //ascii 10 \n (newline character)
    ERR, //only ascii 13 \r (carriage return)
    EXCLAMATION_POINT, 
    QUOTATION_MARK,
    POUND,
    PERCENT, 
    AMPERSAND,
    OPEN_PAREN,
    CLOSE_PAREN,
    ASTERISK,
    PLUS,
    MINUS,
    PERIOD,
    BACKSLASH,
    NUMBER,
    OPEN_ANGLE,
    EQUALS,
    CLOSE_ANGLE,
    ALPHABETIC, //includes underscore
    FORWARDSLASH,
    OPEN_CURLY,
    PIPE,
    CLOSE_CURLY
}; //25 unique character classes
const int UNIQUE_CHARACTERS = 25; //I'll start using more descriptive types for better documentation
const short int UNIQUE_STATES = 50; //50 is a placeholder until I can figure out how many states I need
void set_up_char_class_array(int[] char_class) { //this is a monstrocity.
    for (int i = 0; i < 128; i++) {
        if (i <= 8 || 11 <= i <= 12 || 14 <= i <= 31 || i == 36 || i == 39 || i == 44 || 58 <= i <= 59 || 63 <= i <= 64 || i == 91 || 93 <= i <= 94 || i == 96 || 126 <= i <= 127 ) {char_class[i] = ANY;} 
        else if (i == 9 || i == 32) {char_class[i] = SPACE;} //tab and space
        else if (i == 10) {char_class[i] = NEWLINE;}
        else if (i == 13) {char_class[i] = ERR;} //carriage return since its unique and we don't use it
        else if (i == 33) {char_class[i] = EXCLAMATION_POINT;}
        else if (i == 34) {char_class[i] = QUOTATION_MARK;}
        else if (i == 35) {char_class[i] = POUND;}
        else if (i == 37) {char_class[i] = PERCENT;}
        else if (i == 38) {char_class[i] = AMPERSAND;}
        else if (i == 40) {char_class[i] = OPEN_PAREN;}
        else if (i == 41) {char_class[i] = CLOSE_PAREN;}
        else if (i == 42) {char_class[i] = ASTERISK;}
        else if (i == 43) {char_class[i] = PLUS;}
        else if (i == 45) {char_class[i] = MINUS;}
        else if (i == 46) {char_class[i] = PERIOD;}
        else if (i == 47) {char_class[i] = BACKSLASH;}
        else if (48 <= i <= 57) {char_class[i] = NUMBER;}
        else if (i == 60) {char_class[i] = OPEN_ANGLE;}
        else if (i == 61) {char_class[i] = EQUALS;}
        else if (i == 62) {char_class[i] = CLOSE_ANGLE;}
        else if (65 <= i <= 90 || i == 95 || 97 <= i <= 122) {char_class[i] = ALPHABETIC;}
        else if (i == 123) {char_class[i] = OPEN_CURLY;}
        else if (i == 124) {char_class[i] = PIPE;}
        else if (i == 125) {char_class[i] = CLOSE_CURLY;}
    }
    return;
}

*/

//I don't think I need ANY since it's used like once and I can circumvent it by doing NOT certain characters...
enum Char_classes { 
    ANY, //'.' i.e. "any" does NOT match ascii 10 or 13 \n or \r (newline or carriage return) //ALSO Not truly any, it also excludes every other character in ENUM
    SPACE, //ascii 9 \t (tab) & 32 (space)
    NEWLINE, //ascii 10 \n (newline character)
    ERR, //only ascii 13 \r (carriage return)
    OPERATOR, 
    EXCLAMATION_POINT,
    OPEN_ANGLE,
    EQUALS,
    CLOSE_ANGLE,
    AMPERSAND,
    PIPE,
    QUOTATION_MARK,
    POUND,
    OPEN_PAREN,
    CLOSE_PAREN,
    PERIOD,
    NUMBER,
    ALPHABETIC, //includes underscore
    FORWARDSLASH,
    OPEN_CURLY,
    CLOSE_CURLY
}; //21 unique character classes

const int UNIQUE_CHARACTERS = 21; //I'll start using more descriptive types for better documentation
const int UNIQUE_STATES = 26; //s0-s24 (25 states) + error state 
const int ERROR_STATE = UNIQUE_STATES - 1;
const vector<string> keywords = {"bool", "else", "false", "float", "if", "int", "str", "true", "while"};

void set_up_char_class_array(int char_class[]) { //this is a monstrocity.
    for (int i = 0; i < 128; i++) {
        //if (i <= 8 || 11 <= i <= 12 || 14 <= i <= 31 || i == 36 || i == 39 || i == 44 || 58 <= i <= 59 || 63 <= i <= 64 || i == 91 || 93 <= i <= 94 || i == 96 || 126 <= i <= 127 ) {char_class[i] = ANY;} //I'm gonna wrap this up in the else block for execution-sake
        if (i == 9 || i == 32) {char_class[i] = SPACE;} //tab and space
        else if (i == 10) {char_class[i] = NEWLINE;}
        else if (i == 13) {char_class[i] = ERR;} //carriage return since its unique and we don't use it
        else if (i == 37 || i == 42 || i == 43 || i == 45 || i == 47) {char_class[i] = OPERATOR;} //(+ / - * %)

        else if (i == 33) {char_class[i] = EXCLAMATION_POINT;}
        else if (i == 60) {char_class[i] = OPEN_ANGLE;}
        else if (i == 61) {char_class[i] = EQUALS;}
        else if (i == 62) {char_class[i] = CLOSE_ANGLE;}
        else if (i == 38) {char_class[i] = AMPERSAND;}
        else if (i == 124) {char_class[i] = PIPE;}

        else if (i == 34) {char_class[i] = QUOTATION_MARK;}
        else if (i == 35) {char_class[i] = POUND;}
        else if (i == 40) {char_class[i] = OPEN_PAREN;}
        else if (i == 41) {char_class[i] = CLOSE_PAREN;}
        else if (i == 46) {char_class[i] = PERIOD;}
        else if (48 <= i && i <= 57) {char_class[i] = NUMBER;} 
        else if ((65 <= i && i <= 90) || i == 95 || (97 <= i && i <= 122)) {char_class[i] = ALPHABETIC;} 
        else if (i == 92) {char_class[i] = FORWARDSLASH;} 
        else if (i == 123) {char_class[i] = OPEN_CURLY;}
        else if (i == 125) {char_class[i] = CLOSE_CURLY;}
        else {char_class[i] = ANY;}
    }
    return;
}

const vector<int> accepting_states = {1, 2, 3, 4, 6, 7, 9, 10, 12, 15, 16, 17, 18, 19, 22, 23, 24};
//accpeting states are: 1, 2, 3, 4, 6, 7, 9, 10, 12, 15, 16, 17, 18, 19, 22, 23, 24
//or.. NOT 5, 8, 11, 13, 14, 20, 21, 25
/* LIST OF STATES!
 s0: starting state

 s1: open curly (accepting) NO TRANSITIONS
 s2: close curly (accepting) NO TRANSITIONS
 s3: open paren (accepting) NO TRANSITIONS
 s4: close paren (accepting) NO TRANSITIONS

 s5: pound begin comment/comment contents [q7 in webapp helper]
 s6: end comment (accepting) NO TRANSITIONS

 s7: identifier (accepting) [q3 in webapp helper]

 s8: line continuation begin/whitespace after [q9 in helper]
 s9: line continuation end (accepting) NO TRANSITIONS

 s10: literal-int (accepting) [q12]
 s11: mid-float period "*.   "
 s12: float end (accepting)

 s13: string-literal beign [q15 in webapp helper]
 s14: mid-string <^"\n> 
 s15: string-literal end (accepting) NO TRANSITIONS

 s16: < (accepting) [q18]

 s17: > (accepting)

 s18: = (accepting)

 s19: ! (accepting)

 s20: AND part 1

 s21: OR part 1

 s22: operator final destination EXCEPT <, >, =, ! [AND part 2, OR part 2, <=, >=, !=, ==, + - * / % (accepting) NO TRANSITIONS
 
 s23: newline (accepting) NO TRANSITIONS
 s24: tabs and spaces (accepting) 

 se: s25 in spirit
 */

void set_up_delta_array(int delta[][UNIQUE_CHARACTERS]) {
    for (int i = 0; i < UNIQUE_STATES; i++) { //Outer loop for states
        for (int j = 0; j < UNIQUE_CHARACTERS; j++) { //Inner loop for each character
            switch (i) {
                case 0: { //state 0, starting state
                    if (j == OPEN_CURLY) {delta[i][j] = 1;}
                    else if (j == CLOSE_CURLY) {delta[i][j] = 2;}
                    else if (j == OPEN_PAREN) {delta[i][j] = 3;}
                    else if (j == CLOSE_PAREN) {delta[i][j] = 4;}
                    else if (j == POUND) {delta[i][j] = 5;}
                    else if (j == ALPHABETIC) {delta[i][j] = 7;}
                    else if (j == FORWARDSLASH) {delta[i][j] = 8;}
                    else if (j == NUMBER) {delta[i][j] = 10;}
                    else if (j == QUOTATION_MARK) {delta[i][j] = 13;}
                    else if (j == OPEN_ANGLE) {delta[i][j] = 16;}
                    else if (j == CLOSE_ANGLE) {delta[i][j] = 17;}
                    else if (j == EQUALS) {delta[i][j] = 18;}
                    else if (j == EXCLAMATION_POINT) {delta[i][j] = 19;}
                    else if (j == AMPERSAND) {delta[i][j] = 20;}
                    else if (j == PIPE) {delta[i][j] = 21;}
                    else if (j == OPERATOR) {delta[i][j] = 22;}
                    else if (j == NEWLINE) {delta[i][j] = 23;}
                    else if (j == SPACE) {delta[i][j] = 24;}
                    else {delta[i][j] = ERROR_STATE;} // error state
                    break;
                }
                case 5: {
                    if (j != NEWLINE && j != ERR) {delta[i][j] = 5;}
                    else if (j == NEWLINE) {delta[i][j] = 6;}
                    else {delta[i][j] = ERROR_STATE;} // error state
                    break;
                }
                case 7: {
                    if (j == ALPHABETIC || j == NUMBER) {delta[i][j] = 7;}
                    else {delta[i][j] = ERROR_STATE;} // error state
                    break;
                }
                case 8: {
                    if (j == SPACE) {delta[i][j] = 8;}
                    else if (j == NEWLINE) {delta[i][j] = 9;}
                    else {delta[i][j] = ERROR_STATE;} // error state
                    break;
                }
                case 10: {
                    if (j == NUMBER) {delta[i][j] = 10;}
                    else if (j == PERIOD) {delta[i][j] = 11;}
                    else {delta[i][j] = ERROR_STATE;} // error state
                    break;
                }
                case 11: {
                    if (j == NUMBER) {delta[i][j] = 12;}
                    else {delta[i][j] = ERROR_STATE;} // error state
                    break;
                }
                case 12: {
                    if (j == NUMBER) {delta[i][j] = 12;}
                    else {delta[i][j] = ERROR_STATE;} // error state
                    break;
                }
                case 13: {
                    if (j != QUOTATION_MARK && j != NEWLINE) {delta[i][j] = 14;}
                    else {delta[i][j] = ERROR_STATE;} // error state
                    break;
                }
                case 14: {
                    if (j != QUOTATION_MARK && j != NEWLINE) {delta[i][j] = 14;}
                    else if (j == QUOTATION_MARK) {delta[i][j] = 15;}
                    else {delta[i][j] = ERROR_STATE;} // error state
                    break;
                }
                case 16: {
                    if (j == EQUALS) {delta[i][j] = 22;}
                    else {delta[i][j] = ERROR_STATE;} // error state
                    break;
                }
                case 17: {
                    if (j == EQUALS) {delta[i][j] = 22;}
                    else {delta[i][j] = ERROR_STATE;} // error state
                    break;
                }
                case 18: {
                    if (j == EQUALS) {delta[i][j] = 22;}
                    else {delta[i][j] = ERROR_STATE;} // error state
                    break;
                }
                case 19: {
                    if (j == EQUALS) {delta[i][j] = 22;}
                    else {delta[i][j] = ERROR_STATE;} // error state
                    break;
                }
                case 20: {
                    if (j == AMPERSAND) {delta[i][j] = 22;}
                    else {delta[i][j] = ERROR_STATE;} // error state
                    break;
                }
                case 21: {
                    if (j == PIPE) {delta[i][j] = 22;}
                    else {delta[i][j] = ERROR_STATE;} // error state
                    break;
                }
                case 24: {
                    if (j == SPACE) {delta[i][j] = 24;}
                    else {delta[i][j] = ERROR_STATE;} // error state
                    break;
                }
                default: { //catch-all error state
                    delta[i][j] = ERROR_STATE;
                    break;
                }
            }
        }
    }

    return;
}

bool is_accepting_state(int state) {
    bool is_accepting = false;
    for (const int& an_accepting_state : accepting_states) {
        if (state == an_accepting_state) {is_accepting = true;} 
    }
    return is_accepting;
}

bool is_keyword(string lexeme) { //I'm gonna use a vector with all the keywords and I'll probably do the same for the accepting states
    bool is_a_keyword = false;
    for (const string& a_keyword : keywords) {
        if (lexeme == a_keyword) {is_a_keyword = true;} 
    }
    return is_a_keyword;
}

char get_next_char(ifstream& stream) {
    char temp_char;
    temp_char = stream.get();
    if (stream.good()) {
        return temp_char;
    } else {return EOF;}
} 

void get_tokens(ifstream& stream, vector<Token>& tokens) { //referenced vector is return target
    Token current_token;
    char current_char = '\0';
    bool new_token = true;
    int column;
    int current_state = 0; //0 is designated as the starting state s0
    int previous_state = 0;
    int char_class[128]; //128 for every possible ASCII character, i.e. 2^7 bits (signed) 
    int delta[UNIQUE_STATES][UNIQUE_CHARACTERS];

    set_up_char_class_array(char_class);
    set_up_delta_array(delta); //The state at the last index will be the error state //I'm defining states sort if in the order they appear in scanner_notes.pdf THEN the order I need them in the delta array


    /*Rules for specific tokens
        When we see contiguous spaces and tabs, we record it as a single " " under WHITESPACE
        When we see a newline, we record it as "\n" under WHITESPACE
        Line continuation, the whole thing from \ to the next line is recorded as null under LINE-CONTINUATION. this token ends after the newline
        comments are recorded as null COMMENT and they consume the \n so the next token starts at the beginning of the next line
    
        TODO: FIX PRINT FUNCTION!!    
    */
    current_char = get_next_char(stream);
    while (current_char != EOF) { //outer loop reads the whole file
        while (current_state != ERROR_STATE) { //inner loop reads each token
            if (!new_token) {current_char = get_next_char(stream);} //at the beginning of every new token we need to skip the first char get
            else {new_token = false;}
            if (current_char != EOF) {
                column = char_class[static_cast<int>(current_char)];
                previous_state = current_state;
                current_state = delta[current_state][column];
                current_token.lexeme += current_char;
            } else {current_state = ERROR_STATE;}
        }
        if (is_accepting_state(previous_state)) {
            if (!current_token.lexeme.empty() && current_char != EOF) {current_token.lexeme.pop_back();} //This removes the last character from the end of the lexeme after making sure the string isn't empty. This also protects the last char in case the one after it is EOF
            //could probably break this whole thing out into a classify function or whatever
            if (previous_state == 7) { //7 is the accepting state for identifiers. This block distingushes kewords from idents
                if (!is_keyword(current_token.lexeme)) {current_token.TOKEN_CLASS = "IDENT";} 
                else if (current_token.lexeme == "true" || current_token.lexeme == "false") {current_token.TOKEN_CLASS = "LITERAL-BOOL";}
                else {current_token.TOKEN_CLASS = "KEYWORD";}
            } 
            else if (previous_state == 1) {current_token.TOKEN_CLASS = "BRACE-OPEN";}
            else if (previous_state == 2) {current_token.TOKEN_CLASS = "BRACE-CLOSE";}
            else if (previous_state == 3) {current_token.TOKEN_CLASS = "PAREN-OPEN";}
            else if (previous_state == 4) {current_token.TOKEN_CLASS = "PAREN-CLOSE";}
            else if (previous_state == 6) {current_token.TOKEN_CLASS = "COMMENT"; current_token.null_lexeme = true;}
            else if (previous_state == 9) {current_token.TOKEN_CLASS = "LINE-CONTINUATION"; current_token.null_lexeme = true;}
            else if (previous_state == 10) {current_token.TOKEN_CLASS = "LITERAL-INT";}
            else if (previous_state == 12) {current_token.TOKEN_CLASS = "LITERAL-FLOAT";}
            else if (previous_state == 15) {current_token.TOKEN_CLASS = "LITERAL-STR";}
            else if ((16 <= previous_state && previous_state <= 19) || previous_state == 22) {current_token.TOKEN_CLASS = "OPERATOR";}
            else if (previous_state == 23) {current_token.TOKEN_CLASS = "WHITESPACE"; current_token.lexeme = "\\n";}
            else if (previous_state == 24) {current_token.TOKEN_CLASS = "WHITESPACE"; current_token.lexeme = " ";}

            tokens.push_back(current_token); //add token to list

            current_token.lexeme = ""; //reset working token
            current_token.TOKEN_CLASS = "";
            current_token.null_lexeme = false;

            current_state = 0; //reset states
            previous_state = 0;

            new_token = true; //so we know to use the current_char first
            //leave current_char in case it's EOF so we break out of outer loop
        }
    }
}

#endif