#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

const char* ssid = "IOT_E";
const char* password = "121231212101";
const char* ESP32Client = "ESP32Client20";
const char* topic = "SDM/Node_20";
const int sensorDelayTime = 1000;

IPAddress local_IP(192, 168, 6, 170);
IPAddress gateway(192, 168, 6, 1);
IPAddress subnet(255, 255, 255, 0);

const char* mqtt_server = "192.168.6.99";
const char* mqtt_user = "";
const char* mqtt_pass = "";


String senserState;
String stringzero;
String stringone;
String currentState = "99";
const int senserPin = 35;
bool isWifiDisconnect = false;
const char* zero = "0";
const char* one = "1";


WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;

void setup()
{
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(senserPin, INPUT);
};

void setup_wifi()
{
  delay(10);
  Serial.println();


  Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet)) {
   Serial.println("STA Failed to configure");
  }


  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());

};




// void callback(char* topic, byte* message, unsigned int length)
// {
//   Serial.print("Message arrived on topic: ");
//   Serial.print(topic);
//   Serial.print(". Message: ");
//   String messageRev;

//   for (int i = 0; i < length; i++)
//   {
//     Serial.print((char)message[i]);
//     messageRev += (char)message[i];
//   }
//   Serial.println();
// };

void reconnect()
{
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(ESP32Client, mqtt_user, mqtt_pass))
    {
      Serial.println("connected");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
};

void loop()
{
  senserState = String(digitalRead(senserPin));
  int intsensorState = digitalRead(senserPin);
  int str_len = senserState.length() + 1;
  char charWrite[str_len];
  senserState.toCharArray(charWrite, str_len);

  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  if (currentState != senserState)
  {

    if (intsensorState == 1)
    {
      Serial.print("Sensor state:");
      Serial.println(one);
      client.publish(topic, one );
    }
    else if (intsensorState == 0)
    {
      Serial.print("Sensor state:");
      Serial.println(zero);
      client.publish(topic, zero );
      delay(sensorDelayTime);
    }

    currentState = senserState;
  }
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
};