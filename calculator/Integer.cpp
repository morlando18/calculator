
#include "Integer.h"

// Constructor
Integer::Integer(long value) {
    this->value = value;
}

Integer::Integer(string val){
	const long d2 = strtol( val.c_str(), NULL ,0);
	this->value = d2;
}

Integer::Integer() {
    this->value = 0;
}

// Get-Set methods
long Integer::getValue() {
    return value;
}

void Integer::setValue(long val) {
    this->value = val;
}

void Integer::display() {
    // Implement this.
}

double Integer::toDouble() {
	// get method can also be used here if double value not needed
	return (double) value;
}

string Integer::toString() {
	stringstream valueStream;
	valueStream << value;
	string str = valueStream.str();
	return str;
}
 
// The methods below exist only to satisfy Expressions.h.
map<string, Expressions *> Integer::getValues() {
    //return NULL;
}

void Integer::setValues(string key, Expressions * val) {
}

string Integer::getTranscendentalValue() {
    //return "";
}

void Integer::setTranscendentalValue(string value) {
}

void Integer::simplify() {
}

Expressions * Integer::addition(Expressions *val) {
	if (typeid(*val) == typeid(Integer)) {
		this->value += val->getValue();
		Expressions * result = new Integer(this->toString());
        return result;
	} else if (typeid(*val) == typeid(Rational)) {
		return val->addition(this);
	} else if (typeid(*val) == typeid(Exponential)){
		stringstream valStream;
		valStream << val->toString() << "+" << toString();
		string str = valStream.str();
		return new Expression(str);
	} else {
		stringstream valStream;
		valStream << toString() << "+" << val->toString();
		string str = valStream.str();
		return new Expression(str);
	}
}

Expressions * Integer::subtraction(Expressions *val) {
	if (typeid(*val) == typeid(Integer)) {
        this->value -= val->getValue();
        Expressions * result = new Integer(this->toString());
        return result;
    } else if (typeid(*val) == typeid(Rational)) {
		stringstream RatNumStream;
		RatNumStream << getValue() << "/1";
		string str = RatNumStream.str();
		Expressions * RatNum = new Rational(str);
		return RatNum->subtraction(val);
	} else {
		stringstream valStream;
		valStream << toString() << "-" << val->toString();
		string str = valStream.str();
		return new Expression(str);
	}
}

Expressions * Integer::multiplication(Expressions *val) {
    if (typeid(*val) == typeid(Integer)) {
        this->value *= val->getValue();
        Expressions * result = new Integer(this->toString());
        return result;
    } else if (typeid(*val) == typeid(Rational)) {
		stringstream RatNumStream;
		RatNumStream << getValue() << "/1";
		string str = RatNumStream.str();
		Expressions * RatNum = new Rational(str);
		return RatNum->multiplication(val);
	} else if (typeid(*val) == typeid(Transcendental)) {
        Expressions * coeffs = new Integer(multiplication(val->getValues()["coefficient"])->toString());
    	stringstream valStream;
        valStream << coeffs->toString() << val->getTranscendentalValue();
        string str = valStream.str();
        Expressions * result = new Transcendental(str);
        return result;
    } else if (typeid(*val) == typeid(Exponential)) {
    	return val->multiplication(this);
    } else if (typeid(*val) == typeid(Expression)) {
        return val->multiplication(this);
    } else if (typeid(*val) == typeid(Logarithm)){
    	return new Logarithm(this, val->getValues()["value"], val->getValues()["base"]);
	}else {
		stringstream valStream;
		valStream << toString() << "*" << val->toString();
		string str = valStream.str();
		return new Expression(str);
	}
    // Add cases where this multiplies into coefficients
}

Expressions * Integer::division(Expressions *val) {
    if (typeid(*val) == typeid(Integer)) {
        if (this->value % val->getValue() == 0) {
            this->value /= val->getValue();
            Expressions * result = new Integer(this->toString());
            return result;
        } else {
        	return new Rational(this, val);
        }
    // Add cases where this divides out coefficients
    } else if (typeid(*val) == typeid(Rational)) {
		stringstream RatNumStream;
		RatNumStream << getValue() << "/1";
		string str = RatNumStream.str();
		Expressions * RatNum = new Rational(str);
		return RatNum->division(val);
        /*Expressions* numerator = this;
        Expressions* result = new Rational(this, val);
        return result;*/
    } else {
    	return new Rational(this, val);
    }
}
Expressions * Integer::exponentiation(Expressions *val) {
	if (this->getValue() == 0) {
		return new Integer("0");
	} else {
		if (toString() == "1") {
			return new Integer("1");
		} else {
			stringstream valStream;
			if (typeid(*val) == typeid(Integer)) {
				long valLong = 0;
				valLong = pow(getValue(), val->getValue());
				return new Integer((long)valLong);
			} else if (typeid(*val) == typeid(Rational)) {
				Expressions * intCoefficient = new Integer("1");
				return new Exponential(this, val, intCoefficient);
			} else {
				stringstream valStream;
				valStream << toString() << "^" << val->toString();
				string str = valStream.str();
				return new Exponential(str);
			}
		}
	}
}
Expressions * Integer::nthRoot(Expressions *val) {
	if (this->getValue() == 0) {
		return new Integer("0");
	} else {
		if (toString() == "1") {
			return new Integer("1");
		} else {
			stringstream valStream;
			if (typeid(*val) == typeid(Integer)) {
				long valLong = 0;
				valLong = pow(getValue(), val->getValue());
				return new Integer((long)valLong);
			} else if (typeid(*val) == typeid(Rational)) {
				Expressions * intCoefficient = new Integer("1");
				return new Exponential(this, val, intCoefficient);
			} else {
				stringstream valStream;
				valStream << toString() << "rt" << val->toString();
				string str = valStream.str();
				return new Exponential(str);
			}
		}
	}
}