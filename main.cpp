#include <iostream>
#include <string>
#include "calculator.h"
#include "loader.h"

int main() {
    Loader loader("plugins");

    std::cout << (2 * sin(1.57)) << std::endl;

    Calculator calculator(loader);
    std::string expression;

    std::cout << "Enter an expression (or ~ to exit): ";
    while (std::getline(std::cin, expression)) {
        if (expression == "~") {
            break;
        }
        try {
            double result = calculator.parser(expression);
            std::cout << "Result: " << result << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
        std::cout << "Enter an expression (or ~ to exit): ";
    }

    return 0;
}