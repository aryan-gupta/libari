

#include <unordered_map>
#include <vector>
#include <iostream>

int main() {
	std::unordered_map<int, int> t;
	t.insert({15, 2});
	t.insert({1, 6});
	t.insert({3, 3});
	
	for (auto a : t)
		std::cout << a.first << ' ' << a.second << std::endl;
	
	std::vector<int> p;
	p.push_back(2);
	p.push_back(2);
	p.push_back(2);
	for (auto a : p)
		std::cout << a << ' ';
}