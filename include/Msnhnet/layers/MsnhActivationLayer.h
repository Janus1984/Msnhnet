#ifndef ACTIVATIONLAYER_H
#define ACTIVATIONLAYER_H

#include "Msnhnet/config/MsnhnetCfg.h"
#include "Msnhnet/core/MsnhBlas.h"
#include "Msnhnet/layers/MsnhActivations.h"
#include "Msnhnet/layers/MsnhBaseLayer.h"
#include "Msnhnet/net/MsnhNetwork.h"
#include "Msnhnet/utils/MsnhExport.h"

namespace Msnhnet {

class MsnhNet_API ActivationLayer : public BaseLayer {
public:
    ActivationLayer(int batch, int width, int height, int channel,
                    int inputNum, const ActivationType& activation,
                    const std::vector<float>& actParams);

    ~ActivationLayer() override;

    void forward(NetworkState& netState) override;
    void mallocMemory() override;
    void loadAllWeights(std::vector<float>& weights) override;
    void saveAllWeights(int mainIdx, int branchIdx,
                        int branchIdx1) override;

    void loadPreluWeights(float* weights, int len);
#ifdef USE_GPU
    virtual void forwardGPU(NetworkState& netState);
#endif

protected:
    float* _preluWeights = nullptr;
#ifdef USE_GPU
    float* _gpuPreluWeights = nullptr;
#endif
    int _nPreluWeights = 0;
};
}  // namespace Msnhnet

#endif
