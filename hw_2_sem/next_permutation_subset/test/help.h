#include <string>
#include <gtest/gtest.h>
#include <vector>
#include <random>

std::mt19937 rnd(17);

size_t RandomIndex(size_t size) {
    return rnd() % size;
}

size_t RandomInt() {
    return rnd();
}

template<class U, class P>
void RandomInsert(U& s, P& v) {
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
void RandomSubsegementValuesCheck(U& s, P& v) {
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

    int s_min = s.SubsegmentMin(start, len);
    int v_min = SegmentMin(v, start, len);
    ASSERT_EQ(s_min, v_min); 

    int s_max = s.SubsegmentMax(start, len);
    int v_max = SegmentMax(v, start, len);
    ASSERT_EQ(s_max, v_max); 
}

template<class T>
void SubsegmentSet(T& v, size_t start, size_t len, int val) {
    for (size_t i = 0; i < len; ++i) {
        v[i + start] = val;
    }
}

template<class T>
void SubsegmentAdd(T& v, size_t start, size_t len, int val) {
    for (size_t i = 0; i < len; ++i) {
        v[i + start] += val;
    }
}

template<class U, class P>
void RandomSubsegementSetValue(U& s, P& v) {
    ASSERT_EQ(v.size(), s.size());
    size_t size = s.size();
    int val = rnd();

    size_t start = rnd() % size;
    size_t finish = rnd() % size;
   
    if (finish < start) {
        std::swap(finish, start);
    }

    size_t len = finish - start + 1;

    s.SubsegmentSet(start, len, val);
    SubsegmentSet(v, start, len, val);
}

template<class U, class P>
void RandomSubsegementAddValue(U& s, P& v) {
    ASSERT_EQ(v.size(), s.size());
    size_t size = s.size();
    int val = rnd();

    size_t start = rnd() % size;
    size_t finish = rnd() % size;
   
    if (finish < start) {
        std::swap(finish, start);
    }

    size_t len = finish - start + 1;

    s.SubsegmentAdd(start, len, val);
    SubsegmentAdd(v, start, len, val);
}

template<class U, class T>
void RandomErase(U& s, T& v) {
    ASSERT_EQ(s.size(), v.size());
    size_t idx = RandomIndex(v.size());
    
    s.Erase(idx);
    v.erase(v.begin() + idx);
}

template<class U, class T>
void AssertEqual(U& s, T& v) {
    ASSERT_EQ(s.size(), v.size());
    
    for (size_t i = 0; i < v.size(); ++i) {
        ASSERT_EQ(s[i], v[i]);
    }
}
