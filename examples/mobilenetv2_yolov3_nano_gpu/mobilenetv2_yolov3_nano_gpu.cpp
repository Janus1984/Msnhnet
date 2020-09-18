﻿#include <iostream>
#include "Msnhnet/Msnhnet.h"

int main(int argc, char** argv) 
{
    if(argc != 2)
    {
        std::cout<<"\nYou need to give models dir path.\neg: yolov3 /your/models/dir/path/ \n\nModels folder must be like this:\nmodels\n  |-Lenet5\n    |-Lenet5.msnhnet\n    |-Lenet5.msnhbin";
        getchar();
        return 0;
    }

    std::string msnhnetPath = std::string(argv[1]) + "/mobilenetv2_yolov3_nano/mobilenetv2_yolov3_nano.msnhnet";
    std::string msnhbinPath = std::string(argv[1]) + "/mobilenetv2_yolov3_nano/mobilenetv2_yolov3_nano.msnhbin";
    std::string labelsPath  = "../labels/coco.names";
    std::string imgPath = "../images/dog.jpg";
    try
    {
        Msnhnet::NetBuilder  msnhNet;
        Msnhnet::NetBuilder::setOnlyGpu(true);
        msnhNet.buildNetFromMsnhNet(msnhnetPath);
        std::cout<<msnhNet.getLayerDetail();
        msnhNet.loadWeightsFromMsnhBin(msnhbinPath);
        std::vector<std::string> labels ;
        Msnhnet::IO::readVectorStr(labels, labelsPath.data(), "\n");
        Msnhnet::Point2I inSize = msnhNet.getInputSize();

        std::vector<float> img;
        std::vector<std::vector<Msnhnet::YoloBox>> result;

        img = Msnhnet::OpencvUtil::getImgDataF32C3(imgPath, cv::Size(inSize.x,inSize.y), true);
        for (size_t i = 0; i < 10; i++)
        {
			auto st = Msnhnet::TimeUtil::startRecord();
            result = msnhNet.runYoloGPU(img);
            std::cout<<"time  : " << Msnhnet::TimeUtil::getElapsedTime(st) <<"ms"<<std::endl<<std::flush;
        }
    
        cv::Mat org = cv::imread(imgPath);
        Msnhnet::OpencvUtil::drawYoloBox(org,labels,result,inSize,true);
        cv::imshow("test",org);
        cv::waitKey();
    }
    catch (Msnhnet::Exception ex)
    {
        std::cout<<ex.what()<<" "<<ex.getErrFile() << " " <<ex.getErrLine()<< " "<<ex.getErrFun()<<std::endl;
    }
    return 0;
}
