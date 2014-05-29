#include "PStateMovingCrouch.h"
#include "Logger.h"

void PStateMovingCrouch::enter(){
	this->player->getAnimation()->changeAnimation(3,10,6,true,1);
	this->player->isGrounded = true;
	this->player->maxSpeed /= 2;
	this->player->speed = 15;
}

void PStateMovingCrouch::exit(){
	this->player->maxSpeed *= 2;
	this->player->speed = 20;
}

void PStateMovingCrouch::handleInput(const std::array<bool, GameKeys::MAX> keyStates_){

	if(!keyStates_[GameKeys::CROUCH]){
		this->player->changeState(Player::PStates::IDLE);
		return;
	}

	this->player->slowVx();

	// Jump
	if(keyStates_[GameKeys::SPACE]){
		this->player->jump();
		this->player->isGrounded = false;
		return;
	}

	this->player->move(keyStates_[GameKeys::LEFT], keyStates_[GameKeys::RIGHT]);

	if(keyStates_[GameKeys::ROLL]){
		this->player->changeState(Player::PStates::ROLLING);
		return;
	}

}

PStateMovingCrouch::PStateMovingCrouch(Player* const player_) :
	StatePlayer(player_)
{
	
}
