/******************************************************************************

*******************************************************************************/

#include <iostream>
#include <array>
#include <bitset>

template <class T, std::size_t N>
class Booth
{
  public:
    int function(const std::array<T, N> & data) {
        std::bitset<data.size()> bit_set{};

        return 0;
    };

  private:
};

int function(const std::array<int, 42> & data) {
    std::bitset<data.size()> bit_set{};

    return 0;
}

int main()
{
    std::cout << "std::array test\n";

    std::array<int, 42> data{2};
    std::cout << function(data) << std::endl;

    std::array<int, 3> data2{2};
    Booth<int, 3>booth{};
    std::cout << booth.function(data2) << std::endl;

    return 0;
}