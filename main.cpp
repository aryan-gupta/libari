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
#include <functional>

using namespace std;

// #include "DList.h"
#include "Heap.h"

template<typename DS>
void output(DS& p) {
	for(size_t i = 0; i < p.size(); ++i)
		cout << p[i] << " ";
	cout << endl;
	cout << "Size: " << p.size() << endl;
} 

int main(int argc, char* argv[]) {
	vector<int> tmp = {5, 2, 8, 6, 4, 7, 3, 6, 4, 5, 1, 0, 0, 0};
	
	cout << endl << "Iterator Constructor Test" << endl;
	Heap<int, vector<int>, std::function<bool(int, int)> > tmp2{tmp.begin(), tmp.end(), [](int a, int b) { return a < b; }};
	
	auto test = tmp2;
	
	cout << "Push Test" << endl;
	test.push(4);
	test.push(8);
	test.push(0);
	test.push(4);
	test.push(2);
	test.push(6);
	test.push(9);
	test.push(2);
	test.push(1);
	
	cout << "Returning Front!!       \n";
	cout << tmp2.call_member(&std::vector<int>::size) << endl;
	
	using insert_func_t = void(std::vector<int>::*)(const int&);
	test.call_member(static_cast<insert_func_t>(&std::vector<int>::push_back), 0);
	
	cout << "Top, Pop ,and Empty Test" << endl;
	while (!test.empty()) {
		cout << test.top() << "     " << test.size() << endl;
		test.pop();
	}

	while (!tmp2.empty()) {
		cout << tmp2.top() << "     " << tmp2.size() << endl;
		tmp2.pop();
	}
	
	return 0;
}