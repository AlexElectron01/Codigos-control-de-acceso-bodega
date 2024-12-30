#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

// Configura la dirección I2C y el tamaño del LCD
LiquidCrystal_PCF8574 lcd(0x27); // Cambia 0x27 por la dirección de tu pantalla

void setup() {
  Wire.begin();     // Inicia la comunicación I2C para Arduino UNO
  lcd.begin(16, 2);  // Inicializa el LCD con 16 columnas y 2 filas
  lcd.setBacklight(255);  // Activa la luz de fondo (0 para apagarla)
  lcd.setCursor(0, 0);    // Posiciona el cursor en la primera columna y fila
  lcd.print("Hola, Arduino!");  // Muestra un mensaje en la pantalla
}

void loop() {
  lcd.setCursor(0, 1);     // Posiciona el cursor en la segunda fila
  lcd.print(millis() / 1000);  // Muestra el tiempo en segundos desde el inicio
  delay(1000);              // Espera 1 segundo
}
