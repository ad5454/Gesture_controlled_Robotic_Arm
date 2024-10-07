import cv2
import serial
import time
import mediapipe as mp
import atexit

# Setup for serial communication
ser = serial.Serial("COM8", 9600)
time.sleep(2)

# Initialize MediaPipe Hand detection
mp_hands = mp.solutions.hands
hands = mp_hands.Hands(max_num_hands=1, min_detection_confidence=0.7)
mp_draw = mp.solutions.drawing_utils

# Initialize webcam capture
cap = cv2.VideoCapture(0)

# Function to send "00" on program exit
def send_home_position():
    ser.write("$00".encode())
    print("Sent data: 00 (Home position)")

# Register the send_home_position function to be called on program exit
atexit.register(send_home_position)

# Ensure "00" is sent right after opening the camera
ser.write("00".encode())
print("Sent data: 00 (Home position)")

def detect_fingers(lmlist):
    """
    Detect if each finger is extended or not based on landmarks.
    Returns a list of 0s and 1s where 1 means extended and 0 means not extended.
    """
    fingers = [0, 0, 0, 0, 0]

    # Thumb
    fingers[0] = 1 if lmlist[4][0] > lmlist[3][0] else 0

    # Index Finger
    fingers[1] = 1 if lmlist[8][1] < lmlist[6][1] else 0

    # Middle Finger
    fingers[2] = 1 if lmlist[12][1] < lmlist[10][1] else 0

    # Ring Finger
    fingers[3] = 1 if lmlist[16][1] < lmlist[14][1] else 0

    # Pinky Finger
    fingers[4] = 1 if lmlist[20][1] < lmlist[18][1] else 0

    return fingers

while True:
    success, img = cap.read()
    if not success:
        print("Failed to capture image")
        break

    img_rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    results = hands.process(img_rgb)

    data = "00"  # Default data to send if no gesture is detected

    if results.multi_hand_landmarks:
        for handLms in results.multi_hand_landmarks:
            mp_draw.draw_landmarks(img, handLms, mp_hands.HAND_CONNECTIONS)
            lmlist = [(lm.x, lm.y) for lm in handLms.landmark]

            # Assuming we use the wrist for position detection
            wrist_x, wrist_y = lmlist[0][0] * img.shape[1], lmlist[0][1] * img.shape[0]

            # Determine location based on a 2x4 grid
            if wrist_x <= img.shape[1] / 4:
                if wrist_y <= img.shape[0] / 2:
                    location = "1"  # Top-left
                else:
                    location = "5"  # Bottom-left
            elif wrist_x <= img.shape[1] / 2:
                if wrist_y <= img.shape[0] / 2:
                    location = "2"  # Top-center-left
                else:
                    location = "6"  # Bottom-center-left
            elif wrist_x <= img.shape[1] * 3 / 4:
                if wrist_y <= img.shape[0] / 2:
                    location = "3"  # Top-center-right
                else:
                    location = "7"  # Bottom-center-right
            else:
                if wrist_y <= img.shape[0] / 2:
                    location = "4"  # Top-right
                else:
                    location = "8"  # Bottom-right

            # Detect fingers
            fingers = detect_fingers(lmlist)

            # Gripper status based on fingers
            gripper_status = "1" if any(fingers) else "0"  # Gripper closed if any finger is extended

            # Only send updated data if a gesture is detected
            data = f"${location}{gripper_status}"

    # Send serial data continuously (either "00" or the detected gesture)
    ser.write(data.encode())
    print(f"Sent data: {data}")

    # Show the image
    cv2.imshow("Image", img)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# On exiting the while loop, ensure "00" is sent before closing
send_home_position()

cap.release()
cv2.destroyAllWindows()
ser.close()
