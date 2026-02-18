#ifndef SCRATCH_SETT_H
#define SCRATCH_SETT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <windows.h>
#include <string>
#include <cstring>
#include <cstdio>

#define MAX_CHARACTERS        20
#define DEFAULT_BG_COUNT       2
#define CHARACTER_NAME_LEN    64
#define BG_NAME_LEN          128
#define CHAR_PANEL_THUMB_SIZE 80
#define CHAR_PANEL_PADDING    10
#define EDITOR_BRUSH_DEFAULT   5

struct Character {
    char        name[CHARACTER_NAME_LEN];
    SDL_Texture* texture;
    int         x;
    int         y;
    int         width;
    int         height;
    float       rotation;
    bool        visible;
    bool        isActive;
};

struct BackgroundState {
    SDL_Texture* currentBg;
    char         currentBgName[BG_NAME_LEN];
    SDL_Texture* defaultBgs[DEFAULT_BG_COUNT];
    char         defaultBgNames[DEFAULT_BG_COUNT][BG_NAME_LEN];
};

struct CharacterManager {
    Character characters[MAX_CHARACTERS];
    int       count;
    int       activeIndex;
};

struct BackgroundMenuState {
    bool menuOpen;
    bool showingLibrary;
    bool showingEditor;
};

struct ImageEditorState {
    SDL_Texture* canvas;
    bool         isDrawing;
    int          lastX;
    int          lastY;
    SDL_Color    penColor;
    int          penSize;
    bool         confirmed;
    bool         active;
};

inline void initBackgroundMenuState(BackgroundMenuState* menu) {
    menu->menuOpen      = false;
    menu->showingLibrary = false;
    menu->showingEditor  = false;
}

inline void toggleBackgroundMenu(BackgroundMenuState* menu) {
    menu->menuOpen = !menu->menuOpen;
    if (!menu->menuOpen) {
        menu->showingLibrary = false;
        menu->showingEditor  = false;
    }
}

inline void renderBackgroundMenu(SDL_Renderer* renderer,
                                 TTF_Font*     font,
                                 BackgroundMenuState* menu,
                                 BackgroundState*     bgState,
                                 SDL_Rect             menuButtonRect) {
    SDL_Color btnColor = {60, 60, 180, 255};
    SDL_SetRenderDrawColor(renderer, btnColor.r, btnColor.g, btnColor.b, btnColor.a);
    SDL_RenderFillRect(renderer, &menuButtonRect);
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surf = TTF_RenderText_Blended(font, "Background", white);
    if (surf) {
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_Rect textRect = {menuButtonRect.x + 5, menuButtonRect.y + 5,
                             surf->w, surf->h};
        SDL_RenderCopy(renderer, tex, NULL, &textRect);
        SDL_DestroyTexture(tex);
        SDL_FreeSurface(surf);
    }
    if (menu->menuOpen) {
        int subX = menuButtonRect.x;
        int subY = menuButtonRect.y + menuButtonRect.h + 2;
        int subW = 200;
        int subH = 30;
        char bgLabel[BG_NAME_LEN + 20];
        snprintf(bgLabel, sizeof(bgLabel), "Current: %s", bgState->currentBgName);
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 220);
        SDL_Rect nameRect = {subX, subY, subW, subH};
        SDL_RenderFillRect(renderer, &nameRect);
        SDL_Surface* nameSurf = TTF_RenderText_Blended(font, bgLabel, white);
        if (nameSurf) {
            SDL_Texture* nameTex = SDL_CreateTextureFromSurface(renderer, nameSurf);
            SDL_Rect nr = {subX + 5, subY + 5, nameSurf->w, nameSurf->h};
            SDL_RenderCopy(renderer, nameTex, NULL, &nr);
            SDL_DestroyTexture(nameTex);
            SDL_FreeSurface(nameSurf);
        }
        subY += subH + 2;
        SDL_SetRenderDrawColor(renderer, 50, 120, 50, 255);
        SDL_Rect libRect = {subX, subY, subW, subH};
        SDL_RenderFillRect(renderer, &libRect);
        SDL_Surface* libSurf = TTF_RenderText_Blended(font, "Image Library", white);
        if (libSurf) {
            SDL_Texture* libTex = SDL_CreateTextureFromSurface(renderer, libSurf);
            SDL_Rect lr = {subX + 5, subY + 5, libSurf->w, libSurf->h};
            SDL_RenderCopy(renderer, libTex, NULL, &lr);
            SDL_DestroyTexture(libTex);
            SDL_FreeSurface(libSurf);
        }
        subY += subH + 2;
        SDL_SetRenderDrawColor(renderer, 120, 80, 20, 255);
        SDL_Rect sysRect = {subX, subY, subW, subH};
        SDL_RenderFillRect(renderer, &sysRect);
        SDL_Surface* sysSurf = TTF_RenderText_Blended(font, "Load From System", white);
        if (sysSurf) {
            SDL_Texture* sysTex = SDL_CreateTextureFromSurface(renderer, sysSurf);
            SDL_Rect sr = {subX + 5, subY + 5, sysSurf->w, sysSurf->h};
            SDL_RenderCopy(renderer, sysTex, NULL, &sr);
            SDL_DestroyTexture(sysTex);
            SDL_FreeSurface(sysSurf);
        }
        subY += subH + 2;
        SDL_SetRenderDrawColor(renderer, 130, 30, 130, 255);
        SDL_Rect drawRect = {subX, subY, subW, subH};
        SDL_RenderFillRect(renderer, &drawRect);
        SDL_Surface* drawSurf = TTF_RenderText_Blended(font, "Draw Background", white);
        if (drawSurf) {
            SDL_Texture* drawTex = SDL_CreateTextureFromSurface(renderer, drawSurf);
            SDL_Rect dr = {subX + 5, subY + 5, drawSurf->w, drawSurf->h};
            SDL_RenderCopy(renderer, drawTex, NULL, &dr);
            SDL_DestroyTexture(drawTex);
            SDL_FreeSurface(drawSurf);
        }
    }
}

inline int handleBackgroundMenuClick(BackgroundMenuState* menu,
                                     SDL_Rect menuButtonRect,
                                     int mouseX, int mouseY) {
    if (mouseX >= menuButtonRect.x && mouseX <= menuButtonRect.x + menuButtonRect.w &&
        mouseY >= menuButtonRect.y && mouseY <= menuButtonRect.y + menuButtonRect.h) {
        toggleBackgroundMenu(menu);
        return 0;
    }
    if (!menu->menuOpen) {
        return 0;
    }
    int subX = menuButtonRect.x;
    int subW = 200;
    int subH = 30;
    int subY = menuButtonRect.y + menuButtonRect.h + 2 + subH + 2;
    SDL_Rect libRect = {subX, subY, subW, subH};
    if (mouseX >= libRect.x && mouseX <= libRect.x + libRect.w &&
        mouseY >= libRect.y && mouseY <= libRect.y + libRect.h) {
        menu->showingLibrary = true;
        menu->showingEditor  = false;
        return 1;
    }
    subY += subH + 2;
    SDL_Rect sysRect = {subX, subY, subW, subH};
    if (mouseX >= sysRect.x && mouseX <= sysRect.x + sysRect.w &&
        mouseY >= sysRect.y && mouseY <= sysRect.y + sysRect.h) {
        return 2;
    }
    subY += subH + 2;
    SDL_Rect drawRect = {subX, subY, subW, subH};
    if (mouseX >= drawRect.x && mouseX <= drawRect.x + drawRect.w &&
        mouseY >= drawRect.y && mouseY <= drawRect.y + drawRect.h) {
        menu->showingLibrary = false;
        menu->showingEditor  = true;
        return 3;
    }
    return 0;
}

inline void renderBackground(SDL_Renderer* renderer,BackgroundState* bgState, int windowW, int windowH) {
    if (bgState->currentBg != NULL) {
        SDL_Rect dst = {0, 0, windowW, windowH};
        SDL_RenderCopy(renderer, bgState->currentBg, NULL, &dst);
    } else {
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);
    }
}

inline void loadDefaultBackgrounds(SDL_Renderer* renderer, BackgroundState* bgState,
const char* path1, const char* name1, const char* path2, const char* name2) {
    SDL_Surface* s1 = IMG_Load(path1);
    if (s1) {
        bgState->defaultBgs[0] = SDL_CreateTextureFromSurface(renderer, s1);
        SDL_FreeSurface(s1);
    } else {
        bgState->defaultBgs[0] = NULL;
    }
    strncpy(bgState->defaultBgNames[0], name1, BG_NAME_LEN - 1);
    bgState->defaultBgNames[0][BG_NAME_LEN - 1] = '\0';
    SDL_Surface* s2 = IMG_Load(path2);
    if (s2) {
        bgState->defaultBgs[1] = SDL_CreateTextureFromSurface(renderer, s2);
        SDL_FreeSurface(s2);
    } else {
        bgState->defaultBgs[1] = NULL;
    }
    strncpy(bgState->defaultBgNames[1], name2, BG_NAME_LEN - 1);
    bgState->defaultBgNames[1][BG_NAME_LEN - 1] = '\0';
    if (bgState->currentBgName[0] == '\0') {
        strncpy(bgState->currentBgName, "None", BG_NAME_LEN - 1);
    }
}

inline int renderDefaultBackgroundLibrary(SDL_Renderer* renderer,
                                           TTF_Font*     font,
                                           BackgroundState* bgState,
                                           int panelX, int panelY,
                                           int mouseX, int mouseY,
                                           bool mouseClicked,
                                           int windowW, int windowH) {
    int selectedIndex = -1;
    int thumbW = 160;
    int thumbH = 100;
    int padding = 15;
    int labelH  = 20;
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color panelBg = {20, 20, 20, 210};
    int panelW = DEFAULT_BG_COUNT * (thumbW + padding) + padding;
    int panelH = thumbH + labelH + padding * 3;
    SDL_Rect panel = {panelX, panelY, panelW, panelH};
    SDL_SetRenderDrawColor(renderer,
                            panelBg.r, panelBg.g, panelBg.b, panelBg.a);
    SDL_RenderFillRect(renderer, &panel);
    SDL_Surface* titleSurf = TTF_RenderText_Blended(font, "Select Default Background", white);
    if (titleSurf) {
        SDL_Texture* titleTex = SDL_CreateTextureFromSurface(renderer, titleSurf);
        SDL_Rect tr = {panelX + padding, panelY + 5, titleSurf->w, titleSurf->h};
        SDL_RenderCopy(renderer, titleTex, NULL, &tr);
        SDL_DestroyTexture(titleTex);
        SDL_FreeSurface(titleSurf);
    }
    for (int i = 0; i < DEFAULT_BG_COUNT; i++) {
        int imgX = panelX + padding + i * (thumbW + padding);
        int imgY = panelY + padding + 22;
        SDL_Rect thumbRect = {imgX, imgY, thumbW, thumbH};
        if (bgState->defaultBgs[i] != NULL) {
            SDL_RenderCopy(renderer, bgState->defaultBgs[i], NULL, &thumbRect);
        } else {
            SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
            SDL_RenderFillRect(renderer, &thumbRect);
        }
        SDL_Surface* nameSurf = TTF_RenderText_Blended(font, bgState->defaultBgNames[i], white);
        if (nameSurf) {
            SDL_Texture* nameTex = SDL_CreateTextureFromSurface(renderer, nameSurf);
            SDL_Rect nr = {imgX, imgY + thumbH + 3, nameSurf->w, nameSurf->h};
            SDL_RenderCopy(renderer, nameTex, NULL, &nr);
            SDL_DestroyTexture(nameTex);
            SDL_FreeSurface(nameSurf);
        }
        if (mouseClicked &&
            mouseX >= imgX && mouseX <= imgX + thumbW &&
            mouseY >= imgY && mouseY <= imgY + thumbH) {
            selectedIndex = i;
        }
    }
    if (selectedIndex >= 0 && bgState->defaultBgs[selectedIndex] != NULL) {
        bgState->currentBg = bgState->defaultBgs[selectedIndex];
        strncpy(bgState->currentBgName, bgState->defaultBgNames[selectedIndex], BG_NAME_LEN - 1);
        bgState->currentBgName[BG_NAME_LEN - 1] = '\0';
    }
    return selectedIndex;
}

inline bool loadBackgroundFromSystem(SDL_Renderer* renderer, BackgroundState* bgState, int windowW, int windowH) {
    char filePath[MAX_PATH] = {0};
    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner   = NULL;
    ofn.lpstrFile   = filePath;
    ofn.nMaxFile    = MAX_PATH;
    ofn.lpstrFilter = "Image Files\0*.png;*.jpg;*.jpeg;*.bmp\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags       = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (!GetOpenFileNameA(&ofn)) {
        return false;
    }
    SDL_Surface* raw = IMG_Load(filePath);
    if (!raw) {
        return false;
    }
    SDL_Surface* scaled = SDL_CreateRGBSurfaceWithFormat(0, windowW, windowH,
                              32, SDL_PIXELFORMAT_RGBA32);
    if (!scaled) {
        SDL_FreeSurface(raw);
        return false;
    }
    SDL_BlitScaled(raw, NULL, scaled, NULL);
    SDL_FreeSurface(raw);
    if (bgState->currentBg != NULL &&
        bgState->currentBg != bgState->defaultBgs[0] &&
        bgState->currentBg != bgState->defaultBgs[1]) {
        SDL_DestroyTexture(bgState->currentBg);
    }
    bgState->currentBg = SDL_CreateTextureFromSurface(renderer, scaled);
    SDL_FreeSurface(scaled);
    const char* slash = strrchr(filePath, '\\');
    const char* name  = slash ? slash + 1 : filePath;
    strncpy(bgState->currentBgName, name, BG_NAME_LEN - 1);
    bgState->currentBgName[BG_NAME_LEN - 1] = '\0';
    return true;
}

inline void initImageEditor(SDL_Renderer* renderer, ImageEditorState* editor, int canvasW, int canvasH) {
    editor->canvas = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
    SDL_TEXTUREACCESS_TARGET, canvasW, canvasH);
    SDL_SetRenderTarget(renderer, editor->canvas);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);
    editor->isDrawing  = false;
    editor->lastX      = -1;
    editor->lastY      = -1;
    editor->penColor   = {0, 0, 0, 255};
    editor->penSize    = EDITOR_BRUSH_DEFAULT;
    editor->confirmed  = false;
    editor->active     = true;
}

inline void editorDrawCircle(SDL_Renderer* renderer,
                               SDL_Texture* canvas,
                               int cx, int cy, int radius,
                               SDL_Color color) {
    SDL_SetRenderTarget(renderer, canvas);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int dy = -radius; dy <= radius; dy++) {
        for (int dx = -radius; dx <= radius; dx++) {
            if (dx * dx + dy * dy <= radius * radius) {
                SDL_RenderDrawPoint(renderer, cx + dx, cy + dy);
            }
        }
    }
    SDL_SetRenderTarget(renderer, NULL);
}

inline void handleImageEditorMouse(SDL_Renderer* renderer,
                                    ImageEditorState* editor,
                                    SDL_Event* event,
                                    SDL_Rect canvasRect) {
    if (!editor->active) {
        return;
    }
    if (event->type == SDL_MOUSEBUTTONDOWN &&
        event->button.button == SDL_BUTTON_LEFT) {
        int mx = event->button.x - canvasRect.x;
        int my = event->button.y - canvasRect.y;
        if (mx >= 0 && mx < canvasRect.w &&
            my >= 0 && my < canvasRect.h) {
            editor->isDrawing = true;
            editor->lastX = mx;
            editor->lastY = my;
            editorDrawCircle(renderer, editor->canvas, mx, my, editor->penSize, editor->penColor);
        }
    }
    if (event->type == SDL_MOUSEMOTION && editor->isDrawing) {
        int mx = event->motion.x - canvasRect.x;
        int my = event->motion.y - canvasRect.y;
        if (mx >= 0 && mx < canvasRect.w &&
            my >= 0 && my < canvasRect.h) {
            int x0 = editor->lastX;
            int y0 = editor->lastY;
            int dx = abs(mx - x0);
            int dy = abs(my - y0);
            int steps = (dx > dy) ? dx : dy;
            if (steps == 0) { steps = 1; }
            for (int s = 0; s <= steps; s++) {
                int px = x0 + (mx - x0) * s / steps;
                int py = y0 + (my - y0) * s / steps;
                editorDrawCircle(renderer, editor->canvas, px, py, editor->penSize, editor->penColor);
            }
            editor->lastX = mx;
            editor->lastY = my;
        }
    }
    if (event->type == SDL_MOUSEBUTTONUP &&
        event->button.button == SDL_BUTTON_LEFT) {
        editor->isDrawing = false;
    }
}

inline void renderImageEditor(SDL_Renderer* renderer,
                               TTF_Font*     font,
                               ImageEditorState* editor,
                               SDL_Rect canvasRect,
                               int mouseX, int mouseY,
                               bool mouseClicked) {
    if (!editor->active) {
        return;
    }
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_Rect border = {canvasRect.x - 2, canvasRect.y - 2, canvasRect.w + 4, canvasRect.h + 4};
    SDL_RenderFillRect(renderer, &border);
    SDL_RenderCopy(renderer, editor->canvas, NULL, &canvasRect);
    int toolY = canvasRect.y + canvasRect.h + 8;
    int toolX = canvasRect.x;
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color black = {0,   0,   0,   255};
    SDL_Color palette[] = {
        {0,   0,   0,   255},
        {255, 0,   0,   255},
        {0,   128, 0,   255},
        {0,   0,   255, 255},
        {255, 255, 0,   255},
        {255, 165, 0,   255},
        {128, 0,   128, 255},
        {255, 255, 255, 255}
    };
    int paletteCount = 8;
    int swatchSize   = 24;
    for (int i = 0; i < paletteCount; i++) {
        SDL_Rect swatch = {toolX + i * (swatchSize + 4), toolY,
                            swatchSize, swatchSize};
        SDL_SetRenderDrawColor(renderer, palette[i].r, palette[i].g, palette[i].b, palette[i].a);
        SDL_RenderFillRect(renderer, &swatch);
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        SDL_RenderDrawRect(renderer, &swatch);
        if (mouseClicked &&
            mouseX >= swatch.x && mouseX <= swatch.x + swatch.w &&
            mouseY >= swatch.y && mouseY <= swatch.y + swatch.h) {
            editor->penColor = palette[i];
        }
    }
    int afterPalette = toolX + paletteCount * (swatchSize + 4) + 10;
    SDL_Rect minusBtn = {afterPalette, toolY, 26, 26};
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderFillRect(renderer, &minusBtn);
    SDL_Surface* minSurf = TTF_RenderText_Blended(font, "-", white);
    if (minSurf) {
        SDL_Texture* minTex = SDL_CreateTextureFromSurface(renderer, minSurf);
        SDL_Rect mr = {minusBtn.x + 7, minusBtn.y + 3, minSurf->w, minSurf->h};
        SDL_RenderCopy(renderer, minTex, NULL, &mr);
        SDL_DestroyTexture(minTex);
        SDL_FreeSurface(minSurf);
    }
    if (mouseClicked &&
        mouseX >= minusBtn.x && mouseX <= minusBtn.x + minusBtn.w &&
        mouseY >= minusBtn.y && mouseY <= minusBtn.y + minusBtn.h) {
        if (editor->penSize > 1) {
            editor->penSize--;
        }
    }
    SDL_Rect plusBtn = {afterPalette + 30, toolY, 26, 26};
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderFillRect(renderer, &plusBtn);
    SDL_Surface* plusSurf = TTF_RenderText_Blended(font, "+", white);
    if (plusSurf) {
        SDL_Texture* plusTex = SDL_CreateTextureFromSurface(renderer, plusSurf);
        SDL_Rect pr = {plusBtn.x + 5, plusBtn.y + 3, plusSurf->w, plusSurf->h};
        SDL_RenderCopy(renderer, plusTex, NULL, &pr);
        SDL_DestroyTexture(plusTex);
        SDL_FreeSurface(plusSurf);
    }
    if (mouseClicked &&
        mouseX >= plusBtn.x && mouseX <= plusBtn.x + plusBtn.w &&
        mouseY >= plusBtn.y && mouseY <= plusBtn.y + plusBtn.h) {
        if (editor->penSize < 40) {
            editor->penSize++;
        }
    }
    char sizeLabel[32];
    snprintf(sizeLabel, sizeof(sizeLabel), "Brush: %d", editor->penSize);
    SDL_Surface* szSurf = TTF_RenderText_Blended(font, sizeLabel, white);
    if (szSurf) {
        SDL_Texture* szTex = SDL_CreateTextureFromSurface(renderer, szSurf);
        SDL_Rect szr = {afterPalette + 62, toolY + 4, szSurf->w, szSurf->h};
        SDL_RenderCopy(renderer, szTex, NULL, &szr);
        SDL_DestroyTexture(szTex);
        SDL_FreeSurface(szSurf);
    }
    int confirmX = canvasRect.x + canvasRect.w - 180;
    int confirmY = toolY;
    SDL_Rect confirmBtn = {confirmX, confirmY, 80, 28};
    SDL_SetRenderDrawColor(renderer, 20, 160, 20, 255);
    SDL_RenderFillRect(renderer, &confirmBtn);
    SDL_Surface* confSurf = TTF_RenderText_Blended(font, "Confirm", white);
    if (confSurf) {
        SDL_Texture* confTex = SDL_CreateTextureFromSurface(renderer, confSurf);
        SDL_Rect cr = {confirmBtn.x + 5, confirmBtn.y + 5, confSurf->w, confSurf->h};
        SDL_RenderCopy(renderer, confTex, NULL, &cr);
        SDL_DestroyTexture(confTex);
        SDL_FreeSurface(confSurf);
    }
    if (mouseClicked &&
        mouseX >= confirmBtn.x && mouseX <= confirmBtn.x + confirmBtn.w &&
        mouseY >= confirmBtn.y && mouseY <= confirmBtn.y + confirmBtn.h) {
        editor->confirmed = true;
        editor->active    = false;
    }
    SDL_Rect cancelBtn = {confirmX + 90, confirmY, 80, 28};
    SDL_SetRenderDrawColor(renderer, 180, 20, 20, 255);
    SDL_RenderFillRect(renderer, &cancelBtn);
    SDL_Surface* canSurf = TTF_RenderText_Blended(font, "Cancel", white);
    if (canSurf) {
        SDL_Texture* canTex = SDL_CreateTextureFromSurface(renderer, canSurf);
        SDL_Rect cnr = {cancelBtn.x + 5, cancelBtn.y + 5,
                         canSurf->w, canSurf->h};
        SDL_RenderCopy(renderer, canTex, NULL, &cnr);
        SDL_DestroyTexture(canTex);
        SDL_FreeSurface(canSurf);
    }
    if (mouseClicked &&
        mouseX >= cancelBtn.x && mouseX <= cancelBtn.x + cancelBtn.w &&
        mouseY >= cancelBtn.y && mouseY <= cancelBtn.y + cancelBtn.h) {
        editor->confirmed = false;
        editor->active    = false;
    }
}

inline void applyEditorCanvasAsBackground(SDL_Renderer* renderer,
                                           BackgroundState* bgState,
                                           ImageEditorState* editor,
                                           int windowW, int windowH) {
    if (!editor->confirmed) {
        return;
    }
    SDL_Texture* finalTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
    SDL_TEXTUREACCESS_TARGET, windowW, windowH);
    SDL_SetRenderTarget(renderer, finalTex);
    SDL_RenderCopy(renderer, editor->canvas, NULL, NULL);
    SDL_SetRenderTarget(renderer, NULL);
    if (bgState->currentBg != NULL &&
        bgState->currentBg != bgState->defaultBgs[0] &&
        bgState->currentBg != bgState->defaultBgs[1]) {
        SDL_DestroyTexture(bgState->currentBg);
    }
    bgState->currentBg = finalTex;
    strncpy(bgState->currentBgName, "Custom Drawing", BG_NAME_LEN - 1);
    bgState->currentBgName[BG_NAME_LEN - 1] = '\0';
    SDL_DestroyTexture(editor->canvas);
    editor->canvas    = NULL;
    editor->confirmed = false;
}

inline void initCharacterManager(CharacterManager* mgr) {
    mgr->count       = 0;
    mgr->activeIndex = -1;
    for (int i = 0; i < MAX_CHARACTERS; i++) {
        mgr->characters[i].texture   = NULL;
        mgr->characters[i].visible   = true;
        mgr->characters[i].isActive  = false;
        mgr->characters[i].x        = 0;
        mgr->characters[i].y        = 0;
        mgr->characters[i].width    = 100;
        mgr->characters[i].height   = 100;
        mgr->characters[i].rotation = 0.0f;
        mgr->characters[i].name[0]  = '\0';
    }
}

inline int addCharacterFromFile(SDL_Renderer* renderer, CharacterManager* mgr, const char* imagePath, const char* name) {
    if (mgr->count >= MAX_CHARACTERS) {
        return -1;
    }
    SDL_Surface* surf = IMG_Load(imagePath);
    if (!surf) {
        return -1;
    }
    int idx = mgr->count;
    mgr->characters[idx].texture  = SDL_CreateTextureFromSurface(renderer, surf);
    mgr->characters[idx].width    = surf->w;
    mgr->characters[idx].height   = surf->h;
    SDL_FreeSurface(surf);
    mgr->characters[idx].x        = 100;
    mgr->characters[idx].y        = 100;
    mgr->characters[idx].rotation = 0.0f;
    mgr->characters[idx].visible  = true;
    mgr->characters[idx].isActive = false;
    strncpy(mgr->characters[idx].name, name, CHARACTER_NAME_LEN - 1);
    mgr->characters[idx].name[CHARACTER_NAME_LEN - 1] = '\0';
    mgr->count++;
    return idx;
}

inline int addCharacterFromSystem(SDL_Renderer* renderer, CharacterManager* mgr) {
    char filePath[MAX_PATH] = {0};
    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner   = NULL;
    ofn.lpstrFile   = filePath;
    ofn.nMaxFile    = MAX_PATH;
    ofn.lpstrFilter = "Image Files\0*.png;*.jpg;*.jpeg;*.bmp\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (!GetOpenFileNameA(&ofn)) {
        return -1;
    }
    const char* slash = strrchr(filePath, '\\');
    const char* name  = slash ? slash + 1 : filePath;
    return addCharacterFromFile(renderer, mgr, filePath, name);
}

inline int addCharacterFromLibrary(SDL_Renderer* renderer,
                                    CharacterManager* mgr,
                                    const char* libraryImagePath,
                                    const char* characterName) {
    return addCharacterFromFile(renderer, mgr, libraryImagePath, characterName);
}

inline int addRandomCharacter(SDL_Renderer* renderer,
                               CharacterManager* mgr,
                               const char** libraryPaths,
                               const char** libraryNames,
                               int librarySize) {
    if (librarySize <= 0) {
        return -1;
    }
    int pick = rand() % librarySize;
    return addCharacterFromFile(renderer, mgr, libraryPaths[pick], libraryNames[pick]);
}

inline void setActiveCharacter(CharacterManager* mgr, int index) {
    if (index < 0 || index >= mgr->count) {
        return;
    }
    if (mgr->activeIndex >= 0 && mgr->activeIndex < mgr->count) {
        mgr->characters[mgr->activeIndex].isActive = false;
    }
    mgr->activeIndex = index;
    mgr->characters[index].isActive = true;
}

inline void renameCharacter(CharacterManager* mgr, int index, const char* newName) {
    if (index < 0 || index >= mgr->count) {
        return;
    }
    strncpy(mgr->characters[index].name, newName, CHARACTER_NAME_LEN - 1);
    mgr->characters[index].name[CHARACTER_NAME_LEN - 1] = '\0';
}

inline void setCharacterPosition(CharacterManager* mgr, int index, int x, int y) {
    if (index < 0 || index >= mgr->count) {
        return;
    }
    mgr->characters[index].x = x;
    mgr->characters[index].y = y;
}

inline bool handleCharacterDrag(CharacterManager* mgr,
                                  int mouseX, int mouseY,
                                  bool mouseDown,
                                  bool mouseJustPressed,
                                  int* dragIndex) {
    if (mouseJustPressed) {
        *dragIndex = -1;
        for (int i = 0; i < mgr->count; i++) {
            Character* c = &mgr->characters[i];
            if (!c->visible) { continue; }
            SDL_Rect bounds = {c->x, c->y, c->width, c->height};
            if (mouseX >= bounds.x && mouseX <= bounds.x + bounds.w &&
                mouseY >= bounds.y && mouseY <= bounds.y + bounds.h) {
                *dragIndex = i;
                setActiveCharacter(mgr, i);
                break;
            }
        }
    }
    if (mouseDown && *dragIndex >= 0 && *dragIndex < mgr->count) {
        Character* c = &mgr->characters[*dragIndex];
        c->x = mouseX - c->width  / 2;
        c->y = mouseY - c->height / 2;
        return true;
    }
    if (!mouseDown) {
        *dragIndex = -1;
    }
    return false;
}

inline void setCharacterScale(CharacterManager* mgr, int index, float scale, int originalW, int originalH) {
    if (index < 0 || index >= mgr->count) {
        return;
    }
    if (scale <= 0.0f) { scale = 0.01f; }
    mgr->characters[index].width  = (int)(originalW * scale);
    mgr->characters[index].height = (int)(originalH * scale);
}

inline void setCharacterRotation(CharacterManager* mgr, int index, float angleDegrees) {
    if (index < 0 || index >= mgr->count) {
        return;
    }
    mgr->characters[index].rotation = angleDegrees;
}

inline void toggleCharacterVisibility(CharacterManager* mgr, int index) {
    if (index < 0 || index >= mgr->count) {
        return;
    }
    mgr->characters[index].visible = !mgr->characters[index].visible;
}

inline void setCharacterVisible(CharacterManager* mgr, int index, bool visible) {
    if (index < 0 || index >= mgr->count) {
        return;
    }
    mgr->characters[index].visible = visible;
}

inline bool deleteCharacter(CharacterManager* mgr, int index) {
    if (index < 0 || index >= mgr->count) {
        return false;
    }
    char warningMsg[128];
    snprintf(warningMsg, sizeof(warningMsg),
             "Are you sure you want to delete character \"%s\"?\nThis action cannot be undone.",
             mgr->characters[index].name);
    int result = MessageBoxA(NULL, warningMsg, "Delete Character",
                              MB_YESNO | MB_ICONWARNING);
    if (result != IDYES) {
        return false;
    }
    if (mgr->characters[index].texture != NULL) {
        SDL_DestroyTexture(mgr->characters[index].texture);
        mgr->characters[index].texture = NULL;
    }
    for (int i = index; i < mgr->count - 1; i++) {
        mgr->characters[i] = mgr->characters[i + 1];
    }
    mgr->count--;
    if (mgr->activeIndex == index) {
        mgr->activeIndex = (mgr->count > 0) ? 0 : -1;
        if (mgr->activeIndex >= 0) {
            mgr->characters[mgr->activeIndex].isActive = true;
        }
    } else if (mgr->activeIndex > index) {
        mgr->activeIndex--;
    }
    return true;
}

inline void renderCharacterPanel(SDL_Renderer* renderer,
                                   TTF_Font*     font,
                                   CharacterManager* mgr,
                                   SDL_Rect panelRect,
                                   int mouseX, int mouseY,
                                   bool mouseClicked) {
    SDL_Color panelBg   = {25, 25, 25, 240};
    SDL_Color activeBdr = {0, 200, 255, 255};
    SDL_Color normalBdr = {80, 80, 80, 255};
    SDL_Color white     = {255, 255, 255, 255};
    SDL_SetRenderDrawColor(renderer,
                            panelBg.r, panelBg.g, panelBg.b, panelBg.a);
    SDL_RenderFillRect(renderer, &panelRect);
    SDL_Surface* titleSurf = TTF_RenderText_Blended(font, "Characters", white);
    if (titleSurf) {
        SDL_Texture* titleTex = SDL_CreateTextureFromSurface(renderer, titleSurf);
        SDL_Rect tr = {panelRect.x + 5, panelRect.y + 4, titleSurf->w, titleSurf->h};
        SDL_RenderCopy(renderer, titleTex, NULL, &tr);
        SDL_DestroyTexture(titleTex);
        SDL_FreeSurface(titleSurf);
    }
    int startY   = panelRect.y + 28;
    int thumbSize = CHAR_PANEL_THUMB_SIZE;
    int padding  = CHAR_PANEL_PADDING;
    for (int i = 0; i < mgr->count; i++) {
        Character* c = &mgr->characters[i];
        int thumbX = panelRect.x + padding;
        int thumbY = startY + i * (thumbSize + padding + 20);
        if (thumbY + thumbSize > panelRect.y + panelRect.h) {
            break;
        }
        SDL_Rect thumbRect = {thumbX, thumbY, thumbSize, thumbSize};
        if (c->isActive) {
            SDL_SetRenderDrawColor(renderer, activeBdr.r, activeBdr.g, activeBdr.b, activeBdr.a);
        } else {
            SDL_SetRenderDrawColor(renderer, normalBdr.r, normalBdr.g, normalBdr.b, normalBdr.a);
        }
        SDL_Rect borderRect = {thumbX - 2, thumbY - 2, thumbSize + 4, thumbSize + 4};
        SDL_RenderFillRect(renderer, &borderRect);
        if (c->texture != NULL) {
            Uint8 alpha = c->visible ? 255 : 100;
            SDL_SetTextureAlphaMod(c->texture, alpha);
            SDL_RenderCopy(renderer, c->texture, NULL, &thumbRect);
            SDL_SetTextureAlphaMod(c->texture, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
            SDL_RenderFillRect(renderer, &thumbRect);
        }
        SDL_Surface* nameSurf = TTF_RenderText_Blended(font, c->name, white);
        if (nameSurf) {
            SDL_Texture* nameTex = SDL_CreateTextureFromSurface(renderer, nameSurf);
            int nameX = thumbX;
            int nameY = thumbY + thumbSize + 2;
            SDL_Rect nr = {nameX, nameY, nameSurf->w, nameSurf->h};
            SDL_RenderCopy(renderer, nameTex, NULL, &nr);
            SDL_DestroyTexture(nameTex);
            SDL_FreeSurface(nameSurf);
        }
        if (!c->visible) {
            SDL_Color hiddenColor = {180, 50, 50, 255};
            SDL_Surface* hideSurf = TTF_RenderText_Blended(font, "[hidden]", hiddenColor);
            if (hideSurf) {
                SDL_Texture* hideTex = SDL_CreateTextureFromSurface(renderer, hideSurf);
                SDL_Rect hr = {thumbX + thumbSize - hideSurf->w - 2,
                                thumbY + 2, hideSurf->w, hideSurf->h};
                SDL_RenderCopy(renderer, hideTex, NULL, &hr);
                SDL_DestroyTexture(hideTex);
                SDL_FreeSurface(hideSurf);
            }
        }
        if (mouseClicked &&
            mouseX >= thumbX && mouseX <= thumbX + thumbSize &&
            mouseY >= thumbY && mouseY <= thumbY + thumbSize) {
            setActiveCharacter(mgr, i);
        }
    }
}

inline void renderCharacterSettingsPanel(SDL_Renderer* renderer,
                                          TTF_Font*     font,
                                          CharacterManager* mgr,
                                          SDL_Rect panelRect,
                                          int mouseX, int mouseY,
                                          bool mouseClicked,
                                          bool* requestAddChar,
                                          bool* requestRename,
                                          char* renameBuffer,
                                          int   renameBufferLen) {
    SDL_Color panelBg = {30, 30, 30, 240};
    SDL_Color white   = {255, 255, 255, 255};
    SDL_Color gray    = {160, 160, 160, 255};
    SDL_SetRenderDrawColor(renderer,
                            panelBg.r, panelBg.g, panelBg.b, panelBg.a);
    SDL_RenderFillRect(renderer, &panelRect);
    int cx = panelRect.x + 10;
    int cy = panelRect.y + 10;
    int lineH = 26;
    SDL_Surface* hdrSurf = TTF_RenderText_Blended(font,
                                "Character Settings", white);
    if (hdrSurf) {
        SDL_Texture* hdrTex = SDL_CreateTextureFromSurface(renderer, hdrSurf);
        SDL_Rect hr = {cx, cy, hdrSurf->w, hdrSurf->h};
        SDL_RenderCopy(renderer, hdrTex, NULL, &hr);
        SDL_DestroyTexture(hdrTex);
        SDL_FreeSurface(hdrSurf);
    }
    cy += lineH + 4;
    SDL_Rect addBtn = {cx, cy, 130, 24};
    SDL_SetRenderDrawColor(renderer, 20, 120, 20, 255);
    SDL_RenderFillRect(renderer, &addBtn);
    SDL_Surface* addSurf = TTF_RenderText_Blended(font, "+ Add Character", white);
    if (addSurf) {
        SDL_Texture* addTex = SDL_CreateTextureFromSurface(renderer, addSurf);
        SDL_Rect ar = {addBtn.x + 4, addBtn.y + 4, addSurf->w, addSurf->h};
        SDL_RenderCopy(renderer, addTex, NULL, &ar);
        SDL_DestroyTexture(addTex);
        SDL_FreeSurface(addSurf);
    }
    if (mouseClicked &&
        mouseX >= addBtn.x && mouseX <= addBtn.x + addBtn.w &&
        mouseY >= addBtn.y && mouseY <= addBtn.y + addBtn.h) {
        *requestAddChar = true;
    }
    cy += lineH + 8;
    if (mgr->activeIndex < 0 || mgr->activeIndex >= mgr->count) {
        SDL_Surface* noSurf = TTF_RenderText_Blended(font, "No character selected.", gray);
        if (noSurf) {
            SDL_Texture* noTex = SDL_CreateTextureFromSurface(renderer, noSurf);
            SDL_Rect nr = {cx, cy, noSurf->w, noSurf->h};
            SDL_RenderCopy(renderer, noTex, NULL, &nr);
            SDL_DestroyTexture(noTex);
            SDL_FreeSurface(noSurf);
        }
        return;
    }
    Character* active = &mgr->characters[mgr->activeIndex];
    char nameLabel[CHARACTER_NAME_LEN + 10];
    snprintf(nameLabel, sizeof(nameLabel), "Name: %s", active->name);
    SDL_Surface* nameSurf = TTF_RenderText_Blended(font, nameLabel, white);
    if (nameSurf) {
        SDL_Texture* nameTex = SDL_CreateTextureFromSurface(renderer, nameSurf);
        SDL_Rect nr = {cx, cy, nameSurf->w, nameSurf->h};
        SDL_RenderCopy(renderer, nameTex, NULL, &nr);
        SDL_DestroyTexture(nameTex);
        SDL_FreeSurface(nameSurf);
    }
    cy += lineH;
    SDL_Rect renameBtn = {cx, cy, 80, 22};
    SDL_SetRenderDrawColor(renderer, 70, 70, 150, 255);
    SDL_RenderFillRect(renderer, &renameBtn);
    SDL_Surface* renSurf = TTF_RenderText_Blended(font, "Rename", white);
    if (renSurf) {
        SDL_Texture* renTex = SDL_CreateTextureFromSurface(renderer, renSurf);
        SDL_Rect rr = {renameBtn.x + 4, renameBtn.y + 3, renSurf->w, renSurf->h};
        SDL_RenderCopy(renderer, renTex, NULL, &rr);
        SDL_DestroyTexture(renTex);
        SDL_FreeSurface(renSurf);
    }
    if (mouseClicked &&
        mouseX >= renameBtn.x && mouseX <= renameBtn.x + renameBtn.w &&
        mouseY >= renameBtn.y && mouseY <= renameBtn.y + renameBtn.h) {
        *requestRename = true;
        strncpy(renameBuffer, active->name, renameBufferLen - 1);
        renameBuffer[renameBufferLen - 1] = '\0';
    }
    cy += lineH + 4;
    char posLabel[64];
    snprintf(posLabel, sizeof(posLabel), "X: %d    Y: %d", active->x, active->y);
    SDL_Surface* posSurf = TTF_RenderText_Blended(font, posLabel, white);
    if (posSurf) {
        SDL_Texture* posTex = SDL_CreateTextureFromSurface(renderer, posSurf);
        SDL_Rect pr = {cx, cy, posSurf->w, posSurf->h};
        SDL_RenderCopy(renderer, posTex, NULL, &pr);
        SDL_DestroyTexture(posTex);
        SDL_FreeSurface(posSurf);
    }
    cy += lineH;
    char sizeLabel[64];
    snprintf(sizeLabel, sizeof(sizeLabel),
             "W: %d   H: %d   Rot: %.1f°",
             active->width, active->height, active->rotation);
    SDL_Surface* sizeSurf = TTF_RenderText_Blended(font, sizeLabel, white);
    if (sizeSurf) {
        SDL_Texture* sizeTex = SDL_CreateTextureFromSurface(renderer, sizeSurf);
        SDL_Rect sr = {cx, cy, sizeSurf->w, sizeSurf->h};
        SDL_RenderCopy(renderer, sizeTex, NULL, &sr);
        SDL_DestroyTexture(sizeTex);
        SDL_FreeSurface(sizeSurf);
    }
    cy += lineH;
    SDL_Rect rotMinusBtn = {cx, cy, 28, 22};
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderFillRect(renderer, &rotMinusBtn);
    SDL_Surface* rmSurf = TTF_RenderText_Blended(font, "-", white);
    if (rmSurf) {
        SDL_Texture* rmTex = SDL_CreateTextureFromSurface(renderer, rmSurf);
        SDL_Rect rm = {rotMinusBtn.x + 8, rotMinusBtn.y + 3,
                        rmSurf->w, rmSurf->h};
        SDL_RenderCopy(renderer, rmTex, NULL, &rm);
        SDL_DestroyTexture(rmTex);
        SDL_FreeSurface(rmSurf);
    }
    if (mouseClicked &&
        mouseX >= rotMinusBtn.x && mouseX <= rotMinusBtn.x + rotMinusBtn.w &&
        mouseY >= rotMinusBtn.y && mouseY <= rotMinusBtn.y + rotMinusBtn.h) {
        active->rotation -= 5.0f;
    }
    SDL_Rect rotLabel = {cx + 34, cy, 50, 22};
    SDL_Surface* rotSurf = TTF_RenderText_Blended(font, "Rotate", gray);
    if (rotSurf) {
        SDL_Texture* rotTex = SDL_CreateTextureFromSurface(renderer, rotSurf);
        SDL_Rect rl = {rotLabel.x, rotLabel.y + 3, rotSurf->w, rotSurf->h};
        SDL_RenderCopy(renderer, rotTex, NULL, &rl);
        SDL_DestroyTexture(rotTex);
        SDL_FreeSurface(rotSurf);
    }
    SDL_Rect rotPlusBtn = {cx + 90, cy, 28, 22};
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderFillRect(renderer, &rotPlusBtn);
    SDL_Surface* rpSurf = TTF_RenderText_Blended(font, "+", white);
    if (rpSurf) {
        SDL_Texture* rpTex = SDL_CreateTextureFromSurface(renderer, rpSurf);
        SDL_Rect rp = {rotPlusBtn.x + 6, rotPlusBtn.y + 3,
                        rpSurf->w, rpSurf->h};
        SDL_RenderCopy(renderer, rpTex, NULL, &rp);
        SDL_DestroyTexture(rpTex);
        SDL_FreeSurface(rpSurf);
    }
    if (mouseClicked &&
        mouseX >= rotPlusBtn.x && mouseX <= rotPlusBtn.x + rotPlusBtn.w &&
        mouseY >= rotPlusBtn.y && mouseY <= rotPlusBtn.y + rotPlusBtn.h) {
        active->rotation += 5.0f;
    }
    cy += lineH;
    SDL_Rect sizeMinusBtn = {cx, cy, 28, 22};
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderFillRect(renderer, &sizeMinusBtn);
    SDL_Surface* smSurf = TTF_RenderText_Blended(font, "-", white);
    if (smSurf) {
        SDL_Texture* smTex = SDL_CreateTextureFromSurface(renderer, smSurf);
        SDL_Rect sm = {sizeMinusBtn.x + 8, sizeMinusBtn.y + 3,
                        smSurf->w, smSurf->h};
        SDL_RenderCopy(renderer, smTex, NULL, &sm);
        SDL_DestroyTexture(smTex);
        SDL_FreeSurface(smSurf);
    }
    if (mouseClicked &&
        mouseX >= sizeMinusBtn.x && mouseX <= sizeMinusBtn.x + sizeMinusBtn.w &&
        mouseY >= sizeMinusBtn.y && mouseY <= sizeMinusBtn.y + sizeMinusBtn.h) {
        if (active->width  > 10) { active->width  -= 5; }
        if (active->height > 10) { active->height -= 5; }
    }
    SDL_Surface* sizeLblSurf = TTF_RenderText_Blended(font, "Size", gray);
    if (sizeLblSurf) {
        SDL_Texture* sizeLblTex = SDL_CreateTextureFromSurface(renderer,
                                                                 sizeLblSurf);
        SDL_Rect sl = {cx + 34, cy + 3, sizeLblSurf->w, sizeLblSurf->h};
        SDL_RenderCopy(renderer, sizeLblTex, NULL, &sl);
        SDL_DestroyTexture(sizeLblTex);
        SDL_FreeSurface(sizeLblSurf);
    }
    SDL_Rect sizePlusBtn = {cx + 90, cy, 28, 22};
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderFillRect(renderer, &sizePlusBtn);
    SDL_Surface* spSurf = TTF_RenderText_Blended(font, "+", white);
    if (spSurf) {
        SDL_Texture* spTex = SDL_CreateTextureFromSurface(renderer, spSurf);
        SDL_Rect sp = {sizePlusBtn.x + 6, sizePlusBtn.y + 3,
                        spSurf->w, spSurf->h};
        SDL_RenderCopy(renderer, spTex, NULL, &sp);
        SDL_DestroyTexture(spTex);
        SDL_FreeSurface(spSurf);
    }
    if (mouseClicked &&
        mouseX >= sizePlusBtn.x && mouseX <= sizePlusBtn.x + sizePlusBtn.w &&
        mouseY >= sizePlusBtn.y && mouseY <= sizePlusBtn.y + sizePlusBtn.h) {
        active->width  += 5;
        active->height += 5;
    }
    cy += lineH + 4;
    const char* visLabel = active->visible ? "Hide Character" : "Show Character";
    SDL_Rect visBtn = {cx, cy, 130, 24};
    SDL_SetRenderDrawColor(renderer,
                            active->visible ? 120 : 30,
                            active->visible ? 80  : 120,
                            80, 255);
    SDL_RenderFillRect(renderer, &visBtn);
    SDL_Surface* visSurf = TTF_RenderText_Blended(font, visLabel, white);
    if (visSurf) {
        SDL_Texture* visTex = SDL_CreateTextureFromSurface(renderer, visSurf);
        SDL_Rect vr = {visBtn.x + 5, visBtn.y + 4, visSurf->w, visSurf->h};
        SDL_RenderCopy(renderer, visTex, NULL, &vr);
        SDL_DestroyTexture(visTex);
        SDL_FreeSurface(visSurf);
    }
    if (mouseClicked &&
        mouseX >= visBtn.x && mouseX <= visBtn.x + visBtn.w &&
        mouseY >= visBtn.y && mouseY <= visBtn.y + visBtn.h) {
        toggleCharacterVisibility(mgr, mgr->activeIndex);
    }
    cy += lineH + 4;
    SDL_Rect delBtn = {cx, cy, 130, 24};
    SDL_SetRenderDrawColor(renderer, 180, 30, 30, 255);
    SDL_RenderFillRect(renderer, &delBtn);
    SDL_Surface* delSurf = TTF_RenderText_Blended(font, "Delete Character", white);
    if (delSurf) {
        SDL_Texture* delTex = SDL_CreateTextureFromSurface(renderer, delSurf);
        SDL_Rect dr = {delBtn.x + 5, delBtn.y + 4, delSurf->w, delSurf->h};
        SDL_RenderCopy(renderer, delTex, NULL, &dr);
        SDL_DestroyTexture(delTex);
        SDL_FreeSurface(delSurf);
    }
    if (mouseClicked &&
        mouseX >= delBtn.x && mouseX <= delBtn.x + delBtn.w &&
        mouseY >= delBtn.y && mouseY <= delBtn.y + delBtn.h) {
        deleteCharacter(mgr, mgr->activeIndex);
    }
}

inline void renderAllCharacters(SDL_Renderer* renderer,
                                  CharacterManager* mgr) {
    for (int i = 0; i < mgr->count; i++) {
        Character* c = &mgr->characters[i];
        if (!c->visible || c->texture == NULL) {
            continue;
        }
        SDL_Rect dst = {c->x, c->y, c->width, c->height};
        SDL_Point center = {c->width / 2, c->height / 2};
        SDL_RenderCopyEx(renderer, c->texture, NULL, &dst,
                          (double)c->rotation, &center, SDL_FLIP_NONE);
        if (c->isActive) {
            SDL_SetRenderDrawColor(renderer, 0, 200, 255, 255);
            SDL_Rect outline = {c->x - 2, c->y - 2,
                                 c->width + 4, c->height + 4};
            SDL_RenderDrawRect(renderer, &outline);
        }
    }
}

inline void destroyCharacterManager(CharacterManager* mgr) {
    for (int i = 0; i < mgr->count; i++) {
        if (mgr->characters[i].texture != NULL) {
            SDL_DestroyTexture(mgr->characters[i].texture);
            mgr->characters[i].texture = NULL;
        }
    }
    mgr->count       = 0;
    mgr->activeIndex = -1;
}

inline void destroyBackgroundState(BackgroundState* bgState) {
    for (int i = 0; i < DEFAULT_BG_COUNT; i++) {
        if (bgState->defaultBgs[i] != NULL) {
            SDL_DestroyTexture(bgState->defaultBgs[i]);
            bgState->defaultBgs[i] = NULL;
        }
    }
    bool isDefault = false;
    for (int i = 0; i < DEFAULT_BG_COUNT; i++) {
        if (bgState->currentBg == bgState->defaultBgs[i]) {
            isDefault = true;
            break;
        }
    }
    if (!isDefault && bgState->currentBg != NULL) {
        SDL_DestroyTexture(bgState->currentBg);
    }
    bgState->currentBg = NULL;
}

#endif
