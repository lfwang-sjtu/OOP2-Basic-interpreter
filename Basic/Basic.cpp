/*
 * File: Basic.cpp
 * ---------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * [TODO: extend and correct the documentation]
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
using namespace std;

/* Function prototypes */

void processLine(string line, Program & program, EvalState & state);

/* Main program */

int main() {
   EvalState state;
   Program program;
   cout << "Stub implementation of BASIC" << endl;
   while (true) {
      try {
         processLine(getLine(), program, state);
      } catch (ErrorException & ex) {
         cerr << "Error: " << ex.getMessage() << endl;
      }
   }
   return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program & program, EvalState & state) {
    ///scanner
    TokenScanner scanner;
    scanner.scanNumbers();
    scanner.setInput(line);
    ///process variables
    std::string token;
    int lineNumber;

    scanner.ignoreWhitespace();
    if (scanner.hasMoreTokens())
    {
        token = scanner.nextToken();
        if (scanner.getTokenType(token) == NUMBER)
        {
            lineNumber = stringToInteger(token);
            scanner.ignoreWhitespace();
            if (scanner.hasMoreTokens())
            {
                program.addSourceLine(lineNumber, line);
                token = scanner.nextToken();
                //todo "10 + LET + Expression", we have get rid of the the line number.
                if (token == "REM")
                {
                    program.setParsedStatement(lineNumber,NULL);
                    return;
                }
                if (token == "LET")
                {
                    LetState let(parseExp(scanner));
                    program.setParsedStatement(lineNumber,&let);
                    return;
                }
                if (token == "PRINT")
                {
                    PrintState print(parseExp(scanner));
                    program.setParsedStatement(lineNumber,&print);
                    return;
                }
                if (token == "INPUT")
                {
                    InputState input(parseExp(scanner));
                    program.setParsedStatement(lineNumber,&input);
                    return;
                }
                if (token == "END")
                {
                    EndState end;
                    program.setParsedStatement(lineNumber,&end);
                    return;
                }
                if (token == "GOTO")
                {
                    token = scanner.nextToken();
                    int targetLine = stringToInteger(token);
                    GotoState gotoState(targetLine);
                    program.setParsedStatement(lineNumber,&gotoState);
                    return;
                }
                if (token == "IF")
                {
                    Expression *lh,*rh;
                    string op;
                    int targetLine;
                    try {
                        lh = parseExp(scanner);
                    }
                    catch (string) {

                    }
                    IfState ifState(lh,rh,op,targetLine);
                }
                error("SYNTAX ERROR");
            }
            else
            {
                program.removeSourceLine(lineNumber);
            }
        }
        else
        {
            if (scanner.getTokenType(token) == WORD)
            {
                if (token == "LET")
                {
                    scanner.ignoreWhitespace();
                    LetState let(parseExp(scanner));
                    let.execute(state);
                    return;
                }
                if (token == "PRINT")
                {
                    scanner.ignoreWhitespace();
                    cout << parseExp(scanner)->eval(state) << endl;
                    return;
                }
                if (token == "INPUT")
                {
                    scanner.ignoreWhitespace();
                    InputState input(parseExp(scanner));
                    input.execute(state);
                }
                if (token == "RUN")
                {

                }
                if (token == "LIST")
                {
                    program.listSourceCode();
                    return;
                }
                if (token == "CLEAR")
                {
                    program.clear();
                    ClearState clear;
                    clear.execute(state);
                    return;
                }
                if (token == "QUIT")
                {
                    QuitState quit;
                    quit.execute(state);
                    return;
                }
                if (token == "HELP")
                {
                    HelpState help;
                    help.execute(state);
                    return;
                }
                error("SYNTAX ERROR.");
            }
            else error("SYNTAX ERROR.");
        }
    }
}
