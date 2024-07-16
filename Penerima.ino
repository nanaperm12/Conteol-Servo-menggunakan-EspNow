#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Servo.h>

#define pin_servo D2
Servo servo_ku; 

// struktur data yang diterima
typedef struct struct_message {
  int data_servo;
} struct_message;
struct_message data_terima;
int data_servo_sebelum;

// callback jika menerima data
void cb_terima(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&data_terima, incomingData, sizeof(data_terima));
  Serial.print("Data Terima: ");
  Serial.println(data_terima.data_servo);
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  servo_ku.attach(pin_servo);

  // mengatur esp ke mode station
  WiFi.mode(WIFI_STA);

  // inisialisasi espnow
  if (esp_now_init() != 0) {
    Serial.println("Gagal Inisialisasi espnow");
    return;
  }
  
  // mendaftarkan fungsi callback
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(cb_terima);
}

void loop() {
  if(data_terima.data_servo != data_servo_sebelum){
    servo_ku.write(data_terima.data_servo);
    delay(15);  
    data_servo_sebelum = data_terima.data_servo;
  }
}
