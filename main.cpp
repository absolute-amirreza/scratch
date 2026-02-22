#include "code1.h"
#include "settings.h"
#include "costum.h"
#include "run.h"
#include <ctime>
#include <cstdlib>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    TTF_Init();
    
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    wt = dm.w;
    ht = dm.h - 70;
    
    SDL_Window* window = SDL_CreateWindow("project", 0, 20, wt, ht, SDL_WINDOW_SHOWN);
    renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_TARGETTEXTURE);
    SDL_RaiseWindow(window);
    drawmm( true);
    msx(0, true);
    SDL_RenderPresent(renderer);
    unic();rust();

    SDL_Rect ruc={wt-500,80,500,400};

    ///////

    MONITOR_W = dm.w;
    MONITOR_H = dm.h;

    WINDOW_W = MONITOR_W;
    WINDOW_H = MONITOR_H - 70;

    STAGE_X = MONITOR_W - 500;
    STAGE_Y = 0;
    SETT_PANEL_X = MONITOR_W - 500;
    SETT_PANEL_Y = STAGE_Y + STAGE_H;
    SETT_PANEL_H = MONITOR_H - SETT_PANEL_Y - 70;
    CHAR_PANEL_W = MONITOR_W - 500;
    CHAR_PANEL_Y = 0;

    EDITOR_CANVAS_X = STAGE_X + (WINDOW_W - STAGE_X - EDITOR_CANVAS_W) / 2;
    EDITOR_CANVAS_Y = STAGE_Y + 20;
    EDITOR_CANVAS_RECT = {EDITOR_CANVAS_X, EDITOR_CANVAS_Y, EDITOR_CANVAS_W, EDITOR_CANVAS_H};

    int bgBarW = 40, bgBarH = SETT_PANEL_H, bgBarX = WINDOW_W - bgBarW, bgBarY = SETT_PANEL_Y;
    BG_BAR_RECT = {bgBarX, bgBarY, bgBarW, bgBarH};

    int charBarW = 40, charBarH = SETT_PANEL_H, charBarX = bgBarX - charBarW, charBarY = SETT_PANEL_Y;
    CHAR_BAR_RECT = {charBarX, charBarY, charBarW, charBarH};
    TTF_Font* regular = TTF_OpenFont("assets/fonts/AkzidenzGrotesk-Regular.otf", 16);
    TTF_Font* bold = TTF_OpenFont("assets/fonts/AkzidenzGrotesk-Bold.otf", 18);

    //SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    TTF_Font* font = TTF_OpenFont("assets/fonts/AkzidenzGrotesk-Regular.otf", 14);

    BackgroundState bgState = {};
    BackgroundMenuState bgMenu = {};
    CharacterManager charMgr = {};
    ImageEditorState imgEditor = {};
    CharacterBarState charBar = {};

    initBackgroundMenuState(&bgMenu);
    initCharacterManager(&charMgr);
    initCharacterBarState(&charBar);

    loadDefaultBackgrounds(renderer, &bgState,
                           "assets/backgrounds/abandoned_class.png", "Abandoned Class",
                           "assets/backgrounds/black_forest.png", "Black Forest");

    bool mouseDown = false, mouseJustPressed = false, mouseClicked = false;
    int mouseX = 0, mouseY = 0, dragIndex = -1;
    bool requestAddChar = false, requestRename = false;
    std::string renameBuffer;

///////


    
    exten exc[9];
    exten exb[9];
    
    for(int i = 0; i < 9; i++) {
        exc[i].ecolor = color(i);
        exb[i].ecolor = color(i);
        exc[i].blocks = bclist(i);
        exb[i].blocks = bblist(i);
        exc[i].color();
        exb[i].color();
        
        for(int j = 0; j < exc[i].blocks.size(); j++) {
            exc[i].blocks[j].draw();
            if(j > 0)
                exc[i].blocks[j].r.y = exc[i].blocks[j-1].r.y + exc[i].blocks[j-1].r.h + 10;
            else {
                exc[i].blocks[0].r.y = 5;
                if(i == 7)
                    exc[i].blocks[0].r.y += 36;
            }
            exc[i].blocks[j].r.x = 5 + exc[i].blocks[j].b * 17;
        }
        
        for(int j = 0; j < exb[i].blocks.size(); j++) {
            exb[i].blocks[j].draw();
            if(j > 0)
                exb[i].blocks[j].r.y = exb[i].blocks[j-1].r.y + exb[i].blocks[j-1].r.h + 10;
            else {
                exb[i].blocks[0].r.y = 5;
                if(i == 7)
                    exb[i].blocks[0].r.y += 36;
            }
            exb[i].blocks[j].r.x = 5 + exb[i].blocks[j].b * 17;
        }
        
        exc[i].create();
        exb[i].create();
        exc[i].draw( i);
        exb[i].draw(i);
    }

    codtxs start{};
    spco[0].push_back(start);
    spco[0][0].create(exc[0]);
    spco[0][0].copy();
    SDL_RenderPresent(renderer);
    
    bool drag = false;
    SDL_Texture* backsave = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, wt - 500, ht - 80);
    int m = 0;
    bool aa;
    int xz, yz;
    SDL_Event e;
    tex.push_back(backsave);
    codtxs* working = &spco[0][0];
    vector<block*> current;
    bool myval = false;
    SDL_Texture* creva = nullptr;
    myvc(creva);
    SDL_Rect bn = {149, 5, 120, 72};
    string cva;
    int ivar;
    bool change_c = false,change_e = false;
    int i7;
    bool dellv = false;
    block* typeb = nullptr;
    bool ty = false,mo=false;
    int ts;
    
    while(true) {
        SDL_PollEvent(&e);
        
        if(e.type == SDL_QUIT)
            break;
        else if(e.type == SDL_MOUSEBUTTONDOWN) {
            if(e.button.button == SDL_BUTTON_LEFT) {
                mouseDown = true;
                mouseJustPressed = true;
                mouseX = e.button.x;
                mouseY = e.button.y;
                if(mo){
                    SDL_SetRenderTarget(renderer,working->texture);
                    SDL_RenderCopy(renderer,backsave, nullptr, nullptr);
                    change_c=true;
                    mo = false;
                    if(e.button.x>300&&e.button.x<500&&e.button.y>80&&e.button.y<480){
                        int i=2*((e.button.y-80)/18)+(e.button.x-300)/100;
                        if(i<typeb->m[ts].size()){
                            SDL_Rect cnc={typeb->r.x+50,typeb->r.y,typeb->r.w-50,26};
                            SDL_SetRenderDrawColor(renderer,255,255,255,255);
                            SDL_RenderFillRect(renderer,&cnc);
                            typeb->mod[ts]=typeb->m[ts][i];typeb->draw();
                            SDL_SetRenderTarget(renderer,working->texture);
                            SDL_RenderCopy(renderer,typeb->blu, nullptr,&typeb->r);
                        }
                    }
                }
                else if(m == 7 && myval) {
                    SDL_SetRenderTarget(renderer, working->texture);
                    if(!(e.button.y > 85 && e.button.y < 157 && e.button.x > 149 && e.button.x < 269)) {
                        myval = false;
                        SDL_RenderCopy(renderer, backsave, nullptr, nullptr);
                        SDL_StartTextInput();
                        cva = "";
                        change_c = true;
                    }
                    else if(e.button.y > 133 && e.button.y < 157 && e.button.x > 245 && e.button.x < 269) {
                        bool k = true;
                        for(int i = 4; i < exc[7].blocks.size() && k; i++) {
                            if(exc[7].blocks[i].pn[0] == cva)
                                k = false;
                        }
                        if(k) {
                            myval = false;
                            SDL_RenderCopy(renderer, backsave, nullptr, nullptr);
                            SDL_StartTextInput();
                            block pcb;
                            pcb.hofre[0] = pcb.hofre[1] = pcb.u = pcb.d = false;
                            pcb.b = true;
                            pcb.sit = 2;
                            pcb.order = {70};
                            pcb.pn[0] = cva;
                            pcb.color = color(7);
                            pcb.r.x = 22;
                            pcb.r.y = exc[7].blocks.back().r.y + 36;
                            pcb.draw();
                            exc[7].blocks.push_back(pcb);
                            exb[7].blocks.push_back(pcb);
                            SDL_SetRenderTarget(renderer, exc[7].asli);
                            SDL_RenderCopy(renderer, pcb.blu, nullptr, &pcb.r);
                            rectangleColor(renderer, 5, pcb.r.y + 7, 17, pcb.r.y + 19, 0xff000000);
                            SDL_SetRenderTarget(renderer, exb[7].asli);
                            SDL_RenderCopy(renderer, pcb.blu, nullptr, &pcb.r);
                            rectangleColor(renderer, 5, pcb.r.y + 7, 17, pcb.r.y + 19, 0xff000000);
                            SDL_SetRenderTarget(renderer, working->texture);
                            exc[7].copy();
                            cva = "";
                            change_c = true;
                        }
                    }
                }
                else if(m == 7 && dellv) {
                    change_c = true;
                    dellv = false;
                    SDL_SetRenderTarget(renderer, working->texture);
                    SDL_RenderCopy(renderer, backsave, nullptr, nullptr);
                    if(e.button.y > exc[7].blocks[i7].r.y + 93 && e.button.x > 110 && e.button.x < 149 && e.button.y < exc[7].blocks[i7].r.y + 119) {
                        exc[7].blocks.erase(exc[7].blocks.begin() + i7);
                        for(int k = i7; k < exc[7].blocks.size(); k++)
                            exc[7].blocks[k].r.y -= 36;
                        exc[7].draw( 7);
                        SDL_SetRenderTarget(renderer, working->texture);
                        exc[7].copy();
                        exb[7].blocks.erase(exb[7].blocks.begin() + i7);
                        for(int k = i7; k < exb[7].blocks.size(); k++)
                            exb[7].blocks[k].r.y -= 36;
                        exb[7].draw( 7);
                        SDL_SetRenderTarget(renderer, working->texture);
                    }
                }
                else if(ty) {
                    SDL_SetRenderTarget(renderer, working->texture);
                    ty = false;
                    SDL_StopTextInput();
                    SDL_RenderCopy(renderer, typeb->blu, nullptr, &typeb->r);
                    change_c = true;
                }
                else if(e.button.x < 56 && e.button.y < 530 && e.button.y > 80) {
                    int n = (e.button.y - 80) / 50;
                    if(m != n) {
                        SDL_SetRenderTarget(renderer, working->texture);
                        change_c = true;
                        drawexx(n, m);
                        m = n;
                        exc[n].copy();
                        if(m < 8 && m != 6 && m != 4 && m != 3)
                            for(const string& i : working->bol[m])
                                for(const block& j : exc[n].blocks)
                                    if(j.b && j.pn[0] == i)
                                        bcl( 61, j.r.y + 7, 73, j.r.y + 19, 0xffff0000);
                    }
                }
                else if(e.button.x > 56 && e.button.x < 306 && e.button.y > 80) {
                    SDL_SetRenderTarget(renderer, working->texture);
                    if(m == 7 && !myval && e.button.y > 85 && e.button.x > 61 && e.button.x < 149 && e.button.y < 111) {
                        myval = true;
                        SDL_SetRenderTarget(renderer, backsave);
                        SDL_RenderCopy(renderer, working->texture, nullptr, nullptr);
                        SDL_SetRenderTarget(renderer, working->texture);
                        SDL_RenderCopy(renderer, creva, nullptr, &bn);
                        SDL_StartTextInput();
                        change_c = true;
                    }
                    else {
                        for(const auto& i : exc[m].blocks) {
                            if(e.button.x - 56 > i.r.x && e.button.x - 56 < i.r.x + i.r.w && e.button.y - 80 > i.r.y && e.button.y - 80 < i.r.y + i.r.h && click(i, e.button.x - 56, e.button.y - 80).a > 1) {
                                current = {copyb(i)};
                                drag = true;
                                xz = e.button.x;
                                yz = e.button.y;
                                current[0]->r.x = i.r.x + 56;
                                SDL_SetRenderTarget(renderer, backsave);
                                SDL_RenderCopy(renderer, working->texture, nullptr, nullptr);
                                SDL_SetRenderTarget(renderer, working->texture);
                                change_c = true;
                            }
                            else if(m < 8 && m != 6 && m != 4 && m != 3 && i.b && e.button.x > 61 && e.button.x < 73 && e.button.y > i.r.y + 87 && e.button.y < i.r.y + 99) {
                                auto it = find(working->bol[m].begin(), working->bol[m].end(), i.pn[0]);
                                change_c = true;
                                if(it == working->bol[m].end()) {
                                    bcl(61, i.r.y + 7, 73, i.r.y + 19, 0xffff0000);
                                    working->bol[m].push_back(i.pn[0]);
                                }
                                else {
                                    bcl(61, i.r.y + 7, 73, i.r.y + 19, 0xffe1f0e1);
                                    working->bol[m].erase(it);
                                    rectangleColor(renderer, 61, i.r.y + 7, 73, i.r.y + 19, 0xff000000);
                                }
                            }
                        }
                    }
                }
                else if(e.button.x > 306 && e.button.x < wt - 500 && e.button.y > 80) {
                    SDL_SetRenderTarget(renderer, working->texture);
                    o ss = working->get(e.button.x, e.button.y - 80, working->texture);
                    SDL_SetRenderTarget(renderer, working->texture);
                    if(ss.b) {
                        drag = true;
                        xz = e.button.x;
                        yz = e.button.y;
                        current = ss.j;
                        SDL_SetRenderTarget(renderer, backsave);
                        SDL_RenderCopy(renderer, working->texture, nullptr, nullptr);
                        SDL_SetRenderTarget(renderer, working->texture);
                        change_c= true;
                        for(block* i : current) {
                            i->r.x += e.motion.x - xz;
                            i->r.y += e.motion.y - yz;
                            SDL_RenderCopy(renderer, i->blu, nullptr, &i->r);
                        }
                    }
                    else if(ss.right == 1) {
                        ty = true;
                        typeb = ss.rib;
                        SDL_StartTextInput();
                        change_c = true;
                        ts = ss.ss;
                        bcl(typeb->r.x + typeb->x2v[ts] - 6, typeb->r.y, typeb->r.x + typeb->x2v[ts] - 3, typeb->r.y + 26, 0xff000000);
                    }
                    else if(ss.right ==2) {
                        mo = true;
                        typeb = ss.rib;
                        change_c = true;
                        ts = ss.ss;
                        SDL_SetRenderTarget(renderer,backsave);
                        SDL_RenderCopy(renderer,working->texture, nullptr, nullptr);
                        SDL_SetRenderTarget(renderer,working->texture);
                        bcl(300,0,500,400,typeb->color);
                        for(int i=0;i<typeb->m[ts].size();i++){
                            textRGBA(renderer,i%2==0?300:400,2+(i/2)*18,typeb->m[ts][i].c_str(),"tahoma.ttf",12,0,0,0,255);
                        }
                    }
                }
                else if(e.button.y > 50 && e.button.y < 80) {
                    if(e.button.x<216){
                        SDL_SetRenderTarget(renderer, nullptr);
                        msx( e.button.x, true);
                    }
                    else if(cligri(e.button.x,e.button.y))1;
                }
            }
            else if(e.button.button == SDL_BUTTON_RIGHT) {
                if(m == 7 && !dellv) {
                    SDL_SetRenderTarget(renderer, working->texture);
                    for(int i = 5; i < exc[7].blocks.size(); i++) {
                        if(e.button.x - 56 > exc[7].blocks[i].r.x && e.button.x - 56 < exc[7].blocks[i].r.x + exc[7].blocks[i].r.w && e.button.y - 80 > exc[7].blocks[i].r.y && e.button.y - 80 < exc[7].blocks[i].r.y + exc[7].blocks[i].r.h && click(exc[7].blocks[i], e.button.x - 56, e.button.y - 80).a > 1) {
                            SDL_SetRenderTarget(renderer, backsave);
                            dellv = true;
                            SDL_RenderCopy(renderer, working->texture, nullptr, nullptr);
                            SDL_SetRenderTarget(renderer, working->texture);
                            i7 = i;
                            bcl( 110, exc[7].blocks[i].r.y + 13, 149, exc[7].blocks[i].r.y + 39, 0xffa0a0ff);
                            textRGBA(renderer, 113, exc[7].blocks[i].r.y + 18, "Delete", "tahoma.ttf", 12, 0, 0, 0, 255);
                            change_c = true;
                        }
                    }
                }
            }
        }
        else if(e.type == SDL_TEXTINPUT) {
            if(myval) {
                SDL_SetRenderTarget(renderer, working->texture);
                cva += e.text.text;
                bcl(156, 30, 262, 52, 0xffffffff);
                textRGBA(renderer, 158, 33, cva.c_str(), "tahoma.ttf", 12, 0, 0, 0, 255);
                int ter = convert(cva, 12);
                bcl( 161 + ter, 31, 164 + ter, 51, 0xff000000);
                change_c = true;
            }
            else if(ty) {
                SDL_SetRenderTarget(renderer, working->texture);
                typeb->v[ts] += e.text.text;
                typeb->draw();
                SDL_SetRenderTarget(renderer, working->texture);
                SDL_RenderCopy(renderer, typeb->blu, nullptr, &typeb->r);
                change_c = true;
                bcl( typeb->r.x + typeb->x2v[ts] - 6, typeb->r.y, typeb->r.x + typeb->x2v[ts] - 3, typeb->r.y + 26, 0xff000000);
            }
        }
        else if(e.type == SDL_KEYDOWN) {
            if(myval) {
                SDL_SetRenderTarget(renderer, working->texture);
                if(e.key.keysym.sym == SDLK_BACKSPACE && !cva.empty()) {
                    cva.pop_back();
                    bcl(156, 30, 262, 52, 0xffffffff);
                    if(!cva.empty())
                        textRGBA(renderer, 158, 33, cva.c_str(), "tahoma.ttf", 12, 0, 0, 0, 255);
                    int ter = convert(cva, 12);
                    bcl(161 + ter, 31, 164 + ter, 51, 0xff000000);
                    change_c = true;
                }
            }
            else if(ty) {
                if(e.key.keysym.sym == SDLK_BACKSPACE && !typeb->v[ts].empty()) {
                    SDL_SetRenderTarget(renderer, working->texture);
                    bcl(typeb->r.x + 50, typeb->r.y, typeb->r.x + typeb->r.w, typeb->r.y + typeb->r.h, 0xffffffff);
                    typeb->v[ts].pop_back();
                    typeb->draw();
                    SDL_SetRenderTarget(renderer, working->texture);
                    change_c = true;
                    SDL_RenderCopy(renderer, typeb->blu, nullptr, &typeb->r);
                    bcl(typeb->r.x + typeb->x2v[ts] - 6, typeb->r.y, typeb->r.x + typeb->x2v[ts] - 3, typeb->r.y + 26, 0xff000000);
                }
            }
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                bgMenu.showingLibrary = false;
                bgMenu.showingEditor = false;
                charBar.panelOpen = false;
                if (imgEditor.active) {
                    imgEditor.confirmed = false;
                    imgEditor.active = false;
                }
            }
        }
        else if(e.type == SDL_MOUSEBUTTONUP) {
            mouseClicked = mouseDown;
            mouseDown = false;
            mouseX = e.button.x;
            mouseY = e.button.y;
            if(drag) {
                SDL_SetRenderTarget(renderer, working->texture);
                working->add( backsave, current, working->texture);
                drag = false;
                change_c = true;
            }
        }
        else if(e.type == SDL_MOUSEMOTION) {
            mouseX = e.motion.x;
            mouseY = e.motion.y;
            if(drag) {
                SDL_SetRenderTarget(renderer, working->texture);
                SDL_RenderCopy(renderer, backsave, nullptr, nullptr);
                for(block* i : current) {
                    tekan(i, e.motion.x - xz, e.motion.y - yz);
                    SDL_RenderCopy(renderer, i->blu, nullptr, &i->r);
                }
                xz = e.motion.x;
                yz = e.motion.y;
                change_c = true;
            }
        }
        if(execute()){
            change_e=true;
            SDL_SetRenderTarget(renderer,run);
        }
        if(change_c) {
            change_c = false;
            SDL_SetRenderTarget(renderer, nullptr);
            working->copy();
            SDL_RenderPresent(renderer);
        }
        else if(change_e) {
            change_e = false;
            SDL_SetRenderTarget(renderer, nullptr);
            SDL_RenderCopy(renderer,run, nullptr,&ruc);
            SDL_RenderPresent(renderer);
        }

        if (imgEditor.active) handleImageEditorMouse(renderer, &imgEditor, &e, EDITOR_CANVAS_RECT);
        if (!imgEditor.active) {
            if (mouseX >= STAGE_X && mouseX < STAGE_X + STAGE_W && mouseY >= STAGE_Y && mouseY < STAGE_Y + STAGE_H) {
                handleCharacterDrag(&charMgr, mouseX, mouseY, mouseDown, mouseJustPressed, &dragIndex);
            }
        }
        if (mouseClicked && !imgEditor.active) {
            int bgAction = handleBackgroundBarClick(&bgMenu, BG_BAR_RECT, mouseX, mouseY);
            if (bgAction == 2) {
                loadBackgroundFromSystem(renderer, &bgState, WINDOW_W, WINDOW_H);
                bgMenu.menuOpen = false;
            }
            if (bgAction == 3) {
                initImageEditor(renderer, &imgEditor, EDITOR_CANVAS_W, EDITOR_CANVAS_H);
                bgMenu.menuOpen = false;
            }

            handleCharacterBarClick(&charBar, CHAR_BAR_RECT, mouseX, mouseY);
        }
        if (requestAddChar) {
            requestAddChar = false;
            int choice = MessageBoxA(nullptr,
                                     "Choose how to add a character:\n\nYES  = Load from system (file dialog)\nNO   = Add an orange square",
                                     "Add Character", MB_YESNOCANCEL | MB_ICONQUESTION);
            if (choice == IDYES) {
                int idx = addCharacterFromSystem(renderer, &charMgr);
                if (idx >= 0) {
                    setActiveCharacter(&charMgr, idx);
                    Character& c = charMgr.characters[idx];
                    c.x = STAGE_X + (STAGE_W - c.width)  / 2;
                    c.y = STAGE_Y + (STAGE_H - c.height) / 2;
                }
            }
            else if (choice == IDNO) {
                int idx = addOrangeSquareCharacter(renderer, &charMgr);
                if (idx >= 0) {
                    setActiveCharacter(&charMgr, idx);
                    Character& c = charMgr.characters[idx];
                    c.x = STAGE_X + (STAGE_W - c.width)  / 2;
                    c.y = STAGE_Y + (STAGE_H - c.height) / 2;
                }
            }
        }
        if (requestRename && charMgr.activeIndex >= 0) {
            requestRename = false;
            if (simpleTextInputDialog(renderer, font, "Enter new name:", renameBuffer)) {
                renameCharacter(&charMgr, charMgr.activeIndex, renameBuffer);
            }
        }
        if (!imgEditor.active && imgEditor.confirmed) {
            applyEditorCanvasAsBackground(renderer, &bgState, &imgEditor, WINDOW_W, WINDOW_H);
        }
        if (charBar.panelOpen) {
            int popX = CHAR_BAR_RECT.x - 200;
            int popY = CHAR_BAR_RECT.y;
            renderCharacterBarPanel(renderer, font, &charMgr, popX, popY, mouseX, mouseY, mouseClicked);
        }
        if (bgMenu.showingLibrary) {
            int libX = BG_BAR_RECT.x - 369, libY = BG_BAR_RECT.y + 10 + 32 + 32;
            int sel = renderDefaultBackgroundLibrary(renderer, font, &bgState, libX, libY, mouseX, mouseY, mouseClicked, WINDOW_W, WINDOW_H);
            if (sel >= 0) {
                bgMenu.showingLibrary = false;
                bgMenu.menuOpen = false;
            }
        }
        if (imgEditor.active) {
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
            SDL_Rect full = {0, 0, WINDOW_W, WINDOW_H};
            SDL_RenderFillRect(renderer, &full);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

            renderImageEditor(renderer, font, &imgEditor, EDITOR_CANVAS_RECT, mouseX, mouseY, mouseClicked);
        }

        mouseClicked = false;
        mouseJustPressed = false;
        e.type = 0;
    }
    
    for(auto i : tex) {
        SDL_DestroyTexture(i);
    }
    for(auto i : bbl) {
        SDL_DestroyTexture(i->blu);
        delete i;
    }


    destroyCharacterManager(&charMgr);
    destroyBackgroundState(&bgState);
    if (imgEditor.canvas) SDL_DestroyTexture(imgEditor.canvas);
    if(regular) TTF_CloseFont(regular);
    if(bold) TTF_CloseFont(bold);if(font) TTF_CloseFont(font);


    TTF_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    IMG_Quit();
    SDL_Quit();
    
    return 0;
}
