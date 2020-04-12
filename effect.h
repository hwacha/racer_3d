
#include <vector>
#include "shader.h"
#include "skybox.h"

struct Effect {
	virtual bool expired() { return false; }
    virtual void Draw() { }
	virtual void Update(int dt) { }
};

struct CrashEffect : Effect {
	int ticks;
	void * player;
	glm::vec3 pos;
	int tick_end = 60 * 3;
	ArrayObject quad;
	
    CrashEffect(void * p, glm::vec3 pos) : ticks(0), player(p), pos(pos) {
	  ArrayObject quad = create_fullscreen_quad();
    }	
	bool expired() { return ticks > tick_end; }
	
	void Update(int dt) { this->ticks += dt; }
	
	void Draw() {
	  glBindVertexArray(quad.vao);
	  glDisable(GL_DEPTH_TEST);
	  glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)0);
	  glEnable(GL_DEPTH_TEST);
	}
};

struct EffectSystem {
	Shader shader { "shaders/crash.vs", "shaders/crash.fs" };
	std::vector<Effect *> all_effects{};
	glm::mat4 view;
	glm::mat4 proj;
	
	void create_collision(void * player, glm::vec3 pos) {
		all_effects.push_back(new CrashEffect{player, pos});
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

	void Draw() {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		shader.use();
		shader.setMat4("view", view);
		shader.setMat4("projection", proj);
		
		for(auto effect: all_effects) {
			auto crash = dynamic_cast<CrashEffect*>(effect);
			if (crash) {
				glm::mat4 model {1};
				model = glm::translate(model, {crash->pos});
				shader.setMat4("model", model);
				shader.setFloat("effect_t", crash->ticks);
				crash->Draw();
			}
		}
	}
};
