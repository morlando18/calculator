
#include "Compute.h"
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;
void case1Compute();
void case2Previews();
void case3Help();

Compute calculator;
vector<string> previousExpressions;
vector<Expressions *> previousAnswers;
char mainMenu, reviewMenu, helpMenu;
bool goForward, isContinue;
int previouss;
string expressionn;
string choice;
string answer = "";


int main()
{

    do
    {
            cout << "\nMain menu\n" << endl;
            cout << "(1) Compute new expression." << endl;
            cout << "(2) Review previouss expressions and answers." << endl;
            cout << "(3) Help." << endl;
            cout << "(4) Quit." << endl;
            cout << "\npick from above choices: ";
            cin >> mainMenu;
            cin.ignore();
            cout << endl;

            switch(mainMenu)
            {
                case '1':
                    case1Compute(); break;
                case '2':
                    case2Previews(); break;
                case '3':
                    case3Help(); break;
                case '4':
                    break;
                default:
                    cout << "Invalid selection! Choose 1, 2, 3, or 4!\n" << endl;
            }
    }while(mainMenu != '4' && expressionn != "quit");
    return 0;
}


    void case1Compute(){
        goForward = true;
        while(goForward)
        {
            cout << "\nType 'back' to go back to main menu \nType 'quit' to exit the program." << endl;
            cout << "or \nType your expression: " << endl;
            getline(cin, expressionn);

            if(expressionn == "back" || expressionn == "quit")
                goForward = false;
            else
            {
                string strExp = expressionn;
                stringstream strstream;

                if(strExp.find("ans") != -1)
                {
                    int pos = strExp.find("ans");
                    strstream << strExp.substr(0, pos) << answer << strExp.substr(pos + 3, expressionn.size());
                    strExp = strstream.str();
                }

                if(strExp != expressionn) expressionn = strExp;

                if(calculator.isCorrectExpression(expressionn))
                {
                    try
                    {
                        calculator.calculate();
                        cout << "\nequals  " << (calculator.getLatestAnswer())->toString() << endl;
                        answer = "(" + (calculator.getLatestAnswer())->toString() + ")";
                    }
                    catch(string exception)
                    {
                        cout << exception << endl;
                    }
                }
            }
        }

    }
    void case2Previews(){
        previousExpressions = calculator.getPreviousInputs();

        if(previousExpressions.size() == 0)
            cout << "There are no previous expressions or answers!" << endl;

        else{
            previousAnswers = calculator.getPreviousA();
            do{
                cout << "\nPrevious expressions and answers\n" << endl;

                cout << "(1) View previous expressions." << endl;
                cout << "(2) View previous answers." << endl;
                cout << "(3) Convert a previous answer to a decimal." << endl;
                cout << "(4) Return to Main Menu" << endl;
                cout << "\nType your choice: ";

                cin >> reviewMenu;
                cin.ignore();
                cout << endl;

                switch (reviewMenu)
                {
                    case '1':
                        cout << "\nPreviouss expressions are : \n" << endl;

                        for(int i = previousExpressions.size()-1;i>=0;i--)
                        {
                            cout <<"Expression "<<(previousExpressions.size()-i) << ": "<<previousExpressions[i]<< endl;
                        }
                        break;
                    case '2':
                        cout << "\nPrevious answers are: \n" << endl;

                        for(int i = previousAnswers.size()-1;i>=0;i--)
                        {
                            cout<<"Answer "<<(previousExpressions.size()-i)<<": "<<previousAnswers[i]->toString()<<endl;
                        }
                        break;
                    case '3':
                        do{
                            do{
                                cout << "\nWhich answer you wish to convert (Type '1' for most recent answer): ";
                                cin >> previouss;
                            } while (previouss > previousAnswers.size());

                            cout << "\nOriginal form:" << endl;
                            cout << previousAnswers[previousAnswers.size() - previouss]->toString() << endl;

                            cout << "\nFloating point form : " << endl;
                            cout << previousAnswers[previousAnswers.size() - previouss]->toDouble() << endl;

                            cout << "\nContinue? (Y/N) ";
                            cin >> choice;

                            if(choice == "Y" || choice == "y")isContinue = true;
                            else if(choice == "N" || choice == "n")isContinue = false;

                        }while (isContinue);
                        break;
                    case '4':
                        cout << "Go back to the main menu..." << endl;
                        break;
                    default:
                        cout << " Invalid selection! Please choose 1, 2, 3, or 4.\n" << endl;
                }
            }while(reviewMenu != '4');
        }
    }
    void case3Help(){
        //this do-while loop represents the help menu
        do
        {
            cout << "Help Menu\n" << endl;

            cout << "(1) Examples of an expressions" << endl;
            cout << "(2) Exponents" << endl;
            cout << "(3) Square/Nth roots" << endl;
            cout << "(4) Logarithms" << endl;
            cout << "(5) Fractions" << endl;
            cout << "(6) Decimals" << endl;
            cout << "(7) display of answers" << endl;
            cout << "(8) Previous Answers and ans" << endl;
            cout << "(9) Return to main menu" << endl;
            cout << "\nEnter your choice: ";

            cin >> helpMenu;
            cin.ignore();
            cout << endl;

            switch(helpMenu)
            {
                case '1':
                    cout << "Examples of expressions : \n "
                                    "                  \"2 + 3 * 4 - 5 ^ 2\",\n"
                                    "                   \"2 ^ 3 + 4 * 5 - 2\",\n"
                                    "                   \"( 2 + 3 ) * ( 4 - 5 ) ^ 2\",\n"
                                    "                   \"1 / 3 + 1 / 4 + 1 / 12\",\n"
                                    "                   \"105 / 1344\",\n"
                                    "                   \"8 ^ ( -4 / 3 ) * 4\",\n"
                                    "                   \"3 rt 108 * 3 rt 16\"}; \nApproved Operators: +, -, *, /, ^, rt \nTranscedental Numbers: pi, e \n" << endl;
                    break;
                case '2':
                    cout << "Use the following format: x^y \nThis will be read as 'x raised to the power of y'.\n" << endl;
                    break;
                case '3':
                    cout << "To write out a nth root, use the following formats: n rt x \nThis will read as 'the nth root of x'. \n" << endl;
                    break;
                case '4':
                    cout << "To write out a logarithm, use the following format: log_b:x\nThis will read as 'log base b of x'. \n" << endl;
                    break;
                case '5':
                    cout << "To write out a fraction or rational number, use the following format: a/b \nThis will read as 'a over b'. \n" << endl;
                    break;
                case '6':
                    cout << "Rather than say 4/6= .666…, it would say that 4/6=2/3\n\nExceptions include: \n.3333 = 1/3 \t .6666 = 2/3 \t .9999 = 1\n If you wish to convert a previouss answer to decimal form, \n please go to option 2 on the main menu. \n" << endl;
                    break;
                case '7':
                    cout << "Your answers will be returned in their simplest forms: \nintegers (i.e. 2 + 2 = 4), fractions (i.e. 2/6 = 1/3), \nlogarithms (i.e. log_3:108 = 3+2log_3:2), \nexponentials (i.e. (2*2)^(1/3) = (4)^(1/3)), \nor expressions (i.e. 2 + 2 + pi = 4 + pi).\n" << endl;
                    break;
                case '8':
                    cout << "If you wish to look at your previouss answers or expressions, \n please go to option 2 in the main menu. \n The ans keyword will always store the previouss answer. \n (i.e. 2+2 returns 4, then ans+2 returns 6) \n" << endl;
                    break;
                case '9':
                    cout << "Returning back to the main menu..." << endl;
                    break;
                default:
                    cout << "Invalid selection! Choose 1, 2, 3, 4, 5, 6, 7, 8, or 9.\n" << endl;
            }
        }while(helpMenu != '9');
    }

