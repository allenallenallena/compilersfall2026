#ifndef DIRECT_CODED_IMPLEMENTATION
#define DIRECT_CODED_IMPLEMENTATION 

using std::vector;
using std::ifstream;
using std::string;

//I don't think I need ANY since it's used like once and I can circumvent it by doing NOT certain characters...
enum Char_classes { 
    ANY, //'.' i.e. "any" does NOT match ascii 10 or 13 \n or \r (newline or carriage return) //ALSO Not truly any, it also excludes every other character in ENUM
    SPACE, //ascii 9 \t (tab) & 32 (space)
    NEWLINE,  //10 \n (newline character), 13 \r (carriage return)
    OPERATOR, // +, -, / *, %
    RELATIONAL, // <, >, !
    EQUALS,
    AMPERSAND,
    PIPE,
    QUOTATION_MARK,
    POUND,
    OPEN_PAREN,
    CLOSE_PAREN,
    PERIOD,
    NUMBER,
    ALPHABETIC, //includes underscore
    BACKSLASH,
    OPEN_CURLY,
    CLOSE_CURLY
};
const int UNIQUE_CHARACTERS = 18; 
const vector<string> keywords = {"bool", "else", "false", "float", "if", "int", "str", "true", "while"};

/* This was my initial naiive implementation of setting up this array. it's woefully inneficient. If unique characters scaled by log(n) with the total characters in a langauge and we weren't limited by 7 bits it'd be like O(nlogn) or something like that. for every character of n characters you do about logn comparisons or something like that idk...
void set_up_char_class_array(int char_class[]) { //this is a monstrocity.
    for (int i = 0; i < 128; i++) {
        if (i == 9 || i == 32) {char_class[i] = SPACE;} //tab (\t) & space
        else if (i == 10 || i == 13) {char_class[i] = NEWLINE;} //newline (\n) & carriage return (\r)
        else if (i == 37 || i == 42 || i == 43 || i == 45 || i == 47) {char_class[i] = OPERATOR;} //(+ / - * %)
        else if (i == 33 || i == 60 || i == 62) {char_class[i] = RELATIONAL;} //(< > !)
        else if (i == 61) {char_class[i] = EQUALS;}
        else if (i == 38) {char_class[i] = AMPERSAND;}
        else if (i == 124) {char_class[i] = PIPE;}
        else if (i == 34) {char_class[i] = QUOTATION_MARK;}
        else if (i == 35) {char_class[i] = POUND;}
        else if (i == 40) {char_class[i] = OPEN_PAREN;}
        else if (i == 41) {char_class[i] = CLOSE_PAREN;}
        else if (i == 46) {char_class[i] = PERIOD;}
        else if (48 <= i && i <= 57) {char_class[i] = NUMBER;} 
        else if ((65 <= i && i <= 90) || i == 95 || (97 <= i && i <= 122)) {char_class[i] = ALPHABETIC;} 
        else if (i == 92) {char_class[i] = BACKSLASH;} 
        else if (i == 123) {char_class[i] = OPEN_CURLY;}
        else if (i == 125) {char_class[i] = CLOSE_CURLY;}
        else {char_class[i] = ANY;}
    }
    return;
}
*/

//This one just goes down the line running ONE operation per unique character. This one should be about O(2n) (i.e. O(n)) where n is characters in the language
void set_up_char_class_array(Char_classes char_class[]) { 
    for (int i = 0; i < 128; i++) {char_class[i] = ANY;} 
    char_class['\t'] = char_class['\r'] = char_class[' '] = SPACE;
    char_class['\n'] = NEWLINE;
    char_class['+'] = char_class['/'] = char_class['-'] = char_class['*'] = char_class['%'] = OPERATOR;
    char_class['<'] = char_class['>'] = char_class['!'] = RELATIONAL;
    char_class['='] = EQUALS;
    char_class['&'] = AMPERSAND;
    char_class['|'] = PIPE;
    char_class['"'] = QUOTATION_MARK;
    char_class['#'] = POUND;
    char_class['('] = OPEN_PAREN;
    char_class[')'] = CLOSE_PAREN;
    char_class['.'] = PERIOD;
    for (char i = '0'; i <= '9'; i++) {char_class[static_cast<int>(i)] = NUMBER;} 
    for (char i = 'A'; i <= 'Z'; i++) {char_class[static_cast<int>(i)] = ALPHABETIC;}
    for (char i = 'a'; i <= 'z'; i++) {char_class[static_cast<int>(i)] = ALPHABETIC;}
    char_class['_'] = ALPHABETIC;
    char_class['\\'] = BACKSLASH;
    char_class['{'] = OPEN_CURLY;
    char_class['}'] = CLOSE_CURLY;
    return;
}

enum States { //I tried to label these as appropriately as I could
    START_STATE,
    OPEN_CURLY_STATE,
    CLOSE_CURLY_STATE,
    OPEN_PAREN_STATE,
    CLOSE_PAREN_STATE,
    BEGIN_COMMENT_STATE,
    END_COMMENT_STATE,
    IDENT_STATE,
    BEGIN_LINE_CONT_STATE,
    END_LINE_CONT_STATE,
    INT_STATE,
    BEGIN_FLOAT_STATE,
    END_FLOAT_STATE,
    BEGIN_STRING_STATE,
    ESCAPE_QUOTATION_STATE,
    END_STRING_STATE,
    EITHER_OPERATOR_STATE, //for <, >, !, = since they all can add an = to the end to move to a unique accepting state
    FINAL_OPERATOR_STATE, // for +, -, *, /, % since they all only let you do one and the final state for the other 2 types of ops
    BEGIN_AND_STATE,
    BEGIN_OR_STATE,
    WHITESPACE_STATE,
    ERROR_STATE
};

const int UNIQUE_STATES = 22; //s0-s20 (21 states) + error state 

void set_up_accepting_states(bool accepting_states[]) {
    for (int i = 0; i < UNIQUE_STATES; i++) {
        if (i == OPEN_CURLY_STATE || i == CLOSE_CURLY_STATE || i == OPEN_PAREN_STATE || i == CLOSE_PAREN_STATE || i == BEGIN_COMMENT_STATE || i == END_COMMENT_STATE || i == IDENT_STATE || i == END_LINE_CONT_STATE || i == INT_STATE || i == END_FLOAT_STATE || i == END_STRING_STATE || i == EITHER_OPERATOR_STATE || i == FINAL_OPERATOR_STATE || i == WHITESPACE_STATE) //handpicked
        {accepting_states[i] = true;} else {accepting_states[i] = false;}
    }
    return;
}
/* LIST OF STATES!
 s0: starting state

 s1: open curly (accepting) NO TRANSITIONS
 s2: close curly (accepting) NO TRANSITIONS
 s3: open paren (accepting) NO TRANSITIONS
 s4: close paren (accepting) NO TRANSITIONS

 s5: pound begin comment/comment contents (accpeting)
 s6: end comment (accepting) NO TRANSITIONS

 s7: identifier (accepting) [q3 in webapp helper]

 s8: line continuation begin/whitespace after [q9 in helper]
 s9: line continuation end (accepting) NO TRANSITIONS

 s10: literal-int (accepting) [q12]
 s11: mid-float period "*.   "
 s12: float end (accepting)

 s13: string-literal beign 
 s13: escape " within string 
 s15: string-literal end (accepting) NO TRANSITIONS

 s16: <, >, =, ! (accepting)

 s17: operator final destination EXCEPT <, >, =, ! [AND part 2, OR part 2, <=, >=, !=, ==, + - * / % (accepting) NO TRANSITIONS

 s18: AND part 1
 s19: OR part 1

 s20: whitespace (accepting)

 se: s21 in spirit
 */

void set_up_delta_array(int delta[][UNIQUE_CHARACTERS]) {
    for (int i = 0; i < UNIQUE_STATES; i++) { //Outer loop for states
        for (int j = 0; j < UNIQUE_CHARACTERS; j++) { //Inner loop for each character
            switch (i) {
                case START_STATE: { 
                    if (j == OPEN_CURLY) {delta[i][j] = OPEN_CURLY_STATE;} //DONE
                    else if (j == CLOSE_CURLY) {delta[i][j] = CLOSE_CURLY_STATE;} //DONE
                    else if (j == OPEN_PAREN) {delta[i][j] = OPEN_PAREN_STATE;} //DONE
                    else if (j == CLOSE_PAREN) {delta[i][j] = CLOSE_PAREN_STATE;} //DONE
                    else if (j == POUND) {delta[i][j] = BEGIN_COMMENT_STATE;} //done
                    else if (j == ALPHABETIC) {delta[i][j] = IDENT_STATE;} //done
                    else if (j == BACKSLASH) {delta[i][j] = BEGIN_LINE_CONT_STATE;} //done
                    else if (j == NUMBER) {delta[i][j] = INT_STATE;} //done
                    else if (j == QUOTATION_MARK) {delta[i][j] = BEGIN_STRING_STATE;}
                    else if (j == RELATIONAL || j == EQUALS) {delta[i][j] = EITHER_OPERATOR_STATE;}
                    else if (j == AMPERSAND) {delta[i][j] = BEGIN_AND_STATE;}
                    else if (j == PIPE) {delta[i][j] = BEGIN_OR_STATE;}
                    else if (j == OPERATOR) {delta[i][j] = FINAL_OPERATOR_STATE;}
                    else if (j == SPACE || j == NEWLINE) {delta[i][j] = WHITESPACE_STATE;}
                    else {delta[i][j] = ERROR_STATE;} // error state
                    break;
                }
                case BEGIN_COMMENT_STATE: { //done
                    if (j != NEWLINE) {delta[i][j] = BEGIN_COMMENT_STATE;}
                    else if (j == NEWLINE) {delta[i][j] = END_COMMENT_STATE;}
                    else {delta[i][j] = ERROR_STATE;} // error state
                    break;
                }

                case IDENT_STATE: { //done
                    if (j == ALPHABETIC || j == NUMBER) {delta[i][j] = IDENT_STATE;}
                    else {delta[i][j] = ERROR_STATE;} // error state
                    break;
                }
                case BEGIN_LINE_CONT_STATE: { //done
                    if (j == SPACE) {delta[i][j] = BEGIN_LINE_CONT_STATE;}
                    else if (j == NEWLINE) {delta[i][j] = END_LINE_CONT_STATE;}
                    else {delta[i][j] = ERROR_STATE;} // error state
                    break;
                }
                case INT_STATE: { //done
                    if (j == NUMBER) {delta[i][j] = INT_STATE;}
                    else if (j == PERIOD) {delta[i][j] = BEGIN_FLOAT_STATE;}
                    else {delta[i][j] = ERROR_STATE;} // error state
                    break;
                }
                case BEGIN_FLOAT_STATE: { //done
                    if (j == NUMBER) {delta[i][j] = END_FLOAT_STATE;}
                    else {delta[i][j] = ERROR_STATE;} // error state
                    break;
                }
                case END_FLOAT_STATE: { //done
                    if (j == NUMBER) {delta[i][j] = END_FLOAT_STATE;}
                    else {delta[i][j] = ERROR_STATE;} // error state
                    break;
                }
                case BEGIN_STRING_STATE: { //done
                    if (j == BACKSLASH) {delta[i][j] = ESCAPE_QUOTATION_STATE;}
                    else if (j != QUOTATION_MARK && j != NEWLINE) {delta[i][j] = BEGIN_STRING_STATE;}
                    else if (j == QUOTATION_MARK) {delta[i][j] = END_STRING_STATE;}
                    else {delta[i][j] = ERROR_STATE;} // error state
                    break;
                }
                case ESCAPE_QUOTATION_STATE: { //done
                    if (j == BACKSLASH) {delta[i][j] = ESCAPE_QUOTATION_STATE;}
                    else if (j != NEWLINE) {delta[i][j] = BEGIN_STRING_STATE;}
                    else {delta[i][j] = ERROR_STATE;} // error state
                    break;
                }
                case EITHER_OPERATOR_STATE: { //done
                    if (j == EQUALS) {delta[i][j] = FINAL_OPERATOR_STATE;}
                    else {delta[i][j] = ERROR_STATE;} // error state
                    break;
                }
                case BEGIN_AND_STATE: { //done
                    if (j == AMPERSAND) {delta[i][j] = FINAL_OPERATOR_STATE;}
                    else {delta[i][j] = ERROR_STATE;} // error state
                    break;
                }
                case BEGIN_OR_STATE: { //done
                    if (j == PIPE) {delta[i][j] = FINAL_OPERATOR_STATE;}
                    else {delta[i][j] = ERROR_STATE;} // error state
                    break;
                }
                case WHITESPACE_STATE: { //done
                    if (j == SPACE || j == NEWLINE) {delta[i][j] = WHITESPACE_STATE;}
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

/*
void print_delta_array(int delta [][UNIQUE_CHARACTERS]) { //just for fun

    return;
}
*/

bool is_keyword(string lexeme) { //I'm gonna use a vector with all the keywords
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
    int current_state = START_STATE; 
    int previous_state = START_STATE;
    Char_classes char_class[128]; //128 for every possible ASCII character, i.e. 2^7 bits (signed) //I just learned that you can make arrays of enum types
    int delta[UNIQUE_STATES][UNIQUE_CHARACTERS];
    bool accepting_states[UNIQUE_STATES]; //I think this is a little clever and I will allow myself to be a little proud of this

    set_up_char_class_array(char_class);
    set_up_delta_array(delta); //The state at the last index will be the error state //I'm defining states sort if in the order they appear in scanner_notes.pdf THEN the order I need them in the delta array
    set_up_accepting_states(accepting_states);

    /*Rules for specific tokens
        When we see contiguous spaces and tabs, we record it as a single " " under WHITESPACE
        When we see a newline, we record it as "\n" under WHITESPACE
        Line continuation, the whole thing from \ to the next line is recorded as null under LINE-CONTINUATION. this token ends after the newline
        comments are recorded as null COMMENT and they consume the \n so the next token starts at the beginning of the next line
    
        TODO: FIX PRINT FUNCTION!!    
    */
    current_char = get_next_char(stream); 
    //if file BEGINS WITH EOF
    if (current_char == EOF) { 
        current_token.TOKEN_CLASS = "EOF";
        tokens.push_back(current_token);
        return;
    }

    //if file ends with some kind of whitespace followed by EOF
    while (current_char != EOF) { //outer loop reads the whole file
        while (current_state != ERROR_STATE) { //inner loop reads each token
            if (!new_token) {current_char = get_next_char(stream);} //at the beginning of every new token we need to skip the first char get
            else {new_token = false;}
            if (current_char != EOF) {
                column = char_class[static_cast<int>(current_char)];
                previous_state = current_state;
                current_state = delta[current_state][column];
                if (current_char == '\n') {current_token.lexeme = current_token.lexeme + "\\n";} 
                else if (current_char == '\r') {current_token.lexeme = current_token.lexeme + "\\r";} 
                else if (current_char == '\\') {current_token.lexeme = current_token.lexeme + "\\\\";} 
                else if (current_char == '"') {current_token.lexeme = current_token.lexeme + "\\\"";} 
                else{current_token.lexeme = current_token.lexeme + current_char;}
            } else {
                previous_state = current_state;
                current_state = ERROR_STATE;
            }
        }
        if (accepting_states[previous_state]) {
            if (!current_token.lexeme.empty() && current_char != EOF) {
                if (current_char == '\n' || current_char == '\r' || current_char == '"' || current_char == '\\') {current_token.lexeme.pop_back(); current_token.lexeme.pop_back();} 
                else{current_token.lexeme.pop_back();}
            }
            if (previous_state == IDENT_STATE) {
                if (!is_keyword(current_token.lexeme)) {current_token.TOKEN_CLASS = "IDENT";} 
                else if (current_token.lexeme == "true" || current_token.lexeme == "false") {current_token.TOKEN_CLASS = "LITERAL-BOOL";}
                else {current_token.TOKEN_CLASS = "KEYWORD";}
            } 
            else if (previous_state == OPEN_CURLY_STATE) {current_token.TOKEN_CLASS = "BRACE-OPEN";}
            else if (previous_state == CLOSE_CURLY_STATE) {current_token.TOKEN_CLASS = "BRACE-CLOSE";}
            else if (previous_state == OPEN_PAREN_STATE) {current_token.TOKEN_CLASS = "PAREN-OPEN";}
            else if (previous_state == CLOSE_PAREN_STATE) {current_token.TOKEN_CLASS = "PAREN-CLOSE";}
            else if (previous_state == END_COMMENT_STATE) {current_token.TOKEN_CLASS = "COMMENT"; current_token.null_lexeme = true;}
            else if (previous_state == END_LINE_CONT_STATE) {current_token.TOKEN_CLASS = "LINE-CONTINUATION"; current_token.null_lexeme = true;}
            else if (previous_state == INT_STATE) {current_token.TOKEN_CLASS = "LITERAL-INT";}
            else if (previous_state == END_FLOAT_STATE) {current_token.TOKEN_CLASS = "LITERAL-FLOAT";}
            else if (previous_state == END_STRING_STATE) {current_token.TOKEN_CLASS = "LITERAL-STR";}
            else if (previous_state == EITHER_OPERATOR_STATE || previous_state == FINAL_OPERATOR_STATE) {current_token.TOKEN_CLASS = "OPERATOR";}
            else if (previous_state == WHITESPACE_STATE) {
                if (current_char == EOF) {current_token.TOKEN_CLASS = "EOF";} else {current_token.TOKEN_CLASS = "WHITESPACE";}
            }

            tokens.push_back(current_token); //add token to list
            if (current_token.TOKEN_CLASS == "EOF") {return;}

            current_token.lexeme = ""; //reset working token
            current_token.TOKEN_CLASS = "";
            current_token.null_lexeme = false;

            current_state = START_STATE; //reset states
            previous_state = START_STATE;

            new_token = true; //so we know to use the current_char first
            //leave current_char in case it's EOF so we break out of outer loop
        } else { //last state was NOT an accepting state
            current_token.TOKEN_CLASS = "ERR";
            tokens.push_back(current_token);
            return;
        }
    }

    //if the file abruptly EOF with no whitespace at the end
    if (current_char == EOF) {
        current_token.TOKEN_CLASS = "EOF";
        tokens.push_back(current_token);
    }

    return;
}

#endif
