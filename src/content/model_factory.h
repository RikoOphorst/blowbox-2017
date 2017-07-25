#pragma once

#include "util/shared_ptr.h"
#include "util/weak_ptr.h"
#include "core/scene/entity.h"

struct aiMesh;
struct aiMaterial;
struct aiNode;
enum aiTextureType;

namespace blowbox
{
    class Material;

    /**
    * This is a very straightforward factory. It allows you to load models
    * from disk of any type that Assimp supports. Models aren't added to the
    * scene, you have to do that yourself via EntityFactory::AddChildToEntity().
    *
    * @brief Factory for loading models.
    */
    class ModelFactory
    {
    public:
        /**
        * @brief Loads a model from disk.
        * @param[in] file_path_to_model A file path to the model that you want to load.
        * @returns An entity that sits at the root of the model. Make this Entity a child 
        *          of any other child to start using it.
        */
        static SharedPtr<Entity> LoadModel(const String& file_path_to_model);

    protected:
        /**
        * @brief Converts aiMesh objects to blowbox meshes.
        * @param[in] meshes The meshes that should be converted.
        * @param[in] num_meshes The number of meshes in the meshes array.
        * @param[out] out_meshes The meshes that were created.
        * @param[out] out_material_indices The material indices that each mesh wants.
        */
        static void ProcessMeshes(aiMesh** meshes, unsigned int num_meshes, Vector<SharedPtr<Mesh>>* out_meshes, Vector<int>* out_material_indices);

        /**
        * @brief Converts the vertices in a aiMesh to blowbox vertices.
        * @param[in] mesh The mesh from which the vertices should be processed.
        * @param[out] out_vertices The vertices that were converted.
        */
        static void ProcessVertices(aiMesh* mesh, Vector<Vertex>* out_vertices);

        /**
        * @brief Converts the indices in an aiMesh to blowbox vertices.
        * @param[in] mesh The mesh from which the indices should be processed.
        * @param[out] out_indices The indices that were converted.
        */
        static void ProcessIndices(aiMesh* mesh, Vector<Index>* out_indices);

        /**
        * @brief Processes all imported materials by Assimp to blowbox format.
        * @param[in] materials All materials that were imported by Assimp.
        * @param[in] num_materials The number of materials in the materials array.
        * @param[in] model_directory_path The directory in which the model is located that we're currently loading.
        * @param[out] out_materials The materials that were converted.
        */
        static void ProcessMaterials(aiMaterial** materials, unsigned int num_materials, const String& model_directory_path, Vector<WeakPtr<Material>>* out_materials);

        /**
        * @brief Recursively processes a node. It will output a blowbox Entity.
        * @param[in] node The aiNode that should be processed.
        * @param[in] parent The parent of this node.
        * @param[in] available_meshes An array of meshes from which the meshes should be picked as defined by the node.
        * @param[in] material_indices An array of material indices that describe what the index of the material into the available_materials array is that a Mesh wants.
        * @param[in] available_materials An array of materials from which the materials should be picked as defined by the node.
        */
        static Vector<SharedPtr<Entity>> ProcessNode(aiNode* node, SharedPtr<Entity> parent, const Vector<SharedPtr<Mesh>>& available_meshes, const Vector<int>& material_indices, const Vector<WeakPtr<Material>>& available_materials);

    private:
        /**
        * @brief Converts an aiTextureType to a string.
        * @param[in] type The type to be converted.
        */
        static String ConvertTextureTypeToString(aiTextureType type);

        /**
        * @brief Checks if an aiTextureType is supported by blowbox.
        * @param[in] type The type to be checked.
        */
        static bool CheckIfTextureTypeIsSupported(aiTextureType type);
    };
}