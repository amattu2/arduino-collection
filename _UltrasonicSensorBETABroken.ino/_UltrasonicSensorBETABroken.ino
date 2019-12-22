//------------------------------------------------------------------------------
// Section [#1]: Information, Notes, Etc
// ***************************************
// Ultrasonic sensor
// By Alec M. - (https://github.com/amattu2)
//
// Originally: Ardunio Nano V3.0 ATMEGA328P Module CH340G - 2/13/2016 @ 4:10PM
// ***************************************

// *************************************** \\
// ** Version: 0.11 **                     \\
// ** Beta Version: Yes **                 \\
// *************************************** \\

// *************************************** \\
// Version Keynotes -[
// - First ever working version.
// - Added 'constant'/variables for the LED pins.
// -
// *************************************** \\

//------------------------------------------------------------------------------
// Section [#2]: Initalization, Constants, Pin Numbers

// *************************************** \\
const int Trigpin = A7;                    // Trigger pin on the sensor goes to pin A7 on the Ardunio.
const int Echopin = 2;                     // Echo pin on the sensor goes to pin D2 on the Arduino.
const int Led4 = 4;                        // Con: Pin D4, Purple/Violet
const int Led2 = 5;                        // Con: Pin D5, Yellow
const int Led3 = A4;                       // Con: Pin A4, Red
const int Led1 = A5;                       // Con: Pin A5, Green
// *************************************** \\

//------------------------------------------------------------------------------
// Section [#3]: Beginning Code, Measurements

// *************************************** \\
void setup() {
  Serial.begin(9600);                      // initialize serial communication:
}
// *************************************** \\

// *************************************** \\
void loop() {
  // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  long duration, inches;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(Trigpin, OUTPUT);
  digitalWrite(Trigpin, LOW);
  delayMicroseconds(50);
  digitalWrite(Trigpin, HIGH);
  delayMicroseconds(5);
  digitalWrite(Trigpin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(Echopin, INPUT);
  pinMode(Echopin, HIGH);
  delayMicroseconds(1);  // added by me
  pinMode(Echopin, LOW); //added by me
  duration = pulseIn(Echopin, HIGH);


  // convert the time into a distance
  inches = microsecondsToInches(duration);

  Serial.print(inches);
  Serial.print("in, ");
  Serial.println();

  delay(100);
}

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}
