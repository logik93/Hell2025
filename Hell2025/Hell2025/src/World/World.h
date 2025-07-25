#pragma once
#include "HellTypes.h"
#include "Core/Debug.h"
#include "Game/Allies/Mermaid/Mermaid.h"
#include "Game/Enemies/Kangaroo/Kangaroo.h"
#include "Game/Enemies/Shark/Shark.h"
#include "Types/Game/AnimatedGameObject.h"
#include "Types/Game/Bullet.h"
#include "Types/Game/BulletCasing.h"
#include "Types/Game/Decal.h"
#include "Types/Game/GameObject.h"
#include "Types/Game/Light.h"
#include "Types/Game/PickUp.h"
#include "Types/Effects/VolumetricBloodSplatter.h"
#include "Types/Effects/ScreenSpaceBloodDecal.h"
#include "Types/House/Door.h"
#include "Types/House/Plane.h"
#include "Types/House/Wall.h"
#include "Types/House/Window.h"
#include "Types/Misc/PictureFrame.h"
#include "Types/Misc/Piano.h"
#include "Types/Exterior/Tree.h"
#include "Util/Util.h"
#include "glm/gtx/intersect.hpp"
#include <vector>
#include "Modelling/Clipping.h"

#include "Types/Renderer/MeshBuffer.h"

struct ViewportBvhData {
    std::vector<PrimitiveInstance> instances;
    uint64_t sceneBvhId;
    BvhRayResult closestHit;
};

namespace World {
    AnimatedGameObject* GetRooTest();

    void Init();
    void BeginFrame();
    void EndFrame();
    void Update(float deltaTime);

    void NewRun();

    void SubmitRenderItems();

    void LoadMap(const std::string& mapName);
    void LoadMap(MapCreateInfo* mapCreateInfo);

    void LoadSingleSector(SectorCreateInfo* sectorCreateInfo, bool loadHouses);
    void LoadSingleHouse(HouseCreateInfo* houseCreateInfo);
    
    void LoadDeathMatchMap();

    void ResetWorld();
    void LoadEmptyWorld();
    void NewCampainWorld();

    bool ChunkExists(int x, int z);
    const uint32_t GetChunkCountX();
    const uint32_t GetChunkCountZ();
    const uint32_t GetChunkCount(); 
    const HeightMapChunk* GetChunk(int x, int z);

    void AddBullet(BulletCreateInfo createInfo);
    void AddDoor(DoorCreateInfo createInfo, SpawnOffset spawnOffset = SpawnOffset());
    void AddBulletCasing(BulletCasingCreateInfo createInfo, SpawnOffset spawnOffset = SpawnOffset());
    void AddDecal(const DecalCreateInfo& createInfo);
    void AddGameObject(GameObjectCreateInfo createInfo, SpawnOffset spawnOffset = SpawnOffset());
    void AddHousePlane(PlaneCreateInfo createInfo, SpawnOffset spawnOffset);
    void AddKangaroo(const KangarooCreateInfo& createInfo);
    void AddLight(LightCreateInfo createInfo, SpawnOffset spawnOffset = SpawnOffset());
    void AddMermaid(MermaidCreateInfo createInfo, SpawnOffset spawnOffset = SpawnOffset());
    void AddScreenSpaceBloodDecal(ScreenSpaceBloodDecalCreateInfo createInfo);
    void AddPiano(PianoCreateInfo createInfo, SpawnOffset spawnOffset);
    void AddPickUp(PickUpCreateInfo createInfo, SpawnOffset spawnOffset = SpawnOffset());
    void AddPictureFrame(PictureFrameCreateInfo createInfo, SpawnOffset spawnOffset = SpawnOffset());
    void AddTree(TreeCreateInfo createInfo, SpawnOffset spawnOffset = SpawnOffset());
    uint64_t AddWall(WallCreateInfo createInfo, SpawnOffset spawnOffset = SpawnOffset());
    void AddVolumetricBlood(glm::vec3 position, glm::vec3 front);
    void AddWindow(WindowCreateInfo createInfo, SpawnOffset spawnOffset);

    void AddHouse(HouseCreateInfo houseCreateInfo, SpawnOffset spawnOffset);

    void EnableOcean();
    void DisableOcean();
    bool HasOcean();

    // Logic
    void ProcessBullets();

    // Saving
    SectorCreateInfo CreateSectorInfoFromWorldObjects();
    void SaveSector(SectorCreateInfo sectorCreateInfo);

    // Creation
    void CreateGameObject();
    uint64_t CreateAnimatedGameObject();

    // Removal
    void SetObjectPosition(uint64_t objectID, glm::vec3 position);
    void RemoveObject(uint64_t objectID);
    
    // BVH
    void UpdatePlayerBvhs();
    BvhRayResult ClosestHit(glm::vec3 rayOrigin, glm::vec3 rayDir, float maxRayDistance, int viewportIndex);

    const float GetWorldSpaceWidth();
    const float GetWorldSpaceDepth();
    const uint32_t GetMapWidth();
    const uint32_t GetMapDepth();
    const uint32_t GetHeightMapCount();
    const std::string& GetSectorNameAtLocation(int x, int z);
    const std::string& GetHeightMapNameAtLocation(int x, int z);
    bool IsMapCellInRange(int x, int z);
    void PrintMapCreateInfoDebugInfo();

    void UpdateDoorAndWindowCubeTransforms();
    void ResetWeatherboardMeshBuffer();

    // Util
    bool ObjectTypeIsInteractable(ObjectType objectType, uint64_t objectId, glm::vec3 playerCameraPosition, glm::vec3 rayHitPosition);

    // Map
    const std::string& GetCurrentMapName();

    // House
    void SaveHouse();
    void UpdateClippingCubes();
    void UpdateAllWallCSG();
    void UpdateHouseMeshBuffer();
    void UpdateWeatherBoardMeshBuffer();

    MeshBuffer& GetHouseMeshBuffer();
    MeshBuffer& GetWeatherBoardMeshBuffer();
    Mesh* GetHouseMeshByIndex(uint32_t meshIndex);

    AnimatedGameObject* GetAnimatedGameObjectByObjectId(uint64_t objectID);
    Door* GetDoorByObjectId(uint64_t objectID);
    Door* GetDoorByDoorFrameObjectId(uint64_t objectID);
    Piano* GetPianoByObjectId(uint64_t objectId);
    Piano* GetPianoByMeshNodeObjectId(uint64_t objectId);
    PianoKey* GetPianoKeyByObjectId(uint64_t objectId);
    PickUp* GetPickUpByObjectId(uint64_t objectID);
    PictureFrame* GetPictureFrameByObjectId(uint64_t objectId);
    Plane* GetPlaneByObjectId(uint64_t objectID); 
    Tree* GetTreeByObjectId(uint64_t objectId);
    Wall* GetWallByObjectId(uint64_t objectID);
    Wall* GetWallByWallSegmentObjectId(uint64_t objectID);
    Shark* GetSharkByObjectId(uint64_t objectID);
    Window* GetWindowByObjectId(uint64_t objectId);
    GameObject* GetGameObjectByIndex(int32_t index);
    GameObject* GetGameObjectByName(const std::string& name);
    Light* GetLightByIndex(int32_t index);
    PickUp* GetPickUpByIndex(int32_t index);
    Tree* GetTreeByIndex(int32_t index);

    size_t GetLightCount();

    std::vector<AnimatedGameObject>& GetAnimatedGameObjects();
    std::vector<ScreenSpaceBloodDecal>& GetScreenSpaceBloodDecals();
    std::vector<Bullet>& GetBullets();
    std::vector<BulletCasing>& GetBulletCasings();
    std::vector<ClippingCube>& GetClippingCubes();
    std::vector<Decal>& GetDecals();
    std::vector<Door>& GetDoors();
    std::vector<GameObject>& GetGameObjects();
    std::vector<HeightMapChunk>& GetHeightMapChunks();
    std::vector<Plane>& GetPlanes();
    std::vector<Light>& GetLights();
    std::vector<Kangaroo>& GetKangaroos();
    std::vector<Mermaid>& GetMermaids();
    std::vector<Piano>& GetPianos();
    std::vector<PickUp>& GetPickUps();
    std::vector<PictureFrame>& GetPictureFrames();
    std::vector<Transform>& GetDoorAndWindowCubeTransforms();
    std::vector<Shark>& GetSharks();
    std::vector<Tree>& GetTrees();
    std::vector<Wall>& GetWalls();
    std::vector<VolumetricBloodSplatter>& GetVolumetricBloodSplatters();
    std::vector<Window>& GetWindows();

    //std::vector<RenderItem>& GetRenderItems();
    //std::vector<RenderItem>& GetRenderItemsBlended();
    //std::vector<RenderItem>& GetRenderItemsAlphaDiscarded();
    //std::vector<RenderItem>& GetRenderItemsHairTopLayer();
    //std::vector<RenderItem>& GetRenderItemsHairBottomLayer();
    std::vector<RenderItem>& GetSkinnedRenderItems();
}

/*
    Notes
    
    * Sectors
      - Each sector can or can not have a house
      - There is always a house in the first sector you DON'T spawn in
      - Each sector has a 50% chance of having a house
      - There can never be 2 sectors WITHOUT a house in a row
      - If a sector has no house, then it may be a mermaid shop sector
      - If a sector has no house, then it may be a dense woods sector, aka trees and full of enemies

    * Witches
      - they are a rarer shop than the mermaid shop
      - they sell better items
      - they hover above a fire 

*/