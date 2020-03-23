#include <gtest/gtest.h>
#include <vector>
#include <random>
#include "splay/splay.h"

TEST(Splay, simple_1) {
    TSplayTree<int> o;
    TSplayTree<int> s(10);
    
    s[0]++;
    ASSERT_EQ(s[0], 1);
    s[1] = 16;
    ASSERT_EQ(s[1], 16);
    s.Insert(0, 10);
}

template<class U, class P>
void RandomInsert(U& s, P& v, std::mt19937& rnd) {
    ASSERT_EQ(v.size(), s.size());
    size_t size = s.size();

    size_t id = rnd() % size;
    int value = rnd(); 

    s.Insert(id, value);
    v.insert(v.begin() + id, value);
    ASSERT_EQ(v[id], s[id]);
}

template<class T>
int SegmentSum(T& v, size_t start, size_t len) {
    int res = 0;
    for (size_t i = 0; i < len; ++i) {
        res += v[start + i];
    }
    return res;
}

template<class T>
int SegmentMin(T& v, size_t start, size_t len) {
    int res = v[start];
    for (size_t i = 0; i < len; ++i) {
        res = std::min(v[start + i], res);
    }
    return res;
}

template<class T>
int SegmentMax(T& v, size_t start, size_t len) {
    int res = v[start];
    for (size_t i = 0; i < len; ++i) {
        res = std::max(v[start + i], res);
    }
    return res;
}

template<class U, class P>
void RandomSubsegementValuesCheck(U& s, P& v, std::mt19937& rnd) {
    ASSERT_EQ(v.size(), s.size());
    size_t size = s.size();

    size_t start = rnd() % size;
    size_t finish = rnd() % size;
   
    if (finish < start) {
        std::swap(finish, start);
    }

    size_t len = finish - start + 1;

    int s_sum = s.SubsegmentSum(start, len);
    int v_sum = SegmentSum(v, start, len);
    ASSERT_EQ(s_sum, v_sum); 

    return;
    int s_min = s.SubsegmentMin(start, len);
    int v_min = SegmentMin(v, start, len);
    ASSERT_EQ(s_min, v_min); 

    int s_max = s.SubsegmentMax(start, len);
    int v_max = SegmentMax(v, start, len);
    ASSERT_EQ(s_max, v_max); 
}

TEST(Splay, random_access) {
    const size_t SIZE = 1e3;
    std::mt19937 rnd(SIZE);
    TSplayTree<int> s(SIZE, 1);
    std::vector<int> v(SIZE, 1);
    
    ASSERT_EQ(s.size(), SIZE);
    for (size_t i = 0; i < 10 * SIZE; ++i) {
        size_t id = rnd() % SIZE;
        int val = rnd();

        s[id] += val;
        v[id] += val;

        ASSERT_EQ(s.size(), SIZE);
        ASSERT_EQ(s[id], v[id]);
    }

    ASSERT_EQ(s.size(), SIZE);
}

TEST(Splay, insert_delete) {
    const size_t SIZE = 1e3;
    std::mt19937 rnd(SIZE);

    TSplayTree<int> s(SIZE, 1);
    std::vector<int> v(SIZE, 1);

    for (size_t i = 0; i < 10 * SIZE; ++i) {
        for (int j = 0; j < 3; ++j) {
            ASSERT_EQ(v.size(), s.size());
            size_t size = s.size();
            size_t id = rnd() % size;
            int val = rnd();

            s.Insert(id, val);
            v.insert(v.begin() + id, val);

            ASSERT_EQ(v[id], s[id]);
        } 
        for (int j = 0; j < 2; ++j) {
            ASSERT_EQ(v.size(), s.size());
            size_t size = s.size();
            size_t id = rnd() % size;

            ASSERT_EQ(v[id], s[id]);
            s.Erase(id);
            v.erase(v.begin() + id);
        } 
    }
}

TEST(Splay, segemet_values) {
    const size_t SIZE = 13;
    std::mt19937 rnd(SIZE);

    TSplayTree<int> s(SIZE, 1);
    std::vector<int> v(SIZE, 1);

    for (size_t i = 0; i < 10 * SIZE; ++i) {
        RandomInsert(s, v, rnd);
        RandomSubsegementValuesCheck(s, v, rnd);
    }
}

