#include <iostream>

int multiply(int x, int y) {
    int sign = 1;
    if (y < 0) {
        sign = -sign;
        y = -y;
    }
    int result = 0;
    while (y > 0) {
        int half = y / 2;
        int doubled = half + half;
        int mod2 = y - doubled;
        if (mod2 != 0) {  
            result = result + x;
        }
        x = x + x;
        y = half;
    }
    return (sign < 0) ? -result : result;
}

int main(){
    int x, y;
    std::cin >> x >> y;
    std::cout << multiply(x, y) << std::endl;
}