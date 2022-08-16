
#include <SFML/Graphics.hpp>
#include <qpl/qpl.hpp>


struct opengl_state : qsf::base_state {

	void init() override {
		this->get_texture("background").generateMipmap();
		this->clear_color = qpl::rgb(30, 30, 40);

		this->set_active(true);

		auto x = 1.0f;

		this->cube.vertices = {
			{ qpl::vec(-x, -x, -x), qpl::vec(0, 0), qpl::rgb::white },
			{ qpl::vec( x, -x, -x), qpl::vec(1, 0), qpl::rgb::white },
			{ qpl::vec( x,  x, -x), qpl::vec(1, 1), qpl::rgb::white },
			{ qpl::vec(-x,  x, -x), qpl::vec(0, 1), qpl::rgb::white },
			{ qpl::vec( x,  x, -x), qpl::vec(1, 1), qpl::rgb::white },
			{ qpl::vec( x,  x,  x), qpl::vec(0, 1), qpl::rgb::white },
			{ qpl::vec( x, -x,  x), qpl::vec(0, 0), qpl::rgb::white },
			{ qpl::vec( x, -x, -x), qpl::vec(1, 0), qpl::rgb::white },
			{ qpl::vec(-x, -x,  x), qpl::vec(0, 0), qpl::rgb::white },
			{ qpl::vec( x, -x,  x), qpl::vec(1, 0), qpl::rgb::white },
			{ qpl::vec( x,  x,  x), qpl::vec(1, 1), qpl::rgb::white },
			{ qpl::vec(-x,  x,  x), qpl::vec(0, 1), qpl::rgb::white },
			{ qpl::vec(-x, -x,  x), qpl::vec(0, 0), qpl::rgb::white },
			{ qpl::vec(-x, -x, -x), qpl::vec(1, 0), qpl::rgb::white },
			{ qpl::vec(-x,  x, -x), qpl::vec(1, 1), qpl::rgb::white },
			{ qpl::vec(-x,  x,  x), qpl::vec(0, 1), qpl::rgb::white },
			{ qpl::vec( x, -x,  x), qpl::vec(0, 1), qpl::rgb::white },
			{ qpl::vec(-x, -x,  x), qpl::vec(1, 1), qpl::rgb::white },
			{ qpl::vec(-x, -x, -x), qpl::vec(1, 0), qpl::rgb::white },
			{ qpl::vec( x, -x, -x), qpl::vec(0, 0), qpl::rgb::white },
			{ qpl::vec(-x,  x, -x), qpl::vec(0, 1), qpl::rgb::white },
			{ qpl::vec( x,  x, -x), qpl::vec(1, 1), qpl::rgb::white },
			{ qpl::vec( x,  x,  x), qpl::vec(1, 0), qpl::rgb::white },
			{ qpl::vec(-x,  x,  x), qpl::vec(0, 0), qpl::rgb::white },
		};

		this->cube.indices = {
			2, 1, 0, 3, 2, 0,
			4, 5, 6, 4, 6, 7,
			8, 9, 10, 8, 10, 11,
			14, 13, 12, 15, 14, 12,
			16, 17, 18, 16, 18, 19,
			22, 21, 20, 23, 22, 20
		};
		this->cube2 = this->cube;
		this->cube3 = this->cube;

		this->cube.generate();
		
		this->cube2.move({ 3, 3, 0 });
		this->cube2.generate();
		
		this->cube3.move({ 3, 0, 0 });
		this->cube3.generate();
		
		this->cube.set_texture(this->get_texture("texture"));
		this->cube2.set_texture(this->get_texture("texture"));

		this->set_active(false);

		this->color_gens.resize(this->cube.size());

		this->camera.set_deacceleration(10.0f);
		this->camera.set_acceleration(15.0f);
		this->camera.set_max_velocity(3.0f);
		this->camera.set_position({ 3, 3, 3 });
		this->camera.set_view_rotation({ 2.5, 0.5 });
		

		this->hide_cursor();

		this->background.set_texture(this->get_texture("background"));
		this->logo.set_texture(this->get_texture("logo"));


	}

	void cursor_on() {
		this->show_cursor();
		this->camera.allow_looking = false;
		this->lock_cursor = false;
	}
	void cursor_off() {
		this->hide_cursor();
		this->camera.allow_looking = true;
		this->lock_cursor = true;

		this->set_cursor_position(this->center());
	}

	void updating() override {

		if (this->has_gained_focus()) {
			this->cursor_off();
		}
		if (this->has_lost_focus()) {
			this->cursor_on();
		}

		if (this->lock_cursor) {
			if (this->frame_ctr == 0u) {
				this->cursor_off();
			}
			this->set_cursor_position(this->center());
		}
		this->update(this->camera);

		for (auto& i : this->color_gens) {
			i.update(this->frame_time_f());
		}
		for (qpl::size i = 0u; i < this->cube.size(); ++i) {
			auto value = this->color_gens[i].get();
			this->cube[i].color = value;
			this->cube2[i].color = value + 0.2;
			this->cube3[i].color = value * 2;
		}
		this->cube.update();
		this->cube2.update();
		this->cube3.update();



		this->fps.measure();
		if (this->event().key_holding(sf::Keyboard::F)) {
			qpl::println("FPS = ", this->fps.get_fps_u32(), " ", this->frame_ctr);
		}
	}

	void drawing() override {
		//this->draw(background);
		//this->draw(logo);

		this->draw(this->cube, this->camera);
		this->draw(this->cube2, this->camera);
		this->draw(this->cube3, this->camera);

	}

	qgl::vao<qgl::pos3, qgl::tex, qgl::rgb> cube;
	qgl::vao<qgl::pos3, qgl::tex, qgl::rgb> cube2;
	qgl::vao<qgl::pos3, qgl::rgb> cube3;
	std::vector<qpl::cubic_generator_vector3f> color_gens;
	qsf::sprite background;
	qsf::sprite logo;
	qpl::fps_counter fps;
	qpl::camera camera;
	bool lock_cursor = true;
};

int main() try {
	qsf::framework framework;
	framework.enable_gl();
	framework.set_dimension({ 1440, 900 });
	framework.add_texture("texture", "resources/texture.png");
	framework.add_texture("background", "resources/background.jpg");
	framework.add_texture("logo", "resources/logo.png");
	framework.add_state<opengl_state>();
	framework.set_framerate_limit(160);
	framework.game_loop();
}
catch (std::exception& any) {
	qpl::println("caught exception:\n");
	qpl::println(any.what());
}