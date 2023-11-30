/******************************************************************************
 *
 * Received this sample from Ali Haydar Uluturk <ali.uluturk@trust-in-soft.site>
 * with the title TrustInSoft’s ‘Bug and Vulnerability’ challenge - Can you find the undefined behavior?
 * Just trying to see what can be located.
 *
*******************************************************************************/

#include <iostream>
#include <array>
#include <bitset>

#define OFF_LIMITS 100000


int calculate(int x, int y) {

    return x * x + 1182 * x - (y * y) - 882;

}



int give_me_a_number(int x, int y) {

    if (x <= 0 || x > 10000 || y >= 0 || y < -10000)

        return OFF_LIMITS;

    return (calculate(x - 19, y + 30));
}


int main() {

    int tab[100000];

    // for (int i = 140; i < 180; i = i+1) {
    //     for (int j = -500; j < -400; j = j+1) {
    //         std::cout << i << ":" << j << " - " << give_me_a_number(i, j) << std::endl;
    //     }
    // }


    for (int i = 140; i < 180; i++) {

        for (int j = -500; j < -400; j++) {
            const auto index = give_me_a_number(i, j);
            if (index == 0 ) {
                std::cout << i << ":" << j << " - " << index << std::endl;
                continue;
            }
            tab[std::abs(index)-1] = i+j;

        }

    }

    return 0;
}
