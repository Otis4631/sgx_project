#include<iostream>
#include<stdio.h>
#include<string>
#include<vector>

using namespace std;


int main()
{
    vector<int> a = {1,2,3,4,5};
    auto iter = a.rbegin();
    iter ++;
    
    return 0;
}