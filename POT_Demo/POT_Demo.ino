/*
  POT Demo
  This test is designed to demostrate the possibilities with a potentiometer
*/

/* 
 *  CURRENT PROBLEMS:
 *  - Blue LED 2/3 don't light. Need to test voltage from pin
 *  - Red led is dim
 */

// Define Variables
unsigned int actual_position;
unsigned int assumed_position;
unsigned int actuator_position;

const int Led_red = 3;
const int Led_blue1 = 2;
const int Led_blue2 = A6;
const int Led_blue3 = A7;
void setup() {
  // Initialize Serial Comm
  Serial.begin(9600);
  
  // Set Variables: POT
  actual_position = 0;
  assumed_position = 0;

  // Set Variables: Actuator
  actuator_position = 0;
}

void loop() {
  // Read POT
  int actual_position = analogRead(A1);

  // Control Position
  if (actual_position >= 700 && actual_position <= 795) {
      digitalWrite(Led_red, LOW);
      digitalWrite(Led_blue1, LOW);
      digitalWrite(Led_blue2, LOW);
      digitalWrite(Led_blue3, LOW);
      assumed_position = 0;
      digitalWrite(Led_red, HIGH);
  
  } else if (actual_position >= 796 && actual_position <= 869) {
      digitalWrite(Led_red, LOW);
      assumed_position = 1;
      digitalWrite(Led_blue1, HIGH);
  
  } else if (actual_position >= 870 && actual_position <= 990) {
      digitalWrite(Led_blue1, LOW);
      assumed_position = 2;
      digitalWrite(Led_blue2, HIGH);
  
  } else if (actual_position >= 991) {
      digitalWrite(Led_blue2, LOW);
      assumed_position = 3;
      digitalWrite(Led_blue3, HIGH);
  }
   
  // Control Module
  //if (actual_position >= 1) {
    //analogWrite(2, 255);
  //}
  
  // Debug: Print
  if (actual_position >= 700 && actual_position <= 795) {
      Serial.println("Off - 0");
  } else if (actual_position >= 796 && actual_position <= 869) {
      Serial.println("Low - 1");
  } else if (actual_position >= 870 && actual_position <= 990) {
      Serial.println("Medium - 2");
  } else if (actual_position >= 991) {
      Serial.println("High - 3");
  }
 
  // Delay Execution
  delay(500);
}
