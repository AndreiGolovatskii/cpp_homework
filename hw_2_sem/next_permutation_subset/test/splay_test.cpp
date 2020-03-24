#include <gtest/gtest.h>
#include "splay/splay.h"
#include "help.h"


TEST(Splay, simple_1) {
    TSplayTree<int> o;
    TSplayTree<int> s(10);
    
    s[0]++;
    ASSERT_EQ(s[0], 1);
    s[1] = 16;
    ASSERT_EQ(s[1], 16);
    s.Insert(0, 10);
}


TEST(Splay, random_access) {
    const size_t SIZE = 1e3;
    TSplayTree<int> s(SIZE, 1);
    std::vector<int> v(SIZE, 1);
    
    ASSERT_EQ(s.size(), SIZE);
    for (size_t i = 0; i < 10 * SIZE; ++i) {
        size_t id = RandomIndex(SIZE);
        int val = RandomInt();

        s[id] += val;
        v[id] += val;

        ASSERT_EQ(s.size(), SIZE);
        ASSERT_EQ(s[id], v[id]);
    }
}

TEST(Splay, insert_delete) {
    const size_t SIZE = 1e3;

    TSplayTree<int> s(SIZE, 1);
    std::vector<int> v(SIZE, 1);

    for (size_t i = 0; i < 10 * SIZE; ++i) {
        RandomInsert(s, v);
        RandomInsert(s, v);
        RandomErase(s, v);
    }
    AssertEqual(s, v);
}

TEST(Splay, segmet_values) {
    const size_t SIZE = 1e3;

    TSplayTree<int> s(SIZE, 1);
    std::vector<int> v(SIZE, 1);

    for (size_t i = 0; i < 10 * SIZE; ++i) {
        RandomInsert(s, v);
        RandomSubsegementValuesCheck(s, v);
    }
    AssertEqual(s, v);
}

TEST(Splay, segement_set_add_small) {
    const size_t SIZE = 13;

    TSplayTree<int> s(SIZE, 1);
    std::vector<int> v(SIZE, 1);

    for (size_t i = 0; i < 10 * SIZE; ++i) {
        RandomInsert(s, v);
        RandomSubsegementSetValue(s, v);
        RandomSubsegementAddValue(s, v);
        RandomSubsegementValuesCheck(s, v);
        AssertEqual(s, v);
    }
}

TEST(Splay, segement_set_add_large) {
    const size_t SIZE = 1e3;

    TSplayTree<int> s(SIZE, 1);
    std::vector<int> v(SIZE, 1);

    for (size_t i = 0; i < 10 * SIZE; ++i) {
        RandomInsert(s, v);
        RandomSubsegementSetValue(s, v);
        RandomSubsegementAddValue(s, v);
        RandomSubsegementValuesCheck(s, v);
    }
}
