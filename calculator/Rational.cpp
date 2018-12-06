#include "Rational.h"

Rational::Rational(Expressions * numerator, Expressions * denominator) {
    this->values["numerator"] = numerator;
    this->values["denominator"] = denominator;
    simplify();
}

Rational::Rational(string expression) {
	int pos = expression.find_first_of("/");
	string numeratorString = expression.substr(0, pos);
	string denominatorString = expression.substr(pos + 1, expression.size());
	Expressions * numerator;
	Expressions * denominator;

	//Checks Expressions type of numerator
	if (numeratorString.find_first_of("+-*/") != -1 &&
		numeratorString.find_first_of("+-*/") != 0) {
		numerator = new Expression(numeratorString);
	}
	else if (numeratorString.find_first_of("^") != -1) {
		numerator = new Exponential(numeratorString);
	}
//	else if (numeratorString.find_first_of("rt") != -1) {
//		numerator = new Exponential(numeratorString);
///	}
	else if (numeratorString.find_first_of("log") != -1) {
		numerator = new Logarithm(numeratorString);
	}
	else if (numeratorString.find_first_of("e") != -1 || numeratorString.find_first_of("pi") != -1) {
		numerator = new Transcendental(numeratorString);
	}
	else {
		numerator = new Integer(numeratorString);
	}

	//Checks Expressions type of denominator
	if (denominatorString.find_first_of("+-*/") != -1) {
		denominator = new Expression(denominatorString);
	}
	else if (denominatorString.find_first_of("^") != -1) {
		denominator = new Exponential(denominatorString);
	}
	else if (denominatorString.find_first_of("log") != -1) {
		denominator = new Logarithm(denominatorString);
	}
	else if (denominatorString.find_first_of("e") != -1 || denominatorString.find_first_of("pi") != -1) {
		denominator = new Transcendental(denominatorString);
	}
	else {
		denominator = new Integer(denominatorString);
	}

	//creates Rational object
	this->values["numerator"] = numerator;
	this->values["denominator"] = denominator;
    simplify();
}
Rational::~Rational() {
    delete values["numerator"];
    delete values["denominator"];
}

map<string, Expressions *> Rational::getValues() {
    return values;
}

void Rational::setValues(string key, Expressions * val) {
    values[key] = val;
}

double Rational::toDouble(){
    return values["numerator"]->toDouble() / values["denominator"]->toDouble();
}

string Rational::toString(){
	stringstream valueStream;
	if(typeid(*values["numerator"]) == typeid(Expression) &&
	   typeid(*values["denominator"]) == typeid(Expression))
	{
			valueStream << "(" <<values["numerator"]->toString() << ")" << "/"
					    << "(" <<values["denominator"]->toString() << ")" ;
	}
	else
	{
		valueStream << values["numerator"]->toString();
		if (values["denominator"]->toString() != "1") {
			valueStream << "/" << values["denominator"]->toString();
		}
	}
	string str = valueStream.str();
	return str;
}

//Operation Methods
Expressions *Rational::addition(Expressions *val){
	Expressions * result;
	if (typeid(*val) == typeid(Integer) || 
		typeid(*val) == typeid(Transcendental) ||
		typeid(*val) == typeid(Expression) ||
		typeid(*val) == typeid(Logarithm) ||
		typeid(*val) == typeid(Exponential))
	{
        Expressions * numerator; Expressions * denominator; Expressions * tempValNumerator;

			tempValNumerator = val->multiplication(this->values["denominator"]);
			numerator = this->values["numerator"]->addition(tempValNumerator);
			denominator = this->values["denominator"];

			result = new Rational(numerator, denominator);
	}
	else
	{
		Expressions * numerator; Expressions * denominator;

			if (!this->values["denominator"]->toString().compare(val->getValues()["denominator"]->toString()))
			{
				 numerator = this->values["numerator"]->addition(val->getValues()["numerator"]);
				 denominator = this->values["denominator"];
			}
			else
			{
				Expressions * tempNumerator1 = this->values["numerator"]->multiplication(
						val->getValues()["denominator"]);
				Expressions * tempNumerator2 = this->values["denominator"]->multiplication(
						val->getValues()["numerator"]);
				this->values["numerator"]->division(val->getValues()["denominator"]);
				this->values["denominator"]->division(val->getValues()["numerator"]);
				numerator = tempNumerator1->addition(tempNumerator2);
				denominator = this->values["denominator"]->multiplication(val->getValues()["denominator"]);
			}
			result = new Rational(numerator, denominator);
	}
	result->simplify();
	//Check if the denominator is 1. If so, simply return the numerator.
	if (typeid(*result->getValues()["denominator"]) == typeid(Integer) && result->getValues()["denominator"]->getValue() == 1){
			result = result->getValues()["numerator"];
	}
	return result;
}
Expressions *Rational::subtraction(Expressions *val){
	Expressions * result;
	if (typeid(*val) == typeid(Integer) ||
		typeid(*val) == typeid(Transcendental) ||
		typeid(*val) == typeid(Expression) ||
		typeid(*val) == typeid(Logarithm) ||
		typeid(*val) == typeid(Exponential))
	{
		Expressions * numerator; Expressions * denominator; Expressions * tempValNumerator;

		tempValNumerator = val->multiplication(this->values["denominator"]);
		numerator = this->values["numerator"]->subtraction(tempValNumerator);
		denominator = this->values["denominator"];

		result = new Rational(numerator, denominator);
	}
	else
	{
		Expressions * numerator; Expressions * denominator;

		if (!this->values["denominator"]->toString().compare(val->getValues()["denominator"]->toString()))
		{
			 numerator = this->values["numerator"]->subtraction(val->getValues()["numerator"]);
			 denominator = this->values["denominator"];
		}
		else
		{
			Expressions * tempNumerator1 = this->values["numerator"]->multiplication(val->getValues()["denominator"]);
			Expressions * tempNumerator2 = this->values["denominator"]->multiplication(val->getValues()["numerator"]);
			this->values["numerator"]->division(val->getValues()["denominator"]);
			this->values["denominator"]->division(val->getValues()["numerator"]);
			numerator = tempNumerator1->subtraction(tempNumerator2);
			denominator = this->values["denominator"]->multiplication(val->getValues()["denominator"]);
		}
		result = new Rational(numerator, denominator);
	}
	result->simplify();
	//Check if the denominator is 1. If so, simply return the numerator.
	if (typeid(*result->getValues()["denominator"]) == typeid(Integer) && result->getValues()["denominator"]->getValue() == 1){
			result = result->getValues()["numerator"];
	}
	return result;
}
Expressions *Rational::multiplication(Expressions *val){
	Expressions * result;
	Expressions * numerator1;
	Expressions * denominator1;
	if (typeid(*val) == typeid(Integer) ||
		typeid(*val) == typeid(Transcendental) ||
		typeid(*val) == typeid(Expression) ||
		typeid(*val) == typeid(Logarithm) ||
		typeid(*val) == typeid(Exponential))
	{

		numerator1 = values["numerator"]->multiplication(val);
		denominator1 = values["denominator"];

		result = new Rational(numerator1, denominator1);
	}
	else
	{
		Expressions * numerator1;
		Expressions * denominator1;

		numerator1 = values["numerator"]->multiplication(val->getValues()["numerator"]);
		denominator1 = values["denominator"]->multiplication(val->getValues()["denominator"]);

		result = new Rational(numerator1, denominator1);
	}
	result->simplify();
	//Check if the denominator is 1. If so, simply return the numerator.
	if (typeid(*result->getValues()["denominator"]) == typeid(Integer) && result->getValues()["denominator"]->getValue() == 1){
			result = result->getValues()["numerator"];
	}
	return result;
}
Expressions *Rational::division(Expressions *val){
	Expressions * result;
	if (typeid(*val) == typeid(Integer) ||
		typeid(*val) == typeid(Transcendental) ||
		typeid(*val) == typeid(Expression) ||
		typeid(*val) == typeid(Logarithm) ||
		typeid(*val) == typeid(Exponential))
	{
		Expressions * tempNumerator = new Integer(1);
		Expressions * tempRational = new Rational(tempNumerator, val);
		result = this->multiplication(tempRational);
	}
	else
	{
		Expressions * tempNumerator = val->getValues()["denominator"];
		Expressions * tempDenominator = val->getValues()["numerator"];

		Expressions * tempRational = new Rational(tempNumerator, tempDenominator);
		result = this->multiplication(tempRational);
	}
	//Check for the denominator being 1 is unnecessary here, as multiply already handles that.
	return result;
}
Expressions *Rational::exponentiation(Expressions *val){
    Expressions * numerator = values["numerator"]->exponentiation(val);
    Expressions * denominator = values["denominator"]->exponentiation(val);
    return new Rational(numerator, denominator);
}
Expressions *Rational::nthRoot(Expressions *val){
	Expressions * numerator = values["numerator"]->nthRoot(val);
	Expressions * denominator = values["denominator"]->nthRoot(val);
	return new Rational(numerator, denominator);
}

// Simplify helper method.
void Rational::simplify() {
	if(typeid(*values["denominator"]) != typeid(Expression) && typeid(*values["numerator"]) != typeid(Expression))
	{
		// If the denominator is an Integer and the numerator is an Integer.
		if (typeid(*values["denominator"]) == typeid(Integer) && typeid(*values["numerator"]) == typeid(Integer)) {
			// Find the the common factor via Euclid's method.
			int gcd = findGCD(values["denominator"]->getValue(), values["numerator"]->getValue());
			// Divide the values by that factor to simplify.
			if(gcd < 0)
				gcd *= -1;
			values["denominator"]->setValue(values["denominator"]->getValue() / gcd);
			values["numerator"]->setValue(values["numerator"]->getValue() / gcd);
		}
		// If the denominator is an Integer and the numerator is not.
		else if (typeid(*values["denominator"]) == typeid(Integer) &&typeid(*values["numerator"]) != typeid(Integer)) {
			// And the numerator's coefficient is an Integer
			if (typeid(*values["numerator"]->getValues()["coefficient"]) == typeid(Integer)) {
				int gcd = findGCD(values["denominator"]->getValue(),
								  values["numerator"]->getValues()["coefficient"]->getValue());
				values["denominator"]->setValue(values["denominator"]->getValue() / gcd);
				values["numerator"]->getValues()["coefficient"]->setValue(values["numerator"]->getValues()["coefficient"]->getValue() / gcd);
			}
		}
		// If the denominator is not and Integer and the numerator is.
		else if (typeid(*values["denominator"]) != typeid(Integer) &&
				 typeid(*values["numerator"]) == typeid(Integer)) {
			// And the denominator's coefficient is an Integer.
			if (typeid(*values["denominator"]->getValues()["coefficient"]) == typeid(Integer)) {
				int gcd = findGCD(values["numerator"]->getValue(),
								  values["denominator"]->getValues()["coefficient"]->getValue());
				values["numerator"]->setValue(values["numerator"]->getValue() / gcd);
				values["denominator"]->getValues()["coefficient"]->setValue(values["denominator"]->getValues()["coefficient"]->getValue() / gcd);
			}
		}
		// If the numerator and denominator are both not Integers.
		else if (typeid(*values["denominator"]) != typeid(Integer) &&
				 typeid(*values["numerator"]) != typeid(Integer)) {
			// And both coeffcients are are Integers.
			if (typeid(*values["denominator"]->getValues()["coefficient"]) == typeid(Integer) &&
				typeid(*values["numerator"]->getValues()["coefficient"]) == typeid(Integer)) {
				int gcd = findGCD(values["denominator"]->getValues()["coefficient"]->getValue(),
								  values["numerator"]->getValues()["coefficient"]->getValue());
				values["denominator"]->getValues()["coefficient"]->setValue(values["denominator"]->getValues()["coefficient"]->getValue() / gcd);
				values["numerator"]->getValues()["coefficient"]->setValue(values["numerator"]->getValues()["coefficient"]->getValue() / gcd);
			}
		}
	}
    if (typeid(*values["denominator"]) == typeid(Transcendental) &&
        typeid(*values["numerator"]) == typeid(Transcendental)) {
        if (values["denominator"]->getTranscendentalValue() == 
            values["numerator"]->getTranscendentalValue()) {
            values["numerator"] = values["numerator"]->getValues()["coefficient"];
            values["denominator"] = values["denominator"]->getValues()["coefficient"];
        }
    }
    // If both the denominator and numerator are exponentials,
    else if (typeid(*values["denominator"]) == typeid(Exponential) &&
             typeid(*values["numerator"]) == typeid(Exponential)) {
        // If the both the denominator's and numerator's exponential's contain a value that is in fact an integer,
        if (typeid(*values["denominator"]->getValues()["value"]) == typeid(Integer) &&
            typeid(*values["numerator"]->getValues()["value"]) == typeid(Integer)) {
            // And the values are equal,
            if (values["denominator"]->getValues()["value"]->getValue() == 
                values["numerator"]->getValues()["value"]->getValue()) {
                // And the exponents of each are both RationalNumbers,
                if (typeid(*values["denominator"]->getValues()["exponent"]) == typeid(Rational) &&
                    typeid(*values["numerator"]->getValues()["exponent"]) == typeid(Rational)) {
                    // of which the denominator and numerator are Integers
                    if (typeid(*values["denominator"]->getValues()["exponent"]->getValues()["denominator"]) == typeid(Integer) &&
                        typeid(*values["denominator"]->getValues()["exponent"]->getValues()["numerator"]) == typeid(Integer) &&
                        typeid(*values["numerator"]->getValues()["exponent"]->getValues()["denominator"]) == typeid(Integer) &&
                        typeid(*values["numerator"]->getValues()["exponent"]->getValues()["numerator"]) == typeid(Integer)) {
                        // And the values of the denominator's of the exponents are equal,
                        if (values["numerator"]->getValues()["exponent"]->getValues()["denominator"]->getValue() == 
                            values["denominator"]->getValues()["exponent"]->getValues()["denominator"]->getValue()) {
                            // Set the denominator and numerator equal to only the coefficients.
                            values["numerator"] = values["numerator"]->getValues()["coefficient"];
                            values["denominator"] = values["denominator"]->getValues()["coefficient"];
                        }
                    }
                }
                // Or if the exponents of each are TranscendentalNumbers,
                else if (typeid(*values["denominator"]->getValues()["exponent"]) == typeid(Transcendental) &&
                         typeid(*values["numerator"]->getValues()["exponent"]) == typeid(Transcendental)) {
                    // And the exponents are equal.
                    if (values["denominator"]->getValues()["exponent"]->getTranscendentalValue() ==
                        values["numerator"]->getValues()["exponent"]->getTranscendentalValue()) {
                        // Set the denominator and numerator equal to only the coefficients.
                        values["numerator"] = values["numerator"]->getValues()["coefficient"];
                        values["denominator"] = values["denominator"]->getValues()["coefficient"];
                    }
                }
                // Or if the exponents
            }
        }
        // The denominator's and numerator's values are both trascendental,
        else if (typeid(*values["denominator"]->getValues()["value"]) == typeid(Integer) &&
                 typeid(*values["numerator"]->getValues()["value"]) == typeid(Integer)) {
            cout << "Get here" <<endl;
            // The values of which are equal,
            if (values["denominator"]->getValues()["value"]->getTranscendentalValue() == 
                values["numerator"]->getValues()["value"]->getTranscendentalValue()) {
                // And the exponents of each are both RationalNumbers,
                if (typeid(*values["denominator"]->getValues()["exponent"]) == typeid(Rational) &&
                    typeid(*values["numerator"]->getValues()["exponent"]) == typeid(Rational)) {
                    // of which the denominator and numerator are Integers
                    if (typeid(*values["denominator"]->getValues()["exponent"]->getValues()["denominator"]) == typeid(Integer) &&
                        typeid(*values["denominator"]->getValues()["exponent"]->getValues()["numerator"]) == typeid(Integer) &&
                        typeid(*values["numerator"]->getValues()["exponent"]->getValues()["denominator"]) == typeid(Integer) &&
                        typeid(*values["numerator"]->getValues()["exponent"]->getValues()["numerator"]) == typeid(Integer)) {
                        // And the values of the denominator's of the exponents are equal,
                        if (values["numerator"]->getValues()["exponent"]->getValues()["denominator"]->getValue() == 
                            values["denominator"]->getValues()["exponent"]->getValues()["denominator"]->getValue()) {
                            // Set the denominator and numerator equal to only the coefficients.
                            values["numerator"] = values["numerator"]->getValues()["coefficient"];
                            values["denominator"] = values["denominator"]->getValues()["coefficient"];
                        }
                    }
                }
                // Or if the exponents of each are TranscendentalNumbers,
                else if (typeid(*values["denominator"]->getValues()["exponent"]) == typeid(Transcendental) &&
                         typeid(*values["numerator"]->getValues()["exponent"]) == typeid(Transcendental)) {
                    // And the exponents are equal.
                    if (values["denominator"]->getValues()["exponent"]->getTranscendentalValue() ==
                        values["numerator"]->getValues()["exponent"]->getTranscendentalValue()) {
                        // Set the denominator and numerator equal to only the coefficients.
                        values["numerator"] = values["numerator"]->getValues()["coefficient"];
                        values["denominator"] = values["denominator"]->getValues()["coefficient"];
                    }
                }
            }
        }
    }
    // If the denominator and numerator are Logarithms,
    else if (typeid(*values["denominator"]) == typeid(Logarithm) &&
             typeid(*values["numerator"]) == typeid(Logarithm)) {
        // And the values inside the logs are Integers,
        if (typeid(*values["denominator"]->getValues()["value"]) == typeid(Integer) &&
            typeid(*values["numerator"]->getValues()["value"]) == typeid(Integer)) {
            // And those values are equal,
            if (values["denominator"]->getValues()["value"]->getValue() == 
                values["numerator"]->getValues()["value"]->getValue()) {
                // And the bases are Integers,
                if (typeid(*values["denominator"]->getValues()["base"]) == typeid(Integer) &&
                    typeid(*values["numerator"]->getValues()["base"]) == typeid(Integer)) {
                    // And the bases are equal.
                    if (values["denominator"]->getValues()["base"]->getValue() == 
                        values["numerator"]->getValues()["base"]->getValue()) {
                        // Set the denominator and numerator equal to only the coefficients.
                        values["numerator"] = values["numerator"]->getValues()["coefficient"];
                        values["denominator"] = values["denominator"]->getValues()["coefficient"];
                    }
                }
                // If the bases are Transcendentals
                if (typeid(*values["denominator"]->getValues()["base"]) == typeid(Integer) &&
                    typeid(*values["numerator"]->getValues()["base"]) == typeid(Integer)) {
                    // And the bases are equal,
                    if (values["denominator"]->getValues()["base"]->getTranscendentalValue() == 
                        values["numerator"]->getValues()["base"]->getTranscendentalValue()) {
                        // And the coefficients in front of them are both Integers,
                        if (typeid(*values["denominator"]->getValues()["base"]->getValues()["coefficient"]) ==
                            typeid(Integer) &&
                            typeid(*values["numerator"]->getValues()["base"]->getValues()["coefficient"]) ==
                            typeid(Integer)) {
                            // That are equal,
                            if (values["denominator"]->getValues()["base"]->getValues()["coefficient"]->getValue() ==
                                values["denominator"]->getValues()["base"]->getValues()["coefficient"]->getValue()) {
                                // Set the denominator and numerator equal to only the coefficients.
                                values["numerator"] = values["numerator"]->getValues()["coefficient"];
                                values["denominator"] = values["denominator"]->getValues()["coefficient"];
                            }
                        }
                    }
                }
            }
        }
        // If the values inside the logs are transcendental
        else if (typeid(*values["denominator"]->getValues()["value"]) == typeid(Transcendental) &&
                 typeid(*values["numerator"]->getValues()["value"]) == typeid(Transcendental)) {
            // And those values are equal,
            if (values["denominator"]->getValues()["value"]->getTranscendentalValue() == 
                values["numerator"]->getValues()["value"]->getTranscendentalValue()) {
                // and the coefficients of those transcendental numbers are integers
                if (typeid(*values["denominator"]->getValues()["value"]->getValues()["coefficient"]) ==
                    typeid(Integer) &&
                    typeid(*values["numerator"]->getValues()["value"]->getValues()["coefficient"]) ==
                    typeid(Integer)) {
                    if (values["denominator"]->getValues()["value"]->getValues()["coefficient"]->getValue() ==
                        values["denominator"]->getValues()["value"]->getValues()["coefficient"]->getValue()) {
                        // And the bases are Integers,
                        if (typeid(*values["denominator"]->getValues()["base"]) == typeid(Integer) &&
                            typeid(*values["numerator"]->getValues()["base"]) == typeid(Integer)) {
                            // And the bases are equal.
                            if (values["denominator"]->getValues()["base"]->getValue() == 
                                values["numerator"]->getValues()["base"]->getValue()) {
                                // Set the denominator and numerator equal to only thise coefficients.
                                values["numerator"] = values["numerator"]->getValues()["coefficient"];
                                values["denominator"] = values["denominator"]->getValues()["coefficient"];
                            }
                        }
                        // If the bases are Transcendentals
                        if (typeid(*values["denominator"]->getValues()["base"]) == typeid(Integer) &&
                            typeid(*values["numerator"]->getValues()["base"]) == typeid(Integer)) {
                            // And the bases are equal,
                            if (values["denominator"]->getValues()["base"]->getTranscendentalValue() == 
                                values["numerator"]->getValues()["base"]->getTranscendentalValue()) {
                                // And the coefficients in front of them are both Integers,
                                if (typeid(*values["denominator"]->getValues()["base"]->getValues()["coefficient"]) ==
                                    typeid(Integer) &&
                                    typeid(*values["numerator"]->getValues()["base"]->getValues()["coefficient"]) ==
                                    typeid(Integer)) {
                                    // That are equal,
                                    if (values["denominator"]->getValues()["base"]->getValues()["coefficient"]->getValue() ==
                                        values["denominator"]->getValues()["base"]->getValues()["coefficient"]->getValue()) {
                                        // Set the denominator and numerator equal to only the coefficients.
                                        values["numerator"] = values["numerator"]->getValues()["coefficient"];
                                        values["denominator"] = values["denominator"]->getValues()["coefficient"];
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    //If the top is negative and the bottom is negative, multiply them by -1 to get a positive result.
    if(((typeid(*values["denominator"]) == typeid(Integer) && values["denominator"]->getValue() < 0) ||
    	(typeid(*values["denominator"]) == typeid(Transcendental) &&
    	 typeid(values["denominator"]->getValues()["coefficient"]) == typeid(Integer) &&
    	 values["denominator"]->getValues()["coefficient"]->getValue() < 0)) &&
    	((typeid(*values["numerator"]) == typeid(Integer) && values["numerator"]->getValue() < 0) ||
    	(typeid(*values["numerator"]) == typeid(Transcendental) &&
    	 typeid(values["denominator"]->getValues()["coefficient"]) == typeid(Integer) &&
    	 values["numerator"]->getValues()["coefficient"]->getValue() < 0))){
    		values["denominator"] = values["denominator"]->multiplication(new Integer("-1"));
    		values["numerator"] = values["numerator"]->multiplication(new Integer("-1"));
    }
}
    
// Use euclid's algorithm
int Rational::findGCD(int a, int b) {
    if (b == 0) {
        return a;
    } else {
        return findGCD(b, a % b);
    }
}

string Rational::getTranscendentalValue() {
    return "";
}

void Rational::setTranscendentalValue(string value) {
    return;
}

long Rational::getValue() {
    return 1;
}

void Rational::setValue(long value) {
    return; 
}

void Rational::display()
{

}
