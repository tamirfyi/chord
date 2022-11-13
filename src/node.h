#ifndef NODE_H
#define NODE_H

#include <stdint.h>
#include <map>
#include <cmath>
#include <set>
#include <vector>
#include "tlib.h"

#define BITLENGTH 8
#define OFFSET 1
#define TABLE_SIZE BITLENGTH + OFFSET

using namespace std;

// forward declaration
class Node;

// The following code is just for reference. You can define your own finger table class.
// Since the index uniquely determines the interval, only the successor needs to be maintained.
class FingerTable
{
public:
	int nodeId;
	vector<int> start;
	vector<Node *> successors;

	FingerTable() {}

	FingerTable(int id)
	{
		start.resize(BITLENGTH + 1);
		successors.resize(BITLENGTH + 1);

		nodeId = id;
		for (int i = 1; i < TABLE_SIZE; i++)
		{
			start[i] = (id + ((int)pow(2, i - 1))) % ((int)pow(2, BITLENGTH));
		}
	}

	void prettyPrint();
};

class Node
{
public:
	int nodeId;
	FingerTable fingers;
	Node *predecessor;
	Node *successor;
	map<int, int> keys;

	Node() {}

	Node(int id)
	{
		nodeId = id;
		fingers = FingerTable(id);
	}
	// TODO: implement node join function
	/**
	 * @param node: the first node to contact with to initialize join process. If this is the first node to join the Chord network, the parameter is NULL.
	 */
	void join(Node *node)
	{
		if (!node)
		{
			predecessor = this;
			for (int i = 1; i < TABLE_SIZE; i++)
			{
				fingers.successors[i] = this;
			}
		}
		else
		{
			init_finger_table(node);
			// updateOthers(node);
			// migrateKeys();
		}
	}

	void init_finger_table(Node *m)
	{
		fingers.successors[1] = m->find_successor(fingers.start[1]);
		predecessor = successor->predecessor;
		successor->predecessor = this;

		for (int i = 1; i < TABLE_SIZE - 1; i++)
		{
			Node *nextSuccessor = fingers.successors[i + 1];
			int nextStart = fingers.start[i + 1];
			// bool set = isAnElementOf(nextStart, true, 0, false);
		}
	}

	Node *find_successor(int id)
	{
		Node *m = find_predecessor(id);
		return m->successor;
	}

	Node *find_predecessor(int id)
	{
		Node *m = this;
		while (id <= m->nodeId && id > m->fingers.successors[1]->nodeId)
		{
			m = m->closest_preceding_finger(id);
		}

		return m;
	}

	Node *closest_preceding_finger(int id)
	{
		for (int i = TABLE_SIZE; i >= 1; i--)
		{
			int fingerId = fingers.successors[i]->nodeId;
			if (fingerId > nodeId && fingerId < id)
			{
				return fingers.successors[i];
			}
		}
		return this;
	}

	// TODO: implement DHT lookup
	int find(int key);
	// TODO: implement DHT key insertion
	void insert(int key);
	// TODO: implement DHT key deletion
	void remove(int key);

	void printInfo()
	{
		cout << "------------ Node ID: " << nodeId << " ------------" << endl;
		cout << "Successor: " << fingers.successors[1]->nodeId << " Predecessor: " << predecessor->nodeId << endl
				 << endl;
		cout << "FingerTables:" << endl;
		for (int i = 1; i < TABLE_SIZE; i++)
		{
			cout << "| k =  " << i << " [ " << fingers.start[i] << "\tsucc. = " << fingers.successors[i]->nodeId << " |" << endl;
		}
		cout << "-------------------------------------" << endl;
	}
};

#endif
