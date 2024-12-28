#include <emscripten.h>
#include <emscripten/html5.h>
#include <vector>
#include <stdio.h>
#include "nmc/nmc.hpp"
#include "nmc/chess.hpp"

using namespace chess;


// javascript functions

EM_JS(void, alert_float, (float x), {
  alert(x);
});

EM_JS(void, draw_blankboard, (),{
  let context = Module.canvas.getContext('2d');

  let width = Module.canvas.width = window.innerWidth;
  let height = Module.canvas.height = window.innerWidth+30;
  context.fillStyle = 'rgb(0,0,0)';
  context.fillRect(0,0,width,height);

  let sqsize = width/8;

  for(let ii=0; ii<8; ii++){
    for(let jj=0; jj<8; jj++){
      if ((ii+jj)%2==0 ) {
        context.fillStyle = 'rgb(214,155,39)';
      }
      else{
        context.fillStyle = 'rgb(135,109,19)';   
      }
      context.fillRect(ii*sqsize,jj*sqsize,sqsize,sqsize);
    }
  }

});

EM_JS(void, draw_piece, (string piecepath, int x, int y), {
  let context = Module.canvas.getContext('2d');
  let width = Module.canvas.width = window.innerWidth;

  drawing = new Image();
  drawing.src = piecepath;
  drawing.onload = function() {
    context.drawImage(drawing,x*width,y*width,width,width); 
  };
});

void draw_board(Board bb){
  draw_blankboard();
  for(int x=0; x<8; x++){
    for(int y=0; y<8; y++){
      auto sq = Square(File(x), Rank(7-y));

      switch(bb.at(sq)){
        case Piece::WHITEPAWN :
          draw_piece("assets/Chess_wp.svg", x, y);

      }
        
    }
  }

}

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

  draw_blankboard();
  return 0;
}
