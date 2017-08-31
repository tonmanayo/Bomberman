#include <MainGame.hpp>
#include <player.hpp>

#define GLM_ENABLE_EXPERIMENTAL

class   Scene
{
private:
	MainGame    *_game;
	size_t      _mapWidth;
	size_t      _mapLength;
	int         _enemyCount;
	Player      *_player;
	std::vector<std::string> *_map;
private:
	void    _addWall(float x, float z);
	void    _addBreakableBlock(float x, float z);
	void    _addUnbreakableBlock(float x, float z);
	void    _addFloor(float x, float z);
	void    _addPlayer(float x, float z);
public:
	Scene(MainGame *game, std::vector<std::string> *map, int enemyCount);
	Scene(const Scene & rhs);
	Scene&  operator=(const Scene & rhs);
	~Scene();

	bool    buildMap();

public:
	static  void    updatePlayer(MainGame *game, std::vector<void *> params);
};