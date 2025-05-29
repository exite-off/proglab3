#include "mystruct.h"
#include <cassert>
#include <iostream>
#include <deque>
#include <random>
#include <algorithm>
#include <stdexcept>

int main() {
    const int OPS = 10000;
    std::mt19937_64 rng(123456);
    std::deque<long long> reference;
    MyStruct s;

    for (int i = 0; i < OPS; ++i) {
        int op = rng() % 6;
        long long val = static_cast<long long>(rng() % 1000);

        switch (op) {
        case 0:
            s.push_back(val);
            reference.push_back(val);
            break;
        case 1:
            s.push_front(val);
            reference.push_front(val);
            break;
        case 2:
            if (!reference.empty()) {
                //cout << s.size() << endl;
                s.pop_back();
                reference.pop_back();
            }
            break;
        case 3:
            if (!reference.empty()) {
                s.pop_front();
                reference.pop_front();
            }
            break;
        case 4:
        {
            size_t pos = reference.empty() ? 0 : rng() % reference.size();
            auto it = s.begin();
            std::advance(it, pos);
            s.insert(it, val);
            reference.insert(reference.begin() + pos, val);
            break;
        }
       
        case 5:
            if (!reference.empty()) {
                size_t pos = rng() % reference.size();
                auto it = s.begin();
                std::advance(it, pos);
                s.erase(it);
                reference.erase(reference.begin() + pos);
            }
            break;
        }

        if (i % 1000 == 0) {
            assert(s.size() == reference.size());
            size_t idx = 0;
            for (auto it = s.begin(); it != s.end(); ++it, ++idx) {
                assert(*it == reference[idx]);
            }
        }
    }

    assert(s.size() == reference.size());
    {
        size_t idx = 0;
        for (auto x : s) {
            assert(x == reference[idx++]);
        }
    }

    s.shrink_to_fit();
    std::cout << "Stress test passed: " << OPS << " operations successfully verified." << std::endl;
    return 0;
}