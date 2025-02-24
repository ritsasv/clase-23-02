#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// Configura SoftwareSerial para comunicarte con el DFPlayer Mini (Pin 10: RX, Pin 11: TX)
SoftwareSerial mySoftwareSerial(10, 11);
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando DFPlayer Mini...");
  
  // Inicia la comunicación por SoftwareSerial a 9600 baudios
  mySoftwareSerial.begin(9600);
  
  // Inicializa el DFPlayer Mini
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println("Error al iniciar DFPlayer Mini");
    while (true); // Si falla la inicialización, detiene el programa
  }
  
  Serial.println("DFPlayer Mini iniciado correctamente.");
  
  // Configura el volumen (rango de 0 a 30)
  myDFPlayer.volume(20);
  
  // Reproduce la primera pista al inicio
  myDFPlayer.play(1);
  Serial.println("Reproduciendo pista 1");
  Serial.println("Comandos disponibles: 'n' para siguiente, 'p' para anterior o ingresa un número para reproducir esa pista.");
}

void loop() {
  if (Serial.available()) {
    // Lee la entrada del Monitor Serial hasta el salto de línea
    String input = Serial.readStringUntil('\n');
    input.trim();  // Elimina espacios y saltos de línea innecesarios
    
    if (input.length() > 0) {
      // Cambiar a la siguiente canción
      if (input.equalsIgnoreCase("n")) {
        Serial.println("Siguiente canción...");
        myDFPlayer.next();
      } 
      // Cambiar a la canción anterior
      else if (input.equalsIgnoreCase("p")) {
        Serial.println("Canción anterior...");
        myDFPlayer.previous();
      } 
      // Si se ingresa un número, se interpreta como el número de pista a reproducir
      else {
        int track = input.toInt();
        if (track > 0) {
          Serial.print("Reproduciendo pista: ");
          Serial.println(track);
          myDFPlayer.play(track);
        } else {
          Serial.println("Comando no reconocido. Usa 'n' para siguiente, 'p' para anterior o ingresa un número de pista.");
        }
      }
    }
  }
}
