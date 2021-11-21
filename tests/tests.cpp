#include "../regexp_handling.h"
#include "tests.h"

TEST(SimpleTest, test1) {
    ASSERT_EQ(main_algorithm_with_arguments("ab.", 'a', 1), "YES");
}

TEST(SimpleTest, test2) {
    ASSERT_EQ(main_algorithm_with_arguments("ab.", 'a', 2), "NO");
}

TEST(SimpleTest, test3) {
    ASSERT_EQ(main_algorithm_with_arguments("ab.", 'b', 0), "NO");
}

TEST(SimpleTest, test4) {
    ASSERT_EQ(main_algorithm_with_arguments("b*", 'b', 0), "YES");
}

TEST(SimpleTest, test5) {
    ASSERT_EQ(main_algorithm_with_arguments("ab+", 'b', 0), "YES");
}

TEST(SimpleTest, test6) {
    ASSERT_EQ(main_algorithm_with_arguments("ab+*a+*", 'a', 8), "YES");
}


TEST(incorrect_input, test1) {
    ASSERT_ANY_THROW(main_algorithm_with_arguments("asdjhasdfk", 'a', 1));
}

TEST(incorrect_input, test2) {
    ASSERT_ANY_THROW(main_algorithm_with_arguments("abc.", 'a', 1));
}

TEST(incorrect_input, test3) {
    ASSERT_ANY_THROW(main_algorithm_with_arguments("a+", 'a', 1));
}


TEST(functions_test, test1) {
    ASSERT_EQ(is_alpha('a'), true);
    ASSERT_EQ(is_alpha('b'), true);
    ASSERT_EQ(is_alpha('c'), true);
    ASSERT_EQ(is_alpha('1'), true);
    ASSERT_EQ(is_alpha('d'), false);
    ASSERT_EQ(is_alpha('*'), false);
    ASSERT_EQ(is_alpha(';'), false);
}

TEST(functions_test, test2) {
    ASSERT_EQ(is_operation('.'), true);
    ASSERT_EQ(is_operation('+'), true);
    ASSERT_EQ(is_operation('*'), true);
    ASSERT_EQ(is_operation('a'), false);
    ASSERT_EQ(is_operation('^'), false);
}

TEST(functions_test, test3) {
    ASSERT_EQ(inf_int(1, vector<int>({2})) == inf_int(1, vector<int>({2})), true);
    ASSERT_EQ(inf_int(1, vector<int>({2, 3})) == inf_int(1, vector<int>({2, 3})), true);
    ASSERT_EQ(inf_int(1, vector<int>({2, 3})) == inf_int(1, vector<int>({2, 4})), false);
}

TEST(functions_test, test4) {
    ASSERT_EQ(vector<int>() + vector<int>(), vector<int>());
    ASSERT_EQ(vector<int>({1,2,3,4,5}) + vector<int>({5,4,3,2,1}), vector<int>({1,2,3,4,5,5,4,3,2,1}));
    ASSERT_EQ(vector<int>({5,4,3,2,1}) + vector<int>({1,2,3,4,5}), vector<int>({5,4,3,2,1,1,2,3,4,5}));
}

TEST(functions_test, test5) {
    // getting "(a+b)" from stack, required letter = 'b'
    string operand;
    stack<string> unhandled_expressions;
    unhandled_expressions.push("(a+b)");
    unordered_set<inf_int> number_of_occurences_in_operand;
    stack<unordered_set<inf_int>> occurrences_of_required_letter;
    unordered_set<inf_int> temp;
    temp.insert(inf_int(1));
    occurrences_of_required_letter.push(temp);
    initializing_operands(operand, unhandled_expressions, number_of_occurences_in_operand, occurrences_of_required_letter);
    ASSERT_EQ(operand, "(a+b)");
    ASSERT_TRUE(unhandled_expressions.empty());
    ASSERT_EQ(number_of_occurences_in_operand, temp);
    ASSERT_TRUE(occurrences_of_required_letter.empty());

    // testing empty stack
    ASSERT_ANY_THROW(initializing_operands(operand, unhandled_expressions, number_of_occurences_in_operand, occurrences_of_required_letter));
}

TEST(functions_test, test6) {
    // applying operations to "(a+b)" and "c", required = 'b'
    stack<string> unhandled_expressions;
    stack<unordered_set<inf_int>> occurrences_of_required_letter;

    string operand1 = "(a+b)";
    string operand2 = "c";
    unordered_set<inf_int> temp1;
    temp1.insert(inf_int(0));
    temp1.insert(inf_int(1));
    unordered_set<inf_int> temp2;
    temp2.insert(inf_int(0));

    // testing concatenation
    unhandled_expressions.push(operand1);
    unhandled_expressions.push(operand2);
    occurrences_of_required_letter.push(temp1);
    occurrences_of_required_letter.push(temp2);
    apply_operation('.', unhandled_expressions, occurrences_of_required_letter);
    ASSERT_EQ(unhandled_expressions.top(), "(a+b)c");
    unhandled_expressions.pop();
    unordered_set<inf_int> temp_set1; temp_set1.insert(inf_int(1)); temp_set1.insert(inf_int(0));
    ASSERT_EQ(occurrences_of_required_letter.top(), temp_set1);
    occurrences_of_required_letter.pop();

    // testing plus
    unhandled_expressions.push(operand1);
    unhandled_expressions.push(operand2);
    occurrences_of_required_letter.push(temp1);
    occurrences_of_required_letter.push(temp2);
    apply_operation('+', unhandled_expressions, occurrences_of_required_letter);
    ASSERT_EQ(unhandled_expressions.top(), "((a+b)+c)");
    unhandled_expressions.pop();
    unordered_set<inf_int> temp_set2; temp_set2.insert(inf_int(1)); temp_set2.insert(inf_int(0));
    ASSERT_EQ(occurrences_of_required_letter.top(), temp_set2);
    occurrences_of_required_letter.pop();

    // testing star
    unhandled_expressions.push(operand1);
    occurrences_of_required_letter.push(temp1);

    apply_operation('*', unhandled_expressions, occurrences_of_required_letter);
    ASSERT_EQ(unhandled_expressions.top(), "((a+b))*");
    unhandled_expressions.pop();
    unordered_set<inf_int> temp_set3; temp_set3.insert(inf_int(0, vector<int>({0}))); temp_set3.insert(inf_int(0, vector<int>({1})));
    ASSERT_EQ(occurrences_of_required_letter.top(), temp_set3);
    occurrences_of_required_letter.pop();

    // testing empty stacks
    ASSERT_ANY_THROW(apply_operation('.', unhandled_expressions, occurrences_of_required_letter));
}

TEST(functions_test, test7) {
    int NUMBER_TESTS = 4;
    vector<inf_int> result(NUMBER_TESTS);
    vector<int> required(NUMBER_TESTS);
    vector<int> remainder(NUMBER_TESTS);

    int test = 0;
    result[test] = inf_int(0);
    required[test] = 1;
    remainder[test] = required[test] - result[test].number;
    ASSERT_FALSE(recursive_finding_linear_comb(0, result[test].infinity_part, remainder[test], vector<int>()));

    test = 1;
    result[test] = inf_int(1);
    required[test] = 1;
    remainder[test] = required[test] - result[test].number;
    ASSERT_TRUE(recursive_finding_linear_comb(0, result[test].infinity_part, remainder[test], vector<int>()));

    test = 2;
    result[test] = inf_int(0, vector<int>({7,3}));
    required[test] = 19;
    remainder[test] = required[test] - result[test].number;
    ASSERT_TRUE(recursive_finding_linear_comb(0, result[test].infinity_part, remainder[test], vector<int>()));

    test = 3;
    result[test] = inf_int(0, vector<int>({7,3}));
    required[test] = 5;
    remainder[test] = required[test] - result[test].number;
    ASSERT_FALSE(recursive_finding_linear_comb(0, result[test].infinity_part, remainder[test], vector<int>()));
}

TEST(functions_test, test8) {
    int number_required_of_occurences;
    unordered_set<inf_int> occurrences_of_required_letter;

    number_required_of_occurences = 1;
    occurrences_of_required_letter.insert(inf_int(1));
    ASSERT_EQ(counting_answer('a', number_required_of_occurences, occurrences_of_required_letter), "YES");
    occurrences_of_required_letter.clear();

    number_required_of_occurences = 1;
    occurrences_of_required_letter.insert(inf_int(0, vector<int>({1})));
    ASSERT_EQ(counting_answer('a', number_required_of_occurences, occurrences_of_required_letter), "YES");
    occurrences_of_required_letter.clear();

    number_required_of_occurences = 111;
    occurrences_of_required_letter.insert(inf_int(11, vector<int>({33})));
    ASSERT_EQ(counting_answer('a', number_required_of_occurences, occurrences_of_required_letter), "NO");
    occurrences_of_required_letter.clear();
}

TEST(functions_test, test9) {
    ASSERT_TRUE(is_valid('.', 2));
    ASSERT_FALSE(is_valid('.', 1));
    ASSERT_TRUE(is_valid('*', 1));

}