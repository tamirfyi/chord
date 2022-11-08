#ifndef NODE_H
#define NODE_H

#include <stdint.h>
#include <map>
#include <cmath>
#include <set>
#include <vector>

#define BITLENGTH 8
#define OFFSET 1
#define TABLE_SIZE BITLENGTH + OFFSET

//forward declaration
class Node;

//The following code is just for reference. You can define your own finger table class.
//Since the index uniquely determines the interval, only the successor needs to be maintained.  
class FingerTable{
public:
	FingerTable() {}

	FingerTable(int id){
		start.resize(BITLENGTH + 1);
		successors.resize(BITLENGTH + 1);
		
		nodeId = id;
		for(int i = 1; i < TABLE_SIZE; i++){
			start[i] = (id + ((int) pow(2, i-1))) % ((int) pow(2, BITLENGTH));
		}
	}

	void setSuccessor(int index, Node* n){
		successors[index] = n;
	}

	void setStart(int index, int val){
		start[index] = val;
	}


	void prettyPrint();

private:
	int nodeId;
	std::vector<int> start;
	std::vector<Node*> successors;

};

class Node {
public:
	Node(){}

	Node(int id){
		nodeId = id;
		fingers = FingerTable(id);
	}
	//TODO: implement node join function
	/**
	 * @param node: the first node to contact with to initialize join process. If this is the first node to join the Chord network, the parameter is NULL.
	 */
	void join(Node* node){
		if(!node){
			predecessor = this;
			successor = this;
			for(int i = 1; i < TABLE_SIZE; i++){
				fingers.setSuccessor(i, this);
			}
		} else {
			// initFingerTable(node);
			// updateOthers(node);
			// migrateKeys();
		}
	}
	//TODO: implement DHT lookup
	int find(int key);
	//TODO: implement DHT key insertion
	void insert(int key);
	//TODO: implement DHT key deletion
	void remove(int key);


	int getSuccessor(){
		return successor->nodeId;
	}

	int getPredecessor(){
		return predecessor->nodeId;
	}
private:
	int nodeId;
	FingerTable fingers;
	Node* successor;
	Node* predecessor;
	std::map<int, int> keys;
};

#endif
