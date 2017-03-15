#include <iostream>
#include <cstdarg>
#include <string>
#include <stdexcept>
#include <math.h>
#include <assert.h>

class vec {
public:
    vec(int size) : allocsize_(0), size_(0) {
        realloc(size);
    }

    vec(std::initializer_list<int> lst) : allocsize_(0), size_(0)
    {
        realloc(lst.size());
        std::copy(lst.begin(), lst.end(), arr_);
        size_ = lst.size();
    }

    vec(const std::string &str) : allocsize_(0), size_(0) {
        realloc(str.length());
        size_ = str.length();

        for (int i = 0; i < size_; i++) {
            arr_[i] = (int)str[i];
        }
    }

    //Copy constructor
    vec(const vec& other) :allocsize_(0), size_(0) {
        realloc(other.allocsize_);
        size_ = other.size_;

        for (int i = 0; i < size_; i++) {
            arr_[i] = other.arr_[i];
        }
    }

    //Move Constructor
    vec(vec&& v)
        :allocsize_{v.allocsize_},  // Grab the elements from v
        size_{v.size_},
        arr_{v.arr_}
    {
        v.arr_ = nullptr;  // Now v has no elements
        v.size_ = 0;
        v.allocsize_ = 0;
    }

    ~vec();


    //Generators
    static vec range(int i);
    //static vec range(int a, int b);
    //static vec range(int a, int step, int b);

    std::string str() const;
    std::string as_str() const;
    int size() const;
    int sum() const;
    //int prod() const;
    //vec cumsum() const;
    //vec cummprod() const;
    //double mean() const;
    //int median() const;
    //int mode() const;

    //double regression(const vec& a, const vec& b);


    int head() const;
    vec head(int items) const;
    vec head(int items, int overtake) const;

    void apply(auto fn);
    //vec cumul(auto fn);

    void realloc(int s);
    void resize(int size, int dflt);
    void resize(int size);
    void clear();
    void append(int x);
    //void append(const vec& v);
    void swap(int i, int j);
    void reverse();

    int& operator[](int i);
    const int& operator[](int i) const;
    int get(int i) const;

    int dot_prod(const vec &v1, const vec &v2) const;

    friend std::ostream& operator<<(std::ostream&, const vec&);

    friend vec operator+(const vec& v, int n);
    friend vec operator+(int n, const vec& v);
    friend vec operator+(const vec& v1, const vec& v2);

    friend vec operator*(const vec& v, int n);
    friend vec operator*(int n, const vec& v);
    friend vec operator*(const vec& v1, const vec& v2);

    vec& operator=(const vec& v);

private:
    int* arr_;
    int size_;
    int allocsize_;
};



void print(const vec& v) {std::cout << v << std::endl;};
void print(int n) {std::cout << n << std::endl;};
void print(const std::string &str) {std::cout << str << std::endl;}


vec::~vec() {
    delete[] arr_;
}

int vec::head() const {
    if (size_ == 0) {
        throw std::out_of_range("head(): vec has no elements");
    }
    return arr_[0];
}

vec vec::head(int items) const {
    return head(items, 0);
}

vec vec::head(int items, int overtake) const {
    vec v = vec(items);
    v.size_ = items;

    //Overtake
    if (items > size_) {
        for (int i = 0; i < size_; i++) {
            v.arr_[i] = arr_[i];
        }
        for (int i = size_; i < items; i++) {
            v.arr_[i] = overtake;
        }
    } else {
        for (int i = 0; i < items; i++) {
            v.arr_[i] = arr_[i];
        }
    }

    return v;
}

int vec::size() const {
    return size_;
}

vec vec::range(int n) {
    vec v = vec(n);
    for (int i = 0; i <= n; i++) {
        v.append(i);
    }
    return v;
}

int vec::sum() const {
    int total = 0;
    for (int i = 0; i < size_; i++) {
        total += arr_[i];
    }
    return total;
}

void vec::clear() {
    realloc(0);
}

void vec::resize(int size, int dflt) {
    if (size == size_) {
        return;
    }
    //Making it smaller
    else if (size < size_) {
        realloc(size);
    }

    //Making it bigger
    else {
        int oldsize = size_;
        realloc(size);
        for (int i = oldsize; i < size; i++) {
            arr_[i] = dflt;
        }
        size_ = size;
    }
}

void vec::resize(int size) {
    resize(size, 0);
}

void vec::realloc(int s) {

    //No change in array size
    if (s == allocsize_) {
        return;
    }

    int* newarr = new int[s];

    //We are making the array smaller
    if (s < allocsize_) {
        for (int i = 0; i < s; i++) {
            newarr[i] = arr_[i];
        }
        size_ = s;
    }
    //We are making the array bigger
    else {
        for (int i = 0; i < allocsize_; i++) {
            newarr[i] = arr_[i];
        }
        // Overtake: default value is 0
        for (int i = allocsize_; i < s; i++) {
            newarr[i] = 0;
        }
    }

    //If there is data in the array, free the memory
    if (allocsize_ > 0) {
        delete[] arr_;
    }

    allocsize_ = s;
    arr_ = newarr;
}

void vec::append(int i)  {
    //Do we need to increase the vector array size?
    if (size_+1 > allocsize_) {
        realloc(size_+1);
    }

    arr_[size_] = i;
    size_++;
}

void vec::swap(int i, int j) {
    //Bounds check
    if (!(i >= 0 && i < size_ && j >= 0 && j < size_)) {
        throw std::out_of_range("vec::swap() bounds error");
    }

    int tmp = arr_[i];
    arr_[i] = arr_[j];
    arr_[j] = tmp;
}

void vec::reverse() {
    int i = 0;
    int j = size_-1;
    while (i < j) {
        swap(i++, j--);
    }
}



std::string vec::str() const {
    std::string s = "<";

    for (int i = 0; i < size_; i++) {
        s += std::to_string(arr_[i]);
        if (i < size_-1)
            s += ", ";
    }
    s += ">";
    //s += " (allocsize_: " + std::to_string(allocsize_);
    //s += "; size_: " + std::to_string(size_) + ")";
    return s;
}

std::string vec::as_str() const {
    std::string s;
    s.reserve(size_);
    for (int i = 0; i < size_; i++)
        s += (char)arr_[i];
    return s;
}

int vec::dot_prod(const vec& v1, const vec& v2) const {
    if (v1.size() != v2.size())
        throw std::out_of_range("+: length mismatch");

    int total = 0;
    for (int i = 0; i < v1.size(); i++) {
        total += v1.get(i)*v2.get(i);
    }

    return total;
}

void vec::apply(auto fn) {
    for (int i = 0; i < size_; i++)
        arr_[i] = (int)fn(arr_[i]);
}


std::ostream& operator<<(std::ostream& strm, const vec& v) {
    return strm << v.str();
}

vec operator+(int n, const vec& v) {
    return v+n;
}

vec operator+(const vec& v, int n) {
    vec out = vec(v);
    for (int i = 0; i < v.size_; i++) {
        out.arr_[i] += n;
    }
    return out;
}

vec operator+(const vec& v1, const vec& v2) {
    if (v1.size() != v2.size())
        throw std::out_of_range("+: length mismatch");

    vec out = vec(v1);

    for (int i = 0; i < v1.size(); i++) {
        out[i] = v1.get(i) + v2.get(i);
    }

    return out;
}

vec operator*(int n, const vec& v) {
    return v*n;
}

vec operator*(const vec& v, int n) {
    vec out = vec(v);
    for (int i = 0; i < v.size_; i++) {
        out.arr_[i] *= n;
    }
    return out;
}

vec operator*(const vec& v1, const vec& v2) {
    if (v1.size() != v2.size())
        throw std::out_of_range("+: length mismatch");

    vec out = vec(v1);

    for (int i = 0; i < v1.size(); i++) {
        out[i] = v1.get(i) * v2.get(i);
    }

    return out;
}

int vec::get(int i) const {
    if (i <= size_) {
        return arr_[i];
    }
    throw std::out_of_range("Invalid position!");
}

int& vec::operator[](int i) {
    if (i <= size_) {
        return arr_[i];
    }
    throw std::out_of_range("Invalid position!");
}

const int& vec::operator[](int i) const {
    if (i <= size_) {
        return arr_[i];
    }
    throw std::out_of_range("Invalid position!");
}

vec& vec::operator=(const vec& v)
{
  int* p = new int[v.size()];
  for (int i = 0; i < v.size(); i++)
  {
    p[i] = v.arr_[i];
  }
  delete arr_;
  arr_ = p;
  size_ = v.size_;
  allocsize_ = v.allocsize_;
  return *this;
}

int* begin(vec& v)
{
    return v.size() ? &v[0] : nullptr;
}

int* end(vec& v)
{
    return begin(v) + v.size();
}




void run_tests() {
    vec v{1,2,3};
    assert(v.str() == "<1, 2, 3>");

    v.realloc(2);
    assert(v.str() == "<1, 2>");

    v.realloc(3);
    assert(v.str() == "<1, 2>");

    v.clear();
    assert(v.str() == "<>");

    v.append(9);
    assert(v.str() == "<9>");

    v.append(8);
    assert(v.str() == "<9, 8>");


    // Indexing
    assert(v[0] == 9);
    v[0] = 47;
    assert(v[0] == 47);


    // Arithmetic
    vec v3 = vec(v);
    assert((1000+v3+1000).str() == "<2047, 2008>");
    assert(v3.str() == "<47, 8>");
    assert(v3.sum() == 55);
    v3[0] = 3;
    assert((v3*v3).str() == "<9, 64>");


    // Generators
    assert(vec::range(10).sum() == 55);

    vec v4{1,2,3};
    assert((v4 + v4).str() == "<2, 4, 6>");

    // Resizing
    v4.resize(5, 99);
    assert(v4.str() == "<1, 2, 3, 99, 99>");
    v4.resize(2);
    assert(v4.str() == "<1, 2>");
    v4.resize(3, 9);
    assert((3*v4*3).str() == "<9, 18, 81>");


    // Swap
    v4.swap(0,2);
    assert(v4.str() == "<9, 2, 1>");

    // Reverse
    vec v5 = vec::range(3);
    v5.reverse();
    assert(v5.str() == "<3, 2, 1, 0>");

    // Head
    assert(v5.head() == 3);
    assert(v5.head(2).str() == "<3, 2>");
    assert(v5.head(6,10).str() == "<3, 2, 1, 0, 10, 10>");

    // Char array constructor
    vec v6 = vec("hello");
    assert(v6.str() == "<104, 101, 108, 108, 111>");
    assert(v6.as_str() == "hello");

    // Apply
    vec v7{1, 9, 3};
    auto doubleMe = [](int i){return 2*i;};
    v7.apply(doubleMe);
    assert(v7.str() == "<2, 18, 6>");

    // For Loop
    int count = 0;
    for (auto& i : v7) {
        assert(i == v7[count]);
        count++;
    }
}

int main()
{
    run_tests();
    return 0;
}
