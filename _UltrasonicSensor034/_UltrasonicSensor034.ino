//------------------------------------------------------------------------------
// Section [#1]: Information, Notes, Etc
// *************************************** \\
// Ultrasonic sensor
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

// Sensors, Hearing Aid
const int Trigpin = 11;                    // Blue cord   // Trigger pin on the sensor goes to pin A7 on the Ardunio.
const int Echopin = 13;                     // Yellow cord // Echo pin on the sensor goes to pin D2 on the Arduino.
const int Beeppin = 12;                    // Piezospeaker anode pin goes to A3 on the Arduino.

// LEDs, Visual Aid (Pin numbers)
const int Led4 = 2;                       // Con: Pin A0, White
const int Led2 = 3;                       // Con: Pin A1, Yellow
const int Led3 = A3;                        // Con: Pin D3, Red
const int Led1 = A5;                        // Con: Pin D4, Green

// Distances (In inches)
const int Warn = 24;                        // The distance at which the warn variable becomes active. In inches. Default is 24in.
const int Beep = 12;                        // The distance at which the buzzer becomes active. In inches. Default is 12in.
const int Mread = 200;                      // The distance at which it determines if the sensor misread the distance. Default is 200in.
const int Nread = 0;                        // The distance at which it determines if the sensor is disconnected. Default is, and should remain 0in.

// Piezospeaker Frequencies
const int Tone1 = 3950;                     // #1: Ultra-pitch. Very ear piercing. Use only if needed.
const int Tone2 = 3500;                     // #2: High-pitch.

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
  issuecnt = issuecnt + 1;
  goto issuecntrl;
}
else if (inches != Nread)
{
  noTone(Beeppin);
  digitalWrite(Led4, LOW);
}                            // End IF-#5

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

if ((inches > Beep) && (inches <= Warn))             // Start IF-#2: If closer than 24 inches (default) turn off the green LED, and turn on the yellow one.
{
  digitalWrite(Led2, HIGH);
  Serial.print(inches);
  Serial.print("in");
  Serial.println();
}
else if (inches > Beep)
{
  digitalWrite(Led2, LOW);
}                            // End IF-#2

//**************************************\\
// Section [#6C]: If-Statements [Red LED, Piezospeaker]

if (inches <= Beep)           // Start IF-#3: If closer than 12 inches (default) then sound the buzzer and show on the screen.
{
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
issuecntrl:

delay(300);
if (issuecnt == 4)
{
  Serial.println("Shutting down. Issue detected.");
  noTone(Beeppin);
  digitalWrite(Led4, LOW);
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
