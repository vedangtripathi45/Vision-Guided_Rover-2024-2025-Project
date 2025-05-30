int left_motor_speed;
int right_motor_speed;
// PID parameters
float Kp = 5.0;   // Proportional gain
float Ki = 0.8;   // Integral gain
float Kd = 1.2;   // Derivative gain

float yaw_angle; // comes via serial data
float target_angle = 0;    // Desired yaw angle (0 degrees)
float error = 0, prev_error = 0, integral = 0, derivative = 0;
float pid_output = 0;
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  // Calculate PID error terms
      error = target_angle - yaw_angle;
      integral += error; // Accumulate integral
      derivative = error - prev_error;
  
      // Compute PID output
      
      pid_output = Kp * error + Ki * integral + Kd * derivative;
      controlMotors(pid_output, yaw_angle);

      // Update previous error
      prev_error = error;
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
    setMotorSpeeds(left_motor_speed, right_motor_speed);  /function in motor control
  }
}
