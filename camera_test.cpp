#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    cout << "=== Camera Test (Raw Bayer Mode) ===" << endl;

    // video0 is the unicam MIPI CSI device (OV5647 raw Bayer output)
    VideoCapture cap(0, CAP_V4L2);
    if (!cap.isOpened()) {
        cerr << "FAIL: Could not open /dev/video0" << endl;
        return -1;
    }

    // Request raw Bayer format from the sensor
    // OV5647 outputs SGBRG10 (10-bit packed), but OpenCV handles 8-bit Bayer
    cap.set(CAP_PROP_FOURCC, VideoWriter::fourcc('B','G','1','0'));
    cap.set(CAP_PROP_CONVERT_RGB, 0);  // Disable auto-conversion if supported
    cap.set(CAP_PROP_FRAME_WIDTH,  640);
    cap.set(CAP_PROP_FRAME_HEIGHT, 480);

    cout << "Format requested: BG10 (Bayer)" << endl;

    Mat frame;
    for (int i = 0; i < 20; i++) {
        cap.read(frame);
        if (!frame.empty()) break;
        cout << "  Attempt " << i+1 << ": " << (frame.empty() ? "empty" : "got frame") << endl;
    }

    if (frame.empty()) {
        cout << "Bayer mode failed. Trying auto-negotiated format..." << endl;
        cap.release();

        // Re-open with default format negotiation
        VideoCapture cap2(0, CAP_V4L2);
        if (!cap2.isOpened()) {
            cerr << "FAIL: Could not re-open camera" << endl;
            return -1;
        }
        cap2.set(CAP_PROP_FRAME_WIDTH,  640);
        cap2.set(CAP_PROP_FRAME_HEIGHT, 480);

        for (int i = 0; i < 20; i++) {
            cap2.read(frame);
            if (!frame.empty()) break;
            cout << "  Attempt " << i+1 << ": " << (frame.empty() ? "empty" : "got frame") << endl;
        }

        if (frame.empty()) {
            cerr << "FAIL: Camera opened but produces no frames." << endl;
            cerr << "This is a known kernel 6.12 firmware mismatch issue." << endl;
            cerr << "The bcm2835-isp driver has buffer size incompatibility." << endl;
            cerr << "See dmesg: buffer size < min buf size - driver mismatch to MMAL" << endl;
            return -1;
        }

        cap = cap2;
    }

    cout << "\nSUCCESS: " << frame.cols << "x" << frame.rows
         << " type=" << frame.type() << " channels=" << frame.channels() << endl;

    // If frame is single-channel (Bayer), convert to color
    Mat colorFrame;
    if (frame.channels() == 1) {
        cout << "Frame is Bayer (single channel). Converting to BGR..." << endl;
        cvtColor(frame, colorFrame, COLOR_BayerBG2BGR);
    } else {
        colorFrame = frame.clone();
    }

    string filename = "snapshot.jpg";
    imwrite(filename, colorFrame);
    cout << "Saved: " << filename << endl;

    cap.release();
    return 0;
}
