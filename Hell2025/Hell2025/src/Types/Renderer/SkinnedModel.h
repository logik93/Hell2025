#pragma once
#include "HellEnums.h"
#include "HellTypes.h"
#include "File/FileFormats.h"
#include <map>
#include <vector>

struct SkinnedModel {
    SkinnedModel() = default;

    void Load(SkinnedModelData& skinnedModelData);
    void AddMeshIndex(uint32_t index);
    void SetFileInfo(FileInfo fileInfo);
    void SetLoadingState(LoadingState loadingState);
    void SetVertexCount(uint32_t vertexCount);

    bool BoneExists(const std::string& boneName);
    const FileInfo& GetFileInfo();
    const std::string& GetName();
    uint32_t GetMeshCount();
    uint32_t GetVertexCount();
    uint32_t GetBoneCount();
    uint32_t GetNodeCount();
    LoadingState& GetLoadingState();
    std::vector<uint32_t>& GetMeshIndices();

public:
    std::vector<Node> m_nodes;
    std::vector<glm::mat4> m_boneOffsets;
    std::map<std::string, unsigned int> m_boneMapping;
    std::map<std::string, unsigned int> m_nodeMapping;
    std::vector<int> m_boneNodeIndices;

private:
    LoadingState m_loadingState = LoadingState::AWAITING_LOADING_FROM_DISK;
    FileInfo m_fileInfo;
    uint32_t m_vertexCount = 0;
    uint32_t m_indexCount = 0;
    std::vector<uint32_t> m_meshIndices;
};
