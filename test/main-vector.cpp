/* 
 * Copyright (c) 2017 The Gupta Empire - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Aryan Gupta <me@theguptaempire.net>
 * 
 * =============================================================================
 * @author 			Aryan Gupta
 * @project 		
 * @title 			
 * @date			(YYYY-MM-DD)
 * @fversion		1.0.0
 * @description 	
 * =============================================================================
 */
#include "info.h"

#include <iostream>
#include <vector>
#include <algorithm>

#include "vector.hpp"

using namespace std;

template<typename DS>
void output(DS& p) {
	for(size_t i = 0; i < p.size(); ++i)
		cout << p[i] << " ";
	cout << endl;
	cout << "Size: " << p.size() << endl;
} 

struct Copy {
	Copy() { cout << "Im being defult constructed\n"; }
	Copy(int a, int b, int c) : _a(a), _b(b), _c(c) { cout << "Im being constructed\n"; }
	Copy(const Copy& o) { cout << "Im being copy constructed\n"; }
	Copy(Copy&& o) { cout << "Im being move constructed\n";	} // This is move semantics. I might do
	// a tutorial of this later (it is a BIG topic of C++)
	
	Copy& operator=(const Copy& other) { cout << "Im being copy assigned\n"; return *this; }
	Copy& operator=(Copy&& other) { cout << "Im being move assigned\n"; return *this; }
	
	int _a, _b, _c;
};

int main(int argc, char* argv[]) {
	cout << endl << "Default c'tor test" << endl;
	ari::vector<int> a{};
	cout << endl << a.size() << '\t' << a.capacity() << endl;
	
	cout << endl << "Size c'tor test" << endl;
	ari::vector<int> b(10U, 12);
	cout << endl << b.size() << '\t' << b.capacity() << endl;
	output(b);
	
	cout << endl << "Copy c'tor" << endl;
	ari::vector<int> c{b};
	cout << endl << c.size() << '\t' << c.capacity() << endl;
	output(c);
	
	cout << endl << "Move c'tor" << endl;
	ari::vector<int> d{std::move(c)};
	cout << endl << d.size() << '\t' << d.capacity() << endl;	
	cout << endl << c.size() << '\t' << c.capacity() << endl;
	output(d);
	cout << "c:";
	output(c);
	cout << endl;
	c.push_back(1);
	c.push_back(1);
	c.push_back(1);
	c.insert(c.begin(), {1, 2, 3, 4, 6, 7, 8, 9});	
	cout << endl << c.size() << '\t' << c.capacity() << endl;
	output(c);
	
	cout << endl << "I-List c'tor" << endl;
	ari::vector<int> e = {1, 2, 3, 4, 5, 6, 7, 8};
	output(e);
	ari::vector<int> f{1, 2, 3, 4, 5, 6, 7, 8};
	output(f);
	
	vector<int> tmp = {0, 3, 1, 1, 2, 6};
	
	cout << endl << "Iterator Constructor Test" << endl;
	ari::vector<int> test(tmp.begin(), tmp.end());
	output(test);
	
	cout << endl << "Assign test" << endl;
	test.assign(10U, 99);
	output(test);
	test.assign({1, 2, 3, 4, 5, 6, 7, 8});
	output(test);
	test.assign(f.begin(), f.end());
	output(test);
	
	cout << endl << "Iterator Test" << endl;
	for (auto i = test.begin(); i != test.end(); ++i)
		cout << *i << " ";
	cout << endl;
	

	cout << endl << "Const Iterator Test" << endl;
	for (auto i = test.cbegin(); i != test.cend(); ++i) {
		cout << *i << " ";
		// *i = 4; // error
	}
	cout << endl;

	cout << endl << "Reverse Iterator Test" << endl;
	for (auto i = test.rbegin(); i != test.rend(); ++i) {
		cout << *i << " ";
	}
	cout << endl;

	cout << endl << "Const Reverse Iterator Test" << endl;
	for (auto i = test.crbegin(); i != test.crend(); ++i) {
		cout << *i << " ";
		// *i = 4; // error
	}
	cout << endl;
	
	cout << endl << "Front, back test" << endl;
	cout << "Front: " << test.front() << endl;
	cout << "Back: " << test.back() << endl;
	
	cout << endl << "Suplimentary funtion tests" << endl;
	cout << "vec is " << (test.empty()? "empty" : "not empty") << endl;
	cout << "vec is " << (ari::vector<int>{}.empty()? "empty" : "not empty") << endl;
	cout << "max size is " << test.max_size() << endl;
	
	cout << endl << "Reserve test" << endl;
	test.reserve(100);
	cout << test.capacity() << endl;
	output(test);
	
	cout << endl << "Shrink To Fit Test" << endl;
	test.shrink_to_fit();
	cout << test.capacity() << endl;
	output(test);
	
	cout << endl << "Push Back Test" << endl;
	test.push_back(5);
	output(test);
	test.push_back(7);
	output(test);
	int num1 = 10;
	test.push_back(num1);
	output(test);
	test.push_back(num1);
	output(test);
	
	cout << endl << "Pop Back Test" << endl;
	test.pop_back();
	output(test);
	test.pop_back();
	output(test);
	
	cout << endl << "Shrink To Fit Test" << endl;
	test.shrink_to_fit();
	cout << test.capacity() << endl;
	output(test);
	
	cout << endl << "Auo expansion test" << endl;
	cout << test.capacity() << endl;
	for (int i = 0; i < 9; ++i) {
		test.push_back(i);
	}
	cout << test.capacity() << endl;
	output(test);
	
	cout << endl << "Insert and Expansion test" << endl;
	test.insert(test.begin(), 4);
	test.insert(test.end(), 10);
	test.insert(test.begin() + 3, 10);
	test.insert(test.begin() + 3, 10);
	test.insert(test.begin() + 3, 10);
	cout << test.capacity() << endl;
	output(test);
	
	cout << endl << "Data Test" << endl;
	std::for_each(test.data(), test.data() + test.size(), [](auto i){ cout << i << " ";});
	cout << endl;
	
	cout << endl << "Various insert tests" << endl;
	output(test);
	test.insert(test.begin() + 4, num1);
	output(test);
	test.insert(test.begin(), 3U, num1);
	output(test);
	test.insert(test.rbegin(), num1);
	output(test);
	test.insert(test.rbegin(), 10);
	output(test);
	test.insert(test.rbegin(), 3U, 10);
	output(test);
	
	auto lst = {1, 2, 3};
	test.insert(test.cbegin(), lst.begin(), lst.end());
	output(test);
	
	test.insert(test.crbegin(), lst.begin(), lst.end());
	output(test);
	
	test.insert(test.begin(), {1, 2, 3});
	output(test);
	
	test.insert(test.rbegin(), {1, 2, 3});
	output(test);
	
	ari::vector<Copy> copy_test;
	copy_test.reserve(10U);
	cout << endl << "Emplace tests" << endl;
	copy_test.emplace_back(1, 2, 3);
	copy_test.emplace_back(3, 2, 1);
	copy_test.emplace_back(2, 1, 3);
	
	copy_test.emplace(copy_test.end(), 1, 2, 3);
	copy_test.emplace(copy_test.end(), 3, 2, 1);
	copy_test.emplace(copy_test.rbegin(), 3, 2, 1);
	copy_test.emplace(copy_test.crbegin(), 2, 2, 1);
	
	
	cout << endl << "Operator[] Test" << endl;
	test[2] = 5;
	test[0] = 1;
	output(test);
	
	cout << endl << "At test" << endl;
	test.at(0) = 2;
	try {
		test.at(test.size());
	} catch (...) {
		cout << "Exception caught" << endl;
	}
	
	output(test);
	
	test.erase(test.begin(), test.begin() + 5);
	output(test);
	
	test.erase(test.rbegin(), test.rbegin() + 5);
	output(test);
	
	test.erase(test.begin());
	output(test);
	
	test.erase(test.rbegin());
	output(test);
	
	cout << endl << "Clear test" << endl;
	test.clear();
	output(test);
	
	return 0;
}