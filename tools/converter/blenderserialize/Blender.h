#ifndef _Blender_h_
#define _Blender_h_
// Generated from a Blender(273) file.

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



struct Link
{
    Link *next;
    Link *prev;
};

struct LinkData
{
    LinkData *next;
    LinkData *prev;
    void *data;
};

struct ListBase
{
    void *first;
    void *last;
};

struct vec2s
{
    short x;
    short y;
};

struct vec2f
{
    float x;
    float y;
};

struct vec3f
{
    float x;
    float y;
    float z;
};

struct rcti
{
    int xmin;
    int xmax;
    int ymin;
    int ymax;
};

struct rctf
{
    float xmin;
    float xmax;
    float ymin;
    float ymax;
};

struct ID
{
    void *next;
    void *prev;
    ID *newid;
    Library *lib;
    char name[66];
    short flag;
    int us;
    int icon_id;
    int pad2;
    IDProperty *properties;
};

struct PreviewImage
{
    int w[2];
    int h[2];
    short changed[2];
    short changed_timestamp[2];
    int *rect[2];
    void *gputexture[2];
};

struct IpoDriver
{
    Object *ob;
    short blocktype;
    short adrcode;
    short type;
    short flag;
    char name[128];
};

struct KeyBlock
{
    KeyBlock *next;
    KeyBlock *prev;
    float pos;
    float curval;
    short type;
    short pad1;
    short relative;
    short flag;
    int totelem;
    int uid;
    void *data;
    char name[64];
    char vgroup[64];
    float slidermin;
    float slidermax;
};

struct TextLine
{
    TextLine *next;
    TextLine *prev;
    char *line;
    char *format;
    int len;
    int blen;
};

struct PackedFile
{
    int size;
    int seek;
    void *data;
};

struct ImageUser
{
    Scene *scene;
    int framenr;
    int frames;
    int offset;
    int sfra;
    char fie_ima;
    char cycl;
    char ok;
    char pad;
    short multi_index;
    short layer;
    short pass;
    short flag;
    int pad2;
};

struct RenderSlot
{
    char name[64];
};

struct MTex
{
    short texco;
    short mapto;
    short maptoneg;
    short blendtype;
    Object *object;
    Tex *tex;
    char uvname[64];
    char projx;
    char projy;
    char projz;
    char mapping;
    float ofs[3];
    float size[3];
    float rot;
    short texflag;
    short colormodel;
    short pmapto;
    short pmaptoneg;
    short normapspace;
    short which_output;
    char brush_map_mode;
    char pad[7];
    float r;
    float g;
    float b;
    float k;
    float def_var;
    float rt;
    float colfac;
    float varfac;
    float norfac;
    float dispfac;
    float warpfac;
    float colspecfac;
    float mirrfac;
    float alphafac;
    float difffac;
    float specfac;
    float emitfac;
    float hardfac;
    float raymirrfac;
    float translfac;
    float ambfac;
    float colemitfac;
    float colreflfac;
    float coltransfac;
    float densfac;
    float scatterfac;
    float reflfac;
    float timefac;
    float lengthfac;
    float clumpfac;
    float dampfac;
    float kinkfac;
    float roughfac;
    float padensfac;
    float gravityfac;
    float lifefac;
    float sizefac;
    float ivelfac;
    float fieldfac;
    float shadowfac;
    float zenupfac;
    float zendownfac;
    float blendfac;
};

struct CBData
{
    float r;
    float g;
    float b;
    float a;
    float pos;
    int cur;
};

struct EnvMap
{
    Object *object;
    Image *ima;
    void *cube[6];
    float imat[4][4];
    float obimat[3][3];
    short type;
    short stype;
    float clipsta;
    float clipend;
    float viewscale;
    int notlay;
    short cuberes;
    short depth;
    int ok;
    int lastframe;
    short recalc;
    short lastsize;
};

struct PointDensity
{
    short flag;
    short falloff_type;
    float falloff_softness;
    float radius;
    short source;
    short color_source;
    int totpoints;
    int pdpad;
    Object *object;
    int psys;
    short psys_cache_space;
    short ob_cache_space;
    void *point_tree;
    float *point_data;
    float noise_size;
    short noise_depth;
    short noise_influence;
    short noise_basis;
    short pdpad3[3];
    float noise_fac;
    float speed_scale;
    float falloff_speed_scale;
    float pdpad2;
    ColorBand *coba;
    CurveMapping *falloff_curve;
};

struct VoxelData
{
    int resol[3];
    int interp_type;
    short file_format;
    short flag;
    short extend;
    short smoked_type;
    short data_type;
    short pad;
    int _pad;
    Object *object;
    float int_multiplier;
    int still_frame;
    char source_path[1024];
    float *dataset;
    int cachedframe;
    int ok;
};

struct OceanTex
{
    Object *object;
    char oceanmod[64];
    int output;
    int pad;
};

struct TexMapping
{
    float loc[3];
    float rot[3];
    float size[3];
    int flag;
    char projx;
    char projy;
    char projz;
    char mapping;
    int type;
    float mat[4][4];
    float min[3];
    float max[3];
    Object *ob;
};

struct VolumeSettings
{
    float density;
    float emission;
    float scattering;
    float reflection;
    float emission_col[3];
    float transmission_col[3];
    float reflection_col[3];
    float density_scale;
    float depth_cutoff;
    float asymmetry;
    short stepsize_type;
    short shadeflag;
    short shade_type;
    short precache_resolution;
    float stepsize;
    float ms_diff;
    float ms_intensity;
    float ms_spread;
};

struct GameSettings
{
    int flag;
    int alpha_blend;
    int face_orientation;
    int pad1;
};

struct TexPaintSlot
{
    Image *ima;
    char *uvname;
    int index;
    int pad;
};

struct MetaElem
{
    MetaElem *next;
    MetaElem *prev;
    BoundBox *bb;
    short type;
    short flag;
    short selcol1;
    short selcol2;
    float x;
    float y;
    float z;
    float quat[4];
    float expx;
    float expy;
    float expz;
    float rad;
    float rad2;
    float s;
    float len;
    float *mat;
    float *imat;
};

struct BezTriple
{
    float vec[3][3];
    float alfa;
    float weight;
    float radius;
    char ipo;
    char h1;
    char h2;
    char f1;
    char f2;
    char f3;
    char hide;
    char easing;
    float back;
    float amplitude;
    float period;
    char pad[4];
};

struct BPoint
{
    float vec[4];
    float alfa;
    float weight;
    short f1;
    short hide;
    float radius;
    float pad;
};

struct Nurb
{
    Nurb *next;
    Nurb *prev;
    short type;
    short mat_nr;
    short hide;
    short flag;
    int pntsu;
    int pntsv;
    short pad[2];
    short resolu;
    short resolv;
    short orderu;
    short orderv;
    short flagu;
    short flagv;
    float *knotsu;
    float *knotsv;
    BPoint *bp;
    BezTriple *bezt;
    short tilt_interp;
    short radius_interp;
    int charidx;
};

struct CharInfo
{
    short kern;
    short mat_nr;
    char flag;
    char pad;
    short pad2;
};

struct TextBox
{
    float x;
    float y;
    float w;
    float h;
};

struct TFace
{
    void *tpage;
    float uv[4][2];
    int col[4];
    char flag;
    char transp;
    short mode;
    short tile;
    short unwrap;
};

struct MFace
{
    int v1;
    int v2;
    int v3;
    int v4;
    short mat_nr;
    char edcode;
    char flag;
};

struct MEdge
{
    int v1;
    int v2;
    char crease;
    char bweight;
    short flag;
};

struct MDeformWeight
{
    int def_nr;
    float weight;
};

struct MDeformVert
{
    MDeformWeight *dw;
    int totweight;
    int flag;
};

struct MVert
{
    float co[3];
    short no[3];
    char flag;
    char bweight;
};

struct MCol
{
    char a;
    char r;
    char g;
    char b;
};

struct MPoly
{
    int loopstart;
    int totloop;
    short mat_nr;
    char flag;
    char pad;
};

struct MLoop
{
    int v;
    int e;
};

struct MTexPoly
{
    Image *tpage;
    char flag;
    char transp;
    short mode;
    short tile;
    short pad;
};

struct MLoopUV
{
    float uv[2];
    int flag;
};

struct MLoopCol
{
    char r;
    char g;
    char b;
    char a;
};

struct MSelect
{
    int index;
    int type;
};

struct MTFace
{
    float uv[4][2];
    Image *tpage;
    char flag;
    char transp;
    short mode;
    short tile;
    short unwrap;
};

struct MFloatProperty
{
    float f;
};

struct MIntProperty
{
    int i;
};

struct MStringProperty
{
    char s[255];
    char s_len;
};

struct OrigSpaceFace
{
    float uv[4][2];
};

struct OrigSpaceLoop
{
    float uv[2];
};

struct MDisps
{
    int totdisp;
    int level;
    float (*disps)();
    int *hidden;
};

struct MultiresCol
{
    float a;
    float r;
    float g;
    float b;
};

struct MultiresFace
{
    int v[4];
    int mid;
    char flag;
    char mat_nr;
    char pad[2];
};

struct MultiresEdge
{
    int v[2];
    int mid;
};

struct MultiresLevel
{
    MultiresLevel *next;
    MultiresLevel *prev;
    MultiresFace *faces;
    MultiresColFace *colfaces;
    MultiresEdge *edges;
    int totvert;
    int totface;
    int totedge;
    int pad;
    MVert *verts;
};

struct MRecast
{
    int i;
};

struct GridPaintMask
{
    float *data;
    int level;
    int pad;
};

struct MVertSkin
{
    float radius[3];
    int flag;
};

struct FreestyleEdge
{
    char flag;
    char pad[3];
};

struct FreestyleFace
{
    char flag;
    char pad[3];
};

struct ModifierData
{
    ModifierData *next;
    ModifierData *prev;
    int type;
    int mode;
    int stackindex;
    int pad;
    char name[64];
    Scene *scene;
    char *error;
};

struct MDefInfluence
{
    int vertex;
    float weight;
};

struct MDefCell
{
    int offset;
    int totinfluence;
};

struct EditLatt
{
    Lattice *latt;
    int shapenr;
    char pad[4];
};

struct bDeformGroup
{
    bDeformGroup *next;
    bDeformGroup *prev;
    char name[64];
    char flag;
    char pad[7];
};

struct BoundBox
{
    float vec[8][3];
    int flag;
    int pad;
};

struct LodLevel
{
    LodLevel *next;
    LodLevel *prev;
    Object *source;
    int flags;
    float distance;
};

struct ObHook
{
    ObHook *next;
    ObHook *prev;
    Object *parent;
    float parentinv[4][4];
    float mat[4][4];
    float cent[3];
    float falloff;
    char name[64];
    int *indexar;
    int totindex;
    int curindex;
    short type;
    short active;
    float force;
};

struct DupliObject
{
    DupliObject *next;
    DupliObject *prev;
    Object *ob;
    float mat[4][4];
    float orco[3];
    float uv[2];
    short type;
    char no_draw;
    char animated;
    int persistent_id[8];
    ParticleSystem *particle_system;
};

struct PartDeflect
{
    int flag;
    short deflect;
    short forcefield;
    short falloff;
    short shape;
    short tex_mode;
    short kink;
    short kink_axis;
    short zdir;
    float f_strength;
    float f_damp;
    float f_flow;
    float f_size;
    float f_power;
    float maxdist;
    float mindist;
    float f_power_r;
    float maxrad;
    float minrad;
    float pdef_damp;
    float pdef_rdamp;
    float pdef_perm;
    float pdef_frict;
    float pdef_rfrict;
    float pdef_stickness;
    float absorption;
    float pdef_sbdamp;
    float pdef_sbift;
    float pdef_sboft;
    float clump_fac;
    float clump_pow;
    float kink_freq;
    float kink_shape;
    float kink_amp;
    float free_end;
    float tex_nabla;
    Tex *tex;
    void *rng;
    float f_noise;
    int seed;
    Object *f_source;
};

struct EffectorWeights
{
    Group *group;
    float weight[14];
    float global_gravity;
    short flag;
    short rt[3];
    int pad;
};

struct PTCacheExtra
{
    PTCacheExtra *next;
    PTCacheExtra *prev;
    int type;
    int totdata;
    void *data;
};

struct SBVertex
{
    float vec[4];
};

struct BulletSoftBody
{
    int flag;
    float linStiff;
    float angStiff;
    float volume;
    int viterations;
    int piterations;
    int diterations;
    int citerations;
    float kSRHR_CL;
    float kSKHR_CL;
    float kSSHR_CL;
    float kSR_SPLT_CL;
    float kSK_SPLT_CL;
    float kSS_SPLT_CL;
    float kVCF;
    float kDP;
    float kDG;
    float kLF;
    float kPR;
    float kVC;
    float kDF;
    float kMT;
    float kCHR;
    float kKHR;
    float kSHR;
    float kAHR;
    int collisionflags;
    int numclusteriterations;
    float welding;
    float margin;
};

struct FluidVertexVelocity
{
    float vel[3];
};

struct FluidsimSettings
{
    FluidsimModifierData *fmd;
    int threads;
    int pad1;
    short type;
    short show_advancedoptions;
    short resolutionxyz;
    short previewresxyz;
    float realsize;
    short guiDisplayMode;
    short renderDisplayMode;
    float viscosityValue;
    short viscosityMode;
    short viscosityExponent;
    float grav[3];
    float animStart;
    float animEnd;
    int bakeStart;
    int bakeEnd;
    int frameOffset;
    int pad2;
    float gstar;
    int maxRefine;
    float iniVelx;
    float iniVely;
    float iniVelz;
    Mesh *orgMesh;
    Mesh *meshBB;
    char surfdataPath[1024];
    float bbStart[3];
    float bbSize[3];
    Ipo *ipo;
    short typeFlags;
    char domainNovecgen;
    char volumeInitType;
    float partSlipValue;
    int generateTracers;
    float generateParticles;
    float surfaceSmoothing;
    int surfaceSubdivs;
    int flag;
    float particleInfSize;
    float particleInfAlpha;
    float farFieldSize;
    FluidVertexVelocity *meshVelocities;
    int totvert;
    float cpsTimeStart;
    float cpsTimeEnd;
    float cpsQuality;
    float attractforceStrength;
    float attractforceRadius;
    float velocityforceStrength;
    float velocityforceRadius;
    int lastgoodframe;
    float animRate;
};

struct Base
{
    Base *next;
    Base *prev;
    int lay;
    int selcol;
    int flag;
    short sx;
    short sy;
    Object *object;
};

struct AviCodecData
{
    void *lpFormat;
    void *lpParms;
    int cbFormat;
    int cbParms;
    int fccType;
    int fccHandler;
    int dwKeyFrameEvery;
    int dwQuality;
    int dwBytesPerSecond;
    int dwFlags;
    int dwInterleaveEvery;
    int pad;
    char avicodecname[128];
};

struct QuicktimeCodecData
{
    void *cdParms;
    void *pad;
    int cdSize;
    int pad2;
    char qtcodecname[128];
};

struct QuicktimeCodecSettings
{
    int codecType;
    int codecSpatialQuality;
    int codec;
    int codecFlags;
    int colorDepth;
    int codecTemporalQuality;
    int minSpatialQuality;
    int minTemporalQuality;
    int keyFrameRate;
    int bitRate;
    int audiocodecType;
    int audioSampleRate;
    short audioBitDepth;
    short audioChannels;
    int audioCodecFlags;
    int audioBitRate;
    int pad1;
};

struct FFMpegCodecData
{
    int type;
    int codec;
    int audio_codec;
    int video_bitrate;
    int audio_bitrate;
    int audio_mixrate;
    int audio_channels;
    int audio_pad;
    float audio_volume;
    int gop_size;
    int flags;
    int rc_min_rate;
    int rc_max_rate;
    int rc_buffer_size;
    int mux_packet_size;
    int mux_rate;
    IDProperty *properties;
};

struct AudioData
{
    int mixrate;
    float main;
    float speed_of_sound;
    float doppler_factor;
    int distance_model;
    short flag;
    short pad;
    float volume;
    float pad2;
};

struct RenderProfile
{
    RenderProfile *next;
    RenderProfile *prev;
    char name[32];
    short particle_perc;
    short subsurf_max;
    short shadbufsample_max;
    short pad1;
    float ao_error;
    float pad2;
};

struct GameDome
{
    short res;
    short mode;
    short angle;
    short tilt;
    float resbuf;
    float pad2;
    Text *warptext;
};

struct GameFraming
{
    float col[3];
    char type;
    char pad1;
    char pad2;
    char pad3;
};

struct RecastData
{
    float cellsize;
    float cellheight;
    float agentmaxslope;
    float agentmaxclimb;
    float agentheight;
    float agentradius;
    float edgemaxlen;
    float edgemaxerror;
    float regionminsize;
    float regionmergesize;
    int vertsperpoly;
    float detailsampledist;
    float detailsamplemaxerror;
    short pad1;
    short pad2;
};

struct TimeMarker
{
    TimeMarker *next;
    TimeMarker *prev;
    int frame;
    char name[64];
    int flag;
    Object *camera;
};

struct Paint
{
    Brush *brush;
    Palette *palette;
    void *paint_cursor;
    char paint_cursor_col[4];
    int flags;
    int num_input_samples;
    int symmetry_flags;
};

struct ParticleBrushData
{
    short size;
    short step;
    short invert;
    short count;
    int flag;
    float strength;
};

struct TransformOrientation
{
    TransformOrientation *next;
    TransformOrientation *prev;
    char name[64];
    float mat[3][3];
    int pad;
};

struct UnifiedPaintSettings
{
    int size;
    float unprojected_radius;
    float alpha;
    float weight;
    float rgb[3];
    float secondary_rgb[3];
    int flag;
    float last_rake[2];
    float brush_rotation;
    int draw_anchored;
    int anchored_size;
    char draw_inverted;
    char pad3[7];
    float overlap_factor;
    float anchored_initial_mouse[2];
    int stroke_active;
    float size_pressure_value;
    float tex_mouse[2];
    float mask_tex_mouse[2];
    int do_linear_conversion;
    void *colorspace;
    float pixel_radius;
    int pad4;
};

struct MeshStatVis
{
    char type;
    char _pad1[2];
    char overhang_axis;
    float overhang_min;
    float overhang_max;
    float thickness_min;
    float thickness_max;
    char thickness_samples;
    char _pad2[3];
    float distort_min;
    float distort_max;
    float sharp_min;
    float sharp_max;
};

struct bStats
{
    int totobj;
    int totlamp;
    int totobjsel;
    int totcurve;
    int totmesh;
    int totarmature;
    int totvert;
    int totface;
};

struct UnitSettings
{
    float scale_length;
    char system;
    char system_rotation;
    short flag;
};

struct PhysicsSettings
{
    float gravity[3];
    int flag;
    int quick_cache_step;
    int rt;
};

struct RegionView3D
{
    float winmat[4][4];
    float viewmat[4][4];
    float viewinv[4][4];
    float persmat[4][4];
    float persinv[4][4];
    float viewmatob[4][4];
    float persmatob[4][4];
    float clip[6][4];
    float clip_local[6][4];
    BoundBox *clipbb;
    RegionView3D *localvd;
    void *render_engine;
    void *depths;
    void *gpuoffscreen;
    void *sms;
    void *smooth_timer;
    float twmat[4][4];
    float viewquat[4];
    float dist;
    float camdx;
    float camdy;
    float pixsize;
    float ofs[3];
    float camzoom;
    char is_persp;
    char persp;
    char view;
    char viewlock;
    char viewlock_quad;
    char pad[3];
    float ofs_lock[2];
    short twdrawflag;
    short rflag;
    float lviewquat[4];
    short lpersp;
    short lview;
    float gridview;
    float tw_idot[3];
    float rot_angle;
    float rot_axis[3];
};

struct SpaceTimeCache
{
    SpaceTimeCache *next;
    SpaceTimeCache *prev;
    float *array;
};

struct MaskSpaceInfo
{
    Mask *mask;
    char draw_flag;
    char draw_type;
    char overlay_mode;
    char pad3[5];
};

struct FileSelectParams
{
    char title[96];
    char dir[1090];
    char pad_c1[2];
    char file[256];
    char renamefile[256];
    char renameedit[256];
    char filter_glob[64];
    int active_file;
    int sel_first;
    int sel_last;
    short type;
    short flag;
    short sort;
    short display;
    short filter;
    short f_fp;
    char fp_str[8];
};

struct ConsoleLine
{
    ConsoleLine *next;
    ConsoleLine *prev;
    int len_alloc;
    int len;
    char *line;
    int cursor;
    int type;
};

struct uiFont
{
    uiFont *next;
    uiFont *prev;
    char filename[1024];
    short blf_id;
    short uifont_id;
    short r_to_l;
    short hinting;
};

struct uiFontStyle
{
    short uifont_id;
    short points;
    short kerning;
    char pad[6];
    short italic;
    short bold;
    short shadow;
    short shadx;
    short shady;
    short align;
    float shadowalpha;
    float shadowcolor;
};

struct uiWidgetColors
{
    char outline[4];
    char inner[4];
    char inner_sel[4];
    char item[4];
    char text[4];
    char text_sel[4];
    short shaded;
    short shadetop;
    short shadedown;
    short alpha_check;
};

struct uiWidgetStateColors
{
    char inner_anim[4];
    char inner_anim_sel[4];
    char inner_key[4];
    char inner_key_sel[4];
    char inner_driven[4];
    char inner_driven_sel[4];
    float blend;
    float pad;
};

struct uiPanelColors
{
    char header[4];
    char back[4];
    short show_header;
    short show_back;
    int pad;
};

struct uiGradientColors
{
    char gradient[4];
    char high_gradient[4];
    int show_grad;
    int pad2;
};

struct ThemeWireColor
{
    char solid[4];
    char select[4];
    char active[4];
    short flag;
    short pad;
};

struct bAddon
{
    bAddon *next;
    bAddon *prev;
    char module[64];
    IDProperty *prop;
};

struct bPathCompare
{
    bPathCompare *next;
    bPathCompare *prev;
    char path[768];
    char flag;
    char pad[7];
};

struct SolidLight
{
    int flag;
    int pad;
    float col[4];
    float spec[4];
    float vec[4];
};

struct WalkNavigation
{
    float mouse_speed;
    float walk_speed;
    float walk_speed_factor;
    float view_height;
    float jump_height;
    float teleport_time;
    short flag;
    short pad[3];
};

struct ScrEdge
{
    ScrEdge *next;
    ScrEdge *prev;
    ScrVert *v1;
    ScrVert *v2;
    short border;
    short flag;
    int pad;
};

struct Panel
{
    Panel *next;
    Panel *prev;
    void *type;
    void *layout;
    char panelname[64];
    char tabname[64];
    char drawname[64];
    int ofsx;
    int ofsy;
    int sizex;
    int sizey;
    short labelofs;
    short pad;
    short flag;
    short runtime_flag;
    short control;
    short snap;
    int sortorder;
    Panel *paneltab;
    void *activedata;
};

struct PanelCategoryStack
{
    PanelCategoryStack *next;
    PanelCategoryStack *prev;
    char idname[64];
};

struct uiList
{
    uiList *next;
    uiList *prev;
    void *type;
    char list_id[64];
    int layout_type;
    int flag;
    int list_scroll;
    int list_grip;
    int list_last_len;
    int list_last_activei;
    char filter_byname[64];
    int filter_flag;
    int filter_sort_flag;
    IDProperty *properties;
    void *dyn_data;
};

struct uiPreview
{
    uiPreview *next;
    uiPreview *prev;
    char preview_id[64];
    short height;
    short pad1[3];
};

struct FileGlobal
{
    char subvstr[4];
    short subversion;
    short pads;
    short minversion;
    short minsubversion;
    short displaymode;
    short winpos;
    bScreen *curscreen;
    Scene *curscene;
    int fileflags;
    int globalf;
    uint64_t build_commit_timestamp;
    char build_hash[16];
    char filename[1024];
};

struct StripElem
{
    char name[256];
    int orig_width;
    int orig_height;
};

struct StripCrop
{
    int top;
    int bottom;
    int left;
    int right;
};

struct StripTransform
{
    int xofs;
    int yofs;
};

struct StripColorBalance
{
    float lift[3];
    float gamma[3];
    float gain[3];
    int flag;
    int pad;
};

struct StripProxy
{
    char dir[768];
    char file[256];
    void *anim;
    short tc;
    short quality;
    short build_size_flags;
    short build_tc_flags;
};

struct MetaStack
{
    MetaStack *next;
    MetaStack *prev;
    ListBase *oldbasep;
    Sequence *parseq;
    int disp_range[2];
};

struct WipeVars
{
    float edgeWidth;
    float angle;
    short forward;
    short wipetype;
};

struct GlowVars
{
    float fMini;
    float fClamp;
    float fBoost;
    float dDist;
    int dQuality;
    int bNoComp;
};

struct TransformVars
{
    float ScalexIni;
    float ScaleyIni;
    float xIni;
    float yIni;
    float rotIni;
    int percent;
    int interpolation;
    int uniform_scale;
};

struct SolidColorVars
{
    float col[3];
    float pad;
};

struct SpeedControlVars
{
    float *frameMap;
    float globalSpeed;
    int flags;
    int length;
    int lastValidFrame;
};

struct GaussianBlurVars
{
    float size_x;
    float size_y;
};

struct SequenceModifierData
{
    SequenceModifierData *next;
    SequenceModifierData *prev;
    int type;
    int flag;
    char name[64];
    int mask_input_type;
    int pad;
    Sequence *mask_sequence;
    Mask *mask_id;
};

struct SequencerScopes
{
    void *reference_ibuf;
    void *zebra_ibuf;
    void *waveform_ibuf;
    void *sep_waveform_ibuf;
    void *vector_ibuf;
    void *histogram_ibuf;
};

struct Effect
{
    Effect *next;
    Effect *prev;
    short type;
    short flag;
    short buttype;
    short rt;
};

struct BuildEff
{
    BuildEff *next;
    BuildEff *prev;
    short type;
    short flag;
    short buttype;
    short rt;
    float len;
    float sfra;
};

struct PartEff
{
    PartEff *next;
    PartEff *prev;
    short type;
    short flag;
    short buttype;
    short stype;
    short vertgroup;
    short userjit;
    float sta;
    float end;
    float lifetime;
    int totpart;
    int totkey;
    int seed;
    float normfac;
    float obfac;
    float randfac;
    float texfac;
    float randlife;
    float force[3];
    float damp;
    float nabla;
    float vectsize;
    float maxlen;
    float pad;
    float defvec[3];
    float mult[4];
    float life[4];
    short child[4];
    short mat[4];
    short texmap;
    short curmult;
    short staticstep;
    short omat;
    short timetex;
    short speedtex;
    short flag2;
    short flag2neg;
    short disp;
    short vertgroup_v;
    char vgroupname[64];
    char vgroupname_v[64];
    float imat[4][4];
    void *keys;
    Group *group;
};

struct WaveEff
{
    WaveEff *next;
    WaveEff *prev;
    short type;
    short flag;
    short buttype;
    short stype;
    float startx;
    float starty;
    float height;
    float width;
    float narrow;
    float speed;
    float minfac;
    float damp;
    float timeoffs;
    float lifetime;
};

struct TreeStoreElem
{
    short type;
    short nr;
    short flag;
    short used;
    ID *id;
};

struct TreeStore
{
    int totelem;
    int usedelem;
    TreeStoreElem *data;
};

struct bProperty
{
    bProperty *next;
    bProperty *prev;
    char name[64];
    short type;
    short flag;
    int data;
    void *poin;
};

struct bNearSensor
{
    char name[64];
    float dist;
    float resetdist;
    int lastval;
    int pad;
};

struct bMouseSensor
{
    short type;
    short flag;
    short pad1;
    short mode;
    char propname[64];
    char matname[64];
};

struct bTouchSensor
{
    char name[64];
    Material *ma;
    float dist;
    float pad;
};

struct bKeyboardSensor
{
    short key;
    short qual;
    short type;
    short qual2;
    char targetName[64];
    char toggleName[64];
};

struct bPropertySensor
{
    int type;
    int pad;
    char name[64];
    char value[64];
    char maxvalue[64];
};

struct bActuatorSensor
{
    int type;
    int pad;
    char name[64];
};

struct bDelaySensor
{
    short delay;
    short duration;
    short flag;
    short pad;
};

struct bCollisionSensor
{
    char name[64];
    char materialName[64];
    short damptimer;
    short damp;
    short mode;
    short pad2;
};

struct bRadarSensor
{
    char name[64];
    float angle;
    float range;
    short flag;
    short axis;
};

struct bRandomSensor
{
    char name[64];
    int seed;
    int delay;
};

struct bRaySensor
{
    char name[64];
    float range;
    char propname[64];
    char matname[64];
    short mode;
    short pad1;
    int axisflag;
};

struct bArmatureSensor
{
    char posechannel[64];
    char constraint[64];
    int type;
    float value;
};

struct bMessageSensor
{
    Object *fromObject;
    char subject[64];
    char body[64];
};

struct bSensor
{
    bSensor *next;
    bSensor *prev;
    short type;
    short otype;
    short flag;
    short pulse;
    short freq;
    short totlinks;
    short pad1;
    short pad2;
    char name[64];
    void *data;
    bController **links;
    Object *ob;
    short invert;
    short level;
    short tap;
    short pad;
};

struct bJoystickSensor
{
    char name[64];
    char type;
    char joyindex;
    short flag;
    short axis;
    short axis_single;
    int axisf;
    int button;
    int hat;
    int hatf;
    int precision;
};

struct bExpressionCont
{
    char str[128];
};

struct bPythonCont
{
    Text *text;
    char module[64];
    int mode;
    int flag;
};

struct bController
{
    bController *next;
    bController *prev;
    bController *mynew;
    short type;
    short flag;
    short inputs;
    short totlinks;
    short otype;
    short totslinks;
    short pad2;
    short pad3;
    char name[64];
    void *data;
    bActuator **links;
    bSensor **slinks;
    short val;
    short valo;
    int state_mask;
};

struct bAddObjectActuator
{
    int time;
    int pad;
    Object *ob;
};

struct bActionActuator
{
    bAction *act;
    short type;
    short flag;
    float sta;
    float end;
    char name[64];
    char frameProp[64];
    short blendin;
    short priority;
    short layer;
    short end_reset;
    short strideaxis;
    short blend_mode;
    float stridelength;
    float layer_weight;
};

struct Sound3D
{
    float min_gain;
    float max_gain;
    float reference_distance;
    float max_distance;
    float rolloff_factor;
    float cone_inner_angle;
    float cone_outer_angle;
    float cone_outer_gain;
};

struct bEditObjectActuator
{
    int time;
    short type;
    short flag;
    Object *ob;
    Mesh *me;
    char name[64];
    float linVelocity[3];
    float angVelocity[3];
    float mass;
    short localflag;
    short dyn_operation;
    short upflag;
    short trackflag;
    int pad;
};

struct bSceneActuator
{
    short type;
    short pad1;
    int pad;
    Scene *scene;
    Object *camera;
};

struct bPropertyActuator
{
    int pad;
    int type;
    char name[64];
    char value[64];
    Object *ob;
};

struct bObjectActuator
{
    short flag;
    short type;
    short otype;
    short damping;
    float forceloc[3];
    float forcerot[3];
    float pad[3];
    float pad1[3];
    float dloc[3];
    float drot[3];
    float linearvelocity[3];
    float angularvelocity[3];
    Object *reference;
};

struct bIpoActuator
{
    short flag;
    short type;
    float sta;
    float end;
    char name[64];
    char frameProp[64];
    short pad1;
    short pad2;
    short pad3;
    short pad4;
};

struct bCameraActuator
{
    Object *ob;
    float height;
    float min;
    float max;
    float damping;
    short pad1;
    short axis;
    float pad2;
};

struct bConstraintActuator
{
    short type;
    short mode;
    short flag;
    short damp;
    short time;
    short rotdamp;
    int pad;
    float minloc[3];
    float maxloc[3];
    float minrot[3];
    float maxrot[3];
    char matprop[64];
};

struct bGroupActuator
{
    short flag;
    short type;
    int sta;
    int end;
    char name[64];
    short pad[3];
    short cur;
    short butsta;
    short butend;
};

struct bRandomActuator
{
    int seed;
    int distribution;
    int int_arg_1;
    int int_arg_2;
    float float_arg_1;
    float float_arg_2;
    char propname[64];
};

struct bMessageActuator
{
    char toPropName[64];
    Object *toObject;
    char subject[64];
    short bodyType;
    short pad1;
    int pad2;
    char body[64];
};

struct bGameActuator
{
    short flag;
    short type;
    int sta;
    int end;
    char filename[64];
    char loadaniname[64];
};

struct bVisibilityActuator
{
    int flag;
};

struct bTwoDFilterActuator
{
    char pad[4];
    short type;
    short flag;
    int int_arg;
    float float_arg;
    Text *text;
};

struct bParentActuator
{
    char pad[2];
    short flag;
    int type;
    Object *ob;
};

struct bStateActuator
{
    int type;
    int mask;
};

struct bArmatureActuator
{
    char posechannel[64];
    char constraint[64];
    int type;
    float weight;
    float influence;
    float pad;
    Object *target;
    Object *subtarget;
};

struct bSteeringActuator
{
    char pad[5];
    char flag;
    short facingaxis;
    int type;
    float dist;
    float velocity;
    float acceleration;
    float turnspeed;
    int updateTime;
    Object *target;
    Object *navmesh;
};

struct bMouseActuator
{
    short type;
    short flag;
    int object_axis[2];
    float threshold[2];
    float sensitivity[2];
    float limit_x[2];
    float limit_y[2];
};

struct bActuator
{
    bActuator *next;
    bActuator *prev;
    bActuator *mynew;
    short type;
    short flag;
    short otype;
    short go;
    char name[64];
    void *data;
    Object *ob;
};

struct GroupObject
{
    GroupObject *next;
    GroupObject *prev;
    Object *ob;
    void *lampren;
    short recalc;
    char pad[6];
};

struct bMotionPathVert
{
    float co[3];
    int flag;
};

struct bMotionPath
{
    bMotionPathVert *points;
    int length;
    int start_frame;
    int end_frame;
    int flag;
};

struct bAnimVizSettings
{
    int ghost_sf;
    int ghost_ef;
    int ghost_bc;
    int ghost_ac;
    short ghost_type;
    short ghost_step;
    short ghost_flag;
    short recalc;
    short path_type;
    short path_step;
    short path_viewflag;
    short path_bakeflag;
    int path_sf;
    int path_ef;
    int path_bc;
    int path_ac;
};

struct bIKParam
{
    int iksolver;
};

struct bItasc
{
    int iksolver;
    float precision;
    short numiter;
    short numstep;
    float minstep;
    float maxstep;
    short solver;
    short flag;
    float feedback;
    float maxvel;
    float dampmax;
    float dampeps;
};

struct bConstraintChannel
{
    bConstraintChannel *next;
    bConstraintChannel *prev;
    Ipo *ipo;
    short flag;
    char name[30];
};

struct bConstraint
{
    bConstraint *next;
    bConstraint *prev;
    void *data;
    short type;
    short flag;
    char ownspace;
    char tarspace;
    char name[64];
    short pad;
    float enforce;
    float headtail;
    Ipo *ipo;
    float lin_error;
    float rot_error;
};

struct bConstraintTarget
{
    bConstraintTarget *next;
    bConstraintTarget *prev;
    Object *tar;
    char subtarget[64];
    float matrix[4][4];
    short space;
    short flag;
    short type;
    short rotOrder;
};

struct bKinematicConstraint
{
    Object *tar;
    short iterations;
    short flag;
    short rootbone;
    short max_rootbone;
    char subtarget[64];
    Object *poletar;
    char polesubtarget[64];
    float poleangle;
    float weight;
    float orientweight;
    float grabtarget[3];
    short type;
    short mode;
    float dist;
};

struct bSplineIKConstraint
{
    Object *tar;
    float *points;
    short numpoints;
    short chainlen;
    short flag;
    short xzScaleMode;
};

struct bTrackToConstraint
{
    Object *tar;
    int reserved1;
    int reserved2;
    int flags;
    int pad;
    char subtarget[64];
};

struct bRotateLikeConstraint
{
    Object *tar;
    int flag;
    int reserved1;
    char subtarget[64];
};

struct bLocateLikeConstraint
{
    Object *tar;
    int flag;
    int reserved1;
    char subtarget[64];
};

struct bSizeLikeConstraint
{
    Object *tar;
    int flag;
    int reserved1;
    char subtarget[64];
};

struct bSameVolumeConstraint
{
    int flag;
    float volume;
};

struct bTransLikeConstraint
{
    Object *tar;
    char subtarget[64];
};

struct bMinMaxConstraint
{
    Object *tar;
    int minmaxflag;
    float offset;
    int flag;
    short sticky;
    short stuck;
    short pad1;
    short pad2;
    float cache[3];
    char subtarget[64];
};

struct bActionConstraint
{
    Object *tar;
    short type;
    short local;
    int start;
    int end;
    float min;
    float max;
    int flag;
    bAction *act;
    char subtarget[64];
};

struct bLockTrackConstraint
{
    Object *tar;
    int trackflag;
    int lockflag;
    char subtarget[64];
};

struct bDampTrackConstraint
{
    Object *tar;
    int trackflag;
    int pad;
    char subtarget[64];
};

struct bFollowPathConstraint
{
    Object *tar;
    float offset;
    float offset_fac;
    int followflag;
    short trackflag;
    short upflag;
};

struct bStretchToConstraint
{
    Object *tar;
    int flag;
    int volmode;
    int plane;
    float orglength;
    float bulge;
    float bulge_min;
    float bulge_max;
    float bulge_smooth;
    char subtarget[64];
};

struct bRigidBodyJointConstraint
{
    Object *tar;
    Object *child;
    int type;
    float pivX;
    float pivY;
    float pivZ;
    float axX;
    float axY;
    float axZ;
    float minLimit[6];
    float maxLimit[6];
    float extraFz;
    short flag;
    short pad;
    short pad1;
    short pad2;
};

struct bClampToConstraint
{
    Object *tar;
    int flag;
    int flag2;
};

struct bChildOfConstraint
{
    Object *tar;
    int flag;
    int pad;
    float invmat[4][4];
    char subtarget[64];
};

struct bTransformConstraint
{
    Object *tar;
    char subtarget[64];
    short from;
    short to;
    char map[3];
    char expo;
    float from_min[3];
    float from_max[3];
    float to_min[3];
    float to_max[3];
    float from_min_rot[3];
    float from_max_rot[3];
    float to_min_rot[3];
    float to_max_rot[3];
    float from_min_scale[3];
    float from_max_scale[3];
    float to_min_scale[3];
    float to_max_scale[3];
};

struct bPivotConstraint
{
    Object *tar;
    char subtarget[64];
    float offset[3];
    short rotAxis;
    short flag;
};

struct bLocLimitConstraint
{
    float xmin;
    float xmax;
    float ymin;
    float ymax;
    float zmin;
    float zmax;
    short flag;
    short flag2;
};

struct bRotLimitConstraint
{
    float xmin;
    float xmax;
    float ymin;
    float ymax;
    float zmin;
    float zmax;
    short flag;
    short flag2;
};

struct bSizeLimitConstraint
{
    float xmin;
    float xmax;
    float ymin;
    float ymax;
    float zmin;
    float zmax;
    short flag;
    short flag2;
};

struct bDistLimitConstraint
{
    Object *tar;
    char subtarget[64];
    float dist;
    float soft;
    short flag;
    short mode;
    int pad;
};

struct bShrinkwrapConstraint
{
    Object *target;
    float dist;
    short shrinkType;
    char projAxis;
    char projAxisSpace;
    float projLimit;
    char pad[4];
};

struct bFollowTrackConstraint
{
    MovieClip *clip;
    char track[64];
    int flag;
    int frame_method;
    char object[64];
    Object *camera;
    Object *depth_ob;
};

struct bCameraSolverConstraint
{
    MovieClip *clip;
    int flag;
    int pad;
};

struct bObjectSolverConstraint
{
    MovieClip *clip;
    int flag;
    int pad;
    char object[64];
    float invmat[4][4];
    Object *camera;
};

struct bActionModifier
{
    bActionModifier *next;
    bActionModifier *prev;
    short type;
    short flag;
    char channel[32];
    float noisesize;
    float turbul;
    short channels;
    short no_rot_axis;
    Object *ob;
};

struct bNodeStack
{
    float vec[4];
    float min;
    float max;
    void *data;
    short hasinput;
    short hasoutput;
    short datatype;
    short sockettype;
    short is_copy;
    short external;
    short pad[2];
};

struct bNodeInstanceKey
{
    int value;
};

struct bNodeLink
{
    bNodeLink *next;
    bNodeLink *prev;
    bNode *fromnode;
    bNode *tonode;
    bNodeSocket *fromsock;
    bNodeSocket *tosock;
    int flag;
    int pad;
};

struct bNodeSocketValueInt
{
    int subtype;
    int value;
    int min;
    int max;
};

struct bNodeSocketValueFloat
{
    int subtype;
    float value;
    float min;
    float max;
};

struct bNodeSocketValueBoolean
{
    char value;
    char pad[3];
};

struct bNodeSocketValueVector
{
    int subtype;
    float value[3];
    float min;
    float max;
};

struct bNodeSocketValueRGBA
{
    float value[4];
};

struct bNodeSocketValueString
{
    int subtype;
    int pad;
    char value[1024];
};

struct NodeFrame
{
    short flag;
    short label_size;
};

struct NodeImageAnim
{
    int frames;
    int sfra;
    int nr;
    char cyclic;
    char movie;
    short pad;
};

struct ColorCorrectionData
{
    float saturation;
    float contrast;
    float gamma;
    float gain;
    float lift;
    int pad;
};

struct NodeBokehImage
{
    float angle;
    int flaps;
    float rounding;
    float catadioptric;
    float lensshift;
};

struct NodeBoxMask
{
    float x;
    float y;
    float rotation;
    float height;
    float width;
    int pad;
};

struct NodeEllipseMask
{
    float x;
    float y;
    float rotation;
    float height;
    float width;
    int pad;
};

struct NodeImageLayer
{
    int pass_index;
    int pass_flag;
};

struct NodeBlurData
{
    short sizex;
    short sizey;
    short samples;
    short maxspeed;
    short minspeed;
    short relative;
    short aspect;
    short curved;
    float fac;
    float percentx;
    float percenty;
    short filtertype;
    char bokeh;
    char gamma;
    int image_in_width;
    int image_in_height;
};

struct NodeDBlurData
{
    float center_x;
    float center_y;
    float distance;
    float angle;
    float spin;
    float zoom;
    short iter;
    char wrap;
    char pad;
};

struct NodeBilateralBlurData
{
    float sigma_color;
    float sigma_space;
    short iter;
    short pad;
};

struct NodeHueSat
{
    float hue;
    float sat;
    float val;
};

struct NodeChroma
{
    float t1;
    float t2;
    float t3;
    float fsize;
    float fstrength;
    float falpha;
    float key[4];
    short algorithm;
    short channel;
};

struct NodeTwoXYs
{
    short x1;
    short x2;
    short y1;
    short y2;
    float fac_x1;
    float fac_x2;
    float fac_y1;
    float fac_y2;
};

struct NodeTwoFloats
{
    float x;
    float y;
};

struct NodeGeometry
{
    char uvname[64];
    char colname[64];
};

struct NodeVertexCol
{
    char name[64];
};

struct NodeDefocus
{
    char bktype;
    char pad_c1;
    char preview;
    char gamco;
    short samples;
    short no_zbuf;
    float fstop;
    float maxblur;
    float bthresh;
    float scale;
    float rotation;
    float pad_f1;
};

struct NodeScriptDict
{
    void *dict;
    void *node;
};

struct NodeGlare
{
    char quality;
    char type;
    char iter;
    char angle;
    char pad_c1;
    char size;
    char pad[2];
    float colmod;
    float mix;
    float threshold;
    float fade;
    float angle_ofs;
    float pad_f1;
};

struct NodeTonemap
{
    float key;
    float offset;
    float gamma;
    float f;
    float m;
    float a;
    float c;
    int type;
};

struct NodeLensDist
{
    short jit;
    short proj;
    short fit;
    short pad;
};

struct NodeColorBalance
{
    float slope[3];
    float offset[3];
    float power[3];
    float lift[3];
    float gamma[3];
    float gain[3];
};

struct NodeColorspill
{
    short limchan;
    short unspill;
    float limscale;
    float uspillr;
    float uspillg;
    float uspillb;
};

struct NodeDilateErode
{
    char falloff;
    char pad[7];
};

struct NodeMask
{
    int size_x;
    int size_y;
};

struct NodeShaderAttribute
{
    char name[64];
};

struct NodeShaderVectTransform
{
    int type;
    int convert_from;
    int convert_to;
    int pad;
};

struct TexNodeOutput
{
    char name[64];
};

struct NodeKeyingScreenData
{
    char tracking_object[64];
};

struct NodeKeyingData
{
    float screen_balance;
    float despill_factor;
    float despill_balance;
    int edge_kernel_radius;
    float edge_kernel_tolerance;
    float clip_black;
    float clip_white;
    int dilate_distance;
    int feather_distance;
    int feather_falloff;
    int blur_pre;
    int blur_post;
};

struct NodeTrackPosData
{
    char tracking_object[64];
    char track_name[64];
};

struct NodeTranslateData
{
    char wrap_axis;
    char relative;
    char pad[6];
};

struct NodePlaneTrackDeformData
{
    char tracking_object[64];
    char plane_track_name[64];
};

struct NodeShaderScript
{
    int mode;
    int flag;
    char filepath[1024];
    char bytecode_hash[64];
    char *bytecode;
};

struct NodeShaderTangent
{
    int direction_type;
    int axis;
    char uv_map[64];
};

struct NodeShaderNormalMap
{
    int space;
    char uv_map[64];
};

struct NodeShaderUVMap
{
    char uv_map[64];
};

struct NodeSunBeams
{
    float source[2];
    float ray_length;
};

struct CurveMapPoint
{
    float x;
    float y;
    short flag;
    short shorty;
};

struct CurveMap
{
    short totpoint;
    short flag;
    float range;
    float mintable;
    float maxtable;
    float ext_in[2];
    float ext_out[2];
    CurveMapPoint *curve;
    CurveMapPoint *table;
    CurveMapPoint *premultable;
    float premul_ext_in[2];
    float premul_ext_out[2];
};

struct Histogram
{
    int channels;
    int x_resolution;
    float data_luma[256];
    float data_r[256];
    float data_g[256];
    float data_b[256];
    float data_a[256];
    float xmax;
    float ymax;
    short mode;
    short flag;
    int height;
    float co[2][2];
};

struct ColorManagedViewSettings
{
    int flag;
    int pad;
    char look[64];
    char view_transform[64];
    float exposure;
    float gamma;
    CurveMapping *curve_mapping;
    void *pad2;
};

struct ColorManagedDisplaySettings
{
    char display_device[64];
};

struct ColorManagedColorspaceSettings
{
    char name[64];
};

struct BrushClone
{
    Image *image;
    float offset[2];
    float alpha;
    float pad;
};

struct PaletteColor
{
    PaletteColor *next;
    PaletteColor *prev;
    float rgb[3];
    float value;
};

struct CustomDataLayer
{
    int type;
    int offset;
    int flag;
    int active;
    int active_rnd;
    int active_clone;
    int active_mask;
    int uid;
    char name[64];
    void *data;
};

struct CustomDataExternal
{
    char filename[1024];
};

struct CustomData
{
    CustomDataLayer *layers;
    int typemap[41];
    int totlayer;
    int maxlayer;
    int totsize;
    void *pool;
    CustomDataExternal *external;
};

struct HairKey
{
    float co[3];
    float time;
    float weight;
    short editflag;
    short pad;
};

struct ParticleKey
{
    float co[3];
    float vel[3];
    float rot[4];
    float ave[3];
    float time;
};

struct ParticleSpring
{
    float rest_length;
    int particle_index[2];
    int delete_flag;
};

struct ChildParticle
{
    int num;
    int parent;
    int pa[4];
    float w[4];
    float fuv[4];
    float foffset;
    float rt;
};

struct ParticleTarget
{
    ParticleTarget *next;
    ParticleTarget *prev;
    Object *ob;
    int psys;
    short flag;
    short mode;
    float time;
    float duration;
};

struct ParticleDupliWeight
{
    ParticleDupliWeight *next;
    ParticleDupliWeight *prev;
    Object *ob;
    short count;
    short flag;
    short index;
    short rt;
};

struct SPHFluidSettings
{
    float radius;
    float spring_k;
    float rest_length;
    float plasticity_constant;
    float yield_ratio;
    float plasticity_balance;
    float yield_balance;
    float viscosity_omega;
    float viscosity_beta;
    float stiffness_k;
    float stiffness_knear;
    float rest_density;
    float buoyancy;
    int flag;
    int spring_frames;
    short solver;
    short pad[3];
};

struct ClothSimSettings
{
    void *cache;
    float mingoal;
    float Cdis;
    float Cvi;
    float gravity[3];
    float dt;
    float mass;
    float structural;
    float shear;
    float bending;
    float max_bend;
    float max_struct;
    float max_shear;
    float max_sewing;
    float avg_spring_len;
    float timescale;
    float maxgoal;
    float eff_force_scale;
    float eff_wind_scale;
    float sim_time_old;
    float defgoal;
    float goalspring;
    float goalfrict;
    float velocity_smooth;
    float collider_friction;
    float vel_damping;
    float shrink_min;
    float shrink_max;
    int stepsPerFrame;
    int flags;
    int preroll;
    int maxspringlen;
    short solver_type;
    short vgroup_bend;
    short vgroup_mass;
    short vgroup_struct;
    short vgroup_shrink;
    short shapekey_rest;
    short presets;
    short reset;
    char pad[4];
    EffectorWeights *effector_weights;
};

struct ClothCollSettings
{
    void *collision_list;
    float epsilon;
    float self_friction;
    float friction;
    float selfepsilon;
    float repel_force;
    float distance_repel;
    int flags;
    short self_loop_count;
    short loop_count;
    Group *group;
    short vgroup_selfcol;
    short pad;
    int pad2;
};

struct bGPDspoint
{
    float x;
    float y;
    float z;
    float pressure;
    float time;
    int flag;
};

struct bGPDstroke
{
    bGPDstroke *next;
    bGPDstroke *prev;
    bGPDspoint *points;
    void *pad;
    int totpoints;
    short thickness;
    short flag;
    double inittime;
};

struct wmKeyMapItem
{
    wmKeyMapItem *next;
    wmKeyMapItem *prev;
    char idname[64];
    IDProperty *properties;
    char propvalue_str[64];
    short propvalue;
    short type;
    short val;
    short shift;
    short ctrl;
    short alt;
    short oskey;
    short keymodifier;
    short flag;
    short maptype;
    short id;
    short pad;
    void *ptr;
};

struct wmKeyMapDiffItem
{
    wmKeyMapDiffItem *next;
    wmKeyMapDiffItem *prev;
    wmKeyMapItem *remove_item;
    wmKeyMapItem *add_item;
};

struct FModifier
{
    FModifier *next;
    FModifier *prev;
    void *data;
    char name[64];
    short type;
    short flag;
    float influence;
    float sfra;
    float efra;
    float blendin;
    float blendout;
};

struct FMod_Generator
{
    float *coefficients;
    int arraysize;
    int poly_order;
    int mode;
    int flag;
};

struct FMod_FunctionGenerator
{
    float amplitude;
    float phase_multiplier;
    float phase_offset;
    float value_offset;
    int type;
    int flag;
};

struct FCM_EnvelopeData
{
    float min;
    float max;
    float time;
    short f1;
    short f2;
};

struct FMod_Envelope
{
    FCM_EnvelopeData *data;
    int totvert;
    float midval;
    float min;
    float max;
};

struct FMod_Cycles
{
    short before_mode;
    short after_mode;
    short before_cycles;
    short after_cycles;
};

struct FMod_Python
{
    Text *script;
    IDProperty *prop;
};

struct FMod_Noise
{
    float size;
    float strength;
    float phase;
    float offset;
    short depth;
    short modification;
};

struct FMod_Stepped
{
    float step_size;
    float offset;
    float start_frame;
    float end_frame;
    int flag;
};

struct DriverTarget
{
    ID *id;
    char *rna_path;
    char pchan_name[32];
    short transChan;
    short flag;
    int idtype;
};

struct FPoint
{
    float vec[2];
    int flag;
    int pad;
};

struct AnimMapPair
{
    char from[128];
    char to[128];
};

struct KS_Path
{
    KS_Path *next;
    KS_Path *prev;
    ID *id;
    char group[64];
    int idtype;
    short groupmode;
    short pad;
    char *rna_path;
    int array_index;
    short flag;
    short keyingflag;
};

struct AnimOverride
{
    AnimOverride *next;
    AnimOverride *prev;
    char *rna_path;
    int array_index;
    float value;
};

struct BoidRule
{
    BoidRule *next;
    BoidRule *prev;
    int type;
    int flag;
    char name[32];
};

struct BoidData
{
    float health;
    float acc[3];
    short state_id;
    short mode;
};

struct SmokeFlowSettings
{
    SmokeModifierData *smd;
    void *dm;
    ParticleSystem *psys;
    Tex *noise_texture;
    float *verts_old;
    int numverts;
    float vel_multi;
    float vel_normal;
    float vel_random;
    float density;
    float color[3];
    float fuel_amount;
    float temp;
    float volume_density;
    float surface_distance;
    float particle_size;
    int subframes;
    float texture_size;
    float texture_offset;
    int pad;
    char uvlayer_name[64];
    short vgroup_density;
    short type;
    short source;
    short texture_type;
    int flags;
};

struct SmokeCollSettings
{
    SmokeModifierData *smd;
    void *dm;
    float *verts_old;
    int numverts;
    short type;
    short pad;
};

struct MovieClipUser
{
    int framenr;
    short render_size;
    short render_flag;
};

struct MovieClipProxy
{
    char dir[768];
    short tc;
    short quality;
    short build_size_flag;
    short build_tc_flag;
};

struct MovieReconstructedCamera
{
    int framenr;
    float error;
    float mat[4][4];
};

struct MovieTrackingCamera
{
    void *intrinsics;
    short distortion_model;
    short pad;
    float sensor_width;
    float pixel_aspect;
    float focal;
    short units;
    short pad1;
    float principal[2];
    float k1;
    float k2;
    float k3;
    float division_k1;
    float division_k2;
};

struct MovieTrackingMarker
{
    float pos[2];
    float pattern_corners[4][2];
    float search_min[2];
    float search_max[2];
    int framenr;
    int flag;
};

struct MovieTrackingTrack
{
    MovieTrackingTrack *next;
    MovieTrackingTrack *prev;
    char name[64];
    float pat_min[2];
    float pat_max[2];
    float search_min[2];
    float search_max[2];
    float offset[2];
    int markersnr;
    int last_marker;
    MovieTrackingMarker *markers;
    float bundle_pos[3];
    float error;
    int flag;
    int pat_flag;
    int search_flag;
    float color[3];
    short frames_limit;
    short margin;
    short pattern_match;
    short motion_model;
    int algorithm_flag;
    float minimum_correlation;
    bGPdata *gpd;
    float weight;
    float pad;
};

struct MovieTrackingPlaneMarker
{
    float corners[4][2];
    int framenr;
    int flag;
};

struct MovieTrackingPlaneTrack
{
    MovieTrackingPlaneTrack *next;
    MovieTrackingPlaneTrack *prev;
    char name[64];
    MovieTrackingTrack **point_tracks;
    int point_tracksnr;
    int pad;
    MovieTrackingPlaneMarker *markers;
    int markersnr;
    int flag;
    Image *image;
    float image_opacity;
    int last_marker;
};

struct MovieTrackingSettings
{
    int flag;
    short default_motion_model;
    short default_algorithm_flag;
    float default_minimum_correlation;
    short default_pattern_size;
    short default_search_size;
    short default_frames_limit;
    short default_margin;
    short default_pattern_match;
    short default_flag;
    float default_weight;
    short motion_flag;
    short speed;
    int keyframe1;
    int keyframe2;
    int reconstruction_flag;
    short refine_camera_intrinsics;
    short pad2;
    float dist;
    int clean_frames;
    int clean_action;
    float clean_error;
    float object_distance;
    int pad3;
};

struct MovieTrackingStabilization
{
    int flag;
    int tot_track;
    int act_track;
    float maxscale;
    MovieTrackingTrack *rot_track;
    float locinf;
    float scaleinf;
    float rotinf;
    int filter;
    int ok;
    float scale;
};

struct MovieTrackingReconstruction
{
    int flag;
    float error;
    int last_camera;
    int camnr;
    MovieReconstructedCamera *cameras;
};

struct MovieTrackingStats
{
    char message[256];
};

struct MovieTrackingDopesheetChannel
{
    MovieTrackingDopesheetChannel *next;
    MovieTrackingDopesheetChannel *prev;
    MovieTrackingTrack *track;
    int pad;
    char name[64];
    int tot_segment;
    int *segments;
    int max_segment;
    int total_frames;
};

struct MovieTrackingDopesheetCoverageSegment
{
    MovieTrackingDopesheetCoverageSegment *next;
    MovieTrackingDopesheetCoverageSegment *prev;
    int coverage;
    int start_frame;
    int end_frame;
    int pad;
};

struct DynamicPaintBrushSettings
{
    DynamicPaintModifierData *pmd;
    void *dm;
    ParticleSystem *psys;
    Material *mat;
    int flags;
    int collision;
    float r;
    float g;
    float b;
    float alpha;
    float wetness;
    float particle_radius;
    float particle_smooth;
    float paint_distance;
    ColorBand *paint_ramp;
    ColorBand *vel_ramp;
    short proximity_falloff;
    short wave_type;
    short ray_dir;
    short pad;
    float wave_factor;
    float wave_clamp;
    float max_velocity;
    float smudge_strength;
};

struct MaskParent
{
    int id_type;
    int type;
    ID *id;
    char parent[64];
    char sub_parent[64];
    float parent_orig[2];
    float parent_corners_orig[4][2];
};

struct MaskSplinePointUW
{
    float u;
    float w;
    int flag;
};

struct MaskLayerShape
{
    MaskLayerShape *next;
    MaskLayerShape *prev;
    float *data;
    int tot_vert;
    int frame;
    char flag;
    char pad[7];
};

struct RigidBodyOb
{
    void *physics_object;
    void *physics_shape;
    short type;
    short shape;
    int flag;
    int col_groups;
    short mesh_source;
    short pad;
    float mass;
    float friction;
    float restitution;
    float margin;
    float lin_damping;
    float ang_damping;
    float lin_sleep_thresh;
    float ang_sleep_thresh;
    float orn[4];
    float pos[3];
    float pad1;
};

struct RigidBodyCon
{
    Object *ob1;
    Object *ob2;
    short type;
    short num_solver_iterations;
    int flag;
    float breaking_threshold;
    float pad;
    float limit_lin_x_lower;
    float limit_lin_x_upper;
    float limit_lin_y_lower;
    float limit_lin_y_upper;
    float limit_lin_z_lower;
    float limit_lin_z_upper;
    float limit_ang_x_lower;
    float limit_ang_x_upper;
    float limit_ang_y_lower;
    float limit_ang_y_upper;
    float limit_ang_z_lower;
    float limit_ang_z_upper;
    float spring_stiffness_x;
    float spring_stiffness_y;
    float spring_stiffness_z;
    float spring_damping_x;
    float spring_damping_y;
    float spring_damping_z;
    float motor_lin_target_velocity;
    float motor_ang_target_velocity;
    float motor_lin_max_impulse;
    float motor_ang_max_impulse;
    void *physics_constraint;
};

struct FreestyleLineSet
{
    FreestyleLineSet *next;
    FreestyleLineSet *prev;
    char name[64];
    int flags;
    int selection;
    short qi;
    short pad1;
    int qi_start;
    int qi_end;
    int edge_types;
    int exclude_edge_types;
    int pad2;
    Group *group;
    FreestyleLineStyle *linestyle;
};

struct FreestyleModuleConfig
{
    FreestyleModuleConfig *next;
    FreestyleModuleConfig *prev;
    Text *script;
    short is_displayed;
    short pad[3];
};

struct LineStyleModifier
{
    LineStyleModifier *next;
    LineStyleModifier *prev;
    char name[64];
    int type;
    float influence;
    int flags;
    int blend;
};

struct IDPropertyData
{
    void *pointer;
    ListBase group;
    int val;
    int val2;
};

struct Library
{
    ID id;
    ID *idblock;
    void *filedata;
    char name[1024];
    char filepath[1024];
    Library *parent;
    PackedFile *packedfile;
};

struct Camera
{
    ID id;
    AnimData *adt;
    char type;
    char dtx;
    short flag;
    float passepartalpha;
    float clipsta;
    float clipend;
    float lens;
    float ortho_scale;
    float drawsize;
    float sensor_x;
    float sensor_y;
    float shiftx;
    float shifty;
    float YF_dofdist;
    Ipo *ipo;
    Object *dof_ob;
    char sensor_fit;
    char pad[7];
};

struct ColorBand
{
    short tot;
    short cur;
    char ipotype;
    char ipotype_hue;
    char color_mode;
    char pad[1];
    CBData data[32];
};

struct Lamp
{
    ID id;
    AnimData *adt;
    short type;
    short flag;
    int mode;
    short colormodel;
    short totex;
    float r;
    float g;
    float b;
    float k;
    float shdwr;
    float shdwg;
    float shdwb;
    float shdwpad;
    float energy;
    float dist;
    float spotsize;
    float spotblend;
    float haint;
    float att1;
    float att2;
    CurveMapping *curfalloff;
    short falloff_type;
    short pad2;
    float clipsta;
    float clipend;
    float bias;
    float soft;
    float compressthresh;
    float bleedbias;
    float pad5;
    short bufsize;
    short samp;
    short buffers;
    short filtertype;
    char bufflag;
    char buftype;
    short ray_samp;
    short ray_sampy;
    short ray_sampz;
    short ray_samp_type;
    short area_shape;
    float area_size;
    float area_sizey;
    float area_sizez;
    float adapt_thresh;
    short ray_samp_method;
    short shadowmap_type;
    short texact;
    short shadhalostep;
    short sun_effect_type;
    short skyblendtype;
    float horizon_brightness;
    float spread;
    float sun_brightness;
    float sun_size;
    float backscattered_light;
    float sun_intensity;
    float atm_turbidity;
    float atm_inscattering_factor;
    float atm_extinction_factor;
    float atm_distance_factor;
    float skyblendfac;
    float sky_exposure;
    float shadow_frustum_size;
    short sky_colorspace;
    char pad4[2];
    Ipo *ipo;
    MTex *mtex[18];
    short pr_texture;
    short use_nodes;
    char pad6[4];
    PreviewImage *preview;
    bNodeTree *nodetree;
};

struct VFont
{
    ID id;
    char name[1024];
    void *data;
    PackedFile *packedfile;
    PackedFile *temp_pf;
};

struct EditNurb
{
    ListBase nurbs;
    void *keyindex;
    int shapenr;
    char pad[4];
};

struct MultiresColFace
{
    MultiresCol col[4];
};

struct MappingInfoModifierData
{
    ModifierData modifier;
    Tex *texture;
    Object *map_object;
    char uvlayer_name[64];
    int uvlayer_tmp;
    int texmapping;
};

struct SubsurfModifierData
{
    ModifierData modifier;
    short subdivType;
    short levels;
    short renderLevels;
    short flags;
    void *emCache;
    void *mCache;
};

struct LatticeModifierData
{
    ModifierData modifier;
    Object *object;
    char name[64];
    float strength;
    char pad[4];
};

struct CurveModifierData
{
    ModifierData modifier;
    Object *object;
    char name[64];
    short defaxis;
    char pad[6];
};

struct BuildModifierData
{
    ModifierData modifier;
    float start;
    float length;
    short flag;
    short randomize;
    int seed;
};

struct MaskModifierData
{
    ModifierData modifier;
    Object *ob_arm;
    char vgroup[64];
    int mode;
    int flag;
};

struct ArrayModifierData
{
    ModifierData modifier;
    Object *start_cap;
    Object *end_cap;
    Object *curve_ob;
    Object *offset_ob;
    float offset[3];
    float scale[3];
    float length;
    float merge_dist;
    int fit_type;
    int offset_type;
    int flags;
    int count;
};

struct MirrorModifierData
{
    ModifierData modifier;
    short axis;
    short flag;
    float tolerance;
    Object *mirror_ob;
};

struct EdgeSplitModifierData
{
    ModifierData modifier;
    float split_angle;
    int flags;
};

struct BevelModifierData
{
    ModifierData modifier;
    float value;
    int res;
    short flags;
    short val_flags;
    short lim_flags;
    short e_flags;
    short mat;
    short pad;
    int pad2;
    float profile;
    float bevel_angle;
    char defgrp_name[64];
};

struct SmokeModifierData
{
    ModifierData modifier;
    SmokeDomainSettings *domain;
    SmokeFlowSettings *flow;
    SmokeCollSettings *coll;
    float time;
    int type;
};

struct DisplaceModifierData
{
    ModifierData modifier;
    Tex *texture;
    Object *map_object;
    char uvlayer_name[64];
    int uvlayer_tmp;
    int texmapping;
    float strength;
    int direction;
    char defgrp_name[64];
    float midlevel;
    int pad;
};

struct UVProjectModifierData
{
    ModifierData modifier;
    Object *projectors[10];
    Image *image;
    int flags;
    int num_projectors;
    float aspectx;
    float aspecty;
    float scalex;
    float scaley;
    char uvlayer_name[64];
    int uvlayer_tmp;
    int pad;
};

struct DecimateModifierData
{
    ModifierData modifier;
    float percent;
    short iter;
    char delimit;
    char pad;
    float angle;
    char defgrp_name[64];
    short flag;
    short mode;
    int face_count;
    int pad2;
};

struct SmoothModifierData
{
    ModifierData modifier;
    float fac;
    char defgrp_name[64];
    short flag;
    short repeat;
};

struct CastModifierData
{
    ModifierData modifier;
    Object *object;
    float fac;
    float radius;
    float size;
    char defgrp_name[64];
    short flag;
    short type;
};

struct WaveModifierData
{
    ModifierData modifier;
    Tex *texture;
    Object *map_object;
    char uvlayer_name[64];
    int uvlayer_tmp;
    int texmapping;
    Object *objectcenter;
    char defgrp_name[64];
    short flag;
    short pad;
    float startx;
    float starty;
    float height;
    float width;
    float narrow;
    float speed;
    float damp;
    float falloff;
    float timeoffs;
    float lifetime;
    float pad1;
};

struct ArmatureModifierData
{
    ModifierData modifier;
    short deformflag;
    short multi;
    int pad2;
    Object *object;
    float *prevCos;
    char defgrp_name[64];
};

struct HookModifierData
{
    ModifierData modifier;
    Object *object;
    char subtarget[64];
    float parentinv[4][4];
    float cent[3];
    float falloff;
    int *indexar;
    int totindex;
    float force;
    char name[64];
};

struct SoftbodyModifierData
{
    ModifierData modifier;
};

struct CollisionModifierData
{
    ModifierData modifier;
    MVert *x;
    MVert *xnew;
    MVert *xold;
    MVert *current_xnew;
    MVert *current_x;
    MVert *current_v;
    MFace *mfaces;
    int numverts;
    int numfaces;
    float time_x;
    float time_xnew;
    void *bvhtree;
};

struct SurfaceModifierData
{
    ModifierData modifier;
    MVert *x;
    MVert *v;
    void *dm;
    void *bvhtree;
    int cfra;
    int numverts;
};

struct BooleanModifierData
{
    ModifierData modifier;
    Object *object;
    int operation;
    int pad;
};

struct MeshDeformModifierData
{
    ModifierData modifier;
    Object *object;
    char defgrp_name[64];
    short gridsize;
    short flag;
    short pad[2];
    MDefInfluence *bindinfluences;
    int *bindoffsets;
    float *bindcagecos;
    int totvert;
    int totcagevert;
    MDefCell *dyngrid;
    MDefInfluence *dyninfluences;
    int *dynverts;
    int dyngridsize;
    int totinfluence;
    float dyncellmin[3];
    float dyncellwidth;
    float bindmat[4][4];
    float *bindweights;
    float *bindcos;
    void (*bindfunc)();
};

struct ParticleSystemModifierData
{
    ModifierData modifier;
    ParticleSystem *psys;
    void *dm;
    int totdmvert;
    int totdmedge;
    int totdmface;
    short flag;
    short pad;
};

struct ParticleInstanceModifierData
{
    ModifierData modifier;
    Object *ob;
    short psys;
    short flag;
    short axis;
    short pad;
    float position;
    float random_position;
};

struct ExplodeModifierData
{
    ModifierData modifier;
    int *facepa;
    short flag;
    short vgroup;
    float protect;
    char uvname[64];
};

struct MultiresModifierData
{
    ModifierData modifier;
    char lvl;
    char sculptlvl;
    char renderlvl;
    char totlvl;
    char simple;
    char flags;
    char pad[2];
};

struct FluidsimModifierData
{
    ModifierData modifier;
    FluidsimSettings *fss;
    PointCache *point_cache;
};

struct ShrinkwrapModifierData
{
    ModifierData modifier;
    Object *target;
    Object *auxTarget;
    char vgroup_name[64];
    float keepDist;
    short shrinkType;
    char shrinkOpts;
    char pad1;
    float projLimit;
    char projAxis;
    char subsurfLevels;
    char pad[2];
};

struct SimpleDeformModifierData
{
    ModifierData modifier;
    Object *origin;
    char vgroup_name[64];
    float factor;
    float limit[2];
    char mode;
    char axis;
    char pad[2];
};

struct ShapeKeyModifierData
{
    ModifierData modifier;
};

struct SolidifyModifierData
{
    ModifierData modifier;
    char defgrp_name[64];
    float offset;
    float offset_fac;
    float offset_fac_vg;
    float offset_clamp;
    float pad;
    float crease_inner;
    float crease_outer;
    float crease_rim;
    int flag;
    short mat_ofs;
    short mat_ofs_rim;
};

struct ScrewModifierData
{
    ModifierData modifier;
    Object *ob_axis;
    int steps;
    int render_steps;
    int iter;
    float screw_ofs;
    float angle;
    char axis;
    char pad;
    short flag;
};

struct OceanModifierData
{
    ModifierData modifier;
    void *ocean;
    void *oceancache;
    int resolution;
    int spatial_size;
    float wind_velocity;
    float damp;
    float smallest_wave;
    float depth;
    float wave_alignment;
    float wave_direction;
    float wave_scale;
    float chop_amount;
    float foam_coverage;
    float time;
    int bakestart;
    int bakeend;
    char cachepath[1024];
    char foamlayername[64];
    char cached;
    char geometry_mode;
    char flag;
    char refresh;
    short repeat_x;
    short repeat_y;
    int seed;
    float size;
    float foam_fade;
    int pad;
};

struct WarpModifierData
{
    ModifierData modifier;
    Tex *texture;
    Object *map_object;
    char uvlayer_name[64];
    int uvlayer_tmp;
    int texmapping;
    Object *object_from;
    Object *object_to;
    CurveMapping *curfalloff;
    char defgrp_name[64];
    float strength;
    float falloff_radius;
    char flag;
    char falloff_type;
    char pad[6];
};

struct WeightVGEditModifierData
{
    ModifierData modifier;
    char defgrp_name[64];
    short edit_flags;
    short falloff_type;
    float default_weight;
    CurveMapping *cmap_curve;
    float add_threshold;
    float rem_threshold;
    float mask_constant;
    char mask_defgrp_name[64];
    int mask_tex_use_channel;
    Tex *mask_texture;
    Object *mask_tex_map_obj;
    int mask_tex_mapping;
    char mask_tex_uvlayer_name[64];
    int pad_i1;
};

struct WeightVGMixModifierData
{
    ModifierData modifier;
    char defgrp_name_a[64];
    char defgrp_name_b[64];
    float default_weight_a;
    float default_weight_b;
    char mix_mode;
    char mix_set;
    char pad_c1[6];
    float mask_constant;
    char mask_defgrp_name[64];
    int mask_tex_use_channel;
    Tex *mask_texture;
    Object *mask_tex_map_obj;
    int mask_tex_mapping;
    char mask_tex_uvlayer_name[64];
    int pad_i1;
};

struct WeightVGProximityModifierData
{
    ModifierData modifier;
    char defgrp_name[64];
    int proximity_mode;
    int proximity_flags;
    Object *proximity_ob_target;
    float mask_constant;
    char mask_defgrp_name[64];
    int mask_tex_use_channel;
    Tex *mask_texture;
    Object *mask_tex_map_obj;
    int mask_tex_mapping;
    char mask_tex_uvlayer_name[64];
    float min_dist;
    float max_dist;
    short falloff_type;
    short pad_s1;
};

struct DynamicPaintModifierData
{
    ModifierData modifier;
    DynamicPaintCanvasSettings *canvas;
    DynamicPaintBrushSettings *brush;
    int type;
    int pad;
};

struct RemeshModifierData
{
    ModifierData modifier;
    float threshold;
    float scale;
    float hermite_num;
    char depth;
    char flag;
    char mode;
    char pad;
};

struct SkinModifierData
{
    ModifierData modifier;
    float branch_smoothing;
    char flag;
    char symmetry_axes;
    char pad[2];
};

struct TriangulateModifierData
{
    ModifierData modifier;
    int flag;
    int quad_method;
    int ngon_method;
    int pad;
};

struct LaplacianSmoothModifierData
{
    ModifierData modifier;
    float lambda;
    float lambda_border;
    float pad1;
    char defgrp_name[64];
    short flag;
    short repeat;
};

struct UVWarpModifierData
{
    ModifierData modifier;
    char axis_u;
    char axis_v;
    char pad[6];
    float center[2];
    Object *object_src;
    char bone_src[64];
    Object *object_dst;
    char bone_dst[64];
    char vgroup_name[64];
    char uvlayer_name[64];
};

struct MeshCacheModifierData
{
    ModifierData modifier;
    char flag;
    char type;
    char time_mode;
    char play_mode;
    char forward_axis;
    char up_axis;
    char flip_axis;
    char interp;
    float factor;
    char deform_mode;
    char pad[7];
    float frame_start;
    float frame_scale;
    float eval_frame;
    float eval_time;
    float eval_factor;
    char filepath[1024];
};

struct LaplacianDeformModifierData
{
    ModifierData modifier;
    char anchor_grp_name[64];
    int total_verts;
    int repeat;
    float *vertexco;
    void *cache_system;
    short flag;
    short pad[3];
};

struct WireframeModifierData
{
    ModifierData modifier;
    char defgrp_name[64];
    float offset;
    float offset_fac;
    float offset_fac_vg;
    float crease_weight;
    short flag;
    short mat_ofs;
    short pad[2];
};

struct Lattice
{
    ID id;
    AnimData *adt;
    short pntsu;
    short pntsv;
    short pntsw;
    short flag;
    short opntsu;
    short opntsv;
    short opntsw;
    short pad2;
    char typeu;
    char typev;
    char typew;
    char pad3;
    int actbp;
    float fu;
    float fv;
    float fw;
    float du;
    float dv;
    float dw;
    BPoint *def;
    Ipo *ipo;
    Key *key;
    MDeformVert *dvert;
    char vgroup[64];
    EditLatt *editlatt;
};

struct PTCacheMem
{
    PTCacheMem *next;
    PTCacheMem *prev;
    int frame;
    int totpoint;
    int data_types;
    int flag;
    void *data[8];
    void *cur[8];
    ListBase extradata;
};

struct PointCache
{
    PointCache *next;
    PointCache *prev;
    int flag;
    int step;
    int simframe;
    int startframe;
    int endframe;
    int editframe;
    int last_exact;
    int last_valid;
    int pad;
    int totpoint;
    int index;
    short compression;
    short rt;
    char name[64];
    char prev_name[64];
    char info[64];
    char path[1024];
    char *cached_frames;
    ListBase mem_cache;
    void *edit;
    void (*free_edit)();
};

struct SoftBody
{
    int totpoint;
    int totspring;
    void *bpoint;
    void *bspring;
    char pad;
    char msg_lock;
    short msg_value;
    float nodemass;
    char namedVG_Mass[64];
    float grav;
    float mediafrict;
    float rklimit;
    float physics_speed;
    float goalspring;
    float goalfrict;
    float mingoal;
    float maxgoal;
    float defgoal;
    short vertgroup;
    char namedVG_Softgoal[64];
    short fuzzyness;
    float inspring;
    float infrict;
    char namedVG_Spring_K[64];
    int sfra;
    int efra;
    int interval;
    short local;
    short solverflags;
    SBVertex **keys;
    int totpointkey;
    int totkey;
    float secondspring;
    float colball;
    float balldamp;
    float ballstiff;
    short sbc_mode;
    short aeroedge;
    short minloops;
    short maxloops;
    short choke;
    short solver_ID;
    short plastic;
    short springpreload;
    void *scratch;
    float shearstiff;
    float inpush;
    PointCache *pointcache;
    ListBase ptcaches;
    EffectorWeights *effector_weights;
    float lcom[3];
    float lrot[3][3];
    float lscale[3][3];
    int last_frame;
};

struct ImagePaintSettings
{
    Paint paint;
    short flag;
    short missing_data;
    short seam_bleed;
    short normal_angle;
    short screen_grab_size[2];
    int mode;
    void *paintcursor;
    Image *stencil;
    Image *clone;
    Image *canvas;
    float stencil_col[3];
    float pad1;
};

struct ParticleEditSettings
{
    short flag;
    short totrekey;
    short totaddkey;
    short brushtype;
    ParticleBrushData brush[7];
    void *paintcursor;
    float emitterdist;
    float rt;
    int selectmode;
    int edittype;
    int draw_step;
    int fade_frames;
    Scene *scene;
    Object *object;
};

struct Sculpt
{
    Paint paint;
    int flags;
    int radial_symm[3];
    float detail_size;
    int symmetrize_direction;
    float gravity_factor;
    float constant_detail;
    Object *gravity_object;
    void *pad2;
};

struct UvSculpt
{
    Paint paint;
};

struct VPaint
{
    Paint paint;
    short flag;
    short pad;
    int tot;
    int *vpaint_prev;
    MDeformVert *wpaint_prev;
    void *paintcursor;
};

struct SpaceLink
{
    SpaceLink *next;
    SpaceLink *prev;
    ListBase regionbase;
    int spacetype;
    float blockscale;
    short blockhandler[8];
};

struct SpaceInfo
{
    SpaceLink *next;
    SpaceLink *prev;
    ListBase regionbase;
    int spacetype;
    float blockscale;
    short blockhandler[8];
    char rpt_mask;
    char pad[7];
};

struct SpaceFile
{
    SpaceLink *next;
    SpaceLink *prev;
    ListBase regionbase;
    int spacetype;
    int scroll_offset;
    FileSelectParams *params;
    void *files;
    ListBase *folders_prev;
    ListBase *folders_next;
    wmOperator *op;
    void *smoothscroll_timer;
    void *layout;
    short recentnr;
    short bookmarknr;
    short systemnr;
    short pad2;
};

struct Script
{
    ID id;
    void *py_draw;
    void *py_event;
    void *py_button;
    void *py_browsercallback;
    void *py_globaldict;
    int flags;
    int lastspace;
    char scriptname[1024];
    char scriptarg[256];
};

struct SpaceScript
{
    SpaceLink *next;
    SpaceLink *prev;
    ListBase regionbase;
    int spacetype;
    float blockscale;
    Script *script;
    short flags;
    short menunr;
    int pad1;
    void *but_refs;
};

struct bNodeTreePath
{
    bNodeTreePath *next;
    bNodeTreePath *prev;
    bNodeTree *nodetree;
    bNodeInstanceKey parent_key;
    int pad;
    float view_center[2];
    char node_name[64];
};

struct SpaceLogic
{
    SpaceLink *next;
    SpaceLink *prev;
    ListBase regionbase;
    int spacetype;
    float blockscale;
    short blockhandler[8];
    short flag;
    short scaflag;
    int pad;
    bGPdata *gpd;
};

struct SpaceUserPref
{
    SpaceLink *next;
    SpaceLink *prev;
    ListBase regionbase;
    int spacetype;
    char pad[3];
    char filter_type;
    char filter[64];
};

struct ScrVert
{
    ScrVert *next;
    ScrVert *prev;
    ScrVert *newv;
    vec2s vec;
    short flag;
    short editflag;
};

struct Strip
{
    Strip *next;
    Strip *prev;
    int us;
    int done;
    int startstill;
    int endstill;
    StripElem *stripdata;
    char dir[768];
    StripProxy *proxy;
    StripCrop *crop;
    StripTransform *transform;
    StripColorBalance *color_balance;
    ColorManagedColorspaceSettings colorspace_settings;
};

struct BrightContrastModifierData
{
    SequenceModifierData modifier;
    float bright;
    float contrast;
};

struct SequencerMaskModifierData
{
    SequenceModifierData modifier;
};

struct bSoundActuator
{
    short flag;
    short sndnr;
    int pad1;
    int pad2;
    short pad3[2];
    float volume;
    float pitch;
    bSound *sound;
    Sound3D sound3D;
    short type;
    short pad4;
    short pad5;
    short pad6[1];
};

struct bSound
{
    ID id;
    char name[1024];
    PackedFile *packedfile;
    void *handle;
    PackedFile *newpackedfile;
    Ipo *ipo;
    float volume;
    float attenuation;
    float pitch;
    float min_gain;
    float max_gain;
    float distance;
    int flags;
    int pad;
    void *cache;
    void *waveform;
    void *playback_handle;
    void *mutex;
};

struct Bone
{
    Bone *next;
    Bone *prev;
    IDProperty *prop;
    Bone *parent;
    ListBase childbase;
    char name[64];
    float roll;
    float head[3];
    float tail[3];
    float bone_mat[3][3];
    int flag;
    float arm_head[3];
    float arm_tail[3];
    float arm_mat[4][4];
    float arm_roll;
    float dist;
    float weight;
    float xwidth;
    float length;
    float zwidth;
    float ease1;
    float ease2;
    float rad_head;
    float rad_tail;
    float size[3];
    int layer;
    short segments;
    short pad[1];
};

struct bDopeSheet
{
    ID *source;
    ListBase chanbase;
    Group *filter_grp;
    char searchstr[64];
    int filterflag;
    int flag;
    int renameIndex;
    int pad;
};

struct bActionChannel
{
    bActionChannel *next;
    bActionChannel *prev;
    bActionGroup *grp;
    Ipo *ipo;
    ListBase constraintChannels;
    int flag;
    char name[64];
    int temp;
};

struct bPythonConstraint
{
    Text *text;
    IDProperty *prop;
    int flag;
    int tarnum;
    ListBase targets;
    Object *tar;
    char subtarget[64];
};

struct bActionStrip
{
    bActionStrip *next;
    bActionStrip *prev;
    short flag;
    short mode;
    short stride_axis;
    short curmod;
    Ipo *ipo;
    bAction *act;
    Object *object;
    float start;
    float end;
    float actstart;
    float actend;
    float actoffs;
    float stridelen;
    float repeat;
    float scale;
    float blendin;
    float blendout;
    char stridechannel[32];
    char offs_bone[32];
    ListBase modifiers;
};

struct bNodeSocket
{
    bNodeSocket *next;
    bNodeSocket *prev;
    bNodeSocket *new_sock;
    IDProperty *prop;
    char identifier[64];
    char name[64];
    void *storage;
    short type;
    short flag;
    short limit;
    short in_out;
    void *typeinfo;
    char idname[64];
    float locx;
    float locy;
    void *default_value;
    short stack_index;
    short stack_type;
    int pad;
    void *cache;
    int own_index;
    int to_index;
    bNodeSocket *groupsock;
    bNodeLink *link;
    bNodeStack ns;
};

struct bNodeInstanceHashEntry
{
    bNodeInstanceKey key;
    short tag;
    short pad;
};

struct Scopes
{
    int ok;
    int sample_full;
    int sample_lines;
    float accuracy;
    int wavefrm_mode;
    float wavefrm_alpha;
    float wavefrm_yfac;
    int wavefrm_height;
    float vecscope_alpha;
    int vecscope_height;
    float minmax[3][2];
    Histogram hist;
    float *waveform_1;
    float *waveform_2;
    float *waveform_3;
    float *vecscope;
    int waveform_tot;
    int pad;
};

struct PaintCurvePoint
{
    BezTriple bez;
    float pressure;
};

struct PaintCurve
{
    ID id;
    PaintCurvePoint *points;
    int tot_points;
    int add_index;
};

struct BoidParticle
{
    Object *ground;
    BoidData data;
    float gravity[3];
    float wander[3];
    float rt;
};

struct bGPDframe
{
    bGPDframe *next;
    bGPDframe *prev;
    ListBase strokes;
    int framenum;
    short flag;
    short key_type;
};

struct bGPDlayer
{
    bGPDlayer *next;
    bGPDlayer *prev;
    ListBase frames;
    bGPDframe *actframe;
    short flag;
    short thickness;
    short gstep;
    short gstep_next;
    float gcolor_prev[3];
    float gcolor_next[3];
    float color[4];
    float fill[4];
    char info[128];
};

struct ReportList
{
    ListBase list;
    int printlevel;
    int storelevel;
    int flag;
    int pad;
    void *reporttimer;
};

struct wmKeyConfig
{
    wmKeyConfig *next;
    wmKeyConfig *prev;
    char idname[64];
    char basename[64];
    ListBase keymaps;
    int actkeymap;
    int flag;
};

struct wmOperator
{
    wmOperator *next;
    wmOperator *prev;
    char idname[64];
    IDProperty *properties;
    void *type;
    void *customdata;
    void *py_instance;
    void *ptr;
    ReportList *reports;
    ListBase macro;
    wmOperator *opm;
    void *layout;
    short flag;
    short pad[3];
};

struct FMod_Limits
{
    rctf rect;
    int flag;
    int pad;
};

struct DriverVar
{
    DriverVar *next;
    DriverVar *prev;
    char name[64];
    DriverTarget targets[8];
    short num_targets;
    short type;
    float curval;
};

struct ChannelDriver
{
    ListBase variables;
    char expression[256];
    void *expr_comp;
    float curval;
    float influence;
    int type;
    int flag;
};

struct FCurve
{
    FCurve *next;
    FCurve *prev;
    bActionGroup *grp;
    ChannelDriver *driver;
    ListBase modifiers;
    BezTriple *bezt;
    FPoint *fpt;
    int totvert;
    float curval;
    short flag;
    short extend;
    int array_index;
    char *rna_path;
    int color_mode;
    float color[3];
    float prev_norm_factor;
    float pad;
};

struct AnimMapper
{
    AnimMapper *next;
    AnimMapper *prev;
    bAction *target;
    ListBase mappings;
};

struct NlaTrack
{
    NlaTrack *next;
    NlaTrack *prev;
    ListBase strips;
    int flag;
    int index;
    char name[64];
};

struct KeyingSet
{
    KeyingSet *next;
    KeyingSet *prev;
    ListBase paths;
    char idname[64];
    char name[64];
    char description[240];
    char typeinfo[64];
    short flag;
    short keyingflag;
    int active_path;
};

struct IdAdtTemplate
{
    ID id;
    AnimData *adt;
};

struct BoidRuleGoalAvoid
{
    BoidRule rule;
    Object *ob;
    int options;
    float fear_factor;
    int signal_id;
    int channels;
};

struct BoidRuleAvoidCollision
{
    BoidRule rule;
    int options;
    float look_ahead;
};

struct BoidRuleFollowLeader
{
    BoidRule rule;
    Object *ob;
    float loc[3];
    float oloc[3];
    float cfra;
    float distance;
    int options;
    int queue_size;
};

struct BoidRuleAverageSpeed
{
    BoidRule rule;
    float wander;
    float level;
    float speed;
    float rt;
};

struct BoidRuleFight
{
    BoidRule rule;
    float distance;
    float flee_distance;
};

struct BoidSettings
{
    int options;
    int last_state_id;
    float landing_smoothness;
    float height;
    float banking;
    float pitch;
    float health;
    float aggression;
    float strength;
    float accuracy;
    float range;
    float air_min_speed;
    float air_max_speed;
    float air_max_acc;
    float air_max_ave;
    float air_personal_space;
    float land_jump_speed;
    float land_max_speed;
    float land_max_acc;
    float land_max_ave;
    float land_personal_space;
    float land_stick_force;
    ListBase states;
};

struct SmokeDomainSettings
{
    SmokeModifierData *smd;
    void *fluid;
    void *fluid_mutex;
    Group *fluid_group;
    Group *eff_group;
    Group *coll_group;
    void *wt;
    void *tex;
    void *tex_wt;
    void *tex_shadow;
    void *tex_flame;
    float *shadow;
    float p0[3];
    float p1[3];
    float dp0[3];
    float cell_size[3];
    float global_size[3];
    float prev_loc[3];
    int shift[3];
    float shift_f[3];
    float obj_shift_f[3];
    float imat[4][4];
    float obmat[4][4];
    int base_res[3];
    int res_min[3];
    int res_max[3];
    int res[3];
    int total_cells;
    float dx;
    float scale;
    int adapt_margin;
    int adapt_res;
    float adapt_threshold;
    float alpha;
    float beta;
    int amplify;
    int maxres;
    int flags;
    int viewsettings;
    short noise;
    short diss_percent;
    int diss_speed;
    float strength;
    int res_wt[3];
    float dx_wt;
    int cache_comp;
    int cache_high_comp;
    PointCache *point_cache[2];
    ListBase ptcaches[2];
    EffectorWeights *effector_weights;
    int border_collisions;
    float time_scale;
    float vorticity;
    int active_fields;
    float active_color[3];
    int highres_sampling;
    float burning_rate;
    float flame_smoke;
    float flame_vorticity;
    float flame_ignition;
    float flame_max_temp;
    float flame_smoke_color[3];
};

struct Speaker
{
    ID id;
    AnimData *adt;
    bSound *sound;
    float volume_max;
    float volume_min;
    float distance_max;
    float distance_reference;
    float attenuation;
    float cone_angle_outer;
    float cone_angle_inner;
    float cone_volume_outer;
    float volume;
    float pitch;
    short flag;
    short pad1[3];
};

struct MovieClipScopes
{
    short ok;
    short use_track_mask;
    int track_preview_height;
    int frame_width;
    int frame_height;
    MovieTrackingMarker undist_marker;
    void *track_search;
    void *track_preview;
    float track_pos[2];
    short track_disabled;
    short track_locked;
    int framenr;
    MovieTrackingTrack *track;
    MovieTrackingMarker *marker;
    float slide_scale[2];
};

struct DynamicPaintSurface
{
    DynamicPaintSurface *next;
    DynamicPaintSurface *prev;
    DynamicPaintCanvasSettings *canvas;
    void *data;
    Group *brush_group;
    EffectorWeights *effector_weights;
    PointCache *pointcache;
    ListBase ptcaches;
    int current_frame;
    char name[64];
    short format;
    short type;
    short disp_type;
    short image_fileformat;
    short effect_ui;
    short preview_id;
    short init_color_type;
    short pad_s;
    int flags;
    int effect;
    int image_resolution;
    int substeps;
    int start_frame;
    int end_frame;
    int pad;
    float init_color[4];
    Tex *init_texture;
    char init_layername[64];
    int dry_speed;
    int diss_speed;
    float color_dry_threshold;
    float depth_clamp;
    float disp_factor;
    float spread_speed;
    float color_spread_speed;
    float shrink_speed;
    float drip_vel;
    float drip_acc;
    float influence_scale;
    float radius_scale;
    float wave_damping;
    float wave_speed;
    float wave_timescale;
    float wave_spring;
    float wave_smoothness;
    int pad2;
    char uvlayer_name[64];
    char image_output_path[1024];
    char output_name[64];
    char output_name2[64];
};

struct DynamicPaintCanvasSettings
{
    DynamicPaintModifierData *pmd;
    void *dm;
    ListBase surfaces;
    short active_sur;
    short flags;
    int pad;
    char error[64];
};

struct MaskSpline
{
    MaskSpline *next;
    MaskSpline *prev;
    short flag;
    char offset_mode;
    char weight_interp;
    int tot_point;
    MaskSplinePoint *points;
    MaskParent parent;
    MaskSplinePoint *points_deform;
};

struct RigidBodyWorld
{
    EffectorWeights *effector_weights;
    Group *group;
    Object **objects;
    Group *constraints;
    int pad;
    float ltime;
    PointCache *pointcache;
    ListBase ptcaches;
    int numbodies;
    short steps_per_second;
    short num_solver_iterations;
    int flag;
    float time_scale;
    void *physics_world;
};

struct LineStyleColorModifier_AlongStroke
{
    LineStyleModifier modifier;
    ColorBand *color_ramp;
};

struct LineStyleAlphaModifier_AlongStroke
{
    LineStyleModifier modifier;
    CurveMapping *curve;
    int flags;
    int pad;
};

struct LineStyleThicknessModifier_AlongStroke
{
    LineStyleModifier modifier;
    CurveMapping *curve;
    int flags;
    float value_min;
    float value_max;
    int pad;
};

struct LineStyleColorModifier_DistanceFromCamera
{
    LineStyleModifier modifier;
    ColorBand *color_ramp;
    float range_min;
    float range_max;
};

struct LineStyleAlphaModifier_DistanceFromCamera
{
    LineStyleModifier modifier;
    CurveMapping *curve;
    int flags;
    float range_min;
    float range_max;
    int pad;
};

struct LineStyleThicknessModifier_DistanceFromCamera
{
    LineStyleModifier modifier;
    CurveMapping *curve;
    int flags;
    float range_min;
    float range_max;
    float value_min;
    float value_max;
    int pad;
};

struct LineStyleColorModifier_DistanceFromObject
{
    LineStyleModifier modifier;
    Object *target;
    ColorBand *color_ramp;
    float range_min;
    float range_max;
};

struct LineStyleAlphaModifier_DistanceFromObject
{
    LineStyleModifier modifier;
    Object *target;
    CurveMapping *curve;
    int flags;
    float range_min;
    float range_max;
    int pad;
};

struct LineStyleThicknessModifier_DistanceFromObject
{
    LineStyleModifier modifier;
    Object *target;
    CurveMapping *curve;
    int flags;
    float range_min;
    float range_max;
    float value_min;
    float value_max;
    int pad;
};

struct LineStyleColorModifier_Material
{
    LineStyleModifier modifier;
    ColorBand *color_ramp;
    int flags;
    int mat_attr;
};

struct LineStyleAlphaModifier_Material
{
    LineStyleModifier modifier;
    CurveMapping *curve;
    int flags;
    int mat_attr;
};

struct LineStyleThicknessModifier_Material
{
    LineStyleModifier modifier;
    CurveMapping *curve;
    int flags;
    float value_min;
    float value_max;
    int mat_attr;
};

struct LineStyleGeometryModifier_Sampling
{
    LineStyleModifier modifier;
    float sampling;
    int pad;
};

struct LineStyleGeometryModifier_BezierCurve
{
    LineStyleModifier modifier;
    float error;
    int pad;
};

struct LineStyleGeometryModifier_SinusDisplacement
{
    LineStyleModifier modifier;
    float wavelength;
    float amplitude;
    float phase;
    int pad;
};

struct LineStyleGeometryModifier_SpatialNoise
{
    LineStyleModifier modifier;
    float amplitude;
    float scale;
    int octaves;
    int flags;
};

struct LineStyleGeometryModifier_PerlinNoise1D
{
    LineStyleModifier modifier;
    float frequency;
    float amplitude;
    float angle;
    int octaves;
    int seed;
    int pad1;
};

struct LineStyleGeometryModifier_PerlinNoise2D
{
    LineStyleModifier modifier;
    float frequency;
    float amplitude;
    float angle;
    int octaves;
    int seed;
    int pad1;
};

struct LineStyleGeometryModifier_BackboneStretcher
{
    LineStyleModifier modifier;
    float backbone_length;
    int pad;
};

struct LineStyleGeometryModifier_TipRemover
{
    LineStyleModifier modifier;
    float tip_length;
    int pad;
};

struct LineStyleGeometryModifier_Polygonalization
{
    LineStyleModifier modifier;
    float error;
    int pad;
};

struct LineStyleGeometryModifier_GuidingLines
{
    LineStyleModifier modifier;
    float offset;
    int pad;
};

struct LineStyleGeometryModifier_Blueprint
{
    LineStyleModifier modifier;
    int flags;
    int rounds;
    float backbone_length;
    int random_radius;
    int random_center;
    int random_backbone;
};

struct LineStyleGeometryModifier_2DOffset
{
    LineStyleModifier modifier;
    float start;
    float end;
    float x;
    float y;
};

struct LineStyleGeometryModifier_2DTransform
{
    LineStyleModifier modifier;
    int pivot;
    float scale_x;
    float scale_y;
    float angle;
    float pivot_u;
    float pivot_x;
    float pivot_y;
    int pad;
};

struct LineStyleThicknessModifier_Calligraphy
{
    LineStyleModifier modifier;
    float min_thickness;
    float max_thickness;
    float orientation;
    int pad;
};

struct IDProperty
{
    IDProperty *next;
    IDProperty *prev;
    char type;
    char subtype;
    short flag;
    char name[64];
    int saved;
    IDPropertyData data;
    int len;
    int totallen;
};

struct IpoCurve
{
    IpoCurve *next;
    IpoCurve *prev;
    BPoint *bp;
    BezTriple *bezt;
    rctf maxrct;
    rctf totrct;
    short blocktype;
    short adrcode;
    short vartype;
    short totvert;
    short ipo;
    short extrap;
    short flag;
    short rt;
    float ymin;
    float ymax;
    int bitmask;
    float slide_min;
    float slide_max;
    float curval;
    IpoDriver *driver;
};

struct Key
{
    ID id;
    AnimData *adt;
    KeyBlock *refkey;
    char elemstr[32];
    int elemsize;
    int pad;
    ListBase block;
    Ipo *ipo;
    ID *from;
    short type;
    short totkey;
    short slurph;
    short flag;
    float ctime;
    int uidgen;
};

struct Text
{
    ID id;
    char *name;
    int flags;
    int nlines;
    ListBase lines;
    TextLine *curl;
    TextLine *sell;
    int curc;
    int selc;
    char *undo_buf;
    int undo_pos;
    int undo_len;
    void *compiled;
    double mtime;
};

struct Tex
{
    ID id;
    AnimData *adt;
    float noisesize;
    float turbul;
    float bright;
    float contrast;
    float saturation;
    float rfac;
    float gfac;
    float bfac;
    float filtersize;
    float pad2;
    float mg_H;
    float mg_lacunarity;
    float mg_octaves;
    float mg_offset;
    float mg_gain;
    float dist_amount;
    float ns_outscale;
    float vn_w1;
    float vn_w2;
    float vn_w3;
    float vn_w4;
    float vn_mexp;
    short vn_distm;
    short vn_coltype;
    short noisedepth;
    short noisetype;
    short noisebasis;
    short noisebasis2;
    short imaflag;
    short flag;
    short type;
    short stype;
    float cropxmin;
    float cropymin;
    float cropxmax;
    float cropymax;
    int texfilter;
    int afmax;
    short xrepeat;
    short yrepeat;
    short extend;
    short fie_ima;
    int len;
    int frames;
    int offset;
    int sfra;
    float checkerdist;
    float nabla;
    float pad1;
    ImageUser iuser;
    bNodeTree *nodetree;
    Ipo *ipo;
    Image *ima;
    ColorBand *coba;
    EnvMap *env;
    PreviewImage *preview;
    PointDensity *pd;
    VoxelData *vd;
    OceanTex *ot;
    char use_nodes;
    char pad[7];
};

struct ColorMapping
{
    ColorBand coba;
    float bright;
    float contrast;
    float saturation;
    int flag;
    float blend_color[3];
    float blend_factor;
    int blend_type;
    int pad[3];
};

struct ClothModifierData
{
    ModifierData modifier;
    Scene *scene;
    void *clothObject;
    ClothSimSettings *sim_parms;
    ClothCollSettings *coll_parms;
    PointCache *point_cache;
    ListBase ptcaches;
};

struct World
{
    ID id;
    AnimData *adt;
    short colormodel;
    short totex;
    short texact;
    short mistype;
    float horr;
    float horg;
    float horb;
    float zenr;
    float zeng;
    float zenb;
    float ambr;
    float ambg;
    float ambb;
    float exposure;
    float exp;
    float range;
    float linfac;
    float logfac;
    float gravity;
    float activityBoxRadius;
    short skytype;
    short mode;
    short occlusionRes;
    short physicsEngine;
    short ticrate;
    short maxlogicstep;
    short physubstep;
    short maxphystep;
    float misi;
    float miststa;
    float mistdist;
    float misthi;
    float starr;
    float starg;
    float starb;
    float stark;
    float starsize;
    float starmindist;
    float stardist;
    float starcolnoise;
    short dofsta;
    short dofend;
    short dofmin;
    short dofmax;
    float aodist;
    float aodistfac;
    float aoenergy;
    float aobias;
    short aomode;
    short aosamp;
    short aomix;
    short aocolor;
    float ao_adapt_thresh;
    float ao_adapt_speed_fac;
    float ao_approx_error;
    float ao_approx_correction;
    float ao_indirect_energy;
    float ao_env_energy;
    float ao_pad2;
    short ao_indirect_bounces;
    short ao_pad;
    short ao_samp_method;
    short ao_gather_method;
    short ao_approx_passes;
    short flag;
    float *aosphere;
    float *aotables;
    Ipo *ipo;
    MTex *mtex[18];
    short pr_texture;
    short use_nodes;
    short pad[2];
    PreviewImage *preview;
    bNodeTree *nodetree;
    ListBase gpumaterial;
};

struct ImageFormatData
{
    char imtype;
    char depth;
    char planes;
    char flag;
    char quality;
    char compress;
    char exr_codec;
    char cineon_flag;
    short cineon_white;
    short cineon_black;
    float cineon_gamma;
    char jp2_flag;
    char jp2_codec;
    char pad[6];
    ColorManagedViewSettings view_settings;
    ColorManagedDisplaySettings display_settings;
};

struct BGpic
{
    BGpic *next;
    BGpic *prev;
    Image *ima;
    ImageUser iuser;
    MovieClip *clip;
    MovieClipUser cuser;
    float xof;
    float yof;
    float size;
    float blend;
    short view;
    short flag;
    short source;
    short pad;
};

struct ThemeSpace
{
    char back[4];
    char title[4];
    char text[4];
    char text_hi[4];
    char header[4];
    char header_title[4];
    char header_text[4];
    char header_text_hi[4];
    char tab_active[4];
    char tab_inactive[4];
    char tab_back[4];
    char tab_outline[4];
    char button[4];
    char button_title[4];
    char button_text[4];
    char button_text_hi[4];
    char list[4];
    char list_title[4];
    char list_text[4];
    char list_text_hi[4];
    uiPanelColors panelcolors;
    uiGradientColors gradients;
    char shade1[4];
    char shade2[4];
    char hilite[4];
    char grid[4];
    char view_overlay[4];
    char wire[4];
    char wire_edit[4];
    char select[4];
    char lamp[4];
    char speaker[4];
    char empty[4];
    char camera[4];
    char active[4];
    char group[4];
    char group_active[4];
    char transform[4];
    char vertex[4];
    char vertex_select[4];
    char vertex_unreferenced[4];
    char edge[4];
    char edge_select[4];
    char edge_seam[4];
    char edge_sharp[4];
    char edge_facesel[4];
    char edge_crease[4];
    char face[4];
    char face_select[4];
    char face_dot[4];
    char extra_edge_len[4];
    char extra_edge_angle[4];
    char extra_face_angle[4];
    char extra_face_area[4];
    char normal[4];
    char vertex_normal[4];
    char loop_normal[4];
    char bone_solid[4];
    char bone_pose[4];
    char bone_pose_active[4];
    char strip[4];
    char strip_select[4];
    char cframe[4];
    char freestyle_edge_mark[4];
    char freestyle_face_mark[4];
    char nurb_uline[4];
    char nurb_vline[4];
    char act_spline[4];
    char nurb_sel_uline[4];
    char nurb_sel_vline[4];
    char lastsel_point[4];
    char handle_free[4];
    char handle_auto[4];
    char handle_vect[4];
    char handle_align[4];
    char handle_auto_clamped[4];
    char handle_sel_free[4];
    char handle_sel_auto[4];
    char handle_sel_vect[4];
    char handle_sel_align[4];
    char handle_sel_auto_clamped[4];
    char ds_channel[4];
    char ds_subchannel[4];
    char keytype_keyframe[4];
    char keytype_extreme[4];
    char keytype_breakdown[4];
    char keytype_jitter[4];
    char keytype_keyframe_select[4];
    char keytype_extreme_select[4];
    char keytype_breakdown_select[4];
    char keytype_jitter_select[4];
    char keyborder[4];
    char keyborder_select[4];
    char console_output[4];
    char console_input[4];
    char console_info[4];
    char console_error[4];
    char console_cursor[4];
    char console_select[4];
    char vertex_size;
    char outline_width;
    char facedot_size;
    char noodle_curving;
    char syntaxl[4];
    char syntaxs[4];
    char syntaxb[4];
    char syntaxn[4];
    char syntaxv[4];
    char syntaxc[4];
    char syntaxd[4];
    char syntaxr[4];
    char nodeclass_output[4];
    char nodeclass_filter[4];
    char nodeclass_vector[4];
    char nodeclass_texture[4];
    char nodeclass_shader[4];
    char nodeclass_script[4];
    char nodeclass_pattern[4];
    char nodeclass_layout[4];
    char movie[4];
    char movieclip[4];
    char mask[4];
    char image[4];
    char scene[4];
    char audio[4];
    char effect[4];
    char transition[4];
    char meta[4];
    char editmesh_active[4];
    char handle_vertex[4];
    char handle_vertex_select[4];
    char pad2[4];
    char handle_vertex_size;
    char marker_outline[4];
    char marker[4];
    char act_marker[4];
    char sel_marker[4];
    char dis_marker[4];
    char lock_marker[4];
    char bundle_solid[4];
    char path_before[4];
    char path_after[4];
    char camera_path[4];
    char hpad[3];
    char preview_back[4];
    char preview_stitch_face[4];
    char preview_stitch_edge[4];
    char preview_stitch_vert[4];
    char preview_stitch_stitchable[4];
    char preview_stitch_unstitchable[4];
    char preview_stitch_active[4];
    char uv_shadow[4];
    char uv_others[4];
    char match[4];
    char selected_highlight[4];
    char skin_root[4];
    char anim_active[4];
    char anim_non_active[4];
    char nla_tweaking[4];
    char nla_tweakdupli[4];
    char nla_transition[4];
    char nla_transition_sel[4];
    char nla_meta[4];
    char nla_meta_sel[4];
    char nla_sound[4];
    char nla_sound_sel[4];
    char info_selected[4];
    char info_selected_text[4];
    char info_error[4];
    char info_error_text[4];
    char info_warning[4];
    char info_warning_text[4];
    char info_info[4];
    char info_info_text[4];
    char info_debug[4];
    char info_debug_text[4];
    char paint_curve_pivot[4];
    char paint_curve_handle[4];
};

struct Sequence
{
    Sequence *next;
    Sequence *prev;
    void *tmp;
    void *lib;
    char name[64];
    int flag;
    int type;
    int len;
    int start;
    int startofs;
    int endofs;
    int startstill;
    int endstill;
    int machine;
    int depth;
    int startdisp;
    int enddisp;
    float sat;
    float mul;
    float handsize;
    short anim_preseek;
    short streamindex;
    int multicam_source;
    int clip_flag;
    Strip *strip;
    Ipo *ipo;
    Scene *scene;
    Object *scene_camera;
    MovieClip *clip;
    Mask *mask;
    void *anim;
    float effect_fader;
    float speed_fader;
    Sequence *seq1;
    Sequence *seq2;
    Sequence *seq3;
    ListBase seqbase;
    bSound *sound;
    void *scene_sound;
    float volume;
    float pitch;
    float pan;
    float strobe;
    void *effectdata;
    int anim_startofs;
    int anim_endofs;
    int blend_mode;
    float blend_opacity;
    int sfra;
    char alpha_mode;
    char pad[3];
    ListBase modifiers;
};

struct ColorBalanceModifierData
{
    SequenceModifierData modifier;
    StripColorBalance color_balance;
    float color_multiply;
};

struct Group
{
    ID id;
    ListBase gobject;
    int layer;
    float dupli_ofs[3];
};

struct bActionGroup
{
    bActionGroup *next;
    bActionGroup *prev;
    ListBase channels;
    int flag;
    int customCol;
    char name[64];
    ThemeWireColor cs;
};

struct bNodePreview
{
    bNodeInstanceHashEntry hash_entry;
    char *rect;
    short xsize;
    short ysize;
    int pad;
};

struct ParticleData
{
    ParticleKey state;
    ParticleKey prev_state;
    HairKey *hair;
    ParticleKey *keys;
    BoidParticle *boid;
    int totkey;
    float time;
    float lifetime;
    float dietime;
    int num;
    int num_dmcache;
    float fuv[4];
    float foffset;
    float size;
    float sphdensity;
    int pad;
    int hair_index;
    short flag;
    short alive;
};

struct ParticleSettings
{
    ID id;
    AnimData *adt;
    BoidSettings *boids;
    SPHFluidSettings *fluid;
    EffectorWeights *effector_weights;
    int flag;
    int rt;
    short type;
    short from;
    short distr;
    short texact;
    short phystype;
    short rotmode;
    short avemode;
    short reactevent;
    int draw;
    int pad1;
    short draw_as;
    short draw_size;
    short childtype;
    short pad2;
    short ren_as;
    short subframes;
    short draw_col;
    short draw_step;
    short ren_step;
    short hair_step;
    short keys_step;
    short adapt_angle;
    short adapt_pix;
    short disp;
    short omat;
    short interpolation;
    short integrator;
    short rotfrom;
    short kink;
    short kink_axis;
    short bb_align;
    short bb_uv_split;
    short bb_anim;
    short bb_split_offset;
    float bb_tilt;
    float bb_rand_tilt;
    float bb_offset[2];
    float bb_size[2];
    float bb_vel_head;
    float bb_vel_tail;
    float color_vec_max;
    short simplify_flag;
    short simplify_refsize;
    float simplify_rate;
    float simplify_transition;
    float simplify_viewport;
    float sta;
    float end;
    float lifetime;
    float randlife;
    float timetweak;
    float courant_target;
    float jitfac;
    float eff_hair;
    float grid_rand;
    float ps_offset[1];
    int totpart;
    int userjit;
    int grid_res;
    int effector_amount;
    short time_flag;
    short time_pad[3];
    float normfac;
    float obfac;
    float randfac;
    float partfac;
    float tanfac;
    float tanphase;
    float reactfac;
    float ob_vel[3];
    float avefac;
    float phasefac;
    float randrotfac;
    float randphasefac;
    float mass;
    float size;
    float randsize;
    float acc[3];
    float dragfac;
    float brownfac;
    float dampfac;
    float randlength;
    int child_nbr;
    int ren_child_nbr;
    float parents;
    float childsize;
    float childrandsize;
    float childrad;
    float childflat;
    float clumpfac;
    float clumppow;
    float kink_amp;
    float kink_freq;
    float kink_shape;
    float kink_flat;
    float kink_amp_clump;
    float rough1;
    float rough1_size;
    float rough2;
    float rough2_size;
    float rough2_thres;
    float rough_end;
    float rough_end_shape;
    float clength;
    float clength_thres;
    float parting_fac;
    float parting_min;
    float parting_max;
    float branch_thres;
    float draw_line[2];
    float path_start;
    float path_end;
    int trail_count;
    int keyed_loops;
    MTex *mtex[18];
    Group *dup_group;
    ListBase dupliweights;
    Group *eff_group;
    Object *dup_ob;
    Object *bb_ob;
    Ipo *ipo;
    PartDeflect *pd;
    PartDeflect *pd2;
    short use_modifier_stack;
    short pad[3];
};

struct bGPdata
{
    ID id;
    AnimData *adt;
    ListBase layers;
    int flag;
    short sbuffer_size;
    short sbuffer_sflag;
    void *sbuffer;
};

struct wmKeyMap
{
    wmKeyMap *next;
    wmKeyMap *prev;
    ListBase items;
    ListBase diff_items;
    char idname[64];
    short spaceid;
    short regionid;
    short flag;
    short kmi_id;
    int (*poll)();
    void *modal_items;
};

struct MovieTrackingDopesheet
{
    int ok;
    short sort_method;
    short flag;
    ListBase coverage_segments;
    ListBase channels;
    int tot_channel;
    int pad;
};

struct Mask
{
    ID id;
    AnimData *adt;
    ListBase masklayers;
    int masklay_act;
    int masklay_tot;
    int sfra;
    int efra;
    int flag;
    int pad;
};

struct MaskSplinePoint
{
    BezTriple bezt;
    int pad;
    int tot_uw;
    MaskSplinePointUW *uw;
    MaskParent parent;
};

struct MaskLayer
{
    MaskLayer *next;
    MaskLayer *prev;
    char name[64];
    ListBase splines;
    ListBase splines_shapes;
    MaskSpline *act_spline;
    MaskSplinePoint *act_point;
    float alpha;
    char blend;
    char blend_flag;
    char falloff;
    char pad[7];
    char flag;
    char restrictflag;
};

struct FreestyleConfig
{
    ListBase modules;
    int mode;
    int raycasting_algorithm;
    int flags;
    float sphere_radius;
    float dkr_epsilon;
    float crease_angle;
    ListBase linesets;
};

struct Ipo
{
    ID id;
    ListBase curve;
    rctf cur;
    short blocktype;
    short showkey;
    short muteipo;
    short pad;
};

struct Image
{
    ID id;
    char name[1024];
    void *cache;
    void *gputexture;
    void *anim;
    void *rr;
    void *renders[8];
    short render_slot;
    short last_render_slot;
    short ok;
    short flag;
    short source;
    short type;
    int lastframe;
    short tpageflag;
    short totbind;
    short xrep;
    short yrep;
    short twsta;
    short twend;
    int bindcode;
    int *repbind;
    PackedFile *packedfile;
    PreviewImage *preview;
    float lastupdate;
    int lastused;
    short animspeed;
    short pad2;
    int gen_x;
    int gen_y;
    char gen_type;
    char gen_flag;
    short gen_depth;
    float gen_color[4];
    float aspx;
    float aspy;
    ColorManagedColorspaceSettings colorspace_settings;
    char alpha_mode;
    char pad[7];
    RenderSlot render_slots[8];
};

struct MetaBall
{
    ID id;
    AnimData *adt;
    ListBase elems;
    ListBase disp;
    ListBase *editelems;
    Ipo *ipo;
    Material **mat;
    char flag;
    char flag2;
    short totcol;
    short texflag;
    short pad;
    float loc[3];
    float size[3];
    float rot[3];
    float wiresize;
    float rendersize;
    float thresh;
    MetaElem *lastelem;
};

struct Curve
{
    ID id;
    AnimData *adt;
    BoundBox *bb;
    ListBase nurb;
    EditNurb *editnurb;
    Object *bevobj;
    Object *taperobj;
    Object *textoncurve;
    Ipo *ipo;
    Key *key;
    Material **mat;
    float loc[3];
    float size[3];
    float rot[3];
    short type;
    short texflag;
    short drawflag;
    short twist_mode;
    float twist_smooth;
    float smallcaps_scale;
    int pathlen;
    short bevresol;
    short totcol;
    int flag;
    float width;
    float ext1;
    float ext2;
    short resolu;
    short resolv;
    short resolu_ren;
    short resolv_ren;
    int actnu;
    int actvert;
    char pad[4];
    short lines;
    char spacemode;
    char pad1;
    float spacing;
    float linedist;
    float shear;
    float fsize;
    float wordspace;
    float ulpos;
    float ulheight;
    float xof;
    float yof;
    float linewidth;
    int pos;
    int selstart;
    int selend;
    int len_wchar;
    int len;
    char *str;
    void *editfont;
    char family[64];
    VFont *vfont;
    VFont *vfontb;
    VFont *vfonti;
    VFont *vfontbi;
    TextBox *tb;
    int totbox;
    int actbox;
    CharInfo *strinfo;
    CharInfo curinfo;
    float ctime;
    float bevfac1;
    float bevfac2;
    char bevfac1_mapping;
    char bevfac2_mapping;
    char pad2[2];
};

struct Multires
{
    ListBase levels;
    MVert *verts;
    char level_count;
    char current;
    char newlvl;
    char edgelvl;
    char pinlvl;
    char renderlvl;
    char use_col;
    char flag;
    CustomData vdata;
    CustomData fdata;
    short *edge_flags;
    char *edge_creases;
};

struct SceneRenderLayer
{
    SceneRenderLayer *next;
    SceneRenderLayer *prev;
    char name[64];
    Material *mat_override;
    Group *light_override;
    int lay;
    int lay_zmask;
    int lay_exclude;
    int layflag;
    int passflag;
    int pass_xor;
    int samples;
    float pass_alpha_threshold;
    FreestyleConfig freestyleConfig;
};

struct BakeData
{
    ImageFormatData im_format;
    char filepath[1024];
    short width;
    short height;
    short margin;
    short flag;
    float cage_extrusion;
    float pad2;
    char normal_swizzle[3];
    char normal_space;
    char save_mode;
    char pad[3];
    char cage[64];
};

struct GameData
{
    GameFraming framing;
    short playerflag;
    short xplay;
    short yplay;
    short freqplay;
    short depth;
    short attrib;
    short rt1;
    short rt2;
    short aasamples;
    short pad4[3];
    GameDome dome;
    short stereoflag;
    short stereomode;
    float eyeseparation;
    RecastData recastData;
    float gravity;
    float activityBoxRadius;
    int flag;
    short mode;
    short matmode;
    short occlusionRes;
    short physicsEngine;
    short exitkey;
    short vsync;
    short ticrate;
    short maxlogicstep;
    short physubstep;
    short maxphystep;
    short obstacleSimulation;
    short raster_storage;
    float levelHeight;
    float deactivationtime;
    float lineardeactthreshold;
    float angulardeactthreshold;
    float pad2;
};

struct SpaceText
{
    SpaceLink *next;
    SpaceLink *prev;
    ListBase regionbase;
    int spacetype;
    float blockscale;
    short blockhandler[8];
    Text *text;
    int top;
    int viewlines;
    short flags;
    short menunr;
    short lheight;
    char cwidth;
    char linenrs_tot;
    int left;
    int showlinenrs;
    int tabnumber;
    short showsyntax;
    short line_hlight;
    short overwrite;
    short live_edit;
    float pix_per_line;
    rcti txtscroll;
    rcti txtbar;
    int wordwrap;
    int doplugins;
    char findstr[256];
    char replacestr[256];
    short margin_column;
    short lheight_dpi;
    char pad[4];
    void *drawcache;
    float scroll_accum[2];
};

struct SpaceConsole
{
    SpaceLink *next;
    SpaceLink *prev;
    ListBase regionbase;
    int spacetype;
    float blockscale;
    short blockhandler[8];
    int lheight;
    int pad;
    ListBase scrollback;
    ListBase history;
    char prompt[256];
    char language[32];
    int sel_start;
    int sel_end;
};

struct Editing
{
    ListBase *seqbasep;
    ListBase seqbase;
    ListBase metastack;
    Sequence *act_seq;
    char act_imagedir[1024];
    char act_sounddir[1024];
    int over_ofs;
    int over_cfra;
    int over_flag;
    int pad;
    rctf over_border;
};

struct bArmature
{
    ID id;
    AnimData *adt;
    ListBase bonebase;
    ListBase chainbase;
    ListBase *edbo;
    Bone *act_bone;
    void *act_edbone;
    void *sketch;
    int flag;
    int drawtype;
    int gevertdeformer;
    int pad;
    short deformflag;
    short pathflag;
    int layer_used;
    int layer;
    int layer_protected;
    short ghostep;
    short ghostsize;
    short ghosttype;
    short pathsize;
    int ghostsf;
    int ghostef;
    int pathsf;
    int pathef;
    int pathbc;
    int pathac;
};

struct bPoseChannel
{
    bPoseChannel *next;
    bPoseChannel *prev;
    IDProperty *prop;
    ListBase constraints;
    char name[64];
    short flag;
    short ikflag;
    short protectflag;
    short agrp_index;
    char constflag;
    char selectflag;
    char pad0[6];
    Bone *bone;
    bPoseChannel *parent;
    bPoseChannel *child;
    ListBase iktree;
    ListBase siktree;
    bMotionPath *mpath;
    Object *custom;
    bPoseChannel *custom_tx;
    float loc[3];
    float size[3];
    float eul[3];
    float quat[4];
    float rotAxis[3];
    float rotAngle;
    short rotmode;
    short pad;
    float chan_mat[4][4];
    float pose_mat[4][4];
    float constinv[4][4];
    float pose_head[3];
    float pose_tail[3];
    float limitmin[3];
    float limitmax[3];
    float stiffness[3];
    float ikstretch;
    float ikrotweight;
    float iklinweight;
    void *temp;
};

struct bPose
{
    ListBase chanbase;
    void *chanhash;
    short flag;
    short pad;
    int proxy_layer;
    int pad1;
    float ctime;
    float stride_offset[3];
    float cyclic_offset[3];
    ListBase agroups;
    int active_group;
    int iksolver;
    void *ikdata;
    void *ikparam;
    bAnimVizSettings avs;
    char proxy_act_bone[64];
};

struct NodeImageFile
{
    char name[1024];
    ImageFormatData im_format;
    int sfra;
    int efra;
};

struct NodeImageMultiFile
{
    char base_path[1024];
    ImageFormatData format;
    int sfra;
    int efra;
    int active_input;
    int pad;
};

struct NodeImageMultiFileSocket
{
    short use_render_format;
    short use_node_format;
    int pad1;
    char path[1024];
    ImageFormatData format;
    char layer[30];
    char pad2[2];
};

struct CurveMapping
{
    int flag;
    int cur;
    int preset;
    int changed_timestamp;
    rctf curr;
    rctf clipr;
    CurveMap cm[4];
    float black[3];
    float white[3];
    float bwmul[3];
    float sample[3];
};

struct Palette
{
    ID id;
    ListBase colors;
    ListBase deleted;
    int active_color;
    int pad;
};

struct NlaStrip
{
    NlaStrip *next;
    NlaStrip *prev;
    ListBase strips;
    bAction *act;
    AnimMapper *remap;
    ListBase fcurves;
    ListBase modifiers;
    char name[64];
    float influence;
    float strip_time;
    float start;
    float end;
    float actstart;
    float actend;
    float repeat;
    float scale;
    float blendin;
    float blendout;
    short blendmode;
    short extendmode;
    short pad1;
    short type;
    void *speaker_handle;
    int flag;
    int pad2;
};

struct AnimData
{
    bAction *action;
    bAction *tmpact;
    AnimMapper *remap;
    ListBase nla_tracks;
    NlaStrip *actstrip;
    ListBase drivers;
    ListBase overrides;
    int flag;
    int recalc;
    short act_blendmode;
    short act_extendmode;
    float act_influence;
};

struct BoidState
{
    BoidState *next;
    BoidState *prev;
    ListBase rules;
    ListBase conditions;
    ListBase actions;
    char name[32];
    int id;
    int flag;
    int ruleset_type;
    float rule_fuzziness;
    int signal_id;
    int channels;
    float volume;
    float falloff;
};

struct MovieTrackingObject
{
    MovieTrackingObject *next;
    MovieTrackingObject *prev;
    char name[64];
    int flag;
    float scale;
    ListBase tracks;
    ListBase plane_tracks;
    MovieTrackingReconstruction reconstruction;
    int keyframe1;
    int keyframe2;
};

struct Material
{
    ID id;
    AnimData *adt;
    short material_type;
    short flag;
    float r;
    float g;
    float b;
    float specr;
    float specg;
    float specb;
    float mirr;
    float mirg;
    float mirb;
    float ambr;
    float ambb;
    float ambg;
    float amb;
    float emit;
    float ang;
    float spectra;
    float ray_mirror;
    float alpha;
    float ref;
    float spec;
    float zoffs;
    float add;
    float translucency;
    VolumeSettings vol;
    GameSettings game;
    float fresnel_mir;
    float fresnel_mir_i;
    float fresnel_tra;
    float fresnel_tra_i;
    float filter;
    float tx_limit;
    float tx_falloff;
    short ray_depth;
    short ray_depth_tra;
    short har;
    char seed1;
    char seed2;
    float gloss_mir;
    float gloss_tra;
    short samp_gloss_mir;
    short samp_gloss_tra;
    float adapt_thresh_mir;
    float adapt_thresh_tra;
    float aniso_gloss_mir;
    float dist_mir;
    short fadeto_mir;
    short shade_flag;
    int mode;
    int mode_l;
    int mode2;
    int mode2_l;
    short flarec;
    short starc;
    short linec;
    short ringc;
    float hasize;
    float flaresize;
    float subsize;
    float flareboost;
    float strand_sta;
    float strand_end;
    float strand_ease;
    float strand_surfnor;
    float strand_min;
    float strand_widthfade;
    char strand_uvname[64];
    float sbias;
    float lbias;
    float shad_alpha;
    int septex;
    char rgbsel;
    char texact;
    char pr_type;
    char use_nodes;
    short pr_lamp;
    short pr_texture;
    short ml_flag;
    char mapflag;
    char pad;
    short diff_shader;
    short spec_shader;
    float roughness;
    float refrac;
    float param[4];
    float rms;
    float darkness;
    short texco;
    short mapto;
    ColorBand *ramp_col;
    ColorBand *ramp_spec;
    char rampin_col;
    char rampin_spec;
    char rampblend_col;
    char rampblend_spec;
    short ramp_show;
    short pad3;
    float rampfac_col;
    float rampfac_spec;
    MTex *mtex[18];
    bNodeTree *nodetree;
    Ipo *ipo;
    Group *group;
    PreviewImage *preview;
    float friction;
    float fh;
    float reflect;
    float fhdist;
    float xyfrict;
    short dynamode;
    short pad2;
    float sss_radius[3];
    float sss_col[3];
    float sss_error;
    float sss_scale;
    float sss_ior;
    float sss_colfac;
    float sss_texfac;
    float sss_front;
    float sss_back;
    short sss_flag;
    short sss_preset;
    int mapto_textured;
    short shadowonly_flag;
    short index;
    float line_col[4];
    short line_priority;
    short vcol_alpha;
    short paint_active_slot;
    short paint_clone_slot;
    short tot_slots;
    short pad4[3];
    TexPaintSlot *texpaintslot;
    ListBase gpumaterial;
};

struct uiStyle
{
    uiStyle *next;
    uiStyle *prev;
    char name[64];
    uiFontStyle paneltitle;
    uiFontStyle grouplabel;
    uiFontStyle widgetlabel;
    uiFontStyle widget;
    float panelzoom;
    short minlabelchars;
    short minwidgetchars;
    short columnspace;
    short templatespace;
    short boxspace;
    short buttonspacex;
    short buttonspacey;
    short panelspace;
    short panelouter;
    short pad;
};

struct NodeColorCorrection
{
    ColorCorrectionData master;
    ColorCorrectionData shadows;
    ColorCorrectionData midtones;
    ColorCorrectionData highlights;
    float startmidtones;
    float endmidtones;
};

struct NodeTexBase
{
    TexMapping tex_mapping;
    ColorMapping color_mapping;
};

struct Brush
{
    ID id;
    BrushClone clone;
    CurveMapping *curve;
    MTex mtex;
    MTex mask_mtex;
    Brush *toggle_brush;
    void *icon_imbuf;
    PreviewImage *preview;
    ColorBand *gradient;
    PaintCurve *paint_curve;
    char icon_filepath[1024];
    float normal_weight;
    short blend;
    short ob_mode;
    float weight;
    int size;
    int flag;
    int mask_pressure;
    float jitter;
    int jitter_absolute;
    int overlay_flags;
    int spacing;
    int smooth_stroke_radius;
    float smooth_stroke_factor;
    float rate;
    float rgb[3];
    float alpha;
    float secondary_rgb[3];
    int sculpt_plane;
    float plane_offset;
    float pad;
    int gradient_spacing;
    int gradient_stroke_mode;
    int gradient_fill_mode;
    char sculpt_tool;
    char vertexpaint_tool;
    char imagepaint_tool;
    char mask_tool;
    float autosmooth_factor;
    float crease_pinch_factor;
    float plane_trim;
    float height;
    float texture_sample_bias;
    int texture_overlay_alpha;
    int mask_overlay_alpha;
    int cursor_overlay_alpha;
    float unprojected_radius;
    float sharp_threshold;
    int blur_kernel_radius;
    int blur_mode;
    float fill_threshold;
    float add_col[3];
    float sub_col[3];
    float stencil_pos[2];
    float stencil_dimension[2];
    float mask_stencil_pos[2];
    float mask_stencil_dimension[2];
};

struct ParticleSystem
{
    ParticleSystem *next;
    ParticleSystem *prev;
    ParticleSettings *part;
    ParticleData *particles;
    ChildParticle *child;
    void *edit;
    void (*free_edit)();
    void **pathcache;
    void **childcache;
    ListBase pathcachebufs;
    ListBase childcachebufs;
    ClothModifierData *clmd;
    void *hair_in_dm;
    void *hair_out_dm;
    Object *target_ob;
    void *lattice_deform_data;
    Object *parent;
    ListBase targets;
    char name[64];
    float imat[4][4];
    float cfra;
    float tree_frame;
    float bvhtree_frame;
    int seed;
    int child_seed;
    int flag;
    int totpart;
    int totunexist;
    int totchild;
    int totcached;
    int totchildcache;
    short recalc;
    short target_psys;
    short totkeyed;
    short bakespace;
    char bb_uvname[3][64];
    short vgroup[12];
    short vg_neg;
    short rt3;
    void *renderdata;
    PointCache *pointcache;
    ListBase ptcaches;
    ListBase *effectors;
    ParticleSpring *fluid_springs;
    int tot_fluidsprings;
    int alloc_fluidsprings;
    void *tree;
    void *bvhtree;
    void *pdd;
    float dt_frac;
    float _pad;
};

struct View2D
{
    rctf tot;
    rctf cur;
    rcti vert;
    rcti hor;
    rcti mask;
    float min[2];
    float max[2];
    float minzoom;
    float maxzoom;
    short scroll;
    short scroll_ui;
    short keeptot;
    short keepzoom;
    short keepofs;
    short flag;
    short align;
    short winx;
    short winy;
    short oldwinx;
    short oldwiny;
    short around;
    float *tab_offset;
    int tab_num;
    int tab_cur;
    void *sms;
    void *smooth_timer;
};

struct SpaceClip
{
    SpaceLink *next;
    SpaceLink *prev;
    ListBase regionbase;
    int spacetype;
    float xof;
    float yof;
    float xlockof;
    float ylockof;
    float zoom;
    MovieClipUser user;
    MovieClip *clip;
    MovieClipScopes scopes;
    int flag;
    short mode;
    short view;
    int path_length;
    float loc[2];
    float scale;
    float angle;
    int pad;
    float stabmat[4][4];
    float unistabmat[4][4];
    int postproc_flag;
    short gpencil_src;
    short pad2;
    int around;
    int pad4;
    float cursor[2];
    MaskSpaceInfo mask_info;
};

struct bScreen
{
    ID id;
    ListBase vertbase;
    ListBase edgebase;
    ListBase areabase;
    ListBase regionbase;
    Scene *scene;
    Scene *newscene;
    int redraws_flag;
    int pad1;
    short state;
    short temp;
    short winid;
    short do_draw;
    short do_refresh;
    short do_draw_gesture;
    short do_draw_paintcursor;
    short do_draw_drag;
    short swap;
    short mainwin;
    short subwinactive;
    short pad;
    void *animtimer;
    void *context;
};

struct ScrArea
{
    ScrArea *next;
    ScrArea *prev;
    ScrVert *v1;
    ScrVert *v2;
    ScrVert *v3;
    ScrVert *v4;
    bScreen *full;
    rcti totrct;
    char spacetype;
    char butspacetype;
    short winx;
    short winy;
    short headertype;
    short do_refresh;
    short flag;
    short region_active_win;
    char temp;
    char pad;
    void *type;
    ListBase spacedata;
    ListBase regionbase;
    ListBase handlers;
    ListBase actionzones;
};

struct CurvesModifierData
{
    SequenceModifierData modifier;
    CurveMapping curve_mapping;
};

struct HueCorrectModifierData
{
    SequenceModifierData modifier;
    CurveMapping curve_mapping;
};

struct bAction
{
    ID id;
    ListBase curves;
    ListBase chanbase;
    ListBase groups;
    ListBase markers;
    int flag;
    int active_marker;
    int idroot;
    int pad;
};

struct NodeTexSky
{
    NodeTexBase base;
    int sky_model;
    float sun_direction[3];
    float turbidity;
    float ground_albedo;
};

struct NodeTexChecker
{
    NodeTexBase base;
};

struct NodeTexBrick
{
    NodeTexBase base;
    int offset_freq;
    int squash_freq;
    float offset;
    float squash;
};

struct NodeTexGradient
{
    NodeTexBase base;
    int gradient_type;
    int pad;
};

struct NodeTexNoise
{
    NodeTexBase base;
};

struct NodeTexVoronoi
{
    NodeTexBase base;
    int coloring;
    int pad;
};

struct NodeTexMusgrave
{
    NodeTexBase base;
    int musgrave_type;
    int pad;
};

struct NodeTexWave
{
    NodeTexBase base;
    int wave_type;
    int pad;
};

struct NodeTexMagic
{
    NodeTexBase base;
    int depth;
    int pad;
};

struct wmWindow
{
    wmWindow *next;
    wmWindow *prev;
    void *ghostwin;
    bScreen *screen;
    bScreen *newscreen;
    char screenname[64];
    short posx;
    short posy;
    short sizex;
    short sizey;
    short windowstate;
    short monitor;
    short active;
    short cursor;
    short lastcursor;
    short modalcursor;
    short grabcursor;
    short addmousemove;
    int winid;
    short lock_pie_event;
    short last_pie_event;
    void *eventstate;
    void *curswin;
    void *tweak;
    void *ime_data;
    int drawmethod;
    int drawfail;
    void *drawdata;
    ListBase queue;
    ListBase handlers;
    ListBase modalhandlers;
    ListBase subwindows;
    ListBase gesture;
};

struct FreestyleLineStyle
{
    ID id;
    AnimData *adt;
    float r;
    float g;
    float b;
    float alpha;
    float thickness;
    int thickness_position;
    float thickness_ratio;
    int flag;
    int caps;
    int chaining;
    int rounds;
    float split_length;
    float min_angle;
    float max_angle;
    float min_length;
    float max_length;
    int chain_count;
    short split_dash1;
    short split_gap1;
    short split_dash2;
    short split_gap2;
    short split_dash3;
    short split_gap3;
    int sort_key;
    int integration_type;
    float texstep;
    short texact;
    short pr_texture;
    short use_nodes;
    short pad[3];
    short dash1;
    short gap1;
    short dash2;
    short gap2;
    short dash3;
    short gap3;
    int panel;
    MTex *mtex[18];
    bNodeTree *nodetree;
    ListBase color_modifiers;
    ListBase alpha_modifiers;
    ListBase thickness_modifiers;
    ListBase geometry_modifiers;
};

struct Mesh
{
    ID id;
    AnimData *adt;
    BoundBox *bb;
    Ipo *ipo;
    Key *key;
    Material **mat;
    MSelect *mselect;
    MPoly *mpoly;
    MTexPoly *mtpoly;
    MLoop *mloop;
    MLoopUV *mloopuv;
    MLoopCol *mloopcol;
    MFace *mface;
    MTFace *mtface;
    TFace *tface;
    MVert *mvert;
    MEdge *medge;
    MDeformVert *dvert;
    MCol *mcol;
    Mesh *texcomesh;
    void *edit_btmesh;
    CustomData vdata;
    CustomData edata;
    CustomData fdata;
    CustomData pdata;
    CustomData ldata;
    int totvert;
    int totedge;
    int totface;
    int totselect;
    int totpoly;
    int totloop;
    int act_face;
    float loc[3];
    float size[3];
    float rot[3];
    int drawflag;
    short texflag;
    short flag;
    float smoothresh;
    int pad2;
    char cd_flag;
    char pad;
    char subdiv;
    char subdivr;
    char subsurftype;
    char editflag;
    short totcol;
    Multires *mr;
};

struct ToolSettings
{
    VPaint *vpaint;
    VPaint *wpaint;
    Sculpt *sculpt;
    UvSculpt *uvsculpt;
    float vgroup_weight;
    float doublimit;
    float normalsize;
    short automerge;
    short selectmode;
    char unwrapper;
    char uvcalc_flag;
    char uv_flag;
    char uv_selectmode;
    float uvcalc_margin;
    short autoik_chainlen;
    char gpencil_flags;
    char gpencil_src;
    char pad[4];
    ImagePaintSettings imapaint;
    ParticleEditSettings particle;
    float proportional_size;
    float select_thresh;
    short autokey_mode;
    short autokey_flag;
    char multires_subdiv_type;
    char pad3[1];
    short skgen_resolution;
    float skgen_threshold_internal;
    float skgen_threshold_external;
    float skgen_length_ratio;
    float skgen_length_limit;
    float skgen_angle_limit;
    float skgen_correlation_limit;
    float skgen_symmetry_limit;
    float skgen_retarget_angle_weight;
    float skgen_retarget_length_weight;
    float skgen_retarget_distance_weight;
    short skgen_options;
    char skgen_postpro;
    char skgen_postpro_passes;
    char skgen_subdivisions[3];
    char skgen_multi_level;
    Object *skgen_template;
    char bone_sketching;
    char bone_sketching_convert;
    char skgen_subdivision_number;
    char skgen_retarget_options;
    char skgen_retarget_roll;
    char skgen_side_string[8];
    char skgen_num_string[8];
    char edge_mode;
    char edge_mode_live_unwrap;
    char snap_mode;
    char snap_node_mode;
    char snap_uv_mode;
    short snap_flag;
    short snap_target;
    short proportional;
    short prop_mode;
    char proportional_objects;
    char proportional_mask;
    char auto_normalize;
    char multipaint;
    char weightuser;
    char vgroupsubset;
    int use_uv_sculpt;
    int uv_sculpt_settings;
    int uv_sculpt_tool;
    int uv_relax_method;
    short sculpt_paint_settings;
    short pad5;
    int sculpt_paint_unified_size;
    float sculpt_paint_unified_unprojected_radius;
    float sculpt_paint_unified_alpha;
    UnifiedPaintSettings unified_paint_settings;
    MeshStatVis statvis;
};

struct View3D
{
    SpaceLink *next;
    SpaceLink *prev;
    ListBase regionbase;
    int spacetype;
    float blockscale;
    short blockhandler[8];
    float viewquat[4];
    float dist;
    float bundle_size;
    char bundle_drawtype;
    char pad[3];
    int lay_prev;
    int lay_used;
    short persp;
    short view;
    Object *camera;
    Object *ob_centre;
    rctf render_border;
    ListBase bgpicbase;
    BGpic *bgpic;
    View3D *localvd;
    char ob_centre_bone[64];
    int lay;
    int layact;
    short drawtype;
    short ob_centre_cursor;
    short scenelock;
    short around;
    short flag;
    short flag2;
    float lens;
    float grid;
    float near;
    float far;
    float ofs[3];
    float cursor[3];
    short matcap_icon;
    short gridlines;
    short gridsubdiv;
    char gridflag;
    char twtype;
    char twmode;
    char twflag;
    short flag3;
    ListBase afterdraw_transp;
    ListBase afterdraw_xray;
    ListBase afterdraw_xraytransp;
    char zbuf;
    char transp;
    char xray;
    char pad3[5];
    void *properties_storage;
    Material *defmaterial;
    bGPdata *gpd;
};

struct SpaceImage
{
    SpaceLink *next;
    SpaceLink *prev;
    ListBase regionbase;
    int spacetype;
    int flag;
    Image *image;
    ImageUser iuser;
    CurveMapping *cumap;
    Scopes scopes;
    Histogram sample_line_hist;
    bGPdata *gpd;
    float cursor[2];
    float xof;
    float yof;
    float zoom;
    float centx;
    float centy;
    char mode;
    char pin;
    short pad;
    short curtile;
    short lock;
    char dt_uv;
    char sticky;
    char dt_uvstretch;
    char around;
    MaskSpaceInfo mask_info;
};

struct bNode
{
    bNode *next;
    bNode *prev;
    bNode *new_node;
    IDProperty *prop;
    void *typeinfo;
    char idname[64];
    char name[64];
    int flag;
    short type;
    short pad;
    short done;
    short level;
    short lasty;
    short menunr;
    short stack_index;
    short nr;
    float color[3];
    ListBase inputs;
    ListBase outputs;
    bNode *parent;
    ID *id;
    void *storage;
    bNode *original;
    ListBase internal_links;
    float locx;
    float locy;
    float width;
    float height;
    float miniwidth;
    float offsetx;
    float offsety;
    int update;
    char label[64];
    short custom1;
    short custom2;
    float custom3;
    float custom4;
    short need_exec;
    short exec;
    void *threaddata;
    rctf totr;
    rctf butr;
    rctf prvr;
    short preview_xsize;
    short preview_ysize;
    int pad2;
    void *block;
};

struct NodeTexImage
{
    NodeTexBase base;
    ImageUser iuser;
    int color_space;
    int projection;
    float projection_blend;
    int interpolation;
};

struct NodeTexEnvironment
{
    NodeTexBase base;
    ImageUser iuser;
    int color_space;
    int projection;
};

struct SpaceButs
{
    SpaceLink *next;
    SpaceLink *prev;
    ListBase regionbase;
    int spacetype;
    float blockscale;
    short blockhandler[8];
    View2D v2d;
    short mainb;
    short mainbo;
    short mainbuser;
    short re_align;
    short align;
    short preview;
    short texture_context;
    short texture_context_prev;
    char flag;
    char pad[7];
    void *path;
    int pathflag;
    int dataicon;
    ID *pinid;
    void *texuser;
};

struct SpaceNla
{
    SpaceLink *next;
    SpaceLink *prev;
    ListBase regionbase;
    int spacetype;
    float blockscale;
    short blockhandler[8];
    short autosnap;
    short flag;
    int pad;
    bDopeSheet *ads;
    View2D v2d;
};

struct bNodeTree
{
    ID id;
    AnimData *adt;
    void *typeinfo;
    char idname[64];
    void *interface_type;
    bGPdata *gpd;
    float view_center[2];
    ListBase nodes;
    ListBase links;
    int type;
    int init;
    int cur_index;
    int flag;
    int update;
    short is_updating;
    short done;
    int pad2;
    int nodetype;
    short edit_quality;
    short render_quality;
    int chunksize;
    rctf viewer_border;
    ListBase inputs;
    ListBase outputs;
    void *previews;
    bNodeInstanceKey active_viewer_key;
    int pad;
    void *execdata;
    void (*progress)();
    void (*stats_draw)();
    int (*test_break)();
    void (*update_draw)();
    void *tbh;
    void *prh;
    void *sdh;
    void *udh;
};

struct SpaceIpo
{
    SpaceLink *next;
    SpaceLink *prev;
    ListBase regionbase;
    int spacetype;
    float blockscale;
    short blockhandler[8];
    View2D v2d;
    bDopeSheet *ads;
    ListBase ghostCurves;
    short mode;
    short autosnap;
    int flag;
    float cursorVal;
    int around;
};

struct SpaceTime
{
    SpaceLink *next;
    SpaceLink *prev;
    ListBase regionbase;
    int spacetype;
    float blockscale;
    View2D v2d;
    ListBase caches;
    int cache_display;
    int flag;
};

struct SpaceSeq
{
    SpaceLink *next;
    SpaceLink *prev;
    ListBase regionbase;
    int spacetype;
    float blockscale;
    short blockhandler[8];
    View2D v2d;
    float xof;
    float yof;
    short mainb;
    short render_size;
    short chanshown;
    short zebra;
    int flag;
    float zoom;
    int view;
    int overlay_type;
    int draw_flag;
    int pad;
    bGPdata *gpd;
    SequencerScopes scopes;
};

struct SpaceOops
{
    SpaceLink *next;
    SpaceLink *prev;
    ListBase regionbase;
    int spacetype;
    float blockscale;
    short blockhandler[8];
    View2D v2d;
    ListBase tree;
    void *treestore;
    char search_string[32];
    TreeStoreElem search_tse;
    short flag;
    short outlinevis;
    short storeflag;
    short search_flags;
    void *treehash;
};

struct SpaceNode
{
    SpaceLink *next;
    SpaceLink *prev;
    ListBase regionbase;
    int spacetype;
    float blockscale;
    short blockhandler[8];
    View2D v2d;
    ID *id;
    ID *from;
    short flag;
    short pad1;
    float aspect;
    float pad2;
    float xof;
    float yof;
    float zoom;
    float cursor[2];
    ListBase treepath;
    bNodeTree *nodetree;
    bNodeTree *edittree;
    char tree_idname[64];
    int treetype;
    int pad3;
    short texfrom;
    short shaderfrom;
    short recalc;
    short pad4;
    ListBase linkdrag;
    bGPdata *gpd;
};

struct SpaceAction
{
    SpaceLink *next;
    SpaceLink *prev;
    ListBase regionbase;
    int spacetype;
    float blockscale;
    short blockhandler[8];
    View2D v2d;
    bAction *action;
    bDopeSheet ads;
    char mode;
    char autosnap;
    short flag;
    float timeslide;
};

struct MovieTracking
{
    MovieTrackingSettings settings;
    MovieTrackingCamera camera;
    ListBase tracks;
    ListBase plane_tracks;
    MovieTrackingReconstruction reconstruction;
    MovieTrackingStabilization stabilization;
    MovieTrackingTrack *act_track;
    MovieTrackingPlaneTrack *act_plane_track;
    ListBase objects;
    int objectnr;
    int tot_object;
    MovieTrackingStats *stats;
    MovieTrackingDopesheet dopesheet;
};

struct UserDef
{
    int versionfile;
    int subversionfile;
    int flag;
    int dupflag;
    int savetime;
    char tempdir[768];
    char fontdir[768];
    char renderdir[1024];
    char render_cachedir[768];
    char textudir[768];
    char pythondir[768];
    char sounddir[768];
    char i18ndir[768];
    char image_editor[1024];
    char anim_player[1024];
    int anim_player_preset;
    short v2d_min_gridsize;
    short timecode_style;
    short versions;
    short dbl_click_time;
    short gameflags;
    short wheellinescroll;
    int uiflag;
    int uiflag2;
    int language;
    short userpref;
    short viewzoom;
    int mixbufsize;
    int audiodevice;
    int audiorate;
    int audioformat;
    int audiochannels;
    int scrollback;
    int dpi;
    short encoding;
    short transopts;
    short menuthreshold1;
    short menuthreshold2;
    ListBase themes;
    ListBase uifonts;
    ListBase uistyles;
    ListBase keymaps;
    ListBase user_keymaps;
    ListBase addons;
    ListBase autoexec_paths;
    char keyconfigstr[64];
    short undosteps;
    short undomemory;
    short gp_manhattendist;
    short gp_euclideandist;
    short gp_eraser;
    short gp_settings;
    short tb_leftmouse;
    short tb_rightmouse;
    SolidLight light[3];
    short tw_hotspot;
    short tw_flag;
    short tw_handlesize;
    short tw_size;
    short textimeout;
    short texcollectrate;
    short wmdrawmethod;
    short dragthreshold;
    int memcachelimit;
    int prefetchframes;
    short frameserverport;
    short pad_rot_angle;
    short obcenter_dia;
    short rvisize;
    short rvibright;
    short recent_files;
    short smooth_viewtx;
    short glreslimit;
    short curssize;
    short color_picker_type;
    char ipo_new;
    char keyhandles_new;
    char gpu_select_method;
    char pad1;
    short scrcastfps;
    short scrcastwait;
    short widget_unit;
    short anisotropic_filter;
    short use_16bit_textures;
    short use_gpu_mipmap;
    float ndof_sensitivity;
    float ndof_orbit_sensitivity;
    int ndof_flag;
    short ogl_multisamples;
    short image_draw_method;
    float glalphaclip;
    short autokey_mode;
    short autokey_flag;
    short text_render;
    short pad9;
    ColorBand coba_weight;
    float sculpt_paint_overlay_col[3];
    float gpencil_new_layer_col[4];
    short tweak_threshold;
    char navigation_mode;
    char pad;
    char author[80];
    char font_path_ui[1024];
    int compute_device_type;
    int compute_device_id;
    float fcu_inactive_alpha;
    float pixelsize;
    int virtual_pixel;
    short pie_interaction_type;
    short pie_initial_timeout;
    short pie_animation_timeout;
    short pie_menu_confirm;
    short pie_menu_radius;
    short pie_menu_threshold;
    WalkNavigation walk_navigation;
};

struct wmWindowManager
{
    ID id;
    wmWindow *windrawable;
    wmWindow *winactive;
    ListBase windows;
    int initialized;
    short file_saved;
    short op_undo_depth;
    ListBase operators;
    ListBase queue;
    ReportList reports;
    ListBase jobs;
    ListBase paintcursors;
    ListBase drags;
    ListBase keyconfigs;
    wmKeyConfig *defaultconf;
    wmKeyConfig *addonconf;
    wmKeyConfig *userconf;
    ListBase timers;
    void *autosavetimer;
    char is_interface_locked;
    char par[7];
};

struct RenderData
{
    ImageFormatData im_format;
    AviCodecData *avicodecdata;
    QuicktimeCodecData *qtcodecdata;
    QuicktimeCodecSettings qtcodecsettings;
    FFMpegCodecData ffcodecdata;
    int cfra;
    int sfra;
    int efra;
    float subframe;
    int psfra;
    int pefra;
    int images;
    int framapto;
    short flag;
    short threads;
    float framelen;
    float blurfac;
    float edgeR;
    float edgeG;
    float edgeB;
    short fullscreen;
    short xplay;
    short yplay;
    short freqplay;
    short depth;
    short attrib;
    int frame_step;
    short stereomode;
    short dimensionspreset;
    short filtertype;
    short size;
    short maximsize;
    short pad6;
    int xsch;
    int ysch;
    short xparts;
    short yparts;
    int tilex;
    int tiley;
    short planes;
    short imtype;
    short subimtype;
    short quality;
    short displaymode;
    char use_lock_interface;
    char pad7;
    int scemode;
    int mode;
    int raytrace_options;
    short raytrace_structure;
    short pad1;
    short ocres;
    short pad4;
    short alphamode;
    short osa;
    short frs_sec;
    short edgeint;
    rctf safety;
    rctf border;
    rcti disprect;
    ListBase layers;
    short actlay;
    short mblur_samples;
    float xasp;
    float yasp;
    float frs_sec_base;
    float gauss;
    int color_mgt_flag;
    float postgamma;
    float posthue;
    float postsat;
    float dither_intensity;
    short bake_osa;
    short bake_filter;
    short bake_mode;
    short bake_flag;
    short bake_normal_space;
    short bake_quad_split;
    float bake_maxdist;
    float bake_biasdist;
    short bake_samples;
    short bake_pad;
    float bake_user_scale;
    float bake_pad1;
    char pic[1024];
    int stamp;
    short stamp_font_id;
    short pad3;
    char stamp_udata[768];
    float fg_stamp[4];
    float bg_stamp[4];
    char seq_prev_type;
    char seq_rend_type;
    char seq_flag;
    char pad5[5];
    int simplify_flag;
    short simplify_subsurf;
    short simplify_shadowsamples;
    float simplify_particles;
    float simplify_aosss;
    short cineonwhite;
    short cineonblack;
    float cineongamma;
    short jp2_preset;
    short jp2_depth;
    int rpad3;
    short domeres;
    short domemode;
    short domeangle;
    short dometilt;
    float domeresbuf;
    float pad2;
    Text *dometext;
    int line_thickness_mode;
    float unit_line_thickness;
    char engine[32];
    BakeData bake;
    int preview_start_resolution;
    int pad;
};

struct MovieClip
{
    ID id;
    AnimData *adt;
    char name[1024];
    int source;
    int lastframe;
    int lastsize[2];
    float aspx;
    float aspy;
    void *anim;
    void *cache;
    bGPdata *gpd;
    MovieTracking tracking;
    void *tracking_context;
    MovieClipProxy proxy;
    int flag;
    int len;
    int start_frame;
    int frame_offset;
    ColorManagedColorspaceSettings colorspace_settings;
};

struct ARegion
{
    ARegion *next;
    ARegion *prev;
    View2D v2d;
    rcti winrct;
    rcti drawrct;
    short winx;
    short winy;
    short swinid;
    short regiontype;
    short alignment;
    short flag;
    float fsize;
    short sizex;
    short sizey;
    short do_draw;
    short do_draw_overlay;
    short swap;
    short overlap;
    short flagfullscreen;
    short pad;
    void *type;
    ListBase uiblocks;
    ListBase panels;
    ListBase panels_category_active;
    ListBase ui_lists;
    ListBase ui_previews;
    ListBase handlers;
    ListBase panels_category;
    void *regiontimer;
    char *headerstr;
    void *regiondata;
};

struct Object
{
    ID id;
    AnimData *adt;
    void *sculpt;
    short type;
    short partype;
    int par1;
    int par2;
    int par3;
    char parsubstr[64];
    Object *parent;
    Object *track;
    Object *proxy;
    Object *proxy_group;
    Object *proxy_from;
    Ipo *ipo;
    BoundBox *bb;
    bAction *action;
    bAction *poselib;
    bPose *pose;
    void *data;
    bGPdata *gpd;
    bAnimVizSettings avs;
    bMotionPath *mpath;
    ListBase constraintChannels;
    ListBase effect;
    ListBase defbase;
    ListBase modifiers;
    int mode;
    int restore_mode;
    Material **mat;
    char *matbits;
    int totcol;
    int actcol;
    float loc[3];
    float dloc[3];
    float orig[3];
    float size[3];
    float dsize[3];
    float dscale[3];
    float rot[3];
    float drot[3];
    float quat[4];
    float dquat[4];
    float rotAxis[3];
    float drotAxis[3];
    float rotAngle;
    float drotAngle;
    float obmat[4][4];
    float parentinv[4][4];
    float constinv[4][4];
    float imat[4][4];
    float imat_ren[4][4];
    int lay;
    short flag;
    short colbits;
    short transflag;
    short protectflag;
    short trackflag;
    short upflag;
    short nlaflag;
    short ipoflag;
    short scaflag;
    char scavisflag;
    char depsflag;
    int dupon;
    int dupoff;
    int dupsta;
    int dupend;
    int pad;
    float mass;
    float damping;
    float inertia;
    float formfactor;
    float rdamping;
    float margin;
    float max_vel;
    float min_vel;
    float obstacleRad;
    float step_height;
    float jump_speed;
    float fall_speed;
    short col_group;
    short col_mask;
    short rotmode;
    char boundtype;
    char collision_boundtype;
    short dtx;
    char dt;
    char empty_drawtype;
    float empty_drawsize;
    float dupfacesca;
    ListBase prop;
    ListBase sensors;
    ListBase controllers;
    ListBase actuators;
    float sf;
    short index;
    short actdef;
    float col[4];
    int gameflag;
    int gameflag2;
    BulletSoftBody *bsoft;
    char restrictflag;
    char recalc;
    short softflag;
    float anisotropicFriction[3];
    ListBase constraints;
    ListBase nlastrips;
    ListBase hooks;
    ListBase particlesystem;
    PartDeflect *pd;
    SoftBody *soft;
    Group *dup_group;
    char body_type;
    char shapeflag;
    short shapenr;
    float smoothresh;
    FluidsimSettings *fluidsimSettings;
    void *curve_cache;
    void *derivedDeform;
    void *derivedFinal;
    uint64_t lastDataMask;
    uint64_t customdata_mask;
    int state;
    int init_state;
    ListBase gpulamp;
    ListBase pc_ids;
    ListBase *duplilist;
    RigidBodyOb *rigidbody_object;
    RigidBodyCon *rigidbody_constraint;
    float ima_ofs[2];
    ImageUser *iuser;
    ListBase lodlevels;
    LodLevel *currentlod;
};

struct ThemeUI
{
    uiWidgetColors wcol_regular;
    uiWidgetColors wcol_tool;
    uiWidgetColors wcol_text;
    uiWidgetColors wcol_radio;
    uiWidgetColors wcol_option;
    uiWidgetColors wcol_toggle;
    uiWidgetColors wcol_num;
    uiWidgetColors wcol_numslider;
    uiWidgetColors wcol_menu;
    uiWidgetColors wcol_pulldown;
    uiWidgetColors wcol_menu_back;
    uiWidgetColors wcol_menu_item;
    uiWidgetColors wcol_tooltip;
    uiWidgetColors wcol_box;
    uiWidgetColors wcol_scroll;
    uiWidgetColors wcol_progress;
    uiWidgetColors wcol_list_item;
    uiWidgetColors wcol_pie_menu;
    uiWidgetStateColors wcol_state;
    uiPanelColors panel;
    char widget_emboss[4];
    float menu_shadow_fac;
    short menu_shadow_width;
    short pad[3];
    char iconfile[256];
    float icon_alpha;
    char xaxis[4];
    char yaxis[4];
    char zaxis[4];
};

struct Scene
{
    ID id;
    AnimData *adt;
    Object *camera;
    World *world;
    Scene *set;
    ListBase base;
    Base *basact;
    Object *obedit;
    float cursor[3];
    float twcent[3];
    float twmin[3];
    float twmax[3];
    int lay;
    int layact;
    int lay_updated;
    short flag;
    char use_nodes;
    char pad[1];
    bNodeTree *nodetree;
    Editing *ed;
    ToolSettings *toolsettings;
    void *stats;
    RenderData r;
    AudioData audio;
    ListBase markers;
    ListBase transform_spaces;
    void *sound_scene;
    void *sound_scene_handle;
    void *sound_scrub_handle;
    void *speaker_handles;
    void *fps_info;
    void *theDag;
    short dagflags;
    short recalc;
    int active_keyingset;
    ListBase keyingsets;
    GameFraming framing;
    GameData gm;
    UnitSettings unit;
    bGPdata *gpd;
    PhysicsSettings physics_settings;
    MovieClip *clip;
    uint64_t customdata_mask;
    uint64_t customdata_mask_modal;
    ColorManagedViewSettings view_settings;
    ColorManagedDisplaySettings display_settings;
    ColorManagedColorspaceSettings sequencer_colorspace_settings;
    RigidBodyWorld *rigidbody_world;
};

struct bTheme
{
    bTheme *next;
    bTheme *prev;
    char name[32];
    ThemeUI tui;
    ThemeSpace tbuts;
    ThemeSpace tv3d;
    ThemeSpace tfile;
    ThemeSpace tipo;
    ThemeSpace tinfo;
    ThemeSpace tact;
    ThemeSpace tnla;
    ThemeSpace tseq;
    ThemeSpace tima;
    ThemeSpace text;
    ThemeSpace toops;
    ThemeSpace ttime;
    ThemeSpace tnode;
    ThemeSpace tlogic;
    ThemeSpace tuserpref;
    ThemeSpace tconsole;
    ThemeSpace tclip;
    ThemeWireColor tarm[20];
    int active_theme_area;
    int pad;
};

/** @}*/
}
#endif//_Blender_h_
