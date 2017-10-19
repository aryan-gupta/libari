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

int main(int argc, char* argv[]) {
	vector<int> tmp = {0, 3, 1, 1, 2, 6};
	
	cout << endl << "Iterator Constructor Test" << endl;
	ari::vector<int> test(tmp.begin(), tmp.end());
	output(test);
	
	
	cout << endl << "Push Back Test" << endl;
	test.push_back(5);
	output(test);
	test.push_back(7);
	output(test);
	
	// cout << endl << "Push Front Test" << endl;
	// test.push_front(1);
	// output(test);
	
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
	
	cout << endl << "Pop Back Test" << endl;
	test.pop_back();
	output(test);
	
	cout << endl << "Operator[] Test" << endl;
	test[2] = 5;
	test[0] = 1;
	output(test);
	
	cout << endl << "Insert Test" << endl;
	test.insert(0, 9);
	test.insert(3, 9);
	output(test);
	
	cout << endl << "Expansion test" << endl;
	cout << test.capacity() << endl;
	for (int i = 0; i < 8; ++i) {
		test.push_back(i);
	}
	cout << test.capacity() << endl;
	output(test);
	
	cout << endl << "Shrink To Fit Test" << endl;
	test.shrink_to_fit();
	cout << test.capacity() << endl;
	output(test);
	
	cout << endl << "Insert and Expansion after STF test" << endl;
	test.insert(1, 4);
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
	
	cout << endl << "Clear test" << endl;
	test.clear();
	output(test);
	
	return 0;
}