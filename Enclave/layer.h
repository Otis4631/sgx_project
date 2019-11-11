#include "mat.h"

Mat pad(Mat& x, int pad_times, const double value = 0, bool in_place=false);

class Layer {
    /**
     * 层对象的基类
    */

    private:
       
    public:
        Mat weight;
        Mat bias;
        vect_int weight_shape;
        vect_int bias_shape;
        virtual Mat forward(Mat& x){};
        virtual ~Layer(){};
        
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

class Conv: public Layer {
    /**
     * 卷积层类声明
    */

    private:
        double _conv_single_step(Mat& a_slice_prev, int c);

    public:
        int in_channels;
        int out_channels;
        int f;
        int stride;
        int padding;
        Conv(int _in_channels, int _out_channels, int _f, int _stride, int _padding, Mat* kernel_data);
        Mat forward(Mat& x);
        bool set_weights(vect_double& _weight);


};

class Relu: public Layer {

    public:
        bool inplace;
        Relu(bool _inplace = true);
        forward(Mat& x);
}

