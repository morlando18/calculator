
#ifndef EXPONENTIAL_H
#define	EXPONENTIAL_H

#include "Expressions.h"
#include "Rational.h"
#include "Integer.h"
#include "Expression.h"
#include "Transcendental.h"
#include "Logarithm.h"

using namespace std;

class Exponential : public Expressions {
private:
    map<string, Expressions *> values;

public:
	Exponential(Expressions * value, Expressions * exponent, Expressions * coefficient);
    Exponential(string expression);
	~Exponential();
    void simplify();
    void display();
    double toDouble();
    string toString();
    map<string, Expressions *> getValues();
    void setValues(string key, Expressions * val);

    // Only used in exponent
    vector<int> findPrimeFactors(int number, int i, vector<int> primeFactors);
    void reduceInsideRoot(int &value, int &coefficient, int root, vector<int> primeFactors);
    
    // Used to satisfy Expressions.h
    long getValue();
    void setValue(long value);
    string getTranscendentalValue();
    void setTranscendentalValue(string value);
    
    // Operation methods
    Expressions *addition(Expressions *);
    Expressions *subtraction(Expressions *);
    Expressions *multiplication(Expressions *);
    Expressions *division(Expressions *);
    Expressions *exponentiation(Expressions *);
    Expressions *nthRoot(Expressions *);

};
    
#endif	/* EXPONENTIAL_H */
    
