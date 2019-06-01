#include <Scheduler_int_param.h>



//----------------------------------------------------------
// 2-dimensional array of row pin numbers:
const int row[8] = {
  2, 7, 19, 5, 13, 18, 12, 16
};

// 2-dimensional array of column pin numbers:
const int col[8] = {
  6, 11, 10, 3, 17, 4, 8, 9
};

// 2-dimensional array of pixels:
int pixels[8][8];

// cursor position:
int x = 5;
int y = 5;
//---------------------------------------------------------

int sensor=1;
Scheduler sched;

void setup() {
  randomSeed(0);

 // Serial.begin(9600);
  // put your setup code here, to run once:
  //---------------------------------------------------------
  // initialize the I/O pins as outputs iterate over the pins:
  for (int thisPin = 0; thisPin < 8; thisPin++) {
    // initialize the output pins:
    pinMode(col[thisPin], OUTPUT);
    pinMode(row[thisPin], OUTPUT);
    // take the col pins (i.e. the cathodes) high to ensure that the LEDS are off:
    digitalWrite(col[thisPin], HIGH);
  }

  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      pixels[x][y] = HIGH;
    }
  }
  x = 0;
  y = 0;
  //---------------------------------------------------------
  // sched.addTask(movePixelX)->withInterval(100)->withRepeatCount(0);
  //  sched.addTask(movePixelY)->withInterval(799)->withRepeatCount(0)->withShift(0);
//  for (int i = 0; i < 8; i++) {
//    sched.addTask(moveLine, i)->withInterval((random(9) + 10) * 20)->withRepeatCount(0);
//  }

}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println("loop");
  // delay(1000);
  int emptyCount = sched.emptyCount();
  if (emptyCount > 0) {
    for (int i = 0; i < emptyCount; i++) {
      sched._addTask(moveLine)->withInterval((random(9) + 1) * 10*sensor)->withRepeatCount(2);
    }
  }
sched.run();
readSensors();
//  Serial.println("loop end");
refreshScreen();

}

void readSensors() {
  // turn off the last position:

  // read the sensors for X and Y values:
  sensor = map(analogRead(A1), 0, 1023, 1, 10);
  Serial.print(sensor);


}


void movePixelX() {
  pixels[x][y] = HIGH;
  x = (x + 1) >= 8 ? 0 : (x + 1);
  pixels[x][y] = LOW;
}
void movePixelY() {
  pixels[x][y] = HIGH;
  y = (y + 1) >= 8 ? 0 : (y + 1);
  pixels[x][y] = LOW;
}

void moveLine(int line) {
  for (int i = 0; i < 8; i++) {
    if (pixels[line][i] == LOW) {
      pixels[line][i] = HIGH;
      pixels[line][i == 7 ? 0 : i + 1] = LOW;
      //Serial.println("old pixel");
      return;
    }
  }
  // Serial.println("new pixel");
  pixels[line][0] = LOW;
}

void refreshScreen() {
  // iterate over the rows (anodes):
  for (int thisRow = 0; thisRow < 8; thisRow++) {
    // take the row pin (anode) high:
    digitalWrite(row[thisRow], HIGH);
    // iterate over the cols (cathodes):
    for (int thisCol = 0; thisCol < 8; thisCol++) {
      // get the state of the current pixel;
      int thisPixel = pixels[thisRow][thisCol];
      // when the row is HIGH and the col is LOW,
      // the LED where they meet turns on:
      digitalWrite(col[thisCol], thisPixel);
      // turn the pixel off:
      if (thisPixel == LOW) {
        digitalWrite(col[thisCol], HIGH);
      }
    }
    // take the row pin low to turn off the whole row:
    digitalWrite(row[thisRow], LOW);
  }
}
