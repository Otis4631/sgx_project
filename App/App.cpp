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

    //cout << "char len:" << strlen(str) << " context: " << str << endl;
}


int main(int argc, const char* argv[]) {

    if (argc != 2) {
        std::cerr << "usage: example-app <path-to-exported-script-module>\n";
        return -1;
  } 
    // sgx_enclave_id_t eid = 0;
    // sgx_status_t ret = sgx_create_enclave("enclave.signed.so", SGX_DEBUG_FLAG, NULL, NULL, &eid, NULL);
    // if(ret != SGX_SUCCESS){
    //     std::cerr << "error when create enclave\n" << ret;
    //     return -1;
    // }

    auto module = torch::jit::load(argv[1]);
    //module.dump(true,true,false);
    auto modules = module.get_modules(); // 3
    cout << module.name().qualifiedName() << endl;
    //auto methods = module.get_methods(); // only forward
    //cout << module.num_slots() << endl; //4
    auto seq = (*modules.begin()).get_modules();
    auto conv = *(seq.begin());
    conv.dump(true,true,false);
    auto i = conv.get_attribute("__constants__");
    //'[stride, padding, dilation, groups, bias, padding_mode, output_padding, in_channels, out_channels, kernel_size]'
    cout << conv.get_buffer("__constants__");
    //auto slots = conv.;
    //conv.dump(false,true,false);
    //rcout << slots.size() << endl;
    // for(auto i: slots) {
    //     if(i.name() == "__constants__") {
    //         auto dont = i.value().toIValue();
    //         auto genr = dont.toGenericListRef();
    //         for(auto j: genr) {
    //             cout << j.toTensor() << endl;
    //         }
    //         break;

    //     }
    // }
    //conv.dump(true,true,false);

    //cout << conv_attr<< endl;
 


    //cout << modules[1];

    // std::vector<torch::jit::IValue> inputs;
    // inputs.push_back(torch::ones({1, 3, 224, 224}));
    // auto output = module.forward(inputs).toTensor();
    // cout << output.slice(/*dim=*/1, /*start=*/0, /*end=*/5);
   //hello(eid);

    printf("\nprogram exit...");
}
