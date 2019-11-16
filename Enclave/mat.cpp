#include "mat.h"

#define PRECISON 4
#define P_DEEP 5

int  get_length_from_shape(vect_int& s) {
    int _size = 1;
    int d = (int)s.size();
    while(d)
        _size *= s[-- d];
    return _size;
}
Mat zeros(vect_int shape) {
    int size = (int)get_length_from_shape(shape);
    vect_double _data(size, 0);
    return Mat(_data, shape);
}
Mat ones(vect_int shape) {
    int size = (int)get_length_from_shape(shape);
    vect_double _data(size, 1);
    return Mat(_data, shape);
}

Mat randn(vect_int shape, const vect_int intervals) {
    vect_double data;
    for(auto i: range(get_length_from_shape(shape))) {
        double res = rand_double(intervals);
        data.push_back(res);
    }
    return Mat(data, shape);
}
/*************** Exception  Defination*****************************/
struct ShapeMatchError : public exception
{
  const char * what () const throw ()
  {
    return "ERROR: The shape you passed in does not match the size of the matrix\n";
  }
};

/***************   Class implementation   *****************************/

Mat Mat::dot2d(Mat& rmat) {
    /**
     * 当前矩阵与传入矩阵做矩阵乘法
    */
    if(dimension != 2 || dimension != 2) {
       // TODO: Finish defination of exception 
       printf("only support 2d matrix!\n");
       throw exception();
   }
    if(shape[1] != rmat.shape[0]) {
        // TODO: Finish defination of exception 
       printf("shape(%d, %d) and shape(%d, %d) not aligned!", shape[0], shape[1], rmat.shape[0], rmat.shape[1]);
       throw exception();
    }
    int junction = shape[1];
    int new_size = shape[0] * rmat.shape[1];
    double array_tmp[shape[0]][rmat.shape[1]] = { 0 };
    for(int row = 0; row < shape[0]; row ++) {
        for(int col = 0; col < rmat.shape[1]; col ++) 
            for(int joint = 0; joint < junction; joint ++) {
                array_tmp[row][col] += data[row * junction + joint] * rmat.data[col + joint * rmat.shape[1]];
            }
    }
    double *one = (double*)array_tmp;
    vect_double data_tmp(one, one + new_size);
    vect_int shape_tmp = {shape[0], rmat.shape[1]};
    return Mat(data_tmp, shape_tmp);


}

Mat Mat::copy() {
    /**
     * 返回当前矩阵的复制
    */
   Mat mat_tmp; //pBUG: 内存分配失败.
   mat_tmp.data = this -> data;
   mat_tmp.size = this -> size;
   mat_tmp.shape = this -> shape;
   mat_tmp.dimension = this -> dimension;

   return mat_tmp;
}

void Mat::_indices_new_shape(vector<string>& s, vect_int& res_shape, vector<vect_int>& interval) {
    int iter_shape = -1;
    for(auto index_string : s) {
        iter_shape ++;
        int start_pos = -1;
        int end_pos = -1;
        vect_int array_interval(2); // 记录区间的数组
        vector<string> split_index = split(index_string.c_str(), ":");
        if(split_index.size() == 1) {  // 只有一个，索引值或起始区间
            auto pos = index_string.find(":");
            if(pos != index_string.npos) { // 起始区间到结束区间
                if((int)pos == 0) { // 指明结束区间
                    start_pos = 0;
                    end_pos = stoi(split_index[0]) - 1;
                } 
                else {
                    start_pos = stoi(split_index[0]); //开始区间
                    end_pos = this -> shape[iter_shape] - 1; // 结束区间
                }
                res_shape.push_back(end_pos - start_pos + 1); // 新矩阵的shape 
                array_interval[0] = start_pos; 
                array_interval[1] = end_pos;
                interval.push_back(array_interval);
            }
            else { // 索引值
            start_pos = stoi(split_index[0]);
            end_pos = stoi(split_index[0]);
            array_interval[0] = start_pos; 
            array_interval[1] = end_pos;
            interval.push_back(array_interval);

            //res_shape.push_back(1);
            }
        }
        else if(split_index.size() == 0) { // 全部区间
            res_shape.push_back(shape[iter_shape]);
            start_pos = 0;
            end_pos = shape[iter_shape] - 1;
            array_interval[0] = start_pos; 
            array_interval[1] = end_pos;
            interval.push_back(array_interval);

        } 
        else { // 指明区间
            start_pos = stoi(split_index[0]);
            end_pos = stoi(split_index[1]) - 1;
            if(start_pos > end_pos) {
                //TODO: exception
                printf("start position should less than end position!\n");
                throw exception();
            }
            int new_length = end_pos - start_pos + 1;
            if(new_length > shape[iter_shape]) {
                printf("interval is longer than origin shape!\n");
                throw exception();
            }
            res_shape.push_back(new_length);
            array_interval[0] = start_pos;
            array_interval[1] = end_pos;
            interval.push_back(array_interval);
        }
    }
}

void Mat::expand_from_block(vect_double& data_tmp, int block, int times) {
    /**
     * 将数组按block从头到尾扩展
    */
   int _size = (int)data_tmp.size();
   vect_double source_data(data_tmp);
    for(int old_i = 0; old_i < _size;) {
        for(int t = 0; t < times; t ++) {
            for(int block_i = 0; block_i < block; block_i ++) {
                data_tmp.insert(data_tmp.begin() + old_i + block + block_i, data_tmp[(old_i + block_i)]); // 取余数：限定在源数据空间内，old_i随插入数据增长，不适合直接指示源数据。
            }
            old_i += block;
        }

        _size += block * times;
        old_i += block;
    }
}

Mat Mat::broadcast(vect_int& new_shape) {
    /***
     * 将当前矩阵广播至新的矩阵
     * new_shape: 广播后的新shape
     *  插入操作会使原迭代器失效，改为索引实现
     */
    int new_size = (int)(get_length_from_shape(new_shape));
    int expand_times = new_size / size;
    if(expand_times <= 1) 
        return *(this);

    vect_double data_tmp(data); //pBUG: 扩展空间不足
    vect_int old_shape(shape);
    int old_iter = (int)old_shape.size() - 1;
    int new_iter = (int)new_shape.size() - 1;
    while(new_iter != -1) { //遍历完所有的新的维度
        if(old_iter == -1 || (old_shape[old_iter] == 1 && new_shape[new_iter] != 1)) { // 不存在旧维度或者为1，进行扩展
            int block = 1;
            auto p = old_iter;
            while(p < (int)old_shape.size()) { // 计算block
                    if(p >= 0)
                        block *= old_shape[p];
                    p ++;
            }
            expand_from_block(data_tmp, block, new_shape[new_iter] - 1);
            if(old_iter == -1)
                old_shape.insert(old_shape.begin(), new_shape[new_iter]);

            else
                old_shape[old_iter] = new_shape[new_iter]; 
        }
        new_iter --;
         if(old_iter > -1)
                old_iter --;
    } // while

    if((int)data_tmp.size() != new_size) {
        printf("扩展完成后尺寸不匹配，%d\n", data_tmp.size());
        throw exception(); //TODO: exception handler
    }
    Mat mat_tmp;
    mat_tmp.data = data_tmp;
    mat_tmp.shape = new_shape;
    mat_tmp.size = new_size;
    mat_tmp.dimension = (int)new_shape.size();
    return mat_tmp;
}

bool Mat::is_broadcastable(vect_int _shape, vect_int& lshape, vect_int& rshape) {
    /*
    * _shape：右矩阵shape
    * lshape：可广播时返回左矩阵广播后的shape, 只有在函数返回true即可广播时，这两个值才有效
    * rshape：同上
    */
    lshape.clear(), rshape.clear();
    bool able = true;
    auto _riter = _shape.rbegin();
    auto riter = shape.rbegin();
    for(; _riter != _shape.rend() && riter != shape.rend(); _riter ++, riter++) {
        int l = *riter, r = *_riter;
        if(*_riter != *riter) { // 不同
            if(*_riter == 1 || *riter == 1){ // 但有一个矩阵维度是1，可以广播
                if(*riter == 1)
                    l = *_riter;
                else
                    r = *riter;
            }
            else { // 不同，并且各个维度的长度都不为1，不可广播
                able = false;
                return able; // 不能广播直接返回
            }

        }
        lshape.push_back(l);
        rshape.push_back(r);
    } // for
        /**
         * 维度扩充：低维度的矩阵复制高纬度矩阵对应轴上的长度
         * example:
         * (3,2) * (2,) -> (3,2)
         * (3,3,2) * (1,2) -> (3,3,2)
         */
    while(_riter != _shape.rend()) {
        
        lshape.push_back(*_riter);
        rshape.push_back(*_riter);
        _riter ++;    
    }

    while(riter != shape.rend()) { 
        lshape.push_back(*riter);
        rshape.push_back(*riter);
        riter ++; 
    }
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
   int d;
    if(_shape) {
        d = (int)(*_shape).size();
    } else {
        _shape = &(shape);
        d = dimension;
    }
    string shape_str = "(";
    for(int i =0; i < d; i ++) {
        shape_str += to_string((*_shape)[i]);
        if(i < d - 1 || d == 1)
            shape_str +=  ", ";     
    }
    shape_str += ")\n\n";
    return shape_str;
}

void  Mat::print() {
    char shape_tmp[BUF_SIZE] = {'\0'};
    char matrix_tmp[BUF_SIZE * 10] = {'\0'};
    string matrix_str;
    string shape_str = shape_to_string();
    int _ = 0;
    matrix_to_string(matrix_str, 1, _, 0);
    strncpy(matrix_tmp, matrix_str.c_str(), strlen(matrix_str.c_str()));
    strncpy(shape_tmp, shape_str.c_str(), strlen(shape_str.c_str()));

    //IDEA:
    // 多次OCALL 时间开销大
    printf(matrix_tmp);
    printf(", with shape: ");
    printf(shape_tmp);
}
void  Mat::matrix_to_string(string& s, int dimension_level, int& ped, int c) {
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
            if(i> P_DEEP && i < length - P_DEEP) {
                ped ++;
                continue;
            }
            string tab = "\t";
            string _double_str = tab + to_string(data[ped ++]) ;
            set_precison(_double_str, PRECISON);
            s += _double_str;
            if (i < length - 1)
                s += ", ";
        }
        else {
                if(i > P_DEEP && i < length - P_DEEP) {
                ped += shape[dimension_level];
                continue;
            }
            int comma_gate = 0;
            if (i == length - 1)
                comma_gate = 1;
            matrix_to_string(s, dimension_level + 1, ped, comma_gate);
        }
    }
    s += "]";
    if(1 != dimension_level){ // 不是最后一个‘]’，结束后如果后面还有元素，则加上‘,\n’，如果后面没有元素则不加‘,\n’
        
        if (!c) s+= ", " + get_n_linefeed(1 + dimension - dimension_level) + get_n_space(dimension_level - 1); // c=1, 后面无元素，不用换行
    }
    else
        s += "";
}

void Mat::_partition(vect_double& new_data, vector<vect_int>& intervals, int level, int base, int end) {
    if(level >= dimension)
        return ;
    int block_size = (end - base + 1) / shape[level];
    int n_block = shape[level];
    for(int block_i = 0; block_i < n_block; block_i ++) {
        int interval_start = intervals[level][0];
        int interval_end = intervals[level][1];
        if(block_i >= interval_start && block_i <= interval_end) {
            if(level == dimension - 1) {
                new_data.push_back(data[base + block_i]);
            }
            else {
                _partition(new_data, intervals, level + 1, base + block_i * block_size, base + (block_i + 1) * block_size);
            }
        }
    }
}

Mat Mat::_slice(vect_int& new_shape, vector<vect_int>& intervals) {
    vect_double new_data;
    _partition(new_data, intervals, 0, 0, size - 1);
    // for(auto iter = new_shape.begin(); iter != new_shape.end(); iter ++) {
    //     if(*iter == 1) {
    //         if(new_shape.size() < 2)
    //             break;
    //         new_shape.erase(iter);
    //         iter = new_shape.begin();
    //     }
    // }
    Mat res_mat(new_data, new_shape);
    return res_mat;
}

   /*****Operator Overloading****/
Mat Mat::operator* (Mat& rmatrix) {
    int rsize = rmatrix.size;
    int rdimension = rmatrix.dimension;
    vect_int rshape(rmatrix.shape);
    vect_int res_shape(shape);
    Mat lmat = *this;
    Mat rmat = rmatrix;

    if(size != rsize || !shape_match(rshape, rdimension)){ // shape不同，检查是否可以广播
        vect_int bc_lshape, bc_rshape; //广播之后的两矩阵shape
        bool broadcastable = is_broadcastable(rshape, bc_lshape, bc_rshape);
        res_shape = bc_lshape;
        if(broadcastable) {
             
            lmat = broadcast(bc_lshape);
            rmat = rmatrix.broadcast(bc_rshape);
            //
        }
        else { // TODO: 抛出乘法异常
            printf("无法进行广播\n");
            throw exception();
        }
    }
    vect_double data_tmp;
    vect_int shape_tmp(res_shape);
    int res_size = get_length_from_shape(res_shape);
    for(int i = 0; i < res_size; i++)
        data_tmp.push_back(lmat.data[i] * rmat.data[i]);
    Mat mat(data_tmp, res_shape);
    return mat;
}
Mat Mat::operator/ (Mat& rmatrix) {
    int rsize = rmatrix.size;
    int rdimension = rmatrix.dimension;
    vect_int rshape(rmatrix.shape);
    vect_int res_shape(shape);
    Mat lmat = *this;
    Mat rmat = rmatrix;

    if(size != rsize || !shape_match(rshape, rdimension)){ // shape不同，检查是否可以广播
        vect_int bc_lshape, bc_rshape; //广播之后的两矩阵shape
        bool broadcastable = is_broadcastable(rshape, bc_lshape, bc_rshape);
        res_shape = bc_lshape;
        if(broadcastable) {
             
            lmat = broadcast(bc_lshape);
            rmat = rmatrix.broadcast(bc_rshape);
            //
        }
        else { // TODO: 抛出乘法异常
            printf("无法进行广播\n");
            throw exception();
        }
    }
    vect_double data_tmp;
    vect_int shape_tmp(res_shape);
    int res_size = get_length_from_shape(res_shape);
    for(int i = 0; i < res_size; i++)
        data_tmp.push_back(lmat.data[i] / rmat.data[i]);
    Mat mat(data_tmp, res_shape);
    return mat;
}
Mat Mat::operator+ (Mat& rmatrix) {
        int rsize = rmatrix.size;
    int rdimension = rmatrix.dimension;
    vect_int rshape(rmatrix.shape);
    vect_int res_shape(shape);
    Mat lmat = *this;
    Mat rmat = rmatrix;

    if(size != rsize || !shape_match(rshape, rdimension)){ // shape不同，检查是否可以广播
        vect_int bc_lshape, bc_rshape; //广播之后的两矩阵shape
        bool broadcastable = is_broadcastable(rshape, bc_lshape, bc_rshape);
        res_shape = bc_lshape;
        if(broadcastable) {
             
            lmat = broadcast(bc_lshape);
            rmat = rmatrix.broadcast(bc_rshape);
            //
        }
        else { // TODO: 抛出乘法异常
            printf("无法进行广播\n");
            throw exception();
        }
    }
    vect_double data_tmp;
    vect_int shape_tmp(res_shape);
    int res_size = get_length_from_shape(res_shape);
    for(int i = 0; i < res_size; i++)
        data_tmp.push_back(lmat.data[i] + rmat.data[i]);
    Mat mat(data_tmp, res_shape);
    return mat;
}
Mat Mat::operator- (Mat& rmatrix) {
    int rsize = rmatrix.size;
    int rdimension = rmatrix.dimension;
    vect_int rshape(rmatrix.shape);
    vect_int res_shape(shape);
    Mat lmat = *this;
    Mat rmat = rmatrix;

    if(size != rsize || !shape_match(rshape, rdimension)){ // shape不同，检查是否可以广播
        vect_int bc_lshape, bc_rshape; //广播之后的两矩阵shape
        bool broadcastable = is_broadcastable(rshape, bc_lshape, bc_rshape);
        res_shape = bc_lshape;
        if(broadcastable) {
             
            lmat = broadcast(bc_lshape);
            rmat = rmatrix.broadcast(bc_rshape);
            //
        }
        else { // TODO: 抛出乘法异常
            printf("无法进行广播\n");
            throw exception();
        }
    }
    vect_double data_tmp;
    vect_int shape_tmp(res_shape);
    int res_size = get_length_from_shape(res_shape);
    for(int i = 0; i < res_size; i++)
        data_tmp.push_back(lmat.data[i] - rmat.data[i]);
    Mat mat(data_tmp, res_shape);
    return mat;
}
Mat Mat::operator[](const char* s) {
    vector<string> indices = split(s, ",");

    if(indices.size() != dimension) {
        //TODO:expection
        printf("can't reslove indices strings\n");
        throw exception();
    }
    vect_int new_shape;
    vector<vect_int> intervals;
    _indices_new_shape(indices, new_shape, intervals); //获取切片后的shape和interval

    Mat res_mat = _slice(new_shape, intervals);
    //delete[] s;
    //s = NULL; 
    return res_mat;
}

double Mat::max() {
    double _m = data[0];
    for(auto i : data) {
        if(i > _m)
            _m = i;
    }
    return _m;
}

double Mat::sum() {
    int res = 0;
    for(auto i : data) {
        res += i;
    }
    return res;
}
double Mat::average() {
    return sum() / (size * 1.0);
}


   /*****Operator Overloading****/


/***************   Class implementation   *****************************/
