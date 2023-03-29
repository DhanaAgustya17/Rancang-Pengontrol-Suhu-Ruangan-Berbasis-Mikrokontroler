#include <WiFi.h>
#include <HTTPClient.h>
#define pinSensor 14
#define ledMerah 27
#define ledKuning 5
#define ledPutih 25
int sensorSuhu;

const char *ssid = "dapss";  //Nama Wifi
const char *password = "Dhana123321"; // pass wifi
//IP Address Server yang terpasang XAMPP
const char *host = "192.168.43.117";
 
void setup() {
  Serial.begin(9600);

 pinMode(ledMerah, OUTPUT);
  pinMode(ledPutih, OUTPUT);
   pinMode(ledHijau, OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
 
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  //Jika koneksi berhasil, maka akan muncul address di serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
 
int value = 0;
 
void loop() {

  // Proses Pengiriman
  delay(5000);
  ++value;
 
  // Membaca Sensor Analog
  sensorSuhu = analogRead(pinSensor);
  
   if(sensorSuhu >=0 && sensorSuhu <= 20){
     digitalWrite(ledMerah, LOW);
     digitalWrite(ledPutih, HIGH);
  }else if(sensorSuhu>=21 && sensorSuhu<=34){
     digitalWrite(ledPutih, HIGH);
      }else if(sensorSuhu>34){
        digitalWrite(ledMerah,HIGH);
      }
  Serial.print("connecting to ");
  Serial.println(host);
 
// Mengirimkan ke alamat host dengan port 80
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
 
// Isi Konten yang dikirim adalah alamat ip si esp
  String url = "/web-dhana/write-data.php?data=";
  url += sensorSuhu;
 
  Serial.print("Requesting URL: ");
  Serial.println(url);

// Mengirimkan Request ke Server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 1000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
 
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
 
  Serial.println();
  Serial.println("closing connection");
 
}
