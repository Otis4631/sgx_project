#include<iostream>
#include<stdio.h>
#include<string>
using namespace std;

template<int N, int m>
void PrintValues(int (&ia)[N], int(&ib)[m])
{
    cout << m << endl;; 
    
}

int PRINT_CONTROL = 0;


void fun(int* &a)
{
    cout << sizeof(a);
}

void fun1(int* a){
    cout << "pointer version";

}


int main()
{
    int a[] = {1,2,3};
    fun(a);
    
    return 0;
}