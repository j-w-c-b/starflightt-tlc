/*
STARFLIGHT - THE LOST COLONY
Module.h
Author: Dave Calkins
Date: Dec, 06

* used to provide a hierarchical set of modules which can each perform their
* own processing/rendering to local surfaces which then get rendered back to
* their parent surfaces. this makes the various game visual and computational
* modules independent and allows easily working on one or creating a new
* one without having to understand all the others.
*
* to create a new module, derive from this class and then override the
* virtual methods, making sure to call the base class methods from your
* derived class.
*
* for a visual module, you'll want to call InitCanvas from InitModule to
* ensure a rendering canvas is setup. for a non-visual computational only
* module, don't setup the canvas.
*
* NOTE: non-visual computational only modules can not have any modules
* beneath (children or descendants) them which are visual.
*/

#ifndef MODULE_H
#define MODULE_H

#include "Game.h"
#include <vector>
#include "Events.h"

struct ALLEGRO_BITMAP;
class GameState;
class MessageBoxWindow;

class Module
{
public:

	Module();
	virtual ~Module();

	// used for establishing the hierarchy of modules during game initialization
	// once added, this module takes over ownership of the child module and will
	// delete it when this module is deleted.
	void AddChildModule(Module * m);

	/**
	* call the base class method, perform any module-specific initialization
	* here, and make sure you call InitCanvas() to setup the rendering canvas,
	* unless your derived module class is a non-graphical module. return
	* true on successful initialization or false on failure.
	*
	* NOTE: InitModule is only called once at program startup.
	*/
	virtual bool Init();


	virtual void Update();

	/**
	* perform any module-specific processing and rendering to the frame buffer
	*/
	virtual void Draw();

	/**
	* invoked on active modules when a key is pressed. key code is taken from
	* allegro.h (for example, ALLEGRO_KEY_SPACE). make sure to call the base class impl.
	*/
	virtual void OnKeyPressed(int keyCode);

	/**
	* JH 01-Feb-07
	* Added to handle key press events without the engine trapping press/release
	* events. This will provide support for multiple and repeat key presses.
	*/
	virtual void OnKeyPress(int keyCode);

	/**
	* invoked on active modules when a key is released. key code is taken from
	* allegro.h (for example, ALLEGRO_KEY_SPACE). make sure to call the base class impl.
	*/
	virtual void OnKeyReleased(int keyCode);

	/**
	* called when the mouse moves. call the base class impl, then handle it.
	* coords are relative to the module's upper left corner; this will not be
	* called for non-visual modules.
	*/
	virtual void OnMouseMove(int x, int y);

	/**
	* similar to OnMouseMove, but called when the mouse is clicked; i.e.
	* a button is pressed and then released with the mouse at the same
	* position. the button (0..N-1) which was clicked is provided.
	*/
	virtual void OnMouseClick(int button, int x, int y);

	/**
	* Called when a mouse button is pressed down.
	*/
	virtual void OnMousePressed(int button, int x, int y);

	/**
	* Called when a mouse button is released.
	*/
	virtual void OnMouseReleased(int button, int x, int y);

	/**
	* Called when the mouse wheel is moved up (away from the hand).
	* The position of the mouse when the wheel was moved is passed
	* to this function
	*/
	virtual void OnMouseWheelUp(int x, int y);

	/**
	* Called when the mouse wheel is moved down (towards the hand).
	* The position of the mouse when the wheel was moved is passed
	* to this function
	*/
	virtual void OnMouseWheelDown(int x, int y);

	/**7
	* called when an event is broadcast by a module; call the base class impl
	* then use RTTI (dynamic_cast) to see which specific kind of event is being sent
	*/
	virtual void OnEvent(Event * event);

	/**
	* call the base class method and perform any module-sepcific cleanup
	*/
	virtual void Close();

protected:

	// nybble moved these here so derived classes can know where they are in relation to
	// their parent position relative to parent
	int						m_x;
	int						m_y;

private:


	// child modules (used when this module is a game mode root, not used when it's a normal module)
	std::vector<Module*>	m_modules;

	// overall module count for the game (used for loading screen progress bar)
	static int				m_totalNumModules;
	static int				m_numModulesInitialized;
};

#endif
