# Gesture_controlled_Robotic_Arm

Introduction
This repository contains the code and instructions for building a gesture-controlled robotic arm. The project utilizes computer vision techniques to track hand movements and translate them into instructions for the robotic arm.

This is a gesture-controlled robotic arm.

![Robotic arm](images/robotic_arm.jpg)

The arm uses computer vision to track hand movements and replicate them.

![Hand tracking](images/hand_tracking.png)


Hardware Requirements
Robotic arm with 5 servo motors and 1 stepper motor
Webcam
Software Requirements
Python 3.7.x (compatible with versions less than 3.8)
Arduino IDE
OpenCV
MediaPipe
SerialDevice
cvzone


Installation
Clone the repository:
Bash
git clone https://github.com/your-username/gesture-controlled-robotic-arm


Usage
Open the main.py file in your Python IDE.
Run the script.
Position your hand in front of the webcam.
The system will track your hand movements and control the robotic arm accordingly.

Implementation
The project uses the following steps:
Capture video from the webcam.
Detect hands in the video frame using MediaPipe.
Determine the position of the hand relative to a 4x2 grid.
Generate instructions for the robotic arm based on the hand position.
Send instructions to the robotic arm using serial communication.


Contributions
Contributions are welcome! Feel free to fork this repository, make your changes, and submit a pull request.
