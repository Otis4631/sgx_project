
#include "mat.h"
class Layer {
    /**
     * 层对象的抽象类
    */
    public:
        Mat weight;
        Mat bias;
        vect_int weight_shape;
        vect_int bias_shape;
        virtual Mat forward(Mat& x){};
        //~Layer(){};
};

class Linear: public Layer {
    /**
     * Linear层类声明 
    */
     public:
        Linear(int in_f, int out_f, bool=true); // 默认参数不能同时出现在定义与声明中
        int in_features;
        int out_features;
        bool have_bias;
        Mat forward(Mat& x);
};
class Pool: public Layer {
    /**
     * 池化层类声明 
     */
    public:
        Pool(int _f, int _stride, string _mode="max"):
            f(_f), stride(_stride), mode(_mode){}
        Mat forward(Mat& x);
        
        private:
            int f, stride;
            string mode;
            vect_int out_shape;
};