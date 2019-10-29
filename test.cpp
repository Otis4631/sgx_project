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

void pr(string& s, int shape[], int arr[], int dimension_level, int dimension, int& ped, int c)
{
    s += "[";
    int i = 0;
    int length = shape[dimension_level - 1];
    for(; i<length;i++){
        if(dimension_level == dimension){
            s += to_string(arr[ped ++]);
            if (i < length - 1)
                s += ",";
        }
        else{
            int comma_gate = 0;
            if (i == length - 1)
                comma_gate = 1;
            pr(s, shape, arr, dimension_level + 1, dimension, ped, comma_gate);
        }
    }
    if(dimension_level == dimension){
        s += "]";
        if (!c) s+= ",\n";
    }
        
    
    else
        if(dimension_level == 1)
            s += "]\n";
        else if(1){
            s += "],\n\n";
        }
}


int main()
{

    int mat[] = {1,2,3,4};
    int shape[] = {2,2};
    int d = 2;
    int ped = 0;
    string s;
    pr(s, shape, mat, 1, d, ped, 0);
    cout << s;
    return 0;
}