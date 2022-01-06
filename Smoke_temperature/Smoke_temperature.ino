int ledPin = 10;      // LED+
//float j=0;          
int temp_sensor = A0; // LM35 Output
int smokeA0 = A1;
int IR1_out = HIGH;
int IR2_out = HIGH;
int count = 0;
String apiKey = "";  //Change this key to your "Write API key"


void setup() {
  // put your setup code here, to run once: 
  pinMode(ledPin, OUTPUT); // Set ledPin as Output
  pinMode(smokeA0, INPUT);
  Serial.begin(9600);      // PC Arduino Serial Monitor
  Serial1.begin(115200);   // Arduino to ESP01 Communication
  connectWiFi();           // To connect to Wifi
 } 

void loop() { 
  // put your main code here, to run repeatedly:
  float analog_val = analogRead(temp_sensor);     // Read Analog Temperature
  float digital_vout = (analog_val*5000.0)/1023;  // Convert Analog Tempt. to Digital Value (10 bit ADC) // mV
  float tempc = digital_vout/10;                  // Storing value in Degree Celsius
  float smokec = analogRead(smokeA0);
  //j=tempc;

  //digitalWrite(ledPin, HIGH); // LED ON
  //delay(500);                 // wait for 500 mSec
  //digitalWrite(ledPin, LOW);  // LED OFF

  Serial1.println("AT+CIPMUX=0\r\n");      // To Set MUX = 0
  delay(2000);                             // Wait for 2 sec

  // TCP connection 
  String cmd = "AT+CIPSTART=\"TCP\",\"";   // TCP connection with https://thingspeak.com server
  cmd += "184.106.153.149";                // IP addr of api.thingspeak.com
  cmd += "\",80\r\n\r\n";                  // Port No. = 80

  Serial1.println(cmd);                    // Display above Command on PC
  Serial.println(cmd);                     // Send above command to Rx1, Tx1

  delay(20000);                            // Wait for 20 Sec

  if(Serial1.find("ERROR"))                // If returns error in TCP connection
  { 
    Serial.println("AT+CIPSTART error");   // Display error msg to PC
    //return; 
  }

  // prepare GET string 
  String getStr = "GET /update?api_key=";   
  getStr += apiKey;
  getStr +="&field1=";
  getStr += tempc; 
  //getStr += "\r\n\r\n"; 
  getStr +="&field2=";
  getStr += smokec; 
  getStr += "\r\n\r\n";

  Serial.println(getStr);                 // Display GET String on PC

  cmd = "AT+CIPSEND=";                    // send data length 
  cmd += String(getStr.length());
  cmd+="\r\n";

  Serial.println(cmd);                   // Display Data length on PC
  Serial1.println(cmd);                  // Send Data length command to Tx1, Rx1
  
  delay(20000);                          // wait for 20sec

  if(Serial1.find(">"))                    // If prompt opens //verify connection with cloud
  {
    Serial.println("connected to Cloud");  // Display confirmation msg to PC
    Serial1.print(getStr);                 // Send GET String to Rx1, Tx1
  }
  else
  { 
    Serial1.println("AT+CIPCLOSE\r\n");    // Send Close Connection command to Rx1, Tx1
    Serial.println("AT+CIPCLOSE");         // Display Connection closed command on PC
  } 
  
  // thingspeak free version needs 16 sec delay between updates 
  delay(16000);                            // wait for 16sec

 }

boolean connectWiFi() {               // Connect to Wifi Function
  Serial1.println("AT+CWMODE=1\r\n"); // Setting Mode = 1 
  delay(100);                         // wait for 100 mSec

  String cmd = "AT+CWJAP=\"";         // Connect to WiFi
  cmd += "";                   // ssid_name
  cmd += "\",\"";
  cmd += "";                // password
  cmd += "\"\r\n";              
   
  Serial.println(cmd);                // Display Connect Wifi Command on PC
  Serial1.println(cmd);               // send Connect WiFi command to Rx1, Tx1 
  
  delay(10000);                       // wait for 10 sec

  Serial1.println("AT+CWJAP?");       // Verify Connected WiFi

  if(Serial1.find("+CWJAP"))        
  {
    Serial.println("OK, Connected to WiFi.");         // Display Confirmation msg on PC
    return true;
  }
  else
  {
    Serial.println("Can not connect to the WiFi.");   // Display Error msg on PC
    return false;
  }
}
