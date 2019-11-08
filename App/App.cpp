#include "Enclave_u.h"
#include "sgx_urts.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
// #include <torch/torch.h>

#define BUFSIZE 1024

using namespace std;


void ocall_print_string(const char *str)
{
    printf("%s", str);

    //cout << "char len:" << strlen(str) << " context: " << str << endl;
}

int main()
{ 
    sgx_enclave_id_t eid = 0;
    sgx_status_t ret = sgx_create_enclave("enclave.signed.so", SGX_DEBUG_FLAG, NULL, NULL, &eid, NULL);
    if(ret != SGX_SUCCESS){
        printf("error when create enclave\n");
        printf("%02X", ret);
    }
    // torch::Tensor tensor = torch::eye(3);
    // std::cout << tensor << std::endl;

   hello(eid);

    printf("program exit...");
}
