#include "regexp_handling.h"

#include <iostream>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>
#include <algorithm>

using std::stack;
using std::cin;
using std::cout;
using std::string;
using std::max;
using std::min;
using std::unordered_set;
using std::vector;


string POSITIVE_ANSWER = "YES";
string NEGATIVE_ANSWER = "NO";

string ALPHABET = "abc1";

bool is_alpha(char symbol) {
    for (char letter : ALPHABET) {
        if (symbol == letter) {
            return true;
        }
    }
    return false;
}

string OPERATIONS = ".+*";

bool is_operation(char symbol) {
    for (char operation : OPERATIONS) {
        if (symbol == operation) {
            return true;
        }
    }
    return false;
}

inf_int::inf_int(int number, const vector<int> &infinity_part): number(number), infinity_part(infinity_part) {}


bool operator==(const inf_int& left, const inf_int& right) {
    return (left.number == right.number && left.infinity_part == right.infinity_part);
}

size_t std::hash<inf_int>::operator()(const inf_int& inf_int1) const {
    size_t hash = std::hash<int>()(inf_int1.number);
    for (int i : inf_int1.infinity_part) {
        hash ^= std::hash<int>()(i);
    }
    return  hash;
}

vector<int> operator+ (const vector<int>& left_v, const vector<int>& right_v) {
    vector<int> result = left_v;
    for (int i : right_v) {
        result.push_back(i);
    }
    return result;
}

int apply_operation(char symbol, stack<string>& unhandled_expressions, stack<unordered_set<inf_int>>& occurrences_of_required_letter) {
    string left_operand, right_operand;
    unordered_set<inf_int> number_of_occurences_in_left, number_of_occurences_in_right, new_number_of_occurences;
    switch (symbol) {
        case '.': // 1-st operation

            // getting elements from stack
            right_operand = std::move(unhandled_expressions.top());
            unhandled_expressions.pop();
            left_operand = std::move(unhandled_expressions.top());
            unhandled_expressions.pop();
            number_of_occurences_in_left = std::move(occurrences_of_required_letter.top());
            occurrences_of_required_letter.pop();
            number_of_occurences_in_right = std::move(occurrences_of_required_letter.top());
            occurrences_of_required_letter.pop();

            // occurrences_recounting
            for (auto elem1 : number_of_occurences_in_left) {
                for (auto elem2  :number_of_occurences_in_right) {
                    new_number_of_occurences.insert(inf_int(elem1.number + elem2.number, elem1.infinity_part + elem2.infinity_part));
                }
            }

            // pushing on stack
            unhandled_expressions.push(left_operand + right_operand);
            occurrences_of_required_letter.push(std::move(new_number_of_occurences));
            break;
        case '+': // 2-nd operation

            // getting elements from stack
            right_operand = std::move(unhandled_expressions.top());
            unhandled_expressions.pop();
            left_operand = std::move(unhandled_expressions.top());
            unhandled_expressions.pop();
            number_of_occurences_in_left = std::move(occurrences_of_required_letter.top());
            occurrences_of_required_letter.pop();
            number_of_occurences_in_right = std::move(occurrences_of_required_letter.top());
            occurrences_of_required_letter.pop();

            // occurrences_recounting
            for (auto elem : number_of_occurences_in_left) {
                new_number_of_occurences.insert(std::move(elem));
            }
            for (auto elem : number_of_occurences_in_right) {
                new_number_of_occurences.insert(std::move(elem));
            }

            // pushing on stack
            unhandled_expressions.push(left_operand + '+' + right_operand);
            occurrences_of_required_letter.push(std::move(new_number_of_occurences));
            break;
        case '*': // 3-rd operation

            // getting elements from stack
            left_operand = std::move(unhandled_expressions.top());
            unhandled_expressions.pop();
            number_of_occurences_in_left = std::move(occurrences_of_required_letter.top());
            occurrences_of_required_letter.pop();

            // occurrences_recounting
            vector<int> temp;
            for (auto elem : number_of_occurences_in_left) {
                temp = elem.infinity_part;
                temp.push_back(elem.number);
                new_number_of_occurences.insert(inf_int(0, std::move(temp)));
            }

            // pushing on stack
            unhandled_expressions.push(left_operand + '*');
            occurrences_of_required_letter.push(std::move(new_number_of_occurences));
            break;
    }
    return 0;
}

bool recursive_finding_linear_comb(int recursive_var, const vector<int>& inf_part, int required, vector<int> iterators) {
    if (recursive_var == inf_part.size()) {
        int sum = 0;
        for (int j = 0; j < recursive_var; ++j) {
            sum += inf_part[j] * iterators[j];
        }
        return (sum == required);
    }
    if (!inf_part[recursive_var]) {
        return false;
    }
    for (int i = 0; i * inf_part[recursive_var] < required; ++i) {
        iterators.push_back(i);
        if (recursive_finding_linear_comb(recursive_var + 1, inf_part, required, iterators))
            return true;
        iterators.pop_back();
    }
    return false;
}

string counting_answer(char required_letter, int number_required_of_occurences, unordered_set<inf_int>& occurrences_of_required_letter) {
    for (const auto& elem : occurrences_of_required_letter) {
        if (elem.number == number_required_of_occurences) {
            return POSITIVE_ANSWER;
        }
    }
    int remainder;
    for (const auto& elem : occurrences_of_required_letter) {
        remainder = number_required_of_occurences - elem.number;
        if (recursive_finding_linear_comb(0, elem.infinity_part, remainder, vector<int>())) {
            return POSITIVE_ANSWER;
        }
    }
    return NEGATIVE_ANSWER;
}

bool valid(char symbol, int stack_size) {
    if (symbol == OPERATIONS[2] && stack_size < 1) {
        return false;
    }
    if (symbol != OPERATIONS[2] && stack_size < 2) {
        return false;
    }
    return true;
}

string common_part_in_main_algorithm(const string& regular_expression, char required_letter, int number_required_of_occurences) {
    stack<string> unhandled_expressions;
    stack<unordered_set<inf_int>> occurrences_of_required_letter;
    for (char symbol : regular_expression) {
        if (is_alpha(symbol)) {
            unhandled_expressions.push(string(1, symbol));
            occurrences_of_required_letter.emplace(unordered_set<inf_int>({inf_int((int)(symbol == required_letter), vector<int>())}));
        }
        else {
            if (is_operation(symbol)) {
                // checking for a valid input
                if (!valid(symbol, unhandled_expressions.size())) {
                    throw std::invalid_argument("not a valid input");
                }
                apply_operation(symbol, unhandled_expressions, occurrences_of_required_letter);
            }
            else {
                throw std::invalid_argument("symbol not found");
            }
        }
    }
    string answer;
    if (!unhandled_expressions.empty()) {
        answer = counting_answer(required_letter, number_required_of_occurences, occurrences_of_required_letter.top());
        unhandled_expressions.pop();
        occurrences_of_required_letter.pop();
    }
    else {
        throw std::invalid_argument("runtime-error (broken stack)");
    }
    if (!unhandled_expressions.empty()) {
        throw std::invalid_argument("not a valid input");
    }
    return answer;
}

void main_algorithm_interactive() {
    string regular_expression;
    cin >> regular_expression;
    char required_letter;
    cin >> required_letter;
    int number_required_of_occurences;
    cin >> number_required_of_occurences;

    cout << common_part_in_main_algorithm(regular_expression, required_letter, number_required_of_occurences) << '\n';
}

string main_algorithm_with_arguments(const string& regular_expression, char required_letter, int number_required_of_occurences) {
    return common_part_in_main_algorithm(regular_expression, required_letter, number_required_of_occurences);
}