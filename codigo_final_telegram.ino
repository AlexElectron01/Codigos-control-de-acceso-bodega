#include <WiFi.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

// Configuración WiFi
const char* ssid = "Yoha A14 BE20";
const char* password = "Leonardo2018";

// Token del bot de Telegram
const char* BOT_TOKEN = "8104194718:AAHOD5B8Ap3XKGa-6AgQ0kDOaxGpA81_vZA";

// Variables del bot y el LED
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);
const int ledPin = 2;           // Pin del LED

// Variable para la clave
String clave = "2016";   ///Por defecto el sistema inicia con 2016 como clave
bool esperaToken= false; 
bool esperaClaveNueva= false; 


void setup() {
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);    // Configura el LED como salida
    digitalWrite(ledPin, LOW);  // Asegúrate de que el LED esté apagado inicialmente

    // Conexión a WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Conectando a WiFi...");
    }
    Serial.println("Conexión WiFi establecida.");

    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Configura conexión segura
    Serial.println("Conexión segura configurada.");
}

void loop() {
    // Verifica si hay mensajes nuevos
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    if (numNewMessages > 0) {
        for (int i = 0; i < numNewMessages; i++) {
            // Obtiene la información del mensaje
            String chat_id = bot.messages[i].chat_id;  // ID del chat
            String mensajeRecibido = bot.messages[i].text; // Texto del mensaje

            // Muestra el mensaje recibido en el monitor serial
            Serial.println("Mensaje recibido: " + mensajeRecibido);

            // Compara el mensaje recibido con la clave
           if (!esperaToken && !esperaClaveNueva && !mensajeRecibido.equalsIgnoreCase("CAMBIO CLAVE")) {
            if (mensajeRecibido == clave) {
                digitalWrite(ledPin, HIGH); // Enciende el LED
                bot.sendMessage(chat_id, "Clave correcta. LED encendido.", "Markdown");
                delay(3000);               // Mantén el LED encendido por 3 segundos
                digitalWrite(ledPin, LOW); // Apaga el LED
                bot.sendMessage(chat_id, "LED apagado.", "Markdown");
                
            } 
            else {
                bot.sendMessage(chat_id, "Clave incorrecta. Intente nuevamente.", "Markdown");
                }
            } if (mensajeRecibido.equalsIgnoreCase("CAMBIO CLAVE") && !esperaToken && !esperaClaveNueva ) { ///EqualsInoreCase es para no distinguir entre mayusculas y minusculas
                 bot.sendMessage(chat_id, "Ingrese token de autorizacion", "Markdown"); 
                 esperaToken = true;
            }
            else if (esperaToken == true) {
                 
                 if (mensajeRecibido.equals("1234")) { // Token correcto
                    
                    bot.sendMessage(chat_id, "Token correcto, cambio de clave autorizado", "Markdown");
                    bot.sendMessage(chat_id, "Ingrese nueva clave", "Markdown");
                    
                    esperaClaveNueva = true; // Cambiar estado a "esperando nueva clave"esperandoNuevaClave = true; // Cambia al estado de espera de nueva clave
                    esperaToken = false;
                  }
                  else {
                      bot.sendMessage(chat_id, "Token incorrecto, intente nuevamente", "Markdown");
                  }  
            }
            else if (esperaClaveNueva == true) {
                    
                    if (mensajeRecibido.length() == 4 && mensajeRecibido.toInt() > 0){
                    //clave no mayor a 4 digitos numericos
                    clave ="";
                    clave= mensajeRecibido;
                    bot.sendMessage(chat_id, "Clave actualizada exitosamente", "Markdown");
                    bot.sendMessage(chat_id, "Nueva clave: " + clave, "Markdown");
                    esperaClaveNueva = false; // Cambiar estado a "esperando nueva clave"esperandoNuevaClave = true; // Cambia al estado de espera de nueva clave
                 } else {
                   bot.sendMessage(chat_id, "La clave debe ser un número de 4 dígitos.", "Markdown");
                  }
            }      
    }
 }
}
