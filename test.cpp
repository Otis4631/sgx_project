#include<iostream>
#include<stdio.h>
#include<string>
#include<vector>

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

void fun1(vector<int> a){
    a[2] = 4;

}


int main()
{
    vector<int> a = {1,2,3};
    vector<int>b;
    b = a;
    cout << a[2] << endl;
   fun1(a);
    cout << a[2];
    return 0;
}