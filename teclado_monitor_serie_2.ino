#include <Keypad.h>

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
const unsigned long tiempoActivo = 6000; // Tiempo en milisegundos (6 segundos)

///////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  
  // Configuración de la salida digital
  pinMode(salida, OUTPUT);
  digitalWrite(salida, LOW);
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
        digitalWrite(salida, HIGH); // Activa la salida
        delay(tiempoActivo);        // Mantiene la salida activa por el tiempo definido
        digitalWrite(salida, LOW);  // Desactiva la salida
        Serial.println("Puerta Cerrada.");
      } else {
        Serial.println("Contraseña incorrecta. Inténtelo de nuevo.");
      }
      entrada = ""; // Limpia la entrada para un nuevo intento
    } else if (tecla == '*') {
      // Si se presiona '*', borra la entrada actual
      entrada = "";
      Serial.println("Entrada borrada.");
    } else {
      // Si es un dígito válido, lo agrega a la entrada
      entrada += tecla;
      Serial.print("Entrada actual: ");
      Serial.println(entrada);

      // **Aquí está la nueva funcionalidad**
      if (entrada.length() > 4) {
        Serial.println("Num. de digitos exedido");
        entrada = ""; // Limpia la entrada si excede 4 dígitos
      }
    }
  }
}
