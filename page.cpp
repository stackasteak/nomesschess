#include <emscripten.h>
#include <emscripten/html5.h>
#include <vector>
#include <stdio.h>


// javascript functions

EM_JS(void, alert_float, (float x), {
  alert(x);
});

extern "C"{

void onBack(){ }

void onForw(){ }

void onNew(){ }

void onLoad(){ }

void onStart(){}

}

int main (){

  //float ss = redraw(nx,ny);
  
  //emscripten_set_touchend_callback(
  //      "canvas",
  //      static_cast<void*>(&as),
  //      1,
  //      touchend_callback
  //  );

  //emscripten_set_main_loop_arg(mainloop, static_cast<void*>(&as), 1, false);
  
  return 0;
}
