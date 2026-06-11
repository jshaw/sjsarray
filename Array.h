/*
||
|| @file Array.h
|| @version 1.4
|| @author Alexander Brevig (original, arduino.cc/playground/Code/Array)
|| @author Jordan Shaw (forked 2015, maintained 2015–2026, studiojordanshaw.com)
||
|| @description
|| | Provides a bounds-safe wrapper around raw C++ arrays with utility
|| | functions useful for sensor data and general Arduino sketches.
|| #
||
|| @license
|| |  This library is free software; you can redistribute it and/or
|| |  modify it under the terms of the GNU Lesser General Public
|| |  License as published by the Free Software Foundation; version
|| |  2.1 of the License.
|| |
|| |  This library is distributed in the hope that it will be useful,
|| |  but WITHOUT ANY WARRANTY; without even the implied warranty of
|| |  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
|| |  Lesser General Public License for more details.
|| |
|| |  You should have received a copy of the GNU Lesser General Public
|| |  License along with this library; if not, write to the Free Software
|| |  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
|| #
||
*/

#ifndef ARRAY_H
#define ARRAY_H

#include <Arduino.h>

template<typename type>
class Array {
    public:
        Array(type* newArray, int newSize) : array(newArray), arraySize(newSize) {}

        // Returns the number of elements in the array.
        int size() const {
            return arraySize;
        }

        // Returns the largest value in the array.
        type getMax() const {
            type max = array[0];
            for (int i = 1; i < arraySize; i++) {
                if (array[i] > max) max = array[i];
            }
            return max;
        }

        // Returns the index of the first occurrence of the largest value.
        int getMaxIndex() const {
            int maxIndex = 0;
            for (int i = 1; i < arraySize; i++) {
                if (array[i] > array[maxIndex]) maxIndex = i;
            }
            return maxIndex;
        }

        // Returns the smallest value in the array.
        type getMin() const {
            type min = array[0];
            for (int i = 1; i < arraySize; i++) {
                if (array[i] < min) min = array[i];
            }
            return min;
        }

        // Returns the index of the first occurrence of the smallest value.
        int getMinIndex() const {
            int minIndex = 0;
            for (int i = 1; i < arraySize; i++) {
                if (array[i] < array[minIndex]) minIndex = i;
            }
            return minIndex;
        }

        // Returns the sum of all elements.
        // Use a type wide enough to hold the total without overflow.
        type getSum() const {
            type sum = 0;
            for (int i = 0; i < arraySize; i++) sum += array[i];
            return sum;
        }

        // Returns the arithmetic mean.
        // For integer types this truncates toward zero — for decimal precision use:
        //   float avg = (float)arr.getSum() / arr.size();
        type getAverage() const {
            return getSum() / arraySize;
        }

        // Returns true if the array contains the given value.
        bool contains(type value) const {
            for (int i = 0; i < arraySize; i++) {
                if (array[i] == value) return true;
            }
            return false;
        }

        // Returns the index of the first occurrence of value, or -1 if not found.
        int indexOf(type value) const {
            for (int i = 0; i < arraySize; i++) {
                if (array[i] == value) return i;
            }
            return -1;
        }

        // Sets every element to the given value.
        void fill(type value) {
            for (int i = 0; i < arraySize; i++) array[i] = value;
        }

        // ── Sorting ────────────────────────────────────────────────────────────
        //
        // All three algorithms sort ascending by default. Pass false to sort
        // descending. Works with any type that supports < and > operators:
        // int, float, long, double, and Arduino String all work out of the box.
        // Do NOT use raw char* — use Arduino's String class for text sorting.

        // Sorts in-place using bubble sort.
        // O(n²) time, O(1) space. Safe on all boards including Uno.
        // Best for very small arrays or nearly-sorted data.
        void bubbleSort(bool ascending = true) {
            for (int i = 0; i < arraySize - 1; i++) {
                for (int j = 0; j < arraySize - i - 1; j++) {
                    bool shouldSwap = ascending ? array[j] > array[j + 1]
                                                : array[j] < array[j + 1];
                    if (shouldSwap) swap(array[j], array[j + 1]);
                }
            }
        }

        // Sorts in-place using quicksort. This is the default sort() algorithm.
        // O(n log n) average, O(log n) stack space for recursion.
        // Safe on all boards for typical array sizes.
        void quickSort(bool ascending = true) {
            quickSortHelper(0, arraySize - 1, ascending);
        }

        // Sorts in-place using merge sort.
        // O(n log n) time, stable (preserves original order of equal elements).
        // Allocates O(n) temporary heap memory — on Uno (2KB RAM) keep arrays small.
        // Prefer quickSort() on memory-constrained boards.
        void mergeSort(bool ascending = true) {
            mergeSortHelper(0, arraySize - 1, ascending);
        }

        // Sorts using quicksort ascending by default.
        void sort(bool ascending = true) {
            quickSort(ascending);
        }

        // ── Iterators ──────────────────────────────────────────────────────────

        // Support for range-based for loops: for (int v : myArray) { ... }
        type* begin() { return array; }
        type* end()   { return array + arraySize; }
        const type* begin() const { return array; }
        const type* end()   const { return array + arraySize; }

        // ── Element access ─────────────────────────────────────────────────────

        // Bounds-checked access — out-of-bounds indices clamp to first/last element.
        type& operator[](int index) {
            if (index >= arraySize) return array[arraySize - 1];
            if (index < 0)         return array[0];
            return array[index];
        }

        type operator[](int index) const {
            if (index >= arraySize) return array[arraySize - 1];
            if (index < 0)         return array[0];
            return array[index];
        }

    private:
        type* array;
        int arraySize;

        void swap(type& a, type& b) {
            type tmp = a;
            a = b;
            b = tmp;
        }

        // ── Quicksort internals ────────────────────────────────────────────────

        int partition(int lo, int hi, bool ascending) {
            type pivot = array[hi];
            int i = lo - 1;
            for (int j = lo; j < hi; j++) {
                bool condition = ascending ? array[j] <= pivot : array[j] >= pivot;
                if (condition) {
                    i++;
                    swap(array[i], array[j]);
                }
            }
            swap(array[i + 1], array[hi]);
            return i + 1;
        }

        void quickSortHelper(int lo, int hi, bool ascending) {
            if (lo >= hi) return;
            int p = partition(lo, hi, ascending);
            quickSortHelper(lo, p - 1, ascending);
            quickSortHelper(p + 1, hi, ascending);
        }

        // ── Merge sort internals ───────────────────────────────────────────────

        void merge(int lo, int mid, int hi, bool ascending) {
            int leftSize  = mid - lo + 1;
            int rightSize = hi - mid;

            type* left  = new type[leftSize];
            type* right = new type[rightSize];

            if (!left || !right) {
                delete[] left;
                delete[] right;
                return;
            }

            for (int i = 0; i < leftSize;  i++) left[i]  = array[lo + i];
            for (int i = 0; i < rightSize; i++) right[i] = array[mid + 1 + i];

            int i = 0, j = 0, k = lo;
            while (i < leftSize && j < rightSize) {
                bool pickLeft = ascending ? left[i] <= right[j] : left[i] >= right[j];
                array[k++] = pickLeft ? left[i++] : right[j++];
            }
            while (i < leftSize)  array[k++] = left[i++];
            while (j < rightSize) array[k++] = right[j++];

            delete[] left;
            delete[] right;
        }

        void mergeSortHelper(int lo, int hi, bool ascending) {
            if (lo >= hi) return;
            int mid = lo + (hi - lo) / 2;
            mergeSortHelper(lo, mid, ascending);
            mergeSortHelper(mid + 1, hi, ascending);
            merge(lo, mid, hi, ascending);
        }
};

#endif
