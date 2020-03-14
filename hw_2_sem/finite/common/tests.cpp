#include <gtest/gtest.h>
#include "type_traits.h"
#include <iostream>

TEST(TRAITS, TEST1) {
    bool val = is_prime<17>::value;
    ASSERT_EQ(true, val);
}

TEST(TRAITS, TEST2) {
    bool val = is_prime<15>::value;
    ASSERT_EQ(false, val);
}
 
TEST(TRAITS, TEST3) {
    bool val = is_prime<1>::value;
    ASSERT_EQ(false, val);
}
 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
