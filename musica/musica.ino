#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// En el constructor de SoftwareSerial, el primer parámetro es el pin RX (lo que recibe Arduino)
// y el segundo es el pin TX (lo que envía Arduino).
// Asegúrate de que en el DFPlayer:
//    - El pin TX del DFPlayer va conectado al pin 10 (RX de Arduino).
//    - El pin RX del DFPlayer va conectado al pin 11 (TX de Arduino).
SoftwareSerial mySoftwareSerial(10, 11);
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  // Inicia el Serial para el monitor a 115200
  Serial.begin(115200);
  // Espera a que inicie el Serial si estás en un Arduino que lo soporte
  while (!Serial) {
    ; // Espera
  }
  
  Serial.println("Iniciando DFPlayer Mini...");

  // Inicia la comunicación con el DFPlayer a 9600 baudios
  mySoftwareSerial.begin(9600);
  
  // Inicializa el DFPlayer
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println("Error al iniciar DFPlayer Mini");
    while (true) {
      // Se queda aquí si no pudo inicializar
    }
  }
  
  Serial.println("DFPlayer Mini iniciado correctamente.");
  
  // Opcional: establece volumen por defecto (0–30)
  myDFPlayer.volume(15);
  
  // Reproduce la pista 1 por defecto (asegúrate de que exista en la SD)
  myDFPlayer.play(1);
  
  Serial.println("Reproduciendo pista 1");
  Serial.println("Comandos disponibles: 'n' (siguiente), 'p' (anterior), o un número de pista.");
}

void loop() {
  // Mantén "actualizado" al DFPlayer: si hay eventos (track finalizado, error, etc.) los procesará
  if (myDFPlayer.available()) {
    uint8_t type = myDFPlayer.readType();
    int value = myDFPlayer.read();

    switch (type) {
      case DFPlayerError:
        Serial.print("Error DFPlayer: ");
        Serial.println(value);
        break;
      
      case DFPlayerPlayFinished:
        // Se avisa cuál pista terminó
        Serial.print("Pista finalizada: ");
        Serial.println(value);
        break;
        
      // Otros casos, si necesitas: inicialización, etc.
      default:
        // No hacemos nada
        break;
    }
  }
  
  // Revisa si el usuario escribió algo en el monitor Serial
  if (Serial.available() > 0) {
    // Lee hasta salto de línea
    String input = Serial.readStringUntil('\n');
    // Elimina espacios en blanco al inicio y final
    input.trim();

    // Solo si hay algo válido
    if (input.length() > 0) {
      // 'n' para siguiente canción
      if (input.equalsIgnoreCase("n")) {
        Serial.println("Siguiente canción...");
        myDFPlayer.next();
      }
      // 'p' para anterior
      else if (input.equalsIgnoreCase("p")) {
        Serial.println("Canción anterior...");
        myDFPlayer.previous();
      }
      // En otro caso, tratamos de parsear un número y reproducirlo
      else {
        int track = input.toInt();
        if (track > 0) {
          Serial.print("Reproduciendo pista: ");
          Serial.println(track);
          myDFPlayer.play(track);
        } else {
          Serial.println("Comando no reconocido. Usa 'n', 'p' o un número de pista.");
        }
      }
    }
  }
}
