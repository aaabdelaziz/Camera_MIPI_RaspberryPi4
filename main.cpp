#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>

using namespace cv;
using namespace cv::dnn;
using namespace std;

VideoCapture openCamera() {
    // Try the video devices that actually exist on your system
    // (you have video10-video31, no video0)
    vector<int> deviceIndices = {10, 11, 12, 13, 14, 15, 18, 20, 21};

    for (int idx : deviceIndices) {
        string pipeline =
            "v4l2src device=/dev/video" + to_string(idx) + " ! "
            "video/x-raw,width=640,height=480,framerate=30/1 ! "
            "videoconvert ! "
            "video/x-raw,format=BGR ! "
            "appsink drop=true max-buffers=1 sync=false";

        cout << "Trying /dev/video" << idx << " ..." << endl;
        VideoCapture cap(pipeline, CAP_GSTREAMER);
        if (cap.isOpened()) {
            // Verify we can actually read a frame
            Mat testFrame;
            if (cap.read(testFrame) && !testFrame.empty()) {
                cout << "Success: /dev/video" << idx << endl;
                return cap;
            }
            cap.release();
        }

        // Also try OpenCV's native backend with the index
        VideoCapture cap2(idx);
        if (cap2.isOpened()) {
            Mat testFrame;
            if (cap2.read(testFrame) && !testFrame.empty()) {
                cout << "Success via OpenCV default backend: index " << idx << endl;
                cap2.set(CAP_PROP_FRAME_WIDTH, 640);
                cap2.set(CAP_PROP_FRAME_HEIGHT, 480);
                return cap2;
            }
            cap2.release();
        }
    }

    VideoCapture empty;
    return empty;
}

int main() {
    string modelConfiguration = "./deploy.prototxt";
    string modelWeights = "./res10_300x300_ssd_iter_140000.caffemodel";
    Net net = readNetFromCaffe(modelConfiguration, modelWeights);

    if (net.empty()) {
        cerr << "Error: Could not load the AI model files!" << endl;
        return -1;
    }

    VideoCapture cap = openCamera();
    if (!cap.isOpened()) {
        cerr << "Error: All camera open attempts failed!" << endl;
        return -1;
    }

    Mat frame;
    cout << "Starting camera feed. Press any key to exit..." << endl;

    while (cap.read(frame)) {
        if (frame.empty()) {
            cerr << "Warning: Empty frame received, skipping..." << endl;
            continue;
        }

        Mat inputBlob = blobFromImage(frame, 1.0, Size(300, 300),
                                      Scalar(104.0, 177.0, 123.0), false, false);
        net.setInput(inputBlob);

        Mat detection = net.forward();
        Mat detectionMat(detection.size[2], detection.size[3],
                         CV_32F, detection.ptr<float>());

        for (int i = 0; i < detectionMat.rows; i++) {
            float confidence = detectionMat.at<float>(i, 2);
            if (confidence > 0.5) {
                int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
                int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
                int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
                int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);

                // Clamp to frame bounds
                x1 = max(0, x1); y1 = max(0, y1);
                x2 = min(frame.cols, x2); y2 = min(frame.rows, y2);

                rectangle(frame, Rect(x1, y1, x2 - x1, y2 - y1),
                          Scalar(0, 255, 0), 2);

                string label = "Face: " + to_string(int(confidence * 100)) + "%";
                putText(frame, label, Point(x1, max(y1 - 5, 0)),
                        FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1);
            }
        }

        imshow("C++ Face Detection (OpenCV DNN)", frame);
        if (waitKey(1) >= 0) break;
    }

    cap.release();
    destroyAllWindows();
    return 0;
}