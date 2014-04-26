#include "Game.h"
#include "InputHandler.h"
#include "FPSWrapper.h"
#include "Logger.h"
#include "StateSplash.h"
#include "LevelOne.h"

State* Game::currentState = nullptr;
State* Game::stateSplash = nullptr;
State* Game::levelOne = nullptr;

Game::Game(Window* window_) :
	window(window_),
	isRunning(false)
{
	Game::initializeStates();

	if(this->window != nullptr){
		this->isRunning = true;
		FPSWrapper::initialize(this->fpsManager);

		Game::currentState = Game::stateSplash;
		Game::currentState->load();
	}
	else{
		Logger::error("Game window is null. Game will not run.");
	}
}

Game::~Game(){
	if(this->currentState != nullptr){
		this->currentState->unload();
	}

	Game::destroyStates();

	if(this->window != nullptr){
		this->window->destroy();
		this->window = nullptr;
	}
}

void Game::runGame(){
	
	// Creating the input handler.
	InputHandler* inputHandler = InputHandler::getInstance();
	
	// Get the first game time.
	double totalGameTime = 0.0;
	const double deltaTime = 1.0 / 60.0;
	double accumulatedTime = 0.0;

	// This is the main game loop.
	while(this->isRunning){

		const double frameTime = FPSWrapper::delay(this->fpsManager);
		accumulatedTime += frameTime;

		// Update.
		while(accumulatedTime >= deltaTime){
			inputHandler->handleInput();

			// Check for a quit signal from input.
			if(inputHandler->signalQuit()){
				this->isRunning = false;
				return;
			}

			Game::currentState->update(deltaTime);

			accumulatedTime -= deltaTime;
			totalGameTime += deltaTime;
		}

		// Render.
		window->clear();

		Game::currentState->render();

		window->render();
		
	}

}

void Game::setState(State& state_){
	/// @todo Implement the transition between states.
	Game::currentState->unload();
	Game::currentState = &state_;
	Game::currentState->load();
}

void Game::initializeStates(){
	// Initialize all the states in Game here.
	Game::stateSplash = new StateSplash();
	Game::levelOne = new LevelOne();
}

void Game::destroyStates(){
	// Delete all the states in Game here.
	delete Game::stateSplash;
	delete Game::levelOne;
}