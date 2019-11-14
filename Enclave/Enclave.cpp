#include "mat.h"
#include "Enclave.h"
#include "Enclave_t.h"
#include "layer.h"
#include <sgx_trts.h>


#include "tools.h"

using namespace std;

void test() {
    vect_double data = {2,3,1,9,4,7,3,5,8,2,2,2,1,3,4,5};
    vect_int shape = {1,1,4,4};

    Mat x(data, shape);
    x.print();
    //Pool pool(2, 2, "average")
    Layer* pooling = new Pool(2, 2, "average");
    x = pooling->forward(x);
    x.print();
}

void pad_test() {
    vect_double data = {2,3,1,9,4,7,3,5,8,2,2,2,1,3,4,5};
    vect_int shape = {2,2,2,2};
    Mat x(data, shape);
    x.print();
    pad(x, 3, 0, true);
    x.print();
}

void conv_test() {
    vect_double data = {1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,1,1,0,0,1,1,0,0};
    Mat x(data, {1,1,5,5});
    x.print();
    vect_double w_data = {1,0,1,0,1,0,1,0,1};

    Mat weight(w_data, {1,1,3,3});
    Layer* conv = new Conv(1,1,3,1,0, &weight);
    x = conv -> forward(x);
    x.print();
    delete conv;
}

void dropout_test() {
    vect_double data = {2,3,1,9,4,7,3,5,8,2,2,2,1,3,4,5};
    vect_int shape = {4,4};
    Mat x(data, shape);
    x.print();
    Layer * dropout = new Dropout(0.5);
    x = dropout->forward(x);
    x.print();
}

void adaptivePool_test() {
    vect_double data = {2,3,1,9,4,7,3,5,8,2,2,2,1,3,4,5};
    vect_int shape = {1,1,4,4};
    Mat x(data, shape);
    x.print();
    Layer * apool = new AdaptivePool({2,3});
    x = apool->forward(x);
    x.print();
}

void hello() {
    adaptivePool_test();
}