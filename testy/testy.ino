#include <Keypad.h> //biblioteka do klawiatury

#define LEFT 'L'
#define RIGHT 'R'
#define UP 'F'
#define DOWN 'B'

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
  // put your setup code here, to run once:
  Serial.begin(9600);  //komunikacja z komputerem
  Serial1.begin(9600); //komunikacja z HC-05 - bluetooth

  pinMode(13, OUTPUT);
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
      digitalWrite(13, HIGH);
      delay(500);
      digitalWrite(13, LOW);
      break;
    case DOWN:
      digitalWrite(13, HIGH);
      delay(500);
      digitalWrite(13, LOW);
      break;
    case LEFT:
      digitalWrite(13, HIGH);
      delay(2000);
      digitalWrite(13, LOW);
      break;
    case RIGHT:
      digitalWrite(13, HIGH);
      delay(2000);
      digitalWrite(13, LOW);
      break;
    default:
      //Invalid command received
      break;
  }
}
