#include "tools.h"
#include "Enclave_t.h"

/***************   Tools   *****************************/
int printf(const char *fmt, ...)
{
    char buf[BUFSIZ] = { '\0' };
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, BUFSIZ, fmt, ap);
    va_end(ap);
    ocall_print_string(buf);
    return (int)strnlen(buf, BUFSIZ - 1) + 1;

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

void set_precison(string& s,int precision)//precision n.精度
{
    /**
     * 设置double输出精度
    */
    int pos = (int)s.find(".");
    int i = (int)s.length() - 1 - pos - precision;
    while (i--)
        if(s[s.length() - 1] == '0')
            s.pop_back();
}
/***************   Tools   *****************************/

