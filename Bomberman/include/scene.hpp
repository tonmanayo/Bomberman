#include <MainGame.hpp>
#include <player.hpp>
#include <block.hpp>
#include <glm/geometric.hpp>
#include <glm/common.hpp>
#include "Bomb.hpp"
#include <fstream>
#include <stdlib.h>

#define GRID_START_X (0)
#define GRID_START_Z (0)
#define GRID_BLOCK_SIZE (2.0)
#define HALF_GRID_BLOCK_SIZE (GRID_BLOCK_SIZE / (2.0))
#define PLAYER_SIZE (2.0)
#define HALF_PLAYER_SIZE (PLAYER_SIZE / (2.0))

class   Scene
{
private:
	MainGame    							*_game;
	int         							_enemyCount = 0;
	Player      							*_player;
	std::vector<Bomb>						_bomb;
	std::vector<Player *>   				_enemies;
	std::vector<std::string> 				*_map;
	std::map<int, std::map<int, Block *>>   _blocks;
	int                                     _sceneLevel = 1;
	bool                                    _endLevel;
	bool				   					_levelCompleted = false;
	int								        _difficulty = 2;
	int 									_powerSpeed;
	int                                     _score = 0;
	int                                     _prevScore = 0;
	float                                   _dropStartTime;
	bool                                    _dropped = false;
private:
	std::string                 _floorType;
	std::string                 _wallType;
	std::string                 _unbreakableBlockType;
	std::string                 _breakableBlockType;
	std::string                 _backgroundType;
	std::string                 _mapName;
	std::vector<std::string>    _enemyType;
	float  			            z = GRID_START_Z;
	int 			            gridY = 0;
public:
	size_t		_mapWidth = 0;
	size_t   	_mapLength = 0;

private:
	void    _addWall(float x, float z, int xx, int yy);
	void    _addBreakableBlock(float x, float z, int xx, int yy);
	void    _addUnbreakableBlock(float x, float z, int xx, int yy);
	void    _addBackground();
	void    _addFloor(float x, float z);
	void    _addPlayer(float x, float z);
	void 	_addBomb(float x, float z);
	void 	_addEnemy(float x, float z, std::string &type);
    void    _addPowerUps(float x, float z, int xx, int yy);
	void    _loadNewGameLine(std::string& line);
	void    _loadLoadGameLine(std::string& line);
public:
	Scene() = default;
	Scene(const Scene & rhs);
	Scene&  operator=(const Scene & rhs);
	~Scene();

	static void		collisionPush(Scene *scene, int x, int y, int i, int dirx, int diry);
	std::map<int, std::map<int, Block *>> 	getBlocks() {return _blocks;};
	size_t      getEnemyCount(){ return _enemies.size(); }

    int         getWorldx(float x);
    int  		getWorldy(float y);
    float       getGridx(float x);
    float       getGridy(float y);
	int 		getDifficulty();
	int         getDifficultyValue(){return _difficulty;}
	void		setDifficulty(int value);
	int         getLevel(){return _sceneLevel; }
	void        setLevel(int level){ _sceneLevel = level; }
    void 		CalcEndPos();
	bool        saveGame(std::string fileName);
	bool	    loadGame(MainGame *game, std::string fileName);
	bool        newGame(MainGame *game, std::string mapName);
	bool        isLevelCompleted(){ return _levelCompleted; }
	void        setScore(int score){_score = score; _prevScore = score;}
	void        increaseScore(int val){_score += val;}
	int         getScore(){return _score;}
	int         getPrevScore(){ return _prevScore;}

	static bool checkBlockCollision(glm::vec3 blockPos, glm::vec3 entityPos);
    static bool worldEndLevel(glm::vec3 pos, Scene *scene);
    static void worldGetPower(glm::vec3 pos, Scene *scene);
    static bool breakableBrickCollisionDown(glm::vec3 pos, Scene *scene);
    static bool breakableBrickCollisionUp(glm::vec3 pos, Scene *scene);
    static bool breakableBrickCollisionRight(glm::vec3 pos, Scene *scene);
    static bool breakableBrickCollisionLeft(glm::vec3 pos, Scene *scene);
	static bool PlayerExplosionCollision(glm::vec3 pos, Scene *scene);
	static void enemiesExplosionCollision(glm::vec3 pos, Scene *scene);
    static void renderExplosion(Scene *scene, Bomb &bomb, MainGame *game);
public:
	static	char	oppDir(char dir);
	static  void	updateBomb(MainGame *game, Scene *scene);
	static  void    updatePlayer(MainGame *game, Scene *scene);
    static  void    updateEnemy(MainGame *game,  Scene *scene);
    static  void    bombExplode(Scene *scene, const Bomb &bomb);
	static  void    sceneUpdate(MainGame *game, std::vector<void *> params);
    static  void    bombExplode(std::vector<void *> params, const Bomb &bomb);

    static bool     worldCollisionDown(Player *enemy, glm::vec3 offset, Scene *scene);
    static bool     worldCollisionUp(Player *enemy, glm::vec3 offset, Scene *scene);
    static bool     worldCollisionRight(Player *enemy, glm::vec3 offset, Scene *scene);
    static bool     worldCollisionLeft(Player *enemy, glm::vec3 offset, Scene *scene);

    static void     enemyFollowPlayer(bool collision, std::map<int, std::map<int, Block *>> _blocks, Player *_enemies, Player *_player, int x, int y);

	static void     renderFlameExplosion(glm::vec3 flamePosition);

    bool            enemyPlayerCollision(Player *enemy, Scene *scene);
	Player          *getPlayer(){ return _player; }
};