#include "tools.h"
#ifdef SGX
    #include <sgx_trts.h>
    #include "Enclave_t.h"
#else
    #include <iostream>
    #include <stdio.h>
    #include <stdarg.h>
    #include <omp.h>
#endif
#include <string>
#include <vector>
#include <string.h>
#include <math.h> //fmod
#include <random>

#define BUF_SIZE 1024

/***************   Tools   *****************************/

//C++ 去字符串两边的空格
void trim(string &s) 
{
    if (s.empty()) 
    {
        return ;
    }
    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
}

vector<int> range(int limit) {
    int i = -1;
    vector<int> res;
    while(++ i < limit) 
        res.push_back(i);
    return res;
}

std::vector<std::string> split(const char *s, const char *delim)
{
    std::vector<std::string> result;
    if (s && strlen(s))
    {
        int len = strlen(s);
        char *src = new char[len + 1];
        strncpy(src, s, strlen(s));
        src[len] = '\0';
        char *tokenptr = strtok(src, delim);
        while (tokenptr != NULL)
        {
            std::string tk = tokenptr;
            trim(tk);
            result.emplace_back(tk);
            tokenptr = strtok(NULL, delim);
        }
        delete[] src;
    }
    return result;
}

#ifdef SGX
int printf(const char *fmt, ...)
{
    char buf[BUF_SIZE] = { '\0' };
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, BUF_SIZE, fmt, ap);
    va_end(ap);
    ocall_print_string(buf);
    return (int)strnlen(buf, BUF_SIZE - 1) + 1;

}
#else
int printf(const char *fmt, ...)
{
    char* buf = new char[BUF_SIZE * 5];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, BUF_SIZE * 5, fmt, ap);
    va_end(ap);
    std::cout << buf;
    delete[] buf;
    return (int)strnlen(buf, BUF_SIZE - 1) + 1;

}

#endif


string make_indices(const char *fmt, ...) {
    double rnd = rand();
    // printf("entry pid: %d, rand: %f\n",omp_get_thread_num(), rnd);
    char* static_buf = new char(BUF_SIZE);
    //buf = { '\0' };
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(static_buf, BUF_SIZE, fmt, ap);
    va_end(ap);
    string res = static_buf;
    //delete[] static_buf;
    // printf("%s,:%f--%d\n", res.c_str(), rnd, omp_get_thread_num());
    // printf("Exit pid: %d, rand: %f\n",omp_get_thread_num(), rnd);

    return res;
}

string get_n_space(int n) {
    /**
     * 返回n个空格
     *
    */
    string s = "";
    while(n --)
        s += " ";
    return s;
}
string get_n_linefeed(int n) {
    /**
     * 返回n个换行符
     *
    */
       string s = "";
    while(n --)
        s += "\n";
    return s;
}
void set_precison(string& s, int precision)//precision n.精度
{
    /**
     * 设置double输出精度
    */

    int pos = (int)s.find(".");
    int i = (int)s.length() - 1 - pos - precision;
    while (i-- > -1 || s.back() == '0') {
        s.pop_back();
        if(s.back() == '.') {
            s.pop_back();
            break;

        }
            
    }
        

}
/***************   Tools   *****************************/

/**************** special functions *******************/
double rand_double(vector<int> param) {
    int start = 0;
    int end = 1;
    if(param.size() == 2) {
        start = param[0];
        end = param[1];
    }
    #ifdef SGX
        unsigned char double_buffer[1];
        sgx_status_t res_s = sgx_read_rand(double_buffer, 1);
        if(res_s != SGX_SUCCESS) {
            printf("failed create random number\n");
            throw exception();
        }
        double left = (double)((int)(*double_buffer) / 255.0); // 0-1之间的浮点数
        
        int interval = (int)(end - start);
        double res = left * interval + start;
    #else
        double rand_data = (rand() % 100);
        rand_data /= 100;
        int interval = (int)(end - start);
        double res = rand_data * interval + start;
    #endif
    return res;
}

/**************** special functions *******************/
