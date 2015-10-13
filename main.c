#include "cradle.h"
#include <stdio.h>
#include <stdlib.h>


/*
 *
 * <expression> ::= <term>   | <term> <addop> <expression>
 * <term>       ::= <factor> | <term> <mulop> <factor> 
 * <factor>     ::= <digit>  | '(' <expression> ')'
 * <addop>      ::= '+' | '-'
 * <mulop>      ::= '*' | '/'
 *
 */

void Add();
void Subtract();
void Multiply();
void Divide();
void Factor();
void Term();
void Expression();

//match checks if correct val, then gets next char
void Add()
{
    Match('+');     //Match calls getchar
                    //reports error if not +
    Term();         
    EmitLn("pop ebx");
    EmitLn("add eax, ebx");
}

void Subtract()
{
    Match('-');
    Term();
    EmitLn("pop ebx");
    EmitLn("sub eax, ebx");
    EmitLn("neg eax");         
}

void Multiply()
{
    EmitLn("push eax");
    Match('*');
    Factor();
    EmitLn("pop ebx");
    EmitLn("mul eax, ebx");
}

void Divide()
{
    EmitLn("push eax");
    Match('/');
    Factor();
    EmitLn("pop ebx");
    EmitLn("idiv eax, ebx");
}

void Factor()
{
    if(Look == '(')
    {
        Match('(');
        Expression();
        Match(')');
    }
    else if(IsDigit(Look))
    {
        char emitLn[11] = "mov eax,  \0";
        emitLn[9] = Look;
        EmitLn(emitLn);
        GetChar();
    }
    else
        Expected("Digit");
}

//checks if next 
void Term()
{
    Factor();

    do
    {
        switch(Look) 
        {
            case '*':
                Multiply();
                break;
            case '/':
                Divide();
                break;
            default:
//                Expected("MulOp");
                break;
        }
    } while(IsMulop(Look));
}


//evaluates expression, calls appropriate function
void Expression()
{
    Term();
    do 
    {
        EmitLn("push eax");
        switch(Look) 
        {
            case '+':
                Add();
                break;
            case '-':
                Subtract();
                break;
            default:
                if(Look != 10)
                    Expected("AddOp");
        }
    } while(IsAddop(Look));
}


int main(int argc, char** argv)
{
    Init();
    Expression();
    return 0;
}
