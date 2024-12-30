
#include <Wire.h>
#include <Keypad.h>

#include <LiquidCrystal_PCF8574.h>

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
String contrasena = "2016"; // Contraseña correcta
String entrada = "";        // Almacena la entrada del usuario

// Configuración de la salida digital
const int salida = 2; // GPIO de la salida (ajusta según tu preferencia)
const unsigned long tiempoActivo = 3000; // Tiempo en milisegundos (3 segundos)

void setup() {
  Serial.begin(9600);
  
  // Configuración de la salida digital
  pinMode(salida, OUTPUT);
  digitalWrite(salida, LOW);
  
  // Inicializa el LCD
  Wire.begin();     // Inicia la comunicación I2C para Arduino UNO
  lcd.begin(16, 2);  // Inicializa el LCD con 16 columnas y 2 filas
  lcd.setBacklight(255);  // Activa la luz de fondo (0 para apagarla)
  lcd.setCursor(0, 0);    // Posiciona el cursor en la primera columna y fila
  lcd.print("INGRESAR CLAVE:");  // Muestra un mensaje en la pantalla
  
  Serial.println("Sistema iniciado. Ingrese la contraseña.");
}

////////////////////////////////////////////////////////////////////////////////////////

void loop() {
 
  char tecla = teclado.getKey(); // Lee una tecla del teclado

  
  if (tecla) {
    // Si se presiona una tecla, la muestra en el monitor serial
    Serial.println(tecla);
  
    if (tecla == '#') {
    // Si se presiona '#', verifica la contraseña
          if (entrada == contrasena) {
          Serial.println("Contraseña correcta. Puede entrar.");
          lcd.setCursor(0, 1);    // Posiciona el cursor en la primera columna y  segunda fila
          lcd.print("Abriendo puerta"); //
          digitalWrite(salida, HIGH); // Activa la salida
          delay(tiempoActivo);        // Mantiene la salida activa por el tiempo definido
          digitalWrite(salida, LOW);  // Desactiva la salida
          Serial.println("Puerta Cerrada.");
          lcd.setCursor(0, 1);    // Posiciona el cursor en la primera columna y  segunda fila
          lcd.print("Puerta Cerrada"); //
          delay(3000);  
          lcd.setCursor(0, 1);    // Posiciona el cursor en la primera columna y  segunda fila
          lcd.print("                "); // Imprime 16 espacios para limpiar la línea
          lcd.setCursor(0, 1);          // Vuelve a colocar el cursor al inicio de la segunda línea
          entrada = ""; // Limpia la entrada para un nuevo intento
          } 
      
          else { 
          Serial.println("Contraseña incorrecta. Inténtelo de nuevo.");
          lcd.setCursor(0, 1);    // Posiciona el cursor en la primera columna y  segunda fila
          lcd.print("Clave incorrecta"); // 
          delay(3000);
          lcd.setCursor(0, 1);    // Posiciona el cursor en la primera columna y  segunda fila
          lcd.print("                "); // Imprime 16 espacios para limpiar la línea
          lcd.setCursor(0, 1);    // Posiciona el cursor en la primera columna y  segunda fila
          entrada = ""; // Limpia la entrada para un nuevo intento
          }

    } 
    
    else if (tecla == '*') {
      // Si se presiona '*', borra la entrada actual
          entrada = "";
          Serial.println("Entrada borrada.");
          lcd.setCursor(0, 1);    // Posiciona el cursor en la primera columna y  segunda fila
          lcd.print("                "); // Imprime 16 espacios para limpiar la línea
          lcd.setCursor(0, 1);          // Vuelve a colocar el cursor al inicio de la segunda línea
    }

     
  else {
      // Si es un dígito válido, lo agrega a la entrada
      entrada += tecla;
      Serial.print("Entrada actual: ");
      Serial.println(entrada);
      
      lcd.setCursor(0, 1); // Configura el cursor en la segunda línea
      lcd.print(entrada); // Muestra la cadena actualizada
      // **Aquí está la nueva funcionalidad**
      if (entrada.length() > 4) {
        Serial.println("Num. de digitos exedido");
        entrada = ""; // Limpia la entrada si excede 4 dígitos
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
