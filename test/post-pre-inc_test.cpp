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

#include <iostream>
using namespace std; 
 
int pre() {
	int dat = 5;
	return ++dat;
}

int post() {
	int dat = 5;
	return dat++;
}
 
int main() {
	cout << pre() << endl;
	cout << post() << endl;
}