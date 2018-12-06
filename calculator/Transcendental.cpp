#include "Transcendental.h"

using namespace std;


Transcendental::Transcendental(string expression){
	// find the first e or pi in a string, then treat everything to the right as it's coefficient.
    // for now, just think of e and pi as having Integer coefficients.
    // p for pi, e for well e.
    int pos = expression.find_first_of("pe");
    if(pos == -1) {
		string exception = "ERROR! Enter a valid Transcendental! (e or pi)\n";
		throw exception;
	}
    else if (expression[pos] == 'p') {
        this->value = "pi";
    }
    else if (expression[pos] == 'e') {
        this->value = "e";
    }
    if (pos == 0) {
        values["coefficient"] = new Integer("1");
    }
    else {
        // currently, just row everything to the right in an Integer constructor
        values["coefficient"] = new Integer(expression.substr(0,pos));
    }
}

Transcendental::~Transcendental() {
    delete values["coefficient"];
}

//Get and set methods
map<string, Expressions *> Transcendental::getValues() {
    return values;
}

void Transcendental::setValues(string key, Expressions * val) {
    values[key] = val;
}
string Transcendental::getTranscendentalValue() {
    return value;
}

void Transcendental::setTranscendentalValue(string value) {
    this->value = value;
}

void Transcendental::simplify() {
    // Exists to satisfy Expressions Parent class.
}

void Transcendental::display() {
    // Exists to display Expressions Parent class.
	//Please call getValue() to return pi or e.
}
double Transcendental::toDouble(){
	//Coefficient is currently a number, so I am weary of its implmentation here,
	//as it requires very good operator overloading.
	//If we run out of time, consider changing back to int for simplicity.
	if (!value.compare("pi") || !value.compare("PI")){
		return values["coefficient"]->toDouble()*3.14;
	}
	else if(!value.compare("e")){
		return values["coefficient"]->toDouble()*2.72;
	}
	else{
	    return 0;
	}
}
string Transcendental::toString(){
	stringstream valueStream;
	if(values["coefficient"]->toString() != "1")
		valueStream << values["coefficient"]->toString() << value;

	else
		valueStream  << value;
	string str = valueStream.str();
	return str;
}

//Operation Methods
Expressions *Transcendental::addition(Expressions *val){
	stringstream valStream;
	if (typeid(*val) == typeid(Transcendental) && value.compare(val->getTranscendentalValue()) == 0) {
		Expressions * coeffs = values["coefficient"]->addition(val->getValues()["coefficient"]);
		if(coeffs->toString() != "0")
			valStream << coeffs->toString() << value;
		else
			valStream << coeffs->toString();
		string str = valStream.str();
		return new Transcendental(str);
	}
	else if(!values["coefficient"]->toString().compare("1")){
		valStream << value << "+" << val->toString();
		string str = valStream.str();
		return new Expression(str);
	}
	else{
		valStream << values["coefficient"]->toString() << value << "+" << val->toString();
		string str = valStream.str();
		return new Expression(str);
	}
}
Expressions *Transcendental::subtraction(Expressions *val){
	stringstream valStream;
	if (typeid(*val) == typeid(Transcendental) && value.compare(val->getTranscendentalValue()) == 0) {
		Expressions * coeffs = values["coefficient"]->subtraction(val->getValues()["coefficient"]);
		if(coeffs->toString() != "0"){
			valStream << coeffs->toString() << value;
			string str = valStream.str();
			return new Transcendental(str);
		}
		else{
			valStream << "0";
			string str = valStream.str();
			return new Integer(str);
		}
	}
	else if(!values["coefficient"]->toString().compare("1")){
		valStream << value << "-" << val->toString();
		string str = valStream.str();
		return new Expression(str);
	}
	else{
		valStream << values["coefficient"]->toString() << value << "-" << val->toString();
		string str = valStream.str();
		return new Expression(str);
	}
}
Expressions *Transcendental::multiplication(Expressions *val){
	stringstream valStream;
	if (typeid(*val) == typeid(Transcendental) && value.compare(val->getTranscendentalValue()) == 0) {
		Expressions * coeffs = values["coefficient"]->multiplication(val->getValues()["coefficient"]);
		if(coeffs->toString() != "0"){
			/*valStream << coeffs->toString() << value << "^" << "2";
			string str = valStream.str();
			return new Exponential(str);*/
			Expressions * exponentialValue = new Transcendental("pi");
			if (getTranscendentalValue() == "e"){
				exponentialValue = new Transcendental("e");
			}
			Expressions * exponentialExponent = new Integer("2");
			return new Exponential(exponentialValue, exponentialExponent, coeffs);
		}
		else{
			valStream << coeffs->toString();
			return new Integer("0");
		}
	}
	else if (typeid(*val) == typeid(Integer)){
		Expressions * coeffs = values["coefficient"]->multiplication(val);
		valStream << coeffs->toString() << value;
		string str = valStream.str();
		return new Transcendental(str);
	}
	else if (typeid(*val) == typeid(Rational)){
		return new Rational(this->multiplication(val->getValues()["numerator"]), val->getValues()["denominator"]);
	}
    else if (typeid(*val) == typeid(Expression)) {
        return val->multiplication(this);
    }
	else if(!values["coefficient"]->toString().compare("1")){
		valStream << value << "*" << val->toString();
		string str = valStream.str();
		return new Expression(str);
	}
	else{
		valStream << values["coefficient"]->toString() << value << "*" << val->toString();
		string str = valStream.str();
		return new Expression(str);
	}
}
Expressions *Transcendental::division(Expressions *val){
	if(typeid(*val) == typeid(Rational)){
		return new Rational(this->multiplication(val->getValues()["denominator"]), val->getValues()["numerator"]);
	}
	else{
	    Expressions * result =  new Rational(this, val);
	    if(!result->getValues()["denominator"]->toString().compare("1")){
	    	result = result->getValues()["numerator"];
	    }
	    return result;
	}
}
Expressions *Transcendental::exponentiation(Expressions *val){
	stringstream valStream;
	if (typeid(*val) == typeid(Integer) && val->getValue() == 0){
		return new Integer("1");
	}
	else if (typeid(*val) == typeid(Integer) && val->getValue() == 1){
			return new Transcendental(value);
	}
    else {
        return new Exponential(new Transcendental(value), val, values["coefficient"]->exponentiation(val));
    }
}
Expressions *Transcendental::nthRoot(Expressions *val){
	stringstream valStream;
		return new Exponential(new Transcendental(value), val, values["coefficient"]->nthRoot(val));
}

long Transcendental::getValue(){
  return 0;
}
void Transcendental::setValue(long value){
	return;
}
