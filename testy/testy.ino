#include <Keypad.h>      //biblioteka do klawiatury
#include <AccelStepper.h> //biblioteka do silnikow krokowych

// Przyciski w aplikacji
#define LEFT 'L'
#define RIGHT 'R'
#define UP 'F'
#define DOWN 'B'
#define START 'A'
#define STOP 'P'
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

  if (Serial1.available()) {
    char command = Serial1.read();
    executeCommand(command);
  }

  if (klawisz){
    Serial.println(klawisz);
    //Serial1.println(klawisz);

    executeCommand(klawisz);
  }

  stepper1.run();
  stepper2.run();
}
/**
  * Funkcja wykonuje odpowiedni ruch silnika w zależności od podanej komendy.
  *
  * \param[in] command Litera komendy.
  */
void executeCommand(char command) {
  switch (command) {
    case UP:
      stepper1.move(4000);
      Serial.println("Moving up");
      printMotorPinStates();
      break;
    case DOWN:
      stepper1.move(-4000);
      Serial.println("Moving down");
      printMotorPinStates();
      break;
    case LEFT:
      stepper2.move(4000);
      Serial.println("Moving left");
      printMotorPinStates();
      break;
    case RIGHT:
      stepper2.move(-4000);
      Serial.println("Moving right");
      printMotorPinStates();
      break;
    case START:
      // Enable motors
      Serial.println("Enable motors");
      printMotorPinStates();
      stepper1.enableOutputs();
      stepper2.enableOutputs();
      break;
    case STOP:
      // Disable motors
      Serial.println("Disable motors");
      printMotorPinStates();
      stepper1.disableOutputs();
      stepper2.disableOutputs();
      break;
    default:
      //Invalid command received
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

