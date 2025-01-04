#include <emscripten.h>
#include <emscripten/html5.h>
#include <vector>
#include <algorithm>
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

EM_JS(float, square_size, (),{
  let context = Module.canvas.getContext('2d');
  let width = Module.canvas.width ;//= window.innerWidth;
  let sqsize = width/8;

  return sqsize;
});

EM_JS(void, draw_piece, (int pp, int x, int y), {
  let context = Module.canvas.getContext('2d');
  let width = Module.canvas.width ;//= window.innerWidth;
  let sqsize = width/8;
      
  
  context.drawImage(piece_imgs[pp-1],x*sqsize,y*sqsize,sqsize,sqsize); 
  
});

EM_JS(void, draw_highlight, (int ii, int jj), {
  let context = Module.canvas.getContext('2d');
  let width = Module.canvas.width ;//= window.innerWidth;
  let sqsize = width/8;

  context.strokeStyle = "yellow";
  context.beginPath();
  context.rect(ii*sqsize,jj*sqsize,sqsize,sqsize);
  context.stroke();

});

void draw_board(Board bb){
  draw_blankboard();
  for(int x=0; x<8; x++){
    for(int y=0; y<8; y++){
      auto sq = Square(File(x), Rank(7-y));

      switch(bb.at<Piece>(sq).internal()){
        case Piece::WHITEPAWN :
          draw_piece(1, x, y);
          break;
        case Piece::WHITEKNIGHT :
          draw_piece(2, x, y);
          break;
        case Piece::WHITEBISHOP :
          draw_piece(3, x, y);
          break;
        case Piece::WHITEROOK :
          draw_piece(4, x, y);
          break;
        case Piece::WHITEQUEEN :
          draw_piece(5, x, y);
          break;
        case Piece::WHITEKING :
          draw_piece(6, x, y);
          break;
        case Piece::BLACKPAWN :
          draw_piece(7, x, y);
          break;
        case Piece::BLACKKNIGHT :
          draw_piece(8, x, y);
          break;
        case Piece::BLACKBISHOP :
          draw_piece(9, x, y);
          break;
        case Piece::BLACKROOK :
          draw_piece(10, x, y);
          break;
        case Piece::BLACKQUEEN :
          draw_piece(11, x, y);
          break;
        case Piece::BLACKKING :
          draw_piece(12, x, y);
          break;
        case Piece::NONE :
          break;

      }
        
    }
  }

}

//globals

struct global_data{
  std::vector<Move> history;
  Board bb;
  bool moving;
  Square movefrom;
  Movelist lms;

  float sqsize;

  global_data(){
    bb = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    history = std::vector<Move>(0);
    moving = false;
    movegen::legalmoves<movegen::MoveGenType::ALL>(lms, bb);
  };

  void reset(){
    bb = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    history = std::vector<Move>(0);
    moving=false;
    movegen::legalmoves<movegen::MoveGenType::ALL>(lms, bb);
  };

int iselect(float x){
    int ans=8;
    for(int ii=0; ii<8; ii++){
      if(x < (ii+1)*sqsize){
        ans = ii;
        break;
      }
    }
   return ans;
  }

};

global_data gd;

//callbacks

EM_BOOL touchend_callback(
    int eventType,
    const EmscriptenTouchEvent *event,
    void *userData
) {
    global_data* ud = static_cast<global_data*>(userData);
    int ii = ud->iselect(event->touches[0].clientX );
    int jj = ud->iselect(event->touches[0].clientY );

    auto sq = Square(File(ii), Rank(7-jj));
    auto pp = ud->bb.at<Piece>(sq);
    if(!(ud->moving)){
      if(ii<8 && jj<8  && pp.internal()!= Piece::NONE && pp.color()==ud->bb.sideToMove()){
        draw_highlight(ii,jj);
        ud->movefrom = sq;
        ud->moving = true;
        
      }
    }
    else{
      if(ii<8 && jj<8){
        Move mtry = Move::make<Move::NORMAL>(ud->movefrom, sq);
        if(std::find(ud->lms.begin(), ud->lms.end(), mtry) != ud->lms.end()){
          ud->moving = false;
          ud->bb.makeMove(mtry);
          draw_board(ud->bb);
          //generate legal moves
          movegen::legalmoves<movegen::MoveGenType::ALL>(ud->lms, ud->bb);
        }
        else{
          ud->moving = false;
          draw_board(ud->bb);
        }
      }
    }
  
    return EM_TRUE;
}

//exported functions

extern "C"{

void onBack(){ }

void onForw(){ }

void onNew(){ 
  //draw_blankboard();
  gd.reset();
  draw_board(gd.bb);
}

void onLoad(){ }

void onStart(){}

}

int main (){

  //float ss = redraw(nx,ny);
  
  emscripten_set_touchend_callback(
        "canvas",
        static_cast<void*>(&gd),
        1,
        touchend_callback
  );

  //emscripten_set_main_loop_arg(mainloop, static_cast<void*>(&as), 1, false);
  //preload_pieces();
  draw_blankboard();
  gd.sqsize = square_size();
  //Board bb("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  //draw_board(bb);
  return 0;
}
