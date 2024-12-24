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

)
