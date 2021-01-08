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
using namespace std;

/* Implementation of the Statement class */

Statement::Statement() {
   /* Empty */
}

Statement::~Statement() {
   /* Empty */
}

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

void LetState::execute(EvalState &state) {
    string var = reinterpret_cast<CompoundExp *>(pExp)->getLHS()->toString();
    int value = reinterpret_cast<CompoundExp*>(pExp)->getRHS()->eval(state);
    state.setValue(var,value);
}

///PRINT-----------------------------------------------------------
PrintState::PrintState(Expression *obj) { pExp = obj; }

void PrintState::execute(EvalState &state) { cout << pExp->eval(state) << endl; }

///INPUT-----------------------------------------------------------
InputState::InputState(Expression *obj) {
    if (obj->getType() == IDENTIFIER){ pExp = obj; }
    else error("InputState constructor error.");
}

void InputState::execute(EvalState &state) {
    int value; cin >> value;
    state.setValue(pExp->toString(),value);
}

///END-------------------------------------------------------------
void EndState::execute(EvalState &state) {
    error("this program is ended.");
}

///GOTO------------------------------------------------------------
GotoState::GotoState(int l):lineNumber(l){}

void GotoState::execute(EvalState &state) {
    //todo
}

///IF--------------------------------------------------------------
IfState::IfState(Expression *p1, Expression *p2, std::string op, int l)
        :pExp1(p1),pExp2(p2),lineNumber(l)
{
    if (op == "=" || op == "<" || op == ">") this->op = op;
    else error("SYNTAX ERROR");
}

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

///RUN-------------------------------------------------------------
void RunState::execute(EvalState &state) {}

///LIST------------------------------------------------------------
void ListState::execute(EvalState &state) {}

///CLEAR-----------------------------------------------------------
void ClearState::execute(EvalState &state) {
    state.clearEvalState();
}

///QUIT------------------------------------------------------------
void QuitState::execute(EvalState &state) {
    error("This project is finished.");
}

///HELP------------------------------------------------------------
void HelpState::execute(EvalState &state) {
    cout << "Don't ask for help.";
}