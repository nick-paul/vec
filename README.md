A header-only C++ library for working with lists of numerics

## Examples

### Project Euler 1

Find the sum of all multiples of 3 or 5 below 1000

```c++
// Project Euler 1: 233168

// Create the range <0, 1, .. 999>
vec<int> pe1 = vec<int>::range(1000);

// Use the take() function to filter multiples of 3 or 5
cout << sum(pe1.take(pe1 % 3 == 0 || pe1 % 5 == 0)) << endl;
```

### Project Euler 2

Find the sum of the even Fibonacci numbers below 4 million

```c++
//Project Euler 2: 4613732

// Initilize a vec with the values <0, 1>
vec<int> pe2{0,1};

// Calculate Fibonacci numbers below 4000000
while (pe2[-1] < 4000000)
    pe2.append(pe2[-1] + pe2[-2]);
pe2.pop();

// Filter even numbers using take() and find the sum
cout << sum(pe2.take(pe2 % 2 == 0)) << endl;
```

### Project Euler 6
Find the difference between the sum of the first 100 integers squared and the
first 100 squares.

```c++
// Poject Euler 6: 25164150

// Create the range <1, 2, ...100>
vec<int> pe6 = vec<int>::range(1,100,1);

// pow() is vectorized
cout << pow(sum(pe6),2) - sum(pow(pe6,2)) << endl;
```
