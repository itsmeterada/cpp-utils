//
// movie player
//

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <video file>" << std::endl;
        return -1;
    }
    char *video_file = argv[1];
    // 動画の読み込み
    cv::VideoCapture cap(video_file);
    if (!cap.isOpened()) {
        std::cerr << "Error: Failed to open the video." << std::endl;
        return -1;
    }

    // 音声を再生するための設定
    //cv::createTrackbar("Volume", "Video", 0, 100, [](int, void*) {});
    //cv::setTrackbarPos("Volume", "Video", 50);

    // 動画のフレームを読み込んで表示する
    cv::Mat frame;
    //int volume = cv::getTrackbarPos("Volume", "Video");
    cap.set(cv::CAP_PROP_POS_AVI_RATIO, 0);
    cap.set(cv::CAP_PROP_POS_FRAMES, 0);
    while (true) {
        cap >> frame;
        if (frame.empty()) break;
        cv::imshow("Video", frame);
        //int volume = cv::getTrackbarPos("Volume", "Video");
        //std::cout << "Volume: " << volume << std::endl;
        if (cv::waitKey(30) == 27) break;
    }

    return 0;
}
