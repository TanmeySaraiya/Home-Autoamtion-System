int IR1_out = HIGH;
int IR2_out = HIGH;
int count = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(8, INPUT); // ir sensor input value for incoming Count
  pinMode(13, INPUT); // ir sensor input value for Outgoing Count
  pinMode(10,OUTPUT);
  Serial.begin(9600); 
}

void loop() {
  // put your main code here, to run repeatedly:
  int   IR1_out = digitalRead(8);
  int   IR2_out = digitalRead(13);
  int   LED = digitalRead(10);
    
  if(IR1_out == LOW) {
    IN();
  }

  if(IR2_out == LOW) {
    OUT();
  }
  delay(200);
}

void IN()
{
 do{
    IR2_out = digitalRead(13);
    delay(200);
   }while(IR2_out);
    count ++;
    Serial.print("INSIDE-COUNT:");
    Serial.println(count);
    delay(200);
    if (count > 0){
      digitalWrite(10, HIGH);
    }
    else {
      digitalWrite(10, LOW);
    }
}

void OUT()
{
  do{
    IR1_out = digitalRead(8);
    delay(200);
    }while(IR1_out);
    count--;
    Serial.print("OUTSIDE-COUNT:");
    Serial.println(count);
    delay(200);
    if (count > 0){
      digitalWrite(10, HIGH);
    }
    else {
      digitalWrite(10, LOW);
    }
}
