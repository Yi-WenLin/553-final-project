/* Author: YiWen Lin
   Date: 04/27/2025
   Description: This file provides the defination of the TransferredPhoto class.
   The menber function will follow the flow/pipeline to get the output once 
   applying the style transfer.
   Input -> Preprocess -> Tensor Conversion -> Run in ONNX Inference -> Postprocess -> Output
   Detailed description of each process are commented above the code.
*/

#include "Photo.h"
#include "FilteredPhoto.h"
#include "TransferredPhoto.h"
#include <onnxruntime/onnxruntime_cxx_api.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cstdio>
#include <limits> // using FLT_MAX
#include <array>

using namespace std;
using namespace cv;


TransferredPhoto::TransferredPhoto(const std::string& path)
    : env(ORT_LOGGING_LEVEL_FATAL, "StyleTransfer"), session(nullptr),
      model_path(path), source(nullptr)
{
    // Silence ONNX schema error spam
    freopen("/dev/null", "w", stderr);
    // Set up session in onnxruntime for starting
    session_options.SetIntraOpNumThreads(1);;
    session = Ort::Session(env, model_path.c_str(), session_options);
    freopen("/dev/tty", "w", stderr);

    // Set up session in onnxruntime for starting
    //session_options.SetIntraOpNumThreads(1);;
    //session = Ort::Session(env, model_path.c_str(), session_options);

    // Get input tensor type and shape
    Ort::TypeInfo input_type_info = session.GetInputTypeInfo(0);
    auto tensor_info = input_type_info.GetTensorTypeAndShapeInfo();
    input_dims = tensor_info.GetShape();
}

cv::Mat TransferredPhoto::preprocess(const cv::Mat& img, int target_height, int target_width)
{
    cv::Mat rgb_img, resized, float_img;

    // Convert OpenCV BGR to RGB - Models were trained in RGB - otherwise not comparable
    cvtColor(img, rgb_img, COLOR_BGR2RGB);

    // Resizing img to match the model's expected input size
    resize(rgb_img, resized, Size(target_width, target_height));
    // Normalize pixel values - [0, 255] -> [0.0, 1.0].float
    resized.convertTo(float_img, CV_32F, 1.0 / 255.0);
    return float_img;
}

std::vector<float> TransferredPhoto::convertToTensor(cv::Mat& img)
{
    // Convert the image to tensor - that can apply the trained paramenter in model
    // 1D-Tensor NCHW layout
    std::vector<float> tensor;
    for(int c = 0; c < 3; ++c)  // Channel
    {
        for(int i = 0; i < img.rows; ++i)  // Rows
        {
            for(int j = 0; j < img.cols; ++j)  // Columns
            {
                tensor.push_back(img.at<Vec3f>(i, j)[c]);  // add pixels(i, j) at [0->1->2 channel]
            }
        }
    }
    return tensor;
}

cv::Mat TransferredPhoto::postprocess(float* data, int height, int width)
{
    // Rebuild our image to original channel [0, 255] and BGR mode
    Mat output(height, width, CV_32FC3); // 3-channel matrix.float
    int idx = 0; // Tensor index
    float min_val = FLT_MAX;
    float max_val = -FLT_MAX;

    for(int c = 0; c < 3; ++c)
    {
        for(int i = 0; i < height; ++i)
        {
            for(int j = 0; j < width; ++j)
            {
                float val = data[idx++];
                //min_val = std::min(min_val, val);
                //max_val = std::max(max_val, val);

                // Clip to [0,255] directly
                val = std::min(std::max(val, 0.0f), 255.0f);
                // Set pixel value back
                output.at<Vec3f>(i, j)[c] = val;
            }
        }
    }

    // Check tensor range for converting
    // std::cout << "Clipped tensor range: [" << min_val << ", " << max_val << "]" << std::endl;
    
    // Test A. [0, 255] - Works! - Suitable for all models!
    // Directly clamp and convert
    cv::Mat output_uint8;
    output.convertTo(output_uint8, CV_8UC3);

    // Convert RGB back to BGR
    cv::cvtColor(output_uint8, output_uint8, cv::COLOR_RGB2BGR);

    return output_uint8;

    /* Test B. Normalize from [-2,2] to [0,1]
    output = (output + 2.0f) / 4.0f;

    // Scale
    output *= 255.0f;

    // Convert to uint8
    output.convertTo(output, CV_8UC3);

    // Convert RGB to BGR
    cv::cvtColor(output, output, cv::COLOR_RGB2BGR);

    return output;
    */

    /* Test C. [0, 1]
    // Scale up
    output *= 255.0f;

    // Convert to uint8
    output.convertTo(output, CV_8UC3);

    // Convert RGB to BGR
    cv::cvtColor(output, output, cv::COLOR_RGB2BGR)

    return output;
    */
    
}

cv::Mat TransferredPhoto::apply(const cv::Mat& input_img)
{
    // Pipeline:
    // Input -> Preprocess -> Tensor Conversion -> Run in ONNX Inference -> Postprocess -> Output
    Ort::AllocatorWithDefaultOptions allocator;

    // Dynamic dimension handler - since our input size is flexible
    bool dynamic_height = input_dims[2] <= 0;
    bool dynamic_width = input_dims[3] <= 0;
    int height = dynamic_height ? input_img.rows : static_cast<int>(input_dims[2]);
    int width = dynamic_width ? input_img.cols : static_cast<int>(input_dims[3]);

    // Preprocess image
    Mat preprocessed = preprocess(input_img, height, width);
    // Convert to tensor
    std::vector<float> input_tensor_values = convertToTensor(preprocessed);

    // Set input/output names dynamically
    Ort::AllocatedStringPtr input_name_ptr = session.GetInputNameAllocated(0, allocator);
    const char* input_name = input_name_ptr.get();
    Ort::AllocatedStringPtr output_name_ptr = session.GetOutputNameAllocated(0, allocator);
    const char* output_name = output_name_ptr.get();

    // Setup input Tensor - dimensions(1, 3(channel), height, weight)
    std::array<int64_t, 4> input_shape{1, 3, height, width};
    Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(memory_info, input_tensor_values.data(), 
    input_tensor_values.size(), input_shape.data(), input_shape.size());

    // Run inference -(default running options, single input and single output)
    auto output_tensors = session.Run(Ort::RunOptions{nullptr}, &input_name, &input_tensor, 1, &output_name, 1);

    // Extract the raw output tensor data
    float* output_data = output_tensors.front().GetTensorMutableData<float>();

    // Postprocess image - as we receive raw output
    return postprocess(output_data, height, width);
}

std::string TransferredPhoto::getType() const
{
    return "TransferredPhoto";
}

void TransferredPhoto::printMetadata() const
{
    // Photo::printMetadata();
    std::cout << "---------- Metadata ----------\n";
    std::cout << "Type: " << getType() << std::endl;
    std::cout << "Tags: ";
    for (const auto& tag : tags)
        std::cout << tag << ", ";
    std::cout << "\n";
    if(!image.empty())
        std::cout << "Resolution: " << image.cols << " x " << image.rows << std::endl;
    else
        std::cout << "No image. " << std::endl;
    describeTransfer();
}

void TransferredPhoto::show() const
{
    // Empty Case
    if(image.empty())
    {
        std::cerr << "No image to display. " << std::endl;
        return;
    }
    cv::imshow("Transferred Photo Viewer", image);
    std::cout << "Press '0' to close the image window." << std::endl;
    cv::waitKey(0);
    cv::destroyWindow("Transferred Photo Viewer");
}

void TransferredPhoto::describeTransfer() const
{
    std::cout << "Style: " << model_name << std::endl;
    std::cout << "Original Resolution: " << (getSource()->getImage()).cols << " x " << (getSource()->getImage()).rows << std::endl;
}

void TransferredPhoto::setSource(Photo& original)
{
    if(FilteredPhoto* temp = dynamic_cast<FilteredPhoto*>(&original))
    {
        this->source = temp->getSource();
    }
    else if(TransferredPhoto* temp = dynamic_cast<TransferredPhoto*>(&original))
    {
        this->source = temp->getSource();
    }
    else
    {
        this->source = &original;
    }
}

Photo* TransferredPhoto::getSource() const
{
    return source;
}

std::string TransferredPhoto::getModelName() const
{
    return model_name;
}


void TransferredPhoto::setModelName(std::string& name)
{
    model_name = name;
}