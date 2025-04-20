#include <Keypad.h> //biblioteka do klawiatury

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
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  char klawisz = klawiatura.getKey();

  if (klawisz){
    Serial.println(klawisz);
  }
}
