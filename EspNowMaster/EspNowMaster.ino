//Libs do espnow e wifi
#include <esp_now.h>
#include <WiFi.h>

//Canal usado para conexão
#define CHANNEL 1

//Mac Address dos slaves para os quais iremos enviar a leitura
//Se quiser enviar para todos os Slaves utilize apenas o endereço de broadcast {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}.
//Se quiser enviar para ESPs específicos coloque o Mac Address (obtido através da função WiFi.macAddress())
uint8_t macSlaves[][6] = {
  //Se for enviar para ESPs específicos, coloque cada endereço separado por vírgula
  // {0x24, 0x0A, 0xC4, 0x0E, 0x3F, 0xD1}, {0x24, 0x0A, 0xC4, 0x0E, 0x4E, 0xC3}
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
  //{0xb4, 0xe6, 0x2d, 0xac, 0xc0 ,0x59}
};

void setup() {
  Serial.begin(115200);


  //Colocamos o ESP em modo station
  WiFi.mode(WIFI_STA);

  //Mostramos no Monitor Serial o Mac Address deste ESP quando em modo station
  Serial.print("Mac Address in Station: "); 
  Serial.println(WiFi.macAddress());

  //Chama a função que inicializa o ESPNow
  InitESPNow();

  //Cálculo do tamanho do array com os mac address dos slaves
  //sizeof(macSlaves) retorna a quantidade de bytes que o array macSlaves aponta
  //Sabemos que cada mac address é um array de 6 posições e
  //cada posição possui sizeof(uint8_t) bytes, então
  //a quantidade de slaves é a divisão da quantidade de bytes
  //total do array pela quantidade de posições e o resultado
  //dessa divisão dividimos novamente por quantos bytes cada posição possui
  int slavesCount = sizeof(macSlaves)/6/sizeof(uint8_t);

  //Para cada slave
  for(int i=0; i<slavesCount; i++){
    //Criamos uma variável que irá guardar as informações do slave
    esp_now_peer_info_t slave;
    //Informamos o canal
    slave.channel = CHANNEL;
    //0 para não usar criptografia ou 1 para usar
    slave.encrypt = 0;
    //Copia o endereço do array para a estrutura
    memcpy(slave.peer_addr, macSlaves[i], sizeof(macSlaves[i]));
    //Adiciona o slave
    esp_now_add_peer(&slave);
  }

  pinMode(4, INPUT_PULLUP);
}

void InitESPNow() {
  //Se a inicialização foi bem sucedida
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
  }
  //Se houve erro na inicialização
  else {
    Serial.println("ESPNow Init Failed");
    ESP.restart();
  }
}

//Função que irá fazer a leitura dos pinos
//que estão no array gpios e enviar os valores
//lidos para os outros ESPs
uint8_t code = 50;


void loop() {
uint8_t broadcast[] = {0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF};
  if(!digitalRead(4)){
    esp_now_send(broadcast, &code, sizeof(code));
    delay(250);
  }
}
