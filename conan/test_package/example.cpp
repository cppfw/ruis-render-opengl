#include <ruis/render/opengl/renderer.hpp>

int main(int argc, const char** argv){
	auto r = utki::make_shared<ruis::render::opengl::renderer>();

	std::cout << "hello ruis-render-null!" << std::endl;

	return 0;
}