# Arduino Array Library

A bounds-safe wrapper for raw C++ arrays that adds utility functions useful for sensor data and general Arduino sketches. Originally by Alexander Brevig; updated and extended to work with modern Arduino IDE (1.6+).

---

## Why use this?

Raw C arrays on Arduino have two problems: they don't carry their own size, and out-of-bounds access silently corrupts memory. This library wraps a raw array to give you:

- Bounds-checked `[]` access (clamps instead of corrupting memory)
- A reliable `size()` call — no more passing length separately
- Common operations: min, max, sum, average, search, fill
- Range-based `for` loop support

---

## Compatibility

Works on any Arduino-compatible board. The library is header-only and uses only standard C++11, so it compiles cleanly across all architectures.

| Board | Architecture | Notes |
|-------|-------------|-------|
| Uno, Nano, Mega | AVR 8-bit | 2KB RAM — watch for `getSum()` overflow on large arrays of large values |
| ESP32 | Xtensa LX6/LX7 | 320KB RAM, full C++11 — no practical overflow concern |
| Teensy 4.x | ARM Cortex-M7 | 1MB RAM, 600MHz — same as ESP32 |
| Due, Zero, MKR | ARM Cortex-M | Full C++11 support |

The library is most valuable on AVR boards where `std::array` and `std::vector` aren't available. On ESP32 and ARM boards it works just as well and gives you the same consistent API across all your projects.

---

## Install

### Arduino Library Manager *(recommended)*

1. Open the Arduino IDE
2. Go to **Sketch → Include Library → Manage Libraries...**
3. Search for **Array**
4. Click **Install**

---

### Manual ZIP install

1. Download the latest release as a ZIP from the [Releases](../../releases) page
2. In the Arduino IDE go to **Sketch → Include Library → Add .ZIP Library...**
3. Select the downloaded ZIP

---

### Manual folder install

1. Download or clone this repository
2. Copy the `Array` folder into your Arduino libraries directory:
   - **macOS / Linux:** `~/Documents/Arduino/libraries/`
   - **Windows:** `Documents\Arduino\libraries\`
3. Restart the Arduino IDE

---

Once installed, add to any sketch with:

```cpp
#include <Array.h>
```

---

## Quick start

```cpp
#include <Array.h>

int raw[5] = {3, 1, 4, 1, 5};
Array<int> arr(raw, 5);

void setup() {
  Serial.begin(9600);
  Serial.println(arr.getMax());      // 5
  Serial.println(arr.getMaxIndex()); // 4
  Serial.println(arr.getMin());      // 1
  Serial.println(arr.getMinIndex()); // 1
  Serial.println(arr.getSum());      // 14
  Serial.println(arr.getAverage());  // 2
}
```

---

## API Reference

### Construction

```cpp
Array<datatype> name(rawArray, size);
```

| Parameter | Description |
|-----------|-------------|
| `rawArray` | Pointer to an existing C array |
| `size`     | Number of elements |

The Array object does **not** copy the data — it wraps the original array in place.

---

### `size()`
Returns the number of elements.
```cpp
int n = arr.size();
```

---

### `getMin()` / `getMax()`
Returns the smallest or largest value in the array.
```cpp
int lo = arr.getMin();
int hi = arr.getMax();
```

---

### `getMinIndex()` / `getMaxIndex()`
Returns the **index** of the first occurrence of the minimum or maximum value.
```cpp
int loAt = arr.getMinIndex();
int hiAt = arr.getMaxIndex();
```

---

### `getSum()`
Returns the sum of all elements. Use a type wide enough to hold the total without overflow (e.g., `Array<long>` if your values are large).
```cpp
long total = arr.getSum();
```

---

### `getAverage()`
Returns the arithmetic mean. For integer array types this truncates toward zero — if you need decimal precision, cast manually:
```cpp
float avg = (float)arr.getSum() / arr.size();
```

---

### `contains(value)`
Returns `true` if the array contains the given value.
```cpp
if (arr.contains(9)) { ... }
```

---

### `indexOf(value)`
Returns the index of the **first** occurrence of `value`, or `-1` if not found.
```cpp
int pos = arr.indexOf(6);  // -1 if not found
```

---

### `sort(ascending = true)`
Sorts the array using quicksort. This is the recommended default for most use cases.
```cpp
arr.sort();         // ascending
arr.sort(false);    // descending
```
Works with any type that supports `<` and `>`: `int`, `float`, `long`, `double`, and Arduino `String`. Do **not** use raw `char*` — use `String` for text sorting.

---

### `bubbleSort(ascending = true)`
Simple O(n²) sort. Uses no extra memory and minimal stack space — always safe on all boards including Uno. Best for very small arrays or nearly-sorted data.
```cpp
arr.bubbleSort();
arr.bubbleSort(false); // descending
```

---

### `quickSort(ascending = true)`
In-place O(n log n) average sort with O(log n) recursion stack. The algorithm behind `sort()`. Good general-purpose choice.
```cpp
arr.quickSort();
arr.quickSort(false); // descending
```

---

### `mergeSort(ascending = true)`
Stable O(n log n) sort — equal elements preserve their original relative order. Allocates O(n) temporary heap memory per merge step. On an Uno (2KB RAM) keep arrays small; prefer `quickSort()` on memory-constrained boards.
```cpp
arr.mergeSort();
arr.mergeSort(false); // descending
```

| Algorithm | Time | Extra RAM | Stack | Stable |
|---|---|---|---|---|
| `bubbleSort` | O(n²) | None | Minimal | Yes |
| `quickSort` | O(n log n) avg | None | O(log n) | No |
| `mergeSort` | O(n log n) | O(n) heap | Minimal | Yes |

---

### `fill(value)`
Sets every element to `value`.
```cpp
arr.fill(0);
```

---

### `operator[]`
Bounds-checked element access. Out-of-bounds indices are clamped to the first or last element rather than accessing invalid memory.
```cpp
arr[2]     // element at index 2
arr[0] = 7 // write to element 0
```

---

### Range-based `for` loop
```cpp
for (int v : arr) {
  Serial.println(v);
}
```

---

## Running the tests

The library includes a native test suite that runs on your computer (no hardware needed):

```bash
cd tests
g++ -std=c++11 -I. -I.. -o test_array test_array.cpp && ./test_array
```

---

## Changelog

See [CHANGELOG.md](CHANGELOG.md) for release history.

---

## License

GNU Lesser General Public License v2.1

Originally created by Alexander Brevig (arduino.cc/playground/Code/Array).  
Forked and maintained by [Studio Jordan Shaw](https://studiojordanshaw.com) © 2015–2026.
