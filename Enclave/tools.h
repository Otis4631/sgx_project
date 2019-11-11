#ifndef _TOOLS_H_
#define _TOOLS_H_
#include <sgx_trts.h>
#include <string>
using namespace std;


#define BUF_SIZE 1024  // 12M
int printf(const char *fmt, ...);
const char* make_indices(const char *fmt, ...);

double rand_double(vector<int> param, int precision = 3);
vector<int> range(int limit);
string get_n_space(int n);
string get_n_linefeed(int n);
void set_precison(string& s,int precision);//precision n.精度
std::vector<std::string> split(const char *s, const char *delim);
#endif
