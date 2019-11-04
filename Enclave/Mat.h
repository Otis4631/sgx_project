#ifndef __MAT_H__
#define __MAT_H__

#include <string.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <vector>
#include <exception>
#include <iostream>
#include <tools.h>

using namespace std;

typedef vector<double>  vect_double;
typedef vector<int>     vect_int; 


/***************   Class defination   *****************************/
class Mat
{
    private:
        void matrix_to_string(string& s, int dimension_level, int& ped, int c);
        bool is_broadcastable(vect_int _shape, vect_int& lshape, vect_int& rshape);
        void expand_from_block(vect_double& data_tmp, int block, int times);
        // 获取一位数组总长度
        

    public:
        int             dimension;  // 维度
        int             size;       // 一位数组总长度
        vector<int>     shape;      // shape
        vect_double     data;

        Mat copy();
        Mat dot2d(Mat& rmat);
        Mat broadcast(vect_int& new_shape); 
        string shape_to_string(vect_int* _shape);
        bool shape_match(vect_int _shape, int _size);
        Mat(){};
        Mat(vect_double _data, vector<int> _shape);

        int get_length_from_shape(vect_int& s);
        vect_double flatten();

        Mat operator* (Mat rmatrix);
        Mat operator/ (Mat rmatrix);
        Mat operator+ (Mat rmatrix);
        Mat operator- (Mat rmatrix);

        Mat dot(Mat& rmat); //矩阵乘法
        void print(); 
};
/***************   Class defination   *****************************/

#endif