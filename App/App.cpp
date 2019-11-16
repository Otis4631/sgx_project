#include "Enclave_u.h"
#include "sgx_urts.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <torch/script.h> // One-stop header.
#include <torch/torch.h>

#include <iostream>
#include <memory>

#define BUFSIZE 1024

using namespace std;


void ocall_print_string(const char *str)
{
    printf("%s", str);
}


int main(int argc, const char* argv[]) {

//     if (argc != 2) {
//         std::cerr << "usage: example-app <path-to-exported-script-module>\n";
//         return -1;
//   } 


//     auto module = torch::jit::load(argv[1]);
//     auto modules = module.get_modules(); // 3
//     cout << module.name().qualifiedName() << endl;
//     //auto methods = module.get_methods(); // only forward
//     //cout << module.num_slots() << endl; //4

//     for(auto m: modules) {
//         auto dont = m;
//         cout << dont.get_modules().size() << endl;
//         auto a = m.name(); //Q
//         auto as = a.qualifiedName();
//         cout << as << endl;
        
//     }


    //cout << modules[1];

    // std::vector<torch::jit::IValue> inputs;
    // inputs.push_back(torch::ones({1, 3, 224, 224}));
    // auto output = module.forward(inputs).toTensor();
    // cout << output.slice(/*dim=*/1, /*start=*/0, /*end=*/5);

    sgx_enclave_id_t eid = 0;
    sgx_status_t ret = sgx_create_enclave("enclave.signed.so", SGX_DEBUG_FLAG, NULL, NULL, &eid, NULL);
    if(ret != SGX_SUCCESS){
        std::cerr << "error when create enclave\n" << ret;
        return -1;
    }
    hello(eid);
    sgx_destroy_enclave(eid);
    printf("\nprogram exit...");
}
