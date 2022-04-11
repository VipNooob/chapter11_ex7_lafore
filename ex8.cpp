// parse.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>
#include <typeinfo>

using namespace std;


const int LEN = 80;
const int MAX = 40;



class Token {
public:
    virtual float getNumber() = 0;
    virtual char getOperator() = 0;
};

class Operator : public Token {
private:
    char oper;
public:
    Operator(char ch) :oper(ch) {};
    char getOperator() { return oper; };
    float getNumber() {
        return 0;
    };
};

class Number : public Token {
private:
    float fnum;
public:
    Number(float n) : fnum(n) {};
    float getNumber() { return fnum; };
    char getOperator() { return 'f'; };
};








class stack {
private:
    Token* st[MAX];
    int top;
public:
    stack() :top(0) {}
    void push(char var) {
        st[++top] = new Operator(var);
    }
    void push(float fval) {
        st[++top] = new Number(fval);
    }
    Token* pop() {
        return st[top--];
    }
    int gettop() {
        return top;
    }
};


class Calculator {
private:
    stack s;
    const char* pStr;
    int len;
    int quantity;
public:
    Calculator(const char* s) : quantity(0) {
        pStr = s;
        len = strlen(pStr);
    }
    void parse();
    void solve();



};

void Calculator::parse() {
    char ch;
    char lastval;
    char lastop = 'B'; // bad
    string temp_val = "";


    for (int j = 0; j < len; j++) {
        ch = pStr[j];
        if (ch >= '0' && ch <= '9' || ch == '.') {
            lastop = 'G'; // good
            temp_val += ch;
            if (temp_val[0] == '.') {
                cout << "Expression starts from .\n"; exit(1);
            }
            if (count(temp_val.begin(), temp_val.end(), '.') > 1) {
                cout << "More then one point in a number!\n"; exit(1);
            }
        }
        if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            if (lastop == 'B') { cout << "Expression must be started from a number\n"; exit(1); }
            if (lastop == '+' || lastop == '-' || lastop == '*' || lastop == '/') {
                cout << "Wrong expression, several operations in a row\n"; exit(1);
            }

            lastop = ch;
            s.push(stof(temp_val)); (this->quantity)++;
            temp_val = "";
            s.push(lastop); (this->quantity)++;

        }
        if (j == len - 1 && temp_val != "") {
            s.push(stof(temp_val)); (this->quantity)++;
        }

    }
}


void Calculator::solve() {


    Token* first_val = s.pop();
    Token* first_op = s.pop();
    int length = s.gettop();

    for (int i = 0; i < (quantity - 2) / 2; i++) {

        Token* last_val = s.pop();
        Token* last_op = s.pop();





        //  1 + 2 * 3 --> first multiplication, next addition
        if ((first_op->getOperator() == '*') && (last_op->getOperator() == '+')) {
            first_val = new Number((first_val->getNumber() * last_val->getNumber()));
            first_op = new Operator(last_op->getOperator());
        }

        //  1 - 2 * 3 --> first multiplication, next addition
        else if ((first_op->getOperator() == '*') && (last_op->getOperator() == '-')) {
            first_val = new Number((first_val->getNumber() * last_val->getNumber()));
            first_op = new Operator(last_op->getOperator());
        }

        // 1 + 2 / 3 --> first division (last_val / first_val), next addition
        else if ((first_op->getOperator() == '/') && (last_op->getOperator() == '+')) {
            first_val = new Number((last_val->getNumber() / first_val->getNumber()));
            first_op = new Operator(last_op->getOperator());
        }
        // 1 - 2 / 3 --> first division (last_val / first_val), next addition
        else if ((first_op->getOperator() == '/') && (last_op->getOperator() == '-')) {
            first_val = new Number((last_val->getNumber() / first_val->getNumber()));
            first_op = new Operator(last_op->getOperator());
        }


        // 1 + 2 + 3 
        else if ((first_op->getOperator() == '+') && (last_op->getOperator() == '+')) {
            first_val = new Number((first_val->getNumber() + last_val->getNumber()));
            first_op = new Operator(last_op->getOperator());
        }
        // 1 - 2 - 3 
        else if ((first_op->getOperator() == '-') && (last_op->getOperator() == '-')) {
            first_val = new Number((-(last_val->getNumber()) - first_val->getNumber()));
            first_op = new Operator('+');
        }

        // 1 + 2 - 3 
        else if ((first_op->getOperator() == '-') && (last_op->getOperator() == '+')) {
            first_val = new Number((last_val->getNumber() - first_val->getNumber()));
            first_op = new Operator(last_op->getOperator());
        }

        // 1 - 2 + 3 
        else if ((first_op->getOperator() == '+') && (last_op->getOperator() == '-')) {
            first_val = new Number((-(last_val->getNumber()) + first_val->getNumber()));
            first_op = new Operator('+');
        }

        // 1 * 2 * 3
        else if ((first_op->getOperator() == '*') && (last_op->getOperator() == '*')) {
            first_val = new Number((first_val->getNumber() * last_val->getNumber()));
            first_op = new Operator(last_op->getOperator());
        }

        // 1 * 2 / 3
        else if ((first_op->getOperator() == '/') && (last_op->getOperator() == '*')) {
            first_val = new Number((last_val->getNumber() / first_val->getNumber()));
            first_op = new Operator(last_op->getOperator());
        }

        // 1 / 2 * 3
        else if ((first_op->getOperator() == '*') && (last_op->getOperator() == '/')) {
            first_val = new Number((last_val->getNumber() / first_val->getNumber()));
            first_op = new Operator(last_op->getOperator());
        }

        // 1 / 2 / 3 --> (1 / (2 * 3))
        else if ((first_op->getOperator() == '/') && (last_op->getOperator() == '/')) {
            first_val = new Number((last_val->getNumber() * first_val->getNumber()));
            first_op = new Operator(last_op->getOperator());
        }




        //  1 * 2 + 3 --> (3 + 1 * 2)
        else if ((first_op->getOperator() == '+') && (last_op->getOperator() == '*')) {


            stack temp;
            int size = s.gettop();
            for (int k = 0; k < size; k++) {
                Token* tok_t = s.pop();
                if (Number* t_num = dynamic_cast<Number*>(tok_t)) {
                    temp.push(t_num->getNumber());
                }
                else if (Operator* t_op = dynamic_cast<Operator*>(tok_t)) {
                    temp.push(t_op->getOperator());
                }
            }
            size = temp.gettop();
            s = stack();
            s.push(first_val->getNumber());
            s.push(first_op->getOperator());

            for (int j = 0; j < size; j++) {
                Token* tok_t = temp.pop();

                if (Number* t_num = dynamic_cast<Number*>(tok_t)) {
                    s.push(t_num->getNumber());
                }
                else if (Operator* t_op = dynamic_cast<Operator*>(tok_t)) {
                    s.push(t_op->getOperator());
                }
            }
            first_op = last_op;
            first_val = last_val;
            i--;


        }


        //  1 * 2 - 3 --> (-3 + 1 * 2)

        else if ((first_op->getOperator() == '-') && (last_op->getOperator() == '*')) {


            stack temp;
            int size = s.gettop();
            for (int k = 0; k < size; k++) {
                Token* tok_t = s.pop();
                if (Number* t_num = dynamic_cast<Number*>(tok_t)) {
                    temp.push(t_num->getNumber());
                }
                else if (Operator* t_op = dynamic_cast<Operator*>(tok_t)) {
                    temp.push(t_op->getOperator());
                }
            }
            size = temp.gettop();
            s = stack();

            s.push(-(first_val->getNumber()));
            s.push('+');

            for (int j = 0; j < size; j++) {
                Token* tok_t = temp.pop();

                if (Number* t_num = dynamic_cast<Number*>(tok_t)) {
                    s.push(t_num->getNumber());
                }
                else if (Operator* t_op = dynamic_cast<Operator*>(tok_t)) {
                    s.push(t_op->getOperator());
                }
            }
            first_op = last_op;
            first_val = last_val;
            i--;


        }


        //  1 / 2 - 3 --> (-3 + 1 * 2)

        else if ((first_op->getOperator() == '-') && (last_op->getOperator() == '/')) {


            stack temp;
            int size = s.gettop();
            for (int k = 0; k < size; k++) {
                Token* tok_t = s.pop();
                if (Number* t_num = dynamic_cast<Number*>(tok_t)) {
                    temp.push(t_num->getNumber());
                }
                else if (Operator* t_op = dynamic_cast<Operator*>(tok_t)) {
                    temp.push(t_op->getOperator());
                }
            }
            size = temp.gettop();
            s = stack();

            s.push(-(first_val->getNumber()));
            s.push('+');

            for (int j = 0; j < size; j++) {
                Token* tok_t = temp.pop();

                if (Number* t_num = dynamic_cast<Number*>(tok_t)) {
                    s.push(t_num->getNumber());
                }
                else if (Operator* t_op = dynamic_cast<Operator*>(tok_t)) {
                    s.push(t_op->getOperator());
                }
            }
            first_op = last_op;
            first_val = last_val;
            i--;


        }


        //  1 / 2 + 3 --> (3 + 1 / 2)
        else if ((first_op->getOperator() == '+') && (last_op->getOperator() == '/')) {

            stack temp;
            int size = s.gettop();
            for (int k = 0; k < size; k++) {
                Token* tok_t = s.pop();
                if (Number* t_num = dynamic_cast<Number*>(tok_t)) {
                    temp.push(t_num->getNumber());
                }
                else if (Operator* t_op = dynamic_cast<Operator*>(tok_t)) {
                    temp.push(t_op->getOperator());
                }
            }
            size = temp.gettop();
            s = stack();
            s.push(first_val->getNumber());
            s.push(first_op->getOperator());

            for (int j = 0; j < size; j++) {
                Token* tok_t = temp.pop();

                if (Number* t_num = dynamic_cast<Number*>(tok_t)) {
                    s.push(t_num->getNumber());
                }
                else if (Operator* t_op = dynamic_cast<Operator*>(tok_t)) {
                    s.push(t_op->getOperator());
                }
            }
            first_op = last_op;
            first_val = last_val;
            i--;


        }






    }
    float answer;
    Token* last_element = s.pop();
    switch (first_op->getOperator())
    {
    case '+':
        answer = first_val->getNumber() + last_element->getNumber(); break;
    case '-':
        answer = -first_val->getNumber() + last_element->getNumber(); break;
    case '*':
        answer = first_val->getNumber() * last_element->getNumber(); break;
    case '/':
        answer = last_element->getNumber() / first_val->getNumber(); break;
    default:
        break;
    }

    cout << answer << endl;

}

int main()
{

    char str[LEN];
    cout << "\nEnter the expression in the following form 2+3*4/3-2"
        "\nYou can use real numbers (for example 3.14159)"
        "\nDon't use whitespaces or brackets\n";

    cin >> str;

    Calculator calc(str);
    calc.parse();
    calc.solve();




}

