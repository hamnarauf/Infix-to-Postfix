/*
    a. Implementation of Stack (all data types) 
    b. validation of mathematical expression
        1. check for correct occurance of brackets
        2. check for valid characters
    c. infix to postfix conversion 
    d. evaluation of postfix expression
*/

#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <cmath>

using namespace std;

// ADT for Stack
template <class Type>
class Stack
{
    Type *arr;
    int top;
    int SIZE;

public:
    Stack(int size = 10)
    {
        // initializing the stack
        SIZE = size;
        arr = new Type[SIZE];
        top = -1;
    }

    // function pushes elm to the stack if stack is not full
    void push(Type elm)
    {
        if (!isFull())
            arr[++top] = elm;
    }

    // function returns true if stack is not empty, pops from stack and stores in elm
    bool pop(Type *elm)
    {
        if (!isEmpty())
        {
            *elm = arr[top--];
            return true;
        }
        return false;
    }

    // function returns true if stack is not empty and stores peak of stack in elm
    bool peak(Type *elm)
    {
        if (!isEmpty())
        {
            *elm = arr[top];
            return true;
        }
        return false;
    }

    // function displays all elements in stack
    void display()
    {
        if (!isEmpty())
        {
            for (int i = 0; i <= top; i++)
                cout << arr[i] << endl;
        }
    }

    // function returns true if stack is empty
    bool isEmpty()
    {
        return top == -1;
    }

    // function returns true if stack is full
    bool isFull()
    {
        return top == SIZE;
    }

    // function clears stack
    void clear()
    {
        delete[] arr;
        top = -1;
    }

    // destructor
    ~Stack()
    {
        clear();
    }
};

//ADT for expression conversion from infix to postfix and its evaluation using stack
class InfixToPostfix
{
    string exp;

private:
    // utility functions
    bool isOpenBracket(char sym);
    bool isCloseBracket(char sym);
    bool isBracket(char sym);
    bool isOperator(char sym);
    char getClosingBracket(char openBracket);
    queue<string> splitOnSpaces(string exp);
    queue<string> splitOperands();
    bool checkPrecedence(char oper1, char oper2);
    float calculate(float oper1, float oper2, string operation);

public:
    InfixToPostfix(string exp)
    {
        this->exp = exp;
    }

    /* function return true if the expression is a valid expression:
    1. proper number of closing and opening brackets
    2. expression should only contain digits, operators, decimal point and brackets
    */
    bool validExpression()
    {
        Stack<char> stack(exp.length());
        int count = 0;
        char popElm, popElmClosing;

        for (int i = 0; i < exp.length(); i++)
        {
            // if the character is open bracket, push to stack
            if (isOpenBracket(exp[i]))
                stack.push(exp[i]);

            // if character is a closing bracket, pop from stack
            else if (isCloseBracket(exp[i]))
            {

                // if stack is empty, no opening bracket is present for this closing bracket
                if (!stack.pop(&popElm))
                {
                    cout << "This expression is not correct. " << exp[i] << "' no matching opening bracket" << endl;
                    return false;
                }

                // if stack is not empty pop from stack and check for matching pair
                else
                {
                    // pop from stack -> the opening bracket of the current closing bracket
                    popElmClosing = getClosingBracket((popElm));

                    // matching bracket was not poped
                    if (!(popElmClosing == exp[i]))
                    {
                        cout << "This expression is not correct. " << popElm << "' -not closed." << endl;
                        return false;
                    }
                }
            }

            // a character other then digits, operators, brackets or blank spaces or decimal point is present
            else if (!isdigit(exp[i]) && !isOperator(exp[i]) && !(exp[i] == ' ') && !(exp[i] == '.'))
            {
                cout << "Invalid Expression" << endl;
                return false;
            }
        }

        // if stack is not empty then at least one opening bracket is not closed
        if (!stack.isEmpty())
        {
            cout << "This expression is not correct." << endl;
            return false;
        }
        return true;
    }

    // function coverts the infix expression into postfix
    string covertToPostfix()
    {
        if (!validExpression())
            return "";

        string postfix = "";
        queue<string> operands = splitOperands(); // queue of operands
        Stack<char> stack(exp.length() / 2);
        char popElm, peak;

        stack.push('('); // maintaining brackets around whole expression to pop all at the last

        int i = 0;
        while (i <= exp.length()) //traversing through the expression
        {
            if (i == exp.length())
                exp[i] = ')'; // pops all elements from the stack in the end

            // if digit is found, pop operand from operand queue
            if (isdigit(exp[i]))
            {
                postfix = postfix + operands.front() + " ";
                operands.pop();
                while (isdigit(exp[i]) || exp[i] == '.') //jump to succeeding term
                    i++;
            }

            else
            {
                // for open bracket, push it to stack
                if (isOpenBracket(exp[i]))
                    stack.push(exp[i]);

                // for closing bracket, pop from stack till matching open bracket is found
                else if (isCloseBracket(exp[i]))
                {
                    while (stack.pop(&popElm) && !isOpenBracket(popElm))
                        postfix = postfix + popElm + " ";
                }

                /* for operators, check for peak of stack, if stack is empty or peak is bracket push operator
                else pop from stack till the peak is a operator of higher or same precedence
                then push the incoming operator to the stack
                */
                else if (isOperator(exp[i]))
                {
                    stack.peak(&peak);
                    while (stack.peak(&peak) && isOperator(peak) && checkPrecedence(peak, exp[i])) // precedence check
                    {
                        if (stack.pop(&popElm))
                            postfix = postfix + popElm + " ";
                    }
                    stack.push(exp[i]); //pushing the operator to the stack
                }
                i++;
            }
        }
        return postfix;
    }

    // function takes postfix expression and returns the result
    float evaluateExpression(string postfix)
    {
        queue<string> tokens = splitOnSpaces(postfix); //spliting the postfix expression on spaces
        Stack<float> stack(exp.length() / 2);          //stack for operands
        float operand1, operand2, result;

        while (!tokens.empty()) //loop runs till last token
        {
            // push the digits to the stack till operator is found
            if (isdigit(tokens.front()[0]))
                stack.push(stof(tokens.front())); //converting to float and pushing to stack
            else
            {
                // poping operands from stack
                stack.pop(&operand2);
                stack.pop(&operand1);
                result = calculate(operand1, operand2, tokens.front()); // evaluating the expression
                stack.push(result);
            }
            tokens.pop();
        }
        stack.pop(&result);
        return result;
    }
};

// function return true if sym is an opening bracket
bool InfixToPostfix::isOpenBracket(char sym)
{
    return sym == '{' || sym == '[' || sym == '(';
}

// function return true if sym is a closing bracket
bool InfixToPostfix::isCloseBracket(char sym)
{
    return sym == '}' || sym == ']' || sym == ')';
}

// function returns true if sym is a bracket
bool InfixToPostfix::isBracket(char sym)
{
    return isOpenBracket(sym) || isCloseBracket(sym);
}

// fucntion returns true if sym is an operator
bool InfixToPostfix::isOperator(char sym)
{
    return sym == '+' || sym == '-' || sym == '*' || sym == '/' || sym == '%' || sym == '^';
}

// function returns the matching closing bracket
char InfixToPostfix::getClosingBracket(char openBracket)
{
    if (openBracket == '(')
        return ')';
    else if (openBracket == '{')
        return '}';
    else if (openBracket == '[')
        return ']';
}

// function splits the expression on spaces
queue<string> InfixToPostfix::splitOnSpaces(string exp)
{
    queue<string> tokens; // queue for tokens
    string token = "";

    int i = 0;
    while (i <= (exp.length()))
    {
        if (isspace(exp[i]) || i == exp.length()) // if space is encountered, push token to queue
        {
            if (token != "")
                tokens.push(token);
            token = "";
        }
        else
            token += exp[i];
        i++;
    }
    return tokens;
}

// function returns queue of operands
queue<string> InfixToPostfix::splitOperands()
{
    queue<string> operands; // queue for storing operands
    string numb = "";

    int i = 0;
    while (i <= (exp.length()))
    {
        // if operator, bracket or space is found, push numb into queue
        if (isOperator(exp[i]) || isBracket(exp[i]) || i == exp.length() || isspace(exp[i]))
        {
            if (numb != "")
                operands.push(numb);
            numb = "";
        }
        else // digits
            numb += exp[i];
        i++;
    }
    return operands;
}

// function returns true if precedence of oper1 is higher or equal to oper2
bool InfixToPostfix::checkPrecedence(char oper1, char oper2)
{
    map<char, int> prec = {{'^', 3}, {'*', 2}, {'/', 2}, {'%', 2}, {'+', 1}, {'-', 1}};
    return prec[oper1] >= prec[oper2];
}

// function calculates the expression
float InfixToPostfix::calculate(float oper1, float oper2, string operation)
{
    if (operation == "+")
        return oper1 + oper2;
    else if (operation == "-")
        return oper1 - oper2;
    else if (operation == "*")
        return oper1 * oper2;
    else if (operation == "/")
        return oper1 / oper2;
    else if (operation == "^")
        return pow(oper1, oper2);
    else if (operation == "%")
        return remainder(oper1, oper2);
}

int main()
{
    string infix, postfix;
    float result;
    cout << "Enter Infix: ";
    getline(cin, infix);

    InfixToPostfix inToPost(infix);

    postfix = inToPost.covertToPostfix();
    if (postfix != "")
    {
        result = inToPost.evaluateExpression(postfix);
        cout << "Postfix: " << postfix << endl;
        cout << "Result: " << result << endl;
    }

    return 0;
}