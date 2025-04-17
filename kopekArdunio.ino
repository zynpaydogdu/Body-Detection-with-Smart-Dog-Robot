#include <ESP8266WiFi.h>
#include <Servo.h>

// Wi-Fi bilgileri
const char* ssid = "TURKNET_ACDE0";
const char* password = "turknet3463";

// Servo tanımları
Servo sag_arka_ust;
Servo sag_arka_alt;
Servo sol_arka_ust;
Servo sol_arka_alt;
Servo sag_on_ust;
Servo sag_on_alt;
Servo sol_on_ust;
Servo sol_on_alt;

char veri;

void setup() {
  Serial.begin(115200);

  // Servo pinleri
  sag_arka_ust.attach(13);
  sag_arka_alt.attach(15);
  sol_arka_ust.attach(12);
  sol_arka_alt.attach(14);
  sag_on_ust.attach(0);
  sag_on_alt.attach(2);
  sol_on_ust.attach(5);
  sol_on_alt.attach(4);

  // WiFi: Önce STA (normal Wi-Fi) modunda dene
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("WiFi ağına bağlanılıyor");

  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi ağına bağlanıldı!");
    Serial.print("IP adresi: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nBağlantı başarısız. AP moduna geçiliyor...");

    // Access Point (kendi ağı)
    WiFi.mode(WIFI_AP);
    WiFi.softAP("RobotKontrol", "12345678");

    Serial.println("Kendi WiFi ağı kuruldu!");
    Serial.print("AP IP adresi: ");
    Serial.println(WiFi.softAPIP());  // Genelde 192.168.4.1
  }

  delay(2000);
}

void loop() {
  // Şimdilik komutları seri monitörden alıyoruz
  if (Serial.available() > 0) {
    veri = Serial.read();
    Serial.println(veri);

    if (veri == 'h') {
      idle();
      
    } else if (veri == 'c') {
      ileri();
      
    } else if (veri == 'j') {
      sinav_cek();
    }
  }
  
}

// Fonksiyonlar
void idle() {
  sol_arka_ust.write(90);
  sol_arka_alt.write(90);
  sag_arka_ust.write(90);
  sag_arka_alt.write(70);
  sag_on_ust.write(120);
  sag_on_alt.write(90);
  sol_on_ust.write(60);
  sol_on_alt.write(90);
}

void sinav_cek() {
  sol_arka_ust.write(90);
  sol_arka_alt.write(90);
  sag_arka_ust.write(90);
  sag_arka_alt.write(70);
  sag_on_ust.write(120);
  sag_on_alt.write(90);
  sol_on_ust.write(60);
  sol_on_alt.write(90);

  delay(500);
  sol_arka_alt.write(40);
  sag_arka_alt.write(110);
  sag_on_alt.write(140);
  sol_on_alt.write(40);
  delay(500);
}

void ileri() {
  sol_arka_ust.write(90);
  sag_arka_ust.write(40);
  sag_on_ust.write(120);
  sol_on_ust.write(110);
  delay(100);

  sol_arka_alt.write(90);
  sag_arka_alt.write(120);
  sag_on_alt.write(90);
  sol_on_alt.write(40);
  delay(100);

  sol_arka_ust.write(140);
  sag_arka_ust.write(90);
  sag_on_ust.write(70);
  sol_on_ust.write(60);
  delay(100);

  sol_arka_alt.write(40);
  sag_arka_alt.write(70);
  sag_on_alt.write(140);
  sol_on_alt.write(90);
  delay(100);
}