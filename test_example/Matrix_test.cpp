#include <iostream>
#include <vector>

using namespace std;

typedef vector<double>  vect_double;
typedef vector<int>     vect_int; 



int main() {
/********* test case 1 ***********/
// 双向扩展
    vect_double v1 = {1, 2, 3, 4, 5, 6};
    vect_int  shape1 = {3, 2, 1};
    vect_double v2 = {1, 2, 3, 4, 5, 6};;
    vect_int  shape2 = {1,2,3};

    Mat mat2(v2, shape2);
    Mat mat1(v1, shape1);
    Mat mat = mat1 - mat2;

/******Except: ***********
    [[ 0, -1, -2],
        [-2, -3, -4]],

       [[ 2,  1,  0],
        [ 0, -1, -2]],

       [[ 4,  3,  2],
        [ 2,  1,  0]]]
****/

/********* test case 2 ***********/
// 右侧维度增加
    vect_double v1 = {1, 2, 3, 4, 5, 6};
    vect_int  shape1 = {3, 2};
    vect_double v2 = {1, 2};
    vect_int  shape2 = {2};

    Mat mat2(v2, shape2);
    Mat mat1(v1, shape1);
    Mat mat = mat1 - mat2;

/******Except: ***********
[[0.00,0.00],
  [2.00,2.00],
  [4.00,4.00]], with shape: (3, 2)
****/

/********* test case 2 ***********/
// 左侧维度增加
    vect_double v1 = {1, 2, 3, 4, 5, 6};
    vect_int  shape1 = {3, 2};
    vect_double v2 = {1, 2};
    vect_int  shape2 = {2};

    Mat mat2(v2, shape2);
    Mat mat1(v1, shape1);
    Mat mat = mat2 - mat1;

/******Except: ***********
[[0.00,0.00],
  [2.00,2.00],
  [4.00,4.00]], with shape: (3, 2)
****/

/********* test case 3 ***********/
// dot 矩阵乘法测试
  vect_double v1 = {1, 2, 3, 4, 5, 6};
    vect_int  shape1 = {3, 2};
    vect_double v2 = {1,2};
    vect_int  shape2 = {2,1};

    try{
    Mat mat1(v1, shape1);
    Mat mat2(v2, shape2);
    Mat mat = mat1.dot2d(mat2);
    mat.print();
/******Except: ***********
[[5.00],
 [11.00],
 [17.00]], with shape: (3, 1)
****/

/********* test case 4 ***********/
// slice 测试
vect_double data = {1,2,3,4,5,6,7,8,9,10,11,12,
                        1,2,3,4,5,6,7,8,9,10,11,12};
    vect_int shape = {3,2,2,2};
    
    vect_double data2 = {1,2};
    vect_int shape2 = {2};

    Mat x(data, shape);

    x.print();
    x["1:,1,:1, 0: 1"];

/******Except: ***********

[[[[1, 2],
   [3, 4]],

  [[5, 6],
   [7, 8]]],


 [[[9, 10],
   [11, 12]],

  [[1, 2],
   [3, 4]]],


 [[[5, 6],
   [7, 8]],

  [[9, 10],
   [11, 12]]]], with shape: (3, 2, 2, 2)

[[[1]],

 [[9]]], with shape: (2, 1, 1)

****/

}