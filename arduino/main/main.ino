//left motors
#define IN1 5 // Motor A direction
#define IN2 6  // Motor A direction
//right motors
#define IN3 9 // Motor B direction
#define IN4 10 // Motor B direction

String inputString = "";  // String to store incoming data
bool stringComplete = false;  // Flag to indicate when input is complete
float processedNumbers[5];  // Array to store processed numbers (max size 20)
int processedCount = 0;      // Count of processed numbers
float distance;
float Tdistance=0.3;
char command;
bool r=0;
bool flag=1;
int count=0;
int left_motor_speed;
int right_motor_speed;
// PID parameters
float Kp = 5.0;   // Proportional gain
float Ki = 0.8;   // Integral gain
float Kd = 1.2;   // Derivative gain
  
float yaw_angle;
float target_angle = 0;    // Desired yaw angle (0 degrees)
float error = 0, prev_error = 0, integral = 0, derivative = 0;
float pid_output = 0;

void setup() {
  Serial.begin(115200);  // Start serial communication
  Serial.println("Arduino Ready!");
  pinMode(13,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
}

// Function to capture incoming data
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();  // Read the next character
    inputString += inChar;             // Append the character to the input string
    if (inChar == '\n') {              // Check for end of input (newline character)
      stringComplete = true;           // Mark input as complete
    }
  }
}

// Function to process the list of floating-point numbers
void processFloats(String data) {
  //Serial.println("Processing numbers:");

  // Split the string into individual numbers based on commas
  char *ptr = strtok((char *)data.c_str(), ",");
  while (ptr != NULL) {
    float number = atof(ptr);  // Convert each substring to a float

    // Store the processed number in the array
    if (processedCount < 20) {  // Ensure we don't exceed the array size
      processedNumbers[processedCount++] = number;
    } else {
      //Serial.println("Error: Too many numbers received!");
      break;
    }

    //Serial.print("Number: ");
    //Serial.println(number);

    ptr = strtok(NULL, ",");  // Get the next number
  }
}

void moveForward(int spd) {
  analogWrite(IN1, spd);
  analogWrite(IN2, 0);

  analogWrite(IN3, spd);
  analogWrite(IN4, 0);
}



void setMotorSpeeds(int leftSpeed, int rightSpeed) {
  // Set motor A (left motor)
  analogWrite(IN1, leftSpeed);
  analogWrite(IN2, 0);
  // Set motor B (right motor)
  analogWrite(IN3, rightSpeed);
  analogWrite(IN4, 0);
}

void controlMotors(float pid_output, float yaw) {
  // Determine motor speeds
  if (yaw>0){
    left_motor_speed = constrain(210 , 60, 255);
    right_motor_speed = constrain(190 - abs(2.3*pid_output), 60, 255);
  }
  else if (yaw<0){
    left_motor_speed = constrain(190 - abs(2.3*pid_output), 60, 255);
    right_motor_speed = constrain(210 , 60, 255);
  }

  // If the error is small, move forward
  if (abs(error) < 5 and error!=0) {
    moveForward(190);
  }
  // Otherwise, adjust motor speeds for alignment

  else if(abs(error) < 5 and error ==0){
    moveForward(0);
  }
  else {
    setMotorSpeeds(left_motor_speed, right_motor_speed);
  }
}


void rotate(int speed) {
  Serial.print("Rotating");
  analogWrite(IN2, speed);
  analogWrite(IN1, 0);

  analogWrite(IN4, 0);
  analogWrite(IN3, speed);
  delay(400);
  analogWrite(IN2,0);
  analogWrite(IN1, 0);

  analogWrite(IN4, 0);
  analogWrite(IN3, 0);
  delay(100);
  
}

void loop() {
  // When a complete list has been received
  if (stringComplete and distance!=0.1){
      if (flag == 0)
      {
        count = count+1;
      }
      flag=1;
    //Serial.print("Received list: ");
      //Serial.println(inputString);  // Print the received string
      
      // Process the floating-point numbers and store in array
      processFloats(inputString);
      yaw_angle=processedNumbers[0];
      //command=processedNumbers[2];
  
      distance=processedNumbers[1];

      /*if(abs(yaw_angle)<15){
        digitalWrite(13,HIGH);
      }
  
      else if(abs(yaw_angle)>15){
        digitalWrite(13,LOW);
      }*/
      
      // Print the packed list of processed numbers
      //Serial.println("Packed processed numbers:");
      
  
  
      // Calculate PID error terms
      error = target_angle - yaw_angle;
      integral += error; // Accumulate integral
      derivative = error - prev_error;
  
      // Compute PID output
      
      pid_output = Kp * error + Ki * integral + Kd * derivative;
      Serial.print("[");
      Serial.print(processedNumbers[0]);
      Serial.print(' ');
      Serial.print(processedNumbers[1]);
      Serial.print(' ');
      Serial.print(processedNumbers[2]);
      Serial.print(' ');
      Serial.print(count);
      Serial.println("] , ");
  
      ////if(yaw_angle!=0){
       // moveForward(150);
     // }
      //else{
       // moveForward(0);
     // }
      
      //Serial.println(pid_output);
      // Control motors based on PID output
      //if(distance>Tdistance){
      controlMotors(pid_output, yaw_angle);
      //}
      //else if((distance<=Tdistance) and (distance!=0.0)){
        //delay(5000);
        //moveForward(0);
        
      //}
      
      
  
      // Update previous error
      prev_error = error;
  
      // Clear the input string and reset the count
      inputString = "";
      stringComplete = false;
      processedCount = 0;
      //delay(100);
  }

  else if(distance==0.1 and count<3){
    if (flag==1){
      moveForward(0);
      flag=0;
    }
    delay(100);
    if(processedNumbers[2]!=57.00){
      rotate(210);
    }
    else if(processedNumbers[2]==57.00){
      rotate(0);
    }
  }
  /*else if(distance==0.1 and count==3){
    rotate(0);
  }*/




  
}
