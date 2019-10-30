#include <string.h>
#include "Enclave.h"
#include "Enclave_t.h"
#include <stdio.h>
#include <string>
#include <sgx_trts.h>
#include <stdlib.h>
#include <vector>
#include <exception>
using namespace std;

typedef vector<double>  vect_double;
typedef vector<int>     vect_int; 
#define BUF_SIZE 1023 * 1024 *  1024// 12M

/*************** Exception  Defination*****************************/
struct ShapeMatchError : public exception
{
  const char * what () const throw ()
  {
    return "ERROR: The shape you passed in does not match the size of the matrix\n";
  }
};
 

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

/***************   Tools   *****************************/

/***************   Class defination   *****************************/
class Mat
{
    private:
        
        void matrix_to_string(string& s, int dimension_level, int& ped, int c);
        bool is_boardcastable(vect_int _shape, vect_int& lshape, vect_int& rshape);
        // 获取一位数组总长度
        

    public:
        int             dimension;  // 维度
        int             size;       // 一位数组总长度
        vector<int>     shape;      // shape
        vect_double  data;

        void boardcast(Mat* mat, vect_int new_shape);
        string shape_to_string(vect_int* _shape);
        bool shape_match(vect_int _shape, int _size);
        Mat(vect_double _data, vector<int> _shape);
        int get_length_from_shape(vect_int s);
        vect_double flatten();

        Mat operator* (Mat rmatrix);
        void print(); 
};
/***************   Class defination   *****************************/


/***************   Class implementation   *****************************/

void Mat::boardcast(Mat* mat, vect_int new_shape) {
    //TODO
}

bool Mat::is_boardcastable(vect_int _shape, vect_int& lshape, vect_int& rshape) {
    /*
    * _shape：右矩阵shape
    * lshape：可广播时返回左矩阵广播后的shape, 只有在函数返回true即可广播时，这两个值才有效
    * rshape：同上
    */
    lshape.clear(), rshape.clear();

    auto riter = shape.rbegin();
    bool able = true;
    for(auto _riter = _shape.rbegin(); _riter != _shape.rend() && riter != shape.rend(); _riter ++, riter++) {
        int l = *riter, r = *_riter;
        if(*_riter != *riter) {
            if(*_riter == 1 || *riter == 1){ // 不同但有一个矩阵维度是1，可以广播
                if(*riter == 1)
                    l = *_riter;
                else
                    r = *riter;

            }
            else { // 不同，并且维度都不唯一，不可广播
                able = false;
                return able; // 不能广播直接返回
            }

        }
        lshape.push_back(l);
        rshape.push_back(r);
            
    } // for

    reverse(lshape.begin(), lshape.end());
    reverse(rshape.begin(), rshape.end());
    return able;
}


Mat::Mat(vect_double _data, vect_int _shape) {
    size = (int)_data.size();
    dimension = (int)_shape.size();
    if(size != get_length_from_shape(_shape)) {
        throw ShapeMatchError();
    }
    shape = _shape;
    data = _data;
}

vect_double Mat::flatten() {
    return this -> data;
} 

int  Mat::get_length_from_shape(vect_int s) {
    int _size = 1;
    int d = dimension;
    while(d)
        _size *= s[-- d];
    return _size;
}

bool Mat::shape_match(vect_int _shape, int _size) {
    /*
    * 传入shape是否与当前对象的shape一模一样
    * 
    */
    if(dimension !=  _size) {
        return false;
    }
    for(int i=0;i <  _size; i ++)
        if(shape[i] != _shape[i])
            return false;
    return true;
}

string Mat::shape_to_string(vect_int* _shape = NULL) {
    /**
     * 将shape转化为可打印的string
    */
    if(!_shape)
        _shape = &shape;
    string shape_str = "(";
    for(int i =0; i < dimension; i ++) {
        shape_str += to_string((*_shape)[i]);
        if(i < dimension - 1 || dimension == 1)
            shape_str +=  ", ";     
    }
    shape_str += ")\n";
    return shape_str;
}

void  Mat::print() {
    char shape_tmp[BUFSIZ] = {'\0'};
    char matrix_tmp[BUFSIZ * 2] = {'\0'};
    

    string matrix_str;
    string shape_str = shape_to_string();
    int _ = 0;
    matrix_to_string(matrix_str, 1, _, 0);
    strncpy(matrix_tmp, matrix_str.c_str(), strlen(matrix_str.c_str()));
    strncpy(shape_tmp, shape_str.c_str(), strlen(shape_str.c_str()));

    //IDEA:
    // 两次OCALL 时间开销大
    printf(matrix_tmp);
    printf(shape_tmp);
}

void  Mat::matrix_to_string(string& s, int dimension_level, int& ped, int c)
{
    /**
    * s:                temp string buffer to save print
    * dimension_level   current process level of dimension    
    * ped               current output index of raw data
    * c                 comma gate to control if the comma will be printed
    **/
    s += "[";
    int i = 0;
    int length = shape[dimension_level - 1];
    for(; i<length;i++){
        if(dimension_level == dimension){
            s += to_string(data[ped ++]);
            if (i < length - 1)
                s += ",";
        }
        else{
            int comma_gate = 0;
            if (i == length - 1)
                comma_gate = 1;
            matrix_to_string(s, dimension_level + 1, ped, comma_gate);
        }
    }
    s += "]";
    if(1 != dimension_level){ // 不是最后一个‘]’，结束后如果后面还有元素，则加上‘,\n’，如果后面没有元素则不加‘,\n’
        
        if (!c) s+= "," + get_n_linefeed(1 + dimension - dimension_level) + get_n_space(dimension_level); // c=1, 后面无元素，不用换行
    }
    else
        s += "\n";
}
 
 Mat  Mat::operator* (Mat rmatrix) {
    int rsize = rmatrix.size;
    int rdimension = rmatrix.dimension;
    vect_int rshape(rmatrix.shape);
    vect_double rdata = rmatrix.flatten();

    if(size != rsize || !shape_match(rshape, rdimension)){ // shape不同，检查是否可以广播
        vect_int bc_lshape, bc_rshape; //广播之后的两矩阵shape
        bool boardcastable = is_boardcastable(rshape, bc_lshape, bc_rshape);
        if(boardcastable) {
            //TODO: 执行广播操作
            printf("可以执行广播操作\n");
            printf("左矩阵变为：\n");
            printf(shape_to_string(&bc_lshape).c_str());
            printf("\n");
            printf("右矩阵变为：\n");
            printf(shape_to_string(&bc_lshape).c_str());
            printf("\n");
        }
        else { // TODO: 抛出乘法异常
            printf("无法进行广播\n");
            throw exception();
        }

    }
        
    vect_double data_tmp;
    vect_int shape_tmp(shape);
    for(int i = 0; i < size; i++)
        data_tmp.push_back(data[i] * rdata[i]);
    Mat mat(data_tmp, rshape);
    return mat;

}
/***************   Class implementation   *****************************/

void hello()
{

    vect_double v1 = {1, 2, 3, 4};
    vect_int  shape1 = {4, 1, 1};

    vect_double v2 = {1, 2, 3, 4};
    vect_int  shape2 = {1, 4, 1};
    try{
        Mat mat1(v1, shape1);
        Mat mat2(v2, shape2);
        
        Mat mat = mat1 * mat2;
        mat.print();
    }
       
    catch(exception& e){
        printf(e.what());
    }
    
}







