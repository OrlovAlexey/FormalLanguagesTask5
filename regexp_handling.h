#pragma once

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



bool is_alpha(char symbol);

bool is_operation(char symbol);

struct inf_int {
    int number;
    vector<int> infinity_part;
    inf_int(int number, const vector<int>& infinity_part);
};

bool operator==(const inf_int& left, const inf_int& right);

template <>
class std::hash<inf_int> {
public:
    size_t operator()(const inf_int& inf_int1) const;
};

vector<int> operator+ (const vector<int>& left_v, const vector<int>& right_v);

int apply_operation(char symbol, stack<string>& unhandled_expressions, stack<unordered_set<inf_int>>& occurrences_of_required_letter);

bool recursive(int recursive_var, const vector<int>& inf_part, int required, vector<int> iterators);

string counting_answer(char required_letter, int number_required_of_occurences, unordered_set<inf_int>& occurrences_of_required_letter);

bool valid(char symbol, int stack_size);

string common_part_in_main_algorithm(const string& regular_expression, char required_letter, int number_required_of_occurences);

void main_algorithm_interactive();

string main_algorithm_with_arguments(const string& regular_expression, char required_letter, int number_required_of_occurences);