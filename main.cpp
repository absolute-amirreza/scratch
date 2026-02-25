#include "run.h"
#include "settings.h"
#include "costum.h"
#include <ctime>
#include <cstdlib>

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    TTF_Init();
    srand(static_cast<unsigned>(time(nullptr)));

    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    wt = dm.w;
    ht = dm.h - 70;

    MONITOR_W = dm.w;
    MONITOR_H = dm.h;
    WINDOW_W  = MONITOR_W;
    WINDOW_H  = MONITOR_H - 70;

    STAGE_W      = 500;
    STAGE_H      = 400;
    STAGE_X       = MONITOR_W - 500;
    STAGE_Y       = 80;
    SETT_PANEL_X  = MONITOR_W - 500;
    SETT_PANEL_Y  = STAGE_Y + STAGE_H;
    SETT_PANEL_W  = 500;
    SETT_PANEL_H  = WINDOW_H - SETT_PANEL_Y;

    EDITOR_CANVAS_W = 760;
    EDITOR_CANVAS_H = 480;
    EDITOR_CANVAS_X = STAGE_X + (WINDOW_W - STAGE_X - EDITOR_CANVAS_W) / 2;
    EDITOR_CANVAS_Y = STAGE_Y + 20;
    EDITOR_CANVAS_RECT = {EDITOR_CANVAS_X, EDITOR_CANVAS_Y, EDITOR_CANVAS_W, EDITOR_CANVAS_H};

    int bgBarW = 40, bgBarH = SETT_PANEL_H, bgBarX = WINDOW_W - bgBarW, bgBarY = SETT_PANEL_Y;
    BG_BAR_RECT = {bgBarX, bgBarY, bgBarW, bgBarH};

    int charBarW = 40, charBarH = SETT_PANEL_H, charBarX = bgBarX - charBarW, charBarY = SETT_PANEL_Y;
    CHAR_BAR_RECT = {charBarX, charBarY, charBarW, charBarH};

    SDL_Window* window = SDL_CreateWindow("project", 0, 20, wt, ht, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1,
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
    SDL_RaiseWindow(window);
    drawmm(true);
    msx(0, true);
    SDL_RenderPresent(renderer);
    unic();
    rust();

    TTF_Font* font    = TTF_OpenFont("assets/fonts/AkzidenzGrotesk-Regular.otf", 14);
    if (!font)  font  = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 14);
    TTF_Font* regular = TTF_OpenFont("assets/fonts/AkzidenzGrotesk-Regular.otf", 16);
    TTF_Font* bold    = TTF_OpenFont("assets/fonts/AkzidenzGrotesk-Bold.otf", 18);

    BackgroundState     bgState   = {};
    BackgroundMenuState bgMenu    = {};
    CharacterManager    charMgr   = {};
    ImageEditorState    imgEditor = {};
    CharacterBarState   charBar   = {};
    CostumesMenuState   costumesMenu = {};
    InlineCostumesState inlineCos    = {};

    initBackgroundMenuState(&bgMenu);
    initCharacterManager(&charMgr);
    initCharacterBarState(&charBar);
    initCostumesMenu(&costumesMenu);
    initInlineCostumes(&inlineCos, renderer, wt - 500, ht - 80);

    g_charMgr = &charMgr;
    g_bgState  = &bgState;

    SDL_SetRenderTarget(renderer, nullptr);

    loadDefaultBackgrounds(&bgState,
                           "assets/backgrounds/abandoned_class.png", "Abandoned Class",
                           "assets/backgrounds/black_forest.png",    "Black Forest");

    exten exc[9];
    exten exb[9];

    for (int i = 0; i < 9; i++)
    {
        exc[i].ecolor  = color(i);
        exb[i].ecolor  = color(i);
        exc[i].blocks  = bclist(i);
        exb[i].blocks  = bblist(i);
        exc[i].color();
        exb[i].color();

        for (int j = 0; j < (int)exc[i].blocks.size(); j++)
        {
            exc[i].blocks[j].draw();
            if (j > 0)
                exc[i].blocks[j].r.y = exc[i].blocks[j-1].r.y + exc[i].blocks[j-1].r.h + 10;
            else
            {
                exc[i].blocks[0].r.y = 5;
                if (i == 7) exc[i].blocks[0].r.y += 36;
            }
            exc[i].blocks[j].r.x = 5 + exc[i].blocks[j].b * 17;
        }

        for (int j = 0; j < (int)exb[i].blocks.size(); j++)
        {
            exb[i].blocks[j].draw();
            if (j > 0)
                exb[i].blocks[j].r.y = exb[i].blocks[j-1].r.y + exb[i].blocks[j-1].r.h + 10;
            else
            {
                exb[i].blocks[0].r.y = 5;
                if (i == 7) exb[i].blocks[0].r.y += 36;
            }
            exb[i].blocks[j].r.x = 5 + exb[i].blocks[j].b * 17;
        }

        exc[i].create();
        exb[i].create();
        exc[i].draw(i);
        exb[i].draw(i);
    }

    codtxs startNode{};
    spco[0].reserve(MAX_CHARACTERS);
    spco[1].reserve(4);
    spco[0].push_back(startNode);
    spco[0][0].create(exc[0]);

    {
        codtxs bgNode{};
        spco[1].push_back(bgNode);
        spco[1][0].create(exb[0]);
    }

    spco[0][0].copy();
    SDL_RenderPresent(renderer);

    {
        int defIdx = addOrangeSquareCharacter(&charMgr);
        if (defIdx >= 0)
        {
            setActiveCharacter(&charMgr, defIdx);
            Character& c = charMgr.characters[defIdx];
            c.x = STAGE_X + (STAGE_W - c.width)  / 2;
            c.y = STAGE_Y + (STAGE_H - c.height) / 2;
        }
    }

    SDL_Texture* backsave = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                              SDL_TEXTUREACCESS_TARGET, wt - 500, ht - 80);
    tex.push_back(backsave);

    SDL_Rect ruc         = {wt - 500, 80, 500, 400};
    SDL_Rect stageClip   = {STAGE_X, STAGE_Y, STAGE_W, STAGE_H};
    SDL_Rect settRect    = {SETT_PANEL_X, SETT_PANEL_Y, SETT_PANEL_W, SETT_PANEL_H};
    SDL_Rect stageBorder = {STAGE_X - 1, STAGE_Y - 1, STAGE_W + 2, STAGE_H + 2};

    SDL_Texture* creva = nullptr;
    myvc(creva);
    SDL_Rect bn = {149, 5, 120, 72};

    codtxs*        working = &spco[0][0];
    vector<block*> current;
    block*         typeb   = nullptr;

    bool isSprite     = true;

    int  prevActiveIdx = 0;

    bool drag             = false;
    bool myval            = false;
    bool dellv            = false;
    bool ty               = false;
    bool mo               = false;
    bool change_c         = false;
    bool change_e         = false;
    bool mouseDown        = false;
    bool mouseJustPressed = false;
    bool mouseClicked     = false;
    bool requestAddChar   = false;
    bool requestRename    = false;
    bool running          = true;

    bool editorWasActive  = false;

    int currentTab = 0;

    int m         = 0;
    int xz        = 0, yz    = 0;
    int i7        = 0, ts    = 0;
    int mouseX    = 0, mouseY = 0;
    int dragIndex = -1;

    string cva;
    string renameBuffer;
    SDL_Event e;

    while (running)
    {
        mouseClicked      = false;
        mouseJustPressed  = false;

        while (SDL_PollEvent(&e))
        {
            if (costumesMenu.active)
            {
                if (e.type == SDL_QUIT) { running = false; break; }
                if (e.type == SDL_MOUSEMOTION)
                { mouseX = e.motion.x; mouseY = e.motion.y; }
                if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT)
                { mouseClicked = mouseDown; mouseDown = false; }
                if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
                { mouseDown = true; mouseX = e.button.x; mouseY = e.button.y; }
                cosHandleEvent(&costumesMenu, renderer, font, &e, &charMgr);
                continue;
            }

            if (imgEditor.active)
                handleImageEditorMouse(&imgEditor, &e, EDITOR_CANVAS_RECT);

            switch (e.type)
            {
                case SDL_QUIT:
                    running = false;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (e.button.button == SDL_BUTTON_LEFT)
                    {
                        mouseDown        = true;
                        mouseJustPressed = true;
                        mouseX           = e.button.x;
                        mouseY           = e.button.y;

                        if (mo)
                        {
                            SDL_SetRenderTarget(renderer, working->texture);
                            SDL_RenderCopy(renderer, backsave, nullptr, nullptr);
                            change_c = true;
                            mo = false;
                            if (e.button.x > 300 && e.button.x < 500 &&
                                e.button.y > 80  && e.button.y < 480)
                            {
                                int idx = 2 * ((e.button.y - 80) / 18) + (e.button.x - 300) / 100;
                                if (idx < (int)typeb->m[ts].size())
                                {
                                    SDL_Rect cnc = {typeb->r.x + 50, typeb->r.y, typeb->r.w - 50, 26};
                                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                                    SDL_RenderFillRect(renderer, &cnc);
                                    typeb->mod[ts] = typeb->m[ts][idx];
                                    typeb->draw();
                                    SDL_SetRenderTarget(renderer, working->texture);
                                    SDL_RenderCopy(renderer, typeb->blu, nullptr, &typeb->r);
                                }
                            }
                        }
                        else if (m == 7 && myval)
                        {
                            SDL_SetRenderTarget(renderer, working->texture);
                            if (!(e.button.y > 85  && e.button.y < 157 &&
                                  e.button.x > 149 && e.button.x < 269))
                            {
                                myval = false;
                                SDL_RenderCopy(renderer, backsave, nullptr, nullptr);
                                SDL_StartTextInput();
                                cva      = "";
                                change_c = true;
                            }
                            else if (e.button.y > 133 && e.button.y < 157 &&
                                     e.button.x > 245 && e.button.x < 269)
                            {
                                bool nameUnique = true;
                                for (int i = 4; i < (int)exc[7].blocks.size() && nameUnique; i++)
                                    if (exc[7].blocks[i].pn[0] == cva) nameUnique = false;

                                if (nameUnique)
                                {
                                    myval = false;
                                    SDL_RenderCopy(renderer, backsave, nullptr, nullptr);
                                    SDL_StartTextInput();

                                    block pcb;
                                    pcb.hofre[0] = pcb.hofre[1] = pcb.u = pcb.d = false;
                                    pcb.b     = true;
                                    pcb.sit   = 2;
                                    pcb.order = {70};
                                    pcb.pn[0] = cva;
                                    pcb.color = color(7);
                                    pcb.r.x   = 22;
                                    pcb.r.y   = exc[7].blocks.back().r.y + 36;
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
                                    cva      = "";
                                    change_c = true;
                                }
                            }
                        }
                        else if (m == 7 && dellv)
                        {
                            change_c = true;
                            dellv    = false;
                            SDL_SetRenderTarget(renderer, working->texture);
                            SDL_RenderCopy(renderer, backsave, nullptr, nullptr);
                            if (e.button.y > exc[7].blocks[i7].r.y + 93 &&
                                e.button.x > 110 && e.button.x < 149 &&
                                e.button.y < exc[7].blocks[i7].r.y + 119)
                            {
                                exc[7].blocks.erase(exc[7].blocks.begin() + i7);
                                for (int k = i7; k < (int)exc[7].blocks.size(); k++)
                                    exc[7].blocks[k].r.y -= 36;
                                exc[7].draw(7);
                                SDL_SetRenderTarget(renderer, working->texture);
                                exc[7].copy();

                                exb[7].blocks.erase(exb[7].blocks.begin() + i7);
                                for (int k = i7; k < (int)exb[7].blocks.size(); k++)
                                    exb[7].blocks[k].r.y -= 36;
                                exb[7].draw(7);
                                SDL_SetRenderTarget(renderer, working->texture);
                            }
                        }
                        else if (ty)
                        {
                            SDL_SetRenderTarget(renderer, working->texture);
                            ty = false;
                            SDL_StopTextInput();
                            if (typeb)
                                SDL_RenderCopy(renderer, typeb->blu, nullptr, &typeb->r);
                            typeb    = nullptr;
                            change_c = true;
                        }
                        else if (e.button.x < 56 && e.button.y < 530 && e.button.y > 80)
                        {
                            if (currentTab == 0)
                            {
                                int n = (e.button.y - 80) / 50;
                                if (m != n)
                                {
                                    SDL_SetRenderTarget(renderer, working->texture);
                                    change_c = true;
                                    drawexx(n, m);
                                    m = n;
                                    SDL_SetRenderTarget(renderer, working->texture);
                                    exc[n].copy();
                                    if (m < 8 && m != 6 && m != 4 && m != 3)
                                        for (const string& sv : working->bol[m])
                                            for (const block& bv : exc[n].blocks)
                                                if (bv.b && bv.pn[0] == sv)
                                                    bcl(61, bv.r.y + 7, 73, bv.r.y + 19, 0xffff0000);
                                }
                            }
                        }
                        else if (e.button.x > 56 && e.button.x < 306 && e.button.y > 80)
                        {
                            if (currentTab == 0)
                            {
                                SDL_SetRenderTarget(renderer, working->texture);
                                if (m == 7 && !myval &&
                                    e.button.y > 85 && e.button.x > 61 &&
                                    e.button.x < 149 && e.button.y < 111)
                                {
                                    myval = true;
                                    SDL_SetRenderTarget(renderer, backsave);
                                    SDL_RenderCopy(renderer, working->texture, nullptr, nullptr);
                                    SDL_SetRenderTarget(renderer, working->texture);
                                    SDL_RenderCopy(renderer, creva, nullptr, &bn);
                                    SDL_StartTextInput();
                                    change_c = true;
                                }
                                else
                                {
                                    for (const auto& bv : exc[m].blocks)
                                    {
                                        if (e.button.x - 56 > bv.r.x &&
                                            e.button.x - 56 < bv.r.x + bv.r.w &&
                                            e.button.y - 80 > bv.r.y &&
                                            e.button.y - 80 < bv.r.y + bv.r.h &&
                                            click(bv, e.button.x - 56, e.button.y - 80).a > 1)
                                        {
                                            current         = {copyb(bv)};
                                            drag            = true;
                                            xz              = e.button.x;
                                            yz              = e.button.y;
                                            current[0]->r.x = bv.r.x + 56;
                                            SDL_SetRenderTarget(renderer, backsave);
                                            SDL_RenderCopy(renderer, working->texture, nullptr, nullptr);
                                            SDL_SetRenderTarget(renderer, working->texture);
                                            change_c = true;
                                        }
                                        else if (m < 8 && m != 6 && m != 4 && m != 3 &&
                                                 bv.b &&
                                                 e.button.x > 61 && e.button.x < 73 &&
                                                 e.button.y > bv.r.y + 87 &&
                                                 e.button.y < bv.r.y + 99)
                                        {
                                            auto it = find(working->bol[m].begin(), working->bol[m].end(), bv.pn[0]);
                                            change_c = true;
                                            if (it == working->bol[m].end())
                                            {
                                                bcl(61, bv.r.y + 7, 73, bv.r.y + 19, 0xffff0000);
                                                working->bol[m].push_back(bv.pn[0]);
                                            }
                                            else
                                            {
                                                bcl(61, bv.r.y + 7, 73, bv.r.y + 19, 0xffe1f0e1);
                                                working->bol[m].erase(it);
                                                rectangleColor(renderer, 61, bv.r.y + 7, 73, bv.r.y + 19, 0xff000000);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else if (e.button.x > 306 && e.button.x < wt - 500 && e.button.y > 80)
                        {
                            if (currentTab == 1)
                            {
                                handleInlineCostumesEvent(&inlineCos, renderer, font, &e, &charMgr);
                            }
                            else if (currentTab == 0)
                            {
                                SDL_SetRenderTarget(renderer, working->texture);
                                o ss = working->get(e.button.x, e.button.y - 80, working->texture);
                                SDL_SetRenderTarget(renderer, working->texture);

                                if (ss.b)
                                {
                                    drag    = true;
                                    xz      = e.button.x;
                                    yz      = e.button.y;
                                    current = ss.j;
                                    SDL_SetRenderTarget(renderer, backsave);
                                    SDL_RenderCopy(renderer, working->texture, nullptr, nullptr);
                                    SDL_SetRenderTarget(renderer, working->texture);
                                    change_c = true;
                                    for (block* bv : current)
                                    {
                                        bv->r.x += e.motion.x - xz;
                                        bv->r.y += e.motion.y - yz;
                                        SDL_RenderCopy(renderer, bv->blu, nullptr, &bv->r);
                                    }
                                }
                                else if (ss.right == 1)
                                {
                                    ty       = true;
                                    typeb    = ss.rib;
                                    ts       = ss.ss;
                                    change_c = true;
                                    SDL_StartTextInput();
                                    bcl(typeb->r.x + typeb->x2v[ts] - 6, typeb->r.y,
                                        typeb->r.x + typeb->x2v[ts] - 3, typeb->r.y + 26, 0xff000000);
                                }
                                else if (ss.right == 2)
                                {
                                    mo       = true;
                                    typeb    = ss.rib;
                                    ts       = ss.ss;
                                    change_c = true;
                                    SDL_SetRenderTarget(renderer, backsave);
                                    SDL_RenderCopy(renderer, working->texture, nullptr, nullptr);
                                    SDL_SetRenderTarget(renderer, working->texture);
                                    bcl(300, 0, 500, 400, typeb->color);
                                    for (int i = 0; i < (int)typeb->m[ts].size(); i++)
                                        textRGBA(renderer, i % 2 == 0 ? 300 : 400, 2 + (i / 2) * 18,
                                                 typeb->m[ts][i].c_str(), "tahoma.ttf", 12, 0, 0, 0, 255);
                                }
                            }
                        }
                        else if (e.button.y > 50 && e.button.y < 80)
                        {
                            if (e.button.x < 216)
                            {
                                SDL_SetRenderTarget(renderer, nullptr);
                                int newTab = msx(e.button.x, isSprite);
                                if (newTab != currentTab)
                                {
                                    if (currentTab == 1)
                                    {
                                        if (inlineCos.textMode)
                                        { inlineCos.textMode = false; inlineCos.textBuf.clear(); SDL_StopTextInput(); }
                                        if (inlineCos.renamingCos)
                                        { inlineCos.renamingCos = false; SDL_StopTextInput(); }
                                        inlineCos.isDrawing = false;
                                        change_c = true;
                                    }
                                    if (newTab == 1)
                                    {
                                        if (drag)
                                        {
                                            SDL_SetRenderTarget(renderer, working->texture);
                                            SDL_RenderCopy(renderer, backsave, nullptr, nullptr);
                                            SDL_SetRenderTarget(renderer, nullptr);
                                            current.clear();
                                            drag = false;
                                        }
                                        inlineCosLoadOwner(&inlineCos, renderer, charMgr.activeIndex);
                                        change_c = true;
                                    }
                                    currentTab = newTab;
                                }
                            }
                            else if (cligri(e.button.x, e.button.y) ||
                                     clirec(e.button.x, e.button.y)) {}
                        }

                        else if (e.button.y >= STAGE_Y && e.button.y < STAGE_Y + STAGE_H &&
                                 e.button.x >= STAGE_X && e.button.x < STAGE_X + STAGE_W)
                        {
                            for (int ci = 0; ci < charMgr.count; ci++) {
                                Character& cc = charMgr.characters[ci];
                                if (!cc.visible) continue;
                                if (e.button.x >= cc.x && e.button.x < cc.x + cc.width &&
                                    e.button.y >= cc.y && e.button.y < cc.y + cc.height) {
                                    notifySpriteClicked(ci);
                                    break;
                                }
                            }
                        }

                    }
                    else if (e.button.button == SDL_BUTTON_RIGHT)
                    {
                        if (m == 7 && !dellv)
                        {
                            SDL_SetRenderTarget(renderer, working->texture);
                            for (int i = 5; i < (int)exc[7].blocks.size(); i++)
                            {
                                if (e.button.x - 56 > exc[7].blocks[i].r.x &&
                                    e.button.x - 56 < exc[7].blocks[i].r.x + exc[7].blocks[i].r.w &&
                                    e.button.y - 80 > exc[7].blocks[i].r.y &&
                                    e.button.y - 80 < exc[7].blocks[i].r.y + exc[7].blocks[i].r.h &&
                                    click(exc[7].blocks[i], e.button.x - 56, e.button.y - 80).a > 1)
                                {
                                    SDL_SetRenderTarget(renderer, backsave);
                                    dellv = true;
                                    SDL_RenderCopy(renderer, working->texture, nullptr, nullptr);
                                    SDL_SetRenderTarget(renderer, working->texture);
                                    i7 = i;
                                    bcl(110, exc[7].blocks[i].r.y + 13, 149, exc[7].blocks[i].r.y + 39, 0xffa0a0ff);
                                    textRGBA(renderer, 113, exc[7].blocks[i].r.y + 18, "Delete", "tahoma.ttf", 12, 0, 0, 0, 255);
                                    change_c = true;
                                }
                            }
                        }
                    }
                    break;

                case SDL_MOUSEBUTTONUP:
                    if (e.button.button == SDL_BUTTON_LEFT)
                    {
                        mouseClicked = mouseDown;
                        mouseDown    = false;
                        mouseX       = e.button.x;
                        mouseY       = e.button.y;
                        if (currentTab == 1 && !costumesMenu.active)
                            handleInlineCostumesEvent(&inlineCos, renderer, font, &e, &charMgr);
                        if (drag && currentTab == 0)
                        {
                            SDL_SetRenderTarget(renderer, working->texture);
                            working->add(backsave, current, working->texture);
                            drag     = false;
                            change_c = true;
                        }
                    }
                    break;

                case SDL_MOUSEMOTION:
                    mouseX = e.motion.x;
                    mouseY = e.motion.y;
                    if (currentTab == 1 && !costumesMenu.active)
                    {
                        handleInlineCostumesEvent(&inlineCos, renderer, font, &e, &charMgr);
                    }
                    if (drag && currentTab == 0)
                    {
                        SDL_SetRenderTarget(renderer, working->texture);
                        SDL_RenderCopy(renderer, backsave, nullptr, nullptr);
                        for (block* bv : current)
                        {
                            tekan(bv, e.motion.x - xz, e.motion.y - yz);
                            SDL_RenderCopy(renderer, bv->blu, nullptr, &bv->r);
                        }
                        xz       = e.motion.x;
                        yz       = e.motion.y;
                        change_c = true;
                    }
                    break;

                case SDL_TEXTINPUT:
                    if (g_askActive && !g_askDone)
                        g_askBuf += e.text.text;
                    if (currentTab == 1 && !costumesMenu.active)
                    {
                        handleInlineCostumesEvent(&inlineCos, renderer, font, &e, &charMgr);
                    }
                    else
                    {
                        if (myval)
                        {
                            SDL_SetRenderTarget(renderer, working->texture);
                            cva += e.text.text;
                            bcl(156, 30, 262, 52, 0xffffffff);
                            textRGBA(renderer, 158, 33, cva.c_str(), "tahoma.ttf", 12, 0, 0, 0, 255);
                            int ter = convert(cva, 12);
                            bcl(161 + ter, 31, 164 + ter, 51, 0xff000000);
                            change_c = true;
                        }
                        else if (ty && typeb)
                        {
                            SDL_SetRenderTarget(renderer, working->texture);
                            typeb->v[ts] += e.text.text;
                            typeb->draw();
                            SDL_SetRenderTarget(renderer, working->texture);
                            SDL_RenderCopy(renderer, typeb->blu, nullptr, &typeb->r);
                            change_c = true;
                            bcl(typeb->r.x + typeb->x2v[ts] - 6, typeb->r.y,
                                typeb->r.x + typeb->x2v[ts] - 3, typeb->r.y + 26, 0xff000000);
                        }
                    }
                    break;

                case SDL_KEYDOWN:
                    notifyKeyDown(e.key.keysym.scancode);
                    notifyKeyEvent(e.key.keysym.scancode);
                    if (g_askActive && e.key.keysym.sym == SDLK_BACKSPACE && !g_askBuf.empty())
                        g_askBuf.pop_back();
                    if (g_askActive && e.key.keysym.sym == SDLK_RETURN) {
                        g_askDone = true;
                    }
                    if (currentTab == 1 && !costumesMenu.active)
                    {
                        handleInlineCostumesEvent(&inlineCos, renderer, font, &e, &charMgr);
                    }
                    else
                    {
                        if (myval)
                        {
                            SDL_SetRenderTarget(renderer, working->texture);
                            if (e.key.keysym.sym == SDLK_BACKSPACE && !cva.empty())
                            {
                                cva.pop_back();
                                bcl(156, 30, 262, 52, 0xffffffff);
                                if (!cva.empty())
                                    textRGBA(renderer, 158, 33, cva.c_str(), "tahoma.ttf", 12, 0, 0, 0, 255);
                                int ter = convert(cva, 12);
                                bcl(161 + ter, 31, 164 + ter, 51, 0xff000000);
                                change_c = true;
                            }
                        }
                        else if (ty && typeb)
                        {
                            if (e.key.keysym.sym == SDLK_BACKSPACE &&
                                ts >= 0 && ts < 3 && !typeb->v[ts].empty())
                            {
                                SDL_SetRenderTarget(renderer, working->texture);
                                bcl(typeb->r.x + 50, typeb->r.y,
                                    typeb->r.x + typeb->r.w, typeb->r.y + typeb->r.h, 0xffffffff);
                                typeb->v[ts].pop_back();
                                typeb->draw();
                                SDL_SetRenderTarget(renderer, working->texture);
                                change_c = true;
                                SDL_RenderCopy(renderer, typeb->blu, nullptr, &typeb->r);
                                bcl(typeb->r.x + typeb->x2v[ts] - 6, typeb->r.y,
                                    typeb->r.x + typeb->x2v[ts] - 3, typeb->r.y + 26, 0xff000000);
                            }
                        }
                    }
                    if (e.key.keysym.sym == SDLK_ESCAPE)
                    {
                        bgMenu.showingLibrary = false;
                        bgMenu.showingEditor  = false;
                        charBar.panelOpen     = false;
                        if (costumesMenu.active)
                            closeCostumesMenu(&costumesMenu);
                        if (imgEditor.active)
                        {
                            imgEditor.confirmed = false;
                            imgEditor.active    = false;
                            SDL_SetRenderTarget(renderer, nullptr);
                            drawmm(false);
                            { const int tabX = (currentTab==1)?100:(currentTab==2)?200:0; msx(tabX, isSprite); }
                        }
                    }
                    break;
                case SDL_KEYUP:
                    notifyKeyUp(e.key.keysym.scancode);
                    break;

                default:
                    break;
            }
        }

        if (!costumesMenu.active)
        {
            cosUpdateRecording(&costumesMenu, &charMgr);
            cosUpdatePlayback (&costumesMenu, &charMgr);
        }

        if (execute())
        {
            change_e = true;
        }
        SDL_SetRenderTarget(renderer, nullptr);

        if (!imgEditor.active && !costumesMenu.active)
        {
            if (mouseX >= STAGE_X && mouseX < STAGE_X + STAGE_W &&
                mouseY >= STAGE_Y && mouseY < STAGE_Y + STAGE_H)
            {
                handleCharacterDrag(&charMgr, mouseX, mouseY, mouseDown,
                                    mouseJustPressed, &dragIndex);
                if (mouseDown && dragIndex >= 0 && dragIndex < (int)sp.size()) {
                    sp[dragIndex].x = charMgr.characters[dragIndex].x;
                    sp[dragIndex].y = charMgr.characters[dragIndex].y;
                }
            }
        }


        if (mouseClicked && !imgEditor.active && !costumesMenu.active)
        {
            int bgAction = handleBackgroundBarClick(&bgMenu, BG_BAR_RECT, mouseX, mouseY);
            if (bgAction == 2)
            {
                loadBackgroundFromSystem(&bgState, WINDOW_W, WINDOW_H);
                bgMenu.menuOpen = false;
            }
            if (bgAction == 3)
            {
                initImageEditor(&imgEditor, EDITOR_CANVAS_W, EDITOR_CANVAS_H);
                bgMenu.menuOpen = false;
            }
            handleCharacterBarClick(&charBar, CHAR_BAR_RECT, mouseX, mouseY);
        }

        if (requestAddChar)
        {
            requestAddChar = false;
            int choice = MessageBoxA(nullptr,
                                     "Choose how to add a character:\n\nYES  = Load from system (file dialog)\nNO   = Add an orange square",
                                     "Add Character", MB_YESNOCANCEL | MB_ICONQUESTION);

            auto syncNewChar = [&](int idx)
            {
                if (idx < 0) return;
                setActiveCharacter(&charMgr, idx);
                Character& c = charMgr.characters[idx];
                c.x = STAGE_X + (STAGE_W - c.width)  / 2;
                c.y = STAGE_Y + (STAGE_H - c.height) / 2;

                while ((int)spco[0].size() <= idx)
                {
                    codtxs newNode{};
                    spco[0].push_back(newNode);
                    spco[0].back().create(exc[0]);
                }
                working       = &spco[0][idx];
                isSprite      = true;
                prevActiveIdx = idx;
                change_c      = true;
            };

            if (choice == IDYES)
                syncNewChar(addCharacterFromSystem(&charMgr));
            else if (choice == IDNO)
                syncNewChar(addOrangeSquareCharacter(&charMgr));
        }

        if (requestRename && charMgr.activeIndex >= 0)
        {
            requestRename = false;
            if (simpleTextInputDialog(font, "Enter new name:", renameBuffer))
                renameCharacter(&charMgr, charMgr.activeIndex, renameBuffer);
        }

        if (!imgEditor.active && imgEditor.confirmed)
        {
            applyEditorCanvasAsBackground(renderer, &bgState, &imgEditor, WINDOW_W, WINDOW_H);
            SDL_SetRenderTarget(renderer, nullptr);
            drawmm(false);
            { const int tabX = (currentTab==2)?200:(currentTab==1)?100:0; msx(tabX, isSprite); }
        }
        else if (editorWasActive && !imgEditor.active)
        {
            SDL_SetRenderTarget(renderer, nullptr);
            drawmm(false);
            { const int tabX = (currentTab==2)?200:(currentTab==1)?100:0; msx(tabX, isSprite); }
        }
        editorWasActive = imgEditor.active;

        if (charMgr.activeIndex >= 0 && charMgr.activeIndex != prevActiveIdx)
        {
            while ((int)spco[0].size() <= charMgr.activeIndex)
            {
                codtxs newNode{};
                spco[0].push_back(newNode);
                spco[0].back().create(exc[0]);
            }
            working       = &spco[0][charMgr.activeIndex];
            isSprite      = true;
            prevActiveIdx = charMgr.activeIndex;
            change_c      = true;
        }

        SDL_SetRenderTarget(renderer, nullptr);

        if (!imgEditor.active && !costumesMenu.active)
        {
            if (currentTab == 1 && charMgr.activeIndex != inlineCos.ownerIdx)
                inlineCosLoadOwner(&inlineCos, renderer, charMgr.activeIndex);

            if (currentTab == 1)
            {
                SDL_SetRenderTarget(renderer, nullptr);
                renderInlineCostumesPanel(renderer, font, &inlineCos, &charMgr,
                                          mouseX, mouseY, mouseClicked);
                msx(100, isSprite);
            }
            else
            {
                working->copy();
                const int tabX = (currentTab == 2) ? 200 : 0;
                msx(tabX, isSprite);
            }
        }


        SDL_RenderSetClipRect(renderer, &stageClip);
        renderBackground( &bgState, stageClip);
        renderAllCharacters(&charMgr);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer, run, nullptr, &stageClip);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        {
            int activeIdx = charMgr.activeIndex;
            const codtxs* activeCod = (activeIdx >= 0 && activeIdx < (int)spco[0].size())
                                      ? &spco[0][activeIdx] : nullptr;
            renderVarMonitors(activeCod, activeIdx);
        }
        SDL_RenderSetClipRect(renderer, nullptr);
        if (g_askActive)
            renderAskPrompt(font, WINDOW_W, WINDOW_H, mouseClicked);

        SDL_SetRenderDrawColor(renderer, 60, 60, 80, 255);
        SDL_RenderDrawRect(renderer, &stageBorder);

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 10, 10, 20, 160);
        SDL_RenderFillRect(renderer, &settRect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

        {
            const int cossBtnY = settRect.y + settRect.h - 90;
            bool openReq = renderCostumesButton(
                    renderer, font,
                    settRect.x + 8, cossBtnY, settRect.w - 60,
                    mouseX, mouseY, mouseClicked);
            if (openReq && !costumesMenu.active && charMgr.activeIndex >= 0)
                openCostumesMenu(&costumesMenu, renderer, charMgr.activeIndex, WINDOW_W, WINDOW_H);
        }

        renderCharacterSettingsPanel(font, &charMgr, settRect,
                                     mouseX, mouseY, mouseClicked,
                                     &requestAddChar, &requestRename, renameBuffer);

        renderCharacterBar(font, &charBar, CHAR_BAR_RECT);
        renderBackgroundBar(font, &bgMenu, &bgState, BG_BAR_RECT);

        if (charBar.panelOpen)
        {
            int popX = CHAR_BAR_RECT.x - 200;
            int popY = CHAR_BAR_RECT.y;
            renderCharacterBarPanel(font, &charMgr, popX, popY,
                                    mouseX, mouseY, mouseClicked);
        }

        if (bgMenu.showingLibrary)
        {
            int libX = BG_BAR_RECT.x - 369;
            int libY = BG_BAR_RECT.y + 10 + 32 + 32;
            int sel  = renderDefaultBackgroundLibrary(font, &bgState, libX, libY,
                                                      mouseX, mouseY, mouseClicked,
                                                      WINDOW_W, WINDOW_H);
            if (sel >= 0)
            {
                bgMenu.showingLibrary = false;
                bgMenu.menuOpen       = false;
            }
        }

        if (imgEditor.active)
        {
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
            SDL_Rect full = {0, 0, WINDOW_W, WINDOW_H};
            SDL_RenderFillRect(renderer, &full);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
            renderImageEditor(font, &imgEditor, EDITOR_CANVAS_RECT,
                              mouseX, mouseY, mouseClicked);
        }

        if (costumesMenu.active)
        {
            renderCostumesMenu(renderer, font, &costumesMenu,
                               &charMgr, mouseX, mouseY, mouseClicked);
        }

        change_c = false;
        SDL_RenderPresent(renderer);
    }
    destroyCostumesMenu(&costumesMenu);
    destroyInlineCostumes(&inlineCos);
    destroyAllCostumes();

    for (auto t : tex) SDL_DestroyTexture(t);
    for (auto b : bbl) { SDL_DestroyTexture(b->blu); delete b; }

    destroyCharacterManager(&charMgr);
    destroyBackgroundState(&bgState);
    if (imgEditor.canvas) SDL_DestroyTexture(imgEditor.canvas);
    if (regular) TTF_CloseFont(regular);
    if (bold)    TTF_CloseFont(bold);
    if (font)    TTF_CloseFont(font);

    msxDestroy();
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
