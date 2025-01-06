#include <WiFi.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal_PCF8574.h>

// Configuración WiFi
const char* ssid = "CELERITY_MAX M";
const char* password = "@lexa1213";

// Token del bot de Telegram
const char* BOT_TOKEN = "8104194718:AAHOD5B8Ap3XKGa-6AgQ0kDOaxGpA81_vZA";

// Variables del bot y el LED
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);
const int ledPin = 2;

// Variables para el control de clave y estados
String clave = "2016"; 
bool esperaToken = false;
bool esperaClaveNueva = false;

// Configuración del LCD
LiquidCrystal_PCF8574 lcd(0x27);

// Configuración del teclado matricial 4x4
const byte filas = 4;
const byte columnas = 4;
char teclas[filas][columnas] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte pinesFilas[filas] = {27, 14, 12, 13};
byte pinesColumnas[columnas] = {32, 33, 25, 26};
Keypad teclado = Keypad(makeKeymap(teclas), pinesFilas, pinesColumnas, filas, columnas);

String entradaTeclado = "";

// Variables para alternar entre Telegram y teclado
bool revisarTelegram = true; // Indica si la próxima iteración revisará Telegram
unsigned long intervaloChequeo = 100; // Intervalo entre revisiones
unsigned long ultimaRevision = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Configuración del LCD
  Wire.begin();
  lcd.begin(16, 2);
  lcd.setBacklight(255);
  lcd.setCursor(0, 0);
  lcd.print("INGRESAR CLAVE:");

  // Configuración WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conexión WiFi establecida.");
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
}

void manejarTelegram() {
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  if (numNewMessages > 0) {
    for (int i = 0; i < numNewMessages; i++) {
      String chat_id = bot.messages[i].chat_id;
      String mensajeRecibido = bot.messages[i].text;
      Serial.println("Mensaje recibido: " + mensajeRecibido);

      if (!esperaToken && !esperaClaveNueva && !mensajeRecibido.equalsIgnoreCase("CAMBIO CLAVE")) {
        if (mensajeRecibido == clave) {
          digitalWrite(ledPin, HIGH);
          bot.sendMessage(chat_id, "Clave correcta. LED encendido.", "Markdown");
          delay(3000);
          digitalWrite(ledPin, LOW);
          bot.sendMessage(chat_id, "LED apagado.", "Markdown");
        } else {
          bot.sendMessage(chat_id, "Clave incorrecta. Intente nuevamente.", "Markdown");
        }
      } else if (mensajeRecibido.equalsIgnoreCase("CAMBIO CLAVE") && !esperaToken && !esperaClaveNueva) {
        bot.sendMessage(chat_id, "Ingrese token de autorizacion", "Markdown");
        esperaToken = true;
      } else if (esperaToken) {
        if (mensajeRecibido.equals("1234")) {
          bot.sendMessage(chat_id, "Token correcto. Cambio de clave autorizado.", "Markdown");
          bot.sendMessage(chat_id, "Ingrese nueva clave.", "Markdown");
          esperaClaveNueva = true;
          esperaToken = false;
        } else {
          bot.sendMessage(chat_id, "Token incorrecto. Intente nuevamente.", "Markdown");
        }
      } else if (esperaClaveNueva) {
        if (mensajeRecibido.length() == 4 && mensajeRecibido.toInt() > 0) {
          clave = mensajeRecibido;
          bot.sendMessage(chat_id, "Clave actualizada exitosamente.", "Markdown");
          bot.sendMessage(chat_id, "Nueva clave: " + clave, "Markdown");
          esperaClaveNueva = false;
        } else {
          bot.sendMessage(chat_id, "La clave debe ser un número de 4 dígitos.", "Markdown");
        }
      }
    }
  }
}

void manejarTeclado() {
  char tecla = teclado.getKey();
  if (tecla) {
    Serial.println(tecla);

    if (tecla == '#') {
      if (entradaTeclado == clave) {
        Serial.println("Contraseña correcta. Puede entrar.");
        lcd.setCursor(0, 1);
        lcd.print("Abriendo puerta");
        digitalWrite(ledPin, HIGH);
        delay(3000);
        digitalWrite(ledPin, LOW);
        lcd.setCursor(0, 1);
        lcd.print("Puerta Cerrada");
        delay(3000);
        lcd.setCursor(0, 1);
        lcd.print("                ");
        entradaTeclado = "";
      } else {
        Serial.println("Contraseña incorrecta.");
        lcd.setCursor(0, 1);
        lcd.print("Clave incorrecta");
        delay(3000);
        lcd.setCursor(0, 1);
        lcd.print("                ");
        entradaTeclado = "";
      }
    } else if (tecla == '*') {
      entradaTeclado = "";
      Serial.println("Entrada borrada.");
      lcd.setCursor(0, 1);
      lcd.print("                ");
    } else {
      entradaTeclado += tecla;
      lcd.setCursor(0, 1);
      lcd.print(entradaTeclado);
      if (entradaTeclado.length() > 4) {
        entradaTeclado = "";
        lcd.setCursor(0, 1);
        lcd.print("max.4 caracteres");
        delay(3000);
        lcd.setCursor(0, 1);
        lcd.print("                ");
      }
    }
  }
}

void loop() {
  if (millis() - ultimaRevision > intervaloChequeo) {
    ultimaRevision = millis();

    if (revisarTelegram) {
      manejarTelegram();
    } else {
      manejarTeclado();
    }

    revisarTelegram = !revisarTelegram; // Alterna entre Telegram y teclado
  }
}

