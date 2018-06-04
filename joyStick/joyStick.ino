// analog
int pin_x = 0;
int pin_y = 1;

// digital
int pin_switch = 8;

void setup() {
  Serial.begin(9600);
  
  pinMode(pin_switch, INPUT);
}

 void loop() {
  int x = analogRead(pin_x);   
  int y = analogRead(pin_y);   
  int b = digitalRead(pin_switch);   
//  
//  Serial.print("X:");
//  Serial.print(x);
//  Serial.print(",");
//  Serial.print("Y:");
//  Serial.print(y);
//  Serial.print(",");
//  Serial.print("B:");
//  Serial.print(b);
//  Serial.print("\n");

  if( x <= 256) 
  {
    Serial.print("l\0");
  }
  else if (x >= 768)
  {
    Serial.print("r\0");
  }

  else 
  {
    Serial.print("0\0");
  }
  }
 
