//------------------------------------------------------------------------------
// Section [#1]: Information, Notes, Etc
// *************************************** \\
// Ultrasonic sensor
// By Alec M. - (https://github.com/amattu2)
//
// Originally: Ardunio Nano V3.0 ATMEGA328P Module CH340G - 2/13/2016 @ 4:10PM
// *************************************** \\

// *************************************** \\
// ** Version: V0.27 **
// ** Contains a known bug: Yes **
// ** Beta Version: Yes **
// ** Last Save: 2/15/2016 @ 7:09 PM V0.27**
// ** All saves must:
// ** 1. Count as a new version (version + 0.01 typically)
// ** 2. Have a verification run on it, to verify it would work.
// *************************************** \\

// *************************************** \\
// [- Version Keynotes -] Beta
// - First ever working version.
// - Added 'constant'/variables for the LED pins.
// - Added LED if-statement for testing.
// - Fixed the miss-placed } which caused an error
// - Added constants: mread, beeppin, tone1, tone2
// - Added IF-Statements for Warn & Beep
// - Redefined LED3 to D4 instead of A4 cause it was swapped.
// - Added some explanations.
// - If measurement is over 200 inches (default) then it will restart.
// - IF-#4 now turns off green led, and turns on purple one.
// - Now only displays distance if within the defined variable.
// - Now shows the distance you need to backup if within 12in
// - IF-#4 now turns off the green LEd first, to conserve energy.
// - IF-#5 now displays if the sensor didn't read.
// - IF-#5 now uses "tone1" not "tone2"
// [- End of Version Keynotes -]

// [- Version Breadboard Updates -]
// - Now uses flat designed wires, instead of a mess of wires.
// - Fixed the red LED not working, due to a misplaced pin..
// - Fixed the noread issue. A unplugged +5V pin next to A7.
// - Used a different pizeospeaker
// [- End of Breadboard Updates -]
// *************************************** \\

//------------------------------------------------------------------------------
// Section [#2]: Initalization, Constants, Pin Numbers

// Sensors, Hearing Aid
const int Trigpin = A7;                    // Blue cord   // Trigger pin on the sensor goes to pin A7 on the Ardunio.
const int Echopin = 2;                     // Yellow cord // Echo pin on the sensor goes to pin D2 on the Arduino.
const int Beeppin = 12;                    // Piezospeaker anode pin goes to A3 on the Arduino.

// LEDs, Visual Aid (Pin numbers)
const int Led4 = A4;                       // Con: Pin D4, Purple/Violet
const int Led2 = 5;                        // Con: Pin D5, Yellow
const int Led3 = 4;                        // Con: Pin A4, Red
const int Led1 = A5;                       // Con: Pin A5, Green

// Distances (In inches)
const int Warn = 24;                        // The distance at which the warn variable becomes active. In inches. Default is 24in.
const int Beep = 12;                        // The distance at which the buzzer becomes active. In inches. Default is 12in.
const int Mread = 200;                      // The distance at which it determines if the sensor misread the distance. Default is 200in.
const int Nread = 0;                        // The distance at which it determines if the sensor is disconnected. Default is, and should remain 0in.

// Piezospeaker Frequency
const int Tone1 = 3950;                     // #1: Ultra-pitch. Very ear piercing. Use only if needed.
const int Tone2 = 3500;                     // #2: High-pitch.

//------------------------------------------------------------------------------
// Section [#3]: Beginning Code, More Initalizing


void setup() {
  Serial.begin(9600);                      // initialize serial communication
}

void loop() {
  // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  long duration, inches;

//------------------------------------------------------------------------------
// Section [#4A]: Measurements Sending, Input/Output
reread:

  pinMode(Trigpin, OUTPUT);        // Set pin to send pulses/output.
  digitalWrite(Trigpin, LOW);      // Turn the pin off, just to verify it wasn't on before.
  delayMicroseconds(50);           // Wait 0.05 seconds.
  digitalWrite(Trigpin, HIGH);     // Send the pulses out.
  delayMicroseconds(5);            // Wait .005 seconds
  digitalWrite(Trigpin, LOW);      // Turn the pulses off.

//**************************************\\
// Section [#4B]: Measurement Receiving

  pinMode(Echopin, INPUT);           // Set pin to take input.
  pinMode(Echopin, HIGH);            // Turn the pin on.
  delayMicroseconds(1);              // Pause for 0.001 Second.
  pinMode(Echopin, LOW);             // Turn pin off.
  duration = pulseIn(Echopin, HIGH); // Idk what this does, it's from the original..

//------------------------------------------------------------------------------
// Section [#5]: Coversion (Time to inches)

  inches = microsecondsToInches(duration);

//------------------------------------------------------------------------------
// Section [#6]: If-Statements [No read]

if (inches == Nread)         // Start IF-#5: If the sensor reads 0, then goto the beginning.
{
  digitalWrite(Led1, LOW);
  digitalWrite(Led4, HIGH);
  tone(Beeppin, Tone1);
  Serial.print("Sensor did not read: (");
  Serial.print(inches);
  Serial.println("in)");
  delay(300);
  goto reread;
}                        // End IF-#5
else if (inches != Nread)
{
  noTone(Beeppin);
  digitalWrite(Led4, LOW);
}

//**************************************\\
// Section [#6A]: If-Statements [Green LED]
greenled:

if (inches > Warn)            // Start IF-#1: If object is further than 24 inches or variable "warn" then turn on the green LED.
{
  digitalWrite(Led1, HIGH);
}
else if (inches < Warn)       // Else, if it is closer than 24 inches turn it off.
{
  digitalWrite(Led1, LOW);
}                             // End IF-#1

//**************************************\\
// Section [#6B]: If-Statements [Yellow LED]

if (inches < Warn)            // Start IF-#2: If closer than 24 inches (default) turn off the green LED, and turn on the yellow one.
{
  digitalWrite(Led2, HIGH);
  Serial.print(inches);
  Serial.print("in");
  Serial.println();
}
else if (inches > Warn)
{
  digitalWrite(Led2, LOW);
}                            // End IF-#2

//**************************************\\
// Section [#6C]: If-Statements [Red LED, Piezospeaker]

if (inches < Beep)           // Start IF-#3: If closer than 12 inches (default) then sound the buzzer and show on the screen.
{
  digitalWrite(Led2, LOW);
  digitalWrite(Led3, HIGH);
  tone(Beeppin, Tone2);
  Serial.print("Please reduce following distance by");
  Serial.print(" ");
  Serial.print(Warn - inches);
  Serial.print("in");
  Serial.println();
  delay(300);
}
else if (inches > Beep)
{
  digitalWrite(Led3, LOW);
  noTone(Beeppin);
}                           // End IF-#3

//**************************************\\
// Section [#6D]: If-Statements [Error]

if (inches > Mread)         // Start IF-#4: If the distance is over 200 then the sensor probably misread, and goto the beginning.
{
  digitalWrite(Led1, LOW);         // Turn off the green led.
  digitalWrite(Led4, HIGH);        // Turn on the purple one.
  Serial.println("Whoops! Sensor misread data.");
  delay(300);
  digitalWrite(Led4, LOW);
  goto reread;
}                           // End IF-#4

//------------------------------------------------------------------------------
// Section [#7]: End of loop

delay(300);
}

//------------------------------------------------------------------------------
// Section [#8]: True Conversion.

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}
//------------------------------------------------------------------------------
// Section [End]: Program is over.
