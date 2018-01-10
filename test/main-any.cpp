
#include <vector>
#include <iostream>
#include <string>
#include <cxxabi.h>

#include "../include/any.hpp"


std::string multiply_string(std::string str, size_t n) {
	std::string result;
	while (n --> 0) {
		result += str;
	}
	return result;
}

int main() {
	
	{
		ari::any a = 5;
		std::cout << ari::any_cast<int>(a) << std::endl;
		
		a = 'a';
		std::cout << ari::any_cast<char>(a) << std::endl;
		
		a = 5.0;
		std::cout << ari::any_cast<double>(a) << std::endl;
	}
	
	{
		ari::any b = std::string{"Hello my name is Foo Bar"};
		std::cout << ari::any_cast<std::string>(b) << std::endl;
		
		b = 1;
		std::cout << ari::any_cast<int>(b) << std::endl;
		
		ari::any c = 23421;
		// std::cout << "STOP\n";
		ari::any d{ c };
		// std::cout << "END\n";
		// std::cout << c.type().name() << std::endl;
		// std::cout << d.type().name() << std::endl;
		
		std::cout << ari::any_cast<int>(c) << std::endl;
		std::cout << ari::any_cast<int>(d) << std::endl;
	}
	
	{
		ari::any a = 23421;
		ari::any b{ std::move(a) };
	}
	
	{
		ari::any a;
		a = 4;
		a = multiply_string("Hello", 10);
		
		ari::any b;
		b = 2.0;
		b = a;
		
		int status;
		auto realname = abi::__cxa_demangle(b.type().name(), 0, 0, &status);
		std::cout << realname << "\n";
		
		std::cout << ari::any_cast<std::string>(a) << ari::any_cast<std::string>(b) << "\n";
		
	}
	
	{
		ari::any a = "Hello";
		
		// int status;
		// auto realname = abi::__cxa_demangle(a.type().name(), 0, 0, &status);
		// std::cout << realname << "\n";
		
		try {
			std::cout << ari::any_cast<const char*>(a) << std::endl;
			// std::cout << ari::any_cast<const char (&) [6]>(a) << std::endl;
		} catch (...) {
			std::cout << "ERROR: bad any_cast caught when it shouldn't\n";
		}
		
		try {
			std::cout << ari::any_cast<std::string>(a) << std::endl;
		} catch (ari::bad_any_cast) {
			std::cout << "GOOD: bad any_cast caught properly\n";
		}
	}
	
	{
		ari::any a = std::array<uintmax_t, 2048>{};
		try {
			ari::any_cast<std::array<uintmax_t, 2048>>(a);
		} catch (std::exception e) {
			std::cerr << e.what() << "\n";
		} catch (...) {
			std::cerr << "Somthing broke, dont know what :P\n"; 
		}
		
		a = 'b';
		std::cout << ari::any_cast<char>(a) << std::endl;
	}
	
	{
		std::vector<ari::any> stuff;
		
		stuff.push_back(2);
		stuff.push_back(2.0);
		stuff.push_back('2');
		stuff.push_back('a');
		
		std::cout << ari::any_cast<int>   (stuff[0])
				  << ari::any_cast<double>(stuff[1])
				  << ari::any_cast<char>  (stuff[2])
				  << ari::any_cast<char>  (stuff[2])
				  << std::endl;
	}

	
}