#ifndef IMAP_RENDERER_H_INCLUDED
#define IMAP_RENDERER_H_INCLUDED

#include <tr1/memory>
#include "base_map.h"

class camera_scene_node_interface;

using std::tr1::shared_ptr;

class map_renderer_interface {
public:
    virtual ~map_renderer_interface() {}

    virtual bool initialize(shared_ptr<base_map> map) = 0;
    virtual void generate_resources() = 0;
    virtual void pre_render(shared_ptr<camera_scene_node_interface> camera) = 0;
    virtual void post_render() = 0;
    virtual void render_map() = 0;

    virtual shared_ptr<resource_manager_interface> get_resource_manager() = 0;
};

#endif // IMAP_RENDERER_H_INCLUDED
