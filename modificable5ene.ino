#include <WiFi.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>
/////////////////////
#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal_PCF8574.h>
////////////////////
// Configuración WiFi
const char* ssid = "CELERITY_MAX M";
const char* password = "@lexa1213";

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
/////////////////////////////////////////////////////////////////////////////////////////
// Configura la dirección I2C y el tamaño del LCD
LiquidCrystal_PCF8574 lcd(0x27); // Cambia 0x27 por la dirección de tu pantalla


// Configuración del teclado matricial 4x4
const byte filas = 4; // Número de filas
const byte columnas = 4; // Número de columnas

// Mapeo de teclas (ajusta según tu teclado)
char teclas[filas][columnas] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Pines conectados al teclado (ajusta según tu conexión)
byte pinesFilas[filas] = {27, 14, 12, 13}; // GPIOs para las filas
byte pinesColumnas[columnas] = {32, 33, 25, 26}; // GPIOs para las columnas

Keypad teclado = Keypad(makeKeymap(teclas), pinesFilas, pinesColumnas, filas, columnas);

// Configuración de la contraseña

String entradaTeclado = "";        // Almacena la entrada del usuario
/////////////////////////////////////////////////////

void setup() {
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);    // Configura el LED como salida
    digitalWrite(ledPin, LOW);  // Asegúrate de que el LED esté apagado inicialmente
    ///////////////////////////////////////////////////////////////////////////////////
     Wire.begin();     // Inicia la comunicación I2C para Arduino UNO
     lcd.begin(16, 2);  // Inicializa el LCD con 16 columnas y 2 filas
     lcd.setBacklight(255);  // Activa la luz de fondo (0 para apagarla)
     lcd.setCursor(0, 0);    // Posiciona el cursor en la primera columna y fila
     lcd.print("INGRESAR CLAVE:");  // Muestra un mensaje en la pantalla
  //////////////////////////////////////////////////////////////////////////////////////
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
 char tecla = teclado.getKey(); // Lee una tecla del teclado

  
  if (tecla) {
    // Si se presiona una tecla, la muestra en el monitor serial
    Serial.println(tecla);
  
    if (tecla == '#') {
    // Si se presiona '#', verifica la contraseña
          if (entradaTeclado == clave) {
          Serial.println("Contraseña correcta. Puede entrar.");
          lcd.setCursor(0, 1);    // Posiciona el cursor en la primera columna y  segunda fila
          lcd.print("Abriendo puerta"); //
          digitalWrite(ledPin, HIGH); // Activa la salida
          delay(3000);        // Mantiene la salida activa por el tiempo definido
          digitalWrite(ledPin, LOW);  // Desactiva la salida
          Serial.println("Puerta Cerrada.");
          lcd.setCursor(0, 1);    // Posiciona el cursor en la primera columna y  segunda fila
          lcd.print("Puerta Cerrada"); //
          delay(3000);  
          lcd.setCursor(0, 1);    // Posiciona el cursor en la primera columna y  segunda fila
          lcd.print("                "); // Imprime 16 espacios para limpiar la línea
          lcd.setCursor(0, 1);          // Vuelve a colocar el cursor al inicio de la segunda línea
          entradaTeclado = ""; // Limpia la entrada para un nuevo intento
          } 
      
          else {
          Serial.println("Contraseña incorrecta. Inténtelo de nuevo.");
          lcd.setCursor(0, 1);    // Posiciona el cursor en la primera columna y  segunda fila
          lcd.print("Clave incorrecta"); // 
          delay(3000);
          lcd.setCursor(0, 1);    // Posiciona el cursor en la primera columna y  segunda fila
          lcd.print("                "); // Imprime 16 espacios para limpiar la línea
          lcd.setCursor(0, 1);    // Posiciona el cursor en la primera columna y  segunda fila
          entradaTeclado = ""; // Limpia la entrada para un nuevo intento
          }

    } 
    
    else if (tecla == '*') {
      // Si se presiona '*', borra la entrada actual
          entradaTeclado = "";
          Serial.println("Entrada borrada.");
          lcd.setCursor(0, 1);    // Posiciona el cursor en la primera columna y  segunda fila
          lcd.print("                "); // Imprime 16 espacios para limpiar la línea
          lcd.setCursor(0, 1);          // Vuelve a colocar el cursor al inicio de la segunda línea
    }

     
  else {
      // Si es un dígito válido, lo agrega a la entrada
      entradaTeclado += tecla;
      Serial.print("Entrada actual: ");
      Serial.println(entradaTeclado);
      
      lcd.setCursor(0, 1); // Configura el cursor en la segunda línea
      lcd.print(entradaTeclado); // Muestra la cadena actualizada
      // **Aquí está la nueva funcionalidad**
      if (entradaTeclado.length() > 4) {
        Serial.println("Num. de digitos exedido");
        entradaTeclado = ""; // Limpia la entrada si excede 4 dígitos
        lcd.setCursor(0, 1);    // Posiciona el cursor en la primera columna y  segunda fila
        lcd.print("max.4 caracteres"); //
        delay(3000);        // Mantiene la salida activa por el tiempo definido 
        lcd.setCursor(0, 1);          // Vuelve a colocar el cursor al inicio de la segunda línea
        lcd.print("                "); // Imprime 16 espacios para limpiar la línea
        lcd.setCursor(0, 1);          // Vuelve a colocar el cursor al inicio de la segunda línea
       }
     }
    }

}