#include <iostream>
#include "Application.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
int main() {

	std::shared_ptr<mainNameSpace::Application> app = std::make_shared<mainNameSpace::Application>();
	try
	{
		//Start
		std::cout << "start" << std::endl;
		app->run();
	}
	catch (const std::exception& e)
	{	
		std::cout << e.what() << std::endl;
	}
	return 0;
}