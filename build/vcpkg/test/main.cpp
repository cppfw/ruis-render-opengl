// this file is to test basic usage of the library,
// mainly test that linking to the library works, so just call any
// non-inline function of the library to test it.

#include <ruis/render/opengl/context.hpp>

int main(int argc, const char** argv){
    std::function<void()> f = [](){
        ruis::render::opengl::context c;
    };

    // do not call f() to avoid segmentation fault, becuase OpenGL is not initialized

    if(!f){
        return 1;
    }

    std::cout << "Hello ruis-render-opengl!" << std::endl;
    return 0;
}
