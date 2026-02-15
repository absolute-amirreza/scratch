
#ifndef SCRATCH_CODE_H
#define SCRATCH_CODE_H
#include "event.h"
#include <cmath>
#include <vector>
#include <map>
vector<SDL_Texture*>tex;
map<int ,SDL_Texture*>uni;int aw,ah,bw,bh;
UINT32 color(int n){
    if(n==0)return 0xffe6520e;
    if(n==1)return 0xff870b57;
    if(n==2)return 0xffb8379c;
    if(n==3)return 0xff1cc4d4;
    if(n==4)return 0xff3c78e8;
    if(n==5)return 0xfff7950c;
    if(n==6)return 0xff23a80c;
    if(n==7)return 0xff0642ba;
    else return 0xff1f1fff;
}
void unic(SDL_Renderer*renderer){
    TTF_Font* font = TTF_OpenFont(R"(C:\Windows\Fonts\seguisym.ttf)", 14);
    const char*a1c=u8"\u2B6F";
    SDL_Surface*a1=TTF_RenderUTF8_Blended(font,a1c, {255,255,255,255});
    SDL_Texture*a1t= SDL_CreateTextureFromSurface(renderer,a1);SDL_FreeSurface(a1);
    const char*a2c=u8"\u2B6E";
    SDL_Surface*a2=TTF_RenderUTF8_Blended(font,a2c, {255,255,255,255});
    SDL_Texture*a2t= SDL_CreateTextureFromSurface(renderer,a2);aw=a2->w;ah=a2->h;
    uni[-1]=a1t;uni[1]=a2t;SDL_FreeSurface(a2);
    const char*te=u8"\u25BC";
    for(int i=0;i<9;i++){
        UINT32 z=color(i);UINT8 rrr=z%256;z/=256;UINT8 gg=z%256;z/=256;UINT8 bb=z%256;z/=256;UINT8 aa=z;
        SDL_Color colo = {rrr, gg, bb, aa};
        SDL_Surface*surface=TTF_RenderUTF8_Solid(font,te,colo);
        SDL_Texture*pTexture=SDL_CreateTextureFromSurface(renderer, surface);
        bw=surface->w;bh=surface->h;SDL_FreeSurface(surface);uni[int(color(i))]=pTexture;
    }
    TTF_CloseFont(font);
}
struct block{
    string mod[2]{};//20,21
    vector<string>m[2];
    UINT32 c[2]{};//51,52
    bool var[3]{};int x1v[3]{},x2v[3]{},xco[2]{};

    SDL_Texture*blu=nullptr;
    UINT32 color{};
    int sit{};//0for block/1for condition/2for variable
    string pn[3]{};//71,72,73,1,-1
    string v[3]{};//91,92,93
    bool u{},d{},b{};
    int ss=0;bool hofre[2]{};
    vector<int> order;
    block*vari[3]{};block*candy[2]{};vector<block*>hof[2];
    SDL_Rect r{};int h[2]{};
    void rec(){
        r.h=26+d*6;r.w=3;
        for(const int i:order){
            if(i/10==7)r.w+=convert(pn[i%10],12)+3;
            else if(i/10==4){xco[i%10]=r.w;r.w+=29;}
            else if(i/10==3)r.w+=candy[i%10]->r.w+3;
            else if(i/10==8)r.w+=3+vari[i%10]->r.w;
            else if(i/10==9){
                int j=i%10;x1v[j]=r.w;
                int a=convert(v[j],12)>14?(convert(v[j],12)-14):0;
                r.w+=29+a;x2v[j]=r.w-3;
            }
            else if(i/10==2)r.w+=convert(mod[i%10],12)+20;
            else if(i/10==5)r.w+=29;
        }
        if(sit>0){
            if(r.w>14){r.w+=12;ss=6;}
            else{ss=13-((r.w-6)/2)-3;r.w=26;}
        }
        else{
            for(int i=0;i<2;i++){
                if(hofre[i]){
                    if(hof[i].empty()){h[i]=26;}
                    else{
                        h[i]=0;
                        for(int j=0;j<hof[i].size();j++){
                            h[i]+=hof[i][j]->r.h-6;if(r.w<hof[i][j]->r.w+10){r.w=hof[i][j]->r.w+10;}
                        }
                    }
                    r.h+=h[i]+26;
                }
            }
        }
    }
    void draw(SDL_Renderer*renderer){
        rec();SDL_Texture*tyu=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,r.w,r.h);
        tex.push_back(tyu);
        UINT32 z=color;UINT8 rrr=z%256;z/=256;UINT8 gg=z%256;z/=256;UINT8 bb=z%256;z/=256;UINT8 aa=z;
        SDL_Color colo = {rrr, gg, bb, aa};
        SDL_SetRenderTarget(renderer,tyu);SDL_SetRenderDrawColor(renderer,255,255,255,0);SDL_RenderClear(renderer);
        if(sit==1){
            filledTrigonColor(renderer,0,13,13,0,13,26,color);
            filledTrigonColor(renderer,r.w-13,0,r.w-13,26,r.w,13,color);
            bcl(renderer,13,0,r.w-13,26,color);
        }
        else if(sit==2){
            filledCircleColor(renderer,13,13,13,color);
            filledCircleColor(renderer,r.w-13,13,13,color);
            bcl(renderer,13,0,r.w-13,26,color);
        }
        else{
            bcl(renderer,0,0,r.w,26,color);
            if(d)bcl(renderer,10,r.h-6,20,r.h,color);
            if(u)bcl(renderer,10,0,20,6,0x00ffffff);
            if(hofre[0]){
                bcl(renderer,0,26,10,h[0]+26,color);
                bcl(renderer,0,h[0]+26,r.w,h[0]+52,color);
                bcl(renderer,20,26,30,32,color);
                bcl(renderer,20,h[0]+26,30,h[0]+32,0x00ffffff);
                for(int i=0;i<hof[0].size();i++){
                    hof[0][i]->r.x=r.x+10;
                    if(i==0)hof[0][0]->r.y=r.y+26;
                    else hof[0][i]->r.y=hof[0][i-1]->r.y+hof[0][i-1]->r.h-6;
                    SDL_Rect cop={hof[0][i]->r.x-r.x,hof[0][i]->r.y-r.y,hof[0][i]->r.w,hof[0][i]->r.h};
                    SDL_RenderCopy(renderer,hof[0][i]->blu, nullptr,&cop);
                }
                if(hofre[1]){
                    bcl(renderer,0,52+h[0],10,h[1]+52+h[0],color);
                    bcl(renderer,0,h[1]+52+h[0],r.w,h[1]+78+h[0],color);
                    bcl(renderer,20,52+h[0],30,58+h[0],color);
                    bcl(renderer,20,h[1]+52+h[0],30,h[1]+58+h[0],0x00ffffff);
                    textRGBA(renderer,3,h[0]+33,"else","tahoma.ttf",12,255,255,255,255);
                    for(int i=0;i<hof[1].size();i++){
                        hof[1][i]->r.x=r.x+10;
                        if(i==0)hof[1][i]->r.y=r.y+52+h[0];
                        else hof[1][i]->r.y=hof[1][i-1]->r.y+hof[1][i-1]->r.h-6;
                        SDL_Rect cop={hof[1][i]->r.x-r.x,hof[1][i]->r.y-r.y,hof[1][i]->r.w,hof[1][i]->r.h};
                        SDL_RenderCopy(renderer,hof[1][i]->blu, nullptr,&cop);
                    }
                }
            }
        }
        int xx=3+ss;
        for(int i : order){
            if(i==70){
                textRGBA(renderer,xx,7,pn[0].c_str(),"tahoma.ttf",12,255,255,255,255);
                xx+=convert(pn[0],12)+3;
            }
            else if(i==71){
                textRGBA(renderer,xx,7,pn[1].c_str(),"tahoma.ttf",12,255,255,255,255);
                xx+=convert(pn[1],12)+3;
            }
            else if(i==72){
                textRGBA(renderer,xx,7,pn[2].c_str(),"tahoma.ttf",12,255,255,255,255);
                xx+=convert(pn[2],12)+3;
            }
            else if(i/10==5){
                filledCircleColor(renderer,xx+13,13,13,c[i%10]);xx+=29;
            }
            else if(i/10==4){
                filledTrigonColor(renderer,xx,13,xx+13,0,xx+13,26,0x33000000);
                filledTrigonColor(renderer,xx+26,13,xx+14,0,xx+14,26,0x33000000);
                xx+=29;
            }
            else if(i/10==3){
                candy[i%10]->r={r.x+xx,r.y,candy[i%10]->r.w,candy[i%10]->r.h};
                SDL_Rect o={xx,0,candy[i%10]->r.w,candy[i%10]->r.h};SDL_RenderCopy(renderer,candy[i%10]->blu, nullptr,&o);
                xx+=candy[i%10]->r.w+3;
            }
            else if(i/10==8){
                vari[i%10]->r={r.x+xx,r.y,vari[i%10]->r.w,vari[i%10]->r.h};
                SDL_Rect o={xx,0,vari[i%10]->r.w,vari[i%10]->r.h};SDL_RenderCopy(renderer,vari[i%10]->blu, nullptr,&o);
                xx+=vari[i%10]->r.w+3;
            }
            else if(i/10==2){
                int cc=convert(mod[i%10],12);
                bcl(renderer,xx,4,xx+cc+17,22,0xffffffff);
                textRGBA(renderer,xx+2,5,mod[i%10].c_str(),"tahoma.ttf",12,rrr,gg,bb,aa);
                SDL_Rect er={xx+cc+4,3,bw,bh};
                SDL_RenderCopy(renderer,uni[int(color)], nullptr,&er);
                xx+=cc+20;
            }
            else if(i/10==9){
                int j=i%10;
                if(v[j].empty()){
                    filledCircleColor(renderer,xx+13,13,13,0xffffffff);xx+=29;
                }
                else{
                    int a=convert(v[j],12)>14?(convert(v[j],12)-14):0;int g=a>0?7:(convert(v[j],12)/2);
                    filledCircleColor(renderer,xx+13,13,13,0xffffffff);
                    filledCircleColor(renderer,xx+13+a,13,13,0xffffffff);
                    bcl(renderer,xx+13,0,xx+13+a,26,0xffffffff);
                    textRGBA(renderer,xx-g+13,6,v[j].c_str(),"tahoma.ttf",12,0,0,0,255);
                    xx+=a+29;
                }
            }
            else if(i==-1||i==1){
                int rt=convert("turn",12)+4;
                SDL_Rect rr={rt,5,aw,ah};
                SDL_RenderCopy(renderer,uni[i], nullptr,&rr);
            }
        }
        SDL_SetTextureBlendMode(tyu,SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(renderer,nullptr);
        if(blu!=nullptr){
            auto it=find(tex.rbegin(),tex.rend(),blu);SDL_DestroyTexture(*it);tex.erase(next(it).base());
        }
        blu=tyu;
    }
};
struct exten{

    SDL_Texture* asli;
    UINT32 ecolor;
    void create(SDL_Renderer*renderer){
        asli=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,250,ht-80);
        tex.push_back(asli);
    }
    vector<block>blocks;
    void color(){
        for(auto &i:blocks){
            i.color=ecolor;
        }
    }
    void draw(SDL_Renderer*renderer,int n){
        SDL_SetRenderTarget(renderer,asli);
        SDL_SetRenderDrawColor(renderer,225,245,225,255);
        SDL_RenderClear(renderer);
        for(const block& i:blocks){
            SDL_RenderCopy(renderer,i.blu, nullptr,&i.r);
            if(i.b==true)rectangleColor(renderer,5,i.r.y+7,17,i.r.y+19,0xff000000);
        }
        if(n==7){
            rectangleColor(renderer,5,5,93,31,0xff000000);
            textRGBA(renderer,8,11,"Make a variable","tahoma.ttf",12,0,0,0,255);
        }
        else if(n==8){
            rectangleColor(renderer,5,5,convert("Make a block",12)+11,31,0xff000000);
            textRGBA(renderer,8,11,"Make a block","tahoma.ttf",12,0,0,0,255);
        }
        SDL_SetRenderTarget(renderer, nullptr);
    }
    void copy(SDL_Renderer*renderer)const{
        SDL_Rect rq={56,0,250,ht-80};
        SDL_RenderCopy(renderer,asli, nullptr,&rq);
    }
};
void myvc(SDL_Texture*&texture,SDL_Renderer*renderer){
    texture=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,120,72);
    SDL_SetRenderTarget(renderer,texture);
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);
    rectangleColor(renderer,6,24,114,48,0xff000000);
    bcl(renderer,96,48,120,72,0xffffa0a0);
    textRGBA(renderer,6,6,"name:","tahoma.ttf",12,0,0,0,255);
    textRGBA(renderer,100,52,"OK","tahoma.ttf",12,0,0,0,255);
    bcl(renderer,12,26,15,46,0xff000000);
    SDL_SetRenderTarget(renderer,nullptr);
}
void drawex(SDL_Renderer*renderer) {
    bcl(renderer,0,0,56,ht-80,0xffffffff);
    bcl(renderer,0,0,56,50,0xffe1f5e1);
    const char *a[9] = {"Motion", "Looks", "Sound", "Events", "Control", "Sensing", "Operators", "Variables",
                        "My blocks"};
    for (int i = 0; i < 9; i++) {
        filledCircleColor(renderer, 28, 20 + i * 50, 10, color(i));
        int x = 28 - convert(a[i], 12) / 2;
        textRGBA(renderer, x, 31 + 50 * i, a[i], "tahoma.ttf", 12, 0, 0, 0, 255);
    }
}
void drawexx(SDL_Renderer*renderer,int n,int m) {
    bcl(renderer,0,50*n,56,50+50*n,0xffffffff);
    bcl(renderer,0,50*n,56,50+50*n,0xffe1f5e1);
    const char *a[9] = {"Motion", "Looks", "Sound", "Events", "Control", "Sensing", "Operators", "Variables",
                        "My blocks"};
    filledCircleColor(renderer, 28, 20 + n * 50, 10, color(n));
    int x = 28 - convert(a[n], 12) / 2;
    textRGBA(renderer, x, 31 + 50 * n, a[n], "tahoma.ttf", 12, 0, 0, 0, 255);
    bcl(renderer,0,50*m,56,50+50*m,0xffffffff);
    filledCircleColor(renderer, 28, 20 + m * 50, 10, color(m));
    int xx = 28 - convert(a[m], 12) / 2;
    textRGBA(renderer, xx, 31 + 50 * m, a[m], "tahoma.ttf", 12, 0, 0, 0, 255);
}
void c(block &p){
    p.hofre[0]=p.hofre[1]=p.b=false;
}
vector<block> bclist(int i){
    vector<block> p;
    if(i==0){
        block a1;c(a1);a1.sit=0;a1.u=a1.d=true;
        a1.pn[0]="move";a1.pn[1]="steps";a1.v[0]="10";
        a1.order={70,90,71};
        p.push_back(a1);
        block a2;c(a2);a2.sit=0;a2.u=a2.d=true;
        a2.pn[0]="turn   ";a2.pn[1]="degrees";a2.v[0]="15";
        a2.order={1,70,90,71};
        p.push_back(a2);
        block a3;c(a3);a3.sit=0;a3.u=a3.d=true;
        a3.pn[0]="turn   ";a3.pn[1]="degrees";a3.v[0]="15";
        a3.order={-1,70,90,71};
        p.push_back(a3);
        block a4;c(a4);a4.sit=0;a4.u=a4.d=true;
        a4.pn[0]="go to";a4.mod[0]="random position";
        a4.order={70,20};
        p.push_back(a4);
        block a5;c(a5);a5.sit=0;a5.u=a5.d=true;
        a5.pn[0]="go to x:";a5.pn[1]="y:";a5.v[0]="0";a5.v[1]="0";
        a5.order={70,90,71,91};
        p.push_back(a5);
        block a6;c(a6);a6.sit=0;a6.u=a6.d=true;
        a6.pn[0]="glide";a6.pn[1]="secs to";a6.v[0]="0";a6.mod[0]="random position";
        a6.order={70,90,71,20};
        p.push_back(a6);
        block a7;c(a7);a7.sit=0;a7.u=a7.d=true;
        a7.pn[0]="glide";a7.pn[1]="secs to x:";a7.pn[2]="y:";a7.v[0]="1";a7.v[1]="0";a7.v[2]="0";
        a7.order={70,90,71,91,72,92};
        p.push_back(a7);
        block a8;c(a8);a8.sit=0;a8.u=a8.d=true;
        a8.pn[0]="point in direction";a8.v[0]="90";
        a8.order={70,90};
        p.push_back(a8);
        block a9;c(a9);a9.sit=0;a9.u=a9.d=true;
        a9.pn[0]="point towards";a9.mod[0]="mouse-pointer";
        a9.order={70,20};
        p.push_back(a9);
        block a10;c(a10);a10.sit=0;a10.u=a10.d=true;
        a10.pn[0]="change x by";a10.v[0]="10";
        a10.order={70,90};
        p.push_back(a10);
        block a0;c(a0);a0.sit=0;a0.u=a0.d=true;
        a0.pn[0]="set x to";a0.v[0]="0";
        a0.order={70,90};
        p.push_back(a0);
        block a;c(a);a.sit=0;a.u=a.d=true;
        a.pn[0]="change y by";a.v[0]="10";
        a.order={70,90};
        p.push_back(a);
        block a00;c(a00);a00.sit=0;a00.u=a00.d=true;
        a00.pn[0]="set y to";a00.v[0]="0";
        a00.order={70,90};
        p.push_back(a00);
        block a11;c(a11);a11.sit=0;a11.u=a11.d=true;
        a11.pn[0]="if on edge, bounce";
        a11.order={70};
        p.push_back(a11);
        block a12;c(a12);a12.sit=0;a12.u=a12.d=true;
        a12.pn[0]="set rotation style";a12.mod[0]="left-right";
        a12.order={70,20};
        p.push_back(a12);
        block a13;c(a13);a13.sit=2;a13.b=true;a13.u=a13.d=false;
        a13.pn[0]="x position";
        a13.order={70};
        p.push_back(a13);
        block a14;c(a13);a14.sit=2;a14.b=true;a14.u=a14.d=false;
        a14.pn[0]="y position";
        a14.order={70};
        p.push_back(a14);
        block a15;c(a15);a15.sit=2;a15.b=true;a15.u=a15.d=false;
        a15.pn[0]="direction";
        a15.order={70};
        p.push_back(a15);
    }
    else if(i==1){
        block a1;c(a1);a1.sit=0;a1.u=a1.d=true;
        a1.pn[0]="say";a1.pn[1]="for";a1.pn[2]="seconds";a1.v[0]="Hello!";a1.v[1]="2";
        a1.order={70,90,71,91,72};
        p.push_back(a1);
        block a2;c(a2);a2.sit=0;a2.u=a2.d=true;
        a2.pn[0]="say";a2.v[0]="Hello!";
        a2.order={70,90};
        p.push_back(a2);
        block a3;c(a3);a3.sit=0;a3.u=a3.d=true;
        a3.pn[0]="think";a3.pn[1]="for";a3.pn[2]="seconds";a3.v[0]="Hmm..";a3.v[1]="2";
        a3.order={70,90,71,91,72};
        p.push_back(a3);
        block a4;c(a4);a4.sit=0;a4.u=a4.d=true;
        a4.pn[0]="think";a4.v[0]="Hmm..";
        a4.order={70,90};
        p.push_back(a4);
        block a5;c(a5);a5.sit=0;a5.u=a5.d=true;
        a5.pn[0]="switch costume to";a5.mod[0]="costume2";
        a5.order={70,20};
        p.push_back(a5);
        block a6;c(a6);a6.sit=0;a6.u=a6.d=true;
        a6.pn[0]="next costume";
        a6.order={70};
        p.push_back(a6);
        block a7;c(a7);a7.sit=0;a7.u=a7.d=true;
        a7.pn[0]="switch backdrop to";a7.mod[0]="backdrop1";
        a7.order={70,20};
        p.push_back(a7);
        block a8;c(a8);a8.sit=0;a8.u=a8.d=true;
        a8.pn[0]="next backdrop";
        a8.order={70};
        p.push_back(a8);
        block a9;c(a9);a9.sit=0;a9.u=a9.d=true;
        a9.pn[0]="change size by";a9.v[0]="10";
        a9.order={70,90};
        p.push_back(a9);
        block a10;c(a10);a10.sit=0;a10.u=a10.d=true;
        a10.pn[0]="set size to";a10.v[0]="100";a10.pn[1]="%";
        a10.order={70,90,71};
        p.push_back(a10);
        block a0;c(a0);a0.sit=0;a0.u=a0.d=true;
        a0.pn[0]="change";a0.pn[1]="effect by";a0.v[0]="25";a0.mod[0]="color";
        a0.order={70,20,71,90};
        p.push_back(a0);
        block a;c(a);a.sit=0;a.u=a.d=true;
        a.pn[0]="set";a.pn[1]="effect to";a.v[0]="0";a.mod[0]="color";
        a.order={70,20,71,90};
        p.push_back(a);
        block a00;c(a00);a00.sit=0;a00.u=a00.d=true;
        a00.pn[0]="clear graphic effects";
        a00.order={70};
        p.push_back(a00);
        block a11;c(a11);a11.sit=0;a11.u=a11.d=true;
        a11.pn[0]="show";
        a11.order={70};
        p.push_back(a11);
        block a12;c(a12);a12.sit=0;a12.u=a12.d=true;
        a12.pn[0]="hide";
        a12.order={70};
        p.push_back(a12);
        block aa1;c(aa1);aa1.sit=0;aa1.u=aa1.d=true;
        aa1.pn[0]="go to";aa1.pn[1]="layer";aa1.mod[0]="front";
        aa1.order={70,20,71};
        p.push_back(aa1);
        block aa2;c(aa2);aa2.sit=0;aa2.u=aa2.d=true;
        aa2.pn[0]="go";aa2.pn[1]="layers";aa2.mod[0]="forward";aa2.v[0]="1";
        aa2.order={70,20,90,71};
        p.push_back(aa2);
        block a13;c(a13);a13.sit=2;a13.b=true;a13.u=a13.d=false;
        a13.pn[0]="costume";a13.mod[0]="number";
        a13.order={70,20};
        p.push_back(a13);
        block a14;c(a13);a14.sit=2;a14.b=true;a14.u=a14.d=false;
        a14.pn[0]="backdrop";a14.mod[0]="number";
        a14.order={70,20};
        p.push_back(a14);
        block a15;c(a15);a15.sit=2;a15.b=true;a15.u=a15.d=false;
        a15.pn[0]="size";
        a15.order={70};
        p.push_back(a15);
    }
    else if(i==3){
        block a1;c(a1);a1.sit=0;a1.u=false;a1.d=true;
        a1.pn[0]="when clicked";
        a1.order={70};
        p.push_back(a1);
        block a2;c(a2);a2.sit=0;a2.u=false;a2.d=true;
        a2.pn[0]="when";a2.pn[1]="key pressed";a2.mod[0]="space";
        a2.order={70,20,71};
        p.push_back(a2);
        block a3;c(a3);a3.sit=0;a3.u=false;a3.d=true;
        a3.pn[0]="when this sprite clicked";
        a3.order={70};
        p.push_back(a3);
        block a4;c(a4);a4.sit=0;a4.u=false;a4.d=true;
        a4.pn[0]="when backdrop switches to";a4.mod[0]="backdrop1";
        a4.order={70,20};
        p.push_back(a4);
        block a5;c(a5);a5.sit=0;a5.u=false;a5.d=true;
        a5.pn[0]="when";a5.pn[1]=">";a5.mod[0]="loudness";a5.v[0]="10";
        a5.order={70,20,71,90};
        p.push_back(a5);
        block a6;c(a6);a6.sit=0;a6.u=false;a6.d=true;
        a6.pn[0]="when i receive";a6.mod[0]="massage1";
        a6.order={70,20};
        p.push_back(a6);
        block a7;c(a7);a7.sit=0;a7.u=a7.d=true;
        a7.pn[0]="broadcast";a7.mod[0]="massage1";
        a7.order={70,20};
        p.push_back(a7);
        block a8;c(a8);a8.sit=0;a8.u=a8.d=true;
        a8.pn[0]="broadcast";a8.pn[1]="and wait";a8.mod[0]="massage1";
        a8.order={70,20,71};
        p.push_back(a8);
    }
    else if(i==2){
        block a1;c(a1);a1.sit=0;a1.u=a1.d=true;
        a1.pn[0]="play sound";a1.pn[1]="until done";a1.mod[0]="Meow";
        a1.order={70,20,71};
        p.push_back(a1);
        block a2;c(a2);a2.sit=0;a2.u=a2.d=true;
        a2.pn[0]="set sound";a2.mod[0]="Meow";
        a2.order={70,20};
        p.push_back(a2);
        block a3;c(a3);a3.sit=0;a3.u=a3.d=true;
        a3.pn[0]="stop all sounds";
        a3.order={70};
        p.push_back(a3);
        block a4;c(a4);a4.sit=0;a4.u=a4.d=true;
        a4.pn[0]="change";a4.pn[1]="effect by";a4.v[0]="10";a4.mod[0]="pitch";
        a4.order={70,20,71,90};
        p.push_back(a4);
        block a5;c(a5);a5.sit=0;a5.u=a5.d=true;
        a5.pn[0]="set";a5.pn[1]="effect to";a5.v[0]="100";a5.mod[0]="pitch";
        a5.order={70,20,71,90};
        p.push_back(a5);
        block a6;c(a6);a6.sit=0;a6.u=a6.d=true;
        a6.pn[0]="clear sound effects";
        a6.order={70};
        p.push_back(a6);
        block a7;c(a7);a7.sit=0;a7.u=a7.d=true;
        a7.pn[0]="change volume by";a7.v[0]="-10";
        a7.order={70,90};
        p.push_back(a7);
        block a8;c(a8);a8.sit=0;a8.u=a8.d=true;
        a8.pn[0]="set volume to";a8.pn[1]="%";a8.v[0]="100";
        a8.order={70,90,71};
        p.push_back(a8);
        block a9;c(a9);a9.sit=2;a9.u=a9.d=false;a9.b=true;
        a9.pn[0]="volume";
        a9.order={70};
        p.push_back(a9);
    }
    else if(i==4){
        block a1;c(a1);a1.sit=0;a1.u=a1.d=true;
        a1.pn[0]="wait";a1.pn[1]="seconds";a1.v[0]="1";
        a1.order={70,90,71};
        p.push_back(a1);
        block a2;c(a2);a2.sit=0;a2.u=a2.d=true;a2.hofre[0]=true;
        a2.pn[0]="repeat";a2.v[0]="10";
        a2.order={70,90};
        p.push_back(a2);
        block a3;c(a3);a3.sit=0;a3.u=true;a3.d=false;a3.hofre[0]=true;
        a3.pn[0]="forever";
        a3.order={70};
        p.push_back(a3);
        block a4;c(a4);a4.sit=0;a4.u=a4.d=true;a4.hofre[0]=true;
        a4.pn[0]="if";a4.pn[1]="then";
        a4.order={70,40,71};
        p.push_back(a4);
        block a5;c(a5);a5.sit=0;a5.u=a5.d=true;
        a5.pn[0]="if";a5.pn[1]="then";a5.hofre[0]=a5.hofre[1]=true;
        a5.order={70,40,71};
        p.push_back(a5);
        block a6;c(a6);a6.sit=0;a6.u=a6.d=true;
        a6.pn[0]="wait until";
        a6.order={70,40};
        p.push_back(a6);
        block a7;c(a7);a7.sit=0;a7.u=a7.d=true;a7.hofre[0]=true;
        a7.pn[0]="repeat until";
        a7.order={70,40};
        p.push_back(a7);
        block a8;c(a8);a8.sit=0;a8.u=true;a8.d=false;
        a8.pn[0]="stop";a8.mod[0]="all";
        a8.order={70,20};
        p.push_back(a8);
        block a9;c(a9);a9.sit=0;a9.u=false;a9.d=true;
        a9.pn[0]="when I start as clone";
        a9.order={70};
        p.push_back(a9);
        block a;c(a);a.sit=0;a.u=a.d=true;
        a.pn[0]="create clone of";a.mod[0]="myself";
        a.order={70,20};
        p.push_back(a);
        block a0;c(a0);a0.sit=0;a0.u=true;a0.d=false;
        a0.pn[0]="delete this clone";
        a0.order={70};
        p.push_back(a0);
    }
    else if(i==5){
        block a1;c(a1);a1.sit=1;a1.u=a1.d=false;
        a1.pn[0]="touching";a1.pn[1]="?";a1.mod[0]="mouse-pointer";
        a1.order={70,20,71};
        p.push_back(a1);
        block a2;c(a2);a2.sit=1;a2.u=a2.d=false;
        a2.pn[0]="touching color";a2.pn[1]="?";a2.c[0]=0xffff0000;
        a2.order={70,50,71};
        p.push_back(a2);
        block a3;c(a3);a3.sit=1;a3.u=a3.d=false;
        a3.pn[0]="color";a3.pn[1]="is touching";a3.pn[2]="?";a3.c[0]=0xff00ff00;a3.c[1]=0xff0000ff;
        a3.order={70,50,71,51,72};
        p.push_back(a3);
        block a4;c(a4);a4.sit=2;a4.u=a4.d=false;
        a4.pn[0]="distance to";a4.mod[0]="mouse-pointer";
        a4.order={70,20};
        p.push_back(a4);
        block a5;c(a5);a5.sit=0;a5.u=a5.d=true;
        a5.pn[0]="ask";a5.pn[1]="and wait";a5.v[0]="Whats your name";
        a5.order={70,90,71};
        p.push_back(a5);
        block a6;c(a6);a6.sit=2;a6.u=a6.d=false;a6.b=true;
        a6.pn[0]="answer";
        a6.order={70};
        p.push_back(a6);
        block a7;c(a7);a7.sit=1;a7.u=a7.d=false;
        a7.pn[0]="ket";a7.pn[1]="pressed";a7.mod[0]="space";
        a7.order={70,20,71};
        p.push_back(a7);
        block a8;c(a8);a8.sit=1;a8.u=a8.d=false;
        a8.pn[0]="mouse down?";
        a8.order={70};
        p.push_back(a8);
        block a9;c(a9);a9.sit=2;a9.u=a9.d=false;
        a9.pn[0]="mouse x";
        a9.order={70};
        p.push_back(a9);
        block a10;c(a10);a10.sit=2;a10.u=a10.d=false;
        a10.pn[0]="mouse y";
        a10.order={70};
        p.push_back(a10);
        block a0;c(a0);a0.sit=0;a0.u=a0.d=true;
        a0.pn[0]="set drag mode";a0.mod[0]="draggable";
        a0.order={70,20};
        p.push_back(a0);
        block a;c(a);a.sit=2;a.u=a.d=false;a.b=true;
        a.pn[0]="loudness";
        a.order={70};
        p.push_back(a);
        block a00;c(a00);a00.sit=2;a00.u=a00.d=false;a00.b=true;
        a00.pn[0]="timer";
        a00.order={70};
        p.push_back(a00);
        block a11;c(a11);a11.sit=0;a11.u=a11.d=true;
        a11.pn[0]="reset timer";
        a11.order={70};
        p.push_back(a11);
        block a12;c(a12);a12.sit=2;a12.u=a12.d=false;
        a12.pn[0]="of";a12.mod[0]="backdrop#";a12.mod[1]="Stage";
        a12.order={20,70,21};
        p.push_back(a12);
        block aa1;c(aa1);aa1.sit=2;aa1.u=aa1.d=false;aa1.b=true;
        aa1.pn[0]="current";aa1.mod[0]="year";
        aa1.order={70,20};
        p.push_back(aa1);
        block aa2;c(aa2);aa2.sit=2;aa2.u=aa2.d=false;
        aa2.pn[0]="days since 2000";
        aa2.order={70};
        p.push_back(aa2);
        block a13;c(a13);a13.sit=2;a13.u=a13.d=false;a13.b=true;
        a13.pn[0]="username";
        a13.order={70};
        p.push_back(a13);
    }
    else if(i==6){
        block a1;c(a1);a1.sit=2;a1.u=a1.d=false;
        a1.pn[0]="+";a1.v[0]="";a1.v[1]="";
        a1.order={90,70,91};
        p.push_back(a1);
        block a2;c(a2);a2.sit=2;a2.u=a2.d=false;
        a2.pn[0]="-";a2.v[0]="";a2.v[1]="";
        a2.order={90,70,91};
        p.push_back(a2);
        block a3;c(a3);a3.sit=2;a3.u=a3.d=false;
        a3.pn[0]="*";a3.v[0]="";a3.v[1]="";
        a3.order={90,70,91};
        p.push_back(a3);
        block a4;c(a4);a4.sit=2;a4.u=a4.d=false;
        a4.pn[0]="/";a4.v[0]="";a4.v[1]="";
        a4.order={90,70,91};
        p.push_back(a4);
        block a5;c(a5);a5.sit=2;a5.u=a5.d=false;
        a5.pn[0]="pick random";a5.pn[1]="to";a5.v[0]="1";a5.v[1]="10";
        a5.order={70,90,71,91};
        p.push_back(a5);
        block a6;c(a6);a6.sit=1;a6.u=a6.d=false;
        a6.v[0]="";a6.v[1]="50";a6.pn[0]=">";
        a6.order={90,70,91};
        p.push_back(a6);
        block a7;c(a7);a7.sit=1;a7.u=a7.d=false;
        a7.v[0]="";a7.v[1]="50";a7.pn[0]="<";
        a7.order={90,70,91};
        p.push_back(a7);
        block a8;c(a8);a8.sit=1;a8.u=a8.d=false;
        a8.v[0]="";a8.v[1]="50";a8.pn[0]="=";
        a8.order={90,70,91};
        p.push_back(a8);
        block a9;c(a9);a9.sit=1;a9.u=a9.d=false;
        a9.pn[0]="and";
        a9.order={40,70,41};
        p.push_back(a9);
        block a10;c(a10);a10.sit=1;a10.u=a10.d=false;
        a10.pn[0]="or";
        a10.order={40,70,41};
        p.push_back(a10);
        block a0;c(a0);a0.sit=1;a0.u=a0.d=false;
        a0.pn[0]="not";
        a0.order={70,40};
        p.push_back(a0);
        block a;c(a);a.sit=2;a.u=a.d=false;
        a.pn[0]="join";a.v[0]="apple";a.v[01]="banana";
        a.order={70,90,91};
        p.push_back(a);
        block a00;c(a00);a00.sit=2;a00.u=a00.d=false;
        a00.pn[0]="letter";a00.pn[1]="of";a00.v[0]="1";a00.v[1]="apple";
        a00.order={70,90,71,91};
        p.push_back(a00);
        block a11;c(a11);a11.sit=2;a11.u=a11.d=true;
        a11.pn[0]="length of";a11.v[0]="apple";
        a11.order={70,90};
        p.push_back(a11);
        block a12;c(a12);a12.sit=1;a12.u=a12.d=false;
        a12.pn[0]="contains";a12.pn[1]="?";a12.v[0]="apple";a12.v[1]="a";
        a12.order={90,70,91,71};
        p.push_back(a12);
        block aa1;c(aa1);aa1.sit=2;aa1.u=aa1.d=false;
        aa1.pn[0]="mod";aa1.v[0]="";aa1.v[1]="";
        aa1.order={90,70,91};
        p.push_back(aa1);
        block aa2;c(aa2);aa2.sit=2;aa2.u=aa2.d=false;
        aa2.pn[0]="round";aa2.v[0]="";
        aa2.order={70,90};
        p.push_back(aa2);
        block a13;c(a13);a13.sit=2;a13.u=a13.d=false;
        a13.pn[0]="of";a13.mod[0]="abs";a13.v[0]="";
        a13.order={20,70,90};
        p.push_back(a13);
    }
    else if(i==7){
        block a2;c(a2);a2.sit=0;a2.u=a2.d=true;
        a2.pn[0]="set";a2.pn[1]="to";a2.v[0]="0";a2.mod[0]="my variable";
        a2.order={70,20,71,90};
        p.push_back(a2);
        block a3;c(a3);a3.sit=0;a3.u=a3.d=true;
        a3.pn[0]="change";a3.pn[1]="by";a3.v[0]="1";a3.mod[0]="my variable";
        a3.order={70,20,71,90};
        p.push_back(a3);
        block a4;c(a4);a4.sit=0;a4.u=a4.d=true;
        a4.pn[0]="show variable";a4.mod[0]="my variable";
        a4.order={70,20};
        p.push_back(a4);
        block a5;c(a5);a5.sit=0;a5.u=a5.d=true;
        a5.pn[0]="hide variable";a5.mod[0]="my variable";
        a5.order={70,20};
        p.push_back(a5);
        block a1;c(a1);a1.sit=2;a1.u=a1.d=false;a1.b=true;
        a1.pn[0]="my variable";
        a1.order={70};
        p.push_back(a1);
    }
    return p;
}
vector<block> bblist(int i){
    vector<block> p;
    if(i==1){
        block a7;c(a7);a7.sit=0;a7.u=a7.d=true;
        a7.pn[0]="switch backdrop to";a7.mod[0]="backdrop1";
        a7.order={70,20};
        p.push_back(a7);
        block a7a;c(a7a);a7a.sit=0;a7a.u=a7a.d=true;
        a7a.pn[0]="switch backdrop to";a7a.pn[1]="and wait";a7a.mod[0]="backdrop1";
        a7a.order={70,20,71};
        p.push_back(a7);
        block a8;c(a8);a8.sit=0;a8.u=a8.d=true;
        a8.pn[0]="next backdrop";
        a8.order={70};
        p.push_back(a8);
        block a0;c(a0);a0.sit=0;a0.u=a0.d=true;
        a0.pn[0]="change";a0.pn[1]="effect by";a0.v[0]="25";a0.mod[0]="color";
        a0.order={70,20,71,90};
        p.push_back(a0);
        block a;c(a);a.sit=0;a.u=a.d=true;
        a.pn[0]="set";a.pn[1]="effect to";a.v[0]="0";a.mod[0]="color";
        a.order={70,20,71,90};
        p.push_back(a);
        block a00;c(a00);a00.sit=0;a00.u=a00.d=true;
        a00.pn[0]="clear graphic effects";
        a00.order={70};
        p.push_back(a00);
        block a14;c(a14);a14.sit=2;a14.b=true;a14.u=a14.d=false;
        a14.pn[0]="backdrop";a14.mod[0]="number";
        a14.order={70,20};
        p.push_back(a14);
    }
    else if(i==3){
        block a1;c(a1);a1.sit=0;a1.u=false;a1.d=true;
        a1.pn[0]="when clicked";
        a1.order={70};
        p.push_back(a1);
        block a2;c(a2);a2.sit=0;a2.u=false;a2.d=true;
        a2.pn[0]="when";a2.pn[1]="key pressed";a2.mod[0]="space";
        a2.order={70,20,71};
        p.push_back(a2);
        block a3;c(a3);a3.sit=0;a3.u=false;a3.d=true;
        a3.pn[0]="when stage clicked";
        a3.order={70};
        p.push_back(a3);
        block a4;c(a4);a4.sit=0;a4.u=false;a4.d=true;
        a4.pn[0]="when backdrop switches to";a4.mod[0]="backdrop1";
        a4.order={70,20};
        p.push_back(a4);
        block a5;c(a5);a5.sit=0;a5.u=false;a5.d=true;
        a5.pn[0]="when";a5.pn[1]=">";a5.mod[0]="loudness";a5.v[0]="10";
        a5.order={70,20,71,90};
        p.push_back(a5);
        block a6;c(a6);a6.sit=0;a6.u=false;a6.d=true;
        a6.pn[0]="when i receive";a6.mod[0]="massage1";
        a6.order={70,20};
        p.push_back(a6);
        block a7;c(a7);a7.sit=0;a7.u=a7.d=true;
        a7.pn[0]="broadcast";a7.mod[0]="massage1";
        a7.order={70,20};
        p.push_back(a7);
        block a8;c(a8);a8.sit=0;a8.u=a8.d=true;
        a8.pn[0]="broadcast";a8.pn[1]="and wait";a8.mod[0]="massage1";
        a8.order={70,20,71};
        p.push_back(a8);
    }
    else if(i==2){
        block a1;c(a1);a1.sit=0;a1.u=a1.d=true;
        a1.pn[0]="play sound";a1.pn[1]="until done";a1.mod[0]="pop";
        a1.order={70,20,71};
        p.push_back(a1);
        block a2;c(a2);a2.sit=0;a2.u=a2.d=true;
        a2.pn[0]="set sound";a2.mod[0]="pop";
        a2.order={70,20};
        p.push_back(a2);
        block a3;c(a3);a3.sit=0;a3.u=a3.d=true;
        a3.pn[0]="stop all sounds";
        a3.order={70};
        p.push_back(a3);
        block a4;c(a4);a4.sit=0;a4.u=a4.d=true;
        a4.pn[0]="change";a4.pn[1]="effect by";a4.v[0]="10";a4.mod[0]="pitch";
        a4.order={70,20,71,90};
        p.push_back(a4);
        block a5;c(a5);a5.sit=0;a5.u=a5.d=true;
        a5.pn[0]="set";a5.pn[1]="effect to";a5.v[0]="100";a5.mod[0]="pitch";
        a5.order={70,20,71,90};
        p.push_back(a5);
        block a6;c(a6);a6.sit=0;a6.u=a6.d=true;
        a6.pn[0]="clear sound effects";
        a6.order={70};
        p.push_back(a6);
        block a7;c(a7);a7.sit=0;a7.u=a7.d=true;
        a7.pn[0]="change volume by";a7.v[0]="-10";
        a7.order={70,90};
        p.push_back(a7);
        block a8;c(a8);a8.sit=0;a8.u=a8.d=true;
        a8.pn[0]="set volume to";a8.pn[1]="%";a8.v[0]="100";
        a8.order={70,90,71};
        p.push_back(a8);
        block a9;c(a9);a9.sit=2;a9.u=a9.d=false;a9.b=true;
        a9.pn[0]="volume";
        a9.order={70};
        p.push_back(a9);
    }
    else if(i==4){
        block a1;c(a1);a1.sit=0;a1.u=a1.d=true;
        a1.pn[0]="wait";a1.pn[1]="seconds";a1.v[0]="1";
        a1.order={70,90,71};
        p.push_back(a1);
        block a2;c(a2);a2.sit=0;a2.u=a2.d=true;a2.hofre[0]=1;
        a2.pn[0]="repeat";a2.v[0]="10";
        a2.order={70,90};
        p.push_back(a2);
        block a3;c(a3);a3.sit=0;a3.u=true;a3.d=false;a3.hofre[0]=1;
        a3.pn[0]="forever";
        a3.order={70};
        p.push_back(a3);
        block a4;c(a4);a4.sit=0;a4.u=a4.d=true;
        a4.pn[0]="if";a4.pn[1]="then";a4.hofre[0]=1;
        a4.order={70,3,71};
        p.push_back(a4);
        block a5;c(a5);a5.sit=0;a5.u=a5.d=true;
        a5.pn[0]="if";a5.pn[1]="then";a5.hofre[0]=1;a5.hofre[1]=1;
        a5.order={70,3,71};
        p.push_back(a5);
        block a6;c(a6);a6.sit=0;a6.u=a6.d=true;
        a6.pn[0]="wait until";
        a6.order={70,3};
        p.push_back(a6);
        block a7;c(a7);a7.sit=0;a7.u=a7.d=true;a7.hofre[0]=1;
        a7.pn[0]="repeat until";
        a7.order={70,3};
        p.push_back(a7);
        block a8;c(a8);a8.sit=0;a8.u=true;a8.d=false;
        a8.pn[0]="stop";a8.mod[0]="all";
        a8.order={70,20};
        p.push_back(a8);
        block a;c(a);a.sit=0;a.u=a.d=true;
        a.pn[0]="create clone of";a.mod[0]="Cat";
        a.order={70,20};
        p.push_back(a);
    }
    else if(i==5){
        block a5;c(a5);a5.sit=0;a5.u=a5.d=true;
        a5.pn[0]="ask";a5.pn[1]="and wait";a5.v[0]="Whats your name";
        a5.order={70,90,71};
        p.push_back(a5);
        block a6;c(a6);a6.sit=2;a6.u=a6.d=false;a6.b=true;
        a6.pn[0]="answer";
        a6.order={70};
        p.push_back(a6);
        block a7;c(a7);a7.sit=1;a7.u=a7.d=false;
        a7.pn[0]="ket";a7.pn[1]="pressed";a7.mod[0]="space";
        a7.order={70,20,71};
        p.push_back(a7);
        block a8;c(a8);a8.sit=1;a8.u=a8.d=false;
        a8.pn[0]="mouse down?";
        a8.order={70};
        p.push_back(a8);
        block a9;c(a9);a9.sit=2;a9.u=a9.d=false;
        a9.pn[0]="mouse x";
        a9.order={70};
        p.push_back(a9);
        block a10;c(a10);a10.sit=2;a10.u=a10.d=false;
        a10.pn[0]="mouse y";
        a10.order={70};
        p.push_back(a10);
        block a;c(a);a.sit=2;a.u=a.d=false;a.b=true;
        a.pn[0]="loudness";
        a.order={70};
        p.push_back(a);
        block a00;c(a00);a00.sit=2;a00.u=a00.d=false;a00.b=true;
        a00.pn[0]="timer";
        a00.order={70};
        p.push_back(a00);
        block a11;c(a11);a11.sit=0;a11.u=a11.d=true;
        a11.pn[0]="reset timer";
        a11.order={70};
        p.push_back(a11);
        block a12;c(a12);a12.sit=2;a12.u=a12.d=false;
        a12.pn[0]="of";a12.mod[0]="backdrop#";a12.mod[1]="Stage";
        a12.order={20,70,21};
        p.push_back(a12);
        block aa1;c(aa1);aa1.sit=2;aa1.u=aa1.d=false;aa1.b=true;
        aa1.pn[0]="current";aa1.mod[0]="year";
        aa1.order={70,20};
        p.push_back(aa1);
        block aa2;c(aa2);aa2.sit=2;aa2.u=aa2.d=false;
        aa2.pn[0]="days since 2000";
        aa2.order={70};
        p.push_back(aa2);
        block a13;c(a13);a13.sit=2;a13.u=a13.d=false;a13.b=true;
        a13.pn[0]="username";
        a13.order={70};
        p.push_back(a13);
    }
    else if(i==6||i==7){
        return bclist(i);
    }
    return p;
}
#endif
