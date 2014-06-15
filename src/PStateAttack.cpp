#include "PStateAttack.h"
#include "Logger.h"

void PStateAttack::enter(){
	this->box.x = 58;
	this->box.y = 75;
	this->box.w = 140;
	this->box.h = 160;

	this->player->getAnimation()->changeAnimation(1, 7, 14, false, 0.466);
}

void PStateAttack::exit(){

}

void PStateAttack::handleInput(const std::array<bool, GameKeys::MAX> keyStates_){
	
	if(this->player->getAnimation()->getCurrentFrame() == 14){
		this->player->changeState(Player::PStates::IDLE);
	}
}

PStateAttack::PStateAttack(Player* const player_) :
	StatePlayer(player_)
{

}
