/*
 Basic ESP8266 MQTT example
 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off
 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "twevent";
const char* password = "GeekNight";
const char* mqtt_server = "10.17.73.117";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
const int redLedPin =  D0;
const int greenLedPin =  D3;
const int blueLedPin =  D4;
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  char signal = (char)payload[0];
  if ( signal == 'R') {
      Serial.print(" -- INSIDE R -- ");
//    digitalWrite(redLedPin, LOW);   // Turn the LED on (Note that LOW is the voltage level
//    digitalWrite(greenLedPin, HIGH);
//    digitalWrite(blueLedPin, HIGH);
     analogWrite(redLedPin, 0);
     analogWrite(greenLedPin, 1023);
     analogWrite(blueLedPin, 1023);     
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else if (signal == 'G') {
    Serial.print(" -- INSIDE G -- ");
//    digitalWrite(redLedPin, HIGH);   // Turn the LED on (Note that LOW is the voltage level
//    digitalWrite(greenLedPin, LOW);
//    digitalWrite(blueLedPin, HIGH);
     analogWrite(redLedPin, 1023);
     analogWrite(greenLedPin, 0);
     analogWrite(blueLedPin, 1023);
  } else if (signal == 'B'){
    Serial.print(" -- INSIDE B -- ");
//    digitalWrite(redLedPin, HIGH);   // Turn the LED on (Note that LOW is the voltage level
//    digitalWrite(greenLedPin, HIGH);
//    digitalWrite(blueLedPin, LOW);
     analogWrite(redLedPin, 1023);
     analogWrite(greenLedPin, 1023);
     analogWrite(blueLedPin, 0);
  }
  else {
    Serial.print(" -- OFF ALL -- ");
//    digitalWrite(redLedPin, HIGH);  // Turn the LED off by making the voltage HIGH
//    digitalWrite(greenLedPin, HIGH);
//    digitalWrite(blueLedPin, HIGH);
     analogWrite(redLedPin, 1023);
     analogWrite(greenLedPin, 1023);
     analogWrite(blueLedPin, 1023);
  }
Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
      client.subscribe("lightTest");
      client.subscribe("light");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(redLedPin, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(greenLedPin, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(blueLedPin, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "hello world xyz #%ld", value);
//    Serial.print("Publish message: ");
//    Serial.println(msg);
    //client.publish("outTopic", msg);
  }
}
