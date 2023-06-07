#include <Adafruit_CircuitPlayground.h>
#include <EEPROM.h>
#include <Arduino.h>

#define WINDOW_SIZE 8
#define AXES 3
#define SAMPLES 40
#define SAVED_ADDRESS 0

// FUNCTION DEFINITIONS
void print_gesture_data();
void save_gesture();
void load_gesture();
void read_moving_averaged();
float get_moving_average();
float correlations[AXES] = {0.0};

// Storing the current/moving averaged readings of x, y, z
// from the accelerometer sensor on board in arrays
float current_reading[AXES] = {0};
float moving_averaged_reading[AXES] = {0};

// This is the gesture data that is recorded as a key
float gesture_data[AXES][SAMPLES] = {0};
// This is the gesture data that the user tries to unlock the system with
float real_time_gesture_data[AXES][SAMPLES] = {0};

// 2D array to store the buffered readings to calculate the moving averages for 3 axes
float readings[AXES][WINDOW_SIZE] = {0};

// Takes the current reading of the given axis and returns the moving average value
float get_moving_average(float x, int axis) {
  float results_avg = 0;

  for (int j = 1; j < WINDOW_SIZE; j++) {
    results_avg += readings[axis][j];
    readings[axis][j - 1] = readings[axis][j];
  }
  readings[axis][WINDOW_SIZE - 1] = x;
  results_avg += x;
  return float(results_avg / WINDOW_SIZE);
}

// Sets the gesture data for a given sample
void set_gesture(int s) {
  // Getting current values and calculating moving average for each axis
  for (int axis = 0; axis < 3; axis++) {
    if (axis == 0) {
      current_reading[axis] = CircuitPlayground.motionX();
    } else if (axis == 1) {
      current_reading[axis] = CircuitPlayground.motionY();
    } else {
      current_reading[axis] = CircuitPlayground.motionZ();
    }
    moving_averaged_reading[axis] = get_moving_average(current_reading[axis], axis);
    gesture_data[axis][s] = moving_averaged_reading[axis];
  }
}

// Collects real-time gesture data for a given sample
void collect_real_time_gesture(int s) {
  for (int axis = 0; axis < 3; axis++) {
    if (axis == 0) {
      current_reading[axis] = CircuitPlayground.motionX();
    } else if (axis == 1) {
      current_reading[axis] = CircuitPlayground.motionY();
    } else {
      current_reading[axis] = CircuitPlayground.motionZ();
    }
    moving_averaged_reading[axis] = get_moving_average(current_reading[axis], axis);
    real_time_gesture_data[axis][s] = moving_averaged_reading[axis];
  }
}

// Saves the recorded gesture data to EEPROM
void save_gesture() {
  int address = SAVED_ADDRESS;
  for (int i = 0; i < AXES; i++) {
    for (int j = 0; j < SAMPLES; j++) {
      float data = gesture_data[i][j];
      EEPROM.put(address, data);
      address += sizeof(float);
    }
  }
  Serial.print("############### GESTURE SAVED !! ##############");
}

// Prints the gesture data
void print_gesture_data(float gesture_data[][SAMPLES]) {
   // Print column headers
  Serial.print("Sample");
  for (int axis = 0; axis < AXES; axis++) {
    Serial.print("\tAxis ");
    switch (axis) {
      case 0:
        Serial.print("X");
        break;
      case 1:
        Serial.print("Y");
        break;
      case 2:
        Serial.print("Z");
        break;
      default:
        // handle error case
        break;
    }
  }
  Serial.println();
  // Print data for each sample
  for (int sample = 0; sample < SAMPLES; sample++) {
    Serial.print(sample);
    for (int axis = 0; axis < AXES; axis++) {
      Serial.print("\t");
      Serial.print(gesture_data[axis][sample]);
    }
    Serial.println();
  }
}
// Loads the saved gesture data from EEPROM
void load_gesture() {
  int address = SAVED_ADDRESS;
  Serial.print("\n");
  Serial.print("############LOADING GESTURE!###########");
  Serial.print("\n");
  for (int i = 0; i < AXES; i++) {
    for (int j = 0; j < SAMPLES; j++) {
      EEPROM.get(address, gesture_data[i][j]);
      address += sizeof(float);
    }
  }
  print_gesture_data(gesture_data);
  Serial.print("\n");
  Serial.print("############GESTURE LOADED!###########");
  Serial.print("\n");
}
// Sets the LED pixels to indicate the recording phase
void recording_flag(){
    for (int i = 0; i < 10; i++)
    {
      i=i+1;
      CircuitPlayground.setPixelColor(i, 255, 160, 0); // orange LED
    }
    Serial.print("\n");
    Serial.print("SETTING THE LCOK ");
    Serial.print("\n");
}
// Sets the LED pixels to indicate the attempting phase
void attempting_flag(){
    for (int i = 0; i < 10; i++)
    {
      i=i+1;
      CircuitPlayground.setPixelColor(i, 200, 200, 200); // white LED
    }
    Serial.print("\n");
    Serial.print("ATTEMPTING TO UNLCOK");
    Serial.print("\n");
}
// Sets the LED pixels to indicate a correct gesture
void correct_flag(){
    CircuitPlayground.playTone(1000, 200); // success tone
    for (int i = 0; i < 10; i++)
    {
      i=i+1;
      CircuitPlayground.setPixelColor(i, 0, 255, 0); // green LED
    }
    delay(800);
    CircuitPlayground.clearPixels();
}
// Sets the LED pixels to indicate an incorrect gesture
void incorrect_flag(){
    CircuitPlayground.playTone(200, 400);
    for (int i = 0; i < 10; i++)
    {
      i=i+1;
      CircuitPlayground.setPixelColor(i, 255, 0, 0); // red LED
    }
    delay(800);
    CircuitPlayground.clearPixels();
}

void correlate(float data1[][SAMPLES], float data2[][SAMPLES], float correlations[]) {
  // calculate the correlation between the stored gesture data and real-time gesture data for each axis
  for (int i = 0; i < AXES; i++) {
    float sum1 = 0.0, sum2 = 0.0, sum1_sq = 0.0, sum2_sq = 0.0, p_sum = 0.0;
        // Calculate sums and cross products
    for (int j = 5; j < SAMPLES - 5; j++) {
      sum1 += data1[i][j];
      sum2 += data2[i][j];
      sum1_sq += pow(data1[i][j], 2);
      sum2_sq += pow(data2[i][j], 2);
      p_sum += data1[i][j] * data2[i][j];
    }
    float n = SAMPLES;
      // Calculate the numerator and denominator of the correlation formula
    float numerator = p_sum - (sum1 * sum2 / n);
    float denominator = sqrt((sum1_sq - pow(sum1, 2) / n) * (sum2_sq - pow(sum2, 2) / n));
     // Calculate the correlation coefficient
    correlations[i] = numerator / denominator;
  }
}

//Print the coorelation data, and check if it is in the reasonable range
void check_gesture(){
    Serial.print("\n X ");
    Serial.print(correlations[0]);
    Serial.print("\n");
    Serial.print("\n Y ");
    Serial.print(correlations[1]);
    Serial.print("\n");
    Serial.print("\n Z ");
    Serial.print(correlations[2]);
    Serial.print("\n");
  if (correlations[0]>0.3 && correlations[1]>0.3 && correlations[2]>0.3){
        correct_flag();
      }
  else
  {
        incorrect_flag();
  }
}


void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
}

void loop() {
  // Check if the right button is pressed to record a gesture
  if (CircuitPlayground.rightButton()) {
    // Reset gesture data array
    gesture_data[AXES][SAMPLES] = {0};
    int sample = 0;
    
    // Continue recording while the right button is pressed and the sample count is within the limit
    while (CircuitPlayground.rightButton() && (sample <= SAMPLES)) {
      set_gesture(sample);   // Set the gesture values for the current sample
      delay(20);
      sample++;
      recording_flag();   // Set recording flag to indicate the ongoing recording process
      
      // Check if the maximum sample count is reached
      if (sample == SAMPLES) {
        Serial.print("\n");
        Serial.print("############ENOUGH SAMPLES!!###########");
        Serial.print("\n");
        break;
      }
    }
    
    CircuitPlayground.clearPixels();
    delay(100);
    save_gesture();   // Save the recorded gesture data
    delay(500);
    Serial.print("\n");
    Serial.print("############SAVED GESTURE DATA!!###########");
    Serial.print("\n");
    print_gesture_data(gesture_data);   // Print the recorded gesture data
  }

  // Check if the left button is pressed to collect real-time gesture data
  else if (CircuitPlayground.leftButton()) {
    int sample2 = 0;
    
    // Collect real-time gesture data while the left button is pressed and the sample count is within the limit
    while (CircuitPlayground.leftButton() && (sample2 <= SAMPLES)) {
      collect_real_time_gesture(sample2);   // Collect real-time gesture values for the current sample
      delay(20);
      sample2++;
      attempting_flag();   // Set attempting flag to indicate the ongoing data collection
      
      // Check if the maximum sample count is reached
      if (sample2 == SAMPLES) {
        Serial.print("\n");
        Serial.print("############ENOUGH SAMPLES!!###########");
        Serial.print("\n");
        break;
      }
    }
    CircuitPlayground.clearPixels();
    delay(10);
    Serial.print("\n");
    Serial.print("############REAL TIME GESTURE DATA!!###########");
    Serial.print("\n");
    print_gesture_data(real_time_gesture_data);   // Print the collected real-time gesture data
    delay(5);
    load_gesture();   // Load the saved gesture data
    delay(5);
    
    // Compare the loaded gesture data with the collected real-time gesture data
    correlations[AXES] = {0.0};
    Serial.print("\n");
    Serial.print("############--CORRELATION--###########");
    Serial.print("\n");
    correlate(gesture_data, real_time_gesture_data, correlations);   // Calculate correlations
    check_gesture();   // Check the gesture based on the correlations
    delay(200);
  }
}