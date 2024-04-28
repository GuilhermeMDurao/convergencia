#include <WiFi.h>

#include "esp_wifi.h"

#define maxCh 13  // Máximo de canais (Brasil = 13)

int packetsReceived[maxCh] = { 0 };  // Array para contar a quantidade de pacotes recebidos por canal
int totalPackets = 0;                // Variável para armazenar o somatório de pacotes em todos os canais

void sniffer(void* buf, wifi_promiscuous_pkt_type_t type) {
  wifi_promiscuous_pkt_t* p = (wifi_promiscuous_pkt_t*)buf;
  int channel = p->rx_ctrl.channel;  // Obter o canal do pacote recebido
  packetsReceived[channel - 1]++;    // Incrementar a contagem de pacotes para o canal atual
  totalPackets++;                    // Incrementar o somatório total de pacotes
}

void setup() {
  Serial.begin(115200);
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&cfg);
  esp_wifi_set_storage(WIFI_STORAGE_RAM);
  esp_wifi_set_mode(WIFI_MODE_NULL);
  esp_wifi_start();
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_promiscuous_filter(NULL);
  esp_wifi_set_promiscuous_rx_cb(&sniffer);
}

void loop() {
  // Escaneamento de canais
  for (int curChannel = 1; curChannel <= maxCh; curChannel++) {
    packetsReceived[curChannel - 1] = 0;  // Zerar a contagem de pacotes para o próximo escaneamento
    esp_wifi_set_channel(curChannel, WIFI_SECOND_CHAN_NONE);
    delay(1000 / maxCh);
  }

  // Mostrar resultados
  //Serial.println("Pacotes recebidos por canal:");
  //for (int i = 0; i < maxCh; i++) {
    //Serial.print("Canal ");
    //Serial.print(i + 1);
    //Serial.print(": ");
    //Serial.println(packetsReceived[i]);
    //packetsReceived[i] = 0;  // Zerar a contagem de pacotes para o próximo escaneamento
  //}
  //Serial.print("Total de pacotes: ");
  Serial.println(totalPackets);
  totalPackets = 0;  // Zerar o somatório total de pacotes para o próximo escaneamento
}
