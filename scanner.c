#include "scanner.h"

int getNextToken(TString* token){

  int stateOfAutomat=0;
  int i;
  int aux;
  stringClean(token);

  while (1) {
    
    i=tolower(i);
    i=getchar();

      switch(stateOfAutomat){

        case 0:
          if (isspace(i)) // zistuje biele znaky
          stateOfAutomat=0;
          else if (i=='/')  // komentar alebo delenie
               stateOfAutomat=1;
          else if (isalpha(i) || (i=='_')) {// identifikator,klucove slovo alebo rezervovane klucove slovo
               addToString(token,i);
               stateOfAutomat=3;
          }
          else if (isdigit(i)){         //cislo
                 addToString(token,i);
                 stateOfAutomat=6;
          }
          else if (i=='!')             //retazcovy literal
                 stateOfAutomat=5;
          else if (i=='<')
                 stateOfAutomat=11;
          else if (i=='\'')            //jednoriadkovy komentar
                 stateOfAutomat=4;
          else if (i=='<')
                 stateOfAutomat=12;
          else if (i=='+')
                  return Plus;
          else if (i=='-')
                  return Minus;
          else if (i=='*')
                  return Mul;
          else if (i=='\'')
                  return IntDiv;
          else if (i=='=')
                  return Equal;
          else if (i=='(')
                  return LeftParenthes;
          else if (i==')')
                  return RightParenthes;
          else if (i=='{')
                  return LeftBrace;
          else if (i=='}')
                  return RightBrace;
          else if (i==',')
                  return Comma;
          else if (i==';')
                  return Semicolon;
          else if (i==EOF)
                  return EndOfFile;
          else if (i==EOL)
                  return EndOfLine;
          else
                  return ErrorInLexicalAnalyzer;
        break;

        case 1:
          if(i=='/')
            stateOfAutomat=2;
          else{
            ungetc(i,stdin);
            return Div;
          }
         break;

        case 2:           //blokovy komentar
          if (i=='/'){
            i=getchar();
            if (i=='/')
              stateOfAutomat=0;
            else
              ungetc(i,stdin);
           }
          else if (i==EOF)
            return ErrorInLexicalAnalyzer;
        break;

        case 3:
          if (isalnum(i) || (i=='_')) //identifikator
            addToString(token,i);
          else{
            ungetc(i,stdin);
            if (compareStringAndString(token,"As")== 0)  //rezervovane slova
                return As;
            else if (compareStringAndString(token,"Asc")== 0)
                return Asc;
            else if (compareStringAndString(token,"Declare")== 0)
                return Declare;
            else if (compareStringAndString(token,"Dim")== 0)
                return Dim;
            else if (compareStringAndString(token,"Do")== 0)
                return Do;
            else if (compareStringAndString(token,"Double")== 0)
                return Double;
            else if (compareStringAndString(token,"Else")== 0)
                return Else;
            else if (compareStringAndString(token,"End")== 0)
                return End;
            else if (compareStringAndString(token,"Chr")== 0)
                return Chr;
            else if (compareStringAndString(token,"Function")== 0)
                return Function;
            else if (compareStringAndString(token,"If")== 0)
                return If;
            else if (compareStringAndString(token,"Input")== 0)
                return Input;
            else if (compareStringAndString(token,"Integer")== 0)
                return Integer;
            else if (compareStringAndString(token,"Length")== 0)
                return Length;
            else if (compareStringAndString(token,"Loop")== 0)
                return Loop;
            else if (compareStringAndString(token,"Print")== 0)
                return Print;
            else if (compareStringAndString(token,"Return")== 0)
                return Return;
            else if (compareStringAndString(token,"Scope")== 0)
                return Scope;
            else if (compareStringAndString(token,"String")== 0)
                return String;
            else if (compareStringAndString(token,"SubStr")== 0)
                return SubStr;
            else if (compareStringAndString(token,"Then")== 0)
                return Then;
            else if (compareStringAndString(token,"While")== 0)
                return While;
            else if (compareStringAndString(token,"And")== 0)    //rezervovane klucove slova
                return And;
            else if (compareStringAndString(token,"Boolean")== 0)
                return Boolean;
            else if (compareStringAndString(token,"Continue")== 0)
                return Continue;
            else if (compareStringAndString(token,"Elseif")== 0)
                return Elseif;
            else if (compareStringAndString(token,"Exit")== 0)
                return Exit;
            else if (compareStringAndString(token,"False")== 0)
                return False;
            else if (compareStringAndString(token,"For")== 0)
                return For;
            else if (compareStringAndString(token,"Next")== 0)
                return Next;
            else if (compareStringAndString(token,"Not")== 0)
                return Not;
            else if (compareStringAndString(token,"Or")== 0)
                return Or;
            else if (compareStringAndString(token,"Shared")== 0)
                return Shared;
            else if (compareStringAndString(token,"Static")== 0)
                return Static;
            else if (compareStringAndString(token,"True")== 0)
                return True;
            else
                return ID;

          }

        case 4:          //riadkovy komentar
          if (i=='\n')
            stateOfAutomat=0;
        break;

        case 5:
           if (i=='"'){
             i=getchar();
             if (i<=001 || i>=255)
                return ErrorInLexicalAnalyzer;
               else if(i=='"')
                addToString(token,'\"');
               else if (i=='n')
                addToString(token,'\n');
               else if (i=='t')
                addToString(token,'\t');
               else if (i=='\\')
                addToString(token,'\\');
               else if (isdigit(i)){
                      {aux=i;
                      addToString(token,aux);}
                    if (isdigit(i=getchar())){
                        {aux=aux*10+i;
                        addToString(token,aux);}
                        if (isdigit(i=getchar()))
                            {aux=aux*10+i;
                            addToString(token,aux);}
                            else if (i==EOF)
                                     return ErrorInLexicalAnalyzer;
                        else
                            return ErrorInLexicalAnalyzer;
                  } else if (i==EOF)
                            return ErrorInLexicalAnalyzer;
                         else
                            return ErrorInLexicalAnalyzer;
               }

           else
                return ErrorInLexicalAnalyzer;
               }
        break;

        case 6:
           if (isdigit(i)){
              addToString(token,i);
              stateOfAutomat=6;
           }
           else if (i=='.'){
                    addToString(token,i);
                    stateOfAutomat=7;
           }
           else if ((i=='E') || (i=='e')){
                   addToString(token,i);
                   stateOfAutomat=8;
           }
           else{
                ungetc(i,stdin);
                return valueOfInteger;
           }
        break;

        case 7:
          if (isdigit(i)){
              addToString(token,i);
              stateOfAutomat=8;
          }
          else{
              ungetc(i,stdin);
              return ErrorInLexicalAnalyzer;
          }
        break;

        case 8:
          if (isdigit(i))
              addToString(token,i);
          else if ((i=='E') || (i=='e')){
              addToString(token,i);
              stateOfAutomat=9;
          }
          else{
              ungetc(i,stdin);
              return valueOfDouble;
          }
        break;

        case 9:
          if ((i=='+') || (i=='-')){
              addToString(token,i);
              stateOfAutomat=10;
          }
        break;

        case 10:
          if (isdigit(i)){
              addToString(token,i);
              return valueOfDoubleWithExp;
          }
          else return ErrorInLexicalAnalyzer;
        break;

        case 11:
          if(i=='>')
             return LowerGreater;
           else if (i== '=')
              return LowerOrEqual;
           else{
            ungetc(i,stdin);
            return Lower;
           }
        break;

        case 12:
          if(i=='=')
             return GreaterOrEqual;
          else{
            ungetc(i,stdin);
            return Greater;
           }
        break;


  }
 }
}
