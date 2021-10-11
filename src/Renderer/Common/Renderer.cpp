#include <dcore/Renderer/Renderer.hpp>
using namespace dcore::graphics;

static Renderer *rendererInst = nullptr;

Renderer *Renderer::Instance()
{
	if(!rendererInst)
		rendererInst = new Renderer;
	return rendererInst;
}

void Renderer::SetInstance(Renderer *newInstance)
{ rendererInst = newInstance; }
