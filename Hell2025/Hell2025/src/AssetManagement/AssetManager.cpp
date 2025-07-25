﻿#pragma once
#include "AssetManager.h"

#include "AssetManagement/BakeQueue.h"
#include "API/OpenGL/GL_backend.h"
#include "API/Vulkan/VK_backend.h"
#include "BackEnd/Backend.h"
#include "File/AssimpImporter.h"
#include "Renderer/Renderer.h"
#include "UI/UIBackEnd.h"
#include "Tools/ImageTools.h"
#include "Util/Util.h"
#include "World/World.h"

#include "World/HouseManager.h"
#include "World/HeightMapManager.h"
#include "World/MapManager.h"
#include "World/SectorManager.h"

#include <unordered_map>
#include <future>
#include <mutex>
#include <thread>
#include <vector>

namespace AssetManager {

    std::vector<Animation> g_animations;
    std::vector<Material> g_materials;
    std::vector<Mesh> g_meshes;
    std::vector<Model> g_models;
    std::vector<SpriteSheetTexture> g_spriteSheetTextures;
    std::vector<Texture> g_textures;
    std::vector<SkinnedMesh> g_skinnedMeshes;
    std::vector<SkinnedModel> g_skinnedModels;
    std::unordered_map<std::string, int> g_textureIndexMap;
    std::unordered_map<std::string, int> g_materialIndexMap;
    std::unordered_map<std::string, int> g_modelIndexMap;
    std::vector<std::string> g_loadLog;
    std::vector<std::future<void>> g_futures;
    bool g_loadingComplete = false;

    std::vector<Vertex> g_vertices;
    std::vector<WeightedVertex> g_weightedVertices;
    std::vector<uint32_t> g_indices;
    std::vector<uint32_t> g_weightedIndices;

    void AddItemToLoadLog(std::string text);
    void CompressMissingDDSTexutres();
    void FindAssetPaths();
    void LoadMinimumTextures();
    void LoadTexture(Texture* texture);

    bool FileInfoIsAlbedoTexture(const FileInfo& fileInfo);
    std::string GetMaterialNameFromFileInfo(const FileInfo& fileInfo);

    void Init() {
        CompressMissingDDSTexutres();
        ExportMissingModels();
        ExportMissingModelBvhs();
        ExportMissingSkinnedModels();
        LoadMinimumTextures();
        FindAssetPaths();

        // Fire off all async loading calls
        LoadPendingTexturesAsync();

        LoadPendingAnimationsAsync();
    }

    bool LoadingComplete() {
        return g_loadingComplete;
    }

    void UpdateLoading() {

        LoadPendingModelsAsync();
        LoadPendingSkinnedModelsAsync();

        // Calculate load log text
        std::string text = "";
        int maxLinesDisplayed = 36;
        int endIndex = AssetManager::GetLoadLog().size();
        int beginIndex = std::max(0, endIndex - maxLinesDisplayed);
        for (int i = beginIndex; i < endIndex; i++) {
            text += AssetManager::GetLoadLog()[i] + "\n";
        }

        UIBackEnd::BlitText(text, "StandardFont", 0, 0, Alignment::TOP_LEFT, 2.0f);

        // Loading complete?
        g_loadingComplete = true;
        for (Model& model : g_models) {
            if (model.GetLoadingState() != LoadingState::LOADING_COMPLETE) {
                g_loadingComplete = false;
                return;
            }
        }

        static bool modelsBaked = false;
        if (!modelsBaked) {
            BakePendingModels();
            modelsBaked = true;
        }

        for (Texture& texture : g_textures) {
            texture.CheckForBakeCompletion();
            if (!texture.BakeComplete()) {
                g_loadingComplete = false;
                return;
            }
        }
        for (SkinnedModel& skinnedModel : g_skinnedModels) {
            if (skinnedModel.GetLoadingState() != LoadingState::LOADING_COMPLETE) {
                g_loadingComplete = false;
                std::cout << skinnedModel.GetFileInfo().name;
                return;
            }
        }

        if (LoadingComplete()) {
            //BakePendingModels();
            BuildPrimitives();
            BuildIndexMaps(); // required before BuildMaterials()
            BuildMaterials();
            BuildIndexMaps(); // but also required after BuildMaterials()... FIX! maybe just add to the index map when you create these objects.
            BuildGoldenMaterialVariants();
            BuildIndexMaps();
            BuildSpriteSheetTextures();
            CopyInAllLoadedModelBvhData();

            HeightMapManager::Init();
            HouseManager::Init();
            MapManager::Init();
            SectorManager::LoadSectorsFromDisk();
            World::Init();

            if (BackEnd::GetAPI() == API::OPENGL) {
                OpenGLBackEnd::CleanUpBakingPBOs();
                OpenGLBackEnd::UploadVertexData(g_vertices, g_indices);
                OpenGLBackEnd::UploadWeightedVertexData(g_weightedVertices, g_weightedIndices);
            }

            // Free all cpu texture data
            for (Texture& texture : g_textures) {
                texture.FreeCPUMemory();
            }

            Renderer::InitMain();
            std::cout << "Assets loaded\n";


            //for (Model& model : GetModels()) {
            //    std::cout << "'" << model.GetName() << "'\n";
            //}
        }
    }

    void FindAssetPaths() {
        // Find all animations
        for (FileInfo& fileInfo : Util::IterateDirectory("res/animations")) {
            Animation& animation = g_animations.emplace_back();
            animation.SetFileInfo(fileInfo);
        }
        // Find .model files
        for (FileInfo& fileInfo : Util::IterateDirectory("res/models")) {
            Model& model = g_models.emplace_back();
            model.SetFileInfo(fileInfo);
        }
        // Find skinned model files
        for (FileInfo& fileInfo : Util::IterateDirectory("res/skinned_models")) {
            SkinnedModel& skinnedModel = g_skinnedModels.emplace_back();
            skinnedModel.SetFileInfo(fileInfo);
        }
        // Find sprite sheet textures
        for (FileInfo& fileInfo : Util::IterateDirectory("res/textures/spritesheets")) {
            SpriteSheetTexture& spriteSheetTexture = g_spriteSheetTextures.emplace_back();
            spriteSheetTexture.SetFileInfo(fileInfo);
        }
        // Find all textures
        for (FileInfo& fileInfo : Util::IterateDirectory("res/textures/uncompressed", { "png", "jpg", "tga" })) {
            Texture& texture = g_textures.emplace_back();
            texture.SetFileInfo(fileInfo);
            texture.SetImageDataType(ImageDataType::UNCOMPRESSED);
            texture.SetTextureWrapMode(TextureWrapMode::REPEAT);
            texture.SetMinFilter(TextureFilter::LINEAR_MIPMAP);
            texture.SetMagFilter(TextureFilter::LINEAR);
            texture.RequestMipmaps();
        }
        for (FileInfo& fileInfo : Util::IterateDirectory("res/textures/compressed", { "dds" })) {
            Texture& texture = g_textures.emplace_back();
            texture.SetFileInfo(fileInfo);
            texture.SetImageDataType(ImageDataType::COMPRESSED);
            texture.SetTextureWrapMode(TextureWrapMode::REPEAT);
            texture.SetMinFilter(TextureFilter::LINEAR_MIPMAP);
            texture.SetMagFilter(TextureFilter::LINEAR);
            texture.RequestMipmaps();
        }
        for (FileInfo& fileInfo : Util::IterateDirectory("res/textures/ui", { "png", "jpg", })) {
            Texture& texture = g_textures.emplace_back();
            texture.SetFileInfo(fileInfo);
            texture.SetImageDataType(ImageDataType::UNCOMPRESSED);
            texture.SetTextureWrapMode(TextureWrapMode::CLAMP_TO_EDGE);
            texture.SetMinFilter(TextureFilter::LINEAR);
            texture.SetMagFilter(TextureFilter::LINEAR);
        }
        for (FileInfo& fileInfo : Util::IterateDirectory("res/textures/exr", { "exr" })) {
            Texture& texture = g_textures.emplace_back();
            texture.SetFileInfo(fileInfo);
            texture.SetImageDataType(ImageDataType::EXR);
            texture.SetTextureWrapMode(TextureWrapMode::CLAMP_TO_EDGE);
            texture.SetMinFilter(TextureFilter::LINEAR);
            texture.SetMagFilter(TextureFilter::NEAREST);
        }
        for (FileInfo& fileInfo : Util::IterateDirectory("res/textures/spritesheets", { "png", "jpg", "tga" })) {
            Texture& texture = g_textures.emplace_back();
            texture.SetFileInfo(fileInfo);
            texture.SetImageDataType(ImageDataType::UNCOMPRESSED);
            texture.SetTextureWrapMode(TextureWrapMode::REPEAT);
            texture.SetMinFilter(TextureFilter::LINEAR);
            texture.SetMagFilter(TextureFilter::LINEAR);
        }
    }

    void LoadMinimumTextures() {
        // Find files
        for (FileInfo& fileInfo : Util::IterateDirectory("res/fonts", { "png" })) {
            Texture& texture = g_textures.emplace_back();
            texture.SetFileInfo(fileInfo);
            texture.SetImageDataType(ImageDataType::UNCOMPRESSED);
            texture.SetTextureWrapMode(TextureWrapMode::CLAMP_TO_EDGE);
            texture.SetMinFilter(TextureFilter::NEAREST);
            texture.SetMagFilter(TextureFilter::NEAREST);
        }
        LoadPendingTexturesAsync();
        BakeQueue::ImmediateBakeAllTextures();
        BuildIndexMaps();
    }

    void AddItemToLoadLog(std::string text) {
        std::replace(text.begin(), text.end(), '\\', '/');
        g_loadLog.push_back(text);
        if (false) {
            static std::mutex mutex;
            std::lock_guard<std::mutex> lock(mutex);
            std::cout << text << "\n";
        }
    }

    std::vector<std::string>& GetLoadLog() {
        return g_loadLog;
    }

    void BuildIndexMaps() {
        g_modelIndexMap.clear();
        g_textureIndexMap.clear();
        g_materialIndexMap.clear();
        for (int i = 0; i < g_models.size(); i++) {
            g_modelIndexMap[g_models[i].GetName()] = i;
        }
        for (int i = 0; i < g_textures.size(); i++) {
            g_textureIndexMap[g_textures[i].GetFileInfo().name] = i;
        }
        for (int i = 0; i < g_materials.size(); i++) {
            g_materialIndexMap[g_materials[i].m_name] = i;
        }
    }

    std::vector<Animation>& GetAnimations() {
        return g_animations;
    }

    std::vector<uint32_t>& GetIndies() {
        return g_indices;
    }

    std::vector<Mesh>& GetMeshes() {
        return g_meshes;
    }

    std::vector<Material>& GetMaterials() {
        return g_materials;
    }

    std::vector<Model>& GetModels() {
        return g_models;
    }

    std::vector<SkinnedModel>& GetSkinnedModels() {
        return g_skinnedModels;
    }

    std::vector<SkinnedMesh>& GetSkinnedMeshes() {
        return g_skinnedMeshes;
    }

    std::vector<SpriteSheetTexture>& GetSpriteSheetTextures() {
        return g_spriteSheetTextures;
    }

    std::vector<Texture>& GetTextures() {
        return g_textures;
    }

    std::vector<Vertex>& GetVertices() {
        return g_vertices;
    }

    std::vector<uint32_t>& GetIndices() {
        return g_indices;
    }

    std::vector<uint32_t>& GetWeightedIndies() {
        return g_weightedIndices;
    }

    std::vector<WeightedVertex>& GetWeightedVertices() {
        return g_weightedVertices;
    }

    std::unordered_map<std::string, int>& GetMaterialIndexMap() {
        return g_materialIndexMap;
    }

    std::unordered_map<std::string, int>& GetModelIndexMap() {
        return g_modelIndexMap;
    }

    std::unordered_map<std::string, int>& GetTextureIndexMap() {
        return g_textureIndexMap;
    }

    Mesh* GetCubeMesh() {
        static Mesh* mesh = nullptr;
        
        // Keep trying to get the cube mesh until you successfully get it.
        // This prevents a crash on load when trying to retrieve the cube mesh before it has loaded the cube model file
        if (!mesh) {
            int modelIndex = GetModelIndexByName("Cube");
            if (modelIndex != -1) {
                Model* model = GetModelByIndex(modelIndex);
                if (model) {
                    int meshIndex =model->GetMeshIndices()[0];
                    mesh = GetMeshByIndex(meshIndex);
                }
            }
        }        
        return mesh;
    }

    int GetQuadZFacingMeshIndex() {
        static int meshIndex = GetModelByIndex(GetModelIndexByName("QuadZFacing"))->GetMeshIndices()[0];
        return meshIndex;
    }

    Mesh* GetQuadZFacingMesh() {
        // Clean me up
        static Mesh* mesh = GetMeshByIndex(GetModelByIndex(GetModelIndexByName("QuadZFacing"))->GetMeshIndices()[0]);
        return mesh;
    }
       
    Mesh* GetMeshByModelNameMeshIndex(const std::string& modelName, uint32_t meshIndex) {
        Model* model = GetModelByName(modelName);
        if (!model || meshIndex < 0 && meshIndex >= model->GetMeshCount()) {
            std::cout << "AssetManager::GetMeshByModelNameMeshIndex() failed: model name '" << modelName << "' not found\n";
            return nullptr;
        }
        else {
            return AssetManager::GetMeshByIndex(model->GetMeshIndices()[meshIndex]);
        }
    }

    Mesh* GetMeshByModelNameMeshName(const std::string& modelName, const std::string& meshName) {
        Model* model = GetModelByName(modelName);
        if (!model) return nullptr;

        for (uint32_t meshIndex : model->GetMeshIndices()) {
            Mesh* mesh = GetMeshByIndex(meshIndex);
            if (mesh && mesh->GetName() == meshName) {
                return mesh;
            }
        }
        return nullptr;
    }

    int GetMeshIndexByModelNameMeshName(const std::string& modelName, const std::string& meshName) {
        Model* model = GetModelByName(modelName);
        if (!model) return -1;

        for (uint32_t meshIndex : model->GetMeshIndices()) {
            Mesh* mesh = GetMeshByIndex(meshIndex);
            if (mesh && mesh->GetName() == meshName) {
                return meshIndex;
            }
        }
        return -1;
    }
}




    



    

