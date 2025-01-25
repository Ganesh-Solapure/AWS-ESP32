
#include <pgmspace.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "WiFi.h"
 
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub" // MQTT topic to subscribe to for commands
 
#define THINGNAME "***********" // Unique identifier for your device, change this
 
const char WIFI_SSID[] = "***********"; // Your WiFi SSID
const char WIFI_PASSWORD[] = "***********"; // Your WiFi password
const char AWS_IOT_ENDPOINT[] = "***********"; // Your AWS IoT endpoint, change this
 
// Amazon Root CA 1, necessary for secure communication
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
// Paste certificate here
-----END CERTIFICATE-----
)EOF";
 
// Device Certificate, replace with your device's certificate
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
// Paste certificate here
-----END CERTIFICATE-----
)KEY";
 
// Device Private Key, replace with your device's private key
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
// Paste private key here
-----END RSA PRIVATE KEY-----
)KEY";
 
#define lamp1 19 // GPIO pin for lamp 1
#define lamp2 21 // GPIO pin for lamp 2
#define lamp3 22 // GPIO pin for lamp 3
#define lamp4 23 // GPIO pin for lamp 4
 
WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);
 
// Function to handle incoming MQTT messages
void messageHandler(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Incoming message on topic [");
  Serial.print(topic);
  Serial.print("]: ");
 
  // Construct message from payload
  String received_msg = "";
  for (int i = 0; i < length; i++) 
  {
    received_msg += (char)payload[i];
    Serial.print((char)payload[i]);
  }
  Serial.println();
 
  // Control the lamps based on the received message
  // Check for commands and actuate corresponding lamp
  if (received_msg == "ON1") 
  {
    digitalWrite(lamp1, HIGH); // Turn on lamp 1
    Serial.println("Lamp1 turned on");
  }
  else if (received_msg == "OFF1") 
  {
    digitalWrite(lamp1, LOW); // Turn off lamp 1
    Serial.println("Lamp1 turned off");
  }
  
  // Repeat for other lamps...
  else if (received_msg == "ON2") 
  {
    digitalWrite(lamp2, HIGH);
    Serial.println("Lamp2 turned on");
  }
  else if (received_msg == "OFF2") 
  {
    digitalWrite(lamp2, LOW);
    Serial.println("Lamp2 turned off");
  }
  
  else if (received_msg == "ON3") 
  {
    digitalWrite(lamp3, HIGH);
    Serial.println("Lamp3 turned on");
  }
  else if (received_msg == "OFF3") 
  {
    digitalWrite(lamp3, LOW);
    Serial.println("Lamp3 turned off");
  }
  
  else if (received_msg == "ON4") 
  {
    digitalWrite(lamp4, HIGH);
    Serial.println("Lamp4 turned on");
  }
  else if (received_msg == "OFF4") 
  {
    digitalWrite(lamp4, LOW);
    Serial.println("Lamp4 turned off");
  }
}
 
 
void setup()
{
  Serial.begin(115200); // Start serial communication
 
  // Initialize GPIO pins for lamps as outputs and set them off
  pinMode(lamp1, OUTPUT);
  pinMode(lamp2, OUTPUT);
  pinMode(lamp3, OUTPUT);
  pinMode(lamp4, OUTPUT);
 
  digitalWrite(lamp1, LOW);
  digitalWrite(lamp2, LOW);
  digitalWrite(lamp3, LOW);
  digitalWrite(lamp4, LOW);
 
  WiFi.mode(WIFI_STA); // Set WiFi to station mode
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Connect to WiFi network
 
  Serial.println("Connecting to Wi-Fi");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
 
  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);
 
  // Connect to the MQTT broker on the AWS endpoint
  client.setServer(AWS_IOT_ENDPOINT, 8883);
 
  // Set the function to handle messages
  client.setCallback(messageHandler);
 
  Serial.println("Connecting to AWS IOT");
 
  // Attempt to connect to AWS IoT
  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(100);
  }
 
  // Check if connection was successful
  if (!client.connected())
  {
    Serial.println("AWS IoT Timeout!");
    return;
  }
 
  // Subscribe to the MQTT topic for receiving commands
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
 
  Serial.println("AWS IoT Connected!");
}
 
void loop()
{
  // Keep the MQTT connection alive
  client.loop();
  delay(1000);
}


//watch?v=59xppRDOEZE//https://how2electronics.com/home-automation-using-amazon-aws-iot-core-esp32///
