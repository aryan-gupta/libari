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

// #include "DList.h"
#include "Heap.h"

using namespace std;

template<typename DS>
void output(DS& p) {
	for(size_t i = 0; i < p.size(); ++i)
		cout << p[i] << " ";
	cout << endl;
	cout << "Size: " << p.size() << endl;
} 


template <typename ITER>
void outputB(const ITER& begin, ITER&& end) {
	while(end-- != begin) /// @todo use --> (goes to operator) need to code in iterators
		cout << *end << " ";
	cout << endl;		
}

template <typename ITER>
void outputF(ITER&& begin, const ITER& end) {
	for(; begin != end; ++begin)
		cout << *begin << " ";
	cout << endl;
}

int main(int argc, char* argv[]) {
	vector<int> tmp = {5, 2, 8, 6, 4, 7, 3, 6, 4, 5, 1, 0, 0, 0};
	
	cout << "std::initializer_list test" << endl;
	Heap<int> tmp0 = {1, 3, 5, 1, 7, 2, 6, 3, 8, 9, 12, 5, 3};
	while (!tmp0.empty()) {
		cout << tmp0.top() << endl;
		tmp0.pop();
	}
	
	cout << "std::initializer_list and compare test" << endl;
	Heap<int, std::vector<int>, bool (*) (int, int)> tmp1{
		{1, 3, 5, 1, 7, 2, 6, 3, 8, 9, 12, 5, 3}, 
		[](int a, int b) { return a < b; }
	};
	while (!tmp1.empty()) {
		cout << tmp1.top() << endl;
		tmp1.pop();
	}
	
	
	cout << "Iterator Constructor Test" << endl;
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
	
	cout << "Returning Size!!       " << tmp2.call_member(&std::vector<int>::size) << endl;
	
	using push_back_func_t = void(std::vector<int>::*)(const int&);
	test.call_member(static_cast<push_back_func_t>(&std::vector<int>::push_back), 0);
	test.heapify();
	
	test.call_member([](auto& c){ c.insert(c.begin(), 0); });
	test.call_member([](auto& c, auto num){ c.insert(c.begin(), num); }, 0);
	test.heapify();
	
	cout << "Top, Pop ,and Empty Test" << endl;
	while (!test.empty()) {
		cout << test.top() << "     " << test.size() << endl;
		test.pop();
	}
	cout << endl << endl;
	while (!tmp2.empty()) {
		cout << tmp2.top() << "     " << tmp2.size() << endl;
		tmp2.pop();
	}
	
	cout << endl << "Size Constructor" << endl;
	DList<int> listTest2(6, 2);
	output(listTest);
	
	listTest2.push_back(3);
	listTest2.push_back(7);
	listTest2.push_front(5);
	
	cout << endl << "Forward Iterator" << endl;
	outputF(listTest2.begin(), listTest2.end());
	
	cout << endl << "Backwards Iterator" << endl;
	outputB(listTest2.begin(), listTest2.end());
	
	return 0;
}