/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"
using namespace std;

Program::Program() {
    // Replace this stub with your own code
}

Program::~Program() {
    // Replace this stub with your own code
    for (auto it = Code.begin(); it != Code.end(); ++it) {
        delete Code[it.operator*()].parsedStatement;
    }
}

void Program::clear() {
    // Replace this stub with your own code
    for (auto it = Code.begin(); it != Code.end() ; ++it) {
        delete Code[it.operator*()].parsedStatement;
    }
    Code.clear();
}

void Program::addSourceLine(int lineNumber, string line) {
    // Replace this stub with your own code
    if (Code.containsKey(lineNumber))
    {
        delete Code[lineNumber].parsedStatement;
        Line tmpLine;
        tmpLine.sourceLine = line; tmpLine.parsedStatement = NULL;
        Code.put(lineNumber,tmpLine);
        return;
    }
    else
    {
        Line tmpLine;
        tmpLine.sourceLine = line; tmpLine.parsedStatement = NULL;
        Code.put(lineNumber,tmpLine);
        return;
    }
}

void Program::removeSourceLine(int lineNumber) {
    // Replace this stub with your own code
    delete Code[lineNumber].parsedStatement;
    Code.remove(lineNumber);
}

string Program::getSourceLine(int lineNumber) {
    // Replace this stub with your own code
    return Code[lineNumber].sourceLine;
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    // Replace this stub with your own code
    Code[lineNumber].parsedStatement = stmt;
}

Statement *Program::getParsedStatement(int lineNumber) {
    // Replace this stub with your own code
    return Code[lineNumber].parsedStatement;
}

int Program::getFirstLineNumber() {
    // Replace this stub with your own code
    auto it = Code.begin();
    return it.operator*();
}

int Program::getNextLineNumber(int lineNumber) {
    // Replace this stub with your own code
    auto it = Code.begin();
    if ((++it) != Code.end())
    {
        return it.operator*();
    }
    else return -1;
}

void Program::listSourceCode() {
    for (auto it = Code.begin(); it != Code.end(); ++it) {
        cout << Code[it.operator*()].sourceLine << endl;
    }
}
