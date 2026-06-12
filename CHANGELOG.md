# Changelog

All notable changes to the SJSArray library are documented here.

---

## [1.5.1] — 2026-06-12

### Changed
- `library.properties` author field now lists Jordan Shaw (current author/maintainer). Alexander Brevig remains credited as the original author in the source header and README.

---

## [1.5.0] — 2026-06-11

### Changed
- **Breaking:** library renamed `Array` → `SJSArray` for the Arduino Library Manager (the generic `Array` name risked collisions).
- **Breaking:** header renamed `Array.h` → `SJSArray.h`. Update sketches from `#include <Array.h>` to `#include <SJSArray.h>`.
- **Breaking:** class renamed `Array<type>` → `SJSArray<type>`. Update declarations accordingly.
- Examples folder renamed `Examples/` → `examples/` to match the Arduino Library Specification.
- Include guard renamed `ARRAY_H` → `SJSARRAY_H`; `keywords.txt`, examples, tests, and README updated for the new names.

---

## [1.4.0] — 2026-06-11

### Added
- `sort(ascending)` — default sort using quicksort
- `bubbleSort(ascending)` — O(n²), zero extra memory, safe on all boards
- `quickSort(ascending)` — O(n log n) in-place, O(log n) stack
- `mergeSort(ascending)` — stable O(n log n), allocates O(n) temporary heap memory
- All sort methods accept an optional `bool` to sort descending
- All sort methods work with any type supporting `<` and `>` (int, float, String, etc.)
- 22 new sort tests added to the native test suite (63 total)

---

## [1.3.0] — 2026-06-09

### Added
- `getSum()` — returns the sum of all elements
- `contains(value)` — returns `true` if the array holds the given value
- `indexOf(value)` — returns the first index of a value, or `-1` if not found
- `fill(value)` — sets every element to a given value
- `begin()` / `end()` — enables range-based `for` loops (`for (int v : arr)`)
- `library.properties` — required metadata for the Arduino Library Manager
- `.gitignore` — excludes build artifacts and compiled binaries
- Native test suite (`tests/test_array.cpp`) — 41 tests, runs on desktop with `g++`

### Fixed
- `getMaxIndex()` and `getMinIndex()` returned the wrong type (`type` instead of `int`)
- Tie-breaking in `getMaxIndex`/`getMinIndex` used `<=`/`>=`, returning the *last* match — now returns the *first*
- `const operator[]` returned `0` for out-of-bounds access, inconsistent with the non-const version — now clamps to first/last element
- All read-only methods now correctly marked `const`

### Changed
- Example updated to `.ino` (replaces legacy `.pde` format)
- Example updated to demonstrate all functions including range-based for loop
- Removed stale `applet/` build artifacts from the repository
- README rewritten with correct install path and full API reference

---

## [1.2.0]

### Added
- `getMinIndex()` — returns the index of the minimum value
- `getMaxIndex()` — returns the index of the maximum value

---

## [1.1.0]

### Fixed
- Updated library to compile with Arduino IDE 1.6.5+

---

## [1.0.0] — Original release by Alexander Brevig

### Added
- `Array<type>` template wrapper for raw C arrays
- Bounds-checked `operator[]`
- `size()`, `getMin()`, `getMax()`, `getAverage()`
