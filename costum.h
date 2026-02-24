#ifndef COSTUM_H
#define COSTUM_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include "settings.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct CostumeFrame {
    SDL_Texture* texture = nullptr;
    int          w       = 100;
    int          h       = 100;
    std::string  name;
};

struct AnimKeyframe {
    int    x          = 0;
    int    y          = 0;
    float  rotation   = 0.f;
    int    costumeIdx = 0;
    Uint32 timeMs     = 0;
};

struct RecordedAnimation {
    std::string               name;
    std::vector<AnimKeyframe> keyframes;
    Uint32                    durationMs = 0;
};

struct CharacterCostumes {
    std::vector<CostumeFrame>      costumes;
    int                            active = -1;
    std::vector<RecordedAnimation> animations;
};

static CharacterCostumes g_cc[MAX_CHARACTERS];

enum class CosTool {
    PEN = 0,
    ERASER,
    FILL,
    TEXT,
    LINE,
    CIRCLE,
    RECT
};

struct CostumesMenuState {
    bool active     = false;
    int  ownerIdx   = -1;

    SDL_Rect panelRect   = {};
    SDL_Rect cosListRect = {};
    SDL_Rect canvasRect  = {};
    SDL_Rect toolsRect   = {};
    SDL_Rect animRect    = {};

    SDL_Texture* canvas  = nullptr;
    SDL_Texture* preview = nullptr;
    int cW = 380, cH = 300;

    CosTool   tool     = CosTool::PEN;
    SDL_Color palette[3] = { {0,0,0,255}, {210,50,50,255}, {50,100,210,255} };
    int       colorIdx = 0;
    int       sizePx[3]= { 3, 9, 18 };
    int       sizeIdx  = 0;

    bool isDrawing = false;
    int  startX=-1, startY=-1;
    int  lastX =-1, lastY =-1;

    bool        textMode = false;
    std::string textBuf;
    int         textPX   = 0, textPY = 0;

    std::string posXBuf, posYBuf;
    bool        posXFocus = false, posYFocus = false;

    bool              recording    = false;
    Uint32            recStart     = 0;
    RecordedAnimation curRec;
    bool              namingAnim   = false;
    std::string       animNameBuf;

    bool   playing      = false;
    int    playAnimIdx  = -1;
    Uint32 playStart    = 0;
    int    selAnimIdx   = -1;

    int cosScroll  = 0;
    int animScroll = 0;

    bool        renamingCos = false;
    int         renameIdx   = -1;
    std::string renameBuf;
};

static void cos_disc(SDL_Renderer* r, SDL_Texture* tgt,
                     int cx, int cy, int radius, SDL_Color c)
{
    SDL_SetRenderTarget(r, tgt);
    SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
    for (int dy = -radius; dy <= radius; dy++)
        for (int dx = -radius; dx <= radius; dx++)
            if (dx*dx + dy*dy <= radius*radius)
                SDL_RenderDrawPoint(r, cx+dx, cy+dy);
    SDL_SetRenderTarget(r, nullptr);
}

static void cos_strokeLine(SDL_Renderer* r, SDL_Texture* tgt,
                           int x0, int y0, int x1, int y1,
                           int radius, SDL_Color c)
{
    int steps = std::max({ std::abs(x1-x0), std::abs(y1-y0), 1 });
    for (int i = 0; i <= steps; i++) {
        int px = x0 + (x1-x0)*i/steps;
        int py = y0 + (y1-y0)*i/steps;
        cos_disc(r, tgt, px, py, radius, c);
    }
}

static void cos_outlineRect(SDL_Renderer* r, SDL_Texture* tgt,
                            int x0, int y0, int x1, int y1,
                            int thick, SDL_Color c)
{
    if (x0>x1) std::swap(x0,x1);
    if (y0>y1) std::swap(y0,y1);
    SDL_SetRenderTarget(r, tgt);
    SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
    for (int t = 0; t < thick; t++) {
        SDL_Rect rc = { x0+t, y0+t, x1-x0-2*t, y1-y0-2*t };
        if (rc.w > 0 && rc.h > 0)
            SDL_RenderDrawRect(r, &rc);
    }
    SDL_SetRenderTarget(r, nullptr);
}

static void cos_outlineEllipse(SDL_Renderer* r, SDL_Texture* tgt,
                               int x0, int y0, int x1, int y1, SDL_Color c)
{
    if (x0>x1) std::swap(x0,x1);
    if (y0>y1) std::swap(y0,y1);
    int cx = (x0+x1)/2, cy = (y0+y1)/2;
    double rx = (x1-x0)/2.0, ry = (y1-y0)/2.0;
    if (rx < 1 || ry < 1) return;
    SDL_SetRenderTarget(r, tgt);
    SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
    for (int deg = 0; deg < 360; deg++) {
        double a1 = deg     * M_PI / 180.0;
        double a2 = (deg+1) * M_PI / 180.0;
        SDL_RenderDrawLine(r,
                           (int)(cx + rx*std::cos(a1)), (int)(cy + ry*std::sin(a1)),
                           (int)(cx + rx*std::cos(a2)), (int)(cy + ry*std::sin(a2)));
    }
    SDL_SetRenderTarget(r, nullptr);
}

static void cos_fillCanvas(SDL_Renderer* r, SDL_Texture* tgt,
                           int w, int h, SDL_Color c)
{
    SDL_SetRenderTarget(r, tgt);
    SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
    SDL_Rect rc = {0, 0, w, h};
    SDL_RenderFillRect(r, &rc);
    SDL_SetRenderTarget(r, nullptr);
}

static SDL_Texture* cos_flipTex(SDL_Renderer* r, SDL_Texture* src,
                                int w, int h, bool flipH, bool flipV)
{
    SDL_Texture* dst = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888,
                                         SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetRenderTarget(r, dst);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 0);
    SDL_RenderClear(r);
    SDL_RendererFlip f = SDL_FLIP_NONE;
    if (flipH) f = static_cast<SDL_RendererFlip>(f | SDL_FLIP_HORIZONTAL);
    if (flipV) f = static_cast<SDL_RendererFlip>(f | SDL_FLIP_VERTICAL);
    SDL_Rect dr = {0, 0, w, h};
    SDL_RenderCopyEx(r, src, nullptr, &dr, 0.0, nullptr, f);
    SDL_SetRenderTarget(r, nullptr);
    return dst;
}

static SDL_Texture* cos_blankWhite(SDL_Renderer* r, int w, int h)
{
    SDL_Texture* t = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888,
                                       SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetRenderTarget(r, t);
    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    SDL_RenderClear(r);
    SDL_SetRenderTarget(r, nullptr);
    return t;
}

static SDL_Texture* cos_blankTransparent(SDL_Renderer* r, int w, int h)
{
    SDL_Texture* t = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888,
                                       SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetTextureBlendMode(t, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(r, t);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 0);
    SDL_RenderClear(r);
    SDL_SetRenderTarget(r, nullptr);
    return t;
}

inline void initCostumesMenu(CostumesMenuState* cm)
{
    *cm = CostumesMenuState{};
}

inline void destroyCostumesMenu(CostumesMenuState* cm)
{
    if (cm->canvas)  { SDL_DestroyTexture(cm->canvas);  cm->canvas  = nullptr; }
    if (cm->preview) { SDL_DestroyTexture(cm->preview); cm->preview = nullptr; }
}

inline void destroyCharCostumes(int idx)
{
    if (idx < 0 || idx >= MAX_CHARACTERS) return;
    for (auto& f : g_cc[idx].costumes)
        if (f.texture) { SDL_DestroyTexture(f.texture); f.texture = nullptr; }
    g_cc[idx].costumes.clear();
    g_cc[idx].active = -1;
    g_cc[idx].animations.clear();
}

inline void destroyAllCostumes()
{
    for (int i = 0; i < MAX_CHARACTERS; i++)
        destroyCharCostumes(i);
}

inline void openCostumesMenu(CostumesMenuState* cm, SDL_Renderer* rend,
                             int ownerIdx, int winW, int winH)
{
    cm->active    = true;
    cm->ownerIdx  = ownerIdx;

    const int PW = 920, PH = 610;
    int PX = std::max(0, (winW - PW) / 2);
    int PY = std::max(0, (winH - PH) / 2);

    cm->panelRect   = { PX,        PY,       PW,       PH       };
    cm->cosListRect = { PX,        PY + 38,  205,      PH - 150 };
    cm->cW = 395; cm->cH = 300;
    cm->canvasRect  = { PX + 214,  PY + 48,  cm->cW,   cm->cH   };
    int tX = PX + 214 + cm->cW + 8;
    cm->toolsRect   = { tX,        PY + 38,  PX+PW-tX-6, PH - 150 };
    cm->animRect    = { PX,        PY + PH - 110, PW, 110 };

    if (cm->canvas)  { SDL_DestroyTexture(cm->canvas);  cm->canvas  = nullptr; }
    if (cm->preview) { SDL_DestroyTexture(cm->preview); cm->preview = nullptr; }

    cm->canvas = cos_blankWhite(rend, cm->cW, cm->cH);

    if (ownerIdx >= 0 && ownerIdx < MAX_CHARACTERS) {
        auto& cc = g_cc[ownerIdx];
        if (cc.active >= 0 && cc.active < (int)cc.costumes.size()
            && cc.costumes[cc.active].texture)
        {
            auto& cf = cc.costumes[cc.active];
            float scaleX = (float)cm->cW / (float)cf.w;
            float scaleY = (float)cm->cH / (float)cf.h;
            float scale  = std::min({ scaleX, scaleY, 1.f });
            int dw = (int)(cf.w * scale), dh = (int)(cf.h * scale);
            SDL_Rect dst = { (cm->cW-dw)/2, (cm->cH-dh)/2, dw, dh };
            SDL_SetRenderTarget(rend, cm->canvas);
            SDL_RenderCopy(rend, cf.texture, nullptr, &dst);
            SDL_SetRenderTarget(rend, nullptr);
        }
    }

    cm->preview = cos_blankTransparent(rend, cm->cW, cm->cH);

    cm->isDrawing   = false;
    cm->textMode    = false;
    cm->textBuf.clear();
    cm->posXBuf.clear();
    cm->posYBuf.clear();
    cm->posXFocus   = false;
    cm->posYFocus   = false;
    cm->cosScroll   = 0;
    cm->animScroll  = 0;
    cm->namingAnim  = false;
    cm->renamingCos = false;
}

inline void closeCostumesMenu(CostumesMenuState* cm)
{
    if (cm->textMode) SDL_StopTextInput();
    cm->textMode  = false;
    cm->textBuf.clear();
    cm->posXFocus = false;
    cm->posYFocus = false;
    cm->namingAnim  = false;
    cm->renamingCos = false;
    cm->recording   = false;
    cm->isDrawing   = false;
    if (cm->canvas)  { SDL_DestroyTexture(cm->canvas);  cm->canvas  = nullptr; }
    if (cm->preview) { SDL_DestroyTexture(cm->preview); cm->preview = nullptr; }
    cm->active = false;
}

inline void cosUploadImages(SDL_Renderer* rend, int charIdx)
{
    if (charIdx < 0 || charIdx >= MAX_CHARACTERS) return;

    char fileBuf[32768] = {};
    OPENFILENAMEA ofn   = {};
    ofn.lStructSize  = sizeof(ofn);
    ofn.hwndOwner    = nullptr;
    ofn.lpstrFile    = fileBuf;
    ofn.nMaxFile     = sizeof(fileBuf);
    ofn.lpstrFilter  = "Image Files\0*.png;*.jpg;*.jpeg;*.bmp\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags        = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST
                       | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

    if (!GetOpenFileNameA(&ofn)) return;

    auto& cc = g_cc[charIdx];

    const char* dir = fileBuf;
    const char* ptr = fileBuf + strlen(dir) + 1;

    auto addFrame = [&](const std::string& fullPath) {
        SDL_Surface* surf = IMG_Load(fullPath.c_str());
        if (!surf) return;
        CostumeFrame f;
        f.texture = SDL_CreateTextureFromSurface(rend, surf);
        f.w = surf->w;
        f.h = surf->h;
        size_t sl = fullPath.find_last_of("\\/");
        f.name = (sl != std::string::npos) ? fullPath.substr(sl + 1) : fullPath;
        SDL_FreeSurface(surf);
        cc.costumes.push_back(f);
        if (cc.active < 0) cc.active = 0;
    };

    if (*ptr == '\0') {
        addFrame(fileBuf);
    } else {
        while (*ptr != '\0') {
            addFrame(std::string(dir) + "\\" + std::string(ptr));
            ptr += strlen(ptr) + 1;
        }
    }
}

inline void cosSaveCanvasAsCostume(SDL_Renderer* rend, CostumesMenuState* cm,
                                   std::string customName = "")
{
    if (!cm->canvas || cm->ownerIdx < 0) return;
    auto& cc = g_cc[cm->ownerIdx];

    SDL_Texture* copy = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA8888,
                                          SDL_TEXTUREACCESS_TARGET, cm->cW, cm->cH);
    SDL_SetRenderTarget(rend, copy);
    SDL_RenderCopy(rend, cm->canvas, nullptr, nullptr);
    SDL_SetRenderTarget(rend, nullptr);

    CostumeFrame f;
    f.texture = copy;
    f.w       = cm->cW;
    f.h       = cm->cH;
    f.name    = customName.empty()
                ? ("Costume " + std::to_string((int)cc.costumes.size() + 1))
                : customName;
    cc.costumes.push_back(f);
    cc.active = (int)cc.costumes.size() - 1;
}

inline void cosApplyActiveToCharacter(SDL_Renderer* rend, CostumesMenuState* cm,
                                      CharacterManager* mgr)
{
    if (!mgr || cm->ownerIdx < 0 || cm->ownerIdx >= mgr->count) return;
    auto& cc = g_cc[cm->ownerIdx];
    if (cc.active < 0 || cc.active >= (int)cc.costumes.size()) return;
    auto& cf = cc.costumes[cc.active];
    if (!cf.texture) return;

    auto& ch = mgr->characters[cm->ownerIdx];

    int tw = ch.width  > 0 ? ch.width  : cf.w;
    int th = ch.height > 0 ? ch.height : cf.h;
    SDL_Texture* scaled = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA8888,
                                            SDL_TEXTUREACCESS_TARGET, tw, th);
    SDL_SetRenderTarget(rend, scaled);
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 0);
    SDL_RenderClear(rend);
    SDL_RenderCopy(rend, cf.texture, nullptr, nullptr);
    SDL_SetRenderTarget(rend, nullptr);

    if (ch.texture) SDL_DestroyTexture(ch.texture);
    ch.texture = scaled;
    ch.width   = tw;
    ch.height  = th;
}

inline void cosStartRecording(CostumesMenuState* cm)
{
    cm->recording  = true;
    cm->recStart   = SDL_GetTicks();
    cm->curRec     = RecordedAnimation{};
}

inline void cosUpdateRecording(CostumesMenuState* cm, CharacterManager* mgr)
{
    if (!cm->recording || cm->ownerIdx < 0 || cm->ownerIdx >= mgr->count) return;
    auto& ch = mgr->characters[cm->ownerIdx];
    AnimKeyframe kf;
    kf.x          = ch.x;
    kf.y          = ch.y;
    kf.rotation   = ch.rotation;
    kf.costumeIdx = std::max(0, g_cc[cm->ownerIdx].active);
    kf.timeMs     = SDL_GetTicks() - cm->recStart;
    cm->curRec.keyframes.push_back(kf);
}

inline void cosStopRecording(CostumesMenuState* cm)
{
    if (!cm->recording) return;
    cm->recording         = false;
    cm->curRec.durationMs = SDL_GetTicks() - cm->recStart;
    cm->animNameBuf = "Animation " +
                      std::to_string((int)g_cc[cm->ownerIdx].animations.size() + 1);
    cm->namingAnim  = true;
    SDL_StartTextInput();
}

inline void cosSaveAnimation(CostumesMenuState* cm)
{
    if (cm->ownerIdx < 0 || cm->curRec.keyframes.empty()) return;
    cm->curRec.name = cm->animNameBuf.empty()
                      ? ("Anim " + std::to_string(
                    (int)g_cc[cm->ownerIdx].animations.size() + 1))
                      : cm->animNameBuf;
    g_cc[cm->ownerIdx].animations.push_back(cm->curRec);
    cm->namingAnim = false;
    SDL_StopTextInput();
}

inline void cosPlayAnimation(CostumesMenuState* cm, int animIdx)
{
    if (cm->ownerIdx < 0) return;
    auto& cc = g_cc[cm->ownerIdx];
    if (animIdx < 0 || animIdx >= (int)cc.animations.size()) return;
    cm->playing     = true;
    cm->playAnimIdx = animIdx;
    cm->playStart   = SDL_GetTicks();
}

inline void cosUpdatePlayback(CostumesMenuState* cm, CharacterManager* mgr)
{
    if (!cm->playing || cm->ownerIdx < 0 || cm->ownerIdx >= mgr->count) return;
    auto& cc = g_cc[cm->ownerIdx];
    if (cm->playAnimIdx < 0 || cm->playAnimIdx >= (int)cc.animations.size())
    { cm->playing = false; return; }

    auto& anim = cc.animations[cm->playAnimIdx];
    if (anim.keyframes.empty() || anim.durationMs == 0)
    { cm->playing = false; return; }

    Uint32 elapsed = SDL_GetTicks() - cm->playStart;
    if (elapsed >= anim.durationMs)
    { cm->playing = false; return; }

    auto& ch = mgr->characters[cm->ownerIdx];
    for (int k = (int)anim.keyframes.size() - 1; k >= 0; k--) {
        if (anim.keyframes[k].timeMs <= elapsed) {
            ch.x        = anim.keyframes[k].x;
            ch.y        = anim.keyframes[k].y;
            ch.rotation = anim.keyframes[k].rotation;
            int ci      = anim.keyframes[k].costumeIdx;
            if (ci >= 0 && ci < (int)cc.costumes.size())
                cc.active = ci;
            break;
        }
    }
}

inline bool cosHandleEvent(CostumesMenuState* cm, SDL_Renderer* rend,
                           TTF_Font* font, SDL_Event* ev,
                           CharacterManager* mgr)
{
    if (!cm->active) return false;

    auto& cr = cm->canvasRect;
    SDL_Color col = cm->palette[cm->colorIdx];
    int       sz  = cm->sizePx[cm->sizeIdx];

    if (ev->type == SDL_MOUSEBUTTONDOWN && ev->button.button == SDL_BUTTON_LEFT)
    {
        int mx = ev->button.x, my = ev->button.y;

        SDL_Rect X = { cm->panelRect.x + cm->panelRect.w - 30,
                       cm->panelRect.y + 6, 24, 24 };
        if (mx>=X.x && mx<=X.x+X.w && my>=X.y && my<=X.y+X.h)
        { closeCostumesMenu(cm); return true; }

        bool onCanvas = (mx>=cr.x && mx<cr.x+cr.w &&
                         my>=cr.y && my<cr.y+cr.h);
        if (onCanvas) {
            int cx = mx - cr.x, cy = my - cr.y;

            if (cm->textMode) {
                cm->textPX = cx; cm->textPY = cy;
                return true;
            }

            cm->isDrawing = true;
            cm->startX = cx; cm->startY = cy;
            cm->lastX  = cx; cm->lastY  = cy;

            switch (cm->tool) {
                case CosTool::PEN:
                    cos_disc(rend, cm->canvas, cx, cy, sz, col);
                    break;
                case CosTool::ERASER:
                    cos_disc(rend, cm->canvas, cx, cy, sz*2, {255,255,255,255});
                    break;
                case CosTool::FILL:
                    cos_fillCanvas(rend, cm->canvas, cm->cW, cm->cH, col);
                    cm->isDrawing = false;
                    break;
                default:
                    SDL_SetRenderTarget(rend, cm->preview);
                    SDL_SetRenderDrawColor(rend, 0,0,0,0);
                    SDL_RenderClear(rend);
                    SDL_SetRenderTarget(rend, nullptr);
                    break;
            }
            return true;
        }

        if (!(mx>=cm->panelRect.x && mx<=cm->panelRect.x+cm->panelRect.w &&
              my>=cm->panelRect.y && my<=cm->panelRect.y+cm->panelRect.h))
            return false;

        cm->posXFocus = false;
        cm->posYFocus = false;
        return true;
    }

    if (ev->type == SDL_MOUSEMOTION && cm->isDrawing)
    {
        int mx = ev->motion.x, my = ev->motion.y;
        int cx = mx - cr.x, cy = my - cr.y;

        switch (cm->tool) {
            case CosTool::PEN:
                cos_strokeLine(rend, cm->canvas,
                               cm->lastX, cm->lastY, cx, cy, sz, col);
                break;
            case CosTool::ERASER:
                cos_strokeLine(rend, cm->canvas,
                               cm->lastX, cm->lastY, cx, cy, sz*2,
                               {255,255,255,255});
                break;
            case CosTool::LINE: {
                SDL_SetRenderTarget(rend, cm->preview);
                SDL_SetRenderDrawColor(rend, 0,0,0,0);
                SDL_RenderClear(rend);
                SDL_SetRenderTarget(rend, nullptr);
                cos_strokeLine(rend, cm->preview,
                               cm->startX, cm->startY, cx, cy, sz, col);
                break;
            }
            case CosTool::RECT: {
                SDL_SetRenderTarget(rend, cm->preview);
                SDL_SetRenderDrawColor(rend, 0,0,0,0);
                SDL_RenderClear(rend);
                SDL_SetRenderTarget(rend, nullptr);
                cos_outlineRect(rend, cm->preview,
                                cm->startX, cm->startY, cx, cy, sz, col);
                break;
            }
            case CosTool::CIRCLE: {
                SDL_SetRenderTarget(rend, cm->preview);
                SDL_SetRenderDrawColor(rend, 0,0,0,0);
                SDL_RenderClear(rend);
                SDL_SetRenderTarget(rend, nullptr);
                cos_outlineEllipse(rend, cm->preview,
                                   cm->startX, cm->startY, cx, cy, col);
                break;
            }
            default: break;
        }

        cm->lastX = cx; cm->lastY = cy;
        return true;
    }

    if (ev->type == SDL_MOUSEBUTTONUP && ev->button.button == SDL_BUTTON_LEFT
        && cm->isDrawing)
    {
        int cx = ev->button.x - cr.x, cy = ev->button.y - cr.y;

        switch (cm->tool) {
            case CosTool::LINE:
                cos_strokeLine(rend, cm->canvas,
                               cm->startX, cm->startY, cx, cy, sz, col);
                break;
            case CosTool::RECT:
                cos_outlineRect(rend, cm->canvas,
                                cm->startX, cm->startY, cx, cy, sz, col);
                break;
            case CosTool::CIRCLE:
                cos_outlineEllipse(rend, cm->canvas,
                                   cm->startX, cm->startY, cx, cy, col);
                break;
            default: break;
        }

        SDL_SetRenderTarget(rend, cm->preview);
        SDL_SetRenderDrawColor(rend, 0,0,0,0);
        SDL_RenderClear(rend);
        SDL_SetRenderTarget(rend, nullptr);

        cm->isDrawing = false;
        return true;
    }

    if (ev->type == SDL_TEXTINPUT) {
        if (cm->textMode)    { cm->textBuf     += ev->text.text; return true; }
        if (cm->posXFocus)   { cm->posXBuf     += ev->text.text; return true; }
        if (cm->posYFocus)   { cm->posYBuf     += ev->text.text; return true; }
        if (cm->namingAnim)  { cm->animNameBuf += ev->text.text; return true; }
        if (cm->renamingCos) { cm->renameBuf   += ev->text.text; return true; }
    }

    if (ev->type == SDL_KEYDOWN) {
        auto sym = ev->key.keysym.sym;

        if (cm->textMode) {
            if (sym == SDLK_RETURN) {
                if (font && !cm->textBuf.empty()) {
                    SDL_Surface* ts = TTF_RenderText_Blended(
                            font, cm->textBuf.c_str(),
                            cm->palette[cm->colorIdx]);
                    if (ts) {
                        SDL_Texture* tt = SDL_CreateTextureFromSurface(rend, ts);
                        SDL_SetRenderTarget(rend, cm->canvas);
                        SDL_Rect dst = { cm->textPX, cm->textPY, ts->w, ts->h };
                        SDL_RenderCopy(rend, tt, nullptr, &dst);
                        SDL_SetRenderTarget(rend, nullptr);
                        SDL_DestroyTexture(tt);
                        SDL_FreeSurface(ts);
                    }
                }
                cm->textBuf.clear();
                cm->textMode = false;
                SDL_StopTextInput();
            } else if (sym == SDLK_BACKSPACE && !cm->textBuf.empty()) {
                cm->textBuf.pop_back();
            } else if (sym == SDLK_ESCAPE) {
                cm->textBuf.clear();
                cm->textMode = false;
                SDL_StopTextInput();
            }
            return true;
        }

        if (cm->posXFocus && sym==SDLK_BACKSPACE && !cm->posXBuf.empty())
        { cm->posXBuf.pop_back(); return true; }
        if (cm->posYFocus && sym==SDLK_BACKSPACE && !cm->posYBuf.empty())
        { cm->posYBuf.pop_back(); return true; }

        if (cm->namingAnim) {
            if (sym == SDLK_BACKSPACE && !cm->animNameBuf.empty())
            { cm->animNameBuf.pop_back(); return true; }
            if (sym == SDLK_RETURN)
            { cosSaveAnimation(cm); return true; }
            if (sym == SDLK_ESCAPE)
            { cm->namingAnim = false; SDL_StopTextInput(); return true; }
            return true;
        }

        if (cm->renamingCos) {
            if (sym == SDLK_BACKSPACE && !cm->renameBuf.empty())
            { cm->renameBuf.pop_back(); return true; }
            if (sym == SDLK_RETURN) {
                if (cm->renameIdx >= 0 && cm->ownerIdx >= 0) {
                    auto& cc = g_cc[cm->ownerIdx];
                    if (cm->renameIdx < (int)cc.costumes.size())
                        cc.costumes[cm->renameIdx].name = cm->renameBuf;
                }
                cm->renamingCos = false;
                SDL_StopTextInput();
                return true;
            }
            if (sym == SDLK_ESCAPE)
            { cm->renamingCos = false; SDL_StopTextInput(); return true; }
            return true;
        }

        if (sym == SDLK_ESCAPE) {
            closeCostumesMenu(cm);
            return true;
        }
    }

    return false;
}

inline void renderCostumesMenu(SDL_Renderer* rend, TTF_Font* font,
                               CostumesMenuState* cm, CharacterManager* mgr,
                               int mouseX, int mouseY, bool mouseClicked)
{
    if (!cm->active || !font) return;

    auto drawText = [&](const char* txt, int x, int y, SDL_Color c) {
        SDL_Surface* s = TTF_RenderText_Blended(font, txt, c);
        if (!s) return;
        SDL_Texture* t = SDL_CreateTextureFromSurface(rend, s);
        SDL_Rect r = { x, y, s->w, s->h };
        SDL_RenderCopy(rend, t, nullptr, &r);
        SDL_DestroyTexture(t);
        SDL_FreeSurface(s);
    };

    auto drawTextC = [&](const char* txt, SDL_Rect btn, SDL_Color c) {
        SDL_Surface* s = TTF_RenderText_Blended(font, txt, c);
        if (!s) return;
        SDL_Texture* t = SDL_CreateTextureFromSurface(rend, s);
        SDL_Rect r = { btn.x+(btn.w-s->w)/2, btn.y+(btn.h-s->h)/2, s->w, s->h };
        SDL_RenderCopy(rend, t, nullptr, &r);
        SDL_DestroyTexture(t);
        SDL_FreeSurface(s);
    };

    auto isHit = [&](SDL_Rect b) {
        return mouseX>=b.x && mouseX<=b.x+b.w &&
               mouseY>=b.y && mouseY<=b.y+b.h;
    };

    const SDL_Color WHITE   = {255,255,255,255};
    const SDL_Color GRAY    = {160,160,160,255};
    const SDL_Color CYAN    = {  0,200,255,255};
    const SDL_Color DARK    = { 20, 20, 40,255};
    const SDL_Color PANEL   = { 30, 30, 55,255};
    const SDL_Color TITLEBAR= { 40, 40, 80,255};

    auto& pr  = cm->panelRect;
    auto& clr = cm->cosListRect;
    auto& cvr = cm->canvasRect;
    auto& tr  = cm->toolsRect;
    auto& ar  = cm->animRect;

    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 185);
    SDL_Rect full = { 0, 0, 9999, 9999 };
    SDL_RenderFillRect(rend, &full);
    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_NONE);

    SDL_SetRenderDrawColor(rend, PANEL.r, PANEL.g, PANEL.b, 255);
    SDL_RenderFillRect(rend, &pr);
    SDL_SetRenderDrawColor(rend, 80, 80, 200, 255);
    SDL_RenderDrawRect(rend, &pr);

    SDL_Rect tb = { pr.x, pr.y, pr.w, 36 };
    SDL_SetRenderDrawColor(rend, TITLEBAR.r, TITLEBAR.g, TITLEBAR.b, 255);
    SDL_RenderFillRect(rend, &tb);

    std::string title = "Costumes";
    if (cm->ownerIdx >= 0 && cm->ownerIdx < mgr->count)
        title += "  —  " + mgr->characters[cm->ownerIdx].name;
    drawText(title.c_str(), pr.x + 12, pr.y + 10, WHITE);

    SDL_Rect closeBtn = { pr.x+pr.w-30, pr.y+6, 24, 24 };
    SDL_SetRenderDrawColor(rend, 180,40,40,255);
    SDL_RenderFillRect(rend, &closeBtn);
    drawTextC("X", closeBtn, WHITE);
    if (mouseClicked && isHit(closeBtn)) {
        closeCostumesMenu(cm); return;
    }

    SDL_SetRenderDrawColor(rend, DARK.r, DARK.g, DARK.b, 255);
    SDL_RenderFillRect(rend, &clr);
    SDL_SetRenderDrawColor(rend, 50,50,110,255);
    SDL_RenderDrawRect(rend, &clr);

    drawText("Costumes", clr.x+6, clr.y+5, WHITE);

    const int tSz = 52, tPad = 6, rowH = tSz + tPad + 15;
    int listY0 = clr.y + 26;

    if (cm->ownerIdx >= 0) {
        auto& cc = g_cc[cm->ownerIdx];

        for (int i = 0; i < (int)cc.costumes.size(); i++) {
            int ry = listY0 + i * rowH - cm->cosScroll;
            if (ry + rowH < clr.y+26 || ry > clr.y + clr.h - 58) continue;

            auto& cf = cc.costumes[i];
            bool  sel = (cc.active == i);

            SDL_Rect bg = { clr.x+3, ry, clr.w-6, tSz+4 };
            SDL_SetRenderDrawColor(rend,
                                   sel ? 0  : 40,
                                   sel ? 140: 40,
                                   sel ? 200: 80, 255);
            SDL_RenderFillRect(rend, &bg);

            SDL_Rect thumb = { clr.x+5, ry+2, tSz, tSz };
            if (cf.texture)
                SDL_RenderCopy(rend, cf.texture, nullptr, &thumb);
            else {
                SDL_SetRenderDrawColor(rend, 70,70,70,255);
                SDL_RenderFillRect(rend, &thumb);
            }

            drawText(cf.name.c_str(),
                     clr.x + tSz + 10, ry + (tSz/2 - 7),
                     sel ? CYAN : WHITE);

            if (mouseClicked && isHit(bg)) {
                cc.active = i;
                SDL_SetRenderTarget(rend, cm->canvas);
                SDL_SetRenderDrawColor(rend, 255,255,255,255);
                SDL_RenderClear(rend);
                if (cf.texture) {
                    float sx = (float)cm->cW / std::max(1, cf.w);
                    float sy = (float)cm->cH / std::max(1, cf.h);
                    float sc = std::min({sx, sy, 1.f});
                    int dw = (int)(cf.w*sc), dh = (int)(cf.h*sc);
                    SDL_Rect dst = { (cm->cW-dw)/2, (cm->cH-dh)/2, dw, dh };
                    SDL_RenderCopy(rend, cf.texture, nullptr, &dst);
                }
                SDL_SetRenderTarget(rend, nullptr);
            }

            int renBtnX = clr.x + clr.w - 28;
            SDL_Rect renBtn = { renBtnX, ry+tSz/2-8, 24, 18 };
            SDL_SetRenderDrawColor(rend, 60,60,140,255);
            SDL_RenderFillRect(rend, &renBtn);
            drawTextC("...", renBtn, GRAY);
            if (mouseClicked && isHit(renBtn)) {
                cm->renamingCos = true;
                cm->renameIdx   = i;
                cm->renameBuf   = cf.name;
                SDL_StartTextInput();
            }
        }
    }

    int bY = clr.y + clr.h - 54;
    SDL_Rect upBtn = { clr.x+4, bY,    clr.w-8, 24 };
    SDL_Rect nbBtn = { clr.x+4, bY+28, clr.w-8, 24 };

    SDL_SetRenderDrawColor(rend, 20,110,20,255);
    SDL_RenderFillRect(rend, &upBtn);
    drawTextC("+ Upload Image", upBtn, WHITE);
    if (mouseClicked && isHit(upBtn)) {
        cosUploadImages(rend, cm->ownerIdx);
    }

    SDL_SetRenderDrawColor(rend, 50,50,140,255);
    SDL_RenderFillRect(rend, &nbBtn);
    drawTextC("+ New Blank", nbBtn, WHITE);
    if (mouseClicked && isHit(nbBtn)) {
        SDL_SetRenderTarget(rend, cm->canvas);
        SDL_SetRenderDrawColor(rend, 255,255,255,255);
        SDL_RenderClear(rend);
        SDL_SetRenderTarget(rend, nullptr);
    }

    if (cm->renamingCos && cm->renameIdx >= 0) {
        drawText(("Rename → " + cm->renameBuf + "|").c_str(),
                 clr.x+4, clr.y + clr.h - 74, CYAN);
    }

    SDL_SetRenderDrawColor(rend, 200,200,200,255);
    SDL_Rect cbdr = { cvr.x-2, cvr.y-2, cvr.w+4, cvr.h+4 };
    SDL_RenderFillRect(rend, &cbdr);

    SDL_RenderCopy(rend, cm->canvas, nullptr, &cvr);
    SDL_SetTextureBlendMode(cm->preview, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(rend, cm->preview, nullptr, &cvr);

    if (cm->textMode) {
        int tx = cvr.x + cm->textPX, ty = cvr.y + cm->textPY;
        if (!cm->textBuf.empty()) {
            SDL_Surface* ts = TTF_RenderText_Blended(
                    font, cm->textBuf.c_str(), cm->palette[cm->colorIdx]);
            if (ts) {
                SDL_Texture* tt = SDL_CreateTextureFromSurface(rend, ts);
                SDL_Rect dr = { tx, ty, ts->w, ts->h };
                SDL_RenderCopy(rend, tt, nullptr, &dr);
                SDL_DestroyTexture(tt); SDL_FreeSurface(ts);
            }
        }
        if ((SDL_GetTicks() / 500) % 2 == 0) {
            SDL_SetRenderDrawColor(rend, 0,0,0,255);
            SDL_RenderDrawLine(rend, tx, ty, tx, ty+16);
        }
    }

    drawText("Edit Canvas", cvr.x, cvr.y-18, GRAY);

    SDL_SetRenderDrawColor(rend, 22,22,48,255);
    SDL_RenderFillRect(rend, &tr);
    SDL_SetRenderDrawColor(rend, 50,50,110,255);
    SDL_RenderDrawRect(rend, &tr);

    int ty2 = tr.y + 6;
    const int TW  = tr.w - 10;
    const int TBH = 22;

    drawText("Tools", tr.x+5, ty2, WHITE); ty2 += 20;

    struct TB { const char* lbl; CosTool t; };
    const TB toolBtns[] = {
            { "Pen",       CosTool::PEN    },
            { "Eraser",    CosTool::ERASER },
            { "Fill BG",   CosTool::FILL   },
            { "Text",      CosTool::TEXT   },
            { "Line",      CosTool::LINE   },
            { "Circle",    CosTool::CIRCLE },
            { "Rectangle", CosTool::RECT   },
    };

    for (auto& tb : toolBtns) {
        SDL_Rect btn = { tr.x+5, ty2, TW, TBH };
        bool active  = (cm->tool == tb.t);
        SDL_SetRenderDrawColor(rend,
                               active?75:35, active?75:35, active?175:90, 255);
        SDL_RenderFillRect(rend, &btn);
        if (active) {
            SDL_SetRenderDrawColor(rend, 0,200,255,255);
            SDL_RenderDrawRect(rend, &btn);
        }
        drawTextC(tb.lbl, btn, active ? CYAN : WHITE);
        if (mouseClicked && isHit(btn)) {
            cm->tool = tb.t;
            cm->textMode = (tb.t == CosTool::TEXT);
            if (cm->textMode) SDL_StartTextInput();
            else SDL_StopTextInput();
        }
        ty2 += TBH + 3;
    }

    ty2 += 3;
    SDL_SetRenderDrawColor(rend, 55,55,110,255);
    SDL_RenderDrawLine(rend, tr.x+5, ty2, tr.x+tr.w-5, ty2);
    ty2 += 6;

    drawText("Size:", tr.x+5, ty2, GRAY); ty2 += 17;
    int szW = (TW - 8) / 3;
    const char* szLbl[] = {"S","M","L"};
    for (int si = 0; si < 3; si++) {
        SDL_Rect sb = { tr.x+5 + si*(szW+4), ty2, szW, 22 };
        bool sel = (cm->sizeIdx == si);
        SDL_SetRenderDrawColor(rend, sel?60:30, sel?60:30, sel?175:75, 255);
        SDL_RenderFillRect(rend, &sb);
        if (sel) { SDL_SetRenderDrawColor(rend,0,200,255,255); SDL_RenderDrawRect(rend,&sb); }
        drawTextC(szLbl[si], sb, sel ? CYAN : WHITE);
        if (mouseClicked && isHit(sb)) cm->sizeIdx = si;
    }
    ty2 += 26;

    drawText("Color:", tr.x+5, ty2, GRAY); ty2 += 17;
    int cW2 = (TW - 8) / 3;
    for (int ci = 0; ci < 3; ci++) {
        SDL_Rect cb = { tr.x+5 + ci*(cW2+4), ty2, cW2, 22 };
        SDL_Color& pc = cm->palette[ci];
        SDL_SetRenderDrawColor(rend, pc.r, pc.g, pc.b, 255);
        SDL_RenderFillRect(rend, &cb);
        if (cm->colorIdx == ci) {
            SDL_SetRenderDrawColor(rend, 255,255,255,255); SDL_RenderDrawRect(rend, &cb);
            SDL_Rect inn = { cb.x+1,cb.y+1,cb.w-2,cb.h-2 };
            SDL_SetRenderDrawColor(rend, 0,0,0,255); SDL_RenderDrawRect(rend, &inn);
        }
        if (mouseClicked && isHit(cb)) cm->colorIdx = ci;
    }
    ty2 += 26;

    SDL_SetRenderDrawColor(rend, 55,55,110,255);
    SDL_RenderDrawLine(rend, tr.x+5, ty2, tr.x+tr.w-5, ty2);
    ty2 += 6;

    int halfW = (TW - 4) / 2;
    SDL_Rect flipH = { tr.x+5,          ty2, halfW, 22 };
    SDL_Rect flipV = { tr.x+5+halfW+4,  ty2, halfW, 22 };

    SDL_SetRenderDrawColor(rend, 80,75,30,255);
    SDL_RenderFillRect(rend, &flipH);
    drawTextC("Flip H", flipH, WHITE);
    if (mouseClicked && isHit(flipH)) {
        SDL_Texture* f = cos_flipTex(rend, cm->canvas, cm->cW, cm->cH, true, false);
        SDL_DestroyTexture(cm->canvas);
        cm->canvas = f;
    }

    SDL_SetRenderDrawColor(rend, 75,30,80,255);
    SDL_RenderFillRect(rend, &flipV);
    drawTextC("Flip V", flipV, WHITE);
    if (mouseClicked && isHit(flipV)) {
        SDL_Texture* f = cos_flipTex(rend, cm->canvas, cm->cW, cm->cH, false, true);
        SDL_DestroyTexture(cm->canvas);
        cm->canvas = f;
    }
    ty2 += 26;

    SDL_Rect clrBtn = { tr.x+5, ty2, TW, 22 };
    SDL_SetRenderDrawColor(rend, 140,45,45,255);
    SDL_RenderFillRect(rend, &clrBtn);
    drawTextC("Clear Canvas", clrBtn, WHITE);
    if (mouseClicked && isHit(clrBtn)) {
        SDL_SetRenderTarget(rend, cm->canvas);
        SDL_SetRenderDrawColor(rend, 255,255,255,255);
        SDL_RenderClear(rend);
        SDL_SetRenderTarget(rend, nullptr);
    }
    ty2 += 26;

    SDL_Rect saveBtn = { tr.x+5, ty2, TW, 22 };
    SDL_SetRenderDrawColor(rend, 20,115,60,255);
    SDL_RenderFillRect(rend, &saveBtn);
    drawTextC("Save as Costume", saveBtn, WHITE);
    if (mouseClicked && isHit(saveBtn))
        cosSaveCanvasAsCostume(rend, cm);
    ty2 += 26;

    SDL_Rect applyBtn = { tr.x+5, ty2, TW, 22 };
    SDL_SetRenderDrawColor(rend, 0,90,190,255);
    SDL_RenderFillRect(rend, &applyBtn);
    drawTextC("Apply to Character", applyBtn, WHITE);
    if (mouseClicked && isHit(applyBtn))
        cosApplyActiveToCharacter(rend, cm, mgr);
    ty2 += 30;

    drawText("Position:", tr.x+5, ty2, GRAY); ty2 += 17;

    int fW = (TW - 6) / 2;

    if (cm->posXBuf.empty() && cm->posYBuf.empty() &&
        cm->ownerIdx >= 0 && cm->ownerIdx < mgr->count)
    {
        auto& ch = mgr->characters[cm->ownerIdx];
        cm->posXBuf = std::to_string(ch.x);
        cm->posYBuf = std::to_string(ch.y);
    }

    auto renderField = [&](int fx, int fy, int fw, int fh,
                           const char* prefix, std::string& buf, bool& focus) {
        SDL_Rect fld = { fx, fy, fw, fh };
        SDL_SetRenderDrawColor(rend,
                               focus?28:18, focus?28:18, focus?65:45, 255);
        SDL_RenderFillRect(rend, &fld);
        SDL_SetRenderDrawColor(rend,
                               focus?0:55, focus?190:55, focus?255:110, 255);
        SDL_RenderDrawRect(rend, &fld);
        drawText((std::string(prefix) + buf + (focus?"|":"")).c_str(),
                 fld.x+3, fld.y+(fld.h-14)/2,WHITE);
        if (mouseClicked && isHit(fld)) {
            focus = true;
            SDL_StartTextInput();
        }
    };

    renderField(tr.x+5,      ty2, fW, 22, "X:", cm->posXBuf, cm->posXFocus);
    renderField(tr.x+5+fW+6, ty2, fW, 22, "Y:", cm->posYBuf, cm->posYFocus);

    if (mouseClicked) {
        SDL_Rect xFld = { tr.x+5,      ty2, fW, 22 };
        SDL_Rect yFld = { tr.x+5+fW+6, ty2, fW, 22 };
        if (isHit(xFld)) cm->posYFocus = false;
        if (isHit(yFld)) cm->posXFocus = false;
    }
    ty2 += 26;

    SDL_Rect apPos = { tr.x+5, ty2, TW, 22 };
    SDL_SetRenderDrawColor(rend, 45,95,45,255);
    SDL_RenderFillRect(rend, &apPos);
    drawTextC("Apply Position", apPos, WHITE);
    if (mouseClicked && isHit(apPos)) {
        if (cm->ownerIdx >= 0 && cm->ownerIdx < mgr->count) {
            auto& ch = mgr->characters[cm->ownerIdx];
            try { if (!cm->posXBuf.empty()) ch.x = std::stoi(cm->posXBuf); }
            catch(...) {}
            try { if (!cm->posYBuf.empty()) ch.y = std::stoi(cm->posYBuf); }
            catch(...) {}
        }
    }

    SDL_SetRenderDrawColor(rend, 15,15,32,255);
    SDL_RenderFillRect(rend, &ar);
    SDL_SetRenderDrawColor(rend, 40,40,100,255);
    SDL_RenderDrawRect(rend, &ar);

    int ax = ar.x + 10, ay = ar.y + 8;

    drawText("Animations:", ax, ay, WHITE); ax += 115;

    SDL_Rect recBtn = { ax, ay, 85, 22 };
    SDL_SetRenderDrawColor(rend, cm->recording?200:75, 35, 35, 255);
    SDL_RenderFillRect(rend, &recBtn);
    drawTextC(cm->recording ? "■ Stop" : "● Record", recBtn, WHITE);
    if (mouseClicked && isHit(recBtn)) {
        if (!cm->recording) cosStartRecording(cm);
        else                cosStopRecording(cm);
    }
    ax += 93;

    if (cm->recording) {
        Uint32 sec = (SDL_GetTicks() - cm->recStart) / 1000;
        std::string ri = std::to_string(sec) + "s  "
                         + std::to_string((int)cm->curRec.keyframes.size())
                         + " frames";
        drawText(ri.c_str(), ax, ay+3, {255,80,80,255});
        ax += 130;
    }

    if (cm->namingAnim) {
        SDL_Rect nf = { ax, ay, 155, 22 };
        SDL_SetRenderDrawColor(rend, 20,20,45,255);
        SDL_RenderFillRect(rend, &nf);
        SDL_SetRenderDrawColor(rend, 0,180,255,255);
        SDL_RenderDrawRect(rend, &nf);
        drawText((cm->animNameBuf + "|").c_str(), nf.x+3, nf.y+(nf.h-14)/2, WHITE);
        ax += 163;

        SDL_Rect sv = { ax, ay, 58, 22 };
        SDL_SetRenderDrawColor(rend, 20,115,20,255);
        SDL_RenderFillRect(rend, &sv);
        drawTextC("Save", sv, WHITE);
        if (mouseClicked && isHit(sv)) cosSaveAnimation(cm);
        ax += 66;
    }

    if (cm->ownerIdx >= 0) {
        auto& cc = g_cc[cm->ownerIdx];
        int chipX = ar.x + 10;
        int chipY = ar.y + 38;

        for (int ai = 0; ai < (int)cc.animations.size(); ai++) {
            SDL_Rect chip = { chipX + ai*108, chipY, 100, 22 };
            bool isSel  = (cm->selAnimIdx == ai);
            bool isPlay = (cm->playing && cm->playAnimIdx == ai);
            SDL_SetRenderDrawColor(rend,
                                   isPlay?0:isSel?55:38,
                                   isPlay?155:isSel?115:58,
                                   isPlay?38:isSel?175:78, 255);
            SDL_RenderFillRect(rend, &chip);
            if (isSel) { SDL_SetRenderDrawColor(rend,0,200,255,255); SDL_RenderDrawRect(rend,&chip); }
            drawTextC(cc.animations[ai].name.c_str(), chip, WHITE);
            if (mouseClicked && isHit(chip)) cm->selAnimIdx = ai;
        }

        if (cm->selAnimIdx >= 0 && cm->selAnimIdx < (int)cc.animations.size()) {
            int pX = ar.x + 10 + (int)cc.animations.size()*108;
            SDL_Rect playBtn = { pX, chipY, 88, 22 };
            SDL_SetRenderDrawColor(rend, cm->playing?180:38, cm->playing?70:150, 38, 255);
            SDL_RenderFillRect(rend, &playBtn);
            drawTextC(cm->playing ? "Playing..." : "Play", playBtn, WHITE);
            if (mouseClicked && isHit(playBtn) && !cm->playing)
                cosPlayAnimation(cm, cm->selAnimIdx);
        }

        for (int ai = 0; ai < (int)cc.animations.size(); ai++) {
            SDL_Rect chip = { ar.x+10 + ai*108, chipY, 100, 22 };
            std::string dur = std::to_string(cc.animations[ai].durationMs/1000) + "s";
            drawText(dur.c_str(), chip.x+2, chip.y+chip.h+2, GRAY);
        }
    }

    if (cm->playing) {
        drawText("▶ Playing animation...",
                 ar.x + ar.w - 200, ar.y + 8, {80,255,120,255});
    }
}

inline bool renderCostumesButton(SDL_Renderer* rend, TTF_Font* font,
                                 int x, int y, int w,
                                 int mouseX, int mouseY, bool mouseClicked)
{
    if (!font) return false;
    const SDL_Color WHITE = {255,255,255,255};
    SDL_Rect btn = { x, y, w, 26 };
    SDL_SetRenderDrawColor(rend, 75,35,140,255);
    SDL_RenderFillRect(rend, &btn);
    SDL_SetRenderDrawColor(rend, 140,80,220,255);
    SDL_RenderDrawRect(rend, &btn);

    SDL_Surface* s = TTF_RenderText_Blended(font, "Edit Costumes...", WHITE);
    if (s) {
        SDL_Texture* t = SDL_CreateTextureFromSurface(rend, s);
        SDL_Rect r = { btn.x+(btn.w-s->w)/2, btn.y+(btn.h-s->h)/2, s->w, s->h };
        SDL_RenderCopy(rend, t, nullptr, &r);
        SDL_DestroyTexture(t); SDL_FreeSurface(s);
    }

    return mouseClicked &&
           mouseX>=btn.x && mouseX<=btn.x+btn.w &&
           mouseY>=btn.y && mouseY<=btn.y+btn.h;
}

#endif
