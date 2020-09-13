import torch
import torch.nn as nn
import torch.nn.functional as F
from torch.autograd import Variable
from torch.nn.modules.utils import _pair
import numpy as np
from MsnhBuilder import Msnhnet
import sys
from struct import pack

msnhnet = Msnhnet()
ccc = []
index   = 0

class Hook(object):
    hookInited = False
    def __init__(self,raw,replace,**kwargs):
        self.obj=replace
        self.raw=raw

    def __call__(self,*args,**kwargs):
        if not Hook.hookInited:
            return self.raw(*args,**kwargs)
        else:
            out=self.obj(self.raw,*args,**kwargs)
            return out

def log(*args):
    print(*args)


def _conv2d(raw,inData, weight, bias=None, stride=1, padding=0, dilation=1, groups=1):
    log( "conv2d-i" , inData._cdata)
    x=raw(inData,weight,bias,stride,padding,dilation,groups)
    ccc.append(x)
    log( "conv2d-o" , x._cdata)

    useBias = True
    if bias== None:
        useBias = False

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildConv2d(str(x._cdata), x.size()[1], weight.size()[2], weight.size()[3], 
                        padding[0], padding[1], stride[0], stride[1], dilation[0], dilation[1], groups, useBias)
    return x

def _max_pool2d(raw,inData, kernel_size, stride=None, padding=0, dilation=1,
               ceil_mode=False, return_indices=False):
    log( "max2d-i" , inData._cdata)
    x = raw(inData, kernel_size, stride, padding, dilation,ceil_mode, return_indices)
    ccc.append(x)
    
    ceilMode = ceil_mode

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildPooling(str(x._cdata), "MAX", kernel_size, kernel_size, stride, stride, 
                            padding, padding, ceilMode)

    log( "max2d-o" , x._cdata)
    return x

def _avg_pool2d(raw,inData, kernel_size, stride = None, padding = 0, ceil_mode = False, count_include_pad = True):
    log( "avg2d-i" , inData._cdata)
    x = raw(inData, kernel_size, stride, padding, ceil_mode, count_include_pad)
    ccc.append(x)

    ceilMode = ceil_mode

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildPooling(str(x._cdata), "AVE", kernel_size, kernel_size, stride, stride, 
                            padding, padding, ceilMode)
    log( "avg2d-o" , x._cdata)
    return x

def _adaptive_avg_pool2d(raw, inData, output_size):
    log( "adaptAvg2d-i" , inData._cdata)
    x = raw(inData, output_size)
    ccc.append(x)

    if isinstance(output_size, int):
        out_dim = output_size
    else:
        out_dim = output_size[0]

    tmp = max(inData.shape[2], inData.shape[3])
    stride = tmp //out_dim
    kernel_size = tmp - (out_dim - 1) * stride

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildPooling(str(x._cdata), "AVE", kernel_size, kernel_size, stride, stride, 
                        0, 0, False)
    ccc.append(x)
    log( "adaptAvg2d-o" , x._cdata)
    return x

def _linear(raw,inData, weight, bias=None):
    log( "fc-i" , inData._cdata)
    x=raw(inData,weight,bias)
    
    useBias = True
    if bias== None:
        useBias = False

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildConnect(str(x._cdata), x.size()[1], useBias)

    ccc.append(x)
    log( "fc-o" , x._cdata)
    return x

def _flatten(raw,*args):
    log( "flatten-i" , args[0]._cdata)
    x=raw(*args)
    ccc.append(x)
    log( "flatten-o" , x._cdata)

    key = msnhnet.getLastKey()
    val = msnhnet.name_index_dict[key]
    msnhnet.name_index_dict.pop(key)
    msnhnet.name_index_dict[str(x._cdata)] = val
    return x

def _cat(raw,inputs, dim=0):
    k = 0
    layers = ""
    for input in inputs:
        log( "cat"+str(k)+"-i" , input._cdata)
        layers = layers + str(msnhnet.name_index_dict[str(input._cdata)]) + ","
    x=raw(inputs, dim)
    ccc.append(x)
    log( "cat-o" , x._cdata)
    layers = layers[:-1]
    if dim != 1:
        raise NotImplementedError("cat only supported with dim 1")

    msnhnet.buildRoute(str(x._cdata),layers,False)
    return x

def _dropout(raw,*args):
    log( "dropout-i" , args[0]._cdata)
    x=raw(*args)
    ccc.append(x)

    key = msnhnet.getLastKey()
    val = msnhnet.name_index_dict[key]
    msnhnet.name_index_dict.pop(key)
    msnhnet.name_index_dict[str(x._cdata)] = val

    log( "dropout-o" , x._cdata)
    return x

def _batch_norm(raw,inData, running_mean, running_var, weight=None, bias=None,
               training=False, momentum=0.1, eps=1e-5):
    log( "bn-i" , inData._cdata)
    x = raw(inData, running_mean, running_var, weight, bias, training, momentum, eps)
    ccc.append(x)
    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildBatchNorm(str(x._cdata))
    log( "bn-o" , x._cdata)
    return x

def _interpolate(raw, inData,size=None, scale_factor=None, mode='nearest', align_corners=None):
    # for nearest _interpolate
    
    log( "upsample-i" , inData._cdata)
    x = raw(inData,size , scale_factor ,mode, align_corners)
    ccc.append(x)
    log( "upsample-o" , x._cdata)

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    
    if mode == "nearest" or align_corners == None:
        if size == None and scale_factor != None:
            
            if 10*int(scale_factor) != int(10*scale_factor) :
                raise NotImplementedError("scale must be int")

            strideX = scale_factor
            strideY = scale_factor

            msnhnet.buildUpsample2D(str(x._cdata), strideX, strideY, 1,1, "nearest", 0) 
        elif scale_factor == None and size != None:
            mx = inData.shape[-1]
            my = inData.shape[-2]

            
            if size[0]%x != 0 or size[1]%y != 0 :
                raise NotImplementedError("scale must be int")
            
            strideX = size[0]/mx
            strideY = size[1]/my

            msnhnet.buildUpsample2D(str(x._cdata), strideX, strideY, 1, 1,"nearest",0) 
        else:
            raise NotImplementedError("upsample params error")
    elif mode == "bilinear" :
        if align_corners == None:
            alignCorners = 0
        else:
            if align_corners == True :
                alignCorners = 1
            else:
                alignCorners = 0

        if size == None and scale_factor != None:
            msnhnet.buildUpsample2D(str(x._cdata), 1, 1, scale_factor,scale_factor,"bilinear",alignCorners) 
        elif scale_factor == None and size != None:
            mx = inData.shape[-1]
            my = inData.shape[-2]

            scaleX = size[0]/mx 
            scaleY = size[1]/my 
            msnhnet.buildUpsample2D(str(x._cdata), 1, 1, scaleX,scaleY,"bilinear",alignCorners)
        else:
            raise NotImplementedError("upsample params error")
    else:
        raise NotImplementedError("unsupported type, only nearest/bilinear is supported")

    #print(msnhnet.net)
    return x

def _softmax(raw, inData, dim=None, _stacklevel=3):
    log( "softmax-i" , inData._cdata)
    if dim != None:
        raise NotImplementedError("Soft max not supported yet")
    x=raw(inData, dim=dim)
    ccc.append(x)

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildSoftmax(str(x._cdata))
    log( "softmax-o" , x._cdata)
    return x

# =====  Activation ======
def _elu(raw, inData, inplace=False):
    log( "elu-i" , inData._cdata)
    x = raw(inData,False)
    ccc.append(x)
    log( "elu-o" , x._cdata)

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildActivation(str(x._cdata),"elu")
    return x

def _selu(raw, inData, inplace=False):
    log( "selu-i" , inData._cdata)
    x = raw(inData,False)
    ccc.append(x)
    log( "selu-o" , x._cdata)

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildActivation(str(x._cdata),"selu")
    return x

def _relu(raw, inData, inplace=False):
    log( "relu-i" , inData._cdata)
    x = raw(inData,False)
    ccc.append(x)
    log( "relu-o" , x._cdata)

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildActivation(str(x._cdata),"relu")
    return x

def _relu6(raw, inData, inplace=False):
    log( "relu6-i" , inData._cdata)
    x = raw(inData,False)
    ccc.append(x)
    log( "relu6-o" , x._cdata)

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildActivation(str(x._cdata),"relu6")
    return x

def _leaky_relu(raw, inData, negative_slope=0.01, inplace=False):
    log( "leaky-i" , args[0]._cdata)
    x = raw(inData, negative_slope,inplace)
    ccc.append(x)
    log( "leaky-o" , x._cdata)

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildActivation(str(x._cdata),"leaky",negative_slope)
    return x

def _tanh(raw, inData):
    log( "tanh-i" , inData._cdata)
    x = raw(inData)  
    ccc.append(x)
    log( "tanh-o" , x._cdata)

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildActivation(str(x._cdata),"tanh")
    return x

def _sigmoid(raw, inData):
    log( "sigmoid-i" , inData._cdata)
    x = raw(inData)
    ccc.append(x)
    log( "sigmoid-o" , x._cdata)

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildActivation(str(x._cdata),"sigmoid")
    return x

def _softplus(raw, inData, thresh):
    log( "softplus-i" , inData._cdata)
    x = raw(inData,thresh)
    ccc.append(x)
    log( "softplus-o" , x._cdata)

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildActivation(str(x._cdata),"softplus", thresh)
    return x

# =====  Variable op ======
def _add(inData, *args):
    log( "add-i1" , inData._cdata)
    log( "add-i2" , args[0]._cdata)
    x = raw__add__(inData, *args)
    ccc.append(x)
    log( "add-o" , x._cdata)

    try:
        layer1 = msnhnet.name_index_dict[str(inData._cdata)]
    except:
        raise NotImplementedError(inData._cdata," not contain [add]")

    try:
        layer2 = msnhnet.name_index_dict[str(args[0]._cdata)]
    except:
        raise NotImplementedError(args[0]._cdata," not contain [add]")

    if layer1 == msnhnet.getLastVal() :
        layers = str(layer2)
    elif layer2 == msnhnet.getLastVal() :
        layers = str(layer1)
    else:
        layers = str(layer1) + "," + str(layer2)    

    msnhnet.buildVariableOp(str(x._cdata), layers, "add")
    return x

def _iadd(inData, *args):
    log( "iadd-i1" , inData._cdata)
    log( "iadd-i2" , args[0]._cdata)
    y = raw__iadd__(inData, *args)
    x = y.clone()
    ccc.append(x)
    log( "iadd-o" , x._cdata)

    try:
        layer1 = msnhnet.name_index_dict[str(inData._cdata)]
    except:
        raise NotImplementedError(inData._cdata," not contain [add]")

    try:
        layer2 = msnhnet.name_index_dict[str(args[0]._cdata)]
    except:
        raise NotImplementedError(args[0]._cdata," not contain [add]")

    if layer1 == msnhnet.getLastVal() :
        layers = str(layer2)
    elif layer2 == msnhnet.getLastVal() :
        layers = str(layer1)
    else:
        layers = str(layer1) + "," + str(layer2)    

    msnhnet.buildVariableOp(str(x._cdata), layers, "add")
    return x

def _sub(inData, *args):
    log( "sub-i1" , inData._cdata)
    log( "sub-i2" , args[0]._cdata)
    x = raw__sub__(inData, *args)
    ccc.append(x)
    log( "sub-o" , x._cdata)

    try:
        layer1 = msnhnet.name_index_dict[str(inData._cdata)]
    except:
        raise NotImplementedError(inData._cdata," not contain [sub]")

    try:
        layer2 = msnhnet.name_index_dict[str(args[0]._cdata)]
    except:
        raise NotImplementedError(args[0]._cdata," not contain [sub]")
    
    subMode = "sub"
    if layer1 == msnhnet.getLastVal() :
        layers = str(layer2)
    elif layer2 == msnhnet.getLastVal() :
        subMode = "subInv"
        layers = str(layer1)
    else:
        layers = str(layer1) + "," + str(layer2)  

    msnhnet.buildVariableOp(str(x._cdata), layers, subMode)
    return x

def _isub(inData, *args):
    log( "isub-i1" , inData._cdata)
    log( "isub-i2" , args[0]._cdata)
    y = raw__isub__(inData, *args)
    x = y.clone()
    ccc.append(x)
    log( "isub-o" , x._cdata)

    try:
        layer1 = msnhnet.name_index_dict[str(inData._cdata)]
    except:
        raise NotImplementedError(inData._cdata," not contain [sub]")

    try:
        layer2 = msnhnet.name_index_dict[str(args[0]._cdata)]
    except:
        raise NotImplementedError(args[0]._cdata," not contain [sub]")
    
    subMode = "sub"
    if layer1 == msnhnet.getLastVal() :
        layers = str(layer2)
    elif layer2 == msnhnet.getLastVal() :
        subMode = "subInv"
        layers = str(layer1)
    else:
        layers = str(layer1) + "," + str(layer2) 

    msnhnet.buildVariableOp(str(x._cdata), layers, subMode)
    return x

def _mul(inData, *args):
    log( "mul-i1" , inData._cdata)
    log( "mul-i2" , args[0]._cdata)
    x = raw__mul__(inData, *args)
    ccc.append(x)
    log( "mul-o" , x._cdata)

    try:
        layer1 = msnhnet.name_index_dict[str(inData._cdata)]
    except:
        raise NotImplementedError(inData._cdata," not contain [mul]")

    try:
        layer2 = msnhnet.name_index_dict[str(args[0]._cdata)]
    except:
        raise NotImplementedError(args[0]._cdata," not contain [mul]")

    if layer1 == msnhnet.getLastVal() :
        layers = str(layer2)
    elif layer2 == msnhnet.getLastVal() :
        layers = str(layer1)
    else:
        layers = str(layer1) + "," + str(layer2) 

    msnhnet.buildVariableOp(str(x._cdata), layers, "mul")
    return x

def _imul(inData, *args):
    log( "imul-i1" , inData._cdata)
    log( "imul-i2" , args[0]._cdata)
    y = raw__imul__(inData, *args)
    x = y.clone()
    ccc.append(x)
    log( "imul-o" , x._cdata)

    try:
        layer1 = msnhnet.name_index_dict[str(inData._cdata)]
    except:
        raise NotImplementedError(inData._cdata," not contain [mul]")

    try:
        layer2 = msnhnet.name_index_dict[str(args[0]._cdata)]
    except:
        raise NotImplementedError(args[0]._cdata," not contain [mul]")

    if layer1 == msnhnet.getLastVal() :
        layers = str(layer2)
    elif layer2 == msnhnet.getLastVal() :
        layers = str(layer1)
    else:
        layers = str(layer1) + "," + str(layer2)  

    msnhnet.buildVariableOp(str(x._cdata), layers, "mul")
    return x

def _permute(inData, *args):
    log( "permute-i" , inData._cdata)
    x = raw__permute__(inData, *args)
    ccc.append(x)
    log( "permute-o" , x._cdata)

    dim  = args[0]
    dim0 = args[1]
    dim1 = args[2]
    dim2 = args[3]

    if dim != 0:
        raise NotImplementedError("permute dim0 must be 0")
    msnhnet.buildPermute(str(x._cdata), dim0, dim1, dim2)
    return x   
    
def _mean(inData, *args,**kwargs):
    log( "mean-i" , inData._cdata)
    x=raw_mean(inData, *args,**kwargs)
    ccc.append(x)
    log( "mean-o" , x._cdata)

    if len(args)==1:
        dim=args[0]
    elif 'dim' in kwargs:
        dim=kwargs['dim']
    else:
        raise NotImplementedError('mean operation must specify a dim')

    if dim == 0:
        raise NotImplementedError("mean dim0 is not supported")

    msnhnet.buildReduction(str(x._cdata),"mean",dim)
    return x   

def _sum(inData, *args):
    log( "sum-i" , inData._cdata)
    x = raw__sum__(inData, *args)
    ccc.append(x)
    log( "sum-o" , x._cdata)
    if len(args)==1:
        dim=args[0]
    else:
        raise NotImplementedError('sum operation must specify a dim')
    msnhnet.buildReduction(str(x._cdata),"sum",dim)
    return x

def _div(raw,inputs, inputs2):
    log( "div-i1" , inputs._cdata)
    log( "div-i2" , inputs2._cdata)
    x=raw(inputs, inputs2)
    ccc.append(x)
    log( "div-o" , x._cdata)

    try:
        layer1 = msnhnet.name_index_dict[str(inData._cdata)]
    except:
        raise NotImplementedError(inData._cdata," not contain [div]")

    try:
        layer2 = msnhnet.name_index_dict[str(args[0]._cdata)]
    except:
        raise NotImplementedError(args[0]._cdata," not contain [div]")

    divMode = "div"
    if layer1 == msnhnet.getLastVal() :
        layers = str(layer2)
    elif layer2 == msnhnet.getLastVal() :
        divMode = "divInv"
        layers = str(layer1)
    else:
        layers = str(layer1) + "," + str(layer2) 

    msnhnet.buildVariableOp(str(x._cdata), layers, divMode)

    return x   

def _pow(inData, *args):
    log( "pow-i" , inData._cdata)
    x = raw__pow__(inData, *args)
    constVal = args[0]
    ccc.append(x)
    log( "pow-o" , x._cdata)

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildVariableOp(str(x._cdata),"","pow",constVal)
    return x

def _sqrt(inData, *args):
    log( "sqrt-i" , inData._cdata)
    x = raw__sqrt__(inData, *args)
    ccc.append(x)
    log( "sqrt-o" , x._cdata)

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildVariableOp(str(x._cdata),"","sqrt")
    return x

def _abs(raw, inData, *args):
    log( "abs-i" , inData._cdata)
    x = raw(inData, *args)
    ccc.append(x)
    log( "abs-o" , x._cdata)

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildVariableOp(str(x._cdata),"","abs")
    return x

def _acos(raw, inData, *args):
    log( "acos-i" , inData._cdata)
    x = raw(inData, *args)
    ccc.append(x)
    log( "acos-o" , x._cdata)

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildVariableOp(str(x._cdata),"","acos")
    return x

def _asin(raw, inData, *args):
    log( "asin-i" , inData._cdata)
    x = raw(inData, *args)
    ccc.append(x)
    log( "asin-o" , x._cdata)

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildVariableOp(str(x._cdata),"","asin")
    return x

def _atan(raw, inData, *args):
    log( "atan-i" , inData._cdata)
    x = raw(inData, *args)
    ccc.append(x)
    log( "atan-o" , x._cdata)

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildVariableOp(str(x._cdata),"","atan")
    return x

def _cos(raw, inData, *args):
    log( "cos-i" , inData._cdata)
    x = raw(inData, *args)
    ccc.append(x)
    log( "cos-o" , x._cdata)

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildVariableOp(str(x._cdata),"","cos")
    return x

def _cosh(raw, inData, *args):
    log( "cosh-i" , inData._cdata)
    x = raw(inData, *args)
    ccc.append(x)
    log( "cosh-o" , x._cdata)

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildVariableOp(str(x._cdata),"","cosh")
    return x

def _sin(raw, inData, *args):
    log( "sin-i" , inData._cdata)
    x = raw(inData, *args)
    ccc.append(x)
    log( "sin-o" , x._cdata)

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildVariableOp(str(x._cdata),"","sin")
    return x

def _sinh(raw, inData, *args):
    log( "sinh-i" , inData._cdata)
    x = raw__sinh__(inData, *args)
    ccc.append(x)
    log( "sinh-o" , x._cdata)

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildVariableOp(str(x._cdata),"","sinh")
    return x

def _tan(raw, inData, *args):
    log( "tan-i" , inData._cdata)
    x = raw(inData, *args)
    ccc.append(x)
    log( "tan-o" , x._cdata)

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildVariableOp(str(x._cdata),"","tan")
    return x

def _exp(raw, inData, *args):
    log( "exp-i" , inData._cdata)
    x = raw(inData, *args)
    ccc.append(x)
    log( "exp-o" , x._cdata)

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildVariableOp(str(x._cdata),"","exp")
    return x

def _log(raw, inData, *args):
    log( "log-i" , inData._cdata)
    x = raw(inData, *args)
    ccc.append(x)
    log( "log-o" , x._cdata)

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildVariableOp(str(x._cdata),"","log")
    return x

def _log10(raw, inData, *args):
    log( "log10-i" , inData._cdata)
    x = raw(inData, *args)
    ccc.append(x)
    log( "log10-o" , x._cdata)

    msnhnet.checkInput(inData,sys._getframe().f_code.co_name)
    msnhnet.buildVariableOp(str(x._cdata),"","log10")
    return x

# =====  Variable op not supported ======
''' TODO '''
def _view(inData, *args):
    x=raw_view(inData, *args)
    ccc.append(x)
    raise NotImplementedError("view not supported yet")
    return x  

def _unsqueeze(inData, *args):
    x = raw__unsqueeze__(inData, *args)
    ccc.append(x)
    raise NotImplementedError("unsqueeze not supported yet")
    return x

def _expand_as(inData, *args):
    x = raw__expand_as__(inData, *args)
    ccc.append(x)
    raise NotImplementedError("expand_as not supported yet")
    return x

def _contiguous(inData, *args):
    x = raw__contiguous__(inData, *args)
    ccc.append(x)
    raise NotImplementedError("contiguous not supported yet")
    return x

F.conv2d        =   Hook(F.conv2d,_conv2d)
F.max_pool2d    =   Hook(F.max_pool2d,_max_pool2d)
F.avg_pool2d    =   Hook(F.avg_pool2d,_avg_pool2d)
F.adaptive_avg_pool2d = Hook(F.adaptive_avg_pool2d, _adaptive_avg_pool2d)
F.linear        =   Hook(F.linear, _linear)
torch.flatten   =   Hook(torch.flatten,_flatten)
F.dropout       =   Hook(F.dropout,_dropout)
F.batch_norm    =   Hook(F.batch_norm,_batch_norm)
F.interpolate   =   Hook(F.interpolate,_interpolate)
torch.abs       =   Hook(torch.abs,_abs)
torch.acos      =   Hook(torch.acos,_acos)
torch.asin      =   Hook(torch.asin,_asin)
torch.atan      =   Hook(torch.atan,_atan)
torch.cos       =   Hook(torch.cos,_cos)
torch.cosh      =   Hook(torch.cosh,_cosh)
torch.sin       =   Hook(torch.sin,_sin)
torch.sinh      =   Hook(torch.sinh,_sinh)
torch.tan       =   Hook(torch.tan,_tan)
torch.exp       =   Hook(torch.exp,_exp)
torch.log       =   Hook(torch.log,_log)
torch.log10     =   Hook(torch.log10,_log10)
torch.cat       =   Hook(torch.cat,_cat)

# =====  Activation ======
F.elu           =   Hook(F.elu,_elu)
F.selu          =   Hook(F.selu,_selu)
F.relu          =   Hook(F.relu,_relu)
F.relu6         =   Hook(F.relu6,_relu6)
F.leaky_relu    =   Hook(F.leaky_relu,_leaky_relu)
F.tanh          =   Hook(F.tanh,_tanh)
F.softmax       =   Hook(F.softmax,_softmax)
F.sigmoid       =   Hook(F.sigmoid,_sigmoid)
F.softplus      =   Hook(F.softplus,_softplus)

# =====  Variable op ======
for t in [torch.Tensor]:
    raw_view = t.view
    t.view = _view
    raw_mean = t.mean
    t.mean = _mean
    raw__add__ = t.__add__
    t.__add__ = _add
    raw__iadd__ = t.__iadd__
    t.__iadd__ = _iadd
    raw__sub__ = t.__sub__
    t.__sub__ = _sub
    raw__isub__ = t.__isub__
    t.__isub__ = _isub
    raw__mul__ = t.__mul__
    t.__mul__=_mul
    raw__imul__ = t.__imul__
    t.__imul__ = _imul
    raw__permute__ = t.permute
    t.permute = _permute
    raw__contiguous__ = t.contiguous
    t.contiguous = _contiguous
    raw__pow__ = t.pow
    t.pow = _pow
    raw__sum__ = t.sum
    t.sum = _sum
    raw__sqrt__ = t.sqrt
    t.sqrt = _sqrt
    raw__unsqueeze__ = t.unsqueeze
    t.unsqueeze = _unsqueeze
    raw__expand_as__ = t.expand_as
    t.expand_as = _expand_as



def trans(net, inputVar, msnhnet_path, msnhbin_path):
    Hook.hookInited = True
    msnhnet.buildConfig(str(id(inputVar)), inputVar.size())
    net.forward(inputVar)

    with open(msnhnet_path,"w") as f1:
        f1.write(msnhnet.net)

    val = []
    dd = 0
    for name in net.state_dict():
            if "num_batches_tracked" not in name:
                    c = net.state_dict()[name].data.flatten().numpy().tolist()
                    dd = dd + len(c)
                    print(name, ":", len(c))
                    val.extend(c)

    with open(msnhbin_path,"wb") as f:
        for i in val :
            f.write(pack('f',i))

def transBin(net, msnhbin_path):
    val = []
    dd = 0
    for name in net.state_dict():
            if "num_batches_tracked" not in name:
                    c = net.state_dict()[name].data.flatten().numpy().tolist()
                    dd = dd + len(c)
                    print(name, ":", len(c))
                    val.extend(c)

    with open(msnhbin_path,"wb") as f:
        for i in val :
            f.write(pack('f',i))

def transNet(net, inputVar, msnhnet_path):
    Hook.hookInited = True
    msnhnet.buildConfig(str(id(inputVar)), inputVar.size())
    net.forward(inputVar)

    with open(msnhnet_path,"w") as f1:
        f1.write(msnhnet.net)    