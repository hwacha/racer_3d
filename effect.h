#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include "shader.h"
#include "skybox.h"

unsigned int createTexture(char* filename);

struct Effect {
    ArrayObject quad;
    int ticks;
    int tick_end = 60;

    virtual ~Effect() {
        glDeleteVertexArrays(1, &quad.vao);
        glDeleteBuffers(1, &quad.array_buf);
        glDeleteBuffers(1, &quad.element_buf);
    }

    virtual bool expired() { return ticks > tick_end; }

    virtual void Update(int dt) { this->ticks += dt; }

    virtual void Draw() {
        glBindVertexArray(quad.vao);
        glDisable(GL_DEPTH_TEST);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)0);
        glEnable(GL_DEPTH_TEST);
    }
};

struct WinEffect : Effect {
    int player_id;
    glm::vec3 pos;
    glm::mat4 transform;

    WinEffect(int p, glm::vec3 pos, glm::mat4 m) :
       player_id(p),
       pos(pos),
       transform(m)
    {
        tick_end = 60 * 10;
        ticks = 0;
        quad = create_fullscreen_quad(0);
    }
};

struct CrashEffect : Effect {
    void * player;
    glm::vec3 pos;
    glm::mat4 transform;

    CrashEffect(void * p, glm::vec3 pos, glm::mat4 m) :
       player(p),
       pos(pos),
       transform(m)
    {
        ticks = 0;
        quad = create_fullscreen_quad(0);
    }
};

struct EffectSystem {
    Shader shader { "shaders/crash.vs", "shaders/crash.fs" };
    std::vector<Effect *> all_effects{};
    glm::mat4 view;
    glm::mat4 proj;

    unsigned int explosionTexture = createTexture("assets/exp2_0.png");
    unsigned int youWinTexture = createTexture("assets/winner.png");

    void create_youwin(
        int player_id,
        glm::vec3 player_pos,
        float pitch_rads,
        float yaw_rads
    ) {
        glm::mat4 m {1};
        m = glm::translate(m, player_pos + glm::vec3 { 0.0f, 0.5f, 0.0f});
        m = glm::scale(m, {1.0f, 0.2f, 1.0f});
        m = glm::rotate(m, (3.1416f) / 2 + yaw_rads, {0, 1, 0});
        all_effects.push_back(new WinEffect{player_id, player_pos, m});
    }
    void create_collision(
        void * player,
        glm::vec3 player_pos,
        float pitch_rads,
        float yaw_rads
    ) {
        glm::mat4 m {1};
        m = glm::translate(m, player_pos);
        m = glm::rotate(m, (3.1416f) / 2 + yaw_rads, {0, 1, 0});
        all_effects.push_back(new CrashEffect{player, player_pos, m});
    }

    void Update(int frames) {
        for(auto effect: all_effects)
            effect->Update(frames);
        for(size_t i = all_effects.size(); i -->0; ) {
            if (all_effects[i]->expired()) {
                delete all_effects[i];
                all_effects[i] = all_effects.back();
                all_effects.pop_back();
            }
        }
    }

    void Draw(int viewing_player) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        shader.use();
        shader.setMat4("view", view);
        shader.setMat4("projection", proj);
        
        for(auto effect: all_effects) {
            auto crash = dynamic_cast<CrashEffect*>(effect);
            if (crash) {
                glBindTexture(GL_TEXTURE_2D, explosionTexture);
                shader.setMat4("model", crash->transform);
                shader.setFloat("effect_time", crash->ticks);
                shader.setInt("effect_type", 1);
                crash->Draw();
            }
            auto win = dynamic_cast<WinEffect*>(effect);
            if (win && win->player_id == viewing_player) {
                glBindTexture(GL_TEXTURE_2D, youWinTexture);
                shader.setMat4("model", win->transform);
                shader.setFloat("effect_time", win->ticks);
                shader.setInt("effect_type", 2);
                win->Draw();
            }
        }
    }
};
