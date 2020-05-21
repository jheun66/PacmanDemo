#pragma once

class Player;
class Ghost;
class Tile;
class TileMap;


// ó���� �浹 ���ø� ������ �޴µ� �׳� ����Ѱ� �� ����
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