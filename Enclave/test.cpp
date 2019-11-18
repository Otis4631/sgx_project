#include "mat.h"
#include "tools.h"
#include "layer.h"
#include <vector>
#include <iostream>
#include <ctime>
#include <stdio.h>

#include <omp.h>
using namespace std;
// const char *fun()
// {

//     const char s[] = "222222";
//     return s;
// }
// void conv_test()
// {
//     vect_double data = {1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0};
//     Mat x(data, {1, 1, 5, 5});
//     x.print();
//     vect_double w_data = {1, 0, 1, 0, 1, 0, 1, 0, 1};

//     Mat weight(w_data, {1, 1, 3, 3});
//     Layer *conv = new Conv(1, 1, 3, 1, 0, &weight);
//     x = conv->forward(x);
//     x.print();
//     delete conv;
// }

// void fun(int* a) {
//     for(int i=0;i<100;i++){
//         cout << a[i] << endl;
//     }
// }

void model_test() {
    Mat x = randn({1,3,224,224});
    Conv conv(3, 64, 11, 4, 2);
    time_t begin = clock();
    x = conv.forward(x);
    time_t end = clock();
    x.print();
    cout << "conv forwad take " << (double)(end - begin) / CLOCKS_PER_SEC << " s" << endl;
}


void time_test() {

    Mat x = randn({1,3,224,224});
    time_t start = clock();
    for(auto i: range(55 * 55)) {
        //make_indices(":,:,0:11,0:11");
        x = x[make_indices(":,:,0:11,0:11")];
    }
    time_t end = clock();
    double time = (double)(end - start) / CLOCKS_PER_SEC;
    cout << time << endl;

    x.print();
}

void custom_test() {
    #define MAX 1000000
     vector<int> no_reserse;
    vector<int> reserse;
    
    reserse.resize(MAX);
    time_t t1 = clock();
    for(int i=0;i<MAX / 10;i++) {

        no_reserse.insert(no_reserse.end(),{1,1,1,1,1,1,1,1,1,1});
    }
    time_t t2 = clock();
    for(int i=0;i<MAX;i++) {
        reserse[i] = (i);
    }
    time_t t3 = clock();

    double r1 = (double)(t2-t1) /CLOCKS_PER_SEC;
    double r2 = (double)(t3-t2)/CLOCKS_PER_SEC;
    cout << r1 / (r1+r2)<<endl;
}



void conv_test() {
    vect_double data = {1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,1,1,0,0,1,1,0,0};
    Mat x(data, {1,2,5,5});
    x.print();
    vect_double w_data = {1,0,1,0,1,0,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,0,1,0,1};

    Mat weight(w_data, {2,2,3,3});
    Layer* conv = new Conv(2,2,3,1,0,false, &weight);
    x = conv -> forward(x);
    x.print();
    delete conv;
}
void slice_test() {
    vect_double data = {1,2,3,4,5,6,7,8,9,10,11,12,
                        1,2,3,4,5,6,7,8,9,10,11,12};
    vect_int shape = {3,2,2,2};
    Mat x(data, shape);

    x.print();
    x = x["1:,1,:1, 0: 1"];
    x.print();
}


int main() {
    model_test();

}