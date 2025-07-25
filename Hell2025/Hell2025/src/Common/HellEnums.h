#pragma once
#include <cstdint>

enum class API {
    OPENGL,
    VULKAN,
    UNDEFINED
};

enum class WindowedMode { 
    WINDOWED, 
    FULLSCREEN 
};

enum class Alignment {
    CENTERED,
    CENTERED_HORIZONTAL,
    CENTERED_VERTICAL,
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT
};

enum class LoadingState {
    AWAITING_LOADING_FROM_DISK,
    LOADING_FROM_DISK,
    LOADING_COMPLETE
};

enum class BakeState {
    AWAITING_BAKE,
    BAKING_IN_PROGRESS,
    BAKE_COMPLETE,
    UNDEFINED
};

enum class BlendingMode { 
    BLEND_DISABLED, 
    BLENDED,
    ALPHA_DISCARDED,
    HAIR_UNDER_LAYER,
    HAIR_TOP_LAYER,
    DO_NOT_RENDER 
};

enum class ImageDataType {
    UNCOMPRESSED,
    COMPRESSED,
    EXR,
    UNDEFINED
};

enum class TextureWrapMode {
    REPEAT,
    MIRRORED_REPEAT,
    CLAMP_TO_EDGE,
    CLAMP_TO_BORDER,
    UNDEFINED
};

enum class TextureFilter {
    NEAREST,
    LINEAR,
    LINEAR_MIPMAP,
    UNDEFINED
};

enum class ObjectType {
    NONE = 0,
    CHARACTER_CONTROLLER,
    DECAL,
    DOOR,
    DOOR_FRAME,
    GAME_OBJECT,
    HEIGHT_MAP,
    PLANE,
    LIGHT,
    PICK_UP,
    TREE,
    UNDEFINED,
    PIANO,
    PIANO_KEY,
    PIANO_KEYBOARD_COVER,
    PIANO_TOP_COVER,
    PIANO_SHEET_MUSIC_REST,
    PIANO_SHEET_SUSTAIN_PEDAL,
    PICTURE_FRAME,
    RAGDOLL_ENEMY,
    RAGDOLL_PLAYER,
    SHARK,
    WALL,
    WALL_SEGMENT,
    WINDOW
};

enum class Axis {
    X,
    Y,
    Z,
    NONE,
};

enum class SplitscreenMode { 
    FULLSCREEN, 
    TWO_PLAYER, 
    FOUR_PLAYER, 
    SPLITSCREEN_MODE_COUNT 
};

enum class ShadingMode {
    SHADED,
    WIREFRAME,
    WIREFRAME_OVERLAY,
    SHADING_MODE_COUNT
};

enum class CameraView {
    PERSPECTIVE,
    ORTHO,
    FRONT,
    BACK,
    RIGHT,
    LEFT,
    TOP,
    BOTTOM,
    UNDEFINED
};

enum struct EditorState {
    IDLE,
    RESIZING_HORIZONTAL,
    RESIZING_VERTICAL,
    RESIZING_HORIZONTAL_VERTICAL,
    GIZMO_TRANSLATING,
    GIZMO_SCALING,
    GIZMO_ROTATING,
    DRAGGING_SELECT_RECT,

    // Object placement
    DOOR_PLACEMENT,
    PICTURE_FRAME_PLACEMENT,
    TREE_PLACEMENT,
    WALL_PLACEMENT,
    WINDOW_PLACEMENT
};

enum WeaponAction {
    IDLE = 0,
    FIRE,
    DRY_FIRE,
    RELOAD,
    RELOAD_FROM_EMPTY,
    DRAW_BEGIN,
    DRAWING,
    DRAWING_FIRST,
    DRAWING_WITH_SHOTGUN_PUMP,
    SPAWNING,
    SHOTGUN_UNLOAD_BEGIN,
    SHOTGUN_UNLOAD_SINGLE_SHELL,
    SHOTGUN_UNLOAD_DOUBLE_SHELL,
    SHOTGUN_UNLOAD_END,
    SHOTGUN_RELOAD_BEGIN,
    SHOTGUN_RELOAD_SINGLE_SHELL,
    SHOTGUN_RELOAD_DOUBLE_SHELL,
    SHOTGUN_RELOAD_END,
    SHOTGUN_RELOAD_END_WITH_PUMP,
    ADS_IN,
    ADS_OUT,
    ADS_IDLE,
    ADS_FIRE,
    MELEE,
    TOGGLING_AUTO
};

enum class ShellEjectionState {
    IDLE, AWAITING_SHELL_EJECTION
};

enum InputType {
    KEYBOARD_AND_MOUSE,
    CONTROLLER
};

enum CollisionGroup : uint64_t {
    NO_COLLISION = 0,
    BULLET_CASING = 1,
    CHARACTER_CONTROLLER = 2,
    ENVIROMENT_OBSTACLE = 4,
    GENERIC_BOUNCEABLE = 8,
    ITEM_PICK_UP = 16,
    RAGDOLL_PLAYER = 32,
    DOG_CHARACTER_CONTROLLER = 64,
    GENERTIC_INTERACTBLE = 128,
    ENVIROMENT_OBSTACLE_NO_DOG = 256,
    SHARK = 512,
    LADDER = 1024,
    RAGDOLL_ENEMY = 2048
};

// Re-evaluate how this works, coz it alway fucks you up, 
// and PhysX this group bitmask is used for more than just raycasts, pretty sure
enum RaycastGroup {
    RAYCAST_DISABLED = 0,
    RAYCAST_ENABLED = 1,
    DOBERMAN = 32
};

enum DebugRenderMode {
    NONE = 0, 
    ASTAR_MAP,
    DECALS,
    RAGDOLLS,
    PATHFINDING_RECAST,
    PHYSX_ALL,
    PHYSX_RAYCAST,
    PHYSX_COLLISION,
    RAYTRACE_LAND,
    PHYSX_EDITOR,
    BOUNDING_BOXES,
    RTX_LAND_AABBS,
    RTX_LAND_TRIS,
    RTX_LAND_TOP_LEVEL_ACCELERATION_STRUCTURE,
    RTX_LAND_BOTTOM_LEVEL_ACCELERATION_STRUCTURES,
    RTX_LAND_TOP_AND_BOTTOM_LEVEL_ACCELERATION_STRUCTURES,
    CLIPPING_CUBES,
    HOUSE_GEOMETRY,
    BONES,
    BONE_TANGENTS,
    DEBUG_LINE_MODE_COUNT,
};

enum struct LightType {
    LAMP_POST = 0,
    HANGING_LIGHT 
};

enum struct EditorViewportSplitMode {
    SINGLE,
    FOUR_WAY_SPLIT
};

enum struct PickUpType {
    SHOTGUN_AMMO_BUCKSHOT,
    SHOTGUN_AMMO_SLUG,
    AKS74U,
    REMINGTON_870,
    UNDEFINED
};

enum struct EditorMode {
    HEIGHTMAP_EDITOR,
    HOUSE_EDITOR,
    MAP_EDITOR,
    SECTOR_EDITOR,
    WEAPON_EDITOR
};

enum struct PhysicsType {
    NONE = 0,
    RIGID_DYNAMIC,
    RIGID_STATIC,
    HEIGHT_FIELD,
    GROUND_PLANE,
    CHARACTER_CONTROLLER,
    UNDEFINED
};

enum struct OpeningState {
    CLOSED,
    CLOSING,
    OPEN,
    OPENING
};

enum struct DecalType {
    GLASS,
    PLASTER
};

enum struct TrimType {
    NONE,
    TIMBER,
    PLASTER
};

enum struct WallType {
    INTERIOR,
    WEATHER_BOARDS,
    NONE
};

enum struct TreeType {
    TREE_LARGE_0 = 0,
    TREE_LARGE_1,
    TREE_LARGE_2,
    BLACK_BERRIES,
    UNDEFINED
};

enum class RendererOverrideState {
    NONE = 0,
    BASE_COLOR,
    NORMALS,
    RMA,
    CAMERA_NDOTL,
    TILE_HEATMAP,
    STATE_COUNT,
};

enum class OpenState {
    OPEN,
    OPENING,
    CLOSED,
    CLOSING
};

enum class PictureFrameType {
    BIG_LANDSCAPE,
    TALL_THIN,
    REGULAR_PORTRAIT,
    REGULAR_LANDSCAPE,
    UNDEFINED
}; 

enum class SharkMovementState { 
    STOPPED, 
    FOLLOWING_PATH, 
    FOLLOWING_PATH_ANGRY, 
    ARROW_KEYS, 
    HUNT_PLAYER 
};

enum class SharkHuntingState { 
    CHARGE_PLAYER, 
    BITING_PLAYER, 
    UNDEFINED 
};

enum class SharkMovementDirection {
    STRAIGHT,
    LEFT,
    RIGHT,
    NONE
};

enum class RaycastIgnoreFlags : uint32_t {
    NONE = 0,
    PLAYER_CHARACTER_CONTROLLERS = 1 << 0,
    PLAYER_RAGDOLLS = 1 << 1,
};

inline RaycastIgnoreFlags operator|(RaycastIgnoreFlags a, RaycastIgnoreFlags b) {
    return static_cast<RaycastIgnoreFlags>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

inline RaycastIgnoreFlags operator&(RaycastIgnoreFlags a, RaycastIgnoreFlags b) {
    return static_cast<RaycastIgnoreFlags>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}

inline RaycastIgnoreFlags& operator|=(RaycastIgnoreFlags& a, RaycastIgnoreFlags b) {
    a = a | b;
    return a;
}