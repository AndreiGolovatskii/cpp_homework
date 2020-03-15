#include <gtest/gtest.h>
#include "common/type_traits.h"

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

TEST(TRAITS, TEST4) {
    unsigned val = up_to_2_pow<1>::value;
    ASSERT_EQ(1, val);
}

TEST(TRAITS, TEST5) {
    unsigned val = up_to_2_pow<16>::value;
    ASSERT_EQ(16, val);
}

TEST(TRAITS, TEST6) {
    unsigned val = up_to_2_pow<19>::value;
    ASSERT_EQ(32, val);
}
