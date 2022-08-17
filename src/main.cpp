
#include <SFML/Graphics.hpp>
#include <qpl/qpl.hpp>


struct opengl_state : qsf::base_state {

	void init() override {
		this->clear_color = qpl::rgb(30, 30, 40);

		this->set_active(true);

		auto x = 1.0f; 

		this->cube = qgl::get_cube();


		this->cube.generate();

		this->cube2 = this->cube;
		this->cube2.move({ 1.1f, 1.1f, 0 });
		this->cube2.generate();

		this->cube3 = this->cube;
		this->cube3.move({ 1.1f, 0, 0 });
		this->cube3.generate();

		this->cube4 = this->cube;
		this->cube4.move({ 0, 1.1f, 0 });
		this->cube4.generate();

		this->cube.set_texture(this->get_texture("texture"));
		this->cube2.set_texture(this->get_texture("texture"));

		this->set_active(false);

		this->color_gens.resize(4);
		for (auto& i : this->color_gens) {
			i.resize(this->cube.size());
		}

		this->hide_cursor();
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

	void update_cursor() {
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
	}

	void updating() override {
		this->update_cursor();
		this->update(this->camera);

		for (auto& i : this->color_gens) {
			for (auto& j : i) {
				j.update(this->frame_time_f());
			}
		}
		for (qpl::size i = 0u; i < this->cube.size(); ++i) {
			this->cube[i].color = this->color_gens[0][i].get();
			this->cube2[i].color = this->color_gens[1][i].get() + 0.2;
			this->cube3[i].color = this->color_gens[2][i].get() * 2;
			this->cube4[i].color = this->color_gens[3][i].get() * 2;
		}
		this->cube.update();
		this->cube2.update();
		this->cube3.update();
		this->cube4.update();



		this->fps.measure();
		if (this->event().key_holding(sf::Keyboard::F)) {
			qpl::println("FPS = ", this->fps.get_fps_u32(), " ", this->frame_ctr);
		}
	}

	void drawing() override {
		this->draw(this->cube, this->camera);
		this->draw(this->cube2, this->camera);

		this->draw(this->cube3, this->camera);
		this->draw(this->cube4, this->camera);

	}

	qgl::vertex_array_type<qgl::pos3, qgl::rgb, qgl::tex> cube;
	qgl::vertex_array_type<qgl::pos3, qgl::rgb, qgl::tex> cube2;
	qgl::vertex_array_type<qgl::pos3, qgl::rgb> cube3;
	qgl::vertex_array_type<qgl::pos3, qgl::rgb> cube4;
	std::vector<std::vector<qpl::cubic_generator_vector3f>> color_gens;
	qpl::fps_counter fps;
	qpl::camera camera;
	bool lock_cursor = true;
};

struct shapes_state : qsf::base_state {

	void init() override {
		this->clear_color = qpl::rgb(30, 30, 40);

		this->set_active(true);

		this->shapes.resize(100);
		for (qpl::size i = 0u; i < this->shapes.size(); ++i) {
			this->shapes[i].vertex_array.set_texture(this->get_texture("texture"));
			this->shapes[i].set_position(qpl::vec(i / 10, i % 10, 0) * 1.1);
			this->shapes[i].set_color(qpl::get_random_color());
		}


		this->set_active(false);
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

	void update_cursor() {
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
	}

	void updating() override {
		this->update_cursor();
		this->update(this->camera);
	}

	void drawing() override {
		this->draw(this->shapes, this->camera);

	}

	std::vector<qgl::cuboid> shapes;
	qpl::camera camera;
	bool lock_cursor = true;
};

int main() try {
	qsf::framework framework;
	framework.enable_gl();
	framework.set_dimension({ 1440, 900 });
	framework.add_texture("texture", "resources/texture.png");
	framework.add_state<shapes_state>();
	//framework.add_state<opengl_state>();
	framework.set_framerate_limit(160);
	framework.game_loop();
}
catch (std::exception& any) {
	qpl::println("caught exception:\n");
	qpl::println(any.what());
}