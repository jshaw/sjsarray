#include <Array.h>

const byte SIZE = 10;
int rawArray[SIZE] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
Array<int> arr = Array<int>(rawArray, SIZE);

void setup() {
  Serial.begin(9600);

  // Print all elements using range-based for loop
  Serial.println("Array contents:");
  for (int v : arr) {
    Serial.print(v);
    Serial.print(" ");
  }
  Serial.println();

  // Min / max values and their positions
  Serial.print("Min value:    "); Serial.println(arr.getMin());
  Serial.print("Min at index: "); Serial.println(arr.getMinIndex());
  Serial.print("Max value:    "); Serial.println(arr.getMax());
  Serial.print("Max at index: "); Serial.println(arr.getMaxIndex());

  // Sum and average
  Serial.print("Sum:          "); Serial.println(arr.getSum());
  Serial.print("Average:      "); Serial.println(arr.getAverage());

  // Search
  Serial.print("Contains 9:   "); Serial.println(arr.contains(9) ? "yes" : "no");
  Serial.print("Contains 7:   "); Serial.println(arr.contains(7) ? "yes" : "no");
  Serial.print("Index of 6:   "); Serial.println(arr.indexOf(6));

  // fill
  arr.fill(0);
  Serial.println("After fill(0):");
  for (int i = 0; i < arr.size(); i++) {
    Serial.print(arr[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void loop() { /* nothing to loop */ }
