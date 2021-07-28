#include <iostream>
#include "node.hpp"
#include "iterator.hpp"
#include "bst.hpp"

int main(){
	bst<int, int> tree{}; //create new bst
	tree.insert(std::make_pair(6, 1));
	tree.insert(std::make_pair(4,2));
	tree.insert(std::make_pair(7, 3));
	tree.insert(std::make_pair(3,4));
	auto x1 = std::make_pair(5, 5);
	tree.insert(x1);
	tree.insert(std::make_pair(8,6));
	
	auto x2 = std::make_pair(9, 7);
	tree.emplace(x2);	
	tree.emplace(std::make_pair(10,8));
	std::cout << tree << std::endl;
	
	std::cout << "The key of the root is: " << tree.get_root()->elem.first << std::endl;
	std::cout << "The size of the tree is: " << tree.get_size() << std::endl;
	
	std::cout << "Making a copy of tree" << std::endl;
	bst<int, int> tree_copy;
	tree_copy=tree;
	std::cout << tree_copy << std::endl;
		
	tree.balance();
	std::cout << "Working on the original balanced tree" << std::endl;
	std::cout << "The key of the root is: " << tree.get_root()->elem.first << std::endl;
	std::cout << "The size of the tree is: " << tree.get_size() << std::endl;
	
	std::cout << "Working on the unbalanced copy" << std::endl;
	std::cout << "The key of the root is: " << tree_copy.get_root()->elem.first << std::endl;
	std::cout << "The size of the tree is: " << tree_copy.get_size() << std::endl;
	
	tree.erase(7); //erase on root
	tree_copy.erase(9); //erase on non root
	std::cout << "OT" << std::endl;
	std::cout << tree << std::endl;
	std::cout << "CT" << std::endl;
	std::cout << tree_copy << std::endl;
	
	std::cout <<"Looking for n 9 in OT: " << tree.find(9)->first << std::endl;
	std::cout <<"Looking for n 7 in OT: "<< std::endl;
	tree.find(7);
	
	std::cout << tree_copy[7] << std::endl;
	std::cout << tree_copy << std::endl;
	std::cout << tree_copy[9] << std::endl;
	std::cout << tree_copy << std::endl;
	
	tree.clear();
}
