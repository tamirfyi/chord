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
#define JOIN 0
#define LEAVE 1

using namespace std;

// Forward declaration
class Node;

// FingerTable class -> will be an attribute of the Node class
class FingerTable
{
public:
	int nodeId;								 // ID for Node associated with this finger table
	vector<int> start;				 // Vector for lower bound of finger table entries
	vector<Node *> successors; // Vector for successors of finger table entries

	FingerTable() {}

	/**
	 * Finger table constructor
	 * @param id ID of node that holds this finger table
	 * - Resize the successor and finger table vectors to be 1-based
	 * - Initialize the nodeId attribute to that of the created node
	 * - Initialize the start vector to have correct values based off of the nodeId
	 */
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
};

// Node class
class Node
{
public:
	int nodeId;					 // ID of node
	FingerTable fingers; // Finger table definition
	Node *predecessor;	 // Reference to preceeding node
	Node *successor;		 // Reference to succeeding node
	map<int, int> keys;	 // Vector of keys that node holds

	// Constructor definition
	Node() {}

	/**
	 * Node constructor:
	 * @param id ID of node
	 * - Initialize nodeId
	 * - Initialize FingerTable
	 */
	Node(int id)
	{
		nodeId = id;
		fingers = FingerTable(id);
	}

	/**
	 * Function that joins a node into the network
	 * @param node: the first node to contact with to initialize join process. If this is the first node to join the Chord network, the parameter is NULL.
	 *
	 */
	void join(Node *node)
	{
		// First node joining the network
		if (!node)
		{
			// Set predecessor and successor to joining node
			predecessor = this;
			successor = this;

			// Set all successors to joining node
			for (int i = 1; i < TABLE_SIZE; i++)
			{
				fingers.successors[i] = this;
			}
		}
		else // Not first node joining the network
		{
			init_finger_table(node); // Initialize finger table of joining node
			update_others(nodeId);	 // Update other finger tables to reflect newly joined node
			migrate_keys();					 // Migrate keys that will be associated with new node
		}

		// Print all tables after join
		printAllTables(JOIN, NULL);
	}

	/**
	 * Leave the network and migrate associated keys to successor node
	 */
	void leave()
	{
		// Store pointer to original successor for printAllTables
		Node *succ = successor;

		// Update pointers
		predecessor->successor = successor;
		successor->predecessor = predecessor;

		// Update other nodes from successor
		successor->update_others(successor->nodeId);

		// If there are no keys associated with thise node, do not perform migration
		if (keys.size() == 0)
			return;

		// Migrate keys to successor node
		map<int, int>::iterator it; // Iterator for map of keys
		for (it = keys.begin(); it != keys.end(); it++)
		{
			successor->keys.insert({it->first, it->second});
		}

		// Print all tables after leave
		printAllTables(LEAVE, succ);
	}

	/**
	 * Function to migrate keys after a node joins the network
	 */
	void migrate_keys()
	{
		// If the successor of this node has no keys, do not perform migration
		if (successor->keys.size() == 0)
			return;

		// Migrate keys from successor to current node
		map<int, int>::iterator it; // Iterator for map of keys
		for (it = successor->keys.begin(); it != successor->keys.end(); it++)
		{
			int keyId = it->first; // Store "id" of key
			// Check if key should be inserted into newly joined node
			if (isAnElementOf(predecessor->nodeId, false, nodeId, true, keyId))
			{
				keys.insert({it->first, it->second}); // Insert key into current node
				successor->keys.erase(keyId);					// Erase key from successor

				// Print migration statement
				cout << "Migrated key " << keyId << " from node " << successor->nodeId << " to " << nodeId << endl;
			}
		}
		cout << endl;
	}

	/**
	 * Function that initializes the finger table of newly joined node in a network that already has (at least) 1 node
	 * @param m Node that newly joined node follows
	 */
	void init_finger_table(Node *m)
	{
		fingers.successors[1] = m->find_successor(fingers.start[1]); // update successor
		successor = fingers.successors[1];													 // update successor pointer
		predecessor = successor->predecessor;												 // update predecessor

		successor->predecessor = this; // updating successors predecessor
		predecessor->successor = this; // update predecessors successor

		// For every entry of a node's finger table, find the appropriate successor and update its successor reference
		for (int i = 1; i < TABLE_SIZE - 1; i++)
		{
			bool set = isAnElementOf(nodeId, true, fingers.successors[i]->nodeId, false, fingers.start[i + 1]);
			if (set)
			{
				fingers.successors[i + 1] = fingers.successors[i];
			}
			else
			{
				Node *n = m->find_successor(fingers.start[i + 1]);
				fingers.successors[i + 1] = n;
			}
		}
	}

	/**
	 * Function to update other finger tables that may have been affected by a new node joining the network
	 * @param start ID where the recursive update starts from
	 */
	void update_others(int start)
	{
		// Store reference to predecessor
		Node *p = predecessor;

		// Base case to prevent infinite update_others loop
		if (p->nodeId == start)
		{
			return;
		}

		// Update all entries of the finger table
		for (int i = 1; i < TABLE_SIZE; i++)
		{
			p->fingers.successors[i] = find_successor(p->fingers.start[i]);
		}
		// Recursive call
		p->update_others(start);
	}

	/**
	 * Helper function to find successor node given an ID in the network
	 * @param id ID of node/key in the segment before the successor
	 */
	Node *find_successor(int id)
	{
		if (nodeId == id)
		{
			return this;
		}
		Node *m = find_predecessor(id);

		return m->successor;
	}

	/**
	 * Helper function to find predecessor node given an ID in the network
	 * @param id ID of node/key in the segment following the predecessor
	 */
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

	/**
	 * Helper function to find closest preceeding figner given an ID in the network
	 * @param id ID of node/key to the closest preceeding finger
	 */
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

	/**
	 * Function that performs a lookup for key within the network
	 * @param key ID of key/node to find
	 */
	int find(int key)
	{
		// Find potential successor of key and create reference to key value
		Node *n = find_successor(key);
		int val = n->keys.find(key)->second;

		// If key is not associated with node, return -1
		if (n->keys.find(key) == n->keys.end())
		{
			return -1;
		}

		// If key is not located at this node, print statement containing path
		if (n->nodeId != nodeId)
		{
			cout << "Look-up result of key " << key << " from node " << nodeId << " with path [" << nodeId << ", " << n->nodeId << "] value is ";
		}
		else
		{
			cout << "Look-up result of key " << key << " from node " << nodeId << " with path [" << nodeId << "] value is ";
		}

		// If value == -1, print None, else print the value
		if (val == -1)
		{
			cout << "None" << endl;
		}
		else
		{
			cout << val << endl;
		}
		return val;
	}

	/**
	 * Function to insert a key into the network
	 * @param key "ID" of the key inserted into the network
	 * @param val value of the key inserted into the network
	 */
	void insert(int key, int val = -1)
	{
		// Find successor of key value
		Node *n = find_successor(key);

		// Check if the key already exists at its successor; if it does, update value, otherwise, insert new key into key map
		map<int, int>::iterator it = n->keys.find(key);
		if (it != n->keys.end())
		{
			it->second = val;
		}
		else
		{
			n->keys.insert({key, val}); // if val == -1, then value = "None"
		}
	}

	/**
	 * Function to remove a key from the network
	 */
	void remove(int key)
	{
		// Find successor of desired key and remove it from the key map
		Node *n = find_successor(key);
		n->keys.erase(key);
	}

	/**
	 * Function to print keys associated with a node in the network
	 */
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
		cout << "}" << endl
				 << endl;
	}

	/**
	 * Function to print the information related to a node in the network
	 * Information includes:
	 * - Successor
	 * - Predecessor
	 * - Finger table entries
	 */
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
		cout << "*************************************" << endl
				 << endl;
	}

	/**
	 * Function to print all tables after a node joins or leaves a network
	 * @param mode JOIN = 0, LEAVE = 1
	 * @param leaveSuccessor reference to successor of node leaving the network to format prints correctly
	 */
	void printAllTables(int mode, Node *leaveSuccessor)
	{
		if (mode == JOIN)
		{
			if (nodeId == predecessor->nodeId)
			{
				cout << "******* [ Node " << nodeId << " joined an empty network "
						 << " ] *******" << endl;
			}
			else
			{
				cout << "******* [ Node " << nodeId << " joined Node " << predecessor->nodeId << " ] *******" << endl;
			}
		}
		else if (mode == LEAVE)
		{
			cout << "******* [ Node " << nodeId << " left the network "
					 << " ] *******" << endl;
		}

		Node *itr = this->successor;
		int currentId = this->nodeId;

		if (mode == JOIN)
		{
			printInfo();
			while (currentId != itr->nodeId)
			{
				itr->printInfo();
				itr = itr->successor;
			}
		}
		if (mode == LEAVE)
		{
			currentId = leaveSuccessor->predecessor->nodeId;
			while (currentId != itr->nodeId)
			{
				itr->printInfo();
				itr = itr->successor;
			}
			itr->printInfo();
		}
	}
};

#endif
