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
        //todo 以数字为开头，需要保存在program里处理
        if (scanner.getTokenType(token) == NUMBER)
        {
            lineNumber = stringToInteger(token);
            scanner.ignoreWhitespace();
            if (scanner.hasMoreTokens())
            {
                program.addSourceLine(lineNumber, line);
                token = scanner.nextToken();
                //todo 10 + REM + string
                if (token == "REM")
                {
                    RemState *rem = new RemState;
                    program.setParsedStatement(lineNumber,rem);
                    return;
                }
                //todo 10 + LET + CompoundExp
                if (token == "LET")
                {
                    LetState *let = new LetState(parseExp(scanner));
                    program.setParsedStatement(lineNumber,let);
                    return;
                }
                //todo 10 + PRINT + Expression
                if (token == "PRINT")
                {
                    PrintState *print = new PrintState((parseExp(scanner)));
                    program.setParsedStatement(lineNumber,print);
                    return;
                }
                //todo 10 + INPUT + IdentifierExp
                if (token == "INPUT")
                {
                    InputState *input = new InputState(parseExp(scanner));
                    program.setParsedStatement(lineNumber,input);
                    return;
                }
                // todo 10 + END
                if (token == "END")
                {
                    EndState *end = new EndState;
                    program.setParsedStatement(lineNumber,end);
                    return;
                }
                //todo 10 + GOTO + 20
                if (token == "GOTO")
                {
                    token = scanner.nextToken();
                    int targetLine = stringToInteger(token);
                    GotoState *gotoState = new GotoState(targetLine);
                    program.setParsedStatement(lineNumber,gotoState);
                    return;
                }
                //todo 10 + IF + Exp cmp Exp + THEN + 20
                if (token == "IF")
                {
                    Expression *p1, *p2;
                    std::string cmp;
                    int lineGoto;
                    p1 = readE(scanner);
                    if (p1->getType() == COMPOUND)
                    {
                        if (reinterpret_cast<CompoundExp *>(p1)->getOp() == "=")
                        {
                            scanner.nextToken();
                            lineGoto = stringToInteger(scanner.nextToken());
                            IfState *ifState = new IfState(reinterpret_cast<CompoundExp *>(p1)->getLHS(),reinterpret_cast<CompoundExp *>(p1)->getRHS(),"=",lineGoto);
                            program.setParsedStatement(lineNumber, ifState);
                            return;
                        }
                    }
                    cmp = scanner.nextToken();
                    p2 = readE(scanner);
                    scanner.nextToken();
                    lineGoto = stringToInteger(scanner.nextToken());
                    IfState *ifState = new IfState(p1,p2,cmp,lineGoto);
                    program.setParsedStatement(lineNumber,ifState);
                    return;
                }
            }
            //todo 10
            else
            {
                program.removeSourceLine(lineNumber);
                return;
            }
            cout << "SYNTAX ERROR.\n"; return;
        }
        //todo 直接执行的语句
        if (scanner.getTokenType(token) == WORD)
        {
            //todo LET + CompoundExp
            if (token == "LET")
            {
                scanner.ignoreWhitespace();
                LetState let(parseExp(scanner));
                let.execute(state);
                return;
            }
            //todo PRINT + Expression
            if (token == "PRINT")
            {
                scanner.ignoreWhitespace();
                PrintState print(parseExp(scanner));
                print.execute(state);
                return;
            }
            //todo INPUT + IdentifierExp
            if (token == "INPUT")
            {
                scanner.ignoreWhitespace();
                InputState input(parseExp(scanner));
                input.execute(state);
                return;
            }
            //todo RUN
            if (token == "RUN")
            {
                program.runProgram(state);
                return;
            }
            //todo LIST
            if (token == "LIST")
            {
                program.listSourceCode();
                return;
            }
            //todo CLEAR
            if (token == "CLEAR")
            {
                program.clear();
                state.clearEvalState();
                return;
            }
            //todo QUIT
            if (token == "QUIT")
            {
                exit(0);
            }
            //todo HELP
            if (token == "HELP")
            {
                cout << "Please don't ask for any help.\n";
                return;
            }
            cout << "SYNTAX ERROR.\n"; return;
        }
    }
}