# Camera MIPI RaspberryPi4 Face Detection

A C++ face detection system using MIPI CSI camera modules connected to Raspberry Pi 4, powered by OpenCV DNN with a Caffe SSD model for real-time face detection on embedded hardware.

## Features

- Real-time face detection using OpenCV DNN (SSD Caffe model)
- MIPI CSI camera support via V4L2 / GStreamer
- Optimized for Raspberry Pi 4 (ARM64)
- Confidence-based filtering with bounding boxes and labels
- Camera device auto-discovery utility included
- Lightweight and efficient — no Python runtime overhead

## Prerequisites

### Hardware

- Raspberry Pi 4 (2GB RAM minimum, 4GB+ recommended)
- MIPI CSI Camera Module (OV5647 or compatible)
- 5V 3A Power Supply

### Software

- Raspberry Pi OS (Bullseye/Bookworm, 64-bit recommended)
- OpenCV 4.x with DNN module (compiled with V4L2 and GStreamer support)
- C++17 compiler (g++)

### Install OpenCV on Raspberry Pi

```bash
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    pkg-config \
    libopencv-dev \
    gstreamer1.0-tools \
    libgstreamer1.0-dev \
    libgstreamer-plugins-base1.0-dev
```

## Project Structure

```
Camera_MIPI_RaspberryPi4/
├── main.cpp                                  # Face detection program (OpenCV DNN + Caffe SSD)
├── camera_test.cpp                           # Camera device test & auto-discovery utility
├── deploy.prototxt                           # Caffe model architecture definition
├── res10_300x300_ssd_iter_140000.caffemodel  # Pre-trained SSD face detection weights
├── camera_test                               # Compiled ARM64 binary (camera test)
├── face_detect                               # Compiled ARM64 binary (face detection)
└── README.md                                 # This file
```

## Build

Compile both programs with OpenCV:

```bash
# Camera test utility
g++ -O2 camera_test.cpp -o camera_test $(pkg-config --cflags --libs opencv4)

# Face detection program
g++ -O2 main.cpp -o face_detect $(pkg-config --cflags --libs opencv4)
```

## Usage

### 1. Test Camera Connection

Run the camera test to discover which video device is active:

```bash
./camera_test
```

This scans `/dev/video0` through `/dev/video3` and saves a snapshot from the first working camera. If all fail, check your camera connection:

```bash
# List available video devices
ls /dev/video*

# Check camera detection
vcgencmd get_camera
```

### 2. Run Face Detection

```bash
./face_detect
```

The program:
- Scans `/dev/video10`–`/dev/video21` for a working camera (GStreamer pipeline first, then native V4L2 fallback)
- Loads the Caffe SSD face detection model (`deploy.prototxt` + `res10_300x300_ssd_iter_140000.caffemodel`)
- Displays a live feed with detected faces highlighted and confidence percentages
- Press any key in the display window to exit

## How It Works

### Face Detection Model

- **Architecture**: SSD (Single Shot MultiBox Detector)
- **Backend**: Caffe (loaded via OpenCV DNN `readNetFromCaffe`)
- **Input**: 300×300 RGB blob
- **Mean Subtraction**: (104.0, 177.0, 123.0)
- **Confidence Threshold**: 0.5 (50%)
- **Output**: Bounding boxes with confidence scores per detected face

### Camera Pipeline

The programs use V4L2 for camera access:
- `camera_test.cpp` — uses OpenCV's native `CAP_V4L2` backend, MJPG format at 640×480, 30fps
- `main.cpp` — tries GStreamer pipelines first for each `/dev/video` device, falls back to native OpenCV backend

## Troubleshooting

### Camera Not Found

```bash
# Check available devices
ls /dev/video*

# Test camera with v4l2-ctl
v4l2-ctl --list-formats-ext
```

### Model Not Loading

Ensure the model files are in the same directory as the executable:
```bash
ls -l deploy.prototxt res10_300x300_ssd_iter_140000.caffemodel
```

### Missing GStreamer Support

If GStreamer pipelines fail, OpenCV falls back to native V4L2. For GStreamer support, rebuild OpenCV with:
```bash
cmake -D WITH_GSTREAMER=ON ..
```

### No Display / Headless Mode

For headless operation, use a virtual display:
```bash
export DISPLAY=:0
# or
xvfb-run ./face_detect
```

## References

- [OpenCV DNN Face Detection](https://github.com/opencv/opencv/tree/master/samples/dnn/face_detector)
- [Raspberry Pi Camera Documentation](https://www.raspberrypi.com/documentation/accessories/camera.html)
- [V4L2 Documentation](https://www.kernel.org/doc/html/latest/userspace-api/media/v4l/v4l2.html)

## License

MIT License

---

**Last Updated:** May 25, 2026  
**Maintainer:** [@aaabdelaziz](https://github.com/aaabdelaziz)
