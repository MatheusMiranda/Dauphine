#include "PStateAerial.h"
#include "Logger.h"

void PStateAerial::enter(){
	this->player->getAnimation()->changeAnimation(4,3,14,false,1.4);
	this->player->isGrounded = false;
}

void PStateAerial::exit(){

}

void PStateAerial::handleInput(const std::array<bool, GameKeys::MAX> keyStates_){

	// Idle
    if(this->player->isGrounded){
    	this->player->changeState(Player::PStates::IDLE);
    	return;
    }

	// Gravity
	this->player->applyGravity();

	// Move (while on air)
    this->player->move(keyStates_[GameKeys::LEFT], keyStates_[GameKeys::RIGHT]);
}

PStateAerial::PStateAerial(Player* const player_) :
	StatePlayer(player_)
{

}