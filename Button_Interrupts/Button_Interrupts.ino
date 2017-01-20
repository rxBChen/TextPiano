volatile int state = LOW; //state of whether to pause or not
int buttonPinInterrupt = 2;  //pin for the button

int button1 = 7;
int button2 = 8;

int ledOutputPin = 4; //pin to detect output

//prevent debounce
long debounceTime = 200; //debounce time should not exceed this value

void setup() 
{
  // put your setup code here, to run once:
  //set led to output
  pinMode(ledOutputPin, OUTPUT);
  pinMode(button1, OUTPUT);
  pinMode(button2, OUTPUT);
  //attaching interrupt to button
  attachInterrupt(digitalPinToInterrupt(buttonPinInterrupt), button_ISR, CHANGE);
  Serial.begin(9600);
}

void loop() 
{
  // put your main code here, to run repeatedly:

  //Pause if high, else low
  Serial.print("State: ");
  Serial.println(state);
  if(state == HIGH)
  {
    delay(10);
    digitalWrite(ledOutputPin, HIGH);
  
    Serial.print("Button1: ");
    Serial.println(digitalRead(button1));
    Serial.print("Button2: ");
    Serial.println(digitalRead(button2));
    delay(500);
  }
  else
  {
    digitalWrite(ledOutputPin, LOW);
  }
  
}
void button_ISR()
{
  if(state == HIGH)
  {
    state = LOW;
  }
  else
  {
    state = HIGH;
  }
}

