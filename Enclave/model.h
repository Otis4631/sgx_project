#ifndef _MODEL_H_
#define _MODEL_H_
#include "mat.h"
#include "layer.h"
#include "tools.h"
#include <vector>

using namespace std;
class Model {
    private:
        vector<Layer*> layers;
        size_t n_layers;
        
    public:
        //Model(vector<Layer*> _layers, size_t _n_layers);
        Model();
        Mat forward(Mat& x);
    
};

#endif