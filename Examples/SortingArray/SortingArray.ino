#include <Array.h>

const byte SIZE = 8;
int raw[SIZE] = {64, 25, 12, 22, 11, 90, 3, 47};

void printArray(Array<int>& arr) {
  for (int v : arr) {
    Serial.print(v);
    Serial.print(" ");
  }
  Serial.println();
}

void resetArray(int* dst, const int* src, int n) {
  for (int i = 0; i < n; i++) dst[i] = src[i];
}

void setup() {
  Serial.begin(9600);

  const int original[SIZE] = {64, 25, 12, 22, 11, 90, 3, 47};

  // ── Bubble Sort ────────────────────────────────────────────────────────────
  // Simple and safe. Best for small arrays or nearly-sorted data.
  // Uses no extra memory — always safe on Uno and other AVR boards.
  resetArray(raw, original, SIZE);
  Array<int> arr(raw, SIZE);

  Serial.print("Original:              "); printArray(arr);

  arr.bubbleSort();
  Serial.print("bubbleSort ascending:  "); printArray(arr);

  arr.bubbleSort(false);
  Serial.print("bubbleSort descending: "); printArray(arr);

  // ── Quick Sort ─────────────────────────────────────────────────────────────
  // Fast general-purpose sort. This is what sort() uses internally.
  // O(n log n) average, no extra heap memory needed.
  resetArray(raw, original, SIZE);

  arr.quickSort();
  Serial.print("quickSort ascending:   "); printArray(arr);

  resetArray(raw, original, SIZE);
  arr.quickSort(false);
  Serial.print("quickSort descending:  "); printArray(arr);

  // ── Merge Sort ─────────────────────────────────────────────────────────────
  // Stable sort — equal elements keep their original relative order.
  // Allocates temporary heap memory (O(n)) during sorting.
  // On Uno (2KB RAM) keep arrays small. Prefer quickSort() when RAM is tight.
  resetArray(raw, original, SIZE);

  arr.mergeSort();
  Serial.print("mergeSort ascending:   "); printArray(arr);

  // ── sort() ─────────────────────────────────────────────────────────────────
  // Default sort — uses quickSort. Recommended for most use cases.
  resetArray(raw, original, SIZE);

  arr.sort();
  Serial.print("sort() ascending:      "); printArray(arr);

  // ── Sorting strings ────────────────────────────────────────────────────────
  // Use Arduino's String class (not raw char*) for alphabetical sorting.
  String words[5] = {"Zebra", "Apple", "Mango", "Cherry", "Banana"};
  Array<String> strArr(words, 5);

  Serial.print("Strings original:      ");
  for (String s : strArr) { Serial.print(s); Serial.print(" "); }
  Serial.println();

  strArr.sort();
  Serial.print("Strings sorted:        ");
  for (String s : strArr) { Serial.print(s); Serial.print(" "); }
  Serial.println();
}

void loop() { /* nothing to loop */ }
