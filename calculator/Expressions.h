#ifndef NUMBER_H
#define	NUMBER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <stdexcept>
#include <vector>
#include <string>
#include <map>
#include <typeinfo>
#include <algorithm>

using namespace std;

class Expressions
{
public:
    virtual Expressions *addition(Expressions *) = 0;
    virtual Expressions *subtraction(Expressions *) = 0;
    virtual Expressions *multiplication(Expressions *) = 0;
    virtual Expressions *division(Expressions *) = 0;
    virtual Expressions *exponentiation(Expressions *) = 0;
    virtual Expressions *nthRoot(Expressions *)=0;

    // Returns a map of Expressions pointer properties
    virtual map<string, Expressions *> getValues() = 0;
    virtual void setValues(string key, Expressions * val) = 0;
    virtual void simplify() = 0;
    virtual void display() = 0;
    virtual double toDouble() = 0;
    virtual string toString() = 0;
    
    // Used only for Integers
    virtual long getValue() = 0;
    virtual void setValue(long value) = 0;
    
    // Used only for Transcendentals
    virtual string getTranscendentalValue() = 0;
    virtual void setTranscendentalValue(string value) = 0;

};

#endif	/* NUMBER_H */

