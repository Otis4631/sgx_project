#include<iostream>
#include<stdio.h>
#include<string>
#include<vector>

using namespace std;


int main()
{
    vector<int> a = {1,2,3,4,5};
    for(auto i = a.begin(); i != a.end(); i ++) {
        a.insert(i, 2);
    }
    return 0;
}