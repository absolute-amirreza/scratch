#ifndef SCRATCH_SETTINGS_H
#define SCRATCH_SETTINGS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <vector>
#include <string>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>
#endif

struct Sprite {
    std::string name;
    std::shared_ptr<SDL_Texture> texture;
    SDL_Rect rect{};
    double angle = 0.0;
    double scale = 1.0;
    bool visible = true;
};

struct Background {
    std::string name;
    std::shared_ptr<SDL_Texture> texture;
};

struct Settings {
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;
    TTF_Font* fontBold = nullptr;
    int width = 1200;
    int height = 700;

    std::shared_ptr<Background> currentBg;
    std::vector<std::shared_ptr<Background>> bgLibrary;
    std::vector<std::shared_ptr<Sprite>> sprites;

    int selectedSprite = -1;
    bool bgLibraryOpen = false;
    bool dragging = false;
    int dragOffsetX = 0;
    int dragOffsetY = 0;

    const int leftPanelW = 280;
    const int margin = 15;
    const int stageH = 380;
};

inline std::shared_ptr<SDL_Texture> createColorTexture(SDL_Renderer* r, int w, int h, SDL_Color color) {
    SDL_Surface* surf = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    SDL_FillRect(surf, nullptr, SDL_MapRGBA(surf->format, color.r, color.g, color.b, color.a));
    SDL_Texture* tex = SDL_CreateTextureFromSurface(r, surf);
    SDL_FreeSurface(surf);
    return std::shared_ptr<SDL_Texture>(tex, SDL_DestroyTexture);
}

inline std::shared_ptr<SDL_Texture> loadTexture(SDL_Renderer* r, const std::string& path) {
    SDL_Surface* surf = IMG_Load(path.c_str());
    if (!surf) return createColorTexture(r, 64, 64, {255, 0, 0, 255});
    SDL_Texture* tex = SDL_CreateTextureFromSurface(r, surf);
    SDL_FreeSurface(surf);
    return std::shared_ptr<SDL_Texture>(tex, SDL_DestroyTexture);
}

inline void drawText(SDL_Renderer* r, TTF_Font* f, const std::string& text, int x, int y, SDL_Color color = {70, 70, 70, 255}) {
    if (text.empty() || !f) return;
    SDL_Surface* surf = TTF_RenderText_Blended(f, text.c_str(), color);
    if (!surf) return;
    auto tex = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(r, surf), SDL_DestroyTexture);
    SDL_Rect dst{x, y, surf->w, surf->h};
    SDL_RenderCopy(r, tex.get(), nullptr, &dst);
    SDL_FreeSurface(surf);
}

inline void drawTextCentered(SDL_Renderer* r, TTF_Font* f, const std::string& text, const SDL_Rect& rect, SDL_Color color = {255,255,255,255}) {
    if (text.empty() || !f) return;
    SDL_Surface* surf = TTF_RenderText_Blended(f, text.c_str(), color);
    if (!surf) return;
    auto tex = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(r, surf), SDL_DestroyTexture);
    SDL_Rect dst{rect.x + (rect.w - surf->w)/2, rect.y + (rect.h - surf->h)/2, surf->w, surf->h};
    SDL_RenderCopy(r, tex.get(), nullptr, &dst);
    SDL_FreeSurface(surf);
}

inline bool pointInRect(int x, int y, const SDL_Rect& r) {
    return x >= r.x && x <= r.x + r.w && y >= r.y && y <= r.y + r.h;
}

inline void drawRoundedRect(SDL_Renderer* r, const SDL_Rect& rect, SDL_Color color) {
    SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(r, &rect);
}

inline std::string openFileDialog() {
#ifdef _WIN32
    OPENFILENAMEA ofn{}; char file[512]{};
    ofn.lStructSize = sizeof(ofn); ofn.lpstrFile = file; ofn.nMaxFile = sizeof(file);
    ofn.lpstrFilter = "Images\0*.png;*.jpg;*.jpeg;*.bmp\0";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (GetOpenFileNameA(&ofn)) return std::string(file);
#endif
    return "";
}

inline void handleEvent(Settings& s, const SDL_Event& e) {
    int stageX = s.leftPanelW + s.margin;
    int stageW = s.width - stageX - s.margin;
    int bottomY = s.margin + s.stageH + s.margin;
    int propW = (stageW / 2) - 10;
    int listX = stageX + propW + 20;

    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        int mx = e.button.x; int my = e.button.y;

        if (s.bgLibraryOpen) {
            SDL_Rect menuRect{s.width - 240, s.height - 400, 220, 320};
            if (pointInRect(mx, my, menuRect)) {
                SDL_Rect uploadBtn{menuRect.x + 10, menuRect.y + 45, 200, 40};
                if (pointInRect(mx, my, uploadBtn)) {
                    std::string path = openFileDialog();
                    if (!path.empty()) {
                        auto tex = loadTexture(s.renderer, path);
                        std::string name = path.substr(path.find_last_of("/\\") + 1);
                        auto bg = std::make_shared<Background>();
                        bg->name = name;
                        bg->texture = tex;
                        s.bgLibrary.push_back(bg);
                        s.currentBg = s.bgLibrary.back();
                        s.bgLibraryOpen = false;
                    }
                    return;
                }
                for (size_t i = 0; i < s.bgLibrary.size(); ++i) {
                    SDL_Rect item{menuRect.x + 10, menuRect.y + 95 + static_cast<int>(i) * 45, 200, 40};
                    if (pointInRect(mx, my, item)) {
                        s.currentBg = s.bgLibrary[i];
                        s.bgLibraryOpen = false;
                        return;
                    }
                }
            } else { s.bgLibraryOpen = false; return; }
        }

        if (pointInRect(mx, my, {s.width - 70, s.height - 70, 50, 50})) {
            s.bgLibraryOpen = !s.bgLibraryOpen; return;
        }

        if (pointInRect(mx, my, {s.width - 140, s.height - 70, 50, 50})) {
            SDL_Color rnd = {static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), static_cast<Uint8>(rand()%255), 255};
            auto sp = std::make_shared<Sprite>();
            sp->name = "Sprite " + std::to_string(s.sprites.size()+1);
            sp->texture = createColorTexture(s.renderer, 60, 60, rnd);
            sp->rect = {450, 100, 60, 60};
            sp->angle = 0.0;
            sp->scale = 1.0;
            sp->visible = true;
            s.sprites.push_back(sp);
            s.selectedSprite = static_cast<int>(s.sprites.size()) - 1;
            return;
        }

        for (size_t i = 0; i < s.sprites.size(); ++i) {
            SDL_Rect itemRect{listX + 10 + static_cast<int>(i) * 75, bottomY + 15, 65, 65};
            if (pointInRect(mx, my, itemRect)) { s.selectedSprite = static_cast<int>(i); return; }
        }

        SDL_Rect stageRect{stageX, s.margin, stageW, s.stageH};
        if (pointInRect(mx, my, stageRect)) {
            for (int i = static_cast<int>(s.sprites.size()) - 1; i >= 0; --i) {
                auto& sp = s.sprites[i];
                SDL_Rect r = sp->rect;
                r.w = static_cast<int>(r.w * sp->scale);
                r.h = static_cast<int>(r.h * sp->scale);
                if (pointInRect(mx, my, r)) {
                    s.selectedSprite = i; s.dragging = true;
                    s.dragOffsetX = mx - sp->rect.x; s.dragOffsetY = my - sp->rect.y;
                    return;
                }
            }
        }
    }
    if (e.type == SDL_MOUSEBUTTONUP) s.dragging = false;
    if (e.type == SDL_MOUSEMOTION && s.dragging && s.selectedSprite != -1) {
        s.sprites[s.selectedSprite]->rect.x = e.motion.x - s.dragOffsetX;
        s.sprites[s.selectedSprite]->rect.y = e.motion.y - s.dragOffsetY;
    }
}

inline void render(const Settings& s) {
    SDL_Renderer* r = s.renderer;
    SDL_SetRenderDrawColor(r, 235, 242, 255, 255); SDL_RenderClear(r);

    int stageX = s.leftPanelW + s.margin;
    int stageW = s.width - stageX - s.margin;

    drawRoundedRect(r, {0, 0, s.leftPanelW, s.height}, {255, 255, 255, 255});
    SDL_SetRenderDrawColor(r, 220, 220, 220, 255); SDL_RenderDrawLine(r, s.leftPanelW, 0, s.leftPanelW, s.height);

    SDL_Rect stageRect{stageX, s.margin, stageW, s.stageH};
    drawRoundedRect(r, stageRect, {255, 255, 255, 255});
    if (s.currentBg && s.currentBg->texture) SDL_RenderCopy(r, s.currentBg->texture.get(), nullptr, &stageRect);
    SDL_SetRenderDrawColor(r, 190, 190, 190, 255); SDL_RenderDrawRect(r, &stageRect);

    SDL_RenderSetClipRect(r, &stageRect);
    for (const auto& sp : s.sprites) {
        SDL_Rect dst = sp->rect;
        dst.w = static_cast<int>(dst.w * sp->scale);
        dst.h = static_cast<int>(dst.h * sp->scale);
        SDL_RenderCopyEx(r, sp->texture.get(), nullptr, &dst, sp->angle, nullptr, SDL_FLIP_NONE);
        if (s.selectedSprite != -1 && s.sprites[s.selectedSprite] == sp) {
            SDL_SetRenderDrawColor(r, 76, 151, 255, 255); SDL_RenderDrawRect(r, &dst);
        }
    }
    SDL_RenderSetClipRect(r, nullptr);

    int bottomY = s.margin + s.stageH + s.margin;
    int bottomH = s.height - bottomY - s.margin;
    int propW = (stageW / 2) - 10;

    SDL_Rect propRect{stageX, bottomY, propW, bottomH};
    drawRoundedRect(r, propRect, {255, 255, 255, 255});
    SDL_SetRenderDrawColor(r, 210, 210, 210, 255); SDL_RenderDrawRect(r, &propRect);
    if (s.selectedSprite != -1) {
        auto& sp = s.sprites[s.selectedSprite];
        drawText(r, s.fontBold, "Sprite Properties", propRect.x+15, propRect.y+15, {76, 151, 255, 255});
        drawText(r, s.font, "Name: " + sp->name, propRect.x+15, propRect.y+60);
        drawText(r, s.font, "Pos: X=" + std::to_string(sp->rect.x) + " Y=" + std::to_string(sp->rect.y), propRect.x+15, propRect.y+100);
    }

    int listX = stageX + propW + 20;
    SDL_Rect listRect{listX, bottomY, propW, bottomH};
    drawRoundedRect(r, listRect, {248, 250, 255, 255});
    SDL_SetRenderDrawColor(r, 210, 210, 210, 255); SDL_RenderDrawRect(r, &listRect);
    for (size_t i = 0; i < s.sprites.size(); ++i) {
        SDL_Rect item{listRect.x+10+static_cast<int>(i)*75, listRect.y+15, 65, 65};
        drawRoundedRect(r, item, {255,255,255,255});
        if (s.selectedSprite == static_cast<int>(i)) {
            SDL_SetRenderDrawColor(r, 76, 151, 255, 255);
            SDL_RenderDrawRect(r, &item);
        }
        SDL_Rect iconR{item.x+10, item.y+10, 45, 45};
        SDL_RenderCopy(r, s.sprites[i]->texture.get(), nullptr, &iconR);
    }

    SDL_Rect sBtn{s.width-140, s.height-70, 50, 50};
    drawRoundedRect(r, sBtn, {76, 151, 255, 255}); drawTextCentered(r, s.fontBold, "+S", sBtn);
    SDL_Rect bBtn{s.width-70, s.height-70, 50, 50};
    drawRoundedRect(r, bBtn, {76, 151, 255, 255}); drawTextCentered(r, s.fontBold, "BG", bBtn);

    if (s.bgLibraryOpen) {
        SDL_Rect menuRect{s.width - 240, s.height - 400, 220, 320};
        drawRoundedRect(r, menuRect, {255, 255, 255, 255});
        SDL_SetRenderDrawColor(r, 180, 180, 180, 255); SDL_RenderDrawRect(r, &menuRect);
        drawText(r, s.fontBold, "Backdrops", menuRect.x+15, menuRect.y+15);

        SDL_Rect upBtn{menuRect.x+10, menuRect.y+45, 200, 40};
        drawRoundedRect(r, upBtn, {100, 100, 100, 255});
        drawTextCentered(r, s.font, "Upload Image", upBtn);

        for (size_t i = 0; i < s.bgLibrary.size(); ++i) {
            SDL_Rect item{menuRect.x+10, menuRect.y+95+static_cast<int>(i)*45, 200, 40};
            bool cur = (s.bgLibrary[i] == s.currentBg);
            drawRoundedRect(r, item, cur ? SDL_Color{230,240,255,255} : SDL_Color{245,245,245,255});
            drawText(r, s.font, s.bgLibrary[i]->name, item.x+10, item.y+10);
            if(cur) { SDL_SetRenderDrawColor(r, 76, 151, 255, 255); SDL_RenderDrawRect(r, &item); }
        }
    }
}

#endif
