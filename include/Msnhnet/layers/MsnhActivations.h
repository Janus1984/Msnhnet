#ifndef MSNHACTIVATIONS_H
#define MSNHACTIVATIONS_H

#include <cmath>
#include "Msnhnet/config/MsnhnetCfg.h"
#include "Msnhnet/core/MsnhSimd.h"
#include "Msnhnet/utils/MsnhExport.h"

#ifdef USE_X86
    #include "Msnhnet/layers/MsnhActivationsAvx.h"
#endif

#ifdef USE_GPU
    #include "Msnhnet/layers/cuda/MsnhActivationsGPU.h"
#endif

#ifdef USE_ARM
    #ifdef USE_NEON
        #include "Msnhnet/layers/MsnhActivationsNeon.h"
    #endif
#endif

namespace Msnhnet {

class MsnhNet_API Activations {
public:
    static ActivationType getActivation(const std::string& msg);

    static std::string getActivationStr(const ActivationType& type);

    static float activate(float x, const ActivationType& actType,
                          float params = 0.1F);
    static void activateArray(float* x, int numX,
                              const ActivationType& actType, bool useAVX,
                              float param = 0.1F);
    static void activatePRelu(float* x, int batch, int channels,
                              float* weights, int whStep, bool useAVX);
    static void activateArrayNormCh(float* x, int numX, int batch,
                                    int channels, int whStep,
                                    float* output);
    static void activateArrayNormChSoftMax(float* x, int numX, int batch,
                                           int channels, int whStep,
                                           float* output, int useMaxVal);

private:
    static float logisticActivate(float x) {
        return 1.F / (1.F + expf(-x));
    }

    static float loggyActivate(float x) {
        return (2.F / (1.F + expf(-x))) - 1.F;
    }

    static float reluActivate(float x) {
        return x * static_cast<float>(x > 0);
    }

    static float relu6Activate(float x) {
        return (x > 0 ? x : 0) > 6 ? 6 : (x > 0 ? x : 0);
    }

    static float hardSwishActivate(float x) {
        return x * relu6Activate(x + 3.F) / 6.F;
    }

    static float eluActivate(float x) {
        return ((static_cast<float>(x >= 0) * x) +
                (static_cast<float>(x < 0) * (expf(x) - 1.F)));
    }

    static float seluActivate(float x) {
        return (static_cast<float>(x >= 0) * 1.0507F * x) +
               (static_cast<float>(x < 0) * 1.0507F * 1.6732F *
                (expf(x) - 1));
    }

    static float relieActivate(float x) {
        return (x > 0) ? x : .01F * x;
    }

    static float rampActivate(float x) {
        return (x * static_cast<float>(x > 0)) + (.1F * x);
    }

    static float leakyActivate(float x, float param = 0.1F) {
        return (x > 0) ? x : param * x;
    }

    static float tanhActivate(float x) {
        return ((expf(2 * x) - 1) / (expf(2 * x) + 1));
    }

    static float stairActivate(float x) {
        int n = static_cast<int>(floor(x));
        if (n % 2 == 0) {
            return (floorf(x / 2.F));
        }
        return ((x - floor(x)) + floorf(x / 2.F));
    }

    static float hardtanActivate(float x) {
        if (x < -1) {
            return -1;
        }
        if (x > 1) {
            return 1;
        }
        return x;
    }

    static float softplusActivate(float x, float threshold) {
        if (x > threshold) {
            return x;
        }
        if (x < -threshold) {
            return expf(x);
        }
        return logf(expf(x) + 1);
    }

    static float plseActivate(float x) {
        if (x < -4) {
            return .01F * (x + 4);
        }
        if (x > 4) {
            return .01F * (x - 4) + 1;
        }
        return .125F * x + .5F;
    }

    static float lhtanActivate(float x) {
        if (x < 0.0F) {
            return .001F * x;
        }
        if (x > 1.0F) {
            return (.001F * (x - 1)) + 1;
        }
        return x;
    }

    static float mishActivate(float x) {
        const float mishThreshHold = 20.F;
        return x * tanhf(softplusActivate(x, mishThreshHold));
    }

    static float swishActivate(float x) {
        return x * logisticActivate(x);
    }
};
}  // namespace Msnhnet

#endif  // MSNH_ACTIVATIONS_H