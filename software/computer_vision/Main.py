import serial
import time
import numpy as np
import cv2
import os
def reset_usb(path):
    password='12345678'
    try:
        unbind_path=f"/sys/bus/usb/devices/{path}/remove"
        os.system(f"echo '{password}' | sudo -S tee {unbind_path}")
        time.sleep(2)
        os.system(f"echo '{password}' | sudo -S sh -c 'echo 1 > /sys/bus/usb/drivers_probe'")
        print(f"USB Resetted")
    except Exception as e:
        print("Failed")
end_flag=0
ARUCO_DICT = {
    "DICT_4X4_50": cv2.aruco.DICT_4X4_50,
    "DICT_4X4_100": cv2.aruco.DICT_4X4_100,
    "DICT_4X4_250": cv2.aruco.DICT_4X4_250,
    "DICT_4X4_1000": cv2.aruco.DICT_4X4_1000,
    "DICT_5X5_50": cv2.aruco.DICT_5X5_50,
    "DICT_5X5_100": cv2.aruco.DICT_5X5_100,
    "DICT_5X5_250": cv2.aruco.DICT_5X5_250,
    "DICT_5X5_1000": cv2.aruco.DICT_5X5_1000,
    "DICT_6X6_50": cv2.aruco.DICT_6X6_50,
    "DICT_6X6_100": cv2.aruco.DICT_6X6_100,
    "DICT_6X6_250": cv2.aruco.DICT_6X6_250,
    "DICT_6X6_1000": cv2.aruco.DICT_6X6_1000,
    "DICT_7X7_50": cv2.aruco.DICT_7X7_50,
    "DICT_7X7_100": cv2.aruco.DICT_7X7_100,
    "DICT_7X7_250": cv2.aruco.DICT_7X7_250,
    "DICT_7X7_1000": cv2.aruco.DICT_7X7_1000,
    "DICT_ARUCO_ORIGINAL": cv2.aruco.DICT_ARUCO_ORIGINAL,
    "DICT_APRILTAG_16h5": cv2.aruco.DICT_APRILTAG_16h5,
    "DICT_APRILTAG_25h9": cv2.aruco.DICT_APRILTAG_25h9,
    "DICT_APRILTAG_36h10": cv2.aruco.DICT_APRILTAG_36h10,
    "DICT_APRILTAG_36h11": cv2.aruco.DICT_APRILTAG_36h11}
def aruco_display(corners, ids, rejected, image):
    if len(corners) > 0:
        ids = ids.flatten()
        for (markerCorner, markerID) in zip(corners, ids):
            # Reshape and extract corner points
            corners = markerCorner.reshape((4, 2))
            (topLeft, topRight, bottomRight, bottomLeft) = corners

            # Convert to integer coordinates
            topRight = (int(topRight[0]), int(topRight[1]))
            bottomRight = (int(bottomRight[0]), int(bottomRight[1]))
            bottomLeft = (int(bottomLeft[0]), int(bottomLeft[1]))
            topLeft = (int(topLeft[0]), int(topLeft[1]))

            # Draw lines between corners
            cv2.line(image, topLeft, topRight, (0, 255, 0), 2)
            cv2.line(image, topRight, bottomRight, (0, 255, 0), 2)
            cv2.line(image, bottomRight, bottomLeft, (0, 255, 0), 2)
            cv2.line(image, bottomLeft, topLeft, (0, 255, 0), 2)

            # Compute the center of the marker
            cX = int((topLeft[0] + bottomRight[0]) / 2.0)
            cY = int((topLeft[1] + bottomRight[1]) / 2.0)
            cv2.circle(image, (cX, cY), 4, (0, 0, 255), -1)

            # Put marker ID text
            cv2.putText(image, str(markerID), (topLeft[0], topLeft[1] - 10),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
            print("[Inference] ArUco marker ID: {}".format(markerID))
    return image

def yaw(rid,frame, aruco_dict_type, matrix_coefficients, distortion_coefficients, marker_length=0.072):
    dict={}
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    aruco_dict = cv2.aruco.Dictionary_get(aruco_dict_type)
    parameters = cv2.aruco.DetectorParameters_create()
    corners, ids, rejected_img_points =cv2.aruco.detectMarkers(gray, aruco_dict, parameters=parameters)
    image_height, image_width = frame.shape[:2]
    camera_center = (image_width // 2, image_height // 2)

    if len(corners) > 0:
        # Draw detected markers
        

        # Estimate pose for each marker
        rvecs, tvecs, _ = cv2.aruco.estimatePoseSingleMarkers(corners, marker_length, matrix_coefficients, distortion_coefficients)

        for i in range(len(ids)):
            markCorners=corners[i].reshape(4,2)
            (topLeft, topRight, bottomRight, bottomLeft) = markCorners

            # Convert to integer coordinates
            tvec = tvecs[i][0]
    
            # Translation vector components

            x = tvec[0]  # Position along x-axis (horizontal distance)
            y = tvec[1]  # Position along y-axis (vertical distance)
            z = tvec[2]  # Position along z-axis (depth/distance from camera)

            marker_positon=tvec
            camera_position=np.array([0,0,0])

            trajectory_vector=marker_positon-camera_position
            distance=np.linalg.norm(trajectory_vector)

            # Yaw (horizontal angle, in degrees)
            yaw_angle = np.arctan2(x, z) * 180 / np.pi
            dict[ids[i][0]]=[yaw_angle,distance,ids[i][0]]
        if rid in dict:
            l=dict[rid]
        else:
            l=[0,0,0] 
    else:
        l=[0,0,0]
    return l


aruco_type = "DICT_4X4_100"

arucoDict = cv2.aruco.Dictionary_get(ARUCO_DICT[aruco_type])

arucoParams = cv2.aruco.DetectorParameters_create()


intrinsic_camera = np.array(((933.15867, 0, 657.59),(0,933.1586, 400.36993),(0,0,1)))
distortion = np.array((-0.43948,0.18514,0,0))


cap = cv2.VideoCapture(0)
p=[30,1,25,57]
index=0
pid=30
cid=30

cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)

arduino = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=1)
time.sleep(1)  # Allow time for the connection to establish
port=0
count=0
o=0
ids=[1,25,57]
rin=0
f=0

while cap.isOpened():
    #cv2.imshow('Estimated Pose', img)
    try:
        while True:
            if o==1:
                arduino = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=1)
                time.sleep(1)
                o=0
            # Send the list of floating-point numbers
            #print(f"Sending: {data_to_send.strip()}")
            ret, img = cap.read()
            rid=ids[rin]
            #print(rid)
            #frame=pose_estimation(img, ARUCO_DICT[aruco_type], intrinsic_camera, distortion)
            l=yaw(rid,img, ARUCO_DICT[aruco_type], intrinsic_camera, distortion)
            #print(l)
            dist=l[1]
            id=l[2]
            #print(id)
            if(id!=pid and id!=0):
                if id == p[index+1]:
                    index+=1
                else:
                    id = 0
                if id == p[-1]:
                    end_flag=1
            print(cid,rid,dist)
            cid=p[index]
            
            numbers = l
            data_to_send = ",".join(map(str, numbers)) + "\n"
            #print(numbers)
            if ((dist>0.4 or dist ==0) and id==cid):
                #print('Hi')
                arduino.write(data_to_send.encode('utf-8'))
                #cv2.imshow('Estimated Pose', img)

                # Read response from Arduino
                if arduino.in_waiting > 0:
                    response = arduino.readline().decode('utf-8').strip()
                    print(f"Arduino: {response}")

                time.sleep(0.1)  # Wait before sending again
                count+=1
               # print(count)
                if count == 20:
                    arduino.close()
                   # print("yo")
                    o=1
                    count=0
                
            elif((dist<0.4 and dist!=0)):
                numbers[1]=0.1
                data=",".join(map(str, numbers)) + "\n"
                arduino.write(data.encode('utf-8'))
                count=19
                if end_flag!=1:
                    rin+=1
                
                elif end_flag==1:
                    time.sleep(2)
                    print("Closed.")
                    arduino.close()
                    cap.release()
                    break
                time.sleep(0.1)
            if(id!=0 and id==cid):
                pid=id
            
            

    except (serial.serialutil.SerialException or serial.SerialException):
        arduino.close()
        time.sleep(1)
        o=1
        print(f"port change")
        reset_usb("1-2:1.0")
        

           
            
        #elif port==0:
        #    print(f"port changed to {port}")

        #    arduino = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=1)
        #    port=1


        continue
    except KeyboardInterrupt:
        print("Exiting...")
    finally:
        arduino.close()

    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()