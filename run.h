#ifndef SCRATCH_RUN_H
#define SCRATCH_RUN_H
#include "code1.h"
#include "settings.h"
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

SDL_Texture* run = nullptr;
vector<Character>      sp;   // runtime copies of sprites
vector<BackgroundState> co;  // runtime copies of backgrounds

// Global pointers set in main() so the execution engine can
// read/write the canonical editor state.
CharacterManager* g_charMgr = nullptr;
BackgroundState*  g_bgState  = nullptr;

// True while the program is running (green flag clicked, not yet stopped)
bool g_isRunning = false;

// Per-frame key state for sensing blocks (space, arrows, letters, digits)
bool g_keyState[SDL_NUM_SCANCODES] = {};

struct dxd {
    int i{};    // 0 = sprite lane, 1 = background lane
    int idx{};  // index into sp[] or co[]
};

void rust() {
    run = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                            SDL_TEXTUREACCESS_TARGET, 500, 400);
    tex.push_back(run);
    SDL_SetRenderTarget(renderer, run);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, nullptr);
}

// ---------- forward declaration ----------
void exebl(int& idx, const block* px, dxd& sb, codtxs& cod, int codIdx);

// ---------- helper: map Scratch key name → scancode ----------
static bool checkScratchKey(const string& name) {
    if (name == "space")       return g_keyState[SDL_SCANCODE_SPACE];
    if (name == "up arrow")    return g_keyState[SDL_SCANCODE_UP];
    if (name == "down arrow")  return g_keyState[SDL_SCANCODE_DOWN];
    if (name == "right arrow") return g_keyState[SDL_SCANCODE_RIGHT];
    if (name == "left arrow")  return g_keyState[SDL_SCANCODE_LEFT];
    if (name == "any") {
        for (int i = 0; i < SDL_NUM_SCANCODES; i++) if (g_keyState[i]) return true;
        return false;
    }
    if (name.size() == 1) {
        char c = name[0];
        if (c >= 'a' && c <= 'z') return g_keyState[SDL_SCANCODE_A + (c - 'a')];
        if (c >= '0' && c <= '9') return g_keyState[SDL_SCANCODE_0 + (c - '0')];
    }
    return false;
}

// ---------- helper: evaluate a boolean condition block ----------
static bool evalCond(const block* px, const dxd& sb) {
    if (!px) return false;
    const string& name = px->pn[0];
    auto numv = [](const string& s) -> double {
        if (s.empty()) return 0.0;
        try { return stod(s); } catch (...) { return 0.0; }
    };
    if (name == "<") {
        double a = px->candy[0] ? 0 : numv(px->v[0]);
        double b = px->candy[1] ? 0 : numv(px->v[1]);
        return a < b;
    }
    if (name == ">") {
        double a = px->candy[0] ? 0 : numv(px->v[0]);
        double b = px->candy[1] ? 0 : numv(px->v[1]);
        return a > b;
    }
    if (name == "=")  { return px->v[0] == px->v[1]; }
    if (name == "not"  && px->candy[0]) return !evalCond(px->candy[0], sb);
    if (name == "and"  && px->candy[0] && px->candy[1])
        return evalCond(px->candy[0], sb) && evalCond(px->candy[1], sb);
    if (name == "or"   && px->candy[0] && px->candy[1])
        return evalCond(px->candy[0], sb) || evalCond(px->candy[1], sb);
    // Sensing: key pressed
    if (name == "key"  && px->pn[1] == "pressed") return checkScratchKey(px->mod[0]);
    if (name == "mouse down?") {
        int mx, my;
        return (SDL_GetMouseState(&mx, &my) & SDL_BUTTON(1)) != 0;
    }
    return false;
}

// ---------- main block executor ----------
// idx    : current block index within cod.data[codIdx]; advance to move to next
// px     : the block to execute (= cod.data[codIdx][idx])
// sb     : which character/background this script belongs to
// cod    : the codtxs that owns the script
// codIdx : index of this script within cod.data
void exebl(int& idx, const block* px, dxd& sb, codtxs& cod, int codIdx) {
    if (!px) { idx++; return; }
    const string& name = px->pn[0];
    auto numv = [](const string& s) -> double {
        if (s.empty()) return 0.0;
        try { return stod(s); } catch (...) { return 0.0; }
    };

    // ── EVENT blocks (category 3): triggers, not actions ──
    if (px->color == color(3)) { idx++; return; }

    // ── CONTROL blocks (category 4) ──────────────────────────────────────────
    if (px->color == color(4)) {
        // ----- wait N seconds -----
        if (name == "wait") {
            Uint32 now = SDL_GetTicks();
            if (cod.waitT[codIdx] == 0) {
                Uint32 ms = static_cast<Uint32>(numv(px->v[0]) * 1000.0);
                cod.waitT[codIdx] = now + (ms == 0 ? 1 : ms);
            }
            if (now >= cod.waitT[codIdx]) {
                cod.waitT[codIdx] = 0;
                idx++;
            }
            // Do NOT increment idx if still waiting
            return;
        }
        // ----- wait until <condition> -----
        if (name == "wait until") {
            if (!px->candy[0] || evalCond(px->candy[0], sb))
                idx++;
            return;
        }
        // ----- forever -----
        if (name == "forever") {
            if (px->hofre[0] && !px->hof[0].empty()) {
                int& hi = cod.hofIdx[codIdx];
                if (hi < 0 || hi >= (int)px->hof[0].size()) hi = 0;
                int dummy = 0;
                exebl(dummy, px->hof[0][hi], sb, cod, codIdx);
                if (dummy > 0) {          // child completed this frame
                    hi++;
                    if (hi >= (int)px->hof[0].size()) hi = 0;
                }
                // if dummy == 0: child is still pending (wait) — keep hi
            }
            // Do NOT advance idx — forever loops.
            return;
        }
        // ----- repeat N -----
        if (name == "repeat") {
            int& rc = cod.repeatCnt[codIdx];
            int& hi = cod.hofIdx[codIdx];
            if (rc < 0) { rc = static_cast<int>(numv(px->v[0])); hi = 0; }
            if (rc <= 0) { rc = -1; hi = 0; idx++; return; }
            if (px->hofre[0] && !px->hof[0].empty()) {
                int dummy = 0;
                exebl(dummy, px->hof[0][hi], sb, cod, codIdx);
                if (dummy > 0) {
                    hi++;
                    if (hi >= (int)px->hof[0].size()) { hi = 0; rc--; }
                }
            } else { rc = -1; idx++; }
            return;
        }
        // ----- repeat until <condition> -----
        if (name == "repeat until") {
            int& rc = cod.repeatCnt[codIdx];
            int& hi = cod.hofIdx[codIdx];
            if (rc < 0) { rc = 0; hi = 0; }
            if (px->candy[0] && evalCond(px->candy[0], sb)) { rc = -1; hi = 0; idx++; return; }
            if (px->hofre[0] && !px->hof[0].empty()) {
                int dummy = 0;
                exebl(dummy, px->hof[0][hi], sb, cod, codIdx);
                if (dummy > 0) {
                    hi++;
                    if (hi >= (int)px->hof[0].size()) hi = 0;
                }
            }
            return;
        }
        // ----- if <condition> then [ else ] -----
        if (name == "if") {
            int& phase = cod.repeatCnt[codIdx]; // 0=if-branch, 1=else-branch
            int& hi    = cod.hofIdx[codIdx];
            if (phase < 0) { phase = 0; hi = 0; }
            bool cond  = px->candy[0] ? evalCond(px->candy[0], sb) : false;
            if (phase == 0) {
                if (cond && px->hofre[0] && !px->hof[0].empty()) {
                    int dummy = 0;
                    exebl(dummy, px->hof[0][hi], sb, cod, codIdx);
                    if (dummy > 0) {
                        hi++;
                        if (hi >= (int)px->hof[0].size()) { phase = -1; hi = 0; idx++; }
                    }
                } else { phase = px->hofre[1] ? 1 : -1; hi = 0; if (phase < 0) idx++; }
                return;
            }
            if (phase == 1) {
                if (px->hofre[1] && !px->hof[1].empty()) {
                    int dummy = 0;
                    exebl(dummy, px->hof[1][hi], sb, cod, codIdx);
                    if (dummy > 0) {
                        hi++;
                        if (hi >= (int)px->hof[1].size()) { phase = -1; hi = 0; idx++; }
                    }
                } else { phase = -1; hi = 0; idx++; }
                return;
            }
        }
        // ----- stop -----
        if (name == "stop") {
            const string& opt = px->mod[0];
            if (opt == "all") {
                g_isRunning = false;
                for (int ii = 0; ii < 2; ii++)
                    for (auto& c2 : spco[ii])
                        for (int kk = 0; kk < (int)c2.exeb.size(); kk++) {
                            c2.exeb[kk]      = false;
                            c2.exen[kk]      = 0;
                            c2.waitT[kk]     = 0;
                            c2.hofIdx[kk]    = 0;
                            c2.repeatCnt[kk] = -1;
                        }
            } else if (opt == "this script") {
                cod.exeb[codIdx]      = false;
                cod.exen[codIdx]      = 0;
                cod.waitT[codIdx]     = 0;
                cod.hofIdx[codIdx]    = 0;
                cod.repeatCnt[codIdx] = -1;
            }
            return;
        }
        // Unknown control block → skip
        idx++; return;
    }

    // ── MOTION blocks (category 0, orange) ──────────────────────────────────
    if (px->color == color(0)) {
        if (sb.i == 0 && sb.idx < (int)sp.size()) {
            Character& ch = sp[sb.idx];
            if (name == "move" && px->pn[1] == "steps") {
                double steps = numv(px->v[0]);
                // rotation: 90 = right, 0 = up, -90/270 = left, 180 = down
                double rad = (ch.rotation - 90.0) * M_PI / 180.0;
                ch.x += static_cast<int>(steps * cos(rad));
                ch.y += static_cast<int>(steps * sin(rad));
            }
            else if (name == "turn   " && !px->order.empty() && px->order[0] == 1) {
                ch.rotation += static_cast<float>(numv(px->v[0]));
            }
            else if (name == "turn   " && !px->order.empty() && px->order[0] == -1) {
                ch.rotation -= static_cast<float>(numv(px->v[0]));
            }
            else if (name == "go to x:" && px->pn[1] == "y:") {
                ch.x = STAGE_X + static_cast<int>(numv(px->v[0]));
                ch.y = STAGE_Y + static_cast<int>(numv(px->v[1]));
            }
            else if (name == "change x by") {
                ch.x += static_cast<int>(numv(px->v[0]));
            }
            else if (name == "change y by") {
                ch.y -= static_cast<int>(numv(px->v[0]));
            }
            else if (name == "point in direction") {
                ch.rotation = static_cast<float>(numv(px->v[0]));
            }
            else if (name == "if on edge, bounce") {
                if (ch.x < STAGE_X) {
                    ch.x = STAGE_X;
                    ch.rotation = -ch.rotation;
                }
                if (ch.x + ch.width > STAGE_X + STAGE_W) {
                    ch.x = STAGE_X + STAGE_W - ch.width;
                    ch.rotation = -ch.rotation;
                }
                if (ch.y < STAGE_Y) {
                    ch.y = STAGE_Y;
                    ch.rotation = 180.0f - ch.rotation;
                }
                if (ch.y + ch.height > STAGE_Y + STAGE_H) {
                    ch.y = STAGE_Y + STAGE_H - ch.height;
                    ch.rotation = 180.0f - ch.rotation;
                }
            }
            else if (name == "go to" && px->mod[0] == "random position") {
                ch.x = STAGE_X + rand() % max(1, STAGE_W - ch.width);
                ch.y = STAGE_Y + rand() % max(1, STAGE_H - ch.height);
            }
        }
        idx++; return;
    }

    // ── LOOKS blocks (category 1) ────────────────────────────────────────────
    if (px->color == color(1)) {
        if (sb.i == 0 && sb.idx < (int)sp.size()) {
            Character& ch = sp[sb.idx];
            if (name == "show")  ch.visible = true;
            else if (name == "hide") ch.visible = false;
            else if (name == "set size to") {
                double pct = numv(px->v[0]) / 100.0;
                if (pct > 0.0) {
                    ch.width  = max(1, static_cast<int>(100.0 * pct));
                    ch.height = max(1, static_cast<int>(100.0 * pct));
                }
            }
            else if (name == "change size by") {
                double delta = numv(px->v[0]);
                ch.width  = max(1, ch.width  + static_cast<int>(delta));
                ch.height = max(1, ch.height + static_cast<int>(delta));
            }
            else if (name == "say" || name == "think") {
                SDL_SetRenderTarget(renderer, run);
                int bx = ch.x - STAGE_X + ch.width;
                int by = ch.y - STAGE_Y;
                string txt = px->v[0];
                if (!txt.empty()) {
                    SDL_Rect bubble = {bx, by, static_cast<int>(txt.size()) * 7 + 10, 22};
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 220);
                    SDL_RenderFillRect(renderer, &bubble);
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &bubble);
                }
                SDL_SetRenderTarget(renderer, nullptr);
            }
        }
        idx++; return;
    }

    // ── All other blocks: skip forward ────────────────────────────────────────
    idx++;
}

// ---------- execute() — called once per frame from main loop ---------------
bool execute() {
    bool b = false;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < (int)spco[i].size(); j++) {
            if (i == 0 && j >= (int)sp.size()) continue;
            if (i == 1 && j >= (int)co.size()) continue;

            codtxs& cod = spco[i][j];
            for (int k = 0; k < (int)cod.exeb.size(); k++) {
                if (!cod.exeb[k]) continue;
                if (cod.exen[k] >= (int)cod.data[k].size()) {
                    cod.exeb[k]      = false;
                    cod.exen[k]      = 0;
                    cod.waitT[k]     = 0;
                    cod.hofIdx[k]    = 0;
                    cod.repeatCnt[k] = -1;
                    continue;
                }
                dxd sx; sx.i = i; sx.idx = j;
                exebl(cod.exen[k],
                      cod.data[k][cod.exen[k]],
                      sx, cod, k);
                b = true;
                if (cod.exen[k] >= (int)cod.data[k].size()) {
                    cod.exeb[k]      = false;
                    cod.exen[k]      = 0;
                    cod.waitT[k]     = 0;
                    cod.hofIdx[k]    = 0;
                    cod.repeatCnt[k] = -1;
                }
            }
        }
    }
    // Sync modified runtime characters back to the editor manager,
    // but ONLY when scripts are actively running so mouse drag isn't
    // overwritten every frame after execution finishes.
    if (b && g_charMgr) {
        for (int j = 0; j < (int)sp.size() && j < g_charMgr->count; j++) {
            Character& ec = g_charMgr->characters[j];
            ec.x        = sp[j].x;
            ec.y        = sp[j].y;
            ec.rotation = sp[j].rotation;
            ec.visible  = sp[j].visible;
            ec.width    = sp[j].width;
            ec.height   = sp[j].height;
        }
    }
    return b;
}

// ---------- helper: map SDL_Keycode → Scratch key name ----------------------
static string sdlKeyToScratch(SDL_Keycode key) {
    switch (key) {
        case SDLK_SPACE: return "space";
        case SDLK_UP:    return "up arrow";
        case SDLK_DOWN:  return "down arrow";
        case SDLK_RIGHT: return "right arrow";
        case SDLK_LEFT:  return "left arrow";
        default:
            if (key >= SDLK_a && key <= SDLK_z)
                return string(1, static_cast<char>(key));
            if (key >= SDLK_0 && key <= SDLK_9)
                return string(1, static_cast<char>(key));
            return "";
    }
}

// ---------- clikey() — call from SDL_KEYDOWN to start key-event scripts -----
void clikey(SDL_Keycode key) {
    if (!g_isRunning) return;
    string scrKey = sdlKeyToScratch(key);
    if (scrKey.empty()) return;

    for (int i = 0; i < 2; i++) {
        for (auto& j : spco[i]) {
            for (int k = 0; k < (int)j.data.size(); k++) {
                if (j.data[k].empty()) continue;
                const block* b0 = j.data[k][0];
                // Match "when [key] key pressed"
                if (b0->pn[0] == "when" && b0->pn[1] == "key pressed") {
                    if (b0->mod[0] == scrKey || b0->mod[0] == "any") {
                        j.exeb[k]      = true;
                        j.exen[k]      = 0;
                        j.waitT[k]     = 0;
                        j.hofIdx[k]    = 0;
                        j.repeatCnt[k] = -1;
                    }
                }
            }
        }
    }
}

// ---------- clistop() — stop all execution (red button) --------------------
bool clistop(int x, int y) {
    if (x > wt-570 && x < wt-540 && y > 45 && y < 75) {
        g_isRunning = false;
        for (int i = 0; i < 2; i++)
            for (auto& j : spco[i])
                for (int k = 0; k < (int)j.exeb.size(); k++) {
                    j.exeb[k]      = false;
                    j.exen[k]      = 0;
                    j.waitT[k]     = 0;
                    j.hofIdx[k]    = 0;
                    j.repeatCnt[k] = -1;
                }
        return true;
    }
    return false;
}

// ---------- cligri() — called when the green button is clicked -------------
bool cligri(int x, int y) {
    if (x > wt-535 && x < wt-505 && y > 45 && y < 75) {
        // Snapshot current editor characters/backgrounds as runtime state
        sp.clear();
        if (g_charMgr)
            for (int c = 0; c < g_charMgr->count; c++)
                sp.push_back(g_charMgr->characters[c]);
        co.clear();
        if (g_bgState) co.push_back(*g_bgState);

        // Clear the run texture
        SDL_SetRenderTarget(renderer, run);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderTarget(renderer, nullptr);

        g_isRunning = true;

        // Start every script that begins with a green-flag event
        for (int i = 0; i < 2; i++) {
            for (auto& j : spco[i]) {

                for (int k = 0; k < (int)j.exeb.size(); k++) {
                    j.exeb[k]      = false;
                    j.exen[k]      = 0;
                    j.waitT[k]     = 0;
                    j.hofIdx[k]    = 0;
                    j.repeatCnt[k] = -1;
                }
                for (int k = 0; k < (int)j.data.size(); k++) {
                    if (j.data[k].empty()) continue;
                    const string& ev = j.data[k][0]->pn[0];
                    // "when run clicked" (sprites) or "when clicked" (stage green flag)
                    if (ev == "when run clicked" || ev == "when clicked") {
                        j.exeb[k] = true;
                        j.exen[k] = 0;
                    }
                }
            }
        }
        return true;
    }
    return false;
}
#endif
