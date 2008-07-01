#ifndef SURFACE_LIST_H_INCLUDED
#define SURFACE_LIST_H_INCLUDED

#include <list>

#include "map_limits.h"
#include "map_face.h"

using std::list;

typedef list<map_face*> face_list;

enum sort_order {
	BACK_TO_FRONT,
	FRONT_TO_BACK
};

class surface_list {
	public:

		face_list surfaces;

		void sort_by_texture() {
			//Store an array of tex_index => surface list
			vector< face_list > texture_surfaces;

			//resize to the max textures
			texture_surfaces.resize(map_limits::MAX_TEXTURES);

			// Go through all the surfaces in this surface list
			for (face_list::iterator i = surfaces.begin();
				i != surfaces.end(); ++i) {

				//Associate with the texture ID
				int texture_number = (*i)->get_texture_index();
				texture_surfaces[texture_number].push_back((*i));
			}

			//Clear the surface list
			surfaces.clear();

			//Restore the surfaces ordered by texture
			for (int i = 0; i < map_limits::MAX_TEXTURES; ++i) {
				surfaces.insert(surfaces.end(), texture_surfaces[i].begin(), texture_surfaces[i].end());
			}
		}

		void sort_by_distance(sort_order order, const float* camera_position) {
			// TODO write a sorting function that takes the first vertex of each surfaces and
			// sorts by depth
		}
};

#endif // SURFACE_LIST_H_INCLUDED
