#include "Compute.h"


Compute::Compute() {}

Compute::Compute(string expr ) 		/// constructor
{
	isCorrectExpression(expr);
}

bool Compute::isCorrectExpression(string exp)
{
	bool isCorrect = true;
    vector<string> tempVec = setUpExpression(exp);
																						////
	if(tempVec.empty())
    {
    	cout << "Enter an actual expression!" << endl;
    	isCorrect = false;
    	return isCorrect;
    }


    if(isInfixToRPN(tempVec, expression) )
    {
    	previousInputs.push_back( exp );
    	return isCorrect;
    }

    else
    {
        cout << "\nMismatching parentheses!\n" << endl;
        isCorrect = false;
        return isCorrect;
    }
}

Expressions *Compute::assignToClass(string& token)
{
	Expressions *temp;

	if(isNumeric(token))
	{
		if(token.find_first_of("+-") != -1)
		{
			temp = new Expression(token);
		}
		if(token.find_first_of("pe") != -1)
			temp = new Transcendental(token);
		else if(token.find_first_of("/") != -1)
			temp = new Rational(token);
		else
			temp = new Integer(token);
	}
	return temp;
}

/*
 * returns 1 if op1 is of greater precedence than op2
 * Ex: op1 = * and op2 = +, return 1
 *
 * returns 0 if op1 is of the same precedence as op2
 * Ex: op1 = * and op2 = /, return 0
 *
 * returns -1 if op1 is of lesser precedence than op2
 * Ex: op1 = + and op2 = *, return -1
 *
 */
void Compute::calculate()
{
    stack<Expressions *> st;
    Expressions * val1b;
    Expressions * val2b;
    Expressions * resultt;

    // For each token
																		////
    for ( int i = 0; i < (int) expression.size(); ++i )
    {
    	const string token = expression[ i ];

        // If the token is a value push it onto the stack
        if ( !isOperator(token) )
        {
        	Expressions *token2 = assignToClass(expression[i]);
            st.push(token2);
        }
        else
        {

            // Token is an operator: pop top two entries
            val2b = st.top();
            st.pop();
            //beginning of Expressions type
            if ( !st.empty() )
            {
            	if(token == "n")
            	{
            		Expressions *temp = new Integer("-1");
					resultt = calculate(val2b,temp,"*");
            	}
            	else
            	{
					val1b = st.top();
					st.pop();

					try
					{
						resultt = calculate(val1b, val2b, token);
					}
					catch(string exception)
					{
						throw exception;
					}
            	}
            }
            else
            {
                if ( token == "-" )
                {
                	Expressions *temp = new Integer("-1");
                    resultt =  calculate(val2b,temp,"*");
                }
                else if (token == "n")
                {
                	Expressions *temp = new Integer("-1");
					resultt =  calculate(val2b,temp,"*");
                }
                else
                    resultt = val2b;
            }
            // Push result onto stack
            if(typeid(*resultt) == typeid(Exponential))
            {
            	if(resultt->getValues()["value"]->getValue() == 1)
            	{
            		string temp = resultt->toString();
            		resultt = assignToClass(temp);
            	}
            }
            st.push(resultt);

        }
    }

    if(!st.empty())// && st.top() != " ")
    {
    	previousAns.push_back(st.top());
    	expression.clear();
    }
}

Expressions *Compute::calculate(Expressions * num1, Expressions * num2, string op)
{
	Expressions * result;
	Expressions *n1 = NULL, *n2 = NULL, *temp = NULL;
	stringstream logStream;
	string log, tempStr;

	if(typeid(*num1) == typeid(Expression))
	{
		tempStr = num1->toString();
		n1 = assignToClass(tempStr);
	}
	else if(typeid(*num1) == typeid(Logarithm))
	{
		tempStr = num1->toString();
		if(isNumeric(tempStr))
			n1 = assignToClass(tempStr);
		else
			n1 = new Logarithm(tempStr);
	}
	else
	{
		//determine type of num1
		if(typeid(*num1) == typeid(Integer))
		{
			string temp = num1->toString();
			n1 = new Integer(num1->toString());
		}

		else if(typeid(*num1) == typeid(Transcendental))
			n1 = new Transcendental(num1->toString());

		else if(typeid(*num1) == typeid(Rational))
			n1 = new Rational(num1->toString());

		else if(typeid(*num1) == typeid(Exponential))
		{
			n1 = new Exponential(num1->getValues()["value"],
								 num1->getValues()["exponent"],
								 num1->getValues()["coefficient"]);
		}

		else if(typeid(*num1) == typeid(Logarithm))
			n1 = new Logarithm(num1->toString());
		else
		{
			string temp = num1->toString();
			n1 = new Expression(temp);
		}
	}

	if(typeid(*num2) == typeid(Expression))
	{
		tempStr = num2->toString();
		n2 = assignToClass(tempStr);
	}
	else if(typeid(*num2) == typeid(Logarithm))
	{
		tempStr = num2->toString();
		if(isNumeric(tempStr))
			n2 = assignToClass(tempStr);
		else
			n2 = new Logarithm(tempStr);
	}
	else
	{
		//determine type of num2
		if(typeid(*num2) == typeid(Integer))
			n2 = new Integer(num2->toString());

		else if(typeid(*num2) == typeid(Transcendental))
			n2 = new Transcendental(num2->toString());

		else if(typeid(*num2) == typeid(Rational))
			n2 = new Rational(num2->toString());

		else if(typeid(*num2) == typeid(Exponential))
		{
			n2 = new Exponential(num2->getValues()["value"],
								 num2->getValues()["exponent"],
								 num2->getValues()["coefficient"]);
		}

		else if(typeid(*num2) == typeid(Logarithm))
			n2 = new Logarithm(num2->toString());
		else
		{
			string temp = num2->toString();
			n2 = new Expression(temp);
		}
	}

	if(op == "+")
		result = n1->addition(n2);
	else if(op == "-")
		result = n1->subtraction(n2);
	else if(op == "*")
			result = n1->multiplication(n2);
	else if(op == "/")
	{
		if(typeid(*n2) == typeid(Integer))
		{
			if(n2->getValue() == 0)
			   {
					string exception = "\nERROR: Cannot divide by zero!";
					throw exception;
			   }
		}
			result = n1->division(n2);
	}
	else if(op == "^")
			result = n1->exponentiation(n2);
	else if(op == "rt"){
		Expressions * one = new Integer("1");
		result = calculate(one, n1, "/");
		result = n2->nthRoot(result);
	}

	else if(op == ":")
	{
		if(typeid(*n2) != typeid(Exponential))
		{
			logStream << "log_" << n1->toString() << op << n2->toString();
			log = logStream.str();
			return new Logarithm(log);
		}
		else
		{
			Expressions *coeff = new Integer("1");
			Expressions *value = n2;
			Expressions *base;
			if(typeid(*n1) == typeid(Integer))
				base = new Integer(n1->toString());
			else if(typeid(*n1) == typeid(Transcendental))
				base = new Transcendental(n1->toString());

			return new Logarithm(coeff,value,base);
		}
	}

	return result;
}
int Compute::comparePrecedence(string operator1, string operator2)
{
	//if both operators are the same
	//return 0 for equal precedence
    if(operator1.compare(operator2) == 0)
        return 0;
    
    if(operator1 == "n")
        return 1;
    
    else if(operator1 == "^" || operator1 == "rt")
    {
    	if(operator2 == "n")
    		return -1;
    	else
    		return 1;
    }
    else if(operator1 == ":")
    {
    	if(operator2 == "n" || operator2 == "^" || operator2 == "rt")
			return -1;
		else
			return 1;
    }
    else if(operator1 == "*" || operator1 == "/")
    {
        if(operator2 == "^" || operator2 == "rt" || operator2 == "n" || operator2 == ":")
            return -1;
        else if(operator2 == "*" || operator2 == "/")
        	return 0;
        else
            return 1;
    }
    else
    {
    	if(operator2 == "+" || operator2 == "-")
    		return 0;
    	else
    		return -1;
    }
}

vector<string> Compute::getPreviousInputs()
{
	return previousInputs;
}

vector<Expressions *> Compute::getPreviousA()
{
	return previousAns;
}


Expressions *Compute::getLatestAnswer()
{
	return previousAns.back();
}

vector<string> Compute::setUpExpression(string &expr)
{
    vector<string> vectorSetUpExp;
    string str = "";
    int count = 0;
    bool isDecimal = false;
    
    for(int i = 0; i < (int) expr.length();i++)
    {
        string token(1,expr[i]);
        
        if(token == ".")
        	isDecimal = true;
        else if(isDecimal && isNumeric(token)) count++;

        if(isOperator(token) || isParentheses(token))
        {
			if(token == "r") token = token + (expr[++i]);
        	if(isDecimal)
        	{
        		stringstream strstream;
        		double numer = strtod(str.c_str(), NULL);
        		int denom = 1;
        		bool repeatThrees = false;

        		if(str.find("333") != -1 ||
				   str.find("666") != -1 ||
				   str.find("999") != -1)
					repeatThrees = true;

				if(repeatThrees)
				{
					numer*=3;
					denom*=3;
					numer = ceil(numer);
				}
				else
				{
					for(int i = 0; i < count; i++)
					{
						numer*=10;
						denom*=10;
					}
				}

        		strstream << (int)numer;
        		vectorSetUpExp.push_back(strstream.str());
        		vectorSetUpExp.push_back("/");
        		strstream.str("");
        		strstream << denom;
        		str = strstream.str();
        		isDecimal = false;
        		count = 0;
        	}

			if(!str.empty())
			{
				vectorSetUpExp.push_back(str);
			}
			str = "";
			vectorSetUpExp.push_back(token);

        }
        else if(isNumeric(token))
        {
        	if(token == "p") token = token + (expr[++i]);
        	else if(token == "l") i++;
        	str.append(token);
        }
        else
        {
        	if(str != "")
        	{
        		if(token == "p")
        		{
        			token = token + (expr[++i]);
        			str.append(token);
        		}
        		else
        		{
        			vectorSetUpExp.push_back(str);
        			str = "";
        		}
        	 }
        }

    }
	if(vectorSetUpExp.empty())
    	return vectorSetUpExp;
    else
    {
    	vectorSetUpExp.push_back(str);
    	return vectorSetUpExp;
    }
}

bool Compute::isInfixToRPN(vector<string> &tokens, vector<string> &rpn)
{
    bool success = true;
    bool leadingNegative = false;
    bool pushed = false;
    
    stack<string>stack;
    list<string> list1;

    for(int i = 0; i <tokens.size();i++)
    {
        string token = tokens[i];
        
        if(token == "-" && (i == 0 || tokens[i-1] == "("))			/// if starts with - or (-
        {
        	token = "n";								/// n = negative
        	leadingNegative = true;
        }

        if( isOperator( token ) )
        {
            string operator1 = token;
            if( !stack.empty() )
            {
                string operator2 = stack.top();
                if(!pushed && !leadingNegative && operator1 == "-" && operator2 == "-")  /// checking if both are --
                {
                	stack.pop();
                	operator1 = "+";
                	leadingNegative = false;
                }
                else					/// if not both negative - - , then
                {/// if same operator or o1 is less
					while(isOperator(operator2) && (comparePrecedence(operator1, operator2) == 0 ||
						   							comparePrecedence(operator1, operator2) == -1))
					{
						stack.pop();
						list1.push_back(operator2);    	///  o2 has precedence
						if ( !stack.empty() )
							operator2 = stack.top();

						else
							break;
					}
                }
            }
            stack.push(operator1);
            pushed = false;
        }
        else if ( token == "(" )          
        {
			stack.push( token );
        }
        else if ( token == ")" )          
        {        
            // Until the token at the top of the stack is a left parenthesis,       
            // pop operators off the stack onto the output queue.      
            string topToken  = stack.top();    
  
            while ( topToken != "(" )          
            {                          
                list1.push_back(topToken );
                stack.pop();      
  
                if ( stack.empty() ) 
                    break;    
                
                topToken = stack.top();    
            }                                          
  
            // Pop the left parenthesis from the stack, but not onto the output queue.                                
            if ( !stack.empty() )  stack.pop();
  
            // If the stack runs out without finding a left parenthesis,       
            // then there are mismatched parentheses.                  
            if ( topToken != "(" )      
            {                          
                return false;      
            }
            leadingNegative = false;
        }
        else if(isNumeric(token))
        {
        	list1.push_back(token );
        	pushed = true;
        }
        //TODO will handle pi, e, log, etc.
        else
        {

        }
    }
    
    while ( !stack.empty() )          
    {        
        string stackToken = stack.top();      
  
        // If the operator token on the top of the stack is a parenthesis,       
        // then there are mismatched parentheses.      
        if ( isParentheses( stackToken )   )    
        {   
            success = false;
            return success;      
        }      
  
        // Pop the operator onto the output queue./      
        list1.push_back(stackToken );
        stack.pop();
    }          

    rpn.assign(list1.begin(), list1.end() );
  
    return success;
}

bool Compute::isOperator(string token)
{
	if(token.find_first_of("+-*/^rn:") != -1)
        return true;
    else
        return false;   
}

bool Compute::isNumeric(string token)
{
	if(token.find_first_of("1234567890ep.") != -1 &&
	   token.find_first_of("_") == -1	)
		return true;
	else
		return false;
}

bool Compute::isParentheses(string token)
{  
    if(token == "(" || token == ")")
        return true;
    else
        return false;
}

