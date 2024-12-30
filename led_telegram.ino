#include <WiFi.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

// Configuración WiFi
const char* ssid = "Claro_ROJAS";        // Cambia por el nombre de tu red WiFi
const char* password = "1103857320"; // Cambia por la contraseña de tu red WiFi

// Token del bot de Telegram
const char* BOT_TOKEN = "8104194718:AAHOD5B8Ap3XKGa-6AgQ0kDOaxGpA81_vZA"; // Cambia por el token de tu bot

// Inicialización del cliente WiFi y el bot
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// Pin del LED
const int ledPin = 2;
// Variables de tiempo
unsigned long lastTimeBotRan = 0;
const int BOT_INTERVAL = 1000; // Intervalo de 1 segundo para verificar nuevos mensajes

void setup() {
  Serial.begin(9600); // Inicializa el monitor serial
  pinMode(ledPin, OUTPUT); // Configura el pin del LED como salida
  digitalWrite(ledPin, LOW); // Asegura que el LED esté apagado inicialmente

  // Conexión a WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conexión a WiFi establecida.");

  // Configuración del cliente seguro
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Certificado raíz de Telegram
  Serial.println("Cliente seguro configurado.");
}

void loop() {
  // Verificar si ha pasado el intervalo para consultar nuevos mensajes
  if (millis() - lastTimeBotRan > BOT_INTERVAL) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1); // Obtiene nuevos mensajes
    if (numNewMessages > 0) { // Si hay mensajes nuevos
      for (int i = 0; i < numNewMessages; i++) {
        String chat_id = bot.messages[i].chat_id; // ID del chat
        String text = bot.messages[i].text;       // Texto del mensaje

        // Mostrar mensaje en el monitor serial
        Serial.println("Mensaje recibido:");
        Serial.println("Chat ID: " + chat_id);
        Serial.println("Texto: " + text);

        // Control del LED según el mensaje recibido
        if (text.equalsIgnoreCase("ABRIR")) {
          digitalWrite(ledPin, HIGH); // Enciende el LED
          bot.sendMessage(chat_id, "Abriendo puerta", "Markdown");
          delay(3000);
          bot.sendMessage(chat_id, "Puerta cerrada", "Markdown");
          digitalWrite(ledPin, LOW); // Apaga el LED
          } 
        
         else {
          bot.sendMessage(chat_id, "Comando no reconocido", "Markdown");
        }
      }
    }
    lastTimeBotRan = millis(); // Actualiza el tiempo de la última ejecución
  }
}
