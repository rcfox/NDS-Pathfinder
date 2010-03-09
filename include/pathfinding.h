#ifndef PATHFINDING_H__
#define PATHFINDING_H__

#include <map>
#include <stack>

class node;

typedef std::pair<double,node*> weighted_node;
typedef std::map<double,node*> priority_queue;

typedef std::pair<int,int> coords;
typedef std::map<coords,node*> node_map;

class node
{
public:
	char x;
	char y;
	float path_cost;

	node* parent;

	node_map* find_children(node_map* closed_set);

	node(int x, int y, node* parent);
	~node();

	static void cleanup();
	static node* get_node(int x, int y, node* parent = NULL);
	static node_map* created;

private:

	void find_child(int x, int y, node_map* nm, node_map* closed_set);


};

namespace astar
{
	node* search(node* current, node* target);
	std::stack<node*>* generate_path(node* target);
	bool at_goal(node* current, node* target);

	double evaluation(node* current, node* target);
	double cost(node* current);
	double heuristic(node* current, node* target);
};

#endif // #ifndef PATHFINDING_H__
