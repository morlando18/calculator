
#ifndef LOGARITHM_H
#define	LOGARITHM_H
#include "Expressions.h"
#include "Rational.h"
#include "Integer.h"
#include "Exponential.h"
#include "Transcendental.h"

class Logarithm: public Expressions
{
private:
	map<string, Expressions *> values;
    // char smoothOperator;
public:
    Logarithm(Expressions * coefficient, Expressions * value, Expressions * base);
    Logarithm(string input);
    ~Logarithm();

    void simplify();
    void splitLog(vector<long> primes);
    void display();
    double toDouble();
    string toString();
    
    map<string, Expressions *> getValues();
    void setValues(string key, Expressions * val);
    int logBaseN(long value, long n, int counter);
    vector<long> findPrimeFactors(long number, long i, vector<long> primeFactors);

    // Operation methods
    Expressions *addition(Expressions *);
    Expressions *subtraction(Expressions *);
    Expressions *multiplication(Expressions *);
    Expressions *division(Expressions *);
    Expressions *exponentiation(Expressions *);
    Expressions *nthRoot(Expressions *);

    // Used to satisfy Expressions.h
    long getValue();
    void setValue(long value);
    string getTranscendentalValue();
    void setTranscendentalValue(string value);
};


#endif	/* LOGARITHM_H */

