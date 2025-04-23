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
#define STEP1_PIN 10
#define DIR1_PIN 9
#define EN1_PIN 8
// Silnik 2
#define STEP2_PIN 5
#define DIR2_PIN 6
#define EN2_PIN 7

AccelStepper stepper1(AccelStepper::DRIVER, STEP1_PIN, DIR1_PIN);
AccelStepper stepper2(AccelStepper::DRIVER, STEP2_PIN, DIR2_PIN);

const byte ROWS = 4;
const byte COLS = 3;

byte rowPins[ROWS] = {23, 25, 27, 29};
byte colPins[COLS] = {31, 33, 35};

char keys[ROWS][COLS] = { //mapowanie klawiatury
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

Keypad klawiatura = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); //inicjalizacja klawiatury

void setup() {
  Serial.begin(9600);  //komunikacja z komputerem
  Serial1.begin(9600); //komunikacja z HC-05 - bluetooth

  pinMode(13, OUTPUT);

  stepper1.setMaxSpeed(1000);
  stepper1.setAcceleration(500);
  stepper1.setEnablePin(EN1_PIN);
  stepper1.setPinsInverted(false, false, true); // true - enable aktywne LOW
  stepper1.disableOutputs(); // na start wyłączony

  stepper2.setMaxSpeed(1000);
  stepper2.setAcceleration(500);
  stepper2.setEnablePin(EN2_PIN);
  stepper2.setPinsInverted(false, false, false);
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
    Serial1.println(klawisz);
  }
}

void executeCommand(char command) {
  switch (command) {
    case UP:
      stepper1.move(40);
      break;
    case DOWN:
      stepper1.move(-40);
      break;
    case LEFT:
      stepper2.move(40);
      break;
    case RIGHT:
      stepper2.move(-40);
      break;
    case START:
      stepper1.enableOutputs();
      stepper2.enableOutputs();
      break;
    case STOP:
      stepper1.disableOutputs(); // odłącza silnik
      stepper2.disableOutputs(); // odłącza silnik
      break;
    default:
      //Invalid command received
      break;
  }
}


