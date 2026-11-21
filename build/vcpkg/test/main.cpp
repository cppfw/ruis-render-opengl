// this file is to test basic usage of the library,
// mainly test that linking to the library works, so just reference any
// non-inline function of the library to test it.

#include <ruis/render/opengl/context.hpp>
#include <ruis/render/opengl/factory.hpp>

#include <iostream>

int main(int argc, const char** argv){
    // reference a non-inline function of the library to test that linking works.
    // do not call it (would require a real GL context).
    std::function<void()> f = [](){
        auto nw = utki::make_shared<ruis::render::native_window>();
        auto fac = utki::make_shared<ruis::render::opengl::factory>(nw);
        ruis::render::opengl::context c(nw, fac);
        (void)c;
    };

    if(!f){
        return 1;
    }

    std::cout << "Hello ruis-render-opengl!" << std::endl;
    return 0;
}
