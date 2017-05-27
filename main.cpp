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

#include "DList.h"

int main(int argc, char* argv[]) {
	vector<int> tmp = {0, 3, 1, 4, 2, 6};
	
	DList<int> listTest(tmp.begin(), tmp.end());
	
	listTest.push_back(5);
	cout << listTest.size() << endl;
	
	listTest.push_back(7);
	cout << listTest.size() << endl;
	
	listTest.push_front(2);
	cout << listTest.size() << endl;
	
	
	for(size_t i = 0; i < listTest.size(); ++i)
		cout << listTest[i] << " ";
	cout << endl;
	
	listTest.pop_back();
	cout << listTest.size() << endl;
	
	for(size_t i = 0; i < listTest.size(); ++i)
		cout << listTest[i] << " ";
	cout << endl;
	
	return 0;
}