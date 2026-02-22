
#ifndef SCRATCH_CODE1_H
#define SCRATCH_CODE1_H
#include "code.h"
struct p{
    bool b;int j;
};
struct o{
    bool b{};vector<block*>j;int g{};int right=0;block*rib{};int i1{},i2{};int ss{};
};
int xr,yr,wr,hr;
vector<block*>bbl;
SDL_Color click(const block& p,int x,int y){
    SDL_Color color = {0, 0, 0, 0};
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(
            0, p.r.w, p.r.h, 32, SDL_PIXELFORMAT_RGBA32
    );
    SDL_SetRenderTarget(renderer,p.blu);
    SDL_RenderReadPixels(renderer, nullptr, surface->format->format,surface->pixels, surface->pitch);
    SDL_SetRenderTarget(renderer, nullptr);
    UINT32*pixels=(UINT32*)surface->pixels;
    UINT32 pixel=pixels[(y-p.r.y)*surface->w+(x-p.r.x)];
    SDL_GetRGBA(pixel,surface->format,&color.r,&color.g,&color.b,&color.a);
    SDL_FreeSurface(surface);
    return color;
}
block* copyb(const block& cv){
    block*d=new block(cv);
    d->blu=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,cv.r.w,cv.r.h);
    SDL_SetRenderTarget(renderer,d->blu);
    SDL_SetRenderDrawColor(renderer,255,255,255,0);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer,cv.blu, nullptr, nullptr);
    SDL_SetTextureBlendMode(d->blu,SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, nullptr);
    return d;
}
int wide(const vector<block*>&a){
    int r=0;
    for(auto i:a){
        if(r<i->r.w)r=i->r.w;
    }
    return r;
}
struct codtxs{
    SDL_Texture *texture;
    vector<string> bol[8];
    void create(const exten& i){
        texture=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,wt-500,ht-80);
        tex.push_back(texture);SDL_SetRenderTarget(renderer,texture);SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderClear(renderer);drawex();i.copy();SDL_SetRenderTarget(renderer, nullptr);
    }
    void copy() const{
        SDL_Rect r={0,80,wt-500,ht-80};SDL_RenderCopy(renderer,texture, nullptr,&r);
    }
    vector<vector<block*>>data;
    vector<int>exen;vector<bool>exeb;
    void fill(){
        for(int i=0;i<data.size();i++){exen.push_back(0);exeb.push_back(false);}
    }
    p shart(vector<block*> px,vector<block*>bp){
        if(bp[0]->u&&bp[bp.size()-1]->d){
            for(int i=0;i<px.size();i++){
                if(fabs(bp[0]->r.x-px[i]->r.x)<25&&px[i]->hofre[0]&&px[i]->r.y+px[i]->h[0]+32-bp[0]->r.y>0&&bp[0]->r.y-px[i]->r.y>6){
                    if(px[i]->hof[0].empty()){
                        px[i]->hof[0]=bp;
                        px[i]->draw();return{true,i};
                    }
                    for(int j=0;j<px[i]->hof[0].size();j++){
                        if(fabs(bp[0]->r.y-px[i]->hof[0][j]->r.y+7)<13){
                            px[i]->hof[0].insert(px[i]->hof[0].begin()+j,bp.begin(),bp.end());
                            px[i]->draw();return{true,i};
                        }
                        if(fabs(bp[0]->r.y-(px[i]->h[0]+26+px[i]->r.y)+7)<13){
                            px[i]->hof[0].insert(px[i]->hof[0].begin()+j+1,bp.begin(),bp.end());
                            px[i]->draw();return{true,i};
                        }
                    }
                    p u=shart(px[i]->hof[0],bp);
                    if(u.b){
                        px[i]->draw();return{true,i};
                    }
                }
                if(fabs(bp[0]->r.x-px[i]->r.x)<25&&px[i]->hofre[1]&&px[i]->r.y+px[i]->r.h-bp[0]->r.y>26&&bp[0]->r.y-px[i]->r.y-px[i]->h[0]>32){
                    if(px[i]->hof[1].empty()){
                        px[i]->hof[1]=bp;
                        px[i]->draw();return{true,i};
                    }
                    for(int j=0;j<px[i]->hof[1].size();j++){
                        if(fabs(bp[0]->r.y-px[i]->hof[1][j]->r.y+7)<13){
                            px[i]->hof[1].insert(px[i]->hof[1].begin()+j,bp.begin(),bp.end());
                            px[i]->draw();return{true,i};
                        }
                        if(fabs(bp[0]->r.y-(px[i]->r.h+px[i]->r.y-32)+7)<13){
                            px[i]->hof[1].insert(px[i]->hof[1].begin()+j+1,bp.begin(),bp.end());
                            px[i]->draw();return{true,i};
                        }
                    }
                    p u=shart(px[i]->hof[1],bp);
                    px[i]->draw();if(u.b)return{true,i};
                }
            }
        }
        return {false,0};
    }
    static bool recpo(SDL_Rect rv,const block*pp){
        return  (pp->r.x>rv.x&&pp->r.x<rv.x+rv.w&&pp->r.y>rv.y&&pp->r.y<rv.y+rv.h);
    }
    static bool sit12(block*px,block*bp){
        if(recpo(px->r,bp)){
            for(int i=0;i<px->order.size();i++){
                if(px->order[i]/10==3&&recpo(px->candy[px->order[i]%10]->r,bp)){
                    bool vb=sit12(px->candy[px->order[i]%10],bp);
                    if(vb){
                        px->draw();return true;
                    }
                    if(bp->sit==1)px->candy[px->order[i]%10]=bp;px->draw();return true;
                }
                if(px->order[i]/10==8&&recpo(px->vari[px->order[i]%10]->r,bp)){
                    bool vb=sit12(px->vari[px->order[i]%10],bp);
                    if(vb){
                        px->draw();return true;
                    }
                    if(bp->sit==2)px->vari[px->order[i]%10]=bp;px->draw();return true;
                }
                if(px->order[i]/10==4&&bp->sit==1&&fabs(bp->r.x-px->r.x-px->xco[px->order[i]%10]-13)<13&&bp->r.y-px->r.y<26){
                    px->order[i]-=10;px->candy[px->order[i]%10]=bp;px->draw();return true;
                }
                if(px->order[i]/10==9&&bp->sit==2&&bp->r.x>px->r.x+px->x1v[px->order[i]%10]&&bp->r.x<px->r.x+px->x2v[px->order[i]%10]&&bp->r.y-px->r.y<26){
                    px->order[i]-=10;px->vari[px->order[i]%10]=bp;px->draw();return true;
                }
            }
        }
        return false;
    }
    p dr12(vector<block*>px,block*bp){
        for(int i=0;i<px.size();i++){
            int wx=px[i]->r.w;
            if(sit12(px[i],bp)){wr=wx;return{true,i};}
            if(px[i]->hofre[0]){
                if(dr12(px[i]->hof[0],bp).b){wr=wx;px[i]->draw();return{true,i};}
            }
            if(px[i]->hofre[1]){
                if(dr12(px[i]->hof[1],bp).b){wr=wx;px[i]->draw();return{true,i};}
            }
        }
        return{false,0};
    }
    void add(SDL_Texture*tes,vector<block*>bbc,SDL_Texture*yu){
        int ww=wide(bbc),hh=bbc[bbc.size()-1]->r.y+bbc[bbc.size()-1]->r.h-bbc[0]->r.y;
        if(bbc[0]->r.x+ww/2<306){
            SDL_RenderCopy(renderer,tes, nullptr, nullptr);
        }
        else{
            bool z=true;
            for(int i=0;i<data.size()&&z;i++){
                if(bbc[0]->sit==0){
                    for(int j=0;j<data[i].size()&&z;j++){
                        if(data[i][j]->u&&bbc[bbc.size()-1]->d&&fabs(bbc[0]->r.x-data[i][0]->r.x)<25&&((j==0&&bbc[0]->r.y-data[i][0]->r.y<6&&data[i][0]->r.y-bbc[0]->r.y-hh<19)||(j>0&&bbc[0]->u&&fabs(bbc[0]->r.y-data[i][j]->r.y+7)<13))){
                            z=false;SDL_RenderCopy(renderer,tes, nullptr, nullptr);
                            int x=data[i][j]->r.x,dy=data[i][j]->r.y-bbc[0]->r.y;
                            for(int a=j;a<data[i].size();a++){
                                SDL_SetRenderDrawColor(renderer,255,255,255,255);
                                SDL_RenderFillRect(renderer,&data[i][a]->r);data[i][a]->r.y+=hh-6;
                            }
                            data[i].insert(data[i].begin()+j,bbc.begin(),bbc.end());
                            for(int a=j>0?j-1:0;a<data[i].size();a++){
                                if(a>=j&&a<j+bbc.size()){data[i][a]->r.x=x;data[i][a]->r.y+=dy;}
                                SDL_RenderCopy(renderer,data[i][a]->blu,nullptr,&data[i][a]->r);
                            }
                        }
                        if(z&&j==data[i].size()-1&&data[i][j]->d&&bbc[0]->u&&fabs(bbc[0]->r.x-data[i][0]->r.x)<25&&bbc[0]->r.y-data[i][j]->r.y-data[i][j]->r.h>-26&&bbc[0]->r.y-data[i][j]->r.y-data[i][j]->r.h<19){
                            z=false;SDL_RenderCopy(renderer,tes, nullptr, nullptr);
                            data[i].insert(data[i].begin()+j+1,bbc.begin(),bbc.end());
                            int yz=data[i][j]->r.y+data[i][j]->r.h-6-data[i][j+1]->r.y;
                            for(int a=j+1;a<data[i].size();a++){
                                data[i][a]->r.x=data[i][0]->r.x;data[i][a]->r.y+=yz;
                                SDL_RenderCopy(renderer,data[i][a]->blu,nullptr,&data[i][a]->r);
                            }
                        }
                    }
                    if(z){
                        p u=shart(data[i],bbc);
                        if(u.b){
                            z=false;SDL_SetRenderTarget(renderer,yu);SDL_RenderCopy(renderer,tes, nullptr, nullptr);
                            for(int j=u.j;j<data[i].size();j++){
                                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                                SDL_RenderFillRect(renderer,&data[i][j]->r);
                                if(j>u.j)data[i][j]->r.y=data[i][j-1]->r.y+data[i][j-1]->r.h-6;
                            }
                            for(int j=u.j>0?u.j-1:0;j<data[i].size();j++)SDL_RenderCopy(renderer,data[i][j]->blu,nullptr,&data[i][j]->r);
                        }
                    }
                }
                if(bbc[0]->sit>0&&z){
                    p g=dr12(data[i],bbc[0]);
                    if(g.b){
                        z=false;
                        SDL_SetRenderTarget(renderer,yu);SDL_RenderCopy(renderer,tes, nullptr, nullptr);
                        SDL_SetRenderDrawColor(renderer,255,255,255,255);
                        SDL_Rect pc={data[i][g.j]->r.x+20,data[i][g.j]->r.y,wr-20,data[i][g.j]->r.h-6*(data[i][g.j]->d)};
                        SDL_RenderFillRect(renderer,&pc);
                        SDL_RenderCopy(renderer,data[i][g.j]->blu,nullptr,&data[i][g.j]->r);
                    }
                }
            }
            if(z){
                if(bbc[0]->r.x<306&&bbc[0]->r.x+ww/2>306){
                    SDL_RenderCopy(renderer,tes, nullptr, nullptr);
                    for(auto i:bbc){
                        i->r.x=306;SDL_RenderCopy(renderer,i->blu, nullptr,&i->r);
                    }
                }
                if(bbc[0]->r.x+ww>wt-500) {
                    SDL_RenderCopy(renderer, tes, nullptr, nullptr);
                    for(auto i:bbc){
                        i->r.x=wt-500-ww;SDL_RenderCopy(renderer,i->blu, nullptr,&i->r);
                    }
                }
                if(bbc[0]->r.y<0){
                    SDL_RenderCopy(renderer,tes, nullptr, nullptr);int yz=-bbc[0]->r.y;
                    for(auto i:bbc){
                        i->r.y+=yz;SDL_RenderCopy(renderer,i->blu, nullptr,&i->r);
                    }
                }
                if(bbc[0]->r.y+hh>ht-80){
                    SDL_RenderCopy(renderer,tes, nullptr, nullptr);int yz=ht-80-bbc[0]->r.y-hh;
                    for(auto i:bbc){
                        i->r.y+=yz;SDL_RenderCopy(renderer,i->blu, nullptr,&i->r);
                    }
                }
                data.push_back(bbc);
            }
        }
    }
    static o getko(int x,int y,block*&px){
        for(int &i:px->order){
            if(i/10==9&&x>px->r.x+px->x1v[i%10]&&x<px->r.x+px->x2v[i%10]&&y>px->r.y&&y<px->r.y+26&&click(*px,x,y).r>250){
                o hg;hg.b=false;hg.right=1;hg.ss=i%10;hg.rib=px;return hg;
            }
            if(i/10==2&&x>px->r.x+px->xm[i%10]&&x<px->r.x+px->xm[i%10]+convert(px->mod[i%10],12)+17&&y>px->r.y+4&&y<px->r.y+22){
                o hg;hg.b=false;hg.right=2;hg.ss=i%10;hg.rib=px;return hg;
            }
            if(i/10==3&&x>px->candy[i%10]->r.x&&x<px->candy[i%10]->r.x+px->candy[i%10]->r.w&&y>px->candy[i%10]->r.y&&y<px->candy[i%10]->r.y+px->candy[i%10]->r.h&&click(*px->candy[i%10],x,y).a>1){
                o daf=getko(x,y,px->candy[i%10]);
                if(daf.b){wr=px->r.w,hr=px->r.h,xr=px->r.x,yr=px->r.y;px->draw();return daf;}if(daf.right>0)return daf;
                i+=10;wr=px->r.w,hr=px->r.h,xr=px->r.x,yr=px->r.y;px->draw();return{true,{px->candy[i%10]}};
            }
            if(i/10==8&&x>px->vari[i%10]->r.x&&x<px->vari[i%10]->r.x+px->vari[i%10]->r.w&&y>px->vari[i%10]->r.y&&y<px->vari[i%10]->r.y+px->vari[i%10]->r.h&&click(*px->vari[i%10],x,y).a>1){
                o daf=getko(x,y,px->vari[i%10]);
                if(daf.b){wr=px->r.w,hr=px->r.h,xr=px->r.x,yr=px->r.y;px->draw();return daf;}if(daf.right>0)return daf;
                i+=10;wr=px->r.w,hr=px->r.h,xr=px->r.x,yr=px->r.y;px->v[i%10]="";px->draw();return{true,{px->vari[i%10]}};
            }
        }
        o jk;jk.b=false;return jk;
    }
    o getsh(int x,int y,vector<block*>&px){
        for(int i=0;i<px.size();i++){
            int wr1=px[i]->r.w,hr1=px[i]->r.h,xr1=px[i]->r.x,yr1=px[i]->r.y;
            if(px[i]->hofre[0]){
                o f=getsh(x,y,px[i]->hof[0]);
                if(f.b){px[i]->draw();wr=wr1;hr=hr1;yr=yr1;xr=xr1;return f;}
                if(f.right>0){f.i2=i;return f;}
                if(px[i]->hofre[1]){
                    o f1=getsh(x,y,px[i]->hof[1]);
                    if(f1.b){px[i]->draw();wr=wr1;hr=hr1;yr=yr1;xr=xr1;return f1;}
                    if(f.right>0){f.i2=i;return f;}
                }
            }
            if(x>px[i]->r.x&&x<px[i]->r.x+px[i]->r.w&&y>px[i]->r.y&&y<px[i]->r.y+px[i]->r.h&&click(*px[i],x,y).a>1){
                o daf=getko(x,y,px[i]);
                if(daf.b){px[i]->draw();wr=wr1;hr=hr1;yr=yr1;xr=xr1;return daf;}if(daf.right>0){daf.i2=i;return daf;}
                vector<block*>ans(px.begin()+i,px.end());yr=yr1;xr=xr1;hr=px.back()->r.y+px.back()->r.h-px[i]->r.y;wr=wr1;
                for(int j=i+1;j<px.size();j++){if(wr<px[j]->r.w)wr=px[j]->r.w;}px.erase(px.begin()+i,px.end());
                return{true,ans,i};
            }
        }
        o jk;jk.b=false;return jk;
    }
    o get(int x,int y,SDL_Texture*yu){
        for(int i=0;i<data.size();i++){
            o g=getsh(x,y,data[i]);
            if(g.b){
                SDL_SetRenderTarget(renderer,yu);SDL_Rect oop={xr,yr,wr,hr};SDL_SetRenderDrawColor(renderer,255,255,255,255);
                if(g.g==data[i].size()){
                    SDL_RenderFillRect(renderer,&oop);
                }
                else{
                    for(int a=g.g;a<data[i].size();a++){
                        SDL_Rect tee=data[i][a]->r;if(a==g.g)tee=oop;
                        SDL_RenderFillRect(renderer,&tee);
                    }
                }
                for(int a=g.g>0?g.g-1:0;a<data[i].size();a++){
                    if(a>g.g)data[i][a]->r.y=data[i][a-1]->r.y+data[i][a-1]->r.h-6;
                    SDL_RenderCopy(renderer,data[i][a]->blu, nullptr,&data[i][a]->r);
                }
                return g;
            }
            else if(g.right>0){g.i1=i;return g;}
        }
        o jk;jk.b=false;return jk;
    }
};
void tekan(block*&u,int x,int y){
    for(int i:u->order){
        if(i/10==3)tekan(u->candy[i%10],x,y);
        else if(i/10==8)tekan(u->vari[i%10],x,y);
    }
    if(u->hofre[0]){
        for(auto i:u->hof[0])tekan(i,x,y);
        if(u->hofre[1]){
            for(auto i:u->hof[1])tekan(i,x,y);
        }
    }
    u->r.x+=x;u->r.y+=y;
}
vector<codtxs> spco[2];

#endif //SCRATCH_CODE1_H
