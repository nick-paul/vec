#include <iostream>

#include "vec.h"

using namespace std;

int main()
{
    cout << "hi" << endl;

    cout << sum(vec<int>::range(10)) << endl;
}
