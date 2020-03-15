#include <gtest/gtest.h>
#include "finite/finite.h"

TEST(FINITE, TEST1) {
    //Finite<0> a; compile err
    Finite<17> a(1);
    Finite<17> r = a.Reverse();
   
    ASSERT_EQ(r.Representative(), 1);

    Finite<15> b(1);
    //b.reverse(); no member reverse
    // b / b; compile err, deleter operator
    Finite<17> t(11);
    Finite<17> y(13);
 
    ASSERT_EQ(13, (Finite<17>(11) * y / t).Representative());
}
