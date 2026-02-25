#ifndef SCRATCH_HELP_H
#define SCRATCH_HELP_H
#include "event.h"
struct HelpState {
    bool active = false;
    int  tab    = 0; // 0=Motion, 1=Sound, 2=Control, 3=Events
};

// رندر پنجره راهنما
void renderHelp(HelpState& hs) {
    if (!hs.active) return;

    int wx = wt / 2 - 300, wy = ht / 2 - 250;
    int ww = 600,           wh = 500;
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
    SDL_Rect overlay = {0, 0, wt, ht};
    SDL_RenderFillRect(renderer, &overlay);
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    SDL_Rect win = {wx, wy, ww, wh};
    SDL_RenderFillRect(renderer, &win);

    SDL_SetRenderDrawColor(renderer, 50, 100, 200, 255);
    SDL_Rect titleBar = {wx, wy, ww, 40};
    SDL_RenderFillRect(renderer, &titleBar);
    textRGBA(renderer, wx + 10, wy + 10,
             "Help - راهنما", "tahoma.ttf", 16,
             255, 255, 255, 255);

    // دکمه بستن X
    SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);
    SDL_Rect closeBtn = {wx + ww - 35, wy + 5, 30, 30};
    SDL_RenderFillRect(renderer, &closeBtn);
    textRGBA(renderer, wx + ww - 25, wy + 10,
             "X", "tahomabd.ttf", 14,
             255, 255, 255, 255);

    // تب‌ها
    const char* tabs[] = {"Motion", "Sound", "Control", "Events"};
    for (int i = 0; i < 4; i++) {
        SDL_Rect tabR = {wx + 10 + i * 145, wy + 45, 140, 30};
        if (hs.tab == i)
            SDL_SetRenderDrawColor(renderer, 100, 150, 255, 255);
        else
            SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
        SDL_RenderFillRect(renderer, &tabR);
        textRGBA(renderer, tabR.x + 10, tabR.y + 7,
                 tabs[i], "tahoma.ttf", 13,
                 0, 0, 0, 255);
    }

    // محتوای هر تب
    int cy = wy + 90;
    int cx = wx + 15;

    if (hs.tab == 0) { // Motion
        textRGBA(renderer, cx, cy,      "move N steps",
                 "tahoma.ttf", 13, 30, 30, 30, 255);
        textRGBA(renderer, cx + 20, cy + 18, "کاراکتر را N قدم حرکت میدهد",
                 "tahoma.ttf", 12, 80, 80, 80, 255);

        textRGBA(renderer, cx, cy + 50,  "set x to N",
                 "tahoma.ttf", 13, 30, 30, 30, 255);
        textRGBA(renderer, cx + 20, cy + 68, "موقعیت افقی را تنظیم میکند",
                 "tahoma.ttf", 12, 80, 80, 80, 255);

        textRGBA(renderer, cx, cy + 100, "set y to N",
                 "tahoma.ttf", 13, 30, 30, 30, 255);
        textRGBA(renderer, cx + 20, cy + 118, "موقعیت عمودی را تنظیم میکند",
                 "tahoma.ttf", 12, 80, 80, 80, 255);

        textRGBA(renderer, cx, cy + 150, "turn N degrees",
                 "tahoma.ttf", 13, 30, 30, 30, 255);
        textRGBA(renderer, cx + 20, cy + 168, "کاراکتر را N درجه می‌چرخاند",
                 "tahoma.ttf", 12, 80, 80, 80, 255);
    }
    else if (hs.tab == 1) { // Sound
        textRGBA(renderer, cx, cy,      "play sound NAME",
                 "tahoma.ttf", 13, 30, 30, 30, 255);
        textRGBA(renderer, cx + 20, cy + 18, "صدای انتخابی را پخش میکند",
                 "tahoma.ttf", 12, 80, 80, 80, 255);

        textRGBA(renderer, cx, cy + 50,  "stop all sounds",
                 "tahoma.ttf", 13, 30, 30, 30, 255);
        textRGBA(renderer, cx + 20, cy + 68, "تمام صداها را متوقف میکند",
                 "tahoma.ttf", 12, 80, 80, 80, 255);

        textRGBA(renderer, cx, cy + 100, "set volume to N%",
                 "tahoma.ttf", 13, 30, 30, 30, 255);
        textRGBA(renderer, cx + 20, cy + 118, "صدا را روی N درصد تنظیم میکند",
                 "tahoma.ttf", 12, 80, 80, 80, 255);

        textRGBA(renderer, cx, cy + 150, "change volume by N",
                 "tahoma.ttf", 13, 30, 30, 30, 255);
        textRGBA(renderer, cx + 20, cy + 168, "صدا را N واحد تغییر میدهد",
                 "tahoma.ttf", 12, 80, 80, 80, 255);
    }
    else if (hs.tab == 2) { // Control
        textRGBA(renderer, cx, cy,      "wait N seconds",
                 "tahoma.ttf", 13, 30, 30, 30, 255);
        textRGBA(renderer, cx + 20, cy + 18, "N ثانیه صبر میکند",
                 "tahoma.ttf", 12, 80, 80, 80, 255);

        textRGBA(renderer, cx, cy + 50,  "repeat N",
                 "tahoma.ttf", 13, 30, 30, 30, 255);
        textRGBA(renderer, cx + 20, cy + 68, "بلوک‌های داخل را N بار اجرا میکند",
                 "tahoma.ttf", 12, 80, 80, 80, 255);

        textRGBA(renderer, cx, cy + 100, "forever",
                 "tahoma.ttf", 13, 30, 30, 30, 255);
        textRGBA(renderer, cx + 20, cy + 118, "بلوک‌های داخل را بی‌نهایت اجرا میکند",
                 "tahoma.ttf", 12, 80, 80, 80, 255);

        textRGBA(renderer, cx, cy + 150, "if <condition> then",
                 "tahoma.ttf", 13, 30, 30, 30, 255);
        textRGBA(renderer, cx + 20, cy + 168, "اگر شرط درست بود اجرا میکند",
                 "tahoma.ttf", 12, 80, 80, 80, 255);
    }
    else if (hs.tab == 3) { // Events
        textRGBA(renderer, cx, cy,      "when run clicked",
                 "tahoma.ttf", 13, 30, 30, 30, 255);
        textRGBA(renderer, cx + 20, cy + 18, "وقتی دکمه سبز کلیک شود شروع میکند",
                 "tahoma.ttf", 12, 80, 80, 80, 255);

        textRGBA(renderer, cx, cy + 50,  "when key pressed",
                 "tahoma.ttf", 13, 30, 30, 30, 255);
        textRGBA(renderer, cx + 20, cy + 68, "وقتی کلید فشرده شود اجرا میشود",
                 "tahoma.ttf", 12, 80, 80, 80, 255);
    }
}

// بررسی کلیک روی راهنما
bool handleHelp(HelpState& hs, int mx, int my) {
    if (!hs.active) return false;

    int wx = wt / 2 - 300, wy = ht / 2 - 250;
    int ww = 600;

    // دکمه بستن
    if (mx >= wx + ww - 35 && mx <= wx + ww - 5 &&
        my >= wy + 5       && my <= wy + 35) {
        hs.active = false;
        return true;
    }

    // کلیک روی تب‌ها
    for (int i = 0; i < 4; i++) {
        int tx = wx + 10 + i * 145;
        if (mx >= tx && mx <= tx + 140 &&
            my >= wy + 45 && my <= wy + 75) {
            hs.tab = i;
            return true;
        }
    }

    return true; // کلیک داخل پنجره رو بلاک کن
}
#endif
