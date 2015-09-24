#include <Ethernet.h>
#include <PubSubClient.h>
#include <Servo.h>
#include <String.h>

EthernetClient ethClient;
PubSubClient client(ethClient);

// XXX Replace the string with a unique name
#define UNIQUE_NAME      "water-supply"
#define PUBLISHED_TOPIC  "ku-intel/" UNIQUE_NAME

// XXX Replace the following topic with your own interest
#define SUBSCRIBED_TOPIC "ku-intel/water-supply"


// XXX Replace the numbers below with your MAC address
byte mac[] = {  0x98,0x4F,0xEE,0x01,0x23,0x73 };

// XXX Replace the numbers below with the IP address
// of the MQTT broker
 IPAddress server(158,108,180,11);
 
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
float temp1, temp2, temp3;
String msg = "";
int humidity;
 
void setup()
{
  servo1.attach(3);
  servo2.attach(10);
  servo3.attach(6);
  servo4.attach(11);
  Serial.begin(9600);

  // Bring up the Ethernet interface
  Serial.println("Bringing up the Ethernet interface.");
  system("ifup eth0");
 //Serial.println(system("ifconfig eth0 > /dev/ttyGS0"));
  Ethernet.begin(mac);

  // Prepare initial settings for the client
  client.setServer(server,1883);
  client.setCallback(callback);
  // Allow the hardware to sort itself out
  delay(5000);

}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  Serial.println("message in loop function: "+msg);
  
  humidity = analogRead(A0);
  temp1= analogRead(A1);
  temp2= analogRead(A2);
  temp3= analogRead(A3);
  float C1 = (25*temp1-2050)/100.0;
  float C2 = (25*temp2-2050)/100.0;
  float C3 = (25*temp3-2050)/100.0;
  Serial.print("Degree of field 1 = ");
  Serial.print(C1);
  Serial.print(" C ");
  Serial.print(",   Degree of field 2 = ");
  Serial.print(C2);
  Serial.print(" C ");
  Serial.print(",   Degree of field 2 = ");
  Serial.print(C3);
  Serial.print(" C ");
  Serial.println();
  Serial.print("Humidity = ");
  Serial.println(humidity);
  

  // This check the message from mobile
  if(msg.equals("") || msg.equals("auto")){ // for automatic system
    
   /* if(humidity>350){
      Serial.println("The water supply 2 is opening now.");
      servo2.write(60);
      delay(500);
      servo2.write(0);
      delay(500);
      servo2.write(90);
      delay(500);
      servo2.write(120);
      delay(500);
    }
    else{
      Serial.println("The water supply 2 is closed.");
      servo2.write(0);
    }
   // delay(5000);
    if(C1>=20){
      Serial.println("The water supply 1 is opening now.");
      servo1.write(90);
      delay(500);
      servo1.write(120);
      delay(500);
    }else{
      Serial.println("The water supply 1 is closed.");
      servo1.write(0);
     }

    // delay(5000);
    if(C2>=20){
      Serial.println("The water supply 3 is opening now.");
      servo3.write(90);
      delay(500);
      servo3.write(120);
      delay(500);
    }else{
      Serial.println("The water supply 3 is closed.");
      servo3.write(0);
     }
    // delay(5000);*/
    if(C3>=25){
      Serial.println("The water supply 4 is opening now.");
      servo4.write(90);
      delay(500);
      servo4.write(120);
      delay(500);
    }else{
      Serial.println("The water supply 4 is closed.");
      servo4.write(0);
     }
  }else{ //for manaul system
  }
  
  client.loop();
  
}

void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = 0;
  msg = (char*)payload;
  
  Serial.println("message in callback function: "+msg);
  
  if(msg.equals("os1")){
      servo1.write(90);
      delay(500);
      servo1.write(120);
      delay(500);
    Serial.println("The water supply 1 is opening now.");
  }
  else if(msg.equals("os2")){
      servo2.write(90);
      delay(500);
      servo2.write(120);
      delay(500);
    Serial.println("The water supply 2 is opening now.");
  }
  else if(msg.equals("os3")){
      servo3.write(90);
      delay(500);
      servo3.write(120);
      delay(500);
    Serial.println("The water supply 3 is opening now.");
  }
  else if(msg.equals("os4")){
      servo4.write(90);
      delay(500);
      servo4.write(120);
      delay(500);
    Serial.println("The water supply 4 is opening now.");
  }
  else if(msg.equals("open")){
    servo1.write(90);
    servo2.write(90);
    servo3.write(90);
    servo4.write(90);
    delay(500);
    servo1.write(120);
    servo2.write(120);
    servo3.write(120);
    servo4.write(120);
    delay(500);
    Serial.println("All the water supply is opening now.");
  }
  else if(msg.equals("close")){
    servo1.write(0);
    servo2.write(0);
    servo3.write(0);
    servo4.write(0);
    Serial.println("All the water supply is closed.");
  }
  else if(msg.equals("cs1")){
    servo1.write(0);
    Serial.println("All the water supply 1 is closed.");
  }
  else if(msg.equals("cs2")){
    servo2.write(0);
    Serial.println("All the water supply 2 is closed.");
  }
  else if(msg.equals("cs3")){
    servo3.write(0);
    Serial.println("All the water supply 3 is closed.");
  }
  else if(msg.equals("cs4")){
    servo4.write(0);
    Serial.println("All the water supply 4 is closed.");
  }
  else{
    Serial.println("Do nothing"); 
  }
}

void reconnect() {
  // Loop until connected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(UNIQUE_NAME)) {
      Serial.println("connected");
      client.subscribe(SUBSCRIBED_TOPIC);
      Serial.println("Subscribed to topic: " SUBSCRIBED_TOPIC);
    }
    else {
      Serial.print("failed...");
      delay(5000);
    }
  }
}
