#ifndef TRANSCENDENTALNUMBER_H
#define	TRANSCENDENTALNUMBER_H

#include "Expressions.h"
//#include "Exponential.h"
#include "Expression.h"
#include "Integer.h"
//#include "Logarithm.h"
//#include "Rational.h"

class Transcendental : public Expressions {
private:
	string value;
	map<string, Expressions *> values;

public:

	Transcendental(string value, Expressions * coefficient);
	Transcendental(string value);
	~Transcendental();
	void simplify();
	void display();
	double toDouble();
	string toString();

    //Operation Methods
	Expressions *addition(Expressions *val);
    Expressions *subtraction(Expressions *val);
    Expressions *multiplication(Expressions *val);
    Expressions *division(Expressions *val);
    Expressions *exponentiation(Expressions *val);
	Expressions *nthRoot(Expressions *val);

	map<string, Expressions *> getValues();
	void setValues(string key, Expressions * val);
	string getTranscendentalValue();
	void setTranscendentalValue(string value);
	Expressions * getCoefficient();
	void setCoefficient(Expressions * coefficient);

    // Used to satisfy Expressions.h
    long getValue();
    void setValue(long value);


};

#endif	/* TRANSCENDENTALNUMBER_H */

