#ifndef MAP_H
#define MAP_H

#include <vector>
#include <kazmathxx/plane.h>

#include "iresource.h"

#include "map_vertex.h"
#include "map_face.h"
#include "map_texture.h"

typedef int resource_id;

typedef vector<map_face*> face_array;

using std::vector;

class base_map : public resource_interface {
    public:
			base_map();
      virtual ~base_map();

			face_array& get_faces() { return m_faces; }

			virtual resource_id get_resource_id() const {
				return m_resource_id;
			}

    protected:
			vector<map_vertex> m_vertices;
			face_array m_faces;
			vector<map_texture> m_textures;
			vector<Plane> m_planes;

			virtual void print_statistics();

		private:
			resource_id m_resource_id;
};

#endif // MAP_H
