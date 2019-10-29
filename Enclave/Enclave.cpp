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

#define BUF_SIZE 1023 * 1024 *  1024// 12M

/*************** Exception  Defination*****************************/
struct ShapeMatchError : public exception
{
  const char * what () const throw ()
  {
    return "ERROR: The shape you passed in does not match the size of the matrix\n";
  }
};
 


/*************** Tools   *****************************/
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

 
class Mat
{
    private:
        
        void print_matrix(string& s, int dimension_level, int& ped, int c);

        // 获取一位数组总长度
        

    public:
        int         dimension; // 维度
        int         size;      // 一位数组总长度
        int     *   shape;
        double  *   data;

        template<int data_size, int shape_size>
        Mat(double (&raw_data)[data_size], int (&s)[shape_size]);
        int get_length_from_shape(int *s);
        double* flatten();

        Mat operator* (Mat rmatrix);
        void print(); 
};

// class implementation
double* Mat::flatten() {
    return this -> data;
}


 
int  Mat::get_length_from_shape(int *s) {
    int _size = 1;
    int d = dimension;
    while(d)
        _size *= s[-- d];
    return _size;
}


template<int data_size, int shape_size>
 Mat::Mat(double (&raw_data)[data_size], int (&s)[shape_size]) {
    // 获取维度
    dimension = shape_size;

    //维度与传入矩阵不匹配抛出异常
    size = get_length_from_shape(s);
    
    if(size != data_size)
        throw ShapeMatchError();
    shape = s;
    data = raw_data;    
}

 
void  Mat::print() {
    char shape_tmp[BUFSIZ] = {'\0'};
    char matrix_tmp[BUFSIZ * 2] = {'\0'};
    string shape_str = "（";
    for(int i =0; i < dimension; i ++)
    {
        if(i < dimension - 1){
            shape_str += to_string(shape[i]) + ", ";
        }
        else {
            shape_str += to_string(shape[i]) + ")\n";
        }
             
    }

    string matrix_str;
    int _ = 0;
    print_matrix(matrix_str, 1, _, 0);
    strncpy(matrix_tmp, matrix_str.c_str(), strlen(matrix_str.c_str()));
    strncpy(shape_tmp, shape_str.c_str(), strlen(shape_str.c_str()));

    //TODO:
    // 两次OCALL 时间开销大
    printf(matrix_tmp);
    printf(shape_tmp);
}

 
void  Mat::print_matrix(string& s, int dimension_level, int& ped, int c)
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
            print_matrix(s, dimension_level + 1, ped, comma_gate);
        }
    }
    if(dimension_level == dimension){
        s += "]";
        if (!c) s+= ",\n";
    }
    else
        if(dimension_level == 1)
            s += "]\n";
        else if(1){
            s += "],\n\n";
        }
}
 
 Mat  Mat::operator* (Mat rmatrix) {
    int rsize = rmatrix.size;
    int rdimension = rmatrix.dimension;
    int* rshape = rmatrix.shape;
    double* rdata = rmatrix.flatten();

    if(size != rmatrix.get_length_from_shape(rshape))
        throw ShapeMatchError()
    double tmp[size];



}






void hello()
{
    int size = 8;
    double *v = new double[size]{ 1, 2, 3,
      4, 5, 6, 7, 8
    };
    int  shape[] = {2,2,2};



    try{
        Mat<double> mat(v, shape);
        mat.print();
    }
       
    catch(exception& e){
        printf(e.what());
    }
    
}







