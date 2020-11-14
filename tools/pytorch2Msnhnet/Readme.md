# Pytorch2msnhnet
## Attention!
Alpha version, maybe have some bugs. Only official op is supported, customized op may have some bugs.

## Supported OP
-  conv2d
-  max_pool2d
-  avg_pool2d
-  adaptive_avg_pool2d
-  linear
-  flatten
-  dropout
-  batch_norm
-  interpolate(nearest, bilinear)
-  cat   
-  elu
-  selu
-  relu
-  relu6
-  leaky_relu
-  tanh
-  softmax
-  sigmoid
-  softplus
-  abs    
-  acos   
-  asin   
-  atan   
-  cos    
-  cosh   
-  sin    
-  sinh   
-  tan    
-  exp    
-  log    
-  log10  
-  mean
-  permute
-  view
-  contiguous
-  sqrt
-  pow
-  sum
-  pad
-  +|-|x|/|+=|-=|x=|/=|

## API
- translate pytorch to msnhnet and msnhbin.
    ```def trans(net, inputVar, msnhnet_path, msnhbin_path)```
## Example:
```# Pytorch2msnhnet
import torch
import torch.nn as nn
from torchvision.models import resnet18
from PytorchToMsnhnet import *

resnet18=resnet18(pretrained=True)
resnet18.eval()
input=torch.ones([1,3,224,224])
trans(resnet18, input,"resnet18.msnhnet","resnet18.msnhbin")
```
