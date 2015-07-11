#ifndef _Blender_h_
#define _Blender_h_
// This file has been generated with blenderdna tool.
// This file has to be re-generated every time you use a new version of Blender.
// Generated from a Blender(275) file.

#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif
#ifdef rad2
#undef rad2
#endif


namespace Blender {
/** \addtogroup Blender
*  @{
*/

struct Link;
struct LinkData;
struct ListBase;
struct vec2s;
struct vec2f;
struct vec3f;
struct rcti;
struct rctf;
struct IDPropertyData;
struct IDProperty;
struct ID;
struct Library;
struct PreviewImage;
struct IpoDriver;
struct IpoCurve;
struct Ipo;
struct KeyBlock;
struct Key;
struct TextLine;
struct Text;
struct PackedFile;
struct GPUDOFSettings;
struct GPUSSAOSettings;
struct GPUFXSettings;
struct CameraStereoSettings;
struct Camera;
struct ImageUser;
struct ImageAnim;
struct ImageView;
struct ImagePackedFile;
struct RenderSlot;
struct Image;
struct MTex;
struct CBData;
struct ColorBand;
struct EnvMap;
struct PointDensity;
struct VoxelData;
struct OceanTex;
struct Tex;
struct TexMapping;
struct ColorMapping;
struct Lamp;
struct VolumeSettings;
struct GameSettings;
struct TexPaintSlot;
struct Material;
struct VFont;
struct MetaElem;
struct MetaBall;
struct BezTriple;
struct BPoint;
struct Nurb;
struct CharInfo;
struct TextBox;
struct EditNurb;
struct Curve;
struct Mesh;
struct TFace;
struct MFace;
struct MEdge;
struct MDeformWeight;
struct MDeformVert;
struct MVert;
struct MCol;
struct MPoly;
struct MLoop;
struct MTexPoly;
struct MLoopUV;
struct MLoopCol;
struct MSelect;
struct MTFace;
struct MFloatProperty;
struct MIntProperty;
struct MStringProperty;
struct OrigSpaceFace;
struct OrigSpaceLoop;
struct MDisps;
struct MultiresCol;
struct MultiresColFace;
struct MultiresFace;
struct MultiresEdge;
struct MultiresLevel;
struct Multires;
struct MRecast;
struct GridPaintMask;
struct MVertSkin;
struct FreestyleEdge;
struct FreestyleFace;
struct ModifierData;
struct MappingInfoModifierData;
struct SubsurfModifierData;
struct LatticeModifierData;
struct CurveModifierData;
struct BuildModifierData;
struct MaskModifierData;
struct ArrayModifierData;
struct MirrorModifierData;
struct EdgeSplitModifierData;
struct BevelModifierData;
struct SmokeModifierData;
struct DisplaceModifierData;
struct UVProjectModifierData;
struct DecimateModifierData;
struct SmoothModifierData;
struct CastModifierData;
struct WaveModifierData;
struct ArmatureModifierData;
struct HookModifierData;
struct SoftbodyModifierData;
struct ClothModifierData;
struct CollisionModifierData;
struct SurfaceModifierData;
struct BooleanModifierData;
struct MDefInfluence;
struct MDefCell;
struct MeshDeformModifierData;
struct ParticleSystemModifierData;
struct ParticleInstanceModifierData;
struct ExplodeModifierData;
struct MultiresModifierData;
struct FluidsimModifierData;
struct ShrinkwrapModifierData;
struct SimpleDeformModifierData;
struct ShapeKeyModifierData;
struct SolidifyModifierData;
struct ScrewModifierData;
struct OceanModifierData;
struct WarpModifierData;
struct WeightVGEditModifierData;
struct WeightVGMixModifierData;
struct WeightVGProximityModifierData;
struct DynamicPaintModifierData;
struct RemeshModifierData;
struct SkinModifierData;
struct TriangulateModifierData;
struct LaplacianSmoothModifierData;
struct CorrectiveSmoothModifierData;
struct UVWarpModifierData;
struct MeshCacheModifierData;
struct LaplacianDeformModifierData;
struct WireframeModifierData;
struct DataTransferModifierData;
struct NormalEditModifierData;
struct EditLatt;
struct Lattice;
struct bDeformGroup;
struct BoundBox;
struct LodLevel;
struct Object;
struct ObHook;
struct DupliObject;
struct PartDeflect;
struct EffectorWeights;
struct PTCacheExtra;
struct PTCacheMem;
struct PointCache;
struct SBVertex;
struct BulletSoftBody;
struct SoftBody;
struct FluidVertexVelocity;
struct FluidsimSettings;
struct World;
struct Base;
struct AviCodecData;
struct QuicktimeCodecData;
struct QuicktimeCodecSettings;
struct FFMpegCodecData;
struct AudioData;
struct SceneRenderLayer;
struct SceneRenderView;
struct Stereo3dFormat;
struct ImageFormatData;
struct BakeData;
struct RenderData;
struct RenderProfile;
struct GameDome;
struct GameFraming;
struct RecastData;
struct GameData;
struct TimeMarker;
struct Paint;
struct ImagePaintSettings;
struct ParticleBrushData;
struct ParticleEditSettings;
struct Sculpt;
struct UvSculpt;
struct VPaint;
struct TransformOrientation;
struct UnifiedPaintSettings;
struct MeshStatVis;
struct ToolSettings;
struct bStats;
struct UnitSettings;
struct PhysicsSettings;
struct DisplaySafeAreas;
struct Scene;
struct BGpic;
struct RegionView3D;
struct View3D;
struct View2D;
struct SpaceLink;
struct SpaceInfo;
struct SpaceButs;
struct SpaceOops;
struct SpaceIpo;
struct SpaceNla;
struct SpaceTimeCache;
struct SpaceTime;
struct SpaceSeq;
struct MaskSpaceInfo;
struct FileSelectParams;
struct SpaceFile;
struct FSMenuEntry;
struct SpaceImage;
struct SpaceText;
struct Script;
struct SpaceScript;
struct bNodeTreePath;
struct SpaceNode;
struct SpaceLogic;
struct ConsoleLine;
struct SpaceConsole;
struct SpaceUserPref;
struct SpaceClip;
struct uiFont;
struct uiFontStyle;
struct uiStyle;
struct uiWidgetColors;
struct uiWidgetStateColors;
struct uiPanelColors;
struct uiGradientColors;
struct ThemeUI;
struct ThemeSpace;
struct ThemeWireColor;
struct bTheme;
struct bAddon;
struct bPathCompare;
struct SolidLight;
struct WalkNavigation;
struct UserDef;
struct bScreen;
struct ScrVert;
struct ScrEdge;
struct Panel;
struct PanelCategoryStack;
struct uiList;
struct uiPreview;
struct ScrArea;
struct ARegion;
struct FileGlobal;
struct StripAnim;
struct StripElem;
struct StripCrop;
struct StripTransform;
struct StripColorBalance;
struct StripProxy;
struct Strip;
struct Sequence;
struct MetaStack;
struct Editing;
struct WipeVars;
struct GlowVars;
struct TransformVars;
struct SolidColorVars;
struct SpeedControlVars;
struct GaussianBlurVars;
struct SequenceModifierData;
struct ColorBalanceModifierData;
struct CurvesModifierData;
struct HueCorrectModifierData;
struct BrightContrastModifierData;
struct SequencerMaskModifierData;
struct SequencerScopes;
struct Effect;
struct BuildEff;
struct PartEff;
struct WaveEff;
struct TreeStoreElem;
struct TreeStore;
struct bProperty;
struct bNearSensor;
struct bMouseSensor;
struct bTouchSensor;
struct bKeyboardSensor;
struct bPropertySensor;
struct bActuatorSensor;
struct bDelaySensor;
struct bCollisionSensor;
struct bRadarSensor;
struct bRandomSensor;
struct bRaySensor;
struct bArmatureSensor;
struct bMessageSensor;
struct bSensor;
struct bJoystickSensor;
struct bExpressionCont;
struct bPythonCont;
struct bController;
struct bAddObjectActuator;
struct bActionActuator;
struct Sound3D;
struct bSoundActuator;
struct bEditObjectActuator;
struct bSceneActuator;
struct bPropertyActuator;
struct bObjectActuator;
struct bIpoActuator;
struct bCameraActuator;
struct bConstraintActuator;
struct bGroupActuator;
struct bRandomActuator;
struct bMessageActuator;
struct bGameActuator;
struct bVisibilityActuator;
struct bTwoDFilterActuator;
struct bParentActuator;
struct bStateActuator;
struct bArmatureActuator;
struct bSteeringActuator;
struct bMouseActuator;
struct bActuator;
struct bSound;
struct GroupObject;
struct Group;
struct Bone;
struct bArmature;
struct bMotionPathVert;
struct bMotionPath;
struct bAnimVizSettings;
struct bPoseChannel;
struct bPose;
struct bIKParam;
struct bItasc;
struct bActionGroup;
struct bAction;
struct bDopeSheet;
struct SpaceAction;
struct bActionChannel;
struct bConstraintChannel;
struct bConstraint;
struct bConstraintTarget;
struct bPythonConstraint;
struct bKinematicConstraint;
struct bSplineIKConstraint;
struct bTrackToConstraint;
struct bRotateLikeConstraint;
struct bLocateLikeConstraint;
struct bSizeLikeConstraint;
struct bSameVolumeConstraint;
struct bTransLikeConstraint;
struct bMinMaxConstraint;
struct bActionConstraint;
struct bLockTrackConstraint;
struct bDampTrackConstraint;
struct bFollowPathConstraint;
struct bStretchToConstraint;
struct bRigidBodyJointConstraint;
struct bClampToConstraint;
struct bChildOfConstraint;
struct bTransformConstraint;
struct bPivotConstraint;
struct bLocLimitConstraint;
struct bRotLimitConstraint;
struct bSizeLimitConstraint;
struct bDistLimitConstraint;
struct bShrinkwrapConstraint;
struct bFollowTrackConstraint;
struct bCameraSolverConstraint;
struct bObjectSolverConstraint;
struct bActionModifier;
struct bActionStrip;
struct bNodeStack;
struct bNodeSocket;
struct bNode;
struct bNodeInstanceKey;
struct bNodeInstanceHashEntry;
struct bNodePreview;
struct bNodeLink;
struct bNodeTree;
struct bNodeSocketValueInt;
struct bNodeSocketValueFloat;
struct bNodeSocketValueBoolean;
struct bNodeSocketValueVector;
struct bNodeSocketValueRGBA;
struct bNodeSocketValueString;
struct NodeFrame;
struct NodeImageAnim;
struct ColorCorrectionData;
struct NodeColorCorrection;
struct NodeBokehImage;
struct NodeBoxMask;
struct NodeEllipseMask;
struct NodeImageLayer;
struct NodeBlurData;
struct NodeDBlurData;
struct NodeBilateralBlurData;
struct NodeHueSat;
struct NodeImageFile;
struct NodeImageMultiFile;
struct NodeImageMultiFileSocket;
struct NodeChroma;
struct NodeTwoXYs;
struct NodeTwoFloats;
struct NodeGeometry;
struct NodeVertexCol;
struct NodeDefocus;
struct NodeScriptDict;
struct NodeGlare;
struct NodeTonemap;
struct NodeLensDist;
struct NodeColorBalance;
struct NodeColorspill;
struct NodeDilateErode;
struct NodeMask;
struct NodeTexBase;
struct NodeTexSky;
struct NodeTexImage;
struct NodeTexChecker;
struct NodeTexBrick;
struct NodeTexEnvironment;
struct NodeTexGradient;
struct NodeTexNoise;
struct NodeTexVoronoi;
struct NodeTexMusgrave;
struct NodeTexWave;
struct NodeTexMagic;
struct NodeShaderAttribute;
struct NodeShaderVectTransform;
struct TexNodeOutput;
struct NodeKeyingScreenData;
struct NodeKeyingData;
struct NodeTrackPosData;
struct NodeTranslateData;
struct NodePlaneTrackDeformData;
struct NodeShaderScript;
struct NodeShaderTangent;
struct NodeShaderNormalMap;
struct NodeShaderUVMap;
struct NodeSunBeams;
struct CurveMapPoint;
struct CurveMap;
struct CurveMapping;
struct Histogram;
struct Scopes;
struct ColorManagedViewSettings;
struct ColorManagedDisplaySettings;
struct ColorManagedColorspaceSettings;
struct BrushClone;
struct Brush;
struct PaletteColor;
struct Palette;
struct PaintCurvePoint;
struct PaintCurve;
struct CustomDataLayer;
struct CustomDataExternal;
struct CustomData;
struct HairKey;
struct ParticleKey;
struct BoidParticle;
struct ParticleSpring;
struct ChildParticle;
struct ParticleTarget;
struct ParticleDupliWeight;
struct ParticleData;
struct SPHFluidSettings;
struct ParticleSettings;
struct ParticleSystem;
struct ClothSimSettings;
struct ClothCollSettings;
struct bGPDspoint;
struct bGPDstroke;
struct bGPDframe;
struct bGPDlayer;
struct bGPdata;
struct ReportList;
struct wmWindowManager;
struct wmWindow;
struct wmKeyMapItem;
struct wmKeyMapDiffItem;
struct wmKeyMap;
struct wmKeyConfig;
struct wmOperator;
struct FModifier;
struct FMod_Generator;
struct FMod_FunctionGenerator;
struct FCM_EnvelopeData;
struct FMod_Envelope;
struct FMod_Cycles;
struct FMod_Python;
struct FMod_Limits;
struct FMod_Noise;
struct FMod_Stepped;
struct DriverTarget;
struct DriverVar;
struct ChannelDriver;
struct FPoint;
struct FCurve;
struct AnimMapPair;
struct AnimMapper;
struct NlaStrip;
struct NlaTrack;
struct KS_Path;
struct KeyingSet;
struct AnimOverride;
struct AnimData;
struct IdAdtTemplate;
struct BoidRule;
struct BoidRuleGoalAvoid;
struct BoidRuleAvoidCollision;
struct BoidRuleFollowLeader;
struct BoidRuleAverageSpeed;
struct BoidRuleFight;
struct BoidData;
struct BoidState;
struct BoidSettings;
struct SmokeDomainSettings;
struct SmokeFlowSettings;
struct SmokeCollSettings;
struct Speaker;
struct MovieClipUser;
struct MovieClipProxy;
struct MovieClip;
struct MovieClipScopes;
struct MovieReconstructedCamera;
struct MovieTrackingCamera;
struct MovieTrackingMarker;
struct MovieTrackingTrack;
struct MovieTrackingPlaneMarker;
struct MovieTrackingPlaneTrack;
struct MovieTrackingSettings;
struct MovieTrackingStabilization;
struct MovieTrackingReconstruction;
struct MovieTrackingObject;
struct MovieTrackingStats;
struct MovieTrackingDopesheetChannel;
struct MovieTrackingDopesheetCoverageSegment;
struct MovieTrackingDopesheet;
struct MovieTracking;
struct DynamicPaintSurface;
struct DynamicPaintCanvasSettings;
struct DynamicPaintBrushSettings;
struct Mask;
struct MaskParent;
struct MaskSplinePointUW;
struct MaskSplinePoint;
struct MaskSpline;
struct MaskLayerShape;
struct MaskLayer;
struct RigidBodyWorld;
struct RigidBodyOb;
struct RigidBodyCon;
struct FreestyleLineSet;
struct FreestyleModuleConfig;
struct FreestyleConfig;
struct LineStyleModifier;
struct LineStyleColorModifier_AlongStroke;
struct LineStyleAlphaModifier_AlongStroke;
struct LineStyleThicknessModifier_AlongStroke;
struct LineStyleColorModifier_DistanceFromCamera;
struct LineStyleAlphaModifier_DistanceFromCamera;
struct LineStyleThicknessModifier_DistanceFromCamera;
struct LineStyleColorModifier_DistanceFromObject;
struct LineStyleAlphaModifier_DistanceFromObject;
struct LineStyleThicknessModifier_DistanceFromObject;
struct LineStyleColorModifier_Material;
struct LineStyleAlphaModifier_Material;
struct LineStyleThicknessModifier_Material;
struct LineStyleGeometryModifier_Sampling;
struct LineStyleGeometryModifier_BezierCurve;
struct LineStyleGeometryModifier_SinusDisplacement;
struct LineStyleGeometryModifier_SpatialNoise;
struct LineStyleGeometryModifier_PerlinNoise1D;
struct LineStyleGeometryModifier_PerlinNoise2D;
struct LineStyleGeometryModifier_BackboneStretcher;
struct LineStyleGeometryModifier_TipRemover;
struct LineStyleGeometryModifier_Polygonalization;
struct LineStyleGeometryModifier_GuidingLines;
struct LineStyleGeometryModifier_Blueprint;
struct LineStyleGeometryModifier_2DOffset;
struct LineStyleGeometryModifier_2DTransform;
struct LineStyleThicknessModifier_Calligraphy;
struct FreestyleLineStyle;



// 0 DNA structure 12, 2
struct Link
{
    Link    *next;// 1 type, name 12, 0.
    Link    *prev;// 2 type, name 12, 1.
};

// 1 DNA structure 13, 3
struct LinkData
{
    LinkData    *next;// 1 type, name 13, 0.
    LinkData    *prev;// 2 type, name 13, 1.
    void        *data;// 3 type, name 11, 2.
};

// 2 DNA structure 14, 2
struct ListBase
{
    void    *first;// 1 type, name 11, 3.
    void    *last; // 2 type, name 11, 4.
};

// 3 DNA structure 15, 2
struct vec2s
{
    short    x;// 1 type, name 2, 5.
    short    y;// 2 type, name 2, 6.
};

// 4 DNA structure 16, 2
struct vec2f
{
    float    x;// 1 type, name 7, 5.
    float    y;// 2 type, name 7, 6.
};

// 5 DNA structure 17, 3
struct vec3f
{
    float    x;// 1 type, name 7, 5.
    float    y;// 2 type, name 7, 6.
    float    z;// 3 type, name 7, 7.
};

// 6 DNA structure 18, 4
struct rcti
{
    int    xmin;// 1 type, name 4, 8.
    int    xmax;// 2 type, name 4, 9.
    int    ymin;// 3 type, name 4, 10.
    int    ymax;// 4 type, name 4, 11.
};

// 7 DNA structure 19, 4
struct rctf
{
    float    xmin;// 1 type, name 7, 8.
    float    xmax;// 2 type, name 7, 9.
    float    ymin;// 3 type, name 7, 10.
    float    ymax;// 4 type, name 7, 11.
};

// 8 DNA structure 22, 10
struct ID
{
    void          *next;      // 1 type, name 11, 0.
    void          *prev;      // 2 type, name 11, 1.
    ID            *newid;     // 3 type, name 22, 24.
    Library       *lib;       // 4 type, name 23, 25.
    char          name[66];   // 5 type, name 0, 26.
    short         flag;       // 6 type, name 2, 18.
    int           us;         // 7 type, name 4, 27.
    int           icon_id;    // 8 type, name 4, 28.
    int           pad2;       // 9 type, name 4, 29.
    IDProperty    *properties;// 10 type, name 21, 30.
};

// 9 DNA structure 26, 9
struct PreviewImage
{
    int           w[2];                // 1 type, name 4, 38.
    int           h[2];                // 2 type, name 4, 39.
    short         flag[2];             // 3 type, name 2, 40.
    short         changed_timestamp[2];// 4 type, name 2, 41.
    int           *rect[2];            // 5 type, name 4, 42.
    void          *gputexture[2];      // Note: using void* on undefined DNA type: GPUTexture
    int           icon_id;             // 7 type, name 4, 28.
    char          pad[3];              // 8 type, name 0, 44.
    char          use_deferred;        // 9 type, name 0, 45.
};

// 10 DNA structure 28, 6
struct IpoDriver
{
    Object    *ob;      // 1 type, name 29, 46.
    short     blocktype;// 2 type, name 2, 47.
    short     adrcode;  // 3 type, name 2, 48.
    short     type;     // 4 type, name 2, 16.
    short     flag;     // 5 type, name 2, 18.
    char      name[128];// 6 type, name 0, 49.
};

// 11 DNA structure 34, 15
struct KeyBlock
{
    KeyBlock    *next;     // 1 type, name 34, 0.
    KeyBlock    *prev;     // 2 type, name 34, 1.
    float       pos;       // 3 type, name 7, 69.
    float       curval;    // 4 type, name 7, 62.
    short       type;      // 5 type, name 2, 16.
    short       pad1;      // 6 type, name 2, 70.
    short       relative;  // 7 type, name 2, 71.
    short       flag;      // 8 type, name 2, 18.
    int         totelem;   // 9 type, name 4, 72.
    int         uid;       // 10 type, name 4, 73.
    void        *data;     // 11 type, name 11, 2.
    char        name[64];  // 12 type, name 0, 19.
    char        vgroup[64];// 13 type, name 0, 74.
    float       slidermin; // 14 type, name 7, 75.
    float       slidermax; // 15 type, name 7, 76.
};

// 12 DNA structure 37, 6
struct TextLine
{
    TextLine    *next;  // 1 type, name 37, 0.
    TextLine    *prev;  // 2 type, name 37, 1.
    char        *line;  // 3 type, name 0, 87.
    char        *format;// 4 type, name 0, 88.
    int         len;    // 5 type, name 4, 22.
    int         blen;   // 6 type, name 4, 89.
};

// 13 DNA structure 25, 3
struct PackedFile
{
    int     size; // 1 type, name 4, 103.
    int     seek; // 2 type, name 4, 104.
    void    *data;// 3 type, name 11, 2.
};

// 14 DNA structure 39, 6
struct GPUDOFSettings
{
    float    focus_distance;// 1 type, name 7, 105.
    float    fstop;         // 2 type, name 7, 106.
    float    focal_length;  // 3 type, name 7, 107.
    float    sensor;        // 4 type, name 7, 108.
    int      num_blades;    // 5 type, name 4, 109.
    int      high_quality;  // 6 type, name 4, 110.
};

// 15 DNA structure 40, 6
struct GPUSSAOSettings
{
    float    factor;      // 1 type, name 7, 111.
    float    color[3];    // 2 type, name 7, 112.
    float    distance_max;// 3 type, name 7, 113.
    float    attenuation; // 4 type, name 7, 114.
    int      samples;     // 5 type, name 4, 115.
    int      pad;         // 6 type, name 4, 68.
};

// 16 DNA structure 41, 4
struct GPUFXSettings
{
    GPUDOFSettings     *dof;   // 1 type, name 39, 116.
    GPUSSAOSettings    *ssao;  // 2 type, name 40, 117.
    char               fx_flag;// 3 type, name 0, 118.
    char               pad[7]; // 4 type, name 0, 119.
};

// 17 DNA structure 42, 6
struct CameraStereoSettings
{
    float    interocular_distance;// 1 type, name 7, 120.
    float    convergence_distance;// 2 type, name 7, 121.
    short    convergence_mode;    // 3 type, name 2, 122.
    short    pivot;               // 4 type, name 2, 123.
    short    pad;                 // 5 type, name 2, 68.
    short    pad2;                // 6 type, name 2, 29.
};

// 18 DNA structure 44, 14
struct ImageUser
{
    Scene    *scene;       // 1 type, name 45, 140.
    int      framenr;      // 2 type, name 4, 141.
    int      frames;       // 3 type, name 4, 142.
    int      offset;       // 4 type, name 4, 143.
    int      sfra;         // 5 type, name 4, 144.
    char     fie_ima;      // 6 type, name 0, 145.
    char     cycl;         // 7 type, name 0, 146.
    char     ok;           // 8 type, name 0, 147.
    char     multiview_eye;// 9 type, name 0, 148.
    int      passtype;     // 10 type, name 4, 149.
    short    multi_index;  // 11 type, name 2, 150.
    short    view;         // 12 type, name 2, 151.
    short    layer;        // 13 type, name 2, 152.
    short    flag;         // 14 type, name 2, 18.
};

// 19 DNA structure 46, 3
struct ImageAnim
{
    ImageAnim    *next;// 1 type, name 46, 0.
    ImageAnim    *prev;// 2 type, name 46, 1.
    void         *anim;// Note: using void* on undefined DNA type: anim
};

// 20 DNA structure 48, 4
struct ImageView
{
    ImageView    *next;         // 1 type, name 48, 0.
    ImageView    *prev;         // 2 type, name 48, 1.
    char         name[64];      // 3 type, name 0, 19.
    char         filepath[1024];// 4 type, name 0, 35.
};

// 21 DNA structure 49, 4
struct ImagePackedFile
{
    ImagePackedFile    *next;         // 1 type, name 49, 0.
    ImagePackedFile    *prev;         // 2 type, name 49, 1.
    PackedFile         *packedfile;   // 3 type, name 25, 37.
    char               filepath[1024];// 4 type, name 0, 35.
};

// 22 DNA structure 50, 1
struct RenderSlot
{
    char    name[64];// 1 type, name 0, 19.
};

// 23 DNA structure 56, 69
struct MTex
{
    short     texco;           // 1 type, name 2, 192.
    short     mapto;           // 2 type, name 2, 193.
    short     maptoneg;        // 3 type, name 2, 194.
    short     blendtype;       // 4 type, name 2, 195.
    Object    *object;         // 5 type, name 29, 196.
    Tex       *tex;            // 6 type, name 57, 197.
    char      uvname[64];      // 7 type, name 0, 198.
    char      projx;           // 8 type, name 0, 199.
    char      projy;           // 9 type, name 0, 200.
    char      projz;           // 10 type, name 0, 201.
    char      mapping;         // 11 type, name 0, 202.
    char      brush_map_mode;  // 12 type, name 0, 203.
    char      brush_angle_mode;// 13 type, name 0, 204.
    char      pad[2];          // 14 type, name 0, 205.
    float     ofs[3];          // 15 type, name 7, 206.
    float     size[3];         // 16 type, name 7, 207.
    float     rot;             // 17 type, name 7, 208.
    float     random_angle;    // 18 type, name 7, 209.
    short     texflag;         // 19 type, name 2, 210.
    short     colormodel;      // 20 type, name 2, 211.
    short     pmapto;          // 21 type, name 2, 212.
    short     pmaptoneg;       // 22 type, name 2, 213.
    short     normapspace;     // 23 type, name 2, 214.
    short     which_output;    // 24 type, name 2, 215.
    float     r;               // 25 type, name 7, 216.
    float     g;               // 26 type, name 7, 217.
    float     b;               // 27 type, name 7, 218.
    float     k;               // 28 type, name 7, 219.
    float     def_var;         // 29 type, name 7, 220.
    float     rt;              // 30 type, name 7, 58.
    float     colfac;          // 31 type, name 7, 221.
    float     varfac;          // 32 type, name 7, 222.
    float     norfac;          // 33 type, name 7, 223.
    float     dispfac;         // 34 type, name 7, 224.
    float     warpfac;         // 35 type, name 7, 225.
    float     colspecfac;      // 36 type, name 7, 226.
    float     mirrfac;         // 37 type, name 7, 227.
    float     alphafac;        // 38 type, name 7, 228.
    float     difffac;         // 39 type, name 7, 229.
    float     specfac;         // 40 type, name 7, 230.
    float     emitfac;         // 41 type, name 7, 231.
    float     hardfac;         // 42 type, name 7, 232.
    float     raymirrfac;      // 43 type, name 7, 233.
    float     translfac;       // 44 type, name 7, 234.
    float     ambfac;          // 45 type, name 7, 235.
    float     colemitfac;      // 46 type, name 7, 236.
    float     colreflfac;      // 47 type, name 7, 237.
    float     coltransfac;     // 48 type, name 7, 238.
    float     densfac;         // 49 type, name 7, 239.
    float     scatterfac;      // 50 type, name 7, 240.
    float     reflfac;         // 51 type, name 7, 241.
    float     timefac;         // 52 type, name 7, 242.
    float     lengthfac;       // 53 type, name 7, 243.
    float     clumpfac;        // 54 type, name 7, 244.
    float     dampfac;         // 55 type, name 7, 245.
    float     kinkfac;         // 56 type, name 7, 246.
    float     kinkampfac;      // 57 type, name 7, 247.
    float     roughfac;        // 58 type, name 7, 248.
    float     padensfac;       // 59 type, name 7, 249.
    float     gravityfac;      // 60 type, name 7, 250.
    float     lifefac;         // 61 type, name 7, 251.
    float     sizefac;         // 62 type, name 7, 252.
    float     ivelfac;         // 63 type, name 7, 253.
    float     fieldfac;        // 64 type, name 7, 254.
    int       pad2;            // 65 type, name 4, 29.
    float     shadowfac;       // 66 type, name 7, 255.
    float     zenupfac;        // 67 type, name 7, 256.
    float     zendownfac;      // 68 type, name 7, 257.
    float     blendfac;        // 69 type, name 7, 258.
};

// 24 DNA structure 58, 6
struct CBData
{
    float    r;  // 1 type, name 7, 216.
    float    g;  // 2 type, name 7, 217.
    float    b;  // 3 type, name 7, 218.
    float    a;  // 4 type, name 7, 259.
    float    pos;// 5 type, name 7, 69.
    int      cur;// 6 type, name 4, 65.
};

// 25 DNA structure 60, 17
struct EnvMap
{
    Object    *object;     // 1 type, name 29, 196.
    Image     *ima;        // 2 type, name 51, 266.
    void      *cube[6];    // Note: using void* on undefined DNA type: ImBuf
    float     imat[4][4];  // 4 type, name 7, 268.
    float     obimat[3][3];// 5 type, name 7, 269.
    short     type;        // 6 type, name 2, 16.
    short     stype;       // 7 type, name 2, 270.
    float     clipsta;     // 8 type, name 7, 126.
    float     clipend;     // 9 type, name 7, 127.
    float     viewscale;   // 10 type, name 7, 271.
    int       notlay;      // 11 type, name 4, 272.
    short     cuberes;     // 12 type, name 2, 273.
    short     depth;       // 13 type, name 2, 274.
    int       ok;          // 14 type, name 4, 147.
    int       lastframe;   // 15 type, name 4, 162.
    short     recalc;      // 16 type, name 2, 275.
    short     lastsize;    // 17 type, name 2, 276.
};

// 26 DNA structure 62, 25
struct PointDensity
{
    short           flag;               // 1 type, name 2, 18.
    short           falloff_type;       // 2 type, name 2, 277.
    float           falloff_softness;   // 3 type, name 7, 278.
    float           radius;             // 4 type, name 7, 279.
    short           source;             // 5 type, name 2, 161.
    short           color_source;       // 6 type, name 2, 280.
    int             totpoints;          // 7 type, name 4, 281.
    int             pdpad;              // 8 type, name 4, 282.
    Object          *object;            // 9 type, name 29, 196.
    int             psys;               // 10 type, name 4, 283.
    short           psys_cache_space;   // 11 type, name 2, 284.
    short           ob_cache_space;     // 12 type, name 2, 285.
    void            *point_tree;        // 13 type, name 11, 286.
    float           *point_data;        // 14 type, name 7, 287.
    float           noise_size;         // 15 type, name 7, 288.
    short           noise_depth;        // 16 type, name 2, 289.
    short           noise_influence;    // 17 type, name 2, 290.
    short           noise_basis;        // 18 type, name 2, 291.
    short           pdpad3[3];          // 19 type, name 2, 292.
    float           noise_fac;          // 20 type, name 7, 293.
    float           speed_scale;        // 21 type, name 7, 294.
    float           falloff_speed_scale;// 22 type, name 7, 295.
    float           pdpad2;             // 23 type, name 7, 296.
    ColorBand       *coba;              // 24 type, name 59, 297.
    CurveMapping    *falloff_curve;     // 25 type, name 63, 298.
};

// 27 DNA structure 64, 16
struct VoxelData
{
    int       resol[3];         // 1 type, name 4, 299.
    int       interp_type;      // 2 type, name 4, 300.
    short     file_format;      // 3 type, name 2, 301.
    short     flag;             // 4 type, name 2, 18.
    short     extend;           // 5 type, name 2, 302.
    short     smoked_type;      // 6 type, name 2, 303.
    short     hair_type;        // 7 type, name 2, 304.
    short     data_type;        // 8 type, name 2, 305.
    int       _pad;             // 9 type, name 4, 306.
    Object    *object;          // 10 type, name 29, 196.
    float     int_multiplier;   // 11 type, name 7, 307.
    int       still_frame;      // 12 type, name 4, 308.
    char      source_path[1024];// 13 type, name 0, 309.
    float     *dataset;         // 14 type, name 7, 310.
    int       cachedframe;      // 15 type, name 4, 311.
    int       ok;               // 16 type, name 4, 147.
};

// 28 DNA structure 65, 4
struct OceanTex
{
    Object    *object;     // 1 type, name 29, 196.
    char      oceanmod[64];// 2 type, name 0, 312.
    int       output;      // 3 type, name 4, 313.
    int       pad;         // 4 type, name 4, 68.
};

// 29 DNA structure 67, 13
struct TexMapping
{
    float     loc[3];   // 1 type, name 7, 359.
    float     rot[3];   // 2 type, name 7, 360.
    float     size[3];  // 3 type, name 7, 207.
    int       flag;     // 4 type, name 4, 18.
    char      projx;    // 5 type, name 0, 199.
    char      projy;    // 6 type, name 0, 200.
    char      projz;    // 7 type, name 0, 201.
    char      mapping;  // 8 type, name 0, 202.
    int       type;     // 9 type, name 4, 16.
    float     mat[4][4];// 10 type, name 7, 361.
    float     min[3];   // 11 type, name 7, 362.
    float     max[3];   // 12 type, name 7, 363.
    Object    *ob;      // 13 type, name 29, 46.
};

// 30 DNA structure 70, 18
struct VolumeSettings
{
    float    density;            // 1 type, name 7, 426.
    float    emission;           // 2 type, name 7, 427.
    float    scattering;         // 3 type, name 7, 428.
    float    reflection;         // 4 type, name 7, 429.
    float    emission_col[3];    // 5 type, name 7, 430.
    float    transmission_col[3];// 6 type, name 7, 431.
    float    reflection_col[3];  // 7 type, name 7, 432.
    float    density_scale;      // 8 type, name 7, 433.
    float    depth_cutoff;       // 9 type, name 7, 434.
    float    asymmetry;          // 10 type, name 7, 435.
    short    stepsize_type;      // 11 type, name 2, 436.
    short    shadeflag;          // 12 type, name 2, 437.
    short    shade_type;         // 13 type, name 2, 438.
    short    precache_resolution;// 14 type, name 2, 439.
    float    stepsize;           // 15 type, name 7, 440.
    float    ms_diff;            // 16 type, name 7, 441.
    float    ms_intensity;       // 17 type, name 7, 442.
    float    ms_spread;          // 18 type, name 7, 443.
};

// 31 DNA structure 71, 4
struct GameSettings
{
    int    flag;            // 1 type, name 4, 18.
    int    alpha_blend;     // 2 type, name 4, 444.
    int    face_orientation;// 3 type, name 4, 445.
    int    pad1;            // 4 type, name 4, 70.
};

// 32 DNA structure 72, 4
struct TexPaintSlot
{
    Image    *ima;   // 1 type, name 51, 266.
    char     *uvname;// 2 type, name 0, 446.
    int      index;  // 3 type, name 4, 447.
    int      pad;    // 4 type, name 4, 68.
};

// 33 DNA structure 77, 20
struct MetaElem
{
    MetaElem    *next;  // 1 type, name 77, 0.
    MetaElem    *prev;  // 2 type, name 77, 1.
    BoundBox    *bb;    // 3 type, name 78, 567.
    short       type;   // 4 type, name 2, 16.
    short       flag;   // 5 type, name 2, 18.
    short       selcol1;// 6 type, name 2, 568.
    short       selcol2;// 7 type, name 2, 569.
    float       x;      // 8 type, name 7, 5.
    float       y;      // 9 type, name 7, 6.
    float       z;      // 10 type, name 7, 7.
    float       quat[4];// 11 type, name 7, 570.
    float       expx;   // 12 type, name 7, 571.
    float       expy;   // 13 type, name 7, 572.
    float       expz;   // 14 type, name 7, 573.
    float       rad;    // 15 type, name 7, 574.
    float       rad2;   // 16 type, name 7, 575.
    float       s;      // 17 type, name 7, 576.
    float       len;    // 18 type, name 7, 22.
    float       *mat;   // 19 type, name 7, 577.
    float       *imat;  // 20 type, name 7, 578.
};

// 34 DNA structure 32, 16
struct BezTriple
{
    float    vec[3][3];// 1 type, name 7, 589.
    float    alfa;     // 2 type, name 7, 590.
    float    weight;   // 3 type, name 7, 591.
    float    radius;   // 4 type, name 7, 279.
    char     ipo;      // 5 type, name 0, 56.
    char     h1;       // 6 type, name 0, 592.
    char     h2;       // 7 type, name 0, 593.
    char     f1;       // 8 type, name 0, 594.
    char     f2;       // 9 type, name 0, 595.
    char     f3;       // 10 type, name 0, 596.
    char     hide;     // 11 type, name 0, 597.
    char     easing;   // 12 type, name 0, 598.
    float    back;     // 13 type, name 7, 599.
    float    amplitude;// 14 type, name 7, 600.
    float    period;   // 15 type, name 7, 601.
    char     pad[4];   // 16 type, name 0, 602.
};

// 35 DNA structure 31, 7
struct BPoint
{
    float    vec[4];// 1 type, name 7, 603.
    float    alfa;  // 2 type, name 7, 590.
    float    weight;// 3 type, name 7, 591.
    short    f1;    // 4 type, name 2, 594.
    short    hide;  // 5 type, name 2, 597.
    float    radius;// 6 type, name 7, 279.
    float    pad;   // 7 type, name 7, 68.
};

// 36 DNA structure 80, 22
struct Nurb
{
    Nurb         *next;        // 1 type, name 80, 0.
    Nurb         *prev;        // 2 type, name 80, 1.
    short        type;         // 3 type, name 2, 16.
    short        mat_nr;       // 4 type, name 2, 604.
    short        hide;         // 5 type, name 2, 597.
    short        flag;         // 6 type, name 2, 18.
    int          pntsu;        // 7 type, name 4, 605.
    int          pntsv;        // 8 type, name 4, 606.
    short        pad[2];       // 9 type, name 2, 205.
    short        resolu;       // 10 type, name 2, 607.
    short        resolv;       // 11 type, name 2, 608.
    short        orderu;       // 12 type, name 2, 609.
    short        orderv;       // 13 type, name 2, 610.
    short        flagu;        // 14 type, name 2, 611.
    short        flagv;        // 15 type, name 2, 612.
    float        *knotsu;      // 16 type, name 7, 613.
    float        *knotsv;      // 17 type, name 7, 614.
    BPoint       *bp;          // 18 type, name 31, 50.
    BezTriple    *bezt;        // 19 type, name 32, 51.
    short        tilt_interp;  // 20 type, name 2, 615.
    short        radius_interp;// 21 type, name 2, 616.
    int          charidx;      // 22 type, name 4, 617.
};

// 37 DNA structure 81, 5
struct CharInfo
{
    short    kern;  // 1 type, name 2, 618.
    short    mat_nr;// 2 type, name 2, 604.
    char     flag;  // 3 type, name 0, 18.
    char     pad;   // 4 type, name 0, 68.
    short    pad2;  // 5 type, name 2, 29.
};

// 38 DNA structure 82, 4
struct TextBox
{
    float    x;// 1 type, name 7, 5.
    float    y;// 2 type, name 7, 6.
    float    w;// 3 type, name 7, 619.
    float    h;// 4 type, name 7, 620.
};

// 39 DNA structure 96, 8
struct TFace
{
    void     *tpage;  // 1 type, name 11, 707.
    float    uv[4][2];// 2 type, name 7, 708.
    int      col[4];  // 3 type, name 4, 709.
    char     flag;    // 4 type, name 0, 18.
    char     transp;  // 5 type, name 0, 710.
    short    mode;    // 6 type, name 2, 368.
    short    tile;    // 7 type, name 2, 711.
    short    unwrap;  // 8 type, name 2, 712.
};

// 40 DNA structure 94, 7
struct MFace
{
    int      v1;    // 1 type, name 4, 713.
    int      v2;    // 2 type, name 4, 714.
    int      v3;    // 3 type, name 4, 715.
    int      v4;    // 4 type, name 4, 716.
    short    mat_nr;// 5 type, name 2, 604.
    char     edcode;// 6 type, name 0, 717.
    char     flag;  // 7 type, name 0, 18.
};

// 41 DNA structure 98, 5
struct MEdge
{
    int      v1;     // 1 type, name 4, 713.
    int      v2;     // 2 type, name 4, 714.
    char     crease; // 3 type, name 0, 718.
    char     bweight;// 4 type, name 0, 719.
    short    flag;   // 5 type, name 2, 18.
};

// 42 DNA structure 104, 2
struct MDeformWeight
{
    int      def_nr;// 1 type, name 4, 720.
    float    weight;// 2 type, name 7, 591.
};

// 43 DNA structure 99, 3
struct MDeformVert
{
    MDeformWeight    *dw;      // 1 type, name 104, 721.
    int              totweight;// 2 type, name 4, 722.
    int              flag;     // 3 type, name 4, 18.
};

// 44 DNA structure 97, 4
struct MVert
{
    float    co[3];  // 1 type, name 7, 723.
    short    no[3];  // 2 type, name 2, 724.
    char     flag;   // 3 type, name 0, 18.
    char     bweight;// 4 type, name 0, 719.
};

// 45 DNA structure 100, 4
struct MCol
{
    char    a;// 1 type, name 0, 259.
    char    r;// 2 type, name 0, 216.
    char    g;// 3 type, name 0, 217.
    char    b;// 4 type, name 0, 218.
};

// 46 DNA structure 89, 5
struct MPoly
{
    int      loopstart;// 1 type, name 4, 725.
    int      totloop;  // 2 type, name 4, 698.
    short    mat_nr;   // 3 type, name 2, 604.
    char     flag;     // 4 type, name 0, 18.
    char     pad;      // 5 type, name 0, 68.
};

// 47 DNA structure 91, 2
struct MLoop
{
    int    v;// 1 type, name 4, 726.
    int    e;// 2 type, name 4, 727.
};

// 48 DNA structure 90, 6
struct MTexPoly
{
    Image    *tpage;// 1 type, name 51, 707.
    char     flag;  // 2 type, name 0, 18.
    char     transp;// 3 type, name 0, 710.
    short    mode;  // 4 type, name 2, 368.
    short    tile;  // 5 type, name 2, 711.
    short    pad;   // 6 type, name 2, 68.
};

// 49 DNA structure 92, 2
struct MLoopUV
{
    float    uv[2];// 1 type, name 7, 728.
    int      flag; // 2 type, name 4, 18.
};

// 50 DNA structure 93, 4
struct MLoopCol
{
    char    r;// 1 type, name 0, 216.
    char    g;// 2 type, name 0, 217.
    char    b;// 3 type, name 0, 218.
    char    a;// 4 type, name 0, 259.
};

// 51 DNA structure 88, 2
struct MSelect
{
    int    index;// 1 type, name 4, 447.
    int    type; // 2 type, name 4, 16.
};

// 52 DNA structure 95, 7
struct MTFace
{
    float    uv[4][2];// 1 type, name 7, 708.
    Image    *tpage;  // 2 type, name 51, 707.
    char     flag;    // 3 type, name 0, 18.
    char     transp;  // 4 type, name 0, 710.
    short    mode;    // 5 type, name 2, 368.
    short    tile;    // 6 type, name 2, 711.
    short    unwrap;  // 7 type, name 2, 712.
};

// 53 DNA structure 105, 1
struct MFloatProperty
{
    float    f;// 1 type, name 7, 729.
};

// 54 DNA structure 106, 1
struct MIntProperty
{
    int    i;// 1 type, name 4, 730.
};

// 55 DNA structure 107, 2
struct MStringProperty
{
    char    s[255];// 1 type, name 0, 731.
    char    s_len; // 2 type, name 0, 732.
};

// 56 DNA structure 108, 1
struct OrigSpaceFace
{
    float    uv[4][2];// 1 type, name 7, 708.
};

// 57 DNA structure 109, 1
struct OrigSpaceLoop
{
    float    uv[2];// 1 type, name 7, 728.
};

// 58 DNA structure 110, 4
struct MDisps
{
    int      totdisp;   // 1 type, name 4, 733.
    int      level;     // 2 type, name 4, 734.
    float    (*disps)();// 3 type, name 7, 735.
    int      *hidden;   // 4 type, name 4, 736.
};

// 59 DNA structure 111, 4
struct MultiresCol
{
    float    a;// 1 type, name 7, 259.
    float    r;// 2 type, name 7, 216.
    float    g;// 3 type, name 7, 217.
    float    b;// 4 type, name 7, 218.
};

// 60 DNA structure 113, 5
struct MultiresFace
{
    int     v[4];  // 1 type, name 4, 737.
    int     mid;   // 2 type, name 4, 738.
    char    flag;  // 3 type, name 0, 18.
    char    mat_nr;// 4 type, name 0, 604.
    char    pad[2];// 5 type, name 0, 205.
};

// 61 DNA structure 114, 2
struct MultiresEdge
{
    int    v[2];// 1 type, name 4, 739.
    int    mid; // 2 type, name 4, 738.
};

// 62 DNA structure 115, 10
struct MultiresLevel
{
    MultiresLevel      *next;    // 1 type, name 115, 0.
    MultiresLevel      *prev;    // 2 type, name 115, 1.
    MultiresFace       *faces;   // 3 type, name 113, 740.
    MultiresColFace    *colfaces;// 4 type, name 112, 741.
    MultiresEdge       *edges;   // 5 type, name 114, 742.
    int                totvert;  // 6 type, name 4, 55.
    int                totface;  // 7 type, name 4, 695.
    int                totedge;  // 8 type, name 4, 694.
    int                pad;      // 9 type, name 4, 68.
    MVert              *verts;   // 10 type, name 97, 743.
};

// 63 DNA structure 116, 1
struct MRecast
{
    int    i;// 1 type, name 4, 730.
};

// 64 DNA structure 117, 3
struct GridPaintMask
{
    float    *data;// 1 type, name 7, 2.
    int      level;// 2 type, name 4, 734.
    int      pad;  // 3 type, name 4, 68.
};

// 65 DNA structure 118, 2
struct MVertSkin
{
    float    radius[3];// 1 type, name 7, 754.
    int      flag;     // 2 type, name 4, 18.
};

// 66 DNA structure 119, 2
struct FreestyleEdge
{
    char    flag;  // 1 type, name 0, 18.
    char    pad[3];// 2 type, name 0, 44.
};

// 67 DNA structure 120, 2
struct FreestyleFace
{
    char    flag;  // 1 type, name 0, 18.
    char    pad[3];// 2 type, name 0, 44.
};

// 68 DNA structure 121, 9
struct ModifierData
{
    ModifierData    *next;     // 1 type, name 121, 0.
    ModifierData    *prev;     // 2 type, name 121, 1.
    int             type;      // 3 type, name 4, 16.
    int             mode;      // 4 type, name 4, 368.
    int             stackindex;// 5 type, name 4, 755.
    int             pad;       // 6 type, name 4, 68.
    char            name[64];  // 7 type, name 0, 19.
    Scene           *scene;    // 8 type, name 45, 140.
    char            *error;    // 9 type, name 0, 756.
};

// 69 DNA structure 158, 2
struct MDefInfluence
{
    int      vertex;// 1 type, name 4, 865.
    float    weight;// 2 type, name 7, 591.
};

// 70 DNA structure 159, 2
struct MDefCell
{
    int    offset;      // 1 type, name 4, 143.
    int    totinfluence;// 2 type, name 4, 866.
};

// 71 DNA structure 194, 3
struct EditLatt
{
    Lattice    *latt;  // 1 type, name 195, 1023.
    int        shapenr;// 2 type, name 4, 623.
    char       pad[4]; // 3 type, name 0, 602.
};

// 72 DNA structure 196, 5
struct bDeformGroup
{
    bDeformGroup    *next;   // 1 type, name 196, 0.
    bDeformGroup    *prev;   // 2 type, name 196, 1.
    char            name[64];// 3 type, name 0, 19.
    char            flag;    // 4 type, name 0, 18.
    char            pad[7];  // 5 type, name 0, 119.
};

// 73 DNA structure 78, 3
struct BoundBox
{
    float    vec[8][3];// 1 type, name 7, 1040.
    int      flag;     // 2 type, name 4, 18.
    int      pad;      // 3 type, name 4, 68.
};

// 74 DNA structure 197, 7
struct LodLevel
{
    LodLevel    *next;       // 1 type, name 197, 0.
    LodLevel    *prev;       // 2 type, name 197, 1.
    Object      *source;     // 3 type, name 29, 1041.
    int         flags;       // 4 type, name 4, 91.
    float       distance;    // 5 type, name 7, 1042.
    float       pad;         // 6 type, name 7, 68.
    int         obhysteresis;// 7 type, name 4, 1043.
};

// 75 DNA structure 210, 14
struct ObHook
{
    ObHook    *next;          // 1 type, name 210, 0.
    ObHook    *prev;          // 2 type, name 210, 1.
    Object    *parent;        // 3 type, name 29, 36.
    float     parentinv[4][4];// 4 type, name 7, 833.
    float     mat[4][4];      // 5 type, name 7, 361.
    float     cent[3];        // 6 type, name 7, 834.
    float     falloff;        // 7 type, name 7, 826.
    char      name[64];       // 8 type, name 0, 19.
    int       *indexar;       // 9 type, name 4, 835.
    int       totindex;       // 10 type, name 4, 836.
    int       curindex;       // 11 type, name 4, 1154.
    short     type;           // 12 type, name 2, 16.
    short     active;         // 13 type, name 2, 1155.
    float     force;          // 14 type, name 7, 837.
};

// 76 DNA structure 211, 11
struct DupliObject
{
    DupliObject       *next;           // 1 type, name 211, 0.
    DupliObject       *prev;           // 2 type, name 211, 1.
    Object            *ob;             // 3 type, name 29, 46.
    float             mat[4][4];       // 4 type, name 7, 361.
    float             orco[3];         // 5 type, name 7, 1156.
    float             uv[2];           // 6 type, name 7, 728.
    short             type;            // 7 type, name 2, 16.
    char              no_draw;         // 8 type, name 0, 1157.
    char              animated;        // 9 type, name 0, 1158.
    int               persistent_id[8];// 10 type, name 4, 1159.
    ParticleSystem    *particle_system;// 11 type, name 162, 1160.
};

// 77 DNA structure 205, 41
struct PartDeflect
{
    int       flag;          // 1 type, name 4, 18.
    short     deflect;       // 2 type, name 2, 1161.
    short     forcefield;    // 3 type, name 2, 1162.
    short     falloff;       // 4 type, name 2, 826.
    short     shape;         // 5 type, name 2, 1163.
    short     tex_mode;      // 6 type, name 2, 1164.
    short     kink;          // 7 type, name 2, 1165.
    short     kink_axis;     // 8 type, name 2, 1166.
    short     zdir;          // 9 type, name 2, 1167.
    float     f_strength;    // 10 type, name 7, 1168.
    float     f_damp;        // 11 type, name 7, 1169.
    float     f_flow;        // 12 type, name 7, 1170.
    float     f_size;        // 13 type, name 7, 1171.
    float     f_power;       // 14 type, name 7, 1172.
    float     maxdist;       // 15 type, name 7, 1173.
    float     mindist;       // 16 type, name 7, 1174.
    float     f_power_r;     // 17 type, name 7, 1175.
    float     maxrad;        // 18 type, name 7, 1176.
    float     minrad;        // 19 type, name 7, 1177.
    float     pdef_damp;     // 20 type, name 7, 1178.
    float     pdef_rdamp;    // 21 type, name 7, 1179.
    float     pdef_perm;     // 22 type, name 7, 1180.
    float     pdef_frict;    // 23 type, name 7, 1181.
    float     pdef_rfrict;   // 24 type, name 7, 1182.
    float     pdef_stickness;// 25 type, name 7, 1183.
    float     absorption;    // 26 type, name 7, 1184.
    float     pdef_sbdamp;   // 27 type, name 7, 1185.
    float     pdef_sbift;    // 28 type, name 7, 1186.
    float     pdef_sboft;    // 29 type, name 7, 1187.
    float     clump_fac;     // 30 type, name 7, 1188.
    float     clump_pow;     // 31 type, name 7, 1189.
    float     kink_freq;     // 32 type, name 7, 1190.
    float     kink_shape;    // 33 type, name 7, 1191.
    float     kink_amp;      // 34 type, name 7, 1192.
    float     free_end;      // 35 type, name 7, 1193.
    float     tex_nabla;     // 36 type, name 7, 1194.
    Tex       *tex;          // 37 type, name 57, 197.
    void      *rng;          // Note: using void* on undefined DNA type: RNG
    float     f_noise;       // 39 type, name 7, 1196.
    int       seed;          // 40 type, name 4, 773.
    Object    *f_source;     // 41 type, name 29, 1197.
};

// 78 DNA structure 213, 6
struct EffectorWeights
{
    Group    *group;        // 1 type, name 74, 537.
    float    weight[14];    // 2 type, name 7, 1198.
    float    global_gravity;// 3 type, name 7, 1199.
    short    flag;          // 4 type, name 2, 18.
    short    rt[3];         // 5 type, name 2, 1200.
    int      pad;           // 6 type, name 4, 68.
};

// 79 DNA structure 214, 5
struct PTCacheExtra
{
    PTCacheExtra    *next;  // 1 type, name 214, 0.
    PTCacheExtra    *prev;  // 2 type, name 214, 1.
    int             type;   // 3 type, name 4, 16.
    int             totdata;// 4 type, name 4, 1201.
    void            *data;  // 5 type, name 11, 2.
};

// 80 DNA structure 217, 1
struct SBVertex
{
    float    vec[4];// 1 type, name 7, 603.
};

// 81 DNA structure 204, 30
struct BulletSoftBody
{
    int      flag;                // 1 type, name 4, 18.
    float    linStiff;            // 2 type, name 7, 1222.
    float    angStiff;            // 3 type, name 7, 1223.
    float    volume;              // 4 type, name 7, 1224.
    int      viterations;         // 5 type, name 4, 1225.
    int      piterations;         // 6 type, name 4, 1226.
    int      diterations;         // 7 type, name 4, 1227.
    int      citerations;         // 8 type, name 4, 1228.
    float    kSRHR_CL;            // 9 type, name 7, 1229.
    float    kSKHR_CL;            // 10 type, name 7, 1230.
    float    kSSHR_CL;            // 11 type, name 7, 1231.
    float    kSR_SPLT_CL;         // 12 type, name 7, 1232.
    float    kSK_SPLT_CL;         // 13 type, name 7, 1233.
    float    kSS_SPLT_CL;         // 14 type, name 7, 1234.
    float    kVCF;                // 15 type, name 7, 1235.
    float    kDP;                 // 16 type, name 7, 1236.
    float    kDG;                 // 17 type, name 7, 1237.
    float    kLF;                 // 18 type, name 7, 1238.
    float    kPR;                 // 19 type, name 7, 1239.
    float    kVC;                 // 20 type, name 7, 1240.
    float    kDF;                 // 21 type, name 7, 1241.
    float    kMT;                 // 22 type, name 7, 1242.
    float    kCHR;                // 23 type, name 7, 1243.
    float    kKHR;                // 24 type, name 7, 1244.
    float    kSHR;                // 25 type, name 7, 1245.
    float    kAHR;                // 26 type, name 7, 1246.
    int      collisionflags;      // 27 type, name 4, 1247.
    int      numclusteriterations;// 28 type, name 4, 1248.
    float    welding;             // 29 type, name 7, 1249.
    float    margin;              // 30 type, name 7, 1101.
};

// 82 DNA structure 221, 1
struct FluidVertexVelocity
{
    float    vel[3];// 1 type, name 7, 1299.
};

// 83 DNA structure 167, 54
struct FluidsimSettings
{
    FluidsimModifierData    *fmd;                 // 1 type, name 166, 1300.
    int                     threads;              // 2 type, name 4, 1301.
    int                     pad1;                 // 3 type, name 4, 70.
    short                   type;                 // 4 type, name 2, 16.
    short                   show_advancedoptions; // 5 type, name 2, 1302.
    short                   resolutionxyz;        // 6 type, name 2, 1303.
    short                   previewresxyz;        // 7 type, name 2, 1304.
    float                   realsize;             // 8 type, name 7, 1305.
    short                   guiDisplayMode;       // 9 type, name 2, 1306.
    short                   renderDisplayMode;    // 10 type, name 2, 1307.
    float                   viscosityValue;       // 11 type, name 7, 1308.
    short                   viscosityMode;        // 12 type, name 2, 1309.
    short                   viscosityExponent;    // 13 type, name 2, 1310.
    float                   grav[3];              // 14 type, name 7, 1311.
    float                   animStart;            // 15 type, name 7, 1312.
    float                   animEnd;              // 16 type, name 7, 1313.
    int                     bakeStart;            // 17 type, name 4, 1314.
    int                     bakeEnd;              // 18 type, name 4, 1315.
    int                     frameOffset;          // 19 type, name 4, 1316.
    int                     pad2;                 // 20 type, name 4, 29.
    float                   gstar;                // 21 type, name 7, 1317.
    int                     maxRefine;            // 22 type, name 4, 1318.
    float                   iniVelx;              // 23 type, name 7, 1319.
    float                   iniVely;              // 24 type, name 7, 1320.
    float                   iniVelz;              // 25 type, name 7, 1321.
    Mesh                    *orgMesh;             // 26 type, name 87, 1322.
    Mesh                    *meshBB;              // 27 type, name 87, 1323.
    char                    surfdataPath[1024];   // 28 type, name 0, 1324.
    float                   bbStart[3];           // 29 type, name 7, 1325.
    float                   bbSize[3];            // 30 type, name 7, 1326.
    Ipo                     *ipo;                 // 31 type, name 33, 82.
    short                   typeFlags;            // 32 type, name 2, 1327.
    char                    domainNovecgen;       // 33 type, name 0, 1328.
    char                    volumeInitType;       // 34 type, name 0, 1329.
    float                   partSlipValue;        // 35 type, name 7, 1330.
    int                     generateTracers;      // 36 type, name 4, 1331.
    float                   generateParticles;    // 37 type, name 7, 1332.
    float                   surfaceSmoothing;     // 38 type, name 7, 1333.
    int                     surfaceSubdivs;       // 39 type, name 4, 1334.
    int                     flag;                 // 40 type, name 4, 18.
    float                   particleInfSize;      // 41 type, name 7, 1335.
    float                   particleInfAlpha;     // 42 type, name 7, 1336.
    float                   farFieldSize;         // 43 type, name 7, 1337.
    FluidVertexVelocity     *meshVelocities;      // 44 type, name 221, 1338.
    int                     totvert;              // 45 type, name 4, 55.
    float                   cpsTimeStart;         // 46 type, name 7, 1339.
    float                   cpsTimeEnd;           // 47 type, name 7, 1340.
    float                   cpsQuality;           // 48 type, name 7, 1341.
    float                   attractforceStrength; // 49 type, name 7, 1342.
    float                   attractforceRadius;   // 50 type, name 7, 1343.
    float                   velocityforceStrength;// 51 type, name 7, 1344.
    float                   velocityforceRadius;  // 52 type, name 7, 1345.
    int                     lastgoodframe;        // 53 type, name 4, 1346.
    float                   animRate;             // 54 type, name 7, 1347.
};

// 84 DNA structure 223, 8
struct Base
{
    Base      *next;  // 1 type, name 223, 0.
    Base      *prev;  // 2 type, name 223, 1.
    int       lay;    // 3 type, name 4, 1080.
    int       selcol; // 4 type, name 4, 1407.
    int       flag;   // 5 type, name 4, 18.
    short     sx;     // 6 type, name 2, 1408.
    short     sy;     // 7 type, name 2, 1409.
    Object    *object;// 8 type, name 29, 196.
};

// 85 DNA structure 224, 13
struct AviCodecData
{
    void    *lpFormat;        // 1 type, name 11, 1410.
    void    *lpParms;         // 2 type, name 11, 1411.
    int     cbFormat;         // 3 type, name 4, 1412.
    int     cbParms;          // 4 type, name 4, 1413.
    int     fccType;          // 5 type, name 4, 1414.
    int     fccHandler;       // 6 type, name 4, 1415.
    int     dwKeyFrameEvery;  // 7 type, name 4, 1416.
    int     dwQuality;        // 8 type, name 4, 1417.
    int     dwBytesPerSecond; // 9 type, name 4, 1418.
    int     dwFlags;          // 10 type, name 4, 1419.
    int     dwInterleaveEvery;// 11 type, name 4, 1420.
    int     pad;              // 12 type, name 4, 68.
    char    avicodecname[128];// 13 type, name 0, 1421.
};

// 86 DNA structure 225, 5
struct QuicktimeCodecData
{
    void    *cdParms;        // 1 type, name 11, 1422.
    void    *pad;            // 2 type, name 11, 1423.
    int     cdSize;          // 3 type, name 4, 1424.
    int     pad2;            // 4 type, name 4, 29.
    char    qtcodecname[128];// 5 type, name 0, 1425.
};

// 87 DNA structure 226, 17
struct QuicktimeCodecSettings
{
    int      codecType;           // 1 type, name 4, 1426.
    int      codecSpatialQuality; // 2 type, name 4, 1427.
    int      codec;               // 3 type, name 4, 1428.
    int      codecFlags;          // 4 type, name 4, 1429.
    int      colorDepth;          // 5 type, name 4, 1430.
    int      codecTemporalQuality;// 6 type, name 4, 1431.
    int      minSpatialQuality;   // 7 type, name 4, 1432.
    int      minTemporalQuality;  // 8 type, name 4, 1433.
    int      keyFrameRate;        // 9 type, name 4, 1434.
    int      bitRate;             // 10 type, name 4, 1435.
    int      audiocodecType;      // 11 type, name 4, 1436.
    int      audioSampleRate;     // 12 type, name 4, 1437.
    short    audioBitDepth;       // 13 type, name 2, 1438.
    short    audioChannels;       // 14 type, name 2, 1439.
    int      audioCodecFlags;     // 15 type, name 4, 1440.
    int      audioBitRate;        // 16 type, name 4, 1441.
    int      pad1;                // 17 type, name 4, 70.
};

// 88 DNA structure 227, 17
struct FFMpegCodecData
{
    int           type;           // 1 type, name 4, 16.
    int           codec;          // 2 type, name 4, 1428.
    int           audio_codec;    // 3 type, name 4, 1442.
    int           video_bitrate;  // 4 type, name 4, 1443.
    int           audio_bitrate;  // 5 type, name 4, 1444.
    int           audio_mixrate;  // 6 type, name 4, 1445.
    int           audio_channels; // 7 type, name 4, 1446.
    int           audio_pad;      // 8 type, name 4, 1447.
    float         audio_volume;   // 9 type, name 7, 1448.
    int           gop_size;       // 10 type, name 4, 1449.
    int           flags;          // 11 type, name 4, 91.
    int           rc_min_rate;    // 12 type, name 4, 1450.
    int           rc_max_rate;    // 13 type, name 4, 1451.
    int           rc_buffer_size; // 14 type, name 4, 1452.
    int           mux_packet_size;// 15 type, name 4, 1453.
    int           mux_rate;       // 16 type, name 4, 1454.
    IDProperty    *properties;    // 17 type, name 21, 30.
};

// 89 DNA structure 228, 9
struct AudioData
{
    int      mixrate;       // 1 type, name 4, 1455.
    float    main;          // 2 type, name 7, 1456.
    float    speed_of_sound;// 3 type, name 7, 1457.
    float    doppler_factor;// 4 type, name 7, 1458.
    int      distance_model;// 5 type, name 4, 1459.
    short    flag;          // 6 type, name 2, 18.
    short    pad;           // 7 type, name 2, 68.
    float    volume;        // 8 type, name 7, 1224.
    float    pad2;          // 9 type, name 7, 29.
};

// 90 DNA structure 231, 7
struct SceneRenderView
{
    SceneRenderView    *next;     // 1 type, name 231, 0.
    SceneRenderView    *prev;     // 2 type, name 231, 1.
    char               name[64];  // 3 type, name 0, 19.
    char               suffix[64];// 4 type, name 0, 1469.
    int                viewflag;  // 5 type, name 4, 1470.
    int                pad[2];    // 6 type, name 4, 205.
    char               pad2[4];   // 7 type, name 0, 986.
};

// 91 DNA structure 55, 5
struct Stereo3dFormat
{
    short    flag;          // 1 type, name 2, 18.
    char     display_mode;  // 2 type, name 0, 1471.
    char     anaglyph_type; // 3 type, name 0, 1472.
    char     interlace_type;// 4 type, name 0, 1473.
    char     pad[3];        // 5 type, name 0, 44.
};

// 92 DNA structure 237, 9
struct RenderProfile
{
    RenderProfile    *next;            // 1 type, name 237, 0.
    RenderProfile    *prev;            // 2 type, name 237, 1.
    char             name[32];         // 3 type, name 0, 1601.
    short            particle_perc;    // 4 type, name 2, 1602.
    short            subsurf_max;      // 5 type, name 2, 1603.
    short            shadbufsample_max;// 6 type, name 2, 1604.
    short            pad1;             // 7 type, name 2, 70.
    float            ao_error;         // 8 type, name 7, 1605.
    float            pad2;             // 9 type, name 7, 29.
};

// 93 DNA structure 238, 7
struct GameDome
{
    short    res;      // 1 type, name 2, 790.
    short    mode;     // 2 type, name 2, 368.
    short    angle;    // 3 type, name 2, 814.
    short    tilt;     // 4 type, name 2, 1606.
    float    resbuf;   // 5 type, name 7, 1607.
    float    pad2;     // 6 type, name 7, 29.
    Text     *warptext;// 7 type, name 38, 1608.
};

// 94 DNA structure 239, 5
struct GameFraming
{
    float    col[3];// 1 type, name 7, 1609.
    char     type;  // 2 type, name 0, 16.
    char     pad1;  // 3 type, name 0, 70.
    char     pad2;  // 4 type, name 0, 29.
    char     pad3;  // 5 type, name 0, 534.
};

// 95 DNA structure 240, 15
struct RecastData
{
    float    cellsize;            // 1 type, name 7, 1610.
    float    cellheight;          // 2 type, name 7, 1611.
    float    agentmaxslope;       // 3 type, name 7, 1612.
    float    agentmaxclimb;       // 4 type, name 7, 1613.
    float    agentheight;         // 5 type, name 7, 1614.
    float    agentradius;         // 6 type, name 7, 1615.
    float    edgemaxlen;          // 7 type, name 7, 1616.
    float    edgemaxerror;        // 8 type, name 7, 1617.
    float    regionminsize;       // 9 type, name 7, 1618.
    float    regionmergesize;     // 10 type, name 7, 1619.
    int      vertsperpoly;        // 11 type, name 4, 1620.
    float    detailsampledist;    // 12 type, name 7, 1621.
    float    detailsamplemaxerror;// 13 type, name 7, 1622.
    short    pad1;                // 14 type, name 2, 70.
    short    pad2;                // 15 type, name 2, 29.
};

// 96 DNA structure 242, 6
struct TimeMarker
{
    TimeMarker    *next;   // 1 type, name 242, 0.
    TimeMarker    *prev;   // 2 type, name 242, 1.
    int           frame;   // 3 type, name 4, 1202.
    char          name[64];// 4 type, name 0, 19.
    int           flag;    // 5 type, name 4, 18.
    Object        *camera; // 6 type, name 29, 1643.
};

// 97 DNA structure 243, 8
struct Paint
{
    Brush           *brush;             // 1 type, name 244, 970.
    Palette         *palette;           // 2 type, name 245, 1644.
    CurveMapping    *cavity_curve;      // 3 type, name 63, 1645.
    void            *paint_cursor;      // 4 type, name 11, 1646.
    char            paint_cursor_col[4];// 5 type, name 0, 1647.
    int             flags;              // 6 type, name 4, 91.
    int             num_input_samples;  // 7 type, name 4, 1648.
    int             symmetry_flags;     // 8 type, name 4, 1649.
};

// 98 DNA structure 247, 6
struct ParticleBrushData
{
    short    size;    // 1 type, name 2, 103.
    short    step;    // 2 type, name 2, 1207.
    short    invert;  // 3 type, name 2, 1660.
    short    count;   // 4 type, name 2, 784.
    int      flag;    // 5 type, name 4, 18.
    float    strength;// 6 type, name 7, 767.
};

// 99 DNA structure 252, 5
struct TransformOrientation
{
    TransformOrientation    *next;    // 1 type, name 252, 0.
    TransformOrientation    *prev;    // 2 type, name 252, 1.
    char                    name[64]; // 3 type, name 0, 19.
    float                   mat[3][3];// 4 type, name 7, 1681.
    int                     pad;      // 5 type, name 4, 68.
};

// 100 DNA structure 253, 26
struct UnifiedPaintSettings
{
    int           size;                     // 1 type, name 4, 103.
    float         unprojected_radius;       // 2 type, name 7, 1682.
    float         alpha;                    // 3 type, name 7, 463.
    float         weight;                   // 4 type, name 7, 591.
    float         rgb[3];                   // 5 type, name 7, 1683.
    float         secondary_rgb[3];         // 6 type, name 7, 1684.
    int           flag;                     // 7 type, name 4, 18.
    float         last_rake[2];             // 8 type, name 7, 1685.
    float         last_rake_angle;          // 9 type, name 7, 1686.
    int           last_stroke_valid;        // 10 type, name 4, 1687.
    float         average_stroke_accum[3];  // 11 type, name 7, 1688.
    int           average_stroke_counter;   // 12 type, name 4, 1689.
    float         brush_rotation;           // 13 type, name 7, 1690.
    float         brush_rotation_sec;       // 14 type, name 7, 1691.
    int           anchored_size;            // 15 type, name 4, 1692.
    float         overlap_factor;           // 16 type, name 7, 1693.
    char          draw_inverted;            // 17 type, name 0, 1694.
    char          stroke_active;            // 18 type, name 0, 1695.
    char          draw_anchored;            // 19 type, name 0, 1696.
    char          do_linear_conversion;     // 20 type, name 0, 1697.
    float         anchored_initial_mouse[2];// 21 type, name 7, 1698.
    float         pixel_radius;             // 22 type, name 7, 1699.
    float         size_pressure_value;      // 23 type, name 7, 1700.
    float         tex_mouse[2];             // 24 type, name 7, 1701.
    float         mask_tex_mouse[2];        // 25 type, name 7, 1702.
    void          *colorspace;              // Note: using void* on undefined DNA type: ColorSpace
};

// 101 DNA structure 255, 13
struct MeshStatVis
{
    char     type;             // 1 type, name 0, 16.
    char     _pad1[2];         // 2 type, name 0, 1704.
    char     overhang_axis;    // 3 type, name 0, 1705.
    float    overhang_min;     // 4 type, name 7, 1706.
    float    overhang_max;     // 5 type, name 7, 1707.
    float    thickness_min;    // 6 type, name 7, 1708.
    float    thickness_max;    // 7 type, name 7, 1709.
    char     thickness_samples;// 8 type, name 0, 1710.
    char     _pad2[3];         // 9 type, name 0, 1711.
    float    distort_min;      // 10 type, name 7, 1712.
    float    distort_max;      // 11 type, name 7, 1713.
    float    sharp_min;        // 12 type, name 7, 1714.
    float    sharp_max;        // 13 type, name 7, 1715.
};

// 102 DNA structure 257, 8
struct bStats
{
    int    totobj;     // 1 type, name 4, 1792.
    int    totlamp;    // 2 type, name 4, 1793.
    int    totobjsel;  // 3 type, name 4, 1794.
    int    totcurve;   // 4 type, name 4, 1795.
    int    totmesh;    // 5 type, name 4, 1796.
    int    totarmature;// 6 type, name 4, 1797.
    int    totvert;    // 7 type, name 4, 55.
    int    totface;    // 8 type, name 4, 695.
};

// 103 DNA structure 258, 4
struct UnitSettings
{
    float    scale_length;   // 1 type, name 7, 1798.
    char     system;         // 2 type, name 0, 1799.
    char     system_rotation;// 3 type, name 0, 1800.
    short    flag;           // 4 type, name 2, 18.
};

// 104 DNA structure 259, 4
struct PhysicsSettings
{
    float    gravity[3];      // 1 type, name 7, 1801.
    int      flag;            // 2 type, name 4, 18.
    int      quick_cache_step;// 3 type, name 4, 1802.
    int      rt;              // 4 type, name 4, 58.
};

// 105 DNA structure 260, 4
struct DisplaySafeAreas
{
    float    title[2];        // 1 type, name 7, 1803.
    float    action[2];       // 2 type, name 7, 1804.
    float    title_center[2]; // 3 type, name 7, 1805.
    float    action_center[2];// 4 type, name 7, 1806.
};

// 106 DNA structure 269, 42
struct RegionView3D
{
    float                winmat[4][4];      // 1 type, name 7, 1846.
    float                viewmat[4][4];     // 2 type, name 7, 1847.
    float                viewinv[4][4];     // 3 type, name 7, 1848.
    float                persmat[4][4];     // 4 type, name 7, 1849.
    float                persinv[4][4];     // 5 type, name 7, 1850.
    float                viewcamtexcofac[4];// 6 type, name 7, 1851.
    float                viewmatob[4][4];   // 7 type, name 7, 1852.
    float                persmatob[4][4];   // 8 type, name 7, 1853.
    float                clip[6][4];        // 9 type, name 7, 1854.
    float                clip_local[6][4];  // 10 type, name 7, 1855.
    BoundBox             *clipbb;           // 11 type, name 78, 1856.
    RegionView3D         *localvd;          // 12 type, name 269, 1857.
    void                 *render_engine;    // Note: using void* on undefined DNA type: RenderEngine
    void                 *depths;           // Note: using void* on undefined DNA type: ViewDepths
    void                 *gpuoffscreen;     // 15 type, name 11, 1860.
    void                 *sms;              // Note: using void* on undefined DNA type: SmoothView3DStore
    void                 *smooth_timer;     // Note: using void* on undefined DNA type: wmTimer
    float                twmat[4][4];       // 18 type, name 7, 1863.
    float                viewquat[4];       // 19 type, name 7, 1864.
    float                dist;              // 20 type, name 7, 375.
    float                camdx;             // 21 type, name 7, 1865.
    float                camdy;             // 22 type, name 7, 1866.
    float                pixsize;           // 23 type, name 7, 1867.
    float                ofs[3];            // 24 type, name 7, 206.
    float                camzoom;           // 25 type, name 7, 1868.
    char                 is_persp;          // 26 type, name 0, 1869.
    char                 persp;             // 27 type, name 0, 1870.
    char                 view;              // 28 type, name 0, 151.
    char                 viewlock;          // 29 type, name 0, 1871.
    char                 viewlock_quad;     // 30 type, name 0, 1872.
    char                 pad[3];            // 31 type, name 0, 44.
    float                ofs_lock[2];       // 32 type, name 7, 1873.
    short                twdrawflag;        // 33 type, name 2, 1874.
    short                rflag;             // 34 type, name 2, 1875.
    float                lviewquat[4];      // 35 type, name 7, 1876.
    short                lpersp;            // 36 type, name 2, 1877.
    short                lview;             // 37 type, name 2, 1878.
    float                gridview;          // 38 type, name 7, 1879.
    float                tw_idot[3];        // 39 type, name 7, 1880.
    float                rot_angle;         // 40 type, name 7, 1881.
    float                rot_axis[3];       // 41 type, name 7, 1882.
    void                 *compositor;       // Note: using void* on undefined DNA type: GPUFX
};

// 107 DNA structure 287, 3
struct SpaceTimeCache
{
    SpaceTimeCache    *next; // 1 type, name 287, 0.
    SpaceTimeCache    *prev; // 2 type, name 287, 1.
    float             *array;// 3 type, name 7, 1972.
};

// 108 DNA structure 291, 5
struct MaskSpaceInfo
{
    Mask    *mask;       // 1 type, name 292, 1983.
    char    draw_flag;   // 2 type, name 0, 1980.
    char    draw_type;   // 3 type, name 0, 1984.
    char    overlay_mode;// 4 type, name 0, 1985.
    char    pad3[5];     // 5 type, name 0, 1986.
};

// 109 DNA structure 293, 20
struct FileSelectParams
{
    char     title[96];        // 1 type, name 0, 1987.
    char     dir[1090];        // 2 type, name 0, 1988.
    char     pad_c1[2];        // 3 type, name 0, 1989.
    char     file[256];        // 4 type, name 0, 1990.
    char     renamefile[256];  // 5 type, name 0, 1991.
    char     renameedit[256];  // 6 type, name 0, 1992.
    char     filter_glob[64];  // 7 type, name 0, 1993.
    char     filter_search[64];// 8 type, name 0, 1994.
    int      active_file;      // 9 type, name 4, 1995.
    int      sel_first;        // 10 type, name 4, 1996.
    int      sel_last;         // 11 type, name 4, 1997.
    short    thumbnail_size;   // 12 type, name 2, 1998.
    short    pad;              // 13 type, name 2, 68.
    short    type;             // 14 type, name 2, 16.
    short    flag;             // 15 type, name 2, 18.
    short    sort;             // 16 type, name 2, 1999.
    short    display;          // 17 type, name 2, 2000.
    short    filter;           // 18 type, name 2, 475.
    short    f_fp;             // 19 type, name 2, 2001.
    char     fp_str[8];        // 20 type, name 0, 2002.
};

// 110 DNA structure 298, 6
struct FSMenuEntry
{
    FSMenuEntry    *next;    // 1 type, name 298, 0.
    char           *path;    // 2 type, name 0, 1955.
    char           name[256];// 3 type, name 0, 2015.
    short          save;     // 4 type, name 2, 2016.
    short          valid;    // 5 type, name 2, 2017.
    short          pad[2];   // 6 type, name 2, 205.
};

// 111 DNA structure 309, 7
struct ConsoleLine
{
    ConsoleLine    *next;    // 1 type, name 309, 0.
    ConsoleLine    *prev;    // 2 type, name 309, 1.
    int            len_alloc;// 3 type, name 4, 2077.
    int            len;      // 4 type, name 4, 22.
    char           *line;    // 5 type, name 0, 87.
    int            cursor;   // 6 type, name 4, 2078.
    int            type;     // 7 type, name 4, 16.
};

// 112 DNA structure 314, 7
struct uiFont
{
    uiFont    *next;         // 1 type, name 314, 0.
    uiFont    *prev;         // 2 type, name 314, 1.
    char      filename[1024];// 3 type, name 0, 2095.
    short     blf_id;        // 4 type, name 2, 2096.
    short     uifont_id;     // 5 type, name 2, 2097.
    short     r_to_l;        // 6 type, name 2, 2098.
    short     hinting;       // 7 type, name 2, 2099.
};

// 113 DNA structure 315, 12
struct uiFontStyle
{
    short    uifont_id;  // 1 type, name 2, 2097.
    short    points;     // 2 type, name 2, 2100.
    short    kerning;    // 3 type, name 2, 2101.
    char     pad[6];     // 4 type, name 0, 769.
    short    italic;     // 5 type, name 2, 2102.
    short    bold;       // 6 type, name 2, 2103.
    short    shadow;     // 7 type, name 2, 2104.
    short    shadx;      // 8 type, name 2, 2105.
    short    shady;      // 9 type, name 2, 2106.
    short    align;      // 10 type, name 2, 1938.
    float    shadowalpha;// 11 type, name 7, 2107.
    float    shadowcolor;// 12 type, name 7, 2108.
};

// 114 DNA structure 317, 10
struct uiWidgetColors
{
    char     outline[4];  // 1 type, name 0, 2123.
    char     inner[4];    // 2 type, name 0, 2124.
    char     inner_sel[4];// 3 type, name 0, 2125.
    char     item[4];     // 4 type, name 0, 2126.
    char     text[4];     // 5 type, name 0, 2127.
    char     text_sel[4]; // 6 type, name 0, 2128.
    short    shaded;      // 7 type, name 2, 2129.
    short    shadetop;    // 8 type, name 2, 2130.
    short    shadedown;   // 9 type, name 2, 2131.
    short    alpha_check; // 10 type, name 2, 2132.
};

// 115 DNA structure 318, 8
struct uiWidgetStateColors
{
    char     inner_anim[4];      // 1 type, name 0, 2133.
    char     inner_anim_sel[4];  // 2 type, name 0, 2134.
    char     inner_key[4];       // 3 type, name 0, 2135.
    char     inner_key_sel[4];   // 4 type, name 0, 2136.
    char     inner_driven[4];    // 5 type, name 0, 2137.
    char     inner_driven_sel[4];// 6 type, name 0, 2138.
    float    blend;              // 7 type, name 7, 1844.
    float    pad;                // 8 type, name 7, 68.
};

// 116 DNA structure 319, 5
struct uiPanelColors
{
    char     header[4];  // 1 type, name 0, 2139.
    char     back[4];    // 2 type, name 0, 2140.
    short    show_header;// 3 type, name 2, 2141.
    short    show_back;  // 4 type, name 2, 2142.
    int      pad;        // 5 type, name 4, 68.
};

// 117 DNA structure 320, 4
struct uiGradientColors
{
    char    gradient[4];     // 1 type, name 0, 2143.
    char    high_gradient[4];// 2 type, name 0, 2144.
    int     show_grad;       // 3 type, name 4, 2145.
    int     pad2;            // 4 type, name 4, 29.
};

// 118 DNA structure 323, 5
struct ThemeWireColor
{
    char     solid[4]; // 1 type, name 0, 2356.
    char     select[4];// 2 type, name 0, 2200.
    char     active[4];// 3 type, name 0, 2205.
    short    flag;     // 4 type, name 2, 18.
    short    pad;      // 5 type, name 2, 68.
};

// 119 DNA structure 325, 4
struct bAddon
{
    bAddon        *next;     // 1 type, name 325, 0.
    bAddon        *prev;     // 2 type, name 325, 1.
    char          module[64];// 3 type, name 0, 2377.
    IDProperty    *prop;     // 4 type, name 21, 2378.
};

// 120 DNA structure 326, 5
struct bPathCompare
{
    bPathCompare    *next;    // 1 type, name 326, 0.
    bPathCompare    *prev;    // 2 type, name 326, 1.
    char            path[768];// 3 type, name 0, 2379.
    char            flag;     // 4 type, name 0, 18.
    char            pad[7];   // 5 type, name 0, 119.
};

// 121 DNA structure 327, 5
struct SolidLight
{
    int      flag;   // 1 type, name 4, 18.
    int      pad;    // 2 type, name 4, 68.
    float    col[4]; // 3 type, name 7, 709.
    float    spec[4];// 4 type, name 7, 2380.
    float    vec[4]; // 5 type, name 7, 603.
};

// 122 DNA structure 328, 8
struct WalkNavigation
{
    float    mouse_speed;      // 1 type, name 7, 2381.
    float    walk_speed;       // 2 type, name 7, 2382.
    float    walk_speed_factor;// 3 type, name 7, 2383.
    float    view_height;      // 4 type, name 7, 2384.
    float    jump_height;      // 5 type, name 7, 2385.
    float    teleport_time;    // 6 type, name 7, 2386.
    short    flag;             // 7 type, name 2, 18.
    short    pad[3];           // 8 type, name 2, 44.
};

// 123 DNA structure 332, 7
struct ScrEdge
{
    ScrEdge    *next; // 1 type, name 332, 0.
    ScrEdge    *prev; // 2 type, name 332, 1.
    ScrVert    *v1;   // 3 type, name 331, 2517.
    ScrVert    *v2;   // 4 type, name 331, 2518.
    short      border;// 5 type, name 2, 1538.
    short      flag;  // 6 type, name 2, 18.
    int        pad;   // 7 type, name 4, 68.
};

// 124 DNA structure 333, 20
struct Panel
{
    Panel        *next;        // 1 type, name 333, 0.
    Panel        *prev;        // 2 type, name 333, 1.
    void         *type;        // Note: using void* on undefined DNA type: PanelType
    void         *layout;      // Note: using void* on undefined DNA type: uiLayout
    char         panelname[64];// 5 type, name 0, 2520.
    char         tabname[64];  // 6 type, name 0, 2521.
    char         drawname[64]; // 7 type, name 0, 2522.
    int          ofsx;         // 8 type, name 4, 2523.
    int          ofsy;         // 9 type, name 4, 2524.
    int          sizex;        // 10 type, name 4, 2525.
    int          sizey;        // 11 type, name 4, 2526.
    short        labelofs;     // 12 type, name 2, 2527.
    short        pad;          // 13 type, name 2, 68.
    short        flag;         // 14 type, name 2, 18.
    short        runtime_flag; // 15 type, name 2, 2528.
    short        control;      // 16 type, name 2, 2529.
    short        snap;         // 17 type, name 2, 2530.
    int          sortorder;    // 18 type, name 4, 2531.
    Panel        *paneltab;    // 19 type, name 333, 2532.
    void         *activedata;  // 20 type, name 11, 2533.
};

// 125 DNA structure 336, 3
struct PanelCategoryStack
{
    PanelCategoryStack    *next;     // 1 type, name 336, 0.
    PanelCategoryStack    *prev;     // 2 type, name 336, 1.
    char                  idname[64];// 3 type, name 0, 2534.
};

// 126 DNA structure 337, 15
struct uiList
{
    uiList        *next;            // 1 type, name 337, 0.
    uiList        *prev;            // 2 type, name 337, 1.
    void          *type;            // Note: using void* on undefined DNA type: uiListType
    char          list_id[64];      // 4 type, name 0, 2535.
    int           layout_type;      // 5 type, name 4, 2536.
    int           flag;             // 6 type, name 4, 18.
    int           list_scroll;      // 7 type, name 4, 2537.
    int           list_grip;        // 8 type, name 4, 2538.
    int           list_last_len;    // 9 type, name 4, 2539.
    int           list_last_activei;// 10 type, name 4, 2540.
    char          filter_byname[64];// 11 type, name 0, 2541.
    int           filter_flag;      // 12 type, name 4, 2542.
    int           filter_sort_flag; // 13 type, name 4, 2543.
    IDProperty    *properties;      // 14 type, name 21, 30.
    void          *dyn_data;        // Note: using void* on undefined DNA type: uiListDyn
};

// 127 DNA structure 340, 5
struct uiPreview
{
    uiPreview    *next;         // 1 type, name 340, 0.
    uiPreview    *prev;         // 2 type, name 340, 1.
    char         preview_id[64];// 3 type, name 0, 2545.
    short        height;        // 4 type, name 2, 822.
    short        pad1[3];       // 5 type, name 2, 2546.
};

// 128 DNA structure 345, 12
struct FileGlobal
{
    char        subvstr[4];            // 1 type, name 0, 2573.
    short       subversion;            // 2 type, name 2, 2574.
    short       minversion;            // 3 type, name 2, 2575.
    short       minsubversion;         // 4 type, name 2, 2576.
    char        pad[6];                // 5 type, name 0, 769.
    bScreen     *curscreen;            // 6 type, name 330, 2577.
    Scene       *curscene;             // 7 type, name 45, 2578.
    int         fileflags;             // 8 type, name 4, 2579.
    int         globalf;               // 9 type, name 4, 2580.
    uint64_t    build_commit_timestamp;// 10 type, name 10, 2581.
    char        build_hash[16];        // 11 type, name 0, 2582.
    char        filename[1024];        // 12 type, name 0, 2095.
};

// 129 DNA structure 346, 3
struct StripAnim
{
    StripAnim    *next;// 1 type, name 346, 0.
    StripAnim    *prev;// 2 type, name 346, 1.
    void         *anim;// Note: using void* on undefined DNA type: anim
};

// 130 DNA structure 347, 3
struct StripElem
{
    char    name[256];  // 1 type, name 0, 2015.
    int     orig_width; // 2 type, name 4, 2583.
    int     orig_height;// 3 type, name 4, 2584.
};

// 131 DNA structure 348, 4
struct StripCrop
{
    int    top;   // 1 type, name 4, 2031.
    int    bottom;// 2 type, name 4, 2585.
    int    left;  // 3 type, name 4, 2037.
    int    right; // 4 type, name 4, 2586.
};

// 132 DNA structure 349, 2
struct StripTransform
{
    int    xofs;// 1 type, name 4, 2587.
    int    yofs;// 2 type, name 4, 2588.
};

// 133 DNA structure 350, 5
struct StripColorBalance
{
    float    lift[3]; // 1 type, name 7, 2589.
    float    gamma[3];// 2 type, name 7, 2590.
    float    gain[3]; // 3 type, name 7, 2591.
    int      flag;    // 4 type, name 4, 18.
    int      pad;     // 5 type, name 4, 68.
};

// 134 DNA structure 351, 10
struct StripProxy
{
    char     dir[768];        // 1 type, name 0, 2592.
    char     file[256];       // 2 type, name 0, 1990.
    void     *anim;           // Note: using void* on undefined DNA type: anim
    short    tc;              // 4 type, name 2, 2593.
    short    quality;         // 5 type, name 2, 1476.
    short    build_size_flags;// 6 type, name 2, 2594.
    short    build_tc_flags;  // 7 type, name 2, 2595.
    short    build_flags;     // 8 type, name 2, 2596.
    char     storage;         // 9 type, name 0, 2597.
    char     pad[5];          // 10 type, name 0, 186.
};

// 135 DNA structure 355, 5
struct MetaStack
{
    MetaStack    *next;        // 1 type, name 355, 0.
    MetaStack    *prev;        // 2 type, name 355, 1.
    ListBase     *oldbasep;    // 3 type, name 14, 2636.
    Sequence     *parseq;      // 4 type, name 353, 2637.
    int          disp_range[2];// 5 type, name 4, 2638.
};

// 136 DNA structure 356, 4
struct WipeVars
{
    float    edgeWidth;// 1 type, name 7, 2650.
    float    angle;    // 2 type, name 7, 814.
    short    forward;  // 3 type, name 2, 2651.
    short    wipetype; // 4 type, name 2, 2652.
};

// 137 DNA structure 357, 6
struct GlowVars
{
    float    fMini;   // 1 type, name 7, 2653.
    float    fClamp;  // 2 type, name 7, 2654.
    float    fBoost;  // 3 type, name 7, 2655.
    float    dDist;   // 4 type, name 7, 2656.
    int      dQuality;// 5 type, name 4, 2657.
    int      bNoComp; // 6 type, name 4, 2658.
};

// 138 DNA structure 358, 8
struct TransformVars
{
    float    ScalexIni;    // 1 type, name 7, 2659.
    float    ScaleyIni;    // 2 type, name 7, 2660.
    float    xIni;         // 3 type, name 7, 2661.
    float    yIni;         // 4 type, name 7, 2662.
    float    rotIni;       // 5 type, name 7, 2663.
    int      percent;      // 6 type, name 4, 811.
    int      interpolation;// 7 type, name 4, 2664.
    int      uniform_scale;// 8 type, name 4, 2665.
};

// 139 DNA structure 359, 2
struct SolidColorVars
{
    float    col[3];// 1 type, name 7, 1609.
    float    pad;   // 2 type, name 7, 68.
};

// 140 DNA structure 360, 5
struct SpeedControlVars
{
    float    *frameMap;     // 1 type, name 7, 2666.
    float    globalSpeed;   // 2 type, name 7, 2667.
    int      flags;         // 3 type, name 4, 91.
    int      length;        // 4 type, name 4, 771.
    int      lastValidFrame;// 5 type, name 4, 2668.
};

// 141 DNA structure 361, 2
struct GaussianBlurVars
{
    float    size_x;// 1 type, name 7, 2669.
    float    size_y;// 2 type, name 7, 2670.
};

// 142 DNA structure 362, 9
struct SequenceModifierData
{
    SequenceModifierData    *next;          // 1 type, name 362, 0.
    SequenceModifierData    *prev;          // 2 type, name 362, 1.
    int                     type;           // 3 type, name 4, 16.
    int                     flag;           // 4 type, name 4, 18.
    char                    name[64];       // 5 type, name 0, 19.
    int                     mask_input_type;// 6 type, name 4, 2671.
    int                     pad;            // 7 type, name 4, 68.
    Sequence                *mask_sequence; // 8 type, name 353, 2672.
    Mask                    *mask_id;       // 9 type, name 292, 2673.
};

// 143 DNA structure 290, 6
struct SequencerScopes
{
    void     *reference_ibuf;   // Note: using void* on undefined DNA type: ImBuf
    void     *zebra_ibuf;       // Note: using void* on undefined DNA type: ImBuf
    void     *waveform_ibuf;    // Note: using void* on undefined DNA type: ImBuf
    void     *sep_waveform_ibuf;// Note: using void* on undefined DNA type: ImBuf
    void     *vector_ibuf;      // Note: using void* on undefined DNA type: ImBuf
    void     *histogram_ibuf;   // Note: using void* on undefined DNA type: ImBuf
};

// 144 DNA structure 368, 6
struct Effect
{
    Effect    *next;  // 1 type, name 368, 0.
    Effect    *prev;  // 2 type, name 368, 1.
    short     type;   // 3 type, name 2, 16.
    short     flag;   // 4 type, name 2, 18.
    short     buttype;// 5 type, name 2, 2683.
    short     rt;     // 6 type, name 2, 58.
};

// 145 DNA structure 369, 8
struct BuildEff
{
    BuildEff    *next;  // 1 type, name 369, 0.
    BuildEff    *prev;  // 2 type, name 369, 1.
    short       type;   // 3 type, name 2, 16.
    short       flag;   // 4 type, name 2, 18.
    short       buttype;// 5 type, name 2, 2683.
    short       rt;     // 6 type, name 2, 58.
    float       len;    // 7 type, name 7, 22.
    float       sfra;   // 8 type, name 7, 144.
};

// 146 DNA structure 370, 45
struct PartEff
{
    PartEff     *next;           // 1 type, name 370, 0.
    PartEff     *prev;           // 2 type, name 370, 1.
    short       type;            // 3 type, name 2, 16.
    short       flag;            // 4 type, name 2, 18.
    short       buttype;         // 5 type, name 2, 2683.
    short       stype;           // 6 type, name 2, 270.
    short       vertgroup;       // 7 type, name 2, 1266.
    short       userjit;         // 8 type, name 2, 2684.
    float       sta;             // 9 type, name 7, 2685.
    float       end;             // 10 type, name 7, 2686.
    float       lifetime;        // 11 type, name 7, 828.
    int         totpart;         // 12 type, name 4, 2687.
    int         totkey;          // 13 type, name 4, 84.
    int         seed;            // 14 type, name 4, 773.
    float       normfac;         // 15 type, name 7, 2688.
    float       obfac;           // 16 type, name 7, 2689.
    float       randfac;         // 17 type, name 7, 2690.
    float       texfac;          // 18 type, name 7, 2691.
    float       randlife;        // 19 type, name 7, 2692.
    float       force[3];        // 20 type, name 7, 2693.
    float       damp;            // 21 type, name 7, 825.
    float       nabla;           // 22 type, name 7, 351.
    float       vectsize;        // 23 type, name 7, 2694.
    float       maxlen;          // 24 type, name 7, 2695.
    float       pad;             // 25 type, name 7, 68.
    float       defvec[3];       // 26 type, name 7, 2696.
    float       mult[4];         // 27 type, name 7, 2697.
    float       life[4];         // 28 type, name 7, 2698.
    short       child[4];        // 29 type, name 2, 2699.
    short       mat[4];          // 30 type, name 2, 2700.
    short       texmap;          // 31 type, name 2, 2701.
    short       curmult;         // 32 type, name 2, 2702.
    short       staticstep;      // 33 type, name 2, 2703.
    short       omat;            // 34 type, name 2, 2704.
    short       timetex;         // 35 type, name 2, 2705.
    short       speedtex;        // 36 type, name 2, 2706.
    short       flag2;           // 37 type, name 2, 583.
    short       flag2neg;        // 38 type, name 2, 2707.
    short       disp;            // 39 type, name 2, 580.
    short       vertgroup_v;     // 40 type, name 2, 2708.
    char        vgroupname[64];  // 41 type, name 0, 2709.
    char        vgroupname_v[64];// 42 type, name 0, 2710.
    float       imat[4][4];      // 43 type, name 7, 268.
    void        *keys;           // Note: using void* on undefined DNA type: Particle
    Group       *group;          // 45 type, name 74, 537.
};

// 147 DNA structure 372, 16
struct WaveEff
{
    WaveEff    *next;   // 1 type, name 372, 0.
    WaveEff    *prev;   // 2 type, name 372, 1.
    short      type;    // 3 type, name 2, 16.
    short      flag;    // 4 type, name 2, 18.
    short      buttype; // 5 type, name 2, 2683.
    short      stype;   // 6 type, name 2, 270.
    float      startx;  // 7 type, name 7, 820.
    float      starty;  // 8 type, name 7, 821.
    float      height;  // 9 type, name 7, 822.
    float      width;   // 10 type, name 7, 636.
    float      narrow;  // 11 type, name 7, 823.
    float      speed;   // 12 type, name 7, 824.
    float      minfac;  // 13 type, name 7, 2712.
    float      damp;    // 14 type, name 7, 825.
    float      timeoffs;// 15 type, name 7, 827.
    float      lifetime;// 16 type, name 7, 828.
};

// 148 DNA structure 283, 5
struct TreeStoreElem
{
    short    type;// 1 type, name 2, 16.
    short    nr;  // 2 type, name 2, 2713.
    short    flag;// 3 type, name 2, 18.
    short    used;// 4 type, name 2, 2714.
    ID       *id; // 5 type, name 22, 2068.
};

// 149 DNA structure 373, 3
struct TreeStore
{
    int              totelem; // 1 type, name 4, 72.
    int              usedelem;// 2 type, name 4, 2715.
    TreeStoreElem    *data;   // 3 type, name 283, 2.
};

// 150 DNA structure 374, 7
struct bProperty
{
    bProperty    *next;   // 1 type, name 374, 0.
    bProperty    *prev;   // 2 type, name 374, 1.
    char         name[64];// 3 type, name 0, 19.
    short        type;    // 4 type, name 2, 16.
    short        flag;    // 5 type, name 2, 18.
    int          data;    // 6 type, name 4, 21.
    void         *poin;   // 7 type, name 11, 2716.
};

// 151 DNA structure 375, 5
struct bNearSensor
{
    char     name[64]; // 1 type, name 0, 19.
    float    dist;     // 2 type, name 7, 375.
    float    resetdist;// 3 type, name 7, 2717.
    int      lastval;  // 4 type, name 4, 2718.
    int      pad;      // 5 type, name 4, 68.
};

// 152 DNA structure 376, 6
struct bMouseSensor
{
    short    type;        // 1 type, name 2, 16.
    short    flag;        // 2 type, name 2, 18.
    short    pad1;        // 3 type, name 2, 70.
    short    mode;        // 4 type, name 2, 368.
    char     propname[64];// 5 type, name 0, 2719.
    char     matname[64]; // 6 type, name 0, 2720.
};

// 153 DNA structure 377, 4
struct bTouchSensor
{
    char        name[64];// 1 type, name 0, 19.
    Material    *ma;     // 2 type, name 73, 2721.
    float       dist;    // 3 type, name 7, 375.
    float       pad;     // 4 type, name 7, 68.
};

// 154 DNA structure 378, 6
struct bKeyboardSensor
{
    short    key;           // 1 type, name 2, 2722.
    short    qual;          // 2 type, name 2, 2723.
    short    type;          // 3 type, name 2, 16.
    short    qual2;         // 4 type, name 2, 2724.
    char     targetName[64];// 5 type, name 0, 2725.
    char     toggleName[64];// 6 type, name 0, 2726.
};

// 155 DNA structure 379, 5
struct bPropertySensor
{
    int     type;        // 1 type, name 4, 16.
    int     pad;         // 2 type, name 4, 68.
    char    name[64];    // 3 type, name 0, 19.
    char    value[64];   // 4 type, name 0, 2727.
    char    maxvalue[64];// 5 type, name 0, 2728.
};

// 156 DNA structure 380, 3
struct bActuatorSensor
{
    int     type;    // 1 type, name 4, 16.
    int     pad;     // 2 type, name 4, 68.
    char    name[64];// 3 type, name 0, 19.
};

// 157 DNA structure 381, 4
struct bDelaySensor
{
    short    delay;   // 1 type, name 2, 2729.
    short    duration;// 2 type, name 2, 2730.
    short    flag;    // 3 type, name 2, 18.
    short    pad;     // 4 type, name 2, 68.
};

// 158 DNA structure 382, 6
struct bCollisionSensor
{
    char     name[64];        // 1 type, name 0, 19.
    char     materialName[64];// 2 type, name 0, 2731.
    short    damptimer;       // 3 type, name 2, 2732.
    short    damp;            // 4 type, name 2, 825.
    short    mode;            // 5 type, name 2, 368.
    short    pad2;            // 6 type, name 2, 29.
};

// 159 DNA structure 383, 5
struct bRadarSensor
{
    char     name[64];// 1 type, name 0, 19.
    float    angle;   // 2 type, name 7, 814.
    float    range;   // 3 type, name 7, 1357.
    short    flag;    // 4 type, name 2, 18.
    short    axis;    // 5 type, name 2, 785.
};

// 160 DNA structure 384, 3
struct bRandomSensor
{
    char    name[64];// 1 type, name 0, 19.
    int     seed;    // 2 type, name 4, 773.
    int     delay;   // 3 type, name 4, 2729.
};

// 161 DNA structure 385, 7
struct bRaySensor
{
    char     name[64];    // 1 type, name 0, 19.
    float    range;       // 2 type, name 7, 1357.
    char     propname[64];// 3 type, name 0, 2719.
    char     matname[64]; // 4 type, name 0, 2720.
    short    mode;        // 5 type, name 2, 368.
    short    pad1;        // 6 type, name 2, 70.
    int      axisflag;    // 7 type, name 4, 2733.
};

// 162 DNA structure 386, 4
struct bArmatureSensor
{
    char     posechannel[64];// 1 type, name 0, 2734.
    char     constraint[64]; // 2 type, name 0, 2735.
    int      type;           // 3 type, name 4, 16.
    float    value;          // 4 type, name 7, 789.
};

// 163 DNA structure 387, 3
struct bMessageSensor
{
    Object    *fromObject;// 1 type, name 29, 2736.
    char      subject[64];// 2 type, name 0, 2737.
    char      body[64];   // 3 type, name 0, 2738.
};

// 164 DNA structure 388, 18
struct bSensor
{
    bSensor        *next;   // 1 type, name 388, 0.
    bSensor        *prev;   // 2 type, name 388, 1.
    short          type;    // 3 type, name 2, 16.
    short          otype;   // 4 type, name 2, 2739.
    short          flag;    // 5 type, name 2, 18.
    short          pulse;   // 6 type, name 2, 2740.
    short          freq;    // 7 type, name 2, 2741.
    short          totlinks;// 8 type, name 2, 2742.
    short          pad1;    // 9 type, name 2, 70.
    short          pad2;    // 10 type, name 2, 29.
    char           name[64];// 11 type, name 0, 19.
    void           *data;   // 12 type, name 11, 2.
    bController    **links; // 13 type, name 389, 2743.
    Object         *ob;     // 14 type, name 29, 46.
    short          invert;  // 15 type, name 2, 1660.
    short          level;   // 16 type, name 2, 734.
    short          tap;     // 17 type, name 2, 2744.
    short          pad;     // 18 type, name 2, 68.
};

// 165 DNA structure 390, 11
struct bJoystickSensor
{
    char     name[64];   // 1 type, name 0, 19.
    char     type;       // 2 type, name 0, 16.
    char     joyindex;   // 3 type, name 0, 2745.
    short    flag;       // 4 type, name 2, 18.
    short    axis;       // 5 type, name 2, 785.
    short    axis_single;// 6 type, name 2, 2746.
    int      axisf;      // 7 type, name 4, 2747.
    int      button;     // 8 type, name 4, 2748.
    int      hat;        // 9 type, name 4, 2749.
    int      hatf;       // 10 type, name 4, 2750.
    int      precision;  // 11 type, name 4, 2751.
};

// 166 DNA structure 391, 1
struct bExpressionCont
{
    char    str[128];// 1 type, name 0, 2752.
};

// 167 DNA structure 392, 4
struct bPythonCont
{
    Text    *text;     // 1 type, name 38, 2030.
    char    module[64];// 2 type, name 0, 2377.
    int     mode;      // 3 type, name 4, 368.
    int     flag;      // 4 type, name 4, 18.
};

// 168 DNA structure 389, 18
struct bController
{
    bController    *next;     // 1 type, name 389, 0.
    bController    *prev;     // 2 type, name 389, 1.
    bController    *mynew;    // 3 type, name 389, 2753.
    short          type;      // 4 type, name 2, 16.
    short          flag;      // 5 type, name 2, 18.
    short          inputs;    // 6 type, name 2, 2754.
    short          totlinks;  // 7 type, name 2, 2742.
    short          otype;     // 8 type, name 2, 2739.
    short          totslinks; // 9 type, name 2, 2755.
    short          pad2;      // 10 type, name 2, 29.
    short          pad3;      // 11 type, name 2, 534.
    char           name[64];  // 12 type, name 0, 19.
    void           *data;     // 13 type, name 11, 2.
    bActuator      **links;   // 14 type, name 393, 2743.
    bSensor        **slinks;  // 15 type, name 388, 2756.
    short          val;       // 16 type, name 2, 14.
    short          valo;      // 17 type, name 2, 2757.
    int            state_mask;// 18 type, name 4, 2758.
};

// 169 DNA structure 394, 3
struct bAddObjectActuator
{
    int       time;// 1 type, name 4, 801.
    int       pad; // 2 type, name 4, 68.
    Object    *ob; // 3 type, name 29, 46.
};

// 170 DNA structure 395, 15
struct bActionActuator
{
    bAction    *act;         // 1 type, name 199, 2759.
    short      type;         // 2 type, name 2, 16.
    short      flag;         // 3 type, name 2, 18.
    float      sta;          // 4 type, name 7, 2685.
    float      end;          // 5 type, name 7, 2686.
    char       name[64];     // 6 type, name 0, 19.
    char       frameProp[64];// 7 type, name 0, 2760.
    short      blendin;      // 8 type, name 2, 2761.
    short      priority;     // 9 type, name 2, 2762.
    short      layer;        // 10 type, name 2, 152.
    short      end_reset;    // 11 type, name 2, 2763.
    short      strideaxis;   // 12 type, name 2, 2764.
    short      blend_mode;   // 13 type, name 2, 2634.
    float      stridelength; // 14 type, name 7, 2765.
    float      layer_weight; // 15 type, name 7, 2766.
};

// 171 DNA structure 396, 8
struct Sound3D
{
    float    min_gain;          // 1 type, name 7, 2767.
    float    max_gain;          // 2 type, name 7, 2768.
    float    reference_distance;// 3 type, name 7, 2769.
    float    max_distance;      // 4 type, name 7, 2770.
    float    rolloff_factor;    // 5 type, name 7, 2771.
    float    cone_inner_angle;  // 6 type, name 7, 2772.
    float    cone_outer_angle;  // 7 type, name 7, 2773.
    float    cone_outer_gain;   // 8 type, name 7, 2774.
};

// 172 DNA structure 398, 14
struct bEditObjectActuator
{
    int       time;          // 1 type, name 4, 801.
    short     type;          // 2 type, name 2, 16.
    short     flag;          // 3 type, name 2, 18.
    Object    *ob;           // 4 type, name 29, 46.
    Mesh      *me;           // 5 type, name 87, 2779.
    char      name[64];      // 6 type, name 0, 19.
    float     linVelocity[3];// 7 type, name 7, 2780.
    float     angVelocity[3];// 8 type, name 7, 2781.
    float     mass;          // 9 type, name 7, 1096.
    short     localflag;     // 10 type, name 2, 2782.
    short     dyn_operation; // 11 type, name 2, 2783.
    short     upflag;        // 12 type, name 2, 1085.
    short     trackflag;     // 13 type, name 2, 1084.
    int       pad;           // 14 type, name 4, 68.
};

// 173 DNA structure 399, 5
struct bSceneActuator
{
    short     type;   // 1 type, name 2, 16.
    short     pad1;   // 2 type, name 2, 70.
    int       pad;    // 3 type, name 4, 68.
    Scene     *scene; // 4 type, name 45, 140.
    Object    *camera;// 5 type, name 29, 1643.
};

// 174 DNA structure 400, 5
struct bPropertyActuator
{
    int       pad;      // 1 type, name 4, 68.
    int       type;     // 2 type, name 4, 16.
    char      name[64]; // 3 type, name 0, 19.
    char      value[64];// 4 type, name 0, 2727.
    Object    *ob;      // 5 type, name 29, 46.
};

// 175 DNA structure 401, 13
struct bObjectActuator
{
    short     flag;              // 1 type, name 2, 18.
    short     type;              // 2 type, name 2, 16.
    short     otype;             // 3 type, name 2, 2739.
    short     damping;           // 4 type, name 2, 1097.
    float     forceloc[3];       // 5 type, name 7, 2784.
    float     forcerot[3];       // 6 type, name 7, 2785.
    float     pad[3];            // 7 type, name 7, 44.
    float     pad1[3];           // 8 type, name 7, 2546.
    float     dloc[3];           // 9 type, name 7, 1067.
    float     drot[3];           // 10 type, name 7, 1071.
    float     linearvelocity[3]; // 11 type, name 7, 2786.
    float     angularvelocity[3];// 12 type, name 7, 2787.
    Object    *reference;        // 13 type, name 29, 2788.
};

// 176 DNA structure 402, 10
struct bIpoActuator
{
    short    flag;         // 1 type, name 2, 18.
    short    type;         // 2 type, name 2, 16.
    float    sta;          // 3 type, name 7, 2685.
    float    end;          // 4 type, name 7, 2686.
    char     name[64];     // 5 type, name 0, 19.
    char     frameProp[64];// 6 type, name 0, 2760.
    short    pad1;         // 7 type, name 2, 70.
    short    pad2;         // 8 type, name 2, 29.
    short    pad3;         // 9 type, name 2, 534.
    short    pad4;         // 10 type, name 2, 1532.
};

// 177 DNA structure 403, 8
struct bCameraActuator
{
    Object    *ob;    // 1 type, name 29, 46.
    float     height; // 2 type, name 7, 822.
    float     min;    // 3 type, name 7, 2789.
    float     max;    // 4 type, name 7, 2790.
    float     damping;// 5 type, name 7, 1097.
    short     pad1;   // 6 type, name 2, 70.
    short     axis;   // 7 type, name 2, 785.
    float     pad2;   // 8 type, name 7, 29.
};

// 178 DNA structure 404, 12
struct bConstraintActuator
{
    short    type;       // 1 type, name 2, 16.
    short    mode;       // 2 type, name 2, 368.
    short    flag;       // 3 type, name 2, 18.
    short    damp;       // 4 type, name 2, 825.
    short    time;       // 5 type, name 2, 801.
    short    rotdamp;    // 6 type, name 2, 2791.
    int      pad;        // 7 type, name 4, 68.
    float    minloc[3];  // 8 type, name 7, 2792.
    float    maxloc[3];  // 9 type, name 7, 2793.
    float    minrot[3];  // 10 type, name 7, 2794.
    float    maxrot[3];  // 11 type, name 7, 2795.
    char     matprop[64];// 12 type, name 0, 2796.
};

// 179 DNA structure 405, 9
struct bGroupActuator
{
    short    flag;    // 1 type, name 2, 18.
    short    type;    // 2 type, name 2, 16.
    int      sta;     // 3 type, name 4, 2685.
    int      end;     // 4 type, name 4, 2686.
    char     name[64];// 5 type, name 0, 19.
    short    pad[3];  // 6 type, name 2, 44.
    short    cur;     // 7 type, name 2, 65.
    short    butsta;  // 8 type, name 2, 2797.
    short    butend;  // 9 type, name 2, 2798.
};

// 180 DNA structure 406, 7
struct bRandomActuator
{
    int      seed;        // 1 type, name 4, 773.
    int      distribution;// 2 type, name 4, 2799.
    int      int_arg_1;   // 3 type, name 4, 2800.
    int      int_arg_2;   // 4 type, name 4, 2801.
    float    float_arg_1; // 5 type, name 7, 2802.
    float    float_arg_2; // 6 type, name 7, 2803.
    char     propname[64];// 7 type, name 0, 2719.
};

// 181 DNA structure 407, 7
struct bMessageActuator
{
    char      toPropName[64];// 1 type, name 0, 2804.
    Object    *toObject;     // 2 type, name 29, 2805.
    char      subject[64];   // 3 type, name 0, 2737.
    short     bodyType;      // 4 type, name 2, 2806.
    short     pad1;          // 5 type, name 2, 70.
    int       pad2;          // 6 type, name 4, 29.
    char      body[64];      // 7 type, name 0, 2738.
};

// 182 DNA structure 408, 6
struct bGameActuator
{
    short    flag;           // 1 type, name 2, 18.
    short    type;           // 2 type, name 2, 16.
    int      sta;            // 3 type, name 4, 2685.
    int      end;            // 4 type, name 4, 2686.
    char     filename[64];   // 5 type, name 0, 2807.
    char     loadaniname[64];// 6 type, name 0, 2808.
};

// 183 DNA structure 409, 1
struct bVisibilityActuator
{
    int    flag;// 1 type, name 4, 18.
};

// 184 DNA structure 410, 6
struct bTwoDFilterActuator
{
    char     pad[4];   // 1 type, name 0, 602.
    short    type;     // 2 type, name 2, 16.
    short    flag;     // 3 type, name 2, 18.
    int      int_arg;  // 4 type, name 4, 2809.
    float    float_arg;// 5 type, name 7, 2810.
    Text     *text;    // 6 type, name 38, 2030.
};

// 185 DNA structure 411, 4
struct bParentActuator
{
    char      pad[2];// 1 type, name 0, 205.
    short     flag;  // 2 type, name 2, 18.
    int       type;  // 3 type, name 4, 16.
    Object    *ob;   // 4 type, name 29, 46.
};

// 186 DNA structure 412, 2
struct bStateActuator
{
    int    type;// 1 type, name 4, 16.
    int    mask;// 2 type, name 4, 1928.
};

// 187 DNA structure 413, 8
struct bArmatureActuator
{
    char      posechannel[64];// 1 type, name 0, 2734.
    char      constraint[64]; // 2 type, name 0, 2735.
    int       type;           // 3 type, name 4, 16.
    float     weight;         // 4 type, name 7, 591.
    float     influence;      // 5 type, name 7, 2811.
    float     pad;            // 6 type, name 7, 68.
    Object    *target;        // 7 type, name 29, 896.
    Object    *subtarget;     // 8 type, name 29, 2812.
};

// 188 DNA structure 414, 11
struct bSteeringActuator
{
    char      pad[5];      // 1 type, name 0, 186.
    char      flag;        // 2 type, name 0, 18.
    short     facingaxis;  // 3 type, name 2, 2813.
    int       type;        // 4 type, name 4, 16.
    float     dist;        // 5 type, name 7, 375.
    float     velocity;    // 6 type, name 7, 2814.
    float     acceleration;// 7 type, name 7, 2815.
    float     turnspeed;   // 8 type, name 7, 2816.
    int       updateTime;  // 9 type, name 4, 2817.
    Object    *target;     // 10 type, name 29, 896.
    Object    *navmesh;    // 11 type, name 29, 2818.
};

// 189 DNA structure 415, 7
struct bMouseActuator
{
    short    type;          // 1 type, name 2, 16.
    short    flag;          // 2 type, name 2, 18.
    int      object_axis[2];// 3 type, name 4, 2819.
    float    threshold[2];  // 4 type, name 7, 2820.
    float    sensitivity[2];// 5 type, name 7, 2821.
    float    limit_x[2];    // 6 type, name 7, 2822.
    float    limit_y[2];    // 7 type, name 7, 2823.
};

// 190 DNA structure 393, 10
struct bActuator
{
    bActuator    *next;   // 1 type, name 393, 0.
    bActuator    *prev;   // 2 type, name 393, 1.
    bActuator    *mynew;  // 3 type, name 393, 2753.
    short        type;    // 4 type, name 2, 16.
    short        flag;    // 5 type, name 2, 18.
    short        otype;   // 6 type, name 2, 2739.
    short        go;      // 7 type, name 2, 2824.
    char         name[64];// 8 type, name 0, 19.
    void         *data;   // 9 type, name 11, 2.
    Object       *ob;     // 10 type, name 29, 46.
};

// 191 DNA structure 416, 6
struct GroupObject
{
    GroupObject    *next;   // 1 type, name 416, 0.
    GroupObject    *prev;   // 2 type, name 416, 1.
    Object         *ob;     // 3 type, name 29, 46.
    void           *lampren;// 4 type, name 11, 2829.
    short          recalc;  // 5 type, name 2, 275.
    char           pad[6];  // 6 type, name 0, 769.
};

// 192 DNA structure 420, 2
struct bMotionPathVert
{
    float    co[3];// 1 type, name 7, 723.
    int      flag; // 2 type, name 4, 18.
};

// 193 DNA structure 203, 5
struct bMotionPath
{
    bMotionPathVert    *points;    // 1 type, name 420, 2867.
    int                length;     // 2 type, name 4, 771.
    int                start_frame;// 3 type, name 4, 2868.
    int                end_frame;  // 4 type, name 4, 2869.
    int                flag;       // 5 type, name 4, 18.
};

// 194 DNA structure 202, 16
struct bAnimVizSettings
{
    int      ghost_sf;     // 1 type, name 4, 2870.
    int      ghost_ef;     // 2 type, name 4, 2871.
    int      ghost_bc;     // 3 type, name 4, 2872.
    int      ghost_ac;     // 4 type, name 4, 2873.
    short    ghost_type;   // 5 type, name 2, 2874.
    short    ghost_step;   // 6 type, name 2, 2875.
    short    ghost_flag;   // 7 type, name 2, 2876.
    short    recalc;       // 8 type, name 2, 275.
    short    path_type;    // 9 type, name 2, 2877.
    short    path_step;    // 10 type, name 2, 2878.
    short    path_viewflag;// 11 type, name 2, 2879.
    short    path_bakeflag;// 12 type, name 2, 2880.
    int      path_sf;      // 13 type, name 4, 2881.
    int      path_ef;      // 14 type, name 4, 2882.
    int      path_bc;      // 15 type, name 4, 2883.
    int      path_ac;      // 16 type, name 4, 2884.
};

// 195 DNA structure 422, 1
struct bIKParam
{
    int    iksolver;// 1 type, name 4, 2915.
};

// 196 DNA structure 423, 12
struct bItasc
{
    int      iksolver; // 1 type, name 4, 2915.
    float    precision;// 2 type, name 7, 2751.
    short    numiter;  // 3 type, name 2, 2919.
    short    numstep;  // 4 type, name 2, 2920.
    float    minstep;  // 5 type, name 7, 2921.
    float    maxstep;  // 6 type, name 7, 2922.
    short    solver;   // 7 type, name 2, 2923.
    short    flag;     // 8 type, name 2, 18.
    float    feedback; // 9 type, name 7, 2924.
    float    maxvel;   // 10 type, name 7, 2925.
    float    dampmax;  // 11 type, name 7, 2926.
    float    dampeps;  // 12 type, name 7, 2927.
};

// 197 DNA structure 427, 5
struct bConstraintChannel
{
    bConstraintChannel    *next;   // 1 type, name 427, 0.
    bConstraintChannel    *prev;   // 2 type, name 427, 1.
    Ipo                   *ipo;    // 3 type, name 33, 82.
    short                 flag;    // 4 type, name 2, 18.
    char                  name[30];// 5 type, name 0, 2942.
};

// 198 DNA structure 428, 14
struct bConstraint
{
    bConstraint    *next;    // 1 type, name 428, 0.
    bConstraint    *prev;    // 2 type, name 428, 1.
    void           *data;    // 3 type, name 11, 2.
    short          type;     // 4 type, name 2, 16.
    short          flag;     // 5 type, name 2, 18.
    char           ownspace; // 6 type, name 0, 2943.
    char           tarspace; // 7 type, name 0, 2944.
    char           name[64]; // 8 type, name 0, 19.
    short          pad;      // 9 type, name 2, 68.
    float          enforce;  // 10 type, name 7, 2945.
    float          headtail; // 11 type, name 7, 2946.
    Ipo            *ipo;     // 12 type, name 33, 82.
    float          lin_error;// 13 type, name 7, 2947.
    float          rot_error;// 14 type, name 7, 2948.
};

// 199 DNA structure 429, 9
struct bConstraintTarget
{
    bConstraintTarget    *next;        // 1 type, name 429, 0.
    bConstraintTarget    *prev;        // 2 type, name 429, 1.
    Object               *tar;         // 3 type, name 29, 2949.
    char                 subtarget[64];// 4 type, name 0, 832.
    float                matrix[4][4]; // 5 type, name 7, 2950.
    short                space;        // 6 type, name 2, 2951.
    short                flag;         // 7 type, name 2, 18.
    short                type;         // 8 type, name 2, 16.
    short                rotOrder;     // 9 type, name 2, 2952.
};

// 200 DNA structure 431, 15
struct bKinematicConstraint
{
    Object    *tar;             // 1 type, name 29, 2949.
    short     iterations;       // 2 type, name 2, 2955.
    short     flag;             // 3 type, name 2, 18.
    short     rootbone;         // 4 type, name 2, 2956.
    short     max_rootbone;     // 5 type, name 2, 2957.
    char      subtarget[64];    // 6 type, name 0, 832.
    Object    *poletar;         // 7 type, name 29, 2958.
    char      polesubtarget[64];// 8 type, name 0, 2959.
    float     poleangle;        // 9 type, name 7, 2960.
    float     weight;           // 10 type, name 7, 591.
    float     orientweight;     // 11 type, name 7, 2961.
    float     grabtarget[3];    // 12 type, name 7, 2962.
    short     type;             // 13 type, name 2, 16.
    short     mode;             // 14 type, name 2, 368.
    float     dist;             // 15 type, name 7, 375.
};

// 201 DNA structure 432, 10
struct bSplineIKConstraint
{
    Object    *tar;        // 1 type, name 29, 2949.
    float     *points;     // 2 type, name 7, 2867.
    short     numpoints;   // 3 type, name 2, 2963.
    short     chainlen;    // 4 type, name 2, 2964.
    short     flag;        // 5 type, name 2, 18.
    short     xzScaleMode; // 6 type, name 2, 2965.
    float     bulge;       // 7 type, name 7, 2966.
    float     bulge_min;   // 8 type, name 7, 2967.
    float     bulge_max;   // 9 type, name 7, 2968.
    float     bulge_smooth;// 10 type, name 7, 2969.
};

// 202 DNA structure 433, 6
struct bTrackToConstraint
{
    Object    *tar;         // 1 type, name 29, 2949.
    int       reserved1;    // 2 type, name 4, 2970.
    int       reserved2;    // 3 type, name 4, 2971.
    int       flags;        // 4 type, name 4, 91.
    int       pad;          // 5 type, name 4, 68.
    char      subtarget[64];// 6 type, name 0, 832.
};

// 203 DNA structure 434, 4
struct bRotateLikeConstraint
{
    Object    *tar;         // 1 type, name 29, 2949.
    int       flag;         // 2 type, name 4, 18.
    int       reserved1;    // 3 type, name 4, 2970.
    char      subtarget[64];// 4 type, name 0, 832.
};

// 204 DNA structure 435, 4
struct bLocateLikeConstraint
{
    Object    *tar;         // 1 type, name 29, 2949.
    int       flag;         // 2 type, name 4, 18.
    int       reserved1;    // 3 type, name 4, 2970.
    char      subtarget[64];// 4 type, name 0, 832.
};

// 205 DNA structure 436, 4
struct bSizeLikeConstraint
{
    Object    *tar;         // 1 type, name 29, 2949.
    int       flag;         // 2 type, name 4, 18.
    int       reserved1;    // 3 type, name 4, 2970.
    char      subtarget[64];// 4 type, name 0, 832.
};

// 206 DNA structure 437, 2
struct bSameVolumeConstraint
{
    int      flag;  // 1 type, name 4, 18.
    float    volume;// 2 type, name 7, 1224.
};

// 207 DNA structure 438, 2
struct bTransLikeConstraint
{
    Object    *tar;         // 1 type, name 29, 2949.
    char      subtarget[64];// 2 type, name 0, 832.
};

// 208 DNA structure 439, 10
struct bMinMaxConstraint
{
    Object    *tar;         // 1 type, name 29, 2949.
    int       minmaxflag;   // 2 type, name 4, 2972.
    float     offset;       // 3 type, name 7, 143.
    int       flag;         // 4 type, name 4, 18.
    short     sticky;       // 5 type, name 2, 2027.
    short     stuck;        // 6 type, name 2, 2973.
    short     pad1;         // 7 type, name 2, 70.
    short     pad2;         // 8 type, name 2, 29.
    float     cache[3];     // 9 type, name 7, 2974.
    char      subtarget[64];// 10 type, name 0, 832.
};

// 209 DNA structure 440, 10
struct bActionConstraint
{
    Object     *tar;         // 1 type, name 29, 2949.
    short      type;         // 2 type, name 2, 16.
    short      local;        // 3 type, name 2, 1274.
    int        start;        // 4 type, name 4, 770.
    int        end;          // 5 type, name 4, 2686.
    float      min;          // 6 type, name 7, 2789.
    float      max;          // 7 type, name 7, 2790.
    int        flag;         // 8 type, name 4, 18.
    bAction    *act;         // 9 type, name 199, 2759.
    char       subtarget[64];// 10 type, name 0, 832.
};

// 210 DNA structure 441, 4
struct bLockTrackConstraint
{
    Object    *tar;         // 1 type, name 29, 2949.
    int       trackflag;    // 2 type, name 4, 1084.
    int       lockflag;     // 3 type, name 4, 2975.
    char      subtarget[64];// 4 type, name 0, 832.
};

// 211 DNA structure 442, 4
struct bDampTrackConstraint
{
    Object    *tar;         // 1 type, name 29, 2949.
    int       trackflag;    // 2 type, name 4, 1084.
    int       pad;          // 3 type, name 4, 68.
    char      subtarget[64];// 4 type, name 0, 832.
};

// 212 DNA structure 443, 6
struct bFollowPathConstraint
{
    Object    *tar;      // 1 type, name 29, 2949.
    float     offset;    // 2 type, name 7, 143.
    float     offset_fac;// 3 type, name 7, 907.
    int       followflag;// 4 type, name 4, 2976.
    short     trackflag; // 5 type, name 2, 1084.
    short     upflag;    // 6 type, name 2, 1085.
};

// 213 DNA structure 444, 10
struct bStretchToConstraint
{
    Object    *tar;         // 1 type, name 29, 2949.
    int       flag;         // 2 type, name 4, 18.
    int       volmode;      // 3 type, name 4, 2977.
    int       plane;        // 4 type, name 4, 2978.
    float     orglength;    // 5 type, name 7, 2979.
    float     bulge;        // 6 type, name 7, 2966.
    float     bulge_min;    // 7 type, name 7, 2967.
    float     bulge_max;    // 8 type, name 7, 2968.
    float     bulge_smooth; // 9 type, name 7, 2969.
    char      subtarget[64];// 10 type, name 0, 832.
};

// 214 DNA structure 445, 16
struct bRigidBodyJointConstraint
{
    Object    *tar;       // 1 type, name 29, 2949.
    Object    *child;     // 2 type, name 29, 2891.
    int       type;       // 3 type, name 4, 16.
    float     pivX;       // 4 type, name 7, 2980.
    float     pivY;       // 5 type, name 7, 2981.
    float     pivZ;       // 6 type, name 7, 2982.
    float     axX;        // 7 type, name 7, 2983.
    float     axY;        // 8 type, name 7, 2984.
    float     axZ;        // 9 type, name 7, 2985.
    float     minLimit[6];// 10 type, name 7, 2986.
    float     maxLimit[6];// 11 type, name 7, 2987.
    float     extraFz;    // 12 type, name 7, 2988.
    short     flag;       // 13 type, name 2, 18.
    short     pad;        // 14 type, name 2, 68.
    short     pad1;       // 15 type, name 2, 70.
    short     pad2;       // 16 type, name 2, 29.
};

// 215 DNA structure 446, 3
struct bClampToConstraint
{
    Object    *tar; // 1 type, name 29, 2949.
    int       flag; // 2 type, name 4, 18.
    int       flag2;// 3 type, name 4, 583.
};

// 216 DNA structure 447, 5
struct bChildOfConstraint
{
    Object    *tar;         // 1 type, name 29, 2949.
    int       flag;         // 2 type, name 4, 18.
    int       pad;          // 3 type, name 4, 68.
    float     invmat[4][4]; // 4 type, name 7, 2989.
    char      subtarget[64];// 5 type, name 0, 832.
};

// 217 DNA structure 448, 18
struct bTransformConstraint
{
    Object    *tar;             // 1 type, name 29, 2949.
    char      subtarget[64];    // 2 type, name 0, 832.
    short     from;             // 3 type, name 2, 2990.
    short     to;               // 4 type, name 2, 2991.
    char      map[3];           // 5 type, name 0, 2992.
    char      expo;             // 6 type, name 0, 2993.
    float     from_min[3];      // 7 type, name 7, 2994.
    float     from_max[3];      // 8 type, name 7, 2995.
    float     to_min[3];        // 9 type, name 7, 2996.
    float     to_max[3];        // 10 type, name 7, 2997.
    float     from_min_rot[3];  // 11 type, name 7, 2998.
    float     from_max_rot[3];  // 12 type, name 7, 2999.
    float     to_min_rot[3];    // 13 type, name 7, 3000.
    float     to_max_rot[3];    // 14 type, name 7, 3001.
    float     from_min_scale[3];// 15 type, name 7, 3002.
    float     from_max_scale[3];// 16 type, name 7, 3003.
    float     to_min_scale[3];  // 17 type, name 7, 3004.
    float     to_max_scale[3];  // 18 type, name 7, 3005.
};

// 218 DNA structure 449, 5
struct bPivotConstraint
{
    Object    *tar;         // 1 type, name 29, 2949.
    char      subtarget[64];// 2 type, name 0, 832.
    float     offset[3];    // 3 type, name 7, 779.
    short     rotAxis;      // 4 type, name 2, 3006.
    short     flag;         // 5 type, name 2, 18.
};

// 219 DNA structure 450, 8
struct bLocLimitConstraint
{
    float    xmin; // 1 type, name 7, 8.
    float    xmax; // 2 type, name 7, 9.
    float    ymin; // 3 type, name 7, 10.
    float    ymax; // 4 type, name 7, 11.
    float    zmin; // 5 type, name 7, 3007.
    float    zmax; // 6 type, name 7, 3008.
    short    flag; // 7 type, name 2, 18.
    short    flag2;// 8 type, name 2, 583.
};

// 220 DNA structure 451, 8
struct bRotLimitConstraint
{
    float    xmin; // 1 type, name 7, 8.
    float    xmax; // 2 type, name 7, 9.
    float    ymin; // 3 type, name 7, 10.
    float    ymax; // 4 type, name 7, 11.
    float    zmin; // 5 type, name 7, 3007.
    float    zmax; // 6 type, name 7, 3008.
    short    flag; // 7 type, name 2, 18.
    short    flag2;// 8 type, name 2, 583.
};

// 221 DNA structure 452, 8
struct bSizeLimitConstraint
{
    float    xmin; // 1 type, name 7, 8.
    float    xmax; // 2 type, name 7, 9.
    float    ymin; // 3 type, name 7, 10.
    float    ymax; // 4 type, name 7, 11.
    float    zmin; // 5 type, name 7, 3007.
    float    zmax; // 6 type, name 7, 3008.
    short    flag; // 7 type, name 2, 18.
    short    flag2;// 8 type, name 2, 583.
};

// 222 DNA structure 453, 7
struct bDistLimitConstraint
{
    Object    *tar;         // 1 type, name 29, 2949.
    char      subtarget[64];// 2 type, name 0, 832.
    float     dist;         // 3 type, name 7, 375.
    float     soft;         // 4 type, name 7, 383.
    short     flag;         // 5 type, name 2, 18.
    short     mode;         // 6 type, name 2, 368.
    int       pad;          // 7 type, name 4, 68.
};

// 223 DNA structure 454, 7
struct bShrinkwrapConstraint
{
    Object    *target;      // 1 type, name 29, 896.
    float     dist;         // 2 type, name 7, 375.
    short     shrinkType;   // 3 type, name 2, 900.
    char      projAxis;     // 4 type, name 0, 903.
    char      projAxisSpace;// 5 type, name 0, 3009.
    float     projLimit;    // 6 type, name 7, 902.
    char      pad[4];       // 7 type, name 0, 602.
};

// 224 DNA structure 455, 7
struct bFollowTrackConstraint
{
    MovieClip    *clip;       // 1 type, name 265, 1839.
    char         track[64];   // 2 type, name 0, 3010.
    int          flag;        // 3 type, name 4, 18.
    int          frame_method;// 4 type, name 4, 3011.
    char         object[64];  // 5 type, name 0, 3012.
    Object       *camera;     // 6 type, name 29, 1643.
    Object       *depth_ob;   // 7 type, name 29, 3013.
};

// 225 DNA structure 456, 3
struct bCameraSolverConstraint
{
    MovieClip    *clip;// 1 type, name 265, 1839.
    int          flag; // 2 type, name 4, 18.
    int          pad;  // 3 type, name 4, 68.
};

// 226 DNA structure 457, 6
struct bObjectSolverConstraint
{
    MovieClip    *clip;       // 1 type, name 265, 1839.
    int          flag;        // 2 type, name 4, 18.
    int          pad;         // 3 type, name 4, 68.
    char         object[64];  // 4 type, name 0, 3012.
    float        invmat[4][4];// 5 type, name 7, 2989.
    Object       *camera;     // 6 type, name 29, 1643.
};

// 227 DNA structure 458, 10
struct bActionModifier
{
    bActionModifier    *next;      // 1 type, name 458, 0.
    bActionModifier    *prev;      // 2 type, name 458, 1.
    short              type;       // 3 type, name 2, 16.
    short              flag;       // 4 type, name 2, 18.
    char               channel[32];// 5 type, name 0, 3014.
    float              noisesize;  // 6 type, name 7, 314.
    float              turbul;     // 7 type, name 7, 315.
    short              channels;   // 8 type, name 2, 2928.
    short              no_rot_axis;// 9 type, name 2, 3015.
    Object             *ob;        // 10 type, name 29, 46.
};

// 228 DNA structure 460, 11
struct bNodeStack
{
    float    vec[4];    // 1 type, name 7, 603.
    float    min;       // 2 type, name 7, 2789.
    float    max;       // 3 type, name 7, 2790.
    void     *data;     // 4 type, name 11, 2.
    short    hasinput;  // 5 type, name 2, 3025.
    short    hasoutput; // 6 type, name 2, 3026.
    short    datatype;  // 7 type, name 2, 3027.
    short    sockettype;// 8 type, name 2, 3028.
    short    is_copy;   // 9 type, name 2, 3029.
    short    external;  // 10 type, name 2, 3030.
    short    pad[2];    // 11 type, name 2, 205.
};

// 229 DNA structure 306, 1
struct bNodeInstanceKey
{
    int    value;// 1 type, name 4, 789.
};

// 230 DNA structure 463, 8
struct bNodeLink
{
    bNodeLink      *next;    // 1 type, name 463, 0.
    bNodeLink      *prev;    // 2 type, name 463, 1.
    bNode          *fromnode;// 3 type, name 464, 3075.
    bNode          *tonode;  // 4 type, name 464, 3076.
    bNodeSocket    *fromsock;// 5 type, name 461, 3077.
    bNodeSocket    *tosock;  // 6 type, name 461, 3078.
    int            flag;     // 7 type, name 4, 18.
    int            pad;      // 8 type, name 4, 68.
};

// 231 DNA structure 473, 4
struct bNodeSocketValueInt
{
    int    subtype;// 1 type, name 4, 17.
    int    value;  // 2 type, name 4, 789.
    int    min;    // 3 type, name 4, 2789.
    int    max;    // 4 type, name 4, 2790.
};

// 232 DNA structure 474, 4
struct bNodeSocketValueFloat
{
    int      subtype;// 1 type, name 4, 17.
    float    value;  // 2 type, name 7, 789.
    float    min;    // 3 type, name 7, 2789.
    float    max;    // 4 type, name 7, 2790.
};

// 233 DNA structure 475, 2
struct bNodeSocketValueBoolean
{
    char    value; // 1 type, name 0, 789.
    char    pad[3];// 2 type, name 0, 44.
};

// 234 DNA structure 476, 4
struct bNodeSocketValueVector
{
    int      subtype; // 1 type, name 4, 17.
    float    value[3];// 2 type, name 7, 3101.
    float    min;     // 3 type, name 7, 2789.
    float    max;     // 4 type, name 7, 2790.
};

// 235 DNA structure 477, 1
struct bNodeSocketValueRGBA
{
    float    value[4];// 1 type, name 7, 3102.
};

// 236 DNA structure 478, 3
struct bNodeSocketValueString
{
    int     subtype;    // 1 type, name 4, 17.
    int     pad;        // 2 type, name 4, 68.
    char    value[1024];// 3 type, name 0, 3103.
};

// 237 DNA structure 479, 2
struct NodeFrame
{
    short    flag;      // 1 type, name 2, 18.
    short    label_size;// 2 type, name 2, 3104.
};

// 238 DNA structure 480, 6
struct NodeImageAnim
{
    int      frames;// 1 type, name 4, 142.
    int      sfra;  // 2 type, name 4, 144.
    int      nr;    // 3 type, name 4, 2713.
    char     cyclic;// 4 type, name 0, 3105.
    char     movie; // 5 type, name 0, 3106.
    short    pad;   // 6 type, name 2, 68.
};

// 239 DNA structure 481, 6
struct ColorCorrectionData
{
    float    saturation;// 1 type, name 7, 318.
    float    contrast;  // 2 type, name 7, 317.
    float    gamma;     // 3 type, name 7, 3107.
    float    gain;      // 4 type, name 7, 3108.
    float    lift;      // 5 type, name 7, 3109.
    int      pad;       // 6 type, name 4, 68.
};

// 240 DNA structure 483, 5
struct NodeBokehImage
{
    float    angle;       // 1 type, name 7, 814.
    int      flaps;       // 2 type, name 4, 3116.
    float    rounding;    // 3 type, name 7, 3117.
    float    catadioptric;// 4 type, name 7, 3118.
    float    lensshift;   // 5 type, name 7, 3119.
};

// 241 DNA structure 484, 6
struct NodeBoxMask
{
    float    x;       // 1 type, name 7, 5.
    float    y;       // 2 type, name 7, 6.
    float    rotation;// 3 type, name 7, 1845.
    float    height;  // 4 type, name 7, 822.
    float    width;   // 5 type, name 7, 636.
    int      pad;     // 6 type, name 4, 68.
};

// 242 DNA structure 485, 6
struct NodeEllipseMask
{
    float    x;       // 1 type, name 7, 5.
    float    y;       // 2 type, name 7, 6.
    float    rotation;// 3 type, name 7, 1845.
    float    height;  // 4 type, name 7, 822.
    float    width;   // 5 type, name 7, 636.
    int      pad;     // 6 type, name 4, 68.
};

// 243 DNA structure 486, 2
struct NodeImageLayer
{
    int    pass_index;// 1 type, name 4, 3120.
    int    pass_flag; // 2 type, name 4, 3121.
};

// 244 DNA structure 487, 16
struct NodeBlurData
{
    short    sizex;          // 1 type, name 2, 2525.
    short    sizey;          // 2 type, name 2, 2526.
    short    samples;        // 3 type, name 2, 115.
    short    maxspeed;       // 4 type, name 2, 3122.
    short    minspeed;       // 5 type, name 2, 3123.
    short    relative;       // 6 type, name 2, 71.
    short    aspect;         // 7 type, name 2, 2069.
    short    curved;         // 8 type, name 2, 3124.
    float    fac;            // 9 type, name 7, 817.
    float    percentx;       // 10 type, name 7, 3125.
    float    percenty;       // 11 type, name 7, 3126.
    short    filtertype;     // 12 type, name 2, 390.
    char     bokeh;          // 13 type, name 0, 3127.
    char     gamma;          // 14 type, name 0, 3107.
    int      image_in_width; // 15 type, name 4, 3128.
    int      image_in_height;// 16 type, name 4, 3129.
};

// 245 DNA structure 488, 9
struct NodeDBlurData
{
    float    center_x;// 1 type, name 7, 3130.
    float    center_y;// 2 type, name 7, 3131.
    float    distance;// 3 type, name 7, 1042.
    float    angle;   // 4 type, name 7, 814.
    float    spin;    // 5 type, name 7, 3132.
    float    zoom;    // 6 type, name 7, 1978.
    short    iter;    // 7 type, name 2, 812.
    char     wrap;    // 8 type, name 0, 3133.
    char     pad;     // 9 type, name 0, 68.
};

// 246 DNA structure 489, 4
struct NodeBilateralBlurData
{
    float    sigma_color;// 1 type, name 7, 3134.
    float    sigma_space;// 2 type, name 7, 3135.
    short    iter;       // 3 type, name 2, 812.
    short    pad;        // 4 type, name 2, 68.
};

// 247 DNA structure 490, 3
struct NodeHueSat
{
    float    hue;// 1 type, name 7, 3136.
    float    sat;// 2 type, name 7, 2611.
    float    val;// 3 type, name 7, 14.
};

// 248 DNA structure 494, 9
struct NodeChroma
{
    float    t1;       // 1 type, name 7, 3143.
    float    t2;       // 2 type, name 7, 3144.
    float    t3;       // 3 type, name 7, 3145.
    float    fsize;    // 4 type, name 7, 647.
    float    fstrength;// 5 type, name 7, 3146.
    float    falpha;   // 6 type, name 7, 3147.
    float    key[4];   // 7 type, name 7, 3148.
    short    algorithm;// 8 type, name 2, 3149.
    short    channel;  // 9 type, name 2, 3150.
};

// 249 DNA structure 495, 8
struct NodeTwoXYs
{
    short    x1;    // 1 type, name 2, 3151.
    short    x2;    // 2 type, name 2, 3152.
    short    y1;    // 3 type, name 2, 3153.
    short    y2;    // 4 type, name 2, 3154.
    float    fac_x1;// 5 type, name 7, 3155.
    float    fac_x2;// 6 type, name 7, 3156.
    float    fac_y1;// 7 type, name 7, 3157.
    float    fac_y2;// 8 type, name 7, 3158.
};

// 250 DNA structure 496, 2
struct NodeTwoFloats
{
    float    x;// 1 type, name 7, 5.
    float    y;// 2 type, name 7, 6.
};

// 251 DNA structure 497, 2
struct NodeGeometry
{
    char    uvname[64]; // 1 type, name 0, 198.
    char    colname[64];// 2 type, name 0, 3159.
};

// 252 DNA structure 498, 1
struct NodeVertexCol
{
    char    name[64];// 1 type, name 0, 19.
};

// 253 DNA structure 499, 12
struct NodeDefocus
{
    char     bktype;  // 1 type, name 0, 3160.
    char     pad_c1;  // 2 type, name 0, 3161.
    char     preview; // 3 type, name 0, 1952.
    char     gamco;   // 4 type, name 0, 3162.
    short    samples; // 5 type, name 2, 115.
    short    no_zbuf; // 6 type, name 2, 3163.
    float    fstop;   // 7 type, name 7, 106.
    float    maxblur; // 8 type, name 7, 3164.
    float    bthresh; // 9 type, name 7, 3165.
    float    scale;   // 10 type, name 7, 972.
    float    rotation;// 11 type, name 7, 1845.
    float    pad_f1;  // 12 type, name 7, 3166.
};

// 254 DNA structure 500, 2
struct NodeScriptDict
{
    void    *dict;// 1 type, name 11, 3167.
    void    *node;// 2 type, name 11, 3168.
};

// 255 DNA structure 501, 13
struct NodeGlare
{
    char     quality;  // 1 type, name 0, 1476.
    char     type;     // 2 type, name 0, 16.
    char     iter;     // 3 type, name 0, 812.
    char     angle;    // 4 type, name 0, 814.
    char     pad_c1;   // 5 type, name 0, 3161.
    char     size;     // 6 type, name 0, 103.
    char     pad[2];   // 7 type, name 0, 205.
    float    colmod;   // 8 type, name 7, 3169.
    float    mix;      // 9 type, name 7, 3170.
    float    threshold;// 10 type, name 7, 971.
    float    fade;     // 11 type, name 7, 3171.
    float    angle_ofs;// 12 type, name 7, 3172.
    float    pad_f1;   // 13 type, name 7, 3166.
};

// 256 DNA structure 502, 8
struct NodeTonemap
{
    float    key;   // 1 type, name 7, 2722.
    float    offset;// 2 type, name 7, 143.
    float    gamma; // 3 type, name 7, 3107.
    float    f;     // 4 type, name 7, 729.
    float    m;     // 5 type, name 7, 3173.
    float    a;     // 6 type, name 7, 259.
    float    c;     // 7 type, name 7, 3174.
    int      type;  // 8 type, name 4, 16.
};

// 257 DNA structure 503, 4
struct NodeLensDist
{
    short    jit; // 1 type, name 2, 3175.
    short    proj;// 2 type, name 2, 3176.
    short    fit; // 3 type, name 2, 3177.
    short    pad; // 4 type, name 2, 68.
};

// 258 DNA structure 504, 6
struct NodeColorBalance
{
    float    slope[3]; // 1 type, name 7, 3178.
    float    offset[3];// 2 type, name 7, 779.
    float    power[3]; // 3 type, name 7, 3179.
    float    lift[3];  // 4 type, name 7, 2589.
    float    gamma[3]; // 5 type, name 7, 2590.
    float    gain[3];  // 6 type, name 7, 2591.
};

// 259 DNA structure 505, 6
struct NodeColorspill
{
    short    limchan; // 1 type, name 2, 3180.
    short    unspill; // 2 type, name 2, 3181.
    float    limscale;// 3 type, name 7, 3182.
    float    uspillr; // 4 type, name 7, 3183.
    float    uspillg; // 5 type, name 7, 3184.
    float    uspillb; // 6 type, name 7, 3185.
};

// 260 DNA structure 506, 2
struct NodeDilateErode
{
    char    falloff;// 1 type, name 0, 826.
    char    pad[7]; // 2 type, name 0, 119.
};

// 261 DNA structure 507, 2
struct NodeMask
{
    int    size_x;// 1 type, name 4, 2669.
    int    size_y;// 2 type, name 4, 2670.
};

// 262 DNA structure 520, 1
struct NodeShaderAttribute
{
    char    name[64];// 1 type, name 0, 19.
};

// 263 DNA structure 521, 4
struct NodeShaderVectTransform
{
    int    type;        // 1 type, name 4, 16.
    int    convert_from;// 2 type, name 4, 3202.
    int    convert_to;  // 3 type, name 4, 3203.
    int    pad;         // 4 type, name 4, 68.
};

// 264 DNA structure 522, 1
struct TexNodeOutput
{
    char    name[64];// 1 type, name 0, 19.
};

// 265 DNA structure 523, 1
struct NodeKeyingScreenData
{
    char    tracking_object[64];// 1 type, name 0, 3204.
};

// 266 DNA structure 524, 12
struct NodeKeyingData
{
    float    screen_balance;       // 1 type, name 7, 3205.
    float    despill_factor;       // 2 type, name 7, 3206.
    float    despill_balance;      // 3 type, name 7, 3207.
    int      edge_kernel_radius;   // 4 type, name 4, 3208.
    float    edge_kernel_tolerance;// 5 type, name 7, 3209.
    float    clip_black;           // 6 type, name 7, 3210.
    float    clip_white;           // 7 type, name 7, 3211.
    int      dilate_distance;      // 8 type, name 4, 3212.
    int      feather_distance;     // 9 type, name 4, 3213.
    int      feather_falloff;      // 10 type, name 4, 3214.
    int      blur_pre;             // 11 type, name 4, 3215.
    int      blur_post;            // 12 type, name 4, 3216.
};

// 267 DNA structure 525, 2
struct NodeTrackPosData
{
    char    tracking_object[64];// 1 type, name 0, 3204.
    char    track_name[64];     // 2 type, name 0, 3217.
};

// 268 DNA structure 526, 3
struct NodeTranslateData
{
    char    wrap_axis;// 1 type, name 0, 3218.
    char    relative; // 2 type, name 0, 71.
    char    pad[6];   // 3 type, name 0, 769.
};

// 269 DNA structure 527, 6
struct NodePlaneTrackDeformData
{
    char     tracking_object[64]; // 1 type, name 0, 3204.
    char     plane_track_name[64];// 2 type, name 0, 3219.
    char     flag;                // 3 type, name 0, 18.
    char     motion_blur_samples; // 4 type, name 0, 3220.
    char     pad[2];              // 5 type, name 0, 205.
    float    motion_blur_shutter; // 6 type, name 7, 3221.
};

// 270 DNA structure 528, 5
struct NodeShaderScript
{
    int     mode;             // 1 type, name 4, 368.
    int     flag;             // 2 type, name 4, 18.
    char    filepath[1024];   // 3 type, name 0, 35.
    char    bytecode_hash[64];// 4 type, name 0, 3222.
    char    *bytecode;        // 5 type, name 0, 3223.
};

// 271 DNA structure 529, 3
struct NodeShaderTangent
{
    int     direction_type;// 1 type, name 4, 3224.
    int     axis;          // 2 type, name 4, 785.
    char    uv_map[64];    // 3 type, name 0, 3225.
};

// 272 DNA structure 530, 2
struct NodeShaderNormalMap
{
    int     space;     // 1 type, name 4, 2951.
    char    uv_map[64];// 2 type, name 0, 3225.
};

// 273 DNA structure 531, 1
struct NodeShaderUVMap
{
    char    uv_map[64];// 1 type, name 0, 3225.
};

// 274 DNA structure 532, 2
struct NodeSunBeams
{
    float    source[2]; // 1 type, name 7, 3226.
    float    ray_length;// 2 type, name 7, 3227.
};

// 275 DNA structure 533, 4
struct CurveMapPoint
{
    float    x;     // 1 type, name 7, 5.
    float    y;     // 2 type, name 7, 6.
    short    flag;  // 3 type, name 2, 18.
    short    shorty;// 4 type, name 2, 3228.
};

// 276 DNA structure 534, 12
struct CurveMap
{
    short            totpoint;         // 1 type, name 2, 1203.
    short            flag;             // 2 type, name 2, 18.
    float            range;            // 3 type, name 7, 1357.
    float            mintable;         // 4 type, name 7, 3229.
    float            maxtable;         // 5 type, name 7, 3230.
    float            ext_in[2];        // 6 type, name 7, 3231.
    float            ext_out[2];       // 7 type, name 7, 3232.
    CurveMapPoint    *curve;           // 8 type, name 533, 3233.
    CurveMapPoint    *table;           // 9 type, name 533, 3234.
    CurveMapPoint    *premultable;     // 10 type, name 533, 3235.
    float            premul_ext_in[2]; // 11 type, name 7, 3236.
    float            premul_ext_out[2];// 12 type, name 7, 3237.
};

// 277 DNA structure 301, 13
struct Histogram
{
    int      channels;      // 1 type, name 4, 2928.
    int      x_resolution;  // 2 type, name 4, 3247.
    float    data_luma[256];// 3 type, name 7, 3248.
    float    data_r[256];   // 4 type, name 7, 3249.
    float    data_g[256];   // 5 type, name 7, 3250.
    float    data_b[256];   // 6 type, name 7, 3251.
    float    data_a[256];   // 7 type, name 7, 3252.
    float    xmax;          // 8 type, name 7, 9.
    float    ymax;          // 9 type, name 7, 11.
    short    mode;          // 10 type, name 2, 368.
    short    flag;          // 11 type, name 2, 18.
    int      height;        // 12 type, name 4, 822.
    float    co[2][2];      // 13 type, name 7, 3253.
};

// 278 DNA structure 233, 8
struct ColorManagedViewSettings
{
    int             flag;              // 1 type, name 4, 18.
    int             pad;               // 2 type, name 4, 68.
    char            look[64];          // 3 type, name 0, 3270.
    char            view_transform[64];// 4 type, name 0, 3271.
    float           exposure;          // 5 type, name 7, 1355.
    float           gamma;             // 6 type, name 7, 3107.
    CurveMapping    *curve_mapping;    // 7 type, name 63, 3272.
    void            *pad2;             // 8 type, name 11, 1678.
};

// 279 DNA structure 234, 1
struct ColorManagedDisplaySettings
{
    char    display_device[64];// 1 type, name 0, 3273.
};

// 280 DNA structure 54, 1
struct ColorManagedColorspaceSettings
{
    char    name[64];// 1 type, name 0, 19.
};

// 281 DNA structure 535, 4
struct BrushClone
{
    Image    *image;   // 1 type, name 51, 805.
    float    offset[2];// 2 type, name 7, 3274.
    float    alpha;    // 3 type, name 7, 463.
    float    pad;      // 4 type, name 7, 68.
};

// 282 DNA structure 537, 4
struct PaletteColor
{
    PaletteColor    *next; // 1 type, name 537, 0.
    PaletteColor    *prev; // 2 type, name 537, 1.
    float           rgb[3];// 3 type, name 7, 1683.
    float           value; // 4 type, name 7, 789.
};

// 283 DNA structure 539, 10
struct CustomDataLayer
{
    int     type;        // 1 type, name 4, 16.
    int     offset;      // 2 type, name 4, 143.
    int     flag;        // 3 type, name 4, 18.
    int     active;      // 4 type, name 4, 1155.
    int     active_rnd;  // 5 type, name 4, 3324.
    int     active_clone;// 6 type, name 4, 3325.
    int     active_mask; // 7 type, name 4, 3326.
    int     uid;         // 8 type, name 4, 73.
    char    name[64];    // 9 type, name 0, 19.
    void    *data;       // 10 type, name 11, 2.
};

// 284 DNA structure 540, 1
struct CustomDataExternal
{
    char    filename[1024];// 1 type, name 0, 2095.
};

// 285 DNA structure 102, 8
struct CustomData
{
    CustomDataLayer       *layers;    // 1 type, name 539, 3327.
    int                   typemap[42];// 2 type, name 4, 3328.
    int                   pad_i1;     // 3 type, name 4, 955.
    int                   totlayer;   // 4 type, name 4, 3329.
    int                   maxlayer;   // 5 type, name 4, 3330.
    int                   totsize;    // 6 type, name 4, 3331.
    void                  *pool;      // Note: using void* on undefined DNA type: BLI_mempool
    CustomDataExternal    *external;  // 8 type, name 540, 3333.
};

// 286 DNA structure 541, 6
struct HairKey
{
    float    co[3];      // 1 type, name 7, 723.
    float    time;       // 2 type, name 7, 801.
    float    weight;     // 3 type, name 7, 591.
    short    editflag;   // 4 type, name 2, 705.
    short    pad;        // 5 type, name 2, 68.
    float    world_co[3];// 6 type, name 7, 3334.
};

// 287 DNA structure 542, 5
struct ParticleKey
{
    float    co[3]; // 1 type, name 7, 723.
    float    vel[3];// 2 type, name 7, 1299.
    float    rot[4];// 3 type, name 7, 3335.
    float    ave[3];// 4 type, name 7, 3336.
    float    time;  // 5 type, name 7, 801.
};

// 288 DNA structure 545, 3
struct ParticleSpring
{
    float    rest_length;      // 1 type, name 7, 3339.
    int      particle_index[2];// 2 type, name 4, 3340.
    int      delete_flag;      // 3 type, name 4, 3341.
};

// 289 DNA structure 546, 7
struct ChildParticle
{
    int      num;    // 1 type, name 4, 3342.
    int      parent; // 2 type, name 4, 3343.
    int      pa[4];  // 3 type, name 4, 3344.
    float    w[4];   // 4 type, name 7, 3345.
    float    fuv[4]; // 5 type, name 7, 3346.
    float    foffset;// 6 type, name 7, 3347.
    float    rt;     // 7 type, name 7, 58.
};

// 290 DNA structure 547, 8
struct ParticleTarget
{
    ParticleTarget    *next;   // 1 type, name 547, 0.
    ParticleTarget    *prev;   // 2 type, name 547, 1.
    Object            *ob;     // 3 type, name 29, 46.
    int               psys;    // 4 type, name 4, 283.
    short             flag;    // 5 type, name 2, 18.
    short             mode;    // 6 type, name 2, 368.
    float             time;    // 7 type, name 7, 801.
    float             duration;// 8 type, name 7, 2730.
};

// 291 DNA structure 548, 7
struct ParticleDupliWeight
{
    ParticleDupliWeight    *next;// 1 type, name 548, 0.
    ParticleDupliWeight    *prev;// 2 type, name 548, 1.
    Object                 *ob;  // 3 type, name 29, 46.
    short                  count;// 4 type, name 2, 784.
    short                  flag; // 5 type, name 2, 18.
    short                  index;// 6 type, name 2, 447.
    short                  rt;   // 7 type, name 2, 58.
};

// 292 DNA structure 550, 17
struct SPHFluidSettings
{
    float    radius;             // 1 type, name 7, 279.
    float    spring_k;           // 2 type, name 7, 3356.
    float    rest_length;        // 3 type, name 7, 3339.
    float    plasticity_constant;// 4 type, name 7, 3357.
    float    yield_ratio;        // 5 type, name 7, 3358.
    float    plasticity_balance; // 6 type, name 7, 3359.
    float    yield_balance;      // 7 type, name 7, 3360.
    float    viscosity_omega;    // 8 type, name 7, 3361.
    float    viscosity_beta;     // 9 type, name 7, 3362.
    float    stiffness_k;        // 10 type, name 7, 3363.
    float    stiffness_knear;    // 11 type, name 7, 3364.
    float    rest_density;       // 12 type, name 7, 3365.
    float    buoyancy;           // 13 type, name 7, 3366.
    int      flag;               // 14 type, name 4, 18.
    int      spring_frames;      // 15 type, name 4, 3367.
    short    solver;             // 16 type, name 2, 2923.
    short    pad[3];             // 17 type, name 2, 44.
};

// 293 DNA structure 147, 46
struct ClothSimSettings
{
    void               *cache;           // Note: using void* on undefined DNA type: LinkNode
    float              mingoal;          // 2 type, name 7, 1263.
    float              Cdis;             // 3 type, name 7, 3503.
    float              Cvi;              // 4 type, name 7, 3504.
    float              gravity[3];       // 5 type, name 7, 1801.
    float              dt;               // 6 type, name 7, 1113.
    float              mass;             // 7 type, name 7, 1096.
    float              structural;       // 8 type, name 7, 3505.
    float              shear;            // 9 type, name 7, 646.
    float              bending;          // 10 type, name 7, 3506.
    float              max_bend;         // 11 type, name 7, 3507.
    float              max_struct;       // 12 type, name 7, 3508.
    float              max_shear;        // 13 type, name 7, 3509.
    float              max_sewing;       // 14 type, name 7, 3510.
    float              avg_spring_len;   // 15 type, name 7, 3511.
    float              timescale;        // 16 type, name 7, 3512.
    float              maxgoal;          // 17 type, name 7, 1264.
    float              eff_force_scale;  // 18 type, name 7, 3513.
    float              eff_wind_scale;   // 19 type, name 7, 3514.
    float              sim_time_old;     // 20 type, name 7, 3515.
    float              defgoal;          // 21 type, name 7, 1265.
    float              goalspring;       // 22 type, name 7, 1261.
    float              goalfrict;        // 23 type, name 7, 1262.
    float              velocity_smooth;  // 24 type, name 7, 3516.
    float              density_target;   // 25 type, name 7, 3517.
    float              density_strength; // 26 type, name 7, 3518.
    float              collider_friction;// 27 type, name 7, 3519.
    float              vel_damping;      // 28 type, name 7, 3520.
    float              shrink_min;       // 29 type, name 7, 3521.
    float              shrink_max;       // 30 type, name 7, 3522.
    float              bending_damping;  // 31 type, name 7, 3523.
    float              voxel_cell_size;  // 32 type, name 7, 3524.
    int                pad;              // 33 type, name 4, 68.
    int                stepsPerFrame;    // 34 type, name 4, 3525.
    int                flags;            // 35 type, name 4, 91.
    int                preroll;          // 36 type, name 4, 3526.
    int                maxspringlen;     // 37 type, name 4, 3527.
    short              solver_type;      // 38 type, name 2, 3528.
    short              vgroup_bend;      // 39 type, name 2, 3529.
    short              vgroup_mass;      // 40 type, name 2, 3530.
    short              vgroup_struct;    // 41 type, name 2, 3531.
    short              vgroup_shrink;    // 42 type, name 2, 3532.
    short              shapekey_rest;    // 43 type, name 2, 3533.
    short              presets;          // 44 type, name 2, 3534.
    short              reset;            // 45 type, name 2, 3535.
    EffectorWeights    *effector_weights;// 46 type, name 213, 1294.
};

// 294 DNA structure 148, 15
struct ClothCollSettings
{
    void        *collision_list;// Note: using void* on undefined DNA type: LinkNode
    float       epsilon;        // 2 type, name 7, 3537.
    float       self_friction;  // 3 type, name 7, 3538.
    float       friction;       // 4 type, name 7, 538.
    float       damping;        // 5 type, name 7, 1097.
    float       selfepsilon;    // 6 type, name 7, 3539.
    float       repel_force;    // 7 type, name 7, 3540.
    float       distance_repel; // 8 type, name 7, 3541.
    int         flags;          // 9 type, name 4, 91.
    short       self_loop_count;// 10 type, name 2, 3542.
    short       loop_count;     // 11 type, name 2, 3543.
    int         pad;            // 12 type, name 4, 68.
    Group       *group;         // 13 type, name 74, 537.
    short       vgroup_selfcol; // 14 type, name 2, 3544.
    short       pad2[3];        // 15 type, name 2, 3545.
};

// 295 DNA structure 559, 6
struct bGPDspoint
{
    float    x;       // 1 type, name 7, 5.
    float    y;       // 2 type, name 7, 6.
    float    z;       // 3 type, name 7, 7.
    float    pressure;// 4 type, name 7, 3321.
    float    time;    // 5 type, name 7, 801.
    int      flag;    // 6 type, name 4, 18.
};

// 296 DNA structure 560, 8
struct bGPDstroke
{
    bGPDstroke    *next;    // 1 type, name 560, 0.
    bGPDstroke    *prev;    // 2 type, name 560, 1.
    bGPDspoint    *points;  // 3 type, name 559, 2867.
    void          *pad;     // 4 type, name 11, 1423.
    int           totpoints;// 5 type, name 4, 281.
    short         thickness;// 6 type, name 2, 3546.
    short         flag;     // 7 type, name 2, 18.
    double        inittime; // 8 type, name 8, 3547.
};

// 297 DNA structure 571, 18
struct wmKeyMapItem
{
    wmKeyMapItem    *next;            // 1 type, name 571, 0.
    wmKeyMapItem    *prev;            // 2 type, name 571, 1.
    char            idname[64];       // 3 type, name 0, 2534.
    IDProperty      *properties;      // 4 type, name 21, 30.
    char            propvalue_str[64];// 5 type, name 0, 3610.
    short           propvalue;        // 6 type, name 2, 3611.
    short           type;             // 7 type, name 2, 16.
    short           val;              // 8 type, name 2, 14.
    short           shift;            // 9 type, name 2, 3612.
    short           ctrl;             // 10 type, name 2, 3613.
    short           alt;              // 11 type, name 2, 3614.
    short           oskey;            // 12 type, name 2, 3615.
    short           keymodifier;      // 13 type, name 2, 3616.
    short           flag;             // 14 type, name 2, 18.
    short           maptype;          // 15 type, name 2, 3617.
    short           id;               // 16 type, name 2, 31.
    short           pad;              // 17 type, name 2, 68.
    void            *ptr;             // Note: using void* on undefined DNA type: PointerRNA
};

// 298 DNA structure 573, 4
struct wmKeyMapDiffItem
{
    wmKeyMapDiffItem    *next;       // 1 type, name 573, 0.
    wmKeyMapDiffItem    *prev;       // 2 type, name 573, 1.
    wmKeyMapItem        *remove_item;// 3 type, name 571, 3619.
    wmKeyMapItem        *add_item;   // 4 type, name 571, 3620.
};

// 299 DNA structure 576, 11
struct FModifier
{
    FModifier    *next;    // 1 type, name 576, 0.
    FModifier    *prev;    // 2 type, name 576, 1.
    void         *data;    // 3 type, name 11, 2.
    char         name[64]; // 4 type, name 0, 19.
    short        type;     // 5 type, name 2, 16.
    short        flag;     // 6 type, name 2, 18.
    float        influence;// 7 type, name 7, 2811.
    float        sfra;     // 8 type, name 7, 144.
    float        efra;     // 9 type, name 7, 1272.
    float        blendin;  // 10 type, name 7, 2761.
    float        blendout; // 11 type, name 7, 3022.
};

// 300 DNA structure 577, 5
struct FMod_Generator
{
    float    *coefficients;// 1 type, name 7, 3635.
    int      arraysize;    // 2 type, name 4, 3636.
    int      poly_order;   // 3 type, name 4, 3637.
    int      mode;         // 4 type, name 4, 368.
    int      flag;         // 5 type, name 4, 18.
};

// 301 DNA structure 578, 6
struct FMod_FunctionGenerator
{
    float    amplitude;       // 1 type, name 7, 600.
    float    phase_multiplier;// 2 type, name 7, 3638.
    float    phase_offset;    // 3 type, name 7, 3639.
    float    value_offset;    // 4 type, name 7, 3640.
    int      type;            // 5 type, name 4, 16.
    int      flag;            // 6 type, name 4, 18.
};

// 302 DNA structure 579, 5
struct FCM_EnvelopeData
{
    float    min; // 1 type, name 7, 2789.
    float    max; // 2 type, name 7, 2790.
    float    time;// 3 type, name 7, 801.
    short    f1;  // 4 type, name 2, 594.
    short    f2;  // 5 type, name 2, 595.
};

// 303 DNA structure 580, 5
struct FMod_Envelope
{
    FCM_EnvelopeData    *data;  // 1 type, name 579, 2.
    int                 totvert;// 2 type, name 4, 55.
    float               midval; // 3 type, name 7, 3641.
    float               min;    // 4 type, name 7, 2789.
    float               max;    // 5 type, name 7, 2790.
};

// 304 DNA structure 581, 4
struct FMod_Cycles
{
    short    before_mode;  // 1 type, name 2, 3642.
    short    after_mode;   // 2 type, name 2, 3643.
    short    before_cycles;// 3 type, name 2, 3644.
    short    after_cycles; // 4 type, name 2, 3645.
};

// 305 DNA structure 582, 2
struct FMod_Python
{
    Text          *script;// 1 type, name 38, 2063.
    IDProperty    *prop;  // 2 type, name 21, 2378.
};

// 306 DNA structure 584, 6
struct FMod_Noise
{
    float    size;        // 1 type, name 7, 103.
    float    strength;    // 2 type, name 7, 767.
    float    phase;       // 3 type, name 7, 3647.
    float    offset;      // 4 type, name 7, 143.
    short    depth;       // 5 type, name 2, 274.
    short    modification;// 6 type, name 2, 3648.
};

// 307 DNA structure 585, 5
struct FMod_Stepped
{
    float    step_size;  // 1 type, name 7, 3649.
    float    offset;     // 2 type, name 7, 143.
    float    start_frame;// 3 type, name 7, 2868.
    float    end_frame;  // 4 type, name 7, 2869.
    int      flag;       // 5 type, name 4, 18.
};

// 308 DNA structure 586, 6
struct DriverTarget
{
    ID       *id;           // 1 type, name 22, 2068.
    char     *rna_path;     // 2 type, name 0, 3650.
    char     pchan_name[32];// 3 type, name 0, 3651.
    short    transChan;     // 4 type, name 2, 3652.
    short    flag;          // 5 type, name 2, 18.
    int      idtype;        // 6 type, name 4, 3653.
};

// 309 DNA structure 589, 3
struct FPoint
{
    float    vec[2];// 1 type, name 7, 3659.
    int      flag;  // 2 type, name 4, 18.
    int      pad;   // 3 type, name 4, 68.
};

// 310 DNA structure 591, 2
struct AnimMapPair
{
    char    from[128];// 1 type, name 0, 3664.
    char    to[128];  // 2 type, name 0, 3665.
};

// 311 DNA structure 595, 11
struct KS_Path
{
    KS_Path    *next;         // 1 type, name 595, 0.
    KS_Path    *prev;         // 2 type, name 595, 1.
    ID         *id;           // 3 type, name 22, 2068.
    char       group[64];     // 4 type, name 0, 3674.
    int        idtype;        // 5 type, name 4, 3653.
    short      groupmode;     // 6 type, name 2, 3675.
    short      flag;          // 7 type, name 2, 18.
    char       *rna_path;     // 8 type, name 0, 3650.
    int        array_index;   // 9 type, name 4, 3661.
    short      keyingflag;    // 10 type, name 2, 3676.
    short      keyingoverride;// 11 type, name 2, 3677.
};

// 312 DNA structure 597, 5
struct AnimOverride
{
    AnimOverride    *next;      // 1 type, name 597, 0.
    AnimOverride    *prev;      // 2 type, name 597, 1.
    char            *rna_path;  // 3 type, name 0, 3650.
    int             array_index;// 4 type, name 4, 3661.
    float           value;      // 5 type, name 7, 789.
};

// 313 DNA structure 599, 5
struct BoidRule
{
    BoidRule    *next;   // 1 type, name 599, 0.
    BoidRule    *prev;   // 2 type, name 599, 1.
    int         type;    // 3 type, name 4, 16.
    int         flag;    // 4 type, name 4, 18.
    char        name[32];// 5 type, name 0, 1601.
};

// 314 DNA structure 544, 4
struct BoidData
{
    float    health;  // 1 type, name 7, 3700.
    float    acc[3];  // 2 type, name 7, 3423.
    short    state_id;// 3 type, name 2, 3701.
    short    mode;    // 4 type, name 2, 368.
};

// 315 DNA structure 134, 26
struct SmokeFlowSettings
{
    SmokeModifierData    *smd;            // 1 type, name 132, 3723.
    void                 *dm;             // Note: using void* on undefined DNA type: DerivedMesh
    ParticleSystem       *psys;           // 3 type, name 162, 882.
    Tex                  *noise_texture;  // 4 type, name 57, 3775.
    float                *verts_old;      // 5 type, name 7, 3776.
    int                  numverts;        // 6 type, name 4, 856.
    float                vel_multi;       // 7 type, name 7, 3777.
    float                vel_normal;      // 8 type, name 7, 3778.
    float                vel_random;      // 9 type, name 7, 3779.
    float                density;         // 10 type, name 7, 426.
    float                color[3];        // 11 type, name 7, 112.
    float                fuel_amount;     // 12 type, name 7, 3780.
    float                temp;            // 13 type, name 7, 2503.
    float                volume_density;  // 14 type, name 7, 3781.
    float                surface_distance;// 15 type, name 7, 3782.
    float                particle_size;   // 16 type, name 7, 3783.
    int                  subframes;       // 17 type, name 4, 3379.
    float                texture_size;    // 18 type, name 7, 3784.
    float                texture_offset;  // 19 type, name 7, 3785.
    int                  pad;             // 20 type, name 4, 68.
    char                 uvlayer_name[64];// 21 type, name 0, 760.
    short                vgroup_density;  // 22 type, name 2, 3786.
    short                type;            // 23 type, name 2, 16.
    short                source;          // 24 type, name 2, 161.
    short                texture_type;    // 25 type, name 2, 3787.
    int                  flags;           // 26 type, name 4, 91.
};

// 316 DNA structure 135, 6
struct SmokeCollSettings
{
    SmokeModifierData    *smd;      // 1 type, name 132, 3723.
    void                 *dm;       // Note: using void* on undefined DNA type: DerivedMesh
    float                *verts_old;// 3 type, name 7, 3776.
    int                  numverts;  // 4 type, name 4, 856.
    short                type;      // 5 type, name 2, 16.
    short                pad;       // 6 type, name 2, 68.
};

// 317 DNA structure 268, 3
struct MovieClipUser
{
    int      framenr;    // 1 type, name 4, 141.
    short    render_size;// 2 type, name 2, 1975.
    short    render_flag;// 3 type, name 2, 3794.
};

// 318 DNA structure 609, 5
struct MovieClipProxy
{
    char     dir[768];       // 1 type, name 0, 2592.
    short    tc;             // 2 type, name 2, 2593.
    short    quality;        // 3 type, name 2, 1476.
    short    build_size_flag;// 4 type, name 2, 3795.
    short    build_tc_flag;  // 5 type, name 2, 3796.
};

// 319 DNA structure 614, 3
struct MovieReconstructedCamera
{
    int      framenr;  // 1 type, name 4, 141.
    float    error;    // 2 type, name 7, 3814.
    float    mat[4][4];// 3 type, name 7, 361.
};

// 320 DNA structure 615, 14
struct MovieTrackingCamera
{
    void     *intrinsics;     // 1 type, name 11, 3815.
    short    distortion_model;// 2 type, name 2, 3816.
    short    pad;             // 3 type, name 2, 68.
    float    sensor_width;    // 4 type, name 7, 3817.
    float    pixel_aspect;    // 5 type, name 7, 3818.
    float    focal;           // 6 type, name 7, 3819.
    short    units;           // 7 type, name 2, 3820.
    short    pad1;            // 8 type, name 2, 70.
    float    principal[2];    // 9 type, name 7, 3821.
    float    k1;              // 10 type, name 7, 3822.
    float    k2;              // 11 type, name 7, 3823.
    float    k3;              // 12 type, name 7, 3824.
    float    division_k1;     // 13 type, name 7, 3825.
    float    division_k2;     // 14 type, name 7, 3826.
};

// 321 DNA structure 612, 6
struct MovieTrackingMarker
{
    float    pos[2];               // 1 type, name 7, 3827.
    float    pattern_corners[4][2];// 2 type, name 7, 3828.
    float    search_min[2];        // 3 type, name 7, 3829.
    float    search_max[2];        // 4 type, name 7, 3830.
    int      framenr;              // 5 type, name 4, 141.
    int      flag;                 // 6 type, name 4, 18.
};

// 322 DNA structure 613, 26
struct MovieTrackingTrack
{
    MovieTrackingTrack     *next;              // 1 type, name 613, 0.
    MovieTrackingTrack     *prev;              // 2 type, name 613, 1.
    char                   name[64];           // 3 type, name 0, 19.
    float                  pat_min[2];         // 4 type, name 7, 3831.
    float                  pat_max[2];         // 5 type, name 7, 3832.
    float                  search_min[2];      // 6 type, name 7, 3829.
    float                  search_max[2];      // 7 type, name 7, 3830.
    float                  offset[2];          // 8 type, name 7, 3274.
    int                    markersnr;          // 9 type, name 4, 3833.
    int                    last_marker;        // 10 type, name 4, 3834.
    MovieTrackingMarker    *markers;           // 11 type, name 612, 3835.
    float                  bundle_pos[3];      // 12 type, name 7, 3836.
    float                  error;              // 13 type, name 7, 3814.
    int                    flag;               // 14 type, name 4, 18.
    int                    pat_flag;           // 15 type, name 4, 3837.
    int                    search_flag;        // 16 type, name 4, 3838.
    float                  color[3];           // 17 type, name 7, 112.
    short                  frames_limit;       // 18 type, name 2, 3839.
    short                  margin;             // 19 type, name 2, 1101.
    short                  pattern_match;      // 20 type, name 2, 3840.
    short                  motion_model;       // 21 type, name 2, 3841.
    int                    algorithm_flag;     // 22 type, name 4, 3842.
    float                  minimum_correlation;// 23 type, name 7, 3843.
    bGPdata                *gpd;               // 24 type, name 201, 1057.
    float                  weight;             // 25 type, name 7, 591.
    float                  pad;                // 26 type, name 7, 68.
};

// 323 DNA structure 616, 3
struct MovieTrackingPlaneMarker
{
    float    corners[4][2];// 1 type, name 7, 3844.
    int      framenr;      // 2 type, name 4, 141.
    int      flag;         // 3 type, name 4, 18.
};

// 324 DNA structure 617, 12
struct MovieTrackingPlaneTrack
{
    MovieTrackingPlaneTrack     *next;         // 1 type, name 617, 0.
    MovieTrackingPlaneTrack     *prev;         // 2 type, name 617, 1.
    char                        name[64];      // 3 type, name 0, 19.
    MovieTrackingTrack          **point_tracks;// 4 type, name 613, 3845.
    int                         point_tracksnr;// 5 type, name 4, 3846.
    int                         pad;           // 6 type, name 4, 68.
    MovieTrackingPlaneMarker    *markers;      // 7 type, name 616, 3835.
    int                         markersnr;     // 8 type, name 4, 3833.
    int                         flag;          // 9 type, name 4, 18.
    Image                       *image;        // 10 type, name 51, 805.
    float                       image_opacity; // 11 type, name 7, 3847.
    int                         last_marker;   // 12 type, name 4, 3834.
};

// 325 DNA structure 618, 24
struct MovieTrackingSettings
{
    int      flag;                       // 1 type, name 4, 18.
    short    default_motion_model;       // 2 type, name 2, 3848.
    short    default_algorithm_flag;     // 3 type, name 2, 3849.
    float    default_minimum_correlation;// 4 type, name 7, 3850.
    short    default_pattern_size;       // 5 type, name 2, 3851.
    short    default_search_size;        // 6 type, name 2, 3852.
    short    default_frames_limit;       // 7 type, name 2, 3853.
    short    default_margin;             // 8 type, name 2, 3854.
    short    default_pattern_match;      // 9 type, name 2, 3855.
    short    default_flag;               // 10 type, name 2, 3856.
    float    default_weight;             // 11 type, name 7, 944.
    short    motion_flag;                // 12 type, name 2, 3857.
    short    speed;                      // 13 type, name 2, 824.
    int      keyframe1;                  // 14 type, name 4, 3858.
    int      keyframe2;                  // 15 type, name 4, 3859.
    int      reconstruction_flag;        // 16 type, name 4, 3860.
    short    refine_camera_intrinsics;   // 17 type, name 2, 3861.
    short    pad2;                       // 18 type, name 2, 29.
    float    dist;                       // 19 type, name 7, 375.
    int      clean_frames;               // 20 type, name 4, 3862.
    int      clean_action;               // 21 type, name 4, 3863.
    float    clean_error;                // 22 type, name 7, 3864.
    float    object_distance;            // 23 type, name 7, 3865.
    int      pad3;                       // 24 type, name 4, 534.
};

// 326 DNA structure 619, 11
struct MovieTrackingStabilization
{
    int                   flag;      // 1 type, name 4, 18.
    int                   tot_track; // 2 type, name 4, 3866.
    int                   act_track; // 3 type, name 4, 3867.
    float                 maxscale;  // 4 type, name 7, 3868.
    MovieTrackingTrack    *rot_track;// 5 type, name 613, 3869.
    float                 locinf;    // 6 type, name 7, 3870.
    float                 scaleinf;  // 7 type, name 7, 3871.
    float                 rotinf;    // 8 type, name 7, 3872.
    int                   filter;    // 9 type, name 4, 475.
    int                   ok;        // 10 type, name 4, 147.
    float                 scale;     // 11 type, name 7, 972.
};

// 327 DNA structure 620, 5
struct MovieTrackingReconstruction
{
    int                         flag;       // 1 type, name 4, 18.
    float                       error;      // 2 type, name 7, 3814.
    int                         last_camera;// 3 type, name 4, 3873.
    int                         camnr;      // 4 type, name 4, 3874.
    MovieReconstructedCamera    *cameras;   // 5 type, name 614, 3875.
};

// 328 DNA structure 622, 1
struct MovieTrackingStats
{
    char    message[256];// 1 type, name 0, 3879.
};

// 329 DNA structure 623, 9
struct MovieTrackingDopesheetChannel
{
    MovieTrackingDopesheetChannel    *next;       // 1 type, name 623, 0.
    MovieTrackingDopesheetChannel    *prev;       // 2 type, name 623, 1.
    MovieTrackingTrack               *track;      // 3 type, name 613, 1050.
    int                              pad;         // 4 type, name 4, 68.
    char                             name[64];    // 5 type, name 0, 19.
    int                              tot_segment; // 6 type, name 4, 3880.
    int                              *segments;   // 7 type, name 4, 3881.
    int                              max_segment; // 8 type, name 4, 3882.
    int                              total_frames;// 9 type, name 4, 3883.
};

// 330 DNA structure 624, 6
struct MovieTrackingDopesheetCoverageSegment
{
    MovieTrackingDopesheetCoverageSegment    *next;      // 1 type, name 624, 0.
    MovieTrackingDopesheetCoverageSegment    *prev;      // 2 type, name 624, 1.
    int                                      coverage;   // 3 type, name 4, 3884.
    int                                      start_frame;// 4 type, name 4, 2868.
    int                                      end_frame;  // 5 type, name 4, 2869.
    int                                      pad;        // 6 type, name 4, 68.
};

// 331 DNA structure 182, 24
struct DynamicPaintBrushSettings
{
    DynamicPaintModifierData    *pmd;             // 1 type, name 180, 3928.
    void                        *dm;              // Note: using void* on undefined DNA type: DerivedMesh
    ParticleSystem              *psys;            // 3 type, name 162, 882.
    Material                    *mat;             // 4 type, name 73, 577.
    int                         flags;            // 5 type, name 4, 91.
    int                         collision;        // 6 type, name 4, 3932.
    float                       r;                // 7 type, name 7, 216.
    float                       g;                // 8 type, name 7, 217.
    float                       b;                // 9 type, name 7, 218.
    float                       alpha;            // 10 type, name 7, 463.
    float                       wetness;          // 11 type, name 7, 3933.
    float                       particle_radius;  // 12 type, name 7, 3934.
    float                       particle_smooth;  // 13 type, name 7, 3935.
    float                       paint_distance;   // 14 type, name 7, 3936.
    ColorBand                   *paint_ramp;      // 15 type, name 59, 3937.
    ColorBand                   *vel_ramp;        // 16 type, name 59, 3938.
    short                       proximity_falloff;// 17 type, name 2, 3939.
    short                       wave_type;        // 18 type, name 2, 3201.
    short                       ray_dir;          // 19 type, name 2, 3940.
    short                       pad;              // 20 type, name 2, 68.
    float                       wave_factor;      // 21 type, name 7, 3941.
    float                       wave_clamp;       // 22 type, name 7, 3942.
    float                       max_velocity;     // 23 type, name 7, 3943.
    float                       smudge_strength;  // 24 type, name 7, 3944.
};

// 332 DNA structure 628, 7
struct MaskParent
{
    int      id_type;                  // 1 type, name 4, 3948.
    int      type;                     // 2 type, name 4, 16.
    ID       *id;                      // 3 type, name 22, 2068.
    char     parent[64];               // 4 type, name 0, 3949.
    char     sub_parent[64];           // 5 type, name 0, 3950.
    float    parent_orig[2];           // 6 type, name 7, 3951.
    float    parent_corners_orig[4][2];// 7 type, name 7, 3952.
};

// 333 DNA structure 629, 3
struct MaskSplinePointUW
{
    float    u;   // 1 type, name 7, 3953.
    float    w;   // 2 type, name 7, 619.
    int      flag;// 3 type, name 4, 18.
};

// 334 DNA structure 632, 7
struct MaskLayerShape
{
    MaskLayerShape    *next;   // 1 type, name 632, 0.
    MaskLayerShape    *prev;   // 2 type, name 632, 1.
    float             *data;   // 3 type, name 7, 2.
    int               tot_vert;// 4 type, name 4, 3961.
    int               frame;   // 5 type, name 4, 1202.
    char              flag;    // 6 type, name 0, 18.
    char              pad[7];  // 7 type, name 0, 119.
};

// 335 DNA structure 208, 19
struct RigidBodyOb
{
    void     *physics_object; // 1 type, name 11, 3974.
    void     *physics_shape;  // 2 type, name 11, 3975.
    short    type;            // 3 type, name 2, 16.
    short    shape;           // 4 type, name 2, 1163.
    int      flag;            // 5 type, name 4, 18.
    int      col_groups;      // 6 type, name 4, 3976.
    short    mesh_source;     // 7 type, name 2, 3977.
    short    pad;             // 8 type, name 2, 68.
    float    mass;            // 9 type, name 7, 1096.
    float    friction;        // 10 type, name 7, 538.
    float    restitution;     // 11 type, name 7, 3978.
    float    margin;          // 12 type, name 7, 1101.
    float    lin_damping;     // 13 type, name 7, 3979.
    float    ang_damping;     // 14 type, name 7, 3980.
    float    lin_sleep_thresh;// 15 type, name 7, 3981.
    float    ang_sleep_thresh;// 16 type, name 7, 3982.
    float    orn[4];          // 17 type, name 7, 3983.
    float    pos[3];          // 18 type, name 7, 3984.
    float    pad1;            // 19 type, name 7, 70.
};

// 336 DNA structure 209, 30
struct RigidBodyCon
{
    Object    *ob1;                     // 1 type, name 29, 3985.
    Object    *ob2;                     // 2 type, name 29, 3986.
    short     type;                     // 3 type, name 2, 16.
    short     num_solver_iterations;    // 4 type, name 2, 3972.
    int       flag;                     // 5 type, name 4, 18.
    float     breaking_threshold;       // 6 type, name 7, 3987.
    float     pad;                      // 7 type, name 7, 68.
    float     limit_lin_x_lower;        // 8 type, name 7, 3988.
    float     limit_lin_x_upper;        // 9 type, name 7, 3989.
    float     limit_lin_y_lower;        // 10 type, name 7, 3990.
    float     limit_lin_y_upper;        // 11 type, name 7, 3991.
    float     limit_lin_z_lower;        // 12 type, name 7, 3992.
    float     limit_lin_z_upper;        // 13 type, name 7, 3993.
    float     limit_ang_x_lower;        // 14 type, name 7, 3994.
    float     limit_ang_x_upper;        // 15 type, name 7, 3995.
    float     limit_ang_y_lower;        // 16 type, name 7, 3996.
    float     limit_ang_y_upper;        // 17 type, name 7, 3997.
    float     limit_ang_z_lower;        // 18 type, name 7, 3998.
    float     limit_ang_z_upper;        // 19 type, name 7, 3999.
    float     spring_stiffness_x;       // 20 type, name 7, 4000.
    float     spring_stiffness_y;       // 21 type, name 7, 4001.
    float     spring_stiffness_z;       // 22 type, name 7, 4002.
    float     spring_damping_x;         // 23 type, name 7, 4003.
    float     spring_damping_y;         // 24 type, name 7, 4004.
    float     spring_damping_z;         // 25 type, name 7, 4005.
    float     motor_lin_target_velocity;// 26 type, name 7, 4006.
    float     motor_ang_target_velocity;// 27 type, name 7, 4007.
    float     motor_lin_max_impulse;    // 28 type, name 7, 4008.
    float     motor_ang_max_impulse;    // 29 type, name 7, 4009.
    void      *physics_constraint;      // 30 type, name 11, 4010.
};

// 337 DNA structure 634, 14
struct FreestyleLineSet
{
    FreestyleLineSet      *next;             // 1 type, name 634, 0.
    FreestyleLineSet      *prev;             // 2 type, name 634, 1.
    char                  name[64];          // 3 type, name 0, 19.
    int                   flags;             // 4 type, name 4, 91.
    int                   selection;         // 5 type, name 4, 4011.
    short                 qi;                // 6 type, name 2, 4012.
    short                 pad1;              // 7 type, name 2, 70.
    int                   qi_start;          // 8 type, name 4, 4013.
    int                   qi_end;            // 9 type, name 4, 4014.
    int                   edge_types;        // 10 type, name 4, 4015.
    int                   exclude_edge_types;// 11 type, name 4, 4016.
    int                   pad2;              // 12 type, name 4, 29.
    Group                 *group;            // 13 type, name 74, 537.
    FreestyleLineStyle    *linestyle;        // 14 type, name 635, 4017.
};

// 338 DNA structure 636, 5
struct FreestyleModuleConfig
{
    FreestyleModuleConfig    *next;       // 1 type, name 636, 0.
    FreestyleModuleConfig    *prev;       // 2 type, name 636, 1.
    Text                     *script;     // 3 type, name 38, 2063.
    short                    is_displayed;// 4 type, name 2, 4018.
    short                    pad[3];      // 5 type, name 2, 44.
};

// 339 DNA structure 637, 7
struct LineStyleModifier
{
    LineStyleModifier    *next;    // 1 type, name 637, 0.
    LineStyleModifier    *prev;    // 2 type, name 637, 1.
    char                 name[64]; // 3 type, name 0, 19.
    int                  type;     // 4 type, name 4, 16.
    float                influence;// 5 type, name 7, 2811.
    int                  flags;    // 6 type, name 4, 91.
    int                  blend;    // 7 type, name 4, 1844.
};

// 340 DNA structure 20, 4
struct IDPropertyData
{
    void        *pointer;// 1 type, name 11, 12.
    ListBase    group;   // 2 type, name 14, 13.
    int         val;     // 3 type, name 4, 14.
    int         val2;    // 4 type, name 4, 15.
};

// 341 DNA structure 23, 7
struct Library
{
    ID            id;            // 1 type, name 22, 31.
    ID            *idblock;      // 2 type, name 22, 32.
    void          *filedata;     // Note: using void* on undefined DNA type: FileData
    char          name[1024];    // 4 type, name 0, 34.
    char          filepath[1024];// 5 type, name 0, 35.
    Library       *parent;       // 6 type, name 23, 36.
    PackedFile    *packedfile;   // 7 type, name 25, 37.
};

// 342 DNA structure 59, 7
struct ColorBand
{
    short     tot;        // 1 type, name 2, 260.
    short     cur;        // 2 type, name 2, 65.
    char      ipotype;    // 3 type, name 0, 261.
    char      ipotype_hue;// 4 type, name 0, 262.
    char      color_mode; // 5 type, name 0, 263.
    char      pad[1];     // 6 type, name 0, 264.
    CBData    data[32];   // 7 type, name 58, 265.
};

// 343 DNA structure 69, 75
struct Lamp
{
    ID              id;                     // 1 type, name 22, 31.
    AnimData        *adt;                   // 2 type, name 36, 77.
    short           type;                   // 3 type, name 2, 16.
    short           flag;                   // 4 type, name 2, 18.
    int             mode;                   // 5 type, name 4, 368.
    short           colormodel;             // 6 type, name 2, 211.
    short           totex;                  // 7 type, name 2, 369.
    float           r;                      // 8 type, name 7, 216.
    float           g;                      // 9 type, name 7, 217.
    float           b;                      // 10 type, name 7, 218.
    float           k;                      // 11 type, name 7, 219.
    float           shdwr;                  // 12 type, name 7, 370.
    float           shdwg;                  // 13 type, name 7, 371.
    float           shdwb;                  // 14 type, name 7, 372.
    float           shdwpad;                // 15 type, name 7, 373.
    float           energy;                 // 16 type, name 7, 374.
    float           dist;                   // 17 type, name 7, 375.
    float           spotsize;               // 18 type, name 7, 376.
    float           spotblend;              // 19 type, name 7, 377.
    float           haint;                  // 20 type, name 7, 378.
    float           att1;                   // 21 type, name 7, 379.
    float           att2;                   // 22 type, name 7, 380.
    CurveMapping    *curfalloff;            // 23 type, name 63, 381.
    short           falloff_type;           // 24 type, name 2, 277.
    short           pad2;                   // 25 type, name 2, 29.
    float           clipsta;                // 26 type, name 7, 126.
    float           clipend;                // 27 type, name 7, 127.
    float           bias;                   // 28 type, name 7, 382.
    float           soft;                   // 29 type, name 7, 383.
    float           compressthresh;         // 30 type, name 7, 384.
    float           bleedbias;              // 31 type, name 7, 385.
    float           pad5;                   // 32 type, name 7, 386.
    short           bufsize;                // 33 type, name 2, 387.
    short           samp;                   // 34 type, name 2, 388.
    short           buffers;                // 35 type, name 2, 389.
    short           filtertype;             // 36 type, name 2, 390.
    char            bufflag;                // 37 type, name 0, 391.
    char            buftype;                // 38 type, name 0, 392.
    short           ray_samp;               // 39 type, name 2, 393.
    short           ray_sampy;              // 40 type, name 2, 394.
    short           ray_sampz;              // 41 type, name 2, 395.
    short           ray_samp_type;          // 42 type, name 2, 396.
    short           area_shape;             // 43 type, name 2, 397.
    float           area_size;              // 44 type, name 7, 398.
    float           area_sizey;             // 45 type, name 7, 399.
    float           area_sizez;             // 46 type, name 7, 400.
    float           adapt_thresh;           // 47 type, name 7, 401.
    short           ray_samp_method;        // 48 type, name 2, 402.
    short           shadowmap_type;         // 49 type, name 2, 403.
    short           texact;                 // 50 type, name 2, 404.
    short           shadhalostep;           // 51 type, name 2, 405.
    short           sun_effect_type;        // 52 type, name 2, 406.
    short           skyblendtype;           // 53 type, name 2, 407.
    float           horizon_brightness;     // 54 type, name 7, 408.
    float           spread;                 // 55 type, name 7, 409.
    float           sun_brightness;         // 56 type, name 7, 410.
    float           sun_size;               // 57 type, name 7, 411.
    float           backscattered_light;    // 58 type, name 7, 412.
    float           sun_intensity;          // 59 type, name 7, 413.
    float           atm_turbidity;          // 60 type, name 7, 414.
    float           atm_inscattering_factor;// 61 type, name 7, 415.
    float           atm_extinction_factor;  // 62 type, name 7, 416.
    float           atm_distance_factor;    // 63 type, name 7, 417.
    float           skyblendfac;            // 64 type, name 7, 418.
    float           sky_exposure;           // 65 type, name 7, 419.
    float           shadow_frustum_size;    // 66 type, name 7, 420.
    short           sky_colorspace;         // 67 type, name 2, 421.
    char            pad4[2];                // 68 type, name 0, 422.
    Ipo             *ipo;                   // 69 type, name 33, 82.
    MTex            *mtex[18];              // 70 type, name 56, 423.
    short           pr_texture;             // 71 type, name 2, 424.
    short           use_nodes;              // 72 type, name 2, 358.
    char            pad6[4];                // 73 type, name 0, 425.
    PreviewImage    *preview;               // 74 type, name 26, 172.
    bNodeTree       *nodetree;              // 75 type, name 66, 353.
};

// 344 DNA structure 75, 5
struct VFont
{
    ID            id;         // 1 type, name 22, 31.
    char          name[1024]; // 2 type, name 0, 34.
    void          *data;      // Note: using void* on undefined DNA type: VFontData
    PackedFile    *packedfile;// 4 type, name 25, 37.
    PackedFile    *temp_pf;   // 5 type, name 25, 566.
};

// 345 DNA structure 83, 4
struct EditNurb
{
    ListBase    nurbs;    // 1 type, name 14, 621.
    void        *keyindex;// Note: using void* on undefined DNA type: GHash
    int         shapenr;  // 3 type, name 4, 623.
    char        pad[4];   // 4 type, name 0, 602.
};

// 346 DNA structure 112, 1
struct MultiresColFace
{
    MultiresCol    col[4];// 1 type, name 111, 709.
};

// 347 DNA structure 122, 6
struct MappingInfoModifierData
{
    ModifierData    modifier;        // 1 type, name 121, 757.
    Tex             *texture;        // 2 type, name 57, 758.
    Object          *map_object;     // 3 type, name 29, 759.
    char            uvlayer_name[64];// 4 type, name 0, 760.
    int             uvlayer_tmp;     // 5 type, name 4, 761.
    int             texmapping;      // 6 type, name 4, 762.
};

// 348 DNA structure 123, 7
struct SubsurfModifierData
{
    ModifierData    modifier;    // 1 type, name 121, 757.
    short           subdivType;  // 2 type, name 2, 763.
    short           levels;      // 3 type, name 2, 744.
    short           renderLevels;// 4 type, name 2, 764.
    short           flags;       // 5 type, name 2, 91.
    void            *emCache;    // 6 type, name 11, 765.
    void            *mCache;     // 7 type, name 11, 766.
};

// 349 DNA structure 124, 5
struct LatticeModifierData
{
    ModifierData    modifier;// 1 type, name 121, 757.
    Object          *object; // 2 type, name 29, 196.
    char            name[64];// 3 type, name 0, 19.
    float           strength;// 4 type, name 7, 767.
    char            pad[4];  // 5 type, name 0, 602.
};

// 350 DNA structure 125, 5
struct CurveModifierData
{
    ModifierData    modifier;// 1 type, name 121, 757.
    Object          *object; // 2 type, name 29, 196.
    char            name[64];// 3 type, name 0, 19.
    short           defaxis; // 4 type, name 2, 768.
    char            pad[6];  // 5 type, name 0, 769.
};

// 351 DNA structure 126, 6
struct BuildModifierData
{
    ModifierData    modifier; // 1 type, name 121, 757.
    float           start;    // 2 type, name 7, 770.
    float           length;   // 3 type, name 7, 771.
    short           flag;     // 4 type, name 2, 18.
    short           randomize;// 5 type, name 2, 772.
    int             seed;     // 6 type, name 4, 773.
};

// 352 DNA structure 127, 5
struct MaskModifierData
{
    ModifierData    modifier;  // 1 type, name 121, 757.
    Object          *ob_arm;   // 2 type, name 29, 774.
    char            vgroup[64];// 3 type, name 0, 74.
    int             mode;      // 4 type, name 4, 368.
    int             flag;      // 5 type, name 4, 18.
};

// 353 DNA structure 128, 13
struct ArrayModifierData
{
    ModifierData    modifier;   // 1 type, name 121, 757.
    Object          *start_cap; // 2 type, name 29, 775.
    Object          *end_cap;   // 3 type, name 29, 776.
    Object          *curve_ob;  // 4 type, name 29, 777.
    Object          *offset_ob; // 5 type, name 29, 778.
    float           offset[3];  // 6 type, name 7, 779.
    float           scale[3];   // 7 type, name 7, 780.
    float           length;     // 8 type, name 7, 771.
    float           merge_dist; // 9 type, name 7, 781.
    int             fit_type;   // 10 type, name 4, 782.
    int             offset_type;// 11 type, name 4, 783.
    int             flags;      // 12 type, name 4, 91.
    int             count;      // 13 type, name 4, 784.
};

// 354 DNA structure 129, 5
struct MirrorModifierData
{
    ModifierData    modifier;  // 1 type, name 121, 757.
    short           axis;      // 2 type, name 2, 785.
    short           flag;      // 3 type, name 2, 18.
    float           tolerance; // 4 type, name 7, 786.
    Object          *mirror_ob;// 5 type, name 29, 787.
};

// 355 DNA structure 130, 3
struct EdgeSplitModifierData
{
    ModifierData    modifier;   // 1 type, name 121, 757.
    float           split_angle;// 2 type, name 7, 788.
    int             flags;      // 3 type, name 4, 91.
};

// 356 DNA structure 131, 13
struct BevelModifierData
{
    ModifierData    modifier;       // 1 type, name 121, 757.
    float           value;          // 2 type, name 7, 789.
    int             res;            // 3 type, name 4, 790.
    short           flags;          // 4 type, name 2, 91.
    short           val_flags;      // 5 type, name 2, 791.
    short           lim_flags;      // 6 type, name 2, 792.
    short           e_flags;        // 7 type, name 2, 793.
    short           mat;            // 8 type, name 2, 794.
    short           pad;            // 9 type, name 2, 68.
    int             pad2;           // 10 type, name 4, 29.
    float           profile;        // 11 type, name 7, 795.
    float           bevel_angle;    // 12 type, name 7, 796.
    char            defgrp_name[64];// 13 type, name 0, 797.
};

// 357 DNA structure 132, 6
struct SmokeModifierData
{
    ModifierData           modifier;// 1 type, name 121, 757.
    SmokeDomainSettings    *domain; // 2 type, name 133, 798.
    SmokeFlowSettings      *flow;   // 3 type, name 134, 799.
    SmokeCollSettings      *coll;   // 4 type, name 135, 800.
    float                  time;    // 5 type, name 7, 801.
    int                    type;    // 6 type, name 4, 16.
};

// 358 DNA structure 136, 11
struct DisplaceModifierData
{
    ModifierData    modifier;        // 1 type, name 121, 757.
    Tex             *texture;        // 2 type, name 57, 758.
    Object          *map_object;     // 3 type, name 29, 759.
    char            uvlayer_name[64];// 4 type, name 0, 760.
    int             uvlayer_tmp;     // 5 type, name 4, 761.
    int             texmapping;      // 6 type, name 4, 762.
    float           strength;        // 7 type, name 7, 767.
    int             direction;       // 8 type, name 4, 802.
    char            defgrp_name[64]; // 9 type, name 0, 797.
    float           midlevel;        // 10 type, name 7, 803.
    int             pad;             // 11 type, name 4, 68.
};

// 359 DNA structure 137, 12
struct UVProjectModifierData
{
    ModifierData    modifier;        // 1 type, name 121, 757.
    Object          *projectors[10]; // 2 type, name 29, 804.
    Image           *image;          // 3 type, name 51, 805.
    int             flags;           // 4 type, name 4, 91.
    int             num_projectors;  // 5 type, name 4, 806.
    float           aspectx;         // 6 type, name 7, 807.
    float           aspecty;         // 7 type, name 7, 808.
    float           scalex;          // 8 type, name 7, 809.
    float           scaley;          // 9 type, name 7, 810.
    char            uvlayer_name[64];// 10 type, name 0, 760.
    int             uvlayer_tmp;     // 11 type, name 4, 761.
    int             pad;             // 12 type, name 4, 68.
};

// 360 DNA structure 138, 11
struct DecimateModifierData
{
    ModifierData    modifier;       // 1 type, name 121, 757.
    float           percent;        // 2 type, name 7, 811.
    short           iter;           // 3 type, name 2, 812.
    char            delimit;        // 4 type, name 0, 813.
    char            pad;            // 5 type, name 0, 68.
    float           angle;          // 6 type, name 7, 814.
    char            defgrp_name[64];// 7 type, name 0, 797.
    float           defgrp_factor;  // 8 type, name 7, 815.
    short           flag;           // 9 type, name 2, 18.
    short           mode;           // 10 type, name 2, 368.
    int             face_count;     // 11 type, name 4, 816.
};

// 361 DNA structure 139, 5
struct SmoothModifierData
{
    ModifierData    modifier;       // 1 type, name 121, 757.
    float           fac;            // 2 type, name 7, 817.
    char            defgrp_name[64];// 3 type, name 0, 797.
    short           flag;           // 4 type, name 2, 18.
    short           repeat;         // 5 type, name 2, 818.
};

// 362 DNA structure 140, 8
struct CastModifierData
{
    ModifierData    modifier;       // 1 type, name 121, 757.
    Object          *object;        // 2 type, name 29, 196.
    float           fac;            // 3 type, name 7, 817.
    float           radius;         // 4 type, name 7, 279.
    float           size;           // 5 type, name 7, 103.
    char            defgrp_name[64];// 6 type, name 0, 797.
    short           flag;           // 7 type, name 2, 18.
    short           type;           // 8 type, name 2, 16.
};

// 363 DNA structure 141, 21
struct WaveModifierData
{
    ModifierData    modifier;        // 1 type, name 121, 757.
    Tex             *texture;        // 2 type, name 57, 758.
    Object          *map_object;     // 3 type, name 29, 759.
    char            uvlayer_name[64];// 4 type, name 0, 760.
    int             uvlayer_tmp;     // 5 type, name 4, 761.
    int             texmapping;      // 6 type, name 4, 762.
    Object          *objectcenter;   // 7 type, name 29, 819.
    char            defgrp_name[64]; // 8 type, name 0, 797.
    short           flag;            // 9 type, name 2, 18.
    short           pad;             // 10 type, name 2, 68.
    float           startx;          // 11 type, name 7, 820.
    float           starty;          // 12 type, name 7, 821.
    float           height;          // 13 type, name 7, 822.
    float           width;           // 14 type, name 7, 636.
    float           narrow;          // 15 type, name 7, 823.
    float           speed;           // 16 type, name 7, 824.
    float           damp;            // 17 type, name 7, 825.
    float           falloff;         // 18 type, name 7, 826.
    float           timeoffs;        // 19 type, name 7, 827.
    float           lifetime;        // 20 type, name 7, 828.
    float           pad1;            // 21 type, name 7, 70.
};

// 364 DNA structure 142, 7
struct ArmatureModifierData
{
    ModifierData    modifier;       // 1 type, name 121, 757.
    short           deformflag;     // 2 type, name 2, 829.
    short           multi;          // 3 type, name 2, 830.
    int             pad2;           // 4 type, name 4, 29.
    Object          *object;        // 5 type, name 29, 196.
    float           *prevCos;       // 6 type, name 7, 831.
    char            defgrp_name[64];// 7 type, name 0, 797.
};

// 365 DNA structure 143, 14
struct HookModifierData
{
    ModifierData    modifier;       // 1 type, name 121, 757.
    Object          *object;        // 2 type, name 29, 196.
    char            subtarget[64];  // 3 type, name 0, 832.
    char            flag;           // 4 type, name 0, 18.
    char            falloff_type;   // 5 type, name 0, 277.
    char            pad[6];         // 6 type, name 0, 769.
    float           parentinv[4][4];// 7 type, name 7, 833.
    float           cent[3];        // 8 type, name 7, 834.
    float           falloff;        // 9 type, name 7, 826.
    CurveMapping    *curfalloff;    // 10 type, name 63, 381.
    int             *indexar;       // 11 type, name 4, 835.
    int             totindex;       // 12 type, name 4, 836.
    float           force;          // 13 type, name 7, 837.
    char            name[64];       // 14 type, name 0, 19.
};

// 366 DNA structure 144, 1
struct SoftbodyModifierData
{
    ModifierData    modifier;// 1 type, name 121, 757.
};

// 367 DNA structure 152, 13
struct CollisionModifierData
{
    ModifierData    modifier;     // 1 type, name 121, 757.
    MVert           *x;           // 2 type, name 97, 849.
    MVert           *xnew;        // 3 type, name 97, 850.
    MVert           *xold;        // 4 type, name 97, 851.
    MVert           *current_xnew;// 5 type, name 97, 852.
    MVert           *current_x;   // 6 type, name 97, 853.
    MVert           *current_v;   // 7 type, name 97, 854.
    MFace           *mfaces;      // 8 type, name 94, 855.
    int             numverts;     // 9 type, name 4, 856.
    int             numfaces;     // 10 type, name 4, 857.
    float           time_x;       // 11 type, name 7, 858.
    float           time_xnew;    // 12 type, name 7, 859.
    void            *bvhtree;     // Note: using void* on undefined DNA type: BVHTree
};

// 368 DNA structure 154, 7
struct SurfaceModifierData
{
    ModifierData       modifier;// 1 type, name 121, 757.
    MVert              *x;      // 2 type, name 97, 849.
    MVert              *v;      // 3 type, name 97, 861.
    void               *dm;     // Note: using void* on undefined DNA type: DerivedMesh
    void               *bvhtree;// Note: using void* on undefined DNA type: BVHTreeFromMesh
    int                cfra;    // 6 type, name 4, 863.
    int                numverts;// 7 type, name 4, 856.
};

// 369 DNA structure 157, 4
struct BooleanModifierData
{
    ModifierData    modifier; // 1 type, name 121, 757.
    Object          *object;  // 2 type, name 29, 196.
    int             operation;// 3 type, name 4, 864.
    int             pad;      // 4 type, name 4, 68.
};

// 370 DNA structure 160, 22
struct MeshDeformModifierData
{
    ModifierData     modifier;       // 1 type, name 121, 757.
    Object           *object;        // 2 type, name 29, 196.
    char             defgrp_name[64];// 3 type, name 0, 797.
    short            gridsize;       // 4 type, name 2, 867.
    short            flag;           // 5 type, name 2, 18.
    short            pad[2];         // 6 type, name 2, 205.
    MDefInfluence    *bindinfluences;// 7 type, name 158, 868.
    int              *bindoffsets;   // 8 type, name 4, 869.
    float            *bindcagecos;   // 9 type, name 7, 870.
    int              totvert;        // 10 type, name 4, 55.
    int              totcagevert;    // 11 type, name 4, 871.
    MDefCell         *dyngrid;       // 12 type, name 159, 872.
    MDefInfluence    *dyninfluences; // 13 type, name 158, 873.
    int              *dynverts;      // 14 type, name 4, 874.
    int              dyngridsize;    // 15 type, name 4, 875.
    int              totinfluence;   // 16 type, name 4, 866.
    float            dyncellmin[3];  // 17 type, name 7, 876.
    float            dyncellwidth;   // 18 type, name 7, 877.
    float            bindmat[4][4];  // 19 type, name 7, 878.
    float            *bindweights;   // 20 type, name 7, 879.
    float            *bindcos;       // 21 type, name 7, 880.
    void             (*bindfunc)();  // 22 type, name 11, 881.
};

// 371 DNA structure 161, 8
struct ParticleSystemModifierData
{
    ModifierData      modifier; // 1 type, name 121, 757.
    ParticleSystem    *psys;    // 2 type, name 162, 882.
    void              *dm;      // Note: using void* on undefined DNA type: DerivedMesh
    int               totdmvert;// 4 type, name 4, 883.
    int               totdmedge;// 5 type, name 4, 884.
    int               totdmface;// 6 type, name 4, 885.
    short             flag;     // 7 type, name 2, 18.
    short             pad;      // 8 type, name 2, 68.
};

// 372 DNA structure 163, 8
struct ParticleInstanceModifierData
{
    ModifierData    modifier;       // 1 type, name 121, 757.
    Object          *ob;            // 2 type, name 29, 46.
    short           psys;           // 3 type, name 2, 283.
    short           flag;           // 4 type, name 2, 18.
    short           axis;           // 5 type, name 2, 785.
    short           pad;            // 6 type, name 2, 68.
    float           position;       // 7 type, name 7, 886.
    float           random_position;// 8 type, name 7, 887.
};

// 373 DNA structure 164, 6
struct ExplodeModifierData
{
    ModifierData    modifier;  // 1 type, name 121, 757.
    int             *facepa;   // 2 type, name 4, 888.
    short           flag;      // 3 type, name 2, 18.
    short           vgroup;    // 4 type, name 2, 889.
    float           protect;   // 5 type, name 7, 890.
    char            uvname[64];// 6 type, name 0, 198.
};

// 374 DNA structure 165, 8
struct MultiresModifierData
{
    ModifierData    modifier; // 1 type, name 121, 757.
    char            lvl;      // 2 type, name 0, 891.
    char            sculptlvl;// 3 type, name 0, 892.
    char            renderlvl;// 4 type, name 0, 750.
    char            totlvl;   // 5 type, name 0, 893.
    char            simple;   // 6 type, name 0, 894.
    char            flags;    // 7 type, name 0, 91.
    char            pad[2];   // 8 type, name 0, 205.
};

// 375 DNA structure 166, 3
struct FluidsimModifierData
{
    ModifierData        modifier;    // 1 type, name 121, 757.
    FluidsimSettings    *fss;        // 2 type, name 167, 895.
    PointCache          *point_cache;// 3 type, name 149, 841.
};

// 376 DNA structure 168, 12
struct ShrinkwrapModifierData
{
    ModifierData    modifier;       // 1 type, name 121, 757.
    Object          *target;        // 2 type, name 29, 896.
    Object          *auxTarget;     // 3 type, name 29, 897.
    char            vgroup_name[64];// 4 type, name 0, 898.
    float           keepDist;       // 5 type, name 7, 899.
    short           shrinkType;     // 6 type, name 2, 900.
    char            shrinkOpts;     // 7 type, name 0, 901.
    char            pad1;           // 8 type, name 0, 70.
    float           projLimit;      // 9 type, name 7, 902.
    char            projAxis;       // 10 type, name 0, 903.
    char            subsurfLevels;  // 11 type, name 0, 904.
    char            pad[2];         // 12 type, name 0, 205.
};

// 377 DNA structure 169, 8
struct SimpleDeformModifierData
{
    ModifierData    modifier;       // 1 type, name 121, 757.
    Object          *origin;        // 2 type, name 29, 905.
    char            vgroup_name[64];// 3 type, name 0, 898.
    float           factor;         // 4 type, name 7, 111.
    float           limit[2];       // 5 type, name 7, 906.
    char            mode;           // 6 type, name 0, 368.
    char            axis;           // 7 type, name 0, 785.
    char            pad[2];         // 8 type, name 0, 205.
};

// 378 DNA structure 170, 1
struct ShapeKeyModifierData
{
    ModifierData    modifier;// 1 type, name 121, 757.
};

// 379 DNA structure 171, 13
struct SolidifyModifierData
{
    ModifierData    modifier;       // 1 type, name 121, 757.
    char            defgrp_name[64];// 2 type, name 0, 797.
    float           offset;         // 3 type, name 7, 143.
    float           offset_fac;     // 4 type, name 7, 907.
    float           offset_fac_vg;  // 5 type, name 7, 908.
    float           offset_clamp;   // 6 type, name 7, 909.
    float           pad;            // 7 type, name 7, 68.
    float           crease_inner;   // 8 type, name 7, 910.
    float           crease_outer;   // 9 type, name 7, 911.
    float           crease_rim;     // 10 type, name 7, 912.
    int             flag;           // 11 type, name 4, 18.
    short           mat_ofs;        // 12 type, name 2, 913.
    short           mat_ofs_rim;    // 13 type, name 2, 914.
};

// 380 DNA structure 172, 10
struct ScrewModifierData
{
    ModifierData    modifier;    // 1 type, name 121, 757.
    Object          *ob_axis;    // 2 type, name 29, 915.
    int             steps;       // 3 type, name 4, 916.
    int             render_steps;// 4 type, name 4, 917.
    int             iter;        // 5 type, name 4, 812.
    float           screw_ofs;   // 6 type, name 7, 918.
    float           angle;       // 7 type, name 7, 814.
    char            axis;        // 8 type, name 0, 785.
    char            pad;         // 9 type, name 0, 68.
    short           flag;        // 10 type, name 2, 18.
};

// 381 DNA structure 173, 29
struct OceanModifierData
{
    ModifierData    modifier;         // 1 type, name 121, 757.
    void            *ocean;           // Note: using void* on undefined DNA type: Ocean
    void            *oceancache;      // Note: using void* on undefined DNA type: OceanCache
    int             resolution;       // 4 type, name 4, 921.
    int             spatial_size;     // 5 type, name 4, 922.
    float           wind_velocity;    // 6 type, name 7, 923.
    float           damp;             // 7 type, name 7, 825.
    float           smallest_wave;    // 8 type, name 7, 924.
    float           depth;            // 9 type, name 7, 274.
    float           wave_alignment;   // 10 type, name 7, 925.
    float           wave_direction;   // 11 type, name 7, 926.
    float           wave_scale;       // 12 type, name 7, 927.
    float           chop_amount;      // 13 type, name 7, 928.
    float           foam_coverage;    // 14 type, name 7, 929.
    float           time;             // 15 type, name 7, 801.
    int             bakestart;        // 16 type, name 4, 930.
    int             bakeend;          // 17 type, name 4, 931.
    char            cachepath[1024];  // 18 type, name 0, 932.
    char            foamlayername[64];// 19 type, name 0, 933.
    char            cached;           // 20 type, name 0, 934.
    char            geometry_mode;    // 21 type, name 0, 935.
    char            flag;             // 22 type, name 0, 18.
    char            refresh;          // 23 type, name 0, 936.
    short           repeat_x;         // 24 type, name 2, 937.
    short           repeat_y;         // 25 type, name 2, 938.
    int             seed;             // 26 type, name 4, 773.
    float           size;             // 27 type, name 7, 103.
    float           foam_fade;        // 28 type, name 7, 939.
    int             pad;              // 29 type, name 4, 68.
};

// 382 DNA structure 176, 15
struct WarpModifierData
{
    ModifierData    modifier;        // 1 type, name 121, 757.
    Tex             *texture;        // 2 type, name 57, 758.
    Object          *map_object;     // 3 type, name 29, 759.
    char            uvlayer_name[64];// 4 type, name 0, 760.
    int             uvlayer_tmp;     // 5 type, name 4, 761.
    int             texmapping;      // 6 type, name 4, 762.
    Object          *object_from;    // 7 type, name 29, 940.
    Object          *object_to;      // 8 type, name 29, 941.
    CurveMapping    *curfalloff;     // 9 type, name 63, 381.
    char            defgrp_name[64]; // 10 type, name 0, 797.
    float           strength;        // 11 type, name 7, 767.
    float           falloff_radius;  // 12 type, name 7, 942.
    char            flag;            // 13 type, name 0, 18.
    char            falloff_type;    // 14 type, name 0, 277.
    char            pad[6];          // 15 type, name 0, 769.
};

// 383 DNA structure 177, 16
struct WeightVGEditModifierData
{
    ModifierData    modifier;                 // 1 type, name 121, 757.
    char            defgrp_name[64];          // 2 type, name 0, 797.
    short           edit_flags;               // 3 type, name 2, 943.
    short           falloff_type;             // 4 type, name 2, 277.
    float           default_weight;           // 5 type, name 7, 944.
    CurveMapping    *cmap_curve;              // 6 type, name 63, 945.
    float           add_threshold;            // 7 type, name 7, 946.
    float           rem_threshold;            // 8 type, name 7, 947.
    float           mask_constant;            // 9 type, name 7, 948.
    char            mask_defgrp_name[64];     // 10 type, name 0, 949.
    int             mask_tex_use_channel;     // 11 type, name 4, 950.
    Tex             *mask_texture;            // 12 type, name 57, 951.
    Object          *mask_tex_map_obj;        // 13 type, name 29, 952.
    int             mask_tex_mapping;         // 14 type, name 4, 953.
    char            mask_tex_uvlayer_name[64];// 15 type, name 0, 954.
    int             pad_i1;                   // 16 type, name 4, 955.
};

// 384 DNA structure 178, 16
struct WeightVGMixModifierData
{
    ModifierData    modifier;                 // 1 type, name 121, 757.
    char            defgrp_name_a[64];        // 2 type, name 0, 956.
    char            defgrp_name_b[64];        // 3 type, name 0, 957.
    float           default_weight_a;         // 4 type, name 7, 958.
    float           default_weight_b;         // 5 type, name 7, 959.
    char            mix_mode;                 // 6 type, name 0, 960.
    char            mix_set;                  // 7 type, name 0, 961.
    char            pad_c1[6];                // 8 type, name 0, 962.
    float           mask_constant;            // 9 type, name 7, 948.
    char            mask_defgrp_name[64];     // 10 type, name 0, 949.
    int             mask_tex_use_channel;     // 11 type, name 4, 950.
    Tex             *mask_texture;            // 12 type, name 57, 951.
    Object          *mask_tex_map_obj;        // 13 type, name 29, 952.
    int             mask_tex_mapping;         // 14 type, name 4, 953.
    char            mask_tex_uvlayer_name[64];// 15 type, name 0, 954.
    int             pad_i1;                   // 16 type, name 4, 955.
};

// 385 DNA structure 179, 16
struct WeightVGProximityModifierData
{
    ModifierData    modifier;                 // 1 type, name 121, 757.
    char            defgrp_name[64];          // 2 type, name 0, 797.
    int             proximity_mode;           // 3 type, name 4, 963.
    int             proximity_flags;          // 4 type, name 4, 964.
    Object          *proximity_ob_target;     // 5 type, name 29, 965.
    float           mask_constant;            // 6 type, name 7, 948.
    char            mask_defgrp_name[64];     // 7 type, name 0, 949.
    int             mask_tex_use_channel;     // 8 type, name 4, 950.
    Tex             *mask_texture;            // 9 type, name 57, 951.
    Object          *mask_tex_map_obj;        // 10 type, name 29, 952.
    int             mask_tex_mapping;         // 11 type, name 4, 953.
    char            mask_tex_uvlayer_name[64];// 12 type, name 0, 954.
    float           min_dist;                 // 13 type, name 7, 966.
    float           max_dist;                 // 14 type, name 7, 967.
    short           falloff_type;             // 15 type, name 2, 277.
    short           pad_s1;                   // 16 type, name 2, 968.
};

// 386 DNA structure 180, 5
struct DynamicPaintModifierData
{
    ModifierData                  modifier;// 1 type, name 121, 757.
    DynamicPaintCanvasSettings    *canvas; // 2 type, name 181, 969.
    DynamicPaintBrushSettings     *brush;  // 3 type, name 182, 970.
    int                           type;    // 4 type, name 4, 16.
    int                           pad;     // 5 type, name 4, 68.
};

// 387 DNA structure 183, 8
struct RemeshModifierData
{
    ModifierData    modifier;   // 1 type, name 121, 757.
    float           threshold;  // 2 type, name 7, 971.
    float           scale;      // 3 type, name 7, 972.
    float           hermite_num;// 4 type, name 7, 973.
    char            depth;      // 5 type, name 0, 274.
    char            flag;       // 6 type, name 0, 18.
    char            mode;       // 7 type, name 0, 368.
    char            pad;        // 8 type, name 0, 68.
};

// 388 DNA structure 184, 5
struct SkinModifierData
{
    ModifierData    modifier;        // 1 type, name 121, 757.
    float           branch_smoothing;// 2 type, name 7, 974.
    char            flag;            // 3 type, name 0, 18.
    char            symmetry_axes;   // 4 type, name 0, 975.
    char            pad[2];          // 5 type, name 0, 205.
};

// 389 DNA structure 185, 5
struct TriangulateModifierData
{
    ModifierData    modifier;   // 1 type, name 121, 757.
    int             flag;       // 2 type, name 4, 18.
    int             quad_method;// 3 type, name 4, 976.
    int             ngon_method;// 4 type, name 4, 977.
    int             pad;        // 5 type, name 4, 68.
};

// 390 DNA structure 186, 7
struct LaplacianSmoothModifierData
{
    ModifierData    modifier;       // 1 type, name 121, 757.
    float           lambda;         // 2 type, name 7, 978.
    float           lambda_border;  // 3 type, name 7, 979.
    float           pad1;           // 4 type, name 7, 70.
    char            defgrp_name[64];// 5 type, name 0, 797.
    short           flag;           // 6 type, name 2, 18.
    short           repeat;         // 7 type, name 2, 818.
};

// 391 DNA structure 187, 13
struct CorrectiveSmoothModifierData
{
    ModifierData    modifier;        // 1 type, name 121, 757.
    float           (*bind_coords)();// 2 type, name 7, 980.
    int             bind_coords_num; // 3 type, name 4, 981.
    float           lambda;          // 4 type, name 7, 978.
    short           repeat;          // 5 type, name 2, 818.
    short           flag;            // 6 type, name 2, 18.
    char            smooth_type;     // 7 type, name 0, 982.
    char            rest_source;     // 8 type, name 0, 983.
    char            pad[2];          // 9 type, name 0, 205.
    char            defgrp_name[64]; // 10 type, name 0, 797.
    float           (*delta_cache)();// 11 type, name 7, 984.
    int             delta_cache_num; // 12 type, name 4, 985.
    char            pad2[4];         // 13 type, name 0, 986.
};

// 392 DNA structure 188, 11
struct UVWarpModifierData
{
    ModifierData    modifier;        // 1 type, name 121, 757.
    char            axis_u;          // 2 type, name 0, 987.
    char            axis_v;          // 3 type, name 0, 988.
    char            pad[6];          // 4 type, name 0, 769.
    float           center[2];       // 5 type, name 7, 989.
    Object          *object_src;     // 6 type, name 29, 990.
    char            bone_src[64];    // 7 type, name 0, 991.
    Object          *object_dst;     // 8 type, name 29, 992.
    char            bone_dst[64];    // 9 type, name 0, 993.
    char            vgroup_name[64]; // 10 type, name 0, 898.
    char            uvlayer_name[64];// 11 type, name 0, 760.
};

// 393 DNA structure 189, 18
struct MeshCacheModifierData
{
    ModifierData    modifier;      // 1 type, name 121, 757.
    char            flag;          // 2 type, name 0, 18.
    char            type;          // 3 type, name 0, 16.
    char            time_mode;     // 4 type, name 0, 994.
    char            play_mode;     // 5 type, name 0, 995.
    char            forward_axis;  // 6 type, name 0, 996.
    char            up_axis;       // 7 type, name 0, 997.
    char            flip_axis;     // 8 type, name 0, 998.
    char            interp;        // 9 type, name 0, 999.
    float           factor;        // 10 type, name 7, 111.
    char            deform_mode;   // 11 type, name 0, 1000.
    char            pad[7];        // 12 type, name 0, 119.
    float           frame_start;   // 13 type, name 7, 1001.
    float           frame_scale;   // 14 type, name 7, 1002.
    float           eval_frame;    // 15 type, name 7, 1003.
    float           eval_time;     // 16 type, name 7, 1004.
    float           eval_factor;   // 17 type, name 7, 1005.
    char            filepath[1024];// 18 type, name 0, 35.
};

// 394 DNA structure 190, 8
struct LaplacianDeformModifierData
{
    ModifierData    modifier;           // 1 type, name 121, 757.
    char            anchor_grp_name[64];// 2 type, name 0, 1006.
    int             total_verts;        // 3 type, name 4, 1007.
    int             repeat;             // 4 type, name 4, 818.
    float           *vertexco;          // 5 type, name 7, 1008.
    void            *cache_system;      // 6 type, name 11, 1009.
    short           flag;               // 7 type, name 2, 18.
    short           pad[3];             // 8 type, name 2, 44.
};

// 395 DNA structure 191, 9
struct WireframeModifierData
{
    ModifierData    modifier;       // 1 type, name 121, 757.
    char            defgrp_name[64];// 2 type, name 0, 797.
    float           offset;         // 3 type, name 7, 143.
    float           offset_fac;     // 4 type, name 7, 907.
    float           offset_fac_vg;  // 5 type, name 7, 908.
    float           crease_weight;  // 6 type, name 7, 1010.
    short           flag;           // 7 type, name 2, 18.
    short           mat_ofs;        // 8 type, name 2, 913.
    short           pad[2];         // 9 type, name 2, 205.
};

// 396 DNA structure 192, 17
struct DataTransferModifierData
{
    ModifierData    modifier;            // 1 type, name 121, 757.
    Object          *ob_source;          // 2 type, name 29, 1011.
    int             data_types;          // 3 type, name 4, 1012.
    int             vmap_mode;           // 4 type, name 4, 1013.
    int             emap_mode;           // 5 type, name 4, 1014.
    int             lmap_mode;           // 6 type, name 4, 1015.
    int             pmap_mode;           // 7 type, name 4, 1016.
    float           map_max_distance;    // 8 type, name 7, 1017.
    float           map_ray_radius;      // 9 type, name 7, 1018.
    float           islands_precision;   // 10 type, name 7, 1019.
    int             pad_i1;              // 11 type, name 4, 955.
    int             layers_select_src[4];// 12 type, name 4, 1020.
    int             layers_select_dst[4];// 13 type, name 4, 1021.
    int             mix_mode;            // 14 type, name 4, 960.
    float           mix_factor;          // 15 type, name 7, 1022.
    char            defgrp_name[64];     // 16 type, name 0, 797.
    int             flags;               // 17 type, name 4, 91.
};

// 397 DNA structure 193, 9
struct NormalEditModifierData
{
    ModifierData    modifier;       // 1 type, name 121, 757.
    char            defgrp_name[64];// 2 type, name 0, 797.
    Object          *target;        // 3 type, name 29, 896.
    short           mode;           // 4 type, name 2, 368.
    short           flag;           // 5 type, name 2, 18.
    short           mix_mode;       // 6 type, name 2, 960.
    char            pad[2];         // 7 type, name 0, 205.
    float           mix_factor;     // 8 type, name 7, 1022.
    float           offset[3];      // 9 type, name 7, 779.
};

// 398 DNA structure 195, 27
struct Lattice
{
    ID             id;        // 1 type, name 22, 31.
    AnimData       *adt;      // 2 type, name 36, 77.
    short          pntsu;     // 3 type, name 2, 605.
    short          pntsv;     // 4 type, name 2, 606.
    short          pntsw;     // 5 type, name 2, 1024.
    short          flag;      // 6 type, name 2, 18.
    short          opntsu;    // 7 type, name 2, 1025.
    short          opntsv;    // 8 type, name 2, 1026.
    short          opntsw;    // 9 type, name 2, 1027.
    short          pad2;      // 10 type, name 2, 29.
    char           typeu;     // 11 type, name 0, 1028.
    char           typev;     // 12 type, name 0, 1029.
    char           typew;     // 13 type, name 0, 1030.
    char           pad3;      // 14 type, name 0, 534.
    int            actbp;     // 15 type, name 4, 1031.
    float          fu;        // 16 type, name 7, 1032.
    float          fv;        // 17 type, name 7, 1033.
    float          fw;        // 18 type, name 7, 1034.
    float          du;        // 19 type, name 7, 1035.
    float          dv;        // 20 type, name 7, 1036.
    float          dw;        // 21 type, name 7, 1037.
    BPoint         *def;      // 22 type, name 31, 1038.
    Ipo            *ipo;      // 23 type, name 33, 82.
    Key            *key;      // 24 type, name 35, 629.
    MDeformVert    *dvert;    // 25 type, name 99, 685.
    char           vgroup[64];// 26 type, name 0, 74.
    EditLatt       *editlatt; // 27 type, name 194, 1039.
};

// 399 DNA structure 215, 9
struct PTCacheMem
{
    PTCacheMem    *next;     // 1 type, name 215, 0.
    PTCacheMem    *prev;     // 2 type, name 215, 1.
    int           frame;     // 3 type, name 4, 1202.
    int           totpoint;  // 4 type, name 4, 1203.
    int           data_types;// 5 type, name 4, 1012.
    int           flag;      // 6 type, name 4, 18.
    void          *data[8];  // 7 type, name 11, 1204.
    void          *cur[8];   // 8 type, name 11, 1205.
    ListBase      extradata; // 9 type, name 14, 1206.
};

// 400 DNA structure 149, 23
struct PointCache
{
    PointCache     *next;         // 1 type, name 149, 0.
    PointCache     *prev;         // 2 type, name 149, 1.
    int            flag;          // 3 type, name 4, 18.
    int            step;          // 4 type, name 4, 1207.
    int            simframe;      // 5 type, name 4, 1208.
    int            startframe;    // 6 type, name 4, 1209.
    int            endframe;      // 7 type, name 4, 1210.
    int            editframe;     // 8 type, name 4, 1211.
    int            last_exact;    // 9 type, name 4, 1212.
    int            last_valid;    // 10 type, name 4, 1213.
    int            pad;           // 11 type, name 4, 68.
    int            totpoint;      // 12 type, name 4, 1203.
    int            index;         // 13 type, name 4, 447.
    short          compression;   // 14 type, name 2, 1214.
    short          rt;            // 15 type, name 2, 58.
    char           name[64];      // 16 type, name 0, 19.
    char           prev_name[64]; // 17 type, name 0, 1215.
    char           info[64];      // 18 type, name 0, 1216.
    char           path[1024];    // 19 type, name 0, 1217.
    char           *cached_frames;// 20 type, name 0, 1218.
    ListBase       mem_cache;     // 21 type, name 14, 1219.
    void           *edit;         // Note: using void* on undefined DNA type: PTCacheEdit
    void           (*free_edit)();// 23 type, name 11, 1221.
};

// 401 DNA structure 206, 54
struct SoftBody
{
    int                totpoint;            // 1 type, name 4, 1203.
    int                totspring;           // 2 type, name 4, 1250.
    void               *bpoint;             // Note: using void* on undefined DNA type: BodyPoint
    void               *bspring;            // Note: using void* on undefined DNA type: BodySpring
    char               pad;                 // 5 type, name 0, 68.
    char               msg_lock;            // 6 type, name 0, 1253.
    short              msg_value;           // 7 type, name 2, 1254.
    float              nodemass;            // 8 type, name 7, 1255.
    char               namedVG_Mass[64];    // 9 type, name 0, 1256.
    float              grav;                // 10 type, name 7, 1257.
    float              mediafrict;          // 11 type, name 7, 1258.
    float              rklimit;             // 12 type, name 7, 1259.
    float              physics_speed;       // 13 type, name 7, 1260.
    float              goalspring;          // 14 type, name 7, 1261.
    float              goalfrict;           // 15 type, name 7, 1262.
    float              mingoal;             // 16 type, name 7, 1263.
    float              maxgoal;             // 17 type, name 7, 1264.
    float              defgoal;             // 18 type, name 7, 1265.
    short              vertgroup;           // 19 type, name 2, 1266.
    char               namedVG_Softgoal[64];// 20 type, name 0, 1267.
    short              fuzzyness;           // 21 type, name 2, 1268.
    float              inspring;            // 22 type, name 7, 1269.
    float              infrict;             // 23 type, name 7, 1270.
    char               namedVG_Spring_K[64];// 24 type, name 0, 1271.
    int                sfra;                // 25 type, name 4, 144.
    int                efra;                // 26 type, name 4, 1272.
    int                interval;            // 27 type, name 4, 1273.
    short              local;               // 28 type, name 2, 1274.
    short              solverflags;         // 29 type, name 2, 1275.
    SBVertex           **keys;              // 30 type, name 217, 1276.
    int                totpointkey;         // 31 type, name 4, 1277.
    int                totkey;              // 32 type, name 4, 84.
    float              secondspring;        // 33 type, name 7, 1278.
    float              colball;             // 34 type, name 7, 1279.
    float              balldamp;            // 35 type, name 7, 1280.
    float              ballstiff;           // 36 type, name 7, 1281.
    short              sbc_mode;            // 37 type, name 2, 1282.
    short              aeroedge;            // 38 type, name 2, 1283.
    short              minloops;            // 39 type, name 2, 1284.
    short              maxloops;            // 40 type, name 2, 1285.
    short              choke;               // 41 type, name 2, 1286.
    short              solver_ID;           // 42 type, name 2, 1287.
    short              plastic;             // 43 type, name 2, 1288.
    short              springpreload;       // 44 type, name 2, 1289.
    void               *scratch;            // Note: using void* on undefined DNA type: SBScratch
    float              shearstiff;          // 46 type, name 7, 1291.
    float              inpush;              // 47 type, name 7, 1292.
    PointCache         *pointcache;         // 48 type, name 149, 1293.
    ListBase           ptcaches;            // 49 type, name 14, 842.
    EffectorWeights    *effector_weights;   // 50 type, name 213, 1294.
    float              lcom[3];             // 51 type, name 7, 1295.
    float              lrot[3][3];          // 52 type, name 7, 1296.
    float              lscale[3][3];        // 53 type, name 7, 1297.
    int                last_frame;          // 54 type, name 4, 1298.
};

// 402 DNA structure 246, 13
struct ImagePaintSettings
{
    Paint    paint;              // 1 type, name 243, 1650.
    short    flag;               // 2 type, name 2, 18.
    short    missing_data;       // 3 type, name 2, 1651.
    short    seam_bleed;         // 4 type, name 2, 1652.
    short    normal_angle;       // 5 type, name 2, 1653.
    short    screen_grab_size[2];// 6 type, name 2, 1654.
    int      mode;               // 7 type, name 4, 368.
    void     *paintcursor;       // 8 type, name 11, 1655.
    Image    *stencil;           // 9 type, name 51, 1656.
    Image    *clone;             // 10 type, name 51, 1657.
    Image    *canvas;            // 11 type, name 51, 969.
    float    stencil_col[3];     // 12 type, name 7, 1658.
    float    dither;             // 13 type, name 7, 1659.
};

// 403 DNA structure 248, 15
struct ParticleEditSettings
{
    short                flag;         // 1 type, name 2, 18.
    short                totrekey;     // 2 type, name 2, 1661.
    short                totaddkey;    // 3 type, name 2, 1662.
    short                brushtype;    // 4 type, name 2, 1663.
    ParticleBrushData    brush[7];     // 5 type, name 247, 1664.
    void                 *paintcursor; // 6 type, name 11, 1655.
    float                emitterdist;  // 7 type, name 7, 1665.
    float                rt;           // 8 type, name 7, 58.
    int                  selectmode;   // 9 type, name 4, 1666.
    int                  edittype;     // 10 type, name 4, 1667.
    int                  draw_step;    // 11 type, name 4, 1668.
    int                  fade_frames;  // 12 type, name 4, 1669.
    Scene                *scene;       // 13 type, name 45, 140.
    Object               *object;      // 14 type, name 29, 196.
    Object               *shape_object;// 15 type, name 29, 1670.
};

// 404 DNA structure 249, 11
struct Sculpt
{
    Paint     paint;               // 1 type, name 243, 1650.
    int       flags;               // 2 type, name 4, 91.
    int       radial_symm[3];      // 3 type, name 4, 1671.
    float     detail_size;         // 4 type, name 7, 1672.
    int       symmetrize_direction;// 5 type, name 4, 1673.
    float     gravity_factor;      // 6 type, name 7, 1674.
    float     constant_detail;     // 7 type, name 7, 1675.
    float     detail_percent;      // 8 type, name 7, 1676.
    float     pad;                 // 9 type, name 7, 68.
    Object    *gravity_object;     // 10 type, name 29, 1677.
    void      *pad2;               // 11 type, name 11, 1678.
};

// 405 DNA structure 250, 1
struct UvSculpt
{
    Paint    paint;// 1 type, name 243, 1650.
};

// 406 DNA structure 251, 7
struct VPaint
{
    Paint          paint;       // 1 type, name 243, 1650.
    short          flag;        // 2 type, name 2, 18.
    short          pad;         // 3 type, name 2, 68.
    int            tot;         // 4 type, name 4, 260.
    int            *vpaint_prev;// 5 type, name 4, 1679.
    MDeformVert    *wpaint_prev;// 6 type, name 99, 1680.
    void           *paintcursor;// 7 type, name 11, 1655.
};

// 407 DNA structure 276, 6
struct SpaceLink
{
    SpaceLink    *next;          // 1 type, name 276, 0.
    SpaceLink    *prev;          // 2 type, name 276, 1.
    ListBase     regionbase;     // 3 type, name 14, 1884.
    int          spacetype;      // 4 type, name 4, 1885.
    float        blockscale;     // 5 type, name 7, 1886.
    short        blockhandler[8];// 6 type, name 2, 1887.
};

// 408 DNA structure 279, 8
struct SpaceInfo
{
    SpaceLink    *next;          // 1 type, name 276, 0.
    SpaceLink    *prev;          // 2 type, name 276, 1.
    ListBase     regionbase;     // 3 type, name 14, 1884.
    int          spacetype;      // 4 type, name 4, 1885.
    float        blockscale;     // 5 type, name 7, 1886.
    short        blockhandler[8];// 6 type, name 2, 1887.
    char         rpt_mask;       // 7 type, name 0, 1946.
    char         pad[7];         // 8 type, name 0, 119.
};

// 409 DNA structure 294, 16
struct SpaceFile
{
    SpaceLink           *next;              // 1 type, name 276, 0.
    SpaceLink           *prev;              // 2 type, name 276, 1.
    ListBase            regionbase;         // 3 type, name 14, 1884.
    int                 spacetype;          // 4 type, name 4, 1885.
    int                 scroll_offset;      // 5 type, name 4, 2003.
    FileSelectParams    *params;            // 6 type, name 293, 2004.
    void                *files;             // Note: using void* on undefined DNA type: FileList
    ListBase            *folders_prev;      // 8 type, name 14, 2006.
    ListBase            *folders_next;      // 9 type, name 14, 2007.
    wmOperator          *op;                // 10 type, name 296, 2008.
    void                *smoothscroll_timer;// Note: using void* on undefined DNA type: wmTimer
    void                *layout;            // Note: using void* on undefined DNA type: FileLayout
    short               recentnr;           // 13 type, name 2, 2011.
    short               bookmarknr;         // 14 type, name 2, 2012.
    short               systemnr;           // 15 type, name 2, 2013.
    short               system_bookmarknr;  // 16 type, name 2, 2014.
};

// 410 DNA structure 303, 10
struct Script
{
    ID      id;                 // 1 type, name 22, 31.
    void    *py_draw;           // 2 type, name 11, 2055.
    void    *py_event;          // 3 type, name 11, 2056.
    void    *py_button;         // 4 type, name 11, 2057.
    void    *py_browsercallback;// 5 type, name 11, 2058.
    void    *py_globaldict;     // 6 type, name 11, 2059.
    int     flags;              // 7 type, name 4, 91.
    int     lastspace;          // 8 type, name 4, 2060.
    char    scriptname[1024];   // 9 type, name 0, 2061.
    char    scriptarg[256];     // 10 type, name 0, 2062.
};

// 411 DNA structure 304, 10
struct SpaceScript
{
    SpaceLink    *next;     // 1 type, name 276, 0.
    SpaceLink    *prev;     // 2 type, name 276, 1.
    ListBase     regionbase;// 3 type, name 14, 1884.
    int          spacetype; // 4 type, name 4, 1885.
    float        blockscale;// 5 type, name 7, 1886.
    Script       *script;   // 6 type, name 303, 2063.
    short        flags;     // 7 type, name 2, 91.
    short        menunr;    // 8 type, name 2, 2033.
    int          pad1;      // 9 type, name 4, 70.
    void         *but_refs; // 10 type, name 11, 2064.
};

// 412 DNA structure 305, 7
struct bNodeTreePath
{
    bNodeTreePath       *next;         // 1 type, name 305, 0.
    bNodeTreePath       *prev;         // 2 type, name 305, 1.
    bNodeTree           *nodetree;     // 3 type, name 66, 353.
    bNodeInstanceKey    parent_key;    // 4 type, name 306, 2065.
    int                 pad;           // 5 type, name 4, 68.
    float               view_center[2];// 6 type, name 7, 2066.
    char                node_name[64]; // 7 type, name 0, 2067.
};

// 413 DNA structure 308, 10
struct SpaceLogic
{
    SpaceLink    *next;          // 1 type, name 276, 0.
    SpaceLink    *prev;          // 2 type, name 276, 1.
    ListBase     regionbase;     // 3 type, name 14, 1884.
    int          spacetype;      // 4 type, name 4, 1885.
    float        blockscale;     // 5 type, name 7, 1886.
    short        blockhandler[8];// 6 type, name 2, 1887.
    short        flag;           // 7 type, name 2, 18.
    short        scaflag;        // 8 type, name 2, 1088.
    int          pad;            // 9 type, name 4, 68.
    bGPdata      *gpd;           // 10 type, name 201, 1057.
};

// 414 DNA structure 311, 7
struct SpaceUserPref
{
    SpaceLink    *next;      // 1 type, name 276, 0.
    SpaceLink    *prev;      // 2 type, name 276, 1.
    ListBase     regionbase; // 3 type, name 14, 1884.
    int          spacetype;  // 4 type, name 4, 1885.
    char         pad[3];     // 5 type, name 0, 44.
    char         filter_type;// 6 type, name 0, 2085.
    char         filter[64]; // 7 type, name 0, 2086.
};

// 415 DNA structure 331, 6
struct ScrVert
{
    ScrVert    *next;   // 1 type, name 331, 0.
    ScrVert    *prev;   // 2 type, name 331, 1.
    ScrVert    *newv;   // 3 type, name 331, 2515.
    vec2s      vec;     // 4 type, name 15, 2516.
    short      flag;    // 5 type, name 2, 18.
    short      editflag;// 6 type, name 2, 705.
};

// 416 DNA structure 352, 13
struct Strip
{
    Strip                             *next;              // 1 type, name 352, 0.
    Strip                             *prev;              // 2 type, name 352, 1.
    int                               us;                 // 3 type, name 4, 27.
    int                               done;               // 4 type, name 4, 2598.
    int                               startstill;         // 5 type, name 4, 2599.
    int                               endstill;           // 6 type, name 4, 2600.
    StripElem                         *stripdata;         // 7 type, name 347, 2601.
    char                              dir[768];           // 8 type, name 0, 2592.
    StripProxy                        *proxy;             // 9 type, name 351, 1051.
    StripCrop                         *crop;              // 10 type, name 348, 2602.
    StripTransform                    *transform;         // 11 type, name 349, 2603.
    StripColorBalance                 *color_balance;     // 12 type, name 350, 2604.
    ColorManagedColorspaceSettings    colorspace_settings;// 13 type, name 54, 184.
};

// 417 DNA structure 366, 3
struct BrightContrastModifierData
{
    SequenceModifierData    modifier;// 1 type, name 362, 757.
    float                   bright;  // 2 type, name 7, 316.
    float                   contrast;// 3 type, name 7, 317.
};

// 418 DNA structure 367, 1
struct SequencerMaskModifierData
{
    SequenceModifierData    modifier;// 1 type, name 362, 757.
};

// 419 DNA structure 397, 13
struct bSoundActuator
{
    short      flag;   // 1 type, name 2, 18.
    short      sndnr;  // 2 type, name 2, 2775.
    int        pad1;   // 3 type, name 4, 70.
    int        pad2;   // 4 type, name 4, 29.
    short      pad3[2];// 5 type, name 2, 2776.
    float      volume; // 6 type, name 7, 1224.
    float      pitch;  // 7 type, name 7, 2628.
    bSound     *sound; // 8 type, name 354, 2626.
    Sound3D    sound3D;// 9 type, name 396, 2777.
    short      type;   // 10 type, name 2, 16.
    short      pad4;   // 11 type, name 2, 1532.
    short      pad5;   // 12 type, name 2, 386.
    short      pad6[1];// 13 type, name 2, 2778.
};

// 420 DNA structure 354, 18
struct bSound
{
    ID            id;              // 1 type, name 22, 31.
    char          name[1024];      // 2 type, name 0, 34.
    PackedFile    *packedfile;     // 3 type, name 25, 37.
    void          *handle;         // 4 type, name 11, 2825.
    PackedFile    *newpackedfile;  // 5 type, name 25, 2826.
    Ipo           *ipo;            // 6 type, name 33, 82.
    float         volume;          // 7 type, name 7, 1224.
    float         attenuation;     // 8 type, name 7, 114.
    float         pitch;           // 9 type, name 7, 2628.
    float         min_gain;        // 10 type, name 7, 2767.
    float         max_gain;        // 11 type, name 7, 2768.
    float         distance;        // 12 type, name 7, 1042.
    int           flags;           // 13 type, name 4, 91.
    int           pad;             // 14 type, name 4, 68.
    void          *cache;          // 15 type, name 11, 154.
    void          *waveform;       // 16 type, name 11, 2827.
    void          *playback_handle;// 17 type, name 11, 1826.
    void          *spinlock;       // 18 type, name 11, 2828.
};

// 421 DNA structure 417, 28
struct Bone
{
    Bone          *next;         // 1 type, name 417, 0.
    Bone          *prev;         // 2 type, name 417, 1.
    IDProperty    *prop;         // 3 type, name 21, 2378.
    Bone          *parent;       // 4 type, name 417, 36.
    ListBase      childbase;     // 5 type, name 14, 2832.
    char          name[64];      // 6 type, name 0, 19.
    float         roll;          // 7 type, name 7, 2833.
    float         head[3];       // 8 type, name 7, 2834.
    float         tail[3];       // 9 type, name 7, 2835.
    float         bone_mat[3][3];// 10 type, name 7, 2836.
    int           flag;          // 11 type, name 4, 18.
    float         arm_head[3];   // 12 type, name 7, 2837.
    float         arm_tail[3];   // 13 type, name 7, 2838.
    float         arm_mat[4][4]; // 14 type, name 7, 2839.
    float         arm_roll;      // 15 type, name 7, 2840.
    float         dist;          // 16 type, name 7, 375.
    float         weight;        // 17 type, name 7, 591.
    float         xwidth;        // 18 type, name 7, 2841.
    float         length;        // 19 type, name 7, 771.
    float         zwidth;        // 20 type, name 7, 2842.
    float         ease1;         // 21 type, name 7, 2843.
    float         ease2;         // 22 type, name 7, 2844.
    float         rad_head;      // 23 type, name 7, 2845.
    float         rad_tail;      // 24 type, name 7, 2846.
    float         size[3];       // 25 type, name 7, 207.
    int           layer;         // 26 type, name 4, 152.
    short         segments;      // 27 type, name 2, 2847.
    short         pad[1];        // 28 type, name 2, 264.
};

// 422 DNA structure 285, 8
struct bDopeSheet
{
    ID          *source;      // 1 type, name 22, 1041.
    ListBase    chanbase;     // 2 type, name 14, 2908.
    Group       *filter_grp;  // 3 type, name 74, 2935.
    char        searchstr[64];// 4 type, name 0, 2936.
    int         filterflag;   // 5 type, name 4, 2937.
    int         flag;         // 6 type, name 4, 18.
    int         renameIndex;  // 7 type, name 4, 2938.
    int         pad;          // 8 type, name 4, 68.
};

// 423 DNA structure 426, 8
struct bActionChannel
{
    bActionChannel    *next;             // 1 type, name 426, 0.
    bActionChannel    *prev;             // 2 type, name 426, 1.
    bActionGroup      *grp;              // 3 type, name 424, 2941.
    Ipo               *ipo;              // 4 type, name 33, 82.
    ListBase          constraintChannels;// 5 type, name 14, 1060.
    int               flag;              // 6 type, name 4, 18.
    char              name[64];          // 7 type, name 0, 19.
    int               temp;              // 8 type, name 4, 2503.
};

// 424 DNA structure 430, 7
struct bPythonConstraint
{
    Text          *text;        // 1 type, name 38, 2030.
    IDProperty    *prop;        // 2 type, name 21, 2378.
    int           flag;         // 3 type, name 4, 18.
    int           tarnum;       // 4 type, name 4, 2953.
    ListBase      targets;      // 5 type, name 14, 2954.
    Object        *tar;         // 6 type, name 29, 2949.
    char          subtarget[64];// 7 type, name 0, 832.
};

// 425 DNA structure 459, 22
struct bActionStrip
{
    bActionStrip    *next;            // 1 type, name 459, 0.
    bActionStrip    *prev;            // 2 type, name 459, 1.
    short           flag;             // 3 type, name 2, 18.
    short           mode;             // 4 type, name 2, 368.
    short           stride_axis;      // 5 type, name 2, 3016.
    short           curmod;           // 6 type, name 2, 3017.
    Ipo             *ipo;             // 7 type, name 33, 82.
    bAction         *act;             // 8 type, name 199, 2759.
    Object          *object;          // 9 type, name 29, 196.
    float           start;            // 10 type, name 7, 770.
    float           end;              // 11 type, name 7, 2686.
    float           actstart;         // 12 type, name 7, 3018.
    float           actend;           // 13 type, name 7, 3019.
    float           actoffs;          // 14 type, name 7, 3020.
    float           stridelen;        // 15 type, name 7, 3021.
    float           repeat;           // 16 type, name 7, 818.
    float           scale;            // 17 type, name 7, 972.
    float           blendin;          // 18 type, name 7, 2761.
    float           blendout;         // 19 type, name 7, 3022.
    char            stridechannel[32];// 20 type, name 0, 3023.
    char            offs_bone[32];    // 21 type, name 0, 3024.
    ListBase        modifiers;        // 22 type, name 14, 1063.
};

// 426 DNA structure 461, 25
struct bNodeSocket
{
    bNodeSocket        *next;         // 1 type, name 461, 0.
    bNodeSocket        *prev;         // 2 type, name 461, 1.
    bNodeSocket        *new_sock;     // 3 type, name 461, 3031.
    IDProperty         *prop;         // 4 type, name 21, 2378.
    char               identifier[64];// 5 type, name 0, 3032.
    char               name[64];      // 6 type, name 0, 19.
    void               *storage;      // 7 type, name 11, 3033.
    short              type;          // 8 type, name 2, 16.
    short              flag;          // 9 type, name 2, 18.
    short              limit;         // 10 type, name 2, 3034.
    short              in_out;        // 11 type, name 2, 3035.
    void               *typeinfo;     // Note: using void* on undefined DNA type: bNodeSocketType
    char               idname[64];    // 13 type, name 0, 2534.
    float              locx;          // 14 type, name 7, 3037.
    float              locy;          // 15 type, name 7, 3038.
    void               *default_value;// 16 type, name 11, 3039.
    short              stack_index;   // 17 type, name 2, 3040.
    short              stack_type;    // 18 type, name 2, 3041.
    int                pad;           // 19 type, name 4, 68.
    void               *cache;        // 20 type, name 11, 154.
    int                own_index;     // 21 type, name 4, 3042.
    int                to_index;      // 22 type, name 4, 3043.
    bNodeSocket        *groupsock;    // 23 type, name 461, 3044.
    bNodeLink          *link;         // 24 type, name 463, 3045.
    bNodeStack         ns;            // 25 type, name 460, 3046.
};

// 427 DNA structure 467, 3
struct bNodeInstanceHashEntry
{
    bNodeInstanceKey    key;// 1 type, name 306, 2722.
    short               tag;// 2 type, name 2, 3070.
    short               pad;// 3 type, name 2, 68.
};

// 428 DNA structure 300, 18
struct Scopes
{
    int          ok;             // 1 type, name 4, 147.
    int          sample_full;    // 2 type, name 4, 3254.
    int          sample_lines;   // 3 type, name 4, 3255.
    float        accuracy;       // 4 type, name 7, 3256.
    int          wavefrm_mode;   // 5 type, name 4, 3257.
    float        wavefrm_alpha;  // 6 type, name 7, 3258.
    float        wavefrm_yfac;   // 7 type, name 7, 3259.
    int          wavefrm_height; // 8 type, name 4, 3260.
    float        vecscope_alpha; // 9 type, name 7, 3261.
    int          vecscope_height;// 10 type, name 4, 3262.
    float        minmax[3][2];   // 11 type, name 7, 3263.
    Histogram    hist;           // 12 type, name 301, 3264.
    float        *waveform_1;    // 13 type, name 7, 3265.
    float        *waveform_2;    // 14 type, name 7, 3266.
    float        *waveform_3;    // 15 type, name 7, 3267.
    float        *vecscope;      // 16 type, name 7, 3268.
    int          waveform_tot;   // 17 type, name 4, 3269.
    int          pad;            // 18 type, name 4, 68.
};

// 429 DNA structure 538, 2
struct PaintCurvePoint
{
    BezTriple    bez;     // 1 type, name 32, 3320.
    float        pressure;// 2 type, name 7, 3321.
};

// 430 DNA structure 536, 4
struct PaintCurve
{
    ID                 id;        // 1 type, name 22, 31.
    PaintCurvePoint    *points;   // 2 type, name 538, 2867.
    int                tot_points;// 3 type, name 4, 3322.
    int                add_index; // 4 type, name 4, 3323.
};

// 431 DNA structure 543, 5
struct BoidParticle
{
    Object      *ground;   // 1 type, name 29, 3337.
    BoidData    data;      // 2 type, name 544, 21.
    float       gravity[3];// 3 type, name 7, 1801.
    float       wander[3]; // 4 type, name 7, 3338.
    float       rt;        // 5 type, name 7, 58.
};

// 432 DNA structure 561, 6
struct bGPDframe
{
    bGPDframe    *next;   // 1 type, name 561, 0.
    bGPDframe    *prev;   // 2 type, name 561, 1.
    ListBase     strokes; // 3 type, name 14, 3548.
    int          framenum;// 4 type, name 4, 3549.
    short        flag;    // 5 type, name 2, 18.
    short        key_type;// 6 type, name 2, 3550.
};

// 433 DNA structure 562, 13
struct bGPDlayer
{
    bGPDlayer    *next;         // 1 type, name 562, 0.
    bGPDlayer    *prev;         // 2 type, name 562, 1.
    ListBase     frames;        // 3 type, name 14, 142.
    bGPDframe    *actframe;     // 4 type, name 561, 3551.
    short        flag;          // 5 type, name 2, 18.
    short        thickness;     // 6 type, name 2, 3546.
    short        gstep;         // 7 type, name 2, 3552.
    short        gstep_next;    // 8 type, name 2, 3553.
    float        gcolor_prev[3];// 9 type, name 7, 3554.
    float        gcolor_next[3];// 10 type, name 7, 3555.
    float        color[4];      // 11 type, name 7, 3556.
    float        fill[4];       // 12 type, name 7, 3557.
    char         info[128];     // 13 type, name 0, 3558.
};

// 434 DNA structure 563, 6
struct ReportList
{
    ListBase    list;        // 1 type, name 14, 3562.
    int         printlevel;  // 2 type, name 4, 3563.
    int         storelevel;  // 3 type, name 4, 3564.
    int         flag;        // 4 type, name 4, 18.
    int         pad;         // 5 type, name 4, 68.
    void        *reporttimer;// Note: using void* on undefined DNA type: wmTimer
};

// 435 DNA structure 566, 7
struct wmKeyConfig
{
    wmKeyConfig    *next;       // 1 type, name 566, 0.
    wmKeyConfig    *prev;       // 2 type, name 566, 1.
    char           idname[64];  // 3 type, name 0, 2534.
    char           basename[64];// 4 type, name 0, 3628.
    ListBase       keymaps;     // 5 type, name 14, 2425.
    int            actkeymap;   // 6 type, name 4, 3629.
    int            flag;        // 7 type, name 4, 18.
};

// 436 DNA structure 296, 14
struct wmOperator
{
    wmOperator        *next;       // 1 type, name 296, 0.
    wmOperator        *prev;       // 2 type, name 296, 1.
    char              idname[64];  // 3 type, name 0, 2534.
    IDProperty        *properties; // 4 type, name 21, 30.
    void              *type;       // Note: using void* on undefined DNA type: wmOperatorType
    void              *customdata; // 6 type, name 11, 3630.
    void              *py_instance;// 7 type, name 11, 3631.
    void              *ptr;        // Note: using void* on undefined DNA type: PointerRNA
    ReportList        *reports;    // 9 type, name 563, 3632.
    ListBase          macro;       // 10 type, name 14, 3633.
    wmOperator        *opm;        // 11 type, name 296, 3634.
    void              *layout;     // Note: using void* on undefined DNA type: uiLayout
    short             flag;        // 13 type, name 2, 18.
    short             pad[3];      // 14 type, name 2, 44.
};

// 437 DNA structure 583, 3
struct FMod_Limits
{
    rctf    rect;// 1 type, name 19, 3646.
    int     flag;// 2 type, name 4, 18.
    int     pad; // 3 type, name 4, 68.
};

// 438 DNA structure 587, 7
struct DriverVar
{
    DriverVar       *next;      // 1 type, name 587, 0.
    DriverVar       *prev;      // 2 type, name 587, 1.
    char            name[64];   // 3 type, name 0, 19.
    DriverTarget    targets[8]; // 4 type, name 586, 3654.
    short           num_targets;// 5 type, name 2, 3655.
    short           type;       // 6 type, name 2, 16.
    float           curval;     // 7 type, name 7, 62.
};

// 439 DNA structure 588, 7
struct ChannelDriver
{
    ListBase    variables;      // 1 type, name 14, 3656.
    char        expression[256];// 2 type, name 0, 3657.
    void        *expr_comp;     // 3 type, name 11, 3658.
    float       curval;         // 4 type, name 7, 62.
    float       influence;      // 5 type, name 7, 2811.
    int         type;           // 6 type, name 4, 16.
    int         flag;           // 7 type, name 4, 18.
};

// 440 DNA structure 590, 17
struct FCurve
{
    FCurve           *next;           // 1 type, name 590, 0.
    FCurve           *prev;           // 2 type, name 590, 1.
    bActionGroup     *grp;            // 3 type, name 424, 2941.
    ChannelDriver    *driver;         // 4 type, name 588, 63.
    ListBase         modifiers;       // 5 type, name 14, 1063.
    BezTriple        *bezt;           // 6 type, name 32, 51.
    FPoint           *fpt;            // 7 type, name 589, 3660.
    int              totvert;         // 8 type, name 4, 55.
    float            curval;          // 9 type, name 7, 62.
    short            flag;            // 10 type, name 2, 18.
    short            extend;          // 11 type, name 2, 302.
    int              array_index;     // 12 type, name 4, 3661.
    char             *rna_path;       // 13 type, name 0, 3650.
    int              color_mode;      // 14 type, name 4, 263.
    float            color[3];        // 15 type, name 7, 112.
    float            prev_norm_factor;// 16 type, name 7, 3662.
    float            prev_offset;     // 17 type, name 7, 3663.
};

// 441 DNA structure 592, 4
struct AnimMapper
{
    AnimMapper    *next;   // 1 type, name 592, 0.
    AnimMapper    *prev;   // 2 type, name 592, 1.
    bAction       *target; // 3 type, name 199, 896.
    ListBase      mappings;// 4 type, name 14, 3666.
};

// 442 DNA structure 594, 6
struct NlaTrack
{
    NlaTrack    *next;   // 1 type, name 594, 0.
    NlaTrack    *prev;   // 2 type, name 594, 1.
    ListBase    strips;  // 3 type, name 14, 3667.
    int         flag;    // 4 type, name 4, 18.
    int         index;   // 5 type, name 4, 447.
    char        name[64];// 6 type, name 0, 19.
};

// 443 DNA structure 596, 12
struct KeyingSet
{
    KeyingSet    *next;           // 1 type, name 596, 0.
    KeyingSet    *prev;           // 2 type, name 596, 1.
    ListBase     paths;           // 3 type, name 14, 3678.
    char         idname[64];      // 4 type, name 0, 2534.
    char         name[64];        // 5 type, name 0, 19.
    char         description[240];// 6 type, name 0, 3679.
    char         typeinfo[64];    // 7 type, name 0, 3680.
    int          active_path;     // 8 type, name 4, 3681.
    short        flag;            // 9 type, name 2, 18.
    short        keyingflag;      // 10 type, name 2, 3676.
    short        keyingoverride;  // 11 type, name 2, 3677.
    char         pad[6];          // 12 type, name 0, 769.
};

// 444 DNA structure 598, 2
struct IdAdtTemplate
{
    ID          id;  // 1 type, name 22, 31.
    AnimData    *adt;// 2 type, name 36, 77.
};

// 445 DNA structure 600, 6
struct BoidRuleGoalAvoid
{
    BoidRule    rule;       // 1 type, name 599, 3691.
    Object      *ob;        // 2 type, name 29, 46.
    int         options;    // 3 type, name 4, 3692.
    float       fear_factor;// 4 type, name 7, 3693.
    int         signal_id;  // 5 type, name 4, 3694.
    int         channels;   // 6 type, name 4, 2928.
};

// 446 DNA structure 601, 3
struct BoidRuleAvoidCollision
{
    BoidRule    rule;      // 1 type, name 599, 3691.
    int         options;   // 2 type, name 4, 3692.
    float       look_ahead;// 3 type, name 7, 3695.
};

// 447 DNA structure 602, 8
struct BoidRuleFollowLeader
{
    BoidRule    rule;      // 1 type, name 599, 3691.
    Object      *ob;       // 2 type, name 29, 46.
    float       loc[3];    // 3 type, name 7, 359.
    float       oloc[3];   // 4 type, name 7, 3696.
    float       cfra;      // 5 type, name 7, 863.
    float       distance;  // 6 type, name 7, 1042.
    int         options;   // 7 type, name 4, 3692.
    int         queue_size;// 8 type, name 4, 3697.
};

// 448 DNA structure 603, 5
struct BoidRuleAverageSpeed
{
    BoidRule    rule;  // 1 type, name 599, 3691.
    float       wander;// 2 type, name 7, 3698.
    float       level; // 3 type, name 7, 734.
    float       speed; // 4 type, name 7, 824.
    float       rt;    // 5 type, name 7, 58.
};

// 449 DNA structure 604, 3
struct BoidRuleFight
{
    BoidRule    rule;         // 1 type, name 599, 3691.
    float       distance;     // 2 type, name 7, 1042.
    float       flee_distance;// 3 type, name 7, 3699.
};

// 450 DNA structure 552, 23
struct BoidSettings
{
    int         options;            // 1 type, name 4, 3692.
    int         last_state_id;      // 2 type, name 4, 3707.
    float       landing_smoothness; // 3 type, name 7, 3708.
    float       height;             // 4 type, name 7, 822.
    float       banking;            // 5 type, name 7, 3709.
    float       pitch;              // 6 type, name 7, 2628.
    float       health;             // 7 type, name 7, 3700.
    float       aggression;         // 8 type, name 7, 3710.
    float       strength;           // 9 type, name 7, 767.
    float       accuracy;           // 10 type, name 7, 3256.
    float       range;              // 11 type, name 7, 1357.
    float       air_min_speed;      // 12 type, name 7, 3711.
    float       air_max_speed;      // 13 type, name 7, 3712.
    float       air_max_acc;        // 14 type, name 7, 3713.
    float       air_max_ave;        // 15 type, name 7, 3714.
    float       air_personal_space; // 16 type, name 7, 3715.
    float       land_jump_speed;    // 17 type, name 7, 3716.
    float       land_max_speed;     // 18 type, name 7, 3717.
    float       land_max_acc;       // 19 type, name 7, 3718.
    float       land_max_ave;       // 20 type, name 7, 3719.
    float       land_personal_space;// 21 type, name 7, 3720.
    float       land_stick_force;   // 22 type, name 7, 3721.
    ListBase    states;             // 23 type, name 14, 3722.
};

// 451 DNA structure 133, 62
struct SmokeDomainSettings
{
    SmokeModifierData    *smd;                // 1 type, name 132, 3723.
    void                 *fluid;              // Note: using void* on undefined DNA type: FLUID_3D
    void                 *fluid_mutex;        // 3 type, name 11, 3724.
    Group                *fluid_group;        // 4 type, name 74, 3725.
    Group                *eff_group;          // 5 type, name 74, 3464.
    Group                *coll_group;         // 6 type, name 74, 3726.
    void                 *wt;                 // Note: using void* on undefined DNA type: WTURBULENCE
    void                 *tex;                // Note: using void* on undefined DNA type: GPUTexture
    void                 *tex_wt;             // Note: using void* on undefined DNA type: GPUTexture
    void                 *tex_shadow;         // Note: using void* on undefined DNA type: GPUTexture
    void                 *tex_flame;          // Note: using void* on undefined DNA type: GPUTexture
    float                *shadow;             // 12 type, name 7, 3731.
    float                p0[3];               // 13 type, name 7, 3732.
    float                p1[3];               // 14 type, name 7, 3733.
    float                dp0[3];              // 15 type, name 7, 3734.
    float                cell_size[3];        // 16 type, name 7, 3735.
    float                global_size[3];      // 17 type, name 7, 3736.
    float                prev_loc[3];         // 18 type, name 7, 3737.
    int                  shift[3];            // 19 type, name 4, 3738.
    float                shift_f[3];          // 20 type, name 7, 3739.
    float                obj_shift_f[3];      // 21 type, name 7, 3740.
    float                imat[4][4];          // 22 type, name 7, 268.
    float                obmat[4][4];         // 23 type, name 7, 1077.
    int                  base_res[3];         // 24 type, name 4, 3741.
    int                  res_min[3];          // 25 type, name 4, 3742.
    int                  res_max[3];          // 26 type, name 4, 3743.
    int                  res[3];              // 27 type, name 4, 3744.
    int                  total_cells;         // 28 type, name 4, 3745.
    float                dx;                  // 29 type, name 7, 3746.
    float                scale;               // 30 type, name 7, 972.
    int                  adapt_margin;        // 31 type, name 4, 3747.
    int                  adapt_res;           // 32 type, name 4, 3748.
    float                adapt_threshold;     // 33 type, name 7, 3749.
    float                alpha;               // 34 type, name 7, 463.
    float                beta;                // 35 type, name 7, 3750.
    int                  amplify;             // 36 type, name 4, 3751.
    int                  maxres;              // 37 type, name 4, 3752.
    int                  flags;               // 38 type, name 4, 91.
    int                  viewsettings;        // 39 type, name 4, 3753.
    short                noise;               // 40 type, name 2, 3754.
    short                diss_percent;        // 41 type, name 2, 3755.
    int                  diss_speed;          // 42 type, name 4, 3756.
    float                strength;            // 43 type, name 7, 767.
    int                  res_wt[3];           // 44 type, name 4, 3757.
    float                dx_wt;               // 45 type, name 7, 3758.
    int                  cache_comp;          // 46 type, name 4, 3759.
    int                  cache_high_comp;     // 47 type, name 4, 3760.
    PointCache           *point_cache[2];     // 48 type, name 149, 3761.
    ListBase             ptcaches[2];         // 49 type, name 14, 3762.
    EffectorWeights      *effector_weights;   // 50 type, name 213, 1294.
    int                  border_collisions;   // 51 type, name 4, 3763.
    float                time_scale;          // 52 type, name 7, 3764.
    float                vorticity;           // 53 type, name 7, 3765.
    int                  active_fields;       // 54 type, name 4, 3766.
    float                active_color[3];     // 55 type, name 7, 3767.
    int                  highres_sampling;    // 56 type, name 4, 3768.
    float                burning_rate;        // 57 type, name 7, 3769.
    float                flame_smoke;         // 58 type, name 7, 3770.
    float                flame_vorticity;     // 59 type, name 7, 3771.
    float                flame_ignition;      // 60 type, name 7, 3772.
    float                flame_max_temp;      // 61 type, name 7, 3773.
    float                flame_smoke_color[3];// 62 type, name 7, 3774.
};

// 452 DNA structure 608, 15
struct Speaker
{
    ID          id;                // 1 type, name 22, 31.
    AnimData    *adt;              // 2 type, name 36, 77.
    bSound      *sound;            // 3 type, name 354, 2626.
    float       volume_max;        // 4 type, name 7, 3788.
    float       volume_min;        // 5 type, name 7, 3789.
    float       distance_max;      // 6 type, name 7, 113.
    float       distance_reference;// 7 type, name 7, 3790.
    float       attenuation;       // 8 type, name 7, 114.
    float       cone_angle_outer;  // 9 type, name 7, 3791.
    float       cone_angle_inner;  // 10 type, name 7, 3792.
    float       cone_volume_outer; // 11 type, name 7, 3793.
    float       volume;            // 12 type, name 7, 1224.
    float       pitch;             // 13 type, name 7, 2628.
    short       flag;              // 14 type, name 2, 18.
    short       pad1[3];           // 15 type, name 2, 2546.
};

// 453 DNA structure 313, 15
struct MovieClipScopes
{
    short                  ok;                  // 1 type, name 2, 147.
    short                  use_track_mask;      // 2 type, name 2, 3802.
    int                    track_preview_height;// 3 type, name 4, 3803.
    int                    frame_width;         // 4 type, name 4, 3804.
    int                    frame_height;        // 5 type, name 4, 3805.
    MovieTrackingMarker    undist_marker;       // 6 type, name 612, 3806.
    void                   *track_search;       // Note: using void* on undefined DNA type: ImBuf
    void                   *track_preview;      // Note: using void* on undefined DNA type: ImBuf
    float                  track_pos[2];        // 9 type, name 7, 3809.
    short                  track_disabled;      // 10 type, name 2, 3810.
    short                  track_locked;        // 11 type, name 2, 3811.
    int                    framenr;             // 12 type, name 4, 141.
    MovieTrackingTrack     *track;              // 13 type, name 613, 1050.
    MovieTrackingMarker    *marker;             // 14 type, name 612, 3812.
    float                  slide_scale[2];      // 15 type, name 7, 3813.
};

// 454 DNA structure 626, 50
struct DynamicPaintSurface
{
    DynamicPaintSurface           *next;                  // 1 type, name 626, 0.
    DynamicPaintSurface           *prev;                  // 2 type, name 626, 1.
    DynamicPaintCanvasSettings    *canvas;                // 3 type, name 181, 969.
    void                          *data;                  // Note: using void* on undefined DNA type: PaintSurfaceData
    Group                         *brush_group;           // 5 type, name 74, 3896.
    EffectorWeights               *effector_weights;      // 6 type, name 213, 1294.
    PointCache                    *pointcache;            // 7 type, name 149, 1293.
    ListBase                      ptcaches;               // 8 type, name 14, 842.
    int                           current_frame;          // 9 type, name 4, 3897.
    char                          name[64];               // 10 type, name 0, 19.
    short                         format;                 // 11 type, name 2, 3138.
    short                         type;                   // 12 type, name 2, 16.
    short                         disp_type;              // 13 type, name 2, 3898.
    short                         image_fileformat;       // 14 type, name 2, 3899.
    short                         effect_ui;              // 15 type, name 2, 3900.
    short                         preview_id;             // 16 type, name 2, 3901.
    short                         init_color_type;        // 17 type, name 2, 3902.
    short                         pad_s;                  // 18 type, name 2, 3903.
    int                           flags;                  // 19 type, name 4, 91.
    int                           effect;                 // 20 type, name 4, 1061.
    int                           image_resolution;       // 21 type, name 4, 3904.
    int                           substeps;               // 22 type, name 4, 3905.
    int                           start_frame;            // 23 type, name 4, 2868.
    int                           end_frame;              // 24 type, name 4, 2869.
    int                           pad;                    // 25 type, name 4, 68.
    float                         init_color[4];          // 26 type, name 7, 3906.
    Tex                           *init_texture;          // 27 type, name 57, 3907.
    char                          init_layername[64];     // 28 type, name 0, 3908.
    int                           dry_speed;              // 29 type, name 4, 3909.
    int                           diss_speed;             // 30 type, name 4, 3756.
    float                         color_dry_threshold;    // 31 type, name 7, 3910.
    float                         depth_clamp;            // 32 type, name 7, 3911.
    float                         disp_factor;            // 33 type, name 7, 3912.
    float                         spread_speed;           // 34 type, name 7, 3913.
    float                         color_spread_speed;     // 35 type, name 7, 3914.
    float                         shrink_speed;           // 36 type, name 7, 3915.
    float                         drip_vel;               // 37 type, name 7, 3916.
    float                         drip_acc;               // 38 type, name 7, 3917.
    float                         influence_scale;        // 39 type, name 7, 3918.
    float                         radius_scale;           // 40 type, name 7, 3919.
    float                         wave_damping;           // 41 type, name 7, 3920.
    float                         wave_speed;             // 42 type, name 7, 3921.
    float                         wave_timescale;         // 43 type, name 7, 3922.
    float                         wave_spring;            // 44 type, name 7, 3923.
    float                         wave_smoothness;        // 45 type, name 7, 3924.
    int                           pad2;                   // 46 type, name 4, 29.
    char                          uvlayer_name[64];       // 47 type, name 0, 760.
    char                          image_output_path[1024];// 48 type, name 0, 3925.
    char                          output_name[64];        // 49 type, name 0, 3926.
    char                          output_name2[64];       // 50 type, name 0, 3927.
};

// 455 DNA structure 181, 7
struct DynamicPaintCanvasSettings
{
    DynamicPaintModifierData    *pmd;      // 1 type, name 180, 3928.
    void                        *dm;       // Note: using void* on undefined DNA type: DerivedMesh
    ListBase                    surfaces;  // 3 type, name 14, 3929.
    short                       active_sur;// 4 type, name 2, 3930.
    short                       flags;     // 5 type, name 2, 91.
    int                         pad;       // 6 type, name 4, 68.
    char                        error[64]; // 7 type, name 0, 3931.
};

// 456 DNA structure 631, 9
struct MaskSpline
{
    MaskSpline         *next;         // 1 type, name 631, 0.
    MaskSpline         *prev;         // 2 type, name 631, 1.
    short              flag;          // 3 type, name 2, 18.
    char               offset_mode;   // 4 type, name 0, 3957.
    char               weight_interp; // 5 type, name 0, 3958.
    int                tot_point;     // 6 type, name 4, 3959.
    MaskSplinePoint    *points;       // 7 type, name 630, 2867.
    MaskParent         parent;        // 8 type, name 628, 3343.
    MaskSplinePoint    *points_deform;// 9 type, name 630, 3960.
};

// 457 DNA structure 266, 14
struct RigidBodyWorld
{
    EffectorWeights    *effector_weights;    // 1 type, name 213, 1294.
    Group              *group;               // 2 type, name 74, 537.
    Object             **objects;            // 3 type, name 29, 3967.
    Group              *constraints;         // 4 type, name 74, 3968.
    int                pad;                  // 5 type, name 4, 68.
    float              ltime;                // 6 type, name 7, 3969.
    PointCache         *pointcache;          // 7 type, name 149, 1293.
    ListBase           ptcaches;             // 8 type, name 14, 842.
    int                numbodies;            // 9 type, name 4, 3970.
    short              steps_per_second;     // 10 type, name 2, 3971.
    short              num_solver_iterations;// 11 type, name 2, 3972.
    int                flag;                 // 12 type, name 4, 18.
    float              time_scale;           // 13 type, name 7, 3764.
    void               *physics_world;       // 14 type, name 11, 3973.
};

// 458 DNA structure 638, 2
struct LineStyleColorModifier_AlongStroke
{
    LineStyleModifier    modifier;   // 1 type, name 637, 757.
    ColorBand            *color_ramp;// 2 type, name 59, 4025.
};

// 459 DNA structure 639, 4
struct LineStyleAlphaModifier_AlongStroke
{
    LineStyleModifier    modifier;// 1 type, name 637, 757.
    CurveMapping         *curve;  // 2 type, name 63, 3233.
    int                  flags;   // 3 type, name 4, 91.
    int                  pad;     // 4 type, name 4, 68.
};

// 460 DNA structure 640, 6
struct LineStyleThicknessModifier_AlongStroke
{
    LineStyleModifier    modifier; // 1 type, name 637, 757.
    CurveMapping         *curve;   // 2 type, name 63, 3233.
    int                  flags;    // 3 type, name 4, 91.
    float                value_min;// 4 type, name 7, 4026.
    float                value_max;// 5 type, name 7, 4027.
    int                  pad;      // 6 type, name 4, 68.
};

// 461 DNA structure 641, 4
struct LineStyleColorModifier_DistanceFromCamera
{
    LineStyleModifier    modifier;   // 1 type, name 637, 757.
    ColorBand            *color_ramp;// 2 type, name 59, 4025.
    float                range_min;  // 3 type, name 7, 4028.
    float                range_max;  // 4 type, name 7, 4029.
};

// 462 DNA structure 642, 6
struct LineStyleAlphaModifier_DistanceFromCamera
{
    LineStyleModifier    modifier; // 1 type, name 637, 757.
    CurveMapping         *curve;   // 2 type, name 63, 3233.
    int                  flags;    // 3 type, name 4, 91.
    float                range_min;// 4 type, name 7, 4028.
    float                range_max;// 5 type, name 7, 4029.
    int                  pad;      // 6 type, name 4, 68.
};

// 463 DNA structure 643, 8
struct LineStyleThicknessModifier_DistanceFromCamera
{
    LineStyleModifier    modifier; // 1 type, name 637, 757.
    CurveMapping         *curve;   // 2 type, name 63, 3233.
    int                  flags;    // 3 type, name 4, 91.
    float                range_min;// 4 type, name 7, 4028.
    float                range_max;// 5 type, name 7, 4029.
    float                value_min;// 6 type, name 7, 4026.
    float                value_max;// 7 type, name 7, 4027.
    int                  pad;      // 8 type, name 4, 68.
};

// 464 DNA structure 644, 5
struct LineStyleColorModifier_DistanceFromObject
{
    LineStyleModifier    modifier;   // 1 type, name 637, 757.
    Object               *target;    // 2 type, name 29, 896.
    ColorBand            *color_ramp;// 3 type, name 59, 4025.
    float                range_min;  // 4 type, name 7, 4028.
    float                range_max;  // 5 type, name 7, 4029.
};

// 465 DNA structure 645, 7
struct LineStyleAlphaModifier_DistanceFromObject
{
    LineStyleModifier    modifier; // 1 type, name 637, 757.
    Object               *target;  // 2 type, name 29, 896.
    CurveMapping         *curve;   // 3 type, name 63, 3233.
    int                  flags;    // 4 type, name 4, 91.
    float                range_min;// 5 type, name 7, 4028.
    float                range_max;// 6 type, name 7, 4029.
    int                  pad;      // 7 type, name 4, 68.
};

// 466 DNA structure 646, 9
struct LineStyleThicknessModifier_DistanceFromObject
{
    LineStyleModifier    modifier; // 1 type, name 637, 757.
    Object               *target;  // 2 type, name 29, 896.
    CurveMapping         *curve;   // 3 type, name 63, 3233.
    int                  flags;    // 4 type, name 4, 91.
    float                range_min;// 5 type, name 7, 4028.
    float                range_max;// 6 type, name 7, 4029.
    float                value_min;// 7 type, name 7, 4026.
    float                value_max;// 8 type, name 7, 4027.
    int                  pad;      // 9 type, name 4, 68.
};

// 467 DNA structure 647, 4
struct LineStyleColorModifier_Material
{
    LineStyleModifier    modifier;   // 1 type, name 637, 757.
    ColorBand            *color_ramp;// 2 type, name 59, 4025.
    int                  flags;      // 3 type, name 4, 91.
    int                  mat_attr;   // 4 type, name 4, 4030.
};

// 468 DNA structure 648, 4
struct LineStyleAlphaModifier_Material
{
    LineStyleModifier    modifier;// 1 type, name 637, 757.
    CurveMapping         *curve;  // 2 type, name 63, 3233.
    int                  flags;   // 3 type, name 4, 91.
    int                  mat_attr;// 4 type, name 4, 4030.
};

// 469 DNA structure 649, 6
struct LineStyleThicknessModifier_Material
{
    LineStyleModifier    modifier; // 1 type, name 637, 757.
    CurveMapping         *curve;   // 2 type, name 63, 3233.
    int                  flags;    // 3 type, name 4, 91.
    float                value_min;// 4 type, name 7, 4026.
    float                value_max;// 5 type, name 7, 4027.
    int                  mat_attr; // 6 type, name 4, 4030.
};

// 470 DNA structure 650, 3
struct LineStyleGeometryModifier_Sampling
{
    LineStyleModifier    modifier;// 1 type, name 637, 757.
    float                sampling;// 2 type, name 7, 4031.
    int                  pad;     // 3 type, name 4, 68.
};

// 471 DNA structure 651, 3
struct LineStyleGeometryModifier_BezierCurve
{
    LineStyleModifier    modifier;// 1 type, name 637, 757.
    float                error;   // 2 type, name 7, 3814.
    int                  pad;     // 3 type, name 4, 68.
};

// 472 DNA structure 652, 5
struct LineStyleGeometryModifier_SinusDisplacement
{
    LineStyleModifier    modifier;  // 1 type, name 637, 757.
    float                wavelength;// 2 type, name 7, 4032.
    float                amplitude; // 3 type, name 7, 600.
    float                phase;     // 4 type, name 7, 3647.
    int                  pad;       // 5 type, name 4, 68.
};

// 473 DNA structure 653, 5
struct LineStyleGeometryModifier_SpatialNoise
{
    LineStyleModifier    modifier; // 1 type, name 637, 757.
    float                amplitude;// 2 type, name 7, 600.
    float                scale;    // 3 type, name 7, 972.
    int                  octaves;  // 4 type, name 4, 4033.
    int                  flags;    // 5 type, name 4, 91.
};

// 474 DNA structure 654, 7
struct LineStyleGeometryModifier_PerlinNoise1D
{
    LineStyleModifier    modifier; // 1 type, name 637, 757.
    float                frequency;// 2 type, name 7, 4034.
    float                amplitude;// 3 type, name 7, 600.
    float                angle;    // 4 type, name 7, 814.
    int                  octaves;  // 5 type, name 4, 4033.
    int                  seed;     // 6 type, name 4, 773.
    int                  pad1;     // 7 type, name 4, 70.
};

// 475 DNA structure 655, 7
struct LineStyleGeometryModifier_PerlinNoise2D
{
    LineStyleModifier    modifier; // 1 type, name 637, 757.
    float                frequency;// 2 type, name 7, 4034.
    float                amplitude;// 3 type, name 7, 600.
    float                angle;    // 4 type, name 7, 814.
    int                  octaves;  // 5 type, name 4, 4033.
    int                  seed;     // 6 type, name 4, 773.
    int                  pad1;     // 7 type, name 4, 70.
};

// 476 DNA structure 656, 3
struct LineStyleGeometryModifier_BackboneStretcher
{
    LineStyleModifier    modifier;       // 1 type, name 637, 757.
    float                backbone_length;// 2 type, name 7, 4035.
    int                  pad;            // 3 type, name 4, 68.
};

// 477 DNA structure 657, 3
struct LineStyleGeometryModifier_TipRemover
{
    LineStyleModifier    modifier;  // 1 type, name 637, 757.
    float                tip_length;// 2 type, name 7, 4036.
    int                  pad;       // 3 type, name 4, 68.
};

// 478 DNA structure 658, 3
struct LineStyleGeometryModifier_Polygonalization
{
    LineStyleModifier    modifier;// 1 type, name 637, 757.
    float                error;   // 2 type, name 7, 3814.
    int                  pad;     // 3 type, name 4, 68.
};

// 479 DNA structure 659, 3
struct LineStyleGeometryModifier_GuidingLines
{
    LineStyleModifier    modifier;// 1 type, name 637, 757.
    float                offset;  // 2 type, name 7, 143.
    int                  pad;     // 3 type, name 4, 68.
};

// 480 DNA structure 660, 7
struct LineStyleGeometryModifier_Blueprint
{
    LineStyleModifier    modifier;       // 1 type, name 637, 757.
    int                  flags;          // 2 type, name 4, 91.
    int                  rounds;         // 3 type, name 4, 4037.
    float                backbone_length;// 4 type, name 7, 4035.
    int                  random_radius;  // 5 type, name 4, 4038.
    int                  random_center;  // 6 type, name 4, 4039.
    int                  random_backbone;// 7 type, name 4, 4040.
};

// 481 DNA structure 661, 5
struct LineStyleGeometryModifier_2DOffset
{
    LineStyleModifier    modifier;// 1 type, name 637, 757.
    float                start;   // 2 type, name 7, 770.
    float                end;     // 3 type, name 7, 2686.
    float                x;       // 4 type, name 7, 5.
    float                y;       // 5 type, name 7, 6.
};

// 482 DNA structure 662, 9
struct LineStyleGeometryModifier_2DTransform
{
    LineStyleModifier    modifier;// 1 type, name 637, 757.
    int                  pivot;   // 2 type, name 4, 123.
    float                scale_x; // 3 type, name 7, 4041.
    float                scale_y; // 4 type, name 7, 4042.
    float                angle;   // 5 type, name 7, 814.
    float                pivot_u; // 6 type, name 7, 4043.
    float                pivot_x; // 7 type, name 7, 4044.
    float                pivot_y; // 8 type, name 7, 4045.
    int                  pad;     // 9 type, name 4, 68.
};

// 483 DNA structure 663, 5
struct LineStyleThicknessModifier_Calligraphy
{
    LineStyleModifier    modifier;     // 1 type, name 637, 757.
    float                min_thickness;// 2 type, name 7, 4046.
    float                max_thickness;// 3 type, name 7, 4047.
    float                orientation;  // 4 type, name 7, 4048.
    int                  pad;          // 5 type, name 4, 68.
};

// 484 DNA structure 21, 10
struct IDProperty
{
    IDProperty        *next;   // 1 type, name 21, 0.
    IDProperty        *prev;   // 2 type, name 21, 1.
    char              type;    // 3 type, name 0, 16.
    char              subtype; // 4 type, name 0, 17.
    short             flag;    // 5 type, name 2, 18.
    char              name[64];// 6 type, name 0, 19.
    int               saved;   // 7 type, name 4, 20.
    IDPropertyData    data;    // 8 type, name 20, 21.
    int               len;     // 9 type, name 4, 22.
    int               totallen;// 10 type, name 4, 23.
};

// 485 DNA structure 30, 21
struct IpoCurve
{
    IpoCurve     *next;    // 1 type, name 30, 0.
    IpoCurve     *prev;    // 2 type, name 30, 1.
    BPoint       *bp;      // 3 type, name 31, 50.
    BezTriple    *bezt;    // 4 type, name 32, 51.
    rctf         maxrct;   // 5 type, name 19, 52.
    rctf         totrct;   // 6 type, name 19, 53.
    short        blocktype;// 7 type, name 2, 47.
    short        adrcode;  // 8 type, name 2, 48.
    short        vartype;  // 9 type, name 2, 54.
    short        totvert;  // 10 type, name 2, 55.
    short        ipo;      // 11 type, name 2, 56.
    short        extrap;   // 12 type, name 2, 57.
    short        flag;     // 13 type, name 2, 18.
    short        rt;       // 14 type, name 2, 58.
    float        ymin;     // 15 type, name 7, 10.
    float        ymax;     // 16 type, name 7, 11.
    int          bitmask;  // 17 type, name 4, 59.
    float        slide_min;// 18 type, name 7, 60.
    float        slide_max;// 19 type, name 7, 61.
    float        curval;   // 20 type, name 7, 62.
    IpoDriver    *driver;  // 21 type, name 28, 63.
};

// 486 DNA structure 35, 15
struct Key
{
    ID          id;         // 1 type, name 22, 31.
    AnimData    *adt;       // 2 type, name 36, 77.
    KeyBlock    *refkey;    // 3 type, name 34, 78.
    char        elemstr[32];// 4 type, name 0, 79.
    int         elemsize;   // 5 type, name 4, 80.
    int         pad;        // 6 type, name 4, 68.
    ListBase    block;      // 7 type, name 14, 81.
    Ipo         *ipo;       // 8 type, name 33, 82.
    ID          *from;      // 9 type, name 22, 83.
    int         totkey;     // 10 type, name 4, 84.
    short       flag;       // 11 type, name 2, 18.
    char        type;       // 12 type, name 0, 16.
    char        pad2;       // 13 type, name 0, 29.
    float       ctime;      // 14 type, name 7, 85.
    int         uidgen;     // 15 type, name 4, 86.
};

// 487 DNA structure 38, 14
struct Text
{
    ID          id;       // 1 type, name 22, 31.
    char        *name;    // 2 type, name 0, 90.
    int         flags;    // 3 type, name 4, 91.
    int         nlines;   // 4 type, name 4, 92.
    ListBase    lines;    // 5 type, name 14, 93.
    TextLine    *curl;    // 6 type, name 37, 94.
    TextLine    *sell;    // 7 type, name 37, 95.
    int         curc;     // 8 type, name 4, 96.
    int         selc;     // 9 type, name 4, 97.
    char        *undo_buf;// 10 type, name 0, 98.
    int         undo_pos; // 11 type, name 4, 99.
    int         undo_len; // 12 type, name 4, 100.
    void        *compiled;// 13 type, name 11, 101.
    double      mtime;    // 14 type, name 8, 102.
};

// 488 DNA structure 57, 63
struct Tex
{
    ID              id;           // 1 type, name 22, 31.
    AnimData        *adt;         // 2 type, name 36, 77.
    float           noisesize;    // 3 type, name 7, 314.
    float           turbul;       // 4 type, name 7, 315.
    float           bright;       // 5 type, name 7, 316.
    float           contrast;     // 6 type, name 7, 317.
    float           saturation;   // 7 type, name 7, 318.
    float           rfac;         // 8 type, name 7, 319.
    float           gfac;         // 9 type, name 7, 320.
    float           bfac;         // 10 type, name 7, 321.
    float           filtersize;   // 11 type, name 7, 322.
    float           pad2;         // 12 type, name 7, 29.
    float           mg_H;         // 13 type, name 7, 323.
    float           mg_lacunarity;// 14 type, name 7, 324.
    float           mg_octaves;   // 15 type, name 7, 325.
    float           mg_offset;    // 16 type, name 7, 326.
    float           mg_gain;      // 17 type, name 7, 327.
    float           dist_amount;  // 18 type, name 7, 328.
    float           ns_outscale;  // 19 type, name 7, 329.
    float           vn_w1;        // 20 type, name 7, 330.
    float           vn_w2;        // 21 type, name 7, 331.
    float           vn_w3;        // 22 type, name 7, 332.
    float           vn_w4;        // 23 type, name 7, 333.
    float           vn_mexp;      // 24 type, name 7, 334.
    short           vn_distm;     // 25 type, name 2, 335.
    short           vn_coltype;   // 26 type, name 2, 336.
    short           noisedepth;   // 27 type, name 2, 337.
    short           noisetype;    // 28 type, name 2, 338.
    short           noisebasis;   // 29 type, name 2, 339.
    short           noisebasis2;  // 30 type, name 2, 340.
    short           imaflag;      // 31 type, name 2, 341.
    short           flag;         // 32 type, name 2, 18.
    short           type;         // 33 type, name 2, 16.
    short           stype;        // 34 type, name 2, 270.
    float           cropxmin;     // 35 type, name 7, 342.
    float           cropymin;     // 36 type, name 7, 343.
    float           cropxmax;     // 37 type, name 7, 344.
    float           cropymax;     // 38 type, name 7, 345.
    int             texfilter;    // 39 type, name 4, 346.
    int             afmax;        // 40 type, name 4, 347.
    short           xrepeat;      // 41 type, name 2, 348.
    short           yrepeat;      // 42 type, name 2, 349.
    short           extend;       // 43 type, name 2, 302.
    short           fie_ima;      // 44 type, name 2, 145.
    int             len;          // 45 type, name 4, 22.
    int             frames;       // 46 type, name 4, 142.
    int             offset;       // 47 type, name 4, 143.
    int             sfra;         // 48 type, name 4, 144.
    float           checkerdist;  // 49 type, name 7, 350.
    float           nabla;        // 50 type, name 7, 351.
    float           pad1;         // 51 type, name 7, 70.
    ImageUser       iuser;        // 52 type, name 44, 352.
    bNodeTree       *nodetree;    // 53 type, name 66, 353.
    Ipo             *ipo;         // 54 type, name 33, 82.
    Image           *ima;         // 55 type, name 51, 266.
    ColorBand       *coba;        // 56 type, name 59, 297.
    EnvMap          *env;         // 57 type, name 60, 354.
    PreviewImage    *preview;     // 58 type, name 26, 172.
    PointDensity    *pd;          // 59 type, name 62, 355.
    VoxelData       *vd;          // 60 type, name 64, 356.
    OceanTex        *ot;          // 61 type, name 65, 357.
    char            use_nodes;    // 62 type, name 0, 358.
    char            pad[7];       // 63 type, name 0, 119.
};

// 489 DNA structure 68, 9
struct ColorMapping
{
    ColorBand    coba;          // 1 type, name 59, 364.
    float        bright;        // 2 type, name 7, 316.
    float        contrast;      // 3 type, name 7, 317.
    float        saturation;    // 4 type, name 7, 318.
    int          flag;          // 5 type, name 4, 18.
    float        blend_color[3];// 6 type, name 7, 365.
    float        blend_factor;  // 7 type, name 7, 366.
    int          blend_type;    // 8 type, name 4, 367.
    int          pad[3];        // 9 type, name 4, 44.
};

// 490 DNA structure 145, 13
struct ClothModifierData
{
    ModifierData         modifier;          // 1 type, name 121, 757.
    Scene                *scene;            // 2 type, name 45, 140.
    void                 *clothObject;      // Note: using void* on undefined DNA type: Cloth
    ClothSimSettings     *sim_parms;        // 4 type, name 147, 839.
    ClothCollSettings    *coll_parms;       // 5 type, name 148, 840.
    PointCache           *point_cache;      // 6 type, name 149, 841.
    ListBase             ptcaches;          // 7 type, name 14, 842.
    void                 *hairdata;         // Note: using void* on undefined DNA type: ClothHairData
    float                hair_grid_min[3];  // 9 type, name 7, 844.
    float                hair_grid_max[3];  // 10 type, name 7, 845.
    int                  hair_grid_res[3];  // 11 type, name 4, 846.
    float                hair_grid_cellsize;// 12 type, name 7, 847.
    void                 *solver_result;    // Note: using void* on undefined DNA type: ClothSolverResult
};

// 491 DNA structure 222, 77
struct World
{
    ID              id;                  // 1 type, name 22, 31.
    AnimData        *adt;                // 2 type, name 36, 77.
    short           colormodel;          // 3 type, name 2, 211.
    short           totex;               // 4 type, name 2, 369.
    short           texact;              // 5 type, name 2, 404.
    short           mistype;             // 6 type, name 2, 1348.
    float           horr;                // 7 type, name 7, 1349.
    float           horg;                // 8 type, name 7, 1350.
    float           horb;                // 9 type, name 7, 1351.
    float           zenr;                // 10 type, name 7, 1352.
    float           zeng;                // 11 type, name 7, 1353.
    float           zenb;                // 12 type, name 7, 1354.
    float           ambr;                // 13 type, name 7, 455.
    float           ambg;                // 14 type, name 7, 457.
    float           ambb;                // 15 type, name 7, 456.
    float           exposure;            // 16 type, name 7, 1355.
    float           exp;                 // 17 type, name 7, 1356.
    float           range;               // 18 type, name 7, 1357.
    float           linfac;              // 19 type, name 7, 1358.
    float           logfac;              // 20 type, name 7, 1359.
    float           gravity;             // 21 type, name 7, 1360.
    float           activityBoxRadius;   // 22 type, name 7, 1361.
    short           skytype;             // 23 type, name 2, 1362.
    short           mode;                // 24 type, name 2, 368.
    short           occlusionRes;        // 25 type, name 2, 1363.
    short           physicsEngine;       // 26 type, name 2, 1364.
    short           ticrate;             // 27 type, name 2, 1365.
    short           maxlogicstep;        // 28 type, name 2, 1366.
    short           physubstep;          // 29 type, name 2, 1367.
    short           maxphystep;          // 30 type, name 2, 1368.
    float           misi;                // 31 type, name 7, 1369.
    float           miststa;             // 32 type, name 7, 1370.
    float           mistdist;            // 33 type, name 7, 1371.
    float           misthi;              // 34 type, name 7, 1372.
    float           starr;               // 35 type, name 7, 1373.
    float           starg;               // 36 type, name 7, 1374.
    float           starb;               // 37 type, name 7, 1375.
    float           stark;               // 38 type, name 7, 1376.
    float           starsize;            // 39 type, name 7, 1377.
    float           starmindist;         // 40 type, name 7, 1378.
    float           stardist;            // 41 type, name 7, 1379.
    float           starcolnoise;        // 42 type, name 7, 1380.
    short           dofsta;              // 43 type, name 2, 1381.
    short           dofend;              // 44 type, name 2, 1382.
    short           dofmin;              // 45 type, name 2, 1383.
    short           dofmax;              // 46 type, name 2, 1384.
    float           aodist;              // 47 type, name 7, 1385.
    float           aodistfac;           // 48 type, name 7, 1386.
    float           aoenergy;            // 49 type, name 7, 1387.
    float           aobias;              // 50 type, name 7, 1388.
    short           aomode;              // 51 type, name 2, 1389.
    short           aosamp;              // 52 type, name 2, 1390.
    short           aomix;               // 53 type, name 2, 1391.
    short           aocolor;             // 54 type, name 2, 1392.
    float           ao_adapt_thresh;     // 55 type, name 7, 1393.
    float           ao_adapt_speed_fac;  // 56 type, name 7, 1394.
    float           ao_approx_error;     // 57 type, name 7, 1395.
    float           ao_approx_correction;// 58 type, name 7, 1396.
    float           ao_indirect_energy;  // 59 type, name 7, 1397.
    float           ao_env_energy;       // 60 type, name 7, 1398.
    float           ao_pad2;             // 61 type, name 7, 1399.
    short           ao_indirect_bounces; // 62 type, name 2, 1400.
    short           ao_pad;              // 63 type, name 2, 1401.
    short           ao_samp_method;      // 64 type, name 2, 1402.
    short           ao_gather_method;    // 65 type, name 2, 1403.
    short           ao_approx_passes;    // 66 type, name 2, 1404.
    short           flag;                // 67 type, name 2, 18.
    float           *aosphere;           // 68 type, name 7, 1405.
    float           *aotables;           // 69 type, name 7, 1406.
    Ipo             *ipo;                // 70 type, name 33, 82.
    MTex            *mtex[18];           // 71 type, name 56, 423.
    short           pr_texture;          // 72 type, name 2, 424.
    short           use_nodes;           // 73 type, name 2, 358.
    short           pad[2];              // 74 type, name 2, 205.
    PreviewImage    *preview;            // 75 type, name 26, 172.
    bNodeTree       *nodetree;           // 76 type, name 66, 353.
    ListBase        gpumaterial;         // 77 type, name 14, 565.
};

// 492 DNA structure 267, 15
struct BGpic
{
    BGpic            *next;   // 1 type, name 267, 0.
    BGpic            *prev;   // 2 type, name 267, 1.
    Image            *ima;    // 3 type, name 51, 266.
    ImageUser        iuser;   // 4 type, name 44, 352.
    MovieClip        *clip;   // 5 type, name 265, 1839.
    MovieClipUser    cuser;   // 6 type, name 268, 1843.
    float            xof;     // 7 type, name 7, 651.
    float            yof;     // 8 type, name 7, 652.
    float            size;    // 9 type, name 7, 103.
    float            blend;   // 10 type, name 7, 1844.
    float            rotation;// 11 type, name 7, 1845.
    short            view;    // 12 type, name 2, 151.
    short            flag;    // 13 type, name 2, 18.
    short            source;  // 14 type, name 2, 161.
    char             pad[6];  // 15 type, name 0, 769.
};

// 493 DNA structure 322, 185
struct ThemeSpace
{
    char                back[4];                       // 1 type, name 0, 2140.
    char                title[4];                      // 2 type, name 0, 2174.
    char                text[4];                       // 3 type, name 0, 2127.
    char                text_hi[4];                    // 4 type, name 0, 2175.
    char                header[4];                     // 5 type, name 0, 2139.
    char                header_title[4];               // 6 type, name 0, 2176.
    char                header_text[4];                // 7 type, name 0, 2177.
    char                header_text_hi[4];             // 8 type, name 0, 2178.
    char                tab_active[4];                 // 9 type, name 0, 2179.
    char                tab_inactive[4];               // 10 type, name 0, 2180.
    char                tab_back[4];                   // 11 type, name 0, 2181.
    char                tab_outline[4];                // 12 type, name 0, 2182.
    char                button[4];                     // 13 type, name 0, 2183.
    char                button_title[4];               // 14 type, name 0, 2184.
    char                button_text[4];                // 15 type, name 0, 2185.
    char                button_text_hi[4];             // 16 type, name 0, 2186.
    char                list[4];                       // 17 type, name 0, 2187.
    char                list_title[4];                 // 18 type, name 0, 2188.
    char                list_text[4];                  // 19 type, name 0, 2189.
    char                list_text_hi[4];               // 20 type, name 0, 2190.
    uiPanelColors       panelcolors;                   // 21 type, name 319, 2191.
    uiGradientColors    gradients;                     // 22 type, name 320, 2192.
    char                shade1[4];                     // 23 type, name 0, 2193.
    char                shade2[4];                     // 24 type, name 0, 2194.
    char                hilite[4];                     // 25 type, name 0, 2195.
    char                grid[4];                       // 26 type, name 0, 2196.
    char                view_overlay[4];               // 27 type, name 0, 2197.
    char                wire[4];                       // 28 type, name 0, 2198.
    char                wire_edit[4];                  // 29 type, name 0, 2199.
    char                select[4];                     // 30 type, name 0, 2200.
    char                lamp[4];                       // 31 type, name 0, 2201.
    char                speaker[4];                    // 32 type, name 0, 2202.
    char                empty[4];                      // 33 type, name 0, 2203.
    char                camera[4];                     // 34 type, name 0, 2204.
    char                active[4];                     // 35 type, name 0, 2205.
    char                group[4];                      // 36 type, name 0, 2206.
    char                group_active[4];               // 37 type, name 0, 2207.
    char                transform[4];                  // 38 type, name 0, 2208.
    char                vertex[4];                     // 39 type, name 0, 2209.
    char                vertex_select[4];              // 40 type, name 0, 2210.
    char                vertex_unreferenced[4];        // 41 type, name 0, 2211.
    char                edge[4];                       // 42 type, name 0, 2212.
    char                edge_select[4];                // 43 type, name 0, 2213.
    char                edge_seam[4];                  // 44 type, name 0, 2214.
    char                edge_sharp[4];                 // 45 type, name 0, 2215.
    char                edge_facesel[4];               // 46 type, name 0, 2216.
    char                edge_crease[4];                // 47 type, name 0, 2217.
    char                face[4];                       // 48 type, name 0, 2218.
    char                face_select[4];                // 49 type, name 0, 2219.
    char                face_dot[4];                   // 50 type, name 0, 2220.
    char                extra_edge_len[4];             // 51 type, name 0, 2221.
    char                extra_edge_angle[4];           // 52 type, name 0, 2222.
    char                extra_face_angle[4];           // 53 type, name 0, 2223.
    char                extra_face_area[4];            // 54 type, name 0, 2224.
    char                normal[4];                     // 55 type, name 0, 2225.
    char                vertex_normal[4];              // 56 type, name 0, 2226.
    char                loop_normal[4];                // 57 type, name 0, 2227.
    char                bone_solid[4];                 // 58 type, name 0, 2228.
    char                bone_pose[4];                  // 59 type, name 0, 2229.
    char                bone_pose_active[4];           // 60 type, name 0, 2230.
    char                strip[4];                      // 61 type, name 0, 2231.
    char                strip_select[4];               // 62 type, name 0, 2232.
    char                cframe[4];                     // 63 type, name 0, 2233.
    char                time_keyframe[4];              // 64 type, name 0, 2234.
    char                time_gp_keyframe[4];           // 65 type, name 0, 2235.
    char                freestyle_edge_mark[4];        // 66 type, name 0, 2236.
    char                freestyle_face_mark[4];        // 67 type, name 0, 2237.
    char                nurb_uline[4];                 // 68 type, name 0, 2238.
    char                nurb_vline[4];                 // 69 type, name 0, 2239.
    char                act_spline[4];                 // 70 type, name 0, 2240.
    char                nurb_sel_uline[4];             // 71 type, name 0, 2241.
    char                nurb_sel_vline[4];             // 72 type, name 0, 2242.
    char                lastsel_point[4];              // 73 type, name 0, 2243.
    char                handle_free[4];                // 74 type, name 0, 2244.
    char                handle_auto[4];                // 75 type, name 0, 2245.
    char                handle_vect[4];                // 76 type, name 0, 2246.
    char                handle_align[4];               // 77 type, name 0, 2247.
    char                handle_auto_clamped[4];        // 78 type, name 0, 2248.
    char                handle_sel_free[4];            // 79 type, name 0, 2249.
    char                handle_sel_auto[4];            // 80 type, name 0, 2250.
    char                handle_sel_vect[4];            // 81 type, name 0, 2251.
    char                handle_sel_align[4];           // 82 type, name 0, 2252.
    char                handle_sel_auto_clamped[4];    // 83 type, name 0, 2253.
    char                ds_channel[4];                 // 84 type, name 0, 2254.
    char                ds_subchannel[4];              // 85 type, name 0, 2255.
    char                keytype_keyframe[4];           // 86 type, name 0, 2256.
    char                keytype_extreme[4];            // 87 type, name 0, 2257.
    char                keytype_breakdown[4];          // 88 type, name 0, 2258.
    char                keytype_jitter[4];             // 89 type, name 0, 2259.
    char                keytype_keyframe_select[4];    // 90 type, name 0, 2260.
    char                keytype_extreme_select[4];     // 91 type, name 0, 2261.
    char                keytype_breakdown_select[4];   // 92 type, name 0, 2262.
    char                keytype_jitter_select[4];      // 93 type, name 0, 2263.
    char                keyborder[4];                  // 94 type, name 0, 2264.
    char                keyborder_select[4];           // 95 type, name 0, 2265.
    char                console_output[4];             // 96 type, name 0, 2266.
    char                console_input[4];              // 97 type, name 0, 2267.
    char                console_info[4];               // 98 type, name 0, 2268.
    char                console_error[4];              // 99 type, name 0, 2269.
    char                console_cursor[4];             // 100 type, name 0, 2270.
    char                console_select[4];             // 101 type, name 0, 2271.
    char                vertex_size;                   // 102 type, name 0, 2272.
    char                outline_width;                 // 103 type, name 0, 2273.
    char                facedot_size;                  // 104 type, name 0, 2274.
    char                noodle_curving;                // 105 type, name 0, 2275.
    char                syntaxl[4];                    // 106 type, name 0, 2276.
    char                syntaxs[4];                    // 107 type, name 0, 2277.
    char                syntaxb[4];                    // 108 type, name 0, 2278.
    char                syntaxn[4];                    // 109 type, name 0, 2279.
    char                syntaxv[4];                    // 110 type, name 0, 2280.
    char                syntaxc[4];                    // 111 type, name 0, 2281.
    char                syntaxd[4];                    // 112 type, name 0, 2282.
    char                syntaxr[4];                    // 113 type, name 0, 2283.
    char                nodeclass_output[4];           // 114 type, name 0, 2284.
    char                nodeclass_filter[4];           // 115 type, name 0, 2285.
    char                nodeclass_vector[4];           // 116 type, name 0, 2286.
    char                nodeclass_texture[4];          // 117 type, name 0, 2287.
    char                nodeclass_shader[4];           // 118 type, name 0, 2288.
    char                nodeclass_script[4];           // 119 type, name 0, 2289.
    char                nodeclass_pattern[4];          // 120 type, name 0, 2290.
    char                nodeclass_layout[4];           // 121 type, name 0, 2291.
    char                movie[4];                      // 122 type, name 0, 2292.
    char                movieclip[4];                  // 123 type, name 0, 2293.
    char                mask[4];                       // 124 type, name 0, 2294.
    char                image[4];                      // 125 type, name 0, 2295.
    char                scene[4];                      // 126 type, name 0, 2296.
    char                audio[4];                      // 127 type, name 0, 2297.
    char                effect[4];                     // 128 type, name 0, 2298.
    char                transition[4];                 // 129 type, name 0, 2299.
    char                meta[4];                       // 130 type, name 0, 2300.
    char                editmesh_active[4];            // 131 type, name 0, 2301.
    char                handle_vertex[4];              // 132 type, name 0, 2302.
    char                handle_vertex_select[4];       // 133 type, name 0, 2303.
    char                handle_vertex_size;            // 134 type, name 0, 2304.
    char                clipping_border_3d[4];         // 135 type, name 0, 2305.
    char                marker_outline[4];             // 136 type, name 0, 2306.
    char                marker[4];                     // 137 type, name 0, 2307.
    char                act_marker[4];                 // 138 type, name 0, 2308.
    char                sel_marker[4];                 // 139 type, name 0, 2309.
    char                dis_marker[4];                 // 140 type, name 0, 2310.
    char                lock_marker[4];                // 141 type, name 0, 2311.
    char                bundle_solid[4];               // 142 type, name 0, 2312.
    char                path_before[4];                // 143 type, name 0, 2313.
    char                path_after[4];                 // 144 type, name 0, 2314.
    char                camera_path[4];                // 145 type, name 0, 2315.
    char                hpad[2];                       // 146 type, name 0, 2316.
    char                gp_vertex_size;                // 147 type, name 0, 2317.
    char                gp_vertex[4];                  // 148 type, name 0, 2318.
    char                gp_vertex_select[4];           // 149 type, name 0, 2319.
    char                preview_back[4];               // 150 type, name 0, 2320.
    char                preview_stitch_face[4];        // 151 type, name 0, 2321.
    char                preview_stitch_edge[4];        // 152 type, name 0, 2322.
    char                preview_stitch_vert[4];        // 153 type, name 0, 2323.
    char                preview_stitch_stitchable[4];  // 154 type, name 0, 2324.
    char                preview_stitch_unstitchable[4];// 155 type, name 0, 2325.
    char                preview_stitch_active[4];      // 156 type, name 0, 2326.
    char                uv_shadow[4];                  // 157 type, name 0, 2327.
    char                uv_others[4];                  // 158 type, name 0, 2328.
    char                match[4];                      // 159 type, name 0, 2329.
    char                selected_highlight[4];         // 160 type, name 0, 2330.
    char                skin_root[4];                  // 161 type, name 0, 2331.
    char                anim_active[4];                // 162 type, name 0, 2332.
    char                anim_non_active[4];            // 163 type, name 0, 2333.
    char                nla_tweaking[4];               // 164 type, name 0, 2334.
    char                nla_tweakdupli[4];             // 165 type, name 0, 2335.
    char                nla_transition[4];             // 166 type, name 0, 2336.
    char                nla_transition_sel[4];         // 167 type, name 0, 2337.
    char                nla_meta[4];                   // 168 type, name 0, 2338.
    char                nla_meta_sel[4];               // 169 type, name 0, 2339.
    char                nla_sound[4];                  // 170 type, name 0, 2340.
    char                nla_sound_sel[4];              // 171 type, name 0, 2341.
    char                info_selected[4];              // 172 type, name 0, 2342.
    char                info_selected_text[4];         // 173 type, name 0, 2343.
    char                info_error[4];                 // 174 type, name 0, 2344.
    char                info_error_text[4];            // 175 type, name 0, 2345.
    char                info_warning[4];               // 176 type, name 0, 2346.
    char                info_warning_text[4];          // 177 type, name 0, 2347.
    char                info_info[4];                  // 178 type, name 0, 2348.
    char                info_info_text[4];             // 179 type, name 0, 2349.
    char                info_debug[4];                 // 180 type, name 0, 2350.
    char                info_debug_text[4];            // 181 type, name 0, 2351.
    char                paint_curve_pivot[4];          // 182 type, name 0, 2352.
    char                paint_curve_handle[4];         // 183 type, name 0, 2353.
    char                metadatabg[4];                 // 184 type, name 0, 2354.
    char                metadatatext[4];               // 185 type, name 0, 2355.
};

// 494 DNA structure 363, 3
struct ColorBalanceModifierData
{
    SequenceModifierData    modifier;      // 1 type, name 362, 757.
    StripColorBalance       color_balance; // 2 type, name 350, 2674.
    float                   color_multiply;// 3 type, name 7, 2675.
};

// 495 DNA structure 74, 4
struct Group
{
    ID          id;          // 1 type, name 22, 31.
    ListBase    gobject;     // 2 type, name 14, 2830.
    int         layer;       // 3 type, name 4, 152.
    float       dupli_ofs[3];// 4 type, name 7, 2831.
};

// 496 DNA structure 424, 7
struct bActionGroup
{
    bActionGroup      *next;    // 1 type, name 424, 0.
    bActionGroup      *prev;    // 2 type, name 424, 1.
    ListBase          channels; // 3 type, name 14, 2928.
    int               flag;     // 4 type, name 4, 18.
    int               customCol;// 5 type, name 4, 2929.
    char              name[64]; // 6 type, name 0, 19.
    ThemeWireColor    cs;       // 7 type, name 323, 2930.
};

// 497 DNA structure 468, 5
struct bNodePreview
{
    bNodeInstanceHashEntry    hash_entry;// 1 type, name 467, 3071.
    char                      *rect;     // 2 type, name 0, 3072.
    short                     xsize;     // 3 type, name 2, 3073.
    short                     ysize;     // 4 type, name 2, 3074.
    int                       pad;       // 5 type, name 4, 68.
};

// 498 DNA structure 245, 4
struct Palette
{
    ID          id;          // 1 type, name 22, 31.
    ListBase    colors;      // 2 type, name 14, 3318.
    int         active_color;// 3 type, name 4, 3319.
    int         pad;         // 4 type, name 4, 68.
};

// 499 DNA structure 549, 19
struct ParticleData
{
    ParticleKey     state;      // 1 type, name 542, 1143.
    ParticleKey     prev_state; // 2 type, name 542, 3348.
    HairKey         *hair;      // 3 type, name 541, 3349.
    ParticleKey     *keys;      // 4 type, name 542, 2711.
    BoidParticle    *boid;      // 5 type, name 543, 3350.
    int             totkey;     // 6 type, name 4, 84.
    float           time;       // 7 type, name 7, 801.
    float           lifetime;   // 8 type, name 7, 828.
    float           dietime;    // 9 type, name 7, 3351.
    int             num;        // 10 type, name 4, 3342.
    int             num_dmcache;// 11 type, name 4, 3352.
    float           fuv[4];     // 12 type, name 7, 3346.
    float           foffset;    // 13 type, name 7, 3347.
    float           size;       // 14 type, name 7, 103.
    float           sphdensity; // 15 type, name 7, 3353.
    int             pad;        // 16 type, name 4, 68.
    int             hair_index; // 17 type, name 4, 3354.
    short           flag;       // 18 type, name 2, 18.
    short           alive;      // 19 type, name 2, 3355.
};

// 500 DNA structure 551, 142
struct ParticleSettings
{
    ID                  id;                 // 1 type, name 22, 31.
    AnimData            *adt;               // 2 type, name 36, 77.
    BoidSettings        *boids;             // 3 type, name 552, 3368.
    SPHFluidSettings    *fluid;             // 4 type, name 550, 3369.
    EffectorWeights     *effector_weights;  // 5 type, name 213, 1294.
    int                 flag;               // 6 type, name 4, 18.
    int                 rt;                 // 7 type, name 4, 58.
    short               type;               // 8 type, name 2, 16.
    short               from;               // 9 type, name 2, 2990.
    short               distr;              // 10 type, name 2, 3370.
    short               texact;             // 11 type, name 2, 404.
    short               phystype;           // 12 type, name 2, 3371.
    short               rotmode;            // 13 type, name 2, 1110.
    short               avemode;            // 14 type, name 2, 3372.
    short               reactevent;         // 15 type, name 2, 3373.
    int                 draw;               // 16 type, name 4, 3374.
    int                 pad1;               // 17 type, name 4, 70.
    short               draw_as;            // 18 type, name 2, 3375.
    short               draw_size;          // 19 type, name 2, 3376.
    short               childtype;          // 20 type, name 2, 3377.
    short               pad2;               // 21 type, name 2, 29.
    short               ren_as;             // 22 type, name 2, 3378.
    short               subframes;          // 23 type, name 2, 3379.
    short               draw_col;           // 24 type, name 2, 3380.
    short               draw_step;          // 25 type, name 2, 1668.
    short               ren_step;           // 26 type, name 2, 3381.
    short               hair_step;          // 27 type, name 2, 3382.
    short               keys_step;          // 28 type, name 2, 3383.
    short               adapt_angle;        // 29 type, name 2, 3384.
    short               adapt_pix;          // 30 type, name 2, 3385.
    short               disp;               // 31 type, name 2, 580.
    short               omat;               // 32 type, name 2, 2704.
    short               interpolation;      // 33 type, name 2, 2664.
    short               integrator;         // 34 type, name 2, 3386.
    short               rotfrom;            // 35 type, name 2, 3387.
    short               kink;               // 36 type, name 2, 1165.
    short               kink_axis;          // 37 type, name 2, 1166.
    short               bb_align;           // 38 type, name 2, 3388.
    short               bb_uv_split;        // 39 type, name 2, 3389.
    short               bb_anim;            // 40 type, name 2, 3390.
    short               bb_split_offset;    // 41 type, name 2, 3391.
    float               bb_tilt;            // 42 type, name 7, 3392.
    float               bb_rand_tilt;       // 43 type, name 7, 3393.
    float               bb_offset[2];       // 44 type, name 7, 3394.
    float               bb_size[2];         // 45 type, name 7, 3395.
    float               bb_vel_head;        // 46 type, name 7, 3396.
    float               bb_vel_tail;        // 47 type, name 7, 3397.
    float               color_vec_max;      // 48 type, name 7, 3398.
    short               simplify_flag;      // 49 type, name 2, 1574.
    short               simplify_refsize;   // 50 type, name 2, 3399.
    float               simplify_rate;      // 51 type, name 7, 3400.
    float               simplify_transition;// 52 type, name 7, 3401.
    float               simplify_viewport;  // 53 type, name 7, 3402.
    float               sta;                // 54 type, name 7, 2685.
    float               end;                // 55 type, name 7, 2686.
    float               lifetime;           // 56 type, name 7, 828.
    float               randlife;           // 57 type, name 7, 2692.
    float               timetweak;          // 58 type, name 7, 3403.
    float               courant_target;     // 59 type, name 7, 3404.
    float               jitfac;             // 60 type, name 7, 3405.
    float               eff_hair;           // 61 type, name 7, 3406.
    float               grid_rand;          // 62 type, name 7, 3407.
    float               ps_offset[1];       // 63 type, name 7, 3408.
    int                 totpart;            // 64 type, name 4, 2687.
    int                 userjit;            // 65 type, name 4, 2684.
    int                 grid_res;           // 66 type, name 4, 3409.
    int                 effector_amount;    // 67 type, name 4, 3410.
    short               time_flag;          // 68 type, name 2, 3411.
    short               time_pad[3];        // 69 type, name 2, 3412.
    float               normfac;            // 70 type, name 7, 2688.
    float               obfac;              // 71 type, name 7, 2689.
    float               randfac;            // 72 type, name 7, 2690.
    float               partfac;            // 73 type, name 7, 3413.
    float               tanfac;             // 74 type, name 7, 3414.
    float               tanphase;           // 75 type, name 7, 3415.
    float               reactfac;           // 76 type, name 7, 3416.
    float               ob_vel[3];          // 77 type, name 7, 3417.
    float               avefac;             // 78 type, name 7, 3418.
    float               phasefac;           // 79 type, name 7, 3419.
    float               randrotfac;         // 80 type, name 7, 3420.
    float               randphasefac;       // 81 type, name 7, 3421.
    float               mass;               // 82 type, name 7, 1096.
    float               size;               // 83 type, name 7, 103.
    float               randsize;           // 84 type, name 7, 3422.
    float               acc[3];             // 85 type, name 7, 3423.
    float               dragfac;            // 86 type, name 7, 3424.
    float               brownfac;           // 87 type, name 7, 3425.
    float               dampfac;            // 88 type, name 7, 245.
    float               randlength;         // 89 type, name 7, 3426.
    int                 child_flag;         // 90 type, name 4, 3427.
    int                 pad3;               // 91 type, name 4, 534.
    int                 child_nbr;          // 92 type, name 4, 3428.
    int                 ren_child_nbr;      // 93 type, name 4, 3429.
    float               parents;            // 94 type, name 7, 3430.
    float               childsize;          // 95 type, name 7, 3431.
    float               childrandsize;      // 96 type, name 7, 3432.
    float               childrad;           // 97 type, name 7, 3433.
    float               childflat;          // 98 type, name 7, 3434.
    float               clumpfac;           // 99 type, name 7, 244.
    float               clumppow;           // 100 type, name 7, 3435.
    float               kink_amp;           // 101 type, name 7, 1192.
    float               kink_freq;          // 102 type, name 7, 1190.
    float               kink_shape;         // 103 type, name 7, 1191.
    float               kink_flat;          // 104 type, name 7, 3436.
    float               kink_amp_clump;     // 105 type, name 7, 3437.
    int                 kink_extra_steps;   // 106 type, name 4, 3438.
    int                 pad4;               // 107 type, name 4, 1532.
    float               kink_axis_random;   // 108 type, name 7, 3439.
    float               kink_amp_random;    // 109 type, name 7, 3440.
    float               rough1;             // 110 type, name 7, 3441.
    float               rough1_size;        // 111 type, name 7, 3442.
    float               rough2;             // 112 type, name 7, 3443.
    float               rough2_size;        // 113 type, name 7, 3444.
    float               rough2_thres;       // 114 type, name 7, 3445.
    float               rough_end;          // 115 type, name 7, 3446.
    float               rough_end_shape;    // 116 type, name 7, 3447.
    float               clength;            // 117 type, name 7, 3448.
    float               clength_thres;      // 118 type, name 7, 3449.
    float               parting_fac;        // 119 type, name 7, 3450.
    float               parting_min;        // 120 type, name 7, 3451.
    float               parting_max;        // 121 type, name 7, 3452.
    float               branch_thres;       // 122 type, name 7, 3453.
    float               draw_line[2];       // 123 type, name 7, 3454.
    float               path_start;         // 124 type, name 7, 3455.
    float               path_end;           // 125 type, name 7, 3456.
    int                 trail_count;        // 126 type, name 4, 3457.
    int                 keyed_loops;        // 127 type, name 4, 3458.
    CurveMapping        *clumpcurve;        // 128 type, name 63, 3459.
    CurveMapping        *roughcurve;        // 129 type, name 63, 3460.
    float               clump_noise_size;   // 130 type, name 7, 3461.
    float               bending_random;     // 131 type, name 7, 3462.
    MTex                *mtex[18];          // 132 type, name 56, 423.
    Group               *dup_group;         // 133 type, name 74, 1134.
    ListBase            dupliweights;       // 134 type, name 14, 3463.
    Group               *eff_group;         // 135 type, name 74, 3464.
    Object              *dup_ob;            // 136 type, name 29, 3465.
    Object              *bb_ob;             // 137 type, name 29, 3466.
    Ipo                 *ipo;               // 138 type, name 33, 82.
    PartDeflect         *pd;                // 139 type, name 205, 355.
    PartDeflect         *pd2;               // 140 type, name 205, 3467.
    short               use_modifier_stack; // 141 type, name 2, 3468.
    short               pad5[3];            // 142 type, name 2, 3469.
};

// 501 DNA structure 201, 7
struct bGPdata
{
    ID          id;           // 1 type, name 22, 31.
    AnimData    *adt;         // 2 type, name 36, 77.
    ListBase    layers;       // 3 type, name 14, 1540.
    int         flag;         // 4 type, name 4, 18.
    short       sbuffer_size; // 5 type, name 2, 3559.
    short       sbuffer_sflag;// 6 type, name 2, 3560.
    void        *sbuffer;     // 7 type, name 11, 3561.
};

// 502 DNA structure 574, 11
struct wmKeyMap
{
    wmKeyMap    *next;       // 1 type, name 574, 0.
    wmKeyMap    *prev;       // 2 type, name 574, 1.
    ListBase    items;       // 3 type, name 14, 3621.
    ListBase    diff_items;  // 4 type, name 14, 3622.
    char        idname[64];  // 5 type, name 0, 2534.
    short       spaceid;     // 6 type, name 2, 3623.
    short       regionid;    // 7 type, name 2, 3624.
    short       flag;        // 8 type, name 2, 18.
    short       kmi_id;      // 9 type, name 2, 3625.
    int         (*poll)();   // 10 type, name 4, 3626.
    void        *modal_items;// 11 type, name 11, 3627.
};

// 503 DNA structure 625, 7
struct MovieTrackingDopesheet
{
    int         ok;               // 1 type, name 4, 147.
    short       sort_method;      // 2 type, name 2, 3885.
    short       flag;             // 3 type, name 2, 18.
    ListBase    coverage_segments;// 4 type, name 14, 3886.
    ListBase    channels;         // 5 type, name 14, 2928.
    int         tot_channel;      // 6 type, name 4, 3887.
    int         pad;              // 7 type, name 4, 68.
};

// 504 DNA structure 292, 9
struct Mask
{
    ID          id;         // 1 type, name 22, 31.
    AnimData    *adt;       // 2 type, name 36, 77.
    ListBase    masklayers; // 3 type, name 14, 3945.
    int         masklay_act;// 4 type, name 4, 3946.
    int         masklay_tot;// 5 type, name 4, 3947.
    int         sfra;       // 6 type, name 4, 144.
    int         efra;       // 7 type, name 4, 1272.
    int         flag;       // 8 type, name 4, 18.
    int         pad;        // 9 type, name 4, 68.
};

// 505 DNA structure 630, 5
struct MaskSplinePoint
{
    BezTriple            bezt;  // 1 type, name 32, 3954.
    int                  pad;   // 2 type, name 4, 68.
    int                  tot_uw;// 3 type, name 4, 3955.
    MaskSplinePointUW    *uw;   // 4 type, name 629, 3956.
    MaskParent           parent;// 5 type, name 628, 3343.
};

// 506 DNA structure 633, 14
struct MaskLayer
{
    MaskLayer          *next;         // 1 type, name 633, 0.
    MaskLayer          *prev;         // 2 type, name 633, 1.
    char               name[64];      // 3 type, name 0, 19.
    ListBase           splines;       // 4 type, name 14, 3962.
    ListBase           splines_shapes;// 5 type, name 14, 3963.
    MaskSpline         *act_spline;   // 6 type, name 631, 3964.
    MaskSplinePoint    *act_point;    // 7 type, name 630, 3965.
    float              alpha;         // 8 type, name 7, 463.
    char               blend;         // 9 type, name 0, 1844.
    char               blend_flag;    // 10 type, name 0, 3966.
    char               falloff;       // 11 type, name 0, 826.
    char               pad[7];        // 12 type, name 0, 119.
    char               flag;          // 13 type, name 0, 18.
    char               restrictflag;  // 14 type, name 0, 1126.
};

// 507 DNA structure 230, 8
struct FreestyleConfig
{
    ListBase    modules;             // 1 type, name 14, 4019.
    int         mode;                // 2 type, name 4, 368.
    int         raycasting_algorithm;// 3 type, name 4, 4020.
    int         flags;               // 4 type, name 4, 91.
    float       sphere_radius;       // 5 type, name 7, 4021.
    float       dkr_epsilon;         // 6 type, name 7, 4022.
    float       crease_angle;        // 7 type, name 7, 4023.
    ListBase    linesets;            // 8 type, name 14, 4024.
};

// 508 DNA structure 33, 7
struct Ipo
{
    ID          id;       // 1 type, name 22, 31.
    ListBase    curve;    // 2 type, name 14, 64.
    rctf        cur;      // 3 type, name 19, 65.
    short       blocktype;// 4 type, name 2, 47.
    short       showkey;  // 5 type, name 2, 66.
    short       muteipo;  // 6 type, name 2, 67.
    short       pad;      // 7 type, name 2, 68.
};

// 509 DNA structure 43, 22
struct Camera
{
    ID                      id;            // 1 type, name 22, 31.
    AnimData                *adt;          // 2 type, name 36, 77.
    char                    type;          // 3 type, name 0, 16.
    char                    dtx;           // 4 type, name 0, 124.
    short                   flag;          // 5 type, name 2, 18.
    float                   passepartalpha;// 6 type, name 7, 125.
    float                   clipsta;       // 7 type, name 7, 126.
    float                   clipend;       // 8 type, name 7, 127.
    float                   lens;          // 9 type, name 7, 128.
    float                   ortho_scale;   // 10 type, name 7, 129.
    float                   drawsize;      // 11 type, name 7, 130.
    float                   sensor_x;      // 12 type, name 7, 131.
    float                   sensor_y;      // 13 type, name 7, 132.
    float                   shiftx;        // 14 type, name 7, 133.
    float                   shifty;        // 15 type, name 7, 134.
    float                   YF_dofdist;    // 16 type, name 7, 135.
    Ipo                     *ipo;          // 17 type, name 33, 82.
    Object                  *dof_ob;       // 18 type, name 29, 136.
    GPUDOFSettings          gpu_dof;       // 19 type, name 39, 137.
    char                    sensor_fit;    // 20 type, name 0, 138.
    char                    pad[7];        // 21 type, name 0, 119.
    CameraStereoSettings    stereo;        // 22 type, name 42, 139.
};

// 510 DNA structure 79, 19
struct MetaBall
{
    ID          id;        // 1 type, name 22, 31.
    AnimData    *adt;      // 2 type, name 36, 77.
    ListBase    elems;     // 3 type, name 14, 579.
    ListBase    disp;      // 4 type, name 14, 580.
    ListBase    *editelems;// 5 type, name 14, 581.
    Ipo         *ipo;      // 6 type, name 33, 82.
    Material    **mat;     // 7 type, name 73, 582.
    char        flag;      // 8 type, name 0, 18.
    char        flag2;     // 9 type, name 0, 583.
    short       totcol;    // 10 type, name 2, 584.
    short       texflag;   // 11 type, name 2, 210.
    short       pad;       // 12 type, name 2, 68.
    float       loc[3];    // 13 type, name 7, 359.
    float       size[3];   // 14 type, name 7, 207.
    float       rot[3];    // 15 type, name 7, 360.
    float       wiresize;  // 16 type, name 7, 585.
    float       rendersize;// 17 type, name 7, 586.
    float       thresh;    // 18 type, name 7, 587.
    MetaElem    *lastelem; // 19 type, name 77, 588.
};

// 511 DNA structure 85, 70
struct Curve
{
    ID          id;             // 1 type, name 22, 31.
    AnimData    *adt;           // 2 type, name 36, 77.
    BoundBox    *bb;            // 3 type, name 78, 567.
    ListBase    nurb;           // 4 type, name 14, 624.
    EditNurb    *editnurb;      // 5 type, name 83, 625.
    Object      *bevobj;        // 6 type, name 29, 626.
    Object      *taperobj;      // 7 type, name 29, 627.
    Object      *textoncurve;   // 8 type, name 29, 628.
    Ipo         *ipo;           // 9 type, name 33, 82.
    Key         *key;           // 10 type, name 35, 629.
    Material    **mat;          // 11 type, name 73, 582.
    float       loc[3];         // 12 type, name 7, 359.
    float       size[3];        // 13 type, name 7, 207.
    float       rot[3];         // 14 type, name 7, 360.
    short       type;           // 15 type, name 2, 16.
    short       texflag;        // 16 type, name 2, 210.
    short       drawflag;       // 17 type, name 2, 630.
    short       twist_mode;     // 18 type, name 2, 631.
    float       twist_smooth;   // 19 type, name 7, 632.
    float       smallcaps_scale;// 20 type, name 7, 633.
    int         pathlen;        // 21 type, name 4, 634.
    short       bevresol;       // 22 type, name 2, 635.
    short       totcol;         // 23 type, name 2, 584.
    int         flag;           // 24 type, name 4, 18.
    float       width;          // 25 type, name 7, 636.
    float       ext1;           // 26 type, name 7, 637.
    float       ext2;           // 27 type, name 7, 638.
    short       resolu;         // 28 type, name 2, 607.
    short       resolv;         // 29 type, name 2, 608.
    short       resolu_ren;     // 30 type, name 2, 639.
    short       resolv_ren;     // 31 type, name 2, 640.
    int         actnu;          // 32 type, name 4, 641.
    int         actvert;        // 33 type, name 4, 642.
    char        pad[4];         // 34 type, name 0, 602.
    short       lines;          // 35 type, name 2, 93.
    char        spacemode;      // 36 type, name 0, 643.
    char        pad1;           // 37 type, name 0, 70.
    float       spacing;        // 38 type, name 7, 644.
    float       linedist;       // 39 type, name 7, 645.
    float       shear;          // 40 type, name 7, 646.
    float       fsize;          // 41 type, name 7, 647.
    float       wordspace;      // 42 type, name 7, 648.
    float       ulpos;          // 43 type, name 7, 649.
    float       ulheight;       // 44 type, name 7, 650.
    float       xof;            // 45 type, name 7, 651.
    float       yof;            // 46 type, name 7, 652.
    float       linewidth;      // 47 type, name 7, 653.
    int         pos;            // 48 type, name 4, 69.
    int         selstart;       // 49 type, name 4, 654.
    int         selend;         // 50 type, name 4, 655.
    int         len_wchar;      // 51 type, name 4, 656.
    int         len;            // 52 type, name 4, 22.
    char        *str;           // 53 type, name 0, 657.
    void        *editfont;      // Note: using void* on undefined DNA type: EditFont
    char        family[64];     // 55 type, name 0, 659.
    VFont       *vfont;         // 56 type, name 75, 660.
    VFont       *vfontb;        // 57 type, name 75, 661.
    VFont       *vfonti;        // 58 type, name 75, 662.
    VFont       *vfontbi;       // 59 type, name 75, 663.
    TextBox     *tb;            // 60 type, name 82, 664.
    int         totbox;         // 61 type, name 4, 665.
    int         actbox;         // 62 type, name 4, 666.
    CharInfo    *strinfo;       // 63 type, name 81, 667.
    CharInfo    curinfo;        // 64 type, name 81, 668.
    float       ctime;          // 65 type, name 7, 85.
    float       bevfac1;        // 66 type, name 7, 669.
    float       bevfac2;        // 67 type, name 7, 670.
    char        bevfac1_mapping;// 68 type, name 0, 671.
    char        bevfac2_mapping;// 69 type, name 0, 672.
    char        pad2[2];        // 70 type, name 0, 673.
};

// 512 DNA structure 103, 14
struct Multires
{
    ListBase      levels;       // 1 type, name 14, 744.
    MVert         *verts;       // 2 type, name 97, 743.
    char          level_count;  // 3 type, name 0, 745.
    char          current;      // 4 type, name 0, 746.
    char          newlvl;       // 5 type, name 0, 747.
    char          edgelvl;      // 6 type, name 0, 748.
    char          pinlvl;       // 7 type, name 0, 749.
    char          renderlvl;    // 8 type, name 0, 750.
    char          use_col;      // 9 type, name 0, 751.
    char          flag;         // 10 type, name 0, 18.
    CustomData    vdata;        // 11 type, name 102, 689.
    CustomData    fdata;        // 12 type, name 102, 691.
    short         *edge_flags;  // 13 type, name 2, 752.
    char          *edge_creases;// 14 type, name 0, 753.
};

// 513 DNA structure 229, 14
struct SceneRenderLayer
{
    SceneRenderLayer    *next;               // 1 type, name 229, 0.
    SceneRenderLayer    *prev;               // 2 type, name 229, 1.
    char                name[64];            // 3 type, name 0, 19.
    Material            *mat_override;       // 4 type, name 73, 1460.
    Group               *light_override;     // 5 type, name 74, 1461.
    int                 lay;                 // 6 type, name 4, 1080.
    int                 lay_zmask;           // 7 type, name 4, 1462.
    int                 lay_exclude;         // 8 type, name 4, 1463.
    int                 layflag;             // 9 type, name 4, 1464.
    int                 passflag;            // 10 type, name 4, 1465.
    int                 pass_xor;            // 11 type, name 4, 1466.
    int                 samples;             // 12 type, name 4, 115.
    float               pass_alpha_threshold;// 13 type, name 7, 1467.
    FreestyleConfig     freestyleConfig;     // 14 type, name 230, 1468.
};

// 514 DNA structure 232, 18
struct ImageFormatData
{
    char                           imtype;          // 1 type, name 0, 1474.
    char                           depth;           // 2 type, name 0, 274.
    char                           planes;          // 3 type, name 0, 1475.
    char                           flag;            // 4 type, name 0, 18.
    char                           quality;         // 5 type, name 0, 1476.
    char                           compress;        // 6 type, name 0, 1477.
    char                           exr_codec;       // 7 type, name 0, 1478.
    char                           cineon_flag;     // 8 type, name 0, 1479.
    short                          cineon_white;    // 9 type, name 2, 1480.
    short                          cineon_black;    // 10 type, name 2, 1481.
    float                          cineon_gamma;    // 11 type, name 7, 1482.
    char                           jp2_flag;        // 12 type, name 0, 1483.
    char                           jp2_codec;       // 13 type, name 0, 1484.
    char                           pad[5];          // 14 type, name 0, 186.
    char                           views_format;    // 15 type, name 0, 188.
    Stereo3dFormat                 stereo3d_format; // 16 type, name 55, 1485.
    ColorManagedViewSettings       view_settings;   // 17 type, name 233, 1486.
    ColorManagedDisplaySettings    display_settings;// 18 type, name 234, 1487.
};

// 515 DNA structure 241, 39
struct GameData
{
    GameFraming    framing;              // 1 type, name 239, 1623.
    short          playerflag;           // 2 type, name 2, 1624.
    short          xplay;                // 3 type, name 2, 1509.
    short          yplay;                // 4 type, name 2, 1510.
    short          freqplay;             // 5 type, name 2, 1511.
    short          depth;                // 6 type, name 2, 274.
    short          attrib;               // 7 type, name 2, 1512.
    short          rt1;                  // 8 type, name 2, 1625.
    short          rt2;                  // 9 type, name 2, 1626.
    short          aasamples;            // 10 type, name 2, 1627.
    short          pad4[3];              // 11 type, name 2, 563.
    GameDome       dome;                 // 12 type, name 238, 1628.
    short          stereoflag;           // 13 type, name 2, 1629.
    short          stereomode;           // 14 type, name 2, 1514.
    float          eyeseparation;        // 15 type, name 7, 1630.
    RecastData     recastData;           // 16 type, name 240, 1631.
    float          gravity;              // 17 type, name 7, 1360.
    float          activityBoxRadius;    // 18 type, name 7, 1361.
    int            flag;                 // 19 type, name 4, 18.
    short          mode;                 // 20 type, name 2, 368.
    short          matmode;              // 21 type, name 2, 1632.
    short          occlusionRes;         // 22 type, name 2, 1363.
    short          physicsEngine;        // 23 type, name 2, 1364.
    short          exitkey;              // 24 type, name 2, 1633.
    short          vsync;                // 25 type, name 2, 1634.
    short          ticrate;              // 26 type, name 2, 1365.
    short          maxlogicstep;         // 27 type, name 2, 1366.
    short          physubstep;           // 28 type, name 2, 1367.
    short          maxphystep;           // 29 type, name 2, 1368.
    short          obstacleSimulation;   // 30 type, name 2, 1635.
    short          raster_storage;       // 31 type, name 2, 1636.
    float          levelHeight;          // 32 type, name 7, 1637.
    float          deactivationtime;     // 33 type, name 7, 1638.
    float          lineardeactthreshold; // 34 type, name 7, 1639.
    float          angulardeactthreshold;// 35 type, name 7, 1640.
    short          lodflag;              // 36 type, name 2, 1641.
    short          pad2;                 // 37 type, name 2, 29.
    int            scehysteresis;        // 38 type, name 4, 1642.
    int            pad5;                 // 39 type, name 4, 386.
};

// 516 DNA structure 302, 33
struct SpaceText
{
    SpaceLink    *next;          // 1 type, name 276, 0.
    SpaceLink    *prev;          // 2 type, name 276, 1.
    ListBase     regionbase;     // 3 type, name 14, 1884.
    int          spacetype;      // 4 type, name 4, 1885.
    float        blockscale;     // 5 type, name 7, 1886.
    short        blockhandler[8];// 6 type, name 2, 1887.
    Text         *text;          // 7 type, name 38, 2030.
    int          top;            // 8 type, name 4, 2031.
    int          viewlines;      // 9 type, name 4, 2032.
    short        flags;          // 10 type, name 2, 91.
    short        menunr;         // 11 type, name 2, 2033.
    short        lheight;        // 12 type, name 2, 2034.
    char         cwidth;         // 13 type, name 0, 2035.
    char         linenrs_tot;    // 14 type, name 0, 2036.
    int          left;           // 15 type, name 4, 2037.
    int          showlinenrs;    // 16 type, name 4, 2038.
    int          tabnumber;      // 17 type, name 4, 2039.
    short        showsyntax;     // 18 type, name 2, 2040.
    short        line_hlight;    // 19 type, name 2, 2041.
    short        overwrite;      // 20 type, name 2, 2042.
    short        live_edit;      // 21 type, name 2, 2043.
    float        pix_per_line;   // 22 type, name 7, 2044.
    rcti         txtscroll;      // 23 type, name 18, 2045.
    rcti         txtbar;         // 24 type, name 18, 2046.
    int          wordwrap;       // 25 type, name 4, 2047.
    int          doplugins;      // 26 type, name 4, 2048.
    char         findstr[256];   // 27 type, name 0, 2049.
    char         replacestr[256];// 28 type, name 0, 2050.
    short        margin_column;  // 29 type, name 2, 2051.
    short        lheight_dpi;    // 30 type, name 2, 2052.
    char         pad[4];         // 31 type, name 0, 602.
    void         *drawcache;     // 32 type, name 11, 2053.
    float        scroll_accum[2];// 33 type, name 7, 2054.
};

// 517 DNA structure 310, 14
struct SpaceConsole
{
    SpaceLink    *next;          // 1 type, name 276, 0.
    SpaceLink    *prev;          // 2 type, name 276, 1.
    ListBase     regionbase;     // 3 type, name 14, 1884.
    int          spacetype;      // 4 type, name 4, 1885.
    float        blockscale;     // 5 type, name 7, 1886.
    short        blockhandler[8];// 6 type, name 2, 1887.
    int          lheight;        // 7 type, name 4, 2034.
    int          pad;            // 8 type, name 4, 68.
    ListBase     scrollback;     // 9 type, name 14, 2079.
    ListBase     history;        // 10 type, name 14, 2080.
    char         prompt[256];    // 11 type, name 0, 2081.
    char         language[32];   // 12 type, name 0, 2082.
    int          sel_start;      // 13 type, name 4, 2083.
    int          sel_end;        // 14 type, name 4, 2084.
};

// 518 DNA structure 353, 55
struct Sequence
{
    Sequence          *next;           // 1 type, name 353, 0.
    Sequence          *prev;           // 2 type, name 353, 1.
    void              *tmp;            // 3 type, name 11, 2605.
    void              *lib;            // 4 type, name 11, 25.
    char              name[64];        // 5 type, name 0, 19.
    int               flag;            // 6 type, name 4, 18.
    int               type;            // 7 type, name 4, 16.
    int               len;             // 8 type, name 4, 22.
    int               start;           // 9 type, name 4, 770.
    int               startofs;        // 10 type, name 4, 2606.
    int               endofs;          // 11 type, name 4, 2607.
    int               startstill;      // 12 type, name 4, 2599.
    int               endstill;        // 13 type, name 4, 2600.
    int               machine;         // 14 type, name 4, 2608.
    int               depth;           // 15 type, name 4, 274.
    int               startdisp;       // 16 type, name 4, 2609.
    int               enddisp;         // 17 type, name 4, 2610.
    float             sat;             // 18 type, name 7, 2611.
    float             mul;             // 19 type, name 7, 2612.
    float             handsize;        // 20 type, name 7, 2613.
    short             anim_preseek;    // 21 type, name 2, 2614.
    short             streamindex;     // 22 type, name 2, 2615.
    int               multicam_source; // 23 type, name 4, 2616.
    int               clip_flag;       // 24 type, name 4, 2617.
    Strip             *strip;          // 25 type, name 352, 2618.
    Ipo               *ipo;            // 26 type, name 33, 82.
    Scene             *scene;          // 27 type, name 45, 140.
    Object            *scene_camera;   // 28 type, name 29, 2619.
    MovieClip         *clip;           // 29 type, name 265, 1839.
    Mask              *mask;           // 30 type, name 292, 1983.
    ListBase          anims;           // 31 type, name 14, 156.
    float             effect_fader;    // 32 type, name 7, 2620.
    float             speed_fader;     // 33 type, name 7, 2621.
    Sequence          *seq1;           // 34 type, name 353, 2622.
    Sequence          *seq2;           // 35 type, name 353, 2623.
    Sequence          *seq3;           // 36 type, name 353, 2624.
    ListBase          seqbase;         // 37 type, name 14, 2625.
    bSound            *sound;          // 38 type, name 354, 2626.
    void              *scene_sound;    // 39 type, name 11, 2627.
    float             volume;          // 40 type, name 7, 1224.
    float             pitch;           // 41 type, name 7, 2628.
    float             pan;             // 42 type, name 7, 2629.
    float             strobe;          // 43 type, name 7, 2630.
    void              *effectdata;     // 44 type, name 11, 2631.
    int               anim_startofs;   // 45 type, name 4, 2632.
    int               anim_endofs;     // 46 type, name 4, 2633.
    int               blend_mode;      // 47 type, name 4, 2634.
    float             blend_opacity;   // 48 type, name 7, 2635.
    int               sfra;            // 49 type, name 4, 144.
    char              alpha_mode;      // 50 type, name 0, 185.
    char              pad[2];          // 51 type, name 0, 205.
    char              views_format;    // 52 type, name 0, 188.
    Stereo3dFormat    *stereo3d_format;// 53 type, name 55, 190.
    IDProperty        *prop;           // 54 type, name 21, 2378.
    ListBase          modifiers;       // 55 type, name 14, 1063.
};

// 519 DNA structure 261, 12
struct Editing
{
    ListBase    *seqbasep;         // 1 type, name 14, 2639.
    ListBase    seqbase;           // 2 type, name 14, 2625.
    ListBase    metastack;         // 3 type, name 14, 2640.
    Sequence    *act_seq;          // 4 type, name 353, 2641.
    char        act_imagedir[1024];// 5 type, name 0, 2642.
    char        act_sounddir[1024];// 6 type, name 0, 2643.
    char        proxy_dir[1024];   // 7 type, name 0, 2644.
    int         over_ofs;          // 8 type, name 4, 2645.
    int         over_cfra;         // 9 type, name 4, 2646.
    int         over_flag;         // 10 type, name 4, 2647.
    int         proxy_storage;     // 11 type, name 4, 2648.
    rctf        over_border;       // 12 type, name 19, 2649.
};

// 520 DNA structure 418, 27
struct bArmature
{
    ID          id;             // 1 type, name 22, 31.
    AnimData    *adt;           // 2 type, name 36, 77.
    ListBase    bonebase;       // 3 type, name 14, 2848.
    ListBase    chainbase;      // 4 type, name 14, 2849.
    ListBase    *edbo;          // 5 type, name 14, 2850.
    Bone        *act_bone;      // 6 type, name 417, 2851.
    void        *act_edbone;    // Note: using void* on undefined DNA type: EditBone
    void        *sketch;        // 8 type, name 11, 2853.
    int         flag;           // 9 type, name 4, 18.
    int         drawtype;       // 10 type, name 4, 1897.
    int         gevertdeformer; // 11 type, name 4, 2854.
    int         pad;            // 12 type, name 4, 68.
    short       deformflag;     // 13 type, name 2, 829.
    short       pathflag;       // 14 type, name 2, 1956.
    int         layer_used;     // 15 type, name 4, 2855.
    int         layer;          // 16 type, name 4, 152.
    int         layer_protected;// 17 type, name 4, 2856.
    short       ghostep;        // 18 type, name 2, 2857.
    short       ghostsize;      // 19 type, name 2, 2858.
    short       ghosttype;      // 20 type, name 2, 2859.
    short       pathsize;       // 21 type, name 2, 2860.
    int         ghostsf;        // 22 type, name 4, 2861.
    int         ghostef;        // 23 type, name 4, 2862.
    int         pathsf;         // 24 type, name 4, 2863.
    int         pathef;         // 25 type, name 4, 2864.
    int         pathbc;         // 26 type, name 4, 2865.
    int         pathac;         // 27 type, name 4, 2866.
};

// 521 DNA structure 421, 40
struct bPoseChannel
{
    bPoseChannel    *next;         // 1 type, name 421, 0.
    bPoseChannel    *prev;         // 2 type, name 421, 1.
    IDProperty      *prop;         // 3 type, name 21, 2378.
    ListBase        constraints;   // 4 type, name 14, 1129.
    char            name[64];      // 5 type, name 0, 19.
    short           flag;          // 6 type, name 2, 18.
    short           ikflag;        // 7 type, name 2, 2885.
    short           protectflag;   // 8 type, name 2, 1083.
    short           agrp_index;    // 9 type, name 2, 2886.
    char            constflag;     // 10 type, name 0, 2887.
    char            selectflag;    // 11 type, name 0, 2888.
    char            pad0[6];       // 12 type, name 0, 2889.
    Bone            *bone;         // 13 type, name 417, 2890.
    bPoseChannel    *parent;       // 14 type, name 421, 36.
    bPoseChannel    *child;        // 15 type, name 421, 2891.
    ListBase        iktree;        // 16 type, name 14, 2892.
    ListBase        siktree;       // 17 type, name 14, 2893.
    bMotionPath     *mpath;        // 18 type, name 203, 1059.
    Object          *custom;       // 19 type, name 29, 2894.
    bPoseChannel    *custom_tx;    // 20 type, name 421, 2895.
    float           loc[3];        // 21 type, name 7, 359.
    float           size[3];       // 22 type, name 7, 207.
    float           eul[3];        // 23 type, name 7, 2896.
    float           quat[4];       // 24 type, name 7, 570.
    float           rotAxis[3];    // 25 type, name 7, 1073.
    float           rotAngle;      // 26 type, name 7, 1075.
    short           rotmode;       // 27 type, name 2, 1110.
    short           pad;           // 28 type, name 2, 68.
    float           chan_mat[4][4];// 29 type, name 7, 2897.
    float           pose_mat[4][4];// 30 type, name 7, 2898.
    float           constinv[4][4];// 31 type, name 7, 1078.
    float           pose_head[3];  // 32 type, name 7, 2899.
    float           pose_tail[3];  // 33 type, name 7, 2900.
    float           limitmin[3];   // 34 type, name 7, 2901.
    float           limitmax[3];   // 35 type, name 7, 2902.
    float           stiffness[3];  // 36 type, name 7, 2903.
    float           ikstretch;     // 37 type, name 7, 2904.
    float           ikrotweight;   // 38 type, name 7, 2905.
    float           iklinweight;   // 39 type, name 7, 2906.
    void            *temp;         // 40 type, name 11, 2907.
};

// 522 DNA structure 200, 16
struct bPose
{
    ListBase            chanbase;          // 1 type, name 14, 2908.
    void                *chanhash;         // Note: using void* on undefined DNA type: GHash
    short               flag;              // 3 type, name 2, 18.
    short               pad;               // 4 type, name 2, 68.
    int                 proxy_layer;       // 5 type, name 4, 2910.
    int                 pad1;              // 6 type, name 4, 70.
    float               ctime;             // 7 type, name 7, 85.
    float               stride_offset[3];  // 8 type, name 7, 2911.
    float               cyclic_offset[3];  // 9 type, name 7, 2912.
    ListBase            agroups;           // 10 type, name 14, 2913.
    int                 active_group;      // 11 type, name 4, 2914.
    int                 iksolver;          // 12 type, name 4, 2915.
    void                *ikdata;           // 13 type, name 11, 2916.
    void                *ikparam;          // 14 type, name 11, 2917.
    bAnimVizSettings    avs;               // 15 type, name 202, 1058.
    char                proxy_act_bone[64];// 16 type, name 0, 2918.
};

// 523 DNA structure 63, 11
struct CurveMapping
{
    int         flag;             // 1 type, name 4, 18.
    int         cur;              // 2 type, name 4, 65.
    int         preset;           // 3 type, name 4, 3238.
    int         changed_timestamp;// 4 type, name 4, 3239.
    rctf        curr;             // 5 type, name 19, 3240.
    rctf        clipr;            // 6 type, name 19, 3241.
    CurveMap    cm[4];            // 7 type, name 534, 3242.
    float       black[3];         // 8 type, name 7, 3243.
    float       white[3];         // 9 type, name 7, 3244.
    float       bwmul[3];         // 10 type, name 7, 3245.
    float       sample[3];        // 11 type, name 7, 3246.
};

// 524 DNA structure 593, 25
struct NlaStrip
{
    NlaStrip      *next;          // 1 type, name 593, 0.
    NlaStrip      *prev;          // 2 type, name 593, 1.
    ListBase      strips;         // 3 type, name 14, 3667.
    bAction       *act;           // 4 type, name 199, 2759.
    AnimMapper    *remap;         // 5 type, name 592, 3668.
    ListBase      fcurves;        // 6 type, name 14, 3669.
    ListBase      modifiers;      // 7 type, name 14, 1063.
    char          name[64];       // 8 type, name 0, 19.
    float         influence;      // 9 type, name 7, 2811.
    float         strip_time;     // 10 type, name 7, 3670.
    float         start;          // 11 type, name 7, 770.
    float         end;            // 12 type, name 7, 2686.
    float         actstart;       // 13 type, name 7, 3018.
    float         actend;         // 14 type, name 7, 3019.
    float         repeat;         // 15 type, name 7, 818.
    float         scale;          // 16 type, name 7, 972.
    float         blendin;        // 17 type, name 7, 2761.
    float         blendout;       // 18 type, name 7, 3022.
    short         blendmode;      // 19 type, name 2, 3671.
    short         extendmode;     // 20 type, name 2, 3672.
    short         pad1;           // 21 type, name 2, 70.
    short         type;           // 22 type, name 2, 16.
    void          *speaker_handle;// 23 type, name 11, 3673.
    int           flag;           // 24 type, name 4, 18.
    int           pad2;           // 25 type, name 4, 29.
};

// 525 DNA structure 36, 13
struct AnimData
{
    bAction       *action;       // 1 type, name 199, 1054.
    bAction       *tmpact;       // 2 type, name 199, 3682.
    AnimMapper    *remap;        // 3 type, name 592, 3668.
    ListBase      nla_tracks;    // 4 type, name 14, 3683.
    NlaTrack      *act_track;    // 5 type, name 594, 3684.
    NlaStrip      *actstrip;     // 6 type, name 593, 3685.
    ListBase      drivers;       // 7 type, name 14, 3686.
    ListBase      overrides;     // 8 type, name 14, 3687.
    int           flag;          // 9 type, name 4, 18.
    int           recalc;        // 10 type, name 4, 275.
    short         act_blendmode; // 11 type, name 2, 3688.
    short         act_extendmode;// 12 type, name 2, 3689.
    float         act_influence; // 13 type, name 7, 3690.
};

// 526 DNA structure 605, 14
struct BoidState
{
    BoidState    *next;         // 1 type, name 605, 0.
    BoidState    *prev;         // 2 type, name 605, 1.
    ListBase     rules;         // 3 type, name 14, 3702.
    ListBase     conditions;    // 4 type, name 14, 3703.
    ListBase     actions;       // 5 type, name 14, 3704.
    char         name[32];      // 6 type, name 0, 1601.
    int          id;            // 7 type, name 4, 31.
    int          flag;          // 8 type, name 4, 18.
    int          ruleset_type;  // 9 type, name 4, 3705.
    float        rule_fuzziness;// 10 type, name 7, 3706.
    int          signal_id;     // 11 type, name 4, 3694.
    int          channels;      // 12 type, name 4, 2928.
    float        volume;        // 13 type, name 7, 1224.
    float        falloff;       // 14 type, name 7, 826.
};

// 527 DNA structure 621, 10
struct MovieTrackingObject
{
    MovieTrackingObject            *next;         // 1 type, name 621, 0.
    MovieTrackingObject            *prev;         // 2 type, name 621, 1.
    char                           name[64];      // 3 type, name 0, 19.
    int                            flag;          // 4 type, name 4, 18.
    float                          scale;         // 5 type, name 7, 972.
    ListBase                       tracks;        // 6 type, name 14, 3876.
    ListBase                       plane_tracks;  // 7 type, name 14, 3877.
    MovieTrackingReconstruction    reconstruction;// 8 type, name 620, 3878.
    int                            keyframe1;     // 9 type, name 4, 3858.
    int                            keyframe2;     // 10 type, name 4, 3859.
};

// 528 DNA structure 73, 137
struct Material
{
    ID                id;               // 1 type, name 22, 31.
    AnimData          *adt;             // 2 type, name 36, 77.
    short             material_type;    // 3 type, name 2, 448.
    short             flag;             // 4 type, name 2, 18.
    float             r;                // 5 type, name 7, 216.
    float             g;                // 6 type, name 7, 217.
    float             b;                // 7 type, name 7, 218.
    float             specr;            // 8 type, name 7, 449.
    float             specg;            // 9 type, name 7, 450.
    float             specb;            // 10 type, name 7, 451.
    float             mirr;             // 11 type, name 7, 452.
    float             mirg;             // 12 type, name 7, 453.
    float             mirb;             // 13 type, name 7, 454.
    float             ambr;             // 14 type, name 7, 455.
    float             ambb;             // 15 type, name 7, 456.
    float             ambg;             // 16 type, name 7, 457.
    float             amb;              // 17 type, name 7, 458.
    float             emit;             // 18 type, name 7, 459.
    float             ang;              // 19 type, name 7, 460.
    float             spectra;          // 20 type, name 7, 461.
    float             ray_mirror;       // 21 type, name 7, 462.
    float             alpha;            // 22 type, name 7, 463.
    float             ref;              // 23 type, name 7, 464.
    float             spec;             // 24 type, name 7, 465.
    float             zoffs;            // 25 type, name 7, 466.
    float             add;              // 26 type, name 7, 467.
    float             translucency;     // 27 type, name 7, 468.
    VolumeSettings    vol;              // 28 type, name 70, 469.
    GameSettings      game;             // 29 type, name 71, 470.
    float             fresnel_mir;      // 30 type, name 7, 471.
    float             fresnel_mir_i;    // 31 type, name 7, 472.
    float             fresnel_tra;      // 32 type, name 7, 473.
    float             fresnel_tra_i;    // 33 type, name 7, 474.
    float             filter;           // 34 type, name 7, 475.
    float             tx_limit;         // 35 type, name 7, 476.
    float             tx_falloff;       // 36 type, name 7, 477.
    short             ray_depth;        // 37 type, name 2, 478.
    short             ray_depth_tra;    // 38 type, name 2, 479.
    short             har;              // 39 type, name 2, 480.
    char              seed1;            // 40 type, name 0, 481.
    char              seed2;            // 41 type, name 0, 482.
    float             gloss_mir;        // 42 type, name 7, 483.
    float             gloss_tra;        // 43 type, name 7, 484.
    short             samp_gloss_mir;   // 44 type, name 2, 485.
    short             samp_gloss_tra;   // 45 type, name 2, 486.
    float             adapt_thresh_mir; // 46 type, name 7, 487.
    float             adapt_thresh_tra; // 47 type, name 7, 488.
    float             aniso_gloss_mir;  // 48 type, name 7, 489.
    float             dist_mir;         // 49 type, name 7, 490.
    short             fadeto_mir;       // 50 type, name 2, 491.
    short             shade_flag;       // 51 type, name 2, 492.
    int               mode;             // 52 type, name 4, 368.
    int               mode_l;           // 53 type, name 4, 493.
    int               mode2;            // 54 type, name 4, 494.
    int               mode2_l;          // 55 type, name 4, 495.
    short             flarec;           // 56 type, name 2, 496.
    short             starc;            // 57 type, name 2, 497.
    short             linec;            // 58 type, name 2, 498.
    short             ringc;            // 59 type, name 2, 499.
    float             hasize;           // 60 type, name 7, 500.
    float             flaresize;        // 61 type, name 7, 501.
    float             subsize;          // 62 type, name 7, 502.
    float             flareboost;       // 63 type, name 7, 503.
    float             strand_sta;       // 64 type, name 7, 504.
    float             strand_end;       // 65 type, name 7, 505.
    float             strand_ease;      // 66 type, name 7, 506.
    float             strand_surfnor;   // 67 type, name 7, 507.
    float             strand_min;       // 68 type, name 7, 508.
    float             strand_widthfade; // 69 type, name 7, 509.
    char              strand_uvname[64];// 70 type, name 0, 510.
    float             sbias;            // 71 type, name 7, 511.
    float             lbias;            // 72 type, name 7, 512.
    float             shad_alpha;       // 73 type, name 7, 513.
    int               septex;           // 74 type, name 4, 514.
    char              rgbsel;           // 75 type, name 0, 515.
    char              texact;           // 76 type, name 0, 404.
    char              pr_type;          // 77 type, name 0, 516.
    char              use_nodes;        // 78 type, name 0, 358.
    short             pr_lamp;          // 79 type, name 2, 517.
    short             pr_texture;       // 80 type, name 2, 424.
    short             ml_flag;          // 81 type, name 2, 518.
    char              mapflag;          // 82 type, name 0, 519.
    char              pad;              // 83 type, name 0, 68.
    short             diff_shader;      // 84 type, name 2, 520.
    short             spec_shader;      // 85 type, name 2, 521.
    float             roughness;        // 86 type, name 7, 522.
    float             refrac;           // 87 type, name 7, 523.
    float             param[4];         // 88 type, name 7, 524.
    float             rms;              // 89 type, name 7, 525.
    float             darkness;         // 90 type, name 7, 526.
    short             texco;            // 91 type, name 2, 192.
    short             mapto;            // 92 type, name 2, 193.
    ColorBand         *ramp_col;        // 93 type, name 59, 527.
    ColorBand         *ramp_spec;       // 94 type, name 59, 528.
    char              rampin_col;       // 95 type, name 0, 529.
    char              rampin_spec;      // 96 type, name 0, 530.
    char              rampblend_col;    // 97 type, name 0, 531.
    char              rampblend_spec;   // 98 type, name 0, 532.
    short             ramp_show;        // 99 type, name 2, 533.
    short             pad3;             // 100 type, name 2, 534.
    float             rampfac_col;      // 101 type, name 7, 535.
    float             rampfac_spec;     // 102 type, name 7, 536.
    MTex              *mtex[18];        // 103 type, name 56, 423.
    bNodeTree         *nodetree;        // 104 type, name 66, 353.
    Ipo               *ipo;             // 105 type, name 33, 82.
    Group             *group;           // 106 type, name 74, 537.
    PreviewImage      *preview;         // 107 type, name 26, 172.
    float             friction;         // 108 type, name 7, 538.
    float             fh;               // 109 type, name 7, 539.
    float             reflect;          // 110 type, name 7, 540.
    float             fhdist;           // 111 type, name 7, 541.
    float             xyfrict;          // 112 type, name 7, 542.
    short             dynamode;         // 113 type, name 2, 543.
    short             pad2;             // 114 type, name 2, 29.
    float             sss_radius[3];    // 115 type, name 7, 544.
    float             sss_col[3];       // 116 type, name 7, 545.
    float             sss_error;        // 117 type, name 7, 546.
    float             sss_scale;        // 118 type, name 7, 547.
    float             sss_ior;          // 119 type, name 7, 548.
    float             sss_colfac;       // 120 type, name 7, 549.
    float             sss_texfac;       // 121 type, name 7, 550.
    float             sss_front;        // 122 type, name 7, 551.
    float             sss_back;         // 123 type, name 7, 552.
    short             sss_flag;         // 124 type, name 2, 553.
    short             sss_preset;       // 125 type, name 2, 554.
    int               mapto_textured;   // 126 type, name 4, 555.
    short             shadowonly_flag;  // 127 type, name 2, 556.
    short             index;            // 128 type, name 2, 447.
    float             line_col[4];      // 129 type, name 7, 557.
    short             line_priority;    // 130 type, name 2, 558.
    short             vcol_alpha;       // 131 type, name 2, 559.
    short             paint_active_slot;// 132 type, name 2, 560.
    short             paint_clone_slot; // 133 type, name 2, 561.
    short             tot_slots;        // 134 type, name 2, 562.
    short             pad4[3];          // 135 type, name 2, 563.
    TexPaintSlot      *texpaintslot;    // 136 type, name 72, 564.
    ListBase          gpumaterial;      // 137 type, name 14, 565.
};

// 529 DNA structure 235, 13
struct BakeData
{
    ImageFormatData    im_format;        // 1 type, name 232, 1488.
    char               filepath[1024];   // 2 type, name 0, 35.
    short              width;            // 3 type, name 2, 636.
    short              height;           // 4 type, name 2, 822.
    short              margin;           // 5 type, name 2, 1101.
    short              flag;             // 6 type, name 2, 18.
    float              cage_extrusion;   // 7 type, name 7, 1489.
    float              pad2;             // 8 type, name 7, 29.
    char               normal_swizzle[3];// 9 type, name 0, 1490.
    char               normal_space;     // 10 type, name 0, 1491.
    char               save_mode;        // 11 type, name 0, 1492.
    char               pad[3];           // 12 type, name 0, 44.
    char               cage[64];         // 13 type, name 0, 1493.
};

// 530 DNA structure 316, 18
struct uiStyle
{
    uiStyle        *next;         // 1 type, name 316, 0.
    uiStyle        *prev;         // 2 type, name 316, 1.
    char           name[64];      // 3 type, name 0, 19.
    uiFontStyle    paneltitle;    // 4 type, name 315, 2109.
    uiFontStyle    grouplabel;    // 5 type, name 315, 2110.
    uiFontStyle    widgetlabel;   // 6 type, name 315, 2111.
    uiFontStyle    widget;        // 7 type, name 315, 2112.
    float          panelzoom;     // 8 type, name 7, 2113.
    short          minlabelchars; // 9 type, name 2, 2114.
    short          minwidgetchars;// 10 type, name 2, 2115.
    short          columnspace;   // 11 type, name 2, 2116.
    short          templatespace; // 12 type, name 2, 2117.
    short          boxspace;      // 13 type, name 2, 2118.
    short          buttonspacex;  // 14 type, name 2, 2119.
    short          buttonspacey;  // 15 type, name 2, 2120.
    short          panelspace;    // 16 type, name 2, 2121.
    short          panelouter;    // 17 type, name 2, 2122.
    short          pad;           // 18 type, name 2, 68.
};

// 531 DNA structure 482, 6
struct NodeColorCorrection
{
    ColorCorrectionData    master;       // 1 type, name 481, 3110.
    ColorCorrectionData    shadows;      // 2 type, name 481, 3111.
    ColorCorrectionData    midtones;     // 3 type, name 481, 3112.
    ColorCorrectionData    highlights;   // 4 type, name 481, 3113.
    float                  startmidtones;// 5 type, name 7, 3114.
    float                  endmidtones;  // 6 type, name 7, 3115.
};

// 532 DNA structure 491, 4
struct NodeImageFile
{
    char               name[1024];// 1 type, name 0, 34.
    ImageFormatData    im_format; // 2 type, name 232, 1488.
    int                sfra;      // 3 type, name 4, 144.
    int                efra;      // 4 type, name 4, 1272.
};

// 533 DNA structure 492, 6
struct NodeImageMultiFile
{
    char               base_path[1024];// 1 type, name 0, 3137.
    ImageFormatData    format;         // 2 type, name 232, 3138.
    int                sfra;           // 3 type, name 4, 144.
    int                efra;           // 4 type, name 4, 1272.
    int                active_input;   // 5 type, name 4, 3139.
    int                pad;            // 6 type, name 4, 68.
};

// 534 DNA structure 493, 7
struct NodeImageMultiFileSocket
{
    short              use_render_format;// 1 type, name 2, 3140.
    short              use_node_format;  // 2 type, name 2, 3141.
    int                pad1;             // 3 type, name 4, 70.
    char               path[1024];       // 4 type, name 0, 1217.
    ImageFormatData    format;           // 5 type, name 232, 3138.
    char               layer[30];        // 6 type, name 0, 3142.
    char               pad2[2];          // 7 type, name 0, 673.
};

// 535 DNA structure 508, 2
struct NodeTexBase
{
    TexMapping      tex_mapping;  // 1 type, name 67, 3186.
    ColorMapping    color_mapping;// 2 type, name 68, 3187.
};

// 536 DNA structure 244, 57
struct Brush
{
    ID              id;                       // 1 type, name 22, 31.
    BrushClone      clone;                    // 2 type, name 535, 3275.
    CurveMapping    *curve;                   // 3 type, name 63, 3233.
    MTex            mtex;                     // 4 type, name 56, 3276.
    MTex            mask_mtex;                // 5 type, name 56, 3277.
    Brush           *toggle_brush;            // 6 type, name 244, 3278.
    void            *icon_imbuf;              // Note: using void* on undefined DNA type: ImBuf
    PreviewImage    *preview;                 // 8 type, name 26, 172.
    ColorBand       *gradient;                // 9 type, name 59, 3280.
    PaintCurve      *paint_curve;             // 10 type, name 536, 3281.
    char            icon_filepath[1024];      // 11 type, name 0, 3282.
    float           normal_weight;            // 12 type, name 7, 3283.
    short           blend;                    // 13 type, name 2, 1844.
    short           ob_mode;                  // 14 type, name 2, 3284.
    float           weight;                   // 15 type, name 7, 591.
    int             size;                     // 16 type, name 4, 103.
    int             flag;                     // 17 type, name 4, 18.
    int             mask_pressure;            // 18 type, name 4, 3285.
    float           jitter;                   // 19 type, name 7, 3286.
    int             jitter_absolute;          // 20 type, name 4, 3287.
    int             overlay_flags;            // 21 type, name 4, 3288.
    int             spacing;                  // 22 type, name 4, 644.
    int             smooth_stroke_radius;     // 23 type, name 4, 3289.
    float           smooth_stroke_factor;     // 24 type, name 7, 3290.
    float           rate;                     // 25 type, name 7, 3291.
    float           rgb[3];                   // 26 type, name 7, 1683.
    float           alpha;                    // 27 type, name 7, 463.
    float           secondary_rgb[3];         // 28 type, name 7, 1684.
    int             sculpt_plane;             // 29 type, name 4, 3292.
    float           plane_offset;             // 30 type, name 7, 3293.
    int             flag2;                    // 31 type, name 4, 583.
    int             gradient_spacing;         // 32 type, name 4, 3294.
    int             gradient_stroke_mode;     // 33 type, name 4, 3295.
    int             gradient_fill_mode;       // 34 type, name 4, 3296.
    char            sculpt_tool;              // 35 type, name 0, 3297.
    char            vertexpaint_tool;         // 36 type, name 0, 3298.
    char            imagepaint_tool;          // 37 type, name 0, 3299.
    char            mask_tool;                // 38 type, name 0, 3300.
    float           autosmooth_factor;        // 39 type, name 7, 3301.
    float           crease_pinch_factor;      // 40 type, name 7, 3302.
    float           plane_trim;               // 41 type, name 7, 3303.
    float           height;                   // 42 type, name 7, 822.
    float           texture_sample_bias;      // 43 type, name 7, 3304.
    int             texture_overlay_alpha;    // 44 type, name 4, 3305.
    int             mask_overlay_alpha;       // 45 type, name 4, 3306.
    int             cursor_overlay_alpha;     // 46 type, name 4, 3307.
    float           unprojected_radius;       // 47 type, name 7, 1682.
    float           sharp_threshold;          // 48 type, name 7, 3308.
    int             blur_kernel_radius;       // 49 type, name 4, 3309.
    int             blur_mode;                // 50 type, name 4, 3310.
    float           fill_threshold;           // 51 type, name 7, 3311.
    float           add_col[3];               // 52 type, name 7, 3312.
    float           sub_col[3];               // 53 type, name 7, 3313.
    float           stencil_pos[2];           // 54 type, name 7, 3314.
    float           stencil_dimension[2];     // 55 type, name 7, 3315.
    float           mask_stencil_pos[2];      // 56 type, name 7, 3316.
    float           mask_stencil_dimension[2];// 57 type, name 7, 3317.
};

// 537 DNA structure 162, 51
struct ParticleSystem
{
    ParticleSystem        *next;               // 1 type, name 162, 0.
    ParticleSystem        *prev;               // 2 type, name 162, 1.
    ParticleSettings      *part;               // 3 type, name 551, 3470.
    ParticleData          *particles;          // 4 type, name 549, 3471.
    ChildParticle         *child;              // 5 type, name 546, 2891.
    void                  *edit;               // Note: using void* on undefined DNA type: PTCacheEdit
    void                  (*free_edit)();      // 7 type, name 11, 1221.
    void                  **pathcache;         // Note: using void* on undefined DNA type: ParticleCacheKey
    void                  **childcache;        // Note: using void* on undefined DNA type: ParticleCacheKey
    ListBase              pathcachebufs;       // 10 type, name 14, 3474.
    ListBase              childcachebufs;      // 11 type, name 14, 3475.
    ClothModifierData     *clmd;               // 12 type, name 145, 3476.
    void                  *hair_in_dm;         // Note: using void* on undefined DNA type: DerivedMesh
    void                  *hair_out_dm;        // Note: using void* on undefined DNA type: DerivedMesh
    Object                *target_ob;          // 15 type, name 29, 3479.
    void                  *lattice_deform_data;// Note: using void* on undefined DNA type: LatticeDeformData
    Object                *parent;             // 17 type, name 29, 36.
    ListBase              targets;             // 18 type, name 14, 2954.
    char                  name[64];            // 19 type, name 0, 19.
    float                 imat[4][4];          // 20 type, name 7, 268.
    float                 cfra;                // 21 type, name 7, 863.
    float                 tree_frame;          // 22 type, name 7, 3481.
    float                 bvhtree_frame;       // 23 type, name 7, 3482.
    int                   seed;                // 24 type, name 4, 773.
    int                   child_seed;          // 25 type, name 4, 3483.
    int                   flag;                // 26 type, name 4, 18.
    int                   totpart;             // 27 type, name 4, 2687.
    int                   totunexist;          // 28 type, name 4, 3484.
    int                   totchild;            // 29 type, name 4, 3485.
    int                   totcached;           // 30 type, name 4, 3486.
    int                   totchildcache;       // 31 type, name 4, 3487.
    short                 recalc;              // 32 type, name 2, 275.
    short                 target_psys;         // 33 type, name 2, 3488.
    short                 totkeyed;            // 34 type, name 2, 3489.
    short                 bakespace;           // 35 type, name 2, 3490.
    char                  bb_uvname[3][64];    // 36 type, name 0, 3491.
    short                 vgroup[12];          // 37 type, name 2, 3492.
    short                 vg_neg;              // 38 type, name 2, 3493.
    short                 rt3;                 // 39 type, name 2, 3494.
    void                  *renderdata;         // Note: using void* on undefined DNA type: ParticleRenderData
    PointCache            *pointcache;         // 41 type, name 149, 1293.
    ListBase              ptcaches;            // 42 type, name 14, 842.
    ListBase              *effectors;          // 43 type, name 14, 3496.
    ParticleSpring        *fluid_springs;      // 44 type, name 545, 3497.
    int                   tot_fluidsprings;    // 45 type, name 4, 3498.
    int                   alloc_fluidsprings;  // 46 type, name 4, 3499.
    void                  *tree;               // Note: using void* on undefined DNA type: KDTree
    void                  *bvhtree;            // Note: using void* on undefined DNA type: BVHTree
    void                  *pdd;                // Note: using void* on undefined DNA type: ParticleDrawData
    float                 dt_frac;             // 50 type, name 7, 3502.
    float                 _pad;                // 51 type, name 7, 306.
};

// 538 DNA structure 277, 26
struct View2D
{
    rctf                 tot;          // 1 type, name 19, 260.
    rctf                 cur;          // 2 type, name 19, 65.
    rcti                 vert;         // 3 type, name 18, 1926.
    rcti                 hor;          // 4 type, name 18, 1927.
    rcti                 mask;         // 5 type, name 18, 1928.
    float                min[2];       // 6 type, name 7, 1929.
    float                max[2];       // 7 type, name 7, 1930.
    float                minzoom;      // 8 type, name 7, 1931.
    float                maxzoom;      // 9 type, name 7, 1932.
    short                scroll;       // 10 type, name 2, 1933.
    short                scroll_ui;    // 11 type, name 2, 1934.
    short                keeptot;      // 12 type, name 2, 1935.
    short                keepzoom;     // 13 type, name 2, 1936.
    short                keepofs;      // 14 type, name 2, 1937.
    short                flag;         // 15 type, name 2, 18.
    short                align;        // 16 type, name 2, 1938.
    short                winx;         // 17 type, name 2, 1939.
    short                winy;         // 18 type, name 2, 1940.
    short                oldwinx;      // 19 type, name 2, 1941.
    short                oldwiny;      // 20 type, name 2, 1942.
    short                around;       // 21 type, name 2, 1900.
    float                *tab_offset;  // 22 type, name 7, 1943.
    int                  tab_num;      // 23 type, name 4, 1944.
    int                  tab_cur;      // 24 type, name 4, 1945.
    void                 *sms;         // Note: using void* on undefined DNA type: SmoothView2DStore
    void                 *smooth_timer;// Note: using void* on undefined DNA type: wmTimer
};

// 539 DNA structure 312, 29
struct SpaceClip
{
    SpaceLink          *next;           // 1 type, name 276, 0.
    SpaceLink          *prev;           // 2 type, name 276, 1.
    ListBase           regionbase;      // 3 type, name 14, 1884.
    int                spacetype;       // 4 type, name 4, 1885.
    float              xof;             // 5 type, name 7, 651.
    float              yof;             // 6 type, name 7, 652.
    float              xlockof;         // 7 type, name 7, 2087.
    float              ylockof;         // 8 type, name 7, 2088.
    float              zoom;            // 9 type, name 7, 1978.
    MovieClipUser      user;            // 10 type, name 268, 2089.
    MovieClip          *clip;           // 11 type, name 265, 1839.
    MovieClipScopes    scopes;          // 12 type, name 313, 1981.
    int                flag;            // 13 type, name 4, 18.
    short              mode;            // 14 type, name 2, 368.
    short              view;            // 15 type, name 2, 151.
    int                path_length;     // 16 type, name 4, 2090.
    float              loc[2];          // 17 type, name 7, 2091.
    float              scale;           // 18 type, name 7, 972.
    float              angle;           // 19 type, name 7, 814.
    int                pad;             // 20 type, name 4, 68.
    float              stabmat[4][4];   // 21 type, name 7, 2092.
    float              unistabmat[4][4];// 22 type, name 7, 2093.
    int                postproc_flag;   // 23 type, name 4, 2094.
    short              gpencil_src;     // 24 type, name 2, 1730.
    short              pad2;            // 25 type, name 2, 29.
    int                around;          // 26 type, name 4, 1900.
    int                pad4;            // 27 type, name 4, 1532.
    float              cursor[2];       // 28 type, name 7, 2020.
    MaskSpaceInfo      mask_info;       // 29 type, name 291, 2029.
};

// 540 DNA structure 330, 23
struct bScreen
{
    ID          id;                 // 1 type, name 22, 31.
    ListBase    vertbase;           // 2 type, name 14, 2497.
    ListBase    edgebase;           // 3 type, name 14, 2498.
    ListBase    areabase;           // 4 type, name 14, 2499.
    ListBase    regionbase;         // 5 type, name 14, 1884.
    Scene       *scene;             // 6 type, name 45, 140.
    Scene       *newscene;          // 7 type, name 45, 2500.
    short       winid;              // 8 type, name 2, 2501.
    short       redraws_flag;       // 9 type, name 2, 2502.
    char        temp;               // 10 type, name 0, 2503.
    char        state;              // 11 type, name 0, 1143.
    char        do_draw;            // 12 type, name 0, 2504.
    char        do_refresh;         // 13 type, name 0, 2505.
    char        do_draw_gesture;    // 14 type, name 0, 2506.
    char        do_draw_paintcursor;// 15 type, name 0, 2507.
    char        do_draw_drag;       // 16 type, name 0, 2508.
    char        swap;               // 17 type, name 0, 2509.
    char        skip_handling;      // 18 type, name 0, 2510.
    char        pad[7];             // 19 type, name 0, 119.
    short       mainwin;            // 20 type, name 2, 2511.
    short       subwinactive;       // 21 type, name 2, 2512.
    void        *animtimer;         // Note: using void* on undefined DNA type: wmTimer
    void        *context;           // 23 type, name 11, 2514.
};

// 541 DNA structure 341, 23
struct ScrArea
{
    ScrArea      *next;            // 1 type, name 341, 0.
    ScrArea      *prev;            // 2 type, name 341, 1.
    ScrVert      *v1;              // 3 type, name 331, 2517.
    ScrVert      *v2;              // 4 type, name 331, 2518.
    ScrVert      *v3;              // 5 type, name 331, 2547.
    ScrVert      *v4;              // 6 type, name 331, 2548.
    bScreen      *full;            // 7 type, name 330, 2549.
    rcti         totrct;           // 8 type, name 18, 53.
    char         spacetype;        // 9 type, name 0, 1885.
    char         butspacetype;     // 10 type, name 0, 2550.
    short        winx;             // 11 type, name 2, 1939.
    short        winy;             // 12 type, name 2, 1940.
    short        headertype;       // 13 type, name 2, 2551.
    short        do_refresh;       // 14 type, name 2, 2505.
    short        flag;             // 15 type, name 2, 18.
    short        region_active_win;// 16 type, name 2, 2552.
    char         temp;             // 17 type, name 0, 2503.
    char         pad;              // 18 type, name 0, 68.
    void         *type;            // Note: using void* on undefined DNA type: SpaceType
    ListBase     spacedata;        // 20 type, name 14, 2553.
    ListBase     regionbase;       // 21 type, name 14, 1884.
    ListBase     handlers;         // 22 type, name 14, 2554.
    ListBase     actionzones;      // 23 type, name 14, 2555.
};

// 542 DNA structure 364, 2
struct CurvesModifierData
{
    SequenceModifierData    modifier;     // 1 type, name 362, 757.
    CurveMapping            curve_mapping;// 2 type, name 63, 2676.
};

// 543 DNA structure 365, 2
struct HueCorrectModifierData
{
    SequenceModifierData    modifier;     // 1 type, name 362, 757.
    CurveMapping            curve_mapping;// 2 type, name 63, 2676.
};

// 544 DNA structure 199, 9
struct bAction
{
    ID          id;           // 1 type, name 22, 31.
    ListBase    curves;       // 2 type, name 14, 2931.
    ListBase    chanbase;     // 3 type, name 14, 2908.
    ListBase    groups;       // 4 type, name 14, 2932.
    ListBase    markers;      // 5 type, name 14, 1823.
    int         flag;         // 6 type, name 4, 18.
    int         active_marker;// 7 type, name 4, 2933.
    int         idroot;       // 8 type, name 4, 2934.
    int         pad;          // 9 type, name 4, 68.
};

// 545 DNA structure 509, 5
struct NodeTexSky
{
    NodeTexBase    base;            // 1 type, name 508, 1809.
    int            sky_model;       // 2 type, name 4, 3188.
    float          sun_direction[3];// 3 type, name 7, 3189.
    float          turbidity;       // 4 type, name 7, 3190.
    float          ground_albedo;   // 5 type, name 7, 3191.
};

// 546 DNA structure 511, 1
struct NodeTexChecker
{
    NodeTexBase    base;// 1 type, name 508, 1809.
};

// 547 DNA structure 512, 5
struct NodeTexBrick
{
    NodeTexBase    base;       // 1 type, name 508, 1809.
    int            offset_freq;// 2 type, name 4, 3195.
    int            squash_freq;// 3 type, name 4, 3196.
    float          offset;     // 4 type, name 7, 143.
    float          squash;     // 5 type, name 7, 3197.
};

// 548 DNA structure 514, 3
struct NodeTexGradient
{
    NodeTexBase    base;         // 1 type, name 508, 1809.
    int            gradient_type;// 2 type, name 4, 3198.
    int            pad;          // 3 type, name 4, 68.
};

// 549 DNA structure 515, 1
struct NodeTexNoise
{
    NodeTexBase    base;// 1 type, name 508, 1809.
};

// 550 DNA structure 516, 3
struct NodeTexVoronoi
{
    NodeTexBase    base;    // 1 type, name 508, 1809.
    int            coloring;// 2 type, name 4, 3199.
    int            pad;     // 3 type, name 4, 68.
};

// 551 DNA structure 517, 3
struct NodeTexMusgrave
{
    NodeTexBase    base;         // 1 type, name 508, 1809.
    int            musgrave_type;// 2 type, name 4, 3200.
    int            pad;          // 3 type, name 4, 68.
};

// 552 DNA structure 518, 3
struct NodeTexWave
{
    NodeTexBase    base;     // 1 type, name 508, 1809.
    int            wave_type;// 2 type, name 4, 3201.
    int            pad;      // 3 type, name 4, 68.
};

// 553 DNA structure 519, 3
struct NodeTexMagic
{
    NodeTexBase    base; // 1 type, name 508, 1809.
    int            depth;// 2 type, name 4, 274.
    int            pad;  // 3 type, name 4, 68.
};

// 554 DNA structure 635, 45
struct FreestyleLineStyle
{
    ID           id;                 // 1 type, name 22, 31.
    AnimData     *adt;               // 2 type, name 36, 77.
    float        r;                  // 3 type, name 7, 216.
    float        g;                  // 4 type, name 7, 217.
    float        b;                  // 5 type, name 7, 218.
    float        alpha;              // 6 type, name 7, 463.
    float        thickness;          // 7 type, name 7, 3546.
    int          thickness_position; // 8 type, name 4, 4049.
    float        thickness_ratio;    // 9 type, name 7, 4050.
    int          flag;               // 10 type, name 4, 18.
    int          caps;               // 11 type, name 4, 4051.
    int          chaining;           // 12 type, name 4, 4052.
    int          rounds;             // 13 type, name 4, 4037.
    float        split_length;       // 14 type, name 7, 4053.
    float        min_angle;          // 15 type, name 7, 4054.
    float        max_angle;          // 16 type, name 7, 4055.
    float        min_length;         // 17 type, name 7, 4056.
    float        max_length;         // 18 type, name 7, 4057.
    int          chain_count;        // 19 type, name 4, 4058.
    short        split_dash1;        // 20 type, name 2, 4059.
    short        split_gap1;         // 21 type, name 2, 4060.
    short        split_dash2;        // 22 type, name 2, 4061.
    short        split_gap2;         // 23 type, name 2, 4062.
    short        split_dash3;        // 24 type, name 2, 4063.
    short        split_gap3;         // 25 type, name 2, 4064.
    int          sort_key;           // 26 type, name 4, 4065.
    int          integration_type;   // 27 type, name 4, 4066.
    float        texstep;            // 28 type, name 7, 4067.
    short        texact;             // 29 type, name 2, 404.
    short        pr_texture;         // 30 type, name 2, 424.
    short        use_nodes;          // 31 type, name 2, 358.
    short        pad[3];             // 32 type, name 2, 44.
    short        dash1;              // 33 type, name 2, 4068.
    short        gap1;               // 34 type, name 2, 4069.
    short        dash2;              // 35 type, name 2, 4070.
    short        gap2;               // 36 type, name 2, 4071.
    short        dash3;              // 37 type, name 2, 4072.
    short        gap3;               // 38 type, name 2, 4073.
    int          panel;              // 39 type, name 4, 2165.
    MTex         *mtex[18];          // 40 type, name 56, 423.
    bNodeTree    *nodetree;          // 41 type, name 66, 353.
    ListBase     color_modifiers;    // 42 type, name 14, 4074.
    ListBase     alpha_modifiers;    // 43 type, name 14, 4075.
    ListBase     thickness_modifiers;// 44 type, name 14, 4076.
    ListBase     geometry_modifiers; // 45 type, name 14, 4077.
};

// 555 DNA structure 51, 44
struct Image
{
    ID                                id;                 // 1 type, name 22, 31.
    char                              name[1024];         // 2 type, name 0, 34.
    void                              *cache;             // Note: using void* on undefined DNA type: MovieCache
    void                              *gputexture;        // Note: using void* on undefined DNA type: GPUTexture
    ListBase                          anims;              // 5 type, name 14, 156.
    void                              *rr;                // Note: using void* on undefined DNA type: RenderResult
    void                              *renders[8];        // Note: using void* on undefined DNA type: RenderResult
    short                             render_slot;        // 8 type, name 2, 159.
    short                             last_render_slot;   // 9 type, name 2, 160.
    int                               flag;               // 10 type, name 4, 18.
    short                             source;             // 11 type, name 2, 161.
    short                             type;               // 12 type, name 2, 16.
    int                               lastframe;          // 13 type, name 4, 162.
    short                             tpageflag;          // 14 type, name 2, 163.
    short                             totbind;            // 15 type, name 2, 164.
    short                             xrep;               // 16 type, name 2, 165.
    short                             yrep;               // 17 type, name 2, 166.
    short                             twsta;              // 18 type, name 2, 167.
    short                             twend;              // 19 type, name 2, 168.
    int                               bindcode;           // 20 type, name 4, 169.
    int                               *repbind;           // 21 type, name 4, 170.
    PackedFile                        *packedfile;        // 22 type, name 25, 37.
    ListBase                          packedfiles;        // 23 type, name 14, 171.
    PreviewImage                      *preview;           // 24 type, name 26, 172.
    float                             lastupdate;         // 25 type, name 7, 173.
    int                               lastused;           // 26 type, name 4, 174.
    short                             animspeed;          // 27 type, name 2, 175.
    short                             ok;                 // 28 type, name 2, 147.
    int                               gen_x;              // 29 type, name 4, 176.
    int                               gen_y;              // 30 type, name 4, 177.
    char                              gen_type;           // 31 type, name 0, 178.
    char                              gen_flag;           // 32 type, name 0, 179.
    short                             gen_depth;          // 33 type, name 2, 180.
    float                             gen_color[4];       // 34 type, name 7, 181.
    float                             aspx;               // 35 type, name 7, 182.
    float                             aspy;               // 36 type, name 7, 183.
    ColorManagedColorspaceSettings    colorspace_settings;// 37 type, name 54, 184.
    char                              alpha_mode;         // 38 type, name 0, 185.
    char                              pad[5];             // 39 type, name 0, 186.
    char                              eye;                // 40 type, name 0, 187.
    char                              views_format;       // 41 type, name 0, 188.
    ListBase                          views;              // 42 type, name 14, 189.
    Stereo3dFormat                    *stereo3d_format;   // 43 type, name 55, 190.
    RenderSlot                        render_slots[8];    // 44 type, name 50, 191.
};

// 556 DNA structure 87, 49
struct Mesh
{
    ID             id;          // 1 type, name 22, 31.
    AnimData       *adt;        // 2 type, name 36, 77.
    BoundBox       *bb;         // 3 type, name 78, 567.
    Ipo            *ipo;        // 4 type, name 33, 82.
    Key            *key;        // 5 type, name 35, 629.
    Material       **mat;       // 6 type, name 73, 582.
    MSelect        *mselect;    // 7 type, name 88, 674.
    MPoly          *mpoly;      // 8 type, name 89, 675.
    MTexPoly       *mtpoly;     // 9 type, name 90, 676.
    MLoop          *mloop;      // 10 type, name 91, 677.
    MLoopUV        *mloopuv;    // 11 type, name 92, 678.
    MLoopCol       *mloopcol;   // 12 type, name 93, 679.
    MFace          *mface;      // 13 type, name 94, 680.
    MTFace         *mtface;     // 14 type, name 95, 681.
    TFace          *tface;      // 15 type, name 96, 682.
    MVert          *mvert;      // 16 type, name 97, 683.
    MEdge          *medge;      // 17 type, name 98, 684.
    MDeformVert    *dvert;      // 18 type, name 99, 685.
    MCol           *mcol;       // 19 type, name 100, 686.
    Mesh           *texcomesh;  // 20 type, name 87, 687.
    void           *edit_btmesh;// Note: using void* on undefined DNA type: BMEditMesh
    CustomData     vdata;       // 22 type, name 102, 689.
    CustomData     edata;       // 23 type, name 102, 690.
    CustomData     fdata;       // 24 type, name 102, 691.
    CustomData     pdata;       // 25 type, name 102, 692.
    CustomData     ldata;       // 26 type, name 102, 693.
    int            totvert;     // 27 type, name 4, 55.
    int            totedge;     // 28 type, name 4, 694.
    int            totface;     // 29 type, name 4, 695.
    int            totselect;   // 30 type, name 4, 696.
    int            totpoly;     // 31 type, name 4, 697.
    int            totloop;     // 32 type, name 4, 698.
    int            act_face;    // 33 type, name 4, 699.
    float          loc[3];      // 34 type, name 7, 359.
    float          size[3];     // 35 type, name 7, 207.
    float          rot[3];      // 36 type, name 7, 360.
    int            drawflag;    // 37 type, name 4, 630.
    short          texflag;     // 38 type, name 2, 210.
    short          flag;        // 39 type, name 2, 18.
    float          smoothresh;  // 40 type, name 7, 700.
    int            pad2;        // 41 type, name 4, 29.
    char           cd_flag;     // 42 type, name 0, 701.
    char           pad;         // 43 type, name 0, 68.
    char           subdiv;      // 44 type, name 0, 702.
    char           subdivr;     // 45 type, name 0, 703.
    char           subsurftype; // 46 type, name 0, 704.
    char           editflag;    // 47 type, name 0, 705.
    short          totcol;      // 48 type, name 2, 584.
    Multires       *mr;         // 49 type, name 103, 706.
};

// 557 DNA structure 256, 80
struct ToolSettings
{
    VPaint                  *vpaint;                                // 1 type, name 251, 1716.
    VPaint                  *wpaint;                                // 2 type, name 251, 1717.
    Sculpt                  *sculpt;                                // 3 type, name 249, 1044.
    UvSculpt                *uvsculpt;                              // 4 type, name 250, 1718.
    float                   vgroup_weight;                          // 5 type, name 7, 1719.
    float                   doublimit;                              // 6 type, name 7, 1720.
    float                   normalsize;                             // 7 type, name 7, 1721.
    short                   automerge;                              // 8 type, name 2, 1722.
    short                   selectmode;                             // 9 type, name 2, 1666.
    char                    unwrapper;                              // 10 type, name 0, 1723.
    char                    uvcalc_flag;                            // 11 type, name 0, 1724.
    char                    uv_flag;                                // 12 type, name 0, 1725.
    char                    uv_selectmode;                          // 13 type, name 0, 1726.
    float                   uvcalc_margin;                          // 14 type, name 7, 1727.
    short                   autoik_chainlen;                        // 15 type, name 2, 1728.
    char                    gpencil_flags;                          // 16 type, name 0, 1729.
    char                    gpencil_src;                            // 17 type, name 0, 1730.
    char                    pad[4];                                 // 18 type, name 0, 602.
    ImagePaintSettings      imapaint;                               // 19 type, name 246, 1731.
    ParticleEditSettings    particle;                               // 20 type, name 248, 1732.
    float                   proportional_size;                      // 21 type, name 7, 1733.
    float                   select_thresh;                          // 22 type, name 7, 1734.
    short                   autokey_mode;                           // 23 type, name 2, 1735.
    short                   autokey_flag;                           // 24 type, name 2, 1736.
    char                    multires_subdiv_type;                   // 25 type, name 0, 1737.
    char                    pad3[1];                                // 26 type, name 0, 1738.
    short                   skgen_resolution;                       // 27 type, name 2, 1739.
    float                   skgen_threshold_internal;               // 28 type, name 7, 1740.
    float                   skgen_threshold_external;               // 29 type, name 7, 1741.
    float                   skgen_length_ratio;                     // 30 type, name 7, 1742.
    float                   skgen_length_limit;                     // 31 type, name 7, 1743.
    float                   skgen_angle_limit;                      // 32 type, name 7, 1744.
    float                   skgen_correlation_limit;                // 33 type, name 7, 1745.
    float                   skgen_symmetry_limit;                   // 34 type, name 7, 1746.
    float                   skgen_retarget_angle_weight;            // 35 type, name 7, 1747.
    float                   skgen_retarget_length_weight;           // 36 type, name 7, 1748.
    float                   skgen_retarget_distance_weight;         // 37 type, name 7, 1749.
    short                   skgen_options;                          // 38 type, name 2, 1750.
    char                    skgen_postpro;                          // 39 type, name 0, 1751.
    char                    skgen_postpro_passes;                   // 40 type, name 0, 1752.
    char                    skgen_subdivisions[3];                  // 41 type, name 0, 1753.
    char                    skgen_multi_level;                      // 42 type, name 0, 1754.
    Object                  *skgen_template;                        // 43 type, name 29, 1755.
    char                    bone_sketching;                         // 44 type, name 0, 1756.
    char                    bone_sketching_convert;                 // 45 type, name 0, 1757.
    char                    skgen_subdivision_number;               // 46 type, name 0, 1758.
    char                    skgen_retarget_options;                 // 47 type, name 0, 1759.
    char                    skgen_retarget_roll;                    // 48 type, name 0, 1760.
    char                    skgen_side_string[8];                   // 49 type, name 0, 1761.
    char                    skgen_num_string[8];                    // 50 type, name 0, 1762.
    char                    edge_mode;                              // 51 type, name 0, 1763.
    char                    edge_mode_live_unwrap;                  // 52 type, name 0, 1764.
    char                    snap_mode;                              // 53 type, name 0, 1765.
    char                    snap_node_mode;                         // 54 type, name 0, 1766.
    char                    snap_uv_mode;                           // 55 type, name 0, 1767.
    short                   snap_flag;                              // 56 type, name 2, 1768.
    short                   snap_target;                            // 57 type, name 2, 1769.
    short                   proportional;                           // 58 type, name 2, 1770.
    short                   prop_mode;                              // 59 type, name 2, 1771.
    char                    proportional_objects;                   // 60 type, name 0, 1772.
    char                    proportional_mask;                      // 61 type, name 0, 1773.
    char                    proportional_action;                    // 62 type, name 0, 1774.
    char                    proportional_fcurve;                    // 63 type, name 0, 1775.
    char                    lock_markers;                           // 64 type, name 0, 1776.
    char                    pad4[5];                                // 65 type, name 0, 1777.
    char                    auto_normalize;                         // 66 type, name 0, 1778.
    char                    multipaint;                             // 67 type, name 0, 1779.
    char                    weightuser;                             // 68 type, name 0, 1780.
    char                    vgroupsubset;                           // 69 type, name 0, 1781.
    int                     use_uv_sculpt;                          // 70 type, name 4, 1782.
    int                     uv_sculpt_settings;                     // 71 type, name 4, 1783.
    int                     uv_sculpt_tool;                         // 72 type, name 4, 1784.
    int                     uv_relax_method;                        // 73 type, name 4, 1785.
    short                   sculpt_paint_settings;                  // 74 type, name 2, 1786.
    short                   pad5;                                   // 75 type, name 2, 386.
    int                     sculpt_paint_unified_size;              // 76 type, name 4, 1787.
    float                   sculpt_paint_unified_unprojected_radius;// 77 type, name 7, 1788.
    float                   sculpt_paint_unified_alpha;             // 78 type, name 7, 1789.
    UnifiedPaintSettings    unified_paint_settings;                 // 79 type, name 253, 1790.
    MeshStatVis             statvis;                                // 80 type, name 255, 1791.
};

// 558 DNA structure 299, 27
struct SpaceImage
{
    SpaceLink        *next;           // 1 type, name 276, 0.
    SpaceLink        *prev;           // 2 type, name 276, 1.
    ListBase         regionbase;      // 3 type, name 14, 1884.
    int              spacetype;       // 4 type, name 4, 1885.
    int              flag;            // 5 type, name 4, 18.
    Image            *image;          // 6 type, name 51, 805.
    ImageUser        iuser;           // 7 type, name 44, 352.
    CurveMapping     *cumap;          // 8 type, name 63, 2018.
    Scopes           scopes;          // 9 type, name 300, 1981.
    Histogram        sample_line_hist;// 10 type, name 301, 2019.
    bGPdata          *gpd;            // 11 type, name 201, 1057.
    float            cursor[2];       // 12 type, name 7, 2020.
    float            xof;             // 13 type, name 7, 651.
    float            yof;             // 14 type, name 7, 652.
    float            zoom;            // 15 type, name 7, 1978.
    float            centx;           // 16 type, name 7, 2021.
    float            centy;           // 17 type, name 7, 2022.
    char             mode;            // 18 type, name 0, 368.
    char             pin;             // 19 type, name 0, 2023.
    short            pad;             // 20 type, name 2, 68.
    short            curtile;         // 21 type, name 2, 2024.
    short            lock;            // 22 type, name 2, 2025.
    char             dt_uv;           // 23 type, name 0, 2026.
    char             sticky;          // 24 type, name 0, 2027.
    char             dt_uvstretch;    // 25 type, name 0, 2028.
    char             around;          // 26 type, name 0, 1900.
    MaskSpaceInfo    mask_info;       // 27 type, name 291, 2029.
};

// 559 DNA structure 464, 47
struct bNode
{
    bNode         *next;         // 1 type, name 464, 0.
    bNode         *prev;         // 2 type, name 464, 1.
    bNode         *new_node;     // 3 type, name 464, 3047.
    IDProperty    *prop;         // 4 type, name 21, 2378.
    void          *typeinfo;     // Note: using void* on undefined DNA type: bNodeType
    char          idname[64];    // 6 type, name 0, 2534.
    char          name[64];      // 7 type, name 0, 19.
    int           flag;          // 8 type, name 4, 18.
    short         type;          // 9 type, name 2, 16.
    short         pad;           // 10 type, name 2, 68.
    short         done;          // 11 type, name 2, 2598.
    short         level;         // 12 type, name 2, 734.
    short         lasty;         // 13 type, name 2, 3048.
    short         menunr;        // 14 type, name 2, 2033.
    short         stack_index;   // 15 type, name 2, 3040.
    short         nr;            // 16 type, name 2, 2713.
    float         color[3];      // 17 type, name 7, 112.
    ListBase      inputs;        // 18 type, name 14, 2754.
    ListBase      outputs;       // 19 type, name 14, 3049.
    bNode         *parent;       // 20 type, name 464, 36.
    ID            *id;           // 21 type, name 22, 2068.
    void          *storage;      // 22 type, name 11, 3033.
    bNode         *original;     // 23 type, name 464, 3050.
    ListBase      internal_links;// 24 type, name 14, 3051.
    float         locx;          // 25 type, name 7, 3037.
    float         locy;          // 26 type, name 7, 3038.
    float         width;         // 27 type, name 7, 636.
    float         height;        // 28 type, name 7, 822.
    float         miniwidth;     // 29 type, name 7, 3052.
    float         offsetx;       // 30 type, name 7, 3053.
    float         offsety;       // 31 type, name 7, 3054.
    int           update;        // 32 type, name 4, 3055.
    char          label[64];     // 33 type, name 0, 3056.
    short         custom1;       // 34 type, name 2, 3057.
    short         custom2;       // 35 type, name 2, 3058.
    float         custom3;       // 36 type, name 7, 3059.
    float         custom4;       // 37 type, name 7, 3060.
    short         need_exec;     // 38 type, name 2, 3061.
    short         exec;          // 39 type, name 2, 3062.
    void          *threaddata;   // 40 type, name 11, 3063.
    rctf          totr;          // 41 type, name 19, 3064.
    rctf          butr;          // 42 type, name 19, 3065.
    rctf          prvr;          // 43 type, name 19, 3066.
    short         preview_xsize; // 44 type, name 2, 3067.
    short         preview_ysize; // 45 type, name 2, 3068.
    int           pad2;          // 46 type, name 4, 29.
    void          *block;        // Note: using void* on undefined DNA type: uiBlock
};

// 560 DNA structure 510, 6
struct NodeTexImage
{
    NodeTexBase    base;            // 1 type, name 508, 1809.
    ImageUser      iuser;           // 2 type, name 44, 352.
    int            color_space;     // 3 type, name 4, 3192.
    int            projection;      // 4 type, name 4, 3193.
    float          projection_blend;// 5 type, name 7, 3194.
    int            interpolation;   // 6 type, name 4, 2664.
};

// 561 DNA structure 513, 4
struct NodeTexEnvironment
{
    NodeTexBase    base;       // 1 type, name 508, 1809.
    ImageUser      iuser;      // 2 type, name 44, 352.
    int            color_space;// 3 type, name 4, 3192.
    int            projection; // 4 type, name 4, 3193.
};

// 562 DNA structure 565, 34
struct wmWindow
{
    wmWindow          *next;           // 1 type, name 565, 0.
    wmWindow          *prev;           // 2 type, name 565, 1.
    void              *ghostwin;       // 3 type, name 11, 3586.
    bScreen           *screen;         // 4 type, name 330, 3587.
    bScreen           *newscreen;      // 5 type, name 330, 3588.
    char              screenname[64];  // 6 type, name 0, 3589.
    short             posx;            // 7 type, name 2, 3590.
    short             posy;            // 8 type, name 2, 3591.
    short             sizex;           // 9 type, name 2, 2525.
    short             sizey;           // 10 type, name 2, 2526.
    short             windowstate;     // 11 type, name 2, 3592.
    short             monitor;         // 12 type, name 2, 3593.
    short             active;          // 13 type, name 2, 1155.
    short             cursor;          // 14 type, name 2, 2078.
    short             lastcursor;      // 15 type, name 2, 3594.
    short             modalcursor;     // 16 type, name 2, 3595.
    short             grabcursor;      // 17 type, name 2, 3596.
    short             addmousemove;    // 18 type, name 2, 3597.
    int               winid;           // 19 type, name 4, 2501.
    short             lock_pie_event;  // 20 type, name 2, 3598.
    short             last_pie_event;  // 21 type, name 2, 3599.
    void              *eventstate;     // Note: using void* on undefined DNA type: wmEvent
    void              *curswin;        // Note: using void* on undefined DNA type: wmSubWindow
    void              *tweak;          // Note: using void* on undefined DNA type: wmGesture
    void              *ime_data;       // Note: using void* on undefined DNA type: wmIMEData
    int               drawmethod;      // 26 type, name 4, 3604.
    int               drawfail;        // 27 type, name 4, 3605.
    ListBase          drawdata;        // 28 type, name 14, 3606.
    ListBase          queue;           // 29 type, name 14, 3573.
    ListBase          handlers;        // 30 type, name 14, 2554.
    ListBase          modalhandlers;   // 31 type, name 14, 3607.
    ListBase          subwindows;      // 32 type, name 14, 3608.
    ListBase          gesture;         // 33 type, name 14, 3609.
    Stereo3dFormat    *stereo3d_format;// 34 type, name 55, 190.
};

// 563 DNA structure 275, 62
struct View3D
{
    SpaceLink        *next;                      // 1 type, name 276, 0.
    SpaceLink        *prev;                      // 2 type, name 276, 1.
    ListBase         regionbase;                 // 3 type, name 14, 1884.
    int              spacetype;                  // 4 type, name 4, 1885.
    float            blockscale;                 // 5 type, name 7, 1886.
    short            blockhandler[8];            // 6 type, name 2, 1887.
    float            viewquat[4];                // 7 type, name 7, 1864.
    float            dist;                       // 8 type, name 7, 375.
    float            bundle_size;                // 9 type, name 7, 1888.
    char             bundle_drawtype;            // 10 type, name 0, 1889.
    char             pad[3];                     // 11 type, name 0, 44.
    int              lay_prev;                   // 12 type, name 4, 1890.
    int              lay_used;                   // 13 type, name 4, 1891.
    short            persp;                      // 14 type, name 2, 1870.
    short            view;                       // 15 type, name 2, 151.
    Object           *camera;                    // 16 type, name 29, 1643.
    Object           *ob_centre;                 // 17 type, name 29, 1892.
    rctf             render_border;              // 18 type, name 19, 1893.
    ListBase         bgpicbase;                  // 19 type, name 14, 1894.
    BGpic            *bgpic;                     // 20 type, name 267, 1895.
    View3D           *localvd;                   // 21 type, name 275, 1857.
    char             ob_centre_bone[64];         // 22 type, name 0, 1896.
    int              lay;                        // 23 type, name 4, 1080.
    int              layact;                     // 24 type, name 4, 1816.
    short            drawtype;                   // 25 type, name 2, 1897.
    short            ob_centre_cursor;           // 26 type, name 2, 1898.
    short            scenelock;                  // 27 type, name 2, 1899.
    short            around;                     // 28 type, name 2, 1900.
    short            flag;                       // 29 type, name 2, 18.
    short            flag2;                      // 30 type, name 2, 583.
    float            lens;                       // 31 type, name 7, 128.
    float            grid;                       // 32 type, name 7, 1901.
    float            near;                       // 33 type, name 7, 1902.
    float            far;                        // 34 type, name 7, 1903.
    float            ofs[3];                     // 35 type, name 7, 206.
    float            cursor[3];                  // 36 type, name 7, 1812.
    short            matcap_icon;                // 37 type, name 2, 1904.
    short            gridlines;                  // 38 type, name 2, 1905.
    short            gridsubdiv;                 // 39 type, name 2, 1906.
    char             gridflag;                   // 40 type, name 0, 1907.
    char             twtype;                     // 41 type, name 0, 1908.
    char             twmode;                     // 42 type, name 0, 1909.
    char             twflag;                     // 43 type, name 0, 1910.
    short            flag3;                      // 44 type, name 2, 1911.
    ListBase         afterdraw_transp;           // 45 type, name 14, 1912.
    ListBase         afterdraw_xray;             // 46 type, name 14, 1913.
    ListBase         afterdraw_xraytransp;       // 47 type, name 14, 1914.
    char             zbuf;                       // 48 type, name 0, 1915.
    char             transp;                     // 49 type, name 0, 710.
    char             xray;                       // 50 type, name 0, 1916.
    char             multiview_eye;              // 51 type, name 0, 148.
    char             pad3[4];                    // 52 type, name 0, 1917.
    GPUFXSettings    fx_settings;                // 53 type, name 41, 1918.
    void             *properties_storage;        // 54 type, name 11, 1919.
    Material         *defmaterial;               // 55 type, name 73, 1920.
    bGPdata          *gpd;                       // 56 type, name 201, 1057.
    short            stereo3d_flag;              // 57 type, name 2, 1921.
    char             stereo3d_camera;            // 58 type, name 0, 1922.
    char             pad4;                       // 59 type, name 0, 1532.
    float            stereo3d_convergence_factor;// 60 type, name 7, 1923.
    float            stereo3d_volume_alpha;      // 61 type, name 7, 1924.
    float            stereo3d_convergence_alpha; // 62 type, name 7, 1925.
};

// 564 DNA structure 280, 22
struct SpaceButs
{
    SpaceLink    *next;               // 1 type, name 276, 0.
    SpaceLink    *prev;               // 2 type, name 276, 1.
    ListBase     regionbase;          // 3 type, name 14, 1884.
    int          spacetype;           // 4 type, name 4, 1885.
    float        blockscale;          // 5 type, name 7, 1886.
    short        blockhandler[8];     // 6 type, name 2, 1887.
    View2D       v2d;                 // 7 type, name 277, 1947.
    short        mainb;               // 8 type, name 2, 1948.
    short        mainbo;              // 9 type, name 2, 1949.
    short        mainbuser;           // 10 type, name 2, 1950.
    short        re_align;            // 11 type, name 2, 1951.
    short        align;               // 12 type, name 2, 1938.
    short        preview;             // 13 type, name 2, 1952.
    short        texture_context;     // 14 type, name 2, 1953.
    short        texture_context_prev;// 15 type, name 2, 1954.
    char         flag;                // 16 type, name 0, 18.
    char         pad[7];              // 17 type, name 0, 119.
    void         *path;               // 18 type, name 11, 1955.
    int          pathflag;            // 19 type, name 4, 1956.
    int          dataicon;            // 20 type, name 4, 1957.
    ID           *pinid;              // 21 type, name 22, 1958.
    void         *texuser;            // 22 type, name 11, 1959.
};

// 565 DNA structure 286, 11
struct SpaceNla
{
    SpaceLink     *next;          // 1 type, name 276, 0.
    SpaceLink     *prev;          // 2 type, name 276, 1.
    ListBase      regionbase;     // 3 type, name 14, 1884.
    int           spacetype;      // 4 type, name 4, 1885.
    float         blockscale;     // 5 type, name 7, 1886.
    short         blockhandler[8];// 6 type, name 2, 1887.
    short         autosnap;       // 7 type, name 2, 1970.
    short         flag;           // 8 type, name 2, 18.
    int           pad;            // 9 type, name 4, 68.
    bDopeSheet    *ads;           // 10 type, name 285, 1968.
    View2D        v2d;            // 11 type, name 277, 1947.
};

// 566 DNA structure 66, 36
struct bNodeTree
{
    ID                   id;               // 1 type, name 22, 31.
    AnimData             *adt;             // 2 type, name 36, 77.
    void                 *typeinfo;        // Note: using void* on undefined DNA type: bNodeTreeType
    char                 idname[64];       // 4 type, name 0, 2534.
    void                 *interface_type;  // Note: using void* on undefined DNA type: StructRNA
    bGPdata              *gpd;             // 6 type, name 201, 1057.
    float                view_center[2];   // 7 type, name 7, 2066.
    ListBase             nodes;            // 8 type, name 14, 3080.
    ListBase             links;            // 9 type, name 14, 3081.
    int                  type;             // 10 type, name 4, 16.
    int                  init;             // 11 type, name 4, 3082.
    int                  cur_index;        // 12 type, name 4, 3083.
    int                  flag;             // 13 type, name 4, 18.
    int                  update;           // 14 type, name 4, 3055.
    short                is_updating;      // 15 type, name 2, 3084.
    short                done;             // 16 type, name 2, 2598.
    int                  pad2;             // 17 type, name 4, 29.
    int                  nodetype;         // 18 type, name 4, 3085.
    short                edit_quality;     // 19 type, name 2, 3086.
    short                render_quality;   // 20 type, name 2, 3087.
    int                  chunksize;        // 21 type, name 4, 3088.
    rctf                 viewer_border;    // 22 type, name 19, 3089.
    ListBase             inputs;           // 23 type, name 14, 2754.
    ListBase             outputs;          // 24 type, name 14, 3049.
    void                 *previews;        // Note: using void* on undefined DNA type: bNodeInstanceHash
    bNodeInstanceKey     active_viewer_key;// 26 type, name 306, 3091.
    int                  pad;              // 27 type, name 4, 68.
    void                 *execdata;        // Note: using void* on undefined DNA type: bNodeTreeExec
    void                 (*progress)();    // 29 type, name 11, 3093.
    void                 (*stats_draw)();  // 30 type, name 11, 3094.
    int                  (*test_break)();  // 31 type, name 4, 3095.
    void                 (*update_draw)(); // 32 type, name 11, 3096.
    void                 *tbh;             // 33 type, name 11, 3097.
    void                 *prh;             // 34 type, name 11, 3098.
    void                 *sdh;             // 35 type, name 11, 3099.
    void                 *udh;             // 36 type, name 11, 3100.
};

// 567 DNA structure 284, 14
struct SpaceIpo
{
    SpaceLink     *next;          // 1 type, name 276, 0.
    SpaceLink     *prev;          // 2 type, name 276, 1.
    ListBase      regionbase;     // 3 type, name 14, 1884.
    int           spacetype;      // 4 type, name 4, 1885.
    float         blockscale;     // 5 type, name 7, 1886.
    short         blockhandler[8];// 6 type, name 2, 1887.
    View2D        v2d;            // 7 type, name 277, 1947.
    bDopeSheet    *ads;           // 8 type, name 285, 1968.
    ListBase      ghostCurves;    // 9 type, name 14, 1969.
    short         mode;           // 10 type, name 2, 368.
    short         autosnap;       // 11 type, name 2, 1970.
    int           flag;           // 12 type, name 4, 18.
    float         cursorVal;      // 13 type, name 7, 1971.
    int           around;         // 14 type, name 4, 1900.
};

// 568 DNA structure 288, 9
struct SpaceTime
{
    SpaceLink    *next;        // 1 type, name 276, 0.
    SpaceLink    *prev;        // 2 type, name 276, 1.
    ListBase     regionbase;   // 3 type, name 14, 1884.
    int          spacetype;    // 4 type, name 4, 1885.
    float        blockscale;   // 5 type, name 7, 1886.
    View2D       v2d;          // 6 type, name 277, 1947.
    ListBase     caches;       // 7 type, name 14, 1973.
    int          cache_display;// 8 type, name 4, 1974.
    int          flag;         // 9 type, name 4, 18.
};

// 569 DNA structure 289, 23
struct SpaceSeq
{
    SpaceLink          *next;          // 1 type, name 276, 0.
    SpaceLink          *prev;          // 2 type, name 276, 1.
    ListBase           regionbase;     // 3 type, name 14, 1884.
    int                spacetype;      // 4 type, name 4, 1885.
    float              blockscale;     // 5 type, name 7, 1886.
    short              blockhandler[8];// 6 type, name 2, 1887.
    View2D             v2d;            // 7 type, name 277, 1947.
    float              xof;            // 8 type, name 7, 651.
    float              yof;            // 9 type, name 7, 652.
    short              mainb;          // 10 type, name 2, 1948.
    short              render_size;    // 11 type, name 2, 1975.
    short              chanshown;      // 12 type, name 2, 1976.
    short              zebra;          // 13 type, name 2, 1977.
    int                flag;           // 14 type, name 4, 18.
    float              zoom;           // 15 type, name 7, 1978.
    int                view;           // 16 type, name 4, 151.
    int                overlay_type;   // 17 type, name 4, 1979.
    int                draw_flag;      // 18 type, name 4, 1980.
    int                pad;            // 19 type, name 4, 68.
    bGPdata            *gpd;           // 20 type, name 201, 1057.
    SequencerScopes    scopes;         // 21 type, name 290, 1981.
    char               multiview_eye;  // 22 type, name 0, 148.
    char               pad2[7];        // 23 type, name 0, 1982.
};

// 570 DNA structure 281, 16
struct SpaceOops
{
    SpaceLink        *next;            // 1 type, name 276, 0.
    SpaceLink        *prev;            // 2 type, name 276, 1.
    ListBase         regionbase;       // 3 type, name 14, 1884.
    int              spacetype;        // 4 type, name 4, 1885.
    float            blockscale;       // 5 type, name 7, 1886.
    short            blockhandler[8];  // 6 type, name 2, 1887.
    View2D           v2d;              // 7 type, name 277, 1947.
    ListBase         tree;             // 8 type, name 14, 1960.
    void             *treestore;       // Note: using void* on undefined DNA type: BLI_mempool
    char             search_string[32];// 10 type, name 0, 1962.
    TreeStoreElem    search_tse;       // 11 type, name 283, 1963.
    short            flag;             // 12 type, name 2, 18.
    short            outlinevis;       // 13 type, name 2, 1964.
    short            storeflag;        // 14 type, name 2, 1965.
    short            search_flags;     // 15 type, name 2, 1966.
    void             *treehash;        // 16 type, name 11, 1967.
};

// 571 DNA structure 307, 29
struct SpaceNode
{
    SpaceLink    *next;          // 1 type, name 276, 0.
    SpaceLink    *prev;          // 2 type, name 276, 1.
    ListBase     regionbase;     // 3 type, name 14, 1884.
    int          spacetype;      // 4 type, name 4, 1885.
    float        blockscale;     // 5 type, name 7, 1886.
    short        blockhandler[8];// 6 type, name 2, 1887.
    View2D       v2d;            // 7 type, name 277, 1947.
    ID           *id;            // 8 type, name 22, 2068.
    ID           *from;          // 9 type, name 22, 83.
    short        flag;           // 10 type, name 2, 18.
    short        pad1;           // 11 type, name 2, 70.
    float        aspect;         // 12 type, name 7, 2069.
    float        pad2;           // 13 type, name 7, 29.
    float        xof;            // 14 type, name 7, 651.
    float        yof;            // 15 type, name 7, 652.
    float        zoom;           // 16 type, name 7, 1978.
    float        cursor[2];      // 17 type, name 7, 2020.
    ListBase     treepath;       // 18 type, name 14, 2070.
    bNodeTree    *nodetree;      // 19 type, name 66, 353.
    bNodeTree    *edittree;      // 20 type, name 66, 2071.
    char         tree_idname[64];// 21 type, name 0, 2072.
    int          treetype;       // 22 type, name 4, 2073.
    int          pad3;           // 23 type, name 4, 534.
    short        texfrom;        // 24 type, name 2, 2074.
    short        shaderfrom;     // 25 type, name 2, 2075.
    short        recalc;         // 26 type, name 2, 275.
    short        pad4;           // 27 type, name 2, 1532.
    ListBase     linkdrag;       // 28 type, name 14, 2076.
    bGPdata      *gpd;           // 29 type, name 201, 1057.
};

// 572 DNA structure 425, 13
struct SpaceAction
{
    SpaceLink     *next;          // 1 type, name 276, 0.
    SpaceLink     *prev;          // 2 type, name 276, 1.
    ListBase      regionbase;     // 3 type, name 14, 1884.
    int           spacetype;      // 4 type, name 4, 1885.
    float         blockscale;     // 5 type, name 7, 1886.
    short         blockhandler[8];// 6 type, name 2, 1887.
    View2D        v2d;            // 7 type, name 277, 1947.
    bAction       *action;        // 8 type, name 199, 1054.
    bDopeSheet    ads;            // 9 type, name 285, 2939.
    char          mode;           // 10 type, name 0, 368.
    char          autosnap;       // 11 type, name 0, 1970.
    short         flag;           // 12 type, name 2, 18.
    float         timeslide;      // 13 type, name 7, 2940.
};

// 573 DNA structure 611, 13
struct MovieTracking
{
    MovieTrackingSettings          settings;        // 1 type, name 618, 3888.
    MovieTrackingCamera            camera;          // 2 type, name 615, 3889.
    ListBase                       tracks;          // 3 type, name 14, 3876.
    ListBase                       plane_tracks;    // 4 type, name 14, 3877.
    MovieTrackingReconstruction    reconstruction;  // 5 type, name 620, 3878.
    MovieTrackingStabilization     stabilization;   // 6 type, name 619, 3890.
    MovieTrackingTrack             *act_track;      // 7 type, name 613, 3684.
    MovieTrackingPlaneTrack        *act_plane_track;// 8 type, name 617, 3891.
    ListBase                       objects;         // 9 type, name 14, 3892.
    int                            objectnr;        // 10 type, name 4, 3893.
    int                            tot_object;      // 11 type, name 4, 3894.
    MovieTrackingStats             *stats;          // 12 type, name 622, 1820.
    MovieTrackingDopesheet         dopesheet;       // 13 type, name 625, 3895.
};

// 574 DNA structure 329, 117
struct UserDef
{
    int               versionfile;                // 1 type, name 4, 2387.
    int               subversionfile;             // 2 type, name 4, 2388.
    int               flag;                       // 3 type, name 4, 18.
    int               dupflag;                    // 4 type, name 4, 2389.
    int               savetime;                   // 5 type, name 4, 2390.
    char              tempdir[768];               // 6 type, name 0, 2391.
    char              fontdir[768];               // 7 type, name 0, 2392.
    char              renderdir[1024];            // 8 type, name 0, 2393.
    char              render_cachedir[768];       // 9 type, name 0, 2394.
    char              textudir[768];              // 10 type, name 0, 2395.
    char              pythondir[768];             // 11 type, name 0, 2396.
    char              sounddir[768];              // 12 type, name 0, 2397.
    char              i18ndir[768];               // 13 type, name 0, 2398.
    char              image_editor[1024];         // 14 type, name 0, 2399.
    char              anim_player[1024];          // 15 type, name 0, 2400.
    int               anim_player_preset;         // 16 type, name 4, 2401.
    short             v2d_min_gridsize;           // 17 type, name 2, 2402.
    short             timecode_style;             // 18 type, name 2, 2403.
    short             versions;                   // 19 type, name 2, 2404.
    short             dbl_click_time;             // 20 type, name 2, 2405.
    short             gameflags;                  // 21 type, name 2, 2406.
    short             wheellinescroll;            // 22 type, name 2, 2407.
    int               uiflag;                     // 23 type, name 4, 2408.
    int               uiflag2;                    // 24 type, name 4, 2409.
    int               language;                   // 25 type, name 4, 2410.
    short             userpref;                   // 26 type, name 2, 2411.
    short             viewzoom;                   // 27 type, name 2, 2412.
    int               mixbufsize;                 // 28 type, name 4, 2413.
    int               audiodevice;                // 29 type, name 4, 2414.
    int               audiorate;                  // 30 type, name 4, 2415.
    int               audioformat;                // 31 type, name 4, 2416.
    int               audiochannels;              // 32 type, name 4, 2417.
    int               scrollback;                 // 33 type, name 4, 2079.
    int               dpi;                        // 34 type, name 4, 2418.
    char              pad2[2];                    // 35 type, name 0, 673.
    short             transopts;                  // 36 type, name 2, 2419.
    short             menuthreshold1;             // 37 type, name 2, 2420.
    short             menuthreshold2;             // 38 type, name 2, 2421.
    ListBase          themes;                     // 39 type, name 14, 2422.
    ListBase          uifonts;                    // 40 type, name 14, 2423.
    ListBase          uistyles;                   // 41 type, name 14, 2424.
    ListBase          keymaps;                    // 42 type, name 14, 2425.
    ListBase          user_keymaps;               // 43 type, name 14, 2426.
    ListBase          addons;                     // 44 type, name 14, 2427.
    ListBase          autoexec_paths;             // 45 type, name 14, 2428.
    char              keyconfigstr[64];           // 46 type, name 0, 2429.
    short             undosteps;                  // 47 type, name 2, 2430.
    short             undomemory;                 // 48 type, name 2, 2431.
    short             gp_manhattendist;           // 49 type, name 2, 2432.
    short             gp_euclideandist;           // 50 type, name 2, 2433.
    short             gp_eraser;                  // 51 type, name 2, 2434.
    short             gp_settings;                // 52 type, name 2, 2435.
    short             tb_leftmouse;               // 53 type, name 2, 2436.
    short             tb_rightmouse;              // 54 type, name 2, 2437.
    SolidLight        light[3];                   // 55 type, name 327, 2438.
    short             tw_hotspot;                 // 56 type, name 2, 2439.
    short             tw_flag;                    // 57 type, name 2, 2440.
    short             tw_handlesize;              // 58 type, name 2, 2441.
    short             tw_size;                    // 59 type, name 2, 2442.
    short             textimeout;                 // 60 type, name 2, 2443.
    short             texcollectrate;             // 61 type, name 2, 2444.
    short             wmdrawmethod;               // 62 type, name 2, 2445.
    short             dragthreshold;              // 63 type, name 2, 2446.
    int               memcachelimit;              // 64 type, name 4, 2447.
    int               prefetchframes;             // 65 type, name 4, 2448.
    short             frameserverport;            // 66 type, name 2, 2449.
    short             pad_rot_angle;              // 67 type, name 2, 2450.
    short             obcenter_dia;               // 68 type, name 2, 2451.
    short             rvisize;                    // 69 type, name 2, 2452.
    short             rvibright;                  // 70 type, name 2, 2453.
    short             recent_files;               // 71 type, name 2, 2454.
    short             smooth_viewtx;              // 72 type, name 2, 2455.
    short             glreslimit;                 // 73 type, name 2, 2456.
    short             curssize;                   // 74 type, name 2, 2457.
    short             color_picker_type;          // 75 type, name 2, 2458.
    char              ipo_new;                    // 76 type, name 0, 2459.
    char              keyhandles_new;             // 77 type, name 0, 2460.
    char              gpu_select_method;          // 78 type, name 0, 2461.
    char              view_frame_type;            // 79 type, name 0, 2462.
    int               view_frame_keyframes;       // 80 type, name 4, 2463.
    float             view_frame_seconds;         // 81 type, name 7, 2464.
    short             scrcastfps;                 // 82 type, name 2, 2465.
    short             scrcastwait;                // 83 type, name 2, 2466.
    short             widget_unit;                // 84 type, name 2, 2467.
    short             anisotropic_filter;         // 85 type, name 2, 2468.
    short             use_16bit_textures;         // 86 type, name 2, 2469.
    short             use_gpu_mipmap;             // 87 type, name 2, 2470.
    float             ndof_sensitivity;           // 88 type, name 7, 2471.
    float             ndof_orbit_sensitivity;     // 89 type, name 7, 2472.
    int               ndof_flag;                  // 90 type, name 4, 2473.
    short             ogl_multisamples;           // 91 type, name 2, 2474.
    short             image_draw_method;          // 92 type, name 2, 2475.
    float             glalphaclip;                // 93 type, name 7, 2476.
    short             autokey_mode;               // 94 type, name 2, 1735.
    short             autokey_flag;               // 95 type, name 2, 1736.
    short             text_render;                // 96 type, name 2, 2477.
    short             pad9;                       // 97 type, name 2, 1578.
    ColorBand         coba_weight;                // 98 type, name 59, 2478.
    float             sculpt_paint_overlay_col[3];// 99 type, name 7, 2479.
    float             gpencil_new_layer_col[4];   // 100 type, name 7, 2480.
    short             tweak_threshold;            // 101 type, name 2, 2481.
    char              navigation_mode;            // 102 type, name 0, 2482.
    char              pad;                        // 103 type, name 0, 68.
    char              author[80];                 // 104 type, name 0, 2483.
    char              font_path_ui[1024];         // 105 type, name 0, 2484.
    int               compute_device_type;        // 106 type, name 4, 2485.
    int               compute_device_id;          // 107 type, name 4, 2486.
    float             fcu_inactive_alpha;         // 108 type, name 7, 2487.
    float             pixelsize;                  // 109 type, name 7, 2488.
    int               virtual_pixel;              // 110 type, name 4, 2489.
    short             pie_interaction_type;       // 111 type, name 2, 2490.
    short             pie_initial_timeout;        // 112 type, name 2, 2491.
    short             pie_animation_timeout;      // 113 type, name 2, 2492.
    short             pie_menu_confirm;           // 114 type, name 2, 2493.
    short             pie_menu_radius;            // 115 type, name 2, 2494.
    short             pie_menu_threshold;         // 116 type, name 2, 2495.
    WalkNavigation    walk_navigation;            // 117 type, name 328, 2496.
};

// 575 DNA structure 564, 21
struct wmWindowManager
{
    ID             id;                 // 1 type, name 22, 31.
    wmWindow       *windrawable;       // 2 type, name 565, 3566.
    wmWindow       *winactive;         // 3 type, name 565, 3567.
    ListBase       windows;            // 4 type, name 14, 3568.
    int            initialized;        // 5 type, name 4, 3569.
    short          file_saved;         // 6 type, name 2, 3570.
    short          op_undo_depth;      // 7 type, name 2, 3571.
    ListBase       operators;          // 8 type, name 14, 3572.
    ListBase       queue;              // 9 type, name 14, 3573.
    ReportList     reports;            // 10 type, name 563, 3574.
    ListBase       jobs;               // 11 type, name 14, 3575.
    ListBase       paintcursors;       // 12 type, name 14, 3576.
    ListBase       drags;              // 13 type, name 14, 3577.
    ListBase       keyconfigs;         // 14 type, name 14, 3578.
    wmKeyConfig    *defaultconf;       // 15 type, name 566, 3579.
    wmKeyConfig    *addonconf;         // 16 type, name 566, 3580.
    wmKeyConfig    *userconf;          // 17 type, name 566, 3581.
    ListBase       timers;             // 18 type, name 14, 3582.
    void           *autosavetimer;     // Note: using void* on undefined DNA type: wmTimer
    char           is_interface_locked;// 20 type, name 0, 3584.
    char           par[7];             // 21 type, name 0, 3585.
};

// 576 DNA structure 265, 19
struct MovieClip
{
    ID                                id;                 // 1 type, name 22, 31.
    AnimData                          *adt;               // 2 type, name 36, 77.
    char                              name[1024];         // 3 type, name 0, 34.
    int                               source;             // 4 type, name 4, 161.
    int                               lastframe;          // 5 type, name 4, 162.
    int                               lastsize[2];        // 6 type, name 4, 3797.
    float                             aspx;               // 7 type, name 7, 182.
    float                             aspy;               // 8 type, name 7, 183.
    void                              *anim;              // Note: using void* on undefined DNA type: anim
    void                              *cache;             // Note: using void* on undefined DNA type: MovieClipCache
    bGPdata                           *gpd;               // 11 type, name 201, 1057.
    MovieTracking                     tracking;           // 12 type, name 611, 3798.
    void                              *tracking_context;  // 13 type, name 11, 3799.
    MovieClipProxy                    proxy;              // 14 type, name 609, 3800.
    int                               flag;               // 15 type, name 4, 18.
    int                               len;                // 16 type, name 4, 22.
    int                               start_frame;        // 17 type, name 4, 2868.
    int                               frame_offset;       // 18 type, name 4, 3801.
    ColorManagedColorspaceSettings    colorspace_settings;// 19 type, name 54, 184.
};

// 577 DNA structure 343, 31
struct ARegion
{
    ARegion        *next;                 // 1 type, name 343, 0.
    ARegion        *prev;                 // 2 type, name 343, 1.
    View2D         v2d;                   // 3 type, name 277, 1947.
    rcti           winrct;                // 4 type, name 18, 2556.
    rcti           drawrct;               // 5 type, name 18, 2557.
    short          winx;                  // 6 type, name 2, 1939.
    short          winy;                  // 7 type, name 2, 1940.
    short          swinid;                // 8 type, name 2, 2558.
    short          regiontype;            // 9 type, name 2, 2559.
    short          alignment;             // 10 type, name 2, 2560.
    short          flag;                  // 11 type, name 2, 18.
    float          fsize;                 // 12 type, name 7, 647.
    short          sizex;                 // 13 type, name 2, 2525.
    short          sizey;                 // 14 type, name 2, 2526.
    short          do_draw;               // 15 type, name 2, 2504.
    short          do_draw_overlay;       // 16 type, name 2, 2561.
    short          swap;                  // 17 type, name 2, 2509.
    short          overlap;               // 18 type, name 2, 2562.
    short          flagfullscreen;        // 19 type, name 2, 2563.
    short          pad;                   // 20 type, name 2, 68.
    void           *type;                 // Note: using void* on undefined DNA type: ARegionType
    ListBase       uiblocks;              // 22 type, name 14, 2564.
    ListBase       panels;                // 23 type, name 14, 2565.
    ListBase       panels_category_active;// 24 type, name 14, 2566.
    ListBase       ui_lists;              // 25 type, name 14, 2567.
    ListBase       ui_previews;           // 26 type, name 14, 2568.
    ListBase       handlers;              // 27 type, name 14, 2554.
    ListBase       panels_category;       // 28 type, name 14, 2569.
    void           *regiontimer;          // Note: using void* on undefined DNA type: wmTimer
    char           *headerstr;            // 30 type, name 0, 2571.
    void           *regiondata;           // 31 type, name 11, 2572.
};

// 578 DNA structure 236, 126
struct RenderData
{
    ImageFormatData           im_format;                // 1 type, name 232, 1488.
    AviCodecData              *avicodecdata;            // 2 type, name 224, 1494.
    QuicktimeCodecData        *qtcodecdata;             // 3 type, name 225, 1495.
    QuicktimeCodecSettings    qtcodecsettings;          // 4 type, name 226, 1496.
    FFMpegCodecData           ffcodecdata;              // 5 type, name 227, 1497.
    int                       cfra;                     // 6 type, name 4, 863.
    int                       sfra;                     // 7 type, name 4, 144.
    int                       efra;                     // 8 type, name 4, 1272.
    float                     subframe;                 // 9 type, name 7, 1498.
    int                       psfra;                    // 10 type, name 4, 1499.
    int                       pefra;                    // 11 type, name 4, 1500.
    int                       images;                   // 12 type, name 4, 1501.
    int                       framapto;                 // 13 type, name 4, 1502.
    short                     flag;                     // 14 type, name 2, 18.
    short                     threads;                  // 15 type, name 2, 1301.
    float                     framelen;                 // 16 type, name 7, 1503.
    float                     blurfac;                  // 17 type, name 7, 1504.
    float                     edgeR;                    // 18 type, name 7, 1505.
    float                     edgeG;                    // 19 type, name 7, 1506.
    float                     edgeB;                    // 20 type, name 7, 1507.
    short                     fullscreen;               // 21 type, name 2, 1508.
    short                     xplay;                    // 22 type, name 2, 1509.
    short                     yplay;                    // 23 type, name 2, 1510.
    short                     freqplay;                 // 24 type, name 2, 1511.
    short                     depth;                    // 25 type, name 2, 274.
    short                     attrib;                   // 26 type, name 2, 1512.
    int                       frame_step;               // 27 type, name 4, 1513.
    short                     stereomode;               // 28 type, name 2, 1514.
    short                     dimensionspreset;         // 29 type, name 2, 1515.
    short                     filtertype;               // 30 type, name 2, 390.
    short                     size;                     // 31 type, name 2, 103.
    short                     maximsize;                // 32 type, name 2, 1516.
    short                     pad6;                     // 33 type, name 2, 1517.
    int                       xsch;                     // 34 type, name 4, 1518.
    int                       ysch;                     // 35 type, name 4, 1519.
    short                     xparts;                   // 36 type, name 2, 1520.
    short                     yparts;                   // 37 type, name 2, 1521.
    int                       tilex;                    // 38 type, name 4, 1522.
    int                       tiley;                    // 39 type, name 4, 1523.
    short                     planes;                   // 40 type, name 2, 1475.
    short                     imtype;                   // 41 type, name 2, 1474.
    short                     subimtype;                // 42 type, name 2, 1524.
    short                     quality;                  // 43 type, name 2, 1476.
    short                     displaymode;              // 44 type, name 2, 1525.
    char                      use_lock_interface;       // 45 type, name 0, 1526.
    char                      pad7;                     // 46 type, name 0, 1527.
    int                       scemode;                  // 47 type, name 4, 1528.
    int                       mode;                     // 48 type, name 4, 368.
    int                       raytrace_options;         // 49 type, name 4, 1529.
    short                     raytrace_structure;       // 50 type, name 2, 1530.
    short                     pad1;                     // 51 type, name 2, 70.
    short                     ocres;                    // 52 type, name 2, 1531.
    short                     pad4;                     // 53 type, name 2, 1532.
    short                     alphamode;                // 54 type, name 2, 1533.
    short                     osa;                      // 55 type, name 2, 1534.
    short                     frs_sec;                  // 56 type, name 2, 1535.
    short                     edgeint;                  // 57 type, name 2, 1536.
    rctf                      safety;                   // 58 type, name 19, 1537.
    rctf                      border;                   // 59 type, name 19, 1538.
    rcti                      disprect;                 // 60 type, name 18, 1539.
    ListBase                  layers;                   // 61 type, name 14, 1540.
    short                     actlay;                   // 62 type, name 2, 1541.
    short                     mblur_samples;            // 63 type, name 2, 1542.
    float                     xasp;                     // 64 type, name 7, 1543.
    float                     yasp;                     // 65 type, name 7, 1544.
    float                     frs_sec_base;             // 66 type, name 7, 1545.
    float                     gauss;                    // 67 type, name 7, 1546.
    int                       color_mgt_flag;           // 68 type, name 4, 1547.
    float                     postgamma;                // 69 type, name 7, 1548.
    float                     posthue;                  // 70 type, name 7, 1549.
    float                     postsat;                  // 71 type, name 7, 1550.
    float                     dither_intensity;         // 72 type, name 7, 1551.
    short                     bake_osa;                 // 73 type, name 2, 1552.
    short                     bake_filter;              // 74 type, name 2, 1553.
    short                     bake_mode;                // 75 type, name 2, 1554.
    short                     bake_flag;                // 76 type, name 2, 1555.
    short                     bake_normal_space;        // 77 type, name 2, 1556.
    short                     bake_quad_split;          // 78 type, name 2, 1557.
    float                     bake_maxdist;             // 79 type, name 7, 1558.
    float                     bake_biasdist;            // 80 type, name 7, 1559.
    short                     bake_samples;             // 81 type, name 2, 1560.
    short                     bake_pad;                 // 82 type, name 2, 1561.
    float                     bake_user_scale;          // 83 type, name 7, 1562.
    float                     bake_pad1;                // 84 type, name 7, 1563.
    char                      pic[1024];                // 85 type, name 0, 1564.
    int                       stamp;                    // 86 type, name 4, 1565.
    short                     stamp_font_id;            // 87 type, name 2, 1566.
    short                     pad3;                     // 88 type, name 2, 534.
    char                      stamp_udata[768];         // 89 type, name 0, 1567.
    float                     fg_stamp[4];              // 90 type, name 7, 1568.
    float                     bg_stamp[4];              // 91 type, name 7, 1569.
    char                      seq_prev_type;            // 92 type, name 0, 1570.
    char                      seq_rend_type;            // 93 type, name 0, 1571.
    char                      seq_flag;                 // 94 type, name 0, 1572.
    char                      pad5[5];                  // 95 type, name 0, 1573.
    int                       simplify_flag;            // 96 type, name 4, 1574.
    short                     simplify_subsurf;         // 97 type, name 2, 1575.
    short                     simplify_subsurf_render;  // 98 type, name 2, 1576.
    short                     simplify_shadowsamples;   // 99 type, name 2, 1577.
    short                     pad9;                     // 100 type, name 2, 1578.
    float                     simplify_particles;       // 101 type, name 7, 1579.
    float                     simplify_particles_render;// 102 type, name 7, 1580.
    float                     simplify_aosss;           // 103 type, name 7, 1581.
    short                     cineonwhite;              // 104 type, name 2, 1582.
    short                     cineonblack;              // 105 type, name 2, 1583.
    float                     cineongamma;              // 106 type, name 7, 1584.
    short                     jp2_preset;               // 107 type, name 2, 1585.
    short                     jp2_depth;                // 108 type, name 2, 1586.
    int                       rpad3;                    // 109 type, name 4, 1587.
    short                     domeres;                  // 110 type, name 2, 1588.
    short                     domemode;                 // 111 type, name 2, 1589.
    short                     domeangle;                // 112 type, name 2, 1590.
    short                     dometilt;                 // 113 type, name 2, 1591.
    float                     domeresbuf;               // 114 type, name 7, 1592.
    float                     pad2;                     // 115 type, name 7, 29.
    Text                      *dometext;                // 116 type, name 38, 1593.
    int                       line_thickness_mode;      // 117 type, name 4, 1594.
    float                     unit_line_thickness;      // 118 type, name 7, 1595.
    char                      engine[32];               // 119 type, name 0, 1596.
    BakeData                  bake;                     // 120 type, name 235, 1597.
    int                       preview_start_resolution; // 121 type, name 4, 1598.
    int                       pad;                      // 122 type, name 4, 68.
    ListBase                  views;                    // 123 type, name 14, 189.
    short                     actview;                  // 124 type, name 2, 1599.
    short                     views_format;             // 125 type, name 2, 188.
    short                     pad8[2];                  // 126 type, name 2, 1600.
};

// 579 DNA structure 29, 134
struct Object
{
    ID                  id;                    // 1 type, name 22, 31.
    AnimData            *adt;                  // 2 type, name 36, 77.
    void                *sculpt;               // Note: using void* on undefined DNA type: SculptSession
    short               type;                  // 4 type, name 2, 16.
    short               partype;               // 5 type, name 2, 1045.
    int                 par1;                  // 6 type, name 4, 1046.
    int                 par2;                  // 7 type, name 4, 1047.
    int                 par3;                  // 8 type, name 4, 1048.
    char                parsubstr[64];         // 9 type, name 0, 1049.
    Object              *parent;               // 10 type, name 29, 36.
    Object              *track;                // 11 type, name 29, 1050.
    Object              *proxy;                // 12 type, name 29, 1051.
    Object              *proxy_group;          // 13 type, name 29, 1052.
    Object              *proxy_from;           // 14 type, name 29, 1053.
    Ipo                 *ipo;                  // 15 type, name 33, 82.
    BoundBox            *bb;                   // 16 type, name 78, 567.
    bAction             *action;               // 17 type, name 199, 1054.
    bAction             *poselib;              // 18 type, name 199, 1055.
    bPose               *pose;                 // 19 type, name 200, 1056.
    void                *data;                 // 20 type, name 11, 2.
    bGPdata             *gpd;                  // 21 type, name 201, 1057.
    bAnimVizSettings    avs;                   // 22 type, name 202, 1058.
    bMotionPath         *mpath;                // 23 type, name 203, 1059.
    ListBase            constraintChannels;    // 24 type, name 14, 1060.
    ListBase            effect;                // 25 type, name 14, 1061.
    ListBase            defbase;               // 26 type, name 14, 1062.
    ListBase            modifiers;             // 27 type, name 14, 1063.
    int                 mode;                  // 28 type, name 4, 368.
    int                 restore_mode;          // 29 type, name 4, 1064.
    Material            **mat;                 // 30 type, name 73, 582.
    char                *matbits;              // 31 type, name 0, 1065.
    int                 totcol;                // 32 type, name 4, 584.
    int                 actcol;                // 33 type, name 4, 1066.
    float               loc[3];                // 34 type, name 7, 359.
    float               dloc[3];               // 35 type, name 7, 1067.
    float               orig[3];               // 36 type, name 7, 1068.
    float               size[3];               // 37 type, name 7, 207.
    float               dsize[3];              // 38 type, name 7, 1069.
    float               dscale[3];             // 39 type, name 7, 1070.
    float               rot[3];                // 40 type, name 7, 360.
    float               drot[3];               // 41 type, name 7, 1071.
    float               quat[4];               // 42 type, name 7, 570.
    float               dquat[4];              // 43 type, name 7, 1072.
    float               rotAxis[3];            // 44 type, name 7, 1073.
    float               drotAxis[3];           // 45 type, name 7, 1074.
    float               rotAngle;              // 46 type, name 7, 1075.
    float               drotAngle;             // 47 type, name 7, 1076.
    float               obmat[4][4];           // 48 type, name 7, 1077.
    float               parentinv[4][4];       // 49 type, name 7, 833.
    float               constinv[4][4];        // 50 type, name 7, 1078.
    float               imat[4][4];            // 51 type, name 7, 268.
    float               imat_ren[4][4];        // 52 type, name 7, 1079.
    int                 lay;                   // 53 type, name 4, 1080.
    short               flag;                  // 54 type, name 2, 18.
    short               colbits;               // 55 type, name 2, 1081.
    short               transflag;             // 56 type, name 2, 1082.
    short               protectflag;           // 57 type, name 2, 1083.
    short               trackflag;             // 58 type, name 2, 1084.
    short               upflag;                // 59 type, name 2, 1085.
    short               nlaflag;               // 60 type, name 2, 1086.
    short               ipoflag;               // 61 type, name 2, 1087.
    short               scaflag;               // 62 type, name 2, 1088.
    char                scavisflag;            // 63 type, name 0, 1089.
    char                depsflag;              // 64 type, name 0, 1090.
    int                 dupon;                 // 65 type, name 4, 1091.
    int                 dupoff;                // 66 type, name 4, 1092.
    int                 dupsta;                // 67 type, name 4, 1093.
    int                 dupend;                // 68 type, name 4, 1094.
    int                 lastNeedMapping;       // 69 type, name 4, 1095.
    float               mass;                  // 70 type, name 7, 1096.
    float               damping;               // 71 type, name 7, 1097.
    float               inertia;               // 72 type, name 7, 1098.
    float               formfactor;            // 73 type, name 7, 1099.
    float               rdamping;              // 74 type, name 7, 1100.
    float               margin;                // 75 type, name 7, 1101.
    float               max_vel;               // 76 type, name 7, 1102.
    float               min_vel;               // 77 type, name 7, 1103.
    float               obstacleRad;           // 78 type, name 7, 1104.
    float               step_height;           // 79 type, name 7, 1105.
    float               jump_speed;            // 80 type, name 7, 1106.
    float               fall_speed;            // 81 type, name 7, 1107.
    short               col_group;             // 82 type, name 2, 1108.
    short               col_mask;              // 83 type, name 2, 1109.
    short               rotmode;               // 84 type, name 2, 1110.
    char                boundtype;             // 85 type, name 0, 1111.
    char                collision_boundtype;   // 86 type, name 0, 1112.
    short               dtx;                   // 87 type, name 2, 124.
    char                dt;                    // 88 type, name 0, 1113.
    char                empty_drawtype;        // 89 type, name 0, 1114.
    float               empty_drawsize;        // 90 type, name 7, 1115.
    float               dupfacesca;            // 91 type, name 7, 1116.
    ListBase            prop;                  // 92 type, name 14, 1117.
    ListBase            sensors;               // 93 type, name 14, 1118.
    ListBase            controllers;           // 94 type, name 14, 1119.
    ListBase            actuators;             // 95 type, name 14, 1120.
    float               sf;                    // 96 type, name 7, 1121.
    short               index;                 // 97 type, name 2, 447.
    short               actdef;                // 98 type, name 2, 1122.
    float               col[4];                // 99 type, name 7, 709.
    int                 gameflag;              // 100 type, name 4, 1123.
    int                 gameflag2;             // 101 type, name 4, 1124.
    BulletSoftBody      *bsoft;                // 102 type, name 204, 1125.
    char                restrictflag;          // 103 type, name 0, 1126.
    char                recalc;                // 104 type, name 0, 275.
    short               softflag;              // 105 type, name 2, 1127.
    float               anisotropicFriction[3];// 106 type, name 7, 1128.
    ListBase            constraints;           // 107 type, name 14, 1129.
    ListBase            nlastrips;             // 108 type, name 14, 1130.
    ListBase            hooks;                 // 109 type, name 14, 1131.
    ListBase            particlesystem;        // 110 type, name 14, 1132.
    PartDeflect         *pd;                   // 111 type, name 205, 355.
    SoftBody            *soft;                 // 112 type, name 206, 1133.
    Group               *dup_group;            // 113 type, name 74, 1134.
    char                body_type;             // 114 type, name 0, 1135.
    char                shapeflag;             // 115 type, name 0, 1136.
    short               shapenr;               // 116 type, name 2, 623.
    float               smoothresh;            // 117 type, name 7, 700.
    FluidsimSettings    *fluidsimSettings;     // 118 type, name 167, 1137.
    void                *curve_cache;          // Note: using void* on undefined DNA type: CurveCache
    void                *derivedDeform;        // Note: using void* on undefined DNA type: DerivedMesh
    void                *derivedFinal;         // Note: using void* on undefined DNA type: DerivedMesh
    uint64_t            lastDataMask;          // 122 type, name 10, 1141.
    uint64_t            customdata_mask;       // 123 type, name 10, 1142.
    int                 state;                 // 124 type, name 4, 1143.
    int                 init_state;            // 125 type, name 4, 1144.
    ListBase            gpulamp;               // 126 type, name 14, 1145.
    ListBase            pc_ids;                // 127 type, name 14, 1146.
    ListBase            *duplilist;            // 128 type, name 14, 1147.
    RigidBodyOb         *rigidbody_object;     // 129 type, name 208, 1148.
    RigidBodyCon        *rigidbody_constraint; // 130 type, name 209, 1149.
    float               ima_ofs[2];            // 131 type, name 7, 1150.
    ImageUser           *iuser;                // 132 type, name 44, 1151.
    ListBase            lodlevels;             // 133 type, name 14, 1152.
    LodLevel            *currentlod;           // 134 type, name 197, 1153.
};

// 580 DNA structure 321, 29
struct ThemeUI
{
    uiWidgetColors         wcol_regular;     // 1 type, name 317, 2146.
    uiWidgetColors         wcol_tool;        // 2 type, name 317, 2147.
    uiWidgetColors         wcol_text;        // 3 type, name 317, 2148.
    uiWidgetColors         wcol_radio;       // 4 type, name 317, 2149.
    uiWidgetColors         wcol_option;      // 5 type, name 317, 2150.
    uiWidgetColors         wcol_toggle;      // 6 type, name 317, 2151.
    uiWidgetColors         wcol_num;         // 7 type, name 317, 2152.
    uiWidgetColors         wcol_numslider;   // 8 type, name 317, 2153.
    uiWidgetColors         wcol_menu;        // 9 type, name 317, 2154.
    uiWidgetColors         wcol_pulldown;    // 10 type, name 317, 2155.
    uiWidgetColors         wcol_menu_back;   // 11 type, name 317, 2156.
    uiWidgetColors         wcol_menu_item;   // 12 type, name 317, 2157.
    uiWidgetColors         wcol_tooltip;     // 13 type, name 317, 2158.
    uiWidgetColors         wcol_box;         // 14 type, name 317, 2159.
    uiWidgetColors         wcol_scroll;      // 15 type, name 317, 2160.
    uiWidgetColors         wcol_progress;    // 16 type, name 317, 2161.
    uiWidgetColors         wcol_list_item;   // 17 type, name 317, 2162.
    uiWidgetColors         wcol_pie_menu;    // 18 type, name 317, 2163.
    uiWidgetStateColors    wcol_state;       // 19 type, name 318, 2164.
    uiPanelColors          panel;            // 20 type, name 319, 2165.
    char                   widget_emboss[4]; // 21 type, name 0, 2166.
    float                  menu_shadow_fac;  // 22 type, name 7, 2167.
    short                  menu_shadow_width;// 23 type, name 2, 2168.
    short                  pad[3];           // 24 type, name 2, 44.
    char                   iconfile[256];    // 25 type, name 0, 2169.
    float                  icon_alpha;       // 26 type, name 7, 2170.
    char                   xaxis[4];         // 27 type, name 0, 2171.
    char                   yaxis[4];         // 28 type, name 0, 2172.
    char                   zaxis[4];         // 29 type, name 0, 2173.
};

// 581 DNA structure 45, 51
struct Scene
{
    ID                                id;                           // 1 type, name 22, 31.
    AnimData                          *adt;                         // 2 type, name 36, 77.
    Object                            *camera;                      // 3 type, name 29, 1643.
    World                             *world;                       // 4 type, name 222, 1807.
    Scene                             *set;                         // 5 type, name 45, 1808.
    ListBase                          base;                         // 6 type, name 14, 1809.
    Base                              *basact;                      // 7 type, name 223, 1810.
    Object                            *obedit;                      // 8 type, name 29, 1811.
    float                             cursor[3];                    // 9 type, name 7, 1812.
    float                             twcent[3];                    // 10 type, name 7, 1813.
    float                             twmin[3];                     // 11 type, name 7, 1814.
    float                             twmax[3];                     // 12 type, name 7, 1815.
    int                               lay;                          // 13 type, name 4, 1080.
    int                               layact;                       // 14 type, name 4, 1816.
    int                               lay_updated;                  // 15 type, name 4, 1817.
    short                             flag;                         // 16 type, name 2, 18.
    char                              use_nodes;                    // 17 type, name 0, 358.
    char                              pad[1];                       // 18 type, name 0, 264.
    bNodeTree                         *nodetree;                    // 19 type, name 66, 353.
    Editing                           *ed;                          // 20 type, name 261, 1818.
    ToolSettings                      *toolsettings;                // 21 type, name 256, 1819.
    void                              *stats;                       // Note: using void* on undefined DNA type: SceneStats
    DisplaySafeAreas                  safe_areas;                   // 23 type, name 260, 1821.
    RenderData                        r;                            // 24 type, name 236, 216.
    AudioData                         audio;                        // 25 type, name 228, 1822.
    ListBase                          markers;                      // 26 type, name 14, 1823.
    ListBase                          transform_spaces;             // 27 type, name 14, 1824.
    void                              *sound_scene;                 // 28 type, name 11, 1825.
    void                              *playback_handle;             // 29 type, name 11, 1826.
    void                              *sound_scrub_handle;          // 30 type, name 11, 1827.
    void                              *speaker_handles;             // 31 type, name 11, 1828.
    void                              *fps_info;                    // 32 type, name 11, 1829.
    void                              *depsgraph;                   // Note: using void* on undefined DNA type: Depsgraph
    void                              *pad1;                        // 34 type, name 11, 1831.
    void                              *theDag;                      // Note: using void* on undefined DNA type: DagForest
    short                             dagflags;                     // 36 type, name 2, 1833.
    short                             recalc;                       // 37 type, name 2, 275.
    int                               active_keyingset;             // 38 type, name 4, 1834.
    ListBase                          keyingsets;                   // 39 type, name 14, 1835.
    GameFraming                       framing;                      // 40 type, name 239, 1623.
    GameData                          gm;                           // 41 type, name 241, 1836.
    UnitSettings                      unit;                         // 42 type, name 258, 1837.
    bGPdata                           *gpd;                         // 43 type, name 201, 1057.
    PhysicsSettings                   physics_settings;             // 44 type, name 259, 1838.
    MovieClip                         *clip;                        // 45 type, name 265, 1839.
    uint64_t                          customdata_mask;              // 46 type, name 10, 1142.
    uint64_t                          customdata_mask_modal;        // 47 type, name 10, 1840.
    ColorManagedViewSettings          view_settings;                // 48 type, name 233, 1486.
    ColorManagedDisplaySettings       display_settings;             // 49 type, name 234, 1487.
    ColorManagedColorspaceSettings    sequencer_colorspace_settings;// 50 type, name 54, 1841.
    RigidBodyWorld                    *rigidbody_world;             // 51 type, name 266, 1842.
};

// 582 DNA structure 324, 24
struct bTheme
{
    bTheme            *next;            // 1 type, name 324, 0.
    bTheme            *prev;            // 2 type, name 324, 1.
    char              name[32];         // 3 type, name 0, 1601.
    ThemeUI           tui;              // 4 type, name 321, 2357.
    ThemeSpace        tbuts;            // 5 type, name 322, 2358.
    ThemeSpace        tv3d;             // 6 type, name 322, 2359.
    ThemeSpace        tfile;            // 7 type, name 322, 2360.
    ThemeSpace        tipo;             // 8 type, name 322, 2361.
    ThemeSpace        tinfo;            // 9 type, name 322, 2362.
    ThemeSpace        tact;             // 10 type, name 322, 2363.
    ThemeSpace        tnla;             // 11 type, name 322, 2364.
    ThemeSpace        tseq;             // 12 type, name 322, 2365.
    ThemeSpace        tima;             // 13 type, name 322, 2366.
    ThemeSpace        text;             // 14 type, name 322, 2367.
    ThemeSpace        toops;            // 15 type, name 322, 2368.
    ThemeSpace        ttime;            // 16 type, name 322, 2369.
    ThemeSpace        tnode;            // 17 type, name 322, 2370.
    ThemeSpace        tlogic;           // 18 type, name 322, 2371.
    ThemeSpace        tuserpref;        // 19 type, name 322, 2372.
    ThemeSpace        tconsole;         // 20 type, name 322, 2373.
    ThemeSpace        tclip;            // 21 type, name 322, 2374.
    ThemeWireColor    tarm[20];         // 22 type, name 323, 2375.
    int               active_theme_area;// 23 type, name 4, 2376.
    int               pad;              // 24 type, name 4, 68.
};

/** @}*/
}
#endif//_Blender_h_
