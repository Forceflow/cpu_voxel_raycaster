#ifndef NODE_H_
#define NODE_H_

#pragma once

#include "DataPoint.h"
#include <string>
#include <cstdint>

class Node
{
public:
	size_t children[8];
	size_t data;

	Node();
	bool isLeaf() const;
	bool hasData() const;
	bool isNull() const;
};

inline Node::Node(){
	data = 0;
	for(int i = 0; i<8; i++){
		children[i] = 0;
	}
}

inline bool Node::isNull() const{
	return isLeaf() && !hasData();
}

inline bool Node::isLeaf() const{
	for(int i = 0; i<8; i++){if(children[i] != 0){return false;}}
	return true;
}

inline bool Node::hasData() const{
	return !(data == 0);
}

#endif /* NODE_H_ */
