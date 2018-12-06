#ifndef CALCULATOR_H
#define	CALCULATOR_H

#include <iostream>
#include <cstdlib>
#include <stack>
#include <list>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>

class Expression;
#include "Integer.h"
#include "Transcendental.h"
#include "Exponential.h"
#include "Logarithm.h"
#include "Rational.h"
#include "Expression.h"

using namespace std;

class Compute
{
private:
	vector<Expressions *> previousAns;
	vector<Expression*> expressionB;
	vector<string> previousAnswers;
	vector<string> previousInputs;
    vector<string> expression;
public:
	//constructors
	Compute();
    Compute(string);
        
    //function prototypes

    vector<string> getPreviousInputs();
    vector<Expressions *> getPreviousA();
    vector<string> setUpExpression(string &);

    Expressions * calculate(Expressions *, Expressions *, string);
    Expressions * assignToClass(string&);

    bool isNumeric(string);
    bool isCorrectExpression(string);
    bool isInfixToRPN(vector<string> &, vector<string> &);
    bool isOperator(string);
    bool isParentheses(string);


    void calculate();

    Expressions * getLatestAnswer();
    
	int comparePrecedence(string, string);
};


#endif	/* CALCULATOR_H */

