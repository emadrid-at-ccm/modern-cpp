#include <random>
#include <chrono>

#include <vector>

#include <algorithm>

#include <utility>

#include <assert.h>

template<typename C, typename... Args>
long benchmark(const C &c, Args &&... args) {
    auto begin = std::chrono::high_resolution_clock::now();
    c(std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    return diff.count();
}


template<typename It> int bse(It from, It to, It rb, It re) {
    auto acc = 0;
    while(rb != re) {
        auto p = std::lower_bound(from, to, *rb);
        acc += (p - from);
        ++rb;
    }
    return acc;
}

template<typename T> T log2plus(T val) {
    unsigned long long v = val;
    static constexpr auto bits = sizeof(v) * 8;
    auto clz = __builtin_clzll(v);
    return bits - clz;
}

struct CacheFriendlyBinarySearch {
    inline CacheFriendlyBinarySearch(std::vector<int> &input);

    unsigned lower_bound(int value) const;
private:
    std::vector<int> lookup;
    int straddle;
};

std::vector<int> convert(std::vector<int> &input) {
    auto size = input.size();
    auto log = log2plus(size);
    assert(size == (1 << log) - 1);
    std::vector<int> rv(size);

    auto straddle = 1 << log;
    auto count = 1;
    auto ndx = 0;
    while(1 < straddle) {
        auto newStraddle = straddle/2;
        auto current = newStraddle - 1;
        for(auto i = count;;) {
            rv[ndx] = input[current];
            ++ndx;
            if(!--i) { break; }
            current += straddle;
        }
        count *= 2;
        straddle = newStraddle;
    }
    return rv;
}

CacheFriendlyBinarySearch::CacheFriendlyBinarySearch(std::vector<int> &input):
    lookup(convert(input)), straddle{1 << ((log2plus(input.size())) - 2)}
{}

unsigned CacheFriendlyBinarySearch::lower_bound(int value) const {
    auto s = straddle;
    auto lndx = 0, cndx = 2*s - 1;
    for(;;) {
        if(value <= lookup[lndx]) {
            if(0 == s) { return cndx; }
            lndx = lndx * 2 + 1;
            cndx -= s;
        } else {
            if(0 == s) { return cndx + 1; }
            lndx = lndx * 2 + 2;
            cndx += s;
        }
        s /= 2;
    }
    return cndx;
}

#include <iostream>
#include <memory>
#include <stdlib.h>

void swap(int &l, int &r) {
    auto tmp = l;
    l = r;
    r = tmp;
}

int maxdepth = 0;

#include <array>

template<typename I>
I partition_noRandomAccess(I pivot, I b, I e) {
    while(b != e) {
        // ..., P == *pivot, G0, G1, ..., Gn, L == *b
        if(*b < *pivot) {
            // target: ..., L, P == *pivot, G1, ...., G0 == *b
            swap(*pivot, *b);
            // ..., L == *pivot, G0, G1, ..., Gn, P == *b
            swap(*++pivot, *b);
        }
        ++b;
    }
    return pivot;
}

template<typename I>
I partition_noRandomAccessRotate(I pivot, I b, I e) {
    while(b != e) {
        if(*b < *pivot) {
            // pivot <- b, b <- (pivot + 1), (pivot + 1) <- pivot
            auto tmp = *pivot;
            *pivot = *b;
            *b = *++pivot;
            *pivot = tmp;
        }
        ++b;
    }
    return pivot;    
}

template<typename I, typename ParFun>
void quicksortNoRandomAccess_impl(I begin, I end, ParFun pf) {
    auto bPtr = &*begin;
    struct Frame {
        I b, e;
    };
    std::array<Frame, 50> frames;
    int index = 0;
    frames[0].b = begin;
    frames[0].e = end;
    maxdepth = 0;
    for(;;) {
        auto &frame = frames[index];
        auto b = frame.b;
        auto e = frame.e;

        auto oldB = b;
        auto oldBPtr = &*oldB;
        if(b != e) {
            auto pivot = b++;
            if(b != e) {
                //pivot = partition_noRandomAccess(pivot, b, e);
                pivot = pf(pivot, b, e);
                ++index;
                if(maxdepth < index) { maxdepth = index; }
                if(50 <= index) { throw std::runtime_error("Exhausted"); }
                frames[index].b = oldB;
                frames[index].e = pivot;
                ++pivot;
                frame.b = pivot;
                continue;
            }
        }
        if(!index--) { break; }
    }
}


template<typename I>
void quicksortNoRandomAccess(I begin, I end) {
    quicksortNoRandomAccess_impl(begin, end, partition_noRandomAccess<I>);
}

template<typename I>
void quicksort(I b, I e) {
    auto oldB = b;
    if(b == e) { return; }
    auto pivot = b++;
    if(b == e) { return; }
    pivot = partition_noRandomAccess(pivot, b, e);
    quicksort(oldB, pivot);
    ++pivot;
    quicksort(pivot, e);
}

int main(int argc, const char *argv[]) {
    std::random_device rdevice;
    std::mt19937 gen(rdevice());
    std::uniform_int_distribution<int> dist(0, (1 << 24) - 1); // 16 M
    std::vector<int> sample, lookupV;

    auto constexpr size = (1 << 24) - 1;
    auto constexpr lookCount = 1 << 20;

    for(auto i = size; i--; ) {
        sample.push_back(dist(gen));
        
    }
    for(auto i = lookCount; i--; ) {
        lookupV.push_back(dist(gen));
    }
    auto copy = sample;

    void (*sf)(int *, int *) = std::sort;
    auto stdsort = benchmark(sf, &copy[0], &copy[0] + copy.size());

    auto recopy = sample;

    auto qsort1 = benchmark(
        qsort, recopy.data(), recopy.size(), sizeof(int),
        [](const void *l, const void *r)->int { return *(int *)l - *(int *)r; }
    );

    copy = sample;
    auto qsnra = [](int *l, int *r) {
        quicksortNoRandomAccess(l, r);
    };
    auto noRandomSort = benchmark(
        qsnra, &copy[0], &copy[0] + copy.size()
    );
    copy = sample;
    auto rotate = benchmark(
        quicksortNoRandomAccess_impl<int *, decltype(partition_noRandomAccessRotate<int *>)>,
        &copy[0], &copy[0] + copy.size(), partition_noRandomAccessRotate<int *>
    );


    std::cout << maxdepth << std::endl;

    copy = sample;
    auto recursiveImpl = benchmark(
        quicksort<int *>, &copy[0], &copy[0] + copy.size()
    );

    auto acc2 = 0;
    auto search1m =
        benchmark(
            [&](){
                acc2 = bse(copy.begin(), copy.end(), lookupV.begin(), lookupV.begin() + lookCount);
            }
        );

    std::unique_ptr<CacheFriendlyBinarySearch> ptr;
    auto conversion = benchmark(
        [&]() { ptr.reset(new CacheFriendlyBinarySearch{copy}); }
    );
    CacheFriendlyBinarySearch &cfbs = *ptr;

    auto acc = 0;
    auto cfbsf = [&]() {
        for(unsigned i = lookCount; i--; ) {
            acc += cfbs.lower_bound(lookupV[i]);
        }
        return acc;
    };

    auto cf = benchmark(cfbsf);
    std::cout << stdsort << ' ' << qsort1 << ' ' << noRandomSort << ' ' << rotate << ' ' << recursiveImpl << ' ' << conversion << ' ' << search1m << ' ' << cf << ' ' << acc << ':' << acc2 << std::endl;
    return 0;
}
