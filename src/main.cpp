#include <qpl/qpl.hpp>


struct opengl_state : qsf::base_state {

	void add_cube() {

		auto cube = qgl::get_cube();

		auto index = this->cubes.size() / cube.indices.size();

		this->color_gens.resize(this->color_gens.size() + cube.indices.size());


		auto x = index % 10u;
		auto y = index / 10u;

		decltype(this->cubes) add = cube;
		add.move(qpl::vec(x, y, 0) * 1.1);

		for (auto& i : add.vertices) {
			qpl::println(i);
		}

		this->cubes.add(add);
	}

	void init() override {
		this->clear_color = qpl::rgb(30, 30, 40);

		this->set_active(true);

		auto cube_count = 1u;

		for (qpl::u32 i = 0u; i < cube_count; ++i) {
			this->add_cube();
		}

		this->cubes.generate();

		//qpl::println(this->cubes.get_fragment_shader());
		//qpl::println(this->cubes.get_vertex_shader());


		this->set_active(false);

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

		if (this->event().key_holding(sf::Keyboard::E)) {
			this->add_cube();
			this->cubes.generate();
		}

		for (auto& i : this->color_gens) {
			i.update(this->event().frame_time_f());
		}
		for (qpl::size i = 0u; i < this->cubes.size(); ++i) {
			this->cubes[i].color = this->color_gens[i].get() * 2;
		}
		this->cubes.update();
	}

	void drawing() override {
		this->draw(this->cubes, this->camera);
	}

	qgl::vertex_array<qgl::flag_default, qgl::pos3, qgl::rgb> cubes;
	std::vector<qpl::cubic_generator_vector3f> color_gens;
	qpl::camera camera;
	bool lock_cursor = true;
};

struct sphere_state : qsf::base_state {

	void add_sphere(qpl::size divisions) {

		auto ico_sphere = qgl::make_ico_sphere(divisions);

		auto index = this->sphere.size() / ico_sphere.indices.size();

		this->color_gens.resize(this->color_gens.size() + (ico_sphere.indices.size()) / 3);
		auto x = index % 10u;
		auto y = index / 10u;

		decltype(this->sphere) add = ico_sphere;

		add.move(qpl::vec(x, y, 0) * 2.2);

		this->sphere.add(add);
	}

	void init() override {
		this->clear_color = qpl::rgb(30, 30, 40);

		this->set_active(true);

		auto count = 1u;

		for (qpl::u32 i = 0u; i < count; ++i) {
			++this->sphere_count;
			this->add_sphere(this->divisions);
		}

		for (auto& i : this->sphere.vertices) {

		}
		this->sphere.generate();

		this->set_active(false);

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

		if (this->event().key_single_pressed(sf::Keyboard::E)) {
			++this->divisions;
			this->sphere.clear();
			for (qpl::size i = 0u; i < this->sphere_count; ++i) {
				this->add_sphere(this->divisions);
			}
			this->sphere.update();

			qpl::println(this->sphere.vertices.size());
		}
		if (this->event().key_single_pressed(sf::Keyboard::Q)) {
			if (this->divisions) {
				--this->divisions;
				this->sphere.clear();
				for (qpl::size i = 0u; i < this->sphere_count; ++i) {
					this->add_sphere(this->divisions);
				}
				this->sphere.update();

				qpl::println(this->sphere.vertices.size());
			}
		}
		if (this->event().key_single_pressed(sf::Keyboard::R)) {

			++this->sphere_count;
			this->add_sphere(this->divisions);
			this->sphere.update();

			qpl::println(this->sphere.vertices.size());
		}
		if (this->event().key_single_pressed(sf::Keyboard::T)) {

			--this->sphere_count;
			this->sphere.clear();
			for (qpl::size i = 0u; i < this->sphere_count; ++i) {
				this->add_sphere(this->divisions);
			}
			this->sphere.update();

			qpl::println(this->sphere.vertices.size());
		}

		for (auto& i : this->color_gens) {
			i.update(this->event().frame_time_f());
		}

		this->fps.measure();
		if (this->event().key_holding(sf::Keyboard::F)) {
			qpl::println(this->fps.get_fps_u32(), " FPS");
		}
		for (qpl::size i = 0u; i < this->sphere.size(); i += 3u) {
			auto color = qpl::frgb(this->color_gens[i / 3].get()).intensified(0.5);
			this->sphere[i + 0].color = color;
			this->sphere[i + 1].color = color;
			this->sphere[i + 2].color = color;
		}

		this->sphere.update();
	}

	void drawing() override {
		this->draw(this->sphere, this->camera);
	}

	qgl::vertex_array<qgl::flag_default, qgl::pos3, qgl::rgb> sphere;
	std::vector<qpl::cubic_generator_vector3f> color_gens;
	qpl::camera camera;
	qpl::size divisions = 0u;
	qpl::size sphere_count = 0u;
	bool lock_cursor = true;

	qpl::fps_counter fps;
};

struct perlin_state : qsf::base_state {
	void init() override {
		this->clear_color = qpl::rgb(30, 30, 40);

		this->set_active(true);

		qpl::perlin_noise y_noise;
		y_noise.set_seed_random();

		qpl::perlin_noise c_noise;
		c_noise.set_seed_random();

		auto n = 100;
		auto d = 10;
		for (qpl::i32 z = -n; z < n; ++z) {
			for (qpl::i32 x = -n; x < n; ++x) {

				auto pos = qpl::vec(x, 0, z);
				auto add = [&](qpl::vec2 quad) {

					auto y = y_noise.get(x + quad.x, z + quad.y, 0.01, 15) * d * 5;
					auto c = std::fmod((c_noise.get(x + quad.x, z + quad.y, 0.1, 2) - 0.5) * 3, 1.0);

					qpl::vec3 position = pos + qpl::vec(quad.x, y, quad.y);
					qpl::rgb color = qpl::get_rainbow_color(c);
					return qgl::make_vertex(position / d, color);
				};

				this->va.add(add({ 0, 0 }));
				this->va.add(add({ 1, 0 }));
				this->va.add(add({ 1, 1 }));
				this->va.add(add({ 0, 1 }));
			}
		}


		this->va.generate();
		this->va.primitive_type = qgl::primitive_type::quads;
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

		this->fps.measure();
		if (this->event().key_holding(sf::Keyboard::F)) {
			qpl::println("FPS = ", this->fps.get_fps_u32(), " ", this->frame_ctr);
		}
	}

	void drawing() override {
		this->draw(this->va, this->camera);
	}

	qgl::vertex_array<qgl::flag_primitive_type, qgl::pos3, qgl::rgb> va;

	qpl::fps_counter fps;
	qpl::camera camera;
	bool lock_cursor = true;
};


int main() try {

	//qpl::rgb_type<qpl::u8, 3> r = decltype(r)::red();
	//qpl::println(r);
	//r.brighten(0.7);
	//qpl::println(r);

	//constexpr auto n = qpl::rgb_type<qpl::u8, 4>();
	//qpl::println(n);
	
	constexpr auto m = qpl::rgb_type<qpl::u8, 2>(1, 5, 1);
	//constexpr qpl::vector2i m;

	qsf::framework framework;
	framework.enable_gl();
	framework.set_title("QPL");
	framework.set_dimension({ 1400u, 950u });

	framework.add_state<sphere_state>();
	//framework.add_state<opengl_state>();
	framework.game_loop();
}
catch (std::exception& any) {
	qpl::println("caught exception:\n", any.what());
	qpl::system_pause();
}