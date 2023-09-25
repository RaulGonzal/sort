#include <iostream>
#include <vector>
#include <stack>
#include <sstream>
#include <string>

using namespace std;
//NAME: Raul Gonzalez Cespedes
//FSUID: RG21D

class Identifier{
public:
	Identifier(){
		name = '\0';
		value = 0.0;
	}
	Identifier(string name, double value){
		this->name = name;
		this->value = value;
	}
	void setName(string name){
		this->name = name;
	}
	void setValue(double value){
		this->value = value;
	}
	string getName() const{
		return name;
	}
	double getValue() const{
		return value;
	}
private:
	string name;
	double value;
};

bool isaOperator(const string& str){
    return (str == "+" || str == "-" || str == "*" || str == "/");
}

int precedence(string s){
    if (s == "+" || s == "-"){
        return 1;
    }
    else if(s == "*" || s =="/"){
        return 2; 
    }
    else if(s == "("){
        return 3; 
    }else return 0;
}

bool isNumber(const string& str){
    bool convertible = false;
    try{
        stoi(str);
        convertible = true;
    }
    catch(...){}
    return convertible;
}

void calcPostfix(vector<string> &expression, vector<string> &postfix){
    stack<string> s;
    for (int i = 0; i < expression.size(); i++){
        if (isaOperator(expression[i]) || expression[i] == "(")
        {
            if (s.empty())
            {
                s.push(expression[i]);
            }
            else 
            {
                while (s.top() != "(" && precedence(s.top()) >= precedence(expression[i]))
                {
                    postfix.push_back(s.top());
                    s.pop();
                }
                    s.push(expression[i]);
            }
        }
        else if (expression[i] == ")")
        {
            while(s.top() != "(")
            {
                postfix.push_back(s.top());
                s.pop();
            }
            s.pop();
       }
       else postfix.push_back(expression[i]);
    }
    while (!s.empty()){postfix.push_back(s.top()); s.pop();}
}

double resultEvaluation(const vector<string>& postfix, const vector<Identifier>& identifiers){
    
    stack<double> evaluation;
    for (int i = 0; i < postfix.size(); i++)
    {
        if(isalnum((postfix[i])[0]))
        {
            bool isIdent = false;
            for (int j = 0; j < identifiers.size(); j++)
            {
                if (identifiers[j].getName() == postfix[i])
                {
                  evaluation.push(identifiers[j].getValue());  
                  isIdent = true;
                }
            }
            if (!isIdent){
                evaluation.push(atof(postfix[i].c_str())); 
            }
        }
        else
        {
            double op2 = evaluation.top();
            evaluation.pop();
            double op1 = evaluation.top();
            evaluation.pop();

            if (postfix[i] == "+"){
                evaluation.push(op1 + op2); 
            }
            else if (postfix[i] == "-"){
                evaluation.push(op1 - op2);
            }
            else if (postfix[i] == "*"){
                evaluation.push(op1 * op2);
            }
            else if (postfix[i] == "/"){
                evaluation.push(op1 / op2);
            }
        }
    }
    return evaluation.top();
}

void printExpression(vector<string> &expression, vector<string> &postfix){
    string equation;
    cout << "Enter the infix expression: ";
	getline(cin, equation);
    stringstream ss(equation);

    string token;
    while (ss >> token){
        expression.push_back(token);
    }

    calcPostfix(expression, postfix);

    cout << "The postfix expression is\n";
    for(int i = 0; i < postfix.size(); i++){
        cout << postfix[i] << " ";
    }
    cout << endl;
}

void getIdentifiers(const vector<string>& postfix, vector<Identifier> &identifiers){
    double inVal = 0.0;
    for (int i = 0; i < postfix.size(); i++){
        if (!isNumber(postfix[i])){
            if (postfix[i] != "+" && postfix[i] != "-" && postfix[i] != "*" &&
                postfix[i] != "/" && postfix[i] != "(" && postfix[i] != ")")
                {
                   cout << "Enter the value of " << postfix[i] << ": ";
                   cin >> inVal;
                   Identifier newId(postfix[i], inVal);
                   identifiers.push_back(newId);
                }
        }
    }
}

int main(){
    vector<string> expression;
    vector<string> postfix;
    vector<Identifier> identifiers;

    printExpression(expression, postfix);
    getIdentifiers(postfix, identifiers);
    cout << "The expression evaluates to " << resultEvaluation( postfix, identifiers) << endl;
}