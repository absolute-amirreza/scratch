

#ifndef SCRATCH_EVENT_H
#define SCRATCH_EVENT_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfx.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
int wt,ht;
int convert(const string& t,int f){
    TTF_Font*font=TTF_OpenFont("tahoma.ttf",f);
    int x,y;
    TTF_SizeText(font,t.c_str(),&x,&y);
    TTF_CloseFont(font);
    return x;
}
void bcl(SDL_Renderer *renderer,int x1,int y1,int x2,int y2,UINT32 color){
    SDL_Rect r;
    if(x1>x2){r.x=x2;r.w=x1-x2;}else{r.x=x1;r.w=x2-x1;}
    if(y1>y2){r.y=y2;r.h=y1-y2;}else{r.y=y1;r.h=y2-y1;}
    Uint8 re=color%256;color/=256;
    Uint8 g=color%256;color/=256;
    Uint8 b=color%256;color/=256;
    Uint8 a=color;
    SDL_SetRenderDrawColor(renderer,re,g,b,a);
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_NONE);
    SDL_RenderFillRect(renderer,&r);
}
void drawmm(SDL_Renderer*renderer,bool cb){
    SDL_SetRenderDrawColor(renderer,148,187,235,255);
    SDL_RenderClear(renderer);
    bcl(renderer,0,0,wt,40,0xffff780a);
    textRGBA(renderer,0,7,"SCRATCH","tahomabd.ttf",22,0,0,0,255);
    textRGBA(renderer,130,10,"File    ?","tahomabd.ttf",18,0,0,0,255);
    bcl(renderer,wt-495,45,wt-465,75,0xff00ff00);
    bcl(renderer,wt-460,45,wt-430,75,0xff0000ff);
}
int msx(SDL_Renderer*renderer,int x,bool cb){
    int x1=0,x2=53;
    if(x>143)x1=143,x2=216;
    else if(x>53)x1=53,x2=143;
    bcl(renderer,0,50,216,80,0xffebbb94);
    bcl(renderer,x1,50,x2,80,0xffffffff);
    rectangleColor(renderer,0,50,216,80,0xff000000);
    vlineColor(renderer,53,50,80,0xff000000);
    vlineColor(renderer,143,50,80,0xff000000);
    const char*hj="Costumes";
    if(!cb)hj="Backdrops";
    textRGBA(renderer,5,55,"Code","tahoma.ttf",18,0,0,0,255);
    textRGBA(renderer,60,55,hj,"tahoma.ttf",18,0,0,0,255);
    textRGBA(renderer,150,55,"Sounds","tahoma.ttf",18,0,0,0,255);
    if(x<=53)return 0;
    if(x<=143)return 1;
    return 2;
}
void drawmm(bool cb)
{
    SDL_SetRenderDrawColor(renderer, 148, 187, 235, 255);
    SDL_RenderClear(renderer);
    bcl(0, 0, wt, 40, 0xffff780a);
    textRGBA(renderer, 0,   7, "SCRATCH", "tahomabd.ttf", 22, 0, 0, 0, 255);
    textRGBA(renderer, 130,10, "File",    "tahomabd.ttf", 18, 0, 0, 0, 255);

    // ? button
    bcl(155, 6, 178, 32, 0xff1a6adb);
    textRGBA(renderer, 160, 9, "?", "tahomabd.ttf", 16, 255, 255, 255, 255);

    bcl(wt - 535, 45, wt - 505, 75, 0xff00ff00);
    bcl(wt - 570, 45, wt - 540, 75, 0xff0000ff);
}
#endif
