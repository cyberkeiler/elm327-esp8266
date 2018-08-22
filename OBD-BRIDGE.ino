#include <ESP8266WiFi.h>

const char* ssid     = "WiFi_OBDII";
const char* password = "your-password";

//IP Adress of your ELM327 Dongle
IPAddress server(192, 168, 0, 10);

boolean last_in = false;

WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(10);

  // Connecting to ELM327 WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid);
  // WiFi.begin(ssid, password);  //Use this line if your ELM327 has a password protected WiFi

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connected to Wifi");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if (client.connect(server, 35000)) {
    Serial.println("connected");
    Serial.println("<ATZ");
    // Make a Request
    client.println("ATZ");
  }
  else {
    Serial.println("connection failed");
    ESP.reset();
  }
}

void loop() {
  if (!client.connected()) {
    Serial.println("CLIENT DISCONNECTED - RESET!");
    client.stop();
    ESP.reset();
  }
  // if there are incoming bytes available
  // from the server, read them and print them:
  if (client.available()) {
    if (!last_in) {
      Serial.println("");
      Serial.print(">");
      last_in = true;
    }
    char c = client.read();
    /*Serial.print(" 0x");
    Serial.print(c, HEX);*/
    Serial.print(c);
  }

  // as long as there are bytes in the serial queue,
  // read them and send them out the socket if it's open:
  while (Serial.available() > 0) {
    if (last_in) {
      Serial.println("");
      Serial.print("<< ");
      last_in = false;
    }
    char inChar = Serial.read();
    if (client.connected()) {
      client.print(inChar);
    }
  }
}
