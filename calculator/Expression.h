
#ifndef EXPRESSION_H
#define	EXPRESSION_H

#include "Expressions.h"
#include "Integer.h"
#include "Transcendental.h"
#include "Exponential.h"
#include "Logarithm.h"
#include "Rational.h"

#include <iostream>
#include <cstdlib>
#include <stack>
#include <list>
#include <string>
#include <vector>

using namespace std;

#include "Compute.h"

class Expression : public Expressions {
private:
	vector<string> expression;
	vector<string> operators;

	vector<Expressions *> operands;

	string strExpression;

	bool exponentialPresent;

public:

	Expression(string&);

	vector<string> setExpressionTokens(string&);
	vector<string> getOperators();
	vector<Expressions *> getOperands();

	void infixToRPN(vector<string>& tokens, vector<string>& rpn);
	void addInput(string);
	void simplify();
	void sort();

	double toDouble();
	string toString();

	bool isNumeric(string);
	bool isOperator(string);
	bool isParentheses(string token);

	int comparePrecedence(string op1, string op2);

	Expressions * calculate(Expressions *, Expressions *, string);
	Expressions * assignToClass(string& token);
	Expressions *addition(Expressions *);
    Expressions *subtraction(Expressions *);
    Expressions *multiplication(Expressions *);
   	Expressions *division(Expressions *);
    Expressions *exponentiation(Expressions *);
	Expressions *nthRoot(Expressions *);

	// Used only for other classes
	long getValue();
	void setValue(long value);
	void display();
	string getTranscendentalValue();
	void setTranscendentalValue(string value);
	void setValues(string key, Expressions * val);
	map<string, Expressions *> getValues();
};

#endif	/* EXPRESSION_H */

