#include <iostream>
#include "mystruct.h"

int main() {
    MyStruct s;
    s.push_back(1);
    s.push_back(2);
    s.push_back(3);

    MyStruct::iterator it2 = s.begin();
    for (int i {0}; i < 3; ++i) {
        std::cout << *it2 << " ";
        it2++;
    }
    std::cout << "cap - " << s.capacity() << " size_var = " << s.size() << "\n";
    //
    // s.insert(s.begin(), 4);
    // for (auto& i : s) {
    //     std::cout << i << " ";
    // }
    // std::cout << "cap - " << s.capacity() << " size_var = " << s.size() << "\n";
    //
    // s.clear();
    // s.push_back(5);
    // for (auto& i : s) {
    //     std::cout << i << " ";
    // }
    // std::cout << "cap - " << s.capacity() << " size_var = " << s.size() << "\n";
    //
    // s.erase(s.begin());
    // for (auto& i : s) {
    //     std::cout << i << " ";
    // }
    // std::cout << "cap - " << s.capacity() << " size_var = " << s.size() << "\n";
    //
    // for (int i = 0; i < 1000; ++i) {
    //     s.push_back(i);
    // }
    // std::cout << "cap - " << s.capacity() << " size_var = " << s.size() << "\n";
    // s.clear();
    // std::cout << "cap - " << s.capacity() << " size_var = " << s.size() << "\n";
    //
    // MyStruct s2{1, 2, 3, 4, 5};
    // for (int i = 0; i < 62; ++i) {
    //     s2.push_back(i);
    // }
    // std::cout << "cap - " << s2.capacity() << " size_var = " << s2.size() << "\n";
    // auto it = s2.begin();
    // for (int i = 0; i < 12; ++i) {
    //     s2.erase(it);
    //     ++it;
    // }
    // for (auto& i : s2) {
    //     std::cout << i << " ";
    // }
    // std::cout << "cap - " << s2.capacity() << " size_var = " << s2.size() << "\n";
    // s2.shrink_to_fit();
    // std::cout << "cap - " << s2.capacity() << " size_var = " << s2.size() << "\n";
    // s.shrink_to_fit();
    // std::cout << "cap - " << s.capacity() << " size_var = " << s.size() << "\n";

    // MyStruct str {1, 2, 3, 4, 5};
    // std::cout << "cap - " << str.capacity() << " size_var = " << str.size() << "\n";
    // try {
    //     str.erase(str.begin());
    // } catch (std::out_of_range& e) {
    //     std::cout << e.what() << "\n";
    // }
    // for (auto it = str.begin(); it != str.end(); ++it) {
    //     *it = 1;
    // }
    // for (auto& i : str) {
    //     std::cout << i << " ";
    // }
    // std::cout << "cap - " << str.capacity() << " size_var = " << str.size() << " " << str.empty() << "\n";
    // str.clear();
    // std::cout << str.empty();

    MyStruct str;
    MyStruct::const_iterator it = str.cbegin();
    str.insert(it, 1);
    str.push_back(3);
    str.push_back(4);
    str.push_back(5);
    for (auto& i : str) {
        std::cout << i << " ";
    }
    std::cout << "\ncap - " << str.capacity() << " size_var = " << str.size() << "\n";
    MyStruct::const_iterator it3 = str.insert(it, 2);
    it3++;
    ++it3;
    std::cout << "iterator on inserted el (1): " << *it3 << "\n";
    std::cout << "iterator on inserted el (2): " << *it3 << "\n";
    for (auto& i : str) {
        std::cout << i << " ";
    }
    std::cout << "\ncap - " << str.capacity() << " size_var = " << str.size() << "\n";
    str.clear();
    std::cout << "\ncap - " << str.capacity() << " size_var = " << str.size() << "\n";
    return 0;
}
