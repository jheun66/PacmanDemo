#include "Framework.h"
#include "Tile.h"

float Tile::GetDistance(Tile * node)
{
	// 요기 바꿔주자!
	float distX = abs(position.x - node->position.x);
	float distY = abs(position.y - node->position.y);

	return distX + distY;
}

bool PathFinder::GetPath(Tile * start, Tile * end, vector<Tile*>& path)
{
	Clear();

	//float elapsedTime = 0.0f;

	Tile* currentNode = nullptr;
	Tile* childNode = nullptr;

	float f = 0.0f, g = 0.0f, h = 0.0f;
	openList.push_back(start);
	start->isOpen = true;

	while (!openList.empty())
	{
		//if (elapsedTime > limitTime) return false;

		// 내림차순으로 정렬
		sort(openList.begin(), openList.end(), 
			[](Tile* lhs, Tile* rhs) -> bool
		{
			return lhs->GetF() > rhs->GetF();
			//lhs가 rhs보다 앞에 올 조건
		});

		currentNode = openList.back();
		openList.pop_back();
		currentNode->isOpen = false;

		closeList.push_back(currentNode);
		currentNode->isClose = true;

		if (currentNode == end)
		{
			ReConstructPath(currentNode, path);
			return true;
		}

		for (ChildNode& child : currentNode->childs)
		{
			childNode = child.first;

			g = currentNode->g + child.second;

			/*bool isCheck = true;
			isCheck &= childNode->isClose || childNode->isOpen;
			isCheck &= childNode->g < g;
			if (isCheck)
				continue;*/

			// closelist에 있으면 바로 빠져나옴.
			bool isCheck = true;
			isCheck &= childNode->isClose;
			if (isCheck)
				continue;

			// openlist에 있을 때, g값 비교
			isCheck = true;
			isCheck &= childNode->isOpen;
			isCheck &= childNode->g < g;
			if (isCheck)
				continue;

			h = childNode->GetDistance(end);
			f = g + h;

			childNode->f = f;
			childNode->g = g;
			childNode->h = h;
			childNode->parentNode = currentNode;
			
			if (!childNode->isOpen)
			{
				openList.push_back(childNode);
				childNode->isOpen = true;
			}
		}

		//elapsedTime += Time::Delta();

	}

	return false;
}

void PathFinder::ReConstructPath(Tile * node, vector<Tile*>& path)
{
	Tile* parent = node->parentNode;
	path.push_back(node);

	while (parent != nullptr)
	{
		path.push_back(parent);
		parent = parent->parentNode;
	}
}

void PathFinder::Clear()
{
	openList.clear();
	openList.shrink_to_fit();

	closeList.clear();
	closeList.shrink_to_fit();
}
