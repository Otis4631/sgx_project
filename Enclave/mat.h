#ifndef _MAT_H_
#define _MAT_H_

#include <string.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <vector>
#include <exception>
#include <iostream>
#include "tools.h"

using namespace std;

typedef vector<double>  vect_double;
typedef vector<int>     vect_int; 

int get_length_from_shape(vect_int& s);

/***************   Class defination   *****************************/
class Mat
{
    private:
        void _indices_new_shape(vector<string>& s, vect_int& res_shape, vector<vect_int>& interval);
        void _partition(vect_double& new_data, vector<vect_int>& intervals, int level, int base, int end, int& count);
        Mat _slice(vect_int& new_shape, vector<vect_int>& intervals);
        // 获取切片后的矩阵shape以及区间
        void matrix_to_string(string& s, int dimension_level, int& ped, int c);
        bool is_broadcastable(vect_int _shape, vect_int& lshape, vect_int& rshape);
        void expand_from_block(vect_double& data_tmp, int block, int times);
        // 获取一位数组总长度
        

    public:
        int             dimension;  // 维度
        int             size;       // 一位数组总长度
        vector<int>     shape;      // shape
        vect_double     data;

        double sum();
        double average();
        double max();

        Mat copy();
        Mat dot2d(Mat& rmat);
        Mat broadcast(vect_int& new_shape); 
        string shape_to_string(vect_int* _shape=NULL);
        bool shape_match(vect_int _shape, int _size);
        int reshape(vect_int new_shape);
        Mat(){};

        Mat(vect_double _data, vector<int> _shape);

        Mat operator* (Mat& rmatrix);
        Mat operator/ (Mat& rmatrix);
        Mat operator+ (Mat& rmatrix);
        Mat operator- (Mat& rmatrix);
        Mat operator[](const char* s);
        Mat operator[](string str);

        Mat dot(Mat& rmat); //矩阵乘法
        void print(); 
};
/***************   Class defination   *****************************/

Mat zeros(vect_int shape);
Mat randn(vect_int shape, const vect_int intervals = {});

#endif