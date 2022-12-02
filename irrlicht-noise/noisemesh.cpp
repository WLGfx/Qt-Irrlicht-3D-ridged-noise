#include "noisemesh.h"

#include <QDebug>

using namespace irr;

NoiseMesh::NoiseMesh(irr::IrrlichtDevice *parent, int size, float scale, float nscale, float cutoff)
{
    device = parent;
    smgr = device->getSceneManager();

    mesh_size = size;
    mesh_scale = scale;
    noise_scale = nscale;
    noise_cutoff = cutoff;

    //noise.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
    //noise.SetCellularReturnType(FastNoiseLite::CellularReturnType_CellValue);
    noise.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
    noise.SetFractalType(FastNoiseLite::FractalType_Ridged);
    noise.SetFractalOctaves(3);

    //noise.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);

    init();
}

NoiseMesh::~NoiseMesh() {
    if (mesh_data) free(mesh_data);
    if (mesh_outline) free(mesh_outline);
}

void NoiseMesh::init() {
    mesh_data = (float*)malloc(mesh_size * mesh_size * mesh_size * sizeof(float));
    mesh_outline = (char*)malloc(mesh_size * mesh_size * mesh_size * sizeof(char));

    create_mesh_nodes();
}

void NoiseMesh::regenerate(int seed, int size, float pscale, float nscale, float cutoff) {
    noise.SetSeed(seed);

    mesh_scale = pscale;
    noise_scale = nscale;
    noise_cutoff = cutoff;

    if (mesh_size != size) {
        free(mesh_data);
        free(mesh_outline);
        mesh_data = (float*)malloc(size * size * size * sizeof(float));
        mesh_outline = (char*)malloc(size * size * size * sizeof(char));
    }
    mesh_size = size;

    create_mesh_nodes();
}

void NoiseMesh::generate_mesh_data() {
    float min = 0.0f, max = 0.0f;
    for (int z = 0; z < mesh_size; z++) {
        for (int y = 0; y < mesh_size; y++) {
            for (int x = 0; x < mesh_size; x++) {
                float n = noise.GetNoise(x * noise_scale,
                                         y * noise_scale,
                                         z * noise_scale);
                mesh_set(x, y, z, n);
                if (n < min) min = n;
                if (n > max) max = n;
            }
        }
    }

    qDebug() << "Min:" << min << "Max:" << max;
}

void NoiseMesh::create_mesh_nodes() {
    generate_mesh_data();

    if (node_main) {
        node_main->removeAll();
        //node_main->drop();
    }

    node_main = smgr->addEmptySceneNode();
    //node_main->getMaterial(0).setFlag(video::E_MATERIAL_FLAG::EMF_FOG_ENABLE, true);

    mesh_cube = smgr->getMesh(MESH_FILENAME);

    core::vector3df rot(0, 0, 0);
    core::vector3df dim(mesh_scale, mesh_scale, mesh_scale);

    mesh_object_count = 0;

    // create the outline array
    for (int z = 0; z < mesh_size; z++) {
        for (int y = 0; y < mesh_size; y++) {
            for (int x = 0; x < mesh_size; x++) {
                float n = mesh_get(x, y, z);
                //if (n < noise_cutoff) continue;

                if (n > noise_cutoff && (
                        mesh_get(x - 1, y, z) < noise_cutoff ||
                        mesh_get(x + 1, y, z) < noise_cutoff ||
                        mesh_get(x, y - 1, z) < noise_cutoff ||
                        mesh_get(x, y + 1, z) < noise_cutoff ||
                        mesh_get(x, y, z - 1) < noise_cutoff ||
                        mesh_get(x, y, z + 1) < noise_cutoff)
                    ) {

                    mesh_outline_set(x, y, z, 1);
                    mesh_object_count++;
                } else {
                    mesh_outline_set(x, y, z, 0);
                }
            }
        }
    }

    scene::SMesh *mesh = new scene::SMesh();
    scene::SMeshBuffer *buf = 0;

    int vertex_pos = 0;
    int index_pos = 0;

    int object_count = mesh_object_count;

    scene::IMeshBuffer *mesh_buffer = mesh_cube->getMeshBuffer(0);

    int object_vertices = mesh_buffer->getVertexCount();
    int object_indices = mesh_buffer->getIndexCount();

    int object_max = (65535 - object_vertices) / object_vertices;

    video::S3DVertex *mb_vertices = (video::S3DVertex*)mesh_buffer->getVertices();
    u16 *mb_indices = mesh_buffer->getIndices();

    int object_current_count;

    for (int z = 0; z < mesh_size; z++) {
        for (int y = 0; y < mesh_size; y++) {
            for (int x = 0; x < mesh_size; x++) {
                if (!mesh_outline_get(x, y, z)) continue;

                // generate a new mesh buffer if empty
                if (!buf) {
                    buf = new scene::SMeshBuffer();
                    mesh->addMeshBuffer(buf);
                    buf->drop();

                    object_current_count = object_count > object_max ? object_max : object_count;

                    buf->Vertices.set_used(object_vertices * object_current_count);
                    buf->Indices.set_used(object_indices * object_current_count);
                }

                // copy object and new position
                core::vector3df pos((x - mesh_size / 2) * mesh_scale,
                                    (y - mesh_size / 2) * mesh_scale,
                                    (z - mesh_size / 2) * mesh_scale);

                for (int vertex = 0; vertex < object_vertices; vertex++) {
                    int index = vertex + vertex_pos;
                    //buf->Vertices[index].Pos = mb_vertices[vertex].Pos * dim + pos;
                    //buf->Vertices[index].Normal = mb_vertices[vertex].Normal;
                    //buf->Vertices[index].Color = mb_vertices[vertex].Color;
                    //buf->Vertices[index].TCoords = mb_vertices[vertex].TCoords;
                    buf->Vertices[index] = mb_vertices[vertex];
                    buf->Vertices[index].Pos *= dim;
                    buf->Vertices[index].Pos += pos;
                }

                for (int index = 0; index < object_indices; index++) {
                    buf->Indices[index_pos + index] = mb_indices[index] + vertex_pos;
                }

                vertex_pos += object_vertices;
                index_pos += object_indices;

                object_current_count--;

                if (!object_current_count) {
                    buf->recalculateBoundingBox();

                    buf = 0;
                    vertex_pos = 0;
                    index_pos = 0;
                }
            }
        }
    }

    //node_main = smgr->addMeshSceneNode(mesh, 0);
    //node_main->getMaterial(0).setFlag(video::E_MATERIAL_FLAG::EMF_FOG_ENABLE, true);

    mesh->setMaterialFlag(irr::video::EMF_FOG_ENABLE, true);
    mesh->recalculateBoundingBox();
    smgr->addMeshSceneNode(mesh, node_main);
    mesh->drop();

    qDebug() << "Billboards:" << mesh_object_count;

    qDebug() << mesh_cube->getMeshBufferCount() << "Mesh buffers";
    //scene::IMeshBuffer *mesh_buffer = mesh_cube->getMeshBuffer(0);
    qDebug() << mesh_buffer->getVertexCount() << "Vertices";
    qDebug() << mesh_buffer->getIndexCount() << "Indices";
}
