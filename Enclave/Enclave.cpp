#include "Mat.h"
#include "Enclave.h"
#include "Enclave_t.h"
#include <sgx_trts.h>


#include "tools.h"

using namespace std;

void hello()
{
    vect_double v1 = {1, 2, 3, 4, 5, 6};
    vect_int  shape1 = {3, 2};
    vect_double v2 = {1,2};
    vect_int  shape2 = {2,1};

    try{
    Mat mat1(v1, shape1);
    Mat mat2(v2, shape2);
    Mat mat = mat1.dot2d(mat2);
    mat.print();

    }
    catch(exception& e){
        printf(e.what());
    }
}