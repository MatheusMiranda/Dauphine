#include "LevelTwo.h"
#include "Game.h"
#include "LuaScript.h"
#include "Logger.h"
#include "Enemy.h"
#include "Crosshair.h"
#include "TileMap.h"
#include "Collision.h"
#include "Crosshair.h"

LevelTwo::LevelTwo() :
	Level(),
	items{5000, 6000, 7000, 8000},
	caughtItems{false,false,false,false}
{

}

LevelTwo::~LevelTwo(){

}

void LevelTwo::load(){
	// Changing the music.
	Game::instance().getAudioHandler().changeMusic("res/audio/lv1.wav");

	// Loading the tile/tilemap.
	this->tileMap = new TileMap("res/maps/level2.tmx");

	// Setting the level width/height.
	this->width = this->tileMap->getMapWidth();
	this->height = this->tileMap->getMapHeight();
	SDL_Rect bounds = {0, 0, (int)this->width, (int)this->height};
	this->quadTree = new QuadTree(0, bounds);

	this->background = Game::instance().getResources().get("res/images/lv1_background.png");
	this->checkpoint = Game::instance().getResources().get("res/images/checkpoint.png");

	// Getting information from lua script.
	LuaScript luaLevel1("lua/Level1.lua");
	const std::string pathPlayerSpriteSheet = luaLevel1.unlua_get<std::string>(
		"level.player.spriteSheet");
	const std::string pathBackgroundAudio = luaLevel1.unlua_get<std::string>(
		"level.audio.background");
	const std::string pathEnemy = luaLevel1.unlua_get<std::string>("level.enemy");

	// Changing the music.
	// Game::instance().getAudioHandler().changeMusic(pathBackgroundAudio);

	// Loading the player and the camera.
	Player* lPlayer = new Player(this->tileMap->getInitialX(), this->tileMap->getInitialY(), pathPlayerSpriteSheet);
	Camera* lCamera = new Camera(lPlayer); 
	
	this->playerHud = new PlayerHUD(lPlayer);
	
	Enemy* lEnemy = new Enemy(3712.0, 1400.0, pathEnemy, false, 0.0);
	lEnemy->setLevelWH(this->width, this->height);
	this->enemies.push_back(lEnemy);

	Enemy* lEnemy2 = new Enemy(4992.0, 1400.0, pathEnemy, false, 0.0);
	lEnemy2->setLevelWH(this->width, this->height);
	this->enemies.push_back(lEnemy2);

	Enemy* lEnemy3 = new Enemy(5568.0, 1400.0, pathEnemy, true, 0.0);
	lEnemy3->setLevelWH(this->width, this->height);
	this->enemies.push_back(lEnemy3);

	Enemy* lEnemy4 = new Enemy(7104.0, 1400.0, pathEnemy, true, 0.0);
	lEnemy4->setLevelWH(this->width, this->height);
	this->enemies.push_back(lEnemy4);

	Enemy* lEnemy5 = new Enemy(8256.0, 1400.0, pathEnemy, true, 0.0);
	lEnemy5->setLevelWH(this->width, this->height);
	this->enemies.push_back(lEnemy5);

	Enemy* lEnemy6 = new Enemy(10560.0, 1400.0, pathEnemy, false, 0.0);
	lEnemy6->setLevelWH(this->width, this->height);
	this->enemies.push_back(lEnemy6);

	Enemy* lEnemy7 = new Enemy(10880.0, 1400.0, pathEnemy, false, 0.0);
	lEnemy7->setLevelWH(this->width, this->height);
	this->enemies.push_back(lEnemy7);

		
	// Test text.
	// Text* text = new Text(200.0, 900.0, "res/fonts/KGFeeling22.ttf", 50, "dauphine");
	// addEntity(text);

	// Finally, setting the player and the camera.
	setPlayer(lPlayer);
	Enemy::pLife = this->player->life;

	setCamera(lCamera);

	Game::instance().getFade().fadeOut(0, 0.002);
}

void LevelTwo::unload(){
	Log(DEBUG) << "\tUnloading level 1...";
	cleanEntities();
	clearEnemies();
	this->checkpointVisited = false;	
}

void LevelTwo::update(const double dt_){
	// Populating the QuadTree.
	this->quadTree->setObjects(this->tileMap->getCollisionRects());

	// Updating the entities, using the QuadTree.
	std::vector<CollisionRect> returnObjects;
	for (auto entity : this->entities) {
		returnObjects.clear();
		this->quadTree->retrieve(returnObjects, entity->getBoundingBox());
		entity->setCollisionRects(returnObjects);
		entity->update(dt_);
	}

	// Updating the enemies.
	for(auto enemy : this->enemies){
		returnObjects.clear();
		this->quadTree->retrieve(returnObjects, enemy->getBoundingBox());
		enemy->setCollisionRects(returnObjects);
		enemy->update(dt_);
	}

	// Set to GameOver if the player is dead.
	if(this->player->isDead()){
		Game::instance().setState(Game::GStates::GAMEOVER);
		return;
	}

	// Updating the potions.
	for(auto potion : this->player->potions){
		returnObjects.clear();
		this->quadTree->retrieve(returnObjects, potion->getBoundingBox());
		potion->setCollisionRects(returnObjects);
	}

	/// @todo Maybe refactor this static Enemy::px, Enemy::py.
	// Updating player info for the enemies.
	Enemy::px = this->player->x;
	Enemy::py = this->player->y;

	for (int i = 0; i < NUMBER_ITEMS; ++i){
		if(abs(this->player->x - items[i])<= 20 && caughtItems[i] == false){
			this->player->addPotions(3);
			caughtItems[i]=true;
		}
	}

	if(this->player->life != Enemy::pLife){
		if(this->player->isVulnerable){
			this->player->life--;
			Enemy::pLife = this->player->life;
			this->player->changeState(Player::PStates::HITED);
			this->player->isVulnerable = false;
		}
		else{

		}
	}

	// Updating the HUD.
	this->playerHud->update();

	// Updating the camera.
	this->camera->update();

	// Set next level if end is reached.
	if(this->player->reachedLevelEnd){
		Game::instance().setState(Game::GStates::LEVEL_BOSS);
		return;
	}

	// Updating the potion/enemy collision.
	for(auto potion : this->player->potions){
		for(auto enemy : this->enemies){
			if(Collision::rectsCollided(potion->getBoundingBox(), enemy->getBoundingBox())){
				if(potion->activated){
					
					if(enemy->life > 0 && this->player->canAttack){
						enemy->life -= 100;
						potion->activated = false;
					}
					// Log(DEBUG) << "Enemy Life = " << enemy->life;

					if(enemy->life <= 0)
						enemy->changeState(Enemy::EStates::DEAD);
				}
			}
		}
	}

	// Updating the player attack/enemy collision.
	for(auto enemy : this->enemies){
		if(Collision::rectsCollided(this->player->getBoundingBox(), enemy->getBoundingBox())){
			if(this->player->isRight != enemy->isRight)
				if(this->player->isCurrentState(Player::PStates::ATTACK) || this->player->isCurrentState(Player::PStates::ATTACKMOVING)){
					
					if(enemy->life > 0 && this->player->canAttack){
						enemy->life -= this->player->attackStrength;
						this->player->canAttack = false;
					}
					// Log(DEBUG) << "Enemy Life = " << enemy->life;

					if(enemy->life <= 0)
						enemy->changeState(Enemy::EStates::DEAD);
				}
		}
	}

	//Saving the game state
	if(!this->checkpointVisited && this->player->getBoundingBox().x >= 500 && this->player->getBoundingBox().x <= 550){
		this->checkpoint = Game::instance().getResources().get("res/images/checkpoint_visited.png");
		Game::instance().getSaves().saveLevel(2, this->player, this->enemies);
		this->checkpointVisited = true;
	}
}

void LevelTwo::render(){
	const int cameraX = this->camera->getClip().x;
	const int cameraY = this->camera->getClip().y;

	this->background->render(0, 0);

	this->checkpoint->render(500 - cameraX, 5600 - cameraY);

	// Render the tiles in the TileMap.
	this->tileMap->render(cameraX, cameraY);

	this->playerHud->render();

	for(auto enemy : this->enemies){
		enemy->render(cameraX, cameraY);
	}

	// Render all the entities in the list.
	for(auto entity : this->entities){
        entity->render(cameraX, cameraY);
	}
}

