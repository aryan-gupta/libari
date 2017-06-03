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

#include "DArray.h"

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
	DArray<int> test(tmp.begin(), tmp.end());
	
	cout << endl << "Push Back Test" << endl;
	test.push_back(5);
	output(test);
	test.push_back(7);
	output(test);
	
	cout << endl << "Push Front Test" << endl;
	test.push_front(1);
	output(test);
	
	cout << endl << "Pop Back Test" << endl;
	test.pop_back();
	output(test);
	
	cout << endl << "Pop Front Test" << endl;
	test.pop_front();
	output(test);
	
	cout << endl << "Remove Test" << endl;
	test.remove(5);
	output(test);
	
	cout << endl << "Operator[] Test" << endl;
	test[2] = 5;
	test[0] = 1;
	output(test);
	
	cout << endl << "Insert Test" << endl;
	test.insert(0, 9);
	//test.insert(3, 9);
	output(test);
	
	
	return 0;
}