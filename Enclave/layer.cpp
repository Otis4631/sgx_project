#include "layer.h"
#include "tools.h"
#include <mbusafecrt.h> // sprintf_s()


Mat pad(Mat& x, int pad_times, const double value, bool in_place) {
 if(x.dimension != 4) {
        printf("can not perform pad on dimension %d\n",x.dimension);
        throw exception();
    }
    Mat& res_mat = x;
    if(!in_place)
        Mat res_mat = x.copy();




    vect_int new_shape(res_mat.shape);
    for(auto t: range(pad_times)) {
        int insert_count = 0; //记录插入的数据

        for(auto m: range(res_mat.shape[0] + res_mat.shape[1]))
        {
            int fig_size = new_shape[2] * new_shape[3];
            int base_addr = m * fig_size + insert_count;
            res_mat.data.insert(res_mat.data.begin() + base_addr, new_shape.back() + 3, value); // 插入最上方 1
            insert_count +=  new_shape.back() + 3;
            // 插入两侧 2, 3
            int iter = base_addr + new_shape.back() + 3;
            int step = 0;
            int block = new_shape[3];
            for(auto h: range(new_shape[2])) {
                step += block;
                res_mat.data.insert(res_mat.data.begin() + iter + step, 2, value);
                insert_count += 2;
                step += 2; // step 用于指示当前与base的距离，每隔 block数插入2个value.
            }
            res_mat.data.insert(res_mat.data.begin() + iter + step, new_shape.back() + 1, value); // 4
            insert_count += new_shape.back() + 1;
            } // c, m
            new_shape[2] += 2;
            new_shape[3] += 2;
    } // t

    int new_size = get_length_from_shape(new_shape);
    if(res_mat.data.size() != new_size) {
        printf("data size error after option padding, data size is %d, new shape size is %d\n", res_mat.data.size(), new_size);
        throw exception();
    }
    res_mat.size = new_size;
    res_mat.shape = new_shape;
    return res_mat;
}


double Conv::_conv_single_step(Mat& a_slice_prev, int c) {
    if(a_slice_prev.dimension != 3) {
        printf("can not perform single step conv on dimension %d\n", a_slice_prev.dimension);
        throw exception();
    }
    Mat s = a_slice_prev * weight;
    double Z = s.sum();
    return Z;
}

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
    int out_C = x.shape[1]; 
    int out_H = 1 + (x.shape[2] - f) / stride;
    int out_W = 1 + (x.shape[3] - f) / stride;
    

    vect_int out_shape = {out_m, out_C, out_H, out_W};
    Mat res_mat = zeros(out_shape);

    int vert_start, vert_end, horiz_start, horiz_end;
    int res_index = 0;
    for(auto i: range(out_m))
        for(auto c: range(out_C))
            for(auto h: range(out_H))
                for(auto w: range(out_W)) {
                    vert_start = h * stride;
                    vert_end = vert_start + f;
                    horiz_start = w * stride;
                    horiz_end = horiz_start + f;
                    char char_indices[BUF_SIZE] = {'\0'};
                    sprintf_s(char_indices, BUF_SIZE, "%d,%d,%d:%d,%d:%d",
                     i, c, vert_start, vert_end, horiz_start, horiz_end);
                     Mat a_prev_slice = x[(const char*)char_indices];
                     if(mode == "max") 
                        res_mat.data[res_index ++] = a_prev_slice.max();
                     else
                        res_mat.data[res_index ++] = a_prev_slice.average();
                }
    return res_mat;
}


Conv::Conv(int _in_channels, int _out_channels, int _f, int _stride, int _padding = 0, Mat* kernel_data = NULL): 
    in_channels(_in_channels), out_channels(_out_channels), f(_f), stride(_stride), padding(_padding) {
        weight_shape = {out_channels, in_channels, f, f};
        bias_shape = {out_channels};
        bias = zeros(bias_shape);
        if(kernel_data) {
            if(get_length_from_shape(weight_shape) != kernel_data->size) {
                printf("shape of filter data does not match the parameters passed before\n");
                throw exception();
            }
            weight = *kernel_data;
        }
        
}
bool Conv::set_weights(vect_double& _weight) {
    if(_weight.size() != get_length_from_shape(weight_shape)) {
        printf("shape of weight you passed does not match the weight shape!\n");
        return false;
    }
    Mat res_mat(_weight, weight_shape);
    weight = res_mat;
    return true;
}

Mat Conv::forward(Mat& x) {
    if(x.dimension != 4) {
    printf("can not perform conv forward on dimension %d\n", x.dimension);
    throw exception();
    }
    int out_m = x.shape[0];
    int out_C = weight_shape[0]; //weight 的channel在第一维
    int out_H = 1 + (int)((x.shape[2] + 2 * padding - f) / stride);
    int out_W = 1 + (int)((x.shape[3] + 2 * padding - f) / stride);
    Mat Z = zeros({out_m, out_C, out_H, out_W});

    Mat A_prev_pad = pad(x, padding);
    int res_index = 0;
    for(auto i: range(out_m)) {
        Mat a_prev_pad = A_prev_pad[make_indices("%d,:,:,:", i)];
        for(auto c: range(out_C))
            for(auto h: range(out_H))
                for(auto w: range(out_W))
                 {
                    int vert_start = h * stride;
                    int vert_end = vert_start + f;
                    int horiz_start = w * stride;
                    int horiz_end = horiz_start + f;
                    Mat a_slice_prev = a_prev_pad[make_indices(":,%d:%d,%d:%d",vert_start,vert_end, horiz_start,horiz_end)];
                    Mat _w = weight[make_indices("%d,:,:,:", c)];
                    Z.data[res_index ++]  = (a_slice_prev * _w).sum() + bias.data[c];
                }
    }
    return Z;

}