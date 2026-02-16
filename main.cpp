#include "code1.h"
#include "settings.h"
#include "costum.h"
#include "run.h"
int main(int argc,char*argv[]){
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);TTF_Init();
    SDL_DisplayMode dm;SDL_GetCurrentDisplayMode(0, &dm);wt=dm.w;ht=dm.h-70;
    SDL_Window *window=SDL_CreateWindow("project",0,20,wt,ht,SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer= SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    SDL_RaiseWindow(window);
    SDL_Texture *topmain= SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,wt,ht);
    tex.push_back(topmain);
    drawmm(renderer,true,topmain);
    msx(renderer,0, true);
    SDL_RenderPresent(renderer);unic(renderer);
    exten exc[9];exten exb[9];
    for(int i=0;i<9;i++){
        exc[i].ecolor= color(i);exb[i].ecolor= color(i);
        exc[i].blocks=bclist(i);exb[i].blocks=bblist(i);
        exc[i].color();exb[i].color();
        for(int j=0;j<exc[i].blocks.size();j++){
           exc[i].blocks[j].draw(renderer);//tex.push_back(exc[i].blocks[j].blu);
           if(j>0)exc[i].blocks[j].r.y=exc[i].blocks[j-1].r.y+exc[i].blocks[j-1].r.h+10;
           else{
               exc[i].blocks[0].r.y=5;if(i==7)exc[i].blocks[0].r.y+=36;
           }
            exc[i].blocks[j].r.x=5+exc[i].blocks[j].b*17;
        }
        for(int j=0;j<exb[i].blocks.size();j++){
            exb[i].blocks[j].draw(renderer);//tex.push_back(exb[i].blocks[j].blu);
            if(j>0)exb[i].blocks[j].r.y=exb[i].blocks[j-1].r.y+exb[i].blocks[j-1].r.h+10;
            else{
                exb[i].blocks[0].r.y=5;if(i==7)exb[i].blocks[0].r.y+=36;
            }
            exb[i].blocks[j].r.x=5+exb[i].blocks[j].b*17;
        }
        exc[i].create(renderer);exb[i].create(renderer);
        exc[i].draw(renderer,i);exb[i].draw(renderer,i);
    }
    vector<codtxs> spco[2];
    codtxs start{};spco[0].push_back(start);spco[0][0].create(renderer,exc[0]);spco[0][0].copy(renderer);
    SDL_RenderPresent(renderer);bool drag=false;
    SDL_Texture *backsave=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,wt-500,ht-80);
    int m=0;bool aa;int xz,yz;SDL_Event e;tex.push_back(backsave);codtxs*working=&spco[0][0];vector<block*>current;
    bool myval=false;SDL_Texture*creva=nullptr;myvc(creva,renderer);SDL_Rect bn={149,5,120,72};string cva;int ivar;
    bool change=false;int i7;bool dellv=false;
    SDL_SetRenderTarget(renderer,working->texture);
    block*typeb=nullptr;bool ty=false;int ts;
    while(true){
        SDL_PollEvent(&e);
        if(e.type==SDL_QUIT)break;
        else if(e.type==SDL_MOUSEBUTTONDOWN){
            if(e.button.button==SDL_BUTTON_LEFT){
                if(m==7&&myval){
                    if(!(e.button.y>85&&e.button.y<157&&e.button.x>149&&e.button.x<269)){
                        myval=false;SDL_RenderCopy(renderer,backsave, nullptr, nullptr);
                        SDL_StartTextInput();cva="";change=true;

                    }
                    else if(e.button.y>133&&e.button.y<157&&e.button.x>245&&e.button.x<269){
                        bool k=true;
                        for(int i=4;i<exc[7].blocks.size()&&k;i++){
                            if(exc[7].blocks[i].pn[0]==cva)k=false;
                        }
                        if(k){
                            myval=false;SDL_RenderCopy(renderer,backsave, nullptr, nullptr);
                            SDL_StartTextInput();block pcb;pcb.hofre[0]=pcb.hofre[1]=pcb.u=pcb.d=false;pcb.b=true;pcb.sit=2;
                            pcb.order={70};pcb.pn[0]=cva;pcb.color=color(7);pcb.r.x=22;pcb.r.y=exc[7].blocks.back().r.y+36;pcb.draw(renderer);
                            exc[7].blocks.push_back(pcb);exb[7].blocks.push_back(pcb);
                            SDL_SetRenderTarget(renderer,exc[7].asli);
                            SDL_RenderCopy(renderer,pcb.blu, nullptr,&pcb.r);rectangleColor(renderer,5,pcb.r.y+7,17,pcb.r.y+19,0xff000000);
                            SDL_SetRenderTarget(renderer,exb[7].asli);
                            SDL_RenderCopy(renderer,pcb.blu, nullptr,&pcb.r);rectangleColor(renderer,5,pcb.r.y+7,17,pcb.r.y+19,0xff000000);
                            SDL_SetRenderTarget(renderer,working->texture);exc[7].copy(renderer);cva="";change=true;
                        }
                    }
                }
                else if(m==7&&dellv){
                    change=true;dellv=false;SDL_RenderCopy(renderer,backsave, nullptr, nullptr);
                    if(e.button.y>exc[7].blocks[i7].r.y+93&&e.button.x>110&&e.button.x<149&&e.button.y<exc[7].blocks[i7].r.y+119){
                        exc[7].blocks.erase(exc[7].blocks.begin()+i7);
                        for(int k=i7;k<exc[7].blocks.size();k++)exc[7].blocks[k].r.y-=36;
                        exc[7].draw(renderer,7);
                        SDL_SetRenderTarget(renderer,working->texture);
                        exc[7].copy(renderer);
                        exb[7].blocks.erase(exb[7].blocks.begin()+i7);
                        for(int k=i7;k<exb[7].blocks.size();k++)exb[7].blocks[k].r.y-=36;
                        exb[7].draw(renderer,7);SDL_SetRenderTarget(renderer,working->texture);
                    }
                }
                else if(ty){
                    ty=false;SDL_StopTextInput();SDL_RenderCopy(renderer,typeb->blu, nullptr,&typeb->r);change=true;
                }
                else if(e.button.x<56&&e.button.y<530&&e.button.y>80){
                    int n=(e.button.y-80)/50;change=true;
                    if(m!=n){
                        drawexx(renderer,n,m);m=n;exc[n].copy(renderer);
                        if(m<8&&m!=6&&m!=4&&m!=3)
                            for(const string& i:working->bol[m])
                                for(const block& j:exc[n].blocks)
                                    if(j.b&&j.pn[0]==i)bcl(renderer,61,j.r.y+7,73,j.r.y+19,0xffff0000);
                    }
                }
                else if(e.button.x>56&&e.button.x<306&&e.button.y>80){
                    if(m==7&&!myval&&e.button.y>85&&e.button.x>61&&e.button.x<149&&e.button.y<111){
                        myval=true;SDL_SetRenderTarget(renderer,backsave);
                        SDL_RenderCopy(renderer,working->texture, nullptr, nullptr);
                        SDL_SetRenderTarget(renderer,working->texture);
                        SDL_RenderCopy(renderer,creva,nullptr,&bn);SDL_StartTextInput();change=true;
                    }
                    else{
                        for(const auto& i:exc[m].blocks){
                            if(e.button.x-56>i.r.x&&e.button.x-56<i.r.x+i.r.w&&e.button.y-80>i.r.y&&e.button.y-80<i.r.y+i.r.h&&click(renderer,i,e.button.x-56,e.button.y-80).a>1){
                                current={copyb(renderer,i)};
                                drag=true;xz=e.button.x;yz=e.button.y;current[0]->r.x=i.r.x+56;
                                SDL_SetRenderTarget(renderer,backsave);
                                SDL_RenderCopy(renderer,working->texture, nullptr, nullptr);
                                SDL_SetRenderTarget(renderer,working->texture);change=true;
                            }
                            else if(m<8&&m!=6&&m!=4&&m!=3&&i.b&&e.button.x>61&&e.button.x<73&&e.button.y>i.r.y+87&&e.button.y<i.r.y+99){
                                auto it=find(working->bol[m].begin(),working->bol[m].end(),i.pn[0]);change=true;
                                if(it==working->bol[m].end()){
                                    bcl(renderer,61,i.r.y+7,73,i.r.y+19,0xffff0000);working->bol[m].push_back(i.pn[0]);
                                }
                                else{
                                    bcl(renderer,61,i.r.y+7,73,i.r.y+19,0xffe1f0e1);working->bol[m].erase(it);
                                    rectangleColor(renderer,61,i.r.y+7,73,i.r.y+19,0xff000000);
                                }
                            }
                        }
                    }
                }
                else if(e.button.x>306&&e.button.x<wt-500&&e.button.y>80){
                    o ss=working->get(renderer,e.button.x,e.button.y-80,working->texture);SDL_SetRenderTarget(renderer,working->texture);
                    if(ss.b){
                        drag=true;xz=e.button.x;yz=e.button.y;current=ss.j;
                        SDL_SetRenderTarget(renderer,backsave);
                        SDL_RenderCopy(renderer,working->texture, nullptr, nullptr);
                        SDL_SetRenderTarget(renderer,working->texture);change=true;
                        for(block*i:current){
                            i->r.x+=e.motion.x-xz;i->r.y+=e.motion.y-yz;
                            SDL_RenderCopy(renderer,i->blu, nullptr,&i->r);
                        }
                    }
                    else if(ss.right==1){
                        ty=true;typeb=ss.rib;SDL_StartTextInput();change=true;ts=ss.ss;
                        bcl(renderer,typeb->r.x+typeb->x2v[ts]-6,typeb->r.y,typeb->r.x+typeb->x2v[ts]-3,typeb->r.y+26,0xff000000);
                    }
                }
                else if(e.button.y>50&&e.button.x<216&&e.button.y<80){
                    SDL_SetRenderTarget(renderer,nullptr);msx(renderer,e.button.x, true);SDL_SetRenderTarget(renderer,start.texture);change=true;
                }
            }
            else if(e.button.button==SDL_BUTTON_RIGHT){
                if(m==7&&!dellv){
                    for(int i=5;i<exc[7].blocks.size();i++){
                        if(e.button.x-56>exc[7].blocks[i].r.x&&e.button.x-56<exc[7].blocks[i].r.x+exc[7].blocks[i].r.w&&e.button.y-80>exc[7].blocks[i].r.y&&e.button.y-80<exc[7].blocks[i].r.y+exc[7].blocks[i].r.h&&click(renderer,exc[7].blocks[i],e.button.x-56,e.button.y-80).a>1){
                            SDL_SetRenderTarget(renderer,backsave);dellv=true;
                            SDL_RenderCopy(renderer,working->texture, nullptr, nullptr);
                            SDL_SetRenderTarget(renderer,working->texture);
                            i7=i;bcl(renderer,110,exc[7].blocks[i].r.y+13,149,exc[7].blocks[i].r.y+39,0xffa0a0ff);
                            textRGBA(renderer,113,exc[7].blocks[i].r.y+18,"Delete","tahoma.ttf",12,0,0,0,255);change=true;
                        }
                    }
                }
            }
        }
        else if(e.type==SDL_TEXTINPUT){
            if(myval){
                cva+=e.text.text;bcl(renderer,156,30,262,52,0xffffffff);
                textRGBA(renderer,158,33,cva.c_str(),"tahoma.ttf",12,0,0,0,255);
                int ter=convert(cva,12);bcl(renderer,161+ter,31,164+ter,51,0xff000000);change=true;
            }
            else if(ty){
                typeb->v[ts]+=e.text.text;typeb->draw(renderer);SDL_SetRenderTarget(renderer, working->texture);
                SDL_RenderCopy(renderer,typeb->blu, nullptr,&typeb->r);change=true;
                bcl(renderer,typeb->r.x+typeb->x2v[ts]-6,typeb->r.y,typeb->r.x+typeb->x2v[ts]-3,typeb->r.y+26,0xff000000);
            }
        }
        else if(e.type==SDL_KEYDOWN){
            if(myval){
                if(e.key.keysym.sym==SDLK_BACKSPACE&&!cva.empty()){
                    cva.pop_back();bcl(renderer,156,30,262,52,0xffffffff);
                    if(!cva.empty())textRGBA(renderer,158,33,cva.c_str(),"tahoma.ttf",12,0,0,0,255);
                    int ter=convert(cva,12);bcl(renderer,161+ter,31,164+ter,51,0xff000000);change=true;
                }
            }
            else if(ty){
                if(e.key.keysym.sym==SDLK_BACKSPACE&&!typeb->v[ts].empty()){
                    bcl(renderer,typeb->r.x+50,typeb->r.y,typeb->r.x+typeb->r.w,typeb->r.y+typeb->r.h,0xffffffff);
                    typeb->v[ts].pop_back();typeb->draw(renderer);SDL_SetRenderTarget(renderer, working->texture);change=true;SDL_RenderCopy(renderer,typeb->blu, nullptr,&typeb->r);
                    bcl(renderer,typeb->r.x+typeb->x2v[ts]-6,typeb->r.y,typeb->r.x+typeb->x2v[ts]-3,typeb->r.y+26,0xff000000);
                }
            }
        }
        else if(e.type==SDL_MOUSEBUTTONUP){
            if(drag){
                working->add(renderer,backsave,current,working->texture);
                drag=false;change=true;
            }
        }
        else if(e.type==SDL_MOUSEMOTION){
            if(drag){
                SDL_RenderCopy(renderer,backsave, nullptr, nullptr);
                for(block*i:current){
                    tekan(i,e.motion.x-xz,e.motion.y-yz);SDL_RenderCopy(renderer,i->blu, nullptr,&i->r);
                }
                xz=e.motion.x;yz=e.motion.y;change=true;
            }
        }
        if(change){
            change=false;
            SDL_SetRenderTarget(renderer, nullptr);
            working->copy(renderer);
            SDL_RenderPresent(renderer);
            SDL_SetRenderTarget(renderer, working->texture);
        }
        e.type=0;
    }
    for(auto i:tex){
        SDL_DestroyTexture(i);
    }
    for(auto i:bbl){
        SDL_DestroyTexture(i->blu);
        delete i;
    }
    TTF_Quit();
    SDL_DestroyWindow( window );
    SDL_DestroyRenderer( renderer );
    IMG_Quit();
    SDL_Quit();
    return 0;
}

