//#include "driverlib.h"

// COMMENTED OFF PID FUNCTIONS FOR THE SAKE OF SHOWING MAPPING WITH TEST DATA
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define MINWALLLENGTH 5.0
#define NODELENGTH 15.0
#define NUMOFNODESTODECLADE 30
float frontScannedLength = 0;
float rightScannedLength = 0;
float leftScannedLength = 0;
int direction = 0; // 0 = n, 1=w , 2=s, 3=e
int currentUsedNode = 0;
bool mappingCompleted = false;
int targetx, targety;
// the car will prefer to move left
/*f=g+h */
struct Node
{
	int x, y;
	int status; // 0 = unvisited 1 = visited
	struct Node *left;
	struct Node *right;
	struct Node *front;
	struct Node *back;
	struct Node *parent;
	int f; // a* algo f = total cost of node
	int g; // g = dist btwn current node and start node
	int h; // h = heuristic -> est. dist from goal
};

struct Node *createnode()
{
	struct Node *temp;
	temp = (struct Node *)malloc(sizeof(struct Node));
	temp->left = NULL;
	temp->right = NULL;
	temp->front = NULL;
	temp->back = NULL;
	temp->parent = NULL;
	temp->f = 0;
	temp->g = 0;
	temp->h = 0;
	temp->status = 0;
	temp->x = 0;
	temp->y = 0;
	return temp;
}

struct List
{
	struct List *next;
	struct Node *child;
};

struct List *createlistnode()
{
	struct List *temp;
	temp = (struct List *)malloc(sizeof(struct List));
	temp->next = NULL;
	return temp;
}

struct List *addList(struct List *head, struct Node *node)
{
	struct List *temp, *cur;
	temp = createlistnode();
	temp->child = node;
	if (head == NULL) // error prevention
	{
		head = temp;
	}
	else
	{

		cur = head;
		while (cur->next != NULL)
		{
			cur = cur->next;
		}
		cur->next = temp;

		return head;
	}
}

struct List *removeList(struct List *head, struct Node *remove)
{
	struct List *temp;
	temp = head;
	if (temp->child == remove)
	{
		head = temp->next;
		// free(temp);
		return head;
	}
	while (temp->next != NULL)
	{
		if (temp->next->child == remove)
		{
			temp->next = temp->next->next;
			return head;
		}
		else
		{
			temp = temp->next;
		}
	}
}

struct List *addFrontList(struct List *head, struct Node *node) // pushes obj to front of list
{
	struct List *temp, *cur;
	temp = createlistnode();
	temp->child = node;
	if (head == NULL) // error prevention
	{
		head = temp;
	}
	else
	{
		temp->next = head;
		head = temp;
		return head;
	}
}

struct List *removeFrontList(struct List *head)
{
	struct List *temp;
	temp = head;
	head = temp->next;
	// free(temp);
	return head;
}

bool existInList(struct List *head, struct Node *find)
{
	struct List *temp;
	temp = head;
	while (temp != NULL)
	{
		if (temp->child == find)
		{
			return true;
		}
		else
		{
			temp = temp->next;
		}
	}
	return false;
}

struct List *freeList(struct List *head)
{
	struct List *temp;
	struct List *temp2;
	temp = head;
	temp2 = head;
	while (temp != NULL)
	{
		// free(temp);
		temp = temp2->next;
		temp2 = temp2->next;
	}
}

struct Node *NodeList[NUMOFNODESTODECLADE];
struct Node *currNode;
struct Node *pathfindingNode;
struct Node *goalNode;

struct List *closedList; // nodes that we have "checked"
struct List *openList;	 // neighbouring nodes that have yet to be "checked"
struct List *pathList;

float ScanFoward()
{
	return 100;
}
float ScanLeft()
{
	return 100;
}
float ScanRight()
{
	return 100;
}

// void moveLeft(){	printf("hello world");}
// void moveRight(){	printf("hello world");}
// void moveFront(){	printf("hello world");}
// void moveBack(){	printf("hello world");}

void turnLeftMove()
{
	float currScanLength = 0;
	// turn_left(90);
	frontScannedLength = ScanFoward();
	currScanLength = frontScannedLength;
	// move_forward();
	while ((frontScannedLength - currScanLength) < NODELENGTH)
	{
		// currScanLength = ScanFoward();
		currScanLength--;
	}
	// stop_movement();
}
void moveForward()
{
	float currScanLength = 0;
	frontScannedLength = ScanFoward();
	currScanLength = frontScannedLength;
	// move_forward();
	while ((frontScannedLength - currScanLength) < NODELENGTH)
	{
		// currScanLength = ScanFoward();
		currScanLength--;
	}
	// stop_movement();
}
void turnRightMove()
{
	float currScanLength = 0;
	// turn_right(90);
	frontScannedLength = ScanFoward();
	currScanLength = frontScannedLength;
	// move_forward();
	while ((frontScannedLength - currScanLength) < NODELENGTH)
	{
		// currScanLength = ScanFoward();
		currScanLength--;
	}
	// stop_movement();
}
void turn180Move()
{
	float currScanLength = 0;
	// turn_left(90);
	// turn_left(90);
	frontScannedLength = ScanFoward();
	currScanLength = frontScannedLength;
	// move_forward();
	while ((frontScannedLength - currScanLength) < NODELENGTH)
	{
		// currScanLength = ScanFoward();
		currScanLength--;
	}
	// stop_movement();
}

void moveLeft()
{
	printf("move left\n");
	if (direction == 0)
	{
		currNode = currNode->left;
		turnLeftMove(); // pid turn car left and move forward [true north] to east
	}
	if (direction == 1)
	{
		currNode = currNode->left;
		moveForward(); // call pid to move car forward [true east]
	}
	if (direction == 2)
	{
		currNode = currNode->left;
		turnRightMove(); // turn car right and move forward [true south] to east
	}
	if (direction == 3)
	{
		currNode = currNode->left;
		turn180Move(); // turn pid 180 to move car [true west] to east
	}
	direction = 1;
}

void moveRight()
{
	printf("move right\n");
	if (direction == 0)
	{
		currNode = currNode->right;
		turnRightMove(); // pid turn car right and move forward [true north] to west
	}
	if (direction == 1)
	{
		currNode = currNode->right;
		turn180Move(); // pid turn car 180 and move forward [true east] to west
	}
	if (direction == 2)
	{
		currNode = currNode->right;
		turnLeftMove(); // pid turn car left and move forward [true south] to west
	}
	if (direction == 3)
	{
		currNode = currNode->right;
		moveForward(); // call pid to move car forward [true west]
	}
	direction = 3;
}
void moveBack()
{
	printf("move down\n");
	if (direction == 0)
	{
		currNode = currNode->back;
		turn180Move(); // pid turn car 180 and move forward [true north] to south
	}
	if (direction == 1)
	{
		currNode = currNode->back;
		turnLeftMove(); // pid turn car left and move forward [true east] to south
	}
	if (direction == 2)
	{
		currNode = currNode->back;
		moveForward(); // pid car move forward [true south]
	}
	if (direction == 3)
	{
		currNode = currNode->back;
		turnRightMove(); // call pid to move car forward [true west] to south
	}
	direction = 2;
}
void moveFront()
{
	printf("move foward\n");
	if (direction == 0)
	{
		currNode = currNode->front;
		moveForward(); // pid car move forward [true north] to north
	}
	if (direction == 1)
	{
		currNode = currNode->front;
		turnRightMove(); // pid turn car right and move forward [true west] to north
	}
	if (direction == 2)
	{
		currNode = currNode->front;
		turn180Move(); // pid turn car 180 and ove forward [true south] to north
	}
	if (direction == 3)
	{
		currNode = currNode->front;
		turnLeftMove(); // call pid to move car forward [true east] to north
	}
	direction = 0;
}

bool NodeInNodeList(int x, int y)
{
	for (int i; i < NUMOFNODESTODECLADE; i++)
	{
		if (NodeList[i] != NULL)
		{
			if (NodeList[i]->x == x && NodeList[i]->y == y)
			{
				return true;
			}
		}
	}
	return false;
}

void Move() // its an astar/move function
{
	// issue how to find the path to move to the empty node
	if (true)
	{
		pathfindingNode = currNode;
		freeList(closedList);
		freeList(openList);
		closedList = NULL;
		openList = NULL;
		closedList = addList(closedList, currNode);
		if (currNode->front != NULL) // setup openlist
		{
			openList = addList(openList, currNode->front);
			currNode->front->g = 1;
			currNode->front->h = abs(currNode->front->x - targetx) + abs(currNode->front->y - targety);
			currNode->front->f = currNode->front->g + currNode->front->h;
			currNode->front->parent = currNode;
		}
		if (currNode->back != NULL)
		{
			openList = addList(openList, currNode->back);
			currNode->back->g = 1;
			currNode->back->h = abs(currNode->back->x - targetx) + abs(currNode->back->y - targety);
			currNode->back->f = currNode->back->g + currNode->back->h;
			currNode->back->parent = currNode;
		}
		if (currNode->left != NULL)
		{
			openList = addList(openList, currNode->left);
			currNode->left->g = 1;
			currNode->left->h = abs(currNode->left->x - targetx) + abs(currNode->left->y - targety);
			currNode->left->f = currNode->left->g + currNode->left->h;
			currNode->left->parent = currNode;
		}
		if (currNode->right != NULL)
		{
			openList = addList(openList, currNode->right);
			currNode->right->g = 1;
			currNode->right->h = abs(currNode->right->x - targetx) + abs(currNode->right->y - targety);
			currNode->right->f = currNode->right->g + currNode->right->h;
			currNode->right->parent = currNode;
		}
		//== == == == == == == == == == == == == == == == == == == ;
		struct List *temp;
		while (openList != NULL)
		{
			pathfindingNode = openList->child;
			temp = openList;
			while (temp != NULL)
			{
				if (temp->child->f < pathfindingNode->f)
				{
					pathfindingNode = temp->child;
				}
				temp = temp->next;
			}

			openList = removeList(openList, pathfindingNode); // add current selected lowest f to closed list
			closedList = addList(closedList, pathfindingNode);

			temp = closedList;
			while (temp != NULL) // find current list lowest f
			{
				if (temp->child == goalNode)
				{
					// goalNode->parent = pathfindingNode;

					pathList = addFrontList(pathList, goalNode);
					pathList = addFrontList(pathList, pathfindingNode);
					while (pathfindingNode->parent != currNode)
					{
						pathList = addFrontList(pathList, pathfindingNode->parent);
						pathfindingNode = pathfindingNode->parent;
					}

					while (currNode != goalNode)
					{
						if (currNode->front == pathList->child)
						{
							moveFront();
							pathList = pathList->next;
						}
						else if (currNode->left == pathList->child)
						{
							moveLeft();
							pathList = pathList->next;
						}
						else if (currNode->right == pathList->child)
						{
							moveRight();
							pathList = pathList->next;
						}
						else if (currNode->back == pathList->child)
						{
							moveBack();
							pathList = pathList->next;
						}
						else
						{
							printf("im sorry idk how you ended up here the car is hentak kaki from this point onwards");
						}
					}
					goalNode->status = 1;
					printf("\nSuccessful navigated to Node \n"); // added this
					return;										 // return; // TODO: make path to move allong //DONE
				}
				temp = temp->next;
			}

			if (pathfindingNode->front != NULL) // add left front front and back to the open list
			{
				if (!existInList(closedList, pathfindingNode->front) && !existInList(openList, pathfindingNode->front))
				{
					openList = addList(openList, pathfindingNode->front);
					pathfindingNode->front->parent = pathfindingNode;
					pathfindingNode->front->g = pathfindingNode->g + 1;
					pathfindingNode->front->h = abs(pathfindingNode->front->x - targetx) + abs(pathfindingNode->front->y - targety);
					pathfindingNode->front->f = pathfindingNode->front->h + pathfindingNode->front->g;
				}
				else
				{
					if (pathfindingNode->front->g > pathfindingNode->g + 1)
					{
						pathfindingNode->front->parent = pathfindingNode;
						pathfindingNode->front->g = pathfindingNode->g + 1;
						pathfindingNode->front->f = pathfindingNode->front->h + pathfindingNode->front->g;
					}
				}
			}
			if (pathfindingNode->back != NULL) // add left back back and back to the open list
			{
				if (!existInList(closedList, pathfindingNode->back) && !existInList(openList, pathfindingNode->back))
				{
					openList = addList(openList, pathfindingNode->back);
					pathfindingNode->back->parent = pathfindingNode;
					pathfindingNode->back->g = pathfindingNode->g + 1;
					pathfindingNode->back->h = abs(pathfindingNode->back->x - targetx) + abs(pathfindingNode->back->y - targety);
					pathfindingNode->back->f = pathfindingNode->back->h + pathfindingNode->back->g;
				}
				else
				{
					if (pathfindingNode->back->g > pathfindingNode->g + 1)
					{
						pathfindingNode->back->parent = pathfindingNode;
						pathfindingNode->back->g = pathfindingNode->g + 1;
						pathfindingNode->back->f = pathfindingNode->back->h + pathfindingNode->back->g;
					}
				}
			}
			if (pathfindingNode->left != NULL) // add left right front and back to the open list
			{
				if (!existInList(closedList, pathfindingNode->left) && !existInList(openList, pathfindingNode->left))
				{
					openList = addList(openList, pathfindingNode->left);
					pathfindingNode->left->parent = pathfindingNode;
					pathfindingNode->left->g = pathfindingNode->g + 1;
					pathfindingNode->left->h = abs(pathfindingNode->left->x - targetx) + abs(pathfindingNode->left->y - targety);
					pathfindingNode->left->f = pathfindingNode->left->h + pathfindingNode->left->g;
				}
				else
				{
					if (pathfindingNode->left->g > pathfindingNode->g + 1)
					{
						pathfindingNode->left->parent = pathfindingNode;
						pathfindingNode->left->g = pathfindingNode->g + 1;
						pathfindingNode->left->f = pathfindingNode->left->h + pathfindingNode->left->g;
					}
				}
			}
			if (pathfindingNode->right != NULL) // add left right front and back to the open list
			{
				if (!existInList(closedList, pathfindingNode->right) && !existInList(openList, pathfindingNode->right))
				{
					openList = addList(openList, pathfindingNode->right);
					pathfindingNode->right->parent = pathfindingNode;
					pathfindingNode->right->g = pathfindingNode->g + 1;
					pathfindingNode->right->h = abs(pathfindingNode->right->x - targetx) + abs(pathfindingNode->right->y - targety);
					pathfindingNode->right->f = pathfindingNode->right->h + pathfindingNode->right->g;
				}
				else
				{
					if (pathfindingNode->right->g > pathfindingNode->g + 1)
					{
						pathfindingNode->right->parent = pathfindingNode;
						pathfindingNode->right->g = pathfindingNode->g + 1;
						pathfindingNode->right->f = pathfindingNode->right->h + pathfindingNode->right->g;
					}
				}
			}
		}
	}
}

void InitMapping() // generate neighbouring nodes around car currently
{

	NodeList[0] = createnode();
	currNode = NodeList[0];
	currNode->status = 1;
	currNode->x = 0;
	currNode->y = 0;

	frontScannedLength = ScanFoward();
	rightScannedLength = ScanRight();
	leftScannedLength = ScanLeft();
	currNode->back = NULL;
	if (frontScannedLength < NODELENGTH)
	{
		currNode->front = NULL;
	}
	else
	{
		if (!NodeInNodeList(0, 1))
		{
			currentUsedNode += 1;
			NodeList[currentUsedNode] = createnode();
			currNode->front = NodeList[currentUsedNode];
			currNode->front->back = currNode; // sets the new node to have the current node as its corresponding node side
			currNode->front->status = 0;
		}
	}
	if (rightScannedLength < NODELENGTH)
	{
		currNode->right = NULL;
	}
	else
	{
		if (!NodeInNodeList(1, 0))
		{
			currentUsedNode += 1;
			NodeList[currentUsedNode] = createnode();
			currNode->right = NodeList[currentUsedNode];
			currNode->right->left = currNode; // sets the new node to have the current node as its corresponding node side
			currNode->right->status = 0;
		}
	}
	if (leftScannedLength < NODELENGTH)
	{
		currNode->left = NULL;
	}
	else
	{
		if (!NodeInNodeList(-1, 0))
		{
			currentUsedNode += 1;
			NodeList[currentUsedNode] = createnode();
			currNode->left = NodeList[currentUsedNode];
			currNode->left->right = currNode; // sets the new node to have the current node as its corresponding node side
			currNode->left->status = 0;
		}
	}
}

// finds any unvisited nodes
void FindNode()
{
	for (size_t i = 0; i < NUMOFNODESTODECLADE; i++)
	{
		if (NodeList[i] != NULL)
		{
			if (NodeList[i]->status == 0)
			{
				goalNode = NodeList[i];
				targetx = NodeList[i]->x;
				targety = NodeList[i]->y;
				return;
			}
			// if (NodeList[i]->front != NULL)
			// {
			// 	if (NodeList[i]->front->status == 0)
			// 	{
			// 		goalNode = NodeList[i];
			// 		targetx = NodeList[i]->x;
			// 		targety = NodeList[i]->y;
			// 		return;
			// 	}
			// }
			// if (NodeList[i]->left != NULL)
			// {
			// 	if (NodeList[i]->left->status == 0)
			// 	{
			// 		goalNode = NodeList[i];
			// 		targetx = NodeList[i]->x;
			// 		targety = NodeList[i]->y;
			// 		return;
			// 	}
			// }
			// if (NodeList[i]->right != NULL)
			// {
			// 	if (NodeList[i]->right->status == 0)
			// 	{

			// 		goalNode = NodeList[i];
			// 		targetx = NodeList[i]->x;
			// 		targety = NodeList[i]->y;
			// 		return;
			// 	}
			// }
		}
	}
	// if (i == NUMOFNODESTODECLADE)
	mappingCompleted = true;
}

void GenerateNeighbourNode()
{
	frontScannedLength = ScanFoward();
	rightScannedLength = ScanRight();
	leftScannedLength = ScanLeft();

	if (frontScannedLength < MINWALLLENGTH)
	{
		if (direction == 0)
		{
			currNode->front = NULL;
		}
		else if (direction == 1)
		{
			currNode->left = NULL;
		}
		else if (direction == 2)
		{
			currNode->back = NULL;
		}
		else if (direction == 3)
		{
			currNode->right = NULL;
		}
	}
	else
	{
		if (direction == 0)
		{
			if (!NodeInNodeList(currNode->x, currNode->y + 1))
			{
				currentUsedNode += 1;
				NodeList[currentUsedNode] = createnode();
				currNode->front = NodeList[currentUsedNode];
				currNode->front->back = currNode; // sets the new node to have the current node as its corresponding node side
				currNode->front->status = 0;
			}
		}
		else if (direction == 1)
		{
			if (!NodeInNodeList(currNode->x - 1, currNode->y))
			{
				currentUsedNode += 1;
				NodeList[currentUsedNode] = createnode();
				currNode->left = NodeList[currentUsedNode];
				currNode->left->right = currNode; // sets the new node to have the current node as its corresponding node side
				currNode->left->status = 0;
			}
		}
		else if (direction == 2)
		{
			if (!NodeInNodeList(currNode->x, currNode->y - 1))
			{
				currentUsedNode += 1;
				NodeList[currentUsedNode] = createnode();
				currNode->back = NodeList[currentUsedNode];
				currNode->back->front = currNode; // sets the new node to have the current node as its corresponding node side
				currNode->back->status = 0;
			}
		}
		else if (direction == 3)
		{
			if (!NodeInNodeList(currNode->x + 1, currNode->y))
			{
				currentUsedNode += 1;
				NodeList[currentUsedNode] = createnode();
				currNode->right = NodeList[currentUsedNode];
				currNode->right->left = currNode; // sets the new node to have the current node as its corresponding node side
				currNode->right->status = 0;
			}
		}
	}
	if (rightScannedLength < MINWALLLENGTH)
	{
		if (direction == 0)
		{
			currNode->right = NULL;
		}
		else if (direction == 1)
		{
			currNode->front = NULL;
		}
		else if (direction == 2)
		{
			currNode->left = NULL;
		}
		else if (direction == 3)
		{
			currNode->back = NULL;
		}
	}
	else
	{
		if (direction == 0)
		{
			if (!NodeInNodeList(currNode->x + 1, currNode->y))
			{
				currentUsedNode += 1;
				NodeList[currentUsedNode] = createnode();
				currNode->right = NodeList[currentUsedNode];
				currNode->right->left = currNode; // sets the new node to have the current node as its corresponding node side
				currNode->right->status = 0;
			}
		}
		else if (direction == 1)
		{
			if (!NodeInNodeList(currNode->x, currNode->y + 1))
			{
				currentUsedNode += 1;
				NodeList[currentUsedNode] = createnode();
				currNode->front = NodeList[currentUsedNode];
				currNode->front->back = currNode; // sets the new node to have the current node as its corresponding node side
				currNode->front->status = 0;
			}
		}
		else if (direction == 2)
		{
			if (!NodeInNodeList(currNode->x - 1, currNode->y))
			{
				currentUsedNode += 1;
				NodeList[currentUsedNode] = createnode();
				currNode->left = NodeList[currentUsedNode];
				currNode->left->right = currNode; // sets the new node to have the current node as its corresponding node side
				currNode->left->status = 0;
			}
		}
		else if (direction == 3)
		{
			if (!NodeInNodeList(currNode->x, currNode->y - 1))
			{
				currentUsedNode += 1;
				NodeList[currentUsedNode] = createnode();
				currNode->back = NodeList[currentUsedNode];
				currNode->back->front = currNode; // sets the new node to have the current node as its corresponding node side
				currNode->back->status = 0;
			}
		}
	}
	if (leftScannedLength < MINWALLLENGTH)
	{
		if (direction == 0)
		{
			currNode->left = NULL;
		}
		else if (direction == 1)
		{
			currNode->back = NULL;
		}
		else if (direction == 2)
		{
			currNode->right = NULL;
		}
		else if (direction == 3)
		{
			currNode->front = NULL;
		}
	}
	else
	{
		if (direction == 0)
		{
			if (!NodeInNodeList(currNode->x - 1, currNode->y))
			{
				currentUsedNode += 1;
				NodeList[currentUsedNode] = createnode();
				currNode->left = NodeList[currentUsedNode];
				currNode->left->right = currNode; // sets the new node to have the current node as its corresponding node side
				currNode->left->status = 0;
			}
		}
		else if (direction == 1)
		{
			if (!NodeInNodeList(currNode->x, currNode->y - 1))
			{
				currentUsedNode += 1;
				NodeList[currentUsedNode] = createnode();
				currNode->back = NodeList[currentUsedNode];
				currNode->back->front = currNode; // sets the new node to have the current node as its corresponding node side
				currNode->back->status = 0;
			}
		}
		else if (direction == 2)
		{
			if (!NodeInNodeList(currNode->x + 1, currNode->y))
			{
				currentUsedNode += 1;
				NodeList[currentUsedNode] = createnode();
				currNode->right = NodeList[currentUsedNode];
				currNode->right->left = currNode; // sets the new node to have the current node as its corresponding node side
				currNode->right->status = 0;
			}
		}
		else if (direction == 3)
		{
			if (!NodeInNodeList(currNode->x, currNode->y + 1))
			{
				currentUsedNode += 1;
				NodeList[currentUsedNode] = createnode();
				currNode->front = NodeList[currentUsedNode];
				currNode->front->back = currNode; // sets the new node to have the current node as its corresponding node side
				currNode->front->status = 0;
			}
		}
	}
}
void Map()
{
	InitMapping();
	while (!mappingCompleted)
	{
		FindNode();
		if (mappingCompleted)
		{
			break;
		}
		Move(); 
		GenerateNeighbourNode();
	}
}

void gotoNode(int zzx, int zzy)
{
	struct Node *zzNode;

	for (int i; i < NUMOFNODESTODECLADE; i++)
	{
		if (NodeList[i] != NULL)
		{
			if (NodeList[i]->x == zzx && NodeList[i]->y == zzy)
			{
				goalNode = NodeList[i];
				targetx = goalNode->x;
				targety = goalNode->y;
				Move();
				break;
			}
		}
	}
}

int main()
{
	int i, input1, input2, curX, curY;
	input1 = 0;
	input2 = 0;
	curX= 0;
	curY = 0;

	for (i = 0; i < 9; i++)
	{

		NodeList[i] = createnode();
	}

	// 0,0 column 0, row 1, nodelist 0
	NodeList[0]->x = 0;
	NodeList[0]->y = 0;
	NodeList[0]->front = NodeList[1];
	// NodeList[0]->right = NodeList[3];
	NodeList[0]->status = 1;

	// 0,1 column 0, row 1, nodelist 1
	NodeList[1]->front = NodeList[4];
	NodeList[1]->back = NodeList[0];
	NodeList[1]->right = NodeList[2];
	NodeList[1]->x = 0;
	NodeList[1]->y = 1;
	NodeList[1]->status = 1;

	// 1,1 column 1, row 2, nodelist 2
	NodeList[2]->front = NodeList[5];
	NodeList[2]->left = NodeList[1];
	NodeList[2]->right = NodeList[7];
	NodeList[2]->back = NodeList[3];
	NodeList[2]->x = 1;
	NodeList[2]->y = 1;
	NodeList[2]->status = 1;

	// 1,0 column 1, row 1, nodelist 3
	NodeList[3]->right = NodeList[8];
	NodeList[3]->front = NodeList[2];
	// NodeList[3]->left = NodeList[0];
	NodeList[3]->x = 1;
	NodeList[3]->y = 0;
	NodeList[3]->status = 1;

	// 0,2 column 0, row 3, nodelist 4
	NodeList[4]->back = NodeList[1];
	NodeList[4]->right = NodeList[5];
	NodeList[4]->x = 0;
	NodeList[4]->y = 2;
	NodeList[4]->status = 1;

	// 1,2 column 1, row 3, nodelist 5
	NodeList[5]->back = NodeList[2];
	NodeList[5]->right = NodeList[6];
	NodeList[5]->left = NodeList[4];
	NodeList[5]->x = 1;
	NodeList[5]->y = 2;
	NodeList[5]->status = 1;

	// 2,2 column 2, row 3, nodelist 6
	NodeList[6]->back = NodeList[7];
	NodeList[6]->left = NodeList[5];
	NodeList[6]->x = 2;
	NodeList[6]->y = 2;
	NodeList[6]->status = 1;

	// 2,1 column 2, row 2, nodelist 7
	NodeList[7]->back = NodeList[8];
	NodeList[7]->left = NodeList[2];
	NodeList[7]->front = NodeList[6];
	NodeList[7]->x = 2;
	NodeList[7]->y = 1;
	NodeList[7]->status = 1;

	// 2,0 column 2, row 1, nodelist 8
	NodeList[8]->left = NodeList[3];
	NodeList[8]->front = NodeList[7];
	NodeList[8]->x = 2;
	NodeList[8]->y = 0;
	NodeList[8]->status = 1;

	currNode = NodeList[0];

	while (!mappingCompleted)
	{
		printf("========================\n");
		printf("Choose your destination:\n");
		printf("You are at (%d,%d)\n\nThere is a wall inbetween node 0 and 3\nNode 4 (0,2) Node 5 (1,2) Node 6 (2,2)\nNode 1 (0,1) Node 2 (1,1) Node 7 (2,1)\nNode 0 (0,0)|Node 3 (1,0) Node 8 (2,0)\n", curX, curY);
		printf("\nEnter the x coordinate: \n");
		scanf("%d", &input1);
		printf("\nEnter the y coordinate: \n");
		scanf("%d", &input2);
		printf("\nYour end coordinate entered is: %d, %d\n", input1, input2);
		// hard-coded error message
		if (input1 < 0 || input1 > 2 || input2 < 0 || input2 > 2)
		{
			printf("\nInput Out of Bounds\n");
			continue;
		}
		else
		{
			curX = input1;
			curY = input2;
			gotoNode(input1, input2);
		}
	}
}
