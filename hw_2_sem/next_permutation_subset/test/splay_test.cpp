#include <gtest/gtest.h>
#include "splay/splay.h"
#include "help.h"


TEST(Splay, simple_1) {
    TSplayTree<int64_t> o;
    TSplayTree<int64_t> s(10);
    
    s[0]++;
    ASSERT_EQ(s[0], 1);
    s[1] = 16;
    ASSERT_EQ(s[1], 16);
    s.Insert(0, 10);
}

TEST(Splay, simple_2) {
    size_t SIZE = 10;
    TSplayTree<int64_t> s(SIZE, 0);
    std::vector<int64_t> v(SIZE, 0);

    for (size_t i = 0; i < SIZE; ++i) {
        s[i] = v[i] = i + 1;
    }
    
    s.SubsegmentNextPermutation(0, 5);
    std::next_permutation(v.begin(), v.begin() + 5);
    for (size_t i = 0; i < SIZE; ++i) {
        ASSERT_EQ(s[i], v[i]);
    }

    s.SubsegmentPrevPermutation(0, 5);
    std::prev_permutation(v.begin(), v.begin() + 5);
    for (size_t i = 0; i < SIZE; ++i) {
        ASSERT_EQ(s[i], v[i]);
    }

    s.SubsegmentPrevPermutation(0, 5);
    std::prev_permutation(v.begin(), v.begin() + 5);
    for (size_t i = 0; i < SIZE; ++i) {
        ASSERT_EQ(s[i], v[i]);
    }

    s.SubsegmentNextPermutation(0, 1);
    std::prev_permutation(v.begin(), v.begin() + 1);
    for (size_t i = 0; i < SIZE; ++i) {
        ASSERT_EQ(s[i], v[i]);
    }
}

TEST(Splay, simple_3) {
    size_t SIZE = 3;
    TSplayTree<int64_t> s(SIZE, 0);
    std::vector<int64_t> v(SIZE, 0);

    for (size_t i = 0; i < SIZE; ++i) {
        s[i] = v[i] = i + 1;
    }
    s[0] = s[1];
    v[0] = v[1];
    
    int cnt = 3 * 2 * 1 + 1;
    for (int i = 0; i < cnt; ++i) {
        s.SubsegmentNextPermutation(0, SIZE);
        std::next_permutation(v.begin(), v.begin() + SIZE);
        AssertEqual(s, v);
    }
    for (int i = 0; i < cnt; ++i) {
        s.SubsegmentPrevPermutation(0, SIZE);
        std::prev_permutation(v.begin(), v.begin() + SIZE);
        AssertEqual(s, v);
    }
}

TEST(Splay, simple_4) {
    size_t SIZE = 4;
    TSplayTree<int64_t> s(SIZE, 0);
    std::vector<int64_t> v(SIZE, 0);

    for (size_t i = 0; i < SIZE; ++i) {
        s[i] = v[i] = i + 1;
    }
    s[0] = s[1];
    v[0] = v[1];
    
    int cnt = 4 * 3 * 2 * 1 + 1;
    for (int i = 0; i < cnt; ++i) {
        s.SubsegmentNextPermutation(0, SIZE);
        std::next_permutation(v.begin(), v.begin() + SIZE);
        AssertEqual(s, v);
    }
    for (int i = 0; i < cnt; ++i) {
        s.SubsegmentPrevPermutation(0, SIZE);
        std::prev_permutation(v.begin(), v.begin() + SIZE);
        AssertEqual(s, v);
    }
}
TEST(Splay, random_access) {
    const size_t SIZE = 1e3;
    TSplayTree<int64_t> s(SIZE, 1);
    std::vector<int64_t> v(SIZE, 1);
    
    ASSERT_EQ(s.size(), SIZE);
    for (size_t i = 0; i < 10 * SIZE; ++i) {
        size_t id = RandomIndex(SIZE);
        int64_t val = RandomInt();

        s[id] += val;
        v[id] += val;

        ASSERT_EQ(s.size(), SIZE);
        ASSERT_EQ(s[id], v[id]);
    }
}

TEST(Splay, insert_delete) {
    const size_t SIZE = 1e3;

    TSplayTree<int64_t> s(SIZE, 1);
    std::vector<int64_t> v(SIZE, 1);

    for (size_t i = 0; i < 10 * SIZE; ++i) {
        RandomInsert(s, v);
        RandomInsert(s, v);
        RandomErase(s, v);
    }
    AssertEqual(s, v);
}

TEST(Splay, segmet_values) {
    const size_t SIZE = 1e3;

    TSplayTree<int64_t> s(SIZE, 1);
    std::vector<int64_t> v(SIZE, 1);

    for (size_t i = 0; i < 10 * SIZE; ++i) {
        RandomInsert(s, v);
        RandomSubsegementValuesCheck(s, v);
    }
    AssertEqual(s, v);
}

TEST(Splay, segement_set_add_small) {
    const size_t SIZE = 8;

    TSplayTree<int64_t> s(SIZE, 1);
    std::vector<int64_t> v(SIZE, 1);

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

    TSplayTree<int64_t> s(SIZE, 1);
    std::vector<int64_t> v(SIZE, 1);

    for (size_t i = 0; i < 10 * SIZE; ++i) {
        RandomInsert(s, v);
        RandomSubsegementSetValue(s, v);
        RandomSubsegementAddValue(s, v);
        RandomSubsegementValuesCheck(s, v);
    }
}

TEST(Splay, segement_set_add_reverse_small) {
    const size_t SIZE = 13;

    TSplayTree<int64_t> s(SIZE, 1);
    std::vector<int64_t> v(SIZE, 1);

    for (size_t i = 0; i < 10 * SIZE; ++i) {
        RandomInsert(s, v);
        RandomSubsegementSetValue(s, v);
        RandomSubsegementAddValue(s, v);
        RandomSubsegementReverse(s, v);
        AssertEqual(s, v);
        RandomSubsegementValuesCheck(s, v);
        RandomSubsegementValuesCheck(s, v);
        RandomSubsegementValuesCheck(s, v);
    }
}

TEST(Splay, segement_set_add_reverse_small_2) {
    const size_t SIZE = 3;

    TSplayTree<int64_t> s(SIZE, 1);
    std::vector<int64_t> v(SIZE, 1);
    for (int i = 0; i < 10; ++i) {
        RandomInsert(s, v);
    }

    for (size_t i = 0; i < 10 * SIZE; ++i) {
        RandomSubsegementSetValue(s, v);
        RandomSubsegementAddValue(s, v);
        RandomSubsegementReverse(s, v);
        AssertEqual(s, v);
        RandomSubsegementValuesCheck(s, v);
        RandomSubsegementValuesCheck(s, v);
        RandomSubsegementValuesCheck(s, v);
    }
}

TEST(Splay, segement_set_add_reverse_small_3) {
    const size_t SIZE = 13;

    TSplayTree<int64_t> s(SIZE, 1);
    std::vector<int64_t> v(SIZE, 1);

    for (size_t i = 0; i < 10 * SIZE; ++i) {
        RandomSubsegementSetValue(s, v);
        RandomSubsegementAddValue(s, v);
        RandomSubsegementReverse(s, v);
        RandomSubsegementValuesCheck(s, v);
        RandomSubsegementValuesCheck(s, v);
        RandomSubsegementValuesCheck(s, v);
    }
}

TEST(Splay, segement_set_add_permutation_small) {
    const size_t SIZE = 6;

    TSplayTree<int64_t> s(SIZE, 1);
    std::vector<int64_t> v(SIZE, 1);

    for (size_t i = 0; i < 10 * SIZE; ++i) {
        RandomSubsegementSetValue(s, v, 10);
        RandomSubsegementAddValue(s, v, 10);

        RandomSubsegementNextPermutation(s, v);
        AssertEqual(s, v);
        RandomSubsegementPrevPermutation(s, v);
        AssertEqual(s, v);
    }
}

TEST(Splay, segement_set_add_reverse_large) {
    const size_t SIZE = 1e3;

    TSplayTree<int64_t> s(SIZE, 1);
    std::vector<int64_t> v(SIZE, 1);

    for (size_t i = 0; i < 10 * SIZE; ++i) {
        RandomInsert(s, v);
        RandomSubsegementSetValue(s, v);
        RandomSubsegementAddValue(s, v);
        RandomSubsegementReverse(s, v);
        RandomSubsegementValuesCheck(s, v);
        RandomSubsegementValuesCheck(s, v);
        RandomSubsegementValuesCheck(s, v);
    }
}

TEST(Splay, sample) {
    TSplayTree<int64_t> s(7); 
    for (int i = 0; i < 7; ++i) {
        s[i] = i + 1;
    }

    s.SubsegmentSet(1, 3 - 1 + 1, 5);
    s.Insert(3, 3);
    s.SubsegmentAdd(0, 4 - 0 + 1, 2);
    s.SubsegmentPrevPermutation(0, 6 - 0 + 1);
    s.SubsegmentNextPermutation(0, 3 - 0 + 1);
    s.Erase(2);
}
