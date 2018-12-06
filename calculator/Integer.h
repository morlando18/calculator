#ifndef INTEGER_H
#define	INTEGER_H

#include "Expressions.h"
#include "Rational.h"
#include "Exponential.h"
#include "Transcendental.h"
#include "Logarithm.h"
#include "Expression.h"

using namespace std;

class Integer : public Expressions {
private:
    long value;

public:
    Integer(long);
    Integer(string);
    Integer();
    
    long getValue();
    void setValue(long);
    void display();
    double toDouble();
    string toString();

    // Implemented to satisfy Expressions.h
    map<string, Expressions *> getValues();
    void setValues(string key, Expressions * val);
    string getTranscendentalValue();
    void setTranscendentalValue(string value); 
    void simplify();

    // Abstract operation methods
    Expressions *addition(Expressions *);
    Expressions *subtraction(Expressions *);
    Expressions *multiplication(Expressions *);
    Expressions *division(Expressions *);
    Expressions *exponentiation(Expressions *);
    Expressions *nthRoot(Expressions *);
};

#endif	/* INTEGER_H */

