#ifndef __PATHFINDING_H__
#define __PATHFINDING_H__

#include "Module.h"

#include "Point.h"
#include "DynArray.h"
#include "List.h"

#define DEFAULT_PATH_LENGTH 50
#define INVALID_WALK_CODE 255

class PathFinding : public Module
{
public:

	PathFinding();

	~PathFinding();

	bool CleanUp();

	void SetMap(uint width, uint height, uchar* data);

	int CreatePath(const iPoint& origin, const iPoint& destination);

	const DynArray<iPoint>* GetLastPath() const;

	bool CheckBoundaries(const iPoint& pos) const;

	bool IsWalkable(const iPoint& pos) const;

	uchar GetTileAt(const iPoint& pos) const;

private:

	uint width;
	uint height;

	uchar* map;

	DynArray<iPoint> lastPath;
};

struct PathList;

struct PathNode
{
	int g;
	int h;
	iPoint pos;
	const PathNode* parent;

	PathNode();
	PathNode(int g, int h, const iPoint& pos, const PathNode* parent);
	PathNode(const PathNode& node);

	uint FindWalkableAdjacents(PathList& list_to_fill) const;

	int Score() const;

	int CalculateF(const iPoint& destination);
};

struct PathList
{
	ListItem<PathNode>* Find(const iPoint& point) const;

	ListItem<PathNode>* GetNodeLowestScore() const;

	List<PathNode> list;
};

#endif // __PATHFINDING_H__