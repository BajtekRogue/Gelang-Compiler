#include <iostream>

void f(int* x, int* y){
    *x = *x + *y;
    *y = *x - *y;
}

void g(int* x, int* y){
    f(x, y);
    f(x, y);
}

int main(){
    int a = 5;
    int b = 3;
    g(&a, &b);
    std::cout << a << " " << b << std::endl;
    return 0;
}