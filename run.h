
#ifndef SCRATCH_RUN_H
#define SCRATCH_RUN_H
#include "code1.h"
#include "settings.h"
vector<Sprite>sp;vector<Background>co;
struct dxd{
    int i{};Sprite sp;Background co;
};
void exebl(SDL_Renderer*renderer,int &i,const block*px,dxd& sb){i++;}
bool execute(SDL_Renderer*renderer){
    bool b=false;
    for(int i=0;i<2;i++){
        for(int j=0;j<spco[i].size();j++){
            for(int k=0;k<spco[i][j].exeb.size();k++){
                dxd sx;sx.i=i;if(i==0){sx.sp=sp[j];}else{sx.co=co[j];}
                if(spco[i][j].exeb[k]){
                    exebl(renderer,spco[i][j].exen[k],spco[i][j].data[k][spco[i][j].exen[k]],sx);b=true;
                    if(spco[i][j].exen[k]>spco[i][j].data[k].size()){spco[i][j].exeb[k]=false;spco[i][j].exen[k]=0;}
                }
            }
        }
    }
    return b;
}
bool cligri(int x,int y){
    if(x>wt-495&&x<wt-465&&y>45&&y<75){
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
