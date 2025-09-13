//Autores: Antônio Jacinto de Andrade Neto (RM: 561777), Felipe Bicaletto (RM: 563524), João Vitor dos Santos Pereira (RM: 551695) e Thayná Pereira Simões (RM: 566456) 
//Resumo: Programa para criar um placar eletrônico com ESP32, 4 botões físicos, MQTT e LCD I2C.

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---------------- Configurações editáveis ----------------
const char* SSID = "Wokwi-GUEST";  // Nome da rede Wifi
const char* PASSWORD = "";  // Senha da rede Wifi
const char* BROKER_MQTT = "20.46.254.134";  // IP do broker
const int BROKER_PORT = 1883;  // Porta do Broker
const char* TOPICO_PUBLISH = "/TEF/placar001/attrs";  // Tópico de envio do placar
const char* ID_MQTT = "fiware_001";  // ID do MQTT

// ---------------- Botões ----------------
#define BTN_A_PLUS 25  // Pino do botão que adiciona score no time A
#define BTN_A_MINUS 26  // Pino do botão que remove score no time A
#define BTN_B_PLUS 27  // Pino do botão que adiciona score no time B
#define BTN_B_MINUS 14  //Pino d botão que remove score no time B

// ---------------- Objetos ----------------
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Objeto LCD
WiFiClient espClient;  // Objeto Wifi
PubSubClient MQTT(espClient);  // Objeto MQTT

// ---------------- Placar ----------------
int scoreA = 0;  // Score do time A
int scoreB = 0;  //Score do time B

// ---------------- Funções ----------------

// Inicia monitor serial
void initSerial() {
  Serial.begin(115200);
}

// Inicia conexão wifi
void initWiFi() {
  Serial.println("------ Conexao WiFi ------");
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// Inicia conexão com MQTT Broker
void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
}

// Reconecta ao MQTT
void reconnectMQTT() {
  while (!MQTT.connected()) {
    Serial.print("Tentando conectar ao Broker MQTT...");
    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado!");
    } else {
      Serial.print("Falhou, rc=");
      Serial.print(MQTT.state());
      Serial.println(" tentando novamente em 2s");
      delay(2000);
    }
  }
}

// Verifica conexão wifi e conexão com MQTT Broker
void VerificaConexoesWiFIEMQTT() {
  if (!MQTT.connected()) {
    reconnectMQTT();
  }
  if (WiFi.status() != WL_CONNECTED) {
    initWiFi();
  }
}

// Mostra o placar no display LCD
void mostrarPlacar() {
  lcd.clear();
  lcd.setCursor(7, 0);
  lcd.print("VS");
  lcd.setCursor(0, 0);
  lcd.print("Time A");
  lcd.setCursor(2, 1);
  lcd.print(scoreA);
  lcd.setCursor(10, 0);
  lcd.print("Time B ");
  lcd.setCursor(13, 1);
  lcd.print(scoreB);
}

// Publica gols do time A e gols do time B no tópico definido
void publicarPlacar() {
  char msg[50];
  sprintf(msg, "scoreA|%d|scoreB|%d", scoreA, scoreB);
  MQTT.publish(TOPICO_PUBLISH, msg);
  Serial.print("Placar publicado: ");
  Serial.println(msg);
}

// Função que é executada no início
void setup() {
  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();

  initSerial();
  initWiFi();
  initMQTT();

  pinMode(BTN_A_PLUS, INPUT_PULLUP);
  pinMode(BTN_A_MINUS, INPUT_PULLUP);
  pinMode(BTN_B_PLUS, INPUT_PULLUP);
  pinMode(BTN_B_MINUS, INPUT_PULLUP);

  mostrarPlacar();
  publicarPlacar();
}

// Função que fica rodando durante a aplicação
void loop() {
  VerificaConexoesWiFIEMQTT();
  MQTT.loop();

  // Botão para adicionar score do time A (marcou gol)
  if (digitalRead(BTN_A_PLUS) == LOW) {
    scoreA++;
    mostrarPlacar();
    publicarPlacar();
    delay(500);
  }

  // Botão para remover score do time A (gol anulado)
  if (digitalRead(BTN_A_MINUS) == LOW) {
    if (scoreA > 0) {
      scoreA--;
      mostrarPlacar();
      publicarPlacar();
    };
    delay(500);
  }

  // Botão para adicionar score do time B (marcou gol)
  if (digitalRead(BTN_B_PLUS) == LOW) {
    scoreB++;
    mostrarPlacar();
    publicarPlacar();
    delay(500);
  }

  // Botão para remover score do time B (gol anulado)
  if (digitalRead(BTN_B_MINUS) == LOW) {
    if (scoreB > 0) {
      scoreB--;
      mostrarPlacar();
      publicarPlacar();
    };
    delay(500);
  } 
}