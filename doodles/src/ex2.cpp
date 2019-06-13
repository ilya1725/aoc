//============================================================================
// Name        : ex2.cpp
// Author      : Ilya Katsnelson
// Version     :
// Copyright   : free to use
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <cfenv>
#include <climits>
#include <vector>
#include <array>
#include <algorithm>
#include <array>
#include <set>
#include <unordered_map>
#include <sstream>
#include <utility>
#include <atomic>

int main() {
    float f;
    auto p = reinterpret_cast<int*>(&f);  // uh-oh
    *p = 0x3FF00000;                       // breaks strict aliasing

    uint8_t buf[12] = {0};
    std::array<uint8_t, 12> array{0};
    uint16_t data = 0x5511;

    *(reinterpret_cast<uint16_t*>(&buf[1])) = data;
    *(reinterpret_cast<uint16_t*>(&array[1])) = data;

    {
        uint16_t v1 = 0x1122;
        uint16_t v2 = 0xaaff;
        std::atomic_bool flag1(false);

        uint16_t r1 = v1 | v2;
        std::cout << std::hex << r1 << std::endl;

        uint16_t r2 = static_cast<uint16_t>(flag1.load()) | static_cast<uint16_t>(true * 0xaaff);
        std::cout << std::hex << r2 << std::endl;

        std::cout << __VERSION__ << std::endl;
    }

}
