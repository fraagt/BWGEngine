#include <iostream>
#include "application.hpp"

int main() {
    try {
        Application application;

        application.run();
    } catch (const std::exception &exception) {
        std::cout << exception.what() << '\n';
    }
    return 0;
}
