#include "ssph.h"
#include "RendererComand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"


namespace Sas {

	RendererAPI* RendererComand::s_RendererAPI = new OpenGLRendererAPI;

}