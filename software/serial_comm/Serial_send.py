import serial
import time
import numpy as np
import cv2
end_flag=0

cap = cv2.VideoCapture(1)


cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)

arduino = serial.Serial(port='COM6', baudrate=115200, timeout=1) # Baud Rate and port are set accordingly;
time.sleep(1)  # Allow time for the connection to establish
port=0
count=0
o=0
ids=[1,25,43,57,75,98]


while cap.isOpened():
    #cv2.imshow('Estimated Pose', img)
    try:
        while True:
            if o==1:
                arduino = serial.Serial(port='COM6', baudrate=115200, timeout=1)
                time.sleep(1)
                o=0
            ret, img = cap.read()
            rid=ids[rin]
            print(rid)
            #frame=pose_estimation(img, ARUCO_DICT[aruco_type], intrinsic_camera, distortion)
            numbers = []  # Data list to be sent
            data_to_send = ",".join(map(str, numbers)) + "\n"
            if ((dist>0.3 or dist ==0) and id==cid):
                arduino.write(data_to_send.encode('utf-8'))
                #cv2.imshow('Estimated Pose', img)

                # Read response from Arduino
                if arduino.in_waiting > 0:
                    response = arduino.readline().decode('utf-8').strip()
                    print(f"Arduino: {response}")

                time.sleep(0.1)  # Wait before sending again
                count+=1
                if count == 30:        # Refresh connection after every 30 frames
                    arduino.close()
                    o=1
                    count=0
                
            elif((dist<=0.3 and dist!=0)):    # case when a marker is approached send no data and disconnect at the last id
                if end_flag!=1:
                    rin+=1
                elif end_flag==1:
                    time.sleep(2)
                    print("Closed.")
                    arduino.close()
                    cap.release()
                    break
            if(id!=0 and id==cid):
                pid=id
            
            

    except (serial.serialutil.SerialException or serial.SerialException):  # Exception handling -> Change to the new port
        arduino.close()
        time.sleep(0.5)

        if port==0:
            port=0
            print(f"port changed to {port}")

            arduino = serial.Serial(port='COM6', baudrate=115200, timeout=1)
            
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