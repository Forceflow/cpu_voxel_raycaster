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
	DataPoint const* data;

	Node();
	bool isTerminal() const;
	bool hasData() const;
	bool isNull() const;
};

inline bool Node::isNull() const{
	return isTerminal() && !hasData();
}

inline bool Node::isTerminal() const{
	for(int i = 0; i<8; i++){if(children[i] != 0){return false;}}
	return true;
}

inline bool Node::hasData() const{
	return !(data == NULL);
}

#endif /* NODE_H_ */
