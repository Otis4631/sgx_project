#include "mat.h"
#include "Enclave.h"
#include "Enclave_t.h"
#include "layer.h"
#include <sgx_trts.h>


#include "tools.h"

using namespace std;

void test() {
    vect_double data = {2,3,1,9,4,7,3,5,8,2,2,2,1,3,4,5};
    vect_int shape = {1,4,4,1};

    Mat x(data, shape);
    x.print();
    Pool pooling(2, 2);
    x = pooling.forward(x);
    x.print();
}

void hello() {
    test();
}