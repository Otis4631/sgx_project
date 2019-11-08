#include "tools.h"
#include <sgx_trts.h>
#include "Enclave_t.h"
#include <string>
#include <vector>
#include <string.h>
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
    int i = 0;
    vector<int> res;
    while(i ++ < limit) {
        res.push_back(i);
    }
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
double rand_double() {
    unsigned char double_buffer[1] = {'\0'};
    sgx_status_t res = sgx_read_rand(double_buffer, 1);
    if(res != SGX_SUCCESS) {
        printf("failed create random number\n");
        throw exception();
    }
    return (double)(*double_buffer);
}

/**************** special functions *******************/
