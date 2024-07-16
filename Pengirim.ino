#include <ESP8266WiFi.h>
#include <espnow.h>

#define pin_90 D2
#define pin_180 D1

// mac address tujuan
uint8_t mac_addr_tujuan[] = {0x5C, 0xCF, 0x7F, 0x8A, 0xCA, 0xF5};

// struktur data yang diterima
typedef struct struct_message {
  int data_servo;
} struct_message;
struct_message data_kirim;

// callback Jika data terkirim
void cb_kirim(uint8_t *mac_addr, uint8_t _status) {
  Serial.print("Status Pengiriman: ");
  if (_status == 0){
    Serial.println("Sukses");
  }
  else{
    Serial.println("Gagal");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(pin_90, INPUT_PULLUP);
  pinMode(pin_180, INPUT_PULLUP);

  
  // mengatur esp ke mode station
  WiFi.mode(WIFI_STA);

  // inisialisasi espnow
  if (esp_now_init() != 0) {
    Serial.println("Gagal Inisialisasi espnow");
    return;
  }
  
  // mendaftarkan fungsi callback
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(cb_kirim);
  
  // Menambahkan peer
  esp_now_add_peer(mac_addr_tujuan, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
  if(!digitalRead(pin_90)){
    data_kirim.data_servo = 90;
    esp_now_send(mac_addr_tujuan, (uint8_t *) &data_kirim, sizeof(data_kirim));
    Serial.println("Tekan 90");
    delay(200);
  }
  if(!digitalRead(pin_180)){
    data_kirim.data_servo = 180;
    esp_now_send(mac_addr_tujuan, (uint8_t *) &data_kirim, sizeof(data_kirim));
    Serial.println("Tekan 180");
    delay(200);
  }
}
