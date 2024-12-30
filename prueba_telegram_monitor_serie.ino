#include <WiFi.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

// Configuración WiFi
const char* ssid = "Claro_ROJAS";        // Cambia por el nombre de tu red WiFi
const char* password = "1103857320"; // Cambia por la contraseña de tu red WiFi

// Token del bot de Telegram
const char* BOT_TOKEN = "8104194718:AAHOD5B8Ap3XKGa-6AgQ0kDOaxGpA81_vZA"; // Cambia por el token de tu bot

// Inicializar el cliente WiFi y el bot
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// Variable para rastrear mensajes
unsigned long lastTimeBotRan = 0;
const int BOT_INTERVAL = 1000; // Intervalo de 1 segundo para comprobar nuevos mensajes

void setup() {
  Serial.begin(9600);

  // Conectar a WiFi
  Serial.print("Conectando a WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado a WiFi");

  // Configurar cliente seguro
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Certificado de Telegram
}

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    // Mostrar el mensaje recibido en el monitor serial
    Serial.println("Mensaje recibido:");
    Serial.println("Chat ID: " + chat_id);
    Serial.println("Texto: " + text);
  }
}

void loop() {
  if (millis() - lastTimeBotRan > BOT_INTERVAL) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
