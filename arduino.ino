#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <Servo.h>

// Configurações de rede
#define WIFI_SSID "********" //Colocar Login do Wifi aqui
#define WIFI_PASSWORD "********" //Colocar senha do Wifi aqui

// Configurações Firebase
#define FIREBASE_HOST "substitua-pelo-seu-host" // Substitua pelo host do seu projeto Firebase
#define FIREBASE_AUTH "substitua-pelo-token" // Nunca compartilhe esta chave em repositórios públicos
#define SERVO_PIN D4

FirebaseData fbData;
FirebaseConfig fbConfig;
Servo servo;

void setup() {
  Serial.begin(115200);
  Serial.println("\nIniciando...");
  
  // Conexão WiFi com feedback claro
  Serial.print("Conectando a ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Configuração Firebase
  fbConfig.host = FIREBASE_HOST;
  fbConfig.signer.tokens.legacy_token = FIREBASE_AUTH;
  
  Serial.print("Conectando ao Firebase...");
  Firebase.begin(&fbConfig, nullptr);
  Firebase.reconnectWiFi(true);
  Serial.println(" OK");

  // Configuração do servo
  servo.attach(SERVO_PIN);
  servo.write(0);
  Serial.println("Servo inicializado");
}

void loop() {
  if (Firebase.getInt(fbData, "/controles/servo/angulo")) { 
    int angulo = fbData.intData();
    servo.write(angulo);
    Serial.print("Angulo atualizado: ");
    Serial.println(angulo);
  } else {
    Serial.println("Erro ao ler Firebase: " + fbData.errorReason());
  }
  delay(500);
}
