#include <iostream>
#include <memory>
#include <utility>
#include <functional>
#include <vector>
#include <cmath>
#include "node.hpp"
#include "iterator.hpp"
template <typename K, typename V, typename OP=std::less<K>>
class bst{

	using iterator = Iterator<Node<K,V>, std::pair<const K,V>>;
	using const_iterator = Iterator<Node<K,V>,const std::pair<const K, V>>;
	using pair_type = std::pair<const K, V>;	
	
	std::unique_ptr<Node<K,V>> root;
	std::size_t size;
	OP cmp;
	
	
	Node<K,V>* _copy(Node<K,V>* x, Node<K,V>* dad);
	
	template<typename F>
	std::pair<iterator, bool> _insert(F&& x);
	Node<K,V>* _find(const K& x) const noexcept;
	
	
	public:
	
	bst() noexcept: root{nullptr}, size{0}{}
	
	explicit bst(OP x) noexcept: root{nullptr}, size{0}, cmp{std::move(x)}{}
	
	~bst() noexcept = default;
	
	//copy ctor
	bst(const bst& x): size{x.size}, cmp{x.cmp}{
		if(x.root.get() != nullptr){
			root.reset(_copy(x.root.get(), nullptr)); //line 321, last func
		}
	}
	//copy assingment
	bst& operator=(const bst& x){
		auto tmp = x;
		*this = std::move(tmp);
		return *this;
	}
	
	//move
	bst(bst&& x) noexcept = default; //move ctor
	bst& operator=(bst&& x) noexcept = default; //move assignment
	
	std::size_t get_size() const noexcept {return size;} 
	Node<K,V>* get_root() const noexcept {return root.get();}
	
	
	
	std::pair<iterator, bool> insert(const pair_type& x){return _insert(x);}
	std::pair<iterator, bool> insert(pair_type&& x){return _insert(std::move(x));}
	
	
	template <class... Types>
	std::pair<iterator, bool> emplace(Types&&... args){return insert(std::forward<Types>(args)...);}
	
	Node<K,V>* smallest() const noexcept{    //returns smallest element of bst
		Node<K,V>* tmp= root.get();
		while((tmp->left.get())!=nullptr){
			tmp = tmp->left.get();
		} 
		return tmp;
	}
	
	iterator begin() noexcept{return iterator{smallest()};}
	const_iterator begin() const noexcept{return const_iterator{smallest()};}
	const_iterator cbegin() const noexcept{return const_iterator{smallest()};}
	
	iterator end() noexcept {return iterator{nullptr};}
	const_iterator end() const noexcept {return const_iterator{nullptr};}
	const_iterator cend() const noexcept {return const_iterator{nullptr};}
	
	void clear() noexcept{
		root.reset();
		size=0;
	}
	
	iterator find(const K& x) noexcept {return iterator{_find(x)};}
	const_iterator find(const K& x) const noexcept {return const_iterator{_find(x)};}
	
	
	
	friend
	std::ostream& operator<<(std::ostream& os, const bst& x){
		for(auto i = x.begin(); i != x.end(); i++){
			os << "(" << i->first << "," << i->second << ")         ";
		}
		os<<std::endl;
		return os;
	}
	
	V& operator[](const K& x);
	V& operator[](K&& x);
	
	void balance() noexcept;
	
	void erase(const K& x) noexcept;

};

template <typename K, typename V, typename OP>
template <typename F>
std::pair<Iterator<Node<K,V>, std::pair<const K,V>>, bool> bst<K,V,OP>::_insert(F&& x){

	using iterator = Iterator<Node<K,V>, std::pair<const K,V>>;
	//using const_iterator = Iterator<Node<K,V>,const std::pair<const K, V>>;
	//using pair_type = std::pair<const K, V>;
	

	if(!root){
		root.reset(new Node<K,V>{std::forward<F>(x)});
		auto tmp = root.get();
		auto result = std::make_pair<iterator, bool>(iterator{tmp}, true);
		++size;
		return result;
	}
	else{
		auto dad = root.get();
		auto tmp = root.get();
		while(tmp!=nullptr){
			if(cmp(tmp->elem.first, x.first)){
				dad = tmp;
				tmp = tmp->right.get();
			}
			else if(cmp(x.first, tmp->elem.first)){
				dad = tmp;
				tmp = tmp->left.get();
			}
			else{
				auto result = std::make_pair<iterator, bool>(iterator{nullptr}, false);
				return result;
			}
		}
		if(cmp(dad->elem.first, x.first)){
			dad->right.reset(new Node<K,V>{std::forward<F>(x), dad});
			auto result = std::make_pair<iterator, bool>(iterator{tmp}, true);
			++size;
			return result;			
		}
		else{
			dad->left.reset(new Node<K,V>{std::forward<F>(x), dad});
			auto result = std::make_pair<iterator, bool>(iterator{tmp}, true);
			++size;
			return result;			
		}
	}
}


template <typename K, typename V, typename OP>
Node<K, V>* bst<K,V,OP>::_find(const K& x) const noexcept{
	auto tmp = root.get();
	while(tmp != nullptr){
		if(cmp(x, tmp->elem.first)){
			tmp = tmp->left.get();
			//std::cout << "Going left" << std::endl;
		}
		else if(cmp(tmp->elem.first, x)){
			tmp = tmp->right.get();
			//std::cout << "Going right" << std::endl;
		}
		else{
			return tmp;
		}
	}
	return nullptr;
	
}

template <typename K, typename V, typename OP>
V& bst<K,V,OP>::operator[](const K& x){
	auto tmp = _find(x);
	if (tmp){
		return tmp->elem.second;
	}
	V xo{};	
	insert(std::make_pair(x,xo));
	tmp = _find(x);
	return tmp->elem.second;
		
			
}

template <typename K, typename V, typename OP>
V& bst<K,V,OP>::operator[](K&& x){
	auto tmp = _find(x);
	if (tmp){
		return tmp->elem.second;
	}
	V xo{};	
	insert(std::make_pair(std::move(x),xo));
	tmp = _find(x);
	return tmp->elem.second;
		
			
}

template <typename K, typename V, typename OP>
void bst<K,V,OP>::balance() noexcept{
	std::vector<std::pair<K,V>> tmp;
	for(auto i = begin(); i !=end(); i++){
		tmp.push_back(*i);
	}
	int _size = size;
	clear();
	
	int blocks=_size/2;
	
	for(int i = 1; i<blocks; i++){
		for(int j = 1; j < pow(2,i); j=j+2){
			int index = j*_size/pow(2,i);
			emplace(tmp[index]);
		}
	}
	for(int i = 0; i< _size; i++){
		emplace(tmp[i]);
	}
	
}

template <typename K, typename V, typename OP>
void bst<K,V,OP>::erase(const K& x) noexcept{ 
	auto ptr = _find(x);
	if(ptr == nullptr){
		std::cout << "The key is not present in the tree" << std::endl;
	}
	
	bool is_root = false;
	if (root.get() == ptr){
		is_root = true;
	}
	auto pp = ptr->parent;
	auto pl = ptr->left.get();
	auto pr = ptr->right.get();
	
	if(pr == nullptr){ //no right child
		if(pl == nullptr){ // is a leaf
			if(cmp(ptr->elem.first, pp->elem.first)){
				pp->left.reset();
			}
			else if(cmp(pp->elem.first, ptr->elem.first)){
				pp->right.reset();
			}
			if(is_root){
				root.reset();
			}
		}
		else{ // has a left child but no right one
			ptr->left.release();
			if(cmp(ptr->elem.first, pp->elem.first)){
				pp->left.release();
				pp->left.reset(pl);
			}
			else if(cmp(pp->elem.first, ptr->elem.first)){
				pp->right.release();
				pp->right.reset(pl);
			}
			pl->parent = pp;
			if(is_root){
				root.release();
				root.reset(pl);
			}
		}
	}
	else{
		if(pl == nullptr){ // no left child but has a right one
			ptr->right.release();
			if(cmp(ptr->elem.first, pp->elem.first)){
				pp->left.release();
				pp->left.reset(pr);
			}
			else if(cmp(pp->elem.first, ptr->elem.first)){
				pp->right.release();
				pp->right.reset(pr);
			}
			pr->parent = pp;
			if(is_root){
				root.release();
				root.reset(pr);
			}
		}
		else{ //has both a left and a right child
			auto destination = pr;
			while(destination->left.get() != nullptr){ //search for the leftmost element on the subtree rooted on the righ child of the node we 
				destination = destination->left.get(); // want to erase so that we can connect the subtree rooted on the left child of our 
			} //node there
			if(!is_root){
				if(cmp(ptr->elem.first, pp->elem.first)){
					pp->left.release();
					pp->left.reset(pr);
				}
				else if(cmp(pp->elem.first, ptr->elem.first)){
					pp->right.release();
					pp->right.reset(pr);
				}
			}
			pr->parent = pp;
			pl->parent = destination;
			destination->left.release();
			destination->left.reset(pl);
		}
		if (is_root){
			root.release();
			root.reset(pr);
		}
	}	

	ptr->left.release();
	ptr->right.release();
	ptr->parent = nullptr;
	size = size -1;
	
}
template <typename K, typename V, typename OP>
Node<K,V>* bst<K,V,OP>::_copy(Node<K,V>* x, Node<K,V>* dad){
	Node<K,V>* new_x;
	if(x != nullptr){
		new_x = new Node<K,V>{x->elem, dad};
		new_x->left.reset(_copy(x->left.get(), new_x));
		new_x->right.reset(_copy(x->right.get(), new_x));
	}
	else{
		return nullptr;
	}
	return new_x;
}
