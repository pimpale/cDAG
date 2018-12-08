/*
 * main.c
 *
 *  Created on: Aug 25, 2018
 *      Author: gpi
 */
#include <stdio.h>
#include <stdlib.h>


struct Node
{
	struct Node** ancestors;
	size_t ancestornum;
	size_t ancestorlen;
	struct Node** children;
	size_t childnum;
	size_t childlen;

	char* value;

} typedef Node;

/* will malloc a new node for us */
Node* newNode(char* value)
{
	Node *node = malloc(sizeof(Node));
	node->ancestorlen = 1;
	node->ancestornum = 0;
	node->ancestors = malloc(node->ancestorlen * sizeof(Node*));
	node->childlen = 1;
	node->childnum = 0;
	node->children= malloc(node->childlen * sizeof(Node*));
	node->value = value;
	return (node);
}

/* checks if child is in parent's ancestors */
int inAncestry(Node* parent, Node* child)
{
	for(int i = 0; i < parent->ancestornum; i++)
	{
		if(parent->ancestors[i] == child)
		{
			return (1);
		}
	}
	return (0);
}
/*adds a child child to parent node parent, or does nothing */
void addChild(Node* parent, Node* child)
{
	/*before doing anything, check if child is in parents ancestors */
	if(inAncestry(parent, child))
	{
		fprintf(stderr,"Failed to add node %s as child to node %s due to cycle\n", child->value, parent->value);
		return;
	}

	/*add child link to parent */
	if (parent->childnum + 1 > parent->childlen) /* extend array if full */
	{
		parent->childlen += 10;
		parent->children = realloc(parent->children,parent->childlen * sizeof(Node*));
	}
	parent->children[parent->childnum] = child;
	parent->childnum++;

	/*add all ancestors of parent to child */
	if (child->ancestornum + parent->ancestornum + 1 > child->ancestorlen) /* extend array if full */
	{
		child->ancestorlen += 1 + parent->ancestorlen;
		child->ancestors = realloc(child->ancestors,child->ancestorlen * sizeof(Node*));
	}
	/* add the parent to the child's ancestors */
	child->ancestors[child->ancestornum] = parent;
	child->ancestornum++;
	/*add the parent's ancestor's to the child's ancestors */
	for(int i = 0; i < parent->ancestornum; i++)
	{
		if(!inAncestry(child, parent->ancestors[i]))
		{
			child->ancestors[child->ancestornum] = parent->ancestors[i];
			child->ancestornum++;
		}
	}
}


void printNode(Node* node)
{
	printf("%s -> ", node->value);
	if(node->childnum != 0)
	{
		for(int i = 0; i < node->childnum; i++)
		{
			printf("%s   ",node->children[i]->value);
		}

		printf("\n");
	}
	else
	{
		printf("no child\n");
	}
}



int main(int argc, char** argv)
{
	{
		Node* n1 = newNode("1");
		Node* n2 = newNode("2");
		Node* n3 = newNode("3");
		Node* n4 = newNode("4");
		Node* n5 = newNode("5");
		Node* n6 = newNode("6");

		addChild(n1, n2);
		addChild(n1, n3);
		addChild(n1, n4);
		addChild(n2, n5);
		addChild(n3, n6);
		addChild(n4, n3);
		addChild(n4, n6);
		addChild(n5, n6);

		printNode(n1);
		printNode(n2);
		printNode(n3);
		printNode(n4);
		printNode(n5);
		printNode(n6);
	}

	{
		Node* n1 = newNode("1");
		Node* n2 = newNode("2");
		Node* n3 = newNode("3");

		addChild(n1,n2);
		addChild(n2,n3);
		addChild(n3,n1);

		printNode(n1);
		printNode(n2);
		printNode(n3);
	}
}
