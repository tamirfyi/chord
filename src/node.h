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
			successor = this;
			for (int i = 1; i < TABLE_SIZE; i++)
			{
				fingers.successors[i] = this;
			}
		}
		else
		{
			init_finger_table(node);
			update_others(nodeId);
			migrate_keys();
		}
	}

	void leave()
	{
		// Update pointers
		predecessor->successor = successor;
		successor->predecessor = predecessor;

		successor->update_others(successor->nodeId);

		if (keys.size() == 0)
			return;

		// Migrate keys to successor node
		map<int, int>::iterator it;

		for (it = keys.begin(); it != keys.end(); it++)
		{
			successor->keys.insert({it->first, it->second});
		}
	}

	void migrate_keys()
	{
		// move keys in (predecessor; n] from succesor

		if (successor->keys.size() == 0)
		{
			return;
		}

		map<int, int>::iterator it;

		for (it = successor->keys.begin(); it != successor->keys.end(); it++)
		{
			int keyId = it->first;
			if (isAnElementOf(predecessor->nodeId, false, nodeId, true, keyId))
			{
				keys.insert({it->first, it->second});
				successor->keys.erase(keyId);

				cout << "Migrated key " << keyId << " from node " << successor->nodeId << " to " << nodeId << endl;
			}
		}
	}

	void init_finger_table(Node *m)
	{
		fingers.successors[1] = m->find_successor(fingers.start[1]); // update successor
		successor = fingers.successors[1];													 // update successor pointer
		predecessor = successor->predecessor;												 // update predecessor

		successor->predecessor = this; // updating successors predecessor
		predecessor->successor = this; // update predecessors successor

		for (int i = 1; i < TABLE_SIZE - 1; i++)
		{
			bool set = isAnElementOf(nodeId, true, fingers.successors[i]->nodeId, false, fingers.start[i + 1]);
			if (set)
			{
				fingers.successors[i + 1] = fingers.successors[i];
			}
			else
			{
				Node *x = m->find_successor(fingers.start[i + 1]);
				fingers.successors[i + 1] = x;
			}
		}
	}

	// void update_finger_table(Node *s, int i)
	// {
	// 	Node *p;
	// 	if (isAnElementOf(nodeId, true, fingers.successors[i]->nodeId, false, s->nodeId))
	// 	{
	// 		fingers.successors[i] = s;
	// 		p = predecessor;
	// 		p->update_finger_table(s, i);
	// 	}
	// }

	void update_others(int start)
	{
		Node *p = predecessor;
		if (p->nodeId == start)
		{
			return;
		}

		for (int i = 1; i < TABLE_SIZE; i++)
		{
			p->fingers.successors[i] = find_successor(p->fingers.start[i]);
		}
		p->update_others(start);
	}

	Node *
	find_successor(int id)
	{
		if (nodeId == id)
		{
			return this;
		}
		Node *m = find_predecessor(id);

		return m->successor;
	}

	Node *find_predecessor(int id)
	{
		Node *m = this;

		if (m->nodeId == m->successor->nodeId)
		{
			return m;
		}

		while (!isAnElementOf(m->nodeId, false, m->successor->nodeId, true, id))
		{
			m = m->closest_preceding_finger(id);
		}

		return m;
	}

	Node *closest_preceding_finger(int id)
	{
		// cout << "start cpf (" << id << ')' << endl;
		for (int i = BITLENGTH; i > 0; i--)
		{
			int fingerId = fingers.successors[i]->nodeId;
			if (isAnElementOf(nodeId, false, id, false, fingerId))
			{
				return fingers.successors[i];
			}
		}

		return this;
	}

	// TODO: implement DHT lookup
	int find(int key);

	// TODO: implement DHT key insertion
	void insert(int key, int val = -1)
	{
		Node *n = find_successor(key);
		n->keys.insert({key, val}); // if val == -1, then value = "None"
	}

	// TODO: implement DHT key deletion
	void remove(int key)
	{
		Node *n = find_successor(key);
		n->keys.erase(key);
	}

	void printKeys()
	{
		cout << "------------ Node ID: " << nodeId << " ------------" << endl;
		cout << "{";

		map<int, int>::iterator it;
		for (it = keys.begin(); it != keys.end(); it++)
		{
			if (next(it) == keys.end())
			{
				if (it->second == -1)
				{
					cout << it->first << ": "
							 << "None";
				}
				else
				{
					cout << it->first << ": " << it->second;
				}
			}
			else
			{
				if (it->second == -1)
				{
					cout << it->first << ": "
							 << "None"
							 << ", ";
				}
				else
				{
					cout << it->first << ": " << it->second << ", ";
				}
			}
		}
		cout << "}" << endl;
	}

	void printInfo()
	{
		cout << "------------ Node ID: " << nodeId << " ------------" << endl;
		cout << "Successor: " << successor->nodeId << " Predecessor: " << predecessor->nodeId << endl;
		cout << "FingerTables:" << endl;
		for (int i = 1; i < TABLE_SIZE; i++)
		{
			cout << "| k =  " << i << " [ " << fingers.start[i] << ", " << (i >= TABLE_SIZE - 1 ? nodeId : fingers.start[i + 1]) << " )"
					 << "\tsucc. = " << fingers.successors[i]->nodeId << " |" << endl;
		}
		cout << "-------------------------------------" << endl;
	}
};

#endif
