#include <gtest/gtest.h>
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

TEST(incorrect_input, test1) {
    ASSERT_ANY_THROW(main_algorithm_with_arguments("asdjhasdfk", 'a', 1));
}

TEST(incorrect_input, test2) {
    ASSERT_ANY_THROW(main_algorithm_with_arguments("abc.", 'a', 1));
}
