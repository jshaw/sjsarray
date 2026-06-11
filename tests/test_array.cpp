// Native unit tests for Array.h — compile and run with:
//   cd tests && g++ -std=c++11 -I. -I.. -o test_array test_array.cpp && ./test_array

#include <cstdio>
#include <cstring>
#include "../Array.h"

static int passed = 0;
static int failed = 0;

#define ASSERT_EQ(label, actual, expected)                              \
    do {                                                                \
        if ((actual) == (expected)) {                                   \
            printf("  PASS  %s\n", label);                             \
            passed++;                                                   \
        } else {                                                        \
            printf("  FAIL  %s  (got %d, expected %d)\n",              \
                   label, (int)(actual), (int)(expected));              \
            failed++;                                                   \
        }                                                               \
    } while (0)

#define ASSERT_TRUE(label, expr)   ASSERT_EQ(label, (expr) ? 1 : 0, 1)
#define ASSERT_FALSE(label, expr)  ASSERT_EQ(label, (expr) ? 1 : 0, 0)

// ── helpers ─────────────────────────────────────────────────────────────────

void section(const char* name) { printf("\n%s\n", name); }

// ── test groups ─────────────────────────────────────────────────────────────

void test_size() {
    section("size()");
    int raw[5] = {1, 2, 3, 4, 5};
    Array<int> a(raw, 5);
    ASSERT_EQ("size is 5", a.size(), 5);
}

void test_subscript() {
    section("operator[]");
    int raw[3] = {10, 20, 30};
    Array<int> a(raw, 3);
    ASSERT_EQ("index 0",  a[0], 10);
    ASSERT_EQ("index 2",  a[2], 30);
    ASSERT_EQ("clamp high", a[99], 30);   // clamps to last
    ASSERT_EQ("clamp low",  a[-1], 10);   // clamps to first
}

void test_subscript_const() {
    section("operator[] const");
    int raw[3] = {10, 20, 30};
    const Array<int> a(raw, 3);
    ASSERT_EQ("const index 1",  a[1], 20);
    ASSERT_EQ("const clamp high", a[99], 30);
    ASSERT_EQ("const clamp low",  a[-1], 10);
}

void test_max() {
    section("getMax / getMaxIndex");
    int raw[5] = {3, 1, 4, 1, 5};
    Array<int> a(raw, 5);
    ASSERT_EQ("max value",        a.getMax(),      5);
    ASSERT_EQ("max index",        a.getMaxIndex(), 4);

    // tie — first occurrence wins
    int tie[4] = {5, 2, 5, 1};
    Array<int> b(tie, 4);
    ASSERT_EQ("max tie: first index", b.getMaxIndex(), 0);
}

void test_min() {
    section("getMin / getMinIndex");
    int raw[5] = {3, 1, 4, 1, 5};
    Array<int> a(raw, 5);
    ASSERT_EQ("min value",        a.getMin(),      1);
    ASSERT_EQ("min index",        a.getMinIndex(), 1);

    // tie — first occurrence wins
    int tie[4] = {5, 1, 1, 9};
    Array<int> b(tie, 4);
    ASSERT_EQ("min tie: first index", b.getMinIndex(), 1);
}

void test_sum_average() {
    section("getSum / getAverage");
    int raw[5] = {1, 2, 3, 4, 5};
    Array<int> a(raw, 5);
    ASSERT_EQ("sum",          a.getSum(),     15);
    ASSERT_EQ("average int",  a.getAverage(), 3);  // 15/5 = 3

    // float average
    float fraw[3] = {1.0f, 2.0f, 3.0f};
    Array<float> fa(fraw, 3);
    ASSERT_EQ("float average", (int)(fa.getAverage() * 10), 20); // 2.0 * 10 = 20
}

void test_contains() {
    section("contains()");
    int raw[5] = {3, 1, 4, 1, 5};
    Array<int> a(raw, 5);
    ASSERT_TRUE ("contains 4",  a.contains(4));
    ASSERT_FALSE("contains 99", a.contains(99));
}

void test_indexOf() {
    section("indexOf()");
    int raw[5] = {3, 1, 4, 1, 5};
    Array<int> a(raw, 5);
    ASSERT_EQ("indexOf 4",  a.indexOf(4),  2);
    ASSERT_EQ("indexOf 1 (first)", a.indexOf(1), 1);   // returns first occurrence
    ASSERT_EQ("indexOf 99 not found", a.indexOf(99), -1);
}

void test_fill() {
    section("fill()");
    int raw[4] = {1, 2, 3, 4};
    Array<int> a(raw, 4);
    a.fill(7);
    ASSERT_EQ("fill [0]", a[0], 7);
    ASSERT_EQ("fill [3]", a[3], 7);
    ASSERT_EQ("fill size unchanged", a.size(), 4);
}

void test_range_for() {
    section("range-based for loop");
    int raw[4] = {1, 2, 3, 4};
    Array<int> a(raw, 4);
    int sum = 0;
    for (int v : a) sum += v;
    ASSERT_EQ("range-for sum", sum, 10);
}

void test_write_via_subscript() {
    section("write via operator[]");
    int raw[3] = {1, 2, 3};
    Array<int> a(raw, 3);
    a[1] = 99;
    ASSERT_EQ("write [1]", a[1], 99);
    ASSERT_EQ("unchanged [0]", a[0], 1);
}

void test_negative_values() {
    section("negative values");
    int raw[4] = {-5, -1, -3, -2};
    Array<int> a(raw, 4);
    ASSERT_EQ("min of negatives",       a.getMin(),      -5);
    ASSERT_EQ("min index of negatives", a.getMinIndex(),  0);
    ASSERT_EQ("max of negatives",       a.getMax(),      -1);
    ASSERT_EQ("max index of negatives", a.getMaxIndex(),  1);
}

void test_single_element() {
    section("single-element array");
    int raw[1] = {42};
    Array<int> a(raw, 1);
    ASSERT_EQ("size",      a.size(),         1);
    ASSERT_EQ("min",       a.getMin(),       42);
    ASSERT_EQ("max",       a.getMax(),       42);
    ASSERT_EQ("minIndex",  a.getMinIndex(),  0);
    ASSERT_EQ("maxIndex",  a.getMaxIndex(),  0);
    ASSERT_EQ("sum",       a.getSum(),       42);
    ASSERT_EQ("average",   a.getAverage(),   42);
    ASSERT_TRUE("contains", a.contains(42));
    ASSERT_EQ("indexOf",   a.indexOf(42),    0);
}

// ── sorting helpers ──────────────────────────────────────────────────────────

bool isSortedAsc(int* arr, int n) {
    for (int i = 0; i < n - 1; i++) if (arr[i] > arr[i + 1]) return false;
    return true;
}
bool isSortedDesc(int* arr, int n) {
    for (int i = 0; i < n - 1; i++) if (arr[i] < arr[i + 1]) return false;
    return true;
}

// ── sorting tests ─────────────────────────────────────────────────────────────

void test_bubble_sort() {
    section("bubbleSort()");

    int raw[5] = {5, 3, 1, 4, 2};
    Array<int> a(raw, 5);
    a.bubbleSort();
    ASSERT_TRUE("ascending", isSortedAsc(raw, 5));
    ASSERT_EQ("first element", raw[0], 1);
    ASSERT_EQ("last element",  raw[4], 5);

    int raw2[5] = {5, 3, 1, 4, 2};
    Array<int> b(raw2, 5);
    b.bubbleSort(false);
    ASSERT_TRUE("descending", isSortedDesc(raw2, 5));
    ASSERT_EQ("first desc", raw2[0], 5);

    // already sorted
    int raw3[4] = {1, 2, 3, 4};
    Array<int> c(raw3, 4);
    c.bubbleSort();
    ASSERT_TRUE("already sorted", isSortedAsc(raw3, 4));

    // duplicates
    int raw4[5] = {3, 1, 3, 1, 3};
    Array<int> d(raw4, 5);
    d.bubbleSort();
    ASSERT_TRUE("duplicates sorted", isSortedAsc(raw4, 5));

    // single element
    int raw5[1] = {42};
    Array<int> e(raw5, 1);
    e.bubbleSort();
    ASSERT_EQ("single element unchanged", raw5[0], 42);
}

void test_quick_sort() {
    section("quickSort()");

    int raw[6] = {9, 2, 7, 1, 8, 3};
    Array<int> a(raw, 6);
    a.quickSort();
    ASSERT_TRUE("ascending", isSortedAsc(raw, 6));
    ASSERT_EQ("first element", raw[0], 1);
    ASSERT_EQ("last element",  raw[5], 9);

    int raw2[6] = {9, 2, 7, 1, 8, 3};
    Array<int> b(raw2, 6);
    b.quickSort(false);
    ASSERT_TRUE("descending", isSortedDesc(raw2, 6));

    // reverse sorted (quicksort worst-case input)
    int raw3[5] = {5, 4, 3, 2, 1};
    Array<int> c(raw3, 5);
    c.quickSort();
    ASSERT_TRUE("reverse sorted input", isSortedAsc(raw3, 5));

    // duplicates
    int raw4[5] = {2, 2, 1, 2, 1};
    Array<int> d(raw4, 5);
    d.quickSort();
    ASSERT_TRUE("duplicates", isSortedAsc(raw4, 5));
}

void test_merge_sort() {
    section("mergeSort()");

    int raw[6] = {4, 2, 6, 1, 5, 3};
    Array<int> a(raw, 6);
    a.mergeSort();
    ASSERT_TRUE("ascending", isSortedAsc(raw, 6));
    ASSERT_EQ("first element", raw[0], 1);
    ASSERT_EQ("last element",  raw[5], 6);

    int raw2[6] = {4, 2, 6, 1, 5, 3};
    Array<int> b(raw2, 6);
    b.mergeSort(false);
    ASSERT_TRUE("descending", isSortedDesc(raw2, 6));

    // stable sort — equal elements preserve original relative order
    int raw3[6] = {3, 1, 2, 1, 3, 2};
    Array<int> c(raw3, 6);
    c.mergeSort();
    ASSERT_TRUE("stable sort result sorted", isSortedAsc(raw3, 6));
}

void test_sort_default() {
    section("sort() — default (quicksort ascending)");
    int raw[5] = {5, 3, 1, 4, 2};
    Array<int> a(raw, 5);
    a.sort();
    ASSERT_TRUE("ascending", isSortedAsc(raw, 5));

    int raw2[5] = {5, 3, 1, 4, 2};
    Array<int> b(raw2, 5);
    b.sort(false);
    ASSERT_TRUE("descending", isSortedDesc(raw2, 5));
}

void test_sort_float() {
    section("sort() with float");
    float raw[4] = {3.14f, 1.0f, 2.71f, 0.5f};
    Array<float> a(raw, 4);
    a.sort();
    bool sorted = raw[0] <= raw[1] && raw[1] <= raw[2] && raw[2] <= raw[3];
    ASSERT_TRUE("float sorted ascending", sorted);
}

// ── main ────────────────────────────────────────────────────────────────────

int main() {
    test_size();
    test_subscript();
    test_subscript_const();
    test_max();
    test_min();
    test_sum_average();
    test_contains();
    test_indexOf();
    test_fill();
    test_range_for();
    test_write_via_subscript();
    test_negative_values();
    test_single_element();
    test_bubble_sort();
    test_quick_sort();
    test_merge_sort();
    test_sort_default();
    test_sort_float();

    printf("\n%d passed, %d failed\n", passed, failed);
    return failed > 0 ? 1 : 0;
}
