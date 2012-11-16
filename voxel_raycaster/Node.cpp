#include "Node.h"

Node::Node(){
	data = NULL;
	for(int i = 0; i<8; i++){
		children[i] = 0;
	}
}