
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/OpenGL.hpp>
#include <qpl/qpl.hpp>


namespace qpl {
    class camera {
    public:
        camera() {
            this->construct();
        }

        QPLDLL qpl::vector3f get_view_rotation_coordinates() const;
        QPLDLL qpl::vector3f get_resulting_looking_position() const;
        QPLDLL glm::mat4 get_resulting_projection() const;

        QPLDLL void set_position(qpl::vector3f pos);
        QPLDLL void set_position(qpl::f32 x, qpl::f32 y, qpl::f32 z);
        QPLDLL void set_position_x(qpl::f32 x);
        QPLDLL void set_position_y(qpl::f32 y);
        QPLDLL void set_position_z(qpl::f32 z);
        QPLDLL qpl::vector3f get_position() const;
        QPLDLL qpl::f32 get_position_x() const;
        QPLDLL qpl::f32 get_position_y() const;
        QPLDLL qpl::f32 get_position_z() const;

        QPLDLL void move(qpl::vector3f delta);
        QPLDLL void move(qpl::f32 x, qpl::f32 y, qpl::f32 z);
        QPLDLL void move_x(qpl::f32 x);
        QPLDLL void move_y(qpl::f32 y);
        QPLDLL void move_z(qpl::f32 z);

        QPLDLL void move_forward(qpl::f32 delta = 1);
        QPLDLL void move_backwards(qpl::f32 delta = 1);
        QPLDLL void move_left(qpl::f32 delta = 1);
        QPLDLL void move_right(qpl::f32 delta = 1);
        QPLDLL void move_up(qpl::f32 delta = 1);
        QPLDLL void move_down(qpl::f32 delta = 1);

        QPLDLL void set_view_rotation(qpl::vector2f pos);
        QPLDLL void set_view_rotation(qpl::f32 x, qpl::f32 y);
        QPLDLL void set_view_rotation_x(qpl::f32 x);
        QPLDLL void set_view_rotation_y(qpl::f32 y);

        QPLDLL qpl::vector2f get_view_rotation() const;
        QPLDLL qpl::f32 get_view_rotation_x() const;
        QPLDLL qpl::f32 get_view_rotation_y() const;

        QPLDLL void rotate_view(qpl::vector2f delta);

        QPLDLL void look_backwards();
        QPLDLL void look_up(qpl::f32 delta = qpl::pi_32 / 2);
        QPLDLL void look_down(qpl::f32 delta = qpl::pi_32 / 2);
        QPLDLL void look_left(qpl::f32 delta = qpl::pi_32 / 2);
        QPLDLL void look_right(qpl::f32 delta = qpl::pi_32 / 2);

        QPLDLL void rotate_view_x(qpl::f32 x);
        QPLDLL void rotate_view_y(qpl::f32 x);


        QPLDLL void set_max_velocity(qpl::f32 max);
        QPLDLL qpl::f32 get_max_velocity() const;

        QPLDLL void set_acceleration(qpl::f32 acceleration);
        QPLDLL qpl::f32 get_acceleration() const;

        QPLDLL void set_deacceleration(qpl::f32 deacceleration);
        QPLDLL qpl::f32 get_deacceleration() const;

        QPLDLL void accelerate_forward(qpl::f32 delta = 1);
        QPLDLL void accelerate_backwards(qpl::f32 delta = 1);
        QPLDLL void accelerate_left(qpl::f32 delta = 1);
        QPLDLL void accelerate_right(qpl::f32 delta = 1);
        QPLDLL void accelerate_up(qpl::f32 delta = 1);
        QPLDLL void accelerate_down(qpl::f32 delta = 1);

        QPLDLL void set_speed(qpl::f32 speed);
        QPLDLL qpl::f32 get_speed() const;

        QPLDLL void update(qpl::f32 delta_time);
        QPLDLL qpl::vector3f get_velocity() const;
    private:
        QPLDLL void construct();
        QPLDLL void ignore_y_axis();

        qpl::vector2f m_view_rotation;
        qpl::vector3f m_position;

        qpl::vector3f m_velocites;
        qpl::f32 m_max_velocity;
        qpl::f32 m_accelerate;
        qpl::f32 m_deaccelerate;
        qpl::f32 m_speed;
    };
}

namespace qpl {
	void qpl::camera::construct() {
		this->m_position = { 0, 0, 0 };
		this->m_view_rotation = { 0, 0 };
		this->m_velocites = { 0, 0, 0 };
		this->m_max_velocity = 2;
		this->m_accelerate = 0.2f;
		this->m_deaccelerate = 5.0f;
		this->m_speed = 1.0;
	}


	qpl::vector3f qpl::camera::get_view_rotation_coordinates() const {
		qpl::vector3f result;
		result.x = std::cos(-this->m_view_rotation.x) * std::cos(-this->m_view_rotation.y);
		result.y = std::sin(-this->m_view_rotation.y);
		result.z = std::sin(-this->m_view_rotation.x) * std::cos(-this->m_view_rotation.y);
		return result;
	}
	qpl::vector3f qpl::camera::get_resulting_looking_position() const {
		return this->m_position + this->get_view_rotation_coordinates();
	}
	glm::mat4 qpl::camera::get_resulting_projection() const {
		qpl::vector3f rot = { 0, 1, 0 };
		return glm::lookAt(this->m_position, this->get_resulting_looking_position(), rot);
	}

	void qpl::camera::set_position(qpl::vector3f pos) {
		this->m_position = pos;
	}
	void qpl::camera::set_position(qpl::f32 x, qpl::f32 y, qpl::f32 z) {
		this->set_position({ x, y, z });
	}
	void qpl::camera::set_position_x(qpl::f32 x) {
		this->m_position.x = x;
	}
	void qpl::camera::set_position_y(qpl::f32 y) {
		this->m_position.y = y;
	}
	void qpl::camera::set_position_z(qpl::f32 z) {
		this->m_position.z = z;
	}
	qpl::vector3f qpl::camera::get_position() const {
		return this->m_position;
	}

	qpl::f32 qpl::camera::get_position_x() const {
		return this->m_position.x;
	}
	qpl::f32 qpl::camera::get_position_y() const {
		return this->m_position.y;
	}
	qpl::f32 qpl::camera::get_position_z() const {
		return this->m_position.z;
	}

	void qpl::camera::move(qpl::vector3f delta) {
		this->m_position += delta;
	}
	void qpl::camera::move(qpl::f32 x, qpl::f32 y, qpl::f32 z) {
		this->move({ x, y, z });
	}
	void qpl::camera::move_x(qpl::f32 x) {
		this->m_position.x += x;
	}
	void qpl::camera::move_y(qpl::f32 y) {
		this->m_position.y += y;
	}
	void qpl::camera::move_z(qpl::f32 z) {
		this->m_position.z += z;
	}


	void qpl::camera::move_forward(qpl::f32 delta) {
		auto vec = this->get_view_rotation_coordinates() * delta;
		this->move(vec);
	}
	void qpl::camera::move_backwards(qpl::f32 delta) {
		this->look_backwards();
		auto vec = this->get_view_rotation_coordinates() * delta;
		this->look_backwards();
		this->move(vec);
	}
	void qpl::camera::move_up(qpl::f32 delta) {
		this->look_up();
		auto vec = this->get_view_rotation_coordinates() * delta;
		this->look_down();
		this->move(vec);
	}
	void qpl::camera::move_down(qpl::f32 delta) {
		this->look_down();
		auto vec = this->get_view_rotation_coordinates() * delta;
		this->look_up();
		this->move(vec);
	}
	void qpl::camera::move_left(qpl::f32 delta) {
		this->look_left();
		auto vec = this->get_view_rotation_coordinates() * delta;
		this->look_right();
		this->move(vec);
	}
	void qpl::camera::move_right(qpl::f32 delta) {
		this->look_right();
		auto vec = this->get_view_rotation_coordinates() * delta;
		this->look_left();
		this->move(vec);
	}

	void qpl::camera::set_view_rotation(qpl::vector2f pos) {
		this->set_view_rotation_x(pos.x);
		this->set_view_rotation_y(pos.y);
	}
	void qpl::camera::set_view_rotation(qpl::f32 x, qpl::f32 y) {
		this->set_view_rotation_x(x);
		this->set_view_rotation_y(y);
	}
	void qpl::camera::set_view_rotation_x(qpl::f32 x) {
		this->m_view_rotation.x = x;
	}
	void qpl::camera::set_view_rotation_y(qpl::f32 y) {
		this->m_view_rotation.y = y;
	}
	qpl::vector2f qpl::camera::get_view_rotation() const {
		return this->m_view_rotation;
	}
	qpl::f32 qpl::camera::get_view_rotation_x() const {
		return this->m_view_rotation.x;
	}
	qpl::f32 qpl::camera::get_view_rotation_y() const {
		return this->m_view_rotation.y;
	}

	void qpl::camera::rotate_view(qpl::vector2f delta) {
		this->set_view_rotation(this->get_view_rotation() + delta);
	}

	void qpl::camera::look_backwards() {
		this->rotate_view_y(static_cast<qpl::f32>(qpl::pi));
	}
	void qpl::camera::look_up(qpl::f32 delta) {
		this->rotate_view_y(-delta);
	}
	void qpl::camera::look_down(qpl::f32 delta) {
		this->rotate_view_y(delta);
	}
	void qpl::camera::look_left(qpl::f32 delta) {
		this->rotate_view_x(delta);
	}
	void qpl::camera::look_right(qpl::f32 delta) {
		this->rotate_view_x(-delta);
	}
	void qpl::camera::rotate_view_x(qpl::f32 x) {
		this->set_view_rotation_x(this->m_view_rotation.x + x);
	}
	void qpl::camera::rotate_view_y(qpl::f32 y) {
		this->set_view_rotation_y(this->m_view_rotation.y + y);
	}

	void qpl::camera::set_max_velocity(qpl::f32 max) {
		this->m_max_velocity = max;
	}
	qpl::f32 qpl::camera::get_max_velocity() const {
		return this->m_max_velocity;
	}

	void qpl::camera::set_acceleration(qpl::f32 acceleration) {
		this->m_accelerate = acceleration;
	}
	qpl::f32 qpl::camera::get_acceleration() const {
		return this->m_accelerate;
	}

	void qpl::camera::set_deacceleration(qpl::f32 deacceleration) {
		this->m_deaccelerate = deacceleration;
	}
	qpl::f32 qpl::camera::get_deacceleration() const {
		return this->m_deaccelerate;
	}

	void qpl::camera::accelerate_forward(qpl::f32 delta) {
		auto vec = this->get_view_rotation_coordinates() * delta * this->get_acceleration();
		this->m_velocites += vec * this->m_speed;
	}
	void qpl::camera::accelerate_backwards(qpl::f32 delta) {
		this->look_backwards();
		auto vec = this->get_view_rotation_coordinates() * delta * this->get_acceleration();
		this->look_backwards();
		this->m_velocites += vec * this->m_speed;
	}
	void qpl::camera::accelerate_left(qpl::f32 delta) {
		auto copy = this->m_view_rotation;
		this->look_left();
		this->ignore_y_axis();
		auto vec = this->get_view_rotation_coordinates() * delta * this->get_acceleration();
		this->m_view_rotation = copy;
		this->m_velocites += vec * this->m_speed;
	}
	void qpl::camera::accelerate_right(qpl::f32 delta) {
		auto copy = this->m_view_rotation;
		this->look_right();
		this->ignore_y_axis();
		auto vec = this->get_view_rotation_coordinates() * delta * this->get_acceleration();
		this->m_view_rotation = copy;
		this->m_velocites += vec * this->m_speed;
	}
	void qpl::camera::accelerate_up(qpl::f32 delta) {
		this->m_velocites.y += delta * this->get_acceleration() * this->m_speed;;
	}
	void qpl::camera::accelerate_down(qpl::f32 delta) {
		this->m_velocites.y -= delta * this->get_acceleration() * this->m_speed;;
	}


	void qpl::camera::set_speed(qpl::f32 speed) {
		this->m_speed = speed;
	}
	qpl::f32 qpl::camera::get_speed() const {
		return this->m_speed;
	}

	void qpl::camera::update(qpl::f32 delta_time) {
		this->m_velocites.x = std::max(this->m_velocites.x, -this->m_max_velocity);
		this->m_velocites.x = std::min(this->m_velocites.x, this->m_max_velocity);
		this->m_velocites.y = std::max(this->m_velocites.y, -this->m_max_velocity);
		this->m_velocites.y = std::min(this->m_velocites.y, this->m_max_velocity);
		this->m_velocites.z = std::max(this->m_velocites.z, -this->m_max_velocity);
		this->m_velocites.z = std::min(this->m_velocites.z, this->m_max_velocity);

		this->m_velocites *= (qpl::f32{ 1 } - this->m_deaccelerate * delta_time);
		this->m_position += this->m_velocites;


		this->m_view_rotation.x = static_cast<qpl::f32>(std::fmod(this->m_view_rotation.x, qpl::pi * 2));
		this->m_view_rotation.y = static_cast<qpl::f32>(std::fmod(this->m_view_rotation.y + qpl::pi_32 / 2, qpl::pi * 2)) - qpl::pi_32 / 2;
		this->m_view_rotation.y = std::max(this->m_view_rotation.y, -qpl::pi_32 / 2 + 0.01f);
		this->m_view_rotation.y = std::min(this->m_view_rotation.y, qpl::pi_32 / 2 - 0.01f);
	}
	qpl::vector3f qpl::camera::get_velocity() const {
		return this->m_velocites;
	}

	void qpl::camera::ignore_y_axis() {
		this->m_view_rotation.y = 0.0f;
	}
}

struct opengl_state : qsf::base_state {
    void init() override {
        bool exit = false;
        bool sRgb = false;

        // Create a sprite for the background
        backgroundTexture.setSrgb(sRgb);
        if (!backgroundTexture.loadFromFile("resources/background.jpg"));

        background.setTexture(backgroundTexture);

        // Load a texture to apply to our 3D cube
        if (!texture.loadFromFile("resources/texture.png"));

        // Attempt to generate a mipmap for our cube texture
        // We don't check the return value here since
        // mipmapping is purely optional in this example
        texture.generateMipmap();

        // Make the window the active window for OpenGL calls
        this->framework->window.setActive(true);

        // Enable Z-buffer read and write
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glClearDepth(1.f);

        // Disable lighting
        glDisable(GL_LIGHTING);

        // Configure the viewport (the same size as the window)
        glViewport(0, 0, this->framework->window.getSize().x, this->framework->window.getSize().y);

        // Setup a perspective projection
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        GLfloat ratio = static_cast<float>(this->framework->window.getSize().x) / this->framework->window.getSize().y;
        glFrustum(-ratio, ratio, -1.f, 1.f, 1.f, 500.f);

        // Bind the texture
        glEnable(GL_TEXTURE_2D);
        sf::Texture::bind(&texture);

        // Define a 3D cube (6 faces made of 2 triangles composed by 3 vertices)
        cube =
        {
            // positions    // texture coordinates
            -20, -20, -20,  0, 0,
            -20,  20, -20,  1, 0,
            -20, -20,  20,  0, 1,
            -20, -20,  20,  0, 1,
            -20,  20, -20,  1, 0,
            -20,  20,  20,  1, 1,

             20, -20, -20,  0, 0,
             20,  20, -20,  1, 0,
             20, -20,  20,  0, 1,
             20, -20,  20,  0, 1,
             20,  20, -20,  1, 0,
             20,  20,  20,  1, 1,

            -20, -20, -20,  0, 0,
             20, -20, -20,  1, 0,
            -20, -20,  20,  0, 1,
            -20, -20,  20,  0, 1,
             20, -20, -20,  1, 0,
             20, -20,  20,  1, 1,

            -20,  20, -20,  0, 0,
             20,  20, -20,  1, 0,
            -20,  20,  20,  0, 1,
            -20,  20,  20,  0, 1,
             20,  20, -20,  1, 0,
             20,  20,  20,  1, 1,

            -20, -20, -20,  0, 0,
             20, -20, -20,  1, 0,
            -20,  20, -20,  0, 1,
            -20,  20, -20,  0, 1,
             20, -20, -20,  1, 0,
             20,  20, -20,  1, 1,

            -20, -20,  20,  0, 0,
             20, -20,  20,  1, 0,
            -20,  20,  20,  0, 1,
            -20,  20,  20,  0, 1,
             20, -20,  20,  1, 0,
             20,  20,  20,  1, 1
        };

        // Enable position and texture coordinates vertex components
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glVertexPointer(3, GL_FLOAT, 5 * sizeof(GLfloat), cube.data());
        glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(GLfloat), cube.data() + 3);

        // Disable normal and color vertex components
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);

        // Make the window no longer the active window for OpenGL calls
        this->framework->window.setActive(false);

        this->disallow_clear();
    }
    void updating() override {

    }
    void drawing() override {
        // Make the window no longer the active window for OpenGL calls
        this->framework->window.setActive(false);

        // Draw the background
        this->framework->window.pushGLStates();
        this->framework->window.draw(background);
        this->framework->window.popGLStates();

        // Make the window the active window for OpenGL calls
        this->framework->set_active(true);

        // Clear the depth buffer
        glClear(GL_DEPTH_BUFFER_BIT);

        // We get the position of the mouse cursor, so that we can move the box accordingly
        float x = sf::Mouse::getPosition(this->framework->window).x * 200.f / this->framework->window.getSize().x - 100.f;
        float y = -sf::Mouse::getPosition(this->framework->window).y * 200.f / this->framework->window.getSize().y + 100.f;

        // Apply some transformations
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(x, y, -100.f);
        glRotatef(clock.elapsed_f() * 50.f, 1.f, 0.f, 0.f);
        glRotatef(clock.elapsed_f() * 30.f, 0.f, 1.f, 0.f);
        glRotatef(clock.elapsed_f() * 90.f, 0.f, 0.f, 1.f);

        // Draw the cube
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Make the window no longer the active window for OpenGL calls
        this->framework->set_active(false);
    }

    sf::Texture backgroundTexture;
    sf::Sprite background;
    sf::Texture texture;
    std::vector<GLfloat> cube;
    qpl::clock clock;
};


int main() {
    qsf::framework framework;
    framework.set_title("QPL");
    framework.enable_3d();
    framework.set_dimension({ 800, 600 });

    framework.add_state<opengl_state>();
    framework.game_loop();
}
