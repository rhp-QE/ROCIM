#include <iostream>

#include "Sample.hpp"





int main() {
    using namespace::std;
    int a, b;
    while(cin>>a>>b) {
        cout<<add(a, b)<<endl;
    }
    for(;;);
    return 0;
}