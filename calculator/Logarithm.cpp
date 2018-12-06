#include "Logarithm.h"

using namespace std;

Logarithm::Logarithm(Expressions * coefficient, Expressions * value, Expressions * base) {
    this->values["coefficient"] = coefficient;
    this->values["value"] = value;
    this->values["base"] = base;
    this->values["integer"] = new Integer(0);
    simplify();
}

Logarithm::Logarithm(string expression) {
    Expressions * coefficient; Expressions * value; Expressions * base; Expressions * integer;
    int pos = expression.find_first_of("l");
    int pos2 = expression.find_first_of(":");
    int pos3 = expression.find_first_of("_");
    int pos4 = expression.find_first_of("+");
    if (pos4 != -1) {
        if(expression.substr(0, pos4).find_first_of("pe") != -1) {
            integer = new Transcendental(expression.substr(0, pos4));
        } else {
            integer = new Integer(expression.substr(0, pos4));
        }
        if (pos4 + 1 == pos) {
            coefficient = new Integer(1);
        } else if (expression.substr(pos4 + 1, pos).find_first_of("pe") != -1) {
            coefficient = new Transcendental(expression.substr(pos4 + 1, pos));
        } else {
            coefficient = new Integer(expression.substr(pos4 + 1, pos));
        }
        this->values["integer"] = integer;
    } else {
        if (pos == 0) {
            coefficient = new Integer(1);
        } else if (expression.substr(0, pos).find_first_of("pe") != -1) {
            coefficient = new Transcendental(expression.substr(0, pos));
        } else {
            coefficient = new Integer(expression.substr(0, pos));
        }
        this->values["integer"] = new Integer(0);
    }
    // Next deal with the base...
    if (expression.substr(pos3 + 1, pos2 - (pos3 + 1)).find_first_of("pe") != -1) {
        base = new Transcendental(expression.substr(pos3 + 1, pos2 - (pos3 + 1)));
    } else {
        base = new Integer(expression.substr(pos3 + 1, pos2 - (pos3 + 1)));
    }
    if (expression.substr(pos2 + 1, expression.size() - (pos2 + 1)).find_first_of("pe") != -1) {
        value = new Transcendental(expression.substr(pos2 + 1, expression.size() - (pos2 + 1)));
    } else {
        value = new Integer(expression.substr(pos2 + 1, expression.size() - (pos2 + 1)));
    }
    this->values["coefficient"] = coefficient;
    this->values["value"] = value;
    this->values["base"] = base;
    simplify();
}

Logarithm::~Logarithm() {
    delete values["coefficient"];
    delete values["value"];
    delete values["base"];
    delete values["integer"];
}

// Get and Set Methods
map<string, Expressions *> Logarithm::getValues() {
    return values;
}

void Logarithm::setValues(string key, Expressions * val) {
    values[key] = val;
}

double Logarithm::toDouble(){
    // Uses log() from cmath which gives the natural logarithm.
    return values["coefficient"]->toDouble() * (log(values["value"]->toDouble()) / 
                                                log(values["base"]->toDouble())) + values["integer"]->toDouble();
}

// Needs to be changed.
string Logarithm::toString(){
    stringstream valueStream;
    if (typeid(*values["integer"]) == typeid(Integer)) {
        if (values["integer"]->getValue() != 0) {
            valueStream << values["integer"]->toString();
        }
    } else {
        valueStream << values["integer"]->toString();
    }
    if (values["value"]->toString() != "1") {
        if (values["integer"]->getValue() != 0) {
            valueStream << "+";
        }
        if (typeid(*values["coefficient"]) == typeid(Integer)) {
            if (values["coefficient"]->getValue() != 1) {
                valueStream << values["coefficient"]->toString();
            }
        }
        valueStream << "log_" << values["base"]->toString() << ":" << values["value"]->toString(); 
    }
    string str = valueStream.str();
    return str;
}

void Logarithm::simplify() {
    // If the base is an Integer,
    if (typeid(*values["base"]) == typeid(Integer)) {
        // and the last value in logValues is also an Integer,
        if (typeid(*values["value"]) == typeid(Integer)) {
            if (values["base"]->getValue() > values["value"]->getValue()) {
                long multiplier = (long)(logBaseN(values["base"]->getValue(), values["value"]->getValue(), 0));
                 // If the coefficient is an Integer.
                if (multiplier != 0) {
                    if (typeid(*values["coefficient"]) == typeid(Integer)) {
                        long valueBuilder = 1;
                        for (long i = 0; i < multiplier; i++) {
                            valueBuilder *= values["value"]->getValue();
                        }
                        if ((values["base"]->getValue() / valueBuilder) == 1) {
                            values["integer"] = values["coefficient"]->multiplication(
                                    new Rational(new Integer(1), new Integer(multiplier)));
                            values["value"]->setValue(values["base"]->getValue() / valueBuilder);
                        }
                    }
                }
            } else {
                // See what power of base can be pulled out of the value,
                long multiplier = (long)(logBaseN(values["value"]->getValue(), values["base"]->getValue(), 0));
                // If the coefficient is an Integer.
                if (multiplier != 0) {
                    if (typeid(*values["coefficient"]) == typeid(Integer)) {
                        values["integer"]->setValue(multiplier);
                        // Set the value of the Integer to the coefficient times the multiplier.
                        values["integer"]->setValue(values["coefficient"]->getValue() * values["integer"]->getValue());
                        // Set the value equal to the result of the value divided by the base^multiplier,
                        long valueBuilder = 1;
                        for (long i = 0; i < multiplier; i++) {
                            valueBuilder *= values["base"]->getValue();
                        }
                        values["value"]->setValue(values["value"]->getValue() / valueBuilder);
                    }
                }
            }                    
            // Grab list of prime factors for the remaining value
            vector<long> primes; 
            primes = findPrimeFactors(values["value"]->getValue(), 2, primes);
            // Find the prime that occurs the most often.
            if (primes.size() != 0) {
                int current = primes[0];
                int counter = 1;
                int newCounter = 1;
                int max = primes[0];
                for (int i = 1; i < primes.size(); i++) {
                    if (current == primes[i]) {
                        newCounter++;
                    } else if (current != primes[i]) {
                        current = primes[i];
                        newCounter = 1;
                    }
                    if (newCounter > counter) {
                        counter = newCounter;
                        max = current;
                    }
                }
                int check = values["value"]->getValue();
                for (int i = 1; i < counter; i++) {
                    // divide the value counter - 1 times, this leaves the base in the value.
                    check = check / max;
                }
                if (check == max) {
                    values["value"]->setValue(max);
                    // Set the value of the coefficient equal to itself times the counter,
                    values["coefficient"]->setValue(counter * values["coefficient"]->getValue());
                }
            }
        }
    } else if (typeid(*values["base"]) == typeid(Transcendental) &&
               ((typeid(*values["value"]) == typeid(Transcendental) ||
                 (typeid(*values["value"]) == typeid(Exponential) && 
                  typeid(*values["value"]->getValues()["value"]) == typeid(Transcendental))))) {
        if (typeid(*values["base"]) == typeid(Transcendental) &&
            typeid(*values["value"]) == typeid(Transcendental) &&
            !values["base"]->getTranscendentalValue().compare(values["value"]->getTranscendentalValue()) &&
            values["base"]->getValues()["coefficient"]->getValue() == values["value"]->getValues()["coefficient"]->getValue()) {
            values["base"] = new Integer("2");
            values["value"] = new Integer("2");
            this->simplify();
        } else if (typeid(*values["value"]) == typeid(Exponential) && 
                   typeid(*values["base"]) == typeid(Transcendental) &&
                   typeid(*values["value"]->getValues()["exponent"]) == typeid(Integer) &&
                   !values["base"]->getTranscendentalValue().compare(values["value"]->getValues()["value"]->getTranscendentalValue())){
            values["base"] = new Integer("2");
            long power = pow(2, values["value"]->getValues()["exponent"]->getValue());
            values["value"] = new Integer(power);
            this->simplify();
        } else if (typeid(*values["value"]) == typeid(Exponential) && 
                   typeid(*values["base"]) == typeid(Transcendental) &&
                   typeid(*values["value"]->getValues()["exponent"]) == typeid(Transcendental) &&
                   !values["base"]->getTranscendentalValue().compare(values["value"]->getValues()["value"]->getTranscendentalValue())){
            values["base"] = new Integer("2");
            values["integer"] = new Transcendental(values["value"]->getValues()["exponent"]->getTranscendentalValue());
            values["value"] = new Integer("1");
        } else if (typeid(*values["value"]) == typeid(Exponential) && 
                   typeid(*values["base"]) == typeid(Transcendental) &&
                   typeid(*values["value"]->getValues()["exponent"]) == typeid(Rational) &&
                   !values["base"]->getTranscendentalValue().compare(values["value"]->getValues()["value"]->getTranscendentalValue())){
            values["base"] = new Integer("2");
            values["integer"] = new Rational(values["value"]->getValues()["exponent"]->toString());
            values["value"] = new Integer("1");
        }
    }
}

int Logarithm::logBaseN(long value, long n, int counter) {
    if (value % n != 0) {
        return counter;
    } else {
        return logBaseN(value / n, n, ++counter);
    }
}


vector<long> Logarithm::findPrimeFactors(long number, long i, vector<long> primeFactors) {
    if (number < i) {
        return primeFactors;
    } else if (number % i == 0) {
        primeFactors.push_back(i);
        return findPrimeFactors(number / i, 2, primeFactors);
    } else {
        return findPrimeFactors(number, ++i, primeFactors);
    }
}

Expressions * Logarithm::addition(Expressions *val) {
    if (typeid(*val) == typeid(Logarithm)) {
        if (typeid(*values["value"]) == typeid(Integer) &&
            typeid(*val->getValues()["value"]) == typeid(Integer)) {
            if (values["value"]->getValue() == val->getValues()["value"]->getValue()) {
                if (typeid(*values["base"]) == typeid(Integer) &&
                    typeid(*val->getValues()["base"]) == typeid(Integer)) {
                    if (values["base"]->getValue() == val->getValues()["base"]->getValue()) {
                        Expressions * result = new Logarithm(
                                values["coefficient"]->addition(val->getValues()["coefficient"]),
                                                             values["value"], values["base"]);
                        return result;
                    }
                } else if (typeid(*values["base"]) == typeid(Transcendental) &&
                           typeid(*val->getValues()["base"]) == typeid(Transcendental)) {
                    if (values["base"]->getTranscendentalValue() == val->getValues()["base"]->getTranscendentalValue()) {
                        Expressions * result = new Logarithm(
                                values["coefficient"]->addition(val->getValues()["coefficient"]),
                                                             values["value"], values["base"]);
                        return result;
                    }
                }
            }else if (!values["base"]->toString().compare(val->getValues()["base"]->toString()) &&
        			typeid(*values["coefficient"]) == typeid(Integer) &&
        			typeid(*val->getValues()["coefficient"]) == typeid(Integer)) {
            	values["value"] = new Integer(pow(values["value"]->getValue(), values["coefficient"]->getValue()));
            	val->getValues()["value"] = new Integer(pow(val->getValues()["value"]->getValue(), val->getValues()["coefficient"]->getValue()));
            	Expressions * result = new Logarithm(new Integer(1),
                                                     values["value"]->multiplication(val->getValues()["value"]), values["base"]);
            	return result;
            }
        } else if (typeid(*values["value"]) == typeid(Transcendental) &&
                   typeid(*val->getValues()["value"]) == typeid(Transcendental)) {
            if (values["value"]->getTranscendentalValue() == val->getValues()["value"]->getTranscendentalValue()) {
                if (typeid(*values["base"]) == typeid(Integer) &&
                    typeid(*val->getValues()["base"]) == typeid(Integer)) {
                    if (values["base"]->getValue() == val->getValues()["base"]->getValue()) {
                        Expressions * result = new Logarithm(
                                values["coefficient"]->addition(val->getValues()["coefficient"]),
                                                             values["value"], values["base"]);
                        return result;
                    }
                } else if (typeid(*values["base"]) == typeid(Transcendental) &&
                           typeid(*val->getValues()["base"]) == typeid(Transcendental)) {
                    if (values["base"]->getTranscendentalValue() == val->getValues()["base"]->getTranscendentalValue()) {
                        Expressions * result = new Logarithm(
                                values["coefficient"]->addition(val->getValues()["coefficient"]),
                                                             values["value"], values["base"]);
                        return result;
                    }
                }
            }
        }
    }
}

Expressions * Logarithm::subtraction(Expressions *val) {
    if (typeid(*val) == typeid(Logarithm)) {
        if (val->getValues()["integer"]->getValue() == 0 && values["integer"]->getValue() == 0) {
            if (typeid(*values["value"]) == typeid(Integer) &&
                typeid(*val->getValues()["value"]) == typeid(Integer)) {
                if (values["value"]->getValue() == val->getValues()["value"]->getValue()) {
                    if (typeid(*values["base"]) == typeid(Integer) &&
                        typeid(*val->getValues()["base"]) == typeid(Integer)) {
                        if (values["base"]->getValue() == val->getValues()["base"]->getValue()) {
                            Expressions * result = new Logarithm(
                                    values["coefficient"]->subtraction(val->getValues()["coefficient"]),
                                                                 values["value"], values["base"]);
                            return result;
                        }
                    } else if (typeid(*values["base"]) == typeid(Transcendental) &&
                               typeid(*val->getValues()["base"]) == typeid(Transcendental)) {
                        if (values["base"]->getTranscendentalValue() == val->getValues()["base"]->getTranscendentalValue()) {
                            Expressions * result = new Logarithm(
                                    values["coefficient"]->subtraction(val->getValues()["coefficient"]),
                                                                 values["value"], values["base"]);
                            return result;
                        }
                    }
                }
            } else if (typeid(*values["value"]) == typeid(Transcendental) &&
                       typeid(*val->getValues()["value"]) == typeid(Transcendental)) {
                if (values["value"]->getTranscendentalValue() == val->getValues()["value"]->getTranscendentalValue()) {
                    if (typeid(*values["base"]) == typeid(Integer) &&
                        typeid(*val->getValues()["base"]) == typeid(Integer)) {
                        if (values["base"]->getValue() == val->getValues()["base"]->getValue()) {
                            Expressions * result = new Logarithm(
                                    values["coefficient"]->subtraction(val->getValues()["coefficient"]),
                                                                 values["value"], values["base"]);
                            return result;
                        }
                    } else if (typeid(*values["base"]) == typeid(Transcendental) &&
                               typeid(*val->getValues()["base"]) == typeid(Transcendental)) {
                        if (values["base"]->getTranscendentalValue() == val->getValues()["base"]->getTranscendentalValue()) {
                            Expressions * result = new Logarithm(
                                    values["coefficient"]->subtraction(val->getValues()["coefficient"]),
                                                                 values["value"], values["base"]);
                            return result;
                        }
                    }
                }
            }
        }
    }
}

Expressions * Logarithm::multiplication(Expressions *val) {
    if (typeid(*val) == typeid(Logarithm)) {
        // forget foiling atm...
        if (val->getValues()["integer"]->getValue() == 0 && values["integer"]->getValue() == 0) {    
            if (typeid(*values["value"]) == typeid(Integer) &&
                typeid(*val->getValues()["value"]) == typeid(Integer)) {
                if (values["value"]->getValue() == val->getValues()["value"]->getValue()) {
                    if (typeid(*values["base"]) == typeid(Integer) &&
                        typeid(*val->getValues()["base"]) == typeid(Integer)) {
                        if (values["base"]->getValue() == val->getValues()["base"]->getValue()) {
                            Expressions * result = new Logarithm(
                                    values["coefficient"]->multiplication(val->getValues()["coefficient"]),
                                                                 values["value"], values["base"]);
                            return result;
                        }
                    } else if (typeid(*values["base"]) == typeid(Transcendental) &&
                               typeid(*val->getValues()["base"]) == typeid(Transcendental)) {
                        if (values["base"]->getTranscendentalValue() == val->getValues()["base"]->getTranscendentalValue()) {
                            Expressions * result =  new Logarithm(
                                    values["coefficient"]->multiplication(val->getValues()["coefficient"]),
                                                                  values["value"], values["base"]);
                            return result;
                        }
                    }
                }
            } else if (typeid(*values["value"]) == typeid(Transcendental) &&
                       typeid(*val->getValues()["value"]) == typeid(Transcendental)) {
                if (values["value"]->getTranscendentalValue() == val->getValues()["value"]->getTranscendentalValue()) {
                    if (typeid(*values["base"]) == typeid(Integer) &&
                        typeid(*val->getValues()["base"]) == typeid(Integer)) {
                        if (values["base"]->getValue() == val->getValues()["base"]->getValue()) {
                            Expressions * result =  new Logarithm(
                                    values["coefficient"]->multiplication(val->getValues()["coefficient"]),
                                                                  values["value"], values["base"]);
                            return result;
                        }
                    } else if (typeid(*values["base"]) == typeid(Transcendental) &&
                               typeid(*val->getValues()["base"]) == typeid(Transcendental)) {
                        if (values["base"]->getTranscendentalValue() == val->getValues()["base"]->getTranscendentalValue()) {
                            Expressions * result = new Logarithm(
                                    values["coefficient"]->multiplication(val->getValues()["coefficient"]),
                                                                 values["value"], values["base"]);
                            return result;
                        }
                    }
                }
            }
        }
    }
}

Expressions * Logarithm::division(Expressions *val) {
    // We need to make sure we make a generic Expressions pointer to self.
    Expressions * result =  new Rational(this, val);
    if (!result->getValues()["denominator"]->toString().compare("1")){
        result = result->getValues()["numerator"];
    }
    return result;
}

Expressions * Logarithm::exponentiation(Expressions *val) {
	if (typeid(*val) == typeid(Integer) && val->getValue() == 1){
		return this;
	}
	else if (typeid(*val) == typeid(Integer) && val->getValue() == 0){
		return new Integer(1);
	}
	else{
	    stringstream valStream;
	    valStream << toString() << "^" << val->toString();
	    string str = valStream.str();
	    return new Expression(str);
	}
}
Expressions * Logarithm::nthRoot(Expressions *val) {
    if (typeid(*val) == typeid(Integer) && val->getValue() == 1){
        return this;
    }
    else if (typeid(*val) == typeid(Integer) && val->getValue() == 0){
        return new Integer(1);
    }
    else{
        stringstream valStream;
        valStream << toString() << "rt" << val->toString();
        string str = valStream.str();
        return new Expression(str);
    }
}

// Satisfying our love of maps
string Logarithm::getTranscendentalValue() {
    return "";
}

void Logarithm::setTranscendentalValue(string value) {
    return;
}
long Logarithm::getValue() {
    return 1;
}
void Logarithm::setValue(long value) {
    return;
}

void Logarithm::display() {
    return;
}
