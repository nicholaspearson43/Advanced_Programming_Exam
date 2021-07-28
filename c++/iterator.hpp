#pragma once
#include <iterator>
#include "node.hpp"


template <typename node_T, typename O>
class Iterator{
	
	node_T* current;
	
	public:
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = O;
	using pointer = value_type*;
	using reference = value_type&;
	
	explicit Iterator(node_T* tmp): current{tmp}{}
	Iterator() = default;
	~Iterator() = default;
	
	reference operator*() const{return current->elem;}
	pointer operator->() const{return &**this;}
	
	//pre increment
	Iterator& operator++(){
		if(!current){return *this;}
		else if(current->right.get()){
			current = current->right.get();
			while(current->left.get()){
				current = current->left.get();
			}
		}
		else{
			node_T* tmp = current->parent;
			while(tmp && current == tmp->right.get()){
				current = tmp;
				tmp = current -> parent;
			}
			current = tmp;
		}
		return *this;
	}
	Iterator operator++(int){
		auto tmp{*this};
		++(*this);
		return tmp;
	}
	
	friend bool operator==(const Iterator &a, const Iterator &b){
		return (a.current==b.current);
	}
	friend bool operator!=(const Iterator &a, const Iterator &b){
		return !(a==b);
	}


};
