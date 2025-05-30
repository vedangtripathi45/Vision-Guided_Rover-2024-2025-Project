## Vision Guided Rover
Annual Project - Robotics Club, MNNIT Allahabad

This is the Robotics Club's annual project repository for the Vision Guided Rover. It includes Python code for computer vision and navigation, Arduino firmware, and SolidWorks designs.

The project is about building an autonomous bot that is capable of trversing a specified by itself without any human intervention by using the feed captured by its camera.
It uses computer vision in python for capturing data and image processing, PID for control. The entire process runs on NVIDIA Jetson Nano and Arduino Uno R3.

## 👥 Mentors

| Name               | Year           |
|--------------------|----------------|
| [Sudhanshu Ranjan] | Final Year     |
| [Dhruv Chandhok]   | Pre-Final Year |
| [Kushagra Verma]   | Pre-Final Year |



## 👥 Team Members

|        Name       |
|-------------------|
| [Vedang Tripathi] |
| [Divyansh Pandey] |
| [Harsh Agrawal]   |
| [Kumaradithya]    |
| [Kritika Parihar] |
| [Aman Shukla]     |
| [Anika Saxena]    |
| [Ishant Goyal]    |
| [Kanishk Agrawal] |


## ⚙️ Tech stacks used for this project :

->Arduino IDE
->C++
->Python 
->OpenCV
->NumPy
->SolidWorks 
->PID Control System

->Serial Communication

## 🖥️ Hardware Used

| Component             | Role                                                   |
|-----------------------|--------------------------------------------------------|
| NVIDIA Jetson Nano    | Processes camera feed using computer vision            |
| Arduino Uno R3        | Controls motors and reads sensor data                  |
| USB Camera            | Captures live video stream                             |
| Motor Driver (L298N)  | Drives motors based on Arduino commands                |
| N25 Encoder Motors    | Runs the rover and encoders keep track of the distance |
| Ultrasonic/IR Sensors | Obstacle detection or wall following                   |
| Chassis + Wheels      | Base structure of the rover                            |

Communication: **Jetson Nano ↔ Arduino Uno** via **Serial (USB)**

## 📁 Repository Structure

├── hardware/ → SolidWorks models and wiring diagrams
│ ├── solidworks/ → CAD assemblies and parts
│ └── schematics/ → Circuit layouts
├── arduino/ → Arduino code for PID and motor control
│ ├── motor_control/
│ └── pid_control/
├── software/ → Python code for image processing and navigation
│ ├── computer_vision/
│ └── serial_comm/
├── docs/ →flowchart, equations etc.
├── README.md → You're here!
└── .gitignore → Git ignore rules
