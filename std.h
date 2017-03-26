#pragma once

#include <fstream>
#include <cstring>
#include <climits>
#include <vector>
#include <cmath>

#define die(...) do { printf(__VA_ARGS__); puts(""); exit(1); } while (0)

const int neginf = INT_MIN, inf = INT_MAX;

static int rand_in_range(int min, int max) {
  return min + rand() % (max - min + 1);
}

template <typename T>
class gen_array {
  T *_data;
public:
  size_t length;
  gen_array() : _data(nullptr), length(0) {}
  gen_array(size_t n_length) try : length(n_length) {
    _data = new T [length];
  } catch (...) {
    die("gen_array: failed to allocate memory for constructor");
  }
  gen_array(std::initializer_list<T> l) try : length(l.size()) {
    _data = new T [length];
    size_t i = 0;
    for (const auto &e : l)
      _data[i++] = e;
  } catch (...) {
    die("gen_array: failed to allocate memory from initializer list");
  }
  gen_array(const gen_array &other) {
    operator=(other);
  }
  ~gen_array() {
    if (!_data)
      delete [] _data;
  }
  gen_array& operator=(const gen_array &other) {
    try {
      length = other.length;
      if (!_data)
        delete [] _data;
      _data = new T [length];
      std::memcpy(_data, other._data, other.length * sizeof(T));
    } catch (...) {
      die("gen_array: failed to allocate memory for assignment copy");
    }
    return *this;
  }
  T& operator[](size_t i) {
    if (i >= 1 && i <= length)
      return _data[i - 1];
    else
      die("gen_array: indexing array out of bounds (%d)", (int)i);
  }
  void randomize(int min, int max) {
    for (size_t i = 0; i < length; i++)
      _data[i] = rand_in_range(min, max);
  }
  void randomize() {
    randomize(1, 30);
  }
  void randomize_walk(int min, int max) {
    int prev = 0;
    for (size_t i = 0; i < length; i++) {
      _data[i] = prev + rand_in_range(min, max);
      prev = _data[i];
    }
  }
  void randomize_walk() {
    randomize_walk(-3, 5);
  }
  void print() {
    printf("[");
    for (size_t i = 0; i < length; i++)
      printf("%d%s", _data[i], i == length - 1 ? "" : " ");
    printf("]\n");
  }
};

typedef gen_array<int> array;

// maybe value
struct mvalue {
  int v;
  bool nil;
  mvalue() : nil(true) {}
  mvalue& operator=(int n_v) {
    v = n_v;
    nil = false;
    return *this;
  }
  void print() {
    if (nil)
      puts("nil");
    else
      printf("%d\n", v);
  }
};

struct subarray {
  int start, end, sum;
};

template <typename T>
class gen_matrix {
  T *_data;
public:
  size_t rows, columns;
  gen_matrix() : _data(nullptr), rows(0) {};
  gen_matrix(size_t n_rows, size_t n_columns) try
    : rows(n_rows)
    , columns(n_columns) {
    _data = new T [rows * columns];
  } catch (...) {
    die("gen_matrix: failed to allocate memory for constructor");
  }
  gen_matrix(size_t n_rows, size_t n_columns, std::initializer_list<T> l) try
    : rows(n_rows)
    , columns(n_columns) {
    _data = new T [rows * columns];
    size_t i = 0;
    for (const auto &e : l)
      _data[i++] = e;
  } catch (...) {
    die("gen_matrix: failed to allocate memory for initializer list");
  }
  gen_matrix(const gen_matrix &other) {
    operator=(other);
  }
  ~gen_matrix() {
    if (!_data)
      delete [] _data;
  }
  gen_matrix& operator=(gen_matrix other) {
    try {
      rows = other.rows;
      columns = other.columns;
      if (!_data)
        delete [] _data;
      _data = new T [rows * columns];
      std::memcpy(_data, other._data, other.rows * other.columns * sizeof(T));
    } catch (...) {
      die("gen_matrix: failed to allocate memory for assignment copy");
    }
    return *this;
  }
  T& at(size_t y, size_t x) {
    if (y >= 1 && y <= rows && x >= 1 && x <= columns)
      return _data[(y - 1) * columns + (x - 1)];
    else
      die("gen_matrix: indexing matrix out of bounds (%d, %d)", (int)y, (int)x);
  }
  void randomize(int min, int max) {
    for (size_t y = 1; y <= rows; ++y)
      for (size_t x = 1; x <= columns; ++x)
        at(y, x) = rand_in_range(min, max);
  }
  void randomize() {
    randomize(1, 50);
  }
  void print() {
    for (size_t y = 1; y <= rows; ++y) {
      for (size_t x = 1; x <= columns; ++x)
        if (at(y, x) == inf)
          printf("inf\t");
        else
          printf("%d\t", at(y, x));
      printf("\n");
    }
  }
};

typedef gen_matrix<int> matrix;

template <typename T>
class gen_heap {
protected:
  gen_array<T> _data;
  int _print(size_t idx, bool is_left, int offset, int depth, std::vector<std::string> &s) {
    if (idx > heap_size)
      return 0;
    std::string node_info = std::to_string(_data[idx]);
    int node_info_w = (int)node_info.size();

    int left_w = _print(left(idx), 1, offset, depth + 1, s);
    int right_w = _print(right(idx), 0, offset + left_w + node_info_w, depth + 1, s);

    for (int i = 0; i < node_info_w; i++)
      s[2 * depth][offset + left_w + i] = node_info[i];

    if (depth && is_left) {
      for (int i = 0; i < node_info_w + right_w - 1; i++)
        s[2 * depth - 2][offset + left_w + node_info_w/2 + i + 1] = '_';
      s[2 * depth - 1][offset + left_w + node_info_w/2] = '/';
    } else if (depth && !is_left) {
      for (int i = 0; i < left_w + node_info_w - 3; i++)
        s[2 * depth - 2][offset - node_info_w/2 + i + 1] = '_';
      s[2 * depth - 1][offset + left_w + node_info_w/2 - 1] = '\\';
    }

    return left_w + node_info_w + right_w;
  }
public:
  size_t heap_size;
  gen_heap() : heap_size(0) {}
  gen_heap(const gen_array<T> &A) : _data(A), heap_size(A.length) {
    build_max_heap();
  }
  gen_heap(std::initializer_list<T> l) : _data(l), heap_size(l.size()) {
    build_max_heap();
  }
  size_t parent(size_t i) {
    return i / 2;
  }
  size_t left(size_t i) {
    return 2 * i;
  }
  size_t right(size_t i) {
    return 2 * i + 1;
  }
  void max_heapify_rec(size_t i) {
    size_t l = left(i), r = right(i), largest;
    if (l <= heap_size && _data[l] > _data[i])
      largest = l;
    else
      largest = i;
    if (r <= heap_size && _data[r] > _data[largest])
      largest = r;
    if (largest != i) {
      T t = _data[i];
      _data[i] = _data[largest];
      _data[largest] = t;
      max_heapify(largest);
    }
  }
  void max_heapify(size_t i) {
    while (i < heap_size) {
      size_t l = left(i), r = right(i), largest;
      if (l <= heap_size && _data[l] > _data[i])
        largest = l;
      else
        largest = i;
      if (r <= heap_size && _data[r] > _data[largest])
        largest = r;
      if (largest != i) {
        T t = _data[i];
        _data[i] = _data[largest];
        _data[largest] = t;
        i = largest;
      } else
        break;
    }
  }
  void build_max_heap() {
    for (size_t i = heap_size / 2; i >= 1; --i)
      max_heapify(i);
  }
  void heapsort() {
    for (size_t i = heap_size; i >= 2; --i) {
      std::swap(_data[1], _data[i]);
      --heap_size;
      max_heapify(1);
    }
  }
  size_t height() {
    if (heap_size == 0)
      return 0;
    else
      return std::floor(std::log2(heap_size));
  }
  size_t depth() {
    size_t i = 1, depth = 0;
    while (i <= heap_size) {
      i = left(i);
      ++depth;
    }
    return depth;
  }
  void print(size_t width = 80) {
    _data.print();
    std::vector<std::string> s(depth() * 2 - 1, std::string(width, ' '));
    _print(1, 0, 0, 0, s);
    for (size_t i = 0; i < depth() * 2 - 1; ++i)
      printf("%s\n", s[i].c_str());
  }
};

typedef gen_heap<int> heap;

template <typename T>
class gen_max_priority_queue : public gen_heap<T> {
public:
  T maximum() {
    return this->_data[1];
  }
  T extract_max() {
    if (this->heap_size < 1)
      die("heap underflow");
    T max = this->_data[1];
    this->_data[1] = this->_data[this->heap_size];
    --this->heap_size;
    this->max_heapify(1);
    return max;
  }
  void increase_key(size_t i, T k) {
    if (k < this->_data[i])
      die("new key is smalller than current");
    this->_data[i] = k;
    while (i > 1 && this->_data[this->parent(i)] < this->_data[i]) {
      std::swap(this->_data[i], this->_data[this->parent(i)]);
      i = this->parent(i);
    }
  }
  void insert(T k) {
    ++this->heap_size;
    this->_data[this->heap_size] = neginf;
    this->increase_key(this->heap_size, k);
  }
};

typedef gen_max_priority_queue<int> max_priority_queue;

template <typename T>
class gen_young_tableau {
  gen_matrix<T> _data;
public:
  size_t records;
  gen_young_tableau(size_t rows, size_t columns)
    : _data(rows, columns)
    , records(0) {
    for (size_t y = 1; y <= rows; ++y)
      for (size_t x = 1; x <= columns; ++x)
        _data.at(y, x) = inf;
  }
  gen_young_tableau(size_t rows, size_t columns, gen_array<T> array_elems)
    : gen_young_tableau(rows, columns) {
    gen_array<T> array_copy = array_elems;
    insertion_sort(array_copy);
    for (size_t i = 1, y = 1; y <= rows; ++y)
      for (size_t x = 1; x <= columns; ++x)
        _data.at(y, x) = i <= array_copy.length ? array_copy[i++] : inf;
  }
  bool empty() {
    return _data.at(1, 1) == inf;
  }
  bool full() {
    return _data.at(_data.rows, _data.columns) < inf;
  }
  T extract_min() {
    T val = _data.at(1, 1);
    _data.at(1, 1) = inf;
    youngify(1, 1);
    return val;
  }
  void youngify(size_t y, size_t x) {
    size_t smallest_y = y, smallest_x = x;
    if (y + 1 <= _data.rows && _data.at(y + 1, x) < _data.at(y, x)) {
      smallest_y = y + 1;
      smallest_x = x;
    }
    if (x + 1 <= _data.columns && _data.at(y, x + 1) < _data.at(y, x)) {
      smallest_y = y;
      smallest_x = x + 1;
    }
    if (smallest_y != y || smallest_x != x) {
      std::swap(_data.at(y, x), _data.at(smallest_y, smallest_x));
      youngify(smallest_y, smallest_x);
    }
  }
  void insert(T k) {
    // decrease_key(rows, columns, k);
    _data.at(_data.rows, _data.columns) = k;
  }
  void decrease_key(size_t i, size_t j, T k) {
    if (_data.at(i, j) <= k)
      die("error");
    _data.at(i, j) = k;
    T threshold = inf;
    size_t largest_i = i, largest_j = j;
    while ((i > 1 || j > 1) && _data.at(i, j) < threshold) {
      std::swap(_data.at(i, j), _data.at(largest_i, largest_j));
      i = largest_i;
      j = largest_j;
      if (i - 1 >= 1 && _data.at(i, j) < _data.at(i - 1, j)) {
        largest_i = i - 1;
        largest_j = j;
      }
      if (j - 1 >= 1 && _data.at(i, j) < _data.at(i, j - 1)) {
        largest_i = i;
        largest_j = j - 1;
      }
      threshold = _data.at(largest_i, largest_j);
    }
  }
  void youngify_rev(size_t y, size_t x) {
    size_t largest_y = y, largest_x = x;
    if (y - 1 >= 1 && _data.at(y, x) < _data.at(y - 1, x)) {
      largest_y = y - 1;
      largest_x = x;
    }
    if (x - 1 >= 1 && _data.at(y, x) < _data.at(y, x - 1)) {
      largest_y = y;
      largest_x = x - 1;
    }
    if (largest_y != y || largest_x != x) {
      std::swap(_data.at(y, x), _data.at(largest_y, largest_x));
      youngify_rev(largest_y, largest_x);
    }
  }
  bool search(T k) {
    size_t y = 1, x = _data.columns;
    while (1) {
      T element = _data.at(y, x);
      if (element == k)
        return true;
      else if (element > k) {
        if (x > 1)
          --x;
        else
          return false;
      } else if (element < k) {
        if (y < _data.rows)
          ++y;
        else
          return false;
      }
    }
  }
  void print() {
    _data.print();
  }
};

typedef gen_young_tableau<int> young_tableau;

struct interval {
  float l, r;
};

mvalue search(array &A, int v);
void insertion_sort(array &A);
void selection_sort(array &A);
void merge_sort(array &A);
mvalue bsearch(array &A, int v);
bool sum_exists(array &S, int x);
int inversions(array &A);
subarray find_maximum_subarray(array &A);
subarray find_maximum_subarray_lin(array &A);
matrix square_matrix_mult(matrix &A, matrix &B);
void randomize_in_place(array &A);
int coin_streak(int n);
void log2_benchmark();
void heapsort(array &A);
void quicksort(array &A);
void randomized_quicksort(array &A);
// void fuzzy_sort(gen_array<interval> &A);
void counting_sort(array &A);
void preprocess_count_integers_in_range(array &A, std::vector<int> &C, int &max);
int count_integers_in_range(array &A, array &C, int max, int a, int b, int &n);

