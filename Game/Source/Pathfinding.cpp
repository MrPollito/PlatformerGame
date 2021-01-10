#include "App.h"
#include "PathFinding.h"

#include "Defs.h"
#include "Log.h"

PathFinding::PathFinding() : Module(), map(NULL), lastPath(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name.Create("pathfinding");
}

PathFinding::~PathFinding()
{
	RELEASE_ARRAY(map);
}

bool PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	lastPath.Clear();
	RELEASE_ARRAY(map);

	return true;
}

void PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width * height];
	memcpy(map, data, width * height);
}

bool PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}

bool PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE;
}

uchar PathFinding::GetTileAt(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return map[(pos.y * width) + pos.x];

	return INVALID_WALK_CODE;
}

const DynArray<iPoint>* PathFinding::GetLastPath() const
{
	return &lastPath;
}

ListItem<PathNode>* PathList::Find(const iPoint& point) const
{
	ListItem<PathNode>* item = list.start;
	while (item)
	{
		if (item->data.pos == point)
			return item;
		item = item->next;
	}
	return NULL;
}

ListItem<PathNode>* PathList::GetNodeLowestScore() const
{
	ListItem<PathNode>* ret = NULL;
	int min = 65535;

	ListItem<PathNode>* item = list.end;
	while (item)
	{
		if (item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}

PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

uint PathNode::FindWalkableAdjacents(PathList& listToFill) const
{
	iPoint cell;
	uint before = listToFill.list.Count();

	// north
	cell.Create(pos.x, pos.y + 1);
	if (app->pathfinding->IsWalkable(cell))
		listToFill.list.Add(PathNode(-1, -1, cell, this));

	// south
	cell.Create(pos.x, pos.y - 1);
	if (app->pathfinding->IsWalkable(cell))
		listToFill.list.Add(PathNode(-1, -1, cell, this));

	// east
	cell.Create(pos.x + 1, pos.y);
	if (app->pathfinding->IsWalkable(cell))
		listToFill.list.Add(PathNode(-1, -1, cell, this));

	// west
	cell.Create(pos.x - 1, pos.y);
	if (app->pathfinding->IsWalkable(cell))
		listToFill.list.Add(PathNode(-1, -1, cell, this));

	return listToFill.list.Count();
}

int PathNode::Score() const
{
	return g + h;
}

int PathNode::CalculateF(const iPoint& destination)
{
	g = parent->g + 1;
	h = pos.DistanceTo(destination);

	return g + h;
}

int PathFinding::CreatePath(const iPoint& origin, const iPoint& destination)
{
	if (IsWalkable(origin) == false || IsWalkable(destination) == false)
	{
		return -1;
	}
	lastPath.Clear();

	PathList open;
	PathList closed;
	open.list.Add(PathNode(0, origin.DistanceTo(destination), origin, NULL));
	PathNode* currentNode;

	while (open.GetNodeLowestScore() != NULL)
	{
		currentNode = new PathNode(open.GetNodeLowestScore()->data);
		closed.list.Add(*currentNode);
		open.list.Del(open.Find(currentNode->pos));
		if (currentNode->pos == destination)
		{
			const PathNode* iterator = currentNode;

			for (iterator; iterator->pos != origin; iterator = iterator->parent)
			{
				lastPath.PushBack(iterator->pos);
			}
			lastPath.PushBack(origin);

			lastPath.Flip();
			return 0;
		}

		PathList adjacentList;
		uint limit = currentNode->FindWalkableAdjacents(adjacentList);

		for (uint i = 0; i < limit; i++) {
			if ((closed.Find(adjacentList.list[i].pos)) == NULL)
			{
				if ((open.Find(adjacentList.list[i].pos)) == NULL)
				{
					adjacentList.list[i].CalculateF(destination);
					open.list.Add(adjacentList.list[i]);
				}

				else {
					if (adjacentList.list[i].g < open.Find(adjacentList.list[i].pos)->data.g) {
						adjacentList.list[i].CalculateF(destination);
						open.list.Del(open.Find(adjacentList.list[i].pos));
						open.list.Add(adjacentList.list[i]);
					}
				}
			}
		}
	}
}

