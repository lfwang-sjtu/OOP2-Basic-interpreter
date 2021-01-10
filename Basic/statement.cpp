/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
#include "../StanfordCPPLib/simpio.h"
using namespace std;

/* Implementation of the Statement class */

Statement::Statement() {
   /* Empty */
}

Statement::~Statement() {
   /* Empty */
}

///REM-------------------------------------------------------------


///LET-------------------------------------------------------------
LetState::LetState(Expression *obj) {
    if (obj->getType() == COMPOUND){
        if (reinterpret_cast<CompoundExp *>(obj)->getOp() == "=" && reinterpret_cast<CompoundExp *>(obj)->getLHS()->getType() == IDENTIFIER){
            pExp = obj;
            return;
        }
    }
    error("SYNTAX ERROR");
}

LetState::LetState(const LetState &other) {
    if (other.pExp->getType() == CONSTANT) pExp = new ConstantExp(*(reinterpret_cast<ConstantExp *>(other.pExp)));
    if (other.pExp->getType() == IDENTIFIER) pExp = new IdentifierExp(*(reinterpret_cast<IdentifierExp *>(other.pExp)));
    if (other.pExp->getType() == COMPOUND) pExp = new CompoundExp(*(reinterpret_cast<CompoundExp *>(other.pExp)));
}

LetState::~LetState() noexcept { delete pExp; }

void LetState::execute(EvalState &state) {
    string var = reinterpret_cast<CompoundExp *>(pExp)->getLHS()->toString();
    int value = reinterpret_cast<CompoundExp*>(pExp)->getRHS()->eval(state);
    state.setValue(var,value);
}

///PRINT-----------------------------------------------------------
PrintState::PrintState(Expression *obj) { pExp = obj; }

PrintState::~PrintState() noexcept { delete pExp; }

void PrintState::execute(EvalState &state) { cout << pExp->eval(state) << endl; }

///INPUT-----------------------------------------------------------
InputState::InputState(Expression *obj) {
    if (obj->getType() == IDENTIFIER){ pExp = obj;}
    else error("SYNTAX ERROR.");
}

InputState::~InputState() noexcept { delete pExp; }

void InputState::execute(EvalState &state) {
    std::string token;
    int value;

    while (true)
    {
        try {
            token = getLine("?");
            value = stringToInteger(token);
        }
        catch (...) {
            cout << "INVALID NUMBER.\n";
            continue;
        }
        break;
    }

    state.setValue(pExp->toString(),value);
}

///END-------------------------------------------------------------
void EndState::execute(EvalState &state) {
    throw EndException();
}

///GOTO------------------------------------------------------------
GotoState::GotoState(int l):lineNumber(l){}

void GotoState::execute(EvalState &state) {
    throw GotoException(lineNumber);
}

///IF--------------------------------------------------------------
IfState::IfState(Expression *p1, Expression *p2, std::string op, int l)
        :pExp1(p1),pExp2(p2),lineNumber(l)
{
    if (op == "=" || op == "<" || op == ">") this->op = op;
    else error("SYNTAX ERROR");
}

IfState::~IfState() throw() {delete pExp1; delete pExp2;}

void IfState::execute(EvalState &state) {
    if (op == "="){
        if (pExp1->eval(state) == pExp2->eval(state)) { GotoState(lineNumber).execute(state); return; }
        else return;
    }
    if (op == "<"){
        if (pExp1->eval(state) < pExp2->eval(state)) {GotoState(lineNumber).execute(state); return;}
        else return;
    }
    if (op == ">"){
        if (pExp1->eval(state) > pExp2->eval(state)) {GotoState(lineNumber).execute(state); return;}
        else return;
    }
}