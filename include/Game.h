#ifndef INCLUDE_GAME_H
#define INCLUDE_GAME_H

#include "Window.h"

/**
* Main structure class for the game.
* Contains all the necessary functionalities to loop and update the game.
*/
class Game {

	public:
		/**
		* The constructor.
		* Sets the game window and tells the game that it is OK to begin looping. Also, it begins the FPS manager.
		* @param lWindow : a created Window.
		* @note If the Window parameter is null, the game will not begin.
		*/
		Game(Window *lWindow);

		/**
		* The destructor.
		* Destroys the game's Window, and make sure the main loop stops.
		*/
		~Game();
		
		/**
		* Handles the updates.
		* Updates all game objects. Is part of the main loop.
		* @param dt : Delta time. Time elapsed between one frame and the other, independent of processing speed.
		* @see runGame()
		*/
		void update(double dt);
		
		/**
		* The main game loop.
		* Orders the game structure, such as inputs, updates, and rendering.
		*/
		void runGame();
		
		bool isRunning; /**< Whether the game is currently running/looping or not. */

	private:
		FPSmanager fpsManager; /**< The FPSManager from SDL2_GFX. Handles the framerate capping. */
		Window *gameWindow; /**< The game Window. */

};

#endif //INCLUDE_GAME_H
