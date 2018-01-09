
#include <vector>
#include <iostream>
#include <string>
#include <cxxabi.h>

#include "../include/any.hpp"

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
		a = std::string{"dfhskdjhflksjdhgfkdjhfgjkdhsfjkgsdjkfghdlksjhfgjkhdsfjgkhfeuihrfuisdbfhbjk"
			"kdjhflksjdhfkjsdhfjdshkgjsfdgshdjkfgkjhsdgfkajhsgeufiawbcuebwuecbayuwbchjkxjckhabwecuy"
			"jhflkjsdhfdhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh"
			"jhflkjsdhfdhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh"
			"jhflkjsdhfdhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh"
			"jhflkjsdhfdhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh"
		};
		
		ari::any b;
		b = 2.0;
		b = a;
		
		int status;
		auto realname = abi::__cxa_demangle(b.type().name(), 0, 0, &status);
		std::cout << realname << "\n";
		
		std::cout << ari::any_cast<std::string>(a) << ari::any_cast<std::string>(b) << "\n";
		
	}
	
	{
		// std::any a = "Hello";
		// std::cout << std::any_cast<const char*>(a) << std::endl;
		// std::cout << std::any_cast<std::string>(a) << std::endl;
	}
	
	{
		ari::any a = "Hello";
		std::cout << ari::any_cast<const char*>(a) << std::endl;
		std::cout << ari::any_cast<std::string>(a) << std::endl;
	}
	
	{
		std::vector<ari::any> stuff;
		
		stuff.push_back(2);
		stuff.push_back(2.0);
		stuff.push_back('2');
		stuff.push_back('a');
		
		std::cout << ari::any_cast<int>              (stuff[0])
				  << ari::any_cast<double>           (stuff[1])
				  << ari::any_cast<char>             (stuff[2])
				  << ari::any_cast<char>             (stuff[2])
				  << std::endl;
	}

	
}