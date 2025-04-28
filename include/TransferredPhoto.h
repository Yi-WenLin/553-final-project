#ifndef TRANSFERREDPHOTO_H
#define TRANSFERREDPHOTO_H

#include "Photo.h"
#include <vector>
#include <onnxruntime/onnxruntime_cxx_api.h>


class TransferredPhoto : public Photo
{
    private:
        // onnxruntime settings
        Ort::Env env;
        Ort::Session session;
        Ort::SessionOptions session_options;

        //std::vector<char*> input_names;
        std::vector<int64_t> input_dims;
        std::string model_path;
        std::string model_name;
        Photo* source;  // Track original

        // Image Process FUnction
        cv::Mat preprocess(const cv::Mat& img, int target_height, int target_width);
        std::vector<float> convertToTensor(cv::Mat& img);
        cv::Mat postprocess(float* data, int height, int width);

    public:
        TransferredPhoto(const std::string& path);

        // Core function
        cv::Mat apply(const cv::Mat& input_img);

        // Inherited
        std::string getType() const override;
        void printMetadata() const override;
        void show() const override;

        void describeTransfer() const;

        void setSource(Photo& original);
        Photo* getSource() const override;

        std::string getModelName() const;
        void setModelName(std::string& name);
        

};

#endif
