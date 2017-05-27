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
	vector<int> tmp = {0, 3, 1, 4, 2, 6, 7, 3, 7, 1, 9, 0, 6};
	
	DList<int> listTest(tmp.begin(), tmp.end());
	
	for(size_t i = 0; i < listTest.size(); ++i) {
		cout << listTest[i] << " ";
	}
	
	return 0;
}