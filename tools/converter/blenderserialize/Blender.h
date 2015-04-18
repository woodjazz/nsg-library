#ifndef _Blender_h_
#define _Blender_h_
// Generated from a Blender(274) file.

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
struct Camera;
struct ImageUser;
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



// zu DNA structure 0, 12
struct Link
{
    Link    *next;// 1 type, name 12, 0.
    Link    *prev;// 2 type, name 12, 1.
};

// zu DNA structure 1, 13
struct LinkData
{
    LinkData    *next;// 1 type, name 13, 0.
    LinkData    *prev;// 2 type, name 13, 1.
    void        *data;// 3 type, name 11, 2.
};

// zu DNA structure 2, 14
struct ListBase
{
    void    *first;// 1 type, name 11, 3.
    void    *last; // 2 type, name 11, 4.
};

// zu DNA structure 3, 15
struct vec2s
{
    short    x;// 1 type, name 2, 5.
    short    y;// 2 type, name 2, 6.
};

// zu DNA structure 4, 16
struct vec2f
{
    float    x;// 1 type, name 7, 5.
    float    y;// 2 type, name 7, 6.
};

// zu DNA structure 5, 17
struct vec3f
{
    float    x;// 1 type, name 7, 5.
    float    y;// 2 type, name 7, 6.
    float    z;// 3 type, name 7, 7.
};

// zu DNA structure 6, 18
struct rcti
{
    int    xmin;// 1 type, name 4, 8.
    int    xmax;// 2 type, name 4, 9.
    int    ymin;// 3 type, name 4, 10.
    int    ymax;// 4 type, name 4, 11.
};

// zu DNA structure 7, 19
struct rctf
{
    float    xmin;// 1 type, name 7, 8.
    float    xmax;// 2 type, name 7, 9.
    float    ymin;// 3 type, name 7, 10.
    float    ymax;// 4 type, name 7, 11.
};

// zu DNA structure 8, 22
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

// zu DNA structure 9, 26
struct PreviewImage
{
    int           w[2];                // 1 type, name 4, 38.
    int           h[2];                // 2 type, name 4, 39.
    short         changed[2];          // 3 type, name 2, 40.
    short         changed_timestamp[2];// 4 type, name 2, 41.
    int           *rect[2];            // 5 type, name 4, 42.
    void          *gputexture[2];      // Note: using void* on undefined DNA type: GPUTexture
};

// zu DNA structure 10, 28
struct IpoDriver
{
    Object    *ob;      // 1 type, name 29, 44.
    short     blocktype;// 2 type, name 2, 45.
    short     adrcode;  // 3 type, name 2, 46.
    short     type;     // 4 type, name 2, 16.
    short     flag;     // 5 type, name 2, 18.
    char      name[128];// 6 type, name 0, 47.
};

// zu DNA structure 11, 34
struct KeyBlock
{
    KeyBlock    *next;     // 1 type, name 34, 0.
    KeyBlock    *prev;     // 2 type, name 34, 1.
    float       pos;       // 3 type, name 7, 67.
    float       curval;    // 4 type, name 7, 60.
    short       type;      // 5 type, name 2, 16.
    short       pad1;      // 6 type, name 2, 68.
    short       relative;  // 7 type, name 2, 69.
    short       flag;      // 8 type, name 2, 18.
    int         totelem;   // 9 type, name 4, 70.
    int         uid;       // 10 type, name 4, 71.
    void        *data;     // 11 type, name 11, 2.
    char        name[64];  // 12 type, name 0, 19.
    char        vgroup[64];// 13 type, name 0, 72.
    float       slidermin; // 14 type, name 7, 73.
    float       slidermax; // 15 type, name 7, 74.
};

// zu DNA structure 12, 37
struct TextLine
{
    TextLine    *next;  // 1 type, name 37, 0.
    TextLine    *prev;  // 2 type, name 37, 1.
    char        *line;  // 3 type, name 0, 85.
    char        *format;// 4 type, name 0, 86.
    int         len;    // 5 type, name 4, 22.
    int         blen;   // 6 type, name 4, 87.
};

// zu DNA structure 13, 25
struct PackedFile
{
    int     size; // 1 type, name 4, 101.
    int     seek; // 2 type, name 4, 102.
    void    *data;// 3 type, name 11, 2.
};

// zu DNA structure 14, 39
struct GPUDOFSettings
{
    float    focus_distance;// 1 type, name 7, 103.
    float    fstop;         // 2 type, name 7, 104.
    float    focal_length;  // 3 type, name 7, 105.
    float    sensor;        // 4 type, name 7, 106.
};

// zu DNA structure 15, 40
struct GPUSSAOSettings
{
    float    factor;      // 1 type, name 7, 107.
    float    color[3];    // 2 type, name 7, 108.
    float    distance_max;// 3 type, name 7, 109.
    float    attenuation; // 4 type, name 7, 110.
    int      samples;     // 5 type, name 4, 111.
    int      pad;         // 6 type, name 4, 66.
};

// zu DNA structure 16, 41
struct GPUFXSettings
{
    GPUDOFSettings     *dof;   // 1 type, name 39, 112.
    GPUSSAOSettings    *ssao;  // 2 type, name 40, 113.
    char               fx_flag;// 3 type, name 0, 114.
    char               pad[7]; // 4 type, name 0, 115.
};

// zu DNA structure 17, 43
struct ImageUser
{
    Scene    *scene;     // 1 type, name 44, 131.
    int      framenr;    // 2 type, name 4, 132.
    int      frames;     // 3 type, name 4, 133.
    int      offset;     // 4 type, name 4, 134.
    int      sfra;       // 5 type, name 4, 135.
    char     fie_ima;    // 6 type, name 0, 136.
    char     cycl;       // 7 type, name 0, 137.
    char     ok;         // 8 type, name 0, 138.
    char     pad;        // 9 type, name 0, 66.
    short    multi_index;// 10 type, name 2, 139.
    short    layer;      // 11 type, name 2, 140.
    short    pass;       // 12 type, name 2, 141.
    short    flag;       // 13 type, name 2, 18.
    int      pad2;       // 14 type, name 4, 29.
};

// zu DNA structure 18, 45
struct RenderSlot
{
    char    name[64];// 1 type, name 0, 19.
};

// zu DNA structure 19, 51
struct MTex
{
    short     texco;           // 1 type, name 2, 174.
    short     mapto;           // 2 type, name 2, 175.
    short     maptoneg;        // 3 type, name 2, 176.
    short     blendtype;       // 4 type, name 2, 177.
    Object    *object;         // 5 type, name 29, 178.
    Tex       *tex;            // 6 type, name 52, 179.
    char      uvname[64];      // 7 type, name 0, 180.
    char      projx;           // 8 type, name 0, 181.
    char      projy;           // 9 type, name 0, 182.
    char      projz;           // 10 type, name 0, 183.
    char      mapping;         // 11 type, name 0, 184.
    char      brush_map_mode;  // 12 type, name 0, 185.
    char      brush_angle_mode;// 13 type, name 0, 186.
    char      pad[2];          // 14 type, name 0, 187.
    float     ofs[3];          // 15 type, name 7, 188.
    float     size[3];         // 16 type, name 7, 189.
    float     rot;             // 17 type, name 7, 190.
    float     random_angle;    // 18 type, name 7, 191.
    short     texflag;         // 19 type, name 2, 192.
    short     colormodel;      // 20 type, name 2, 193.
    short     pmapto;          // 21 type, name 2, 194.
    short     pmaptoneg;       // 22 type, name 2, 195.
    short     normapspace;     // 23 type, name 2, 196.
    short     which_output;    // 24 type, name 2, 197.
    float     r;               // 25 type, name 7, 198.
    float     g;               // 26 type, name 7, 199.
    float     b;               // 27 type, name 7, 200.
    float     k;               // 28 type, name 7, 201.
    float     def_var;         // 29 type, name 7, 202.
    float     rt;              // 30 type, name 7, 56.
    float     colfac;          // 31 type, name 7, 203.
    float     varfac;          // 32 type, name 7, 204.
    float     norfac;          // 33 type, name 7, 205.
    float     dispfac;         // 34 type, name 7, 206.
    float     warpfac;         // 35 type, name 7, 207.
    float     colspecfac;      // 36 type, name 7, 208.
    float     mirrfac;         // 37 type, name 7, 209.
    float     alphafac;        // 38 type, name 7, 210.
    float     difffac;         // 39 type, name 7, 211.
    float     specfac;         // 40 type, name 7, 212.
    float     emitfac;         // 41 type, name 7, 213.
    float     hardfac;         // 42 type, name 7, 214.
    float     raymirrfac;      // 43 type, name 7, 215.
    float     translfac;       // 44 type, name 7, 216.
    float     ambfac;          // 45 type, name 7, 217.
    float     colemitfac;      // 46 type, name 7, 218.
    float     colreflfac;      // 47 type, name 7, 219.
    float     coltransfac;     // 48 type, name 7, 220.
    float     densfac;         // 49 type, name 7, 221.
    float     scatterfac;      // 50 type, name 7, 222.
    float     reflfac;         // 51 type, name 7, 223.
    float     timefac;         // 52 type, name 7, 224.
    float     lengthfac;       // 53 type, name 7, 225.
    float     clumpfac;        // 54 type, name 7, 226.
    float     dampfac;         // 55 type, name 7, 227.
    float     kinkfac;         // 56 type, name 7, 228.
    float     kinkampfac;      // 57 type, name 7, 229.
    float     roughfac;        // 58 type, name 7, 230.
    float     padensfac;       // 59 type, name 7, 231.
    float     gravityfac;      // 60 type, name 7, 232.
    float     lifefac;         // 61 type, name 7, 233.
    float     sizefac;         // 62 type, name 7, 234.
    float     ivelfac;         // 63 type, name 7, 235.
    float     fieldfac;        // 64 type, name 7, 236.
    int       pad2;            // 65 type, name 4, 29.
    float     shadowfac;       // 66 type, name 7, 237.
    float     zenupfac;        // 67 type, name 7, 238.
    float     zendownfac;      // 68 type, name 7, 239.
    float     blendfac;        // 69 type, name 7, 240.
};

// zu DNA structure 20, 53
struct CBData
{
    float    r;  // 1 type, name 7, 198.
    float    g;  // 2 type, name 7, 199.
    float    b;  // 3 type, name 7, 200.
    float    a;  // 4 type, name 7, 241.
    float    pos;// 5 type, name 7, 67.
    int      cur;// 6 type, name 4, 63.
};

// zu DNA structure 21, 55
struct EnvMap
{
    Object    *object;     // 1 type, name 29, 178.
    Image     *ima;        // 2 type, name 46, 248.
    void      *cube[6];    // Note: using void* on undefined DNA type: ImBuf
    float     imat[4][4];  // 4 type, name 7, 250.
    float     obimat[3][3];// 5 type, name 7, 251.
    short     type;        // 6 type, name 2, 16.
    short     stype;       // 7 type, name 2, 252.
    float     clipsta;     // 8 type, name 7, 118.
    float     clipend;     // 9 type, name 7, 119.
    float     viewscale;   // 10 type, name 7, 253.
    int       notlay;      // 11 type, name 4, 254.
    short     cuberes;     // 12 type, name 2, 255.
    short     depth;       // 13 type, name 2, 256.
    int       ok;          // 14 type, name 4, 138.
    int       lastframe;   // 15 type, name 4, 150.
    short     recalc;      // 16 type, name 2, 257.
    short     lastsize;    // 17 type, name 2, 258.
};

// zu DNA structure 22, 57
struct PointDensity
{
    short           flag;               // 1 type, name 2, 18.
    short           falloff_type;       // 2 type, name 2, 259.
    float           falloff_softness;   // 3 type, name 7, 260.
    float           radius;             // 4 type, name 7, 261.
    short           source;             // 5 type, name 2, 149.
    short           color_source;       // 6 type, name 2, 262.
    int             totpoints;          // 7 type, name 4, 263.
    int             pdpad;              // 8 type, name 4, 264.
    Object          *object;            // 9 type, name 29, 178.
    int             psys;               // 10 type, name 4, 265.
    short           psys_cache_space;   // 11 type, name 2, 266.
    short           ob_cache_space;     // 12 type, name 2, 267.
    void            *point_tree;        // 13 type, name 11, 268.
    float           *point_data;        // 14 type, name 7, 269.
    float           noise_size;         // 15 type, name 7, 270.
    short           noise_depth;        // 16 type, name 2, 271.
    short           noise_influence;    // 17 type, name 2, 272.
    short           noise_basis;        // 18 type, name 2, 273.
    short           pdpad3[3];          // 19 type, name 2, 274.
    float           noise_fac;          // 20 type, name 7, 275.
    float           speed_scale;        // 21 type, name 7, 276.
    float           falloff_speed_scale;// 22 type, name 7, 277.
    float           pdpad2;             // 23 type, name 7, 278.
    ColorBand       *coba;              // 24 type, name 54, 279.
    CurveMapping    *falloff_curve;     // 25 type, name 58, 280.
};

// zu DNA structure 23, 59
struct VoxelData
{
    int       resol[3];         // 1 type, name 4, 281.
    int       interp_type;      // 2 type, name 4, 282.
    short     file_format;      // 3 type, name 2, 283.
    short     flag;             // 4 type, name 2, 18.
    short     extend;           // 5 type, name 2, 284.
    short     smoked_type;      // 6 type, name 2, 285.
    short     hair_type;        // 7 type, name 2, 286.
    short     data_type;        // 8 type, name 2, 287.
    int       _pad;             // 9 type, name 4, 288.
    Object    *object;          // 10 type, name 29, 178.
    float     int_multiplier;   // 11 type, name 7, 289.
    int       still_frame;      // 12 type, name 4, 290.
    char      source_path[1024];// 13 type, name 0, 291.
    float     *dataset;         // 14 type, name 7, 292.
    int       cachedframe;      // 15 type, name 4, 293.
    int       ok;               // 16 type, name 4, 138.
};

// zu DNA structure 24, 60
struct OceanTex
{
    Object    *object;     // 1 type, name 29, 178.
    char      oceanmod[64];// 2 type, name 0, 294.
    int       output;      // 3 type, name 4, 295.
    int       pad;         // 4 type, name 4, 66.
};

// zu DNA structure 25, 62
struct TexMapping
{
    float     loc[3];   // 1 type, name 7, 341.
    float     rot[3];   // 2 type, name 7, 342.
    float     size[3];  // 3 type, name 7, 189.
    int       flag;     // 4 type, name 4, 18.
    char      projx;    // 5 type, name 0, 181.
    char      projy;    // 6 type, name 0, 182.
    char      projz;    // 7 type, name 0, 183.
    char      mapping;  // 8 type, name 0, 184.
    int       type;     // 9 type, name 4, 16.
    float     mat[4][4];// 10 type, name 7, 343.
    float     min[3];   // 11 type, name 7, 344.
    float     max[3];   // 12 type, name 7, 345.
    Object    *ob;      // 13 type, name 29, 44.
};

// zu DNA structure 26, 65
struct VolumeSettings
{
    float    density;            // 1 type, name 7, 409.
    float    emission;           // 2 type, name 7, 410.
    float    scattering;         // 3 type, name 7, 411.
    float    reflection;         // 4 type, name 7, 412.
    float    emission_col[3];    // 5 type, name 7, 413.
    float    transmission_col[3];// 6 type, name 7, 414.
    float    reflection_col[3];  // 7 type, name 7, 415.
    float    density_scale;      // 8 type, name 7, 416.
    float    depth_cutoff;       // 9 type, name 7, 417.
    float    asymmetry;          // 10 type, name 7, 418.
    short    stepsize_type;      // 11 type, name 2, 419.
    short    shadeflag;          // 12 type, name 2, 420.
    short    shade_type;         // 13 type, name 2, 421.
    short    precache_resolution;// 14 type, name 2, 422.
    float    stepsize;           // 15 type, name 7, 423.
    float    ms_diff;            // 16 type, name 7, 424.
    float    ms_intensity;       // 17 type, name 7, 425.
    float    ms_spread;          // 18 type, name 7, 426.
};

// zu DNA structure 27, 66
struct GameSettings
{
    int    flag;            // 1 type, name 4, 18.
    int    alpha_blend;     // 2 type, name 4, 427.
    int    face_orientation;// 3 type, name 4, 428.
    int    pad1;            // 4 type, name 4, 68.
};

// zu DNA structure 28, 67
struct TexPaintSlot
{
    Image    *ima;   // 1 type, name 46, 248.
    char     *uvname;// 2 type, name 0, 429.
    int      index;  // 3 type, name 4, 430.
    int      pad;    // 4 type, name 4, 66.
};

// zu DNA structure 29, 72
struct MetaElem
{
    MetaElem    *next;  // 1 type, name 72, 0.
    MetaElem    *prev;  // 2 type, name 72, 1.
    BoundBox    *bb;    // 3 type, name 73, 550.
    short       type;   // 4 type, name 2, 16.
    short       flag;   // 5 type, name 2, 18.
    short       selcol1;// 6 type, name 2, 551.
    short       selcol2;// 7 type, name 2, 552.
    float       x;      // 8 type, name 7, 5.
    float       y;      // 9 type, name 7, 6.
    float       z;      // 10 type, name 7, 7.
    float       quat[4];// 11 type, name 7, 553.
    float       expx;   // 12 type, name 7, 554.
    float       expy;   // 13 type, name 7, 555.
    float       expz;   // 14 type, name 7, 556.
    float       rad;    // 15 type, name 7, 557.
    float       rad2;   // 16 type, name 7, 558.
    float       s;      // 17 type, name 7, 559.
    float       len;    // 18 type, name 7, 22.
    float       *mat;   // 19 type, name 7, 560.
    float       *imat;  // 20 type, name 7, 561.
};

// zu DNA structure 30, 32
struct BezTriple
{
    float    vec[3][3];// 1 type, name 7, 572.
    float    alfa;     // 2 type, name 7, 573.
    float    weight;   // 3 type, name 7, 574.
    float    radius;   // 4 type, name 7, 261.
    char     ipo;      // 5 type, name 0, 54.
    char     h1;       // 6 type, name 0, 575.
    char     h2;       // 7 type, name 0, 576.
    char     f1;       // 8 type, name 0, 577.
    char     f2;       // 9 type, name 0, 578.
    char     f3;       // 10 type, name 0, 579.
    char     hide;     // 11 type, name 0, 580.
    char     easing;   // 12 type, name 0, 581.
    float    back;     // 13 type, name 7, 582.
    float    amplitude;// 14 type, name 7, 583.
    float    period;   // 15 type, name 7, 584.
    char     pad[4];   // 16 type, name 0, 585.
};

// zu DNA structure 31, 31
struct BPoint
{
    float    vec[4];// 1 type, name 7, 586.
    float    alfa;  // 2 type, name 7, 573.
    float    weight;// 3 type, name 7, 574.
    short    f1;    // 4 type, name 2, 577.
    short    hide;  // 5 type, name 2, 580.
    float    radius;// 6 type, name 7, 261.
    float    pad;   // 7 type, name 7, 66.
};

// zu DNA structure 32, 75
struct Nurb
{
    Nurb         *next;        // 1 type, name 75, 0.
    Nurb         *prev;        // 2 type, name 75, 1.
    short        type;         // 3 type, name 2, 16.
    short        mat_nr;       // 4 type, name 2, 587.
    short        hide;         // 5 type, name 2, 580.
    short        flag;         // 6 type, name 2, 18.
    int          pntsu;        // 7 type, name 4, 588.
    int          pntsv;        // 8 type, name 4, 589.
    short        pad[2];       // 9 type, name 2, 187.
    short        resolu;       // 10 type, name 2, 590.
    short        resolv;       // 11 type, name 2, 591.
    short        orderu;       // 12 type, name 2, 592.
    short        orderv;       // 13 type, name 2, 593.
    short        flagu;        // 14 type, name 2, 594.
    short        flagv;        // 15 type, name 2, 595.
    float        *knotsu;      // 16 type, name 7, 596.
    float        *knotsv;      // 17 type, name 7, 597.
    BPoint       *bp;          // 18 type, name 31, 48.
    BezTriple    *bezt;        // 19 type, name 32, 49.
    short        tilt_interp;  // 20 type, name 2, 598.
    short        radius_interp;// 21 type, name 2, 599.
    int          charidx;      // 22 type, name 4, 600.
};

// zu DNA structure 33, 76
struct CharInfo
{
    short    kern;  // 1 type, name 2, 601.
    short    mat_nr;// 2 type, name 2, 587.
    char     flag;  // 3 type, name 0, 18.
    char     pad;   // 4 type, name 0, 66.
    short    pad2;  // 5 type, name 2, 29.
};

// zu DNA structure 34, 77
struct TextBox
{
    float    x;// 1 type, name 7, 5.
    float    y;// 2 type, name 7, 6.
    float    w;// 3 type, name 7, 602.
    float    h;// 4 type, name 7, 603.
};

// zu DNA structure 35, 91
struct TFace
{
    void     *tpage;  // 1 type, name 11, 690.
    float    uv[4][2];// 2 type, name 7, 691.
    int      col[4];  // 3 type, name 4, 692.
    char     flag;    // 4 type, name 0, 18.
    char     transp;  // 5 type, name 0, 693.
    short    mode;    // 6 type, name 2, 351.
    short    tile;    // 7 type, name 2, 694.
    short    unwrap;  // 8 type, name 2, 695.
};

// zu DNA structure 36, 89
struct MFace
{
    int      v1;    // 1 type, name 4, 696.
    int      v2;    // 2 type, name 4, 697.
    int      v3;    // 3 type, name 4, 698.
    int      v4;    // 4 type, name 4, 699.
    short    mat_nr;// 5 type, name 2, 587.
    char     edcode;// 6 type, name 0, 700.
    char     flag;  // 7 type, name 0, 18.
};

// zu DNA structure 37, 93
struct MEdge
{
    int      v1;     // 1 type, name 4, 696.
    int      v2;     // 2 type, name 4, 697.
    char     crease; // 3 type, name 0, 701.
    char     bweight;// 4 type, name 0, 702.
    short    flag;   // 5 type, name 2, 18.
};

// zu DNA structure 38, 99
struct MDeformWeight
{
    int      def_nr;// 1 type, name 4, 703.
    float    weight;// 2 type, name 7, 574.
};

// zu DNA structure 39, 94
struct MDeformVert
{
    MDeformWeight    *dw;      // 1 type, name 99, 704.
    int              totweight;// 2 type, name 4, 705.
    int              flag;     // 3 type, name 4, 18.
};

// zu DNA structure 40, 92
struct MVert
{
    float    co[3];  // 1 type, name 7, 706.
    short    no[3];  // 2 type, name 2, 707.
    char     flag;   // 3 type, name 0, 18.
    char     bweight;// 4 type, name 0, 702.
};

// zu DNA structure 41, 95
struct MCol
{
    char    a;// 1 type, name 0, 241.
    char    r;// 2 type, name 0, 198.
    char    g;// 3 type, name 0, 199.
    char    b;// 4 type, name 0, 200.
};

// zu DNA structure 42, 84
struct MPoly
{
    int      loopstart;// 1 type, name 4, 708.
    int      totloop;  // 2 type, name 4, 681.
    short    mat_nr;   // 3 type, name 2, 587.
    char     flag;     // 4 type, name 0, 18.
    char     pad;      // 5 type, name 0, 66.
};

// zu DNA structure 43, 86
struct MLoop
{
    int    v;// 1 type, name 4, 709.
    int    e;// 2 type, name 4, 710.
};

// zu DNA structure 44, 85
struct MTexPoly
{
    Image    *tpage;// 1 type, name 46, 690.
    char     flag;  // 2 type, name 0, 18.
    char     transp;// 3 type, name 0, 693.
    short    mode;  // 4 type, name 2, 351.
    short    tile;  // 5 type, name 2, 694.
    short    pad;   // 6 type, name 2, 66.
};

// zu DNA structure 45, 87
struct MLoopUV
{
    float    uv[2];// 1 type, name 7, 711.
    int      flag; // 2 type, name 4, 18.
};

// zu DNA structure 46, 88
struct MLoopCol
{
    char    r;// 1 type, name 0, 198.
    char    g;// 2 type, name 0, 199.
    char    b;// 3 type, name 0, 200.
    char    a;// 4 type, name 0, 241.
};

// zu DNA structure 47, 83
struct MSelect
{
    int    index;// 1 type, name 4, 430.
    int    type; // 2 type, name 4, 16.
};

// zu DNA structure 48, 90
struct MTFace
{
    float    uv[4][2];// 1 type, name 7, 691.
    Image    *tpage;  // 2 type, name 46, 690.
    char     flag;    // 3 type, name 0, 18.
    char     transp;  // 4 type, name 0, 693.
    short    mode;    // 5 type, name 2, 351.
    short    tile;    // 6 type, name 2, 694.
    short    unwrap;  // 7 type, name 2, 695.
};

// zu DNA structure 49, 100
struct MFloatProperty
{
    float    f;// 1 type, name 7, 712.
};

// zu DNA structure 50, 101
struct MIntProperty
{
    int    i;// 1 type, name 4, 713.
};

// zu DNA structure 51, 102
struct MStringProperty
{
    char    s[255];// 1 type, name 0, 714.
    char    s_len; // 2 type, name 0, 715.
};

// zu DNA structure 52, 103
struct OrigSpaceFace
{
    float    uv[4][2];// 1 type, name 7, 691.
};

// zu DNA structure 53, 104
struct OrigSpaceLoop
{
    float    uv[2];// 1 type, name 7, 711.
};

// zu DNA structure 54, 105
struct MDisps
{
    int      totdisp;   // 1 type, name 4, 716.
    int      level;     // 2 type, name 4, 717.
    float    (*disps)();// 3 type, name 7, 718.
    int      *hidden;   // 4 type, name 4, 719.
};

// zu DNA structure 55, 106
struct MultiresCol
{
    float    a;// 1 type, name 7, 241.
    float    r;// 2 type, name 7, 198.
    float    g;// 3 type, name 7, 199.
    float    b;// 4 type, name 7, 200.
};

// zu DNA structure 56, 108
struct MultiresFace
{
    int     v[4];  // 1 type, name 4, 720.
    int     mid;   // 2 type, name 4, 721.
    char    flag;  // 3 type, name 0, 18.
    char    mat_nr;// 4 type, name 0, 587.
    char    pad[2];// 5 type, name 0, 187.
};

// zu DNA structure 57, 109
struct MultiresEdge
{
    int    v[2];// 1 type, name 4, 722.
    int    mid; // 2 type, name 4, 721.
};

// zu DNA structure 58, 110
struct MultiresLevel
{
    MultiresLevel      *next;    // 1 type, name 110, 0.
    MultiresLevel      *prev;    // 2 type, name 110, 1.
    MultiresFace       *faces;   // 3 type, name 108, 723.
    MultiresColFace    *colfaces;// 4 type, name 107, 724.
    MultiresEdge       *edges;   // 5 type, name 109, 725.
    int                totvert;  // 6 type, name 4, 53.
    int                totface;  // 7 type, name 4, 678.
    int                totedge;  // 8 type, name 4, 677.
    int                pad;      // 9 type, name 4, 66.
    MVert              *verts;   // 10 type, name 92, 726.
};

// zu DNA structure 59, 111
struct MRecast
{
    int    i;// 1 type, name 4, 713.
};

// zu DNA structure 60, 112
struct GridPaintMask
{
    float    *data;// 1 type, name 7, 2.
    int      level;// 2 type, name 4, 717.
    int      pad;  // 3 type, name 4, 66.
};

// zu DNA structure 61, 113
struct MVertSkin
{
    float    radius[3];// 1 type, name 7, 737.
    int      flag;     // 2 type, name 4, 18.
};

// zu DNA structure 62, 114
struct FreestyleEdge
{
    char    flag;  // 1 type, name 0, 18.
    char    pad[3];// 2 type, name 0, 350.
};

// zu DNA structure 63, 115
struct FreestyleFace
{
    char    flag;  // 1 type, name 0, 18.
    char    pad[3];// 2 type, name 0, 350.
};

// zu DNA structure 64, 116
struct ModifierData
{
    ModifierData    *next;     // 1 type, name 116, 0.
    ModifierData    *prev;     // 2 type, name 116, 1.
    int             type;      // 3 type, name 4, 16.
    int             mode;      // 4 type, name 4, 351.
    int             stackindex;// 5 type, name 4, 738.
    int             pad;       // 6 type, name 4, 66.
    char            name[64];  // 7 type, name 0, 19.
    Scene           *scene;    // 8 type, name 44, 131.
    char            *error;    // 9 type, name 0, 739.
};

// zu DNA structure 65, 153
struct MDefInfluence
{
    int      vertex;// 1 type, name 4, 847.
    float    weight;// 2 type, name 7, 574.
};

// zu DNA structure 66, 154
struct MDefCell
{
    int    offset;      // 1 type, name 4, 134.
    int    totinfluence;// 2 type, name 4, 848.
};

// zu DNA structure 67, 188
struct EditLatt
{
    Lattice    *latt;  // 1 type, name 189, 998.
    int        shapenr;// 2 type, name 4, 606.
    char       pad[4]; // 3 type, name 0, 585.
};

// zu DNA structure 68, 190
struct bDeformGroup
{
    bDeformGroup    *next;   // 1 type, name 190, 0.
    bDeformGroup    *prev;   // 2 type, name 190, 1.
    char            name[64];// 3 type, name 0, 19.
    char            flag;    // 4 type, name 0, 18.
    char            pad[7];  // 5 type, name 0, 115.
};

// zu DNA structure 69, 73
struct BoundBox
{
    float    vec[8][3];// 1 type, name 7, 1015.
    int      flag;     // 2 type, name 4, 18.
    int      pad;      // 3 type, name 4, 66.
};

// zu DNA structure 70, 191
struct LodLevel
{
    LodLevel    *next;   // 1 type, name 191, 0.
    LodLevel    *prev;   // 2 type, name 191, 1.
    Object      *source; // 3 type, name 29, 1016.
    int         flags;   // 4 type, name 4, 89.
    float       distance;// 5 type, name 7, 1017.
};

// zu DNA structure 71, 204
struct ObHook
{
    ObHook    *next;          // 1 type, name 204, 0.
    ObHook    *prev;          // 2 type, name 204, 1.
    Object    *parent;        // 3 type, name 29, 36.
    float     parentinv[4][4];// 4 type, name 7, 815.
    float     mat[4][4];      // 5 type, name 7, 343.
    float     cent[3];        // 6 type, name 7, 816.
    float     falloff;        // 7 type, name 7, 808.
    char      name[64];       // 8 type, name 0, 19.
    int       *indexar;       // 9 type, name 4, 817.
    int       totindex;       // 10 type, name 4, 818.
    int       curindex;       // 11 type, name 4, 1127.
    short     type;           // 12 type, name 2, 16.
    short     active;         // 13 type, name 2, 1128.
    float     force;          // 14 type, name 7, 819.
};

// zu DNA structure 72, 205
struct DupliObject
{
    DupliObject       *next;           // 1 type, name 205, 0.
    DupliObject       *prev;           // 2 type, name 205, 1.
    Object            *ob;             // 3 type, name 29, 44.
    float             mat[4][4];       // 4 type, name 7, 343.
    float             orco[3];         // 5 type, name 7, 1129.
    float             uv[2];           // 6 type, name 7, 711.
    short             type;            // 7 type, name 2, 16.
    char              no_draw;         // 8 type, name 0, 1130.
    char              animated;        // 9 type, name 0, 1131.
    int               persistent_id[8];// 10 type, name 4, 1132.
    ParticleSystem    *particle_system;// 11 type, name 157, 1133.
};

// zu DNA structure 73, 199
struct PartDeflect
{
    int       flag;          // 1 type, name 4, 18.
    short     deflect;       // 2 type, name 2, 1134.
    short     forcefield;    // 3 type, name 2, 1135.
    short     falloff;       // 4 type, name 2, 808.
    short     shape;         // 5 type, name 2, 1136.
    short     tex_mode;      // 6 type, name 2, 1137.
    short     kink;          // 7 type, name 2, 1138.
    short     kink_axis;     // 8 type, name 2, 1139.
    short     zdir;          // 9 type, name 2, 1140.
    float     f_strength;    // 10 type, name 7, 1141.
    float     f_damp;        // 11 type, name 7, 1142.
    float     f_flow;        // 12 type, name 7, 1143.
    float     f_size;        // 13 type, name 7, 1144.
    float     f_power;       // 14 type, name 7, 1145.
    float     maxdist;       // 15 type, name 7, 1146.
    float     mindist;       // 16 type, name 7, 1147.
    float     f_power_r;     // 17 type, name 7, 1148.
    float     maxrad;        // 18 type, name 7, 1149.
    float     minrad;        // 19 type, name 7, 1150.
    float     pdef_damp;     // 20 type, name 7, 1151.
    float     pdef_rdamp;    // 21 type, name 7, 1152.
    float     pdef_perm;     // 22 type, name 7, 1153.
    float     pdef_frict;    // 23 type, name 7, 1154.
    float     pdef_rfrict;   // 24 type, name 7, 1155.
    float     pdef_stickness;// 25 type, name 7, 1156.
    float     absorption;    // 26 type, name 7, 1157.
    float     pdef_sbdamp;   // 27 type, name 7, 1158.
    float     pdef_sbift;    // 28 type, name 7, 1159.
    float     pdef_sboft;    // 29 type, name 7, 1160.
    float     clump_fac;     // 30 type, name 7, 1161.
    float     clump_pow;     // 31 type, name 7, 1162.
    float     kink_freq;     // 32 type, name 7, 1163.
    float     kink_shape;    // 33 type, name 7, 1164.
    float     kink_amp;      // 34 type, name 7, 1165.
    float     free_end;      // 35 type, name 7, 1166.
    float     tex_nabla;     // 36 type, name 7, 1167.
    Tex       *tex;          // 37 type, name 52, 179.
    void      *rng;          // Note: using void* on undefined DNA type: RNG
    float     f_noise;       // 39 type, name 7, 1169.
    int       seed;          // 40 type, name 4, 756.
    Object    *f_source;     // 41 type, name 29, 1170.
};

// zu DNA structure 74, 207
struct EffectorWeights
{
    Group    *group;        // 1 type, name 69, 520.
    float    weight[14];    // 2 type, name 7, 1171.
    float    global_gravity;// 3 type, name 7, 1172.
    short    flag;          // 4 type, name 2, 18.
    short    rt[3];         // 5 type, name 2, 1173.
    int      pad;           // 6 type, name 4, 66.
};

// zu DNA structure 75, 208
struct PTCacheExtra
{
    PTCacheExtra    *next;  // 1 type, name 208, 0.
    PTCacheExtra    *prev;  // 2 type, name 208, 1.
    int             type;   // 3 type, name 4, 16.
    int             totdata;// 4 type, name 4, 1174.
    void            *data;  // 5 type, name 11, 2.
};

// zu DNA structure 76, 211
struct SBVertex
{
    float    vec[4];// 1 type, name 7, 586.
};

// zu DNA structure 77, 198
struct BulletSoftBody
{
    int      flag;                // 1 type, name 4, 18.
    float    linStiff;            // 2 type, name 7, 1195.
    float    angStiff;            // 3 type, name 7, 1196.
    float    volume;              // 4 type, name 7, 1197.
    int      viterations;         // 5 type, name 4, 1198.
    int      piterations;         // 6 type, name 4, 1199.
    int      diterations;         // 7 type, name 4, 1200.
    int      citerations;         // 8 type, name 4, 1201.
    float    kSRHR_CL;            // 9 type, name 7, 1202.
    float    kSKHR_CL;            // 10 type, name 7, 1203.
    float    kSSHR_CL;            // 11 type, name 7, 1204.
    float    kSR_SPLT_CL;         // 12 type, name 7, 1205.
    float    kSK_SPLT_CL;         // 13 type, name 7, 1206.
    float    kSS_SPLT_CL;         // 14 type, name 7, 1207.
    float    kVCF;                // 15 type, name 7, 1208.
    float    kDP;                 // 16 type, name 7, 1209.
    float    kDG;                 // 17 type, name 7, 1210.
    float    kLF;                 // 18 type, name 7, 1211.
    float    kPR;                 // 19 type, name 7, 1212.
    float    kVC;                 // 20 type, name 7, 1213.
    float    kDF;                 // 21 type, name 7, 1214.
    float    kMT;                 // 22 type, name 7, 1215.
    float    kCHR;                // 23 type, name 7, 1216.
    float    kKHR;                // 24 type, name 7, 1217.
    float    kSHR;                // 25 type, name 7, 1218.
    float    kAHR;                // 26 type, name 7, 1219.
    int      collisionflags;      // 27 type, name 4, 1220.
    int      numclusteriterations;// 28 type, name 4, 1221.
    float    welding;             // 29 type, name 7, 1222.
    float    margin;              // 30 type, name 7, 1074.
};

// zu DNA structure 78, 215
struct FluidVertexVelocity
{
    float    vel[3];// 1 type, name 7, 1272.
};

// zu DNA structure 79, 162
struct FluidsimSettings
{
    FluidsimModifierData    *fmd;                 // 1 type, name 161, 1273.
    int                     threads;              // 2 type, name 4, 1274.
    int                     pad1;                 // 3 type, name 4, 68.
    short                   type;                 // 4 type, name 2, 16.
    short                   show_advancedoptions; // 5 type, name 2, 1275.
    short                   resolutionxyz;        // 6 type, name 2, 1276.
    short                   previewresxyz;        // 7 type, name 2, 1277.
    float                   realsize;             // 8 type, name 7, 1278.
    short                   guiDisplayMode;       // 9 type, name 2, 1279.
    short                   renderDisplayMode;    // 10 type, name 2, 1280.
    float                   viscosityValue;       // 11 type, name 7, 1281.
    short                   viscosityMode;        // 12 type, name 2, 1282.
    short                   viscosityExponent;    // 13 type, name 2, 1283.
    float                   grav[3];              // 14 type, name 7, 1284.
    float                   animStart;            // 15 type, name 7, 1285.
    float                   animEnd;              // 16 type, name 7, 1286.
    int                     bakeStart;            // 17 type, name 4, 1287.
    int                     bakeEnd;              // 18 type, name 4, 1288.
    int                     frameOffset;          // 19 type, name 4, 1289.
    int                     pad2;                 // 20 type, name 4, 29.
    float                   gstar;                // 21 type, name 7, 1290.
    int                     maxRefine;            // 22 type, name 4, 1291.
    float                   iniVelx;              // 23 type, name 7, 1292.
    float                   iniVely;              // 24 type, name 7, 1293.
    float                   iniVelz;              // 25 type, name 7, 1294.
    Mesh                    *orgMesh;             // 26 type, name 82, 1295.
    Mesh                    *meshBB;              // 27 type, name 82, 1296.
    char                    surfdataPath[1024];   // 28 type, name 0, 1297.
    float                   bbStart[3];           // 29 type, name 7, 1298.
    float                   bbSize[3];            // 30 type, name 7, 1299.
    Ipo                     *ipo;                 // 31 type, name 33, 80.
    short                   typeFlags;            // 32 type, name 2, 1300.
    char                    domainNovecgen;       // 33 type, name 0, 1301.
    char                    volumeInitType;       // 34 type, name 0, 1302.
    float                   partSlipValue;        // 35 type, name 7, 1303.
    int                     generateTracers;      // 36 type, name 4, 1304.
    float                   generateParticles;    // 37 type, name 7, 1305.
    float                   surfaceSmoothing;     // 38 type, name 7, 1306.
    int                     surfaceSubdivs;       // 39 type, name 4, 1307.
    int                     flag;                 // 40 type, name 4, 18.
    float                   particleInfSize;      // 41 type, name 7, 1308.
    float                   particleInfAlpha;     // 42 type, name 7, 1309.
    float                   farFieldSize;         // 43 type, name 7, 1310.
    FluidVertexVelocity     *meshVelocities;      // 44 type, name 215, 1311.
    int                     totvert;              // 45 type, name 4, 53.
    float                   cpsTimeStart;         // 46 type, name 7, 1312.
    float                   cpsTimeEnd;           // 47 type, name 7, 1313.
    float                   cpsQuality;           // 48 type, name 7, 1314.
    float                   attractforceStrength; // 49 type, name 7, 1315.
    float                   attractforceRadius;   // 50 type, name 7, 1316.
    float                   velocityforceStrength;// 51 type, name 7, 1317.
    float                   velocityforceRadius;  // 52 type, name 7, 1318.
    int                     lastgoodframe;        // 53 type, name 4, 1319.
    float                   animRate;             // 54 type, name 7, 1320.
};

// zu DNA structure 80, 217
struct Base
{
    Base      *next;  // 1 type, name 217, 0.
    Base      *prev;  // 2 type, name 217, 1.
    int       lay;    // 3 type, name 4, 1054.
    int       selcol; // 4 type, name 4, 1380.
    int       flag;   // 5 type, name 4, 18.
    short     sx;     // 6 type, name 2, 1381.
    short     sy;     // 7 type, name 2, 1382.
    Object    *object;// 8 type, name 29, 178.
};

// zu DNA structure 81, 218
struct AviCodecData
{
    void    *lpFormat;        // 1 type, name 11, 1383.
    void    *lpParms;         // 2 type, name 11, 1384.
    int     cbFormat;         // 3 type, name 4, 1385.
    int     cbParms;          // 4 type, name 4, 1386.
    int     fccType;          // 5 type, name 4, 1387.
    int     fccHandler;       // 6 type, name 4, 1388.
    int     dwKeyFrameEvery;  // 7 type, name 4, 1389.
    int     dwQuality;        // 8 type, name 4, 1390.
    int     dwBytesPerSecond; // 9 type, name 4, 1391.
    int     dwFlags;          // 10 type, name 4, 1392.
    int     dwInterleaveEvery;// 11 type, name 4, 1393.
    int     pad;              // 12 type, name 4, 66.
    char    avicodecname[128];// 13 type, name 0, 1394.
};

// zu DNA structure 82, 219
struct QuicktimeCodecData
{
    void    *cdParms;        // 1 type, name 11, 1395.
    void    *pad;            // 2 type, name 11, 1396.
    int     cdSize;          // 3 type, name 4, 1397.
    int     pad2;            // 4 type, name 4, 29.
    char    qtcodecname[128];// 5 type, name 0, 1398.
};

// zu DNA structure 83, 220
struct QuicktimeCodecSettings
{
    int      codecType;           // 1 type, name 4, 1399.
    int      codecSpatialQuality; // 2 type, name 4, 1400.
    int      codec;               // 3 type, name 4, 1401.
    int      codecFlags;          // 4 type, name 4, 1402.
    int      colorDepth;          // 5 type, name 4, 1403.
    int      codecTemporalQuality;// 6 type, name 4, 1404.
    int      minSpatialQuality;   // 7 type, name 4, 1405.
    int      minTemporalQuality;  // 8 type, name 4, 1406.
    int      keyFrameRate;        // 9 type, name 4, 1407.
    int      bitRate;             // 10 type, name 4, 1408.
    int      audiocodecType;      // 11 type, name 4, 1409.
    int      audioSampleRate;     // 12 type, name 4, 1410.
    short    audioBitDepth;       // 13 type, name 2, 1411.
    short    audioChannels;       // 14 type, name 2, 1412.
    int      audioCodecFlags;     // 15 type, name 4, 1413.
    int      audioBitRate;        // 16 type, name 4, 1414.
    int      pad1;                // 17 type, name 4, 68.
};

// zu DNA structure 84, 221
struct FFMpegCodecData
{
    int           type;           // 1 type, name 4, 16.
    int           codec;          // 2 type, name 4, 1401.
    int           audio_codec;    // 3 type, name 4, 1415.
    int           video_bitrate;  // 4 type, name 4, 1416.
    int           audio_bitrate;  // 5 type, name 4, 1417.
    int           audio_mixrate;  // 6 type, name 4, 1418.
    int           audio_channels; // 7 type, name 4, 1419.
    int           audio_pad;      // 8 type, name 4, 1420.
    float         audio_volume;   // 9 type, name 7, 1421.
    int           gop_size;       // 10 type, name 4, 1422.
    int           flags;          // 11 type, name 4, 89.
    int           rc_min_rate;    // 12 type, name 4, 1423.
    int           rc_max_rate;    // 13 type, name 4, 1424.
    int           rc_buffer_size; // 14 type, name 4, 1425.
    int           mux_packet_size;// 15 type, name 4, 1426.
    int           mux_rate;       // 16 type, name 4, 1427.
    IDProperty    *properties;    // 17 type, name 21, 30.
};

// zu DNA structure 85, 222
struct AudioData
{
    int      mixrate;       // 1 type, name 4, 1428.
    float    main;          // 2 type, name 7, 1429.
    float    speed_of_sound;// 3 type, name 7, 1430.
    float    doppler_factor;// 4 type, name 7, 1431.
    int      distance_model;// 5 type, name 4, 1432.
    short    flag;          // 6 type, name 2, 18.
    short    pad;           // 7 type, name 2, 66.
    float    volume;        // 8 type, name 7, 1197.
    float    pad2;          // 9 type, name 7, 29.
};

// zu DNA structure 86, 230
struct RenderProfile
{
    RenderProfile    *next;            // 1 type, name 230, 0.
    RenderProfile    *prev;            // 2 type, name 230, 1.
    char             name[32];         // 3 type, name 0, 1563.
    short            particle_perc;    // 4 type, name 2, 1564.
    short            subsurf_max;      // 5 type, name 2, 1565.
    short            shadbufsample_max;// 6 type, name 2, 1566.
    short            pad1;             // 7 type, name 2, 68.
    float            ao_error;         // 8 type, name 7, 1567.
    float            pad2;             // 9 type, name 7, 29.
};

// zu DNA structure 87, 231
struct GameDome
{
    short    res;      // 1 type, name 2, 773.
    short    mode;     // 2 type, name 2, 351.
    short    angle;    // 3 type, name 2, 797.
    short    tilt;     // 4 type, name 2, 1568.
    float    resbuf;   // 5 type, name 7, 1569.
    float    pad2;     // 6 type, name 7, 29.
    Text     *warptext;// 7 type, name 38, 1570.
};

// zu DNA structure 88, 232
struct GameFraming
{
    float    col[3];// 1 type, name 7, 1571.
    char     type;  // 2 type, name 0, 16.
    char     pad1;  // 3 type, name 0, 68.
    char     pad2;  // 4 type, name 0, 29.
    char     pad3;  // 5 type, name 0, 517.
};

// zu DNA structure 89, 233
struct RecastData
{
    float    cellsize;            // 1 type, name 7, 1572.
    float    cellheight;          // 2 type, name 7, 1573.
    float    agentmaxslope;       // 3 type, name 7, 1574.
    float    agentmaxclimb;       // 4 type, name 7, 1575.
    float    agentheight;         // 5 type, name 7, 1576.
    float    agentradius;         // 6 type, name 7, 1577.
    float    edgemaxlen;          // 7 type, name 7, 1578.
    float    edgemaxerror;        // 8 type, name 7, 1579.
    float    regionminsize;       // 9 type, name 7, 1580.
    float    regionmergesize;     // 10 type, name 7, 1581.
    int      vertsperpoly;        // 11 type, name 4, 1582.
    float    detailsampledist;    // 12 type, name 7, 1583.
    float    detailsamplemaxerror;// 13 type, name 7, 1584.
    short    pad1;                // 14 type, name 2, 68.
    short    pad2;                // 15 type, name 2, 29.
};

// zu DNA structure 90, 235
struct TimeMarker
{
    TimeMarker    *next;   // 1 type, name 235, 0.
    TimeMarker    *prev;   // 2 type, name 235, 1.
    int           frame;   // 3 type, name 4, 1175.
    char          name[64];// 4 type, name 0, 19.
    int           flag;    // 5 type, name 4, 18.
    Object        *camera; // 6 type, name 29, 1603.
};

// zu DNA structure 91, 236
struct Paint
{
    Brush           *brush;             // 1 type, name 237, 952.
    Palette         *palette;           // 2 type, name 238, 1604.
    CurveMapping    *cavity_curve;      // 3 type, name 58, 1605.
    void            *paint_cursor;      // 4 type, name 11, 1606.
    char            paint_cursor_col[4];// 5 type, name 0, 1607.
    int             flags;              // 6 type, name 4, 89.
    int             num_input_samples;  // 7 type, name 4, 1608.
    int             symmetry_flags;     // 8 type, name 4, 1609.
};

// zu DNA structure 92, 240
struct ParticleBrushData
{
    short    size;    // 1 type, name 2, 101.
    short    step;    // 2 type, name 2, 1180.
    short    invert;  // 3 type, name 2, 1620.
    short    count;   // 4 type, name 2, 767.
    int      flag;    // 5 type, name 4, 18.
    float    strength;// 6 type, name 7, 750.
};

// zu DNA structure 93, 245
struct TransformOrientation
{
    TransformOrientation    *next;    // 1 type, name 245, 0.
    TransformOrientation    *prev;    // 2 type, name 245, 1.
    char                    name[64]; // 3 type, name 0, 19.
    float                   mat[3][3];// 4 type, name 7, 1640.
    int                     pad;      // 5 type, name 4, 66.
};

// zu DNA structure 94, 246
struct UnifiedPaintSettings
{
    int           size;                     // 1 type, name 4, 101.
    float         unprojected_radius;       // 2 type, name 7, 1641.
    float         alpha;                    // 3 type, name 7, 446.
    float         weight;                   // 4 type, name 7, 574.
    float         rgb[3];                   // 5 type, name 7, 1642.
    float         secondary_rgb[3];         // 6 type, name 7, 1643.
    int           flag;                     // 7 type, name 4, 18.
    float         last_rake[2];             // 8 type, name 7, 1644.
    float         last_rake_angle;          // 9 type, name 7, 1645.
    int           last_stroke_valid;        // 10 type, name 4, 1646.
    float         average_stroke_accum[3];  // 11 type, name 7, 1647.
    int           average_stroke_counter;   // 12 type, name 4, 1648.
    float         brush_rotation;           // 13 type, name 7, 1649.
    float         brush_rotation_sec;       // 14 type, name 7, 1650.
    int           anchored_size;            // 15 type, name 4, 1651.
    float         overlap_factor;           // 16 type, name 7, 1652.
    char          draw_inverted;            // 17 type, name 0, 1653.
    char          stroke_active;            // 18 type, name 0, 1654.
    char          draw_anchored;            // 19 type, name 0, 1655.
    char          do_linear_conversion;     // 20 type, name 0, 1656.
    float         anchored_initial_mouse[2];// 21 type, name 7, 1657.
    float         pixel_radius;             // 22 type, name 7, 1658.
    float         size_pressure_value;      // 23 type, name 7, 1659.
    float         tex_mouse[2];             // 24 type, name 7, 1660.
    float         mask_tex_mouse[2];        // 25 type, name 7, 1661.
    void          *colorspace;              // Note: using void* on undefined DNA type: ColorSpace
};

// zu DNA structure 95, 248
struct MeshStatVis
{
    char     type;             // 1 type, name 0, 16.
    char     _pad1[2];         // 2 type, name 0, 1663.
    char     overhang_axis;    // 3 type, name 0, 1664.
    float    overhang_min;     // 4 type, name 7, 1665.
    float    overhang_max;     // 5 type, name 7, 1666.
    float    thickness_min;    // 6 type, name 7, 1667.
    float    thickness_max;    // 7 type, name 7, 1668.
    char     thickness_samples;// 8 type, name 0, 1669.
    char     _pad2[3];         // 9 type, name 0, 1670.
    float    distort_min;      // 10 type, name 7, 1671.
    float    distort_max;      // 11 type, name 7, 1672.
    float    sharp_min;        // 12 type, name 7, 1673.
    float    sharp_max;        // 13 type, name 7, 1674.
};

// zu DNA structure 96, 250
struct bStats
{
    int    totobj;     // 1 type, name 4, 1747.
    int    totlamp;    // 2 type, name 4, 1748.
    int    totobjsel;  // 3 type, name 4, 1749.
    int    totcurve;   // 4 type, name 4, 1750.
    int    totmesh;    // 5 type, name 4, 1751.
    int    totarmature;// 6 type, name 4, 1752.
    int    totvert;    // 7 type, name 4, 53.
    int    totface;    // 8 type, name 4, 678.
};

// zu DNA structure 97, 251
struct UnitSettings
{
    float    scale_length;   // 1 type, name 7, 1753.
    char     system;         // 2 type, name 0, 1754.
    char     system_rotation;// 3 type, name 0, 1755.
    short    flag;           // 4 type, name 2, 18.
};

// zu DNA structure 98, 252
struct PhysicsSettings
{
    float    gravity[3];      // 1 type, name 7, 1756.
    int      flag;            // 2 type, name 4, 18.
    int      quick_cache_step;// 3 type, name 4, 1757.
    int      rt;              // 4 type, name 4, 56.
};

// zu DNA structure 99, 253
struct DisplaySafeAreas
{
    float    title[2];        // 1 type, name 7, 1758.
    float    action[2];       // 2 type, name 7, 1759.
    float    title_center[2]; // 3 type, name 7, 1760.
    float    action_center[2];// 4 type, name 7, 1761.
};

// zu DNA structure 100, 261
struct RegionView3D
{
    float                winmat[4][4];      // 1 type, name 7, 1800.
    float                viewmat[4][4];     // 2 type, name 7, 1801.
    float                viewinv[4][4];     // 3 type, name 7, 1802.
    float                persmat[4][4];     // 4 type, name 7, 1803.
    float                persinv[4][4];     // 5 type, name 7, 1804.
    float                viewcamtexcofac[4];// 6 type, name 7, 1805.
    float                viewmatob[4][4];   // 7 type, name 7, 1806.
    float                persmatob[4][4];   // 8 type, name 7, 1807.
    float                clip[6][4];        // 9 type, name 7, 1808.
    float                clip_local[6][4];  // 10 type, name 7, 1809.
    BoundBox             *clipbb;           // 11 type, name 73, 1810.
    RegionView3D         *localvd;          // 12 type, name 261, 1811.
    void                 *render_engine;    // Note: using void* on undefined DNA type: RenderEngine
    void                 *depths;           // Note: using void* on undefined DNA type: ViewDepths
    void                 *gpuoffscreen;     // 15 type, name 11, 1814.
    void                 *sms;              // Note: using void* on undefined DNA type: SmoothView3DStore
    void                 *smooth_timer;     // Note: using void* on undefined DNA type: wmTimer
    float                twmat[4][4];       // 18 type, name 7, 1817.
    float                viewquat[4];       // 19 type, name 7, 1818.
    float                dist;              // 20 type, name 7, 358.
    float                camdx;             // 21 type, name 7, 1819.
    float                camdy;             // 22 type, name 7, 1820.
    float                pixsize;           // 23 type, name 7, 1821.
    float                ofs[3];            // 24 type, name 7, 188.
    float                camzoom;           // 25 type, name 7, 1822.
    char                 is_persp;          // 26 type, name 0, 1823.
    char                 persp;             // 27 type, name 0, 1824.
    char                 view;              // 28 type, name 0, 1799.
    char                 viewlock;          // 29 type, name 0, 1825.
    char                 viewlock_quad;     // 30 type, name 0, 1826.
    char                 pad[3];            // 31 type, name 0, 350.
    float                ofs_lock[2];       // 32 type, name 7, 1827.
    short                twdrawflag;        // 33 type, name 2, 1828.
    short                rflag;             // 34 type, name 2, 1829.
    float                lviewquat[4];      // 35 type, name 7, 1830.
    short                lpersp;            // 36 type, name 2, 1831.
    short                lview;             // 37 type, name 2, 1832.
    float                gridview;          // 38 type, name 7, 1833.
    float                tw_idot[3];        // 39 type, name 7, 1834.
    float                rot_angle;         // 40 type, name 7, 1835.
    float                rot_axis[3];       // 41 type, name 7, 1836.
    void                 *compositor;       // Note: using void* on undefined DNA type: GPUFX
};

// zu DNA structure 101, 279
struct SpaceTimeCache
{
    SpaceTimeCache    *next; // 1 type, name 279, 0.
    SpaceTimeCache    *prev; // 2 type, name 279, 1.
    float             *array;// 3 type, name 7, 1921.
};

// zu DNA structure 102, 283
struct MaskSpaceInfo
{
    Mask    *mask;       // 1 type, name 284, 1931.
    char    draw_flag;   // 2 type, name 0, 1929.
    char    draw_type;   // 3 type, name 0, 1932.
    char    overlay_mode;// 4 type, name 0, 1933.
    char    pad3[5];     // 5 type, name 0, 1871.
};

// zu DNA structure 103, 285
struct FileSelectParams
{
    char     title[96];        // 1 type, name 0, 1934.
    char     dir[1090];        // 2 type, name 0, 1935.
    char     pad_c1[2];        // 3 type, name 0, 1936.
    char     file[256];        // 4 type, name 0, 1937.
    char     renamefile[256];  // 5 type, name 0, 1938.
    char     renameedit[256];  // 6 type, name 0, 1939.
    char     filter_glob[64];  // 7 type, name 0, 1940.
    char     filter_search[64];// 8 type, name 0, 1941.
    int      active_file;      // 9 type, name 4, 1942.
    int      sel_first;        // 10 type, name 4, 1943.
    int      sel_last;         // 11 type, name 4, 1944.
    short    type;             // 12 type, name 2, 16.
    short    flag;             // 13 type, name 2, 18.
    short    sort;             // 14 type, name 2, 1945.
    short    display;          // 15 type, name 2, 1946.
    short    filter;           // 16 type, name 2, 458.
    short    f_fp;             // 17 type, name 2, 1947.
    char     fp_str[8];        // 18 type, name 0, 1948.
};

// zu DNA structure 104, 290
struct FSMenuEntry
{
    FSMenuEntry    *next;    // 1 type, name 290, 0.
    char           *path;    // 2 type, name 0, 1904.
    char           name[256];// 3 type, name 0, 1961.
    short          save;     // 4 type, name 2, 1962.
    short          valid;    // 5 type, name 2, 1963.
    short          pad[2];   // 6 type, name 2, 187.
};

// zu DNA structure 105, 301
struct ConsoleLine
{
    ConsoleLine    *next;    // 1 type, name 301, 0.
    ConsoleLine    *prev;    // 2 type, name 301, 1.
    int            len_alloc;// 3 type, name 4, 2023.
    int            len;      // 4 type, name 4, 22.
    char           *line;    // 5 type, name 0, 85.
    int            cursor;   // 6 type, name 4, 2024.
    int            type;     // 7 type, name 4, 16.
};

// zu DNA structure 106, 306
struct uiFont
{
    uiFont    *next;         // 1 type, name 306, 0.
    uiFont    *prev;         // 2 type, name 306, 1.
    char      filename[1024];// 3 type, name 0, 2041.
    short     blf_id;        // 4 type, name 2, 2042.
    short     uifont_id;     // 5 type, name 2, 2043.
    short     r_to_l;        // 6 type, name 2, 2044.
    short     hinting;       // 7 type, name 2, 2045.
};

// zu DNA structure 107, 307
struct uiFontStyle
{
    short    uifont_id;  // 1 type, name 2, 2043.
    short    points;     // 2 type, name 2, 2046.
    short    kerning;    // 3 type, name 2, 2047.
    char     pad[6];     // 4 type, name 0, 752.
    short    italic;     // 5 type, name 2, 2048.
    short    bold;       // 6 type, name 2, 2049.
    short    shadow;     // 7 type, name 2, 2050.
    short    shadx;      // 8 type, name 2, 2051.
    short    shady;      // 9 type, name 2, 2052.
    short    align;      // 10 type, name 2, 1887.
    float    shadowalpha;// 11 type, name 7, 2053.
    float    shadowcolor;// 12 type, name 7, 2054.
};

// zu DNA structure 108, 309
struct uiWidgetColors
{
    char     outline[4];  // 1 type, name 0, 2069.
    char     inner[4];    // 2 type, name 0, 2070.
    char     inner_sel[4];// 3 type, name 0, 2071.
    char     item[4];     // 4 type, name 0, 2072.
    char     text[4];     // 5 type, name 0, 2073.
    char     text_sel[4]; // 6 type, name 0, 2074.
    short    shaded;      // 7 type, name 2, 2075.
    short    shadetop;    // 8 type, name 2, 2076.
    short    shadedown;   // 9 type, name 2, 2077.
    short    alpha_check; // 10 type, name 2, 2078.
};

// zu DNA structure 109, 310
struct uiWidgetStateColors
{
    char     inner_anim[4];      // 1 type, name 0, 2079.
    char     inner_anim_sel[4];  // 2 type, name 0, 2080.
    char     inner_key[4];       // 3 type, name 0, 2081.
    char     inner_key_sel[4];   // 4 type, name 0, 2082.
    char     inner_driven[4];    // 5 type, name 0, 2083.
    char     inner_driven_sel[4];// 6 type, name 0, 2084.
    float    blend;              // 7 type, name 7, 1797.
    float    pad;                // 8 type, name 7, 66.
};

// zu DNA structure 110, 311
struct uiPanelColors
{
    char     header[4];  // 1 type, name 0, 2085.
    char     back[4];    // 2 type, name 0, 2086.
    short    show_header;// 3 type, name 2, 2087.
    short    show_back;  // 4 type, name 2, 2088.
    int      pad;        // 5 type, name 4, 66.
};

// zu DNA structure 111, 312
struct uiGradientColors
{
    char    gradient[4];     // 1 type, name 0, 2089.
    char    high_gradient[4];// 2 type, name 0, 2090.
    int     show_grad;       // 3 type, name 4, 2091.
    int     pad2;            // 4 type, name 4, 29.
};

// zu DNA structure 112, 315
struct ThemeWireColor
{
    char     solid[4]; // 1 type, name 0, 2300.
    char     select[4];// 2 type, name 0, 2146.
    char     active[4];// 3 type, name 0, 2151.
    short    flag;     // 4 type, name 2, 18.
    short    pad;      // 5 type, name 2, 66.
};

// zu DNA structure 113, 317
struct bAddon
{
    bAddon        *next;     // 1 type, name 317, 0.
    bAddon        *prev;     // 2 type, name 317, 1.
    char          module[64];// 3 type, name 0, 2321.
    IDProperty    *prop;     // 4 type, name 21, 2322.
};

// zu DNA structure 114, 318
struct bPathCompare
{
    bPathCompare    *next;    // 1 type, name 318, 0.
    bPathCompare    *prev;    // 2 type, name 318, 1.
    char            path[768];// 3 type, name 0, 2323.
    char            flag;     // 4 type, name 0, 18.
    char            pad[7];   // 5 type, name 0, 115.
};

// zu DNA structure 115, 319
struct SolidLight
{
    int      flag;   // 1 type, name 4, 18.
    int      pad;    // 2 type, name 4, 66.
    float    col[4]; // 3 type, name 7, 692.
    float    spec[4];// 4 type, name 7, 2324.
    float    vec[4]; // 5 type, name 7, 586.
};

// zu DNA structure 116, 320
struct WalkNavigation
{
    float    mouse_speed;      // 1 type, name 7, 2325.
    float    walk_speed;       // 2 type, name 7, 2326.
    float    walk_speed_factor;// 3 type, name 7, 2327.
    float    view_height;      // 4 type, name 7, 2328.
    float    jump_height;      // 5 type, name 7, 2329.
    float    teleport_time;    // 6 type, name 7, 2330.
    short    flag;             // 7 type, name 2, 18.
    short    pad[3];           // 8 type, name 2, 350.
};

// zu DNA structure 117, 324
struct ScrEdge
{
    ScrEdge    *next; // 1 type, name 324, 0.
    ScrEdge    *prev; // 2 type, name 324, 1.
    ScrVert    *v1;   // 3 type, name 323, 2459.
    ScrVert    *v2;   // 4 type, name 323, 2460.
    short      border;// 5 type, name 2, 1505.
    short      flag;  // 6 type, name 2, 18.
    int        pad;   // 7 type, name 4, 66.
};

// zu DNA structure 118, 325
struct Panel
{
    Panel        *next;        // 1 type, name 325, 0.
    Panel        *prev;        // 2 type, name 325, 1.
    void         *type;        // Note: using void* on undefined DNA type: PanelType
    void         *layout;      // Note: using void* on undefined DNA type: uiLayout
    char         panelname[64];// 5 type, name 0, 2462.
    char         tabname[64];  // 6 type, name 0, 2463.
    char         drawname[64]; // 7 type, name 0, 2464.
    int          ofsx;         // 8 type, name 4, 2465.
    int          ofsy;         // 9 type, name 4, 2466.
    int          sizex;        // 10 type, name 4, 2467.
    int          sizey;        // 11 type, name 4, 2468.
    short        labelofs;     // 12 type, name 2, 2469.
    short        pad;          // 13 type, name 2, 66.
    short        flag;         // 14 type, name 2, 18.
    short        runtime_flag; // 15 type, name 2, 2470.
    short        control;      // 16 type, name 2, 2471.
    short        snap;         // 17 type, name 2, 2472.
    int          sortorder;    // 18 type, name 4, 2473.
    Panel        *paneltab;    // 19 type, name 325, 2474.
    void         *activedata;  // 20 type, name 11, 2475.
};

// zu DNA structure 119, 328
struct PanelCategoryStack
{
    PanelCategoryStack    *next;     // 1 type, name 328, 0.
    PanelCategoryStack    *prev;     // 2 type, name 328, 1.
    char                  idname[64];// 3 type, name 0, 2476.
};

// zu DNA structure 120, 329
struct uiList
{
    uiList        *next;            // 1 type, name 329, 0.
    uiList        *prev;            // 2 type, name 329, 1.
    void          *type;            // Note: using void* on undefined DNA type: uiListType
    char          list_id[64];      // 4 type, name 0, 2477.
    int           layout_type;      // 5 type, name 4, 2478.
    int           flag;             // 6 type, name 4, 18.
    int           list_scroll;      // 7 type, name 4, 2479.
    int           list_grip;        // 8 type, name 4, 2480.
    int           list_last_len;    // 9 type, name 4, 2481.
    int           list_last_activei;// 10 type, name 4, 2482.
    char          filter_byname[64];// 11 type, name 0, 2483.
    int           filter_flag;      // 12 type, name 4, 2484.
    int           filter_sort_flag; // 13 type, name 4, 2485.
    IDProperty    *properties;      // 14 type, name 21, 30.
    void          *dyn_data;        // Note: using void* on undefined DNA type: uiListDyn
};

// zu DNA structure 121, 332
struct uiPreview
{
    uiPreview    *next;         // 1 type, name 332, 0.
    uiPreview    *prev;         // 2 type, name 332, 1.
    char         preview_id[64];// 3 type, name 0, 2487.
    short        height;        // 4 type, name 2, 804.
    short        pad1[3];       // 5 type, name 2, 2488.
};

// zu DNA structure 122, 337
struct FileGlobal
{
    char        subvstr[4];            // 1 type, name 0, 2515.
    short       subversion;            // 2 type, name 2, 2516.
    short       minversion;            // 3 type, name 2, 2517.
    short       minsubversion;         // 4 type, name 2, 2518.
    char        pad[6];                // 5 type, name 0, 752.
    bScreen     *curscreen;            // 6 type, name 322, 2519.
    Scene       *curscene;             // 7 type, name 44, 2520.
    int         fileflags;             // 8 type, name 4, 2521.
    int         globalf;               // 9 type, name 4, 2522.
    uint64_t    build_commit_timestamp;// 10 type, name 10, 2523.
    char        build_hash[16];        // 11 type, name 0, 2524.
    char        filename[1024];        // 12 type, name 0, 2041.
};

// zu DNA structure 123, 338
struct StripElem
{
    char    name[256];  // 1 type, name 0, 1961.
    int     orig_width; // 2 type, name 4, 2525.
    int     orig_height;// 3 type, name 4, 2526.
};

// zu DNA structure 124, 339
struct StripCrop
{
    int    top;   // 1 type, name 4, 1977.
    int    bottom;// 2 type, name 4, 2527.
    int    left;  // 3 type, name 4, 1983.
    int    right; // 4 type, name 4, 2528.
};

// zu DNA structure 125, 340
struct StripTransform
{
    int    xofs;// 1 type, name 4, 2529.
    int    yofs;// 2 type, name 4, 2530.
};

// zu DNA structure 126, 341
struct StripColorBalance
{
    float    lift[3]; // 1 type, name 7, 2531.
    float    gamma[3];// 2 type, name 7, 2532.
    float    gain[3]; // 3 type, name 7, 2533.
    int      flag;    // 4 type, name 4, 18.
    int      pad;     // 5 type, name 4, 66.
};

// zu DNA structure 127, 342
struct StripProxy
{
    char     dir[768];        // 1 type, name 0, 2534.
    char     file[256];       // 2 type, name 0, 1937.
    void     *anim;           // Note: using void* on undefined DNA type: anim
    short    tc;              // 4 type, name 2, 2535.
    short    quality;         // 5 type, name 2, 1444.
    short    build_size_flags;// 6 type, name 2, 2536.
    short    build_tc_flags;  // 7 type, name 2, 2537.
    short    build_flags;     // 8 type, name 2, 2538.
    char     pad[6];          // 9 type, name 0, 752.
};

// zu DNA structure 128, 346
struct MetaStack
{
    MetaStack    *next;        // 1 type, name 346, 0.
    MetaStack    *prev;        // 2 type, name 346, 1.
    ListBase     *oldbasep;    // 3 type, name 14, 2577.
    Sequence     *parseq;      // 4 type, name 344, 2578.
    int          disp_range[2];// 5 type, name 4, 2579.
};

// zu DNA structure 129, 347
struct WipeVars
{
    float    edgeWidth;// 1 type, name 7, 2589.
    float    angle;    // 2 type, name 7, 797.
    short    forward;  // 3 type, name 2, 2590.
    short    wipetype; // 4 type, name 2, 2591.
};

// zu DNA structure 130, 348
struct GlowVars
{
    float    fMini;   // 1 type, name 7, 2592.
    float    fClamp;  // 2 type, name 7, 2593.
    float    fBoost;  // 3 type, name 7, 2594.
    float    dDist;   // 4 type, name 7, 2595.
    int      dQuality;// 5 type, name 4, 2596.
    int      bNoComp; // 6 type, name 4, 2597.
};

// zu DNA structure 131, 349
struct TransformVars
{
    float    ScalexIni;    // 1 type, name 7, 2598.
    float    ScaleyIni;    // 2 type, name 7, 2599.
    float    xIni;         // 3 type, name 7, 2600.
    float    yIni;         // 4 type, name 7, 2601.
    float    rotIni;       // 5 type, name 7, 2602.
    int      percent;      // 6 type, name 4, 794.
    int      interpolation;// 7 type, name 4, 2603.
    int      uniform_scale;// 8 type, name 4, 2604.
};

// zu DNA structure 132, 350
struct SolidColorVars
{
    float    col[3];// 1 type, name 7, 1571.
    float    pad;   // 2 type, name 7, 66.
};

// zu DNA structure 133, 351
struct SpeedControlVars
{
    float    *frameMap;     // 1 type, name 7, 2605.
    float    globalSpeed;   // 2 type, name 7, 2606.
    int      flags;         // 3 type, name 4, 89.
    int      length;        // 4 type, name 4, 754.
    int      lastValidFrame;// 5 type, name 4, 2607.
};

// zu DNA structure 134, 352
struct GaussianBlurVars
{
    float    size_x;// 1 type, name 7, 2608.
    float    size_y;// 2 type, name 7, 2609.
};

// zu DNA structure 135, 353
struct SequenceModifierData
{
    SequenceModifierData    *next;          // 1 type, name 353, 0.
    SequenceModifierData    *prev;          // 2 type, name 353, 1.
    int                     type;           // 3 type, name 4, 16.
    int                     flag;           // 4 type, name 4, 18.
    char                    name[64];       // 5 type, name 0, 19.
    int                     mask_input_type;// 6 type, name 4, 2610.
    int                     pad;            // 7 type, name 4, 66.
    Sequence                *mask_sequence; // 8 type, name 344, 2611.
    Mask                    *mask_id;       // 9 type, name 284, 2612.
};

// zu DNA structure 136, 282
struct SequencerScopes
{
    void     *reference_ibuf;   // Note: using void* on undefined DNA type: ImBuf
    void     *zebra_ibuf;       // Note: using void* on undefined DNA type: ImBuf
    void     *waveform_ibuf;    // Note: using void* on undefined DNA type: ImBuf
    void     *sep_waveform_ibuf;// Note: using void* on undefined DNA type: ImBuf
    void     *vector_ibuf;      // Note: using void* on undefined DNA type: ImBuf
    void     *histogram_ibuf;   // Note: using void* on undefined DNA type: ImBuf
};

// zu DNA structure 137, 359
struct Effect
{
    Effect    *next;  // 1 type, name 359, 0.
    Effect    *prev;  // 2 type, name 359, 1.
    short     type;   // 3 type, name 2, 16.
    short     flag;   // 4 type, name 2, 18.
    short     buttype;// 5 type, name 2, 2622.
    short     rt;     // 6 type, name 2, 56.
};

// zu DNA structure 138, 360
struct BuildEff
{
    BuildEff    *next;  // 1 type, name 360, 0.
    BuildEff    *prev;  // 2 type, name 360, 1.
    short       type;   // 3 type, name 2, 16.
    short       flag;   // 4 type, name 2, 18.
    short       buttype;// 5 type, name 2, 2622.
    short       rt;     // 6 type, name 2, 56.
    float       len;    // 7 type, name 7, 22.
    float       sfra;   // 8 type, name 7, 135.
};

// zu DNA structure 139, 361
struct PartEff
{
    PartEff     *next;           // 1 type, name 361, 0.
    PartEff     *prev;           // 2 type, name 361, 1.
    short       type;            // 3 type, name 2, 16.
    short       flag;            // 4 type, name 2, 18.
    short       buttype;         // 5 type, name 2, 2622.
    short       stype;           // 6 type, name 2, 252.
    short       vertgroup;       // 7 type, name 2, 1239.
    short       userjit;         // 8 type, name 2, 2623.
    float       sta;             // 9 type, name 7, 2624.
    float       end;             // 10 type, name 7, 2625.
    float       lifetime;        // 11 type, name 7, 810.
    int         totpart;         // 12 type, name 4, 2626.
    int         totkey;          // 13 type, name 4, 82.
    int         seed;            // 14 type, name 4, 756.
    float       normfac;         // 15 type, name 7, 2627.
    float       obfac;           // 16 type, name 7, 2628.
    float       randfac;         // 17 type, name 7, 2629.
    float       texfac;          // 18 type, name 7, 2630.
    float       randlife;        // 19 type, name 7, 2631.
    float       force[3];        // 20 type, name 7, 2632.
    float       damp;            // 21 type, name 7, 807.
    float       nabla;           // 22 type, name 7, 333.
    float       vectsize;        // 23 type, name 7, 2633.
    float       maxlen;          // 24 type, name 7, 2634.
    float       pad;             // 25 type, name 7, 66.
    float       defvec[3];       // 26 type, name 7, 2635.
    float       mult[4];         // 27 type, name 7, 2636.
    float       life[4];         // 28 type, name 7, 2637.
    short       child[4];        // 29 type, name 2, 2638.
    short       mat[4];          // 30 type, name 2, 2639.
    short       texmap;          // 31 type, name 2, 2640.
    short       curmult;         // 32 type, name 2, 2641.
    short       staticstep;      // 33 type, name 2, 2642.
    short       omat;            // 34 type, name 2, 2643.
    short       timetex;         // 35 type, name 2, 2644.
    short       speedtex;        // 36 type, name 2, 2645.
    short       flag2;           // 37 type, name 2, 566.
    short       flag2neg;        // 38 type, name 2, 2646.
    short       disp;            // 39 type, name 2, 563.
    short       vertgroup_v;     // 40 type, name 2, 2647.
    char        vgroupname[64];  // 41 type, name 0, 2648.
    char        vgroupname_v[64];// 42 type, name 0, 2649.
    float       imat[4][4];      // 43 type, name 7, 250.
    void        *keys;           // Note: using void* on undefined DNA type: Particle
    Group       *group;          // 45 type, name 69, 520.
};

// zu DNA structure 140, 363
struct WaveEff
{
    WaveEff    *next;   // 1 type, name 363, 0.
    WaveEff    *prev;   // 2 type, name 363, 1.
    short      type;    // 3 type, name 2, 16.
    short      flag;    // 4 type, name 2, 18.
    short      buttype; // 5 type, name 2, 2622.
    short      stype;   // 6 type, name 2, 252.
    float      startx;  // 7 type, name 7, 802.
    float      starty;  // 8 type, name 7, 803.
    float      height;  // 9 type, name 7, 804.
    float      width;   // 10 type, name 7, 619.
    float      narrow;  // 11 type, name 7, 805.
    float      speed;   // 12 type, name 7, 806.
    float      minfac;  // 13 type, name 7, 2651.
    float      damp;    // 14 type, name 7, 807.
    float      timeoffs;// 15 type, name 7, 809.
    float      lifetime;// 16 type, name 7, 810.
};

// zu DNA structure 141, 275
struct TreeStoreElem
{
    short    type;// 1 type, name 2, 16.
    short    nr;  // 2 type, name 2, 2652.
    short    flag;// 3 type, name 2, 18.
    short    used;// 4 type, name 2, 2653.
    ID       *id; // 5 type, name 22, 2014.
};

// zu DNA structure 142, 364
struct TreeStore
{
    int              totelem; // 1 type, name 4, 70.
    int              usedelem;// 2 type, name 4, 2654.
    TreeStoreElem    *data;   // 3 type, name 275, 2.
};

// zu DNA structure 143, 365
struct bProperty
{
    bProperty    *next;   // 1 type, name 365, 0.
    bProperty    *prev;   // 2 type, name 365, 1.
    char         name[64];// 3 type, name 0, 19.
    short        type;    // 4 type, name 2, 16.
    short        flag;    // 5 type, name 2, 18.
    int          data;    // 6 type, name 4, 21.
    void         *poin;   // 7 type, name 11, 2655.
};

// zu DNA structure 144, 366
struct bNearSensor
{
    char     name[64]; // 1 type, name 0, 19.
    float    dist;     // 2 type, name 7, 358.
    float    resetdist;// 3 type, name 7, 2656.
    int      lastval;  // 4 type, name 4, 2657.
    int      pad;      // 5 type, name 4, 66.
};

// zu DNA structure 145, 367
struct bMouseSensor
{
    short    type;        // 1 type, name 2, 16.
    short    flag;        // 2 type, name 2, 18.
    short    pad1;        // 3 type, name 2, 68.
    short    mode;        // 4 type, name 2, 351.
    char     propname[64];// 5 type, name 0, 2658.
    char     matname[64]; // 6 type, name 0, 2659.
};

// zu DNA structure 146, 368
struct bTouchSensor
{
    char        name[64];// 1 type, name 0, 19.
    Material    *ma;     // 2 type, name 68, 2660.
    float       dist;    // 3 type, name 7, 358.
    float       pad;     // 4 type, name 7, 66.
};

// zu DNA structure 147, 369
struct bKeyboardSensor
{
    short    key;           // 1 type, name 2, 2661.
    short    qual;          // 2 type, name 2, 2662.
    short    type;          // 3 type, name 2, 16.
    short    qual2;         // 4 type, name 2, 2663.
    char     targetName[64];// 5 type, name 0, 2664.
    char     toggleName[64];// 6 type, name 0, 2665.
};

// zu DNA structure 148, 370
struct bPropertySensor
{
    int     type;        // 1 type, name 4, 16.
    int     pad;         // 2 type, name 4, 66.
    char    name[64];    // 3 type, name 0, 19.
    char    value[64];   // 4 type, name 0, 2666.
    char    maxvalue[64];// 5 type, name 0, 2667.
};

// zu DNA structure 149, 371
struct bActuatorSensor
{
    int     type;    // 1 type, name 4, 16.
    int     pad;     // 2 type, name 4, 66.
    char    name[64];// 3 type, name 0, 19.
};

// zu DNA structure 150, 372
struct bDelaySensor
{
    short    delay;   // 1 type, name 2, 2668.
    short    duration;// 2 type, name 2, 2669.
    short    flag;    // 3 type, name 2, 18.
    short    pad;     // 4 type, name 2, 66.
};

// zu DNA structure 151, 373
struct bCollisionSensor
{
    char     name[64];        // 1 type, name 0, 19.
    char     materialName[64];// 2 type, name 0, 2670.
    short    damptimer;       // 3 type, name 2, 2671.
    short    damp;            // 4 type, name 2, 807.
    short    mode;            // 5 type, name 2, 351.
    short    pad2;            // 6 type, name 2, 29.
};

// zu DNA structure 152, 374
struct bRadarSensor
{
    char     name[64];// 1 type, name 0, 19.
    float    angle;   // 2 type, name 7, 797.
    float    range;   // 3 type, name 7, 1330.
    short    flag;    // 4 type, name 2, 18.
    short    axis;    // 5 type, name 2, 768.
};

// zu DNA structure 153, 375
struct bRandomSensor
{
    char    name[64];// 1 type, name 0, 19.
    int     seed;    // 2 type, name 4, 756.
    int     delay;   // 3 type, name 4, 2668.
};

// zu DNA structure 154, 376
struct bRaySensor
{
    char     name[64];    // 1 type, name 0, 19.
    float    range;       // 2 type, name 7, 1330.
    char     propname[64];// 3 type, name 0, 2658.
    char     matname[64]; // 4 type, name 0, 2659.
    short    mode;        // 5 type, name 2, 351.
    short    pad1;        // 6 type, name 2, 68.
    int      axisflag;    // 7 type, name 4, 2672.
};

// zu DNA structure 155, 377
struct bArmatureSensor
{
    char     posechannel[64];// 1 type, name 0, 2673.
    char     constraint[64]; // 2 type, name 0, 2674.
    int      type;           // 3 type, name 4, 16.
    float    value;          // 4 type, name 7, 772.
};

// zu DNA structure 156, 378
struct bMessageSensor
{
    Object    *fromObject;// 1 type, name 29, 2675.
    char      subject[64];// 2 type, name 0, 2676.
    char      body[64];   // 3 type, name 0, 2677.
};

// zu DNA structure 157, 379
struct bSensor
{
    bSensor        *next;   // 1 type, name 379, 0.
    bSensor        *prev;   // 2 type, name 379, 1.
    short          type;    // 3 type, name 2, 16.
    short          otype;   // 4 type, name 2, 2678.
    short          flag;    // 5 type, name 2, 18.
    short          pulse;   // 6 type, name 2, 2679.
    short          freq;    // 7 type, name 2, 2680.
    short          totlinks;// 8 type, name 2, 2681.
    short          pad1;    // 9 type, name 2, 68.
    short          pad2;    // 10 type, name 2, 29.
    char           name[64];// 11 type, name 0, 19.
    void           *data;   // 12 type, name 11, 2.
    bController    **links; // 13 type, name 380, 2682.
    Object         *ob;     // 14 type, name 29, 44.
    short          invert;  // 15 type, name 2, 1620.
    short          level;   // 16 type, name 2, 717.
    short          tap;     // 17 type, name 2, 2683.
    short          pad;     // 18 type, name 2, 66.
};

// zu DNA structure 158, 381
struct bJoystickSensor
{
    char     name[64];   // 1 type, name 0, 19.
    char     type;       // 2 type, name 0, 16.
    char     joyindex;   // 3 type, name 0, 2684.
    short    flag;       // 4 type, name 2, 18.
    short    axis;       // 5 type, name 2, 768.
    short    axis_single;// 6 type, name 2, 2685.
    int      axisf;      // 7 type, name 4, 2686.
    int      button;     // 8 type, name 4, 2687.
    int      hat;        // 9 type, name 4, 2688.
    int      hatf;       // 10 type, name 4, 2689.
    int      precision;  // 11 type, name 4, 2690.
};

// zu DNA structure 159, 382
struct bExpressionCont
{
    char    str[128];// 1 type, name 0, 2691.
};

// zu DNA structure 160, 383
struct bPythonCont
{
    Text    *text;     // 1 type, name 38, 1976.
    char    module[64];// 2 type, name 0, 2321.
    int     mode;      // 3 type, name 4, 351.
    int     flag;      // 4 type, name 4, 18.
};

// zu DNA structure 161, 380
struct bController
{
    bController    *next;     // 1 type, name 380, 0.
    bController    *prev;     // 2 type, name 380, 1.
    bController    *mynew;    // 3 type, name 380, 2692.
    short          type;      // 4 type, name 2, 16.
    short          flag;      // 5 type, name 2, 18.
    short          inputs;    // 6 type, name 2, 2693.
    short          totlinks;  // 7 type, name 2, 2681.
    short          otype;     // 8 type, name 2, 2678.
    short          totslinks; // 9 type, name 2, 2694.
    short          pad2;      // 10 type, name 2, 29.
    short          pad3;      // 11 type, name 2, 517.
    char           name[64];  // 12 type, name 0, 19.
    void           *data;     // 13 type, name 11, 2.
    bActuator      **links;   // 14 type, name 384, 2682.
    bSensor        **slinks;  // 15 type, name 379, 2695.
    short          val;       // 16 type, name 2, 14.
    short          valo;      // 17 type, name 2, 2696.
    int            state_mask;// 18 type, name 4, 2697.
};

// zu DNA structure 162, 385
struct bAddObjectActuator
{
    int       time;// 1 type, name 4, 784.
    int       pad; // 2 type, name 4, 66.
    Object    *ob; // 3 type, name 29, 44.
};

// zu DNA structure 163, 386
struct bActionActuator
{
    bAction    *act;         // 1 type, name 193, 2698.
    short      type;         // 2 type, name 2, 16.
    short      flag;         // 3 type, name 2, 18.
    float      sta;          // 4 type, name 7, 2624.
    float      end;          // 5 type, name 7, 2625.
    char       name[64];     // 6 type, name 0, 19.
    char       frameProp[64];// 7 type, name 0, 2699.
    short      blendin;      // 8 type, name 2, 2700.
    short      priority;     // 9 type, name 2, 2701.
    short      layer;        // 10 type, name 2, 140.
    short      end_reset;    // 11 type, name 2, 2702.
    short      strideaxis;   // 12 type, name 2, 2703.
    short      blend_mode;   // 13 type, name 2, 2575.
    float      stridelength; // 14 type, name 7, 2704.
    float      layer_weight; // 15 type, name 7, 2705.
};

// zu DNA structure 164, 387
struct Sound3D
{
    float    min_gain;          // 1 type, name 7, 2706.
    float    max_gain;          // 2 type, name 7, 2707.
    float    reference_distance;// 3 type, name 7, 2708.
    float    max_distance;      // 4 type, name 7, 2709.
    float    rolloff_factor;    // 5 type, name 7, 2710.
    float    cone_inner_angle;  // 6 type, name 7, 2711.
    float    cone_outer_angle;  // 7 type, name 7, 2712.
    float    cone_outer_gain;   // 8 type, name 7, 2713.
};

// zu DNA structure 165, 389
struct bEditObjectActuator
{
    int       time;          // 1 type, name 4, 784.
    short     type;          // 2 type, name 2, 16.
    short     flag;          // 3 type, name 2, 18.
    Object    *ob;           // 4 type, name 29, 44.
    Mesh      *me;           // 5 type, name 82, 2718.
    char      name[64];      // 6 type, name 0, 19.
    float     linVelocity[3];// 7 type, name 7, 2719.
    float     angVelocity[3];// 8 type, name 7, 2720.
    float     mass;          // 9 type, name 7, 1069.
    short     localflag;     // 10 type, name 2, 2721.
    short     dyn_operation; // 11 type, name 2, 2722.
    short     upflag;        // 12 type, name 2, 1059.
    short     trackflag;     // 13 type, name 2, 1058.
    int       pad;           // 14 type, name 4, 66.
};

// zu DNA structure 166, 390
struct bSceneActuator
{
    short     type;   // 1 type, name 2, 16.
    short     pad1;   // 2 type, name 2, 68.
    int       pad;    // 3 type, name 4, 66.
    Scene     *scene; // 4 type, name 44, 131.
    Object    *camera;// 5 type, name 29, 1603.
};

// zu DNA structure 167, 391
struct bPropertyActuator
{
    int       pad;      // 1 type, name 4, 66.
    int       type;     // 2 type, name 4, 16.
    char      name[64]; // 3 type, name 0, 19.
    char      value[64];// 4 type, name 0, 2666.
    Object    *ob;      // 5 type, name 29, 44.
};

// zu DNA structure 168, 392
struct bObjectActuator
{
    short     flag;              // 1 type, name 2, 18.
    short     type;              // 2 type, name 2, 16.
    short     otype;             // 3 type, name 2, 2678.
    short     damping;           // 4 type, name 2, 1070.
    float     forceloc[3];       // 5 type, name 7, 2723.
    float     forcerot[3];       // 6 type, name 7, 2724.
    float     pad[3];            // 7 type, name 7, 350.
    float     pad1[3];           // 8 type, name 7, 2488.
    float     dloc[3];           // 9 type, name 7, 1041.
    float     drot[3];           // 10 type, name 7, 1045.
    float     linearvelocity[3]; // 11 type, name 7, 2725.
    float     angularvelocity[3];// 12 type, name 7, 2726.
    Object    *reference;        // 13 type, name 29, 2727.
};

// zu DNA structure 169, 393
struct bIpoActuator
{
    short    flag;         // 1 type, name 2, 18.
    short    type;         // 2 type, name 2, 16.
    float    sta;          // 3 type, name 7, 2624.
    float    end;          // 4 type, name 7, 2625.
    char     name[64];     // 5 type, name 0, 19.
    char     frameProp[64];// 6 type, name 0, 2699.
    short    pad1;         // 7 type, name 2, 68.
    short    pad2;         // 8 type, name 2, 29.
    short    pad3;         // 9 type, name 2, 517.
    short    pad4;         // 10 type, name 2, 1499.
};

// zu DNA structure 170, 394
struct bCameraActuator
{
    Object    *ob;    // 1 type, name 29, 44.
    float     height; // 2 type, name 7, 804.
    float     min;    // 3 type, name 7, 2728.
    float     max;    // 4 type, name 7, 2729.
    float     damping;// 5 type, name 7, 1070.
    short     pad1;   // 6 type, name 2, 68.
    short     axis;   // 7 type, name 2, 768.
    float     pad2;   // 8 type, name 7, 29.
};

// zu DNA structure 171, 395
struct bConstraintActuator
{
    short    type;       // 1 type, name 2, 16.
    short    mode;       // 2 type, name 2, 351.
    short    flag;       // 3 type, name 2, 18.
    short    damp;       // 4 type, name 2, 807.
    short    time;       // 5 type, name 2, 784.
    short    rotdamp;    // 6 type, name 2, 2730.
    int      pad;        // 7 type, name 4, 66.
    float    minloc[3];  // 8 type, name 7, 2731.
    float    maxloc[3];  // 9 type, name 7, 2732.
    float    minrot[3];  // 10 type, name 7, 2733.
    float    maxrot[3];  // 11 type, name 7, 2734.
    char     matprop[64];// 12 type, name 0, 2735.
};

// zu DNA structure 172, 396
struct bGroupActuator
{
    short    flag;    // 1 type, name 2, 18.
    short    type;    // 2 type, name 2, 16.
    int      sta;     // 3 type, name 4, 2624.
    int      end;     // 4 type, name 4, 2625.
    char     name[64];// 5 type, name 0, 19.
    short    pad[3];  // 6 type, name 2, 350.
    short    cur;     // 7 type, name 2, 63.
    short    butsta;  // 8 type, name 2, 2736.
    short    butend;  // 9 type, name 2, 2737.
};

// zu DNA structure 173, 397
struct bRandomActuator
{
    int      seed;        // 1 type, name 4, 756.
    int      distribution;// 2 type, name 4, 2738.
    int      int_arg_1;   // 3 type, name 4, 2739.
    int      int_arg_2;   // 4 type, name 4, 2740.
    float    float_arg_1; // 5 type, name 7, 2741.
    float    float_arg_2; // 6 type, name 7, 2742.
    char     propname[64];// 7 type, name 0, 2658.
};

// zu DNA structure 174, 398
struct bMessageActuator
{
    char      toPropName[64];// 1 type, name 0, 2743.
    Object    *toObject;     // 2 type, name 29, 2744.
    char      subject[64];   // 3 type, name 0, 2676.
    short     bodyType;      // 4 type, name 2, 2745.
    short     pad1;          // 5 type, name 2, 68.
    int       pad2;          // 6 type, name 4, 29.
    char      body[64];      // 7 type, name 0, 2677.
};

// zu DNA structure 175, 399
struct bGameActuator
{
    short    flag;           // 1 type, name 2, 18.
    short    type;           // 2 type, name 2, 16.
    int      sta;            // 3 type, name 4, 2624.
    int      end;            // 4 type, name 4, 2625.
    char     filename[64];   // 5 type, name 0, 2746.
    char     loadaniname[64];// 6 type, name 0, 2747.
};

// zu DNA structure 176, 400
struct bVisibilityActuator
{
    int    flag;// 1 type, name 4, 18.
};

// zu DNA structure 177, 401
struct bTwoDFilterActuator
{
    char     pad[4];   // 1 type, name 0, 585.
    short    type;     // 2 type, name 2, 16.
    short    flag;     // 3 type, name 2, 18.
    int      int_arg;  // 4 type, name 4, 2748.
    float    float_arg;// 5 type, name 7, 2749.
    Text     *text;    // 6 type, name 38, 1976.
};

// zu DNA structure 178, 402
struct bParentActuator
{
    char      pad[2];// 1 type, name 0, 187.
    short     flag;  // 2 type, name 2, 18.
    int       type;  // 3 type, name 4, 16.
    Object    *ob;   // 4 type, name 29, 44.
};

// zu DNA structure 179, 403
struct bStateActuator
{
    int    type;// 1 type, name 4, 16.
    int    mask;// 2 type, name 4, 1877.
};

// zu DNA structure 180, 404
struct bArmatureActuator
{
    char      posechannel[64];// 1 type, name 0, 2673.
    char      constraint[64]; // 2 type, name 0, 2674.
    int       type;           // 3 type, name 4, 16.
    float     weight;         // 4 type, name 7, 574.
    float     influence;      // 5 type, name 7, 2750.
    float     pad;            // 6 type, name 7, 66.
    Object    *target;        // 7 type, name 29, 878.
    Object    *subtarget;     // 8 type, name 29, 2751.
};

// zu DNA structure 181, 405
struct bSteeringActuator
{
    char      pad[5];      // 1 type, name 0, 2752.
    char      flag;        // 2 type, name 0, 18.
    short     facingaxis;  // 3 type, name 2, 2753.
    int       type;        // 4 type, name 4, 16.
    float     dist;        // 5 type, name 7, 358.
    float     velocity;    // 6 type, name 7, 2754.
    float     acceleration;// 7 type, name 7, 2755.
    float     turnspeed;   // 8 type, name 7, 2756.
    int       updateTime;  // 9 type, name 4, 2757.
    Object    *target;     // 10 type, name 29, 878.
    Object    *navmesh;    // 11 type, name 29, 2758.
};

// zu DNA structure 182, 406
struct bMouseActuator
{
    short    type;          // 1 type, name 2, 16.
    short    flag;          // 2 type, name 2, 18.
    int      object_axis[2];// 3 type, name 4, 2759.
    float    threshold[2];  // 4 type, name 7, 2760.
    float    sensitivity[2];// 5 type, name 7, 2761.
    float    limit_x[2];    // 6 type, name 7, 2762.
    float    limit_y[2];    // 7 type, name 7, 2763.
};

// zu DNA structure 183, 384
struct bActuator
{
    bActuator    *next;   // 1 type, name 384, 0.
    bActuator    *prev;   // 2 type, name 384, 1.
    bActuator    *mynew;  // 3 type, name 384, 2692.
    short        type;    // 4 type, name 2, 16.
    short        flag;    // 5 type, name 2, 18.
    short        otype;   // 6 type, name 2, 2678.
    short        go;      // 7 type, name 2, 2764.
    char         name[64];// 8 type, name 0, 19.
    void         *data;   // 9 type, name 11, 2.
    Object       *ob;     // 10 type, name 29, 44.
};

// zu DNA structure 184, 407
struct GroupObject
{
    GroupObject    *next;   // 1 type, name 407, 0.
    GroupObject    *prev;   // 2 type, name 407, 1.
    Object         *ob;     // 3 type, name 29, 44.
    void           *lampren;// 4 type, name 11, 2770.
    short          recalc;  // 5 type, name 2, 257.
    char           pad[6];  // 6 type, name 0, 752.
};

// zu DNA structure 185, 411
struct bMotionPathVert
{
    float    co[3];// 1 type, name 7, 706.
    int      flag; // 2 type, name 4, 18.
};

// zu DNA structure 186, 197
struct bMotionPath
{
    bMotionPathVert    *points;    // 1 type, name 411, 2808.
    int                length;     // 2 type, name 4, 754.
    int                start_frame;// 3 type, name 4, 2809.
    int                end_frame;  // 4 type, name 4, 2810.
    int                flag;       // 5 type, name 4, 18.
};

// zu DNA structure 187, 196
struct bAnimVizSettings
{
    int      ghost_sf;     // 1 type, name 4, 2811.
    int      ghost_ef;     // 2 type, name 4, 2812.
    int      ghost_bc;     // 3 type, name 4, 2813.
    int      ghost_ac;     // 4 type, name 4, 2814.
    short    ghost_type;   // 5 type, name 2, 2815.
    short    ghost_step;   // 6 type, name 2, 2816.
    short    ghost_flag;   // 7 type, name 2, 2817.
    short    recalc;       // 8 type, name 2, 257.
    short    path_type;    // 9 type, name 2, 2818.
    short    path_step;    // 10 type, name 2, 2819.
    short    path_viewflag;// 11 type, name 2, 2820.
    short    path_bakeflag;// 12 type, name 2, 2821.
    int      path_sf;      // 13 type, name 4, 2822.
    int      path_ef;      // 14 type, name 4, 2823.
    int      path_bc;      // 15 type, name 4, 2824.
    int      path_ac;      // 16 type, name 4, 2825.
};

// zu DNA structure 188, 413
struct bIKParam
{
    int    iksolver;// 1 type, name 4, 2856.
};

// zu DNA structure 189, 414
struct bItasc
{
    int      iksolver; // 1 type, name 4, 2856.
    float    precision;// 2 type, name 7, 2690.
    short    numiter;  // 3 type, name 2, 2860.
    short    numstep;  // 4 type, name 2, 2861.
    float    minstep;  // 5 type, name 7, 2862.
    float    maxstep;  // 6 type, name 7, 2863.
    short    solver;   // 7 type, name 2, 2864.
    short    flag;     // 8 type, name 2, 18.
    float    feedback; // 9 type, name 7, 2865.
    float    maxvel;   // 10 type, name 7, 2866.
    float    dampmax;  // 11 type, name 7, 2867.
    float    dampeps;  // 12 type, name 7, 2868.
};

// zu DNA structure 190, 418
struct bConstraintChannel
{
    bConstraintChannel    *next;   // 1 type, name 418, 0.
    bConstraintChannel    *prev;   // 2 type, name 418, 1.
    Ipo                   *ipo;    // 3 type, name 33, 80.
    short                 flag;    // 4 type, name 2, 18.
    char                  name[30];// 5 type, name 0, 2883.
};

// zu DNA structure 191, 419
struct bConstraint
{
    bConstraint    *next;    // 1 type, name 419, 0.
    bConstraint    *prev;    // 2 type, name 419, 1.
    void           *data;    // 3 type, name 11, 2.
    short          type;     // 4 type, name 2, 16.
    short          flag;     // 5 type, name 2, 18.
    char           ownspace; // 6 type, name 0, 2884.
    char           tarspace; // 7 type, name 0, 2885.
    char           name[64]; // 8 type, name 0, 19.
    short          pad;      // 9 type, name 2, 66.
    float          enforce;  // 10 type, name 7, 2886.
    float          headtail; // 11 type, name 7, 2887.
    Ipo            *ipo;     // 12 type, name 33, 80.
    float          lin_error;// 13 type, name 7, 2888.
    float          rot_error;// 14 type, name 7, 2889.
};

// zu DNA structure 192, 420
struct bConstraintTarget
{
    bConstraintTarget    *next;        // 1 type, name 420, 0.
    bConstraintTarget    *prev;        // 2 type, name 420, 1.
    Object               *tar;         // 3 type, name 29, 2890.
    char                 subtarget[64];// 4 type, name 0, 814.
    float                matrix[4][4]; // 5 type, name 7, 2891.
    short                space;        // 6 type, name 2, 2892.
    short                flag;         // 7 type, name 2, 18.
    short                type;         // 8 type, name 2, 16.
    short                rotOrder;     // 9 type, name 2, 2893.
};

// zu DNA structure 193, 422
struct bKinematicConstraint
{
    Object    *tar;             // 1 type, name 29, 2890.
    short     iterations;       // 2 type, name 2, 2896.
    short     flag;             // 3 type, name 2, 18.
    short     rootbone;         // 4 type, name 2, 2897.
    short     max_rootbone;     // 5 type, name 2, 2898.
    char      subtarget[64];    // 6 type, name 0, 814.
    Object    *poletar;         // 7 type, name 29, 2899.
    char      polesubtarget[64];// 8 type, name 0, 2900.
    float     poleangle;        // 9 type, name 7, 2901.
    float     weight;           // 10 type, name 7, 574.
    float     orientweight;     // 11 type, name 7, 2902.
    float     grabtarget[3];    // 12 type, name 7, 2903.
    short     type;             // 13 type, name 2, 16.
    short     mode;             // 14 type, name 2, 351.
    float     dist;             // 15 type, name 7, 358.
};

// zu DNA structure 194, 423
struct bSplineIKConstraint
{
    Object    *tar;        // 1 type, name 29, 2890.
    float     *points;     // 2 type, name 7, 2808.
    short     numpoints;   // 3 type, name 2, 2904.
    short     chainlen;    // 4 type, name 2, 2905.
    short     flag;        // 5 type, name 2, 18.
    short     xzScaleMode; // 6 type, name 2, 2906.
    float     bulge;       // 7 type, name 7, 2907.
    float     bulge_min;   // 8 type, name 7, 2908.
    float     bulge_max;   // 9 type, name 7, 2909.
    float     bulge_smooth;// 10 type, name 7, 2910.
};

// zu DNA structure 195, 424
struct bTrackToConstraint
{
    Object    *tar;         // 1 type, name 29, 2890.
    int       reserved1;    // 2 type, name 4, 2911.
    int       reserved2;    // 3 type, name 4, 2912.
    int       flags;        // 4 type, name 4, 89.
    int       pad;          // 5 type, name 4, 66.
    char      subtarget[64];// 6 type, name 0, 814.
};

// zu DNA structure 196, 425
struct bRotateLikeConstraint
{
    Object    *tar;         // 1 type, name 29, 2890.
    int       flag;         // 2 type, name 4, 18.
    int       reserved1;    // 3 type, name 4, 2911.
    char      subtarget[64];// 4 type, name 0, 814.
};

// zu DNA structure 197, 426
struct bLocateLikeConstraint
{
    Object    *tar;         // 1 type, name 29, 2890.
    int       flag;         // 2 type, name 4, 18.
    int       reserved1;    // 3 type, name 4, 2911.
    char      subtarget[64];// 4 type, name 0, 814.
};

// zu DNA structure 198, 427
struct bSizeLikeConstraint
{
    Object    *tar;         // 1 type, name 29, 2890.
    int       flag;         // 2 type, name 4, 18.
    int       reserved1;    // 3 type, name 4, 2911.
    char      subtarget[64];// 4 type, name 0, 814.
};

// zu DNA structure 199, 428
struct bSameVolumeConstraint
{
    int      flag;  // 1 type, name 4, 18.
    float    volume;// 2 type, name 7, 1197.
};

// zu DNA structure 200, 429
struct bTransLikeConstraint
{
    Object    *tar;         // 1 type, name 29, 2890.
    char      subtarget[64];// 2 type, name 0, 814.
};

// zu DNA structure 201, 430
struct bMinMaxConstraint
{
    Object    *tar;         // 1 type, name 29, 2890.
    int       minmaxflag;   // 2 type, name 4, 2913.
    float     offset;       // 3 type, name 7, 134.
    int       flag;         // 4 type, name 4, 18.
    short     sticky;       // 5 type, name 2, 1973.
    short     stuck;        // 6 type, name 2, 2914.
    short     pad1;         // 7 type, name 2, 68.
    short     pad2;         // 8 type, name 2, 29.
    float     cache[3];     // 9 type, name 7, 2915.
    char      subtarget[64];// 10 type, name 0, 814.
};

// zu DNA structure 202, 431
struct bActionConstraint
{
    Object     *tar;         // 1 type, name 29, 2890.
    short      type;         // 2 type, name 2, 16.
    short      local;        // 3 type, name 2, 1247.
    int        start;        // 4 type, name 4, 753.
    int        end;          // 5 type, name 4, 2625.
    float      min;          // 6 type, name 7, 2728.
    float      max;          // 7 type, name 7, 2729.
    int        flag;         // 8 type, name 4, 18.
    bAction    *act;         // 9 type, name 193, 2698.
    char       subtarget[64];// 10 type, name 0, 814.
};

// zu DNA structure 203, 432
struct bLockTrackConstraint
{
    Object    *tar;         // 1 type, name 29, 2890.
    int       trackflag;    // 2 type, name 4, 1058.
    int       lockflag;     // 3 type, name 4, 2916.
    char      subtarget[64];// 4 type, name 0, 814.
};

// zu DNA structure 204, 433
struct bDampTrackConstraint
{
    Object    *tar;         // 1 type, name 29, 2890.
    int       trackflag;    // 2 type, name 4, 1058.
    int       pad;          // 3 type, name 4, 66.
    char      subtarget[64];// 4 type, name 0, 814.
};

// zu DNA structure 205, 434
struct bFollowPathConstraint
{
    Object    *tar;      // 1 type, name 29, 2890.
    float     offset;    // 2 type, name 7, 134.
    float     offset_fac;// 3 type, name 7, 889.
    int       followflag;// 4 type, name 4, 2917.
    short     trackflag; // 5 type, name 2, 1058.
    short     upflag;    // 6 type, name 2, 1059.
};

// zu DNA structure 206, 435
struct bStretchToConstraint
{
    Object    *tar;         // 1 type, name 29, 2890.
    int       flag;         // 2 type, name 4, 18.
    int       volmode;      // 3 type, name 4, 2918.
    int       plane;        // 4 type, name 4, 2919.
    float     orglength;    // 5 type, name 7, 2920.
    float     bulge;        // 6 type, name 7, 2907.
    float     bulge_min;    // 7 type, name 7, 2908.
    float     bulge_max;    // 8 type, name 7, 2909.
    float     bulge_smooth; // 9 type, name 7, 2910.
    char      subtarget[64];// 10 type, name 0, 814.
};

// zu DNA structure 207, 436
struct bRigidBodyJointConstraint
{
    Object    *tar;       // 1 type, name 29, 2890.
    Object    *child;     // 2 type, name 29, 2832.
    int       type;       // 3 type, name 4, 16.
    float     pivX;       // 4 type, name 7, 2921.
    float     pivY;       // 5 type, name 7, 2922.
    float     pivZ;       // 6 type, name 7, 2923.
    float     axX;        // 7 type, name 7, 2924.
    float     axY;        // 8 type, name 7, 2925.
    float     axZ;        // 9 type, name 7, 2926.
    float     minLimit[6];// 10 type, name 7, 2927.
    float     maxLimit[6];// 11 type, name 7, 2928.
    float     extraFz;    // 12 type, name 7, 2929.
    short     flag;       // 13 type, name 2, 18.
    short     pad;        // 14 type, name 2, 66.
    short     pad1;       // 15 type, name 2, 68.
    short     pad2;       // 16 type, name 2, 29.
};

// zu DNA structure 208, 437
struct bClampToConstraint
{
    Object    *tar; // 1 type, name 29, 2890.
    int       flag; // 2 type, name 4, 18.
    int       flag2;// 3 type, name 4, 566.
};

// zu DNA structure 209, 438
struct bChildOfConstraint
{
    Object    *tar;         // 1 type, name 29, 2890.
    int       flag;         // 2 type, name 4, 18.
    int       pad;          // 3 type, name 4, 66.
    float     invmat[4][4]; // 4 type, name 7, 2930.
    char      subtarget[64];// 5 type, name 0, 814.
};

// zu DNA structure 210, 439
struct bTransformConstraint
{
    Object    *tar;             // 1 type, name 29, 2890.
    char      subtarget[64];    // 2 type, name 0, 814.
    short     from;             // 3 type, name 2, 2931.
    short     to;               // 4 type, name 2, 2932.
    char      map[3];           // 5 type, name 0, 2933.
    char      expo;             // 6 type, name 0, 2934.
    float     from_min[3];      // 7 type, name 7, 2935.
    float     from_max[3];      // 8 type, name 7, 2936.
    float     to_min[3];        // 9 type, name 7, 2937.
    float     to_max[3];        // 10 type, name 7, 2938.
    float     from_min_rot[3];  // 11 type, name 7, 2939.
    float     from_max_rot[3];  // 12 type, name 7, 2940.
    float     to_min_rot[3];    // 13 type, name 7, 2941.
    float     to_max_rot[3];    // 14 type, name 7, 2942.
    float     from_min_scale[3];// 15 type, name 7, 2943.
    float     from_max_scale[3];// 16 type, name 7, 2944.
    float     to_min_scale[3];  // 17 type, name 7, 2945.
    float     to_max_scale[3];  // 18 type, name 7, 2946.
};

// zu DNA structure 211, 440
struct bPivotConstraint
{
    Object    *tar;         // 1 type, name 29, 2890.
    char      subtarget[64];// 2 type, name 0, 814.
    float     offset[3];    // 3 type, name 7, 762.
    short     rotAxis;      // 4 type, name 2, 2947.
    short     flag;         // 5 type, name 2, 18.
};

// zu DNA structure 212, 441
struct bLocLimitConstraint
{
    float    xmin; // 1 type, name 7, 8.
    float    xmax; // 2 type, name 7, 9.
    float    ymin; // 3 type, name 7, 10.
    float    ymax; // 4 type, name 7, 11.
    float    zmin; // 5 type, name 7, 2948.
    float    zmax; // 6 type, name 7, 2949.
    short    flag; // 7 type, name 2, 18.
    short    flag2;// 8 type, name 2, 566.
};

// zu DNA structure 213, 442
struct bRotLimitConstraint
{
    float    xmin; // 1 type, name 7, 8.
    float    xmax; // 2 type, name 7, 9.
    float    ymin; // 3 type, name 7, 10.
    float    ymax; // 4 type, name 7, 11.
    float    zmin; // 5 type, name 7, 2948.
    float    zmax; // 6 type, name 7, 2949.
    short    flag; // 7 type, name 2, 18.
    short    flag2;// 8 type, name 2, 566.
};

// zu DNA structure 214, 443
struct bSizeLimitConstraint
{
    float    xmin; // 1 type, name 7, 8.
    float    xmax; // 2 type, name 7, 9.
    float    ymin; // 3 type, name 7, 10.
    float    ymax; // 4 type, name 7, 11.
    float    zmin; // 5 type, name 7, 2948.
    float    zmax; // 6 type, name 7, 2949.
    short    flag; // 7 type, name 2, 18.
    short    flag2;// 8 type, name 2, 566.
};

// zu DNA structure 215, 444
struct bDistLimitConstraint
{
    Object    *tar;         // 1 type, name 29, 2890.
    char      subtarget[64];// 2 type, name 0, 814.
    float     dist;         // 3 type, name 7, 358.
    float     soft;         // 4 type, name 7, 366.
    short     flag;         // 5 type, name 2, 18.
    short     mode;         // 6 type, name 2, 351.
    int       pad;          // 7 type, name 4, 66.
};

// zu DNA structure 216, 445
struct bShrinkwrapConstraint
{
    Object    *target;      // 1 type, name 29, 878.
    float     dist;         // 2 type, name 7, 358.
    short     shrinkType;   // 3 type, name 2, 882.
    char      projAxis;     // 4 type, name 0, 885.
    char      projAxisSpace;// 5 type, name 0, 2950.
    float     projLimit;    // 6 type, name 7, 884.
    char      pad[4];       // 7 type, name 0, 585.
};

// zu DNA structure 217, 446
struct bFollowTrackConstraint
{
    MovieClip    *clip;       // 1 type, name 257, 1792.
    char         track[64];   // 2 type, name 0, 2951.
    int          flag;        // 3 type, name 4, 18.
    int          frame_method;// 4 type, name 4, 2952.
    char         object[64];  // 5 type, name 0, 2953.
    Object       *camera;     // 6 type, name 29, 1603.
    Object       *depth_ob;   // 7 type, name 29, 2954.
};

// zu DNA structure 218, 447
struct bCameraSolverConstraint
{
    MovieClip    *clip;// 1 type, name 257, 1792.
    int          flag; // 2 type, name 4, 18.
    int          pad;  // 3 type, name 4, 66.
};

// zu DNA structure 219, 448
struct bObjectSolverConstraint
{
    MovieClip    *clip;       // 1 type, name 257, 1792.
    int          flag;        // 2 type, name 4, 18.
    int          pad;         // 3 type, name 4, 66.
    char         object[64];  // 4 type, name 0, 2953.
    float        invmat[4][4];// 5 type, name 7, 2930.
    Object       *camera;     // 6 type, name 29, 1603.
};

// zu DNA structure 220, 449
struct bActionModifier
{
    bActionModifier    *next;      // 1 type, name 449, 0.
    bActionModifier    *prev;      // 2 type, name 449, 1.
    short              type;       // 3 type, name 2, 16.
    short              flag;       // 4 type, name 2, 18.
    char               channel[32];// 5 type, name 0, 2955.
    float              noisesize;  // 6 type, name 7, 296.
    float              turbul;     // 7 type, name 7, 297.
    short              channels;   // 8 type, name 2, 2869.
    short              no_rot_axis;// 9 type, name 2, 2956.
    Object             *ob;        // 10 type, name 29, 44.
};

// zu DNA structure 221, 451
struct bNodeStack
{
    float    vec[4];    // 1 type, name 7, 586.
    float    min;       // 2 type, name 7, 2728.
    float    max;       // 3 type, name 7, 2729.
    void     *data;     // 4 type, name 11, 2.
    short    hasinput;  // 5 type, name 2, 2966.
    short    hasoutput; // 6 type, name 2, 2967.
    short    datatype;  // 7 type, name 2, 2968.
    short    sockettype;// 8 type, name 2, 2969.
    short    is_copy;   // 9 type, name 2, 2970.
    short    external;  // 10 type, name 2, 2971.
    short    pad[2];    // 11 type, name 2, 187.
};

// zu DNA structure 222, 298
struct bNodeInstanceKey
{
    int    value;// 1 type, name 4, 772.
};

// zu DNA structure 223, 454
struct bNodeLink
{
    bNodeLink      *next;    // 1 type, name 454, 0.
    bNodeLink      *prev;    // 2 type, name 454, 1.
    bNode          *fromnode;// 3 type, name 455, 3016.
    bNode          *tonode;  // 4 type, name 455, 3017.
    bNodeSocket    *fromsock;// 5 type, name 452, 3018.
    bNodeSocket    *tosock;  // 6 type, name 452, 3019.
    int            flag;     // 7 type, name 4, 18.
    int            pad;      // 8 type, name 4, 66.
};

// zu DNA structure 224, 464
struct bNodeSocketValueInt
{
    int    subtype;// 1 type, name 4, 17.
    int    value;  // 2 type, name 4, 772.
    int    min;    // 3 type, name 4, 2728.
    int    max;    // 4 type, name 4, 2729.
};

// zu DNA structure 225, 465
struct bNodeSocketValueFloat
{
    int      subtype;// 1 type, name 4, 17.
    float    value;  // 2 type, name 7, 772.
    float    min;    // 3 type, name 7, 2728.
    float    max;    // 4 type, name 7, 2729.
};

// zu DNA structure 226, 466
struct bNodeSocketValueBoolean
{
    char    value; // 1 type, name 0, 772.
    char    pad[3];// 2 type, name 0, 350.
};

// zu DNA structure 227, 467
struct bNodeSocketValueVector
{
    int      subtype; // 1 type, name 4, 17.
    float    value[3];// 2 type, name 7, 3042.
    float    min;     // 3 type, name 7, 2728.
    float    max;     // 4 type, name 7, 2729.
};

// zu DNA structure 228, 468
struct bNodeSocketValueRGBA
{
    float    value[4];// 1 type, name 7, 3043.
};

// zu DNA structure 229, 469
struct bNodeSocketValueString
{
    int     subtype;    // 1 type, name 4, 17.
    int     pad;        // 2 type, name 4, 66.
    char    value[1024];// 3 type, name 0, 3044.
};

// zu DNA structure 230, 470
struct NodeFrame
{
    short    flag;      // 1 type, name 2, 18.
    short    label_size;// 2 type, name 2, 3045.
};

// zu DNA structure 231, 471
struct NodeImageAnim
{
    int      frames;// 1 type, name 4, 133.
    int      sfra;  // 2 type, name 4, 135.
    int      nr;    // 3 type, name 4, 2652.
    char     cyclic;// 4 type, name 0, 3046.
    char     movie; // 5 type, name 0, 3047.
    short    pad;   // 6 type, name 2, 66.
};

// zu DNA structure 232, 472
struct ColorCorrectionData
{
    float    saturation;// 1 type, name 7, 300.
    float    contrast;  // 2 type, name 7, 299.
    float    gamma;     // 3 type, name 7, 3048.
    float    gain;      // 4 type, name 7, 3049.
    float    lift;      // 5 type, name 7, 3050.
    int      pad;       // 6 type, name 4, 66.
};

// zu DNA structure 233, 474
struct NodeBokehImage
{
    float    angle;       // 1 type, name 7, 797.
    int      flaps;       // 2 type, name 4, 3057.
    float    rounding;    // 3 type, name 7, 3058.
    float    catadioptric;// 4 type, name 7, 3059.
    float    lensshift;   // 5 type, name 7, 3060.
};

// zu DNA structure 234, 475
struct NodeBoxMask
{
    float    x;       // 1 type, name 7, 5.
    float    y;       // 2 type, name 7, 6.
    float    rotation;// 3 type, name 7, 1798.
    float    height;  // 4 type, name 7, 804.
    float    width;   // 5 type, name 7, 619.
    int      pad;     // 6 type, name 4, 66.
};

// zu DNA structure 235, 476
struct NodeEllipseMask
{
    float    x;       // 1 type, name 7, 5.
    float    y;       // 2 type, name 7, 6.
    float    rotation;// 3 type, name 7, 1798.
    float    height;  // 4 type, name 7, 804.
    float    width;   // 5 type, name 7, 619.
    int      pad;     // 6 type, name 4, 66.
};

// zu DNA structure 236, 477
struct NodeImageLayer
{
    int    pass_index;// 1 type, name 4, 3061.
    int    pass_flag; // 2 type, name 4, 3062.
};

// zu DNA structure 237, 478
struct NodeBlurData
{
    short    sizex;          // 1 type, name 2, 2467.
    short    sizey;          // 2 type, name 2, 2468.
    short    samples;        // 3 type, name 2, 111.
    short    maxspeed;       // 4 type, name 2, 3063.
    short    minspeed;       // 5 type, name 2, 3064.
    short    relative;       // 6 type, name 2, 69.
    short    aspect;         // 7 type, name 2, 2015.
    short    curved;         // 8 type, name 2, 3065.
    float    fac;            // 9 type, name 7, 799.
    float    percentx;       // 10 type, name 7, 3066.
    float    percenty;       // 11 type, name 7, 3067.
    short    filtertype;     // 12 type, name 2, 373.
    char     bokeh;          // 13 type, name 0, 3068.
    char     gamma;          // 14 type, name 0, 3048.
    int      image_in_width; // 15 type, name 4, 3069.
    int      image_in_height;// 16 type, name 4, 3070.
};

// zu DNA structure 238, 479
struct NodeDBlurData
{
    float    center_x;// 1 type, name 7, 3071.
    float    center_y;// 2 type, name 7, 3072.
    float    distance;// 3 type, name 7, 1017.
    float    angle;   // 4 type, name 7, 797.
    float    spin;    // 5 type, name 7, 3073.
    float    zoom;    // 6 type, name 7, 1927.
    short    iter;    // 7 type, name 2, 795.
    char     wrap;    // 8 type, name 0, 3074.
    char     pad;     // 9 type, name 0, 66.
};

// zu DNA structure 239, 480
struct NodeBilateralBlurData
{
    float    sigma_color;// 1 type, name 7, 3075.
    float    sigma_space;// 2 type, name 7, 3076.
    short    iter;       // 3 type, name 2, 795.
    short    pad;        // 4 type, name 2, 66.
};

// zu DNA structure 240, 481
struct NodeHueSat
{
    float    hue;// 1 type, name 7, 3077.
    float    sat;// 2 type, name 7, 2552.
    float    val;// 3 type, name 7, 14.
};

// zu DNA structure 241, 485
struct NodeChroma
{
    float    t1;       // 1 type, name 7, 3084.
    float    t2;       // 2 type, name 7, 3085.
    float    t3;       // 3 type, name 7, 3086.
    float    fsize;    // 4 type, name 7, 630.
    float    fstrength;// 5 type, name 7, 3087.
    float    falpha;   // 6 type, name 7, 3088.
    float    key[4];   // 7 type, name 7, 3089.
    short    algorithm;// 8 type, name 2, 3090.
    short    channel;  // 9 type, name 2, 3091.
};

// zu DNA structure 242, 486
struct NodeTwoXYs
{
    short    x1;    // 1 type, name 2, 3092.
    short    x2;    // 2 type, name 2, 3093.
    short    y1;    // 3 type, name 2, 3094.
    short    y2;    // 4 type, name 2, 3095.
    float    fac_x1;// 5 type, name 7, 3096.
    float    fac_x2;// 6 type, name 7, 3097.
    float    fac_y1;// 7 type, name 7, 3098.
    float    fac_y2;// 8 type, name 7, 3099.
};

// zu DNA structure 243, 487
struct NodeTwoFloats
{
    float    x;// 1 type, name 7, 5.
    float    y;// 2 type, name 7, 6.
};

// zu DNA structure 244, 488
struct NodeGeometry
{
    char    uvname[64]; // 1 type, name 0, 180.
    char    colname[64];// 2 type, name 0, 3100.
};

// zu DNA structure 245, 489
struct NodeVertexCol
{
    char    name[64];// 1 type, name 0, 19.
};

// zu DNA structure 246, 490
struct NodeDefocus
{
    char     bktype;  // 1 type, name 0, 3101.
    char     pad_c1;  // 2 type, name 0, 3102.
    char     preview; // 3 type, name 0, 1901.
    char     gamco;   // 4 type, name 0, 3103.
    short    samples; // 5 type, name 2, 111.
    short    no_zbuf; // 6 type, name 2, 3104.
    float    fstop;   // 7 type, name 7, 104.
    float    maxblur; // 8 type, name 7, 3105.
    float    bthresh; // 9 type, name 7, 3106.
    float    scale;   // 10 type, name 7, 954.
    float    rotation;// 11 type, name 7, 1798.
    float    pad_f1;  // 12 type, name 7, 3107.
};

// zu DNA structure 247, 491
struct NodeScriptDict
{
    void    *dict;// 1 type, name 11, 3108.
    void    *node;// 2 type, name 11, 3109.
};

// zu DNA structure 248, 492
struct NodeGlare
{
    char     quality;  // 1 type, name 0, 1444.
    char     type;     // 2 type, name 0, 16.
    char     iter;     // 3 type, name 0, 795.
    char     angle;    // 4 type, name 0, 797.
    char     pad_c1;   // 5 type, name 0, 3102.
    char     size;     // 6 type, name 0, 101.
    char     pad[2];   // 7 type, name 0, 187.
    float    colmod;   // 8 type, name 7, 3110.
    float    mix;      // 9 type, name 7, 3111.
    float    threshold;// 10 type, name 7, 953.
    float    fade;     // 11 type, name 7, 3112.
    float    angle_ofs;// 12 type, name 7, 3113.
    float    pad_f1;   // 13 type, name 7, 3107.
};

// zu DNA structure 249, 493
struct NodeTonemap
{
    float    key;   // 1 type, name 7, 2661.
    float    offset;// 2 type, name 7, 134.
    float    gamma; // 3 type, name 7, 3048.
    float    f;     // 4 type, name 7, 712.
    float    m;     // 5 type, name 7, 3114.
    float    a;     // 6 type, name 7, 241.
    float    c;     // 7 type, name 7, 3115.
    int      type;  // 8 type, name 4, 16.
};

// zu DNA structure 250, 494
struct NodeLensDist
{
    short    jit; // 1 type, name 2, 3116.
    short    proj;// 2 type, name 2, 3117.
    short    fit; // 3 type, name 2, 3118.
    short    pad; // 4 type, name 2, 66.
};

// zu DNA structure 251, 495
struct NodeColorBalance
{
    float    slope[3]; // 1 type, name 7, 3119.
    float    offset[3];// 2 type, name 7, 762.
    float    power[3]; // 3 type, name 7, 3120.
    float    lift[3];  // 4 type, name 7, 2531.
    float    gamma[3]; // 5 type, name 7, 2532.
    float    gain[3];  // 6 type, name 7, 2533.
};

// zu DNA structure 252, 496
struct NodeColorspill
{
    short    limchan; // 1 type, name 2, 3121.
    short    unspill; // 2 type, name 2, 3122.
    float    limscale;// 3 type, name 7, 3123.
    float    uspillr; // 4 type, name 7, 3124.
    float    uspillg; // 5 type, name 7, 3125.
    float    uspillb; // 6 type, name 7, 3126.
};

// zu DNA structure 253, 497
struct NodeDilateErode
{
    char    falloff;// 1 type, name 0, 808.
    char    pad[7]; // 2 type, name 0, 115.
};

// zu DNA structure 254, 498
struct NodeMask
{
    int    size_x;// 1 type, name 4, 2608.
    int    size_y;// 2 type, name 4, 2609.
};

// zu DNA structure 255, 511
struct NodeShaderAttribute
{
    char    name[64];// 1 type, name 0, 19.
};

// zu DNA structure 256, 512
struct NodeShaderVectTransform
{
    int    type;        // 1 type, name 4, 16.
    int    convert_from;// 2 type, name 4, 3143.
    int    convert_to;  // 3 type, name 4, 3144.
    int    pad;         // 4 type, name 4, 66.
};

// zu DNA structure 257, 513
struct TexNodeOutput
{
    char    name[64];// 1 type, name 0, 19.
};

// zu DNA structure 258, 514
struct NodeKeyingScreenData
{
    char    tracking_object[64];// 1 type, name 0, 3145.
};

// zu DNA structure 259, 515
struct NodeKeyingData
{
    float    screen_balance;       // 1 type, name 7, 3146.
    float    despill_factor;       // 2 type, name 7, 3147.
    float    despill_balance;      // 3 type, name 7, 3148.
    int      edge_kernel_radius;   // 4 type, name 4, 3149.
    float    edge_kernel_tolerance;// 5 type, name 7, 3150.
    float    clip_black;           // 6 type, name 7, 3151.
    float    clip_white;           // 7 type, name 7, 3152.
    int      dilate_distance;      // 8 type, name 4, 3153.
    int      feather_distance;     // 9 type, name 4, 3154.
    int      feather_falloff;      // 10 type, name 4, 3155.
    int      blur_pre;             // 11 type, name 4, 3156.
    int      blur_post;            // 12 type, name 4, 3157.
};

// zu DNA structure 260, 516
struct NodeTrackPosData
{
    char    tracking_object[64];// 1 type, name 0, 3145.
    char    track_name[64];     // 2 type, name 0, 3158.
};

// zu DNA structure 261, 517
struct NodeTranslateData
{
    char    wrap_axis;// 1 type, name 0, 3159.
    char    relative; // 2 type, name 0, 69.
    char    pad[6];   // 3 type, name 0, 752.
};

// zu DNA structure 262, 518
struct NodePlaneTrackDeformData
{
    char     tracking_object[64]; // 1 type, name 0, 3145.
    char     plane_track_name[64];// 2 type, name 0, 3160.
    char     flag;                // 3 type, name 0, 18.
    char     motion_blur_samples; // 4 type, name 0, 3161.
    char     pad[2];              // 5 type, name 0, 187.
    float    motion_blur_shutter; // 6 type, name 7, 3162.
};

// zu DNA structure 263, 519
struct NodeShaderScript
{
    int     mode;             // 1 type, name 4, 351.
    int     flag;             // 2 type, name 4, 18.
    char    filepath[1024];   // 3 type, name 0, 35.
    char    bytecode_hash[64];// 4 type, name 0, 3163.
    char    *bytecode;        // 5 type, name 0, 3164.
};

// zu DNA structure 264, 520
struct NodeShaderTangent
{
    int     direction_type;// 1 type, name 4, 3165.
    int     axis;          // 2 type, name 4, 768.
    char    uv_map[64];    // 3 type, name 0, 3166.
};

// zu DNA structure 265, 521
struct NodeShaderNormalMap
{
    int     space;     // 1 type, name 4, 2892.
    char    uv_map[64];// 2 type, name 0, 3166.
};

// zu DNA structure 266, 522
struct NodeShaderUVMap
{
    char    uv_map[64];// 1 type, name 0, 3166.
};

// zu DNA structure 267, 523
struct NodeSunBeams
{
    float    source[2]; // 1 type, name 7, 3167.
    float    ray_length;// 2 type, name 7, 3168.
};

// zu DNA structure 268, 524
struct CurveMapPoint
{
    float    x;     // 1 type, name 7, 5.
    float    y;     // 2 type, name 7, 6.
    short    flag;  // 3 type, name 2, 18.
    short    shorty;// 4 type, name 2, 3169.
};

// zu DNA structure 269, 525
struct CurveMap
{
    short            totpoint;         // 1 type, name 2, 1176.
    short            flag;             // 2 type, name 2, 18.
    float            range;            // 3 type, name 7, 1330.
    float            mintable;         // 4 type, name 7, 3170.
    float            maxtable;         // 5 type, name 7, 3171.
    float            ext_in[2];        // 6 type, name 7, 3172.
    float            ext_out[2];       // 7 type, name 7, 3173.
    CurveMapPoint    *curve;           // 8 type, name 524, 3174.
    CurveMapPoint    *table;           // 9 type, name 524, 3175.
    CurveMapPoint    *premultable;     // 10 type, name 524, 3176.
    float            premul_ext_in[2]; // 11 type, name 7, 3177.
    float            premul_ext_out[2];// 12 type, name 7, 3178.
};

// zu DNA structure 270, 293
struct Histogram
{
    int      channels;      // 1 type, name 4, 2869.
    int      x_resolution;  // 2 type, name 4, 3188.
    float    data_luma[256];// 3 type, name 7, 3189.
    float    data_r[256];   // 4 type, name 7, 3190.
    float    data_g[256];   // 5 type, name 7, 3191.
    float    data_b[256];   // 6 type, name 7, 3192.
    float    data_a[256];   // 7 type, name 7, 3193.
    float    xmax;          // 8 type, name 7, 9.
    float    ymax;          // 9 type, name 7, 11.
    short    mode;          // 10 type, name 2, 351.
    short    flag;          // 11 type, name 2, 18.
    int      height;        // 12 type, name 4, 804.
    float    co[2][2];      // 13 type, name 7, 3194.
};

// zu DNA structure 271, 226
struct ColorManagedViewSettings
{
    int             flag;              // 1 type, name 4, 18.
    int             pad;               // 2 type, name 4, 66.
    char            look[64];          // 3 type, name 0, 3211.
    char            view_transform[64];// 4 type, name 0, 3212.
    float           exposure;          // 5 type, name 7, 1328.
    float           gamma;             // 6 type, name 7, 3048.
    CurveMapping    *curve_mapping;    // 7 type, name 58, 3213.
    void            *pad2;             // 8 type, name 11, 1637.
};

// zu DNA structure 272, 227
struct ColorManagedDisplaySettings
{
    char    display_device[64];// 1 type, name 0, 3214.
};

// zu DNA structure 273, 50
struct ColorManagedColorspaceSettings
{
    char    name[64];// 1 type, name 0, 19.
};

// zu DNA structure 274, 526
struct BrushClone
{
    Image    *image;   // 1 type, name 46, 788.
    float    offset[2];// 2 type, name 7, 3215.
    float    alpha;    // 3 type, name 7, 446.
    float    pad;      // 4 type, name 7, 66.
};

// zu DNA structure 275, 528
struct PaletteColor
{
    PaletteColor    *next; // 1 type, name 528, 0.
    PaletteColor    *prev; // 2 type, name 528, 1.
    float           rgb[3];// 3 type, name 7, 1642.
    float           value; // 4 type, name 7, 772.
};

// zu DNA structure 276, 530
struct CustomDataLayer
{
    int     type;        // 1 type, name 4, 16.
    int     offset;      // 2 type, name 4, 134.
    int     flag;        // 3 type, name 4, 18.
    int     active;      // 4 type, name 4, 1128.
    int     active_rnd;  // 5 type, name 4, 3266.
    int     active_clone;// 6 type, name 4, 3267.
    int     active_mask; // 7 type, name 4, 3268.
    int     uid;         // 8 type, name 4, 71.
    char    name[64];    // 9 type, name 0, 19.
    void    *data;       // 10 type, name 11, 2.
};

// zu DNA structure 277, 531
struct CustomDataExternal
{
    char    filename[1024];// 1 type, name 0, 2041.
};

// zu DNA structure 278, 97
struct CustomData
{
    CustomDataLayer       *layers;    // 1 type, name 530, 3269.
    int                   typemap[42];// 2 type, name 4, 3270.
    int                   pad_i1;     // 3 type, name 4, 937.
    int                   totlayer;   // 4 type, name 4, 3271.
    int                   maxlayer;   // 5 type, name 4, 3272.
    int                   totsize;    // 6 type, name 4, 3273.
    void                  *pool;      // Note: using void* on undefined DNA type: BLI_mempool
    CustomDataExternal    *external;  // 8 type, name 531, 3275.
};

// zu DNA structure 279, 532
struct HairKey
{
    float    co[3];      // 1 type, name 7, 706.
    float    time;       // 2 type, name 7, 784.
    float    weight;     // 3 type, name 7, 574.
    short    editflag;   // 4 type, name 2, 688.
    short    pad;        // 5 type, name 2, 66.
    float    world_co[3];// 6 type, name 7, 3276.
};

// zu DNA structure 280, 533
struct ParticleKey
{
    float    co[3]; // 1 type, name 7, 706.
    float    vel[3];// 2 type, name 7, 1272.
    float    rot[4];// 3 type, name 7, 3277.
    float    ave[3];// 4 type, name 7, 3278.
    float    time;  // 5 type, name 7, 784.
};

// zu DNA structure 281, 536
struct ParticleSpring
{
    float    rest_length;      // 1 type, name 7, 3281.
    int      particle_index[2];// 2 type, name 4, 3282.
    int      delete_flag;      // 3 type, name 4, 3283.
};

// zu DNA structure 282, 537
struct ChildParticle
{
    int      num;    // 1 type, name 4, 3284.
    int      parent; // 2 type, name 4, 3285.
    int      pa[4];  // 3 type, name 4, 3286.
    float    w[4];   // 4 type, name 7, 3287.
    float    fuv[4]; // 5 type, name 7, 3288.
    float    foffset;// 6 type, name 7, 3289.
    float    rt;     // 7 type, name 7, 56.
};

// zu DNA structure 283, 538
struct ParticleTarget
{
    ParticleTarget    *next;   // 1 type, name 538, 0.
    ParticleTarget    *prev;   // 2 type, name 538, 1.
    Object            *ob;     // 3 type, name 29, 44.
    int               psys;    // 4 type, name 4, 265.
    short             flag;    // 5 type, name 2, 18.
    short             mode;    // 6 type, name 2, 351.
    float             time;    // 7 type, name 7, 784.
    float             duration;// 8 type, name 7, 2669.
};

// zu DNA structure 284, 539
struct ParticleDupliWeight
{
    ParticleDupliWeight    *next;// 1 type, name 539, 0.
    ParticleDupliWeight    *prev;// 2 type, name 539, 1.
    Object                 *ob;  // 3 type, name 29, 44.
    short                  count;// 4 type, name 2, 767.
    short                  flag; // 5 type, name 2, 18.
    short                  index;// 6 type, name 2, 430.
    short                  rt;   // 7 type, name 2, 56.
};

// zu DNA structure 285, 541
struct SPHFluidSettings
{
    float    radius;             // 1 type, name 7, 261.
    float    spring_k;           // 2 type, name 7, 3298.
    float    rest_length;        // 3 type, name 7, 3281.
    float    plasticity_constant;// 4 type, name 7, 3299.
    float    yield_ratio;        // 5 type, name 7, 3300.
    float    plasticity_balance; // 6 type, name 7, 3301.
    float    yield_balance;      // 7 type, name 7, 3302.
    float    viscosity_omega;    // 8 type, name 7, 3303.
    float    viscosity_beta;     // 9 type, name 7, 3304.
    float    stiffness_k;        // 10 type, name 7, 3305.
    float    stiffness_knear;    // 11 type, name 7, 3306.
    float    rest_density;       // 12 type, name 7, 3307.
    float    buoyancy;           // 13 type, name 7, 3308.
    int      flag;               // 14 type, name 4, 18.
    int      spring_frames;      // 15 type, name 4, 3309.
    short    solver;             // 16 type, name 2, 2864.
    short    pad[3];             // 17 type, name 2, 350.
};

// zu DNA structure 286, 142
struct ClothSimSettings
{
    void               *cache;           // Note: using void* on undefined DNA type: LinkNode
    float              mingoal;          // 2 type, name 7, 1236.
    float              Cdis;             // 3 type, name 7, 3445.
    float              Cvi;              // 4 type, name 7, 3446.
    float              gravity[3];       // 5 type, name 7, 1756.
    float              dt;               // 6 type, name 7, 1086.
    float              mass;             // 7 type, name 7, 1069.
    float              structural;       // 8 type, name 7, 3447.
    float              shear;            // 9 type, name 7, 629.
    float              bending;          // 10 type, name 7, 3448.
    float              max_bend;         // 11 type, name 7, 3449.
    float              max_struct;       // 12 type, name 7, 3450.
    float              max_shear;        // 13 type, name 7, 3451.
    float              max_sewing;       // 14 type, name 7, 3452.
    float              avg_spring_len;   // 15 type, name 7, 3453.
    float              timescale;        // 16 type, name 7, 3454.
    float              maxgoal;          // 17 type, name 7, 1237.
    float              eff_force_scale;  // 18 type, name 7, 3455.
    float              eff_wind_scale;   // 19 type, name 7, 3456.
    float              sim_time_old;     // 20 type, name 7, 3457.
    float              defgoal;          // 21 type, name 7, 1238.
    float              goalspring;       // 22 type, name 7, 1234.
    float              goalfrict;        // 23 type, name 7, 1235.
    float              velocity_smooth;  // 24 type, name 7, 3458.
    float              density_target;   // 25 type, name 7, 3459.
    float              density_strength; // 26 type, name 7, 3460.
    float              collider_friction;// 27 type, name 7, 3461.
    float              vel_damping;      // 28 type, name 7, 3462.
    float              shrink_min;       // 29 type, name 7, 3463.
    float              shrink_max;       // 30 type, name 7, 3464.
    float              bending_damping;  // 31 type, name 7, 3465.
    float              voxel_cell_size;  // 32 type, name 7, 3466.
    int                pad;              // 33 type, name 4, 66.
    int                stepsPerFrame;    // 34 type, name 4, 3467.
    int                flags;            // 35 type, name 4, 89.
    int                preroll;          // 36 type, name 4, 3468.
    int                maxspringlen;     // 37 type, name 4, 3469.
    short              solver_type;      // 38 type, name 2, 3470.
    short              vgroup_bend;      // 39 type, name 2, 3471.
    short              vgroup_mass;      // 40 type, name 2, 3472.
    short              vgroup_struct;    // 41 type, name 2, 3473.
    short              vgroup_shrink;    // 42 type, name 2, 3474.
    short              shapekey_rest;    // 43 type, name 2, 3475.
    short              presets;          // 44 type, name 2, 3476.
    short              reset;            // 45 type, name 2, 3477.
    EffectorWeights    *effector_weights;// 46 type, name 207, 1267.
};

// zu DNA structure 287, 143
struct ClothCollSettings
{
    void        *collision_list;// Note: using void* on undefined DNA type: LinkNode
    float       epsilon;        // 2 type, name 7, 3479.
    float       self_friction;  // 3 type, name 7, 3480.
    float       friction;       // 4 type, name 7, 521.
    float       damping;        // 5 type, name 7, 1070.
    float       selfepsilon;    // 6 type, name 7, 3481.
    float       repel_force;    // 7 type, name 7, 3482.
    float       distance_repel; // 8 type, name 7, 3483.
    int         flags;          // 9 type, name 4, 89.
    short       self_loop_count;// 10 type, name 2, 3484.
    short       loop_count;     // 11 type, name 2, 3485.
    int         pad;            // 12 type, name 4, 66.
    Group       *group;         // 13 type, name 69, 520.
    short       vgroup_selfcol; // 14 type, name 2, 3486.
    short       pad2[3];        // 15 type, name 2, 3487.
};

// zu DNA structure 288, 550
struct bGPDspoint
{
    float    x;       // 1 type, name 7, 5.
    float    y;       // 2 type, name 7, 6.
    float    z;       // 3 type, name 7, 7.
    float    pressure;// 4 type, name 7, 3263.
    float    time;    // 5 type, name 7, 784.
    int      flag;    // 6 type, name 4, 18.
};

// zu DNA structure 289, 551
struct bGPDstroke
{
    bGPDstroke    *next;    // 1 type, name 551, 0.
    bGPDstroke    *prev;    // 2 type, name 551, 1.
    bGPDspoint    *points;  // 3 type, name 550, 2808.
    void          *pad;     // 4 type, name 11, 1396.
    int           totpoints;// 5 type, name 4, 263.
    short         thickness;// 6 type, name 2, 3488.
    short         flag;     // 7 type, name 2, 18.
    double        inittime; // 8 type, name 8, 3489.
};

// zu DNA structure 290, 562
struct wmKeyMapItem
{
    wmKeyMapItem    *next;            // 1 type, name 562, 0.
    wmKeyMapItem    *prev;            // 2 type, name 562, 1.
    char            idname[64];       // 3 type, name 0, 2476.
    IDProperty      *properties;      // 4 type, name 21, 30.
    char            propvalue_str[64];// 5 type, name 0, 3552.
    short           propvalue;        // 6 type, name 2, 3553.
    short           type;             // 7 type, name 2, 16.
    short           val;              // 8 type, name 2, 14.
    short           shift;            // 9 type, name 2, 3554.
    short           ctrl;             // 10 type, name 2, 3555.
    short           alt;              // 11 type, name 2, 3556.
    short           oskey;            // 12 type, name 2, 3557.
    short           keymodifier;      // 13 type, name 2, 3558.
    short           flag;             // 14 type, name 2, 18.
    short           maptype;          // 15 type, name 2, 3559.
    short           id;               // 16 type, name 2, 31.
    short           pad;              // 17 type, name 2, 66.
    void            *ptr;             // Note: using void* on undefined DNA type: PointerRNA
};

// zu DNA structure 291, 564
struct wmKeyMapDiffItem
{
    wmKeyMapDiffItem    *next;       // 1 type, name 564, 0.
    wmKeyMapDiffItem    *prev;       // 2 type, name 564, 1.
    wmKeyMapItem        *remove_item;// 3 type, name 562, 3561.
    wmKeyMapItem        *add_item;   // 4 type, name 562, 3562.
};

// zu DNA structure 292, 567
struct FModifier
{
    FModifier    *next;    // 1 type, name 567, 0.
    FModifier    *prev;    // 2 type, name 567, 1.
    void         *data;    // 3 type, name 11, 2.
    char         name[64]; // 4 type, name 0, 19.
    short        type;     // 5 type, name 2, 16.
    short        flag;     // 6 type, name 2, 18.
    float        influence;// 7 type, name 7, 2750.
    float        sfra;     // 8 type, name 7, 135.
    float        efra;     // 9 type, name 7, 1245.
    float        blendin;  // 10 type, name 7, 2700.
    float        blendout; // 11 type, name 7, 2963.
};

// zu DNA structure 293, 568
struct FMod_Generator
{
    float    *coefficients;// 1 type, name 7, 3577.
    int      arraysize;    // 2 type, name 4, 3578.
    int      poly_order;   // 3 type, name 4, 3579.
    int      mode;         // 4 type, name 4, 351.
    int      flag;         // 5 type, name 4, 18.
};

// zu DNA structure 294, 569
struct FMod_FunctionGenerator
{
    float    amplitude;       // 1 type, name 7, 583.
    float    phase_multiplier;// 2 type, name 7, 3580.
    float    phase_offset;    // 3 type, name 7, 3581.
    float    value_offset;    // 4 type, name 7, 3582.
    int      type;            // 5 type, name 4, 16.
    int      flag;            // 6 type, name 4, 18.
};

// zu DNA structure 295, 570
struct FCM_EnvelopeData
{
    float    min; // 1 type, name 7, 2728.
    float    max; // 2 type, name 7, 2729.
    float    time;// 3 type, name 7, 784.
    short    f1;  // 4 type, name 2, 577.
    short    f2;  // 5 type, name 2, 578.
};

// zu DNA structure 296, 571
struct FMod_Envelope
{
    FCM_EnvelopeData    *data;  // 1 type, name 570, 2.
    int                 totvert;// 2 type, name 4, 53.
    float               midval; // 3 type, name 7, 3583.
    float               min;    // 4 type, name 7, 2728.
    float               max;    // 5 type, name 7, 2729.
};

// zu DNA structure 297, 572
struct FMod_Cycles
{
    short    before_mode;  // 1 type, name 2, 3584.
    short    after_mode;   // 2 type, name 2, 3585.
    short    before_cycles;// 3 type, name 2, 3586.
    short    after_cycles; // 4 type, name 2, 3587.
};

// zu DNA structure 298, 573
struct FMod_Python
{
    Text          *script;// 1 type, name 38, 2009.
    IDProperty    *prop;  // 2 type, name 21, 2322.
};

// zu DNA structure 299, 575
struct FMod_Noise
{
    float    size;        // 1 type, name 7, 101.
    float    strength;    // 2 type, name 7, 750.
    float    phase;       // 3 type, name 7, 3589.
    float    offset;      // 4 type, name 7, 134.
    short    depth;       // 5 type, name 2, 256.
    short    modification;// 6 type, name 2, 3590.
};

// zu DNA structure 300, 576
struct FMod_Stepped
{
    float    step_size;  // 1 type, name 7, 3591.
    float    offset;     // 2 type, name 7, 134.
    float    start_frame;// 3 type, name 7, 2809.
    float    end_frame;  // 4 type, name 7, 2810.
    int      flag;       // 5 type, name 4, 18.
};

// zu DNA structure 301, 577
struct DriverTarget
{
    ID       *id;           // 1 type, name 22, 2014.
    char     *rna_path;     // 2 type, name 0, 3592.
    char     pchan_name[32];// 3 type, name 0, 3593.
    short    transChan;     // 4 type, name 2, 3594.
    short    flag;          // 5 type, name 2, 18.
    int      idtype;        // 6 type, name 4, 3595.
};

// zu DNA structure 302, 580
struct FPoint
{
    float    vec[2];// 1 type, name 7, 3601.
    int      flag;  // 2 type, name 4, 18.
    int      pad;   // 3 type, name 4, 66.
};

// zu DNA structure 303, 582
struct AnimMapPair
{
    char    from[128];// 1 type, name 0, 3605.
    char    to[128];  // 2 type, name 0, 3606.
};

// zu DNA structure 304, 586
struct KS_Path
{
    KS_Path    *next;         // 1 type, name 586, 0.
    KS_Path    *prev;         // 2 type, name 586, 1.
    ID         *id;           // 3 type, name 22, 2014.
    char       group[64];     // 4 type, name 0, 3615.
    int        idtype;        // 5 type, name 4, 3595.
    short      groupmode;     // 6 type, name 2, 3616.
    short      flag;          // 7 type, name 2, 18.
    char       *rna_path;     // 8 type, name 0, 3592.
    int        array_index;   // 9 type, name 4, 3603.
    short      keyingflag;    // 10 type, name 2, 3617.
    short      keyingoverride;// 11 type, name 2, 3618.
};

// zu DNA structure 305, 588
struct AnimOverride
{
    AnimOverride    *next;      // 1 type, name 588, 0.
    AnimOverride    *prev;      // 2 type, name 588, 1.
    char            *rna_path;  // 3 type, name 0, 3592.
    int             array_index;// 4 type, name 4, 3603.
    float           value;      // 5 type, name 7, 772.
};

// zu DNA structure 306, 590
struct BoidRule
{
    BoidRule    *next;   // 1 type, name 590, 0.
    BoidRule    *prev;   // 2 type, name 590, 1.
    int         type;    // 3 type, name 4, 16.
    int         flag;    // 4 type, name 4, 18.
    char        name[32];// 5 type, name 0, 1563.
};

// zu DNA structure 307, 535
struct BoidData
{
    float    health;  // 1 type, name 7, 3640.
    float    acc[3];  // 2 type, name 7, 3365.
    short    state_id;// 3 type, name 2, 3641.
    short    mode;    // 4 type, name 2, 351.
};

// zu DNA structure 308, 129
struct SmokeFlowSettings
{
    SmokeModifierData    *smd;            // 1 type, name 127, 3663.
    void                 *dm;             // Note: using void* on undefined DNA type: DerivedMesh
    ParticleSystem       *psys;           // 3 type, name 157, 864.
    Tex                  *noise_texture;  // 4 type, name 52, 3715.
    float                *verts_old;      // 5 type, name 7, 3716.
    int                  numverts;        // 6 type, name 4, 838.
    float                vel_multi;       // 7 type, name 7, 3717.
    float                vel_normal;      // 8 type, name 7, 3718.
    float                vel_random;      // 9 type, name 7, 3719.
    float                density;         // 10 type, name 7, 409.
    float                color[3];        // 11 type, name 7, 108.
    float                fuel_amount;     // 12 type, name 7, 3720.
    float                temp;            // 13 type, name 7, 2445.
    float                volume_density;  // 14 type, name 7, 3721.
    float                surface_distance;// 15 type, name 7, 3722.
    float                particle_size;   // 16 type, name 7, 3723.
    int                  subframes;       // 17 type, name 4, 3321.
    float                texture_size;    // 18 type, name 7, 3724.
    float                texture_offset;  // 19 type, name 7, 3725.
    int                  pad;             // 20 type, name 4, 66.
    char                 uvlayer_name[64];// 21 type, name 0, 743.
    short                vgroup_density;  // 22 type, name 2, 3726.
    short                type;            // 23 type, name 2, 16.
    short                source;          // 24 type, name 2, 149.
    short                texture_type;    // 25 type, name 2, 3727.
    int                  flags;           // 26 type, name 4, 89.
};

// zu DNA structure 309, 130
struct SmokeCollSettings
{
    SmokeModifierData    *smd;      // 1 type, name 127, 3663.
    void                 *dm;       // Note: using void* on undefined DNA type: DerivedMesh
    float                *verts_old;// 3 type, name 7, 3716.
    int                  numverts;  // 4 type, name 4, 838.
    short                type;      // 5 type, name 2, 16.
    short                pad;       // 6 type, name 2, 66.
};

// zu DNA structure 310, 260
struct MovieClipUser
{
    int      framenr;    // 1 type, name 4, 132.
    short    render_size;// 2 type, name 2, 1924.
    short    render_flag;// 3 type, name 2, 3734.
};

// zu DNA structure 311, 600
struct MovieClipProxy
{
    char     dir[768];       // 1 type, name 0, 2534.
    short    tc;             // 2 type, name 2, 2535.
    short    quality;        // 3 type, name 2, 1444.
    short    build_size_flag;// 4 type, name 2, 3735.
    short    build_tc_flag;  // 5 type, name 2, 3736.
};

// zu DNA structure 312, 605
struct MovieReconstructedCamera
{
    int      framenr;  // 1 type, name 4, 132.
    float    error;    // 2 type, name 7, 3754.
    float    mat[4][4];// 3 type, name 7, 343.
};

// zu DNA structure 313, 606
struct MovieTrackingCamera
{
    void     *intrinsics;     // 1 type, name 11, 3755.
    short    distortion_model;// 2 type, name 2, 3756.
    short    pad;             // 3 type, name 2, 66.
    float    sensor_width;    // 4 type, name 7, 3757.
    float    pixel_aspect;    // 5 type, name 7, 3758.
    float    focal;           // 6 type, name 7, 3759.
    short    units;           // 7 type, name 2, 3760.
    short    pad1;            // 8 type, name 2, 68.
    float    principal[2];    // 9 type, name 7, 3761.
    float    k1;              // 10 type, name 7, 3762.
    float    k2;              // 11 type, name 7, 3763.
    float    k3;              // 12 type, name 7, 3764.
    float    division_k1;     // 13 type, name 7, 3765.
    float    division_k2;     // 14 type, name 7, 3766.
};

// zu DNA structure 314, 603
struct MovieTrackingMarker
{
    float    pos[2];               // 1 type, name 7, 3767.
    float    pattern_corners[4][2];// 2 type, name 7, 3768.
    float    search_min[2];        // 3 type, name 7, 3769.
    float    search_max[2];        // 4 type, name 7, 3770.
    int      framenr;              // 5 type, name 4, 132.
    int      flag;                 // 6 type, name 4, 18.
};

// zu DNA structure 315, 604
struct MovieTrackingTrack
{
    MovieTrackingTrack     *next;              // 1 type, name 604, 0.
    MovieTrackingTrack     *prev;              // 2 type, name 604, 1.
    char                   name[64];           // 3 type, name 0, 19.
    float                  pat_min[2];         // 4 type, name 7, 3771.
    float                  pat_max[2];         // 5 type, name 7, 3772.
    float                  search_min[2];      // 6 type, name 7, 3769.
    float                  search_max[2];      // 7 type, name 7, 3770.
    float                  offset[2];          // 8 type, name 7, 3215.
    int                    markersnr;          // 9 type, name 4, 3773.
    int                    last_marker;        // 10 type, name 4, 3774.
    MovieTrackingMarker    *markers;           // 11 type, name 603, 3775.
    float                  bundle_pos[3];      // 12 type, name 7, 3776.
    float                  error;              // 13 type, name 7, 3754.
    int                    flag;               // 14 type, name 4, 18.
    int                    pat_flag;           // 15 type, name 4, 3777.
    int                    search_flag;        // 16 type, name 4, 3778.
    float                  color[3];           // 17 type, name 7, 108.
    short                  frames_limit;       // 18 type, name 2, 3779.
    short                  margin;             // 19 type, name 2, 1074.
    short                  pattern_match;      // 20 type, name 2, 3780.
    short                  motion_model;       // 21 type, name 2, 3781.
    int                    algorithm_flag;     // 22 type, name 4, 3782.
    float                  minimum_correlation;// 23 type, name 7, 3783.
    bGPdata                *gpd;               // 24 type, name 195, 1031.
    float                  weight;             // 25 type, name 7, 574.
    float                  pad;                // 26 type, name 7, 66.
};

// zu DNA structure 316, 607
struct MovieTrackingPlaneMarker
{
    float    corners[4][2];// 1 type, name 7, 3784.
    int      framenr;      // 2 type, name 4, 132.
    int      flag;         // 3 type, name 4, 18.
};

// zu DNA structure 317, 608
struct MovieTrackingPlaneTrack
{
    MovieTrackingPlaneTrack     *next;         // 1 type, name 608, 0.
    MovieTrackingPlaneTrack     *prev;         // 2 type, name 608, 1.
    char                        name[64];      // 3 type, name 0, 19.
    MovieTrackingTrack          **point_tracks;// 4 type, name 604, 3785.
    int                         point_tracksnr;// 5 type, name 4, 3786.
    int                         pad;           // 6 type, name 4, 66.
    MovieTrackingPlaneMarker    *markers;      // 7 type, name 607, 3775.
    int                         markersnr;     // 8 type, name 4, 3773.
    int                         flag;          // 9 type, name 4, 18.
    Image                       *image;        // 10 type, name 46, 788.
    float                       image_opacity; // 11 type, name 7, 3787.
    int                         last_marker;   // 12 type, name 4, 3774.
};

// zu DNA structure 318, 609
struct MovieTrackingSettings
{
    int      flag;                       // 1 type, name 4, 18.
    short    default_motion_model;       // 2 type, name 2, 3788.
    short    default_algorithm_flag;     // 3 type, name 2, 3789.
    float    default_minimum_correlation;// 4 type, name 7, 3790.
    short    default_pattern_size;       // 5 type, name 2, 3791.
    short    default_search_size;        // 6 type, name 2, 3792.
    short    default_frames_limit;       // 7 type, name 2, 3793.
    short    default_margin;             // 8 type, name 2, 3794.
    short    default_pattern_match;      // 9 type, name 2, 3795.
    short    default_flag;               // 10 type, name 2, 3796.
    float    default_weight;             // 11 type, name 7, 926.
    short    motion_flag;                // 12 type, name 2, 3797.
    short    speed;                      // 13 type, name 2, 806.
    int      keyframe1;                  // 14 type, name 4, 3798.
    int      keyframe2;                  // 15 type, name 4, 3799.
    int      reconstruction_flag;        // 16 type, name 4, 3800.
    short    refine_camera_intrinsics;   // 17 type, name 2, 3801.
    short    pad2;                       // 18 type, name 2, 29.
    float    dist;                       // 19 type, name 7, 358.
    int      clean_frames;               // 20 type, name 4, 3802.
    int      clean_action;               // 21 type, name 4, 3803.
    float    clean_error;                // 22 type, name 7, 3804.
    float    object_distance;            // 23 type, name 7, 3805.
    int      pad3;                       // 24 type, name 4, 517.
};

// zu DNA structure 319, 610
struct MovieTrackingStabilization
{
    int                   flag;      // 1 type, name 4, 18.
    int                   tot_track; // 2 type, name 4, 3806.
    int                   act_track; // 3 type, name 4, 3807.
    float                 maxscale;  // 4 type, name 7, 3808.
    MovieTrackingTrack    *rot_track;// 5 type, name 604, 3809.
    float                 locinf;    // 6 type, name 7, 3810.
    float                 scaleinf;  // 7 type, name 7, 3811.
    float                 rotinf;    // 8 type, name 7, 3812.
    int                   filter;    // 9 type, name 4, 458.
    int                   ok;        // 10 type, name 4, 138.
    float                 scale;     // 11 type, name 7, 954.
};

// zu DNA structure 320, 611
struct MovieTrackingReconstruction
{
    int                         flag;       // 1 type, name 4, 18.
    float                       error;      // 2 type, name 7, 3754.
    int                         last_camera;// 3 type, name 4, 3813.
    int                         camnr;      // 4 type, name 4, 3814.
    MovieReconstructedCamera    *cameras;   // 5 type, name 605, 3815.
};

// zu DNA structure 321, 613
struct MovieTrackingStats
{
    char    message[256];// 1 type, name 0, 3819.
};

// zu DNA structure 322, 614
struct MovieTrackingDopesheetChannel
{
    MovieTrackingDopesheetChannel    *next;       // 1 type, name 614, 0.
    MovieTrackingDopesheetChannel    *prev;       // 2 type, name 614, 1.
    MovieTrackingTrack               *track;      // 3 type, name 604, 1024.
    int                              pad;         // 4 type, name 4, 66.
    char                             name[64];    // 5 type, name 0, 19.
    int                              tot_segment; // 6 type, name 4, 3820.
    int                              *segments;   // 7 type, name 4, 3821.
    int                              max_segment; // 8 type, name 4, 3822.
    int                              total_frames;// 9 type, name 4, 3823.
};

// zu DNA structure 323, 615
struct MovieTrackingDopesheetCoverageSegment
{
    MovieTrackingDopesheetCoverageSegment    *next;      // 1 type, name 615, 0.
    MovieTrackingDopesheetCoverageSegment    *prev;      // 2 type, name 615, 1.
    int                                      coverage;   // 3 type, name 4, 3824.
    int                                      start_frame;// 4 type, name 4, 2809.
    int                                      end_frame;  // 5 type, name 4, 2810.
    int                                      pad;        // 6 type, name 4, 66.
};

// zu DNA structure 324, 177
struct DynamicPaintBrushSettings
{
    DynamicPaintModifierData    *pmd;             // 1 type, name 175, 3869.
    void                        *dm;              // Note: using void* on undefined DNA type: DerivedMesh
    ParticleSystem              *psys;            // 3 type, name 157, 864.
    Material                    *mat;             // 4 type, name 68, 560.
    int                         flags;            // 5 type, name 4, 89.
    int                         collision;        // 6 type, name 4, 3873.
    float                       r;                // 7 type, name 7, 198.
    float                       g;                // 8 type, name 7, 199.
    float                       b;                // 9 type, name 7, 200.
    float                       alpha;            // 10 type, name 7, 446.
    float                       wetness;          // 11 type, name 7, 3874.
    float                       particle_radius;  // 12 type, name 7, 3875.
    float                       particle_smooth;  // 13 type, name 7, 3876.
    float                       paint_distance;   // 14 type, name 7, 3877.
    ColorBand                   *paint_ramp;      // 15 type, name 54, 3878.
    ColorBand                   *vel_ramp;        // 16 type, name 54, 3879.
    short                       proximity_falloff;// 17 type, name 2, 3880.
    short                       wave_type;        // 18 type, name 2, 3142.
    short                       ray_dir;          // 19 type, name 2, 3881.
    short                       pad;              // 20 type, name 2, 66.
    float                       wave_factor;      // 21 type, name 7, 3882.
    float                       wave_clamp;       // 22 type, name 7, 3883.
    float                       max_velocity;     // 23 type, name 7, 3884.
    float                       smudge_strength;  // 24 type, name 7, 3885.
};

// zu DNA structure 325, 619
struct MaskParent
{
    int      id_type;                  // 1 type, name 4, 3889.
    int      type;                     // 2 type, name 4, 16.
    ID       *id;                      // 3 type, name 22, 2014.
    char     parent[64];               // 4 type, name 0, 3890.
    char     sub_parent[64];           // 5 type, name 0, 3891.
    float    parent_orig[2];           // 6 type, name 7, 3892.
    float    parent_corners_orig[4][2];// 7 type, name 7, 3893.
};

// zu DNA structure 326, 620
struct MaskSplinePointUW
{
    float    u;   // 1 type, name 7, 3894.
    float    w;   // 2 type, name 7, 602.
    int      flag;// 3 type, name 4, 18.
};

// zu DNA structure 327, 623
struct MaskLayerShape
{
    MaskLayerShape    *next;   // 1 type, name 623, 0.
    MaskLayerShape    *prev;   // 2 type, name 623, 1.
    float             *data;   // 3 type, name 7, 2.
    int               tot_vert;// 4 type, name 4, 3902.
    int               frame;   // 5 type, name 4, 1175.
    char              flag;    // 6 type, name 0, 18.
    char              pad[7];  // 7 type, name 0, 115.
};

// zu DNA structure 328, 202
struct RigidBodyOb
{
    void     *physics_object; // 1 type, name 11, 3915.
    void     *physics_shape;  // 2 type, name 11, 3916.
    short    type;            // 3 type, name 2, 16.
    short    shape;           // 4 type, name 2, 1136.
    int      flag;            // 5 type, name 4, 18.
    int      col_groups;      // 6 type, name 4, 3917.
    short    mesh_source;     // 7 type, name 2, 3918.
    short    pad;             // 8 type, name 2, 66.
    float    mass;            // 9 type, name 7, 1069.
    float    friction;        // 10 type, name 7, 521.
    float    restitution;     // 11 type, name 7, 3919.
    float    margin;          // 12 type, name 7, 1074.
    float    lin_damping;     // 13 type, name 7, 3920.
    float    ang_damping;     // 14 type, name 7, 3921.
    float    lin_sleep_thresh;// 15 type, name 7, 3922.
    float    ang_sleep_thresh;// 16 type, name 7, 3923.
    float    orn[4];          // 17 type, name 7, 3924.
    float    pos[3];          // 18 type, name 7, 3925.
    float    pad1;            // 19 type, name 7, 68.
};

// zu DNA structure 329, 203
struct RigidBodyCon
{
    Object    *ob1;                     // 1 type, name 29, 3926.
    Object    *ob2;                     // 2 type, name 29, 3927.
    short     type;                     // 3 type, name 2, 16.
    short     num_solver_iterations;    // 4 type, name 2, 3913.
    int       flag;                     // 5 type, name 4, 18.
    float     breaking_threshold;       // 6 type, name 7, 3928.
    float     pad;                      // 7 type, name 7, 66.
    float     limit_lin_x_lower;        // 8 type, name 7, 3929.
    float     limit_lin_x_upper;        // 9 type, name 7, 3930.
    float     limit_lin_y_lower;        // 10 type, name 7, 3931.
    float     limit_lin_y_upper;        // 11 type, name 7, 3932.
    float     limit_lin_z_lower;        // 12 type, name 7, 3933.
    float     limit_lin_z_upper;        // 13 type, name 7, 3934.
    float     limit_ang_x_lower;        // 14 type, name 7, 3935.
    float     limit_ang_x_upper;        // 15 type, name 7, 3936.
    float     limit_ang_y_lower;        // 16 type, name 7, 3937.
    float     limit_ang_y_upper;        // 17 type, name 7, 3938.
    float     limit_ang_z_lower;        // 18 type, name 7, 3939.
    float     limit_ang_z_upper;        // 19 type, name 7, 3940.
    float     spring_stiffness_x;       // 20 type, name 7, 3941.
    float     spring_stiffness_y;       // 21 type, name 7, 3942.
    float     spring_stiffness_z;       // 22 type, name 7, 3943.
    float     spring_damping_x;         // 23 type, name 7, 3944.
    float     spring_damping_y;         // 24 type, name 7, 3945.
    float     spring_damping_z;         // 25 type, name 7, 3946.
    float     motor_lin_target_velocity;// 26 type, name 7, 3947.
    float     motor_ang_target_velocity;// 27 type, name 7, 3948.
    float     motor_lin_max_impulse;    // 28 type, name 7, 3949.
    float     motor_ang_max_impulse;    // 29 type, name 7, 3950.
    void      *physics_constraint;      // 30 type, name 11, 3951.
};

// zu DNA structure 330, 625
struct FreestyleLineSet
{
    FreestyleLineSet      *next;             // 1 type, name 625, 0.
    FreestyleLineSet      *prev;             // 2 type, name 625, 1.
    char                  name[64];          // 3 type, name 0, 19.
    int                   flags;             // 4 type, name 4, 89.
    int                   selection;         // 5 type, name 4, 3952.
    short                 qi;                // 6 type, name 2, 3953.
    short                 pad1;              // 7 type, name 2, 68.
    int                   qi_start;          // 8 type, name 4, 3954.
    int                   qi_end;            // 9 type, name 4, 3955.
    int                   edge_types;        // 10 type, name 4, 3956.
    int                   exclude_edge_types;// 11 type, name 4, 3957.
    int                   pad2;              // 12 type, name 4, 29.
    Group                 *group;            // 13 type, name 69, 520.
    FreestyleLineStyle    *linestyle;        // 14 type, name 626, 3958.
};

// zu DNA structure 331, 627
struct FreestyleModuleConfig
{
    FreestyleModuleConfig    *next;       // 1 type, name 627, 0.
    FreestyleModuleConfig    *prev;       // 2 type, name 627, 1.
    Text                     *script;     // 3 type, name 38, 2009.
    short                    is_displayed;// 4 type, name 2, 3959.
    short                    pad[3];      // 5 type, name 2, 350.
};

// zu DNA structure 332, 628
struct LineStyleModifier
{
    LineStyleModifier    *next;    // 1 type, name 628, 0.
    LineStyleModifier    *prev;    // 2 type, name 628, 1.
    char                 name[64]; // 3 type, name 0, 19.
    int                  type;     // 4 type, name 4, 16.
    float                influence;// 5 type, name 7, 2750.
    int                  flags;    // 6 type, name 4, 89.
    int                  blend;    // 7 type, name 4, 1797.
};

// zu DNA structure 333, 20
struct IDPropertyData
{
    void        *pointer;// 1 type, name 11, 12.
    ListBase    group;   // 2 type, name 14, 13.
    int         val;     // 3 type, name 4, 14.
    int         val2;    // 4 type, name 4, 15.
};

// zu DNA structure 334, 23
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

// zu DNA structure 335, 54
struct ColorBand
{
    short     tot;        // 1 type, name 2, 242.
    short     cur;        // 2 type, name 2, 63.
    char      ipotype;    // 3 type, name 0, 243.
    char      ipotype_hue;// 4 type, name 0, 244.
    char      color_mode; // 5 type, name 0, 245.
    char      pad[1];     // 6 type, name 0, 246.
    CBData    data[32];   // 7 type, name 53, 247.
};

// zu DNA structure 336, 64
struct Lamp
{
    ID              id;                     // 1 type, name 22, 31.
    AnimData        *adt;                   // 2 type, name 36, 75.
    short           type;                   // 3 type, name 2, 16.
    short           flag;                   // 4 type, name 2, 18.
    int             mode;                   // 5 type, name 4, 351.
    short           colormodel;             // 6 type, name 2, 193.
    short           totex;                  // 7 type, name 2, 352.
    float           r;                      // 8 type, name 7, 198.
    float           g;                      // 9 type, name 7, 199.
    float           b;                      // 10 type, name 7, 200.
    float           k;                      // 11 type, name 7, 201.
    float           shdwr;                  // 12 type, name 7, 353.
    float           shdwg;                  // 13 type, name 7, 354.
    float           shdwb;                  // 14 type, name 7, 355.
    float           shdwpad;                // 15 type, name 7, 356.
    float           energy;                 // 16 type, name 7, 357.
    float           dist;                   // 17 type, name 7, 358.
    float           spotsize;               // 18 type, name 7, 359.
    float           spotblend;              // 19 type, name 7, 360.
    float           haint;                  // 20 type, name 7, 361.
    float           att1;                   // 21 type, name 7, 362.
    float           att2;                   // 22 type, name 7, 363.
    CurveMapping    *curfalloff;            // 23 type, name 58, 364.
    short           falloff_type;           // 24 type, name 2, 259.
    short           pad2;                   // 25 type, name 2, 29.
    float           clipsta;                // 26 type, name 7, 118.
    float           clipend;                // 27 type, name 7, 119.
    float           bias;                   // 28 type, name 7, 365.
    float           soft;                   // 29 type, name 7, 366.
    float           compressthresh;         // 30 type, name 7, 367.
    float           bleedbias;              // 31 type, name 7, 368.
    float           pad5;                   // 32 type, name 7, 369.
    short           bufsize;                // 33 type, name 2, 370.
    short           samp;                   // 34 type, name 2, 371.
    short           buffers;                // 35 type, name 2, 372.
    short           filtertype;             // 36 type, name 2, 373.
    char            bufflag;                // 37 type, name 0, 374.
    char            buftype;                // 38 type, name 0, 375.
    short           ray_samp;               // 39 type, name 2, 376.
    short           ray_sampy;              // 40 type, name 2, 377.
    short           ray_sampz;              // 41 type, name 2, 378.
    short           ray_samp_type;          // 42 type, name 2, 379.
    short           area_shape;             // 43 type, name 2, 380.
    float           area_size;              // 44 type, name 7, 381.
    float           area_sizey;             // 45 type, name 7, 382.
    float           area_sizez;             // 46 type, name 7, 383.
    float           adapt_thresh;           // 47 type, name 7, 384.
    short           ray_samp_method;        // 48 type, name 2, 385.
    short           shadowmap_type;         // 49 type, name 2, 386.
    short           texact;                 // 50 type, name 2, 387.
    short           shadhalostep;           // 51 type, name 2, 388.
    short           sun_effect_type;        // 52 type, name 2, 389.
    short           skyblendtype;           // 53 type, name 2, 390.
    float           horizon_brightness;     // 54 type, name 7, 391.
    float           spread;                 // 55 type, name 7, 392.
    float           sun_brightness;         // 56 type, name 7, 393.
    float           sun_size;               // 57 type, name 7, 394.
    float           backscattered_light;    // 58 type, name 7, 395.
    float           sun_intensity;          // 59 type, name 7, 396.
    float           atm_turbidity;          // 60 type, name 7, 397.
    float           atm_inscattering_factor;// 61 type, name 7, 398.
    float           atm_extinction_factor;  // 62 type, name 7, 399.
    float           atm_distance_factor;    // 63 type, name 7, 400.
    float           skyblendfac;            // 64 type, name 7, 401.
    float           sky_exposure;           // 65 type, name 7, 402.
    float           shadow_frustum_size;    // 66 type, name 7, 403.
    short           sky_colorspace;         // 67 type, name 2, 404.
    char            pad4[2];                // 68 type, name 0, 405.
    Ipo             *ipo;                   // 69 type, name 33, 80.
    MTex            *mtex[18];              // 70 type, name 51, 406.
    short           pr_texture;             // 71 type, name 2, 407.
    short           use_nodes;              // 72 type, name 2, 340.
    char            pad6[4];                // 73 type, name 0, 408.
    PreviewImage    *preview;               // 74 type, name 26, 159.
    bNodeTree       *nodetree;              // 75 type, name 61, 335.
};

// zu DNA structure 337, 70
struct VFont
{
    ID            id;         // 1 type, name 22, 31.
    char          name[1024]; // 2 type, name 0, 34.
    void          *data;      // Note: using void* on undefined DNA type: VFontData
    PackedFile    *packedfile;// 4 type, name 25, 37.
    PackedFile    *temp_pf;   // 5 type, name 25, 549.
};

// zu DNA structure 338, 78
struct EditNurb
{
    ListBase    nurbs;    // 1 type, name 14, 604.
    void        *keyindex;// Note: using void* on undefined DNA type: GHash
    int         shapenr;  // 3 type, name 4, 606.
    char        pad[4];   // 4 type, name 0, 585.
};

// zu DNA structure 339, 107
struct MultiresColFace
{
    MultiresCol    col[4];// 1 type, name 106, 692.
};

// zu DNA structure 340, 117
struct MappingInfoModifierData
{
    ModifierData    modifier;        // 1 type, name 116, 740.
    Tex             *texture;        // 2 type, name 52, 741.
    Object          *map_object;     // 3 type, name 29, 742.
    char            uvlayer_name[64];// 4 type, name 0, 743.
    int             uvlayer_tmp;     // 5 type, name 4, 744.
    int             texmapping;      // 6 type, name 4, 745.
};

// zu DNA structure 341, 118
struct SubsurfModifierData
{
    ModifierData    modifier;    // 1 type, name 116, 740.
    short           subdivType;  // 2 type, name 2, 746.
    short           levels;      // 3 type, name 2, 727.
    short           renderLevels;// 4 type, name 2, 747.
    short           flags;       // 5 type, name 2, 89.
    void            *emCache;    // 6 type, name 11, 748.
    void            *mCache;     // 7 type, name 11, 749.
};

// zu DNA structure 342, 119
struct LatticeModifierData
{
    ModifierData    modifier;// 1 type, name 116, 740.
    Object          *object; // 2 type, name 29, 178.
    char            name[64];// 3 type, name 0, 19.
    float           strength;// 4 type, name 7, 750.
    char            pad[4];  // 5 type, name 0, 585.
};

// zu DNA structure 343, 120
struct CurveModifierData
{
    ModifierData    modifier;// 1 type, name 116, 740.
    Object          *object; // 2 type, name 29, 178.
    char            name[64];// 3 type, name 0, 19.
    short           defaxis; // 4 type, name 2, 751.
    char            pad[6];  // 5 type, name 0, 752.
};

// zu DNA structure 344, 121
struct BuildModifierData
{
    ModifierData    modifier; // 1 type, name 116, 740.
    float           start;    // 2 type, name 7, 753.
    float           length;   // 3 type, name 7, 754.
    short           flag;     // 4 type, name 2, 18.
    short           randomize;// 5 type, name 2, 755.
    int             seed;     // 6 type, name 4, 756.
};

// zu DNA structure 345, 122
struct MaskModifierData
{
    ModifierData    modifier;  // 1 type, name 116, 740.
    Object          *ob_arm;   // 2 type, name 29, 757.
    char            vgroup[64];// 3 type, name 0, 72.
    int             mode;      // 4 type, name 4, 351.
    int             flag;      // 5 type, name 4, 18.
};

// zu DNA structure 346, 123
struct ArrayModifierData
{
    ModifierData    modifier;   // 1 type, name 116, 740.
    Object          *start_cap; // 2 type, name 29, 758.
    Object          *end_cap;   // 3 type, name 29, 759.
    Object          *curve_ob;  // 4 type, name 29, 760.
    Object          *offset_ob; // 5 type, name 29, 761.
    float           offset[3];  // 6 type, name 7, 762.
    float           scale[3];   // 7 type, name 7, 763.
    float           length;     // 8 type, name 7, 754.
    float           merge_dist; // 9 type, name 7, 764.
    int             fit_type;   // 10 type, name 4, 765.
    int             offset_type;// 11 type, name 4, 766.
    int             flags;      // 12 type, name 4, 89.
    int             count;      // 13 type, name 4, 767.
};

// zu DNA structure 347, 124
struct MirrorModifierData
{
    ModifierData    modifier;  // 1 type, name 116, 740.
    short           axis;      // 2 type, name 2, 768.
    short           flag;      // 3 type, name 2, 18.
    float           tolerance; // 4 type, name 7, 769.
    Object          *mirror_ob;// 5 type, name 29, 770.
};

// zu DNA structure 348, 125
struct EdgeSplitModifierData
{
    ModifierData    modifier;   // 1 type, name 116, 740.
    float           split_angle;// 2 type, name 7, 771.
    int             flags;      // 3 type, name 4, 89.
};

// zu DNA structure 349, 126
struct BevelModifierData
{
    ModifierData    modifier;       // 1 type, name 116, 740.
    float           value;          // 2 type, name 7, 772.
    int             res;            // 3 type, name 4, 773.
    short           flags;          // 4 type, name 2, 89.
    short           val_flags;      // 5 type, name 2, 774.
    short           lim_flags;      // 6 type, name 2, 775.
    short           e_flags;        // 7 type, name 2, 776.
    short           mat;            // 8 type, name 2, 777.
    short           pad;            // 9 type, name 2, 66.
    int             pad2;           // 10 type, name 4, 29.
    float           profile;        // 11 type, name 7, 778.
    float           bevel_angle;    // 12 type, name 7, 779.
    char            defgrp_name[64];// 13 type, name 0, 780.
};

// zu DNA structure 350, 127
struct SmokeModifierData
{
    ModifierData           modifier;// 1 type, name 116, 740.
    SmokeDomainSettings    *domain; // 2 type, name 128, 781.
    SmokeFlowSettings      *flow;   // 3 type, name 129, 782.
    SmokeCollSettings      *coll;   // 4 type, name 130, 783.
    float                  time;    // 5 type, name 7, 784.
    int                    type;    // 6 type, name 4, 16.
};

// zu DNA structure 351, 131
struct DisplaceModifierData
{
    ModifierData    modifier;        // 1 type, name 116, 740.
    Tex             *texture;        // 2 type, name 52, 741.
    Object          *map_object;     // 3 type, name 29, 742.
    char            uvlayer_name[64];// 4 type, name 0, 743.
    int             uvlayer_tmp;     // 5 type, name 4, 744.
    int             texmapping;      // 6 type, name 4, 745.
    float           strength;        // 7 type, name 7, 750.
    int             direction;       // 8 type, name 4, 785.
    char            defgrp_name[64]; // 9 type, name 0, 780.
    float           midlevel;        // 10 type, name 7, 786.
    int             pad;             // 11 type, name 4, 66.
};

// zu DNA structure 352, 132
struct UVProjectModifierData
{
    ModifierData    modifier;        // 1 type, name 116, 740.
    Object          *projectors[10]; // 2 type, name 29, 787.
    Image           *image;          // 3 type, name 46, 788.
    int             flags;           // 4 type, name 4, 89.
    int             num_projectors;  // 5 type, name 4, 789.
    float           aspectx;         // 6 type, name 7, 790.
    float           aspecty;         // 7 type, name 7, 791.
    float           scalex;          // 8 type, name 7, 792.
    float           scaley;          // 9 type, name 7, 793.
    char            uvlayer_name[64];// 10 type, name 0, 743.
    int             uvlayer_tmp;     // 11 type, name 4, 744.
    int             pad;             // 12 type, name 4, 66.
};

// zu DNA structure 353, 133
struct DecimateModifierData
{
    ModifierData    modifier;       // 1 type, name 116, 740.
    float           percent;        // 2 type, name 7, 794.
    short           iter;           // 3 type, name 2, 795.
    char            delimit;        // 4 type, name 0, 796.
    char            pad;            // 5 type, name 0, 66.
    float           angle;          // 6 type, name 7, 797.
    char            defgrp_name[64];// 7 type, name 0, 780.
    short           flag;           // 8 type, name 2, 18.
    short           mode;           // 9 type, name 2, 351.
    int             face_count;     // 10 type, name 4, 798.
    int             pad2;           // 11 type, name 4, 29.
};

// zu DNA structure 354, 134
struct SmoothModifierData
{
    ModifierData    modifier;       // 1 type, name 116, 740.
    float           fac;            // 2 type, name 7, 799.
    char            defgrp_name[64];// 3 type, name 0, 780.
    short           flag;           // 4 type, name 2, 18.
    short           repeat;         // 5 type, name 2, 800.
};

// zu DNA structure 355, 135
struct CastModifierData
{
    ModifierData    modifier;       // 1 type, name 116, 740.
    Object          *object;        // 2 type, name 29, 178.
    float           fac;            // 3 type, name 7, 799.
    float           radius;         // 4 type, name 7, 261.
    float           size;           // 5 type, name 7, 101.
    char            defgrp_name[64];// 6 type, name 0, 780.
    short           flag;           // 7 type, name 2, 18.
    short           type;           // 8 type, name 2, 16.
};

// zu DNA structure 356, 136
struct WaveModifierData
{
    ModifierData    modifier;        // 1 type, name 116, 740.
    Tex             *texture;        // 2 type, name 52, 741.
    Object          *map_object;     // 3 type, name 29, 742.
    char            uvlayer_name[64];// 4 type, name 0, 743.
    int             uvlayer_tmp;     // 5 type, name 4, 744.
    int             texmapping;      // 6 type, name 4, 745.
    Object          *objectcenter;   // 7 type, name 29, 801.
    char            defgrp_name[64]; // 8 type, name 0, 780.
    short           flag;            // 9 type, name 2, 18.
    short           pad;             // 10 type, name 2, 66.
    float           startx;          // 11 type, name 7, 802.
    float           starty;          // 12 type, name 7, 803.
    float           height;          // 13 type, name 7, 804.
    float           width;           // 14 type, name 7, 619.
    float           narrow;          // 15 type, name 7, 805.
    float           speed;           // 16 type, name 7, 806.
    float           damp;            // 17 type, name 7, 807.
    float           falloff;         // 18 type, name 7, 808.
    float           timeoffs;        // 19 type, name 7, 809.
    float           lifetime;        // 20 type, name 7, 810.
    float           pad1;            // 21 type, name 7, 68.
};

// zu DNA structure 357, 137
struct ArmatureModifierData
{
    ModifierData    modifier;       // 1 type, name 116, 740.
    short           deformflag;     // 2 type, name 2, 811.
    short           multi;          // 3 type, name 2, 812.
    int             pad2;           // 4 type, name 4, 29.
    Object          *object;        // 5 type, name 29, 178.
    float           *prevCos;       // 6 type, name 7, 813.
    char            defgrp_name[64];// 7 type, name 0, 780.
};

// zu DNA structure 358, 138
struct HookModifierData
{
    ModifierData    modifier;       // 1 type, name 116, 740.
    Object          *object;        // 2 type, name 29, 178.
    char            subtarget[64];  // 3 type, name 0, 814.
    char            flag;           // 4 type, name 0, 18.
    char            falloff_type;   // 5 type, name 0, 259.
    char            pad[6];         // 6 type, name 0, 752.
    float           parentinv[4][4];// 7 type, name 7, 815.
    float           cent[3];        // 8 type, name 7, 816.
    float           falloff;        // 9 type, name 7, 808.
    CurveMapping    *curfalloff;    // 10 type, name 58, 364.
    int             *indexar;       // 11 type, name 4, 817.
    int             totindex;       // 12 type, name 4, 818.
    float           force;          // 13 type, name 7, 819.
    char            name[64];       // 14 type, name 0, 19.
};

// zu DNA structure 359, 139
struct SoftbodyModifierData
{
    ModifierData    modifier;// 1 type, name 116, 740.
};

// zu DNA structure 360, 147
struct CollisionModifierData
{
    ModifierData    modifier;     // 1 type, name 116, 740.
    MVert           *x;           // 2 type, name 92, 831.
    MVert           *xnew;        // 3 type, name 92, 832.
    MVert           *xold;        // 4 type, name 92, 833.
    MVert           *current_xnew;// 5 type, name 92, 834.
    MVert           *current_x;   // 6 type, name 92, 835.
    MVert           *current_v;   // 7 type, name 92, 836.
    MFace           *mfaces;      // 8 type, name 89, 837.
    int             numverts;     // 9 type, name 4, 838.
    int             numfaces;     // 10 type, name 4, 839.
    float           time_x;       // 11 type, name 7, 840.
    float           time_xnew;    // 12 type, name 7, 841.
    void            *bvhtree;     // Note: using void* on undefined DNA type: BVHTree
};

// zu DNA structure 361, 149
struct SurfaceModifierData
{
    ModifierData       modifier;// 1 type, name 116, 740.
    MVert              *x;      // 2 type, name 92, 831.
    MVert              *v;      // 3 type, name 92, 843.
    void               *dm;     // Note: using void* on undefined DNA type: DerivedMesh
    void               *bvhtree;// Note: using void* on undefined DNA type: BVHTreeFromMesh
    int                cfra;    // 6 type, name 4, 845.
    int                numverts;// 7 type, name 4, 838.
};

// zu DNA structure 362, 152
struct BooleanModifierData
{
    ModifierData    modifier; // 1 type, name 116, 740.
    Object          *object;  // 2 type, name 29, 178.
    int             operation;// 3 type, name 4, 846.
    int             pad;      // 4 type, name 4, 66.
};

// zu DNA structure 363, 155
struct MeshDeformModifierData
{
    ModifierData     modifier;       // 1 type, name 116, 740.
    Object           *object;        // 2 type, name 29, 178.
    char             defgrp_name[64];// 3 type, name 0, 780.
    short            gridsize;       // 4 type, name 2, 849.
    short            flag;           // 5 type, name 2, 18.
    short            pad[2];         // 6 type, name 2, 187.
    MDefInfluence    *bindinfluences;// 7 type, name 153, 850.
    int              *bindoffsets;   // 8 type, name 4, 851.
    float            *bindcagecos;   // 9 type, name 7, 852.
    int              totvert;        // 10 type, name 4, 53.
    int              totcagevert;    // 11 type, name 4, 853.
    MDefCell         *dyngrid;       // 12 type, name 154, 854.
    MDefInfluence    *dyninfluences; // 13 type, name 153, 855.
    int              *dynverts;      // 14 type, name 4, 856.
    int              dyngridsize;    // 15 type, name 4, 857.
    int              totinfluence;   // 16 type, name 4, 848.
    float            dyncellmin[3];  // 17 type, name 7, 858.
    float            dyncellwidth;   // 18 type, name 7, 859.
    float            bindmat[4][4];  // 19 type, name 7, 860.
    float            *bindweights;   // 20 type, name 7, 861.
    float            *bindcos;       // 21 type, name 7, 862.
    void             (*bindfunc)();  // 22 type, name 11, 863.
};

// zu DNA structure 364, 156
struct ParticleSystemModifierData
{
    ModifierData      modifier; // 1 type, name 116, 740.
    ParticleSystem    *psys;    // 2 type, name 157, 864.
    void              *dm;      // Note: using void* on undefined DNA type: DerivedMesh
    int               totdmvert;// 4 type, name 4, 865.
    int               totdmedge;// 5 type, name 4, 866.
    int               totdmface;// 6 type, name 4, 867.
    short             flag;     // 7 type, name 2, 18.
    short             pad;      // 8 type, name 2, 66.
};

// zu DNA structure 365, 158
struct ParticleInstanceModifierData
{
    ModifierData    modifier;       // 1 type, name 116, 740.
    Object          *ob;            // 2 type, name 29, 44.
    short           psys;           // 3 type, name 2, 265.
    short           flag;           // 4 type, name 2, 18.
    short           axis;           // 5 type, name 2, 768.
    short           pad;            // 6 type, name 2, 66.
    float           position;       // 7 type, name 7, 868.
    float           random_position;// 8 type, name 7, 869.
};

// zu DNA structure 366, 159
struct ExplodeModifierData
{
    ModifierData    modifier;  // 1 type, name 116, 740.
    int             *facepa;   // 2 type, name 4, 870.
    short           flag;      // 3 type, name 2, 18.
    short           vgroup;    // 4 type, name 2, 871.
    float           protect;   // 5 type, name 7, 872.
    char            uvname[64];// 6 type, name 0, 180.
};

// zu DNA structure 367, 160
struct MultiresModifierData
{
    ModifierData    modifier; // 1 type, name 116, 740.
    char            lvl;      // 2 type, name 0, 873.
    char            sculptlvl;// 3 type, name 0, 874.
    char            renderlvl;// 4 type, name 0, 733.
    char            totlvl;   // 5 type, name 0, 875.
    char            simple;   // 6 type, name 0, 876.
    char            flags;    // 7 type, name 0, 89.
    char            pad[2];   // 8 type, name 0, 187.
};

// zu DNA structure 368, 161
struct FluidsimModifierData
{
    ModifierData        modifier;    // 1 type, name 116, 740.
    FluidsimSettings    *fss;        // 2 type, name 162, 877.
    PointCache          *point_cache;// 3 type, name 144, 823.
};

// zu DNA structure 369, 163
struct ShrinkwrapModifierData
{
    ModifierData    modifier;       // 1 type, name 116, 740.
    Object          *target;        // 2 type, name 29, 878.
    Object          *auxTarget;     // 3 type, name 29, 879.
    char            vgroup_name[64];// 4 type, name 0, 880.
    float           keepDist;       // 5 type, name 7, 881.
    short           shrinkType;     // 6 type, name 2, 882.
    char            shrinkOpts;     // 7 type, name 0, 883.
    char            pad1;           // 8 type, name 0, 68.
    float           projLimit;      // 9 type, name 7, 884.
    char            projAxis;       // 10 type, name 0, 885.
    char            subsurfLevels;  // 11 type, name 0, 886.
    char            pad[2];         // 12 type, name 0, 187.
};

// zu DNA structure 370, 164
struct SimpleDeformModifierData
{
    ModifierData    modifier;       // 1 type, name 116, 740.
    Object          *origin;        // 2 type, name 29, 887.
    char            vgroup_name[64];// 3 type, name 0, 880.
    float           factor;         // 4 type, name 7, 107.
    float           limit[2];       // 5 type, name 7, 888.
    char            mode;           // 6 type, name 0, 351.
    char            axis;           // 7 type, name 0, 768.
    char            pad[2];         // 8 type, name 0, 187.
};

// zu DNA structure 371, 165
struct ShapeKeyModifierData
{
    ModifierData    modifier;// 1 type, name 116, 740.
};

// zu DNA structure 372, 166
struct SolidifyModifierData
{
    ModifierData    modifier;       // 1 type, name 116, 740.
    char            defgrp_name[64];// 2 type, name 0, 780.
    float           offset;         // 3 type, name 7, 134.
    float           offset_fac;     // 4 type, name 7, 889.
    float           offset_fac_vg;  // 5 type, name 7, 890.
    float           offset_clamp;   // 6 type, name 7, 891.
    float           pad;            // 7 type, name 7, 66.
    float           crease_inner;   // 8 type, name 7, 892.
    float           crease_outer;   // 9 type, name 7, 893.
    float           crease_rim;     // 10 type, name 7, 894.
    int             flag;           // 11 type, name 4, 18.
    short           mat_ofs;        // 12 type, name 2, 895.
    short           mat_ofs_rim;    // 13 type, name 2, 896.
};

// zu DNA structure 373, 167
struct ScrewModifierData
{
    ModifierData    modifier;    // 1 type, name 116, 740.
    Object          *ob_axis;    // 2 type, name 29, 897.
    int             steps;       // 3 type, name 4, 898.
    int             render_steps;// 4 type, name 4, 899.
    int             iter;        // 5 type, name 4, 795.
    float           screw_ofs;   // 6 type, name 7, 900.
    float           angle;       // 7 type, name 7, 797.
    char            axis;        // 8 type, name 0, 768.
    char            pad;         // 9 type, name 0, 66.
    short           flag;        // 10 type, name 2, 18.
};

// zu DNA structure 374, 168
struct OceanModifierData
{
    ModifierData    modifier;         // 1 type, name 116, 740.
    void            *ocean;           // Note: using void* on undefined DNA type: Ocean
    void            *oceancache;      // Note: using void* on undefined DNA type: OceanCache
    int             resolution;       // 4 type, name 4, 903.
    int             spatial_size;     // 5 type, name 4, 904.
    float           wind_velocity;    // 6 type, name 7, 905.
    float           damp;             // 7 type, name 7, 807.
    float           smallest_wave;    // 8 type, name 7, 906.
    float           depth;            // 9 type, name 7, 256.
    float           wave_alignment;   // 10 type, name 7, 907.
    float           wave_direction;   // 11 type, name 7, 908.
    float           wave_scale;       // 12 type, name 7, 909.
    float           chop_amount;      // 13 type, name 7, 910.
    float           foam_coverage;    // 14 type, name 7, 911.
    float           time;             // 15 type, name 7, 784.
    int             bakestart;        // 16 type, name 4, 912.
    int             bakeend;          // 17 type, name 4, 913.
    char            cachepath[1024];  // 18 type, name 0, 914.
    char            foamlayername[64];// 19 type, name 0, 915.
    char            cached;           // 20 type, name 0, 916.
    char            geometry_mode;    // 21 type, name 0, 917.
    char            flag;             // 22 type, name 0, 18.
    char            refresh;          // 23 type, name 0, 918.
    short           repeat_x;         // 24 type, name 2, 919.
    short           repeat_y;         // 25 type, name 2, 920.
    int             seed;             // 26 type, name 4, 756.
    float           size;             // 27 type, name 7, 101.
    float           foam_fade;        // 28 type, name 7, 921.
    int             pad;              // 29 type, name 4, 66.
};

// zu DNA structure 375, 171
struct WarpModifierData
{
    ModifierData    modifier;        // 1 type, name 116, 740.
    Tex             *texture;        // 2 type, name 52, 741.
    Object          *map_object;     // 3 type, name 29, 742.
    char            uvlayer_name[64];// 4 type, name 0, 743.
    int             uvlayer_tmp;     // 5 type, name 4, 744.
    int             texmapping;      // 6 type, name 4, 745.
    Object          *object_from;    // 7 type, name 29, 922.
    Object          *object_to;      // 8 type, name 29, 923.
    CurveMapping    *curfalloff;     // 9 type, name 58, 364.
    char            defgrp_name[64]; // 10 type, name 0, 780.
    float           strength;        // 11 type, name 7, 750.
    float           falloff_radius;  // 12 type, name 7, 924.
    char            flag;            // 13 type, name 0, 18.
    char            falloff_type;    // 14 type, name 0, 259.
    char            pad[6];          // 15 type, name 0, 752.
};

// zu DNA structure 376, 172
struct WeightVGEditModifierData
{
    ModifierData    modifier;                 // 1 type, name 116, 740.
    char            defgrp_name[64];          // 2 type, name 0, 780.
    short           edit_flags;               // 3 type, name 2, 925.
    short           falloff_type;             // 4 type, name 2, 259.
    float           default_weight;           // 5 type, name 7, 926.
    CurveMapping    *cmap_curve;              // 6 type, name 58, 927.
    float           add_threshold;            // 7 type, name 7, 928.
    float           rem_threshold;            // 8 type, name 7, 929.
    float           mask_constant;            // 9 type, name 7, 930.
    char            mask_defgrp_name[64];     // 10 type, name 0, 931.
    int             mask_tex_use_channel;     // 11 type, name 4, 932.
    Tex             *mask_texture;            // 12 type, name 52, 933.
    Object          *mask_tex_map_obj;        // 13 type, name 29, 934.
    int             mask_tex_mapping;         // 14 type, name 4, 935.
    char            mask_tex_uvlayer_name[64];// 15 type, name 0, 936.
    int             pad_i1;                   // 16 type, name 4, 937.
};

// zu DNA structure 377, 173
struct WeightVGMixModifierData
{
    ModifierData    modifier;                 // 1 type, name 116, 740.
    char            defgrp_name_a[64];        // 2 type, name 0, 938.
    char            defgrp_name_b[64];        // 3 type, name 0, 939.
    float           default_weight_a;         // 4 type, name 7, 940.
    float           default_weight_b;         // 5 type, name 7, 941.
    char            mix_mode;                 // 6 type, name 0, 942.
    char            mix_set;                  // 7 type, name 0, 943.
    char            pad_c1[6];                // 8 type, name 0, 944.
    float           mask_constant;            // 9 type, name 7, 930.
    char            mask_defgrp_name[64];     // 10 type, name 0, 931.
    int             mask_tex_use_channel;     // 11 type, name 4, 932.
    Tex             *mask_texture;            // 12 type, name 52, 933.
    Object          *mask_tex_map_obj;        // 13 type, name 29, 934.
    int             mask_tex_mapping;         // 14 type, name 4, 935.
    char            mask_tex_uvlayer_name[64];// 15 type, name 0, 936.
    int             pad_i1;                   // 16 type, name 4, 937.
};

// zu DNA structure 378, 174
struct WeightVGProximityModifierData
{
    ModifierData    modifier;                 // 1 type, name 116, 740.
    char            defgrp_name[64];          // 2 type, name 0, 780.
    int             proximity_mode;           // 3 type, name 4, 945.
    int             proximity_flags;          // 4 type, name 4, 946.
    Object          *proximity_ob_target;     // 5 type, name 29, 947.
    float           mask_constant;            // 6 type, name 7, 930.
    char            mask_defgrp_name[64];     // 7 type, name 0, 931.
    int             mask_tex_use_channel;     // 8 type, name 4, 932.
    Tex             *mask_texture;            // 9 type, name 52, 933.
    Object          *mask_tex_map_obj;        // 10 type, name 29, 934.
    int             mask_tex_mapping;         // 11 type, name 4, 935.
    char            mask_tex_uvlayer_name[64];// 12 type, name 0, 936.
    float           min_dist;                 // 13 type, name 7, 948.
    float           max_dist;                 // 14 type, name 7, 949.
    short           falloff_type;             // 15 type, name 2, 259.
    short           pad_s1;                   // 16 type, name 2, 950.
};

// zu DNA structure 379, 175
struct DynamicPaintModifierData
{
    ModifierData                  modifier;// 1 type, name 116, 740.
    DynamicPaintCanvasSettings    *canvas; // 2 type, name 176, 951.
    DynamicPaintBrushSettings     *brush;  // 3 type, name 177, 952.
    int                           type;    // 4 type, name 4, 16.
    int                           pad;     // 5 type, name 4, 66.
};

// zu DNA structure 380, 178
struct RemeshModifierData
{
    ModifierData    modifier;   // 1 type, name 116, 740.
    float           threshold;  // 2 type, name 7, 953.
    float           scale;      // 3 type, name 7, 954.
    float           hermite_num;// 4 type, name 7, 955.
    char            depth;      // 5 type, name 0, 256.
    char            flag;       // 6 type, name 0, 18.
    char            mode;       // 7 type, name 0, 351.
    char            pad;        // 8 type, name 0, 66.
};

// zu DNA structure 381, 179
struct SkinModifierData
{
    ModifierData    modifier;        // 1 type, name 116, 740.
    float           branch_smoothing;// 2 type, name 7, 956.
    char            flag;            // 3 type, name 0, 18.
    char            symmetry_axes;   // 4 type, name 0, 957.
    char            pad[2];          // 5 type, name 0, 187.
};

// zu DNA structure 382, 180
struct TriangulateModifierData
{
    ModifierData    modifier;   // 1 type, name 116, 740.
    int             flag;       // 2 type, name 4, 18.
    int             quad_method;// 3 type, name 4, 958.
    int             ngon_method;// 4 type, name 4, 959.
    int             pad;        // 5 type, name 4, 66.
};

// zu DNA structure 383, 181
struct LaplacianSmoothModifierData
{
    ModifierData    modifier;       // 1 type, name 116, 740.
    float           lambda;         // 2 type, name 7, 960.
    float           lambda_border;  // 3 type, name 7, 961.
    float           pad1;           // 4 type, name 7, 68.
    char            defgrp_name[64];// 5 type, name 0, 780.
    short           flag;           // 6 type, name 2, 18.
    short           repeat;         // 7 type, name 2, 800.
};

// zu DNA structure 384, 182
struct UVWarpModifierData
{
    ModifierData    modifier;        // 1 type, name 116, 740.
    char            axis_u;          // 2 type, name 0, 962.
    char            axis_v;          // 3 type, name 0, 963.
    char            pad[6];          // 4 type, name 0, 752.
    float           center[2];       // 5 type, name 7, 964.
    Object          *object_src;     // 6 type, name 29, 965.
    char            bone_src[64];    // 7 type, name 0, 966.
    Object          *object_dst;     // 8 type, name 29, 967.
    char            bone_dst[64];    // 9 type, name 0, 968.
    char            vgroup_name[64]; // 10 type, name 0, 880.
    char            uvlayer_name[64];// 11 type, name 0, 743.
};

// zu DNA structure 385, 183
struct MeshCacheModifierData
{
    ModifierData    modifier;      // 1 type, name 116, 740.
    char            flag;          // 2 type, name 0, 18.
    char            type;          // 3 type, name 0, 16.
    char            time_mode;     // 4 type, name 0, 969.
    char            play_mode;     // 5 type, name 0, 970.
    char            forward_axis;  // 6 type, name 0, 971.
    char            up_axis;       // 7 type, name 0, 972.
    char            flip_axis;     // 8 type, name 0, 973.
    char            interp;        // 9 type, name 0, 974.
    float           factor;        // 10 type, name 7, 107.
    char            deform_mode;   // 11 type, name 0, 975.
    char            pad[7];        // 12 type, name 0, 115.
    float           frame_start;   // 13 type, name 7, 976.
    float           frame_scale;   // 14 type, name 7, 977.
    float           eval_frame;    // 15 type, name 7, 978.
    float           eval_time;     // 16 type, name 7, 979.
    float           eval_factor;   // 17 type, name 7, 980.
    char            filepath[1024];// 18 type, name 0, 35.
};

// zu DNA structure 386, 184
struct LaplacianDeformModifierData
{
    ModifierData    modifier;           // 1 type, name 116, 740.
    char            anchor_grp_name[64];// 2 type, name 0, 981.
    int             total_verts;        // 3 type, name 4, 982.
    int             repeat;             // 4 type, name 4, 800.
    float           *vertexco;          // 5 type, name 7, 983.
    void            *cache_system;      // 6 type, name 11, 984.
    short           flag;               // 7 type, name 2, 18.
    short           pad[3];             // 8 type, name 2, 350.
};

// zu DNA structure 387, 185
struct WireframeModifierData
{
    ModifierData    modifier;       // 1 type, name 116, 740.
    char            defgrp_name[64];// 2 type, name 0, 780.
    float           offset;         // 3 type, name 7, 134.
    float           offset_fac;     // 4 type, name 7, 889.
    float           offset_fac_vg;  // 5 type, name 7, 890.
    float           crease_weight;  // 6 type, name 7, 985.
    short           flag;           // 7 type, name 2, 18.
    short           mat_ofs;        // 8 type, name 2, 895.
    short           pad[2];         // 9 type, name 2, 187.
};

// zu DNA structure 388, 186
struct DataTransferModifierData
{
    ModifierData    modifier;            // 1 type, name 116, 740.
    Object          *ob_source;          // 2 type, name 29, 986.
    int             data_types;          // 3 type, name 4, 987.
    int             vmap_mode;           // 4 type, name 4, 988.
    int             emap_mode;           // 5 type, name 4, 989.
    int             lmap_mode;           // 6 type, name 4, 990.
    int             pmap_mode;           // 7 type, name 4, 991.
    float           map_max_distance;    // 8 type, name 7, 992.
    float           map_ray_radius;      // 9 type, name 7, 993.
    float           islands_precision;   // 10 type, name 7, 994.
    int             pad_i1;              // 11 type, name 4, 937.
    int             layers_select_src[4];// 12 type, name 4, 995.
    int             layers_select_dst[4];// 13 type, name 4, 996.
    int             mix_mode;            // 14 type, name 4, 942.
    float           mix_factor;          // 15 type, name 7, 997.
    char            defgrp_name[64];     // 16 type, name 0, 780.
    int             flags;               // 17 type, name 4, 89.
};

// zu DNA structure 389, 187
struct NormalEditModifierData
{
    ModifierData    modifier;       // 1 type, name 116, 740.
    char            defgrp_name[64];// 2 type, name 0, 780.
    Object          *target;        // 3 type, name 29, 878.
    short           mode;           // 4 type, name 2, 351.
    short           flag;           // 5 type, name 2, 18.
    short           mix_mode;       // 6 type, name 2, 942.
    char            pad[2];         // 7 type, name 0, 187.
    float           mix_factor;     // 8 type, name 7, 997.
    float           offset[3];      // 9 type, name 7, 762.
};

// zu DNA structure 390, 189
struct Lattice
{
    ID             id;        // 1 type, name 22, 31.
    AnimData       *adt;      // 2 type, name 36, 75.
    short          pntsu;     // 3 type, name 2, 588.
    short          pntsv;     // 4 type, name 2, 589.
    short          pntsw;     // 5 type, name 2, 999.
    short          flag;      // 6 type, name 2, 18.
    short          opntsu;    // 7 type, name 2, 1000.
    short          opntsv;    // 8 type, name 2, 1001.
    short          opntsw;    // 9 type, name 2, 1002.
    short          pad2;      // 10 type, name 2, 29.
    char           typeu;     // 11 type, name 0, 1003.
    char           typev;     // 12 type, name 0, 1004.
    char           typew;     // 13 type, name 0, 1005.
    char           pad3;      // 14 type, name 0, 517.
    int            actbp;     // 15 type, name 4, 1006.
    float          fu;        // 16 type, name 7, 1007.
    float          fv;        // 17 type, name 7, 1008.
    float          fw;        // 18 type, name 7, 1009.
    float          du;        // 19 type, name 7, 1010.
    float          dv;        // 20 type, name 7, 1011.
    float          dw;        // 21 type, name 7, 1012.
    BPoint         *def;      // 22 type, name 31, 1013.
    Ipo            *ipo;      // 23 type, name 33, 80.
    Key            *key;      // 24 type, name 35, 612.
    MDeformVert    *dvert;    // 25 type, name 94, 668.
    char           vgroup[64];// 26 type, name 0, 72.
    EditLatt       *editlatt; // 27 type, name 188, 1014.
};

// zu DNA structure 391, 209
struct PTCacheMem
{
    PTCacheMem    *next;     // 1 type, name 209, 0.
    PTCacheMem    *prev;     // 2 type, name 209, 1.
    int           frame;     // 3 type, name 4, 1175.
    int           totpoint;  // 4 type, name 4, 1176.
    int           data_types;// 5 type, name 4, 987.
    int           flag;      // 6 type, name 4, 18.
    void          *data[8];  // 7 type, name 11, 1177.
    void          *cur[8];   // 8 type, name 11, 1178.
    ListBase      extradata; // 9 type, name 14, 1179.
};

// zu DNA structure 392, 144
struct PointCache
{
    PointCache     *next;         // 1 type, name 144, 0.
    PointCache     *prev;         // 2 type, name 144, 1.
    int            flag;          // 3 type, name 4, 18.
    int            step;          // 4 type, name 4, 1180.
    int            simframe;      // 5 type, name 4, 1181.
    int            startframe;    // 6 type, name 4, 1182.
    int            endframe;      // 7 type, name 4, 1183.
    int            editframe;     // 8 type, name 4, 1184.
    int            last_exact;    // 9 type, name 4, 1185.
    int            last_valid;    // 10 type, name 4, 1186.
    int            pad;           // 11 type, name 4, 66.
    int            totpoint;      // 12 type, name 4, 1176.
    int            index;         // 13 type, name 4, 430.
    short          compression;   // 14 type, name 2, 1187.
    short          rt;            // 15 type, name 2, 56.
    char           name[64];      // 16 type, name 0, 19.
    char           prev_name[64]; // 17 type, name 0, 1188.
    char           info[64];      // 18 type, name 0, 1189.
    char           path[1024];    // 19 type, name 0, 1190.
    char           *cached_frames;// 20 type, name 0, 1191.
    ListBase       mem_cache;     // 21 type, name 14, 1192.
    void           *edit;         // Note: using void* on undefined DNA type: PTCacheEdit
    void           (*free_edit)();// 23 type, name 11, 1194.
};

// zu DNA structure 393, 200
struct SoftBody
{
    int                totpoint;            // 1 type, name 4, 1176.
    int                totspring;           // 2 type, name 4, 1223.
    void               *bpoint;             // Note: using void* on undefined DNA type: BodyPoint
    void               *bspring;            // Note: using void* on undefined DNA type: BodySpring
    char               pad;                 // 5 type, name 0, 66.
    char               msg_lock;            // 6 type, name 0, 1226.
    short              msg_value;           // 7 type, name 2, 1227.
    float              nodemass;            // 8 type, name 7, 1228.
    char               namedVG_Mass[64];    // 9 type, name 0, 1229.
    float              grav;                // 10 type, name 7, 1230.
    float              mediafrict;          // 11 type, name 7, 1231.
    float              rklimit;             // 12 type, name 7, 1232.
    float              physics_speed;       // 13 type, name 7, 1233.
    float              goalspring;          // 14 type, name 7, 1234.
    float              goalfrict;           // 15 type, name 7, 1235.
    float              mingoal;             // 16 type, name 7, 1236.
    float              maxgoal;             // 17 type, name 7, 1237.
    float              defgoal;             // 18 type, name 7, 1238.
    short              vertgroup;           // 19 type, name 2, 1239.
    char               namedVG_Softgoal[64];// 20 type, name 0, 1240.
    short              fuzzyness;           // 21 type, name 2, 1241.
    float              inspring;            // 22 type, name 7, 1242.
    float              infrict;             // 23 type, name 7, 1243.
    char               namedVG_Spring_K[64];// 24 type, name 0, 1244.
    int                sfra;                // 25 type, name 4, 135.
    int                efra;                // 26 type, name 4, 1245.
    int                interval;            // 27 type, name 4, 1246.
    short              local;               // 28 type, name 2, 1247.
    short              solverflags;         // 29 type, name 2, 1248.
    SBVertex           **keys;              // 30 type, name 211, 1249.
    int                totpointkey;         // 31 type, name 4, 1250.
    int                totkey;              // 32 type, name 4, 82.
    float              secondspring;        // 33 type, name 7, 1251.
    float              colball;             // 34 type, name 7, 1252.
    float              balldamp;            // 35 type, name 7, 1253.
    float              ballstiff;           // 36 type, name 7, 1254.
    short              sbc_mode;            // 37 type, name 2, 1255.
    short              aeroedge;            // 38 type, name 2, 1256.
    short              minloops;            // 39 type, name 2, 1257.
    short              maxloops;            // 40 type, name 2, 1258.
    short              choke;               // 41 type, name 2, 1259.
    short              solver_ID;           // 42 type, name 2, 1260.
    short              plastic;             // 43 type, name 2, 1261.
    short              springpreload;       // 44 type, name 2, 1262.
    void               *scratch;            // Note: using void* on undefined DNA type: SBScratch
    float              shearstiff;          // 46 type, name 7, 1264.
    float              inpush;              // 47 type, name 7, 1265.
    PointCache         *pointcache;         // 48 type, name 144, 1266.
    ListBase           ptcaches;            // 49 type, name 14, 824.
    EffectorWeights    *effector_weights;   // 50 type, name 207, 1267.
    float              lcom[3];             // 51 type, name 7, 1268.
    float              lrot[3][3];          // 52 type, name 7, 1269.
    float              lscale[3][3];        // 53 type, name 7, 1270.
    int                last_frame;          // 54 type, name 4, 1271.
};

// zu DNA structure 394, 239
struct ImagePaintSettings
{
    Paint    paint;              // 1 type, name 236, 1610.
    short    flag;               // 2 type, name 2, 18.
    short    missing_data;       // 3 type, name 2, 1611.
    short    seam_bleed;         // 4 type, name 2, 1612.
    short    normal_angle;       // 5 type, name 2, 1613.
    short    screen_grab_size[2];// 6 type, name 2, 1614.
    int      mode;               // 7 type, name 4, 351.
    void     *paintcursor;       // 8 type, name 11, 1615.
    Image    *stencil;           // 9 type, name 46, 1616.
    Image    *clone;             // 10 type, name 46, 1617.
    Image    *canvas;            // 11 type, name 46, 951.
    float    stencil_col[3];     // 12 type, name 7, 1618.
    float    dither;             // 13 type, name 7, 1619.
};

// zu DNA structure 395, 241
struct ParticleEditSettings
{
    short                flag;         // 1 type, name 2, 18.
    short                totrekey;     // 2 type, name 2, 1621.
    short                totaddkey;    // 3 type, name 2, 1622.
    short                brushtype;    // 4 type, name 2, 1623.
    ParticleBrushData    brush[7];     // 5 type, name 240, 1624.
    void                 *paintcursor; // 6 type, name 11, 1615.
    float                emitterdist;  // 7 type, name 7, 1625.
    float                rt;           // 8 type, name 7, 56.
    int                  selectmode;   // 9 type, name 4, 1626.
    int                  edittype;     // 10 type, name 4, 1627.
    int                  draw_step;    // 11 type, name 4, 1628.
    int                  fade_frames;  // 12 type, name 4, 1629.
    Scene                *scene;       // 13 type, name 44, 131.
    Object               *object;      // 14 type, name 29, 178.
    Object               *shape_object;// 15 type, name 29, 1630.
};

// zu DNA structure 396, 242
struct Sculpt
{
    Paint     paint;               // 1 type, name 236, 1610.
    int       flags;               // 2 type, name 4, 89.
    int       radial_symm[3];      // 3 type, name 4, 1631.
    float     detail_size;         // 4 type, name 7, 1632.
    int       symmetrize_direction;// 5 type, name 4, 1633.
    float     gravity_factor;      // 6 type, name 7, 1634.
    float     constant_detail;     // 7 type, name 7, 1635.
    Object    *gravity_object;     // 8 type, name 29, 1636.
    void      *pad2;               // 9 type, name 11, 1637.
};

// zu DNA structure 397, 243
struct UvSculpt
{
    Paint    paint;// 1 type, name 236, 1610.
};

// zu DNA structure 398, 244
struct VPaint
{
    Paint          paint;       // 1 type, name 236, 1610.
    short          flag;        // 2 type, name 2, 18.
    short          pad;         // 3 type, name 2, 66.
    int            tot;         // 4 type, name 4, 242.
    int            *vpaint_prev;// 5 type, name 4, 1638.
    MDeformVert    *wpaint_prev;// 6 type, name 94, 1639.
    void           *paintcursor;// 7 type, name 11, 1615.
};

// zu DNA structure 399, 268
struct SpaceLink
{
    SpaceLink    *next;          // 1 type, name 268, 0.
    SpaceLink    *prev;          // 2 type, name 268, 1.
    ListBase     regionbase;     // 3 type, name 14, 1838.
    int          spacetype;      // 4 type, name 4, 1839.
    float        blockscale;     // 5 type, name 7, 1840.
    short        blockhandler[8];// 6 type, name 2, 1841.
};

// zu DNA structure 400, 271
struct SpaceInfo
{
    SpaceLink    *next;          // 1 type, name 268, 0.
    SpaceLink    *prev;          // 2 type, name 268, 1.
    ListBase     regionbase;     // 3 type, name 14, 1838.
    int          spacetype;      // 4 type, name 4, 1839.
    float        blockscale;     // 5 type, name 7, 1840.
    short        blockhandler[8];// 6 type, name 2, 1841.
    char         rpt_mask;       // 7 type, name 0, 1895.
    char         pad[7];         // 8 type, name 0, 115.
};

// zu DNA structure 401, 286
struct SpaceFile
{
    SpaceLink           *next;              // 1 type, name 268, 0.
    SpaceLink           *prev;              // 2 type, name 268, 1.
    ListBase            regionbase;         // 3 type, name 14, 1838.
    int                 spacetype;          // 4 type, name 4, 1839.
    int                 scroll_offset;      // 5 type, name 4, 1949.
    FileSelectParams    *params;            // 6 type, name 285, 1950.
    void                *files;             // Note: using void* on undefined DNA type: FileList
    ListBase            *folders_prev;      // 8 type, name 14, 1952.
    ListBase            *folders_next;      // 9 type, name 14, 1953.
    wmOperator          *op;                // 10 type, name 288, 1954.
    void                *smoothscroll_timer;// Note: using void* on undefined DNA type: wmTimer
    void                *layout;            // Note: using void* on undefined DNA type: FileLayout
    short               recentnr;           // 13 type, name 2, 1957.
    short               bookmarknr;         // 14 type, name 2, 1958.
    short               systemnr;           // 15 type, name 2, 1959.
    short               system_bookmarknr;  // 16 type, name 2, 1960.
};

// zu DNA structure 402, 295
struct Script
{
    ID      id;                 // 1 type, name 22, 31.
    void    *py_draw;           // 2 type, name 11, 2001.
    void    *py_event;          // 3 type, name 11, 2002.
    void    *py_button;         // 4 type, name 11, 2003.
    void    *py_browsercallback;// 5 type, name 11, 2004.
    void    *py_globaldict;     // 6 type, name 11, 2005.
    int     flags;              // 7 type, name 4, 89.
    int     lastspace;          // 8 type, name 4, 2006.
    char    scriptname[1024];   // 9 type, name 0, 2007.
    char    scriptarg[256];     // 10 type, name 0, 2008.
};

// zu DNA structure 403, 296
struct SpaceScript
{
    SpaceLink    *next;     // 1 type, name 268, 0.
    SpaceLink    *prev;     // 2 type, name 268, 1.
    ListBase     regionbase;// 3 type, name 14, 1838.
    int          spacetype; // 4 type, name 4, 1839.
    float        blockscale;// 5 type, name 7, 1840.
    Script       *script;   // 6 type, name 295, 2009.
    short        flags;     // 7 type, name 2, 89.
    short        menunr;    // 8 type, name 2, 1979.
    int          pad1;      // 9 type, name 4, 68.
    void         *but_refs; // 10 type, name 11, 2010.
};

// zu DNA structure 404, 297
struct bNodeTreePath
{
    bNodeTreePath       *next;         // 1 type, name 297, 0.
    bNodeTreePath       *prev;         // 2 type, name 297, 1.
    bNodeTree           *nodetree;     // 3 type, name 61, 335.
    bNodeInstanceKey    parent_key;    // 4 type, name 298, 2011.
    int                 pad;           // 5 type, name 4, 66.
    float               view_center[2];// 6 type, name 7, 2012.
    char                node_name[64]; // 7 type, name 0, 2013.
};

// zu DNA structure 405, 300
struct SpaceLogic
{
    SpaceLink    *next;          // 1 type, name 268, 0.
    SpaceLink    *prev;          // 2 type, name 268, 1.
    ListBase     regionbase;     // 3 type, name 14, 1838.
    int          spacetype;      // 4 type, name 4, 1839.
    float        blockscale;     // 5 type, name 7, 1840.
    short        blockhandler[8];// 6 type, name 2, 1841.
    short        flag;           // 7 type, name 2, 18.
    short        scaflag;        // 8 type, name 2, 1062.
    int          pad;            // 9 type, name 4, 66.
    bGPdata      *gpd;           // 10 type, name 195, 1031.
};

// zu DNA structure 406, 303
struct SpaceUserPref
{
    SpaceLink    *next;      // 1 type, name 268, 0.
    SpaceLink    *prev;      // 2 type, name 268, 1.
    ListBase     regionbase; // 3 type, name 14, 1838.
    int          spacetype;  // 4 type, name 4, 1839.
    char         pad[3];     // 5 type, name 0, 350.
    char         filter_type;// 6 type, name 0, 2031.
    char         filter[64]; // 7 type, name 0, 2032.
};

// zu DNA structure 407, 323
struct ScrVert
{
    ScrVert    *next;   // 1 type, name 323, 0.
    ScrVert    *prev;   // 2 type, name 323, 1.
    ScrVert    *newv;   // 3 type, name 323, 2457.
    vec2s      vec;     // 4 type, name 15, 2458.
    short      flag;    // 5 type, name 2, 18.
    short      editflag;// 6 type, name 2, 688.
};

// zu DNA structure 408, 343
struct Strip
{
    Strip                             *next;              // 1 type, name 343, 0.
    Strip                             *prev;              // 2 type, name 343, 1.
    int                               us;                 // 3 type, name 4, 27.
    int                               done;               // 4 type, name 4, 2539.
    int                               startstill;         // 5 type, name 4, 2540.
    int                               endstill;           // 6 type, name 4, 2541.
    StripElem                         *stripdata;         // 7 type, name 338, 2542.
    char                              dir[768];           // 8 type, name 0, 2534.
    StripProxy                        *proxy;             // 9 type, name 342, 1025.
    StripCrop                         *crop;              // 10 type, name 339, 2543.
    StripTransform                    *transform;         // 11 type, name 340, 2544.
    StripColorBalance                 *color_balance;     // 12 type, name 341, 2545.
    ColorManagedColorspaceSettings    colorspace_settings;// 13 type, name 50, 171.
};

// zu DNA structure 409, 357
struct BrightContrastModifierData
{
    SequenceModifierData    modifier;// 1 type, name 353, 740.
    float                   bright;  // 2 type, name 7, 298.
    float                   contrast;// 3 type, name 7, 299.
};

// zu DNA structure 410, 358
struct SequencerMaskModifierData
{
    SequenceModifierData    modifier;// 1 type, name 353, 740.
};

// zu DNA structure 411, 388
struct bSoundActuator
{
    short      flag;   // 1 type, name 2, 18.
    short      sndnr;  // 2 type, name 2, 2714.
    int        pad1;   // 3 type, name 4, 68.
    int        pad2;   // 4 type, name 4, 29.
    short      pad3[2];// 5 type, name 2, 2715.
    float      volume; // 6 type, name 7, 1197.
    float      pitch;  // 7 type, name 7, 2569.
    bSound     *sound; // 8 type, name 345, 2567.
    Sound3D    sound3D;// 9 type, name 387, 2716.
    short      type;   // 10 type, name 2, 16.
    short      pad4;   // 11 type, name 2, 1499.
    short      pad5;   // 12 type, name 2, 369.
    short      pad6[1];// 13 type, name 2, 2717.
};

// zu DNA structure 412, 345
struct bSound
{
    ID            id;              // 1 type, name 22, 31.
    char          name[1024];      // 2 type, name 0, 34.
    PackedFile    *packedfile;     // 3 type, name 25, 37.
    void          *handle;         // 4 type, name 11, 2765.
    PackedFile    *newpackedfile;  // 5 type, name 25, 2766.
    Ipo           *ipo;            // 6 type, name 33, 80.
    float         volume;          // 7 type, name 7, 1197.
    float         attenuation;     // 8 type, name 7, 110.
    float         pitch;           // 9 type, name 7, 2569.
    float         min_gain;        // 10 type, name 7, 2706.
    float         max_gain;        // 11 type, name 7, 2707.
    float         distance;        // 12 type, name 7, 1017.
    int           flags;           // 13 type, name 4, 89.
    int           pad;             // 14 type, name 4, 66.
    void          *cache;          // 15 type, name 11, 142.
    void          *waveform;       // 16 type, name 11, 2767.
    void          *playback_handle;// 17 type, name 11, 2768.
    void          *spinlock;       // 18 type, name 11, 2769.
};

// zu DNA structure 413, 408
struct Bone
{
    Bone          *next;         // 1 type, name 408, 0.
    Bone          *prev;         // 2 type, name 408, 1.
    IDProperty    *prop;         // 3 type, name 21, 2322.
    Bone          *parent;       // 4 type, name 408, 36.
    ListBase      childbase;     // 5 type, name 14, 2773.
    char          name[64];      // 6 type, name 0, 19.
    float         roll;          // 7 type, name 7, 2774.
    float         head[3];       // 8 type, name 7, 2775.
    float         tail[3];       // 9 type, name 7, 2776.
    float         bone_mat[3][3];// 10 type, name 7, 2777.
    int           flag;          // 11 type, name 4, 18.
    float         arm_head[3];   // 12 type, name 7, 2778.
    float         arm_tail[3];   // 13 type, name 7, 2779.
    float         arm_mat[4][4]; // 14 type, name 7, 2780.
    float         arm_roll;      // 15 type, name 7, 2781.
    float         dist;          // 16 type, name 7, 358.
    float         weight;        // 17 type, name 7, 574.
    float         xwidth;        // 18 type, name 7, 2782.
    float         length;        // 19 type, name 7, 754.
    float         zwidth;        // 20 type, name 7, 2783.
    float         ease1;         // 21 type, name 7, 2784.
    float         ease2;         // 22 type, name 7, 2785.
    float         rad_head;      // 23 type, name 7, 2786.
    float         rad_tail;      // 24 type, name 7, 2787.
    float         size[3];       // 25 type, name 7, 189.
    int           layer;         // 26 type, name 4, 140.
    short         segments;      // 27 type, name 2, 2788.
    short         pad[1];        // 28 type, name 2, 246.
};

// zu DNA structure 414, 277
struct bDopeSheet
{
    ID          *source;      // 1 type, name 22, 1016.
    ListBase    chanbase;     // 2 type, name 14, 2849.
    Group       *filter_grp;  // 3 type, name 69, 2876.
    char        searchstr[64];// 4 type, name 0, 2877.
    int         filterflag;   // 5 type, name 4, 2878.
    int         flag;         // 6 type, name 4, 18.
    int         renameIndex;  // 7 type, name 4, 2879.
    int         pad;          // 8 type, name 4, 66.
};

// zu DNA structure 415, 417
struct bActionChannel
{
    bActionChannel    *next;             // 1 type, name 417, 0.
    bActionChannel    *prev;             // 2 type, name 417, 1.
    bActionGroup      *grp;              // 3 type, name 415, 2882.
    Ipo               *ipo;              // 4 type, name 33, 80.
    ListBase          constraintChannels;// 5 type, name 14, 1034.
    int               flag;              // 6 type, name 4, 18.
    char              name[64];          // 7 type, name 0, 19.
    int               temp;              // 8 type, name 4, 2445.
};

// zu DNA structure 416, 421
struct bPythonConstraint
{
    Text          *text;        // 1 type, name 38, 1976.
    IDProperty    *prop;        // 2 type, name 21, 2322.
    int           flag;         // 3 type, name 4, 18.
    int           tarnum;       // 4 type, name 4, 2894.
    ListBase      targets;      // 5 type, name 14, 2895.
    Object        *tar;         // 6 type, name 29, 2890.
    char          subtarget[64];// 7 type, name 0, 814.
};

// zu DNA structure 417, 450
struct bActionStrip
{
    bActionStrip    *next;            // 1 type, name 450, 0.
    bActionStrip    *prev;            // 2 type, name 450, 1.
    short           flag;             // 3 type, name 2, 18.
    short           mode;             // 4 type, name 2, 351.
    short           stride_axis;      // 5 type, name 2, 2957.
    short           curmod;           // 6 type, name 2, 2958.
    Ipo             *ipo;             // 7 type, name 33, 80.
    bAction         *act;             // 8 type, name 193, 2698.
    Object          *object;          // 9 type, name 29, 178.
    float           start;            // 10 type, name 7, 753.
    float           end;              // 11 type, name 7, 2625.
    float           actstart;         // 12 type, name 7, 2959.
    float           actend;           // 13 type, name 7, 2960.
    float           actoffs;          // 14 type, name 7, 2961.
    float           stridelen;        // 15 type, name 7, 2962.
    float           repeat;           // 16 type, name 7, 800.
    float           scale;            // 17 type, name 7, 954.
    float           blendin;          // 18 type, name 7, 2700.
    float           blendout;         // 19 type, name 7, 2963.
    char            stridechannel[32];// 20 type, name 0, 2964.
    char            offs_bone[32];    // 21 type, name 0, 2965.
    ListBase        modifiers;        // 22 type, name 14, 1037.
};

// zu DNA structure 418, 452
struct bNodeSocket
{
    bNodeSocket        *next;         // 1 type, name 452, 0.
    bNodeSocket        *prev;         // 2 type, name 452, 1.
    bNodeSocket        *new_sock;     // 3 type, name 452, 2972.
    IDProperty         *prop;         // 4 type, name 21, 2322.
    char               identifier[64];// 5 type, name 0, 2973.
    char               name[64];      // 6 type, name 0, 19.
    void               *storage;      // 7 type, name 11, 2974.
    short              type;          // 8 type, name 2, 16.
    short              flag;          // 9 type, name 2, 18.
    short              limit;         // 10 type, name 2, 2975.
    short              in_out;        // 11 type, name 2, 2976.
    void               *typeinfo;     // Note: using void* on undefined DNA type: bNodeSocketType
    char               idname[64];    // 13 type, name 0, 2476.
    float              locx;          // 14 type, name 7, 2978.
    float              locy;          // 15 type, name 7, 2979.
    void               *default_value;// 16 type, name 11, 2980.
    short              stack_index;   // 17 type, name 2, 2981.
    short              stack_type;    // 18 type, name 2, 2982.
    int                pad;           // 19 type, name 4, 66.
    void               *cache;        // 20 type, name 11, 142.
    int                own_index;     // 21 type, name 4, 2983.
    int                to_index;      // 22 type, name 4, 2984.
    bNodeSocket        *groupsock;    // 23 type, name 452, 2985.
    bNodeLink          *link;         // 24 type, name 454, 2986.
    bNodeStack         ns;            // 25 type, name 451, 2987.
};

// zu DNA structure 419, 458
struct bNodeInstanceHashEntry
{
    bNodeInstanceKey    key;// 1 type, name 298, 2661.
    short               tag;// 2 type, name 2, 3011.
    short               pad;// 3 type, name 2, 66.
};

// zu DNA structure 420, 292
struct Scopes
{
    int          ok;             // 1 type, name 4, 138.
    int          sample_full;    // 2 type, name 4, 3195.
    int          sample_lines;   // 3 type, name 4, 3196.
    float        accuracy;       // 4 type, name 7, 3197.
    int          wavefrm_mode;   // 5 type, name 4, 3198.
    float        wavefrm_alpha;  // 6 type, name 7, 3199.
    float        wavefrm_yfac;   // 7 type, name 7, 3200.
    int          wavefrm_height; // 8 type, name 4, 3201.
    float        vecscope_alpha; // 9 type, name 7, 3202.
    int          vecscope_height;// 10 type, name 4, 3203.
    float        minmax[3][2];   // 11 type, name 7, 3204.
    Histogram    hist;           // 12 type, name 293, 3205.
    float        *waveform_1;    // 13 type, name 7, 3206.
    float        *waveform_2;    // 14 type, name 7, 3207.
    float        *waveform_3;    // 15 type, name 7, 3208.
    float        *vecscope;      // 16 type, name 7, 3209.
    int          waveform_tot;   // 17 type, name 4, 3210.
    int          pad;            // 18 type, name 4, 66.
};

// zu DNA structure 421, 529
struct PaintCurvePoint
{
    BezTriple    bez;     // 1 type, name 32, 3262.
    float        pressure;// 2 type, name 7, 3263.
};

// zu DNA structure 422, 527
struct PaintCurve
{
    ID                 id;        // 1 type, name 22, 31.
    PaintCurvePoint    *points;   // 2 type, name 529, 2808.
    int                tot_points;// 3 type, name 4, 3264.
    int                add_index; // 4 type, name 4, 3265.
};

// zu DNA structure 423, 534
struct BoidParticle
{
    Object      *ground;   // 1 type, name 29, 3279.
    BoidData    data;      // 2 type, name 535, 21.
    float       gravity[3];// 3 type, name 7, 1756.
    float       wander[3]; // 4 type, name 7, 3280.
    float       rt;        // 5 type, name 7, 56.
};

// zu DNA structure 424, 552
struct bGPDframe
{
    bGPDframe    *next;   // 1 type, name 552, 0.
    bGPDframe    *prev;   // 2 type, name 552, 1.
    ListBase     strokes; // 3 type, name 14, 3490.
    int          framenum;// 4 type, name 4, 3491.
    short        flag;    // 5 type, name 2, 18.
    short        key_type;// 6 type, name 2, 3492.
};

// zu DNA structure 425, 553
struct bGPDlayer
{
    bGPDlayer    *next;         // 1 type, name 553, 0.
    bGPDlayer    *prev;         // 2 type, name 553, 1.
    ListBase     frames;        // 3 type, name 14, 133.
    bGPDframe    *actframe;     // 4 type, name 552, 3493.
    short        flag;          // 5 type, name 2, 18.
    short        thickness;     // 6 type, name 2, 3488.
    short        gstep;         // 7 type, name 2, 3494.
    short        gstep_next;    // 8 type, name 2, 3495.
    float        gcolor_prev[3];// 9 type, name 7, 3496.
    float        gcolor_next[3];// 10 type, name 7, 3497.
    float        color[4];      // 11 type, name 7, 3498.
    float        fill[4];       // 12 type, name 7, 3499.
    char         info[128];     // 13 type, name 0, 3500.
};

// zu DNA structure 426, 554
struct ReportList
{
    ListBase    list;        // 1 type, name 14, 3504.
    int         printlevel;  // 2 type, name 4, 3505.
    int         storelevel;  // 3 type, name 4, 3506.
    int         flag;        // 4 type, name 4, 18.
    int         pad;         // 5 type, name 4, 66.
    void        *reporttimer;// Note: using void* on undefined DNA type: wmTimer
};

// zu DNA structure 427, 557
struct wmKeyConfig
{
    wmKeyConfig    *next;       // 1 type, name 557, 0.
    wmKeyConfig    *prev;       // 2 type, name 557, 1.
    char           idname[64];  // 3 type, name 0, 2476.
    char           basename[64];// 4 type, name 0, 3570.
    ListBase       keymaps;     // 5 type, name 14, 2369.
    int            actkeymap;   // 6 type, name 4, 3571.
    int            flag;        // 7 type, name 4, 18.
};

// zu DNA structure 428, 288
struct wmOperator
{
    wmOperator        *next;       // 1 type, name 288, 0.
    wmOperator        *prev;       // 2 type, name 288, 1.
    char              idname[64];  // 3 type, name 0, 2476.
    IDProperty        *properties; // 4 type, name 21, 30.
    void              *type;       // Note: using void* on undefined DNA type: wmOperatorType
    void              *customdata; // 6 type, name 11, 3572.
    void              *py_instance;// 7 type, name 11, 3573.
    void              *ptr;        // Note: using void* on undefined DNA type: PointerRNA
    ReportList        *reports;    // 9 type, name 554, 3574.
    ListBase          macro;       // 10 type, name 14, 3575.
    wmOperator        *opm;        // 11 type, name 288, 3576.
    void              *layout;     // Note: using void* on undefined DNA type: uiLayout
    short             flag;        // 13 type, name 2, 18.
    short             pad[3];      // 14 type, name 2, 350.
};

// zu DNA structure 429, 574
struct FMod_Limits
{
    rctf    rect;// 1 type, name 19, 3588.
    int     flag;// 2 type, name 4, 18.
    int     pad; // 3 type, name 4, 66.
};

// zu DNA structure 430, 578
struct DriverVar
{
    DriverVar       *next;      // 1 type, name 578, 0.
    DriverVar       *prev;      // 2 type, name 578, 1.
    char            name[64];   // 3 type, name 0, 19.
    DriverTarget    targets[8]; // 4 type, name 577, 3596.
    short           num_targets;// 5 type, name 2, 3597.
    short           type;       // 6 type, name 2, 16.
    float           curval;     // 7 type, name 7, 60.
};

// zu DNA structure 431, 579
struct ChannelDriver
{
    ListBase    variables;      // 1 type, name 14, 3598.
    char        expression[256];// 2 type, name 0, 3599.
    void        *expr_comp;     // 3 type, name 11, 3600.
    float       curval;         // 4 type, name 7, 60.
    float       influence;      // 5 type, name 7, 2750.
    int         type;           // 6 type, name 4, 16.
    int         flag;           // 7 type, name 4, 18.
};

// zu DNA structure 432, 581
struct FCurve
{
    FCurve           *next;           // 1 type, name 581, 0.
    FCurve           *prev;           // 2 type, name 581, 1.
    bActionGroup     *grp;            // 3 type, name 415, 2882.
    ChannelDriver    *driver;         // 4 type, name 579, 61.
    ListBase         modifiers;       // 5 type, name 14, 1037.
    BezTriple        *bezt;           // 6 type, name 32, 49.
    FPoint           *fpt;            // 7 type, name 580, 3602.
    int              totvert;         // 8 type, name 4, 53.
    float            curval;          // 9 type, name 7, 60.
    short            flag;            // 10 type, name 2, 18.
    short            extend;          // 11 type, name 2, 284.
    int              array_index;     // 12 type, name 4, 3603.
    char             *rna_path;       // 13 type, name 0, 3592.
    int              color_mode;      // 14 type, name 4, 245.
    float            color[3];        // 15 type, name 7, 108.
    float            prev_norm_factor;// 16 type, name 7, 3604.
    float            pad;             // 17 type, name 7, 66.
};

// zu DNA structure 433, 583
struct AnimMapper
{
    AnimMapper    *next;   // 1 type, name 583, 0.
    AnimMapper    *prev;   // 2 type, name 583, 1.
    bAction       *target; // 3 type, name 193, 878.
    ListBase      mappings;// 4 type, name 14, 3607.
};

// zu DNA structure 434, 585
struct NlaTrack
{
    NlaTrack    *next;   // 1 type, name 585, 0.
    NlaTrack    *prev;   // 2 type, name 585, 1.
    ListBase    strips;  // 3 type, name 14, 3608.
    int         flag;    // 4 type, name 4, 18.
    int         index;   // 5 type, name 4, 430.
    char        name[64];// 6 type, name 0, 19.
};

// zu DNA structure 435, 587
struct KeyingSet
{
    KeyingSet    *next;           // 1 type, name 587, 0.
    KeyingSet    *prev;           // 2 type, name 587, 1.
    ListBase     paths;           // 3 type, name 14, 3619.
    char         idname[64];      // 4 type, name 0, 2476.
    char         name[64];        // 5 type, name 0, 19.
    char         description[240];// 6 type, name 0, 3620.
    char         typeinfo[64];    // 7 type, name 0, 3621.
    int          active_path;     // 8 type, name 4, 3622.
    short        flag;            // 9 type, name 2, 18.
    short        keyingflag;      // 10 type, name 2, 3617.
    short        keyingoverride;  // 11 type, name 2, 3618.
    char         pad[6];          // 12 type, name 0, 752.
};

// zu DNA structure 436, 589
struct IdAdtTemplate
{
    ID          id;  // 1 type, name 22, 31.
    AnimData    *adt;// 2 type, name 36, 75.
};

// zu DNA structure 437, 591
struct BoidRuleGoalAvoid
{
    BoidRule    rule;       // 1 type, name 590, 3631.
    Object      *ob;        // 2 type, name 29, 44.
    int         options;    // 3 type, name 4, 3632.
    float       fear_factor;// 4 type, name 7, 3633.
    int         signal_id;  // 5 type, name 4, 3634.
    int         channels;   // 6 type, name 4, 2869.
};

// zu DNA structure 438, 592
struct BoidRuleAvoidCollision
{
    BoidRule    rule;      // 1 type, name 590, 3631.
    int         options;   // 2 type, name 4, 3632.
    float       look_ahead;// 3 type, name 7, 3635.
};

// zu DNA structure 439, 593
struct BoidRuleFollowLeader
{
    BoidRule    rule;      // 1 type, name 590, 3631.
    Object      *ob;       // 2 type, name 29, 44.
    float       loc[3];    // 3 type, name 7, 341.
    float       oloc[3];   // 4 type, name 7, 3636.
    float       cfra;      // 5 type, name 7, 845.
    float       distance;  // 6 type, name 7, 1017.
    int         options;   // 7 type, name 4, 3632.
    int         queue_size;// 8 type, name 4, 3637.
};

// zu DNA structure 440, 594
struct BoidRuleAverageSpeed
{
    BoidRule    rule;  // 1 type, name 590, 3631.
    float       wander;// 2 type, name 7, 3638.
    float       level; // 3 type, name 7, 717.
    float       speed; // 4 type, name 7, 806.
    float       rt;    // 5 type, name 7, 56.
};

// zu DNA structure 441, 595
struct BoidRuleFight
{
    BoidRule    rule;         // 1 type, name 590, 3631.
    float       distance;     // 2 type, name 7, 1017.
    float       flee_distance;// 3 type, name 7, 3639.
};

// zu DNA structure 442, 543
struct BoidSettings
{
    int         options;            // 1 type, name 4, 3632.
    int         last_state_id;      // 2 type, name 4, 3647.
    float       landing_smoothness; // 3 type, name 7, 3648.
    float       height;             // 4 type, name 7, 804.
    float       banking;            // 5 type, name 7, 3649.
    float       pitch;              // 6 type, name 7, 2569.
    float       health;             // 7 type, name 7, 3640.
    float       aggression;         // 8 type, name 7, 3650.
    float       strength;           // 9 type, name 7, 750.
    float       accuracy;           // 10 type, name 7, 3197.
    float       range;              // 11 type, name 7, 1330.
    float       air_min_speed;      // 12 type, name 7, 3651.
    float       air_max_speed;      // 13 type, name 7, 3652.
    float       air_max_acc;        // 14 type, name 7, 3653.
    float       air_max_ave;        // 15 type, name 7, 3654.
    float       air_personal_space; // 16 type, name 7, 3655.
    float       land_jump_speed;    // 17 type, name 7, 3656.
    float       land_max_speed;     // 18 type, name 7, 3657.
    float       land_max_acc;       // 19 type, name 7, 3658.
    float       land_max_ave;       // 20 type, name 7, 3659.
    float       land_personal_space;// 21 type, name 7, 3660.
    float       land_stick_force;   // 22 type, name 7, 3661.
    ListBase    states;             // 23 type, name 14, 3662.
};

// zu DNA structure 443, 128
struct SmokeDomainSettings
{
    SmokeModifierData    *smd;                // 1 type, name 127, 3663.
    void                 *fluid;              // Note: using void* on undefined DNA type: FLUID_3D
    void                 *fluid_mutex;        // 3 type, name 11, 3664.
    Group                *fluid_group;        // 4 type, name 69, 3665.
    Group                *eff_group;          // 5 type, name 69, 3406.
    Group                *coll_group;         // 6 type, name 69, 3666.
    void                 *wt;                 // Note: using void* on undefined DNA type: WTURBULENCE
    void                 *tex;                // Note: using void* on undefined DNA type: GPUTexture
    void                 *tex_wt;             // Note: using void* on undefined DNA type: GPUTexture
    void                 *tex_shadow;         // Note: using void* on undefined DNA type: GPUTexture
    void                 *tex_flame;          // Note: using void* on undefined DNA type: GPUTexture
    float                *shadow;             // 12 type, name 7, 3671.
    float                p0[3];               // 13 type, name 7, 3672.
    float                p1[3];               // 14 type, name 7, 3673.
    float                dp0[3];              // 15 type, name 7, 3674.
    float                cell_size[3];        // 16 type, name 7, 3675.
    float                global_size[3];      // 17 type, name 7, 3676.
    float                prev_loc[3];         // 18 type, name 7, 3677.
    int                  shift[3];            // 19 type, name 4, 3678.
    float                shift_f[3];          // 20 type, name 7, 3679.
    float                obj_shift_f[3];      // 21 type, name 7, 3680.
    float                imat[4][4];          // 22 type, name 7, 250.
    float                obmat[4][4];         // 23 type, name 7, 1051.
    int                  base_res[3];         // 24 type, name 4, 3681.
    int                  res_min[3];          // 25 type, name 4, 3682.
    int                  res_max[3];          // 26 type, name 4, 3683.
    int                  res[3];              // 27 type, name 4, 3684.
    int                  total_cells;         // 28 type, name 4, 3685.
    float                dx;                  // 29 type, name 7, 3686.
    float                scale;               // 30 type, name 7, 954.
    int                  adapt_margin;        // 31 type, name 4, 3687.
    int                  adapt_res;           // 32 type, name 4, 3688.
    float                adapt_threshold;     // 33 type, name 7, 3689.
    float                alpha;               // 34 type, name 7, 446.
    float                beta;                // 35 type, name 7, 3690.
    int                  amplify;             // 36 type, name 4, 3691.
    int                  maxres;              // 37 type, name 4, 3692.
    int                  flags;               // 38 type, name 4, 89.
    int                  viewsettings;        // 39 type, name 4, 3693.
    short                noise;               // 40 type, name 2, 3694.
    short                diss_percent;        // 41 type, name 2, 3695.
    int                  diss_speed;          // 42 type, name 4, 3696.
    float                strength;            // 43 type, name 7, 750.
    int                  res_wt[3];           // 44 type, name 4, 3697.
    float                dx_wt;               // 45 type, name 7, 3698.
    int                  cache_comp;          // 46 type, name 4, 3699.
    int                  cache_high_comp;     // 47 type, name 4, 3700.
    PointCache           *point_cache[2];     // 48 type, name 144, 3701.
    ListBase             ptcaches[2];         // 49 type, name 14, 3702.
    EffectorWeights      *effector_weights;   // 50 type, name 207, 1267.
    int                  border_collisions;   // 51 type, name 4, 3703.
    float                time_scale;          // 52 type, name 7, 3704.
    float                vorticity;           // 53 type, name 7, 3705.
    int                  active_fields;       // 54 type, name 4, 3706.
    float                active_color[3];     // 55 type, name 7, 3707.
    int                  highres_sampling;    // 56 type, name 4, 3708.
    float                burning_rate;        // 57 type, name 7, 3709.
    float                flame_smoke;         // 58 type, name 7, 3710.
    float                flame_vorticity;     // 59 type, name 7, 3711.
    float                flame_ignition;      // 60 type, name 7, 3712.
    float                flame_max_temp;      // 61 type, name 7, 3713.
    float                flame_smoke_color[3];// 62 type, name 7, 3714.
};

// zu DNA structure 444, 599
struct Speaker
{
    ID          id;                // 1 type, name 22, 31.
    AnimData    *adt;              // 2 type, name 36, 75.
    bSound      *sound;            // 3 type, name 345, 2567.
    float       volume_max;        // 4 type, name 7, 3728.
    float       volume_min;        // 5 type, name 7, 3729.
    float       distance_max;      // 6 type, name 7, 109.
    float       distance_reference;// 7 type, name 7, 3730.
    float       attenuation;       // 8 type, name 7, 110.
    float       cone_angle_outer;  // 9 type, name 7, 3731.
    float       cone_angle_inner;  // 10 type, name 7, 3732.
    float       cone_volume_outer; // 11 type, name 7, 3733.
    float       volume;            // 12 type, name 7, 1197.
    float       pitch;             // 13 type, name 7, 2569.
    short       flag;              // 14 type, name 2, 18.
    short       pad1[3];           // 15 type, name 2, 2488.
};

// zu DNA structure 445, 305
struct MovieClipScopes
{
    short                  ok;                  // 1 type, name 2, 138.
    short                  use_track_mask;      // 2 type, name 2, 3742.
    int                    track_preview_height;// 3 type, name 4, 3743.
    int                    frame_width;         // 4 type, name 4, 3744.
    int                    frame_height;        // 5 type, name 4, 3745.
    MovieTrackingMarker    undist_marker;       // 6 type, name 603, 3746.
    void                   *track_search;       // Note: using void* on undefined DNA type: ImBuf
    void                   *track_preview;      // Note: using void* on undefined DNA type: ImBuf
    float                  track_pos[2];        // 9 type, name 7, 3749.
    short                  track_disabled;      // 10 type, name 2, 3750.
    short                  track_locked;        // 11 type, name 2, 3751.
    int                    framenr;             // 12 type, name 4, 132.
    MovieTrackingTrack     *track;              // 13 type, name 604, 1024.
    MovieTrackingMarker    *marker;             // 14 type, name 603, 3752.
    float                  slide_scale[2];      // 15 type, name 7, 3753.
};

// zu DNA structure 446, 617
struct DynamicPaintSurface
{
    DynamicPaintSurface           *next;                  // 1 type, name 617, 0.
    DynamicPaintSurface           *prev;                  // 2 type, name 617, 1.
    DynamicPaintCanvasSettings    *canvas;                // 3 type, name 176, 951.
    void                          *data;                  // Note: using void* on undefined DNA type: PaintSurfaceData
    Group                         *brush_group;           // 5 type, name 69, 3837.
    EffectorWeights               *effector_weights;      // 6 type, name 207, 1267.
    PointCache                    *pointcache;            // 7 type, name 144, 1266.
    ListBase                      ptcaches;               // 8 type, name 14, 824.
    int                           current_frame;          // 9 type, name 4, 3838.
    char                          name[64];               // 10 type, name 0, 19.
    short                         format;                 // 11 type, name 2, 3079.
    short                         type;                   // 12 type, name 2, 16.
    short                         disp_type;              // 13 type, name 2, 3839.
    short                         image_fileformat;       // 14 type, name 2, 3840.
    short                         effect_ui;              // 15 type, name 2, 3841.
    short                         preview_id;             // 16 type, name 2, 3842.
    short                         init_color_type;        // 17 type, name 2, 3843.
    short                         pad_s;                  // 18 type, name 2, 3844.
    int                           flags;                  // 19 type, name 4, 89.
    int                           effect;                 // 20 type, name 4, 1035.
    int                           image_resolution;       // 21 type, name 4, 3845.
    int                           substeps;               // 22 type, name 4, 3846.
    int                           start_frame;            // 23 type, name 4, 2809.
    int                           end_frame;              // 24 type, name 4, 2810.
    int                           pad;                    // 25 type, name 4, 66.
    float                         init_color[4];          // 26 type, name 7, 3847.
    Tex                           *init_texture;          // 27 type, name 52, 3848.
    char                          init_layername[64];     // 28 type, name 0, 3849.
    int                           dry_speed;              // 29 type, name 4, 3850.
    int                           diss_speed;             // 30 type, name 4, 3696.
    float                         color_dry_threshold;    // 31 type, name 7, 3851.
    float                         depth_clamp;            // 32 type, name 7, 3852.
    float                         disp_factor;            // 33 type, name 7, 3853.
    float                         spread_speed;           // 34 type, name 7, 3854.
    float                         color_spread_speed;     // 35 type, name 7, 3855.
    float                         shrink_speed;           // 36 type, name 7, 3856.
    float                         drip_vel;               // 37 type, name 7, 3857.
    float                         drip_acc;               // 38 type, name 7, 3858.
    float                         influence_scale;        // 39 type, name 7, 3859.
    float                         radius_scale;           // 40 type, name 7, 3860.
    float                         wave_damping;           // 41 type, name 7, 3861.
    float                         wave_speed;             // 42 type, name 7, 3862.
    float                         wave_timescale;         // 43 type, name 7, 3863.
    float                         wave_spring;            // 44 type, name 7, 3864.
    float                         wave_smoothness;        // 45 type, name 7, 3865.
    int                           pad2;                   // 46 type, name 4, 29.
    char                          uvlayer_name[64];       // 47 type, name 0, 743.
    char                          image_output_path[1024];// 48 type, name 0, 3866.
    char                          output_name[64];        // 49 type, name 0, 3867.
    char                          output_name2[64];       // 50 type, name 0, 3868.
};

// zu DNA structure 447, 176
struct DynamicPaintCanvasSettings
{
    DynamicPaintModifierData    *pmd;      // 1 type, name 175, 3869.
    void                        *dm;       // Note: using void* on undefined DNA type: DerivedMesh
    ListBase                    surfaces;  // 3 type, name 14, 3870.
    short                       active_sur;// 4 type, name 2, 3871.
    short                       flags;     // 5 type, name 2, 89.
    int                         pad;       // 6 type, name 4, 66.
    char                        error[64]; // 7 type, name 0, 3872.
};

// zu DNA structure 448, 622
struct MaskSpline
{
    MaskSpline         *next;         // 1 type, name 622, 0.
    MaskSpline         *prev;         // 2 type, name 622, 1.
    short              flag;          // 3 type, name 2, 18.
    char               offset_mode;   // 4 type, name 0, 3898.
    char               weight_interp; // 5 type, name 0, 3899.
    int                tot_point;     // 6 type, name 4, 3900.
    MaskSplinePoint    *points;       // 7 type, name 621, 2808.
    MaskParent         parent;        // 8 type, name 619, 3285.
    MaskSplinePoint    *points_deform;// 9 type, name 621, 3901.
};

// zu DNA structure 449, 258
struct RigidBodyWorld
{
    EffectorWeights    *effector_weights;    // 1 type, name 207, 1267.
    Group              *group;               // 2 type, name 69, 520.
    Object             **objects;            // 3 type, name 29, 3908.
    Group              *constraints;         // 4 type, name 69, 3909.
    int                pad;                  // 5 type, name 4, 66.
    float              ltime;                // 6 type, name 7, 3910.
    PointCache         *pointcache;          // 7 type, name 144, 1266.
    ListBase           ptcaches;             // 8 type, name 14, 824.
    int                numbodies;            // 9 type, name 4, 3911.
    short              steps_per_second;     // 10 type, name 2, 3912.
    short              num_solver_iterations;// 11 type, name 2, 3913.
    int                flag;                 // 12 type, name 4, 18.
    float              time_scale;           // 13 type, name 7, 3704.
    void               *physics_world;       // 14 type, name 11, 3914.
};

// zu DNA structure 450, 629
struct LineStyleColorModifier_AlongStroke
{
    LineStyleModifier    modifier;   // 1 type, name 628, 740.
    ColorBand            *color_ramp;// 2 type, name 54, 3966.
};

// zu DNA structure 451, 630
struct LineStyleAlphaModifier_AlongStroke
{
    LineStyleModifier    modifier;// 1 type, name 628, 740.
    CurveMapping         *curve;  // 2 type, name 58, 3174.
    int                  flags;   // 3 type, name 4, 89.
    int                  pad;     // 4 type, name 4, 66.
};

// zu DNA structure 452, 631
struct LineStyleThicknessModifier_AlongStroke
{
    LineStyleModifier    modifier; // 1 type, name 628, 740.
    CurveMapping         *curve;   // 2 type, name 58, 3174.
    int                  flags;    // 3 type, name 4, 89.
    float                value_min;// 4 type, name 7, 3967.
    float                value_max;// 5 type, name 7, 3968.
    int                  pad;      // 6 type, name 4, 66.
};

// zu DNA structure 453, 632
struct LineStyleColorModifier_DistanceFromCamera
{
    LineStyleModifier    modifier;   // 1 type, name 628, 740.
    ColorBand            *color_ramp;// 2 type, name 54, 3966.
    float                range_min;  // 3 type, name 7, 3969.
    float                range_max;  // 4 type, name 7, 3970.
};

// zu DNA structure 454, 633
struct LineStyleAlphaModifier_DistanceFromCamera
{
    LineStyleModifier    modifier; // 1 type, name 628, 740.
    CurveMapping         *curve;   // 2 type, name 58, 3174.
    int                  flags;    // 3 type, name 4, 89.
    float                range_min;// 4 type, name 7, 3969.
    float                range_max;// 5 type, name 7, 3970.
    int                  pad;      // 6 type, name 4, 66.
};

// zu DNA structure 455, 634
struct LineStyleThicknessModifier_DistanceFromCamera
{
    LineStyleModifier    modifier; // 1 type, name 628, 740.
    CurveMapping         *curve;   // 2 type, name 58, 3174.
    int                  flags;    // 3 type, name 4, 89.
    float                range_min;// 4 type, name 7, 3969.
    float                range_max;// 5 type, name 7, 3970.
    float                value_min;// 6 type, name 7, 3967.
    float                value_max;// 7 type, name 7, 3968.
    int                  pad;      // 8 type, name 4, 66.
};

// zu DNA structure 456, 635
struct LineStyleColorModifier_DistanceFromObject
{
    LineStyleModifier    modifier;   // 1 type, name 628, 740.
    Object               *target;    // 2 type, name 29, 878.
    ColorBand            *color_ramp;// 3 type, name 54, 3966.
    float                range_min;  // 4 type, name 7, 3969.
    float                range_max;  // 5 type, name 7, 3970.
};

// zu DNA structure 457, 636
struct LineStyleAlphaModifier_DistanceFromObject
{
    LineStyleModifier    modifier; // 1 type, name 628, 740.
    Object               *target;  // 2 type, name 29, 878.
    CurveMapping         *curve;   // 3 type, name 58, 3174.
    int                  flags;    // 4 type, name 4, 89.
    float                range_min;// 5 type, name 7, 3969.
    float                range_max;// 6 type, name 7, 3970.
    int                  pad;      // 7 type, name 4, 66.
};

// zu DNA structure 458, 637
struct LineStyleThicknessModifier_DistanceFromObject
{
    LineStyleModifier    modifier; // 1 type, name 628, 740.
    Object               *target;  // 2 type, name 29, 878.
    CurveMapping         *curve;   // 3 type, name 58, 3174.
    int                  flags;    // 4 type, name 4, 89.
    float                range_min;// 5 type, name 7, 3969.
    float                range_max;// 6 type, name 7, 3970.
    float                value_min;// 7 type, name 7, 3967.
    float                value_max;// 8 type, name 7, 3968.
    int                  pad;      // 9 type, name 4, 66.
};

// zu DNA structure 459, 638
struct LineStyleColorModifier_Material
{
    LineStyleModifier    modifier;   // 1 type, name 628, 740.
    ColorBand            *color_ramp;// 2 type, name 54, 3966.
    int                  flags;      // 3 type, name 4, 89.
    int                  mat_attr;   // 4 type, name 4, 3971.
};

// zu DNA structure 460, 639
struct LineStyleAlphaModifier_Material
{
    LineStyleModifier    modifier;// 1 type, name 628, 740.
    CurveMapping         *curve;  // 2 type, name 58, 3174.
    int                  flags;   // 3 type, name 4, 89.
    int                  mat_attr;// 4 type, name 4, 3971.
};

// zu DNA structure 461, 640
struct LineStyleThicknessModifier_Material
{
    LineStyleModifier    modifier; // 1 type, name 628, 740.
    CurveMapping         *curve;   // 2 type, name 58, 3174.
    int                  flags;    // 3 type, name 4, 89.
    float                value_min;// 4 type, name 7, 3967.
    float                value_max;// 5 type, name 7, 3968.
    int                  mat_attr; // 6 type, name 4, 3971.
};

// zu DNA structure 462, 641
struct LineStyleGeometryModifier_Sampling
{
    LineStyleModifier    modifier;// 1 type, name 628, 740.
    float                sampling;// 2 type, name 7, 3972.
    int                  pad;     // 3 type, name 4, 66.
};

// zu DNA structure 463, 642
struct LineStyleGeometryModifier_BezierCurve
{
    LineStyleModifier    modifier;// 1 type, name 628, 740.
    float                error;   // 2 type, name 7, 3754.
    int                  pad;     // 3 type, name 4, 66.
};

// zu DNA structure 464, 643
struct LineStyleGeometryModifier_SinusDisplacement
{
    LineStyleModifier    modifier;  // 1 type, name 628, 740.
    float                wavelength;// 2 type, name 7, 3973.
    float                amplitude; // 3 type, name 7, 583.
    float                phase;     // 4 type, name 7, 3589.
    int                  pad;       // 5 type, name 4, 66.
};

// zu DNA structure 465, 644
struct LineStyleGeometryModifier_SpatialNoise
{
    LineStyleModifier    modifier; // 1 type, name 628, 740.
    float                amplitude;// 2 type, name 7, 583.
    float                scale;    // 3 type, name 7, 954.
    int                  octaves;  // 4 type, name 4, 3974.
    int                  flags;    // 5 type, name 4, 89.
};

// zu DNA structure 466, 645
struct LineStyleGeometryModifier_PerlinNoise1D
{
    LineStyleModifier    modifier; // 1 type, name 628, 740.
    float                frequency;// 2 type, name 7, 3975.
    float                amplitude;// 3 type, name 7, 583.
    float                angle;    // 4 type, name 7, 797.
    int                  octaves;  // 5 type, name 4, 3974.
    int                  seed;     // 6 type, name 4, 756.
    int                  pad1;     // 7 type, name 4, 68.
};

// zu DNA structure 467, 646
struct LineStyleGeometryModifier_PerlinNoise2D
{
    LineStyleModifier    modifier; // 1 type, name 628, 740.
    float                frequency;// 2 type, name 7, 3975.
    float                amplitude;// 3 type, name 7, 583.
    float                angle;    // 4 type, name 7, 797.
    int                  octaves;  // 5 type, name 4, 3974.
    int                  seed;     // 6 type, name 4, 756.
    int                  pad1;     // 7 type, name 4, 68.
};

// zu DNA structure 468, 647
struct LineStyleGeometryModifier_BackboneStretcher
{
    LineStyleModifier    modifier;       // 1 type, name 628, 740.
    float                backbone_length;// 2 type, name 7, 3976.
    int                  pad;            // 3 type, name 4, 66.
};

// zu DNA structure 469, 648
struct LineStyleGeometryModifier_TipRemover
{
    LineStyleModifier    modifier;  // 1 type, name 628, 740.
    float                tip_length;// 2 type, name 7, 3977.
    int                  pad;       // 3 type, name 4, 66.
};

// zu DNA structure 470, 649
struct LineStyleGeometryModifier_Polygonalization
{
    LineStyleModifier    modifier;// 1 type, name 628, 740.
    float                error;   // 2 type, name 7, 3754.
    int                  pad;     // 3 type, name 4, 66.
};

// zu DNA structure 471, 650
struct LineStyleGeometryModifier_GuidingLines
{
    LineStyleModifier    modifier;// 1 type, name 628, 740.
    float                offset;  // 2 type, name 7, 134.
    int                  pad;     // 3 type, name 4, 66.
};

// zu DNA structure 472, 651
struct LineStyleGeometryModifier_Blueprint
{
    LineStyleModifier    modifier;       // 1 type, name 628, 740.
    int                  flags;          // 2 type, name 4, 89.
    int                  rounds;         // 3 type, name 4, 3978.
    float                backbone_length;// 4 type, name 7, 3976.
    int                  random_radius;  // 5 type, name 4, 3979.
    int                  random_center;  // 6 type, name 4, 3980.
    int                  random_backbone;// 7 type, name 4, 3981.
};

// zu DNA structure 473, 652
struct LineStyleGeometryModifier_2DOffset
{
    LineStyleModifier    modifier;// 1 type, name 628, 740.
    float                start;   // 2 type, name 7, 753.
    float                end;     // 3 type, name 7, 2625.
    float                x;       // 4 type, name 7, 5.
    float                y;       // 5 type, name 7, 6.
};

// zu DNA structure 474, 653
struct LineStyleGeometryModifier_2DTransform
{
    LineStyleModifier    modifier;// 1 type, name 628, 740.
    int                  pivot;   // 2 type, name 4, 3982.
    float                scale_x; // 3 type, name 7, 3983.
    float                scale_y; // 4 type, name 7, 3984.
    float                angle;   // 5 type, name 7, 797.
    float                pivot_u; // 6 type, name 7, 3985.
    float                pivot_x; // 7 type, name 7, 3986.
    float                pivot_y; // 8 type, name 7, 3987.
    int                  pad;     // 9 type, name 4, 66.
};

// zu DNA structure 475, 654
struct LineStyleThicknessModifier_Calligraphy
{
    LineStyleModifier    modifier;     // 1 type, name 628, 740.
    float                min_thickness;// 2 type, name 7, 3988.
    float                max_thickness;// 3 type, name 7, 3989.
    float                orientation;  // 4 type, name 7, 3990.
    int                  pad;          // 5 type, name 4, 66.
};

// zu DNA structure 476, 21
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

// zu DNA structure 477, 30
struct IpoCurve
{
    IpoCurve     *next;    // 1 type, name 30, 0.
    IpoCurve     *prev;    // 2 type, name 30, 1.
    BPoint       *bp;      // 3 type, name 31, 48.
    BezTriple    *bezt;    // 4 type, name 32, 49.
    rctf         maxrct;   // 5 type, name 19, 50.
    rctf         totrct;   // 6 type, name 19, 51.
    short        blocktype;// 7 type, name 2, 45.
    short        adrcode;  // 8 type, name 2, 46.
    short        vartype;  // 9 type, name 2, 52.
    short        totvert;  // 10 type, name 2, 53.
    short        ipo;      // 11 type, name 2, 54.
    short        extrap;   // 12 type, name 2, 55.
    short        flag;     // 13 type, name 2, 18.
    short        rt;       // 14 type, name 2, 56.
    float        ymin;     // 15 type, name 7, 10.
    float        ymax;     // 16 type, name 7, 11.
    int          bitmask;  // 17 type, name 4, 57.
    float        slide_min;// 18 type, name 7, 58.
    float        slide_max;// 19 type, name 7, 59.
    float        curval;   // 20 type, name 7, 60.
    IpoDriver    *driver;  // 21 type, name 28, 61.
};

// zu DNA structure 478, 35
struct Key
{
    ID          id;         // 1 type, name 22, 31.
    AnimData    *adt;       // 2 type, name 36, 75.
    KeyBlock    *refkey;    // 3 type, name 34, 76.
    char        elemstr[32];// 4 type, name 0, 77.
    int         elemsize;   // 5 type, name 4, 78.
    int         pad;        // 6 type, name 4, 66.
    ListBase    block;      // 7 type, name 14, 79.
    Ipo         *ipo;       // 8 type, name 33, 80.
    ID          *from;      // 9 type, name 22, 81.
    int         totkey;     // 10 type, name 4, 82.
    short       flag;       // 11 type, name 2, 18.
    char        type;       // 12 type, name 0, 16.
    char        pad2;       // 13 type, name 0, 29.
    float       ctime;      // 14 type, name 7, 83.
    int         uidgen;     // 15 type, name 4, 84.
};

// zu DNA structure 479, 38
struct Text
{
    ID          id;       // 1 type, name 22, 31.
    char        *name;    // 2 type, name 0, 88.
    int         flags;    // 3 type, name 4, 89.
    int         nlines;   // 4 type, name 4, 90.
    ListBase    lines;    // 5 type, name 14, 91.
    TextLine    *curl;    // 6 type, name 37, 92.
    TextLine    *sell;    // 7 type, name 37, 93.
    int         curc;     // 8 type, name 4, 94.
    int         selc;     // 9 type, name 4, 95.
    char        *undo_buf;// 10 type, name 0, 96.
    int         undo_pos; // 11 type, name 4, 97.
    int         undo_len; // 12 type, name 4, 98.
    void        *compiled;// 13 type, name 11, 99.
    double      mtime;    // 14 type, name 8, 100.
};

// zu DNA structure 480, 42
struct Camera
{
    ID                id;            // 1 type, name 22, 31.
    AnimData          *adt;          // 2 type, name 36, 75.
    char              type;          // 3 type, name 0, 16.
    char              dtx;           // 4 type, name 0, 116.
    short             flag;          // 5 type, name 2, 18.
    float             passepartalpha;// 6 type, name 7, 117.
    float             clipsta;       // 7 type, name 7, 118.
    float             clipend;       // 8 type, name 7, 119.
    float             lens;          // 9 type, name 7, 120.
    float             ortho_scale;   // 10 type, name 7, 121.
    float             drawsize;      // 11 type, name 7, 122.
    float             sensor_x;      // 12 type, name 7, 123.
    float             sensor_y;      // 13 type, name 7, 124.
    float             shiftx;        // 14 type, name 7, 125.
    float             shifty;        // 15 type, name 7, 126.
    float             YF_dofdist;    // 16 type, name 7, 127.
    Ipo               *ipo;          // 17 type, name 33, 80.
    Object            *dof_ob;       // 18 type, name 29, 128.
    GPUDOFSettings    gpu_dof;       // 19 type, name 39, 129.
    char              sensor_fit;    // 20 type, name 0, 130.
    char              pad[7];        // 21 type, name 0, 115.
};

// zu DNA structure 481, 52
struct Tex
{
    ID              id;           // 1 type, name 22, 31.
    AnimData        *adt;         // 2 type, name 36, 75.
    float           noisesize;    // 3 type, name 7, 296.
    float           turbul;       // 4 type, name 7, 297.
    float           bright;       // 5 type, name 7, 298.
    float           contrast;     // 6 type, name 7, 299.
    float           saturation;   // 7 type, name 7, 300.
    float           rfac;         // 8 type, name 7, 301.
    float           gfac;         // 9 type, name 7, 302.
    float           bfac;         // 10 type, name 7, 303.
    float           filtersize;   // 11 type, name 7, 304.
    float           pad2;         // 12 type, name 7, 29.
    float           mg_H;         // 13 type, name 7, 305.
    float           mg_lacunarity;// 14 type, name 7, 306.
    float           mg_octaves;   // 15 type, name 7, 307.
    float           mg_offset;    // 16 type, name 7, 308.
    float           mg_gain;      // 17 type, name 7, 309.
    float           dist_amount;  // 18 type, name 7, 310.
    float           ns_outscale;  // 19 type, name 7, 311.
    float           vn_w1;        // 20 type, name 7, 312.
    float           vn_w2;        // 21 type, name 7, 313.
    float           vn_w3;        // 22 type, name 7, 314.
    float           vn_w4;        // 23 type, name 7, 315.
    float           vn_mexp;      // 24 type, name 7, 316.
    short           vn_distm;     // 25 type, name 2, 317.
    short           vn_coltype;   // 26 type, name 2, 318.
    short           noisedepth;   // 27 type, name 2, 319.
    short           noisetype;    // 28 type, name 2, 320.
    short           noisebasis;   // 29 type, name 2, 321.
    short           noisebasis2;  // 30 type, name 2, 322.
    short           imaflag;      // 31 type, name 2, 323.
    short           flag;         // 32 type, name 2, 18.
    short           type;         // 33 type, name 2, 16.
    short           stype;        // 34 type, name 2, 252.
    float           cropxmin;     // 35 type, name 7, 324.
    float           cropymin;     // 36 type, name 7, 325.
    float           cropxmax;     // 37 type, name 7, 326.
    float           cropymax;     // 38 type, name 7, 327.
    int             texfilter;    // 39 type, name 4, 328.
    int             afmax;        // 40 type, name 4, 329.
    short           xrepeat;      // 41 type, name 2, 330.
    short           yrepeat;      // 42 type, name 2, 331.
    short           extend;       // 43 type, name 2, 284.
    short           fie_ima;      // 44 type, name 2, 136.
    int             len;          // 45 type, name 4, 22.
    int             frames;       // 46 type, name 4, 133.
    int             offset;       // 47 type, name 4, 134.
    int             sfra;         // 48 type, name 4, 135.
    float           checkerdist;  // 49 type, name 7, 332.
    float           nabla;        // 50 type, name 7, 333.
    float           pad1;         // 51 type, name 7, 68.
    ImageUser       iuser;        // 52 type, name 43, 334.
    bNodeTree       *nodetree;    // 53 type, name 61, 335.
    Ipo             *ipo;         // 54 type, name 33, 80.
    Image           *ima;         // 55 type, name 46, 248.
    ColorBand       *coba;        // 56 type, name 54, 279.
    EnvMap          *env;         // 57 type, name 55, 336.
    PreviewImage    *preview;     // 58 type, name 26, 159.
    PointDensity    *pd;          // 59 type, name 57, 337.
    VoxelData       *vd;          // 60 type, name 59, 338.
    OceanTex        *ot;          // 61 type, name 60, 339.
    char            use_nodes;    // 62 type, name 0, 340.
    char            pad[7];       // 63 type, name 0, 115.
};

// zu DNA structure 482, 63
struct ColorMapping
{
    ColorBand    coba;          // 1 type, name 54, 346.
    float        bright;        // 2 type, name 7, 298.
    float        contrast;      // 3 type, name 7, 299.
    float        saturation;    // 4 type, name 7, 300.
    int          flag;          // 5 type, name 4, 18.
    float        blend_color[3];// 6 type, name 7, 347.
    float        blend_factor;  // 7 type, name 7, 348.
    int          blend_type;    // 8 type, name 4, 349.
    int          pad[3];        // 9 type, name 4, 350.
};

// zu DNA structure 483, 140
struct ClothModifierData
{
    ModifierData         modifier;          // 1 type, name 116, 740.
    Scene                *scene;            // 2 type, name 44, 131.
    void                 *clothObject;      // Note: using void* on undefined DNA type: Cloth
    ClothSimSettings     *sim_parms;        // 4 type, name 142, 821.
    ClothCollSettings    *coll_parms;       // 5 type, name 143, 822.
    PointCache           *point_cache;      // 6 type, name 144, 823.
    ListBase             ptcaches;          // 7 type, name 14, 824.
    void                 *hairdata;         // Note: using void* on undefined DNA type: ClothHairData
    float                hair_grid_min[3];  // 9 type, name 7, 826.
    float                hair_grid_max[3];  // 10 type, name 7, 827.
    int                  hair_grid_res[3];  // 11 type, name 4, 828.
    float                hair_grid_cellsize;// 12 type, name 7, 829.
    void                 *solver_result;    // Note: using void* on undefined DNA type: ClothSolverResult
};

// zu DNA structure 484, 216
struct World
{
    ID              id;                  // 1 type, name 22, 31.
    AnimData        *adt;                // 2 type, name 36, 75.
    short           colormodel;          // 3 type, name 2, 193.
    short           totex;               // 4 type, name 2, 352.
    short           texact;              // 5 type, name 2, 387.
    short           mistype;             // 6 type, name 2, 1321.
    float           horr;                // 7 type, name 7, 1322.
    float           horg;                // 8 type, name 7, 1323.
    float           horb;                // 9 type, name 7, 1324.
    float           zenr;                // 10 type, name 7, 1325.
    float           zeng;                // 11 type, name 7, 1326.
    float           zenb;                // 12 type, name 7, 1327.
    float           ambr;                // 13 type, name 7, 438.
    float           ambg;                // 14 type, name 7, 440.
    float           ambb;                // 15 type, name 7, 439.
    float           exposure;            // 16 type, name 7, 1328.
    float           exp;                 // 17 type, name 7, 1329.
    float           range;               // 18 type, name 7, 1330.
    float           linfac;              // 19 type, name 7, 1331.
    float           logfac;              // 20 type, name 7, 1332.
    float           gravity;             // 21 type, name 7, 1333.
    float           activityBoxRadius;   // 22 type, name 7, 1334.
    short           skytype;             // 23 type, name 2, 1335.
    short           mode;                // 24 type, name 2, 351.
    short           occlusionRes;        // 25 type, name 2, 1336.
    short           physicsEngine;       // 26 type, name 2, 1337.
    short           ticrate;             // 27 type, name 2, 1338.
    short           maxlogicstep;        // 28 type, name 2, 1339.
    short           physubstep;          // 29 type, name 2, 1340.
    short           maxphystep;          // 30 type, name 2, 1341.
    float           misi;                // 31 type, name 7, 1342.
    float           miststa;             // 32 type, name 7, 1343.
    float           mistdist;            // 33 type, name 7, 1344.
    float           misthi;              // 34 type, name 7, 1345.
    float           starr;               // 35 type, name 7, 1346.
    float           starg;               // 36 type, name 7, 1347.
    float           starb;               // 37 type, name 7, 1348.
    float           stark;               // 38 type, name 7, 1349.
    float           starsize;            // 39 type, name 7, 1350.
    float           starmindist;         // 40 type, name 7, 1351.
    float           stardist;            // 41 type, name 7, 1352.
    float           starcolnoise;        // 42 type, name 7, 1353.
    short           dofsta;              // 43 type, name 2, 1354.
    short           dofend;              // 44 type, name 2, 1355.
    short           dofmin;              // 45 type, name 2, 1356.
    short           dofmax;              // 46 type, name 2, 1357.
    float           aodist;              // 47 type, name 7, 1358.
    float           aodistfac;           // 48 type, name 7, 1359.
    float           aoenergy;            // 49 type, name 7, 1360.
    float           aobias;              // 50 type, name 7, 1361.
    short           aomode;              // 51 type, name 2, 1362.
    short           aosamp;              // 52 type, name 2, 1363.
    short           aomix;               // 53 type, name 2, 1364.
    short           aocolor;             // 54 type, name 2, 1365.
    float           ao_adapt_thresh;     // 55 type, name 7, 1366.
    float           ao_adapt_speed_fac;  // 56 type, name 7, 1367.
    float           ao_approx_error;     // 57 type, name 7, 1368.
    float           ao_approx_correction;// 58 type, name 7, 1369.
    float           ao_indirect_energy;  // 59 type, name 7, 1370.
    float           ao_env_energy;       // 60 type, name 7, 1371.
    float           ao_pad2;             // 61 type, name 7, 1372.
    short           ao_indirect_bounces; // 62 type, name 2, 1373.
    short           ao_pad;              // 63 type, name 2, 1374.
    short           ao_samp_method;      // 64 type, name 2, 1375.
    short           ao_gather_method;    // 65 type, name 2, 1376.
    short           ao_approx_passes;    // 66 type, name 2, 1377.
    short           flag;                // 67 type, name 2, 18.
    float           *aosphere;           // 68 type, name 7, 1378.
    float           *aotables;           // 69 type, name 7, 1379.
    Ipo             *ipo;                // 70 type, name 33, 80.
    MTex            *mtex[18];           // 71 type, name 51, 406.
    short           pr_texture;          // 72 type, name 2, 407.
    short           use_nodes;           // 73 type, name 2, 340.
    short           pad[2];              // 74 type, name 2, 187.
    PreviewImage    *preview;            // 75 type, name 26, 159.
    bNodeTree       *nodetree;           // 76 type, name 61, 335.
    ListBase        gpumaterial;         // 77 type, name 14, 548.
};

// zu DNA structure 485, 225
struct ImageFormatData
{
    char                           imtype;          // 1 type, name 0, 1442.
    char                           depth;           // 2 type, name 0, 256.
    char                           planes;          // 3 type, name 0, 1443.
    char                           flag;            // 4 type, name 0, 18.
    char                           quality;         // 5 type, name 0, 1444.
    char                           compress;        // 6 type, name 0, 1445.
    char                           exr_codec;       // 7 type, name 0, 1446.
    char                           cineon_flag;     // 8 type, name 0, 1447.
    short                          cineon_white;    // 9 type, name 2, 1448.
    short                          cineon_black;    // 10 type, name 2, 1449.
    float                          cineon_gamma;    // 11 type, name 7, 1450.
    char                           jp2_flag;        // 12 type, name 0, 1451.
    char                           jp2_codec;       // 13 type, name 0, 1452.
    char                           pad[6];          // 14 type, name 0, 752.
    ColorManagedViewSettings       view_settings;   // 15 type, name 226, 1453.
    ColorManagedDisplaySettings    display_settings;// 16 type, name 227, 1454.
};

// zu DNA structure 486, 259
struct BGpic
{
    BGpic            *next;   // 1 type, name 259, 0.
    BGpic            *prev;   // 2 type, name 259, 1.
    Image            *ima;    // 3 type, name 46, 248.
    ImageUser        iuser;   // 4 type, name 43, 334.
    MovieClip        *clip;   // 5 type, name 257, 1792.
    MovieClipUser    cuser;   // 6 type, name 260, 1796.
    float            xof;     // 7 type, name 7, 634.
    float            yof;     // 8 type, name 7, 635.
    float            size;    // 9 type, name 7, 101.
    float            blend;   // 10 type, name 7, 1797.
    float            rotation;// 11 type, name 7, 1798.
    short            view;    // 12 type, name 2, 1799.
    short            flag;    // 13 type, name 2, 18.
    short            source;  // 14 type, name 2, 149.
    char             pad[6];  // 15 type, name 0, 752.
};

// zu DNA structure 487, 314
struct ThemeSpace
{
    char                back[4];                       // 1 type, name 0, 2086.
    char                title[4];                      // 2 type, name 0, 2120.
    char                text[4];                       // 3 type, name 0, 2073.
    char                text_hi[4];                    // 4 type, name 0, 2121.
    char                header[4];                     // 5 type, name 0, 2085.
    char                header_title[4];               // 6 type, name 0, 2122.
    char                header_text[4];                // 7 type, name 0, 2123.
    char                header_text_hi[4];             // 8 type, name 0, 2124.
    char                tab_active[4];                 // 9 type, name 0, 2125.
    char                tab_inactive[4];               // 10 type, name 0, 2126.
    char                tab_back[4];                   // 11 type, name 0, 2127.
    char                tab_outline[4];                // 12 type, name 0, 2128.
    char                button[4];                     // 13 type, name 0, 2129.
    char                button_title[4];               // 14 type, name 0, 2130.
    char                button_text[4];                // 15 type, name 0, 2131.
    char                button_text_hi[4];             // 16 type, name 0, 2132.
    char                list[4];                       // 17 type, name 0, 2133.
    char                list_title[4];                 // 18 type, name 0, 2134.
    char                list_text[4];                  // 19 type, name 0, 2135.
    char                list_text_hi[4];               // 20 type, name 0, 2136.
    uiPanelColors       panelcolors;                   // 21 type, name 311, 2137.
    uiGradientColors    gradients;                     // 22 type, name 312, 2138.
    char                shade1[4];                     // 23 type, name 0, 2139.
    char                shade2[4];                     // 24 type, name 0, 2140.
    char                hilite[4];                     // 25 type, name 0, 2141.
    char                grid[4];                       // 26 type, name 0, 2142.
    char                view_overlay[4];               // 27 type, name 0, 2143.
    char                wire[4];                       // 28 type, name 0, 2144.
    char                wire_edit[4];                  // 29 type, name 0, 2145.
    char                select[4];                     // 30 type, name 0, 2146.
    char                lamp[4];                       // 31 type, name 0, 2147.
    char                speaker[4];                    // 32 type, name 0, 2148.
    char                empty[4];                      // 33 type, name 0, 2149.
    char                camera[4];                     // 34 type, name 0, 2150.
    char                active[4];                     // 35 type, name 0, 2151.
    char                group[4];                      // 36 type, name 0, 2152.
    char                group_active[4];               // 37 type, name 0, 2153.
    char                transform[4];                  // 38 type, name 0, 2154.
    char                vertex[4];                     // 39 type, name 0, 2155.
    char                vertex_select[4];              // 40 type, name 0, 2156.
    char                vertex_unreferenced[4];        // 41 type, name 0, 2157.
    char                edge[4];                       // 42 type, name 0, 2158.
    char                edge_select[4];                // 43 type, name 0, 2159.
    char                edge_seam[4];                  // 44 type, name 0, 2160.
    char                edge_sharp[4];                 // 45 type, name 0, 2161.
    char                edge_facesel[4];               // 46 type, name 0, 2162.
    char                edge_crease[4];                // 47 type, name 0, 2163.
    char                face[4];                       // 48 type, name 0, 2164.
    char                face_select[4];                // 49 type, name 0, 2165.
    char                face_dot[4];                   // 50 type, name 0, 2166.
    char                extra_edge_len[4];             // 51 type, name 0, 2167.
    char                extra_edge_angle[4];           // 52 type, name 0, 2168.
    char                extra_face_angle[4];           // 53 type, name 0, 2169.
    char                extra_face_area[4];            // 54 type, name 0, 2170.
    char                normal[4];                     // 55 type, name 0, 2171.
    char                vertex_normal[4];              // 56 type, name 0, 2172.
    char                loop_normal[4];                // 57 type, name 0, 2173.
    char                bone_solid[4];                 // 58 type, name 0, 2174.
    char                bone_pose[4];                  // 59 type, name 0, 2175.
    char                bone_pose_active[4];           // 60 type, name 0, 2176.
    char                strip[4];                      // 61 type, name 0, 2177.
    char                strip_select[4];               // 62 type, name 0, 2178.
    char                cframe[4];                     // 63 type, name 0, 2179.
    char                time_keyframe[4];              // 64 type, name 0, 2180.
    char                time_gp_keyframe[4];           // 65 type, name 0, 2181.
    char                freestyle_edge_mark[4];        // 66 type, name 0, 2182.
    char                freestyle_face_mark[4];        // 67 type, name 0, 2183.
    char                nurb_uline[4];                 // 68 type, name 0, 2184.
    char                nurb_vline[4];                 // 69 type, name 0, 2185.
    char                act_spline[4];                 // 70 type, name 0, 2186.
    char                nurb_sel_uline[4];             // 71 type, name 0, 2187.
    char                nurb_sel_vline[4];             // 72 type, name 0, 2188.
    char                lastsel_point[4];              // 73 type, name 0, 2189.
    char                handle_free[4];                // 74 type, name 0, 2190.
    char                handle_auto[4];                // 75 type, name 0, 2191.
    char                handle_vect[4];                // 76 type, name 0, 2192.
    char                handle_align[4];               // 77 type, name 0, 2193.
    char                handle_auto_clamped[4];        // 78 type, name 0, 2194.
    char                handle_sel_free[4];            // 79 type, name 0, 2195.
    char                handle_sel_auto[4];            // 80 type, name 0, 2196.
    char                handle_sel_vect[4];            // 81 type, name 0, 2197.
    char                handle_sel_align[4];           // 82 type, name 0, 2198.
    char                handle_sel_auto_clamped[4];    // 83 type, name 0, 2199.
    char                ds_channel[4];                 // 84 type, name 0, 2200.
    char                ds_subchannel[4];              // 85 type, name 0, 2201.
    char                keytype_keyframe[4];           // 86 type, name 0, 2202.
    char                keytype_extreme[4];            // 87 type, name 0, 2203.
    char                keytype_breakdown[4];          // 88 type, name 0, 2204.
    char                keytype_jitter[4];             // 89 type, name 0, 2205.
    char                keytype_keyframe_select[4];    // 90 type, name 0, 2206.
    char                keytype_extreme_select[4];     // 91 type, name 0, 2207.
    char                keytype_breakdown_select[4];   // 92 type, name 0, 2208.
    char                keytype_jitter_select[4];      // 93 type, name 0, 2209.
    char                keyborder[4];                  // 94 type, name 0, 2210.
    char                keyborder_select[4];           // 95 type, name 0, 2211.
    char                console_output[4];             // 96 type, name 0, 2212.
    char                console_input[4];              // 97 type, name 0, 2213.
    char                console_info[4];               // 98 type, name 0, 2214.
    char                console_error[4];              // 99 type, name 0, 2215.
    char                console_cursor[4];             // 100 type, name 0, 2216.
    char                console_select[4];             // 101 type, name 0, 2217.
    char                vertex_size;                   // 102 type, name 0, 2218.
    char                outline_width;                 // 103 type, name 0, 2219.
    char                facedot_size;                  // 104 type, name 0, 2220.
    char                noodle_curving;                // 105 type, name 0, 2221.
    char                syntaxl[4];                    // 106 type, name 0, 2222.
    char                syntaxs[4];                    // 107 type, name 0, 2223.
    char                syntaxb[4];                    // 108 type, name 0, 2224.
    char                syntaxn[4];                    // 109 type, name 0, 2225.
    char                syntaxv[4];                    // 110 type, name 0, 2226.
    char                syntaxc[4];                    // 111 type, name 0, 2227.
    char                syntaxd[4];                    // 112 type, name 0, 2228.
    char                syntaxr[4];                    // 113 type, name 0, 2229.
    char                nodeclass_output[4];           // 114 type, name 0, 2230.
    char                nodeclass_filter[4];           // 115 type, name 0, 2231.
    char                nodeclass_vector[4];           // 116 type, name 0, 2232.
    char                nodeclass_texture[4];          // 117 type, name 0, 2233.
    char                nodeclass_shader[4];           // 118 type, name 0, 2234.
    char                nodeclass_script[4];           // 119 type, name 0, 2235.
    char                nodeclass_pattern[4];          // 120 type, name 0, 2236.
    char                nodeclass_layout[4];           // 121 type, name 0, 2237.
    char                movie[4];                      // 122 type, name 0, 2238.
    char                movieclip[4];                  // 123 type, name 0, 2239.
    char                mask[4];                       // 124 type, name 0, 2240.
    char                image[4];                      // 125 type, name 0, 2241.
    char                scene[4];                      // 126 type, name 0, 2242.
    char                audio[4];                      // 127 type, name 0, 2243.
    char                effect[4];                     // 128 type, name 0, 2244.
    char                transition[4];                 // 129 type, name 0, 2245.
    char                meta[4];                       // 130 type, name 0, 2246.
    char                editmesh_active[4];            // 131 type, name 0, 2247.
    char                handle_vertex[4];              // 132 type, name 0, 2248.
    char                handle_vertex_select[4];       // 133 type, name 0, 2249.
    char                handle_vertex_size;            // 134 type, name 0, 2250.
    char                clipping_border_3d[4];         // 135 type, name 0, 2251.
    char                marker_outline[4];             // 136 type, name 0, 2252.
    char                marker[4];                     // 137 type, name 0, 2253.
    char                act_marker[4];                 // 138 type, name 0, 2254.
    char                sel_marker[4];                 // 139 type, name 0, 2255.
    char                dis_marker[4];                 // 140 type, name 0, 2256.
    char                lock_marker[4];                // 141 type, name 0, 2257.
    char                bundle_solid[4];               // 142 type, name 0, 2258.
    char                path_before[4];                // 143 type, name 0, 2259.
    char                path_after[4];                 // 144 type, name 0, 2260.
    char                camera_path[4];                // 145 type, name 0, 2261.
    char                hpad[2];                       // 146 type, name 0, 2262.
    char                gp_vertex_size;                // 147 type, name 0, 2263.
    char                gp_vertex[4];                  // 148 type, name 0, 2264.
    char                gp_vertex_select[4];           // 149 type, name 0, 2265.
    char                preview_back[4];               // 150 type, name 0, 2266.
    char                preview_stitch_face[4];        // 151 type, name 0, 2267.
    char                preview_stitch_edge[4];        // 152 type, name 0, 2268.
    char                preview_stitch_vert[4];        // 153 type, name 0, 2269.
    char                preview_stitch_stitchable[4];  // 154 type, name 0, 2270.
    char                preview_stitch_unstitchable[4];// 155 type, name 0, 2271.
    char                preview_stitch_active[4];      // 156 type, name 0, 2272.
    char                uv_shadow[4];                  // 157 type, name 0, 2273.
    char                uv_others[4];                  // 158 type, name 0, 2274.
    char                match[4];                      // 159 type, name 0, 2275.
    char                selected_highlight[4];         // 160 type, name 0, 2276.
    char                skin_root[4];                  // 161 type, name 0, 2277.
    char                anim_active[4];                // 162 type, name 0, 2278.
    char                anim_non_active[4];            // 163 type, name 0, 2279.
    char                nla_tweaking[4];               // 164 type, name 0, 2280.
    char                nla_tweakdupli[4];             // 165 type, name 0, 2281.
    char                nla_transition[4];             // 166 type, name 0, 2282.
    char                nla_transition_sel[4];         // 167 type, name 0, 2283.
    char                nla_meta[4];                   // 168 type, name 0, 2284.
    char                nla_meta_sel[4];               // 169 type, name 0, 2285.
    char                nla_sound[4];                  // 170 type, name 0, 2286.
    char                nla_sound_sel[4];              // 171 type, name 0, 2287.
    char                info_selected[4];              // 172 type, name 0, 2288.
    char                info_selected_text[4];         // 173 type, name 0, 2289.
    char                info_error[4];                 // 174 type, name 0, 2290.
    char                info_error_text[4];            // 175 type, name 0, 2291.
    char                info_warning[4];               // 176 type, name 0, 2292.
    char                info_warning_text[4];          // 177 type, name 0, 2293.
    char                info_info[4];                  // 178 type, name 0, 2294.
    char                info_info_text[4];             // 179 type, name 0, 2295.
    char                info_debug[4];                 // 180 type, name 0, 2296.
    char                info_debug_text[4];            // 181 type, name 0, 2297.
    char                paint_curve_pivot[4];          // 182 type, name 0, 2298.
    char                paint_curve_handle[4];         // 183 type, name 0, 2299.
};

// zu DNA structure 488, 344
struct Sequence
{
    Sequence     *next;          // 1 type, name 344, 0.
    Sequence     *prev;          // 2 type, name 344, 1.
    void         *tmp;           // 3 type, name 11, 2546.
    void         *lib;           // 4 type, name 11, 25.
    char         name[64];       // 5 type, name 0, 19.
    int          flag;           // 6 type, name 4, 18.
    int          type;           // 7 type, name 4, 16.
    int          len;            // 8 type, name 4, 22.
    int          start;          // 9 type, name 4, 753.
    int          startofs;       // 10 type, name 4, 2547.
    int          endofs;         // 11 type, name 4, 2548.
    int          startstill;     // 12 type, name 4, 2540.
    int          endstill;       // 13 type, name 4, 2541.
    int          machine;        // 14 type, name 4, 2549.
    int          depth;          // 15 type, name 4, 256.
    int          startdisp;      // 16 type, name 4, 2550.
    int          enddisp;        // 17 type, name 4, 2551.
    float        sat;            // 18 type, name 7, 2552.
    float        mul;            // 19 type, name 7, 2553.
    float        handsize;       // 20 type, name 7, 2554.
    short        anim_preseek;   // 21 type, name 2, 2555.
    short        streamindex;    // 22 type, name 2, 2556.
    int          multicam_source;// 23 type, name 4, 2557.
    int          clip_flag;      // 24 type, name 4, 2558.
    Strip        *strip;         // 25 type, name 343, 2559.
    Ipo          *ipo;           // 26 type, name 33, 80.
    Scene        *scene;         // 27 type, name 44, 131.
    Object       *scene_camera;  // 28 type, name 29, 2560.
    MovieClip    *clip;          // 29 type, name 257, 1792.
    Mask         *mask;          // 30 type, name 284, 1931.
    void         *anim;          // Note: using void* on undefined DNA type: anim
    float        effect_fader;   // 32 type, name 7, 2561.
    float        speed_fader;    // 33 type, name 7, 2562.
    Sequence     *seq1;          // 34 type, name 344, 2563.
    Sequence     *seq2;          // 35 type, name 344, 2564.
    Sequence     *seq3;          // 36 type, name 344, 2565.
    ListBase     seqbase;        // 37 type, name 14, 2566.
    bSound       *sound;         // 38 type, name 345, 2567.
    void         *scene_sound;   // 39 type, name 11, 2568.
    float        volume;         // 40 type, name 7, 1197.
    float        pitch;          // 41 type, name 7, 2569.
    float        pan;            // 42 type, name 7, 2570.
    float        strobe;         // 43 type, name 7, 2571.
    void         *effectdata;    // 44 type, name 11, 2572.
    int          anim_startofs;  // 45 type, name 4, 2573.
    int          anim_endofs;    // 46 type, name 4, 2574.
    int          blend_mode;     // 47 type, name 4, 2575.
    float        blend_opacity;  // 48 type, name 7, 2576.
    int          sfra;           // 49 type, name 4, 135.
    char         alpha_mode;     // 50 type, name 0, 172.
    char         pad[3];         // 51 type, name 0, 350.
    ListBase     modifiers;      // 52 type, name 14, 1037.
};

// zu DNA structure 489, 354
struct ColorBalanceModifierData
{
    SequenceModifierData    modifier;      // 1 type, name 353, 740.
    StripColorBalance       color_balance; // 2 type, name 341, 2613.
    float                   color_multiply;// 3 type, name 7, 2614.
};

// zu DNA structure 490, 69
struct Group
{
    ID          id;          // 1 type, name 22, 31.
    ListBase    gobject;     // 2 type, name 14, 2771.
    int         layer;       // 3 type, name 4, 140.
    float       dupli_ofs[3];// 4 type, name 7, 2772.
};

// zu DNA structure 491, 415
struct bActionGroup
{
    bActionGroup      *next;    // 1 type, name 415, 0.
    bActionGroup      *prev;    // 2 type, name 415, 1.
    ListBase          channels; // 3 type, name 14, 2869.
    int               flag;     // 4 type, name 4, 18.
    int               customCol;// 5 type, name 4, 2870.
    char              name[64]; // 6 type, name 0, 19.
    ThemeWireColor    cs;       // 7 type, name 315, 2871.
};

// zu DNA structure 492, 459
struct bNodePreview
{
    bNodeInstanceHashEntry    hash_entry;// 1 type, name 458, 3012.
    char                      *rect;     // 2 type, name 0, 3013.
    short                     xsize;     // 3 type, name 2, 3014.
    short                     ysize;     // 4 type, name 2, 3015.
    int                       pad;       // 5 type, name 4, 66.
};

// zu DNA structure 493, 540
struct ParticleData
{
    ParticleKey     state;      // 1 type, name 533, 1116.
    ParticleKey     prev_state; // 2 type, name 533, 3290.
    HairKey         *hair;      // 3 type, name 532, 3291.
    ParticleKey     *keys;      // 4 type, name 533, 2650.
    BoidParticle    *boid;      // 5 type, name 534, 3292.
    int             totkey;     // 6 type, name 4, 82.
    float           time;       // 7 type, name 7, 784.
    float           lifetime;   // 8 type, name 7, 810.
    float           dietime;    // 9 type, name 7, 3293.
    int             num;        // 10 type, name 4, 3284.
    int             num_dmcache;// 11 type, name 4, 3294.
    float           fuv[4];     // 12 type, name 7, 3288.
    float           foffset;    // 13 type, name 7, 3289.
    float           size;       // 14 type, name 7, 101.
    float           sphdensity; // 15 type, name 7, 3295.
    int             pad;        // 16 type, name 4, 66.
    int             hair_index; // 17 type, name 4, 3296.
    short           flag;       // 18 type, name 2, 18.
    short           alive;      // 19 type, name 2, 3297.
};

// zu DNA structure 494, 542
struct ParticleSettings
{
    ID                  id;                 // 1 type, name 22, 31.
    AnimData            *adt;               // 2 type, name 36, 75.
    BoidSettings        *boids;             // 3 type, name 543, 3310.
    SPHFluidSettings    *fluid;             // 4 type, name 541, 3311.
    EffectorWeights     *effector_weights;  // 5 type, name 207, 1267.
    int                 flag;               // 6 type, name 4, 18.
    int                 rt;                 // 7 type, name 4, 56.
    short               type;               // 8 type, name 2, 16.
    short               from;               // 9 type, name 2, 2931.
    short               distr;              // 10 type, name 2, 3312.
    short               texact;             // 11 type, name 2, 387.
    short               phystype;           // 12 type, name 2, 3313.
    short               rotmode;            // 13 type, name 2, 1083.
    short               avemode;            // 14 type, name 2, 3314.
    short               reactevent;         // 15 type, name 2, 3315.
    int                 draw;               // 16 type, name 4, 3316.
    int                 pad1;               // 17 type, name 4, 68.
    short               draw_as;            // 18 type, name 2, 3317.
    short               draw_size;          // 19 type, name 2, 3318.
    short               childtype;          // 20 type, name 2, 3319.
    short               pad2;               // 21 type, name 2, 29.
    short               ren_as;             // 22 type, name 2, 3320.
    short               subframes;          // 23 type, name 2, 3321.
    short               draw_col;           // 24 type, name 2, 3322.
    short               draw_step;          // 25 type, name 2, 1628.
    short               ren_step;           // 26 type, name 2, 3323.
    short               hair_step;          // 27 type, name 2, 3324.
    short               keys_step;          // 28 type, name 2, 3325.
    short               adapt_angle;        // 29 type, name 2, 3326.
    short               adapt_pix;          // 30 type, name 2, 3327.
    short               disp;               // 31 type, name 2, 563.
    short               omat;               // 32 type, name 2, 2643.
    short               interpolation;      // 33 type, name 2, 2603.
    short               integrator;         // 34 type, name 2, 3328.
    short               rotfrom;            // 35 type, name 2, 3329.
    short               kink;               // 36 type, name 2, 1138.
    short               kink_axis;          // 37 type, name 2, 1139.
    short               bb_align;           // 38 type, name 2, 3330.
    short               bb_uv_split;        // 39 type, name 2, 3331.
    short               bb_anim;            // 40 type, name 2, 3332.
    short               bb_split_offset;    // 41 type, name 2, 3333.
    float               bb_tilt;            // 42 type, name 7, 3334.
    float               bb_rand_tilt;       // 43 type, name 7, 3335.
    float               bb_offset[2];       // 44 type, name 7, 3336.
    float               bb_size[2];         // 45 type, name 7, 3337.
    float               bb_vel_head;        // 46 type, name 7, 3338.
    float               bb_vel_tail;        // 47 type, name 7, 3339.
    float               color_vec_max;      // 48 type, name 7, 3340.
    short               simplify_flag;      // 49 type, name 2, 1541.
    short               simplify_refsize;   // 50 type, name 2, 3341.
    float               simplify_rate;      // 51 type, name 7, 3342.
    float               simplify_transition;// 52 type, name 7, 3343.
    float               simplify_viewport;  // 53 type, name 7, 3344.
    float               sta;                // 54 type, name 7, 2624.
    float               end;                // 55 type, name 7, 2625.
    float               lifetime;           // 56 type, name 7, 810.
    float               randlife;           // 57 type, name 7, 2631.
    float               timetweak;          // 58 type, name 7, 3345.
    float               courant_target;     // 59 type, name 7, 3346.
    float               jitfac;             // 60 type, name 7, 3347.
    float               eff_hair;           // 61 type, name 7, 3348.
    float               grid_rand;          // 62 type, name 7, 3349.
    float               ps_offset[1];       // 63 type, name 7, 3350.
    int                 totpart;            // 64 type, name 4, 2626.
    int                 userjit;            // 65 type, name 4, 2623.
    int                 grid_res;           // 66 type, name 4, 3351.
    int                 effector_amount;    // 67 type, name 4, 3352.
    short               time_flag;          // 68 type, name 2, 3353.
    short               time_pad[3];        // 69 type, name 2, 3354.
    float               normfac;            // 70 type, name 7, 2627.
    float               obfac;              // 71 type, name 7, 2628.
    float               randfac;            // 72 type, name 7, 2629.
    float               partfac;            // 73 type, name 7, 3355.
    float               tanfac;             // 74 type, name 7, 3356.
    float               tanphase;           // 75 type, name 7, 3357.
    float               reactfac;           // 76 type, name 7, 3358.
    float               ob_vel[3];          // 77 type, name 7, 3359.
    float               avefac;             // 78 type, name 7, 3360.
    float               phasefac;           // 79 type, name 7, 3361.
    float               randrotfac;         // 80 type, name 7, 3362.
    float               randphasefac;       // 81 type, name 7, 3363.
    float               mass;               // 82 type, name 7, 1069.
    float               size;               // 83 type, name 7, 101.
    float               randsize;           // 84 type, name 7, 3364.
    float               acc[3];             // 85 type, name 7, 3365.
    float               dragfac;            // 86 type, name 7, 3366.
    float               brownfac;           // 87 type, name 7, 3367.
    float               dampfac;            // 88 type, name 7, 227.
    float               randlength;         // 89 type, name 7, 3368.
    int                 child_flag;         // 90 type, name 4, 3369.
    int                 pad3;               // 91 type, name 4, 517.
    int                 child_nbr;          // 92 type, name 4, 3370.
    int                 ren_child_nbr;      // 93 type, name 4, 3371.
    float               parents;            // 94 type, name 7, 3372.
    float               childsize;          // 95 type, name 7, 3373.
    float               childrandsize;      // 96 type, name 7, 3374.
    float               childrad;           // 97 type, name 7, 3375.
    float               childflat;          // 98 type, name 7, 3376.
    float               clumpfac;           // 99 type, name 7, 226.
    float               clumppow;           // 100 type, name 7, 3377.
    float               kink_amp;           // 101 type, name 7, 1165.
    float               kink_freq;          // 102 type, name 7, 1163.
    float               kink_shape;         // 103 type, name 7, 1164.
    float               kink_flat;          // 104 type, name 7, 3378.
    float               kink_amp_clump;     // 105 type, name 7, 3379.
    int                 kink_extra_steps;   // 106 type, name 4, 3380.
    int                 pad4;               // 107 type, name 4, 1499.
    float               kink_axis_random;   // 108 type, name 7, 3381.
    float               kink_amp_random;    // 109 type, name 7, 3382.
    float               rough1;             // 110 type, name 7, 3383.
    float               rough1_size;        // 111 type, name 7, 3384.
    float               rough2;             // 112 type, name 7, 3385.
    float               rough2_size;        // 113 type, name 7, 3386.
    float               rough2_thres;       // 114 type, name 7, 3387.
    float               rough_end;          // 115 type, name 7, 3388.
    float               rough_end_shape;    // 116 type, name 7, 3389.
    float               clength;            // 117 type, name 7, 3390.
    float               clength_thres;      // 118 type, name 7, 3391.
    float               parting_fac;        // 119 type, name 7, 3392.
    float               parting_min;        // 120 type, name 7, 3393.
    float               parting_max;        // 121 type, name 7, 3394.
    float               branch_thres;       // 122 type, name 7, 3395.
    float               draw_line[2];       // 123 type, name 7, 3396.
    float               path_start;         // 124 type, name 7, 3397.
    float               path_end;           // 125 type, name 7, 3398.
    int                 trail_count;        // 126 type, name 4, 3399.
    int                 keyed_loops;        // 127 type, name 4, 3400.
    CurveMapping        *clumpcurve;        // 128 type, name 58, 3401.
    CurveMapping        *roughcurve;        // 129 type, name 58, 3402.
    float               clump_noise_size;   // 130 type, name 7, 3403.
    float               bending_random;     // 131 type, name 7, 3404.
    MTex                *mtex[18];          // 132 type, name 51, 406.
    Group               *dup_group;         // 133 type, name 69, 1107.
    ListBase            dupliweights;       // 134 type, name 14, 3405.
    Group               *eff_group;         // 135 type, name 69, 3406.
    Object              *dup_ob;            // 136 type, name 29, 3407.
    Object              *bb_ob;             // 137 type, name 29, 3408.
    Ipo                 *ipo;               // 138 type, name 33, 80.
    PartDeflect         *pd;                // 139 type, name 199, 337.
    PartDeflect         *pd2;               // 140 type, name 199, 3409.
    short               use_modifier_stack; // 141 type, name 2, 3410.
    short               pad5[3];            // 142 type, name 2, 3411.
};

// zu DNA structure 495, 195
struct bGPdata
{
    ID          id;           // 1 type, name 22, 31.
    AnimData    *adt;         // 2 type, name 36, 75.
    ListBase    layers;       // 3 type, name 14, 1507.
    int         flag;         // 4 type, name 4, 18.
    short       sbuffer_size; // 5 type, name 2, 3501.
    short       sbuffer_sflag;// 6 type, name 2, 3502.
    void        *sbuffer;     // 7 type, name 11, 3503.
};

// zu DNA structure 496, 565
struct wmKeyMap
{
    wmKeyMap    *next;       // 1 type, name 565, 0.
    wmKeyMap    *prev;       // 2 type, name 565, 1.
    ListBase    items;       // 3 type, name 14, 3563.
    ListBase    diff_items;  // 4 type, name 14, 3564.
    char        idname[64];  // 5 type, name 0, 2476.
    short       spaceid;     // 6 type, name 2, 3565.
    short       regionid;    // 7 type, name 2, 3566.
    short       flag;        // 8 type, name 2, 18.
    short       kmi_id;      // 9 type, name 2, 3567.
    int         (*poll)();   // 10 type, name 4, 3568.
    void        *modal_items;// 11 type, name 11, 3569.
};

// zu DNA structure 497, 616
struct MovieTrackingDopesheet
{
    int         ok;               // 1 type, name 4, 138.
    short       sort_method;      // 2 type, name 2, 3825.
    short       flag;             // 3 type, name 2, 18.
    ListBase    coverage_segments;// 4 type, name 14, 3826.
    ListBase    channels;         // 5 type, name 14, 2869.
    int         tot_channel;      // 6 type, name 4, 3827.
    int         pad;              // 7 type, name 4, 66.
};

// zu DNA structure 498, 284
struct Mask
{
    ID          id;         // 1 type, name 22, 31.
    AnimData    *adt;       // 2 type, name 36, 75.
    ListBase    masklayers; // 3 type, name 14, 3886.
    int         masklay_act;// 4 type, name 4, 3887.
    int         masklay_tot;// 5 type, name 4, 3888.
    int         sfra;       // 6 type, name 4, 135.
    int         efra;       // 7 type, name 4, 1245.
    int         flag;       // 8 type, name 4, 18.
    int         pad;        // 9 type, name 4, 66.
};

// zu DNA structure 499, 621
struct MaskSplinePoint
{
    BezTriple            bezt;  // 1 type, name 32, 3895.
    int                  pad;   // 2 type, name 4, 66.
    int                  tot_uw;// 3 type, name 4, 3896.
    MaskSplinePointUW    *uw;   // 4 type, name 620, 3897.
    MaskParent           parent;// 5 type, name 619, 3285.
};

// zu DNA structure 500, 624
struct MaskLayer
{
    MaskLayer          *next;         // 1 type, name 624, 0.
    MaskLayer          *prev;         // 2 type, name 624, 1.
    char               name[64];      // 3 type, name 0, 19.
    ListBase           splines;       // 4 type, name 14, 3903.
    ListBase           splines_shapes;// 5 type, name 14, 3904.
    MaskSpline         *act_spline;   // 6 type, name 622, 3905.
    MaskSplinePoint    *act_point;    // 7 type, name 621, 3906.
    float              alpha;         // 8 type, name 7, 446.
    char               blend;         // 9 type, name 0, 1797.
    char               blend_flag;    // 10 type, name 0, 3907.
    char               falloff;       // 11 type, name 0, 808.
    char               pad[7];        // 12 type, name 0, 115.
    char               flag;          // 13 type, name 0, 18.
    char               restrictflag;  // 14 type, name 0, 1099.
};

// zu DNA structure 501, 224
struct FreestyleConfig
{
    ListBase    modules;             // 1 type, name 14, 3960.
    int         mode;                // 2 type, name 4, 351.
    int         raycasting_algorithm;// 3 type, name 4, 3961.
    int         flags;               // 4 type, name 4, 89.
    float       sphere_radius;       // 5 type, name 7, 3962.
    float       dkr_epsilon;         // 6 type, name 7, 3963.
    float       crease_angle;        // 7 type, name 7, 3964.
    ListBase    linesets;            // 8 type, name 14, 3965.
};

// zu DNA structure 502, 33
struct Ipo
{
    ID          id;       // 1 type, name 22, 31.
    ListBase    curve;    // 2 type, name 14, 62.
    rctf        cur;      // 3 type, name 19, 63.
    short       blocktype;// 4 type, name 2, 45.
    short       showkey;  // 5 type, name 2, 64.
    short       muteipo;  // 6 type, name 2, 65.
    short       pad;      // 7 type, name 2, 66.
};

// zu DNA structure 503, 46
struct Image
{
    ID                                id;                 // 1 type, name 22, 31.
    char                              name[1024];         // 2 type, name 0, 34.
    void                              *cache;             // Note: using void* on undefined DNA type: MovieCache
    void                              *gputexture;        // Note: using void* on undefined DNA type: GPUTexture
    void                              *anim;              // Note: using void* on undefined DNA type: anim
    void                              *rr;                // Note: using void* on undefined DNA type: RenderResult
    void                              *renders[8];        // Note: using void* on undefined DNA type: RenderResult
    short                             render_slot;        // 8 type, name 2, 147.
    short                             last_render_slot;   // 9 type, name 2, 148.
    short                             ok;                 // 10 type, name 2, 138.
    short                             flag;               // 11 type, name 2, 18.
    short                             source;             // 12 type, name 2, 149.
    short                             type;               // 13 type, name 2, 16.
    int                               lastframe;          // 14 type, name 4, 150.
    short                             tpageflag;          // 15 type, name 2, 151.
    short                             totbind;            // 16 type, name 2, 152.
    short                             xrep;               // 17 type, name 2, 153.
    short                             yrep;               // 18 type, name 2, 154.
    short                             twsta;              // 19 type, name 2, 155.
    short                             twend;              // 20 type, name 2, 156.
    int                               bindcode;           // 21 type, name 4, 157.
    int                               *repbind;           // 22 type, name 4, 158.
    PackedFile                        *packedfile;        // 23 type, name 25, 37.
    PreviewImage                      *preview;           // 24 type, name 26, 159.
    float                             lastupdate;         // 25 type, name 7, 160.
    int                               lastused;           // 26 type, name 4, 161.
    short                             animspeed;          // 27 type, name 2, 162.
    short                             pad2;               // 28 type, name 2, 29.
    int                               gen_x;              // 29 type, name 4, 163.
    int                               gen_y;              // 30 type, name 4, 164.
    char                              gen_type;           // 31 type, name 0, 165.
    char                              gen_flag;           // 32 type, name 0, 166.
    short                             gen_depth;          // 33 type, name 2, 167.
    float                             gen_color[4];       // 34 type, name 7, 168.
    float                             aspx;               // 35 type, name 7, 169.
    float                             aspy;               // 36 type, name 7, 170.
    ColorManagedColorspaceSettings    colorspace_settings;// 37 type, name 50, 171.
    char                              alpha_mode;         // 38 type, name 0, 172.
    char                              pad[7];             // 39 type, name 0, 115.
    RenderSlot                        render_slots[8];    // 40 type, name 45, 173.
};

// zu DNA structure 504, 74
struct MetaBall
{
    ID          id;        // 1 type, name 22, 31.
    AnimData    *adt;      // 2 type, name 36, 75.
    ListBase    elems;     // 3 type, name 14, 562.
    ListBase    disp;      // 4 type, name 14, 563.
    ListBase    *editelems;// 5 type, name 14, 564.
    Ipo         *ipo;      // 6 type, name 33, 80.
    Material    **mat;     // 7 type, name 68, 565.
    char        flag;      // 8 type, name 0, 18.
    char        flag2;     // 9 type, name 0, 566.
    short       totcol;    // 10 type, name 2, 567.
    short       texflag;   // 11 type, name 2, 192.
    short       pad;       // 12 type, name 2, 66.
    float       loc[3];    // 13 type, name 7, 341.
    float       size[3];   // 14 type, name 7, 189.
    float       rot[3];    // 15 type, name 7, 342.
    float       wiresize;  // 16 type, name 7, 568.
    float       rendersize;// 17 type, name 7, 569.
    float       thresh;    // 18 type, name 7, 570.
    MetaElem    *lastelem; // 19 type, name 72, 571.
};

// zu DNA structure 505, 80
struct Curve
{
    ID          id;             // 1 type, name 22, 31.
    AnimData    *adt;           // 2 type, name 36, 75.
    BoundBox    *bb;            // 3 type, name 73, 550.
    ListBase    nurb;           // 4 type, name 14, 607.
    EditNurb    *editnurb;      // 5 type, name 78, 608.
    Object      *bevobj;        // 6 type, name 29, 609.
    Object      *taperobj;      // 7 type, name 29, 610.
    Object      *textoncurve;   // 8 type, name 29, 611.
    Ipo         *ipo;           // 9 type, name 33, 80.
    Key         *key;           // 10 type, name 35, 612.
    Material    **mat;          // 11 type, name 68, 565.
    float       loc[3];         // 12 type, name 7, 341.
    float       size[3];        // 13 type, name 7, 189.
    float       rot[3];         // 14 type, name 7, 342.
    short       type;           // 15 type, name 2, 16.
    short       texflag;        // 16 type, name 2, 192.
    short       drawflag;       // 17 type, name 2, 613.
    short       twist_mode;     // 18 type, name 2, 614.
    float       twist_smooth;   // 19 type, name 7, 615.
    float       smallcaps_scale;// 20 type, name 7, 616.
    int         pathlen;        // 21 type, name 4, 617.
    short       bevresol;       // 22 type, name 2, 618.
    short       totcol;         // 23 type, name 2, 567.
    int         flag;           // 24 type, name 4, 18.
    float       width;          // 25 type, name 7, 619.
    float       ext1;           // 26 type, name 7, 620.
    float       ext2;           // 27 type, name 7, 621.
    short       resolu;         // 28 type, name 2, 590.
    short       resolv;         // 29 type, name 2, 591.
    short       resolu_ren;     // 30 type, name 2, 622.
    short       resolv_ren;     // 31 type, name 2, 623.
    int         actnu;          // 32 type, name 4, 624.
    int         actvert;        // 33 type, name 4, 625.
    char        pad[4];         // 34 type, name 0, 585.
    short       lines;          // 35 type, name 2, 91.
    char        spacemode;      // 36 type, name 0, 626.
    char        pad1;           // 37 type, name 0, 68.
    float       spacing;        // 38 type, name 7, 627.
    float       linedist;       // 39 type, name 7, 628.
    float       shear;          // 40 type, name 7, 629.
    float       fsize;          // 41 type, name 7, 630.
    float       wordspace;      // 42 type, name 7, 631.
    float       ulpos;          // 43 type, name 7, 632.
    float       ulheight;       // 44 type, name 7, 633.
    float       xof;            // 45 type, name 7, 634.
    float       yof;            // 46 type, name 7, 635.
    float       linewidth;      // 47 type, name 7, 636.
    int         pos;            // 48 type, name 4, 67.
    int         selstart;       // 49 type, name 4, 637.
    int         selend;         // 50 type, name 4, 638.
    int         len_wchar;      // 51 type, name 4, 639.
    int         len;            // 52 type, name 4, 22.
    char        *str;           // 53 type, name 0, 640.
    void        *editfont;      // Note: using void* on undefined DNA type: EditFont
    char        family[64];     // 55 type, name 0, 642.
    VFont       *vfont;         // 56 type, name 70, 643.
    VFont       *vfontb;        // 57 type, name 70, 644.
    VFont       *vfonti;        // 58 type, name 70, 645.
    VFont       *vfontbi;       // 59 type, name 70, 646.
    TextBox     *tb;            // 60 type, name 77, 647.
    int         totbox;         // 61 type, name 4, 648.
    int         actbox;         // 62 type, name 4, 649.
    CharInfo    *strinfo;       // 63 type, name 76, 650.
    CharInfo    curinfo;        // 64 type, name 76, 651.
    float       ctime;          // 65 type, name 7, 83.
    float       bevfac1;        // 66 type, name 7, 652.
    float       bevfac2;        // 67 type, name 7, 653.
    char        bevfac1_mapping;// 68 type, name 0, 654.
    char        bevfac2_mapping;// 69 type, name 0, 655.
    char        pad2[2];        // 70 type, name 0, 656.
};

// zu DNA structure 506, 98
struct Multires
{
    ListBase      levels;       // 1 type, name 14, 727.
    MVert         *verts;       // 2 type, name 92, 726.
    char          level_count;  // 3 type, name 0, 728.
    char          current;      // 4 type, name 0, 729.
    char          newlvl;       // 5 type, name 0, 730.
    char          edgelvl;      // 6 type, name 0, 731.
    char          pinlvl;       // 7 type, name 0, 732.
    char          renderlvl;    // 8 type, name 0, 733.
    char          use_col;      // 9 type, name 0, 734.
    char          flag;         // 10 type, name 0, 18.
    CustomData    vdata;        // 11 type, name 97, 672.
    CustomData    fdata;        // 12 type, name 97, 674.
    short         *edge_flags;  // 13 type, name 2, 735.
    char          *edge_creases;// 14 type, name 0, 736.
};

// zu DNA structure 507, 223
struct SceneRenderLayer
{
    SceneRenderLayer    *next;               // 1 type, name 223, 0.
    SceneRenderLayer    *prev;               // 2 type, name 223, 1.
    char                name[64];            // 3 type, name 0, 19.
    Material            *mat_override;       // 4 type, name 68, 1433.
    Group               *light_override;     // 5 type, name 69, 1434.
    int                 lay;                 // 6 type, name 4, 1054.
    int                 lay_zmask;           // 7 type, name 4, 1435.
    int                 lay_exclude;         // 8 type, name 4, 1436.
    int                 layflag;             // 9 type, name 4, 1437.
    int                 passflag;            // 10 type, name 4, 1438.
    int                 pass_xor;            // 11 type, name 4, 1439.
    int                 samples;             // 12 type, name 4, 111.
    float               pass_alpha_threshold;// 13 type, name 7, 1440.
    FreestyleConfig     freestyleConfig;     // 14 type, name 224, 1441.
};

// zu DNA structure 508, 228
struct BakeData
{
    ImageFormatData    im_format;        // 1 type, name 225, 1455.
    char               filepath[1024];   // 2 type, name 0, 35.
    short              width;            // 3 type, name 2, 619.
    short              height;           // 4 type, name 2, 804.
    short              margin;           // 5 type, name 2, 1074.
    short              flag;             // 6 type, name 2, 18.
    float              cage_extrusion;   // 7 type, name 7, 1456.
    float              pad2;             // 8 type, name 7, 29.
    char               normal_swizzle[3];// 9 type, name 0, 1457.
    char               normal_space;     // 10 type, name 0, 1458.
    char               save_mode;        // 11 type, name 0, 1459.
    char               pad[3];           // 12 type, name 0, 350.
    char               cage[64];         // 13 type, name 0, 1460.
};

// zu DNA structure 509, 234
struct GameData
{
    GameFraming    framing;              // 1 type, name 232, 1585.
    short          playerflag;           // 2 type, name 2, 1586.
    short          xplay;                // 3 type, name 2, 1476.
    short          yplay;                // 4 type, name 2, 1477.
    short          freqplay;             // 5 type, name 2, 1478.
    short          depth;                // 6 type, name 2, 256.
    short          attrib;               // 7 type, name 2, 1479.
    short          rt1;                  // 8 type, name 2, 1587.
    short          rt2;                  // 9 type, name 2, 1588.
    short          aasamples;            // 10 type, name 2, 1589.
    short          pad4[3];              // 11 type, name 2, 546.
    GameDome       dome;                 // 12 type, name 231, 1590.
    short          stereoflag;           // 13 type, name 2, 1591.
    short          stereomode;           // 14 type, name 2, 1481.
    float          eyeseparation;        // 15 type, name 7, 1592.
    RecastData     recastData;           // 16 type, name 233, 1593.
    float          gravity;              // 17 type, name 7, 1333.
    float          activityBoxRadius;    // 18 type, name 7, 1334.
    int            flag;                 // 19 type, name 4, 18.
    short          mode;                 // 20 type, name 2, 351.
    short          matmode;              // 21 type, name 2, 1594.
    short          occlusionRes;         // 22 type, name 2, 1336.
    short          physicsEngine;        // 23 type, name 2, 1337.
    short          exitkey;              // 24 type, name 2, 1595.
    short          vsync;                // 25 type, name 2, 1596.
    short          ticrate;              // 26 type, name 2, 1338.
    short          maxlogicstep;         // 27 type, name 2, 1339.
    short          physubstep;           // 28 type, name 2, 1340.
    short          maxphystep;           // 29 type, name 2, 1341.
    short          obstacleSimulation;   // 30 type, name 2, 1597.
    short          raster_storage;       // 31 type, name 2, 1598.
    float          levelHeight;          // 32 type, name 7, 1599.
    float          deactivationtime;     // 33 type, name 7, 1600.
    float          lineardeactthreshold; // 34 type, name 7, 1601.
    float          angulardeactthreshold;// 35 type, name 7, 1602.
    float          pad2;                 // 36 type, name 7, 29.
};

// zu DNA structure 510, 294
struct SpaceText
{
    SpaceLink    *next;          // 1 type, name 268, 0.
    SpaceLink    *prev;          // 2 type, name 268, 1.
    ListBase     regionbase;     // 3 type, name 14, 1838.
    int          spacetype;      // 4 type, name 4, 1839.
    float        blockscale;     // 5 type, name 7, 1840.
    short        blockhandler[8];// 6 type, name 2, 1841.
    Text         *text;          // 7 type, name 38, 1976.
    int          top;            // 8 type, name 4, 1977.
    int          viewlines;      // 9 type, name 4, 1978.
    short        flags;          // 10 type, name 2, 89.
    short        menunr;         // 11 type, name 2, 1979.
    short        lheight;        // 12 type, name 2, 1980.
    char         cwidth;         // 13 type, name 0, 1981.
    char         linenrs_tot;    // 14 type, name 0, 1982.
    int          left;           // 15 type, name 4, 1983.
    int          showlinenrs;    // 16 type, name 4, 1984.
    int          tabnumber;      // 17 type, name 4, 1985.
    short        showsyntax;     // 18 type, name 2, 1986.
    short        line_hlight;    // 19 type, name 2, 1987.
    short        overwrite;      // 20 type, name 2, 1988.
    short        live_edit;      // 21 type, name 2, 1989.
    float        pix_per_line;   // 22 type, name 7, 1990.
    rcti         txtscroll;      // 23 type, name 18, 1991.
    rcti         txtbar;         // 24 type, name 18, 1992.
    int          wordwrap;       // 25 type, name 4, 1993.
    int          doplugins;      // 26 type, name 4, 1994.
    char         findstr[256];   // 27 type, name 0, 1995.
    char         replacestr[256];// 28 type, name 0, 1996.
    short        margin_column;  // 29 type, name 2, 1997.
    short        lheight_dpi;    // 30 type, name 2, 1998.
    char         pad[4];         // 31 type, name 0, 585.
    void         *drawcache;     // 32 type, name 11, 1999.
    float        scroll_accum[2];// 33 type, name 7, 2000.
};

// zu DNA structure 511, 302
struct SpaceConsole
{
    SpaceLink    *next;          // 1 type, name 268, 0.
    SpaceLink    *prev;          // 2 type, name 268, 1.
    ListBase     regionbase;     // 3 type, name 14, 1838.
    int          spacetype;      // 4 type, name 4, 1839.
    float        blockscale;     // 5 type, name 7, 1840.
    short        blockhandler[8];// 6 type, name 2, 1841.
    int          lheight;        // 7 type, name 4, 1980.
    int          pad;            // 8 type, name 4, 66.
    ListBase     scrollback;     // 9 type, name 14, 2025.
    ListBase     history;        // 10 type, name 14, 2026.
    char         prompt[256];    // 11 type, name 0, 2027.
    char         language[32];   // 12 type, name 0, 2028.
    int          sel_start;      // 13 type, name 4, 2029.
    int          sel_end;        // 14 type, name 4, 2030.
};

// zu DNA structure 512, 254
struct Editing
{
    ListBase    *seqbasep;         // 1 type, name 14, 2580.
    ListBase    seqbase;           // 2 type, name 14, 2566.
    ListBase    metastack;         // 3 type, name 14, 2581.
    Sequence    *act_seq;          // 4 type, name 344, 2582.
    char        act_imagedir[1024];// 5 type, name 0, 2583.
    char        act_sounddir[1024];// 6 type, name 0, 2584.
    int         over_ofs;          // 7 type, name 4, 2585.
    int         over_cfra;         // 8 type, name 4, 2586.
    int         over_flag;         // 9 type, name 4, 2587.
    int         pad;               // 10 type, name 4, 66.
    rctf        over_border;       // 11 type, name 19, 2588.
};

// zu DNA structure 513, 409
struct bArmature
{
    ID          id;             // 1 type, name 22, 31.
    AnimData    *adt;           // 2 type, name 36, 75.
    ListBase    bonebase;       // 3 type, name 14, 2789.
    ListBase    chainbase;      // 4 type, name 14, 2790.
    ListBase    *edbo;          // 5 type, name 14, 2791.
    Bone        *act_bone;      // 6 type, name 408, 2792.
    void        *act_edbone;    // Note: using void* on undefined DNA type: EditBone
    void        *sketch;        // 8 type, name 11, 2794.
    int         flag;           // 9 type, name 4, 18.
    int         drawtype;       // 10 type, name 4, 1851.
    int         gevertdeformer; // 11 type, name 4, 2795.
    int         pad;            // 12 type, name 4, 66.
    short       deformflag;     // 13 type, name 2, 811.
    short       pathflag;       // 14 type, name 2, 1905.
    int         layer_used;     // 15 type, name 4, 2796.
    int         layer;          // 16 type, name 4, 140.
    int         layer_protected;// 17 type, name 4, 2797.
    short       ghostep;        // 18 type, name 2, 2798.
    short       ghostsize;      // 19 type, name 2, 2799.
    short       ghosttype;      // 20 type, name 2, 2800.
    short       pathsize;       // 21 type, name 2, 2801.
    int         ghostsf;        // 22 type, name 4, 2802.
    int         ghostef;        // 23 type, name 4, 2803.
    int         pathsf;         // 24 type, name 4, 2804.
    int         pathef;         // 25 type, name 4, 2805.
    int         pathbc;         // 26 type, name 4, 2806.
    int         pathac;         // 27 type, name 4, 2807.
};

// zu DNA structure 514, 412
struct bPoseChannel
{
    bPoseChannel    *next;         // 1 type, name 412, 0.
    bPoseChannel    *prev;         // 2 type, name 412, 1.
    IDProperty      *prop;         // 3 type, name 21, 2322.
    ListBase        constraints;   // 4 type, name 14, 1102.
    char            name[64];      // 5 type, name 0, 19.
    short           flag;          // 6 type, name 2, 18.
    short           ikflag;        // 7 type, name 2, 2826.
    short           protectflag;   // 8 type, name 2, 1057.
    short           agrp_index;    // 9 type, name 2, 2827.
    char            constflag;     // 10 type, name 0, 2828.
    char            selectflag;    // 11 type, name 0, 2829.
    char            pad0[6];       // 12 type, name 0, 2830.
    Bone            *bone;         // 13 type, name 408, 2831.
    bPoseChannel    *parent;       // 14 type, name 412, 36.
    bPoseChannel    *child;        // 15 type, name 412, 2832.
    ListBase        iktree;        // 16 type, name 14, 2833.
    ListBase        siktree;       // 17 type, name 14, 2834.
    bMotionPath     *mpath;        // 18 type, name 197, 1033.
    Object          *custom;       // 19 type, name 29, 2835.
    bPoseChannel    *custom_tx;    // 20 type, name 412, 2836.
    float           loc[3];        // 21 type, name 7, 341.
    float           size[3];       // 22 type, name 7, 189.
    float           eul[3];        // 23 type, name 7, 2837.
    float           quat[4];       // 24 type, name 7, 553.
    float           rotAxis[3];    // 25 type, name 7, 1047.
    float           rotAngle;      // 26 type, name 7, 1049.
    short           rotmode;       // 27 type, name 2, 1083.
    short           pad;           // 28 type, name 2, 66.
    float           chan_mat[4][4];// 29 type, name 7, 2838.
    float           pose_mat[4][4];// 30 type, name 7, 2839.
    float           constinv[4][4];// 31 type, name 7, 1052.
    float           pose_head[3];  // 32 type, name 7, 2840.
    float           pose_tail[3];  // 33 type, name 7, 2841.
    float           limitmin[3];   // 34 type, name 7, 2842.
    float           limitmax[3];   // 35 type, name 7, 2843.
    float           stiffness[3];  // 36 type, name 7, 2844.
    float           ikstretch;     // 37 type, name 7, 2845.
    float           ikrotweight;   // 38 type, name 7, 2846.
    float           iklinweight;   // 39 type, name 7, 2847.
    void            *temp;         // 40 type, name 11, 2848.
};

// zu DNA structure 515, 194
struct bPose
{
    ListBase            chanbase;          // 1 type, name 14, 2849.
    void                *chanhash;         // Note: using void* on undefined DNA type: GHash
    short               flag;              // 3 type, name 2, 18.
    short               pad;               // 4 type, name 2, 66.
    int                 proxy_layer;       // 5 type, name 4, 2851.
    int                 pad1;              // 6 type, name 4, 68.
    float               ctime;             // 7 type, name 7, 83.
    float               stride_offset[3];  // 8 type, name 7, 2852.
    float               cyclic_offset[3];  // 9 type, name 7, 2853.
    ListBase            agroups;           // 10 type, name 14, 2854.
    int                 active_group;      // 11 type, name 4, 2855.
    int                 iksolver;          // 12 type, name 4, 2856.
    void                *ikdata;           // 13 type, name 11, 2857.
    void                *ikparam;          // 14 type, name 11, 2858.
    bAnimVizSettings    avs;               // 15 type, name 196, 1032.
    char                proxy_act_bone[64];// 16 type, name 0, 2859.
};

// zu DNA structure 516, 482
struct NodeImageFile
{
    char               name[1024];// 1 type, name 0, 34.
    ImageFormatData    im_format; // 2 type, name 225, 1455.
    int                sfra;      // 3 type, name 4, 135.
    int                efra;      // 4 type, name 4, 1245.
};

// zu DNA structure 517, 483
struct NodeImageMultiFile
{
    char               base_path[1024];// 1 type, name 0, 3078.
    ImageFormatData    format;         // 2 type, name 225, 3079.
    int                sfra;           // 3 type, name 4, 135.
    int                efra;           // 4 type, name 4, 1245.
    int                active_input;   // 5 type, name 4, 3080.
    int                pad;            // 6 type, name 4, 66.
};

// zu DNA structure 518, 484
struct NodeImageMultiFileSocket
{
    short              use_render_format;// 1 type, name 2, 3081.
    short              use_node_format;  // 2 type, name 2, 3082.
    int                pad1;             // 3 type, name 4, 68.
    char               path[1024];       // 4 type, name 0, 1190.
    ImageFormatData    format;           // 5 type, name 225, 3079.
    char               layer[30];        // 6 type, name 0, 3083.
    char               pad2[2];          // 7 type, name 0, 656.
};

// zu DNA structure 519, 58
struct CurveMapping
{
    int         flag;             // 1 type, name 4, 18.
    int         cur;              // 2 type, name 4, 63.
    int         preset;           // 3 type, name 4, 3179.
    int         changed_timestamp;// 4 type, name 4, 3180.
    rctf        curr;             // 5 type, name 19, 3181.
    rctf        clipr;            // 6 type, name 19, 3182.
    CurveMap    cm[4];            // 7 type, name 525, 3183.
    float       black[3];         // 8 type, name 7, 3184.
    float       white[3];         // 9 type, name 7, 3185.
    float       bwmul[3];         // 10 type, name 7, 3186.
    float       sample[3];        // 11 type, name 7, 3187.
};

// zu DNA structure 520, 238
struct Palette
{
    ID          id;          // 1 type, name 22, 31.
    ListBase    colors;      // 2 type, name 14, 3259.
    ListBase    deleted;     // 3 type, name 14, 3260.
    int         active_color;// 4 type, name 4, 3261.
    int         pad;         // 5 type, name 4, 66.
};

// zu DNA structure 521, 584
struct NlaStrip
{
    NlaStrip      *next;          // 1 type, name 584, 0.
    NlaStrip      *prev;          // 2 type, name 584, 1.
    ListBase      strips;         // 3 type, name 14, 3608.
    bAction       *act;           // 4 type, name 193, 2698.
    AnimMapper    *remap;         // 5 type, name 583, 3609.
    ListBase      fcurves;        // 6 type, name 14, 3610.
    ListBase      modifiers;      // 7 type, name 14, 1037.
    char          name[64];       // 8 type, name 0, 19.
    float         influence;      // 9 type, name 7, 2750.
    float         strip_time;     // 10 type, name 7, 3611.
    float         start;          // 11 type, name 7, 753.
    float         end;            // 12 type, name 7, 2625.
    float         actstart;       // 13 type, name 7, 2959.
    float         actend;         // 14 type, name 7, 2960.
    float         repeat;         // 15 type, name 7, 800.
    float         scale;          // 16 type, name 7, 954.
    float         blendin;        // 17 type, name 7, 2700.
    float         blendout;       // 18 type, name 7, 2963.
    short         blendmode;      // 19 type, name 2, 3612.
    short         extendmode;     // 20 type, name 2, 3613.
    short         pad1;           // 21 type, name 2, 68.
    short         type;           // 22 type, name 2, 16.
    void          *speaker_handle;// 23 type, name 11, 3614.
    int           flag;           // 24 type, name 4, 18.
    int           pad2;           // 25 type, name 4, 29.
};

// zu DNA structure 522, 36
struct AnimData
{
    bAction       *action;       // 1 type, name 193, 1028.
    bAction       *tmpact;       // 2 type, name 193, 3623.
    AnimMapper    *remap;        // 3 type, name 583, 3609.
    ListBase      nla_tracks;    // 4 type, name 14, 3624.
    NlaStrip      *actstrip;     // 5 type, name 584, 3625.
    ListBase      drivers;       // 6 type, name 14, 3626.
    ListBase      overrides;     // 7 type, name 14, 3627.
    int           flag;          // 8 type, name 4, 18.
    int           recalc;        // 9 type, name 4, 257.
    short         act_blendmode; // 10 type, name 2, 3628.
    short         act_extendmode;// 11 type, name 2, 3629.
    float         act_influence; // 12 type, name 7, 3630.
};

// zu DNA structure 523, 596
struct BoidState
{
    BoidState    *next;         // 1 type, name 596, 0.
    BoidState    *prev;         // 2 type, name 596, 1.
    ListBase     rules;         // 3 type, name 14, 3642.
    ListBase     conditions;    // 4 type, name 14, 3643.
    ListBase     actions;       // 5 type, name 14, 3644.
    char         name[32];      // 6 type, name 0, 1563.
    int          id;            // 7 type, name 4, 31.
    int          flag;          // 8 type, name 4, 18.
    int          ruleset_type;  // 9 type, name 4, 3645.
    float        rule_fuzziness;// 10 type, name 7, 3646.
    int          signal_id;     // 11 type, name 4, 3634.
    int          channels;      // 12 type, name 4, 2869.
    float        volume;        // 13 type, name 7, 1197.
    float        falloff;       // 14 type, name 7, 808.
};

// zu DNA structure 524, 612
struct MovieTrackingObject
{
    MovieTrackingObject            *next;         // 1 type, name 612, 0.
    MovieTrackingObject            *prev;         // 2 type, name 612, 1.
    char                           name[64];      // 3 type, name 0, 19.
    int                            flag;          // 4 type, name 4, 18.
    float                          scale;         // 5 type, name 7, 954.
    ListBase                       tracks;        // 6 type, name 14, 3816.
    ListBase                       plane_tracks;  // 7 type, name 14, 3817.
    MovieTrackingReconstruction    reconstruction;// 8 type, name 611, 3818.
    int                            keyframe1;     // 9 type, name 4, 3798.
    int                            keyframe2;     // 10 type, name 4, 3799.
};

// zu DNA structure 525, 68
struct Material
{
    ID                id;               // 1 type, name 22, 31.
    AnimData          *adt;             // 2 type, name 36, 75.
    short             material_type;    // 3 type, name 2, 431.
    short             flag;             // 4 type, name 2, 18.
    float             r;                // 5 type, name 7, 198.
    float             g;                // 6 type, name 7, 199.
    float             b;                // 7 type, name 7, 200.
    float             specr;            // 8 type, name 7, 432.
    float             specg;            // 9 type, name 7, 433.
    float             specb;            // 10 type, name 7, 434.
    float             mirr;             // 11 type, name 7, 435.
    float             mirg;             // 12 type, name 7, 436.
    float             mirb;             // 13 type, name 7, 437.
    float             ambr;             // 14 type, name 7, 438.
    float             ambb;             // 15 type, name 7, 439.
    float             ambg;             // 16 type, name 7, 440.
    float             amb;              // 17 type, name 7, 441.
    float             emit;             // 18 type, name 7, 442.
    float             ang;              // 19 type, name 7, 443.
    float             spectra;          // 20 type, name 7, 444.
    float             ray_mirror;       // 21 type, name 7, 445.
    float             alpha;            // 22 type, name 7, 446.
    float             ref;              // 23 type, name 7, 447.
    float             spec;             // 24 type, name 7, 448.
    float             zoffs;            // 25 type, name 7, 449.
    float             add;              // 26 type, name 7, 450.
    float             translucency;     // 27 type, name 7, 451.
    VolumeSettings    vol;              // 28 type, name 65, 452.
    GameSettings      game;             // 29 type, name 66, 453.
    float             fresnel_mir;      // 30 type, name 7, 454.
    float             fresnel_mir_i;    // 31 type, name 7, 455.
    float             fresnel_tra;      // 32 type, name 7, 456.
    float             fresnel_tra_i;    // 33 type, name 7, 457.
    float             filter;           // 34 type, name 7, 458.
    float             tx_limit;         // 35 type, name 7, 459.
    float             tx_falloff;       // 36 type, name 7, 460.
    short             ray_depth;        // 37 type, name 2, 461.
    short             ray_depth_tra;    // 38 type, name 2, 462.
    short             har;              // 39 type, name 2, 463.
    char              seed1;            // 40 type, name 0, 464.
    char              seed2;            // 41 type, name 0, 465.
    float             gloss_mir;        // 42 type, name 7, 466.
    float             gloss_tra;        // 43 type, name 7, 467.
    short             samp_gloss_mir;   // 44 type, name 2, 468.
    short             samp_gloss_tra;   // 45 type, name 2, 469.
    float             adapt_thresh_mir; // 46 type, name 7, 470.
    float             adapt_thresh_tra; // 47 type, name 7, 471.
    float             aniso_gloss_mir;  // 48 type, name 7, 472.
    float             dist_mir;         // 49 type, name 7, 473.
    short             fadeto_mir;       // 50 type, name 2, 474.
    short             shade_flag;       // 51 type, name 2, 475.
    int               mode;             // 52 type, name 4, 351.
    int               mode_l;           // 53 type, name 4, 476.
    int               mode2;            // 54 type, name 4, 477.
    int               mode2_l;          // 55 type, name 4, 478.
    short             flarec;           // 56 type, name 2, 479.
    short             starc;            // 57 type, name 2, 480.
    short             linec;            // 58 type, name 2, 481.
    short             ringc;            // 59 type, name 2, 482.
    float             hasize;           // 60 type, name 7, 483.
    float             flaresize;        // 61 type, name 7, 484.
    float             subsize;          // 62 type, name 7, 485.
    float             flareboost;       // 63 type, name 7, 486.
    float             strand_sta;       // 64 type, name 7, 487.
    float             strand_end;       // 65 type, name 7, 488.
    float             strand_ease;      // 66 type, name 7, 489.
    float             strand_surfnor;   // 67 type, name 7, 490.
    float             strand_min;       // 68 type, name 7, 491.
    float             strand_widthfade; // 69 type, name 7, 492.
    char              strand_uvname[64];// 70 type, name 0, 493.
    float             sbias;            // 71 type, name 7, 494.
    float             lbias;            // 72 type, name 7, 495.
    float             shad_alpha;       // 73 type, name 7, 496.
    int               septex;           // 74 type, name 4, 497.
    char              rgbsel;           // 75 type, name 0, 498.
    char              texact;           // 76 type, name 0, 387.
    char              pr_type;          // 77 type, name 0, 499.
    char              use_nodes;        // 78 type, name 0, 340.
    short             pr_lamp;          // 79 type, name 2, 500.
    short             pr_texture;       // 80 type, name 2, 407.
    short             ml_flag;          // 81 type, name 2, 501.
    char              mapflag;          // 82 type, name 0, 502.
    char              pad;              // 83 type, name 0, 66.
    short             diff_shader;      // 84 type, name 2, 503.
    short             spec_shader;      // 85 type, name 2, 504.
    float             roughness;        // 86 type, name 7, 505.
    float             refrac;           // 87 type, name 7, 506.
    float             param[4];         // 88 type, name 7, 507.
    float             rms;              // 89 type, name 7, 508.
    float             darkness;         // 90 type, name 7, 509.
    short             texco;            // 91 type, name 2, 174.
    short             mapto;            // 92 type, name 2, 175.
    ColorBand         *ramp_col;        // 93 type, name 54, 510.
    ColorBand         *ramp_spec;       // 94 type, name 54, 511.
    char              rampin_col;       // 95 type, name 0, 512.
    char              rampin_spec;      // 96 type, name 0, 513.
    char              rampblend_col;    // 97 type, name 0, 514.
    char              rampblend_spec;   // 98 type, name 0, 515.
    short             ramp_show;        // 99 type, name 2, 516.
    short             pad3;             // 100 type, name 2, 517.
    float             rampfac_col;      // 101 type, name 7, 518.
    float             rampfac_spec;     // 102 type, name 7, 519.
    MTex              *mtex[18];        // 103 type, name 51, 406.
    bNodeTree         *nodetree;        // 104 type, name 61, 335.
    Ipo               *ipo;             // 105 type, name 33, 80.
    Group             *group;           // 106 type, name 69, 520.
    PreviewImage      *preview;         // 107 type, name 26, 159.
    float             friction;         // 108 type, name 7, 521.
    float             fh;               // 109 type, name 7, 522.
    float             reflect;          // 110 type, name 7, 523.
    float             fhdist;           // 111 type, name 7, 524.
    float             xyfrict;          // 112 type, name 7, 525.
    short             dynamode;         // 113 type, name 2, 526.
    short             pad2;             // 114 type, name 2, 29.
    float             sss_radius[3];    // 115 type, name 7, 527.
    float             sss_col[3];       // 116 type, name 7, 528.
    float             sss_error;        // 117 type, name 7, 529.
    float             sss_scale;        // 118 type, name 7, 530.
    float             sss_ior;          // 119 type, name 7, 531.
    float             sss_colfac;       // 120 type, name 7, 532.
    float             sss_texfac;       // 121 type, name 7, 533.
    float             sss_front;        // 122 type, name 7, 534.
    float             sss_back;         // 123 type, name 7, 535.
    short             sss_flag;         // 124 type, name 2, 536.
    short             sss_preset;       // 125 type, name 2, 537.
    int               mapto_textured;   // 126 type, name 4, 538.
    short             shadowonly_flag;  // 127 type, name 2, 539.
    short             index;            // 128 type, name 2, 430.
    float             line_col[4];      // 129 type, name 7, 540.
    short             line_priority;    // 130 type, name 2, 541.
    short             vcol_alpha;       // 131 type, name 2, 542.
    short             paint_active_slot;// 132 type, name 2, 543.
    short             paint_clone_slot; // 133 type, name 2, 544.
    short             tot_slots;        // 134 type, name 2, 545.
    short             pad4[3];          // 135 type, name 2, 546.
    TexPaintSlot      *texpaintslot;    // 136 type, name 67, 547.
    ListBase          gpumaterial;      // 137 type, name 14, 548.
};

// zu DNA structure 526, 308
struct uiStyle
{
    uiStyle        *next;         // 1 type, name 308, 0.
    uiStyle        *prev;         // 2 type, name 308, 1.
    char           name[64];      // 3 type, name 0, 19.
    uiFontStyle    paneltitle;    // 4 type, name 307, 2055.
    uiFontStyle    grouplabel;    // 5 type, name 307, 2056.
    uiFontStyle    widgetlabel;   // 6 type, name 307, 2057.
    uiFontStyle    widget;        // 7 type, name 307, 2058.
    float          panelzoom;     // 8 type, name 7, 2059.
    short          minlabelchars; // 9 type, name 2, 2060.
    short          minwidgetchars;// 10 type, name 2, 2061.
    short          columnspace;   // 11 type, name 2, 2062.
    short          templatespace; // 12 type, name 2, 2063.
    short          boxspace;      // 13 type, name 2, 2064.
    short          buttonspacex;  // 14 type, name 2, 2065.
    short          buttonspacey;  // 15 type, name 2, 2066.
    short          panelspace;    // 16 type, name 2, 2067.
    short          panelouter;    // 17 type, name 2, 2068.
    short          pad;           // 18 type, name 2, 66.
};

// zu DNA structure 527, 473
struct NodeColorCorrection
{
    ColorCorrectionData    master;       // 1 type, name 472, 3051.
    ColorCorrectionData    shadows;      // 2 type, name 472, 3052.
    ColorCorrectionData    midtones;     // 3 type, name 472, 3053.
    ColorCorrectionData    highlights;   // 4 type, name 472, 3054.
    float                  startmidtones;// 5 type, name 7, 3055.
    float                  endmidtones;  // 6 type, name 7, 3056.
};

// zu DNA structure 528, 499
struct NodeTexBase
{
    TexMapping      tex_mapping;  // 1 type, name 62, 3127.
    ColorMapping    color_mapping;// 2 type, name 63, 3128.
};

// zu DNA structure 529, 237
struct Brush
{
    ID              id;                       // 1 type, name 22, 31.
    BrushClone      clone;                    // 2 type, name 526, 3216.
    CurveMapping    *curve;                   // 3 type, name 58, 3174.
    MTex            mtex;                     // 4 type, name 51, 3217.
    MTex            mask_mtex;                // 5 type, name 51, 3218.
    Brush           *toggle_brush;            // 6 type, name 237, 3219.
    void            *icon_imbuf;              // Note: using void* on undefined DNA type: ImBuf
    PreviewImage    *preview;                 // 8 type, name 26, 159.
    ColorBand       *gradient;                // 9 type, name 54, 3221.
    PaintCurve      *paint_curve;             // 10 type, name 527, 3222.
    char            icon_filepath[1024];      // 11 type, name 0, 3223.
    float           normal_weight;            // 12 type, name 7, 3224.
    short           blend;                    // 13 type, name 2, 1797.
    short           ob_mode;                  // 14 type, name 2, 3225.
    float           weight;                   // 15 type, name 7, 574.
    int             size;                     // 16 type, name 4, 101.
    int             flag;                     // 17 type, name 4, 18.
    int             mask_pressure;            // 18 type, name 4, 3226.
    float           jitter;                   // 19 type, name 7, 3227.
    int             jitter_absolute;          // 20 type, name 4, 3228.
    int             overlay_flags;            // 21 type, name 4, 3229.
    int             spacing;                  // 22 type, name 4, 627.
    int             smooth_stroke_radius;     // 23 type, name 4, 3230.
    float           smooth_stroke_factor;     // 24 type, name 7, 3231.
    float           rate;                     // 25 type, name 7, 3232.
    float           rgb[3];                   // 26 type, name 7, 1642.
    float           alpha;                    // 27 type, name 7, 446.
    float           secondary_rgb[3];         // 28 type, name 7, 1643.
    int             sculpt_plane;             // 29 type, name 4, 3233.
    float           plane_offset;             // 30 type, name 7, 3234.
    int             flag2;                    // 31 type, name 4, 566.
    int             gradient_spacing;         // 32 type, name 4, 3235.
    int             gradient_stroke_mode;     // 33 type, name 4, 3236.
    int             gradient_fill_mode;       // 34 type, name 4, 3237.
    char            sculpt_tool;              // 35 type, name 0, 3238.
    char            vertexpaint_tool;         // 36 type, name 0, 3239.
    char            imagepaint_tool;          // 37 type, name 0, 3240.
    char            mask_tool;                // 38 type, name 0, 3241.
    float           autosmooth_factor;        // 39 type, name 7, 3242.
    float           crease_pinch_factor;      // 40 type, name 7, 3243.
    float           plane_trim;               // 41 type, name 7, 3244.
    float           height;                   // 42 type, name 7, 804.
    float           texture_sample_bias;      // 43 type, name 7, 3245.
    int             texture_overlay_alpha;    // 44 type, name 4, 3246.
    int             mask_overlay_alpha;       // 45 type, name 4, 3247.
    int             cursor_overlay_alpha;     // 46 type, name 4, 3248.
    float           unprojected_radius;       // 47 type, name 7, 1641.
    float           sharp_threshold;          // 48 type, name 7, 3249.
    int             blur_kernel_radius;       // 49 type, name 4, 3250.
    int             blur_mode;                // 50 type, name 4, 3251.
    float           fill_threshold;           // 51 type, name 7, 3252.
    float           add_col[3];               // 52 type, name 7, 3253.
    float           sub_col[3];               // 53 type, name 7, 3254.
    float           stencil_pos[2];           // 54 type, name 7, 3255.
    float           stencil_dimension[2];     // 55 type, name 7, 3256.
    float           mask_stencil_pos[2];      // 56 type, name 7, 3257.
    float           mask_stencil_dimension[2];// 57 type, name 7, 3258.
};

// zu DNA structure 530, 157
struct ParticleSystem
{
    ParticleSystem        *next;               // 1 type, name 157, 0.
    ParticleSystem        *prev;               // 2 type, name 157, 1.
    ParticleSettings      *part;               // 3 type, name 542, 3412.
    ParticleData          *particles;          // 4 type, name 540, 3413.
    ChildParticle         *child;              // 5 type, name 537, 2832.
    void                  *edit;               // Note: using void* on undefined DNA type: PTCacheEdit
    void                  (*free_edit)();      // 7 type, name 11, 1194.
    void                  **pathcache;         // Note: using void* on undefined DNA type: ParticleCacheKey
    void                  **childcache;        // Note: using void* on undefined DNA type: ParticleCacheKey
    ListBase              pathcachebufs;       // 10 type, name 14, 3416.
    ListBase              childcachebufs;      // 11 type, name 14, 3417.
    ClothModifierData     *clmd;               // 12 type, name 140, 3418.
    void                  *hair_in_dm;         // Note: using void* on undefined DNA type: DerivedMesh
    void                  *hair_out_dm;        // Note: using void* on undefined DNA type: DerivedMesh
    Object                *target_ob;          // 15 type, name 29, 3421.
    void                  *lattice_deform_data;// Note: using void* on undefined DNA type: LatticeDeformData
    Object                *parent;             // 17 type, name 29, 36.
    ListBase              targets;             // 18 type, name 14, 2895.
    char                  name[64];            // 19 type, name 0, 19.
    float                 imat[4][4];          // 20 type, name 7, 250.
    float                 cfra;                // 21 type, name 7, 845.
    float                 tree_frame;          // 22 type, name 7, 3423.
    float                 bvhtree_frame;       // 23 type, name 7, 3424.
    int                   seed;                // 24 type, name 4, 756.
    int                   child_seed;          // 25 type, name 4, 3425.
    int                   flag;                // 26 type, name 4, 18.
    int                   totpart;             // 27 type, name 4, 2626.
    int                   totunexist;          // 28 type, name 4, 3426.
    int                   totchild;            // 29 type, name 4, 3427.
    int                   totcached;           // 30 type, name 4, 3428.
    int                   totchildcache;       // 31 type, name 4, 3429.
    short                 recalc;              // 32 type, name 2, 257.
    short                 target_psys;         // 33 type, name 2, 3430.
    short                 totkeyed;            // 34 type, name 2, 3431.
    short                 bakespace;           // 35 type, name 2, 3432.
    char                  bb_uvname[3][64];    // 36 type, name 0, 3433.
    short                 vgroup[12];          // 37 type, name 2, 3434.
    short                 vg_neg;              // 38 type, name 2, 3435.
    short                 rt3;                 // 39 type, name 2, 3436.
    void                  *renderdata;         // Note: using void* on undefined DNA type: ParticleRenderData
    PointCache            *pointcache;         // 41 type, name 144, 1266.
    ListBase              ptcaches;            // 42 type, name 14, 824.
    ListBase              *effectors;          // 43 type, name 14, 3438.
    ParticleSpring        *fluid_springs;      // 44 type, name 536, 3439.
    int                   tot_fluidsprings;    // 45 type, name 4, 3440.
    int                   alloc_fluidsprings;  // 46 type, name 4, 3441.
    void                  *tree;               // Note: using void* on undefined DNA type: KDTree
    void                  *bvhtree;            // Note: using void* on undefined DNA type: BVHTree
    void                  *pdd;                // Note: using void* on undefined DNA type: ParticleDrawData
    float                 dt_frac;             // 50 type, name 7, 3444.
    float                 _pad;                // 51 type, name 7, 288.
};

// zu DNA structure 531, 269
struct View2D
{
    rctf                 tot;          // 1 type, name 19, 242.
    rctf                 cur;          // 2 type, name 19, 63.
    rcti                 vert;         // 3 type, name 18, 1875.
    rcti                 hor;          // 4 type, name 18, 1876.
    rcti                 mask;         // 5 type, name 18, 1877.
    float                min[2];       // 6 type, name 7, 1878.
    float                max[2];       // 7 type, name 7, 1879.
    float                minzoom;      // 8 type, name 7, 1880.
    float                maxzoom;      // 9 type, name 7, 1881.
    short                scroll;       // 10 type, name 2, 1882.
    short                scroll_ui;    // 11 type, name 2, 1883.
    short                keeptot;      // 12 type, name 2, 1884.
    short                keepzoom;     // 13 type, name 2, 1885.
    short                keepofs;      // 14 type, name 2, 1886.
    short                flag;         // 15 type, name 2, 18.
    short                align;        // 16 type, name 2, 1887.
    short                winx;         // 17 type, name 2, 1888.
    short                winy;         // 18 type, name 2, 1889.
    short                oldwinx;      // 19 type, name 2, 1890.
    short                oldwiny;      // 20 type, name 2, 1891.
    short                around;       // 21 type, name 2, 1854.
    float                *tab_offset;  // 22 type, name 7, 1892.
    int                  tab_num;      // 23 type, name 4, 1893.
    int                  tab_cur;      // 24 type, name 4, 1894.
    void                 *sms;         // Note: using void* on undefined DNA type: SmoothView2DStore
    void                 *smooth_timer;// Note: using void* on undefined DNA type: wmTimer
};

// zu DNA structure 532, 304
struct SpaceClip
{
    SpaceLink          *next;           // 1 type, name 268, 0.
    SpaceLink          *prev;           // 2 type, name 268, 1.
    ListBase           regionbase;      // 3 type, name 14, 1838.
    int                spacetype;       // 4 type, name 4, 1839.
    float              xof;             // 5 type, name 7, 634.
    float              yof;             // 6 type, name 7, 635.
    float              xlockof;         // 7 type, name 7, 2033.
    float              ylockof;         // 8 type, name 7, 2034.
    float              zoom;            // 9 type, name 7, 1927.
    MovieClipUser      user;            // 10 type, name 260, 2035.
    MovieClip          *clip;           // 11 type, name 257, 1792.
    MovieClipScopes    scopes;          // 12 type, name 305, 1930.
    int                flag;            // 13 type, name 4, 18.
    short              mode;            // 14 type, name 2, 351.
    short              view;            // 15 type, name 2, 1799.
    int                path_length;     // 16 type, name 4, 2036.
    float              loc[2];          // 17 type, name 7, 2037.
    float              scale;           // 18 type, name 7, 954.
    float              angle;           // 19 type, name 7, 797.
    int                pad;             // 20 type, name 4, 66.
    float              stabmat[4][4];   // 21 type, name 7, 2038.
    float              unistabmat[4][4];// 22 type, name 7, 2039.
    int                postproc_flag;   // 23 type, name 4, 2040.
    short              gpencil_src;     // 24 type, name 2, 1689.
    short              pad2;            // 25 type, name 2, 29.
    int                around;          // 26 type, name 4, 1854.
    int                pad4;            // 27 type, name 4, 1499.
    float              cursor[2];       // 28 type, name 7, 1966.
    MaskSpaceInfo      mask_info;       // 29 type, name 283, 1975.
};

// zu DNA structure 533, 322
struct bScreen
{
    ID          id;                 // 1 type, name 22, 31.
    ListBase    vertbase;           // 2 type, name 14, 2439.
    ListBase    edgebase;           // 3 type, name 14, 2440.
    ListBase    areabase;           // 4 type, name 14, 2441.
    ListBase    regionbase;         // 5 type, name 14, 1838.
    Scene       *scene;             // 6 type, name 44, 131.
    Scene       *newscene;          // 7 type, name 44, 2442.
    short       winid;              // 8 type, name 2, 2443.
    short       redraws_flag;       // 9 type, name 2, 2444.
    char        temp;               // 10 type, name 0, 2445.
    char        state;              // 11 type, name 0, 1116.
    char        do_draw;            // 12 type, name 0, 2446.
    char        do_refresh;         // 13 type, name 0, 2447.
    char        do_draw_gesture;    // 14 type, name 0, 2448.
    char        do_draw_paintcursor;// 15 type, name 0, 2449.
    char        do_draw_drag;       // 16 type, name 0, 2450.
    char        swap;               // 17 type, name 0, 2451.
    char        skip_handling;      // 18 type, name 0, 2452.
    char        pad[7];             // 19 type, name 0, 115.
    short       mainwin;            // 20 type, name 2, 2453.
    short       subwinactive;       // 21 type, name 2, 2454.
    void        *animtimer;         // Note: using void* on undefined DNA type: wmTimer
    void        *context;           // 23 type, name 11, 2456.
};

// zu DNA structure 534, 333
struct ScrArea
{
    ScrArea      *next;            // 1 type, name 333, 0.
    ScrArea      *prev;            // 2 type, name 333, 1.
    ScrVert      *v1;              // 3 type, name 323, 2459.
    ScrVert      *v2;              // 4 type, name 323, 2460.
    ScrVert      *v3;              // 5 type, name 323, 2489.
    ScrVert      *v4;              // 6 type, name 323, 2490.
    bScreen      *full;            // 7 type, name 322, 2491.
    rcti         totrct;           // 8 type, name 18, 51.
    char         spacetype;        // 9 type, name 0, 1839.
    char         butspacetype;     // 10 type, name 0, 2492.
    short        winx;             // 11 type, name 2, 1888.
    short        winy;             // 12 type, name 2, 1889.
    short        headertype;       // 13 type, name 2, 2493.
    short        do_refresh;       // 14 type, name 2, 2447.
    short        flag;             // 15 type, name 2, 18.
    short        region_active_win;// 16 type, name 2, 2494.
    char         temp;             // 17 type, name 0, 2445.
    char         pad;              // 18 type, name 0, 66.
    void         *type;            // Note: using void* on undefined DNA type: SpaceType
    ListBase     spacedata;        // 20 type, name 14, 2495.
    ListBase     regionbase;       // 21 type, name 14, 1838.
    ListBase     handlers;         // 22 type, name 14, 2496.
    ListBase     actionzones;      // 23 type, name 14, 2497.
};

// zu DNA structure 535, 355
struct CurvesModifierData
{
    SequenceModifierData    modifier;     // 1 type, name 353, 740.
    CurveMapping            curve_mapping;// 2 type, name 58, 2615.
};

// zu DNA structure 536, 356
struct HueCorrectModifierData
{
    SequenceModifierData    modifier;     // 1 type, name 353, 740.
    CurveMapping            curve_mapping;// 2 type, name 58, 2615.
};

// zu DNA structure 537, 193
struct bAction
{
    ID          id;           // 1 type, name 22, 31.
    ListBase    curves;       // 2 type, name 14, 2872.
    ListBase    chanbase;     // 3 type, name 14, 2849.
    ListBase    groups;       // 4 type, name 14, 2873.
    ListBase    markers;      // 5 type, name 14, 1778.
    int         flag;         // 6 type, name 4, 18.
    int         active_marker;// 7 type, name 4, 2874.
    int         idroot;       // 8 type, name 4, 2875.
    int         pad;          // 9 type, name 4, 66.
};

// zu DNA structure 538, 500
struct NodeTexSky
{
    NodeTexBase    base;            // 1 type, name 499, 1764.
    int            sky_model;       // 2 type, name 4, 3129.
    float          sun_direction[3];// 3 type, name 7, 3130.
    float          turbidity;       // 4 type, name 7, 3131.
    float          ground_albedo;   // 5 type, name 7, 3132.
};

// zu DNA structure 539, 502
struct NodeTexChecker
{
    NodeTexBase    base;// 1 type, name 499, 1764.
};

// zu DNA structure 540, 503
struct NodeTexBrick
{
    NodeTexBase    base;       // 1 type, name 499, 1764.
    int            offset_freq;// 2 type, name 4, 3136.
    int            squash_freq;// 3 type, name 4, 3137.
    float          offset;     // 4 type, name 7, 134.
    float          squash;     // 5 type, name 7, 3138.
};

// zu DNA structure 541, 505
struct NodeTexGradient
{
    NodeTexBase    base;         // 1 type, name 499, 1764.
    int            gradient_type;// 2 type, name 4, 3139.
    int            pad;          // 3 type, name 4, 66.
};

// zu DNA structure 542, 506
struct NodeTexNoise
{
    NodeTexBase    base;// 1 type, name 499, 1764.
};

// zu DNA structure 543, 507
struct NodeTexVoronoi
{
    NodeTexBase    base;    // 1 type, name 499, 1764.
    int            coloring;// 2 type, name 4, 3140.
    int            pad;     // 3 type, name 4, 66.
};

// zu DNA structure 544, 508
struct NodeTexMusgrave
{
    NodeTexBase    base;         // 1 type, name 499, 1764.
    int            musgrave_type;// 2 type, name 4, 3141.
    int            pad;          // 3 type, name 4, 66.
};

// zu DNA structure 545, 509
struct NodeTexWave
{
    NodeTexBase    base;     // 1 type, name 499, 1764.
    int            wave_type;// 2 type, name 4, 3142.
    int            pad;      // 3 type, name 4, 66.
};

// zu DNA structure 546, 510
struct NodeTexMagic
{
    NodeTexBase    base; // 1 type, name 499, 1764.
    int            depth;// 2 type, name 4, 256.
    int            pad;  // 3 type, name 4, 66.
};

// zu DNA structure 547, 556
struct wmWindow
{
    wmWindow       *next;         // 1 type, name 556, 0.
    wmWindow       *prev;         // 2 type, name 556, 1.
    void           *ghostwin;     // 3 type, name 11, 3528.
    bScreen        *screen;       // 4 type, name 322, 3529.
    bScreen        *newscreen;    // 5 type, name 322, 3530.
    char           screenname[64];// 6 type, name 0, 3531.
    short          posx;          // 7 type, name 2, 3532.
    short          posy;          // 8 type, name 2, 3533.
    short          sizex;         // 9 type, name 2, 2467.
    short          sizey;         // 10 type, name 2, 2468.
    short          windowstate;   // 11 type, name 2, 3534.
    short          monitor;       // 12 type, name 2, 3535.
    short          active;        // 13 type, name 2, 1128.
    short          cursor;        // 14 type, name 2, 2024.
    short          lastcursor;    // 15 type, name 2, 3536.
    short          modalcursor;   // 16 type, name 2, 3537.
    short          grabcursor;    // 17 type, name 2, 3538.
    short          addmousemove;  // 18 type, name 2, 3539.
    int            winid;         // 19 type, name 4, 2443.
    short          lock_pie_event;// 20 type, name 2, 3540.
    short          last_pie_event;// 21 type, name 2, 3541.
    void           *eventstate;   // Note: using void* on undefined DNA type: wmEvent
    void           *curswin;      // Note: using void* on undefined DNA type: wmSubWindow
    void           *tweak;        // Note: using void* on undefined DNA type: wmGesture
    void           *ime_data;     // Note: using void* on undefined DNA type: wmIMEData
    int            drawmethod;    // 26 type, name 4, 3546.
    int            drawfail;      // 27 type, name 4, 3547.
    void           *drawdata;     // 28 type, name 11, 3548.
    ListBase       queue;         // 29 type, name 14, 3515.
    ListBase       handlers;      // 30 type, name 14, 2496.
    ListBase       modalhandlers; // 31 type, name 14, 3549.
    ListBase       subwindows;    // 32 type, name 14, 3550.
    ListBase       gesture;       // 33 type, name 14, 3551.
};

// zu DNA structure 548, 626
struct FreestyleLineStyle
{
    ID           id;                 // 1 type, name 22, 31.
    AnimData     *adt;               // 2 type, name 36, 75.
    float        r;                  // 3 type, name 7, 198.
    float        g;                  // 4 type, name 7, 199.
    float        b;                  // 5 type, name 7, 200.
    float        alpha;              // 6 type, name 7, 446.
    float        thickness;          // 7 type, name 7, 3488.
    int          thickness_position; // 8 type, name 4, 3991.
    float        thickness_ratio;    // 9 type, name 7, 3992.
    int          flag;               // 10 type, name 4, 18.
    int          caps;               // 11 type, name 4, 3993.
    int          chaining;           // 12 type, name 4, 3994.
    int          rounds;             // 13 type, name 4, 3978.
    float        split_length;       // 14 type, name 7, 3995.
    float        min_angle;          // 15 type, name 7, 3996.
    float        max_angle;          // 16 type, name 7, 3997.
    float        min_length;         // 17 type, name 7, 3998.
    float        max_length;         // 18 type, name 7, 3999.
    int          chain_count;        // 19 type, name 4, 4000.
    short        split_dash1;        // 20 type, name 2, 4001.
    short        split_gap1;         // 21 type, name 2, 4002.
    short        split_dash2;        // 22 type, name 2, 4003.
    short        split_gap2;         // 23 type, name 2, 4004.
    short        split_dash3;        // 24 type, name 2, 4005.
    short        split_gap3;         // 25 type, name 2, 4006.
    int          sort_key;           // 26 type, name 4, 4007.
    int          integration_type;   // 27 type, name 4, 4008.
    float        texstep;            // 28 type, name 7, 4009.
    short        texact;             // 29 type, name 2, 387.
    short        pr_texture;         // 30 type, name 2, 407.
    short        use_nodes;          // 31 type, name 2, 340.
    short        pad[3];             // 32 type, name 2, 350.
    short        dash1;              // 33 type, name 2, 4010.
    short        gap1;               // 34 type, name 2, 4011.
    short        dash2;              // 35 type, name 2, 4012.
    short        gap2;               // 36 type, name 2, 4013.
    short        dash3;              // 37 type, name 2, 4014.
    short        gap3;               // 38 type, name 2, 4015.
    int          panel;              // 39 type, name 4, 2111.
    MTex         *mtex[18];          // 40 type, name 51, 406.
    bNodeTree    *nodetree;          // 41 type, name 61, 335.
    ListBase     color_modifiers;    // 42 type, name 14, 4016.
    ListBase     alpha_modifiers;    // 43 type, name 14, 4017.
    ListBase     thickness_modifiers;// 44 type, name 14, 4018.
    ListBase     geometry_modifiers; // 45 type, name 14, 4019.
};

// zu DNA structure 549, 82
struct Mesh
{
    ID             id;          // 1 type, name 22, 31.
    AnimData       *adt;        // 2 type, name 36, 75.
    BoundBox       *bb;         // 3 type, name 73, 550.
    Ipo            *ipo;        // 4 type, name 33, 80.
    Key            *key;        // 5 type, name 35, 612.
    Material       **mat;       // 6 type, name 68, 565.
    MSelect        *mselect;    // 7 type, name 83, 657.
    MPoly          *mpoly;      // 8 type, name 84, 658.
    MTexPoly       *mtpoly;     // 9 type, name 85, 659.
    MLoop          *mloop;      // 10 type, name 86, 660.
    MLoopUV        *mloopuv;    // 11 type, name 87, 661.
    MLoopCol       *mloopcol;   // 12 type, name 88, 662.
    MFace          *mface;      // 13 type, name 89, 663.
    MTFace         *mtface;     // 14 type, name 90, 664.
    TFace          *tface;      // 15 type, name 91, 665.
    MVert          *mvert;      // 16 type, name 92, 666.
    MEdge          *medge;      // 17 type, name 93, 667.
    MDeformVert    *dvert;      // 18 type, name 94, 668.
    MCol           *mcol;       // 19 type, name 95, 669.
    Mesh           *texcomesh;  // 20 type, name 82, 670.
    void           *edit_btmesh;// Note: using void* on undefined DNA type: BMEditMesh
    CustomData     vdata;       // 22 type, name 97, 672.
    CustomData     edata;       // 23 type, name 97, 673.
    CustomData     fdata;       // 24 type, name 97, 674.
    CustomData     pdata;       // 25 type, name 97, 675.
    CustomData     ldata;       // 26 type, name 97, 676.
    int            totvert;     // 27 type, name 4, 53.
    int            totedge;     // 28 type, name 4, 677.
    int            totface;     // 29 type, name 4, 678.
    int            totselect;   // 30 type, name 4, 679.
    int            totpoly;     // 31 type, name 4, 680.
    int            totloop;     // 32 type, name 4, 681.
    int            act_face;    // 33 type, name 4, 682.
    float          loc[3];      // 34 type, name 7, 341.
    float          size[3];     // 35 type, name 7, 189.
    float          rot[3];      // 36 type, name 7, 342.
    int            drawflag;    // 37 type, name 4, 613.
    short          texflag;     // 38 type, name 2, 192.
    short          flag;        // 39 type, name 2, 18.
    float          smoothresh;  // 40 type, name 7, 683.
    int            pad2;        // 41 type, name 4, 29.
    char           cd_flag;     // 42 type, name 0, 684.
    char           pad;         // 43 type, name 0, 66.
    char           subdiv;      // 44 type, name 0, 685.
    char           subdivr;     // 45 type, name 0, 686.
    char           subsurftype; // 46 type, name 0, 687.
    char           editflag;    // 47 type, name 0, 688.
    short          totcol;      // 48 type, name 2, 567.
    Multires       *mr;         // 49 type, name 98, 689.
};

// zu DNA structure 550, 249
struct ToolSettings
{
    VPaint                  *vpaint;                                // 1 type, name 244, 1675.
    VPaint                  *wpaint;                                // 2 type, name 244, 1676.
    Sculpt                  *sculpt;                                // 3 type, name 242, 1018.
    UvSculpt                *uvsculpt;                              // 4 type, name 243, 1677.
    float                   vgroup_weight;                          // 5 type, name 7, 1678.
    float                   doublimit;                              // 6 type, name 7, 1679.
    float                   normalsize;                             // 7 type, name 7, 1680.
    short                   automerge;                              // 8 type, name 2, 1681.
    short                   selectmode;                             // 9 type, name 2, 1626.
    char                    unwrapper;                              // 10 type, name 0, 1682.
    char                    uvcalc_flag;                            // 11 type, name 0, 1683.
    char                    uv_flag;                                // 12 type, name 0, 1684.
    char                    uv_selectmode;                          // 13 type, name 0, 1685.
    float                   uvcalc_margin;                          // 14 type, name 7, 1686.
    short                   autoik_chainlen;                        // 15 type, name 2, 1687.
    char                    gpencil_flags;                          // 16 type, name 0, 1688.
    char                    gpencil_src;                            // 17 type, name 0, 1689.
    char                    pad[4];                                 // 18 type, name 0, 585.
    ImagePaintSettings      imapaint;                               // 19 type, name 239, 1690.
    ParticleEditSettings    particle;                               // 20 type, name 241, 1691.
    float                   proportional_size;                      // 21 type, name 7, 1692.
    float                   select_thresh;                          // 22 type, name 7, 1693.
    short                   autokey_mode;                           // 23 type, name 2, 1694.
    short                   autokey_flag;                           // 24 type, name 2, 1695.
    char                    multires_subdiv_type;                   // 25 type, name 0, 1696.
    char                    pad3[1];                                // 26 type, name 0, 1697.
    short                   skgen_resolution;                       // 27 type, name 2, 1698.
    float                   skgen_threshold_internal;               // 28 type, name 7, 1699.
    float                   skgen_threshold_external;               // 29 type, name 7, 1700.
    float                   skgen_length_ratio;                     // 30 type, name 7, 1701.
    float                   skgen_length_limit;                     // 31 type, name 7, 1702.
    float                   skgen_angle_limit;                      // 32 type, name 7, 1703.
    float                   skgen_correlation_limit;                // 33 type, name 7, 1704.
    float                   skgen_symmetry_limit;                   // 34 type, name 7, 1705.
    float                   skgen_retarget_angle_weight;            // 35 type, name 7, 1706.
    float                   skgen_retarget_length_weight;           // 36 type, name 7, 1707.
    float                   skgen_retarget_distance_weight;         // 37 type, name 7, 1708.
    short                   skgen_options;                          // 38 type, name 2, 1709.
    char                    skgen_postpro;                          // 39 type, name 0, 1710.
    char                    skgen_postpro_passes;                   // 40 type, name 0, 1711.
    char                    skgen_subdivisions[3];                  // 41 type, name 0, 1712.
    char                    skgen_multi_level;                      // 42 type, name 0, 1713.
    Object                  *skgen_template;                        // 43 type, name 29, 1714.
    char                    bone_sketching;                         // 44 type, name 0, 1715.
    char                    bone_sketching_convert;                 // 45 type, name 0, 1716.
    char                    skgen_subdivision_number;               // 46 type, name 0, 1717.
    char                    skgen_retarget_options;                 // 47 type, name 0, 1718.
    char                    skgen_retarget_roll;                    // 48 type, name 0, 1719.
    char                    skgen_side_string[8];                   // 49 type, name 0, 1720.
    char                    skgen_num_string[8];                    // 50 type, name 0, 1721.
    char                    edge_mode;                              // 51 type, name 0, 1722.
    char                    edge_mode_live_unwrap;                  // 52 type, name 0, 1723.
    char                    snap_mode;                              // 53 type, name 0, 1724.
    char                    snap_node_mode;                         // 54 type, name 0, 1725.
    char                    snap_uv_mode;                           // 55 type, name 0, 1726.
    short                   snap_flag;                              // 56 type, name 2, 1727.
    short                   snap_target;                            // 57 type, name 2, 1728.
    short                   proportional;                           // 58 type, name 2, 1729.
    short                   prop_mode;                              // 59 type, name 2, 1730.
    char                    proportional_objects;                   // 60 type, name 0, 1731.
    char                    proportional_mask;                      // 61 type, name 0, 1732.
    char                    auto_normalize;                         // 62 type, name 0, 1733.
    char                    multipaint;                             // 63 type, name 0, 1734.
    char                    weightuser;                             // 64 type, name 0, 1735.
    char                    vgroupsubset;                           // 65 type, name 0, 1736.
    int                     use_uv_sculpt;                          // 66 type, name 4, 1737.
    int                     uv_sculpt_settings;                     // 67 type, name 4, 1738.
    int                     uv_sculpt_tool;                         // 68 type, name 4, 1739.
    int                     uv_relax_method;                        // 69 type, name 4, 1740.
    short                   sculpt_paint_settings;                  // 70 type, name 2, 1741.
    short                   pad5;                                   // 71 type, name 2, 369.
    int                     sculpt_paint_unified_size;              // 72 type, name 4, 1742.
    float                   sculpt_paint_unified_unprojected_radius;// 73 type, name 7, 1743.
    float                   sculpt_paint_unified_alpha;             // 74 type, name 7, 1744.
    UnifiedPaintSettings    unified_paint_settings;                 // 75 type, name 246, 1745.
    MeshStatVis             statvis;                                // 76 type, name 248, 1746.
};

// zu DNA structure 551, 291
struct SpaceImage
{
    SpaceLink        *next;           // 1 type, name 268, 0.
    SpaceLink        *prev;           // 2 type, name 268, 1.
    ListBase         regionbase;      // 3 type, name 14, 1838.
    int              spacetype;       // 4 type, name 4, 1839.
    int              flag;            // 5 type, name 4, 18.
    Image            *image;          // 6 type, name 46, 788.
    ImageUser        iuser;           // 7 type, name 43, 334.
    CurveMapping     *cumap;          // 8 type, name 58, 1964.
    Scopes           scopes;          // 9 type, name 292, 1930.
    Histogram        sample_line_hist;// 10 type, name 293, 1965.
    bGPdata          *gpd;            // 11 type, name 195, 1031.
    float            cursor[2];       // 12 type, name 7, 1966.
    float            xof;             // 13 type, name 7, 634.
    float            yof;             // 14 type, name 7, 635.
    float            zoom;            // 15 type, name 7, 1927.
    float            centx;           // 16 type, name 7, 1967.
    float            centy;           // 17 type, name 7, 1968.
    char             mode;            // 18 type, name 0, 351.
    char             pin;             // 19 type, name 0, 1969.
    short            pad;             // 20 type, name 2, 66.
    short            curtile;         // 21 type, name 2, 1970.
    short            lock;            // 22 type, name 2, 1971.
    char             dt_uv;           // 23 type, name 0, 1972.
    char             sticky;          // 24 type, name 0, 1973.
    char             dt_uvstretch;    // 25 type, name 0, 1974.
    char             around;          // 26 type, name 0, 1854.
    MaskSpaceInfo    mask_info;       // 27 type, name 283, 1975.
};

// zu DNA structure 552, 455
struct bNode
{
    bNode         *next;         // 1 type, name 455, 0.
    bNode         *prev;         // 2 type, name 455, 1.
    bNode         *new_node;     // 3 type, name 455, 2988.
    IDProperty    *prop;         // 4 type, name 21, 2322.
    void          *typeinfo;     // Note: using void* on undefined DNA type: bNodeType
    char          idname[64];    // 6 type, name 0, 2476.
    char          name[64];      // 7 type, name 0, 19.
    int           flag;          // 8 type, name 4, 18.
    short         type;          // 9 type, name 2, 16.
    short         pad;           // 10 type, name 2, 66.
    short         done;          // 11 type, name 2, 2539.
    short         level;         // 12 type, name 2, 717.
    short         lasty;         // 13 type, name 2, 2989.
    short         menunr;        // 14 type, name 2, 1979.
    short         stack_index;   // 15 type, name 2, 2981.
    short         nr;            // 16 type, name 2, 2652.
    float         color[3];      // 17 type, name 7, 108.
    ListBase      inputs;        // 18 type, name 14, 2693.
    ListBase      outputs;       // 19 type, name 14, 2990.
    bNode         *parent;       // 20 type, name 455, 36.
    ID            *id;           // 21 type, name 22, 2014.
    void          *storage;      // 22 type, name 11, 2974.
    bNode         *original;     // 23 type, name 455, 2991.
    ListBase      internal_links;// 24 type, name 14, 2992.
    float         locx;          // 25 type, name 7, 2978.
    float         locy;          // 26 type, name 7, 2979.
    float         width;         // 27 type, name 7, 619.
    float         height;        // 28 type, name 7, 804.
    float         miniwidth;     // 29 type, name 7, 2993.
    float         offsetx;       // 30 type, name 7, 2994.
    float         offsety;       // 31 type, name 7, 2995.
    int           update;        // 32 type, name 4, 2996.
    char          label[64];     // 33 type, name 0, 2997.
    short         custom1;       // 34 type, name 2, 2998.
    short         custom2;       // 35 type, name 2, 2999.
    float         custom3;       // 36 type, name 7, 3000.
    float         custom4;       // 37 type, name 7, 3001.
    short         need_exec;     // 38 type, name 2, 3002.
    short         exec;          // 39 type, name 2, 3003.
    void          *threaddata;   // 40 type, name 11, 3004.
    rctf          totr;          // 41 type, name 19, 3005.
    rctf          butr;          // 42 type, name 19, 3006.
    rctf          prvr;          // 43 type, name 19, 3007.
    short         preview_xsize; // 44 type, name 2, 3008.
    short         preview_ysize; // 45 type, name 2, 3009.
    int           pad2;          // 46 type, name 4, 29.
    void          *block;        // Note: using void* on undefined DNA type: uiBlock
};

// zu DNA structure 553, 501
struct NodeTexImage
{
    NodeTexBase    base;            // 1 type, name 499, 1764.
    ImageUser      iuser;           // 2 type, name 43, 334.
    int            color_space;     // 3 type, name 4, 3133.
    int            projection;      // 4 type, name 4, 3134.
    float          projection_blend;// 5 type, name 7, 3135.
    int            interpolation;   // 6 type, name 4, 2603.
};

// zu DNA structure 554, 504
struct NodeTexEnvironment
{
    NodeTexBase    base;       // 1 type, name 499, 1764.
    ImageUser      iuser;      // 2 type, name 43, 334.
    int            color_space;// 3 type, name 4, 3133.
    int            projection; // 4 type, name 4, 3134.
};

// zu DNA structure 555, 267
struct View3D
{
    SpaceLink        *next;               // 1 type, name 268, 0.
    SpaceLink        *prev;               // 2 type, name 268, 1.
    ListBase         regionbase;          // 3 type, name 14, 1838.
    int              spacetype;           // 4 type, name 4, 1839.
    float            blockscale;          // 5 type, name 7, 1840.
    short            blockhandler[8];     // 6 type, name 2, 1841.
    float            viewquat[4];         // 7 type, name 7, 1818.
    float            dist;                // 8 type, name 7, 358.
    float            bundle_size;         // 9 type, name 7, 1842.
    char             bundle_drawtype;     // 10 type, name 0, 1843.
    char             pad[3];              // 11 type, name 0, 350.
    int              lay_prev;            // 12 type, name 4, 1844.
    int              lay_used;            // 13 type, name 4, 1845.
    short            persp;               // 14 type, name 2, 1824.
    short            view;                // 15 type, name 2, 1799.
    Object           *camera;             // 16 type, name 29, 1603.
    Object           *ob_centre;          // 17 type, name 29, 1846.
    rctf             render_border;       // 18 type, name 19, 1847.
    ListBase         bgpicbase;           // 19 type, name 14, 1848.
    BGpic            *bgpic;              // 20 type, name 259, 1849.
    View3D           *localvd;            // 21 type, name 267, 1811.
    char             ob_centre_bone[64];  // 22 type, name 0, 1850.
    int              lay;                 // 23 type, name 4, 1054.
    int              layact;              // 24 type, name 4, 1771.
    short            drawtype;            // 25 type, name 2, 1851.
    short            ob_centre_cursor;    // 26 type, name 2, 1852.
    short            scenelock;           // 27 type, name 2, 1853.
    short            around;              // 28 type, name 2, 1854.
    short            flag;                // 29 type, name 2, 18.
    short            flag2;               // 30 type, name 2, 566.
    float            lens;                // 31 type, name 7, 120.
    float            grid;                // 32 type, name 7, 1855.
    float            near;                // 33 type, name 7, 1856.
    float            far;                 // 34 type, name 7, 1857.
    float            ofs[3];              // 35 type, name 7, 188.
    float            cursor[3];           // 36 type, name 7, 1767.
    short            matcap_icon;         // 37 type, name 2, 1858.
    short            gridlines;           // 38 type, name 2, 1859.
    short            gridsubdiv;          // 39 type, name 2, 1860.
    char             gridflag;            // 40 type, name 0, 1861.
    char             twtype;              // 41 type, name 0, 1862.
    char             twmode;              // 42 type, name 0, 1863.
    char             twflag;              // 43 type, name 0, 1864.
    short            flag3;               // 44 type, name 2, 1865.
    ListBase         afterdraw_transp;    // 45 type, name 14, 1866.
    ListBase         afterdraw_xray;      // 46 type, name 14, 1867.
    ListBase         afterdraw_xraytransp;// 47 type, name 14, 1868.
    char             zbuf;                // 48 type, name 0, 1869.
    char             transp;              // 49 type, name 0, 693.
    char             xray;                // 50 type, name 0, 1870.
    char             pad3[5];             // 51 type, name 0, 1871.
    GPUFXSettings    fx_settings;         // 52 type, name 41, 1872.
    void             *properties_storage; // 53 type, name 11, 1873.
    Material         *defmaterial;        // 54 type, name 68, 1874.
    bGPdata          *gpd;                // 55 type, name 195, 1031.
};

// zu DNA structure 556, 272
struct SpaceButs
{
    SpaceLink    *next;               // 1 type, name 268, 0.
    SpaceLink    *prev;               // 2 type, name 268, 1.
    ListBase     regionbase;          // 3 type, name 14, 1838.
    int          spacetype;           // 4 type, name 4, 1839.
    float        blockscale;          // 5 type, name 7, 1840.
    short        blockhandler[8];     // 6 type, name 2, 1841.
    View2D       v2d;                 // 7 type, name 269, 1896.
    short        mainb;               // 8 type, name 2, 1897.
    short        mainbo;              // 9 type, name 2, 1898.
    short        mainbuser;           // 10 type, name 2, 1899.
    short        re_align;            // 11 type, name 2, 1900.
    short        align;               // 12 type, name 2, 1887.
    short        preview;             // 13 type, name 2, 1901.
    short        texture_context;     // 14 type, name 2, 1902.
    short        texture_context_prev;// 15 type, name 2, 1903.
    char         flag;                // 16 type, name 0, 18.
    char         pad[7];              // 17 type, name 0, 115.
    void         *path;               // 18 type, name 11, 1904.
    int          pathflag;            // 19 type, name 4, 1905.
    int          dataicon;            // 20 type, name 4, 1906.
    ID           *pinid;              // 21 type, name 22, 1907.
    void         *texuser;            // 22 type, name 11, 1908.
};

// zu DNA structure 557, 278
struct SpaceNla
{
    SpaceLink     *next;          // 1 type, name 268, 0.
    SpaceLink     *prev;          // 2 type, name 268, 1.
    ListBase      regionbase;     // 3 type, name 14, 1838.
    int           spacetype;      // 4 type, name 4, 1839.
    float         blockscale;     // 5 type, name 7, 1840.
    short         blockhandler[8];// 6 type, name 2, 1841.
    short         autosnap;       // 7 type, name 2, 1919.
    short         flag;           // 8 type, name 2, 18.
    int           pad;            // 9 type, name 4, 66.
    bDopeSheet    *ads;           // 10 type, name 277, 1917.
    View2D        v2d;            // 11 type, name 269, 1896.
};

// zu DNA structure 558, 61
struct bNodeTree
{
    ID                   id;               // 1 type, name 22, 31.
    AnimData             *adt;             // 2 type, name 36, 75.
    void                 *typeinfo;        // Note: using void* on undefined DNA type: bNodeTreeType
    char                 idname[64];       // 4 type, name 0, 2476.
    void                 *interface_type;  // Note: using void* on undefined DNA type: StructRNA
    bGPdata              *gpd;             // 6 type, name 195, 1031.
    float                view_center[2];   // 7 type, name 7, 2012.
    ListBase             nodes;            // 8 type, name 14, 3021.
    ListBase             links;            // 9 type, name 14, 3022.
    int                  type;             // 10 type, name 4, 16.
    int                  init;             // 11 type, name 4, 3023.
    int                  cur_index;        // 12 type, name 4, 3024.
    int                  flag;             // 13 type, name 4, 18.
    int                  update;           // 14 type, name 4, 2996.
    short                is_updating;      // 15 type, name 2, 3025.
    short                done;             // 16 type, name 2, 2539.
    int                  pad2;             // 17 type, name 4, 29.
    int                  nodetype;         // 18 type, name 4, 3026.
    short                edit_quality;     // 19 type, name 2, 3027.
    short                render_quality;   // 20 type, name 2, 3028.
    int                  chunksize;        // 21 type, name 4, 3029.
    rctf                 viewer_border;    // 22 type, name 19, 3030.
    ListBase             inputs;           // 23 type, name 14, 2693.
    ListBase             outputs;          // 24 type, name 14, 2990.
    void                 *previews;        // Note: using void* on undefined DNA type: bNodeInstanceHash
    bNodeInstanceKey     active_viewer_key;// 26 type, name 298, 3032.
    int                  pad;              // 27 type, name 4, 66.
    void                 *execdata;        // Note: using void* on undefined DNA type: bNodeTreeExec
    void                 (*progress)();    // 29 type, name 11, 3034.
    void                 (*stats_draw)();  // 30 type, name 11, 3035.
    int                  (*test_break)();  // 31 type, name 4, 3036.
    void                 (*update_draw)(); // 32 type, name 11, 3037.
    void                 *tbh;             // 33 type, name 11, 3038.
    void                 *prh;             // 34 type, name 11, 3039.
    void                 *sdh;             // 35 type, name 11, 3040.
    void                 *udh;             // 36 type, name 11, 3041.
};

// zu DNA structure 559, 276
struct SpaceIpo
{
    SpaceLink     *next;          // 1 type, name 268, 0.
    SpaceLink     *prev;          // 2 type, name 268, 1.
    ListBase      regionbase;     // 3 type, name 14, 1838.
    int           spacetype;      // 4 type, name 4, 1839.
    float         blockscale;     // 5 type, name 7, 1840.
    short         blockhandler[8];// 6 type, name 2, 1841.
    View2D        v2d;            // 7 type, name 269, 1896.
    bDopeSheet    *ads;           // 8 type, name 277, 1917.
    ListBase      ghostCurves;    // 9 type, name 14, 1918.
    short         mode;           // 10 type, name 2, 351.
    short         autosnap;       // 11 type, name 2, 1919.
    int           flag;           // 12 type, name 4, 18.
    float         cursorVal;      // 13 type, name 7, 1920.
    int           around;         // 14 type, name 4, 1854.
};

// zu DNA structure 560, 280
struct SpaceTime
{
    SpaceLink    *next;        // 1 type, name 268, 0.
    SpaceLink    *prev;        // 2 type, name 268, 1.
    ListBase     regionbase;   // 3 type, name 14, 1838.
    int          spacetype;    // 4 type, name 4, 1839.
    float        blockscale;   // 5 type, name 7, 1840.
    View2D       v2d;          // 6 type, name 269, 1896.
    ListBase     caches;       // 7 type, name 14, 1922.
    int          cache_display;// 8 type, name 4, 1923.
    int          flag;         // 9 type, name 4, 18.
};

// zu DNA structure 561, 281
struct SpaceSeq
{
    SpaceLink          *next;          // 1 type, name 268, 0.
    SpaceLink          *prev;          // 2 type, name 268, 1.
    ListBase           regionbase;     // 3 type, name 14, 1838.
    int                spacetype;      // 4 type, name 4, 1839.
    float              blockscale;     // 5 type, name 7, 1840.
    short              blockhandler[8];// 6 type, name 2, 1841.
    View2D             v2d;            // 7 type, name 269, 1896.
    float              xof;            // 8 type, name 7, 634.
    float              yof;            // 9 type, name 7, 635.
    short              mainb;          // 10 type, name 2, 1897.
    short              render_size;    // 11 type, name 2, 1924.
    short              chanshown;      // 12 type, name 2, 1925.
    short              zebra;          // 13 type, name 2, 1926.
    int                flag;           // 14 type, name 4, 18.
    float              zoom;           // 15 type, name 7, 1927.
    int                view;           // 16 type, name 4, 1799.
    int                overlay_type;   // 17 type, name 4, 1928.
    int                draw_flag;      // 18 type, name 4, 1929.
    int                pad;            // 19 type, name 4, 66.
    bGPdata            *gpd;           // 20 type, name 195, 1031.
    SequencerScopes    scopes;         // 21 type, name 282, 1930.
};

// zu DNA structure 562, 273
struct SpaceOops
{
    SpaceLink        *next;            // 1 type, name 268, 0.
    SpaceLink        *prev;            // 2 type, name 268, 1.
    ListBase         regionbase;       // 3 type, name 14, 1838.
    int              spacetype;        // 4 type, name 4, 1839.
    float            blockscale;       // 5 type, name 7, 1840.
    short            blockhandler[8];  // 6 type, name 2, 1841.
    View2D           v2d;              // 7 type, name 269, 1896.
    ListBase         tree;             // 8 type, name 14, 1909.
    void             *treestore;       // Note: using void* on undefined DNA type: BLI_mempool
    char             search_string[32];// 10 type, name 0, 1911.
    TreeStoreElem    search_tse;       // 11 type, name 275, 1912.
    short            flag;             // 12 type, name 2, 18.
    short            outlinevis;       // 13 type, name 2, 1913.
    short            storeflag;        // 14 type, name 2, 1914.
    short            search_flags;     // 15 type, name 2, 1915.
    void             *treehash;        // 16 type, name 11, 1916.
};

// zu DNA structure 563, 299
struct SpaceNode
{
    SpaceLink    *next;          // 1 type, name 268, 0.
    SpaceLink    *prev;          // 2 type, name 268, 1.
    ListBase     regionbase;     // 3 type, name 14, 1838.
    int          spacetype;      // 4 type, name 4, 1839.
    float        blockscale;     // 5 type, name 7, 1840.
    short        blockhandler[8];// 6 type, name 2, 1841.
    View2D       v2d;            // 7 type, name 269, 1896.
    ID           *id;            // 8 type, name 22, 2014.
    ID           *from;          // 9 type, name 22, 81.
    short        flag;           // 10 type, name 2, 18.
    short        pad1;           // 11 type, name 2, 68.
    float        aspect;         // 12 type, name 7, 2015.
    float        pad2;           // 13 type, name 7, 29.
    float        xof;            // 14 type, name 7, 634.
    float        yof;            // 15 type, name 7, 635.
    float        zoom;           // 16 type, name 7, 1927.
    float        cursor[2];      // 17 type, name 7, 1966.
    ListBase     treepath;       // 18 type, name 14, 2016.
    bNodeTree    *nodetree;      // 19 type, name 61, 335.
    bNodeTree    *edittree;      // 20 type, name 61, 2017.
    char         tree_idname[64];// 21 type, name 0, 2018.
    int          treetype;       // 22 type, name 4, 2019.
    int          pad3;           // 23 type, name 4, 517.
    short        texfrom;        // 24 type, name 2, 2020.
    short        shaderfrom;     // 25 type, name 2, 2021.
    short        recalc;         // 26 type, name 2, 257.
    short        pad4;           // 27 type, name 2, 1499.
    ListBase     linkdrag;       // 28 type, name 14, 2022.
    bGPdata      *gpd;           // 29 type, name 195, 1031.
};

// zu DNA structure 564, 416
struct SpaceAction
{
    SpaceLink     *next;          // 1 type, name 268, 0.
    SpaceLink     *prev;          // 2 type, name 268, 1.
    ListBase      regionbase;     // 3 type, name 14, 1838.
    int           spacetype;      // 4 type, name 4, 1839.
    float         blockscale;     // 5 type, name 7, 1840.
    short         blockhandler[8];// 6 type, name 2, 1841.
    View2D        v2d;            // 7 type, name 269, 1896.
    bAction       *action;        // 8 type, name 193, 1028.
    bDopeSheet    ads;            // 9 type, name 277, 2880.
    char          mode;           // 10 type, name 0, 351.
    char          autosnap;       // 11 type, name 0, 1919.
    short         flag;           // 12 type, name 2, 18.
    float         timeslide;      // 13 type, name 7, 2881.
};

// zu DNA structure 565, 602
struct MovieTracking
{
    MovieTrackingSettings          settings;        // 1 type, name 609, 3828.
    MovieTrackingCamera            camera;          // 2 type, name 606, 3829.
    ListBase                       tracks;          // 3 type, name 14, 3816.
    ListBase                       plane_tracks;    // 4 type, name 14, 3817.
    MovieTrackingReconstruction    reconstruction;  // 5 type, name 611, 3818.
    MovieTrackingStabilization     stabilization;   // 6 type, name 610, 3830.
    MovieTrackingTrack             *act_track;      // 7 type, name 604, 3831.
    MovieTrackingPlaneTrack        *act_plane_track;// 8 type, name 608, 3832.
    ListBase                       objects;         // 9 type, name 14, 3833.
    int                            objectnr;        // 10 type, name 4, 3834.
    int                            tot_object;      // 11 type, name 4, 3835.
    MovieTrackingStats             *stats;          // 12 type, name 613, 1775.
    MovieTrackingDopesheet         dopesheet;       // 13 type, name 616, 3836.
};

// zu DNA structure 566, 321
struct UserDef
{
    int               versionfile;                // 1 type, name 4, 2331.
    int               subversionfile;             // 2 type, name 4, 2332.
    int               flag;                       // 3 type, name 4, 18.
    int               dupflag;                    // 4 type, name 4, 2333.
    int               savetime;                   // 5 type, name 4, 2334.
    char              tempdir[768];               // 6 type, name 0, 2335.
    char              fontdir[768];               // 7 type, name 0, 2336.
    char              renderdir[1024];            // 8 type, name 0, 2337.
    char              render_cachedir[768];       // 9 type, name 0, 2338.
    char              textudir[768];              // 10 type, name 0, 2339.
    char              pythondir[768];             // 11 type, name 0, 2340.
    char              sounddir[768];              // 12 type, name 0, 2341.
    char              i18ndir[768];               // 13 type, name 0, 2342.
    char              image_editor[1024];         // 14 type, name 0, 2343.
    char              anim_player[1024];          // 15 type, name 0, 2344.
    int               anim_player_preset;         // 16 type, name 4, 2345.
    short             v2d_min_gridsize;           // 17 type, name 2, 2346.
    short             timecode_style;             // 18 type, name 2, 2347.
    short             versions;                   // 19 type, name 2, 2348.
    short             dbl_click_time;             // 20 type, name 2, 2349.
    short             gameflags;                  // 21 type, name 2, 2350.
    short             wheellinescroll;            // 22 type, name 2, 2351.
    int               uiflag;                     // 23 type, name 4, 2352.
    int               uiflag2;                    // 24 type, name 4, 2353.
    int               language;                   // 25 type, name 4, 2354.
    short             userpref;                   // 26 type, name 2, 2355.
    short             viewzoom;                   // 27 type, name 2, 2356.
    int               mixbufsize;                 // 28 type, name 4, 2357.
    int               audiodevice;                // 29 type, name 4, 2358.
    int               audiorate;                  // 30 type, name 4, 2359.
    int               audioformat;                // 31 type, name 4, 2360.
    int               audiochannels;              // 32 type, name 4, 2361.
    int               scrollback;                 // 33 type, name 4, 2025.
    int               dpi;                        // 34 type, name 4, 2362.
    char              pad2[2];                    // 35 type, name 0, 656.
    short             transopts;                  // 36 type, name 2, 2363.
    short             menuthreshold1;             // 37 type, name 2, 2364.
    short             menuthreshold2;             // 38 type, name 2, 2365.
    ListBase          themes;                     // 39 type, name 14, 2366.
    ListBase          uifonts;                    // 40 type, name 14, 2367.
    ListBase          uistyles;                   // 41 type, name 14, 2368.
    ListBase          keymaps;                    // 42 type, name 14, 2369.
    ListBase          user_keymaps;               // 43 type, name 14, 2370.
    ListBase          addons;                     // 44 type, name 14, 2371.
    ListBase          autoexec_paths;             // 45 type, name 14, 2372.
    char              keyconfigstr[64];           // 46 type, name 0, 2373.
    short             undosteps;                  // 47 type, name 2, 2374.
    short             undomemory;                 // 48 type, name 2, 2375.
    short             gp_manhattendist;           // 49 type, name 2, 2376.
    short             gp_euclideandist;           // 50 type, name 2, 2377.
    short             gp_eraser;                  // 51 type, name 2, 2378.
    short             gp_settings;                // 52 type, name 2, 2379.
    short             tb_leftmouse;               // 53 type, name 2, 2380.
    short             tb_rightmouse;              // 54 type, name 2, 2381.
    SolidLight        light[3];                   // 55 type, name 319, 2382.
    short             tw_hotspot;                 // 56 type, name 2, 2383.
    short             tw_flag;                    // 57 type, name 2, 2384.
    short             tw_handlesize;              // 58 type, name 2, 2385.
    short             tw_size;                    // 59 type, name 2, 2386.
    short             textimeout;                 // 60 type, name 2, 2387.
    short             texcollectrate;             // 61 type, name 2, 2388.
    short             wmdrawmethod;               // 62 type, name 2, 2389.
    short             dragthreshold;              // 63 type, name 2, 2390.
    int               memcachelimit;              // 64 type, name 4, 2391.
    int               prefetchframes;             // 65 type, name 4, 2392.
    short             frameserverport;            // 66 type, name 2, 2393.
    short             pad_rot_angle;              // 67 type, name 2, 2394.
    short             obcenter_dia;               // 68 type, name 2, 2395.
    short             rvisize;                    // 69 type, name 2, 2396.
    short             rvibright;                  // 70 type, name 2, 2397.
    short             recent_files;               // 71 type, name 2, 2398.
    short             smooth_viewtx;              // 72 type, name 2, 2399.
    short             glreslimit;                 // 73 type, name 2, 2400.
    short             curssize;                   // 74 type, name 2, 2401.
    short             color_picker_type;          // 75 type, name 2, 2402.
    char              ipo_new;                    // 76 type, name 0, 2403.
    char              keyhandles_new;             // 77 type, name 0, 2404.
    char              gpu_select_method;          // 78 type, name 0, 2405.
    char              pad1;                       // 79 type, name 0, 68.
    short             scrcastfps;                 // 80 type, name 2, 2406.
    short             scrcastwait;                // 81 type, name 2, 2407.
    short             widget_unit;                // 82 type, name 2, 2408.
    short             anisotropic_filter;         // 83 type, name 2, 2409.
    short             use_16bit_textures;         // 84 type, name 2, 2410.
    short             use_gpu_mipmap;             // 85 type, name 2, 2411.
    float             ndof_sensitivity;           // 86 type, name 7, 2412.
    float             ndof_orbit_sensitivity;     // 87 type, name 7, 2413.
    int               ndof_flag;                  // 88 type, name 4, 2414.
    short             ogl_multisamples;           // 89 type, name 2, 2415.
    short             image_draw_method;          // 90 type, name 2, 2416.
    float             glalphaclip;                // 91 type, name 7, 2417.
    short             autokey_mode;               // 92 type, name 2, 1694.
    short             autokey_flag;               // 93 type, name 2, 1695.
    short             text_render;                // 94 type, name 2, 2418.
    short             pad9;                       // 95 type, name 2, 2419.
    ColorBand         coba_weight;                // 96 type, name 54, 2420.
    float             sculpt_paint_overlay_col[3];// 97 type, name 7, 2421.
    float             gpencil_new_layer_col[4];   // 98 type, name 7, 2422.
    short             tweak_threshold;            // 99 type, name 2, 2423.
    char              navigation_mode;            // 100 type, name 0, 2424.
    char              pad;                        // 101 type, name 0, 66.
    char              author[80];                 // 102 type, name 0, 2425.
    char              font_path_ui[1024];         // 103 type, name 0, 2426.
    int               compute_device_type;        // 104 type, name 4, 2427.
    int               compute_device_id;          // 105 type, name 4, 2428.
    float             fcu_inactive_alpha;         // 106 type, name 7, 2429.
    float             pixelsize;                  // 107 type, name 7, 2430.
    int               virtual_pixel;              // 108 type, name 4, 2431.
    short             pie_interaction_type;       // 109 type, name 2, 2432.
    short             pie_initial_timeout;        // 110 type, name 2, 2433.
    short             pie_animation_timeout;      // 111 type, name 2, 2434.
    short             pie_menu_confirm;           // 112 type, name 2, 2435.
    short             pie_menu_radius;            // 113 type, name 2, 2436.
    short             pie_menu_threshold;         // 114 type, name 2, 2437.
    WalkNavigation    walk_navigation;            // 115 type, name 320, 2438.
};

// zu DNA structure 567, 555
struct wmWindowManager
{
    ID             id;                 // 1 type, name 22, 31.
    wmWindow       *windrawable;       // 2 type, name 556, 3508.
    wmWindow       *winactive;         // 3 type, name 556, 3509.
    ListBase       windows;            // 4 type, name 14, 3510.
    int            initialized;        // 5 type, name 4, 3511.
    short          file_saved;         // 6 type, name 2, 3512.
    short          op_undo_depth;      // 7 type, name 2, 3513.
    ListBase       operators;          // 8 type, name 14, 3514.
    ListBase       queue;              // 9 type, name 14, 3515.
    ReportList     reports;            // 10 type, name 554, 3516.
    ListBase       jobs;               // 11 type, name 14, 3517.
    ListBase       paintcursors;       // 12 type, name 14, 3518.
    ListBase       drags;              // 13 type, name 14, 3519.
    ListBase       keyconfigs;         // 14 type, name 14, 3520.
    wmKeyConfig    *defaultconf;       // 15 type, name 557, 3521.
    wmKeyConfig    *addonconf;         // 16 type, name 557, 3522.
    wmKeyConfig    *userconf;          // 17 type, name 557, 3523.
    ListBase       timers;             // 18 type, name 14, 3524.
    void           *autosavetimer;     // Note: using void* on undefined DNA type: wmTimer
    char           is_interface_locked;// 20 type, name 0, 3526.
    char           par[7];             // 21 type, name 0, 3527.
};

// zu DNA structure 568, 229
struct RenderData
{
    ImageFormatData           im_format;               // 1 type, name 225, 1455.
    AviCodecData              *avicodecdata;           // 2 type, name 218, 1461.
    QuicktimeCodecData        *qtcodecdata;            // 3 type, name 219, 1462.
    QuicktimeCodecSettings    qtcodecsettings;         // 4 type, name 220, 1463.
    FFMpegCodecData           ffcodecdata;             // 5 type, name 221, 1464.
    int                       cfra;                    // 6 type, name 4, 845.
    int                       sfra;                    // 7 type, name 4, 135.
    int                       efra;                    // 8 type, name 4, 1245.
    float                     subframe;                // 9 type, name 7, 1465.
    int                       psfra;                   // 10 type, name 4, 1466.
    int                       pefra;                   // 11 type, name 4, 1467.
    int                       images;                  // 12 type, name 4, 1468.
    int                       framapto;                // 13 type, name 4, 1469.
    short                     flag;                    // 14 type, name 2, 18.
    short                     threads;                 // 15 type, name 2, 1274.
    float                     framelen;                // 16 type, name 7, 1470.
    float                     blurfac;                 // 17 type, name 7, 1471.
    float                     edgeR;                   // 18 type, name 7, 1472.
    float                     edgeG;                   // 19 type, name 7, 1473.
    float                     edgeB;                   // 20 type, name 7, 1474.
    short                     fullscreen;              // 21 type, name 2, 1475.
    short                     xplay;                   // 22 type, name 2, 1476.
    short                     yplay;                   // 23 type, name 2, 1477.
    short                     freqplay;                // 24 type, name 2, 1478.
    short                     depth;                   // 25 type, name 2, 256.
    short                     attrib;                  // 26 type, name 2, 1479.
    int                       frame_step;              // 27 type, name 4, 1480.
    short                     stereomode;              // 28 type, name 2, 1481.
    short                     dimensionspreset;        // 29 type, name 2, 1482.
    short                     filtertype;              // 30 type, name 2, 373.
    short                     size;                    // 31 type, name 2, 101.
    short                     maximsize;               // 32 type, name 2, 1483.
    short                     pad6;                    // 33 type, name 2, 1484.
    int                       xsch;                    // 34 type, name 4, 1485.
    int                       ysch;                    // 35 type, name 4, 1486.
    short                     xparts;                  // 36 type, name 2, 1487.
    short                     yparts;                  // 37 type, name 2, 1488.
    int                       tilex;                   // 38 type, name 4, 1489.
    int                       tiley;                   // 39 type, name 4, 1490.
    short                     planes;                  // 40 type, name 2, 1443.
    short                     imtype;                  // 41 type, name 2, 1442.
    short                     subimtype;               // 42 type, name 2, 1491.
    short                     quality;                 // 43 type, name 2, 1444.
    short                     displaymode;             // 44 type, name 2, 1492.
    char                      use_lock_interface;      // 45 type, name 0, 1493.
    char                      pad7;                    // 46 type, name 0, 1494.
    int                       scemode;                 // 47 type, name 4, 1495.
    int                       mode;                    // 48 type, name 4, 351.
    int                       raytrace_options;        // 49 type, name 4, 1496.
    short                     raytrace_structure;      // 50 type, name 2, 1497.
    short                     pad1;                    // 51 type, name 2, 68.
    short                     ocres;                   // 52 type, name 2, 1498.
    short                     pad4;                    // 53 type, name 2, 1499.
    short                     alphamode;               // 54 type, name 2, 1500.
    short                     osa;                     // 55 type, name 2, 1501.
    short                     frs_sec;                 // 56 type, name 2, 1502.
    short                     edgeint;                 // 57 type, name 2, 1503.
    rctf                      safety;                  // 58 type, name 19, 1504.
    rctf                      border;                  // 59 type, name 19, 1505.
    rcti                      disprect;                // 60 type, name 18, 1506.
    ListBase                  layers;                  // 61 type, name 14, 1507.
    short                     actlay;                  // 62 type, name 2, 1508.
    short                     mblur_samples;           // 63 type, name 2, 1509.
    float                     xasp;                    // 64 type, name 7, 1510.
    float                     yasp;                    // 65 type, name 7, 1511.
    float                     frs_sec_base;            // 66 type, name 7, 1512.
    float                     gauss;                   // 67 type, name 7, 1513.
    int                       color_mgt_flag;          // 68 type, name 4, 1514.
    float                     postgamma;               // 69 type, name 7, 1515.
    float                     posthue;                 // 70 type, name 7, 1516.
    float                     postsat;                 // 71 type, name 7, 1517.
    float                     dither_intensity;        // 72 type, name 7, 1518.
    short                     bake_osa;                // 73 type, name 2, 1519.
    short                     bake_filter;             // 74 type, name 2, 1520.
    short                     bake_mode;               // 75 type, name 2, 1521.
    short                     bake_flag;               // 76 type, name 2, 1522.
    short                     bake_normal_space;       // 77 type, name 2, 1523.
    short                     bake_quad_split;         // 78 type, name 2, 1524.
    float                     bake_maxdist;            // 79 type, name 7, 1525.
    float                     bake_biasdist;           // 80 type, name 7, 1526.
    short                     bake_samples;            // 81 type, name 2, 1527.
    short                     bake_pad;                // 82 type, name 2, 1528.
    float                     bake_user_scale;         // 83 type, name 7, 1529.
    float                     bake_pad1;               // 84 type, name 7, 1530.
    char                      pic[1024];               // 85 type, name 0, 1531.
    int                       stamp;                   // 86 type, name 4, 1532.
    short                     stamp_font_id;           // 87 type, name 2, 1533.
    short                     pad3;                    // 88 type, name 2, 517.
    char                      stamp_udata[768];        // 89 type, name 0, 1534.
    float                     fg_stamp[4];             // 90 type, name 7, 1535.
    float                     bg_stamp[4];             // 91 type, name 7, 1536.
    char                      seq_prev_type;           // 92 type, name 0, 1537.
    char                      seq_rend_type;           // 93 type, name 0, 1538.
    char                      seq_flag;                // 94 type, name 0, 1539.
    char                      pad5[5];                 // 95 type, name 0, 1540.
    int                       simplify_flag;           // 96 type, name 4, 1541.
    short                     simplify_subsurf;        // 97 type, name 2, 1542.
    short                     simplify_shadowsamples;  // 98 type, name 2, 1543.
    float                     simplify_particles;      // 99 type, name 7, 1544.
    float                     simplify_aosss;          // 100 type, name 7, 1545.
    short                     cineonwhite;             // 101 type, name 2, 1546.
    short                     cineonblack;             // 102 type, name 2, 1547.
    float                     cineongamma;             // 103 type, name 7, 1548.
    short                     jp2_preset;              // 104 type, name 2, 1549.
    short                     jp2_depth;               // 105 type, name 2, 1550.
    int                       rpad3;                   // 106 type, name 4, 1551.
    short                     domeres;                 // 107 type, name 2, 1552.
    short                     domemode;                // 108 type, name 2, 1553.
    short                     domeangle;               // 109 type, name 2, 1554.
    short                     dometilt;                // 110 type, name 2, 1555.
    float                     domeresbuf;              // 111 type, name 7, 1556.
    float                     pad2;                    // 112 type, name 7, 29.
    Text                      *dometext;               // 113 type, name 38, 1557.
    int                       line_thickness_mode;     // 114 type, name 4, 1558.
    float                     unit_line_thickness;     // 115 type, name 7, 1559.
    char                      engine[32];              // 116 type, name 0, 1560.
    BakeData                  bake;                    // 117 type, name 228, 1561.
    int                       preview_start_resolution;// 118 type, name 4, 1562.
    int                       pad;                     // 119 type, name 4, 66.
};

// zu DNA structure 569, 257
struct MovieClip
{
    ID                                id;                 // 1 type, name 22, 31.
    AnimData                          *adt;               // 2 type, name 36, 75.
    char                              name[1024];         // 3 type, name 0, 34.
    int                               source;             // 4 type, name 4, 149.
    int                               lastframe;          // 5 type, name 4, 150.
    int                               lastsize[2];        // 6 type, name 4, 3737.
    float                             aspx;               // 7 type, name 7, 169.
    float                             aspy;               // 8 type, name 7, 170.
    void                              *anim;              // Note: using void* on undefined DNA type: anim
    void                              *cache;             // Note: using void* on undefined DNA type: MovieClipCache
    bGPdata                           *gpd;               // 11 type, name 195, 1031.
    MovieTracking                     tracking;           // 12 type, name 602, 3738.
    void                              *tracking_context;  // 13 type, name 11, 3739.
    MovieClipProxy                    proxy;              // 14 type, name 600, 3740.
    int                               flag;               // 15 type, name 4, 18.
    int                               len;                // 16 type, name 4, 22.
    int                               start_frame;        // 17 type, name 4, 2809.
    int                               frame_offset;       // 18 type, name 4, 3741.
    ColorManagedColorspaceSettings    colorspace_settings;// 19 type, name 50, 171.
};

// zu DNA structure 570, 335
struct ARegion
{
    ARegion        *next;                 // 1 type, name 335, 0.
    ARegion        *prev;                 // 2 type, name 335, 1.
    View2D         v2d;                   // 3 type, name 269, 1896.
    rcti           winrct;                // 4 type, name 18, 2498.
    rcti           drawrct;               // 5 type, name 18, 2499.
    short          winx;                  // 6 type, name 2, 1888.
    short          winy;                  // 7 type, name 2, 1889.
    short          swinid;                // 8 type, name 2, 2500.
    short          regiontype;            // 9 type, name 2, 2501.
    short          alignment;             // 10 type, name 2, 2502.
    short          flag;                  // 11 type, name 2, 18.
    float          fsize;                 // 12 type, name 7, 630.
    short          sizex;                 // 13 type, name 2, 2467.
    short          sizey;                 // 14 type, name 2, 2468.
    short          do_draw;               // 15 type, name 2, 2446.
    short          do_draw_overlay;       // 16 type, name 2, 2503.
    short          swap;                  // 17 type, name 2, 2451.
    short          overlap;               // 18 type, name 2, 2504.
    short          flagfullscreen;        // 19 type, name 2, 2505.
    short          pad;                   // 20 type, name 2, 66.
    void           *type;                 // Note: using void* on undefined DNA type: ARegionType
    ListBase       uiblocks;              // 22 type, name 14, 2506.
    ListBase       panels;                // 23 type, name 14, 2507.
    ListBase       panels_category_active;// 24 type, name 14, 2508.
    ListBase       ui_lists;              // 25 type, name 14, 2509.
    ListBase       ui_previews;           // 26 type, name 14, 2510.
    ListBase       handlers;              // 27 type, name 14, 2496.
    ListBase       panels_category;       // 28 type, name 14, 2511.
    void           *regiontimer;          // Note: using void* on undefined DNA type: wmTimer
    char           *headerstr;            // 30 type, name 0, 2513.
    void           *regiondata;           // 31 type, name 11, 2514.
};

// zu DNA structure 571, 29
struct Object
{
    ID                  id;                    // 1 type, name 22, 31.
    AnimData            *adt;                  // 2 type, name 36, 75.
    void                *sculpt;               // Note: using void* on undefined DNA type: SculptSession
    short               type;                  // 4 type, name 2, 16.
    short               partype;               // 5 type, name 2, 1019.
    int                 par1;                  // 6 type, name 4, 1020.
    int                 par2;                  // 7 type, name 4, 1021.
    int                 par3;                  // 8 type, name 4, 1022.
    char                parsubstr[64];         // 9 type, name 0, 1023.
    Object              *parent;               // 10 type, name 29, 36.
    Object              *track;                // 11 type, name 29, 1024.
    Object              *proxy;                // 12 type, name 29, 1025.
    Object              *proxy_group;          // 13 type, name 29, 1026.
    Object              *proxy_from;           // 14 type, name 29, 1027.
    Ipo                 *ipo;                  // 15 type, name 33, 80.
    BoundBox            *bb;                   // 16 type, name 73, 550.
    bAction             *action;               // 17 type, name 193, 1028.
    bAction             *poselib;              // 18 type, name 193, 1029.
    bPose               *pose;                 // 19 type, name 194, 1030.
    void                *data;                 // 20 type, name 11, 2.
    bGPdata             *gpd;                  // 21 type, name 195, 1031.
    bAnimVizSettings    avs;                   // 22 type, name 196, 1032.
    bMotionPath         *mpath;                // 23 type, name 197, 1033.
    ListBase            constraintChannels;    // 24 type, name 14, 1034.
    ListBase            effect;                // 25 type, name 14, 1035.
    ListBase            defbase;               // 26 type, name 14, 1036.
    ListBase            modifiers;             // 27 type, name 14, 1037.
    int                 mode;                  // 28 type, name 4, 351.
    int                 restore_mode;          // 29 type, name 4, 1038.
    Material            **mat;                 // 30 type, name 68, 565.
    char                *matbits;              // 31 type, name 0, 1039.
    int                 totcol;                // 32 type, name 4, 567.
    int                 actcol;                // 33 type, name 4, 1040.
    float               loc[3];                // 34 type, name 7, 341.
    float               dloc[3];               // 35 type, name 7, 1041.
    float               orig[3];               // 36 type, name 7, 1042.
    float               size[3];               // 37 type, name 7, 189.
    float               dsize[3];              // 38 type, name 7, 1043.
    float               dscale[3];             // 39 type, name 7, 1044.
    float               rot[3];                // 40 type, name 7, 342.
    float               drot[3];               // 41 type, name 7, 1045.
    float               quat[4];               // 42 type, name 7, 553.
    float               dquat[4];              // 43 type, name 7, 1046.
    float               rotAxis[3];            // 44 type, name 7, 1047.
    float               drotAxis[3];           // 45 type, name 7, 1048.
    float               rotAngle;              // 46 type, name 7, 1049.
    float               drotAngle;             // 47 type, name 7, 1050.
    float               obmat[4][4];           // 48 type, name 7, 1051.
    float               parentinv[4][4];       // 49 type, name 7, 815.
    float               constinv[4][4];        // 50 type, name 7, 1052.
    float               imat[4][4];            // 51 type, name 7, 250.
    float               imat_ren[4][4];        // 52 type, name 7, 1053.
    int                 lay;                   // 53 type, name 4, 1054.
    short               flag;                  // 54 type, name 2, 18.
    short               colbits;               // 55 type, name 2, 1055.
    short               transflag;             // 56 type, name 2, 1056.
    short               protectflag;           // 57 type, name 2, 1057.
    short               trackflag;             // 58 type, name 2, 1058.
    short               upflag;                // 59 type, name 2, 1059.
    short               nlaflag;               // 60 type, name 2, 1060.
    short               ipoflag;               // 61 type, name 2, 1061.
    short               scaflag;               // 62 type, name 2, 1062.
    char                scavisflag;            // 63 type, name 0, 1063.
    char                depsflag;              // 64 type, name 0, 1064.
    int                 dupon;                 // 65 type, name 4, 1065.
    int                 dupoff;                // 66 type, name 4, 1066.
    int                 dupsta;                // 67 type, name 4, 1067.
    int                 dupend;                // 68 type, name 4, 1068.
    int                 pad;                   // 69 type, name 4, 66.
    float               mass;                  // 70 type, name 7, 1069.
    float               damping;               // 71 type, name 7, 1070.
    float               inertia;               // 72 type, name 7, 1071.
    float               formfactor;            // 73 type, name 7, 1072.
    float               rdamping;              // 74 type, name 7, 1073.
    float               margin;                // 75 type, name 7, 1074.
    float               max_vel;               // 76 type, name 7, 1075.
    float               min_vel;               // 77 type, name 7, 1076.
    float               obstacleRad;           // 78 type, name 7, 1077.
    float               step_height;           // 79 type, name 7, 1078.
    float               jump_speed;            // 80 type, name 7, 1079.
    float               fall_speed;            // 81 type, name 7, 1080.
    short               col_group;             // 82 type, name 2, 1081.
    short               col_mask;              // 83 type, name 2, 1082.
    short               rotmode;               // 84 type, name 2, 1083.
    char                boundtype;             // 85 type, name 0, 1084.
    char                collision_boundtype;   // 86 type, name 0, 1085.
    short               dtx;                   // 87 type, name 2, 116.
    char                dt;                    // 88 type, name 0, 1086.
    char                empty_drawtype;        // 89 type, name 0, 1087.
    float               empty_drawsize;        // 90 type, name 7, 1088.
    float               dupfacesca;            // 91 type, name 7, 1089.
    ListBase            prop;                  // 92 type, name 14, 1090.
    ListBase            sensors;               // 93 type, name 14, 1091.
    ListBase            controllers;           // 94 type, name 14, 1092.
    ListBase            actuators;             // 95 type, name 14, 1093.
    float               sf;                    // 96 type, name 7, 1094.
    short               index;                 // 97 type, name 2, 430.
    short               actdef;                // 98 type, name 2, 1095.
    float               col[4];                // 99 type, name 7, 692.
    int                 gameflag;              // 100 type, name 4, 1096.
    int                 gameflag2;             // 101 type, name 4, 1097.
    BulletSoftBody      *bsoft;                // 102 type, name 198, 1098.
    char                restrictflag;          // 103 type, name 0, 1099.
    char                recalc;                // 104 type, name 0, 257.
    short               softflag;              // 105 type, name 2, 1100.
    float               anisotropicFriction[3];// 106 type, name 7, 1101.
    ListBase            constraints;           // 107 type, name 14, 1102.
    ListBase            nlastrips;             // 108 type, name 14, 1103.
    ListBase            hooks;                 // 109 type, name 14, 1104.
    ListBase            particlesystem;        // 110 type, name 14, 1105.
    PartDeflect         *pd;                   // 111 type, name 199, 337.
    SoftBody            *soft;                 // 112 type, name 200, 1106.
    Group               *dup_group;            // 113 type, name 69, 1107.
    char                body_type;             // 114 type, name 0, 1108.
    char                shapeflag;             // 115 type, name 0, 1109.
    short               shapenr;               // 116 type, name 2, 606.
    float               smoothresh;            // 117 type, name 7, 683.
    FluidsimSettings    *fluidsimSettings;     // 118 type, name 162, 1110.
    void                *curve_cache;          // Note: using void* on undefined DNA type: CurveCache
    void                *derivedDeform;        // Note: using void* on undefined DNA type: DerivedMesh
    void                *derivedFinal;         // Note: using void* on undefined DNA type: DerivedMesh
    uint64_t            lastDataMask;          // 122 type, name 10, 1114.
    uint64_t            customdata_mask;       // 123 type, name 10, 1115.
    int                 state;                 // 124 type, name 4, 1116.
    int                 init_state;            // 125 type, name 4, 1117.
    ListBase            gpulamp;               // 126 type, name 14, 1118.
    ListBase            pc_ids;                // 127 type, name 14, 1119.
    ListBase            *duplilist;            // 128 type, name 14, 1120.
    RigidBodyOb         *rigidbody_object;     // 129 type, name 202, 1121.
    RigidBodyCon        *rigidbody_constraint; // 130 type, name 203, 1122.
    float               ima_ofs[2];            // 131 type, name 7, 1123.
    ImageUser           *iuser;                // 132 type, name 43, 1124.
    ListBase            lodlevels;             // 133 type, name 14, 1125.
    LodLevel            *currentlod;           // 134 type, name 191, 1126.
};

// zu DNA structure 572, 313
struct ThemeUI
{
    uiWidgetColors         wcol_regular;     // 1 type, name 309, 2092.
    uiWidgetColors         wcol_tool;        // 2 type, name 309, 2093.
    uiWidgetColors         wcol_text;        // 3 type, name 309, 2094.
    uiWidgetColors         wcol_radio;       // 4 type, name 309, 2095.
    uiWidgetColors         wcol_option;      // 5 type, name 309, 2096.
    uiWidgetColors         wcol_toggle;      // 6 type, name 309, 2097.
    uiWidgetColors         wcol_num;         // 7 type, name 309, 2098.
    uiWidgetColors         wcol_numslider;   // 8 type, name 309, 2099.
    uiWidgetColors         wcol_menu;        // 9 type, name 309, 2100.
    uiWidgetColors         wcol_pulldown;    // 10 type, name 309, 2101.
    uiWidgetColors         wcol_menu_back;   // 11 type, name 309, 2102.
    uiWidgetColors         wcol_menu_item;   // 12 type, name 309, 2103.
    uiWidgetColors         wcol_tooltip;     // 13 type, name 309, 2104.
    uiWidgetColors         wcol_box;         // 14 type, name 309, 2105.
    uiWidgetColors         wcol_scroll;      // 15 type, name 309, 2106.
    uiWidgetColors         wcol_progress;    // 16 type, name 309, 2107.
    uiWidgetColors         wcol_list_item;   // 17 type, name 309, 2108.
    uiWidgetColors         wcol_pie_menu;    // 18 type, name 309, 2109.
    uiWidgetStateColors    wcol_state;       // 19 type, name 310, 2110.
    uiPanelColors          panel;            // 20 type, name 311, 2111.
    char                   widget_emboss[4]; // 21 type, name 0, 2112.
    float                  menu_shadow_fac;  // 22 type, name 7, 2113.
    short                  menu_shadow_width;// 23 type, name 2, 2114.
    short                  pad[3];           // 24 type, name 2, 350.
    char                   iconfile[256];    // 25 type, name 0, 2115.
    float                  icon_alpha;       // 26 type, name 7, 2116.
    char                   xaxis[4];         // 27 type, name 0, 2117.
    char                   yaxis[4];         // 28 type, name 0, 2118.
    char                   zaxis[4];         // 29 type, name 0, 2119.
};

// zu DNA structure 573, 44
struct Scene
{
    ID                                id;                           // 1 type, name 22, 31.
    AnimData                          *adt;                         // 2 type, name 36, 75.
    Object                            *camera;                      // 3 type, name 29, 1603.
    World                             *world;                       // 4 type, name 216, 1762.
    Scene                             *set;                         // 5 type, name 44, 1763.
    ListBase                          base;                         // 6 type, name 14, 1764.
    Base                              *basact;                      // 7 type, name 217, 1765.
    Object                            *obedit;                      // 8 type, name 29, 1766.
    float                             cursor[3];                    // 9 type, name 7, 1767.
    float                             twcent[3];                    // 10 type, name 7, 1768.
    float                             twmin[3];                     // 11 type, name 7, 1769.
    float                             twmax[3];                     // 12 type, name 7, 1770.
    int                               lay;                          // 13 type, name 4, 1054.
    int                               layact;                       // 14 type, name 4, 1771.
    int                               lay_updated;                  // 15 type, name 4, 1772.
    short                             flag;                         // 16 type, name 2, 18.
    char                              use_nodes;                    // 17 type, name 0, 340.
    char                              pad[1];                       // 18 type, name 0, 246.
    bNodeTree                         *nodetree;                    // 19 type, name 61, 335.
    Editing                           *ed;                          // 20 type, name 254, 1773.
    ToolSettings                      *toolsettings;                // 21 type, name 249, 1774.
    void                              *stats;                       // Note: using void* on undefined DNA type: SceneStats
    DisplaySafeAreas                  safe_areas;                   // 23 type, name 253, 1776.
    RenderData                        r;                            // 24 type, name 229, 198.
    AudioData                         audio;                        // 25 type, name 222, 1777.
    ListBase                          markers;                      // 26 type, name 14, 1778.
    ListBase                          transform_spaces;             // 27 type, name 14, 1779.
    void                              *sound_scene;                 // 28 type, name 11, 1780.
    void                              *sound_scene_handle;          // 29 type, name 11, 1781.
    void                              *sound_scrub_handle;          // 30 type, name 11, 1782.
    void                              *speaker_handles;             // 31 type, name 11, 1783.
    void                              *fps_info;                    // 32 type, name 11, 1784.
    void                              *theDag;                      // Note: using void* on undefined DNA type: DagForest
    short                             dagflags;                     // 34 type, name 2, 1786.
    short                             recalc;                       // 35 type, name 2, 257.
    int                               active_keyingset;             // 36 type, name 4, 1787.
    ListBase                          keyingsets;                   // 37 type, name 14, 1788.
    GameFraming                       framing;                      // 38 type, name 232, 1585.
    GameData                          gm;                           // 39 type, name 234, 1789.
    UnitSettings                      unit;                         // 40 type, name 251, 1790.
    bGPdata                           *gpd;                         // 41 type, name 195, 1031.
    PhysicsSettings                   physics_settings;             // 42 type, name 252, 1791.
    MovieClip                         *clip;                        // 43 type, name 257, 1792.
    uint64_t                          customdata_mask;              // 44 type, name 10, 1115.
    uint64_t                          customdata_mask_modal;        // 45 type, name 10, 1793.
    ColorManagedViewSettings          view_settings;                // 46 type, name 226, 1453.
    ColorManagedDisplaySettings       display_settings;             // 47 type, name 227, 1454.
    ColorManagedColorspaceSettings    sequencer_colorspace_settings;// 48 type, name 50, 1794.
    RigidBodyWorld                    *rigidbody_world;             // 49 type, name 258, 1795.
};

// zu DNA structure 574, 316
struct bTheme
{
    bTheme            *next;            // 1 type, name 316, 0.
    bTheme            *prev;            // 2 type, name 316, 1.
    char              name[32];         // 3 type, name 0, 1563.
    ThemeUI           tui;              // 4 type, name 313, 2301.
    ThemeSpace        tbuts;            // 5 type, name 314, 2302.
    ThemeSpace        tv3d;             // 6 type, name 314, 2303.
    ThemeSpace        tfile;            // 7 type, name 314, 2304.
    ThemeSpace        tipo;             // 8 type, name 314, 2305.
    ThemeSpace        tinfo;            // 9 type, name 314, 2306.
    ThemeSpace        tact;             // 10 type, name 314, 2307.
    ThemeSpace        tnla;             // 11 type, name 314, 2308.
    ThemeSpace        tseq;             // 12 type, name 314, 2309.
    ThemeSpace        tima;             // 13 type, name 314, 2310.
    ThemeSpace        text;             // 14 type, name 314, 2311.
    ThemeSpace        toops;            // 15 type, name 314, 2312.
    ThemeSpace        ttime;            // 16 type, name 314, 2313.
    ThemeSpace        tnode;            // 17 type, name 314, 2314.
    ThemeSpace        tlogic;           // 18 type, name 314, 2315.
    ThemeSpace        tuserpref;        // 19 type, name 314, 2316.
    ThemeSpace        tconsole;         // 20 type, name 314, 2317.
    ThemeSpace        tclip;            // 21 type, name 314, 2318.
    ThemeWireColor    tarm[20];         // 22 type, name 315, 2319.
    int               active_theme_area;// 23 type, name 4, 2320.
    int               pad;              // 24 type, name 4, 66.
};

/** @}*/
}
#endif//_Blender_h_
