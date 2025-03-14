#include <ruis/render/opengl/context.hpp>

int main(int argc, const char** argv){
	auto c = utki::make_shared<ruis::render::opengl::context>();

	std::cout << "hello ruis-render-opengl!" << std::endl;

	return 0;
}