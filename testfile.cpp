#include "vec.h"

#include <assert.h>
#include <complex.h>

template <typename T>
void print(T n) {std::cout << n << std::endl;};

void run_tests();

int main()
{

    run_tests();

    vec<int> vi;

    vi = vec<int>::range(3);





    return 0;
}



void run_tests()
{
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

    vi = vec<int>::range(3);
    assert(vi.tail() == 2);
    assert(vi.tail(0).str() == "<>");
    assert(vi.tail(1).str() == "<2>");
    assert(vi.tail(5).str() == "<0, 0, 0, 1, 2>");
    assert(vi.tail(5, 99).str() == "<99, 99, 0, 1, 2>");

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

    // Ranges
    assert(vec<int>::range(1,-9,-10).str() == "<1, -9>");
    assert(pow(vec<int>::range(5), 2).str() == "<0, 1, 4, 9, 16>");
    assert(pow(2, vec<int>::range(5)).str() == "<1, 2, 4, 8, 16>");
    assert(pow(vec<int>::range(4), vec<int>::range(4)).str() == "<1, 1, 4, 27>");
    assert((vec<int>::range(0)).str() == "<>");
    assert(vec<int>::range(-1).str() == "<0>");
    assert(vec<int>::range(1).str() == "<0>");


    assert(sum(pow(vec<int>::range(1,10,1),2)) == 385);
    assert(pow(sum(vec<int>::range(1,10,1)),2) == 3025);

    assert( (vec<bool>{true, false} || false).str() == "<1, 0>");

    vec<vec<int>> mat{ {1,2,3}, {4, 5, 6} };
    assert((mat * mat).str() == "<<1, 4, 9>, <16, 25, 36>>");

    assert( (mat + (vec<int>{1, 2})).str() == "<<2, 3, 4>, <6, 7, 8>>");

    // Conversions
    // vec <op> vec: use type from left side
    assert((vec<float>{1.2, 3.4} + vec<int>{1, 2}).str() == "<2.2, 5.4>");
    assert((vec<int>{1, 2} + vec<float>{1.2, 3.4}).str() == "<2, 5>");

    assert((vec<int>{1, 2} + 1.5).str() == "<2, 3>");
    assert((1.5 + vec<int>{1, 2}).str() == "<2, 3>");

    assert((vec<int>{1,2,3} > 2).str() == "<0, 0, 1>");

    vi = vec<int>{1, 2, 3, 4};
    vec<bool> vb{0,1,1,0};
    assert((vi.take(vb && 1)).str() == "<2, 3>");

    vi = vec<int>();
    assert((vi.take(!(vi < 3 || vi == 4))).str() == "<>");

    vi = vec<int>{1,2,3,4,5};
    assert((vi.take(!(vi < 3 || vi == 4))).str() == "<3, 5>");

    // non-zero is true
    assert((vi.take((vi < 3) + 1)).str() == "<1, 2, 3, 4, 5>");

    vec<int> vr = vec<int>::range(-2,2);
    assert(vr.str() == "<-2, -1, 0, 1, 2>");
    assert((1+vr).str() == "<-1, 0, 1, 2, 3>");

    vr = vec<int>::range(-100,100, 50);
    assert(vr.str() == "<-100, -50, 0, 50, 100>");

    vr = vec<int>::range(10);
    int sumSqEvens = sum(vr.apply_to(vr % 2 == 0, [](int i){return i * i;}));
    assert(sumSqEvens == 145);

    vec<double> v_empty;
    assert(v_empty.str() == "<>");
    assert(sum(v_empty) == 0);
    assert(sum(sin(v_empty)) == 0);
    assert((v_empty + v_empty * 10 - v_empty).str() == "<>");

    vi = vec<int>{1, 2, 3, 4};
    vf = vec<float>{0.2f, 3.5f, -53.0f};
    assert(max(vi) == 4);
    assert(min(vi) == 1);
    assert(max(vf) == 3.5f);
    assert(min(vf) == -53.0f);



}
