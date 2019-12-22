//------------------------------------------------------------------------------
// Section [#1]: Information, Notes, Etc
// *************************************** \\
// Ultrasonic sensor Distance Control System
// * Designed to keep your monitor your distance from another vehicle using Ultrasonic range sensing technology.
// ** Notifies you via beeping instead of lights, complying with most new vehicle notfication regulations.
// By Alec M. - (https://github.com/amattu2)
//
// Originally: Ardunio Nano V3.0 ATMEGA328P Module CH340G - 2/13/2016 @ 4:10PM
// *************************************** \\

// *************************************** \\
// ** Version: See excel **
// ** Contains a known bug?: See excel **
// ** Beta Version?: See excel **
// ** Last Save: N/A **

// *************************************** \\
// ** All saves must:
// ** 1. Count as a new version (version + amount of changes)
// ** 2. Have a verification run on it, to verify it would work.
// ** 3. Be saved under a new file name.
// ** 4. Be documented in excel datasheet.
// *************************************** \\

// *************************************** \\
// [- Version Keynotes -]
// - Moved these notes to the excel sheet.
// [- End of Version Keynotes -]

// [- Version Breadboard Updates -]
// - Moved to excel sheet.
// [- End of Breadboard Updates -]
// *************************************** \\

//------------------------------------------------------------------------------
// Section [#2]: Initalization, Constants, Pin Numbers, Values

// Counting Values
unsigned int issuecnt = 0;
unsigned int warncnt = 0;

// Sensor #'s, Buzzer Pin #'s
const int Trigpin = 11;                    // Blue cord   // Trigger pin on the sensor goes to pin A7 on the Ardunio.
const int Echopin = 13;                    // Yellow cord // Echo pin on the sensor goes to pin D2 on the Arduino.
const int Beeppin = 12;                    // Piezospeaker anode pin goes to A3 on the Arduino.

// Delay Times
const int Time1 = 1000;                    // First delay time
const int Time2 = 750;                     // Second delay time
const int Time3 = 500;                     // Third delay time
const int Time4 = 250;                     // Third delay time
const int TimeEmc = 10;                    // Emergency time

// Distances (In inches)
const int Warn1 = 120;                        // The furthest warning @10ft
const int Warn2 = 60;                         // The secondary warning @5ft
const int Warn3 = 42;                         // Third warning @3.5ft
const int Warn4 = 24;                         // Forth warning @2ft
const int CrDetect = 12;                      // Crash detection system @1ft

// Error Variables
const int Mread = 300;                      // The distance at which it determines if the sensor misread the distance. Default is 300in.
const int Nread = 0;                        // The distance at which it determines if the sensor is disconnected. Default is, and should remain 0in.

// Piezospeaker Frequencies
const int Emc = 2500;                       // #1: Used for CrDetect.
const int Tone1 = 3000;                     // Used for Warn #1-3.
const int Tone2 = 3250;                     // Used for Warn #4
const int NTone = 2750;                     // Used for anything related to debugging.

//------------------------------------------------------------------------------
// Section [#3]: Beginning Code, More Initalizing

void setup() {
  Serial.begin(9600);                      // initialize serial communication @ 9600 Baud.
}

void loop() {
  // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  long duration, inches;

//------------------------------------------------------------------------------
// Section [#4A]: Measurements Sending, Input/Output, Rerun
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
  delayMicroseconds(1);              // Pause for 0.01 Second.
  pinMode(Echopin, LOW);             // Turn pin off.
  duration = pulseIn(Echopin, HIGH); // More calculations

//------------------------------------------------------------------------------
// Section [#5]: Coversion (Time to inches)

  inches = microsecondsToInches(duration);

//------------------------------------------------------------------------------
// Section [#6]: If-Statements [No read]

if (inches == Nread)         // Start IF-#5: If the sensor reads 0, then goto the beginning.
{
  tone(Beeppin, NTone);
  Serial.print("Sensor did not read: (");
  Serial.print(inches);
  Serial.print("in)");
  Serial.print("(#");
  Serial.print(issuecnt);
  Serial.println(")");
  delay(300);
  issuecnt = issuecnt + 1;
  goto issuecntrl;
}
else if (inches != Nread)
{
  noTone(Beeppin);           // If there isn't an issue, turn off the buzzer.
  issuecnt = 0;
}                            // End IF-#5

//**************************************\\
// Section [#6A]: If-Statements [Warning #1, 10ft]

if (((inches > Warn2) && (inches <= Warn1) && (warncnt <= 1)))            // Start IF-#1:
{
  delay(Time1);
  tone(Beeppin, Tone1);
  warncnt + 1;
}                            // End IF-#1
else {
  noTone(Beeppin);
}

//**************************************\\
// Section [#6B]: If-Statements [Warning #2, 5ft]

if ((inches > Warn3) && (inches <= Warn2))           // Start IF-#2
{
  tone(Beeppin, Tone1);
  delay(Time2);
}
else {
  noTone(Beeppin);
}                           // End IF-#2

//**************************************\\
// Section [#6C]: If-Statements [Warning #3, 3.5ft]

if ((inches > Warn4) && (inches <= Warn3))           // Start IF-#3
{
  tone(Beeppin, Tone1);
  delay(Time2);
}
else {
  noTone(Beeppin);
}                           // End IF-#3

//**************************************\\
// Section [#6C]: If-Statements [Warning #4, 2ft]

if (inches <= Warn4)                               // Start IF-#4
{
  tone(Beeppin, 1);
  delay(Time4);
}
else {
  noTone(Beeppin);
}                           // End IF-#4

//**************************************\\
// Section [#6C]: If-Statements [Advanced Crash Prevention, -1ft]

if (inches <= CrDetect)                               // IF-Statement - Crash Prevention
{
  tone(Beeppin, Emc);
  delay(TimeEmc);
}
else {
  noTone(Beeppin);
}                           // End IF-Statement Crash Prevention

//**************************************\\
// Section [#6D]: If-Statements [Error]

if (inches > Mread)         // Start IF-#4: If the distance is over 200 then the sensor probably misread, and goto the beginning.
{
  Serial.println("Whoops! Sensor misread data.");
  delay(300);
  goto reread;
}                           // End IF-Error

//------------------------------------------------------------------------------
// Section [#7]: End of loop
issuecntrl:

delay(300);
if (issuecnt == 5)
{
  Serial.println("Shutting down. Issue detected.");
  noTone(Beeppin);
  while(1) { }
}
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

// Section [End]: Program is over.
