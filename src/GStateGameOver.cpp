#include "GStateGameOver.h"
#include "LuaScript.h"
#include "Game.h"

#include <string>

GStateGameOver::GStateGameOver() :
	gameOverImage(nullptr),
	passedTime(0.0),
	lifeTime(0.0)
{

}

GStateGameOver::~GStateGameOver(){

}

void GStateGameOver::update(const double dt_){
	this->passedTime += dt_;
}

void GStateGameOver::load(){
	Log(DEBUG) << "Loading Game Over...";

	LuaScript luaGameOver("lua/GameOver.lua");
	const std::string pathGameOver = luaGameOver.unlua_get<std::string>("gameOver.images.gameOver");
	const double luaLifeTime = luaGameOver.unlua_get<double>("gameOver.lifeTime");

    this->gameOverImage = Game::instance().getResources().get(pathGameOver);
	this->lifeTime = luaLifeTime;
}

void GStateGameOver::unload(){
	Log(DEBUG) << "\tUnloading Game Over...";
	cleanEntities();
}

void GStateGameOver::render(){
	if(this->gameOverImage != nullptr){
		this->gameOverImage->render(0, 0, nullptr, true);
	}
	else{
		Log(WARN) << "No image set for the game over screen!";
	}
}
