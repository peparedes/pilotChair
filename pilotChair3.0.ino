/*
 * Pilot Chair
 * 
 * supervisor | paredesp@cs.stanford.edu 
 * developers | minkyuk@andrew.cmu.edu, ml27@princeton.edu
 *         
 * pilotChair.ino supports the activations and control of the motors in the back of the car seat,
 * motivated and developed from an human-computer interaction perspective. Features with the motor
 * range from activation of a single motor to a activation of a row. Signals received from the seat
 * through arduino are total 14, representing activation of each row/col for a 7x7 matrix of motors.
 * Note that not all entries have a corresponding motor; mat is specficed in constant below.
 * Mixtures of interventions involve an apparent-tactile motion for which each signal sent is overlapped
 * by a factor (e.g. 20ms) for an effective swipe.
 * 
 */

// A0, A1 are pre-defined as analaog inputs for first row and col.
#define BUTT1_PIN A2
#define BUTT2_PIN A3
#define BUTT3_PIN A4
#define BUTT4_PIN A5
#define BUTT5_PIN A6

// 2-dimensional array of row pin numbers:
const byte colPins[7] = {2, 3, 4, 5, 6, 7, A1};
// 2-dimensional array of column pin numbers:
const byte rowPins[7] = {8, 9, 10, 11, 12, 13, A0};

/*
 * Pre-defined dimensions of the matrix of motors.
 */
const int mat[7][7] = {{1, 1, 1, 1, 1, 1, 1}, {0, 1, 1, 1, 1, 1, 0}, 
{0, 1, 1, 1, 1, 1, 0}, {0, 1, 1, 1, 1, 1, 0}, {1, 1, 1, 1, 1, 1, 1}, 
{1, 0, 1, 1, 1, 0, 1}, {0, 0, 1, 1, 1, 0, 0}};
 
//Header_begin

/*
 * Swipe down a column with overlapping delays.
 */
void fastSwipeOneCol(byte col, int dur_t, int delay_t);

/*
 * Swipe down two specifed columns with overlapping delays.
 */
void fastSwipeTwoColDown(byte col1, byte col2, int dur_t, int delay_t);

/*
 * Swipe up two column with overlapping delays.
 */
void fastSwipeTwoColUp(byte col1, byte col2, int dur_t, int delay_t);

/*
 * Specific type of breathing intervention for fast and quick inhale & exhale.
 */
void dragonBreath(byte bottom, byte top);

/*
 * Swipe down all rows with overlapping delays.
 */
void swipeAllRowsTopToBottom(int dur_t, int delay_t);

/*
 * Swipe a row with no overlap; mainly for testing.
 */
void rowTest(byte r);

/*
 * Swipe a col with no overlap; mainly for testing.
 */
void colTest(byte c);

/*
 * Switch on a row WITHOUT reset; must be coupled with manual resets.
 */
void rowOn(byte row);

/*
 * Switch on a col WITHOUT reset; must be coupled with manual resets.
 */
void colOn(byte col);

/*
 * Switch off a row WITHOUT reset; must be coupled with manual resets.
 */
void rowOff(byte row);

/*
 * Switch off a col WITHOUT reset; must be coupled with manual resets.
 */
void colOff(byte col);

/*
 * Switch on multiple rows WITHOUT reset; must be coupled with manual resets.
 * Specify starting and ending index of the row. s < i < e.
 */
void multiRowOn(byte startRow, byte endRow);

/*
 * Switch on multiple rows WITHOUT reset; must be coupled with manual resets.
 * Specify starting and ending index of the row. s < i < e.
 */
void multiColOn(byte startCol, byte endCol);

/*
 * Switch on a row WITH hard reset.
 */
void pckge_rowOn(byte row);

/*
 * Switch on a row WITH reset.
 */
void pckge_colOn(byte col);

/*
 * Switch on a single motor WITH reset.
 */
void singleton(byte row, byte col);

/*
 * Resets all rows except for specified row.
 * Useful to follow up with a change that incorporates no reset.
 */
void resetRowsExceptForOne(int row);

/*
 * Resets all rows except for specified row.
 * Useful to follow up with a change that incorporates no reset.
 */
void resetRowsExceptForTwo(byte row1, byte row2);

/*
 * Resets all rows except for specified row.
 * Useful to follow up with a change that incorporates no reset.
 */
void resetColsExceptForOne(int col);

/*
 * Resets all rows except for specified row.
 * Useful to follow up with a change that incorporates no reset.
 */
void resetColsExceptForTwo(byte col1, byte col2);

/* 
 *  Activates a region enclosed by specified coordinates 
 *  at left-hand corner and right-hand corner, respectively.
 *  @param (row1, col1): left-hand corner
 *  @param (row2, col2): right-hand corner
 */
void region(byte row1, byte row2, byte col1, byte col2);
/*
 * Resets all pins by setting columns to high and rows to low
 * respectively.
 */
void resetPins();

/*
 * Breathing intervention; swipe up the rows and holds the vibration,
 * then swipe down.
 */
void breathe(byte bottom, byte top);

/*
 * If returns 1, the motor is existent. otherwise, the motor is inactivate,
 * or, missing.
 */
int getValueAt(int row, int col);
void swipeUShaped(int wait_ms);

/*
 * The following four swiping interventions are inherited from old project
 * in which Apparent Tactile Motion was unneccessary. Note that these 
 * do not implement overlapping features.
 */
void swipeLeftToRight(int wait_ms);
void swipeRightToLeft(int wait_ms);
void swipeTopToBottom(int wait_ms);
void swipeBottomToTop(int wait_ms);

// Header_end

void setup() {
  Serial.begin(9600);

  // Setup input pins
  pinMode(BUTT1_PIN, INPUT_PULLUP);
  pinMode(BUTT2_PIN, INPUT_PULLUP);
  pinMode(BUTT3_PIN, INPUT_PULLUP);
  pinMode(BUTT4_PIN, INPUT_PULLUP);

  // Setup output pins
  resetPins();
}

// To turn a pin on, write LOW to column, and HIGH to row

void loop() {
  int time = 200;

  if (digitalRead(BUTT1_PIN) == LOW) {
    fastSwipeOneCol(2, 100, 50); // Swipe LEFT Shoulder Blade
  }

  if (digitalRead(BUTT2_PIN) == LOW) {
    fastSwipeOneCol(4, 100, 50); // Swipe RIGHT Shoulder Blade
  }

  if (digitalRead(BUTT3_PIN) == LOW) { //LEFT Region
    region(1, 2, 1, 2);
  }

  if (digitalRead(BUTT4_PIN) == LOW) { //RIGHT Region
    region(1, 2, 4, 5);
  }

  if (digitalRead(BUTT1_PIN) == LOW) { // Dragon's breath
    dragonBreath(6, 1); 
  }

  if (digitalRead(BUTT2_PIN) == LOW) { // Slow breath
    breathe(5, 2); 
  }

  if (digitalRead(BUTT3_PIN) == LOW) { //Swipe UP
    fastSwipeTwoColUp(2, 4, 100, 50);
  }

  if (digitalRead(BUTT4_PIN) == LOW) {
    fastSwipeTwoColDown(2, 4, 100, 50); //Swipe DOWN
  }  

  if (digitalRead(BUTT4_PIN) == LOW) { //Left to right, right to left
    swipeLeftToRight(100);
    delay(600);
    swipeRightToLeft(100);
  }
}

void fastSwipeOneCol(byte col, int dur_t, int delay_t) {
  
  /* Turn on all columns */
  for (int i = 0; i < sizeof(colPins); i++) {
    digitalWrite(colPins[i], LOW);
  }
  
  /* Enter the Overlap Mission */
  for (int j = 0; j < sizeof(rowPins)-1; j++) {
    digitalWrite(rowPins[j], HIGH);
    resetColsExceptForOne(col);
    delay(dur_t);
    digitalWrite(rowPins[j+1], HIGH);
    resetColsExceptForOne(col);
    delay(delay_t);
    digitalWrite(rowPins[j], LOW);
    resetColsExceptForOne(col);
  }

  /* Optional: reset all pins after each mission*/
  resetPins();
}

void fastSwipeTwoColDown(byte col1, byte col2, int dur_t, int delay_t) {
  /* Turn on all columns */
  for (int i = 0; i < sizeof(colPins); i++) {
    if(i == col1 || i == col2) {
      colOn(i);
    } 
    else {
      colOff(i);
    }
    rowOff(i);
  }

  for (int j = 0; j < sizeof(rowPins); j++) {
    if(j == 0) { // first index
      //Phase 1
      rowOn(j);
      delay(dur_t);

      //Phase 2
      rowOn(j+1);
      delay(delay_t);

      //Phase 3
      rowOff(j);
    }
    
    if((j > 0) && (j < sizeof(rowPins)-1)) { // in betweens
      //Phase 1
      rowOn(j);
      delay(dur_t-delay_t);

      //Phase 2
      rowOff(j-1);
      rowOn(j+1);
      delay(delay_t);     
    }
    
    if(j == sizeof(rowPins)-1) { // last index 
      //Phase 1
      rowOn(j);
      delay(dur_t-delay_t);

      //Phase 2
      rowOff(j-1);
      delay(delay_t);
    }
  }
  resetPins();
}

void fastSwipeTwoColUp(byte col1, byte col2, int dur_t, int delay_t) {
  /* Turn on all columns */
  for (int i = 0; i < sizeof(colPins); i++) {
    if(i == col1 || i == col2) {
      colOn(i);
    } 
    else {
      colOff(i);
    }
    rowOff(i);
  }

  for (int j = sizeof(rowPins) - 1; j >=0; j--) {
    if(j == sizeof(rowPins) - 1) { // first index
      //Phase 1
      rowOn(j);
      delay(dur_t);

      //Phase 2
      rowOn(j-1);
      delay(delay_t);

      //Phase 3
      rowOff(j);
    }
    
    if((j > 0) && (j < sizeof(rowPins)-1)) { // in betweens
      //Phase 1
      rowOn(j);
      delay(dur_t-delay_t);

      //Phase 2
      rowOff(j+1);
      rowOn(j-1);
      delay(delay_t);     
    }
    
    if(j == 0) { // last index 
      //Phase 1
      rowOn(j);
      delay(dur_t-delay_t);

      //Phase 2
      rowOff(j+1);
      delay(delay_t);
    }
  }
  resetPins();
}

void dragonBreath(byte bottom, byte top) {
  if (bottom >= sizeof(rowPins)) {
    bottom = sizeof(rowPins) - 1;
  }
  
  if (top < 0) {
    top = 0;
  }
  
  for (int i = bottom; i > bottom/2; i--) { //in
    resetPins();
    pckge_rowOn(i);
    delay(200);
  }

  resetPins();
  delay(400); 

  for (int i = bottom/2; i >= top; i--) { //in
    resetPins();
    pckge_rowOn(i);
    delay(200);
  }

  resetPins();
  delay(800);
  
  swipeAllRowsTopToBottom(90, 40);
  
  resetPins(); // Turn all off when done
  delay(300);
}

void swipeAllRowsTopToBottom(int dur_t, int delay_t) {

  /* Turn on all columns */
  for (int i = 0; i < sizeof(colPins); i++) {
    colOn(i);
  }
  for (int j = 0; j < sizeof(rowPins)-1; j++) {
    if(j == 0) {
      rowOn(j);
      delay(dur_t);
      rowOn(j+1);
      delay(delay_t);
      rowOff(j);
    }
    else {
      rowOn(j);
      delay(dur_t - delay_t);
      rowOn(j+1);
      delay(delay_t);
      rowOff(j);
    }
  }
  /* Optional: reset all pins after each mission */
  resetPins(); 
}

void interestIng(int dur_t, int delay_t) {
 /* Turn on all columns */
  for (int i = 0; i < sizeof(colPins); i++) {
    colOn(i);
  }
  for (int j = 1; j < sizeof(rowPins)-1; j++) {
    if(j == 1) {
      rowOn(j);
      delay(dur_t);
      rowOn(j-1);
      delay(delay_t);
      rowOff(j);
    }
    else {
      rowOn(j);
      delay(dur_t - delay_t);
      rowOn(j-1);
      delay(delay_t);
      rowOff(j);
    }
  }
  /* Optional: reset all pins after each mission */
  //resetPins();  
}

void rowTest(byte r) {
  resetPins();
  pckge_rowOn(r);
}

void colTest(byte c) {
  resetPins();
  pckge_colOn(c);
}

void rowOn(byte row) {
  digitalWrite(rowPins[row], HIGH);
}

void colOn(byte col) {
  digitalWrite(colPins[col], LOW);
}

void rowOff(byte row) {
  digitalWrite(rowPins[row], LOW);
}

void colOff(byte col) {
  digitalWrite(colPins[col], HIGH);
}

void multiRowOn(byte startRow, byte endRow){

  // Turn on all the columnscol
  for (int i = 0; i < sizeof(colPins); i++) {
    digitalWrite(colPins[i], LOW);
  }
  
  for(int j = startRow; j < endRow+1; j++){
    digitalWrite(rowPins[j], HIGH);
  }
}

void multiColOn(byte startCol, byte endCol) {
 // Turn on all the rows
  for (int i = 0; i < sizeof(rowPins); i++) {
    digitalWrite(rowPins[i], HIGH);
  }
  
  for(int j = startCol; j < endCol+1; j++){
    digitalWrite(colPins[colPins[j]], LOW);
  }
}

// Turn an entire row on
void pckge_rowOn(byte row) {
  // Turn on all the columns
  for (int i = 0; i < sizeof(colPins); i++) {
    digitalWrite(colPins[i], LOW);
  }
  digitalWrite(rowPins[row], HIGH);
}

//Turn an entire column on
void pckge_colOn(byte col) {
  // Turn on all the columns
  for (int i = 0; i < sizeof(rowPins); i++) {
    digitalWrite(rowPins[i], HIGH);
  }

  digitalWrite(colPins[col], LOW);
}

// version 1 : Row -> Col
void singleton(byte row, byte col) {
  digitalWrite(colPins[col], LOW);
  delay(150);
  resetRowsExceptForOne(row);
  delay(150);
}

// version 2 : Col -> Row

void resetRowsExceptForOne(int row) {
  for (int i = 0; i < sizeof(rowPins); i++) {
    pinMode(rowPins[i], OUTPUT);
    if (i == row) {
      digitalWrite(rowPins[i], HIGH);
    } else {
      digitalWrite(rowPins[i], LOW);
    }
  }
}

void resetRowsExceptForTwo(byte row1, byte row2) {
  for (int i = 0; i < sizeof(rowPins); i++) {
    pinMode(rowPins[i], OUTPUT);
    if ((i == row1) || (i == row2)) {
      digitalWrite(rowPins[i], HIGH);
    } else {
      digitalWrite(rowPins[i], LOW);
    }
  }
}

void resetColsExceptForOne(int col) {
  for (int i = 0; i < sizeof(colPins); i++) {
    pinMode(colPins[i], OUTPUT);
    if (i == col) {
      digitalWrite(colPins[i], LOW);
    } else {
      digitalWrite(colPins[i], HIGH);
    }
  }
}

void resetColsExceptForTwo(byte col1, byte col2) {
  for (int i = 0; i < sizeof(colPins); i++) {
    pinMode(colPins[i], OUTPUT);
    if ((i == col1) || (i == col2)) {
      digitalWrite(colPins[i], LOW);
    } else {
      digitalWrite(colPins[i], HIGH);
    }
  }
}

void region(byte row1, byte row2, byte col1, byte col2) {
  resetColsExceptForTwo(col1, col2);
  //digitalWrite(rowPins[row2], HIGH);
  //digitalWrite(rowPins[row1], HIGH);
  for (int i = 0; i < 2; i++) {
    digitalWrite(rowPins[row2], HIGH);
    digitalWrite(rowPins[row1], HIGH);
    delay(100);
    digitalWrite(rowPins[row2], LOW);
    digitalWrite(rowPins[row1], LOW);
    delay(100);
  } 
  digitalWrite(rowPins[row2], HIGH);
  digitalWrite(rowPins[row1], HIGH);
  delay(400);
   
  resetPins();
  delay(200);
}

// Turns off ALL LEDs
void resetPins() {
  for (int i = 0; i < sizeof(rowPins); i++) {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], LOW);
  }

  for (int i = 0; i < sizeof(colPins); i++) {
    pinMode(colPins[i], OUTPUT);
    digitalWrite(colPins[i], HIGH);
  }
}


// wait_ms is the time between switching from one row/colum to another. 
// use a lower value to get the animation to go faster.
void swipeLeftToRight(int wait_ms) {
  for (int i = 0; i < sizeof(colPins); i++) {
    resetPins();
    pckge_colOn(i);
    delay(wait_ms);
  }

  resetPins(); // Turn all off when done
}

// wait_ms is the time between switching from one row/colum to another.
//use a lower value to get the animation to go faster.
void swipeRightToLeft(int wait_ms) {
  for (int i = sizeof(colPins); i >= 0 ; i--) {
    resetPins();
    pckge_colOn(i);
    delay(wait_ms);
  }

  resetPins(); // Turn all off when done
}

void swipeTopToBottom(int wait_ms) {
  for (int i = 0; i < sizeof(rowPins); i++) {
    resetPins();
    pckge_rowOn(i);
    delay(wait_ms);
  }

  resetPins(); // Turn all off when done
}

void swipeBottomToTop(int wait_ms) {
  for (int i = sizeof(rowPins); i >= 0; i--) {
    resetPins();
    pckge_rowOn(i);
    delay(wait_ms);
  }

  resetPins(); // Turn all off when done
}

void breathe(byte bottom, byte top) {
  if (bottom >= sizeof(rowPins)) {
    bottom = sizeof(rowPins) - 1;
  }
  
  if (top < 0) {
    top = 0;
  }
  
  for (int i = bottom; i >= top; i--) { //inhale
    resetPins();
    pckge_rowOn(i);
    delay(350);
  }

  for (int j = 0; j < 3; j++) { //hold
    resetPins();
    delay(700);
    pckge_rowOn(top);
    delay(300);
  }
  resetPins();
  delay(700); 
  
  for (int i = top; i <= bottom; i++) { //exhale
    resetPins();
    pckge_rowOn(i);
    delay(350);
  }

  for (int j = 0; j < 2; j++) { //hold
    resetPins();
    delay(700);
    pckge_rowOn(bottom);
    delay(300);
  }
  resetPins(); // Turn all off when done
  delay(700); 
}

/* Experimental Sesh */
/* (newly added features) */

void testCol(int time) {
  swipeLeftToRight(time);
}

void testRow(int time) {
  swipeTopToBottom(time);
}

void action_cross(int wait_ms) {
  colOn(3);
  delay(wait_ms);
  colOn(4);
  delay(wait_ms);
  rowOn(3);
  delay(wait_ms);
  rowOn(4);
  delay(wait_ms);
}

void everySingleOne(int wait_ms) {
  //resetPins();
  for (int i = 0; i < sizeof(colPins); i++) {
    for (int j = 0; j < sizeof(rowPins); j++) {
      if(getValueAt(i,j) == 1) {
        singleton(i,j);
        delay(10);
        resetPins();
      }
    }
  }
  resetPins();
}

int getValueAt(int row, int col) {
  return mat[row][col];
}

void swipeUShaped(int wait_ms){
  multiColOn(4,6);
  delay(wait_ms);
  resetPins();
  multiRowOn(0,3);
  delay(wait_ms);
  resetPins();
  multiColOn(0,3);
  delay(wait_ms);
  resetPins();
}
