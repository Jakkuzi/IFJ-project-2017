#include "scanner.h"

int getNextToken(TString *token) {


    int stateOfAutomat = 0;
    int i;
    //int aux;
    stringClean(token);


    while (1) {

        i = getchar();
        i = tolower(i);
        switch (stateOfAutomat) {

            case 0:
                if (i == EOL)
                    return EndOfLine;
                if (isspace(i)) // zistuje biele znaky
                    stateOfAutomat = 0;
                else if (i == '/') {  // komentar alebo delenie
                    stateOfAutomat = 1;
                } else if (isalpha(i) || (i == '_')) {// identifikator,klucove slovo alebo rezervovane klucove slovo
                    addToString(token, i);
                    stateOfAutomat = 3;
                } else if (i == '0') {
                    i = getchar();
                    if (!isdigit(i)) {
                        addToString(token, '0');
                        stateOfAutomat = 6;
                    }
                    ungetc(i, stdin);
                } else if (isdigit(i)) {         //cislo
                    addToString(token, i);
                    stateOfAutomat = 6;
                } else if (i == '!') {             //retazcovy literal
                    i = getchar();
                    if (i != '"')
                        return ErrorInLexicalAnalyzer;
                    stateOfAutomat = 5;
                } else if (i == '<') {
                    addToString(token, i);
                    stateOfAutomat = 11;
                } else if (i == '\'') {           //jednoriadkovy komentar
                    //addToString(token,i);
                    stateOfAutomat = 4;
                } else if (i == '>') {
                    addToString(token, i);
                    stateOfAutomat = 12;
                } else if (i == '+') {
                    addToString(token, i);
                    return Plus;
                } else if (i == '-') {
                    addToString(token, i);
                    return Minus;
                } else if (i == '*') {
                    addToString(token, i);
                    return Mul;
                }
/**/      else if (i == '\\') {
                    addToString(token, i);
                    return IntDiv;
                } else if (i == '=') {
                    addToString(token, i);
                    return Equal;
                } else if (i == '(') {
                    addToString(token, i);
                    return LeftParenthes;
                } else if (i == ')') {
                    addToString(token, i);
                    return RightParenthes;
                } else if (i == '{') {
                    addToString(token, i);
                    return LeftBrace;
                } else if (i == '}') {
                    addToString(token, i);
                    return RightBrace;
                } else if (i == ',') {
                    addToString(token, i);
                    return Comma;
                } else if (i == ';') {
                    addToString(token, i);
                    return Semicolon;
                } else if (i == EOF)
                    return EndOfFile;
                else
                    return ErrorInLexicalAnalyzer;
                break;

            case 1:
                if (i == '\'')
                    stateOfAutomat = 2;
                else {
                    ungetc(i, stdin);
                    addToString(token, '/');
                    return Div;
                }
                break;

            case 2:           //blokovy komentar
                if (i == '\'') {
                    i = getchar();
                    if (i == '/')
                        stateOfAutomat = 0;
                    else {
                        ungetc(i, stdin);
                        return EndOfLine;
                    }
                } else if (i == EOF)
                    return ErrorInLexicalAnalyzer;
                break;


            case 3:
                if (isalnum(i) || (i == '_')) //identifikator
                    addToString(token, i);
                else if (compareStringAndString(token, "asc") == 0) {
                    ungetc(i, stdin);
                    return ID;
                } else if (compareStringAndString(token, "chr") == 0) {
                    ungetc(i, stdin);
                    return ID;
                } else if (compareStringAndString(token, "length") == 0) {
                    ungetc(i, stdin);
                    return ID;
                } else if (compareStringAndString(token, "substr") == 0) {
                    ungetc(i, stdin);
                    return ID;
                } else {
                    ungetc(i, stdin);
                    if (compareStringAndString(token, "as") == 0)  //rezervovane slova
                        return As;
                    else if (compareStringAndString(token, "declare") == 0)
                        return Declare;
                    else if (compareStringAndString(token, "dim") == 0)
                        return Dim;
                    else if (compareStringAndString(token, "do") == 0)
                        return Do;
                    else if (compareStringAndString(token, "double") == 0)
                        return Double;
                    else if (compareStringAndString(token, "else") == 0)
                        return Else;
                    else if (compareStringAndString(token, "end") == 0)
                        return End;
                    else if (compareStringAndString(token, "function") == 0)
                        return Function;
                    else if (compareStringAndString(token, "if") == 0)
                        return If;
                    else if (compareStringAndString(token, "input") == 0)
                        return Input;
                    else if (compareStringAndString(token, "integer") == 0)
                        return Integer;
                    else if (compareStringAndString(token, "loop") == 0)
                        return Loop;
                    else if (compareStringAndString(token, "print") == 0)
                        return Print;
                    else if (compareStringAndString(token, "return") == 0)
                        return Return;
                    else if (compareStringAndString(token, "scope") == 0)
                        return Scope;
                    else if (compareStringAndString(token, "string") == 0)
                        return String;
                    else if (compareStringAndString(token, "then") == 0)
                        return Then;
                    else if (compareStringAndString(token, "while") == 0)
                        return While;
                    else if (compareStringAndString(token, "and") == 0)    //rezervovane klucove slova
                        return And;
                    else if (compareStringAndString(token, "boolean") == 0)
                        return Boolean;
                    else if (compareStringAndString(token, "continue") == 0)
                        return Continue;
                    else if (compareStringAndString(token, "elseif") == 0)
                        return Elseif;
                    else if (compareStringAndString(token, "exit") == 0)
                        return Exit;
                    else if (compareStringAndString(token, "false") == 0)
                        return False;
                    else if (compareStringAndString(token, "for") == 0)
                        return For;
                    else if (compareStringAndString(token, "next") == 0)
                        return Next;
                    else if (compareStringAndString(token, "not") == 0)
                        return Not;
                    else if (compareStringAndString(token, "or") == 0)
                        return Or;
                    else if (compareStringAndString(token, "shared") == 0)
                        return Shared;
                    else if (compareStringAndString(token, "static") == 0)
                        return Static;
                    else if (compareStringAndString(token, "true") == 0)
                        return True;
                    else
                        return ID;

                }
                break;
            case 4:          //riadkovy komentar
                if (i == '\n') {
                    stateOfAutomat = 0;
                    return EndOfLine;
                }
                break;


            case 5:

                if (i == '\\') {
                    i = getchar();
                    if (i == '"')
                        addToString(token, '"');
                    else if (i == 'n')
                        addToString(token, '\n');
                    else if (i == 't')
                        addToString(token, '\t');
                    else if (i == '\\')
                        addToString(token, '\\');
                    else {
                        int value[4];
                        int j;
                        for (j = 0; (j <= 2) && isdigit(i); j++) {
                            value[j] = i - '0';
                            if (j + 1 <= 2)
                                i = getchar();
                        }
                        int result;
                        if (j == 3) result = value[0] * 100 + value[1] * 10 + value[2];
                        if ((result >= 001) && (result <= 255)) {
                            addToString(token, (int) result);
                        } else {
                            return ErrorInLexicalAnalyzer;
                        }
                    }
                } else if (isprint(i)) {

                    if (i == '"')
                        return valueOfString;
                    addToString(token, i);
                } else {
                    ungetc(i, stdin);
                    return ErrorInLexicalAnalyzer;
                }
                break;

            case 6:
                if (isdigit(i)) {
                    addToString(token, i);
                    stateOfAutomat = 6;
                } else if (i == '.') {
                    addToString(token, i);
                    stateOfAutomat = 7;
                } else if ((i == 'E') || (i == 'e')) {
                    addToString(token, i);
                    stateOfAutomat = 13;
                } else {
                    ungetc(i, stdin);
                    return valueOfInteger;
                }
                break;

            case 7:
                if (isdigit(i)) {
                    addToString(token, i);
                    stateOfAutomat = 8;
                } else {
                    ungetc(i, stdin);
                    return ErrorInLexicalAnalyzer;
                }
                break;


            case 8:
                if (isdigit(i))
                    addToString(token, i);
                else if ((i == 'E') || (i == 'e')) {
                    addToString(token, i);
                    stateOfAutomat = 9;
                } else {
                    ungetc(i, stdin);
                    return valueOfDouble;
                }
                break;

            case 9:
                if ((i == '+') || (i == '-')) {
                    addToString(token, i);
                    stateOfAutomat = 10;
                } else if (i == '0') {
                    i = getchar();
                    if (!isdigit(i)) {
                        addToString(token, '0');
                        stateOfAutomat = 10;
                    }
                } else if (isdigit(i)) {
                    addToString(token, i);
                    stateOfAutomat = 10;
                } else
                    return ErrorInLexicalAnalyzer;
                break;

            case 10:
                if (i == '0') {
                    i = getchar();
                    if (!isdigit(i)) {
                        addToString(token, '0');
                        stateOfAutomat = 10;
                    }
                } else if (isdigit(i)) {
                    addToString(token, i);
                    stateOfAutomat = 10;
                } else {
                    ungetc(i, stdin);
                    return valueOfDouble;
                }
                break;

            case 11:
                if (i == '>')
                    return LowerGreater;
                else if (i == '=')
                    return LowerOrEqual;
                else {
                    ungetc(i, stdin);
                    return Lower;
                }
            case 12:
                if (i == '=')
                    return GreaterOrEqual;
                else {
                    ungetc(i, stdin);
                    return Greater;
                }
            case 13:
                if (i == '0') {
                    i = getchar();
                    if (!isdigit(i)) {
                        addToString(token, '0');
                        stateOfAutomat = 10;
                    }
                } else if (isdigit(i)) {
                    addToString(token, i);
                    stateOfAutomat = 10;
                } else if ((i == '+') || (i == '-')) {
                    addToString(token, i);
                    stateOfAutomat = 10;
                } else
                    return ErrorInLexicalAnalyzer;

                break;
        }
    }
}
