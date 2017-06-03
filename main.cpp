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
	DList<int> listTest(tmp.begin(), tmp.end());
	
	cout << endl << "Push Back Test" << endl;
	listTest.push_back(5);
	output(listTest);
	listTest.push_back(7);
	output(listTest);
	
	cout << endl << "Push Front Test" << endl;
	listTest.push_front(1);
	output(listTest);
	
	cout << endl << "Pop Back Test" << endl;
	listTest.pop_back();
	output(listTest);
	
	cout << endl << "Pop Front Test" << endl;
	listTest.pop_front();
	output(listTest);
	
	cout << endl << "Remove Test" << endl;
	listTest.remove(5);
	output(listTest);
	
	cout << endl << "Operator[] Test" << endl;
	listTest[2] = 5;
	listTest[0] = 1;
	output(listTest);
	
	cout << endl << "Insert Test" << endl;
	listTest.insert(0, 9);
	//listTest.insert(3, 9);
	output(listTest);
	
	
	return 0;
}