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
	vector<int> tmp = {};
	
	cout << endl << "Iterator Constructor Test" << endl;
	Heap<int, vector<int>, me::greater<int> > test(tmp.begin(), tmp.end());
	
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
	
	cout << "Top, Pop ,and Empty Test" << endl;
	while (!test.empty()) {
		cout << test.top() << "     " << test.size() << endl;
		test.pop();
	}
	
	return 0;
}