#include <iostream>
#include <cstdarg>
#include <string>
#include <stdexcept>
#include <math.h>
#include <sstream>

// For testing
#include <assert.h>
#include <complex.h>
template <typename T>
void print(T n) {std::cout << n << std::endl;};



////////////////////////////////
// Vectorized Function Macros //
////////////////////////////////

// Vectorized functions
// fn(op) = {fn(op0), fn(op1), ...}
// sin, abs, sqrt, etc...
#define VECTORIZE_FN(FN) template <typename T>\
vec<T> FN(const vec<T>& v)\
{\
    vec<T> w(v.size_);\
    w.size_ = v.size_;\
    for (int i = 0; i < v.size_; i++){\
        w.arr_[i] = FN(v.arr_[i]);\
    }\
    return w;\
}

#define VECTORIZE_FN_PROTO(FN) template <typename Y>\
friend vec<Y> FN(const vec<Y>& v);





////////////////////////////
// Binary Operator Macros //
////////////////////////////

// A binary operator taking an atom on the left
//   and a `vec` on the right
#define BOP_ATM_VEC(OP) template <typename T, typename Q>\
vec<T> operator OP(Q n, const vec<T>& v) {\
    const int size = v.size();\
    vec<T> out = vec<T>(size); /* Allocate vector `out` */\
    out.size_ = size;\
    for (int i = 0; i < size; i++) {\
        out[i] = n OP v[i];\
    }\
    return out;\
}

// A binary operator taking a `vec` on the left
//   and an atom on the right
#define BOP_VEC_ATM(OP) template <typename T, typename Q>\
vec<T> operator OP(const vec<T>& v, Q n) {\
    const int size = v.size();\
    vec<T> out(size); /* Allocate a new vector `out` */\
    out.size_ = size;\
    for (int i = 0; i < size; i++) {\
        out[i] = v[i] OP n;\
    }\
    return out;\
}

// A binary operator taking a `vec` on both the right
//   hand side and the left hand side. Vecs have different types
#define BOP_VECT_VEC(OP) template <typename T, typename Q>\
vec<T> operator OP(const vec<T>& v1, const vec<Q>& v2) {\
    if (v1.size() != v2.size()) {\
        throw std::out_of_range("length error");\
    }\
    const int size = v1.size();\
    vec<T> out(size);\
    out.size_ = size;\
    for (int i = 0; i < size; i++) {\
        out[i] = v1[i] OP v2[i];\
    }\
    return out;\
}



// Implement all three cases for binary operators
#define IMPL_BOP(OP) BOP_VECT_VEC( OP ) \
BOP_ATM_VEC( OP ) \
BOP_VEC_ATM( OP )


// Define prototype functions for binary operators
#define PROTO_BOP(OP) template <typename Y, typename Q>\
friend vec<Y> operator OP(const vec<Y>& v, Q n);\
template <typename Y, typename Q>\
friend vec<Y> operator OP(Q n, const vec<Y>& v);\
template <typename Y, typename Q>\
friend vec<Y> operator OP(const vec<Y>& v1, const vec<Q>& v2);



////////////////////////////////
// Comparison Operator Macros //
////////////////////////////////

// A binary operator taking an atom on the left
//   and a `vec` on the right
#define COMP_ATM_VEC(OP) template <typename T, typename Q>\
vec<bool> operator OP(Q n, const vec<T>& v) {\
    const int size = v.size();\
    vec<bool> out(size); /* Allocate vector `out` */\
    out.size_ = size;\
    for (int i = 0; i < size; i++) {\
        out[i] = n OP v[i];\
    }\
    return out;\
}

// A binary operator taking a `vec` on the left
//   and an atom on the right
#define COMP_VEC_ATM(OP) template <typename T, typename Q>\
vec<bool> operator OP(const vec<T>& v, Q n) {\
    const int size = v.size();\
    vec<bool> out(size); /* Allocate a new vector `out` */\
    out.size_ = size;\
    for (int i = 0; i < size; i++) {\
        out[i] = v[i] OP n;\
    }\
    return out;\
}

// A binary operator taking a `vec` on both the right
//   hand side and the left hand side. Vecs have different types
#define COMP_VECT_VEC(OP) template <typename T, typename Q>\
vec<bool> operator OP(const vec<T>& v1, const vec<Q>& v2) {\
    if (v1.size() != v2.size()) {\
        throw std::out_of_range("length error");\
    }\
    const int size = v1.size();\
    vec<bool> out(size);\
    out.size_ = size;\
    for (int i = 0; i < size; i++) {\
        out[i] = v1[i] OP v2[i];\
    }\
    return out;\
}



// Implement all three cases for binary operators
#define IMPL_COMP(OP) COMP_VECT_VEC( OP ) \
COMP_ATM_VEC( OP ) \
COMP_VEC_ATM( OP )


// Define prototype functions for binary operators
#define PROTO_COMP(OP) template <typename Y, typename Q>\
friend vec<bool> operator OP(const vec<Y>& v, Q n);\
template <typename Y, typename Q>\
friend vec<bool> operator OP(Q n, const vec<Y>& v);\
template <typename Y, typename Q>\
friend vec<bool> operator OP(const vec<Y>& v1, const vec<Q>& v2);





/////////////////////
// The `vec` Class //
/////////////////////


template <typename T>
class vec {
public:
    // Constructor / Destructors
    vec();                              // Default Constructor
    vec(int size);                      // Preallocated memory constructor
    vec(std::initializer_list<T> lst);  // initializer_list constructor
    vec(const vec& other);              // Copy constructor
    vec(vec<T>&& v);                    // Move constructor
    ~vec() {delete[] arr_;};            // Destructor
    vec& operator=(const vec& v);

    // Utils / Access
    int size() const {return size_;};
    T& operator[](int i);
    const T& operator[](int i) const;


    // Resource Mgmt.
    void realloc(int s);
    void resize(int size, T dflt);
    void resize(int size);
    void clear();

    // Aggregate Operations
    template <typename Y>
    friend Y sum(const vec<Y>& v);

    template <typename Y>
    friend Y prod(const vec<Y>& v);
    //vec cumsum() const;
    //vec cummprod() const;
    //double mean() const;
    //int median() const;
    //int mode() const;
    //int max() const;
    //int min() const;

    // Output
    template <typename Y>
    friend std::ostream& operator<<(std::ostream&, const vec<Y>&);

    std::string str() const;

    // Modification
    void append(T x);
    void append(const vec<T>& v);
    void swap(int i, int j);
    void reverse();

    // Sublists
    T head() const;
    vec<T> head(int items) const;
    vec<T> head(int items, T overtake) const;

    void pop();
    void pop(int n);

    vec<T> take(vec<bool> filter) const;

    // Functional
    vec<T> apply(auto fn);
    vec<T> apply_to(vec<bool> filter, auto fn);

    //Generators
    static vec<T> range(T i);
    static vec<T> range(T a, T b);
    static vec<T> range(T a, T b, T inc);

    // Statistics
    //double regression(const vec& a, const vec& b);



    // Operators
    PROTO_BOP(+)
    PROTO_BOP(-)
    PROTO_BOP(*)
    PROTO_BOP(/)
    PROTO_BOP(%)
    PROTO_BOP(&&)
    PROTO_BOP(||)
    PROTO_BOP(&)
    PROTO_BOP(|)

    // Comparison
    PROTO_COMP(>)
    PROTO_COMP(<)
    PROTO_COMP(<=)
    PROTO_COMP(>=)
    PROTO_COMP(==)
    PROTO_COMP(!=)

    friend vec<bool> operator!(const vec<bool>& v);



    // Math
    //sin, cos, etc...
    //static int dot_prod(const vec &v1, const vec &v2) const;
    VECTORIZE_FN_PROTO(sin);
    VECTORIZE_FN_PROTO(cos);
    VECTORIZE_FN_PROTO(tan);
    VECTORIZE_FN_PROTO(acos);
    VECTORIZE_FN_PROTO(asin);
    VECTORIZE_FN_PROTO(atan);
    VECTORIZE_FN_PROTO(atan2);
    VECTORIZE_FN_PROTO(cosh);
    VECTORIZE_FN_PROTO(sinh);
    VECTORIZE_FN_PROTO(tanh);
    VECTORIZE_FN_PROTO(acosh);
    VECTORIZE_FN_PROTO(asinh);
    VECTORIZE_FN_PROTO(atanh);
    VECTORIZE_FN_PROTO(exp);
    VECTORIZE_FN_PROTO(log);
    VECTORIZE_FN_PROTO(log10);
    VECTORIZE_FN_PROTO(log2);
    VECTORIZE_FN_PROTO(sqrt);
    VECTORIZE_FN_PROTO(cbrt);
    VECTORIZE_FN_PROTO(ceil);
    VECTORIZE_FN_PROTO(floor);
    VECTORIZE_FN_PROTO(abs);

    vec<T> power(T i);


private:
    T* arr_;
    int size_;
    int allocsize_;
};







//////////////////
// Constructors //
//////////////////

template<typename T>
vec<T>::vec() : allocsize_{0}, size_{0}, arr_{new T[0]}
{}

template <typename T>
vec<T>::vec(int size) : allocsize_(0), size_(0)
{
    realloc(size);
}

template <typename T>
vec<T>::vec(std::initializer_list<T> lst) : allocsize_(0), size_(0)
{
    realloc(lst.size());
    std::copy(lst.begin(), lst.end(), arr_);
    size_ = lst.size();
}


//Copy constructor
template <typename T>
vec<T>::vec(const vec<T>& other) :allocsize_(0), size_(0)
{
    realloc(other.allocsize_);
    size_ = other.size_;

    for (int i = 0; i < size_; i++)
        arr_[i] = other.arr_[i];
}

//Move Constructor
template <typename T>
vec<T>::vec(vec<T>&& v)
    :allocsize_{v.allocsize_},  // Grab the elements from v
    size_{v.size_},
    arr_{v.arr_}
{
    v.arr_ = nullptr;  // Now v has no elements
    v.size_ = 0;
    v.allocsize_ = 0;
}

template <typename T>
vec<T>& vec<T>::operator=(const vec<T>& v)
{
  T* p = new T[v.size()];
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



/////////////////////////
// Resource Management //
/////////////////////////


template <typename T>
void vec<T>::realloc(int s) {

    //No change in array size
    if (s == allocsize_)
        return;

    T* newarr = new T[s];

    //We are making the array smaller
    if (s < allocsize_)
    {
        for (int i = 0; i < s; i++)
            newarr[i] = arr_[i];

        size_ = s;
    }

    //We are making the array bigger
    else
    {
        // Copy the existing elements
        for (int i = 0; i < allocsize_; i++)
            newarr[i] = arr_[i];

        // Leave newly allocated space uninitilized
    }

    //If there is data in the array, free the memory
    if (allocsize_ > 0)
        delete[] arr_;

    allocsize_ = s;
    arr_ = newarr;
}



template <typename T>
void vec<T>::resize(int size, T dflt)
{
    // Same size, do nothing
    if (size == size_)
        return;

    //Making it smaller
    else if (size < size_)
        realloc(size);

    //Making it bigger
    else
    {
        int oldsize = size_;
        realloc(size);

        for (int i = oldsize; i < size; i++)
            arr_[i] = dflt;

        size_ = size;
    }
}

template <typename T>
void vec<T>::resize(int size)
{
    realloc(size);
    size_ = size;
}

template <typename T>
void vec<T>::clear()
{
    realloc(0);
}




/////////////////////
// Utils. / Access //
/////////////////////

template <typename T>
T& vec<T>::operator[](int i)
{
    if (i < 0)
        i = size_ + i;

    if (i <= size_)
        return arr_[i];

    throw std::out_of_range("Invalid position!");
}

template <typename T>
const T& vec<T>::operator[](int i) const
{
    if (i <= size_)
        return arr_[i];

    throw std::out_of_range("Invalid position!");
}



//////////////////////////
// Aggregate Operations //
//////////////////////////

template <typename T>
T sum(const vec<T>& v)
{
    T total = 0;
    for (int i = 0; i < v.size_; i++) {
        total += v.
        arr_[i];
    }
    return total;
}

template <typename T>
T prod(const vec<T>& v)
{
    T total = 1;
    for (int i = 0; i < v.size_; i++) {
        total *= v.arr_[i];
    }
    return total;
}





////////////
// Output //
////////////

template <typename T>
std::string vec<T>::str() const {
    std::stringstream s;
    s << "<";

    for (int i = 0; i < size_; i++) {
        s << arr_[i];
        if (i < size_-1)
            s << ", ";
    }
    s << ">";
    //s += " (allocsize_: " + std::to_string(allocsize_);
    //s += "; size_: " + std::to_string(size_) + ")";
    return s.str();
}

template <typename T>
std::ostream& operator<<(std::ostream& strm, const vec<T>& v) {
    return strm << v.str();
}





//////////////////
// Modification //
//////////////////

template <typename T>
void vec<T>::append(T x)  {
    //Do we need to increase the vector array size?
    if (size_+1 > allocsize_) {
        realloc(size_+1);
    }

    arr_[size_] = x;
    size_++;
}

template <typename T>
void vec<T>::append(const vec<T>& v)
{
    int newsize = size_ + v.size_;

    realloc(newsize);

    // Copy the items
    for (int i = size_, j = 0; i < newsize; i++, j++)
        arr_[i] = v.arr_[j];

    size_ = newsize;
}

template <typename T>
void vec<T>::swap(int i, int j) {
    //Bounds check
    if (!(i >= 0 && i < size_ && j >= 0 && j < size_)) {
        throw std::out_of_range("vec::swap() bounds error");
    }

    int tmp = arr_[i];
    arr_[i] = arr_[j];
    arr_[j] = tmp;
}

template <typename T>
void vec<T>::reverse() {
    int i = 0;
    int j = size_-1;
    while (i < j) {
        swap(i++, j--);
    }
}






//////////////
// Sublists //
//////////////

template <typename T>
T vec<T>::head() const
{
    if (size_ == 0)
        throw std::out_of_range("head(): vec has no elements");

    return arr_[0];
}

template <typename T>
vec<T> vec<T>::head(int items) const
{
    return head(items, 0);
}

template <typename T>
vec<T> vec<T>::head(int items, T overtake) const
{
    vec v = vec<T>(items);
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

template <typename T>
void vec<T>::pop()
{
    size_ -= 1;
}

template <typename T>
void vec<T>::pop(int n)
{
    size_ -= n;

    if (size_ < 0)
        size_ = 0;
}

template <typename T>
vec<T> vec<T>::take(vec<bool> filter) const
{
    if (filter.size() != size_)
        throw std::out_of_range("take: length error");

    const int size = filter.size();
    int newsize = 0;

    // Count the length of items
    for (int i = 0; i < size; i++)
        newsize += filter[i] ? 1 : 0;

    // Initilize the new vec
    vec<T> out(newsize);
    out.size_ = newsize;


    // Add the items
    for (int i = 0, curr_idx = 0; i < size; i++)
    {
        if (filter[i])
        {
            out[curr_idx] = arr_[i];
            curr_idx++;
        }
    }

    return out;
}





////////////////
// Functional //
////////////////

template <typename T>
vec<T> vec<T>::apply(auto fn) {
    for (int i = 0; i < size_; i++)
        arr_[i] = fn(arr_[i]);
    return *this;
}

template <typename T>
vec<T> vec<T>::apply_to(vec<bool> filter, auto fn) {
    if (filter.size() != size_)
        throw std::out_of_range("take: length error");

    for (int i = 0; i < size_; i++)
        if (filter[i])
            arr_[i] = fn(arr_[i]);

    return *this;
}


////////////////
// Operators  //
////////////////


IMPL_BOP(+);
IMPL_BOP(-);
IMPL_BOP(*);
IMPL_BOP(/);
IMPL_BOP(%);
IMPL_BOP(&&);
IMPL_BOP(||);
IMPL_BOP(&);
IMPL_BOP(|);

IMPL_COMP(<);
IMPL_COMP(>);
IMPL_COMP(<=);
IMPL_COMP(>=);
IMPL_COMP(==);
IMPL_COMP(!=);


vec<bool> operator!(const vec<bool>& v)
{
    const int size = v.size();
    vec<bool> out(size);
    out.size_ = size;

    for (int i = 0; i < size; i++)
    {
        out[i] = !v[i];
    }

    return out;
}


////////////////
// VECTORIZED //
////////////////

VECTORIZE_FN(sin);
VECTORIZE_FN(cos);
VECTORIZE_FN(tan);
VECTORIZE_FN(acos);
VECTORIZE_FN(asin);
VECTORIZE_FN(atan);
VECTORIZE_FN(atan2);
VECTORIZE_FN(cosh);
VECTORIZE_FN(sinh);
VECTORIZE_FN(tanh);
VECTORIZE_FN(acosh);
VECTORIZE_FN(asinh);
VECTORIZE_FN(atanh);
VECTORIZE_FN(exp);
VECTORIZE_FN(log);
VECTORIZE_FN(log10);
VECTORIZE_FN(log2);
VECTORIZE_FN(sqrt);
VECTORIZE_FN(cbrt);
VECTORIZE_FN(ceil);
VECTORIZE_FN(floor);
VECTORIZE_FN(abs);

////////////////
// Generators //
////////////////


template <typename T>
vec<T> vec<T>::range(T n)
{
    vec<T> v(n);
    v.size_ = n;

    for (int i = 0; i < n; i++)
        v.arr_[i] = i;

    return v;
}

template <typename T>
vec<T> vec<T>::range(T a, T b)
{
    // If a < b, count up, else count down
    T inc = 0;
    if (a < b) {
        inc = 1;
    } else {
        inc = -1;
    }

    return vec<T>::range(a, b, inc);
}

template <typename T>
vec<T> vec<T>::range(T a, T b, T inc)
{
    // Invalid arguments
    // a will never become b with the given inc
    if ( (a < b && inc <= 0) || (a > b && inc >= 0))
        throw std::invalid_argument("invalid range");

    T cur = a; // current
    vec<T> v(abs(a-b) / abs(inc));

    if (a < b)
    {
        while (cur <= b)
        {
            v.append(cur);
            cur += inc;
        }
    }
    else
    {
        while (cur >= b)
        {
            v.append(cur);
            cur += inc;
        }
    }

    return v;
}




//
// std::string vec::as_str() const {
//     std::string s;
//     s.reserve(size_);
//     for (int i = 0; i < size_; i++)
//         s += (char)arr_[i];
//     return s;
// }
//
// int vec::dot_prod(const vec& v1, const vec& v2) const {
//     if (v1.size() != v2.size())
//         throw std::out_of_range("+: length mismatch");
//
//     int total = 0;
//     for (int i = 0; i < v1.size(); i++) {
//         total += v1.get(i)*v2.get(i);
//     }
//
//     return total;
// }
//

template <typename T>
vec<T> pow(const vec<T>& v, T n)
{
    const int size = v.size();

    vec<T> w(size);
    w.resize(size);

    for (int i = 0; i < size; i++)
        w[i] = pow(v[i], n);

    return w;
}

template <typename T>
vec<T> pow(T n, const vec<T>& v)
{
    const int size = v.size();

    vec<T> w(size);
    w.resize(size);

    for (int i = 0; i < size; i++)
        w[i] = pow(n, v[i]);

    return w;
}


template <typename T>
vec<T> pow(const vec<T>& a, const vec<T>& b)
{
    if (a.size() != b.size())
        throw std::out_of_range("size mismatch");

    const int size = a.size();

    vec<T> w(size);
    w.resize(size);

    for (int i = 0; i < size; i++)
        w[i] = pow(a[i], b[i]);

    return w;
}


template <typename T>
T* begin(vec<T>& v)
{
    return v.size() ? &v[0] : nullptr;
}

template <typename T>
T* end(vec<T>& v)
{
    return begin(v) + v.size();
}




void run_tests() {
    vec<int> vi{1,2,3};
    vec<float> vf{1.2, 3.3, 4.5};


    assert(vi.str() == "<1, 2, 3>");



    // Resource Mgmt.
    vi.realloc(2);
    assert(vi.str() == "<1, 2>");

    vi.realloc(3);
    assert(vi.str() == "<1, 2>");

    vi.clear();
    assert(vi.str() == "<>");

    vi.resize(2, 4);
    assert(vi.str() == "<4, 4>");

    vi.resize(3);
    assert(vi.str() == "<4, 4, 0>");

    vi.resize(2);
    assert(vi.str() == "<4, 4>");





    // Aggregate Operations
    assert(sum(vi) == 8);
    assert(sum(vf) == 9.0f);
    assert(prod(vi) == 16);

    // Modifications
    vi.append(5);
    assert(vi.str() == "<4, 4, 5>");

    vi.reverse();
    assert(vi.str() == "<5, 4, 4>");

    vec<int> vi2{9, 8};
    vi.append(vi2);
    assert(vi.str() == "<5, 4, 4, 9, 8>");

    vec<int> vi3;
    vi.append(vi3);
    assert(vi.str() == "<5, 4, 4, 9, 8>");

    // Indexing
    assert(vi[0] == 5);
    assert(vi[-1] == 8);
    vi[0] = 47;
    vi[-1] = 3;
    assert(vi[0] == 47);
    assert(vi[-1] == 3);

    // Sublists
    assert(vf.head() == 1.2f);
    assert(vf.head(2).str() == "<1.2, 3.3>");
    assert(vf.head(4).str() == "<1.2, 3.3, 4.5, 0>");
    assert(vf.head(4, 1.0f).str() == "<1.2, 3.3, 4.5, 1>");

    // Functional
    vi = vec<int>{1,2,3};
    vi.apply([](int i){return 2*i;});
    assert(vi.str() == "<2, 4, 6>");

    auto square = [](int i){return i*i;};
    vi.apply(square);
    assert(vi.str() == "<4, 16, 36>");


    // Operators
    vi = vec<int>{1,2,3};
    vi2 = vec<int>{3,2,1};
    assert((1000+vi+1000).str() == "<2001, 2002, 2003>");
    assert(vi.str() == "<1, 2, 3>");
    assert((vi*vi).str() == "<1, 4, 9>");
    assert((vi-vi2).str() == "<-2, 0, 2>");
    assert((vi-1).str() == "<0, 1, 2>");
    assert((vi>2).str() == "<0, 0, 1>");

    // Vectorized functions
    assert(abs(vec<int>{-1, 0, 1}).str() == "<1, 0, 1>");

    // For Loop
    int count = 0;
    for (auto& i : vi) {
        assert(i == vi[count]);
        count++;
    }

    // Comparison / filter
    vi = vec<int>{1,2,3,4,5};
    assert(vi.take(vi > 3).str() == "<4, 5>");
    assert(vi.take(vi < 3 || vi == 4).str() == "<1, 2, 4>");
    assert(vi.take(vi <= 3 && vi != 2).str() == "<1, 3>");


    // Misc
    // Project Euler 1: 233168
    vec<int> pe1 = vec<int>::range(1000);
    assert(sum(pe1.take(pe1 % 3 == 0 || pe1 % 5 == 0)) == 233168);

    //Project Euler 2: 4613732
    vec<int> pe2{0,1};
    while (pe2[-1] < 4000000)
        pe2.append(pe2[-1] + pe2[-2]);
    pe2.pop();
    assert(sum(pe2.take(pe2 % 2 == 0)) == 4613732);

    // Poject Euler 6: 25164150
    vec<int> pe6 = vec<int>::range(1,100,1);
    int pe6_n = pow(sum(pe6),2) - sum(pow(pe6,2));
    assert(pe6_n == 25164150);

    assert(vec<int>::range(1,-9,-10).str() == "<1, -9>");


    assert(pow(vec<int>::range(5), 2).str() == "<0, 1, 4, 9, 16>");
    assert(pow(2, vec<int>::range(5)).str() == "<1, 2, 4, 8, 16>");
    assert(pow(vec<int>::range(4), vec<int>::range(4)).str() == "<1, 1, 4, 27>");


    assert(sum(pow(vec<int>::range(1,10,1),2)) == 385);
    assert(pow(sum(vec<int>::range(1,10,1)),2) == 3025);


}

#define V vec<int>

int main()
{

    run_tests();

    vec<int> vi{1,2,3};
    //print(vi);

    vec<double> vd{1,2,3};
    //print(vd);

    vec<char> vc{'c','d'};
    //print(vc);

    vec<double> vds = sin(vd);

    print(vds);

    print(sin(vec<float>{3.1415926595, 2}));

    print(abs(vec<int>{1, -1, 7, -7}));

    print(vec<int>{1, 2, 3} - vec<int>{3, 2, 1});

    print(V{1,2,3});

    print(vec<bool>{true, false} || false);

    print(vec<int>{1, 2, 3} % 2);

    vec<vec<int>> mat{ {1,2,3}, {4, 5, 6} };
    print(mat * mat);

    print(mat + (vec<int>{1, 2}));

    print(vec<float>{1.2, 3.4} + vec<int>{1, 2});
    print(vec<int>{1, 2} + vec<float>{1.2, 3.4});
    print(vec<int>{1, 2} + 1.5);
    print(1.5 + vec<int>{1, 2});


    for (auto& row : mat) {
        print(row);
    }


    print(vec<int>{1,2,3} > 2);

    vi = vec<int>{1, 2, 3, 4};
    vec<bool> vb{0,1,1,0};
    print(vi.take(vb && 1));

    print(vi.take(!(vi < 3 || vi == 4)));

    print(vi.take((vi < 3) + 1));

    vec<int> vr = vec<int>::range(10);
    print(vr);
    print(1+vr);


    int sumSqEvens = sum(vr.apply_to(vr % 2 == 0, [](int i){return i * i;}));
    print(sumSqEvens);








    return 0;
}
