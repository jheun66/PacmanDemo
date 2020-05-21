#pragma once

class Tile
{
	friend class PathFinder;
public:
	Vector3 GetPosition() { return position; }
	void SetPosition(Vector3 position) { this->position = position; }

	Vector2 GetStartUV() { return startUV; }
	void SetStartUV(Vector2 startUV) { this->startUV = startUV; }

	Vector2 GetEndUV() { return endUV; }
	void SetEndUV(Vector2 endUV) { this->endUV = endUV; }

	Color GetColor() { return color; }
	void SetColor(Color color) { this->color = color; }

	string GetSpriteName() { return spriteName; }
	void SetSpriteName(string spriteName) { this->spriteName = spriteName; }

	// 맨하탄 거리사용 (유클리디안은 대각선 뚫고 가니)
	float GetDistance(Tile* node); // h값을 만드는 함수

	float GetF() { return f; }
	float GetG() { return g; }
	float GetH() { return h; }


	void AddChild(pair<Tile*, float> child)
	{
		childs.push_back(child);
	}

	bool GetIsWalkable() { return isWalkable; }
	void SetIsWalkable(bool isWalkable) { this->isWalkable = isWalkable; }

	uint GetIndex() { return index; }
	void SetIndex(uint index) { this->index = index; }

	void Release()
	{
		isOpen = isClose = false;
		f = g = h = 0.0f;
		childs.clear();
		childs.shrink_to_fit();
		parentNode = nullptr;

		color = Values::Black;
	}

	bool IsEaten() { return isEaten; }
	void EatPallet() { isEaten = true; }

private:
	Vector3 position = Values::ZeroVector;
	Vector2 startUV = Vector2(0, 0);
	Vector2 endUV = Vector2(0, 0);
	Color color = Values::Black;
	
	uint index = -1;

	bool isWalkable = true;
	string spriteName = "";


	bool isEaten = false;

	float f = 0.0f;
	float g = 0.0f;
	float h = 0.0f;

	bool isOpen = false;
	bool isClose = false;

	Tile* parentNode = nullptr;
	vector<pair<Tile*, float>> childs;
};

class PathFinder : public SingletonBase<PathFinder>
{
public:
	friend class SingletonBase<PathFinder>;

	typedef pair<Tile*, float> ChildNode;

public:
	bool GetPath(Tile* start, Tile* end, vector<Tile*>& path);
	static void ReConstructPath(Tile* node, vector<Tile*>& path);

	void Clear();

private:
	PathFinder() = default;
	~PathFinder() = default;

private:
	vector<Tile*> openList;
	vector<Tile*> closeList;

	float limitTime = 2.0f;
};


// 길찾기 A* (다익스트라) 
// 트리구조 최소비용 신장트리
//  F G+H
//  G 이동비용 누적값
//  H(휴리스틱 추정치)
//
//  openlist
//  closelist