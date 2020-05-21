#pragma once

class Player;
class Ghost;
class Tile;
class TileMap;


// 처음엔 충돌 관련만 넣으려 햇는데 그냥 잡다한거 다 넣음
class Collision
{
public:
	static void WithSPellet(Player* player, TileMap* tm);
	static void WithLPellet(Player* player, vector<Ghost*> ghosts, TileMap* tm);
	static void WithGhost(Player* player, Ghost* ghost, TileMap* tm);


	static bool IsWin();

private:
	static uint pelletCount;

};