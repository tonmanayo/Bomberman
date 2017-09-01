#include <MainGame.hpp>
#include <player.hpp>
#include <block.hpp>

#define GRID_START_X (0)
#define GRID_START_Z (0)
#define GRID_BLOCK_SIZE (2)

class   Scene
{
private:
	MainGame    *_game;
	size_t      _mapWidth;
	size_t      _mapLength;
	int         _enemyCount;
	Player      *_player;
	std::vector<std::string> *_map;
	std::map<int, std::map<int, Block *>>    _blocks;
private:
	void    _addWall(float x, float z);
	void    _addBreakableBlock(float x, float z, int xx, int yy);
	void    _addUnbreakableBlock(float x, float z, int xx, int yy);
	void    _addFloor(float x, float z);
	void    _addPlayer(float x, float z);
public:
	Scene(MainGame *game, std::vector<std::string> *map, int enemyCount);
	Scene(const Scene & rhs);
	Scene&  operator=(const Scene & rhs);
	~Scene();

	bool    buildMap();
	static bool    worldCollision(glm::vec3 pos, glm::vec3 offset);

public:
	static  void    updatePlayer(MainGame *game, std::vector<void *> params);
};