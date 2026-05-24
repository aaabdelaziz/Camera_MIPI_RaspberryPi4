# Camera MIPI RaspberryPi4 Face Detection

A Python-based face detection system using MIPI CSI camera modules connected to Raspberry Pi 4. This project leverages real-time computer vision to detect and track faces using optimized deep learning models suitable for embedded systems.

## Features

- 🎥 **Real-time Face Detection** - Using MIPI CSI camera interface
- 🚀 **Optimized for Raspberry Pi 4** - Lightweight models for efficient processing
- 📹 **Video Recording** - Capture detected faces with bounding boxes
- 🔍 **Multiple Detection Models** - Support for various face detection backends
- 📊 **Performance Monitoring** - FPS tracking and latency metrics
- 🛠️ **Easy Configuration** - Simple config-based setup

## Prerequisites

### Hardware Requirements

- Raspberry Pi 4 (2GB RAM minimum, 4GB+ recommended)
- MIPI CSI Camera Module (v2 or v3)
- 5V 3A Power Supply
- MicroSD Card (32GB+ recommended)
- Optional: Heat sink for Raspberry Pi

### Software Requirements

- Raspberry Pi OS (Bullseye or later)
- Python 3.8+
- libatlas-base-dev
- libjasper-dev
- libtiff5
- libjasper1
- libharfbuzz0b
- libwebp6

## Installation

### 1. Enable Camera Interface

```bash
sudo raspi-config
# Navigate to Interface Options -> Camera -> Enable
# Select Yes and reboot
```

### 2. Install System Dependencies

```bash
sudo apt-get update
sudo apt-get upgrade -y
sudo apt-get install -y \
    libatlas-base-dev \
    libjasper-dev \
    libtiff5 \
    libjasper1 \
    libharfbuzz0b \
    libwebp6 \
    python3-pip \
    python3-dev \
    git
```

### 3. Clone Repository

```bash
git clone https://github.com/aaabdelaziz/Camera_MIPI_RaspberryPi4.git
cd Camera_MIPI_RaspberryPi4
```

### 4. Create Virtual Environment

```bash
python3 -m venv venv
source venv/bin/activate
```

### 5. Install Python Dependencies

```bash
pip install --upgrade pip
pip install -r requirements.txt
```

## Usage

### Basic Face Detection

```bash
python detect_faces.py
```

### Run with Configuration File

```bash
python detect_faces.py --config config.yaml
```

### Record Video with Detected Faces

```bash
python detect_faces.py --output video --output_path ./detected_faces.mp4
```

### Command Line Options

```
--config CONFIG           Path to configuration file (default: config.yaml)
--model MODEL            Face detection model (default: cascade)
                        Options: cascade, ssd, yolov3
--confidence CONF        Confidence threshold (default: 0.5)
--output OUTPUT          Output mode (default: display)
                        Options: display, video, none
--output_path PATH       Path to save output video
--fps FPS                Target FPS (default: 30)
--width WIDTH            Frame width (default: 640)
--height HEIGHT          Frame height (default: 480)
--display                Show detection on screen (requires X11)
```

## Project Structure

```
Camera_MIPI_RaspberryPi4/
├── detect_faces.py           # Main detection script
├── config.yaml               # Configuration file
├── requirements.txt          # Python dependencies
├── src/
│   ├── camera.py            # Camera interface
│   ├── detector.py          # Face detection logic
│   ├── models/              # Pre-trained models
│   └── utils.py             # Utility functions
├── trained_models/          # Downloaded models directory
├── output_videos/           # Output video directory
├── detected_faces/          # Detected faces storage
├── calibration/             # Camera calibration data
└── README.md                # This file
```

## Configuration

Edit `config.yaml` to customize:

```yaml
# Camera settings
camera:
  resolution: [640, 480]
  framerate: 30
  rotation: 0

# Detection settings
detection:
  model: "cascade"
  confidence_threshold: 0.5
  
# Output settings
output:
  save_video: true
  save_faces: false
  display: false
```

## Supported Models

| Model | Speed | Accuracy | Memory | Notes |
|-------|-------|----------|--------|-------|
| Cascade Classifier | ⚡⚡⚡ | ⭐⭐⭐ | Low | Fast, good for real-time |
| SSD MobileNet | ⚡⚡ | ⭐⭐⭐⭐ | Medium | Balanced performance |
| YOLOv3 Tiny | ⚡⚡ | ⭐⭐⭐⭐ | Medium | High accuracy |

## API Reference

### Camera Class

```python
from src.camera import Camera

camera = Camera(resolution=(640, 480), framerate=30)
frame = camera.read()
camera.release()
```

### Detector Class

```python
from src.detector import FaceDetector

detector = FaceDetector(model="cascade")
faces = detector.detect(frame)
```

## Performance Tips

1. **Resolution Trade-off**: Lower resolution = faster processing (try 320x240 for weak systems)
2. **Model Selection**: Use Cascade Classifier for maximum speed
3. **Threading**: Enable multi-threading for camera reading
4. **CPU Tuning**: Consider overclocking carefully
5. **Cooling**: Ensure proper heat dissipation

## Troubleshooting

### Camera Not Detected

```bash
# Check camera connection
vcgencmd get_camera

# Test camera
raspistill -o test.jpg
```

### Low FPS Performance

- Reduce frame resolution
- Switch to faster model (Cascade)
- Disable display output
- Lower confidence threshold

### Memory Issues

- Reduce frame buffer size
- Lower resolution
- Use lighter model

## Contributing

Contributions are welcome! Please:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## References

- [Raspberry Pi Documentation](https://www.raspberrypi.com/documentation/)
- [OpenCV Face Detection](https://docs.opencv.org/4.x/df/d2d/group__objdetect__cascades.html)
- [MIPI CSI Camera](https://www.raspberrypi.com/documentation/accessories/camera.html)

## Acknowledgments

- Raspberry Pi Foundation for excellent documentation
- OpenCV community for powerful computer vision tools
- Contributors and testers

## Support

For issues, questions, or suggestions:

- 📧 Open an issue on GitHub
- 💬 Start a discussion
- 📝 Check existing issues first

---

**Last Updated:** May 24, 2026  
**Maintainer:** [@aaabdelaziz](https://github.com/aaabdelaziz)
