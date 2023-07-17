#include "Mouse.h"
#include "clonebin.h"
#include "selection.h"
#include "sounds.h"
#include "ray.h"

RBX::Sound* ping = RBX::Sound::fromFile(GetFileInPath("/content/sounds/electronicpingshort.wav"));
RBX::PVInstance* lastClone, *lastSelection;

void RBX::CloneBin::update(RenderDevice* rd, UserInput* ui)
{
	if (RBX::Selection::selection && RBX::Selection::clicked)
	{
		RBX::PVInstance* clone = static_cast<RBX::PVInstance*>(RBX::Selection::selection->clone());
		Vector3 p;

		p = clone->getPosition();

		if (RBX::Selection::selection != lastSelection)
			lastClone = 0;

		if (lastClone)
			p = lastClone->getPosition();

		clone->setParent(RBX::Selection::selection->getParent());
		clone->setPosition(p + Vector3(0, clone->getSize().y/1.2, 0));

		lastClone = clone;
		lastSelection = RBX::Selection::selection;

		RBX::Selection::clicked = 0;
		RBX::Selection::selection = clone;

		ping->play();

		Rendering::cursor_custom = Texture::fromFile(GetFileInPath("/content/textures/CloneDownCursor.png"));
	}
	else
	{
		RBX::PVInstance* i;
		i = RBX::Mouse::getTarget();
		if (active && !i)
		{
			Rendering::cursor_custom = Texture::fromFile(GetFileInPath("/content/textures/CloneCursor.png"));
		}
		else
		{
			Rendering::cursor_custom = Texture::fromFile(GetFileInPath("/content/textures/CloneOverCursor.png"));
		}
	}

}

void RBX::CloneBin::activate()
{
	RBX::Selection::canSelect = 1;
}

void RBX::CloneBin::deactivate()
{
	Rendering::cursor_custom = 0;
	lastSelection = 0;
	lastClone = 0;
	RBX::Selection::canSelect = 0;
	RBX::Selection::selection = 0;
}
