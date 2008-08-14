#ifndef OPENGL2DRIVER_H_INCLUDED
#define OPENGL2DRIVER_H_INCLUDED

#include <stack>

#include "kazmathxx/mat4.h"

#include "utilities/colour.h"
#include "utilities/timer.h"
#include "igraphicsdriver.h"

using std::stack;

enum matrix_mode {
	MODELVIEW = 0,
	PROJECTION,
	TEXTURE,
	MAX_MATRIX_MODE
};

class opengl_2_driver : public graphics_driver_interface {
	public:
		opengl_2_driver() {
			m_Ext.vertexBufferObjects = false;
			m_timer = shared_ptr<timer_interface>(new timer());
		}

		struct ExtensionsEnabled {
			bool vertexBufferObjects;
		} m_Ext;


		void begin_scene();
		void end_scene();
		void draw_3d_triangle(const vector<Vec3>& vertices, const Colour& colour);

		shared_ptr<timer_interface> get_timer() {
			return m_timer;
		}

		void load_identity() {
			//Set the current matrix to the identity matrix
			kmMat4Identity(&m_current_matrices[m_current_matrix_mode]);
		}

		void load_matrix(const kmMat4& mat) {
			kmMat4Assign(&m_current_matrices[m_current_matrix_mode], &mat);
		}

		void push_matrix() {
			Mat4 mat = m_current_matrices[m_current_matrix_mode];
			m_matrix_stacks[m_current_matrix_mode].push(mat);
		}

		void pop_matrix() {
			m_current_matrices[m_current_matrix_mode] = m_matrix_stacks[m_current_matrix_mode].top();
			m_matrix_stacks[m_current_matrix_mode].pop();
		}

		void translate(float x, float y, float z) {
			Mat4 translation;
			//Create the translation matrix
			kmMat4Translation(&translation, x, y, z);

			//Multiply the current matrix by it
			kmMat4Multiply(&m_current_matrices[m_current_matrix_mode], &translation, &m_current_matrices[m_current_matrix_mode]);
		}

		void mult_matrix(const kmMat4& mat) {
			kmMat4Multiply(&m_current_matrices[m_current_matrix_mode], &mat, &m_current_matrices[m_current_matrix_mode]);
		}

		Mat4 get_projection_matrix() const {
			return m_current_matrices[PROJECTION];
		}

		Mat4 get_modelview_matrix() const {
			return m_current_matrices[MODELVIEW];
		}

	protected:
		bool do_initialize();

	private:
		bool load_supported_extensions();

		shared_ptr<timer_interface> m_timer;

		matrix_mode m_current_matrix_mode;

		//An array of matrix stacks
		stack<Mat4> m_matrix_stacks[MAX_MATRIX_MODE];

		Mat4 m_current_matrices[MAX_MATRIX_MODE];

};


#endif // OPENGL2DRIVER_H_INCLUDED
