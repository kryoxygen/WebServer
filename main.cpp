#include <vector>
#include <iostream>
int main() {
	std::vector<int> v({1, 0,23,123,3445,123,457,321,23,45,1,23,4,5,6,7,8,9,10});
	v.push_back(0);
	std::vector<int>::iterator it = v.begin();
	while(it != v.end()) {
		std::cout << *it << std::endl;
		++it;
	}
	return 0;
}
