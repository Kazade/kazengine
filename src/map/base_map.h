#ifndef MAP_H
#define MAP_H

#include <vector>
#include <tr1/memory>
#include <kazmathxx/plane.h>

#include "iresource.h"

#include "map_vertex.h"
#include "map_face.h"
#include "map_texture.h"

using std::vector;
using std::tr1::shared_ptr;

typedef int resource_id;
typedef vector<shared_ptr<map_face> > face_array;

class resource_manager_interface;

class base_map : public resource_interface {
    public:
			base_map(resource_manager_interface* owning_manager);
      virtual ~base_map();

			face_array& get_faces() { return m_faces; }

			virtual resource_id get_resource_id() const {
				return m_resource_id;
			}

			virtual resource_manager_interface* get_owning_resource_manager() {
				return m_owning_resource_manager;
			}

			vector<map_texture>& get_textures() {
				return m_textures;
			}

			vector<Plane>& get_planes() {
				return m_planes;
			}

    protected:
			vector<map_vertex> m_vertices;
			face_array m_faces;
			vector<map_texture> m_textures;
			vector<Plane> m_planes;

			virtual void print_statistics();

		private:
			resource_id m_resource_id;

			resource_manager_interface* m_owning_resource_manager;
};

#endif // MAP_H
