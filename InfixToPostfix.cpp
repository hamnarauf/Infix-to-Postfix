#include <iostream>
using namespace std;


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
        {
            arr[++top] = elm;
        }
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
            {
                cout << arr[i] << endl;
            }
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

