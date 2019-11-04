#ifndef _TOOLS_H_
#define _TOOLS_H_
#include <sgx_trts.h>
#include<string>
using namespace std;

#define BUF_SIZE 1023 * 1024 *  1024// 12M
int printf(const char *fmt, ...);
string get_n_space(int n);
string get_n_linefeed(int n);
void set_precison(string& s,int precision);//precision n.精度


#endif
