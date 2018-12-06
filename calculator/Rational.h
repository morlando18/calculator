#ifndef RATIONALNUMBER_H
#define	RATIONALNUMBER_H

#include "Expressions.h"
#include "Integer.h"
#include "Exponential.h"
#include "Expression.h"
#include "Transcendental.h"
#include "Logarithm.h"

class Rational : public Expressions {
private:
    map<string, Expressions *> values;

public:
    Rational(Expressions * numerator, Expressions * denominator);
	Rational(string expression);
    ~Rational();
    void simplify();
    void display();
    double toDouble();
    string toString();
    map<string, Expressions *> getValues();
    void setValues(string key, Expressions * val);
    
    //Operation Methods
    Expressions *addition(Expressions *);
    Expressions *subtraction(Expressions *);
    Expressions *multiplication(Expressions *);
    Expressions *division(Expressions *);
    Expressions *exponentiation(Expressions *);
    Expressions *nthRoot(Expressions *);

    // Only used in Rational
    int findGCD(int a, int b);
    
    // Used to satisfy Expressions.h
    long getValue();
    void setValue(long value);
    string getTranscendentalValue();
    void setTranscendentalValue(string value); 

};

#endif	/* RATIONALNUMBER_H */
