#include <qpl/qpl.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

struct opengl_state : qsf::base_state {

	void add_cube() {

		auto cube = qgl::get_cube();

		auto index = this->cubes.size() / cube.indices.size();

		this->color_gens.resize(this->color_gens.size() + cube.indices.size());


		//auto x = index % 10u;
		//auto y = index / 10u;

		auto x = qpl::random(-3, 3);
		auto y = qpl::random(-3, 3);
		auto z = qpl::random(-3, 3);

		decltype(this->cubes) add = cube;
		add.move(qpl::vec(x, y, z) * 1.1);

		for (auto& i : add.vertices) {
			qpl::println(i);
		}

		this->cubes.add(add);
		this->cubes.update();
	}

	void init() override {
		this->clear_color = qpl::rgb(30, 30, 40);

		this->set_active(true);

		auto cube_count = 10u;

		for (qpl::u32 i = 0u; i < cube_count; ++i) {
			this->add_cube();
		}

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

	qgl::vertex_array<qgl::flag_default, qgl::pos3, qgl::frgb> cubes;
	std::vector<qpl::cubic_generator_vector3f> color_gens;
	qpl::camera camera;
	bool lock_cursor = true;
};

struct sphere_state : qsf::base_state {

	void add_sphere(qpl::size divisions) {

		auto ico_sphere = qgl::make_ico_sphere(divisions);

		auto index = this->sphere.size() / ico_sphere.indices.size();

		//this->color_gens.resize(this->color_gens.size() + (ico_sphere.indices.size()) / 3);
		auto x = index % 10u;
		auto y = index / 10u;

		decltype(this->sphere) add = ico_sphere;

		add.move(qpl::vec(x, y, 0) * 2.2);

		this->sphere.add(add);
		for (auto& i : this->sphere) {
			i.color = qpl::rgb::black();
		}

		this->sphere_mesh.vertices.clear();
		for (qpl::size i = 0u; i < this->sphere.vertices.size() - 1; ++i) {

			auto a = this->sphere[i].position;
			auto b = this->sphere[i + 1].position;

			auto diff = a - b;
			auto length = diff.length();

			auto normal = (a - b) / length;
			auto normalx = qpl::vec(b.x - a.x, a.y - b.y, a.z - b.z) / length;
			auto normaly = qpl::vec(a.x - b.x, b.y - a.y, a.z - b.z) / length;
			auto normalz = qpl::vec(a.x - b.x, a.y - b.y, b.z - a.z) / length;

			auto thickness = 1.0 / 5;
			auto offx = -normalx * thickness / 2;
			auto offy = -normaly * thickness / 2;
			auto offz = -normalz * thickness / 2;

			

			auto add = [&](qpl::vec3 v1, qpl::vec3 v2, qpl::vec3 normal, qpl::vec3 off, qpl::frgb color) {
				this->sphere_mesh.add(qgl::make_vertex(v1 + normal * thickness + off, color));
				this->sphere_mesh.add(qgl::make_vertex(v1 - normal * thickness + off, color));
				this->sphere_mesh.add(qgl::make_vertex(v2 - normal * thickness + off, color));
				this->sphere_mesh.add(qgl::make_vertex(v2 + normal * thickness + off, color));

				this->sphere_mesh.add(qgl::make_vertex(v2 + normal * thickness + off, color));
				this->sphere_mesh.add(qgl::make_vertex(v2 - normal * thickness + off, color));
				this->sphere_mesh.add(qgl::make_vertex(v1 - normal * thickness + off, color));
				this->sphere_mesh.add(qgl::make_vertex(v1 + normal * thickness + off, color));
			};


			//add(b, a, normalx, offx, qpl::frgb::red());
			//add(a, b, normalx, -offx, qpl::frgb::red());
			add(b, a, normaly, offz, qpl::frgb::red());
			add(a, b, normaly, -offz, qpl::frgb::white());
			break;

			//this->sphere_mesh.add(qgl::make_vertex(b + normalx * thickness + offx, qpl::frgb::red()));
			//this->sphere_mesh.add(qgl::make_vertex(b - normalx * thickness + offx, qpl::frgb::red()));
			//this->sphere_mesh.add(qgl::make_vertex(a - normalx * thickness + offx, qpl::frgb::red()));
			//this->sphere_mesh.add(qgl::make_vertex(a + normalx * thickness + offx, qpl::frgb::red()));
			//
			//this->sphere_mesh.add(qgl::make_vertex(a + normalx * thickness - offx, qpl::frgb::red()));
			//this->sphere_mesh.add(qgl::make_vertex(a - normalx * thickness - offx, qpl::frgb::red()));
			//this->sphere_mesh.add(qgl::make_vertex(b - normalx * thickness - offx, qpl::frgb::red()));
			//this->sphere_mesh.add(qgl::make_vertex(b + normalx * thickness - offx, qpl::frgb::red()));
			//
			//this->sphere_mesh.add(qgl::make_vertex(b + normaly * thickness + offy, qpl::frgb::white()));
			//this->sphere_mesh.add(qgl::make_vertex(b - normaly * thickness + offy, qpl::frgb::white()));
			//this->sphere_mesh.add(qgl::make_vertex(a - normaly * thickness + offy, qpl::frgb::white()));
			//this->sphere_mesh.add(qgl::make_vertex(a + normaly * thickness + offy, qpl::frgb::white()));
			//
			//this->sphere_mesh.add(qgl::make_vertex(a + normaly * thickness - offy, qpl::frgb::white()));
			//this->sphere_mesh.add(qgl::make_vertex(a - normaly * thickness - offy, qpl::frgb::white()));
			//this->sphere_mesh.add(qgl::make_vertex(b - normaly * thickness - offy, qpl::frgb::white()));
			//this->sphere_mesh.add(qgl::make_vertex(b + normaly * thickness - offy, qpl::frgb::white()));
			//break;

			//this->sphere_mesh.add(qgl::make_vertex(a + normaly * thickness, qpl::frgb::blue()));
			//this->sphere_mesh.add(qgl::make_vertex(a - normaly * thickness, qpl::frgb::blue()));
			//this->sphere_mesh.add(qgl::make_vertex(b - normaly * thickness, qpl::frgb::blue()));
			//this->sphere_mesh.add(qgl::make_vertex(b + normaly * thickness, qpl::frgb::blue()));

		}
		this->sphere_mesh.update();

	}

	void init() override {
		this->clear_color = qpl::rgb(30, 30, 40);

		this->set_active(true);

		auto count = 1u;

		for (qpl::u32 i = 0u; i < count; ++i) {
			++this->sphere_count;
			this->add_sphere(this->divisions);
		}

		this->set_active(false);
		this->sphere_mesh.set_primitive_type(qgl::primitive_type::quads);

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

		if (this->event().key_holding(sf::Keyboard::C)) {
			this->camera.set_speed(0.1f);
		}
		else {
			this->camera.set_speed(1.0f);
		}
		this->update(this->camera);

		//for (auto& i : this->color_gens) {
		//	i.update(this->event().frame_time_f());
		//}
		//
		//this->fps.measure();
		//if (this->event().key_holding(sf::Keyboard::F)) {
		//	qpl::println(this->fps.get_fps_u32(), " FPS");
		//}
		//for (qpl::size i = 0u; i < this->sphere_mesh.size(); i += 6u) {
		//	auto color = qpl::frgb(this->color_gens[i / 6].get()).intensified(0.5);
		//
		//	for (qpl::size j = 0u; j < 6u; ++j) {
		//		this->sphere_mesh[i + j].color = color;
		//	}
		//}
	}

	void drawing() override {
		//this->draw(this->sphere, this->camera);
		this->draw(this->sphere_mesh, this->camera);
	}

	qgl::vertex_array<qgl::flag_default, qgl::pos3, qgl::frgb> sphere;
	qgl::vertex_array<qgl::flag_default | qgl::flag_bit_primitive_type | qgl::flag_bit_autoupdate, qgl::pos3, qgl::frgb> sphere_mesh;


	//std::vector<qpl::cubic_generator_vector3f> color_gens;
	qpl::camera camera;
	qpl::size divisions = 0u;
	qpl::size sphere_count = 0u;
	bool lock_cursor = true;

	qpl::fps_counter fps;
};

struct line_state : qsf::base_state {

	void make_va(bool randomize) {
		this->va.clear();
		this->lines.clear();

		if (randomize) {
			this->a = qpl::random(qpl::vec(-1.0f, -1.0f, -1.0f), qpl::vec(1.0f, 1.0f, 1.0f));
			this->b = qpl::random(qpl::vec(-1.0f, -1.0f, -1.0f), qpl::vec(1.0f, 1.0f, 1.0f));
		}

		auto diff = a - b;
		auto length = diff.length();

		auto normal = (a - b) / length;

		

		auto normalx = qpl::vec(normal.y, normal.x, normal.z);
		auto normaly = qpl::vec(normal.x, -normal.y, normal.z);
		auto normalz = qpl::vec(normal.x, normal.y, -normal.z);

		auto thickness = 1.0 / 5;
		auto offx = (-normalx) * thickness / 2;
		auto offy = (-normaly) * thickness / 2;
		auto offz = (-normalz) * thickness / 2;

		auto perpx = qpl::vec3::cross(normal, qpl::vec(1, 0, 0));
		//auto perpy = qpl::vec3::cross(normal, qpl::vec(0, 1, 0));

		qpl::vec3 rotate = glm::rotate(glm::vec3(perpx), (qpl::f32)clock.elapsed_f_reset(), glm::vec3(normal));
		//auto rotate = qpl::vec3(glm::rotate((qpl::f32)clock.elapsed_f(), glm::vec3(normal))) * perpx;

		//auto rotate = glm::rotate(glm::vec3(), 1.0f, glm::vec3());

		//auto perpz = qpl::vec3::cross(normal, qpl::vec(0, 0, 1));

		//perpx = qpl::vec3::rotate(perpx, this->clock.elapsed_f(), qpl::vec3(1, 0, 0));
		//perpy = qpl::vec3::rotate(perpy, this->clock.elapsed_f(), qpl::vec3(1, 0, 0));
		//perpz = qpl::vec3::rotate(perpz, this->clock.elapsed_f(), qpl::vec3(1, 0, 0));

		auto add = [&](qpl::vec3 v1, qpl::vec3 v2, qpl::vec3 normal, qpl::vec3 off, qpl::frgb color) {
			this->va.add(qgl::make_vertex(v1 + normal * thickness + off, color));
			this->va.add(qgl::make_vertex(v1 - normal * thickness + off, color));
			this->va.add(qgl::make_vertex(v2 - normal * thickness + off, color));
			this->va.add(qgl::make_vertex(v2 + normal * thickness + off, color));

			this->va.add(qgl::make_vertex(v2 + normal * thickness + off, color));
			this->va.add(qgl::make_vertex(v2 - normal * thickness + off, color));
			this->va.add(qgl::make_vertex(v1 - normal * thickness + off, color));
			this->va.add(qgl::make_vertex(v1 + normal * thickness + off, color));
		};


		//add(b, a, normalx, offx, qpl::frgb::red());
		//add(a, b, normalx, -offx, qpl::frgb::red());
		add(a, b, normal, 0, qpl::frgb::red());
		//add(b, a, normalx, -offx, qpl::frgb::white());

		this->lines.add(qgl::make_vertex(a, qpl::rgb::green()));
		this->lines.add(qgl::make_vertex(b, qpl::rgb::green()));

		this->lines.add(qgl::make_vertex(a, qpl::rgb::blue()));
		this->lines.add(qgl::make_vertex(a + perpx, qpl::rgb::blue()));

		this->lines.add(qgl::make_vertex(a, qpl::rgb::blue()));
		this->lines.add(qgl::make_vertex(a - perpx, qpl::rgb::blue()));

		this->lines.add(qgl::make_vertex(a, qpl::rgb::blue()));
		this->lines.add(qgl::make_vertex(a + rotate, qpl::rgb::blue()));

		//this->lines.add(qgl::make_vertex(a, qpl::rgb::red()));
		//this->lines.add(qgl::make_vertex(a + perpy, qpl::rgb::red()));
		//this->lines.add(qgl::make_vertex(a, qpl::rgb::yellow()));
		//this->lines.add(qgl::make_vertex(a + perpy * 2, qpl::rgb::yellow()));
		//
		//this->lines.add(qgl::make_vertex(a, qpl::rgb::magenta()));
		//this->lines.add(qgl::make_vertex(a + perpy * 2, qpl::rgb::magenta()));

		this->va.update();
		this->lines.update();

	}

	void init() override {
		this->clear_color = qpl::rgb(30, 30, 40);

		this->set_active(true);

		auto count = 1u;

		this->make_va(true);

		this->set_active(false);
		this->va.set_primitive_type(qgl::primitive_type::quads);
		this->lines.set_primitive_type(qgl::primitive_type::lines);

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

		if (this->event().key_holding(sf::Keyboard::C)) {
			this->camera.set_speed(0.1f);
		}
		else {
			this->camera.set_speed(1.0f);
		}

		this->update(this->camera);

		if (this->event().key_single_pressed(sf::Keyboard::T)) {
			this->make_va(true);
		}
		else {
			this->make_va(false);
		}
	}

	void drawing() override {
		this->draw(this->va, this->camera);
		this->draw(this->lines, this->camera);
	}

	qgl::vertex_array<qgl::flag_default | qgl::flag_bit_primitive_type, qgl::pos3, qgl::frgb> va;
	qgl::vertex_array<qgl::flag_default | qgl::flag_bit_primitive_type, qgl::pos3, qgl::frgb> lines;


	//std::vector<qpl::cubic_generator_vector3f> color_gens;
	qpl::camera camera;
	bool lock_cursor = true;

	qpl::vec3 a;
	qpl::vec3 b;
	qpl::clock clock;

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

	qgl::vertex_array<qgl::flag_bit_primitive_type, qgl::pos3, qgl::frgb> va;

	qpl::fps_counter fps;
	qpl::camera camera;
	bool lock_cursor = true;
};


int main() try {
	qsf::framework framework;
	framework.enable_gl();
	framework.set_title("QPL");
	framework.set_dimension({ 1400u, 950u });

	framework.add_state<line_state>();
	//framework.add_state<opengl_state>();
	framework.game_loop();
}
catch (std::exception& any) {
	qpl::println("caught exception:\n", any.what());
	qpl::system_pause();
}