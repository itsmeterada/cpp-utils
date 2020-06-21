#include <iostream>
#include <string>

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#ifndef SIMD_OPENCV_ENABLE
#define SIMD_OPENCV_ENABLE
#endif
#include "Simd/SimdDetection.hpp"
#include "Simd/SimdDrawing.hpp"

int main(int argc, char * argv[])
{
    if (argc < 2)
    {
        std::cout << "You have to set video source! It can be 0 for camera or video file name." << std::endl;
        return 1;
    }
    std::string source = argv[1];

    cv::VideoCapture capture;
    if (source == "0")
        capture.open(0);
    else
        capture.open(source);
    if (!capture.isOpened())
    {
        std::cout << "Can't capture '" << source << "' !" << std::endl;
        return 1;
    }

    typedef Simd::Detection<Simd::Allocator> Detection;
    Detection detection;
    detection.Load("data/cascade/haar_face_0.xml");
    bool inited = false;

    const char * WINDOW_NAME = "FaceDetection";
    cv::namedWindow(WINDOW_NAME, 1);
    for (;;)
    {
        cv::Mat frame;
        capture >> frame;

        Detection::View image = frame;

        if (!inited)
        {
            detection.Init(image.Size(), 1.2, image.Size() / 20);
            inited = true;
        }

        Detection::Objects objects;
        detection.Detect(image, objects);

        for (size_t i = 0; i < objects.size(); ++i)
            Simd::DrawRectangle(image, objects[i].rect, Simd::Pixel::Bgr24(0, 255, 255));

        cv::imshow(WINDOW_NAME, frame);
        if (cv::waitKey(1) == 27)// "press 'Esc' to break video";
            break;
    }
    return 0;
}
