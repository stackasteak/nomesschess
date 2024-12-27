#include <emscripten.h>
#include <emscripten/html5.h>
#include <vector>
#include <stdio.h>
#include "nmc/nmc.hpp"


// javascript functions

EM_JS(void, alert_float, (float x), {
  alert(x);
});

EM_JS(void, drawboard, (),{
  let context = Module.canvas.getContext('2d');

  let width = Module.canvas.width = window.innerWidth;
  let height = Module.canvas.height = window.innerWidth+30;
  context.fillStyle = 'rgb(0,0,0)';
  context.fillRect(0,0,width,height);

  let sqsize = width/8;

  for(let ii=0; ii<8; ii++){
    for(let jj=0; jj<8; jj++){
      if ((ii*jj)%2==0 ) {
        context.fillStyle = 'rgb(10,20,20)';
      else{
        context.fillStyle = 'rgb(30,30,10)';   
      }
      context.fillRect(ii*sqsize,jj*sqsize,(ii+1)*sqsize,(jj+1)*sqsize);
    }
  }

  
  drawing = new Image();
  drawing.src = "assets/Chess_bdt45.svg"; // can also be a remote URL e.g. http://
  drawing.onload = function() {
    context.drawImage(drawing,0,0); 
  };

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

  drawboard();
  return 0;
}
