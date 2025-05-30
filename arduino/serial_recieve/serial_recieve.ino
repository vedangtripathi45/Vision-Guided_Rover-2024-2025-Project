// This arduino code is used to recieve the incoming data from the microprocessor via serial communication. It recieves data in a list.

String inputString = "";  // String to store incoming data
bool stringComplete = false;  // Flag to indicate when input is complete
float processedNumbers[5];  // Array to store processed numbers (max size 20)
int processedCount = 0;      // Count of processed numbers
float yaw_angle;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  // Start serial communication
  Serial.println("Arduino Ready!");

}

void loop() {
  // put your main code here, to run repeatedly:

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
