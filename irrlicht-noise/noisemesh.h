#ifndef NOISEMESH_H
#define NOISEMESH_H

/*
 * Simple 3D noise using billboards to start off with (Now gone)
 *
 * Changed to constructing a 3d array of noise values and then using a peak
 * cutoff value which defines the outline of the mesh data.
 */

#include "irrlicht.h"
#include "FastNoiseLite.h"

class NoiseMesh
{
public:
    NoiseMesh(irr::IrrlichtDevice *parent, int size, float scale, float noise_scale = 1.0f, float cutoff = 0.9f);
    ~NoiseMesh();

    void regenerate(int seed, int size, float pscale, float nscale, float cutoff);

private:
    irr::IrrlichtDevice *device;
    irr::scene::ISceneManager *smgr;

    int mesh_size;
    float mesh_scale;
    float noise_scale;
    float noise_cutoff;

    irr::scene::ISceneNode *node_main = 0;

    FastNoiseLite noise;

    void init();

    // Updated version

    float *mesh_data = 0;
    char *mesh_outline = 0;
    int mesh_object_count = 0;
    // could be inlined, but not bothered (up to compiler)
    void mesh_set(int x, int y, int z, float v) {
        long index = z * mesh_size * mesh_size + y * mesh_size + x;
        mesh_data[index] = v;
    }
    // account for overflow on the edges
    float mesh_get(int x, int y, int z) {
        if (x < 0 || x >= mesh_size) return 0;
        if (y < 0 || y >= mesh_size) return 0;
        if (z < 0 || z >= mesh_size) return 0;
        long index = z * mesh_size * mesh_size + y * mesh_size + x;
        return mesh_data[index];
    }
    void mesh_outline_set(int x, int y, int z, char v) {
        long index = z * mesh_size * mesh_size + y * mesh_size + x;
        mesh_outline[index] = v;
    }
    char mesh_outline_get(int x, int y, int z) {
        long index = z * mesh_size * mesh_size + y * mesh_size + x;
        return mesh_outline[index];
    }
    void generate_mesh_data();
    void create_mesh_nodes();

    irr::scene::IMesh *mesh_cube;
    //char *mesh_filename = "../cube.obj";
};

#define MESH_FILENAME "../cube.obj"

#endif // NOISEMESH_H
