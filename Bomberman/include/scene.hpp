#include <MainGame.hpp>
#include <player.hpp>
#include <block.hpp>
#include <glm/geometric.hpp>
#include <glm/common.hpp>
#include "Bomb.hpp"

#define GRID_START_X (0)
#define GRID_START_Z (0)
#define GRID_BLOCK_SIZE (2.0)
#define HALF_GRID_BLOCK_SIZE (GRID_BLOCK_SIZE / (2.0))
#define PLAYER_SIZE (2.05)
#define HALF_PLAYER_SIZE (PLAYER_SIZE / (2.0))

class   Scene
{
private:
	MainGame    								*_game;
	size_t      								_mapWidth;
	size_t      								_mapLength;
	int         								_enemyCount;
	Player      								*_player;
	std::vector<Bomb>							_bomb;
	std::vector<Player*>						_enemies;
    int                 						_nbBombs;
	std::vector<std::string> *					_map;
	std::map<int, std::map<int, Block *>>		_blocks;

private:
	void    _addWall(float x, float z, int xx, int yy);
	void    _addBreakableBlock(float x, float z, int xx, int yy);
	void    _addUnbreakableBlock(float x, float z, int xx, int yy);
	void    _addFloor(float x, float z);
	void    _addPlayer(float x, float z);
	void 	_addBomb(float x, float z);
	void 	_addEnemy(float x, float z);
public:
	Scene(MainGame *game, std::vector<std::string> *map, int enemyCount);
	Scene(const Scene & rhs);
	Scene&  operator=(const Scene & rhs);
	~Scene();

    std::map<int, std::map<int, Block *>> getBlocks() {return _blocks;};
    int             getWorldx(float x);
    int             getWorldy(float y);
    float           getGridx(float x);
    float           getGridy(float y);

	bool            buildMap();
	static bool     worldCollisionUp(glm::vec3 pos, glm::vec3 offset, Scene *scene);
	static bool     worldCollisionDown(glm::vec3 pos, glm::vec3 offset, Scene *scene);
	static bool     worldCollisionLeft(glm::vec3 pos, glm::vec3 offset, Scene *scene);
	static bool     worldCollisionRight(glm::vec3 pos, glm::vec3 offset, Scene *scene);
	static bool     checkBlockCollision(glm::vec3 blockPos, glm::vec3 entityPos);
	static bool     checkBlockCollision1(glm::vec3 blockPos, glm::vec3 entityPos);
	static void		updateBomb(MainGame *game, std::vector<void *> params);

    static bool     breakableBrickCollisionDown(glm::vec3 pos, Scene *scene);
    static bool     breakableBrickCollisionUp(glm::vec3 pos, Scene *scene);
    static bool     breakableBrickCollisionRight(glm::vec3 pos, Scene *scene);
    static bool     breakableBrickCollisionLeft(glm::vec3 pos, Scene *scene);

	static bool     PlayerExplosionCollision(glm::vec3 pos, Scene *scene);

public:
	static  void    updatePlayer(MainGame *game, std::vector<void *> params);
    static  void    updateEnemy(MainGame *game, std::vector<void *> params);
    static  void    bombExplode(std::vector<void *> params, const Bomb &bomb);
};