#include "model.h"
#include "layer.h"
#include "tools.h"

Model::Model() {
        try{
            layers = *(new vector<Layer*>(19));
            layers[0] = (new Conv(3, 64, 11, 4, 2));
            layers[1] = (new Relu());
            layers[2] = (new Pool(3, 2));
            layers[3] = (new Conv(64, 192, 5, 1, 2));
            layers[4] = (new Relu());
            layers[5] = (new Pool(3, 2));
            layers[6] = new Conv(192, 384, 3, 1, 1);
            layers[7] = new Relu();
            layers[8] = new Conv(384, 256, 3, 1, 1);
            layers[9] = new Relu();
            layers[10] = new Conv(256, 256, 3, 1, 1);
            layers[11] = new Relu();
            layers[12] = new Pool(3, 2);

            layers[13] = new AdaptivePool({6,6}, "ave");
            
            //layers[14] = new Dropout()
            layers[14] = new Linear(9216, 4096);
            layers[15] = new Relu();
            layers[16] = new Linear(4096, 4096);
            layers[17] = new Relu();
            layers[18] = new Linear(4096, 10); 
            n_layers = layers.size();
        } 
        catch(bad_alloc &memExp)
        {
            printf("something goes wrong when alloc mermory\n");
            printf(memExp.what());
        }
}
Mat Model::forward(Mat& x) {
    printf("layer size: %d\n", layers.size());
    for(auto i: range(n_layers)) {
        printf("  %d: \n", i);
        x = layers[i]->forward(x);
        printf(x.shape_to_string().c_str());
        
    }
    return x;
}