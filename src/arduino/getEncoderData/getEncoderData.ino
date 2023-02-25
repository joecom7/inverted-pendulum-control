#define PPR 1000

int encoderPin1 = 2;
int encoderPin2 = 3;
int ledPin1     = 5; // PWM
 
volatile int lastEncoded = 0;
volatile long encoderValue = 0;
 
long lastencoderValue = 0;
 
int lastMSB = 0;
int lastLSB = 0;
 
void setup() {
  Serial.begin (9600);
 
  pinMode(encoderPin1, INPUT);
  pinMode(encoderPin2, INPUT);
  pinMode(ledPin1, OUTPUT);
 
  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on
  analogWrite (ledPin1,LOW);
 
  attachInterrupt(0, updateEncoder, CHANGE);
  attachInterrupt(1, updateEncoder, CHANGE);
 
}
 
void loop(){
  Serial.println(readAngle());
}

double readAngle(){
  return  (encoderValue*2*PI)/PPR;
}
 
void updateEncoder(){
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit
 
  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value
 
  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;
 
  lastEncoded = encoded; //store this value for next time
}