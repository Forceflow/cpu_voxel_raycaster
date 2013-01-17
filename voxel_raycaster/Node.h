#ifndef NODE_H_
#define NODE_H_

#pragma once

#include "DataPoint.h"
#include <string>
#include <cstdint>

#define NOCHILD -1

class Node
{
public:
	size_t children_base;
	char children_offset[8];
	//size_t children[8];
	size_t data;

	Node();
	bool hasChild(int i) const;
	size_t getChildPos(int i) const;
	bool isLeaf() const;
	bool hasData() const;
	bool isNull() const;
};

inline Node::Node(){
	data = 0;
	for(int i = 0; i<8; i++){
		children_offset[i] = NOCHILD;
	}
}

inline bool Node::hasChild(int i) const{
	if(children_offset[i] == NOCHILD){
		return false;
	}
	return true;
}

inline size_t Node::getChildPos(int i) const{
	if(children_offset[i] == NOCHILD){
		return 0;
	}
	else{
		return children_base + children_offset[i];
	}
}

inline bool Node::isNull() const{
	return isLeaf() && !hasData();
}

inline bool Node::isLeaf() const{
	for(int i = 0; i<8; i++){if(children_offset[i] != NOCHILD){return false;}}
	return true;
}

inline bool Node::hasData() const{
	return !(data == 0);
}

#endif /* NODE_H_ */
