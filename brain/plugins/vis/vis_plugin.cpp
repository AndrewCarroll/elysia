#include "../../Platform.hpp"
#include "BrainPlugins.hpp"
#include "Visualization.hpp"
#include "GraphicsSystem.hpp"
static int core_plugin_refcount = 0;

SIRIKATA_PLUGIN_EXPORT_C void init() {
    core_plugin_refcount++;
    Elysia::BrainPlugins::registerBrainPlugin("Vis",&Elysia::makeVisualization);
}

SIRIKATA_PLUGIN_EXPORT_C void destroy() {
    Elysia::Deinitialize();
    core_plugin_refcount--;
}

SIRIKATA_PLUGIN_EXPORT_C const char* name() {
    return "vis";
}

SIRIKATA_PLUGIN_EXPORT_C int refcount() {
    return core_plugin_refcount;
}
