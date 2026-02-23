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
SDL_Renderer* renderer=nullptr;
int convert(const string& t,int f){
    TTF_Font*font=TTF_OpenFont("tahoma.ttf",f);
    int x,y;
    TTF_SizeText(font,t.c_str(),&x,&y);
    TTF_CloseFont(font);
    return x;
}
void bcl(int x1,int y1,int x2,int y2,UINT32 color){
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
void drawmm(bool cb){
    SDL_SetRenderDrawColor(renderer,148,187,235,255);
    SDL_RenderClear(renderer);
    bcl(0,0,wt,40,0xffff780a);
    textRGBA(renderer,0,7,"SCRATCH","tahomabd.ttf",22,0,0,0,255);
    textRGBA(renderer,130,10,"File    ?","tahomabd.ttf",18,0,0,0,255);
    bcl(wt-535,45,wt-505,75,0xff00ff00);
    bcl(wt-570,45,wt-540,75,0xff0000ff);
}
SDL_Texture* g_tabBarTex     = nullptr;
int          g_tabBarLastTab = -1;
bool         g_tabBarLastCb  = true;


void msxDestroy(){
    if(g_tabBarTex){ SDL_DestroyTexture(g_tabBarTex); g_tabBarTex=nullptr; }
    g_tabBarLastTab=-1;
}

int msx(int x, bool cb){

    int tabIdx = 0;
    if(x > 143) tabIdx = 2;
    else if(x > 53) tabIdx = 1;


    if(g_tabBarTex == nullptr || tabIdx != g_tabBarLastTab || cb != g_tabBarLastCb){
        g_tabBarLastTab = tabIdx;
        g_tabBarLastCb  = cb;

        if(g_tabBarTex) SDL_DestroyTexture(g_tabBarTex);

        g_tabBarTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                        SDL_TEXTUREACCESS_TARGET, 216, 30);
        SDL_SetRenderTarget(renderer, g_tabBarTex);


        SDL_SetRenderDrawColor(renderer, 0xeb, 0xbb, 0x94, 0xff);
        SDL_RenderClear(renderer);


        int x1=0, x2=53;
        if(tabIdx==2){ x1=143; x2=216; }
        else if(tabIdx==1){ x1=53;  x2=143; }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect sel={x1, 0, x2-x1, 30};
        SDL_RenderFillRect(renderer, &sel);

        rectangleColor(renderer, 0, 0, 215, 29, 0xff000000);
        vlineColor(renderer, 53,  0, 29, 0xff000000);
        vlineColor(renderer, 143, 0, 29, 0xff000000);

        const char* hj = cb ? "Costumes" : "Backdrops";
        textRGBA(renderer,  5, 5, "Code",   "tahoma.ttf", 18, 0, 0, 0, 255);
        textRGBA(renderer, 60, 5, hj,       "tahoma.ttf", 18, 0, 0, 0, 255);
        textRGBA(renderer,150, 5, "Sounds", "tahoma.ttf", 18, 0, 0, 0, 255);

        SDL_SetRenderTarget(renderer, nullptr);
    }

    SDL_Rect dest = {0, 50, 216, 30};
    SDL_RenderCopy(renderer, g_tabBarTex, nullptr, &dest);

    return tabIdx;
}
#endif