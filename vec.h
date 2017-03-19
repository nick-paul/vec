#include <iostream>
#include <cstdarg>
#include <string>
#include <stdexcept>
#include <math.h>
#include <sstream>





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
    ~vec() {if (allocsize_ > 0) delete[] arr_;};            // Destructor
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
    template <typename Y>
    friend Y max(const vec<Y>& v);

    template <typename Y>
    friend Y min(const vec<Y>& v);

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
    T tail() const;
    vec<T> tail(int items) const;
    vec<T> tail(int items, T overtake) const;

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

template <typename T>
T max(const vec<T>& v)
{
    T cur_max;
    if (v.size_ > 0)
        cur_max = v.arr_[0];
    else
        throw std::out_of_range("max: empty vector");

    for (int i = 1; i < v.size_; i++)
        cur_max = cur_max < v.arr_[i] ? v.arr_[i] : cur_max;

    return cur_max;
}


template <typename T>
T min(const vec<T>& v)
{
    T cur_min;
    if (v.size_ > 0)
        cur_min = v.arr_[0];
    else
        throw std::out_of_range("min: empty vector");

    for (int i = 1; i < v.size_; i++)
        cur_min = cur_min > v.arr_[i] ? v.arr_[i] : cur_min;

    return cur_min;
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
T vec<T>::tail() const
{
    if (size_ == 0)
        throw std::out_of_range("tail(): vec has no elements");

    return arr_[size_-1];
}

template <typename T>
vec<T> vec<T>::tail(int items) const
{
    return tail(items, 0);
}

template <typename T>
vec<T> vec<T>::tail(int items, T overtake) const
{
    vec v = vec<T>(items);
    v.size_ = items;
    //Overtake
    if (items > size_)
    {
        int start = items-size_;
        // overtake
        for (int i = 0; i < start; i++)
            v.arr_[i] = overtake;

        // copy in the back of the array
        for (int i = start, j = 0; i < items; i++, j++)
            v.arr_[i] = arr_[j];

    } else {
        int start = size_ - items;
        for (int i = start, j = 0; j < items; j++, i++)
            v.arr_[j] = arr_[i];
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
    if (n > 0)
        return vec<T>::range(0, n-1, 1);
    else if (n < 0)
        return vec<T>::range(0, n+1, -1);
    else
        return vec<T>(0);
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
    {
        throw std::invalid_argument("invalid range: " + std::to_string(a)
            + ", " + std::to_string(b) + ", " + std::to_string(inc));
    }

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
    else if (a > b)
    {
        while (cur >= b)
        {
            v.append(cur);
            cur += inc;

        }
    }
    else
    {
        v.append(cur);
    }


    return v;
}


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
