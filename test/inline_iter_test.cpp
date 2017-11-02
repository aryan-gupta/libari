


// TEST CONCLUSION:
// in conclusion, with any optimization, the 


/////// START ASM 
/* 
00000000 <_main>:
   0:	55                   	push   %ebp
   1:	89 e5                	mov    %esp,%ebp
   3:	83 e4 f0             	and    $0xfffffff0,%esp
   6:	83 ec 10             	sub    $0x10,%esp
   9:	e8 00 00 00 00       	call   e <_main+0xe>
   e:	c7 04 24 18 00 00 00 	movl   $0x18,(%esp)
  15:	e8 00 00 00 00       	call   1a <_main+0x1a>
  1a:	c7 00 01 00 00 00    	movl   $0x1,(%eax)
  20:	c7 40 04 02 00 00 00 	movl   $0x2,0x4(%eax)
  27:	c7 40 08 03 00 00 00 	movl   $0x3,0x8(%eax)
  2e:	c7 40 0c 04 00 00 00 	movl   $0x4,0xc(%eax)
  35:	c7 40 10 05 00 00 00 	movl   $0x5,0x10(%eax)
  3c:	c7 40 14 06 00 00 00 	movl   $0x6,0x14(%eax)
  43:	a3 00 00 00 00       	mov    %eax,0x0
  48:	c9                   	leave  
  49:	c3                   	ret    
  4a:	90                   	nop
  4b:	90                   	nop
   */
///////// END ASM

#include <iostream>

int* dat;

class AsmTest1 {
public:
	void operator++() {
		++dat0;
	}
	
	int* dat0;

};

class AsmTest2 {
public:
	void operator++() {
		++dat1;
	}
	
	AsmTest1 dat1;
};

class AsmTest3 {
public:
	void operator++() {
		++dat2;
	}
	
	AsmTest2 dat2;
};

int main() {
	dat = new int[6]{1, 2, 3, 4, 5, 6};
	
	/// THIS PRODUCES Identical binaries
	AsmTest1 a{dat};
	AsmTest2 b{a};
	AsmTest3 c{b};
	
	++c;
	
	std::cout << *a.dat0 << std::endl;
	
	/// Than this
	int* d = dat;
	++d;
	std::cout << *d << std::endl;
}