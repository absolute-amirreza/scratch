#ifndef SCRATCH_SETT_H
#define SCRATCH_SETT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <windows.h>
#include <string>
#include <algorithm>
using namespace std;

constexpr int MAX_CHARACTERS = 20;
constexpr int DEFAULT_BG_COUNT = 2;
constexpr int CHAR_PANEL_THUMB_SIZE = 80;
constexpr int CHAR_PANEL_PADDING = 10;
constexpr int EDITOR_BRUSH_DEFAULT = 5;

struct Character {
    std::string name;
    SDL_Texture* texture = nullptr;
    int x = 0, y = 0, width = 100, height = 100;
    float rotation = 0.0f;
    bool visible = true, isActive = false;
};
struct BackgroundState {
    SDL_Texture* currentBg = nullptr;
    std::string currentBgName;
    SDL_Texture* defaultBgs[DEFAULT_BG_COUNT] = {nullptr, nullptr};
    std::string defaultBgNames[DEFAULT_BG_COUNT];
};

struct CharacterManager {
    Character characters[MAX_CHARACTERS];
    int count = 0, activeIndex = -1;
};

struct BackgroundMenuState {
    bool menuOpen = false, showingLibrary = false, showingEditor = false;
};

struct CharacterBarState {
    bool panelOpen = false;
};

struct ImageEditorState {
    SDL_Texture* canvas = nullptr;
    bool isDrawing = false;
    int lastX = -1, lastY = -1;
    SDL_Color penColor = {0, 0, 0, 255};
    int penSize = EDITOR_BRUSH_DEFAULT;
    bool confirmed = false, active = false;
};

inline void setActiveCharacter(CharacterManager* mgr, int index);
inline int  addOrangeSquareCharacter(SDL_Renderer* renderer, CharacterManager* mgr);

inline void initCharacterBarState(CharacterBarState* cb) {
    cb->panelOpen = false;
}

inline void renderCharacterBar(SDL_Renderer* renderer, TTF_Font* font,
                               CharacterBarState* cb, SDL_Rect barRect) {
    constexpr SDL_Color white = {255, 255, 255, 255};

    SDL_SetRenderDrawColor(renderer, 25, 25, 50, 255);
    SDL_RenderFillRect(renderer, &barRect);
    SDL_SetRenderDrawColor(renderer, 70, 70, 140, 255);
    SDL_RenderDrawRect(renderer, &barRect);

    SDL_Rect btnRect = {barRect.x + 5, barRect.y + 10, barRect.w - 10, 90};
    SDL_SetRenderDrawColor(renderer,
                           cb->panelOpen ? 100 : 60,
                           cb->panelOpen ? 60  : 60,
                           cb->panelOpen ? 220 : 180, 255);
    SDL_RenderFillRect(renderer, &btnRect);

    SDL_Surface* surf = TTF_RenderText_Blended(font, "CH", white);
    if (surf) {
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_Rect tr = {btnRect.x + (btnRect.w - surf->w) / 2,
                       btnRect.y + (btnRect.h - surf->h) / 2, surf->w, surf->h};
        SDL_RenderCopy(renderer, tex, nullptr, &tr);
        SDL_DestroyTexture(tex);
        SDL_FreeSurface(surf);
    }
}

inline void handleCharacterBarClick(CharacterBarState* cb, SDL_Rect barRect,
                                    int mouseX, int mouseY) {
    SDL_Rect btnRect = {barRect.x + 5, barRect.y + 10, barRect.w - 10, 90};
    if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w &&
        mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
        cb->panelOpen = !cb->panelOpen;
    }
}

inline void renderCharacterBarPanel(SDL_Renderer* renderer, TTF_Font* font,
                                    CharacterManager* mgr,
                                    int panelX, int panelY,
                                    int mouseX, int mouseY, bool mouseClicked) {
    constexpr SDL_Color white   = {255, 255, 255, 255};
    constexpr SDL_Color active  = {0, 200, 255, 255};
    constexpr SDL_Color normal  = {80, 80, 80, 255};
    constexpr SDL_Color hidden  = {180, 50, 50, 255};
    constexpr int thumbW = 50, thumbH = 50, padding = 8, rowH = thumbH + padding + 16;

    const int panelW = 190;
    const int panelH = padding + (mgr->count > 0 ? mgr->count * rowH : rowH) + padding + 20;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 20, 20, 35, 220);
    SDL_Rect panel = {panelX, panelY, panelW, panelH};
    SDL_RenderFillRect(renderer, &panel);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, 70, 70, 140, 255);
    SDL_RenderDrawRect(renderer, &panel);

    SDL_Surface* titleSurf = TTF_RenderText_Blended(font, "Characters", white);
    if (titleSurf) {
        SDL_Texture* titleTex = SDL_CreateTextureFromSurface(renderer, titleSurf);
        SDL_Rect tr = {panelX + padding, panelY + 4, titleSurf->w, titleSurf->h};
        SDL_RenderCopy(renderer, titleTex, nullptr, &tr);
        SDL_DestroyTexture(titleTex);
        SDL_FreeSurface(titleSurf);
    }

    if (mgr->count == 0) {
        SDL_Color gray = {140, 140, 140, 255};
        SDL_Surface* ns = TTF_RenderText_Blended(font, "No characters added.", gray);
        if (ns) {
            SDL_Texture* nt = SDL_CreateTextureFromSurface(renderer, ns);
            SDL_Rect nr = {panelX + padding, panelY + 24, ns->w, ns->h};
            SDL_RenderCopy(renderer, nt, nullptr, &nr);
            SDL_DestroyTexture(nt);
            SDL_FreeSurface(ns);
        }
        return;
    }

    for (int i = 0; i < mgr->count; i++) {
        Character& c = mgr->characters[i];
        const int rowY = panelY + 22 + padding + i * rowH;
        const int thumbX = panelX + padding;
        const int thumbY = rowY;

        SDL_Rect borderRect = {thumbX - 2, thumbY - 2, thumbW + 4, thumbH + 4};
        const SDL_Color& bdr = c.isActive ? active : normal;
        SDL_SetRenderDrawColor(renderer, bdr.r, bdr.g, bdr.b, bdr.a);
        SDL_RenderFillRect(renderer, &borderRect);

        SDL_Rect thumbRect = {thumbX, thumbY, thumbW, thumbH};
        if (c.texture != nullptr) {
            SDL_SetTextureAlphaMod(c.texture, c.visible ? 255 : 100);
            SDL_RenderCopy(renderer, c.texture, nullptr, &thumbRect);
            SDL_SetTextureAlphaMod(c.texture, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
            SDL_RenderFillRect(renderer, &thumbRect);
        }

        SDL_Surface* ns = TTF_RenderText_Blended(font, c.name.c_str(), white);
        if (ns) {
            SDL_Texture* nt = SDL_CreateTextureFromSurface(renderer, ns);
            SDL_Rect nr = {thumbX + thumbW + 6, thumbY + (thumbH - ns->h) / 2, ns->w, ns->h};
            SDL_RenderCopy(renderer, nt, nullptr, &nr);
            SDL_DestroyTexture(nt);
            SDL_FreeSurface(ns);
        }

        if (!c.visible) {
            SDL_Surface* hs = TTF_RenderText_Blended(font, "[hidden]", hidden);
            if (hs) {
                SDL_Texture* ht = SDL_CreateTextureFromSurface(renderer, hs);
                SDL_Rect hr = {thumbX + thumbW + 6, thumbY + (thumbH - hs->h) / 2 + 14, hs->w, hs->h};
                SDL_RenderCopy(renderer, ht, nullptr, &hr);
                SDL_DestroyTexture(ht);
                SDL_FreeSurface(hs);
            }
        }

        SDL_Rect rowHitbox = {panelX + padding, thumbY, panelW - padding * 2, thumbH};
        if (mouseClicked &&
            mouseX >= rowHitbox.x && mouseX <= rowHitbox.x + rowHitbox.w &&
            mouseY >= rowHitbox.y && mouseY <= rowHitbox.y + rowHitbox.h) {
            setActiveCharacter(mgr, i);
        }
    }
}

inline void initBackgroundMenuState(BackgroundMenuState* menu) {
    menu->menuOpen = false;
    menu->showingLibrary = false;
    menu->showingEditor = false;
}

inline void toggleBackgroundMenu(BackgroundMenuState* menu) {
    menu->menuOpen = !menu->menuOpen;
    if (!menu->menuOpen) {
        menu->showingLibrary = false;
        menu->showingEditor = false;
    }
}

inline void renderBackgroundBar(SDL_Renderer* renderer, TTF_Font* font,
                                BackgroundMenuState* menu, BackgroundState* bgState,
                                SDL_Rect barRect) {
    constexpr SDL_Color white = {255, 255, 255, 255};

    SDL_SetRenderDrawColor(renderer, 25, 25, 50, 255);
    SDL_RenderFillRect(renderer, &barRect);
    SDL_SetRenderDrawColor(renderer, 70, 70, 140, 255);
    SDL_RenderDrawRect(renderer, &barRect);

    SDL_Rect btnRect = {barRect.x + 5, barRect.y + 10, barRect.w - 10, 90};
    SDL_SetRenderDrawColor(renderer, 60, 60, 180, 255);
    SDL_RenderFillRect(renderer, &btnRect);

    SDL_Surface* surf = TTF_RenderText_Blended(font, "BG", white);
    if (surf) {
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_Rect tr = {btnRect.x + (btnRect.w - surf->w) / 2,
                       btnRect.y + (btnRect.h - surf->h) / 2, surf->w, surf->h};
        SDL_RenderCopy(renderer, tex, nullptr, &tr);
        SDL_DestroyTexture(tex);
        SDL_FreeSurface(surf);
    }

    if (menu->menuOpen) {
        constexpr int subW = 180, subH = 30;
        const int subX = barRect.x - subW - 4;
        int subY = barRect.y + 10;

        const std::string bgLabel = "BG: " + bgState->currentBgName;
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 230);
        SDL_Rect nameRect = {subX, subY, subW, subH};
        SDL_RenderFillRect(renderer, &nameRect);
        SDL_Surface* nameSurf = TTF_RenderText_Blended(font, bgLabel.c_str(), white);
        if (nameSurf) {
            SDL_Texture* nameTex = SDL_CreateTextureFromSurface(renderer, nameSurf);
            SDL_Rect nr = {subX + 5, subY + 7, nameSurf->w, nameSurf->h};
            SDL_RenderCopy(renderer, nameTex, nullptr, &nr);
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
            SDL_Rect lr = {subX + 5, subY + 7, libSurf->w, libSurf->h};
            SDL_RenderCopy(renderer, libTex, nullptr, &lr);
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
            SDL_Rect sr = {subX + 5, subY + 7, sysSurf->w, sysSurf->h};
            SDL_RenderCopy(renderer, sysTex, nullptr, &sr);
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
            SDL_Rect dr = {subX + 5, subY + 7, drawSurf->w, drawSurf->h};
            SDL_RenderCopy(renderer, drawTex, nullptr, &dr);
            SDL_DestroyTexture(drawTex);
            SDL_FreeSurface(drawSurf);
        }
    }
}

inline int handleBackgroundBarClick(BackgroundMenuState* menu, SDL_Rect barRect,
                                    int mouseX, int mouseY) {
    SDL_Rect btnRect = {barRect.x + 5, barRect.y + 10, barRect.w - 10, 90};
    if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w &&
        mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
        toggleBackgroundMenu(menu);
        return 0;
    }
    if (!menu->menuOpen) return 0;

    constexpr int subW = 180, subH = 30;
    const int subX = barRect.x - subW - 4;
    int subY = barRect.y + 10 + subH + 2;

    SDL_Rect libRect = {subX, subY, subW, subH};
    if (mouseX >= libRect.x && mouseX <= libRect.x + libRect.w &&
        mouseY >= libRect.y && mouseY <= libRect.y + libRect.h) {
        menu->showingLibrary = true;
        menu->showingEditor = false;
        return 1;
    }
    subY += subH + 2;
    SDL_Rect sysRect = {subX, subY, subW, subH};
    if (mouseX >= sysRect.x && mouseX <= sysRect.x + sysRect.w &&
        mouseY >= sysRect.y && mouseY <= sysRect.y + sysRect.h) return 2;

    subY += subH + 2;
    SDL_Rect drawRect = {subX, subY, subW, subH};
    if (mouseX >= drawRect.x && mouseX <= drawRect.x + drawRect.w &&
        mouseY >= drawRect.y && mouseY <= drawRect.y + drawRect.h) {
        menu->showingLibrary = false;
        menu->showingEditor = true;
        return 3;
    }
    return 0;
}

inline void renderBackground(SDL_Renderer* renderer, BackgroundState* bgState,
                             SDL_Rect stageRect) {
    SDL_SetRenderDrawColor(renderer, 130, 130, 130, 255);
    SDL_RenderFillRect(renderer, &stageRect);
    if (bgState->currentBg != nullptr)
        SDL_RenderCopy(renderer, bgState->currentBg, nullptr, &stageRect);
}

inline void loadDefaultBackgrounds(SDL_Renderer* renderer, BackgroundState* bgState,
                                   const std::string& path1, const std::string& name1,
                                   const std::string& path2, const std::string& name2) {
    SDL_Surface* s1 = IMG_Load(path1.c_str());
    if (s1) {
        bgState->defaultBgs[0] = SDL_CreateTextureFromSurface(renderer, s1);
        SDL_FreeSurface(s1);
    }
    bgState->defaultBgNames[0] = name1;

    SDL_Surface* s2 = IMG_Load(path2.c_str());
    if (s2) {
        bgState->defaultBgs[1] = SDL_CreateTextureFromSurface(renderer, s2);
        SDL_FreeSurface(s2);
    }
    bgState->defaultBgNames[1] = name2;

    if (bgState->currentBgName.empty()) bgState->currentBgName = "None";
}

inline int renderDefaultBackgroundLibrary(SDL_Renderer* renderer, TTF_Font* font,
                                          BackgroundState* bgState,
                                          int panelX, int panelY,
                                          int mouseX, int mouseY, bool mouseClicked,
                                          int, int) {
    int selectedIndex = -1;
    constexpr int thumbW = 160, thumbH = 100, padding = 15;
    constexpr SDL_Color white = {255, 255, 255, 255};
    constexpr SDL_Color panelBg = {20, 20, 20, 210};

    const int panelW = DEFAULT_BG_COUNT * (thumbW + padding) + padding;
    const int panelH = thumbH + 20 + padding * 3;

    SDL_Rect panel = {panelX, panelY, panelW, panelH};
    SDL_SetRenderDrawColor(renderer, panelBg.r, panelBg.g, panelBg.b, panelBg.a);
    SDL_RenderFillRect(renderer, &panel);

    SDL_Surface* titleSurf = TTF_RenderText_Blended(font, "Select Default Background", white);
    if (titleSurf) {
        SDL_Texture* titleTex = SDL_CreateTextureFromSurface(renderer, titleSurf);
        SDL_Rect tr = {panelX + padding, panelY + 5, titleSurf->w, titleSurf->h};
        SDL_RenderCopy(renderer, titleTex, nullptr, &tr);
        SDL_DestroyTexture(titleTex);
        SDL_FreeSurface(titleSurf);
    }

    for (int i = 0; i < DEFAULT_BG_COUNT; i++) {
        const int imgX = panelX + padding + i * (thumbW + padding);
        const int imgY = panelY + padding + 22;
        SDL_Rect thumbRect = {imgX, imgY, thumbW, thumbH};

        if (bgState->defaultBgs[i] != nullptr) {
            SDL_RenderCopy(renderer, bgState->defaultBgs[i], nullptr, &thumbRect);
        } else {
            SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
            SDL_RenderFillRect(renderer, &thumbRect);
        }

        SDL_Surface* nameSurf = TTF_RenderText_Blended(font, bgState->defaultBgNames[i].c_str(), white);
        if (nameSurf) {
            SDL_Texture* nameTex = SDL_CreateTextureFromSurface(renderer, nameSurf);
            SDL_Rect nr = {imgX, imgY + thumbH + 3, nameSurf->w, nameSurf->h};
            SDL_RenderCopy(renderer, nameTex, nullptr, &nr);
            SDL_DestroyTexture(nameTex);
            SDL_FreeSurface(nameSurf);
        }

        if (mouseClicked &&
            mouseX >= imgX && mouseX <= imgX + thumbW &&
            mouseY >= imgY && mouseY <= imgY + thumbH) {
            selectedIndex = i;
        }
    }

    if (selectedIndex >= 0 && bgState->defaultBgs[selectedIndex] != nullptr) {
        bgState->currentBg = bgState->defaultBgs[selectedIndex];
        bgState->currentBgName = bgState->defaultBgNames[selectedIndex];
    }
    return selectedIndex;
}

inline bool loadBackgroundFromSystem(SDL_Renderer* renderer, BackgroundState* bgState, int, int) {
    char filePath[MAX_PATH] = {0};
    OPENFILENAMEA ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = "Image Files\0*.png;*.jpg;*.jpeg;*.bmp\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (!GetOpenFileNameA(&ofn)) return false;

    SDL_Surface* raw = IMG_Load(filePath);
    if (!raw) return false;

    if (bgState->currentBg != nullptr &&
        bgState->currentBg != bgState->defaultBgs[0] &&
        bgState->currentBg != bgState->defaultBgs[1]) {
        SDL_DestroyTexture(bgState->currentBg);
    }

    bgState->currentBg = SDL_CreateTextureFromSurface(renderer, raw);
    SDL_FreeSurface(raw);

    const std::string fullPath = filePath;
    const auto slashPos = fullPath.find_last_of('\\');
    bgState->currentBgName = (slashPos != std::string::npos)
                             ? fullPath.substr(slashPos + 1)
                             : fullPath;
    return true;
}

inline void initImageEditor(SDL_Renderer* renderer, ImageEditorState* editor,
                            int canvasW, int canvasH) {
    editor->canvas = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                       SDL_TEXTUREACCESS_TARGET, canvasW, canvasH);
    SDL_SetRenderTarget(renderer, editor->canvas);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, nullptr);

    editor->isDrawing = false;
    editor->lastX = -1;
    editor->lastY = -1;
    editor->penColor = {0, 0, 0, 255};
    editor->penSize = EDITOR_BRUSH_DEFAULT;
    editor->confirmed = false;
    editor->active = true;
}

inline void editorDrawCircle(SDL_Renderer* renderer, SDL_Texture* canvas,
                             int cx, int cy, int radius, SDL_Color color) {
    SDL_SetRenderTarget(renderer, canvas);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int dy = -radius; dy <= radius; dy++) {
        for (int dx = -radius; dx <= radius; dx++) {
            if (dx * dx + dy * dy <= radius * radius)
                SDL_RenderDrawPoint(renderer, cx + dx, cy + dy);
        }
    }
    SDL_SetRenderTarget(renderer, nullptr);
}

inline void handleImageEditorMouse(SDL_Renderer* renderer, ImageEditorState* editor,
                                   SDL_Event* event, SDL_Rect canvasRect) {
    if (!editor->active) return;

    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        const int mx = event->button.x - canvasRect.x;
        const int my = event->button.y - canvasRect.y;
        if (mx >= 0 && mx < canvasRect.w && my >= 0 && my < canvasRect.h) {
            editor->isDrawing = true;
            editor->lastX = mx;
            editor->lastY = my;
            editorDrawCircle(renderer, editor->canvas, mx, my, editor->penSize, editor->penColor);
        }
    }
    if (event->type == SDL_MOUSEMOTION && editor->isDrawing) {
        const int mx = event->motion.x - canvasRect.x;
        const int my = event->motion.y - canvasRect.y;
        if (mx >= 0 && mx < canvasRect.w && my >= 0 && my < canvasRect.h) {
            const int x0 = editor->lastX, y0 = editor->lastY;
            const int steps = std::max({std::abs(mx - x0), std::abs(my - y0), 1});
            for (int s = 0; s <= steps; s++) {
                const int px = x0 + (mx - x0) * s / steps;
                const int py = y0 + (my - y0) * s / steps;
                editorDrawCircle(renderer, editor->canvas, px, py, editor->penSize, editor->penColor);
            }
            editor->lastX = mx;
            editor->lastY = my;
        }
    }
    if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)
        editor->isDrawing = false;
}

inline void renderImageEditor(SDL_Renderer* renderer, TTF_Font* font,
                              ImageEditorState* editor, SDL_Rect canvasRect,
                              int mouseX, int mouseY, bool mouseClicked) {
    if (!editor->active) return;

    constexpr SDL_Color white = {255, 255, 255, 255};

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_Rect border = {canvasRect.x - 2, canvasRect.y - 2,
                       canvasRect.w + 4, canvasRect.h + 4};
    SDL_RenderFillRect(renderer, &border);
    SDL_RenderCopy(renderer, editor->canvas, nullptr, &canvasRect);

    const int toolY = canvasRect.y + canvasRect.h + 8, toolX = canvasRect.x;

    constexpr SDL_Color palette[] = {
            {0, 0, 0, 255}, {255, 0, 0, 255}, {0, 128, 0, 255}, {0, 0, 255, 255},
            {255, 255, 0, 255}, {255, 165, 0, 255}, {128, 0, 128, 255}, {255, 255, 255, 255}
    };
    constexpr int paletteCount = 8, swatchSize = 24;

    for (int i = 0; i < paletteCount; i++) {
        SDL_Rect swatch = {toolX + i * (swatchSize + 4), toolY, swatchSize, swatchSize};
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

    const int afterPalette = toolX + paletteCount * (swatchSize + 4) + 10;

    SDL_Rect minusBtn = {afterPalette, toolY, 26, 26};
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderFillRect(renderer, &minusBtn);
    SDL_Surface* minSurf = TTF_RenderText_Blended(font, "-", white);
    if (minSurf) {
        SDL_Texture* minTex = SDL_CreateTextureFromSurface(renderer, minSurf);
        SDL_Rect mr = {minusBtn.x + 7, minusBtn.y + 3, minSurf->w, minSurf->h};
        SDL_RenderCopy(renderer, minTex, nullptr, &mr);
        SDL_DestroyTexture(minTex);
        SDL_FreeSurface(minSurf);
    }
    if (mouseClicked &&
        mouseX >= minusBtn.x && mouseX <= minusBtn.x + minusBtn.w &&
        mouseY >= minusBtn.y && mouseY <= minusBtn.y + minusBtn.h) {
        if (editor->penSize > 1) editor->penSize--;
    }

    SDL_Rect plusBtn = {afterPalette + 30, toolY, 26, 26};
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderFillRect(renderer, &plusBtn);
    SDL_Surface* plusSurf = TTF_RenderText_Blended(font, "+", white);
    if (plusSurf) {
        SDL_Texture* plusTex = SDL_CreateTextureFromSurface(renderer, plusSurf);
        SDL_Rect pr = {plusBtn.x + 5, plusBtn.y + 3, plusSurf->w, plusSurf->h};
        SDL_RenderCopy(renderer, plusTex, nullptr, &pr);
        SDL_DestroyTexture(plusTex);
        SDL_FreeSurface(plusSurf);
    }
    if (mouseClicked &&
        mouseX >= plusBtn.x && mouseX <= plusBtn.x + plusBtn.w &&
        mouseY >= plusBtn.y && mouseY <= plusBtn.y + plusBtn.h) {
        if (editor->penSize < 40) editor->penSize++;
    }

    const std::string sizeLabel = "Brush: " + std::to_string(editor->penSize);
    SDL_Surface* szSurf = TTF_RenderText_Blended(font, sizeLabel.c_str(), white);
    if (szSurf) {
        SDL_Texture* szTex = SDL_CreateTextureFromSurface(renderer, szSurf);
        SDL_Rect szr = {afterPalette + 62, toolY + 4, szSurf->w, szSurf->h};
        SDL_RenderCopy(renderer, szTex, nullptr, &szr);
        SDL_DestroyTexture(szTex);
        SDL_FreeSurface(szSurf);
    }

    const int confirmX = canvasRect.x + canvasRect.w - 180, confirmY = toolY;

    SDL_Rect confirmBtn = {confirmX, confirmY, 80, 28};
    SDL_SetRenderDrawColor(renderer, 20, 160, 20, 255);
    SDL_RenderFillRect(renderer, &confirmBtn);
    SDL_Surface* confSurf = TTF_RenderText_Blended(font, "Confirm", white);
    if (confSurf) {
        SDL_Texture* confTex = SDL_CreateTextureFromSurface(renderer, confSurf);
        SDL_Rect cr = {confirmBtn.x + 5, confirmBtn.y + 5, confSurf->w, confSurf->h};
        SDL_RenderCopy(renderer, confTex, nullptr, &cr);
        SDL_DestroyTexture(confTex);
        SDL_FreeSurface(confSurf);
    }
    if (mouseClicked &&
        mouseX >= confirmBtn.x && mouseX <= confirmBtn.x + confirmBtn.w &&
        mouseY >= confirmBtn.y && mouseY <= confirmBtn.y + confirmBtn.h) {
        editor->confirmed = true;
        editor->active = false;
    }

    SDL_Rect cancelBtn = {confirmX + 90, confirmY, 80, 28};
    SDL_SetRenderDrawColor(renderer, 180, 20, 20, 255);
    SDL_RenderFillRect(renderer, &cancelBtn);
    SDL_Surface* canSurf = TTF_RenderText_Blended(font, "Cancel", white);
    if (canSurf) {
        SDL_Texture* canTex = SDL_CreateTextureFromSurface(renderer, canSurf);
        SDL_Rect cnr = {cancelBtn.x + 5, cancelBtn.y + 5, canSurf->w, canSurf->h};
        SDL_RenderCopy(renderer, canTex, nullptr, &cnr);
        SDL_DestroyTexture(canTex);
        SDL_FreeSurface(canSurf);
    }
    if (mouseClicked &&
        mouseX >= cancelBtn.x && mouseX <= cancelBtn.x + cancelBtn.w &&
        mouseY >= cancelBtn.y && mouseY <= cancelBtn.y + cancelBtn.h) {
        editor->confirmed = false;
        editor->active = false;
    }
}

inline void applyEditorCanvasAsBackground(SDL_Renderer*, BackgroundState* bgState,
                                          ImageEditorState* editor, int, int) {
    if (!editor->confirmed) return;

    if (bgState->currentBg != nullptr &&
        bgState->currentBg != bgState->defaultBgs[0] &&
        bgState->currentBg != bgState->defaultBgs[1]) {
        SDL_DestroyTexture(bgState->currentBg);
    }

    bgState->currentBg = editor->canvas;
    bgState->currentBgName = "Custom Drawing";
    editor->canvas = nullptr;
    editor->confirmed = false;
}

inline void initCharacterManager(CharacterManager* mgr) {
    mgr->count = 0;
    mgr->activeIndex = -1;
    for (auto& c : mgr->characters) c = Character{};
}

inline int addCharacterFromFile(SDL_Renderer* renderer, CharacterManager* mgr,
                                const std::string& imagePath, const std::string& name) {
    if (mgr->count >= MAX_CHARACTERS) return -1;

    SDL_Surface* surf = IMG_Load(imagePath.c_str());
    if (!surf) return -1;

    const int idx = mgr->count;
    Character& c = mgr->characters[idx];
    c.texture = SDL_CreateTextureFromSurface(renderer, surf);
    c.width = surf->w;
    c.height = surf->h;
    SDL_FreeSurface(surf);
    c.x = 100;
    c.y = 100;
    c.rotation = 0.0f;
    c.visible = true;
    c.isActive = false;
    c.name = name;
    mgr->count++;
    return idx;
}

inline int addOrangeSquareCharacter(SDL_Renderer* renderer, CharacterManager* mgr) {
    if (mgr->count >= MAX_CHARACTERS) return -1;
    constexpr int SZ = 100;
    SDL_Surface* surf = SDL_CreateRGBSurfaceWithFormat(0, SZ, SZ, 32, SDL_PIXELFORMAT_RGBA8888);
    if (!surf) return -1;
    SDL_FillRect(surf, nullptr, SDL_MapRGBA(surf->format, 255, 140, 0, 255));
    const int idx = mgr->count;
    Character& c = mgr->characters[idx];
    c.texture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    c.width = SZ; c.height = SZ;
    c.x = 100; c.y = 100;
    c.rotation = 0.0f; c.visible = true; c.isActive = false;
    c.name = "Orange Square";
    mgr->count++;
    return idx;
}

inline int addCharacterFromSystem(SDL_Renderer* renderer, CharacterManager* mgr) {
    char filePath[MAX_PATH] = {0};
    OPENFILENAMEA ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = "Image Files\0*.png;*.jpg;*.jpeg;*.bmp\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (!GetOpenFileNameA(&ofn)) return -1;

    const std::string fullPath = filePath;
    const auto slashPos = fullPath.find_last_of('\\');
    const std::string name = (slashPos != std::string::npos)
                             ? fullPath.substr(slashPos + 1)
                             : fullPath;
    return addCharacterFromFile(renderer, mgr, fullPath, name);
}

inline int addRandomCharacter(SDL_Renderer* renderer, CharacterManager* mgr,
                              const char** libraryPaths, const char** libraryNames,
                              int librarySize) {
    if (librarySize <= 0) return -1;
    const int pick = rand() % librarySize;
    return addCharacterFromFile(renderer, mgr, libraryPaths[pick], libraryNames[pick]);
}

inline void setActiveCharacter(CharacterManager* mgr, int index) {
    if (index < 0 || index >= mgr->count) return;
    if (mgr->activeIndex >= 0 && mgr->activeIndex < mgr->count)
        mgr->characters[mgr->activeIndex].isActive = false;
    mgr->activeIndex = index;
    mgr->characters[index].isActive = true;
}

inline void renameCharacter(CharacterManager* mgr, int index, const std::string& newName) {
    if (index < 0 || index >= mgr->count) return;
    mgr->characters[index].name = newName;
}

inline void setCharacterPosition(CharacterManager* mgr, int index, int x, int y) {
    if (index < 0 || index >= mgr->count) return;
    mgr->characters[index].x = x;
    mgr->characters[index].y = y;
}

inline bool handleCharacterDrag(CharacterManager* mgr, int mouseX, int mouseY,
                                bool mouseDown, bool mouseJustPressed, int* dragIndex) {
    if (mouseJustPressed) {
        *dragIndex = -1;
        for (int i = 0; i < mgr->count; i++) {
            Character& c = mgr->characters[i];
            if (!c.visible) continue;
            SDL_Rect bounds = {c.x, c.y, c.width, c.height};
            if (mouseX >= bounds.x && mouseX <= bounds.x + bounds.w &&
                mouseY >= bounds.y && mouseY <= bounds.y + bounds.h) {
                *dragIndex = i;
                setActiveCharacter(mgr, i);
                break;
            }
        }
    }
    if (mouseDown && *dragIndex >= 0 && *dragIndex < mgr->count) {
        Character& c = mgr->characters[*dragIndex];
        c.x = mouseX - c.width / 2;
        c.y = mouseY - c.height / 2;
        return true;
    }
    if (!mouseDown) *dragIndex = -1;
    return false;
}

inline void setCharacterRotation(CharacterManager* mgr, int index, float angleDegrees) {
    if (index < 0 || index >= mgr->count) return;
    mgr->characters[index].rotation = angleDegrees;
}

inline void toggleCharacterVisibility(CharacterManager* mgr, int index) {
    if (index < 0 || index >= mgr->count) return;
    mgr->characters[index].visible = !mgr->characters[index].visible;
}

inline bool deleteCharacter(CharacterManager* mgr, int index) {
    if (index < 0 || index >= mgr->count) return false;

    const std::string msg = "Are you sure you want to delete character \""
                            + mgr->characters[index].name
                            + "\"?\nThis action cannot be undone.";
    if (MessageBoxA(nullptr, msg.c_str(), "Delete Character",
                    MB_YESNO | MB_ICONWARNING) != IDYES) return false;

    if (mgr->characters[index].texture != nullptr) {
        SDL_DestroyTexture(mgr->characters[index].texture);
        mgr->characters[index].texture = nullptr;
    }

    for (int i = index; i < mgr->count - 1; i++)
        mgr->characters[i] = mgr->characters[i + 1];

    mgr->count--;
    if (mgr->activeIndex == index) {
        mgr->activeIndex = (mgr->count > 0) ? 0 : -1;
        if (mgr->activeIndex >= 0) mgr->characters[mgr->activeIndex].isActive = true;
    } else if (mgr->activeIndex > index) {
        mgr->activeIndex--;
    }
    return true;
}

inline void renderCharacterPanel(SDL_Renderer* renderer, TTF_Font* font,
                                 CharacterManager* mgr, SDL_Rect panelRect,
                                 int mouseX, int mouseY, bool mouseClicked) {
    constexpr SDL_Color panelBg = {25, 25, 25, 240};
    constexpr SDL_Color activeBdr = {0, 200, 255, 255};
    constexpr SDL_Color normalBdr = {80, 80, 80, 255};
    constexpr SDL_Color white = {255, 255, 255, 255};

    SDL_SetRenderDrawColor(renderer, panelBg.r, panelBg.g, panelBg.b, panelBg.a);
    SDL_RenderFillRect(renderer, &panelRect);

    SDL_Surface* titleSurf = TTF_RenderText_Blended(font, "Characters", white);
    if (titleSurf) {
        SDL_Texture* titleTex = SDL_CreateTextureFromSurface(renderer, titleSurf);
        SDL_Rect tr = {panelRect.x + 5, panelRect.y + 4, titleSurf->w, titleSurf->h};
        SDL_RenderCopy(renderer, titleTex, nullptr, &tr);
        SDL_DestroyTexture(titleTex);
        SDL_FreeSurface(titleSurf);
    }

    const int startY = panelRect.y + 28;
    constexpr int thumbSize = CHAR_PANEL_THUMB_SIZE, padding = CHAR_PANEL_PADDING;

    for (int i = 0; i < mgr->count; i++) {
        Character& c = mgr->characters[i];
        const int thumbX = panelRect.x + padding;
        const int thumbY = startY + i * (thumbSize + padding + 20);
        if (thumbY + thumbSize > panelRect.y + panelRect.h) break;

        SDL_Rect thumbRect = {thumbX, thumbY, thumbSize, thumbSize};
        SDL_Rect borderRect = {thumbX - 2, thumbY - 2, thumbSize + 4, thumbSize + 4};

        const SDL_Color& bdr = c.isActive ? activeBdr : normalBdr;
        SDL_SetRenderDrawColor(renderer, bdr.r, bdr.g, bdr.b, bdr.a);
        SDL_RenderFillRect(renderer, &borderRect);

        if (c.texture != nullptr) {
            SDL_SetTextureAlphaMod(c.texture, c.visible ? 255 : 100);
            SDL_RenderCopy(renderer, c.texture, nullptr, &thumbRect);
            SDL_SetTextureAlphaMod(c.texture, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
            SDL_RenderFillRect(renderer, &thumbRect);
        }

        SDL_Surface* nameSurf = TTF_RenderText_Blended(font, c.name.c_str(), white);
        if (nameSurf) {
            SDL_Texture* nameTex = SDL_CreateTextureFromSurface(renderer, nameSurf);
            SDL_Rect nr = {thumbX, thumbY + thumbSize + 2, nameSurf->w, nameSurf->h};
            SDL_RenderCopy(renderer, nameTex, nullptr, &nr);
            SDL_DestroyTexture(nameTex);
            SDL_FreeSurface(nameSurf);
        }

        if (!c.visible) {
            constexpr SDL_Color hiddenColor = {180, 50, 50, 255};
            SDL_Surface* hideSurf = TTF_RenderText_Blended(font, "[hidden]", hiddenColor);
            if (hideSurf) {
                SDL_Texture* hideTex = SDL_CreateTextureFromSurface(renderer, hideSurf);
                SDL_Rect hr = {thumbX + thumbSize - hideSurf->w - 2,
                               thumbY + 2, hideSurf->w, hideSurf->h};
                SDL_RenderCopy(renderer, hideTex, nullptr, &hr);
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

inline void renderCharacterSettingsPanel(SDL_Renderer* renderer, TTF_Font* font,
                                         CharacterManager* mgr, SDL_Rect panelRect,
                                         int mouseX, int mouseY, bool mouseClicked,
                                         bool* requestAddChar, bool* requestRename,
                                         std::string& renameBuffer) {
    constexpr SDL_Color panelBg = {30, 30, 30, 240};
    constexpr SDL_Color white = {255, 255, 255, 255};
    constexpr SDL_Color gray = {160, 160, 160, 255};

    SDL_SetRenderDrawColor(renderer, panelBg.r, panelBg.g, panelBg.b, panelBg.a);
    SDL_RenderFillRect(renderer, &panelRect);

    int cx = panelRect.x + 10, cy = panelRect.y + 10;
    constexpr int lineH = 26;

    SDL_Surface* hdrSurf = TTF_RenderText_Blended(font, "Character Settings", white);
    if (hdrSurf) {
        SDL_Texture* hdrTex = SDL_CreateTextureFromSurface(renderer, hdrSurf);
        SDL_Rect hr = {cx, cy, hdrSurf->w, hdrSurf->h};
        SDL_RenderCopy(renderer, hdrTex, nullptr, &hr);
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
        SDL_RenderCopy(renderer, addTex, nullptr, &ar);
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
            SDL_RenderCopy(renderer, noTex, nullptr, &nr);
            SDL_DestroyTexture(noTex);
            SDL_FreeSurface(noSurf);
        }
        return;
    }

    Character& active = mgr->characters[mgr->activeIndex];

    const std::string nameLabel = "Name: " + active.name;
    SDL_Surface* nameSurf = TTF_RenderText_Blended(font, nameLabel.c_str(), white);
    if (nameSurf) {
        SDL_Texture* nameTex = SDL_CreateTextureFromSurface(renderer, nameSurf);
        SDL_Rect nr = {cx, cy, nameSurf->w, nameSurf->h};
        SDL_RenderCopy(renderer, nameTex, nullptr, &nr);
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
        SDL_RenderCopy(renderer, renTex, nullptr, &rr);
        SDL_DestroyTexture(renTex);
        SDL_FreeSurface(renSurf);
    }
    if (mouseClicked &&
        mouseX >= renameBtn.x && mouseX <= renameBtn.x + renameBtn.w &&
        mouseY >= renameBtn.y && mouseY <= renameBtn.y + renameBtn.h) {
        *requestRename = true;
        renameBuffer = active.name;
    }
    cy += lineH + 4;

    const std::string posLabel = "X: " + std::to_string(active.x)
                                 + "    Y: " + std::to_string(active.y);
    SDL_Surface* posSurf = TTF_RenderText_Blended(font, posLabel.c_str(), white);
    if (posSurf) {
        SDL_Texture* posTex = SDL_CreateTextureFromSurface(renderer, posSurf);
        SDL_Rect pr = {cx, cy, posSurf->w, posSurf->h};
        SDL_RenderCopy(renderer, posTex, nullptr, &pr);
        SDL_DestroyTexture(posTex);
        SDL_FreeSurface(posSurf);
    }
    cy += lineH;

    const std::string sizeLabel = "W: " + std::to_string(active.width)
                                  + "   H: " + std::to_string(active.height)
                                  + "   Rot: " + std::to_string(static_cast<int>(active.rotation)) + "deg";
    SDL_Surface* sizeSurf = TTF_RenderText_Blended(font, sizeLabel.c_str(), white);
    if (sizeSurf) {
        SDL_Texture* sizeTex = SDL_CreateTextureFromSurface(renderer, sizeSurf);
        SDL_Rect sr = {cx, cy, sizeSurf->w, sizeSurf->h};
        SDL_RenderCopy(renderer, sizeTex, nullptr, &sr);
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
        SDL_Rect rm = {rotMinusBtn.x + 8, rotMinusBtn.y + 3, rmSurf->w, rmSurf->h};
        SDL_RenderCopy(renderer, rmTex, nullptr, &rm);
        SDL_DestroyTexture(rmTex);
        SDL_FreeSurface(rmSurf);
    }
    if (mouseClicked &&
        mouseX >= rotMinusBtn.x && mouseX <= rotMinusBtn.x + rotMinusBtn.w &&
        mouseY >= rotMinusBtn.y && mouseY <= rotMinusBtn.y + rotMinusBtn.h) {
        active.rotation -= 5.0f;
    }

    SDL_Surface* rotSurf = TTF_RenderText_Blended(font, "Rotate", gray);
    if (rotSurf) {
        SDL_Texture* rotTex = SDL_CreateTextureFromSurface(renderer, rotSurf);
        SDL_Rect rl = {cx + 34, cy + 3, rotSurf->w, rotSurf->h};
        SDL_RenderCopy(renderer, rotTex, nullptr, &rl);
        SDL_DestroyTexture(rotTex);
        SDL_FreeSurface(rotSurf);
    }

    SDL_Rect rotPlusBtn = {cx + 90, cy, 28, 22};
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderFillRect(renderer, &rotPlusBtn);
    SDL_Surface* rpSurf = TTF_RenderText_Blended(font, "+", white);
    if (rpSurf) {
        SDL_Texture* rpTex = SDL_CreateTextureFromSurface(renderer, rpSurf);
        SDL_Rect rp = {rotPlusBtn.x + 6, rotPlusBtn.y + 3, rpSurf->w, rpSurf->h};
        SDL_RenderCopy(renderer, rpTex, nullptr, &rp);
        SDL_DestroyTexture(rpTex);
        SDL_FreeSurface(rpSurf);
    }
    if (mouseClicked &&
        mouseX >= rotPlusBtn.x && mouseX <= rotPlusBtn.x + rotPlusBtn.w &&
        mouseY >= rotPlusBtn.y && mouseY <= rotPlusBtn.y + rotPlusBtn.h) {
        active.rotation += 5.0f;
    }
    cy += lineH;

    SDL_Rect sizeMinusBtn = {cx, cy, 28, 22};
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderFillRect(renderer, &sizeMinusBtn);
    SDL_Surface* smSurf = TTF_RenderText_Blended(font, "-", white);
    if (smSurf) {
        SDL_Texture* smTex = SDL_CreateTextureFromSurface(renderer, smSurf);
        SDL_Rect sm = {sizeMinusBtn.x + 8, sizeMinusBtn.y + 3, smSurf->w, smSurf->h};
        SDL_RenderCopy(renderer, smTex, nullptr, &sm);
        SDL_DestroyTexture(smTex);
        SDL_FreeSurface(smSurf);
    }
    if (mouseClicked &&
        mouseX >= sizeMinusBtn.x && mouseX <= sizeMinusBtn.x + sizeMinusBtn.w &&
        mouseY >= sizeMinusBtn.y && mouseY <= sizeMinusBtn.y + sizeMinusBtn.h) {
        if (active.width > 10) active.width -= 5;
        if (active.height > 10) active.height -= 5;
    }

    SDL_Surface* sizeLblSurf = TTF_RenderText_Blended(font, "Size", gray);
    if (sizeLblSurf) {
        SDL_Texture* sizeLblTex = SDL_CreateTextureFromSurface(renderer, sizeLblSurf);
        SDL_Rect sl = {cx + 34, cy + 3, sizeLblSurf->w, sizeLblSurf->h};
        SDL_RenderCopy(renderer, sizeLblTex, nullptr, &sl);
        SDL_DestroyTexture(sizeLblTex);
        SDL_FreeSurface(sizeLblSurf);
    }

    SDL_Rect sizePlusBtn = {cx + 90, cy, 28, 22};
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderFillRect(renderer, &sizePlusBtn);
    SDL_Surface* spSurf = TTF_RenderText_Blended(font, "+", white);
    if (spSurf) {
        SDL_Texture* spTex = SDL_CreateTextureFromSurface(renderer, spSurf);
        SDL_Rect sp = {sizePlusBtn.x + 6, sizePlusBtn.y + 3, spSurf->w, spSurf->h};
        SDL_RenderCopy(renderer, spTex, nullptr, &sp);
        SDL_DestroyTexture(spTex);
        SDL_FreeSurface(spSurf);
    }
    if (mouseClicked &&
        mouseX >= sizePlusBtn.x && mouseX <= sizePlusBtn.x + sizePlusBtn.w &&
        mouseY >= sizePlusBtn.y && mouseY <= sizePlusBtn.y + sizePlusBtn.h) {
        active.width += 5;
        active.height += 5;
    }
    cy += lineH + 4;

    const char* visLabel = active.visible ? "Hide Character" : "Show Character";
    SDL_Rect visBtn = {cx, cy, 130, 24};
    SDL_SetRenderDrawColor(renderer,
                           active.visible ? 120 : 30,
                           active.visible ? 80 : 120,
                           80, 255);
    SDL_RenderFillRect(renderer, &visBtn);
    SDL_Surface* visSurf = TTF_RenderText_Blended(font, visLabel, white);
    if (visSurf) {
        SDL_Texture* visTex = SDL_CreateTextureFromSurface(renderer, visSurf);
        SDL_Rect vr = {visBtn.x + 5, visBtn.y + 4, visSurf->w, visSurf->h};
        SDL_RenderCopy(renderer, visTex, nullptr, &vr);
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
        SDL_RenderCopy(renderer, delTex, nullptr, &dr);
        SDL_DestroyTexture(delTex);
        SDL_FreeSurface(delSurf);
    }
    if (mouseClicked &&
        mouseX >= delBtn.x && mouseX <= delBtn.x + delBtn.w &&
        mouseY >= delBtn.y && mouseY <= delBtn.y + delBtn.h) {
        deleteCharacter(mgr, mgr->activeIndex);
    }
}

inline void renderAllCharacters(SDL_Renderer* renderer, CharacterManager* mgr) {
    for (int i = 0; i < mgr->count; i++) {
        Character& c = mgr->characters[i];
        if (!c.visible || c.texture == nullptr) continue;

        SDL_Rect dst = {c.x, c.y, c.width, c.height};
        SDL_Point center = {c.width / 2, c.height / 2};
        SDL_RenderCopyEx(renderer, c.texture, nullptr, &dst,
                         static_cast<double>(c.rotation), &center, SDL_FLIP_NONE);
        if (c.isActive) {
            SDL_SetRenderDrawColor(renderer, 0, 200, 255, 255);
            SDL_Rect outline = {c.x - 2, c.y - 2, c.width + 4, c.height + 4};
            SDL_RenderDrawRect(renderer, &outline);
        }
    }
}

inline void destroyCharacterManager(CharacterManager* mgr) {
    for (int i = 0; i < mgr->count; i++) {
        if (mgr->characters[i].texture != nullptr) {
            SDL_DestroyTexture(mgr->characters[i].texture);
            mgr->characters[i].texture = nullptr;
        }
    }
    mgr->count = 0;
    mgr->activeIndex = -1;
}

inline void destroyBackgroundState(BackgroundState* bgState) {
    bool isDefault = false;
    for (int i = 0; i < DEFAULT_BG_COUNT; i++) {
        if (bgState->currentBg == bgState->defaultBgs[i]) isDefault = true;
        if (bgState->defaultBgs[i] != nullptr) {
            SDL_DestroyTexture(bgState->defaultBgs[i]);
            bgState->defaultBgs[i] = nullptr;
        }
    }
    if (!isDefault && bgState->currentBg != nullptr)
        SDL_DestroyTexture(bgState->currentBg);
    bgState->currentBg = nullptr;
}
static int MONITOR_W = 0, MONITOR_H = 0;
static int WINDOW_W = 0, WINDOW_H = 0, WINDOW_X = 0, WINDOW_Y = 20;
static int TOOLBAR_H = 0;
static int STAGE_W = 500, STAGE_H = 400, STAGE_X = 0, STAGE_Y = 0;
static int SETT_PANEL_W = 500, SETT_PANEL_H = 0, SETT_PANEL_X = 0, SETT_PANEL_Y = 400;
static int CHAR_PANEL_W = 0, CHAR_PANEL_X = 0, CHAR_PANEL_Y = 0;
static SDL_Rect BG_BAR_RECT = {};
static SDL_Rect CHAR_BAR_RECT = {};
static int EDITOR_CANVAS_W = 760, EDITOR_CANVAS_H = 480, EDITOR_CANVAS_X = 0, EDITOR_CANVAS_Y = 0;
static SDL_Rect EDITOR_CANVAS_RECT = {};

static bool simpleTextInputDialog(SDL_Renderer* renderer, TTF_Font* font,
                                  const std::string& prompt, std::string& outBuf) {
    SDL_StartTextInput();
    std::string text = outBuf;
    bool done = false, confirm = false;

    while (!done) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) { done = true; break; }
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_RETURN) { confirm = true; done = true; }
                else if (e.key.keysym.sym == SDLK_ESCAPE) { done = true; }
                else if (e.key.keysym.sym == SDLK_BACKSPACE) { if (!text.empty()) text.pop_back(); }
            }
            if (e.type == SDL_TEXTINPUT) { text += e.text.text; }
        }

        constexpr SDL_Color white = {255, 255, 255, 255}, cyan = {100, 220, 255, 255}, hint = {150, 150, 150, 255};

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);
        SDL_Rect overlay = {0, 0, WINDOW_W, WINDOW_H};
        SDL_RenderFillRect(renderer, &overlay);

        SDL_Rect box = {WINDOW_W / 2 - 220, WINDOW_H / 2 - 60, 440, 120};
        SDL_SetRenderDrawColor(renderer, 40, 40, 60, 255);
        SDL_RenderFillRect(renderer, &box);
        SDL_SetRenderDrawColor(renderer, 100, 100, 200, 255);
        SDL_RenderDrawRect(renderer, &box);

        SDL_Surface* ps = TTF_RenderText_Blended(font, prompt.c_str(), white);
        if (ps) {
            SDL_Texture* pt = SDL_CreateTextureFromSurface(renderer, ps);
            SDL_Rect pr = {box.x + 10, box.y + 10, ps->w, ps->h};
            SDL_RenderCopy(renderer, pt, nullptr, &pr);
            SDL_DestroyTexture(pt);
            SDL_FreeSurface(ps);
        }

        SDL_Rect field = {box.x + 10, box.y + 50, box.w - 20, 28};
        SDL_SetRenderDrawColor(renderer, 20, 20, 40, 255);
        SDL_RenderFillRect(renderer, &field);
        SDL_SetRenderDrawColor(renderer, 80, 80, 160, 255);
        SDL_RenderDrawRect(renderer, &field);

        const std::string display = text + "|";
        SDL_Surface* ts = TTF_RenderText_Blended(font, display.c_str(), cyan);
        if (ts) {
            SDL_Texture* tt = SDL_CreateTextureFromSurface(renderer, ts);
            SDL_Rect tr = {field.x + 5, field.y + 4, ts->w, ts->h};
            SDL_RenderCopy(renderer, tt, nullptr, &tr);
            SDL_DestroyTexture(tt);
            SDL_FreeSurface(ts);
        }

        SDL_Surface* hs = TTF_RenderText_Blended(font, "Enter = confirm    Esc = cancel", hint);
        if (hs) {
            SDL_Texture* ht = SDL_CreateTextureFromSurface(renderer, hs);
            SDL_Rect hr = {box.x + 10, box.y + 90, hs->w, hs->h};
            SDL_RenderCopy(renderer, ht, nullptr, &hr);
            SDL_DestroyTexture(ht);
            SDL_FreeSurface(hs);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_StopTextInput();
    if (confirm) outBuf = text;
    return confirm;
}

static const char* LIBRARY_PATHS[] = {"assets/characters/cat.png", "assets/characters/dog.png", "assets/characters/robot.png"};
static const char* LIBRARY_NAMES[] = {"Cat", "Dog", "Robot"};
static const int LIBRARY_SIZE = 3;

static void renderStageBorder(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 60, 60, 80, 255);
    SDL_Rect border = {STAGE_X - 1, STAGE_Y - 1, STAGE_W + 2, STAGE_H + 2};
    SDL_RenderDrawRect(renderer, &border);
}

#endif