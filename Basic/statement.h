/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface specified in Chapter 17, which is an
 * excellent model for the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include "evalstate.h"
#include "exp.h"

class EndException{};

class GotoException{
private:
    int lineNumber;
public:
    GotoException(int LN):lineNumber(LN){}
    ~GotoException() = default;
    int getLineNumber(){return lineNumber;}
};

/*
 * Type: StatementType
 * -------------------
 * This enumerated type is used to differentiate the different
 * expression types: REM, LET, PRINT, INPUT, END, GOTO, IF, RUN, LIST, CLEAR, QUIT, HELP.
 */

enum StatementType { REM, LET, PRINT, INPUT, END, GOTO, IF, RUN, LIST, CLEAR, QUIT, HELP };

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

   Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

   virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

   virtual void execute(EvalState & state) = 0;

//   virtual StatementType getType() = 0;

};

/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */

class RemState: public Statement{
public:
    RemState() = default;
    ~RemState() = default;
    void execute(EvalState &state) override{};
};

class LetState: public Statement{
private:
    Expression *pExp;
public:
    LetState(Expression *obj);
    LetState(const LetState &other);
    ~LetState();
    void execute(EvalState &state) override;
};

class PrintState: public Statement{
private:
    Expression *pExp;
public:
    PrintState(Expression *obj);
    ~PrintState();
    void execute(EvalState &state) override;
};

class InputState: public Statement{
private:
    Expression *pExp;
public:
    InputState(Expression * obj);
    ~InputState();
    void execute(EvalState &state) override;
};

class EndState: public Statement{
public:
    EndState() = default;
    ~EndState() = default;
    void execute(EvalState &state) override;
};

class GotoState: public Statement{
private:
    int lineNumber;
public:
    GotoState(int l);
    ~GotoState() = default;
    void execute(EvalState &state) override;
};

class IfState: public Statement{
private:
    Expression *pExp1, *pExp2;
    std::string op;
    int lineNumber;
public:
    IfState(Expression *p1, Expression *p2, std::string op, int l);
    ~IfState();
    void execute(EvalState &state) override;
};

#endif
