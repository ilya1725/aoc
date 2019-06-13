#include "ex3.h"

Manager::Manager() {
    faults_ = std::make_unique<DataReader>(kActuatorsCommand, kActuatorsProxy);
}

Manager::~Manager() {
}

int main() {
    Manager m;

    return 0;
}
