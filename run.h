#ifndef SCRATCH_RUN_H
#define SCRATCH_RUN_H
#include "code1.h"
#include "settings.h"
#include <cmath>
#include <map>
#include <sstream>
#include <iomanip>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

SDL_Texture* run = nullptr;
vector<Character>       sp;
vector<BackgroundState> co;

CharacterManager* g_charMgr = nullptr;
BackgroundState*  g_bgState  = nullptr;

static map<const block*, Uint32> g_blkWaitT;
static map<const block*, int>    g_blkHofIdx;
static map<const block*, int>    g_blkRepCnt;

static bool g_keyState[512] = {};

static map<string, string> g_vars;

static bool   g_askActive = false;  // currently waiting for user input
static bool   g_askDone   = false;  // user pressed Enter
static string g_askBuf;             // typed text
static string g_answer;             // result of last ask
static Uint32 g_timerBase = 0;      // for "timer" reporter

struct dxd { int i{}; int idx{}; };

static bool hasReporter(const block* px, int x) {
    for (int o : px->order) if (o/10 == 8 && o%10 == x) return true;
    return false;
}

static bool hasCondition(const block* px, int x) {
    for (int o : px->order) if (o/10 == 3 && o%10 == x) return true;
    return false;
}

static double evalNum(const block* px, const dxd& sb);
static bool   evalCond(const block* px, const dxd& sb);
static string evalStr(const block* px, const dxd& sb);

void exebl(int& i, const block* px, dxd& sb) {
    const string& name = px->pn[0];

    if (name == "play sound") {
        soundMgr.play(px->v[0]);
    }
    else if (name == "play sound until done") {
        soundMgr.play(px->v[0]);
        SDL_Delay(500); 
    }
    else if (name == "stop all sounds") {
        soundMgr.stopAll();
    }
    else if (name == "change volume by") {
        soundMgr.changeVolume(stoi(px->v[0]));
    }
    else if (name == "set volume to") {
        soundMgr.setVolume(stoi(px->v[0]));
    }

    i++;
}
;
void doBroadcast(const string& msgName);
static void stopAll();
static void ensureVectors(codtxs& cod);


static double numv(const string& s) {
    if (s.empty()) return 0.0;
    try { return stod(s); } catch (...) { return 0.0; }
}

static string dtos(double v) {
    if (v == (long long)v) return to_string((long long)v);
    ostringstream ss; ss << setprecision(6) << v; return ss.str();
}

static void ensureVectors(codtxs& cod) {
    while ((int)cod.exeb.size() < (int)cod.data.size()) {
        cod.exen.push_back(0);
        cod.exeb.push_back(false);
        cod.waitT.push_back(0);
        cod.hofIdx.push_back(0);
        cod.repeatCnt.push_back(-1);
    }
}

static SDL_Scancode keyNameToScancode(const string& k) {
    if (k == "space")       return SDL_SCANCODE_SPACE;
    if (k == "up arrow")    return SDL_SCANCODE_UP;
    if (k == "down arrow")  return SDL_SCANCODE_DOWN;
    if (k == "left arrow")  return SDL_SCANCODE_LEFT;
    if (k == "right arrow") return SDL_SCANCODE_RIGHT;
    if (k == "any")         return SDL_SCANCODE_UNKNOWN; // special flag
    if (k.size() == 1) {
        char c = k[0];
        if (c >= 'a' && c <= 'z') return (SDL_Scancode)(SDL_SCANCODE_A + (c - 'a'));
        if (c >= '0' && c <= '9') return (SDL_Scancode)(SDL_SCANCODE_0 + (c - '0'));
    }
    return SDL_SCANCODE_UNKNOWN;
}

static bool isKeyDown(const string& k) {
    if (k == "any") {
        for (int i = 1; i < 512; i++) if (g_keyState[i]) return true;
        return false;
    }
    SDL_Scancode sc = keyNameToScancode(k);
    return sc != SDL_SCANCODE_UNKNOWN && sc < 512 && g_keyState[sc];
}

void notifyKeyDown(SDL_Scancode sc) { if (sc < 512) g_keyState[sc] = true; }
void notifyKeyUp  (SDL_Scancode sc) { if (sc < 512) g_keyState[sc] = false; }

void notifyKeyEvent(SDL_Scancode sc) {
    for (int i = 0; i < 2; i++) {
        for (auto& cod : spco[i]) {
            ensureVectors(cod);
            for (int k = 0; k < (int)cod.data.size(); k++) {
                if (cod.data[k].empty()) continue;
                const block* b = cod.data[k][0];
                if (!(b->pn[0] == "when" && b->pn[1] == "key pressed")) continue;
                bool matches = false;
                if (b->mod[0] == "any") {
                    matches = true;
                } else {
                    SDL_Scancode exp = keyNameToScancode(b->mod[0]);
                    matches = (exp != SDL_SCANCODE_UNKNOWN && exp == sc);
                }
                if (matches && !cod.exeb[k]) {
                    cod.exeb[k] = true;
                    cod.exen[k] = 1; // skip the event trigger
                }
            }
        }
    }
}

void notifySpriteClicked(int charIdx) {
    if (charIdx < 0 || charIdx >= (int)spco[0].size()) return;
    codtxs& cod = spco[0][charIdx];
    ensureVectors(cod);
    for (int k = 0; k < (int)cod.data.size(); k++) {
        if (cod.data[k].empty()) continue;
        if (cod.data[k][0]->pn[0] == "when this sprite clicked") {
            cod.exeb[k] = true;
            cod.exen[k] = 1;
        }
    }
}

static void stopAll() {
    for (int i = 0; i < 2; i++)
        for (auto& cod : spco[i])
            for (int kk = 0; kk < (int)cod.exeb.size(); kk++) {
                cod.exeb[kk] = false;
                cod.exen[kk] = 0;
            }
    g_blkWaitT.clear();
    g_blkHofIdx.clear();
    g_blkRepCnt.clear();
    g_askActive = g_askDone = false;
}

void doBroadcast(const string& msgName) {
    for (int i = 0; i < 2; i++) {
        for (auto& cod : spco[i]) {
            ensureVectors(cod);
            for (int k = 0; k < (int)cod.data.size(); k++) {
                if (cod.data[k].empty()) continue;
                const block* b = cod.data[k][0];
                if (b->pn[0] == "when i receive" && b->mod[0] == msgName) {
                    cod.exeb[k] = true;
                    cod.exen[k] = 1;
                }
            }
        }
    }
}

static double evalNum(const block* px, const dxd& sb) {
    if (!px) return 0.0;
    const string& name = px->pn[0];

    auto slotN = [&](int x) -> double {
        return (hasReporter(px, x) && px->vari[x]) ? evalNum(px->vari[x], sb) : numv(px->v[x]);
    };

    if (name == "+")  return slotN(0) + slotN(1);
    if (name == "-")  return slotN(0) - slotN(1);
    if (name == "*")  return slotN(0) * slotN(1);
    if (name == "/")  { double d = slotN(1); return d != 0.0 ? slotN(0)/d : 0.0; }
    if (name == "mod"){ double d = slotN(1); return d != 0.0 ? fmod(slotN(0), d) : 0.0; }
    if (name == "of") {
        double v = slotN(0);
        const string& fn = px->mod[0];
        if (fn == "abs")     return fabs(v);
        if (fn == "floor")   return floor(v);
        if (fn == "ceiling") return ceil(v);
        if (fn == "sqrt")    return sqrt(max(0.0, v));
        if (fn == "sin")     return sin(v * M_PI / 180.0);
        if (fn == "cos")     return cos(v * M_PI / 180.0);
        return v;
    }
    if (name == "length of") return (double)evalStr(px, sb).size();
    if (name == "letter")    {
        string s = evalStr(px, sb); // uses v[1]
        int idx = (int)slotN(0) - 1;
        return (idx >= 0 && idx < (int)s.size()) ? (double)s[idx] : 0.0;
    }


    if (sb.i == 0 && sb.idx < (int)sp.size()) {
        const Character& ch = sp[sb.idx];
        if (name == "x position") return (double)(ch.x - STAGE_X);
        if (name == "y position") return (double)((STAGE_Y + STAGE_H) - (ch.y + ch.height));
        if (name == "direction")  return (double)ch.rotation;
        if (name == "size")       return (double)ch.width;
    }

    if (name == "mouse x") { int mx, my; SDL_GetMouseState(&mx, &my); return (double)(mx - STAGE_X); }
    if (name == "mouse y") { int mx, my; SDL_GetMouseState(&mx, &my); return (double)((STAGE_Y + STAGE_H) - my); }
    if (name == "timer")   return (SDL_GetTicks() - g_timerBase) / 1000.0;
    if (name == "loudness")return 0.0; // stub
    if (name == "answer")  { try { return stod(g_answer); } catch(...) { return 0.0; } }

    if (g_vars.count(name)) return numv(g_vars[name]);

    return numv(px->v[0]);
}

static string evalStr(const block* px, const dxd& sb) {
    if (!px) return "";
    const string& name = px->pn[0];

    auto slotS = [&](int x) -> string {
        if (hasReporter(px, x) && px->vari[x]) return evalStr(px->vari[x], sb);
        return px->v[x];
    };

    if (name == "join")   return slotS(0) + slotS(1);
    if (name == "letter") {
        string s = slotS(1);
        int idx = (int)numv(slotS(0)) - 1;
        if (idx >= 0 && idx < (int)s.size()) return string(1, s[idx]);
        return "";
    }
    if (name == "length of") return to_string((int)slotS(0).size());
    if (name == "answer")    return g_answer;

    return dtos(evalNum(px, sb));
}

static bool evalCond(const block* px, const dxd& sb) {
    if (!px) return false;
    const string& name = px->pn[0];

    auto slotN = [&](int x) -> double {
        return (hasReporter(px, x) && px->vari[x]) ? evalNum(px->vari[x], sb) : numv(px->v[x]);
    };
    auto slotS = [&](int x) -> string {
        if (hasReporter(px, x) && px->vari[x]) return evalStr(px->vari[x], sb);
        return px->v[x];
    };
    auto slotC = [&](int x) -> bool {
        return (hasCondition(px, x) && px->candy[x]) ? evalCond(px->candy[x], sb) : false;
    };

    if (name == "<")   return slotN(0) < slotN(1);
    if (name == ">")   return slotN(0) > slotN(1);
    if (name == "=")   {
        double a, b;
        try { a = stod(slotS(0)); b = stod(slotS(1)); return a == b; } catch(...) {}
        return slotS(0) == slotS(1);
    }
    if (name == "and") return slotC(0) && slotC(1);
    if (name == "or")  return slotC(0) || slotC(1);
    if (name == "not") return !slotC(0);

    if (name == "key" || name == "ket") // "ket" is a typo in bblist
        return isKeyDown(px->mod[0]);
    if (name == "mouse down?")
        return (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(1)) != 0;

    if (name == "touching" || name == "touching color" || name == "color")
        return false;

    return false;
}

static void drawSayBubble(const Character& ch, const string& txt, bool visible) {
    SDL_SetRenderTarget(renderer, run);
    SDL_SetTextureBlendMode(run, SDL_BLENDMODE_BLEND);

    int bx = ch.x - STAGE_X + ch.width;
    int bw = max(40, (int)txt.size() * 7 + 10);
    if (bx + bw > STAGE_W) bx = max(0, STAGE_W - bw);
    int by = max(0, ch.y - STAGE_Y - 26);
    SDL_Rect bubble = {bx, by, bw, 24};

    if (visible) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 230);
        SDL_RenderFillRect(renderer, &bubble);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &bubble);
        textRGBA(renderer, bx + 4, by + 5, txt.c_str(), "tahoma.ttf", 12, 0, 0, 0, 255);
    } else {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderFillRect(renderer, &bubble);
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderTarget(renderer, nullptr);
}

void exebl(int& idx, const block* px, dxd& sb, codtxs& cod, int codIdx) {
    if (!px) { idx++; return; }
    const string& name = px->pn[0];

    auto slotN = [&](int x) -> double {
        return (hasReporter(px, x) && px->vari[x]) ? evalNum(px->vari[x], sb) : numv(px->v[x]);
    };
    auto slotS = [&](int x) -> string {
        if (hasReporter(px, x) && px->vari[x]) return evalStr(px->vari[x], sb);
        return px->v[x];
    };

    if (px->color == color(3)) {
        if (name == "broadcast") {
            doBroadcast(px->mod[0]);
            idx++; return;
        }
        if (name == "broadcast" && px->pn[1] == "and wait") {
            doBroadcast(px->mod[0]);
            idx++; return;
        }
        // Pure triggers — skip when encountered mid-script
        idx++; return;
    }

    if (px->color == color(4)) {

        // wait N seconds
        if (name == "wait") {
            Uint32 now = SDL_GetTicks();
            auto it = g_blkWaitT.find(px);
            if (it == g_blkWaitT.end()) {
                Uint32 ms = (Uint32)(slotN(0) * 1000.0);
                g_blkWaitT[px] = now + (ms == 0 ? 1 : ms);
                return; // armed, not yet done
            }
            if (now >= it->second) { g_blkWaitT.erase(it); idx++; }
            return;
        }

        // wait until <condition>
        if (name == "wait until") {
            bool cond = (hasCondition(px, 0) && px->candy[0])
                        ? evalCond(px->candy[0], sb) : true;
            if (cond) idx++;
            return;
        }

        if (name == "forever") {
            if (px->hofre[0] && !px->hof[0].empty()) {
                int& hi = g_blkHofIdx[px];
                if (hi < 0 || hi >= (int)px->hof[0].size()) hi = 0;
                exebl(hi, px->hof[0][hi], sb, cod, codIdx);
                if (hi >= (int)px->hof[0].size()) hi = 0;
            }
            return;
        }

        if (name == "repeat") {
            bool first = (g_blkRepCnt.find(px) == g_blkRepCnt.end());
            int& rc = g_blkRepCnt[px];
            int& hi = g_blkHofIdx[px];
            if (first) { rc = (int)slotN(0); hi = 0; }
            if (rc <= 0) { g_blkRepCnt.erase(px); g_blkHofIdx.erase(px); idx++; return; }
            if (px->hofre[0] && !px->hof[0].empty()) {
                exebl(hi, px->hof[0][hi], sb, cod, codIdx);
                if (hi >= (int)px->hof[0].size()) { hi = 0; rc--; }
            } else { g_blkRepCnt.erase(px); g_blkHofIdx.erase(px); idx++; }
            return;
        }

        if (name == "repeat until") {
            if (g_blkRepCnt.find(px) == g_blkRepCnt.end()) { g_blkRepCnt[px] = 1; g_blkHofIdx[px] = 0; }
            int& hi = g_blkHofIdx[px];
            bool cond = (hasCondition(px, 0) && px->candy[0])
                        ? evalCond(px->candy[0], sb) : false;
            if (cond) { g_blkRepCnt.erase(px); g_blkHofIdx.erase(px); idx++; return; }
            if (px->hofre[0] && !px->hof[0].empty()) {
                exebl(hi, px->hof[0][hi], sb, cod, codIdx);
                if (hi >= (int)px->hof[0].size()) hi = 0;
            }
            return;
        }

        if (name == "if") {
            bool started = (g_blkRepCnt.find(px) != g_blkRepCnt.end());
            int& phase = g_blkRepCnt[px];
            int& hi    = g_blkHofIdx[px];
            if (!started) { phase = 0; hi = 0; }

            if (phase == 0) {
                bool cond = (hasCondition(px, 0) && px->candy[0])
                            ? evalCond(px->candy[0], sb) : false;
                if (cond && px->hofre[0] && !px->hof[0].empty()) {
                    exebl(hi, px->hof[0][hi], sb, cod, codIdx);
                    if (hi >= (int)px->hof[0].size()) {
                        g_blkRepCnt.erase(px); g_blkHofIdx.erase(px); idx++;
                    }
                } else if (cond) {
                    g_blkRepCnt.erase(px); g_blkHofIdx.erase(px); idx++;
                } else {
                    if (px->hofre[1]) { phase = 1; hi = 0; }
                    else { g_blkRepCnt.erase(px); g_blkHofIdx.erase(px); idx++; }
                }
                return;
            }
            if (phase == 1) {
                if (px->hofre[1] && !px->hof[1].empty()) {
                    exebl(hi, px->hof[1][hi], sb, cod, codIdx);
                    if (hi >= (int)px->hof[1].size()) {
                        g_blkRepCnt.erase(px); g_blkHofIdx.erase(px); idx++;
                    }
                } else { g_blkRepCnt.erase(px); g_blkHofIdx.erase(px); idx++; }
                return;
            }
            g_blkRepCnt.erase(px); g_blkHofIdx.erase(px); idx++;
            return;
        }

        if (name == "stop all") { stopAll(); return; }
        if (name == "stop") {
            const string& opt = px->mod[0];
            if (opt == "all") { stopAll(); }
            else if (opt == "this script") {
                cod.exeb[codIdx] = false;
                cod.exen[codIdx] = 0;
            }
            return;
        }

        idx++; return; // unknown control block
    }

    if (px->color == color(0)) {
        if (sb.i == 0 && sb.idx < (int)sp.size()) {
            Character& ch = sp[sb.idx];
            if (name == "move") {
                double steps = slotN(0);
                double rad = (ch.rotation - 90.0) * M_PI / 180.0;
                ch.x += (int)(steps * cos(rad));
                ch.y += (int)(steps * sin(rad));
            }
            else if (name == "turn   ") {
                bool ccw = (!px->order.empty() && px->order[0] == -1);
                ch.rotation += (float)(ccw ? -slotN(0) : slotN(0));
            }
            else if (name == "go to") {
                if (px->mod[0] == "random position") {
                    ch.x = STAGE_X + rand() % max(1, STAGE_W - ch.width);
                    ch.y = STAGE_Y + rand() % max(1, STAGE_H - ch.height);
                } else if (px->mod[0] == "mouse pointer") {
                    int mx, my; SDL_GetMouseState(&mx, &my);
                    ch.x = mx - ch.width / 2;
                    ch.y = my - ch.height / 2;
                }
            }
            else if (name == "go to x:") {
                ch.x = STAGE_X + (int)slotN(0);
                ch.y = STAGE_Y + (int)slotN(1);
            }
            else if (name == "set x to")     ch.x = STAGE_X + (int)slotN(0);
            else if (name == "set y to")     ch.y = STAGE_Y + STAGE_H - (int)slotN(0) - ch.height;
            else if (name == "change x by")  ch.x += (int)slotN(0);
            else if (name == "change y by")  ch.y -= (int)slotN(0); // screen y is inverted
            else if (name == "point in direction") ch.rotation = (float)slotN(0);
            else if (name == "if on edge, bounce") {
                if (ch.x < STAGE_X)                        { ch.x = STAGE_X; ch.rotation = -ch.rotation; }
                if (ch.x + ch.width > STAGE_X + STAGE_W)  { ch.x = STAGE_X + STAGE_W - ch.width; ch.rotation = -ch.rotation; }
                if (ch.y < STAGE_Y)                        { ch.y = STAGE_Y; ch.rotation = 180.0f - ch.rotation; }
                if (ch.y + ch.height > STAGE_Y + STAGE_H) { ch.y = STAGE_Y + STAGE_H - ch.height; ch.rotation = 180.0f - ch.rotation; }
            }
        }
        idx++; return;
    }

    if (px->color == color(1)) {
        if (sb.i == 0 && sb.idx < (int)sp.size()) {
            Character& ch = sp[sb.idx];

            if (name == "show")       ch.visible = true;
            else if (name == "hide")  ch.visible = false;
            else if (name == "set size to") {
                double pct = slotN(0) / 100.0;
                ch.width  = max(1, (int)(100.0 * pct));
                ch.height = max(1, (int)(100.0 * pct));
            }
            else if (name == "change size by") {
                double d = slotN(0);
                ch.width  = max(1, ch.width  + (int)d);
                ch.height = max(1, ch.height + (int)d);
            }
            else if (name == "say" || name == "think") {
                bool timed = (px->pn[1] == "for" && px->pn[2] == "seconds");
                string txt = slotS(0);

                if (timed) {
                    auto it = g_blkWaitT.find(px);
                    if (it == g_blkWaitT.end()) {
                        Uint32 ms = (Uint32)(slotN(1) * 1000.0);
                        g_blkWaitT[px] = SDL_GetTicks() + (ms == 0 ? 1 : ms);
                        drawSayBubble(ch, txt, true);
                        return; // don't advance — wait for timer
                    }
                    if (SDL_GetTicks() >= it->second) {
                        drawSayBubble(ch, txt, false);
                        g_blkWaitT.erase(it);
                        idx++;
                    }
                    return;
                }
                drawSayBubble(ch, txt, true);
            }
        }
        idx++; return;
    }

    if (px->color == color(2)) {
        // "change volume by", "set volume to", etc. — no audio engine yet
        idx++; return;
    }

    if (px->color == color(5)) {
        if (name == "reset timer") {
            g_timerBase = SDL_GetTicks();
        }
        else if (name == "ask" && px->pn[1] == "and wait") {
            // Poll: on first call, arm the ask dialog; on subsequent calls, check done.
            auto it = g_blkWaitT.find(px);
            if (it == g_blkWaitT.end()) {
                g_blkWaitT[px] = 1; // use 1 as "armed" marker (not a real time)
                g_askActive = true;
                g_askDone   = false;
                g_askBuf    = "";
                return; // wait
            }
            if (g_askDone) {
                g_answer = g_askBuf;
                g_askActive = false;
                g_askDone   = false;
                g_blkWaitT.erase(it);
                idx++;
            }
            return;
        }
        idx++; return;
    }


    if (px->color == color(6)) { idx++; return; }

    if (px->color == color(7)) {
        if (name == "set") {
            string varName = px->mod[0];
            g_vars[varName] = dtos(slotN(0));
        }
        idx++; return;
    }

    idx++;
}

bool execute() {
    bool b = false;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < (int)spco[i].size(); j++) {
            if (i == 0 && j >= (int)sp.size()) continue;
            if (i == 1 && j >= (int)co.size()) continue;

            codtxs& cod = spco[i][j];
            ensureVectors(cod);

            for (int k = 0; k < (int)cod.exeb.size(); k++) {
                if (!cod.exeb[k]) continue;
                if (cod.exen[k] >= (int)cod.data[k].size()) {
                    cod.exeb[k] = false;
                    cod.exen[k] = 0;
                    continue;
                }
                b = true;
                dxd sx; sx.i = i; sx.idx = j;
                exebl(cod.exen[k],
                      cod.data[k][cod.exen[k]],
                      sx, cod, k);
                if (cod.exen[k] >= (int)cod.data[k].size()) {
                    cod.exeb[k] = false;
                    cod.exen[k] = 0;
                }
            }
        }
    }

    if (g_charMgr) {
        for (int j = 0; j < (int)sp.size() && j < g_charMgr->count; j++) {
            g_charMgr->characters[j].x        = sp[j].x;
            g_charMgr->characters[j].y        = sp[j].y;
            g_charMgr->characters[j].rotation = sp[j].rotation;
            g_charMgr->characters[j].visible  = sp[j].visible;
            g_charMgr->characters[j].width    = sp[j].width;
            g_charMgr->characters[j].height   = sp[j].height;
        }
    }
    return b;
}

void renderVarMonitors(const codtxs* cod, int charIdx) {
    if (!cod) return;

    int mx = STAGE_X + 4;
    int my = STAGE_Y + STAGE_H - 18;
    const int LINE_H = 18;

    auto draw = [&](const string& varName, const string& val) {
        string label = varName + ": " + val;
        int w = (int)label.size() * 7 + 8;
        SDL_Rect bg = {mx, my, w, 16};
        SDL_SetRenderDrawColor(renderer, 220, 220, 180, 200);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(renderer, &bg);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &bg);
        textRGBA(renderer, mx + 3, my + 2,
                 label.c_str(), "tahoma.ttf", 10, 0, 0, 100, 255);
        my -= LINE_H;
    };

    for (const string& vn : cod->bol[7]) {
        string val = g_vars.count(vn) ? g_vars[vn] : "0";
        draw(vn, val);
    }

    for (const string& vn : cod->bol[5]) {
        string val = "0";
        if (vn == "timer")    val = dtos((SDL_GetTicks() - g_timerBase) / 1000.0);
        else if (vn == "answer")  val = g_answer;
        else if (vn == "loudness") val = "0";
        else if (vn == "username") val = "player";
        draw(vn, val);
    }

    if (charIdx >= 0 && charIdx < (int)sp.size()) {
        const Character& ch = sp[charIdx];
        for (const string& vn : cod->bol[0]) {
            string val = "0";
            if (vn == "x position") val = dtos(ch.x - STAGE_X);
            else if (vn == "y position") val = dtos((STAGE_Y + STAGE_H) - (ch.y + ch.height));
            else if (vn == "direction")  val = dtos(ch.rotation);
            draw(vn, val);
        }
    }

    if (charIdx >= 0 && charIdx < (int)sp.size()) {
        const Character& ch = sp[charIdx];
        for (const string& vn : cod->bol[1]) {
            string val = "0";
            if (vn == "size") val = dtos(ch.width);
            draw(vn, val);
        }
    }

    for (const string& vn : cod->bol[2]) {
        draw(vn, "100"); // volume stub
    }
}

bool renderAskPrompt(TTF_Font* font, int winW, int winH, bool mouseClicked) {
    if (!g_askActive) return false;

    const int barH = 36, barY = winH - barH - 2;
    SDL_Rect bar = {STAGE_X, barY, STAGE_W, barH};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &bar);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &bar);

    string display = g_askBuf + "|";
    if (font) {
        SDL_Color black = {0, 0, 0, 255};
        SDL_Surface* s = TTF_RenderText_Blended(font, display.c_str(), black);
        if (s) {
            SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
            SDL_Rect tr = {bar.x + 6, bar.y + (bar.h - s->h)/2, s->w, s->h};
            SDL_RenderCopy(renderer, t, nullptr, &tr);
            SDL_DestroyTexture(t); SDL_FreeSurface(s);
        }
    }
    return true;
}

bool cligri(int x, int y) {
    if (x > wt-535 && x < wt-505 && y > 45 && y < 75) {
        sp.clear();
        if (g_charMgr)
            for (int c = 0; c < g_charMgr->count; c++)
                sp.push_back(g_charMgr->characters[c]);
        co.clear();
        if (g_bgState) co.push_back(*g_bgState);

        SDL_SetRenderTarget(renderer, run);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        SDL_RenderClear(renderer);
        SDL_SetRenderTarget(renderer, nullptr);

        g_blkWaitT.clear();
        g_blkHofIdx.clear();
        g_blkRepCnt.clear();
        memset(g_keyState, 0, sizeof(g_keyState));
        g_timerBase = SDL_GetTicks();
        g_askActive = g_askDone = false;
        g_askBuf = "";

        for (int i = 0; i < 2; i++) {
            for (auto& j : spco[i]) {
                ensureVectors(j);
                for (int k = 0; k < (int)j.exeb.size(); k++) {
                    j.exeb[k] = false;
                    j.exen[k] = 0;
                }
                for (int k = 0; k < (int)j.data.size(); k++) {
                    if (j.data[k].empty()) continue;
                    const string& evtName = j.data[k][0]->pn[0];
                    if (evtName == "when run clicked" || evtName == "when clicked") {
                        j.exeb[k] = true;
                        j.exen[k] = 1;
                    }
                }
            }
        }
        return true;
    }
    return false;
}

bool clirec(int x, int y) {
    if (x > wt-570 && x < wt-540 && y > 45 && y < 75) {
        stopAll();
        SDL_SetRenderTarget(renderer, run);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        SDL_RenderClear(renderer);
        SDL_SetRenderTarget(renderer, nullptr);
        memset(g_keyState, 0, sizeof(g_keyState));
        g_askActive = g_askDone = false;
        return true;
    }
    return false;
}

void rust() {
    run = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                            SDL_TEXTUREACCESS_TARGET, 500, 400);
    SDL_SetTextureBlendMode(run, SDL_BLENDMODE_BLEND);
    tex.push_back(run);
    SDL_SetRenderTarget(renderer, run);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, nullptr);
}

#endif
