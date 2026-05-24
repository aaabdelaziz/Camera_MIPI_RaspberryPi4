#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    cout << "=== Camera Test ===" << endl;

    // Try Logitech BRIO first (/dev/video1), then fallback to video0
    vector<pair<string, int>> cameras = {
        {"Logitech BRIO",  1},
        {"Pi OV5647 CSI",  0},
        {"video2",         2},
        {"video3",         3},
    };

    for (auto& [name, idx] : cameras) {
        cout << "\nTrying " << name << " (/dev/video" << idx << ")..." << endl;

        VideoCapture cap(idx, CAP_V4L2);
        if (!cap.isOpened()) {
            cout << "FAIL: Could not open" << endl;
            continue;
        }

        // Set resolution — BRIO supports up to 4K but 640x480 is fast
        cap.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M','J','P','G'));
        cap.set(CAP_PROP_FRAME_WIDTH,  640);
        cap.set(CAP_PROP_FRAME_HEIGHT, 480);
        cap.set(CAP_PROP_FPS, 30);

        Mat frame;
        for (int i = 0; i < 10; i++) {  // drain buffer + warm up
            cap.read(frame);
            if (!frame.empty()) break;
        }

        if (frame.empty()) {
            cout << "FAIL: No frames" << endl;
            cap.release();
            continue;
        }

        cout << "SUCCESS: " << frame.cols << "x" << frame.rows
             << " channels=" << frame.channels() << endl;

        string filename = "snapshot_video" + to_string(idx) + ".jpg";
        imwrite(filename, frame);
        cout << "Saved: " << filename << endl;

        cap.release();
        return 0;
    }

    cerr << "All cameras failed." << endl;
    return -1;
}