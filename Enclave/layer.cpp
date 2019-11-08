
#include "layer.h"
#include "tools.h"
#include <mbusafecrt.h> // sprintf_s()


Linear::Linear(int in_f, int out_f, bool bi): 
            in_features(in_f), out_features(out_f), have_bias(bi) {
    weight_shape.push_back(in_features);
    weight_shape.push_back(out_features);

    weight = zeros(weight_shape); //TODO: choose best weight initializer
    weight.print();
    if(have_bias) {
        bias_shape.push_back(out_f);
        bias = zeros(bias_shape);
    }
}

Mat Linear::forward(Mat& x) {
    if(x.shape[1] != weight_shape[0]) {
        printf("can't forward due to not matched shape, x: %d != weight: %d\n", x.shape[1], weight_shape[0]);
        throw exception(); // TODO: exception
    }
    Mat res_mat = x.dot2d(weight);
    return res_mat;
}


Mat Pool::forward(Mat& x) {
    if(x.dimension != 4) {
        // TODO: exception
        printf("can not perform pooling forward on dimension %d\n", x.dimension);
        throw exception();
    }

    int out_m = x.shape[0];
    int out_H = 1 + (x.shape[1] - f) / stride;
    int out_W = 1 + (x.shape[2] - f) / stride;
    int out_C = x.shape[3]; 

    vect_int out_shape = {out_m, out_H, out_W, out_C};
    Mat res_mat = zeros(out_shape);

    int vert_start, vert_end, horiz_start, horiz_end;
    for(auto i: range(out_m))
        for(auto h: range(out_H))
            for(auto w: range(out_W))
                for(auto c: range(out_C)) {
                    vert_start = h * stride;
                    vert_end = vert_start + f;
                    horiz_start = w * stride;
                    horiz_end = horiz_start + f;
                    char char_indices[BUF_SIZE] = {'\0'};
                    sprintf_s(char_indices, BUF_SIZE, "%d,%d:%d,%d:%d,%d",
                     i, vert_start, vert_end, horiz_start, horiz_end, c);
                     Mat a_prev_slice = x[(const char*)char_indices];
                     if(mode == "max") 
                        res_mat.data[i * out_m + h * out_H + w * out_W + c * out_C] = a_prev_slice.max();
                     else
                        res_mat.data[i * out_m + h * out_H + w * out_W + c * out_C] = a_prev_slice.average();
                }
    return res_mat;
}


