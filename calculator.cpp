#include "calculator.h"
#include <iostream>
#include <stdexcept>
#include <cctype>


void Calculator::skip_spaces(const std::string& expression, size_t& pos) {
    while (pos < expression.length() && isspace(expression[pos])) {
        pos++;
    }
}

double Calculator::parser(const std::string& expression) {
    size_t pos = 0;
    return parse_expression(expression, pos);
}

double Calculator::call(const std::string& func_name, double arg) {
    return loader.method_function(func_name, arg);
}

double Calculator::call(const std::string& func_name, double arg1, double arg2) {
    return loader.method_function(func_name, arg1, arg2);
}

double Calculator::parse_expression(const std::string& expression, size_t& pos) {
    double result = parse_term(expression, pos);
    while (pos < expression.length()) {
        skip_spaces(expression, pos);
        char op = expression[pos];
        if (op == '+' || op == '-') {
            pos++;
            double term = parse_term(expression, pos);
            if (op == '+') {
                result += term;
            }
            else {
                result -= term;
            }
        }
        else {
            break;
        }
    }

    return result;
}

double Calculator::parse_term(const std::string& expression, size_t& pos) {
    double result = parse_brackets(expression, pos);
    while (pos < expression.length()) {
        skip_spaces(expression, pos);
        char op = expression[pos];
        if (op == '*' || op == '/') {
            pos++;
            double factor = parse_brackets(expression, pos);
            if (op == '/') {
                result /= factor;
            }
            else if (op == '*') {
                result *= factor;
            }
        }
        else {
            break;
        }
    }

    return result;
}

double Calculator::parse_brackets(const std::string& expression, size_t& pos) {
    double result = 0;

    skip_spaces(expression, pos);

    if (isdigit(expression[pos])) {
        size_t start = pos;
        while (pos < expression.length() && (isdigit(expression[pos]) || expression[pos] == '.')) {
            pos++;
        }
        result = std::stod(expression.substr(start, pos - start));
        if (isalpha(expression[pos])) {
            throw std::invalid_argument("Invalid call syntax");
        }
    }
    else if (expression[pos] == '-' && (pos == 0 || expression[pos - 1] == '(')) {
        pos++;
        size_t start = pos;
        while (pos < expression.length() && (isdigit(expression[pos]) || expression[pos] == '.')) {
            pos++;
        }
        result = std::stod(expression.substr(start, pos - start)) * (-1);
    }
    else if (expression[pos] == '(') {
        pos++;
        result = parse_expression(expression, pos);
        if (expression[pos] == ')') {
            pos++;
        }
        else {
            throw std::invalid_argument("Wrong brackets");
        }
    }
    else {
        size_t start = pos;
        while (pos < expression.length() && isalpha(expression[pos])) {
            pos++;
        }

        std::string func_name = expression.substr(start, pos - start);
        if (expression[pos] == '(') {
            pos++;
            double arg = parse_expression(expression, pos);
            if (expression[pos] == ')') {
                pos++;
            }
            else {
                throw std::invalid_argument("Wrong brackets");
            }
            result = call(func_name, arg);
        }
        else {
            throw std::invalid_argument("Invalid function call syntax");
        }
    }

    if (expression[pos] == '^') {
        pos++;
        double factor = parse_brackets(expression, pos);
        result = call("pow", result, factor);
    }
    return result;
}