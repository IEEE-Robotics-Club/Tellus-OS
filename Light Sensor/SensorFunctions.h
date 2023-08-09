float readLightSensor() {                                     // reads the light sensor and returns a float(0-100) represents light exposure percentage
  const int pin = A6;                                         // Testing on an arduino I used pin A0, needs to change to A6 for teensy

  int val = analogRead(pin);                                  // Read the value from the photoresistor
  int percentage = (100 - (map(val, 55, 1020, 0, 100)));      // Convert value to percentage

  return percentage;                                          // Return the percentage value
}
