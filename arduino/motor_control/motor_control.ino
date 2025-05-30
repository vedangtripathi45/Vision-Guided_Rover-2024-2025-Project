//left motors
#define IN1 5 // Motor A direction
#define IN2 6  // Motor A direction
//right motors
#define IN3 9 // Motor B direction
#define IN4 10 // Motor B direction

int left_motor_speed;
int right_motor_speed;
float distance;// From Serial Data
float yaw_angle;// From Serial Data
//pin numbers are set accordingly

void setup() {
  // put your setup code here, to run once:
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (stringComplete and distance!=0.1){
    controlMotors(pid_output, yaw_angle); //move according to the PID Output
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
}

void moveForward(int spd) {
  analogWrite(IN1, spd);
  analogWrite(IN2, 0);

  analogWrite(IN3, spd);
  analogWrite(IN4, 0);
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
