#include <Keypad.h>      //biblioteka do klawiatury
#include <AccelStepper.h> //biblioteka do silnikow krokowych

// Silnik 1
#define STEP1_PIN 5
#define DIR1_PIN 6
#define EN1_PIN 7
// Silnik 2
#define STEP2_PIN 10
#define DIR2_PIN 9
#define EN2_PIN 8

AccelStepper stepper1(AccelStepper::DRIVER, STEP1_PIN, DIR1_PIN);
AccelStepper stepper2(AccelStepper::DRIVER, STEP2_PIN, DIR2_PIN);

const byte ROWS = 4;
const byte COLS = 3;

byte rowPins[ROWS] = {23, 25, 27, 29};
byte colPins[COLS] = {31, 33, 35};

String commandBuffer = "";

/**Do klawiatury, znaki jakie na niej są.
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
*/
char keys[ROWS][COLS] = { //mapowanie klawiatury
  {'1','F','3'},
  {'L','5','R'},
  {'7','B','9'},
  {'A','0','P'}
};

Keypad klawiatura = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); //inicjalizacja klawiatury

void setup() {
  Serial.begin(9600);  //komunikacja z komputerem
  Serial1.begin(9600); //komunikacja z HC-05 - bluetooth

  pinMode(13, OUTPUT);

  stepper1.setMaxSpeed(2000);
  stepper1.setAcceleration(1000);
  stepper1.setEnablePin(EN1_PIN);
  stepper1.setPinsInverted(false, false, true); // true - enable aktywne LOW
  stepper1.disableOutputs(); // na start wyłączony

  stepper2.setMaxSpeed(2000);
  stepper2.setAcceleration(1000);
  stepper2.setEnablePin(EN2_PIN);
  stepper2.setPinsInverted(false, false, true);
  stepper2.disableOutputs();
}

void loop() {
  char klawisz = klawiatura.getKey();

  // Odczyt z Bluetooth
  while (Serial1.available()) {
    char c = Serial1.read();
    if (c == '\n') {
      if (commandBuffer.length() > 0) {
        executeCommand(commandBuffer);
        commandBuffer = "";
      }
    } else {
      commandBuffer += c;
    }
  }

  if (klawisz) {
    Serial.println(klawisz);
    executeCommand(String(klawisz));
  }

  stepper1.run();
  stepper2.run();
}
/**
  * Funkcja wykonuje odpowiedni ruch silnika w zależności od podanej komendy.
  *
  * \param[in] command Litera komendy.
  */
void executeCommand(String cmd) {

  Serial.print("Received command: ");
  Serial.println(cmd);

  char dir = cmd.charAt(0); // kierunek: F, B, L, R
  int speed = 1000; // domyślna prędkość
  if (cmd.length() > 1) {
    speed = cmd.substring(1).toInt() * 20; // 99 → 1980 (maxSpeed)
  }

  switch (dir) {
    case 'F':
      stepper1.setMaxSpeed(speed);
      stepper1.move(100); // symuluj jazdę w nieskończoność
      break;
    case 'B':
      stepper1.setMaxSpeed(speed);
      stepper1.move(-100);
      break;
    case 'L':
      stepper2.setMaxSpeed(speed);
      stepper2.move(100);
      break;
    case 'R':
      stepper2.setMaxSpeed(speed);
      stepper2.move(-100);
      break;
    case 'Y':
      stepper1.enableOutputs();
      stepper2.enableOutputs();
      break;
    case 'Z':
      stepper1.disableOutputs();
      stepper2.disableOutputs();
      break;
    default:
      break;
  }
}

void printMotorPinStates() {
  int step1State = digitalRead(10);
  int dir1State = digitalRead(9);
  int en1State  = digitalRead(8);

  int step2State = digitalRead(5);
  int dir2State = digitalRead(6);
  int en2State  = digitalRead(7);

  Serial.print("STEP1: "); Serial.print(step1State);
  Serial.print(" DIR1: "); Serial.print(dir1State);
  Serial.print(" EN1: ");  Serial.print(en1State);

  Serial.print(" | STEP2: "); Serial.print(step2State);
  Serial.print(" DIR2: "); Serial.print(dir2State);
  Serial.print(" EN2: ");  Serial.println(en2State);
}

