#pragma once

#ifndef SCENE_H
#define SCENE_H

#include <glfw3.h>
#include "QuatCamera.h"

namespace imat2908
{

class Scene
{
public:
	Scene() : m_animate(true) { }
	
    /**
		Load textures, initialize shaders, etc.
     */
    virtual void initScene(QuatCamera camera) = 0;

    /**
		Draw your scene.
     */
    virtual void render(QuatCamera camera) = 0;

    /**
		Called when screen is resized.
     */
    virtual void resize(QuatCamera camera,int, int) = 0;
    
	/**
		Used to update the lighting parameters based on the user's keyboard input.
	 */
	virtual void animate(bool &shift, bool &a, bool &d, bool &s, bool &r) = 0;
    
protected:
	bool m_animate;
};

}

#endif // SCENE_H
