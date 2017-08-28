#include <MainGame.hpp>
#include <player.hpp>
#include <block.hpp>
#include <glm/geometric.hpp>
#include <glm/common.hpp>

#define GRID_START_X (-4)
#define GRID_START_Z (-1)
#define GRID_BLOCK_SIZE (2)
#define PLAYER_SIZE (1.5)

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
	static bool     worldCollision(glm::vec3 pos, glm::vec3 offset, Scene *scene);
	static bool     checkBlockCollision(glm::vec3 blockPos, glm::vec3 entityPos);

public:
	static  void    updatePlayer(MainGame *game, std::vector<void *> params);
};