
#ifndef SCRATCH_RUN_H
#define SCRATCH_RUN_H
#include "code1.h"
#include "settings.h"
SDL_Texture* run=nullptr;
vector<Character>sp;vector<BackgroundState>co;
struct dxd{
    int i{};Character spx;BackgroundState cox;
};
void rust(){
    run=SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 500, 400);tex.push_back(run);
}
void exebl(int &i,const block*px,dxd& sb){i++;}
bool execute(){
    bool b=false;
    for(int i=0;i<2;i++){
        for(int j=0;j<spco[i].size();j++){
            for(int k=0;k<spco[i][j].exeb.size();k++){
                dxd sx;sx.i=i;if(i==0){sx.spx=sp[j];}else{sx.cox=co[j];}
                if(spco[i][j].exeb[k]){
                    exebl(spco[i][j].exen[k],spco[i][j].data[k][spco[i][j].exen[k]],sx);b=true;
                    if(spco[i][j].exen[k]>spco[i][j].data[k].size()){spco[i][j].exeb[k]=false;spco[i][j].exen[k]=0;}
                }
            }
        }
    }
    return b;
}
bool cligri(int x,int y){
    if(x>wt-535&&x<wt-505&&y>45&&y<75){
        for(int i=0;i<2;i++){
            for(auto j:spco[i]){
                for(int k=0;k<j.data.size();k++){
                    if(j.data[k][0]->pn[0]=="when run clicked"){j.fill();j.exeb[k]=true;}
                }
            }
        }
        return true;
    }
    return false;
}
#endif
