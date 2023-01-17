#include <Servo.h>
#include <Arduino.h>
#include <U8x8lib.h>


//U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display
int twistPin = 2; //Holds our potentiometer in Analog 2
int twist = 0; //holds the value of our potentiometer reading
unsigned long timer = 0;
int buttonPin = 3;//holds location of button in D3
int buttonState = 0;//hold state of button, either 0 or 1
unsigned long selectedTime = 0; //holds final time selected
unsigned long endTime = 0; //holds time when timer is up
Servo motor;//a servo named fan
int motorPin = 5;//holds location of servo
unsigned long startTime = 0;//holds time since servo turned on (millis)
unsigned long MIN = 1;//minimum time that can be selected
unsigned long MAX = 60;//maximum time that can be selected
int holder = 0; //if 0, we need to set time, if 1 we count down
int fanPin = 2; //holds location of fan
void setup()
{
  u8x8.setBusClock(100000);
  Serial.begin(9600); //starts our serial monitor with 9600 refresh rate
  pinMode(twistPin, INPUT); //set potentiometer as an input
  pinMode(fanPin, OUTPUT);
  u8x8.begin();
  u8x8.setFlipMode(1);
  motor.attach(motorPin);
  MIN = MIN * 1000;
  MAX = MAX * 1000;
}

void loop()
{
  twist = analogRead(twistPin); //read the potentiometer and save value to twist
  timer = map(twist, 0, 1023, MIN, MAX);//convert potent pin to time
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, 0);
  u8x8.print("          ");
  u8x8.setCursor(0, 0);
  u8x8.print("Timer: "); //print label
  u8x8.print(timer);//shows time being selected
  Serial.println((timer / 1000));
  Serial.print("Set Timer To: ");
  buttonState = digitalRead(buttonPin);//save value of buttonPin to buttonState

  if (buttonState == 1 && holder == 0) //if button is pressed...
  {
    holder = 1;
    Serial.println((timer / 1000)); //shows timer
    selectedTime = timer;//save the time selected
    startTime = millis();//get current time
    endTime = startTime + selectedTime; //compare time right now vs time when we started counting in terms of set duration

    while (endTime > millis())
    {
      u8x8.print(((endTime - millis()) / 1000)); //shows timer going down
      Serial.println(((endTime - millis()) / 1000)); //shows timer going down
      Serial.print("Time Left: ");
      delay(900);//delay 1000 milliseconds
    }
  }

  if (endTime <= millis() && holder == 1) //when the timer is up...
  {
    holder = 0;
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.setCursor(2, 1);
    u8x8.print("Timer is Up");//shows that timer is up
    Serial.println("         ");
    Serial.println("Timer is Up!"); //shows that timer is up on serialmonitor
    selectedTime = 0;
    endTime = 0;
    digitalWrite(fanPin, HIGH); //turns fan on
    delay(10000);
    digitalWrite(fanPin, LOW); //turns fan off
    delay(4000);
    Serial.println("Reseting...");
  }
}
