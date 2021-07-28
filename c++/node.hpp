#pragma once
#include <iostream>
#include <memory>
#include <utility>
template <typename K, typename V>
struct Node{
	//member variables
	std::pair<const K, V> elem;
	Node<K,V>* parent;
	std::unique_ptr<Node<K,V>> left, right;
	
	//default ctor
	Node() noexcept =default;
	//ctor with a key and a value
	Node(K key, V value) noexcept : elem{std::make_pair(key, value)}, parent{nullptr}, left{nullptr}, right{nullptr}{}
	
	//ctor with a pair of key and value
	explicit Node(std::pair<K,V> _elem) noexcept: elem{_elem}, parent{nullptr}, left{nullptr}, right{nullptr} {}

	//ctor with a key, a value and a pointer to its parent
	Node(K key, V value, Node<K,V>* ptr) noexcept: elem{std::make_pair(key,value)}, parent{ptr}, left{nullptr}, right{nullptr} {}
	
	//ctor with a pair of key and value, and a pointer to its parent
	Node(std::pair<K,V> _elem, Node<K,V>* ptr) noexcept: elem{_elem}, parent{ptr}, left{nullptr}, right{nullptr} {}
	//default dtor
	~Node() noexcept = default;
	//copy constructor 
	Node(const Node& x): elem{x.elem}, parent{nullptr}, left{nullptr}, right{nullptr}{}	
};
//put-to override
template <typename K, typename V>
std::ostream& operator <<(std::ostream& os, const Node<K,V>& x){
		os<<"This node has key and value: (" << x.elem.first << "," << x.elem.second << "). \n";
		os<<"Its parent is stored in the address " << x.parent << "\n";
		os<<"Its left and right children are stored in: L="<< x.left.get() << ", R=" << x.right.get() << std::endl; 
		return os;
	}
