#ifndef PATHFINDING_CPP__
#define PATHFINDING_CPP__

#include "pathfinding.h"
#include <boost/foreach.hpp>

#include "draw.h"

#include <nds.h>
#include <math.h>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <cassert>

node_map* node::created = new node_map();
void node::cleanup()
{
	std::pair<coords,node*> n;
	BOOST_FOREACH(n, *created)
	{
		delete n.second;
	}
	created->clear();
}

node::node(int x_pos, int y_pos, node* node_parent) : x(x_pos), y(y_pos), path_cost(0), parent(node_parent)
{
	if (parent != NULL)
	{
		path_cost = parent->path_cost + astar::heuristic(this,parent);
	}
}
node::~node() { }

node_map* node::find_children(node_map* closed_set)
{
    node_map* nm = new node_map();

	find_child(x+1,y,nm,closed_set);
	find_child(x-1,y,nm,closed_set);
	find_child(x,y+1,nm,closed_set);
	find_child(x,y-1,nm,closed_set);
	find_child(x+1,y+1,nm,closed_set);
	find_child(x-1,y+1,nm,closed_set);
	find_child(x+1,y-1,nm,closed_set);
	find_child(x-1,y-1,nm,closed_set);

	return nm;
}

void node::find_child(int x, int y, node_map* nm, node_map* closed_set)
{
	if (x < 0 || x > SCREEN_WIDTH || y < 0 || y > SCREEN_HEIGHT) return;
	if (VRAM_A[x+ y*SCREEN_WIDTH] == RGB15(31,31,31)) return;

	if (closed_set->find(coords(x,y)) == closed_set->end())
	{
		node* n = get_node(x,y,this);
		std::pair< coords,node* > nc = std::pair< coords,node* >( coords(x,y), n );

		nm->insert(nc);
	}
}

node* node::get_node(int x, int y, node* parent)
{
	coords c(x,y);
	node_map::iterator itr = created->find(c);

	if (itr == created->end())
	{
		node* n = new node(x,y,parent);
		created->insert(std::pair< coords, node* >(c, n));
		draw::plot(x,y,RGB15(0,31,0));
		return n;
	}
	else
	{
		return itr->second;
	}
}

node* astar::search(node* current, node* target)
{
	node_map* closed = new node_map();
	priority_queue* opened = new priority_queue();

	node* x = NULL;

	opened->insert( weighted_node(evaluation(current,target),current) );

	while(!opened->empty())
	{
		x = opened->rbegin()->second; // Biggest evaluation() value.

		if (x->x == target->x && x->y == target->y) break;

		opened->erase(--opened->end()); // Remove x from opened
		closed->insert(std::pair<coords,node*>(coords(x->x,x->y),x));

		std::pair<coords,node*> cn;
		node_map* nm = x->find_children(closed);
		BOOST_FOREACH(cn, *nm)
		{
			node* n = cn.second;
			double tent_g_score = x->path_cost + heuristic(x,n);
			bool better = false;

			bool in_opened = false;
			BOOST_FOREACH(weighted_node wn, *opened)
			{
				if (wn.second == n) { in_opened = true; break; }
			}

			if (!in_opened)
			{
				opened->insert(weighted_node(evaluation(n,target),n));
				better = true;
			}
			else if (tent_g_score < n->path_cost)
			{
				better = true;
			}

			if (better)
			{
				n->parent = x;
				n->path_cost = tent_g_score;
			}
		}
		delete nm;

	}

	delete closed;
	delete opened;

	return x;
}

double astar::evaluation(node* current, node* target)
{
	return 1/(heuristic(current, target) + cost(current));
}

double astar::cost(node* current)
{
	//return 0;
	return current->path_cost;
}

double astar::heuristic(node* current, node* target)
{
	return sqrt((current->x - target->x) * (current->x - target->x) + (current->y - target->y) * (current->y - target->y));
	//return abs(current->x - target->x) + abs(current->y - target->y);
}

std::stack<node*>* astar::generate_path(node* target)
{
	std::stack<node*>* path = new std::stack<node*>;

	node* n = target;
	while (n != NULL)
	{
		path->push(n);
		n = n->parent;
	}

	return path;
}

bool astar::at_goal(node* current, node* target)
{
	return (current->x == target->x && current->y == target->y);
}

#endif // #ifndef PATHFINDING_CPP__
