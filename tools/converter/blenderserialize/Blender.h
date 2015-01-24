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

// 9 DNA structure 26, 6
struct PreviewImage
{
    int           w[2];                // 1 type, name 4, 38.
    int           h[2];                // 2 type, name 4, 39.
    short         changed[2];          // 3 type, name 2, 40.
    short         changed_timestamp[2];// 4 type, name 2, 41.
    int           *rect[2];            // 5 type, name 4, 42.
    void          *gputexture[2];      // Note: using void* on undefined DNA type: GPUTexture
};

// 10 DNA structure 28, 6
struct IpoDriver
{
    Object    *ob;      // 1 type, name 29, 44.
    short     blocktype;// 2 type, name 2, 45.
    short     adrcode;  // 3 type, name 2, 46.
    short     type;     // 4 type, name 2, 16.
    short     flag;     // 5 type, name 2, 18.
    char      name[128];// 6 type, name 0, 47.
};

// 11 DNA structure 34, 15
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

// 12 DNA structure 37, 6
struct TextLine
{
    TextLine    *next;  // 1 type, name 37, 0.
    TextLine    *prev;  // 2 type, name 37, 1.
    char        *line;  // 3 type, name 0, 86.
    char        *format;// 4 type, name 0, 87.
    int         len;    // 5 type, name 4, 22.
    int         blen;   // 6 type, name 4, 88.
};

// 13 DNA structure 25, 3
struct PackedFile
{
    int     size; // 1 type, name 4, 102.
    int     seek; // 2 type, name 4, 103.
    void    *data;// 3 type, name 11, 2.
};

// 14 DNA structure 40, 14
struct ImageUser
{
    Scene    *scene;     // 1 type, name 41, 119.
    int      framenr;    // 2 type, name 4, 120.
    int      frames;     // 3 type, name 4, 121.
    int      offset;     // 4 type, name 4, 122.
    int      sfra;       // 5 type, name 4, 123.
    char     fie_ima;    // 6 type, name 0, 124.
    char     cycl;       // 7 type, name 0, 125.
    char     ok;         // 8 type, name 0, 126.
    char     pad;        // 9 type, name 0, 66.
    short    multi_index;// 10 type, name 2, 127.
    short    layer;      // 11 type, name 2, 128.
    short    pass;       // 12 type, name 2, 129.
    short    flag;       // 13 type, name 2, 18.
    int      pad2;       // 14 type, name 4, 29.
};

// 15 DNA structure 42, 1
struct RenderSlot
{
    char    name[64];// 1 type, name 0, 19.
};

// 16 DNA structure 48, 65
struct MTex
{
    short     texco;         // 1 type, name 2, 162.
    short     mapto;         // 2 type, name 2, 163.
    short     maptoneg;      // 3 type, name 2, 164.
    short     blendtype;     // 4 type, name 2, 165.
    Object    *object;       // 5 type, name 29, 166.
    Tex       *tex;          // 6 type, name 49, 167.
    char      uvname[64];    // 7 type, name 0, 168.
    char      projx;         // 8 type, name 0, 169.
    char      projy;         // 9 type, name 0, 170.
    char      projz;         // 10 type, name 0, 171.
    char      mapping;       // 11 type, name 0, 172.
    float     ofs[3];        // 12 type, name 7, 173.
    float     size[3];       // 13 type, name 7, 174.
    float     rot;           // 14 type, name 7, 175.
    short     texflag;       // 15 type, name 2, 176.
    short     colormodel;    // 16 type, name 2, 177.
    short     pmapto;        // 17 type, name 2, 178.
    short     pmaptoneg;     // 18 type, name 2, 179.
    short     normapspace;   // 19 type, name 2, 180.
    short     which_output;  // 20 type, name 2, 181.
    char      brush_map_mode;// 21 type, name 0, 182.
    char      pad[7];        // 22 type, name 0, 118.
    float     r;             // 23 type, name 7, 183.
    float     g;             // 24 type, name 7, 184.
    float     b;             // 25 type, name 7, 185.
    float     k;             // 26 type, name 7, 186.
    float     def_var;       // 27 type, name 7, 187.
    float     rt;            // 28 type, name 7, 56.
    float     colfac;        // 29 type, name 7, 188.
    float     varfac;        // 30 type, name 7, 189.
    float     norfac;        // 31 type, name 7, 190.
    float     dispfac;       // 32 type, name 7, 191.
    float     warpfac;       // 33 type, name 7, 192.
    float     colspecfac;    // 34 type, name 7, 193.
    float     mirrfac;       // 35 type, name 7, 194.
    float     alphafac;      // 36 type, name 7, 195.
    float     difffac;       // 37 type, name 7, 196.
    float     specfac;       // 38 type, name 7, 197.
    float     emitfac;       // 39 type, name 7, 198.
    float     hardfac;       // 40 type, name 7, 199.
    float     raymirrfac;    // 41 type, name 7, 200.
    float     translfac;     // 42 type, name 7, 201.
    float     ambfac;        // 43 type, name 7, 202.
    float     colemitfac;    // 44 type, name 7, 203.
    float     colreflfac;    // 45 type, name 7, 204.
    float     coltransfac;   // 46 type, name 7, 205.
    float     densfac;       // 47 type, name 7, 206.
    float     scatterfac;    // 48 type, name 7, 207.
    float     reflfac;       // 49 type, name 7, 208.
    float     timefac;       // 50 type, name 7, 209.
    float     lengthfac;     // 51 type, name 7, 210.
    float     clumpfac;      // 52 type, name 7, 211.
    float     dampfac;       // 53 type, name 7, 212.
    float     kinkfac;       // 54 type, name 7, 213.
    float     roughfac;      // 55 type, name 7, 214.
    float     padensfac;     // 56 type, name 7, 215.
    float     gravityfac;    // 57 type, name 7, 216.
    float     lifefac;       // 58 type, name 7, 217.
    float     sizefac;       // 59 type, name 7, 218.
    float     ivelfac;       // 60 type, name 7, 219.
    float     fieldfac;      // 61 type, name 7, 220.
    float     shadowfac;     // 62 type, name 7, 221.
    float     zenupfac;      // 63 type, name 7, 222.
    float     zendownfac;    // 64 type, name 7, 223.
    float     blendfac;      // 65 type, name 7, 224.
};

// 17 DNA structure 50, 6
struct CBData
{
    float    r;  // 1 type, name 7, 183.
    float    g;  // 2 type, name 7, 184.
    float    b;  // 3 type, name 7, 185.
    float    a;  // 4 type, name 7, 225.
    float    pos;// 5 type, name 7, 67.
    int      cur;// 6 type, name 4, 63.
};

// 18 DNA structure 52, 17
struct EnvMap
{
    Object    *object;     // 1 type, name 29, 166.
    Image     *ima;        // 2 type, name 43, 232.
    void      *cube[6];    // Note: using void* on undefined DNA type: ImBuf
    float     imat[4][4];  // 4 type, name 7, 234.
    float     obimat[3][3];// 5 type, name 7, 235.
    short     type;        // 6 type, name 2, 16.
    short     stype;       // 7 type, name 2, 236.
    float     clipsta;     // 8 type, name 7, 106.
    float     clipend;     // 9 type, name 7, 107.
    float     viewscale;   // 10 type, name 7, 237.
    int       notlay;      // 11 type, name 4, 238.
    short     cuberes;     // 12 type, name 2, 239.
    short     depth;       // 13 type, name 2, 240.
    int       ok;          // 14 type, name 4, 126.
    int       lastframe;   // 15 type, name 4, 138.
    short     recalc;      // 16 type, name 2, 241.
    short     lastsize;    // 17 type, name 2, 242.
};

// 19 DNA structure 54, 25
struct PointDensity
{
    short           flag;               // 1 type, name 2, 18.
    short           falloff_type;       // 2 type, name 2, 243.
    float           falloff_softness;   // 3 type, name 7, 244.
    float           radius;             // 4 type, name 7, 245.
    short           source;             // 5 type, name 2, 137.
    short           color_source;       // 6 type, name 2, 246.
    int             totpoints;          // 7 type, name 4, 247.
    int             pdpad;              // 8 type, name 4, 248.
    Object          *object;            // 9 type, name 29, 166.
    int             psys;               // 10 type, name 4, 249.
    short           psys_cache_space;   // 11 type, name 2, 250.
    short           ob_cache_space;     // 12 type, name 2, 251.
    void            *point_tree;        // 13 type, name 11, 252.
    float           *point_data;        // 14 type, name 7, 253.
    float           noise_size;         // 15 type, name 7, 254.
    short           noise_depth;        // 16 type, name 2, 255.
    short           noise_influence;    // 17 type, name 2, 256.
    short           noise_basis;        // 18 type, name 2, 257.
    short           pdpad3[3];          // 19 type, name 2, 258.
    float           noise_fac;          // 20 type, name 7, 259.
    float           speed_scale;        // 21 type, name 7, 260.
    float           falloff_speed_scale;// 22 type, name 7, 261.
    float           pdpad2;             // 23 type, name 7, 262.
    ColorBand       *coba;              // 24 type, name 51, 263.
    CurveMapping    *falloff_curve;     // 25 type, name 55, 264.
};

// 20 DNA structure 56, 16
struct VoxelData
{
    int       resol[3];         // 1 type, name 4, 265.
    int       interp_type;      // 2 type, name 4, 266.
    short     file_format;      // 3 type, name 2, 267.
    short     flag;             // 4 type, name 2, 18.
    short     extend;           // 5 type, name 2, 268.
    short     smoked_type;      // 6 type, name 2, 269.
    short     data_type;        // 7 type, name 2, 270.
    short     pad;              // 8 type, name 2, 66.
    int       _pad;             // 9 type, name 4, 271.
    Object    *object;          // 10 type, name 29, 166.
    float     int_multiplier;   // 11 type, name 7, 272.
    int       still_frame;      // 12 type, name 4, 273.
    char      source_path[1024];// 13 type, name 0, 274.
    float     *dataset;         // 14 type, name 7, 275.
    int       cachedframe;      // 15 type, name 4, 276.
    int       ok;               // 16 type, name 4, 126.
};

// 21 DNA structure 57, 4
struct OceanTex
{
    Object    *object;     // 1 type, name 29, 166.
    char      oceanmod[64];// 2 type, name 0, 277.
    int       output;      // 3 type, name 4, 278.
    int       pad;         // 4 type, name 4, 66.
};

// 22 DNA structure 59, 13
struct TexMapping
{
    float     loc[3];   // 1 type, name 7, 324.
    float     rot[3];   // 2 type, name 7, 325.
    float     size[3];  // 3 type, name 7, 174.
    int       flag;     // 4 type, name 4, 18.
    char      projx;    // 5 type, name 0, 169.
    char      projy;    // 6 type, name 0, 170.
    char      projz;    // 7 type, name 0, 171.
    char      mapping;  // 8 type, name 0, 172.
    int       type;     // 9 type, name 4, 16.
    float     mat[4][4];// 10 type, name 7, 326.
    float     min[3];   // 11 type, name 7, 327.
    float     max[3];   // 12 type, name 7, 328.
    Object    *ob;      // 13 type, name 29, 44.
};

// 23 DNA structure 62, 18
struct VolumeSettings
{
    float    density;            // 1 type, name 7, 392.
    float    emission;           // 2 type, name 7, 393.
    float    scattering;         // 3 type, name 7, 394.
    float    reflection;         // 4 type, name 7, 395.
    float    emission_col[3];    // 5 type, name 7, 396.
    float    transmission_col[3];// 6 type, name 7, 397.
    float    reflection_col[3];  // 7 type, name 7, 398.
    float    density_scale;      // 8 type, name 7, 399.
    float    depth_cutoff;       // 9 type, name 7, 400.
    float    asymmetry;          // 10 type, name 7, 401.
    short    stepsize_type;      // 11 type, name 2, 402.
    short    shadeflag;          // 12 type, name 2, 403.
    short    shade_type;         // 13 type, name 2, 404.
    short    precache_resolution;// 14 type, name 2, 405.
    float    stepsize;           // 15 type, name 7, 406.
    float    ms_diff;            // 16 type, name 7, 407.
    float    ms_intensity;       // 17 type, name 7, 408.
    float    ms_spread;          // 18 type, name 7, 409.
};

// 24 DNA structure 63, 4
struct GameSettings
{
    int    flag;            // 1 type, name 4, 18.
    int    alpha_blend;     // 2 type, name 4, 410.
    int    face_orientation;// 3 type, name 4, 411.
    int    pad1;            // 4 type, name 4, 68.
};

// 25 DNA structure 64, 4
struct TexPaintSlot
{
    Image    *ima;   // 1 type, name 43, 232.
    char     *uvname;// 2 type, name 0, 412.
    int      index;  // 3 type, name 4, 413.
    int      pad;    // 4 type, name 4, 66.
};

// 26 DNA structure 69, 20
struct MetaElem
{
    MetaElem    *next;  // 1 type, name 69, 0.
    MetaElem    *prev;  // 2 type, name 69, 1.
    BoundBox    *bb;    // 3 type, name 70, 533.
    short       type;   // 4 type, name 2, 16.
    short       flag;   // 5 type, name 2, 18.
    short       selcol1;// 6 type, name 2, 534.
    short       selcol2;// 7 type, name 2, 535.
    float       x;      // 8 type, name 7, 5.
    float       y;      // 9 type, name 7, 6.
    float       z;      // 10 type, name 7, 7.
    float       quat[4];// 11 type, name 7, 536.
    float       expx;   // 12 type, name 7, 537.
    float       expy;   // 13 type, name 7, 538.
    float       expz;   // 14 type, name 7, 539.
    float       rad;    // 15 type, name 7, 540.
    float       rad2;   // 16 type, name 7, 541.
    float       s;      // 17 type, name 7, 542.
    float       len;    // 18 type, name 7, 22.
    float       *mat;   // 19 type, name 7, 543.
    float       *imat;  // 20 type, name 7, 544.
};

// 27 DNA structure 32, 16
struct BezTriple
{
    float    vec[3][3];// 1 type, name 7, 555.
    float    alfa;     // 2 type, name 7, 556.
    float    weight;   // 3 type, name 7, 557.
    float    radius;   // 4 type, name 7, 245.
    char     ipo;      // 5 type, name 0, 54.
    char     h1;       // 6 type, name 0, 558.
    char     h2;       // 7 type, name 0, 559.
    char     f1;       // 8 type, name 0, 560.
    char     f2;       // 9 type, name 0, 561.
    char     f3;       // 10 type, name 0, 562.
    char     hide;     // 11 type, name 0, 563.
    char     easing;   // 12 type, name 0, 564.
    float    back;     // 13 type, name 7, 565.
    float    amplitude;// 14 type, name 7, 566.
    float    period;   // 15 type, name 7, 567.
    char     pad[4];   // 16 type, name 0, 568.
};

// 28 DNA structure 31, 7
struct BPoint
{
    float    vec[4];// 1 type, name 7, 569.
    float    alfa;  // 2 type, name 7, 556.
    float    weight;// 3 type, name 7, 557.
    short    f1;    // 4 type, name 2, 560.
    short    hide;  // 5 type, name 2, 563.
    float    radius;// 6 type, name 7, 245.
    float    pad;   // 7 type, name 7, 66.
};

// 29 DNA structure 72, 22
struct Nurb
{
    Nurb         *next;        // 1 type, name 72, 0.
    Nurb         *prev;        // 2 type, name 72, 1.
    short        type;         // 3 type, name 2, 16.
    short        mat_nr;       // 4 type, name 2, 570.
    short        hide;         // 5 type, name 2, 563.
    short        flag;         // 6 type, name 2, 18.
    int          pntsu;        // 7 type, name 4, 571.
    int          pntsv;        // 8 type, name 4, 572.
    short        pad[2];       // 9 type, name 2, 573.
    short        resolu;       // 10 type, name 2, 574.
    short        resolv;       // 11 type, name 2, 575.
    short        orderu;       // 12 type, name 2, 576.
    short        orderv;       // 13 type, name 2, 577.
    short        flagu;        // 14 type, name 2, 578.
    short        flagv;        // 15 type, name 2, 579.
    float        *knotsu;      // 16 type, name 7, 580.
    float        *knotsv;      // 17 type, name 7, 581.
    BPoint       *bp;          // 18 type, name 31, 48.
    BezTriple    *bezt;        // 19 type, name 32, 49.
    short        tilt_interp;  // 20 type, name 2, 582.
    short        radius_interp;// 21 type, name 2, 583.
    int          charidx;      // 22 type, name 4, 584.
};

// 30 DNA structure 73, 5
struct CharInfo
{
    short    kern;  // 1 type, name 2, 585.
    short    mat_nr;// 2 type, name 2, 570.
    char     flag;  // 3 type, name 0, 18.
    char     pad;   // 4 type, name 0, 66.
    short    pad2;  // 5 type, name 2, 29.
};

// 31 DNA structure 74, 4
struct TextBox
{
    float    x;// 1 type, name 7, 5.
    float    y;// 2 type, name 7, 6.
    float    w;// 3 type, name 7, 586.
    float    h;// 4 type, name 7, 587.
};

// 32 DNA structure 88, 8
struct TFace
{
    void     *tpage;  // 1 type, name 11, 674.
    float    uv[4][2];// 2 type, name 7, 675.
    int      col[4];  // 3 type, name 4, 676.
    char     flag;    // 4 type, name 0, 18.
    char     transp;  // 5 type, name 0, 677.
    short    mode;    // 6 type, name 2, 334.
    short    tile;    // 7 type, name 2, 678.
    short    unwrap;  // 8 type, name 2, 679.
};

// 33 DNA structure 86, 7
struct MFace
{
    int      v1;    // 1 type, name 4, 680.
    int      v2;    // 2 type, name 4, 681.
    int      v3;    // 3 type, name 4, 682.
    int      v4;    // 4 type, name 4, 683.
    short    mat_nr;// 5 type, name 2, 570.
    char     edcode;// 6 type, name 0, 684.
    char     flag;  // 7 type, name 0, 18.
};

// 34 DNA structure 90, 5
struct MEdge
{
    int      v1;     // 1 type, name 4, 680.
    int      v2;     // 2 type, name 4, 681.
    char     crease; // 3 type, name 0, 685.
    char     bweight;// 4 type, name 0, 686.
    short    flag;   // 5 type, name 2, 18.
};

// 35 DNA structure 96, 2
struct MDeformWeight
{
    int      def_nr;// 1 type, name 4, 687.
    float    weight;// 2 type, name 7, 557.
};

// 36 DNA structure 91, 3
struct MDeformVert
{
    MDeformWeight    *dw;      // 1 type, name 96, 688.
    int              totweight;// 2 type, name 4, 689.
    int              flag;     // 3 type, name 4, 18.
};

// 37 DNA structure 89, 4
struct MVert
{
    float    co[3];  // 1 type, name 7, 690.
    short    no[3];  // 2 type, name 2, 691.
    char     flag;   // 3 type, name 0, 18.
    char     bweight;// 4 type, name 0, 686.
};

// 38 DNA structure 92, 4
struct MCol
{
    char    a;// 1 type, name 0, 225.
    char    r;// 2 type, name 0, 183.
    char    g;// 3 type, name 0, 184.
    char    b;// 4 type, name 0, 185.
};

// 39 DNA structure 81, 5
struct MPoly
{
    int      loopstart;// 1 type, name 4, 692.
    int      totloop;  // 2 type, name 4, 665.
    short    mat_nr;   // 3 type, name 2, 570.
    char     flag;     // 4 type, name 0, 18.
    char     pad;      // 5 type, name 0, 66.
};

// 40 DNA structure 83, 2
struct MLoop
{
    int    v;// 1 type, name 4, 693.
    int    e;// 2 type, name 4, 694.
};

// 41 DNA structure 82, 6
struct MTexPoly
{
    Image    *tpage;// 1 type, name 43, 674.
    char     flag;  // 2 type, name 0, 18.
    char     transp;// 3 type, name 0, 677.
    short    mode;  // 4 type, name 2, 334.
    short    tile;  // 5 type, name 2, 678.
    short    pad;   // 6 type, name 2, 66.
};

// 42 DNA structure 84, 2
struct MLoopUV
{
    float    uv[2];// 1 type, name 7, 695.
    int      flag; // 2 type, name 4, 18.
};

// 43 DNA structure 85, 4
struct MLoopCol
{
    char    r;// 1 type, name 0, 183.
    char    g;// 2 type, name 0, 184.
    char    b;// 3 type, name 0, 185.
    char    a;// 4 type, name 0, 225.
};

// 44 DNA structure 80, 2
struct MSelect
{
    int    index;// 1 type, name 4, 413.
    int    type; // 2 type, name 4, 16.
};

// 45 DNA structure 87, 7
struct MTFace
{
    float    uv[4][2];// 1 type, name 7, 675.
    Image    *tpage;  // 2 type, name 43, 674.
    char     flag;    // 3 type, name 0, 18.
    char     transp;  // 4 type, name 0, 677.
    short    mode;    // 5 type, name 2, 334.
    short    tile;    // 6 type, name 2, 678.
    short    unwrap;  // 7 type, name 2, 679.
};

// 46 DNA structure 97, 1
struct MFloatProperty
{
    float    f;// 1 type, name 7, 696.
};

// 47 DNA structure 98, 1
struct MIntProperty
{
    int    i;// 1 type, name 4, 697.
};

// 48 DNA structure 99, 2
struct MStringProperty
{
    char    s[255];// 1 type, name 0, 698.
    char    s_len; // 2 type, name 0, 699.
};

// 49 DNA structure 100, 1
struct OrigSpaceFace
{
    float    uv[4][2];// 1 type, name 7, 675.
};

// 50 DNA structure 101, 1
struct OrigSpaceLoop
{
    float    uv[2];// 1 type, name 7, 695.
};

// 51 DNA structure 102, 4
struct MDisps
{
    int      totdisp;   // 1 type, name 4, 700.
    int      level;     // 2 type, name 4, 701.
    float    (*disps)();// 3 type, name 7, 702.
    int      *hidden;   // 4 type, name 4, 703.
};

// 52 DNA structure 103, 4
struct MultiresCol
{
    float    a;// 1 type, name 7, 225.
    float    r;// 2 type, name 7, 183.
    float    g;// 3 type, name 7, 184.
    float    b;// 4 type, name 7, 185.
};

// 53 DNA structure 105, 5
struct MultiresFace
{
    int     v[4];  // 1 type, name 4, 704.
    int     mid;   // 2 type, name 4, 705.
    char    flag;  // 3 type, name 0, 18.
    char    mat_nr;// 4 type, name 0, 570.
    char    pad[2];// 5 type, name 0, 573.
};

// 54 DNA structure 106, 2
struct MultiresEdge
{
    int    v[2];// 1 type, name 4, 706.
    int    mid; // 2 type, name 4, 705.
};

// 55 DNA structure 107, 10
struct MultiresLevel
{
    MultiresLevel      *next;    // 1 type, name 107, 0.
    MultiresLevel      *prev;    // 2 type, name 107, 1.
    MultiresFace       *faces;   // 3 type, name 105, 707.
    MultiresColFace    *colfaces;// 4 type, name 104, 708.
    MultiresEdge       *edges;   // 5 type, name 106, 709.
    int                totvert;  // 6 type, name 4, 53.
    int                totface;  // 7 type, name 4, 662.
    int                totedge;  // 8 type, name 4, 661.
    int                pad;      // 9 type, name 4, 66.
    MVert              *verts;   // 10 type, name 89, 710.
};

// 56 DNA structure 108, 1
struct MRecast
{
    int    i;// 1 type, name 4, 697.
};

// 57 DNA structure 109, 3
struct GridPaintMask
{
    float    *data;// 1 type, name 7, 2.
    int      level;// 2 type, name 4, 701.
    int      pad;  // 3 type, name 4, 66.
};

// 58 DNA structure 110, 2
struct MVertSkin
{
    float    radius[3];// 1 type, name 7, 721.
    int      flag;     // 2 type, name 4, 18.
};

// 59 DNA structure 111, 2
struct FreestyleEdge
{
    char    flag;  // 1 type, name 0, 18.
    char    pad[3];// 2 type, name 0, 333.
};

// 60 DNA structure 112, 2
struct FreestyleFace
{
    char    flag;  // 1 type, name 0, 18.
    char    pad[3];// 2 type, name 0, 333.
};

// 61 DNA structure 113, 9
struct ModifierData
{
    ModifierData    *next;     // 1 type, name 113, 0.
    ModifierData    *prev;     // 2 type, name 113, 1.
    int             type;      // 3 type, name 4, 16.
    int             mode;      // 4 type, name 4, 334.
    int             stackindex;// 5 type, name 4, 722.
    int             pad;       // 6 type, name 4, 66.
    char            name[64];  // 7 type, name 0, 19.
    Scene           *scene;    // 8 type, name 41, 119.
    char            *error;    // 9 type, name 0, 723.
};

// 62 DNA structure 148, 2
struct MDefInfluence
{
    int      vertex;// 1 type, name 4, 825.
    float    weight;// 2 type, name 7, 557.
};

// 63 DNA structure 149, 2
struct MDefCell
{
    int    offset;      // 1 type, name 4, 122.
    int    totinfluence;// 2 type, name 4, 826.
};

// 64 DNA structure 181, 3
struct EditLatt
{
    Lattice    *latt;  // 1 type, name 182, 965.
    int        shapenr;// 2 type, name 4, 590.
    char       pad[4]; // 3 type, name 0, 568.
};

// 65 DNA structure 183, 5
struct bDeformGroup
{
    bDeformGroup    *next;   // 1 type, name 183, 0.
    bDeformGroup    *prev;   // 2 type, name 183, 1.
    char            name[64];// 3 type, name 0, 19.
    char            flag;    // 4 type, name 0, 18.
    char            pad[7];  // 5 type, name 0, 118.
};

// 66 DNA structure 70, 3
struct BoundBox
{
    float    vec[8][3];// 1 type, name 7, 982.
    int      flag;     // 2 type, name 4, 18.
    int      pad;      // 3 type, name 4, 66.
};

// 67 DNA structure 184, 5
struct LodLevel
{
    LodLevel    *next;   // 1 type, name 184, 0.
    LodLevel    *prev;   // 2 type, name 184, 1.
    Object      *source; // 3 type, name 29, 983.
    int         flags;   // 4 type, name 4, 90.
    float       distance;// 5 type, name 7, 984.
};

// 68 DNA structure 197, 14
struct ObHook
{
    ObHook    *next;          // 1 type, name 197, 0.
    ObHook    *prev;          // 2 type, name 197, 1.
    Object    *parent;        // 3 type, name 29, 36.
    float     parentinv[4][4];// 4 type, name 7, 799.
    float     mat[4][4];      // 5 type, name 7, 326.
    float     cent[3];        // 6 type, name 7, 800.
    float     falloff;        // 7 type, name 7, 792.
    char      name[64];       // 8 type, name 0, 19.
    int       *indexar;       // 9 type, name 4, 801.
    int       totindex;       // 10 type, name 4, 802.
    int       curindex;       // 11 type, name 4, 1094.
    short     type;           // 12 type, name 2, 16.
    short     active;         // 13 type, name 2, 1095.
    float     force;          // 14 type, name 7, 803.
};

// 69 DNA structure 198, 11
struct DupliObject
{
    DupliObject       *next;           // 1 type, name 198, 0.
    DupliObject       *prev;           // 2 type, name 198, 1.
    Object            *ob;             // 3 type, name 29, 44.
    float             mat[4][4];       // 4 type, name 7, 326.
    float             orco[3];         // 5 type, name 7, 1096.
    float             uv[2];           // 6 type, name 7, 695.
    short             type;            // 7 type, name 2, 16.
    char              no_draw;         // 8 type, name 0, 1097.
    char              animated;        // 9 type, name 0, 1098.
    int               persistent_id[8];// 10 type, name 4, 1099.
    ParticleSystem    *particle_system;// 11 type, name 152, 1100.
};

// 70 DNA structure 192, 41
struct PartDeflect
{
    int       flag;          // 1 type, name 4, 18.
    short     deflect;       // 2 type, name 2, 1101.
    short     forcefield;    // 3 type, name 2, 1102.
    short     falloff;       // 4 type, name 2, 792.
    short     shape;         // 5 type, name 2, 1103.
    short     tex_mode;      // 6 type, name 2, 1104.
    short     kink;          // 7 type, name 2, 1105.
    short     kink_axis;     // 8 type, name 2, 1106.
    short     zdir;          // 9 type, name 2, 1107.
    float     f_strength;    // 10 type, name 7, 1108.
    float     f_damp;        // 11 type, name 7, 1109.
    float     f_flow;        // 12 type, name 7, 1110.
    float     f_size;        // 13 type, name 7, 1111.
    float     f_power;       // 14 type, name 7, 1112.
    float     maxdist;       // 15 type, name 7, 1113.
    float     mindist;       // 16 type, name 7, 1114.
    float     f_power_r;     // 17 type, name 7, 1115.
    float     maxrad;        // 18 type, name 7, 1116.
    float     minrad;        // 19 type, name 7, 1117.
    float     pdef_damp;     // 20 type, name 7, 1118.
    float     pdef_rdamp;    // 21 type, name 7, 1119.
    float     pdef_perm;     // 22 type, name 7, 1120.
    float     pdef_frict;    // 23 type, name 7, 1121.
    float     pdef_rfrict;   // 24 type, name 7, 1122.
    float     pdef_stickness;// 25 type, name 7, 1123.
    float     absorption;    // 26 type, name 7, 1124.
    float     pdef_sbdamp;   // 27 type, name 7, 1125.
    float     pdef_sbift;    // 28 type, name 7, 1126.
    float     pdef_sboft;    // 29 type, name 7, 1127.
    float     clump_fac;     // 30 type, name 7, 1128.
    float     clump_pow;     // 31 type, name 7, 1129.
    float     kink_freq;     // 32 type, name 7, 1130.
    float     kink_shape;    // 33 type, name 7, 1131.
    float     kink_amp;      // 34 type, name 7, 1132.
    float     free_end;      // 35 type, name 7, 1133.
    float     tex_nabla;     // 36 type, name 7, 1134.
    Tex       *tex;          // 37 type, name 49, 167.
    void      *rng;          // Note: using void* on undefined DNA type: RNG
    float     f_noise;       // 39 type, name 7, 1136.
    int       seed;          // 40 type, name 4, 740.
    Object    *f_source;     // 41 type, name 29, 1137.
};

// 71 DNA structure 200, 6
struct EffectorWeights
{
    Group    *group;        // 1 type, name 66, 503.
    float    weight[14];    // 2 type, name 7, 1138.
    float    global_gravity;// 3 type, name 7, 1139.
    short    flag;          // 4 type, name 2, 18.
    short    rt[3];         // 5 type, name 2, 1140.
    int      pad;           // 6 type, name 4, 66.
};

// 72 DNA structure 201, 5
struct PTCacheExtra
{
    PTCacheExtra    *next;  // 1 type, name 201, 0.
    PTCacheExtra    *prev;  // 2 type, name 201, 1.
    int             type;   // 3 type, name 4, 16.
    int             totdata;// 4 type, name 4, 1141.
    void            *data;  // 5 type, name 11, 2.
};

// 73 DNA structure 204, 1
struct SBVertex
{
    float    vec[4];// 1 type, name 7, 569.
};

// 74 DNA structure 191, 30
struct BulletSoftBody
{
    int      flag;                // 1 type, name 4, 18.
    float    linStiff;            // 2 type, name 7, 1163.
    float    angStiff;            // 3 type, name 7, 1164.
    float    volume;              // 4 type, name 7, 1165.
    int      viterations;         // 5 type, name 4, 1166.
    int      piterations;         // 6 type, name 4, 1167.
    int      diterations;         // 7 type, name 4, 1168.
    int      citerations;         // 8 type, name 4, 1169.
    float    kSRHR_CL;            // 9 type, name 7, 1170.
    float    kSKHR_CL;            // 10 type, name 7, 1171.
    float    kSSHR_CL;            // 11 type, name 7, 1172.
    float    kSR_SPLT_CL;         // 12 type, name 7, 1173.
    float    kSK_SPLT_CL;         // 13 type, name 7, 1174.
    float    kSS_SPLT_CL;         // 14 type, name 7, 1175.
    float    kVCF;                // 15 type, name 7, 1176.
    float    kDP;                 // 16 type, name 7, 1177.
    float    kDG;                 // 17 type, name 7, 1178.
    float    kLF;                 // 18 type, name 7, 1179.
    float    kPR;                 // 19 type, name 7, 1180.
    float    kVC;                 // 20 type, name 7, 1181.
    float    kDF;                 // 21 type, name 7, 1182.
    float    kMT;                 // 22 type, name 7, 1183.
    float    kCHR;                // 23 type, name 7, 1184.
    float    kKHR;                // 24 type, name 7, 1185.
    float    kSHR;                // 25 type, name 7, 1186.
    float    kAHR;                // 26 type, name 7, 1187.
    int      collisionflags;      // 27 type, name 4, 1188.
    int      numclusteriterations;// 28 type, name 4, 1189.
    float    welding;             // 29 type, name 7, 1190.
    float    margin;              // 30 type, name 7, 1041.
};

// 75 DNA structure 208, 1
struct FluidVertexVelocity
{
    float    vel[3];// 1 type, name 7, 1240.
};

// 76 DNA structure 157, 54
struct FluidsimSettings
{
    FluidsimModifierData    *fmd;                 // 1 type, name 156, 1241.
    int                     threads;              // 2 type, name 4, 1242.
    int                     pad1;                 // 3 type, name 4, 68.
    short                   type;                 // 4 type, name 2, 16.
    short                   show_advancedoptions; // 5 type, name 2, 1243.
    short                   resolutionxyz;        // 6 type, name 2, 1244.
    short                   previewresxyz;        // 7 type, name 2, 1245.
    float                   realsize;             // 8 type, name 7, 1246.
    short                   guiDisplayMode;       // 9 type, name 2, 1247.
    short                   renderDisplayMode;    // 10 type, name 2, 1248.
    float                   viscosityValue;       // 11 type, name 7, 1249.
    short                   viscosityMode;        // 12 type, name 2, 1250.
    short                   viscosityExponent;    // 13 type, name 2, 1251.
    float                   grav[3];              // 14 type, name 7, 1252.
    float                   animStart;            // 15 type, name 7, 1253.
    float                   animEnd;              // 16 type, name 7, 1254.
    int                     bakeStart;            // 17 type, name 4, 1255.
    int                     bakeEnd;              // 18 type, name 4, 1256.
    int                     frameOffset;          // 19 type, name 4, 1257.
    int                     pad2;                 // 20 type, name 4, 29.
    float                   gstar;                // 21 type, name 7, 1258.
    int                     maxRefine;            // 22 type, name 4, 1259.
    float                   iniVelx;              // 23 type, name 7, 1260.
    float                   iniVely;              // 24 type, name 7, 1261.
    float                   iniVelz;              // 25 type, name 7, 1262.
    Mesh                    *orgMesh;             // 26 type, name 79, 1263.
    Mesh                    *meshBB;              // 27 type, name 79, 1264.
    char                    surfdataPath[1024];   // 28 type, name 0, 1265.
    float                   bbStart[3];           // 29 type, name 7, 1266.
    float                   bbSize[3];            // 30 type, name 7, 1267.
    Ipo                     *ipo;                 // 31 type, name 33, 80.
    short                   typeFlags;            // 32 type, name 2, 1268.
    char                    domainNovecgen;       // 33 type, name 0, 1269.
    char                    volumeInitType;       // 34 type, name 0, 1270.
    float                   partSlipValue;        // 35 type, name 7, 1271.
    int                     generateTracers;      // 36 type, name 4, 1272.
    float                   generateParticles;    // 37 type, name 7, 1273.
    float                   surfaceSmoothing;     // 38 type, name 7, 1274.
    int                     surfaceSubdivs;       // 39 type, name 4, 1275.
    int                     flag;                 // 40 type, name 4, 18.
    float                   particleInfSize;      // 41 type, name 7, 1276.
    float                   particleInfAlpha;     // 42 type, name 7, 1277.
    float                   farFieldSize;         // 43 type, name 7, 1278.
    FluidVertexVelocity     *meshVelocities;      // 44 type, name 208, 1279.
    int                     totvert;              // 45 type, name 4, 53.
    float                   cpsTimeStart;         // 46 type, name 7, 1280.
    float                   cpsTimeEnd;           // 47 type, name 7, 1281.
    float                   cpsQuality;           // 48 type, name 7, 1282.
    float                   attractforceStrength; // 49 type, name 7, 1283.
    float                   attractforceRadius;   // 50 type, name 7, 1284.
    float                   velocityforceStrength;// 51 type, name 7, 1285.
    float                   velocityforceRadius;  // 52 type, name 7, 1286.
    int                     lastgoodframe;        // 53 type, name 4, 1287.
    float                   animRate;             // 54 type, name 7, 1288.
};

// 77 DNA structure 210, 8
struct Base
{
    Base      *next;  // 1 type, name 210, 0.
    Base      *prev;  // 2 type, name 210, 1.
    int       lay;    // 3 type, name 4, 1021.
    int       selcol; // 4 type, name 4, 1348.
    int       flag;   // 5 type, name 4, 18.
    short     sx;     // 6 type, name 2, 1349.
    short     sy;     // 7 type, name 2, 1350.
    Object    *object;// 8 type, name 29, 166.
};

// 78 DNA structure 211, 13
struct AviCodecData
{
    void    *lpFormat;        // 1 type, name 11, 1351.
    void    *lpParms;         // 2 type, name 11, 1352.
    int     cbFormat;         // 3 type, name 4, 1353.
    int     cbParms;          // 4 type, name 4, 1354.
    int     fccType;          // 5 type, name 4, 1355.
    int     fccHandler;       // 6 type, name 4, 1356.
    int     dwKeyFrameEvery;  // 7 type, name 4, 1357.
    int     dwQuality;        // 8 type, name 4, 1358.
    int     dwBytesPerSecond; // 9 type, name 4, 1359.
    int     dwFlags;          // 10 type, name 4, 1360.
    int     dwInterleaveEvery;// 11 type, name 4, 1361.
    int     pad;              // 12 type, name 4, 66.
    char    avicodecname[128];// 13 type, name 0, 1362.
};

// 79 DNA structure 212, 5
struct QuicktimeCodecData
{
    void    *cdParms;        // 1 type, name 11, 1363.
    void    *pad;            // 2 type, name 11, 1364.
    int     cdSize;          // 3 type, name 4, 1365.
    int     pad2;            // 4 type, name 4, 29.
    char    qtcodecname[128];// 5 type, name 0, 1366.
};

// 80 DNA structure 213, 17
struct QuicktimeCodecSettings
{
    int      codecType;           // 1 type, name 4, 1367.
    int      codecSpatialQuality; // 2 type, name 4, 1368.
    int      codec;               // 3 type, name 4, 1369.
    int      codecFlags;          // 4 type, name 4, 1370.
    int      colorDepth;          // 5 type, name 4, 1371.
    int      codecTemporalQuality;// 6 type, name 4, 1372.
    int      minSpatialQuality;   // 7 type, name 4, 1373.
    int      minTemporalQuality;  // 8 type, name 4, 1374.
    int      keyFrameRate;        // 9 type, name 4, 1375.
    int      bitRate;             // 10 type, name 4, 1376.
    int      audiocodecType;      // 11 type, name 4, 1377.
    int      audioSampleRate;     // 12 type, name 4, 1378.
    short    audioBitDepth;       // 13 type, name 2, 1379.
    short    audioChannels;       // 14 type, name 2, 1380.
    int      audioCodecFlags;     // 15 type, name 4, 1381.
    int      audioBitRate;        // 16 type, name 4, 1382.
    int      pad1;                // 17 type, name 4, 68.
};

// 81 DNA structure 214, 17
struct FFMpegCodecData
{
    int           type;           // 1 type, name 4, 16.
    int           codec;          // 2 type, name 4, 1369.
    int           audio_codec;    // 3 type, name 4, 1383.
    int           video_bitrate;  // 4 type, name 4, 1384.
    int           audio_bitrate;  // 5 type, name 4, 1385.
    int           audio_mixrate;  // 6 type, name 4, 1386.
    int           audio_channels; // 7 type, name 4, 1387.
    int           audio_pad;      // 8 type, name 4, 1388.
    float         audio_volume;   // 9 type, name 7, 1389.
    int           gop_size;       // 10 type, name 4, 1390.
    int           flags;          // 11 type, name 4, 90.
    int           rc_min_rate;    // 12 type, name 4, 1391.
    int           rc_max_rate;    // 13 type, name 4, 1392.
    int           rc_buffer_size; // 14 type, name 4, 1393.
    int           mux_packet_size;// 15 type, name 4, 1394.
    int           mux_rate;       // 16 type, name 4, 1395.
    IDProperty    *properties;    // 17 type, name 21, 30.
};

// 82 DNA structure 215, 9
struct AudioData
{
    int      mixrate;       // 1 type, name 4, 1396.
    float    main;          // 2 type, name 7, 1397.
    float    speed_of_sound;// 3 type, name 7, 1398.
    float    doppler_factor;// 4 type, name 7, 1399.
    int      distance_model;// 5 type, name 4, 1400.
    short    flag;          // 6 type, name 2, 18.
    short    pad;           // 7 type, name 2, 66.
    float    volume;        // 8 type, name 7, 1165.
    float    pad2;          // 9 type, name 7, 29.
};

// 83 DNA structure 223, 9
struct RenderProfile
{
    RenderProfile    *next;            // 1 type, name 223, 0.
    RenderProfile    *prev;            // 2 type, name 223, 1.
    char             name[32];         // 3 type, name 0, 1532.
    short            particle_perc;    // 4 type, name 2, 1533.
    short            subsurf_max;      // 5 type, name 2, 1534.
    short            shadbufsample_max;// 6 type, name 2, 1535.
    short            pad1;             // 7 type, name 2, 68.
    float            ao_error;         // 8 type, name 7, 1536.
    float            pad2;             // 9 type, name 7, 29.
};

// 84 DNA structure 224, 7
struct GameDome
{
    short    res;      // 1 type, name 2, 757.
    short    mode;     // 2 type, name 2, 334.
    short    angle;    // 3 type, name 2, 781.
    short    tilt;     // 4 type, name 2, 1537.
    float    resbuf;   // 5 type, name 7, 1538.
    float    pad2;     // 6 type, name 7, 29.
    Text     *warptext;// 7 type, name 38, 1539.
};

// 85 DNA structure 225, 5
struct GameFraming
{
    float    col[3];// 1 type, name 7, 1540.
    char     type;  // 2 type, name 0, 16.
    char     pad1;  // 3 type, name 0, 68.
    char     pad2;  // 4 type, name 0, 29.
    char     pad3;  // 5 type, name 0, 500.
};

// 86 DNA structure 226, 15
struct RecastData
{
    float    cellsize;            // 1 type, name 7, 1541.
    float    cellheight;          // 2 type, name 7, 1542.
    float    agentmaxslope;       // 3 type, name 7, 1543.
    float    agentmaxclimb;       // 4 type, name 7, 1544.
    float    agentheight;         // 5 type, name 7, 1545.
    float    agentradius;         // 6 type, name 7, 1546.
    float    edgemaxlen;          // 7 type, name 7, 1547.
    float    edgemaxerror;        // 8 type, name 7, 1548.
    float    regionminsize;       // 9 type, name 7, 1549.
    float    regionmergesize;     // 10 type, name 7, 1550.
    int      vertsperpoly;        // 11 type, name 4, 1551.
    float    detailsampledist;    // 12 type, name 7, 1552.
    float    detailsamplemaxerror;// 13 type, name 7, 1553.
    short    pad1;                // 14 type, name 2, 68.
    short    pad2;                // 15 type, name 2, 29.
};

// 87 DNA structure 228, 6
struct TimeMarker
{
    TimeMarker    *next;   // 1 type, name 228, 0.
    TimeMarker    *prev;   // 2 type, name 228, 1.
    int           frame;   // 3 type, name 4, 1142.
    char          name[64];// 4 type, name 0, 19.
    int           flag;    // 5 type, name 4, 18.
    Object        *camera; // 6 type, name 29, 1572.
};

// 88 DNA structure 229, 7
struct Paint
{
    Brush      *brush;             // 1 type, name 230, 931.
    Palette    *palette;           // 2 type, name 231, 1573.
    void       *paint_cursor;      // 3 type, name 11, 1574.
    char       paint_cursor_col[4];// 4 type, name 0, 1575.
    int        flags;              // 5 type, name 4, 90.
    int        num_input_samples;  // 6 type, name 4, 1576.
    int        symmetry_flags;     // 7 type, name 4, 1577.
};

// 89 DNA structure 233, 6
struct ParticleBrushData
{
    short    size;    // 1 type, name 2, 102.
    short    step;    // 2 type, name 2, 1148.
    short    invert;  // 3 type, name 2, 1587.
    short    count;   // 4 type, name 2, 751.
    int      flag;    // 5 type, name 4, 18.
    float    strength;// 6 type, name 7, 734.
};

// 90 DNA structure 238, 5
struct TransformOrientation
{
    TransformOrientation    *next;    // 1 type, name 238, 0.
    TransformOrientation    *prev;    // 2 type, name 238, 1.
    char                    name[64]; // 3 type, name 0, 19.
    float                   mat[3][3];// 4 type, name 7, 1606.
    int                     pad;      // 5 type, name 4, 66.
};

// 91 DNA structure 239, 23
struct UnifiedPaintSettings
{
    int           size;                     // 1 type, name 4, 102.
    float         unprojected_radius;       // 2 type, name 7, 1607.
    float         alpha;                    // 3 type, name 7, 429.
    float         weight;                   // 4 type, name 7, 557.
    float         rgb[3];                   // 5 type, name 7, 1608.
    float         secondary_rgb[3];         // 6 type, name 7, 1609.
    int           flag;                     // 7 type, name 4, 18.
    float         last_rake[2];             // 8 type, name 7, 1610.
    float         brush_rotation;           // 9 type, name 7, 1611.
    int           draw_anchored;            // 10 type, name 4, 1612.
    int           anchored_size;            // 11 type, name 4, 1613.
    char          draw_inverted;            // 12 type, name 0, 1614.
    char          pad3[7];                  // 13 type, name 0, 1615.
    float         overlap_factor;           // 14 type, name 7, 1616.
    float         anchored_initial_mouse[2];// 15 type, name 7, 1617.
    int           stroke_active;            // 16 type, name 4, 1618.
    float         size_pressure_value;      // 17 type, name 7, 1619.
    float         tex_mouse[2];             // 18 type, name 7, 1620.
    float         mask_tex_mouse[2];        // 19 type, name 7, 1621.
    int           do_linear_conversion;     // 20 type, name 4, 1622.
    void          *colorspace;              // Note: using void* on undefined DNA type: ColorSpace
    float         pixel_radius;             // 22 type, name 7, 1624.
    int           pad4;                     // 23 type, name 4, 1468.
};

// 92 DNA structure 241, 13
struct MeshStatVis
{
    char     type;             // 1 type, name 0, 16.
    char     _pad1[2];         // 2 type, name 0, 1625.
    char     overhang_axis;    // 3 type, name 0, 1626.
    float    overhang_min;     // 4 type, name 7, 1627.
    float    overhang_max;     // 5 type, name 7, 1628.
    float    thickness_min;    // 6 type, name 7, 1629.
    float    thickness_max;    // 7 type, name 7, 1630.
    char     thickness_samples;// 8 type, name 0, 1631.
    char     _pad2[3];         // 9 type, name 0, 1632.
    float    distort_min;      // 10 type, name 7, 1633.
    float    distort_max;      // 11 type, name 7, 1634.
    float    sharp_min;        // 12 type, name 7, 1635.
    float    sharp_max;        // 13 type, name 7, 1636.
};

// 93 DNA structure 243, 8
struct bStats
{
    int    totobj;     // 1 type, name 4, 1709.
    int    totlamp;    // 2 type, name 4, 1710.
    int    totobjsel;  // 3 type, name 4, 1711.
    int    totcurve;   // 4 type, name 4, 1712.
    int    totmesh;    // 5 type, name 4, 1713.
    int    totarmature;// 6 type, name 4, 1714.
    int    totvert;    // 7 type, name 4, 53.
    int    totface;    // 8 type, name 4, 662.
};

// 94 DNA structure 244, 4
struct UnitSettings
{
    float    scale_length;   // 1 type, name 7, 1715.
    char     system;         // 2 type, name 0, 1716.
    char     system_rotation;// 3 type, name 0, 1717.
    short    flag;           // 4 type, name 2, 18.
};

// 95 DNA structure 245, 4
struct PhysicsSettings
{
    float    gravity[3];      // 1 type, name 7, 1718.
    int      flag;            // 2 type, name 4, 18.
    int      quick_cache_step;// 3 type, name 4, 1719.
    int      rt;              // 4 type, name 4, 56.
};

// 96 DNA structure 253, 40
struct RegionView3D
{
    float                winmat[4][4];    // 1 type, name 7, 1756.
    float                viewmat[4][4];   // 2 type, name 7, 1757.
    float                viewinv[4][4];   // 3 type, name 7, 1758.
    float                persmat[4][4];   // 4 type, name 7, 1759.
    float                persinv[4][4];   // 5 type, name 7, 1760.
    float                viewmatob[4][4]; // 6 type, name 7, 1761.
    float                persmatob[4][4]; // 7 type, name 7, 1762.
    float                clip[6][4];      // 8 type, name 7, 1763.
    float                clip_local[6][4];// 9 type, name 7, 1764.
    BoundBox             *clipbb;         // 10 type, name 70, 1765.
    RegionView3D         *localvd;        // 11 type, name 253, 1766.
    void                 *render_engine;  // Note: using void* on undefined DNA type: RenderEngine
    void                 *depths;         // Note: using void* on undefined DNA type: ViewDepths
    void                 *gpuoffscreen;   // 14 type, name 11, 1769.
    void                 *sms;            // Note: using void* on undefined DNA type: SmoothView3DStore
    void                 *smooth_timer;   // Note: using void* on undefined DNA type: wmTimer
    float                twmat[4][4];     // 17 type, name 7, 1772.
    float                viewquat[4];     // 18 type, name 7, 1773.
    float                dist;            // 19 type, name 7, 341.
    float                camdx;           // 20 type, name 7, 1774.
    float                camdy;           // 21 type, name 7, 1775.
    float                pixsize;         // 22 type, name 7, 1776.
    float                ofs[3];          // 23 type, name 7, 173.
    float                camzoom;         // 24 type, name 7, 1777.
    char                 is_persp;        // 25 type, name 0, 1778.
    char                 persp;           // 26 type, name 0, 1779.
    char                 view;            // 27 type, name 0, 1755.
    char                 viewlock;        // 28 type, name 0, 1780.
    char                 viewlock_quad;   // 29 type, name 0, 1781.
    char                 pad[3];          // 30 type, name 0, 333.
    float                ofs_lock[2];     // 31 type, name 7, 1782.
    short                twdrawflag;      // 32 type, name 2, 1783.
    short                rflag;           // 33 type, name 2, 1784.
    float                lviewquat[4];    // 34 type, name 7, 1785.
    short                lpersp;          // 35 type, name 2, 1786.
    short                lview;           // 36 type, name 2, 1787.
    float                gridview;        // 37 type, name 7, 1788.
    float                tw_idot[3];      // 38 type, name 7, 1789.
    float                rot_angle;       // 39 type, name 7, 1790.
    float                rot_axis[3];     // 40 type, name 7, 1791.
};

// 97 DNA structure 270, 3
struct SpaceTimeCache
{
    SpaceTimeCache    *next; // 1 type, name 270, 0.
    SpaceTimeCache    *prev; // 2 type, name 270, 1.
    float             *array;// 3 type, name 7, 1874.
};

// 98 DNA structure 274, 5
struct MaskSpaceInfo
{
    Mask    *mask;       // 1 type, name 275, 1884.
    char    draw_flag;   // 2 type, name 0, 1882.
    char    draw_type;   // 3 type, name 0, 1885.
    char    overlay_mode;// 4 type, name 0, 1886.
    char    pad3[5];     // 5 type, name 0, 1825.
};

// 99 DNA structure 276, 17
struct FileSelectParams
{
    char     title[96];      // 1 type, name 0, 1887.
    char     dir[1090];      // 2 type, name 0, 1888.
    char     pad_c1[2];      // 3 type, name 0, 1889.
    char     file[256];      // 4 type, name 0, 1890.
    char     renamefile[256];// 5 type, name 0, 1891.
    char     renameedit[256];// 6 type, name 0, 1892.
    char     filter_glob[64];// 7 type, name 0, 1893.
    int      active_file;    // 8 type, name 4, 1894.
    int      sel_first;      // 9 type, name 4, 1895.
    int      sel_last;       // 10 type, name 4, 1896.
    short    type;           // 11 type, name 2, 16.
    short    flag;           // 12 type, name 2, 18.
    short    sort;           // 13 type, name 2, 1897.
    short    display;        // 14 type, name 2, 1898.
    short    filter;         // 15 type, name 2, 441.
    short    f_fp;           // 16 type, name 2, 1899.
    char     fp_str[8];      // 17 type, name 0, 1900.
};

// 100 DNA structure 291, 7
struct ConsoleLine
{
    ConsoleLine    *next;    // 1 type, name 291, 0.
    ConsoleLine    *prev;    // 2 type, name 291, 1.
    int            len_alloc;// 3 type, name 4, 1971.
    int            len;      // 4 type, name 4, 22.
    char           *line;    // 5 type, name 0, 86.
    int            cursor;   // 6 type, name 4, 1972.
    int            type;     // 7 type, name 4, 16.
};

// 101 DNA structure 296, 7
struct uiFont
{
    uiFont    *next;         // 1 type, name 296, 0.
    uiFont    *prev;         // 2 type, name 296, 1.
    char      filename[1024];// 3 type, name 0, 1989.
    short     blf_id;        // 4 type, name 2, 1990.
    short     uifont_id;     // 5 type, name 2, 1991.
    short     r_to_l;        // 6 type, name 2, 1992.
    short     hinting;       // 7 type, name 2, 1993.
};

// 102 DNA structure 297, 12
struct uiFontStyle
{
    short    uifont_id;  // 1 type, name 2, 1991.
    short    points;     // 2 type, name 2, 1994.
    short    kerning;    // 3 type, name 2, 1995.
    char     pad[6];     // 4 type, name 0, 736.
    short    italic;     // 5 type, name 2, 1996.
    short    bold;       // 6 type, name 2, 1997.
    short    shadow;     // 7 type, name 2, 1998.
    short    shadx;      // 8 type, name 2, 1999.
    short    shady;      // 9 type, name 2, 2000.
    short    align;      // 10 type, name 2, 1840.
    float    shadowalpha;// 11 type, name 7, 2001.
    float    shadowcolor;// 12 type, name 7, 2002.
};

// 103 DNA structure 299, 10
struct uiWidgetColors
{
    char     outline[4];  // 1 type, name 0, 2017.
    char     inner[4];    // 2 type, name 0, 2018.
    char     inner_sel[4];// 3 type, name 0, 2019.
    char     item[4];     // 4 type, name 0, 2020.
    char     text[4];     // 5 type, name 0, 2021.
    char     text_sel[4]; // 6 type, name 0, 2022.
    short    shaded;      // 7 type, name 2, 2023.
    short    shadetop;    // 8 type, name 2, 2024.
    short    shadedown;   // 9 type, name 2, 2025.
    short    alpha_check; // 10 type, name 2, 2026.
};

// 104 DNA structure 300, 8
struct uiWidgetStateColors
{
    char     inner_anim[4];      // 1 type, name 0, 2027.
    char     inner_anim_sel[4];  // 2 type, name 0, 2028.
    char     inner_key[4];       // 3 type, name 0, 2029.
    char     inner_key_sel[4];   // 4 type, name 0, 2030.
    char     inner_driven[4];    // 5 type, name 0, 2031.
    char     inner_driven_sel[4];// 6 type, name 0, 2032.
    float    blend;              // 7 type, name 7, 1754.
    float    pad;                // 8 type, name 7, 66.
};

// 105 DNA structure 301, 5
struct uiPanelColors
{
    char     header[4];  // 1 type, name 0, 2033.
    char     back[4];    // 2 type, name 0, 2034.
    short    show_header;// 3 type, name 2, 2035.
    short    show_back;  // 4 type, name 2, 2036.
    int      pad;        // 5 type, name 4, 66.
};

// 106 DNA structure 302, 4
struct uiGradientColors
{
    char    gradient[4];     // 1 type, name 0, 2037.
    char    high_gradient[4];// 2 type, name 0, 2038.
    int     show_grad;       // 3 type, name 4, 2039.
    int     pad2;            // 4 type, name 4, 29.
};

// 107 DNA structure 305, 5
struct ThemeWireColor
{
    char     solid[4]; // 1 type, name 0, 2243.
    char     select[4];// 2 type, name 0, 2094.
    char     active[4];// 3 type, name 0, 2099.
    short    flag;     // 4 type, name 2, 18.
    short    pad;      // 5 type, name 2, 66.
};

// 108 DNA structure 307, 4
struct bAddon
{
    bAddon        *next;     // 1 type, name 307, 0.
    bAddon        *prev;     // 2 type, name 307, 1.
    char          module[64];// 3 type, name 0, 2264.
    IDProperty    *prop;     // 4 type, name 21, 2265.
};

// 109 DNA structure 308, 5
struct bPathCompare
{
    bPathCompare    *next;    // 1 type, name 308, 0.
    bPathCompare    *prev;    // 2 type, name 308, 1.
    char            path[768];// 3 type, name 0, 2266.
    char            flag;     // 4 type, name 0, 18.
    char            pad[7];   // 5 type, name 0, 118.
};

// 110 DNA structure 309, 5
struct SolidLight
{
    int      flag;   // 1 type, name 4, 18.
    int      pad;    // 2 type, name 4, 66.
    float    col[4]; // 3 type, name 7, 676.
    float    spec[4];// 4 type, name 7, 2267.
    float    vec[4]; // 5 type, name 7, 569.
};

// 111 DNA structure 310, 8
struct WalkNavigation
{
    float    mouse_speed;      // 1 type, name 7, 2268.
    float    walk_speed;       // 2 type, name 7, 2269.
    float    walk_speed_factor;// 3 type, name 7, 2270.
    float    view_height;      // 4 type, name 7, 2271.
    float    jump_height;      // 5 type, name 7, 2272.
    float    teleport_time;    // 6 type, name 7, 2273.
    short    flag;             // 7 type, name 2, 18.
    short    pad[3];           // 8 type, name 2, 333.
};

// 112 DNA structure 314, 7
struct ScrEdge
{
    ScrEdge    *next; // 1 type, name 314, 0.
    ScrEdge    *prev; // 2 type, name 314, 1.
    ScrVert    *v1;   // 3 type, name 313, 2402.
    ScrVert    *v2;   // 4 type, name 313, 2403.
    short      border;// 5 type, name 2, 1474.
    short      flag;  // 6 type, name 2, 18.
    int        pad;   // 7 type, name 4, 66.
};

// 113 DNA structure 315, 20
struct Panel
{
    Panel        *next;        // 1 type, name 315, 0.
    Panel        *prev;        // 2 type, name 315, 1.
    void         *type;        // Note: using void* on undefined DNA type: PanelType
    void         *layout;      // Note: using void* on undefined DNA type: uiLayout
    char         panelname[64];// 5 type, name 0, 2405.
    char         tabname[64];  // 6 type, name 0, 2406.
    char         drawname[64]; // 7 type, name 0, 2407.
    int          ofsx;         // 8 type, name 4, 2408.
    int          ofsy;         // 9 type, name 4, 2409.
    int          sizex;        // 10 type, name 4, 2410.
    int          sizey;        // 11 type, name 4, 2411.
    short        labelofs;     // 12 type, name 2, 2412.
    short        pad;          // 13 type, name 2, 66.
    short        flag;         // 14 type, name 2, 18.
    short        runtime_flag; // 15 type, name 2, 2413.
    short        control;      // 16 type, name 2, 2414.
    short        snap;         // 17 type, name 2, 2415.
    int          sortorder;    // 18 type, name 4, 2416.
    Panel        *paneltab;    // 19 type, name 315, 2417.
    void         *activedata;  // 20 type, name 11, 2418.
};

// 114 DNA structure 318, 3
struct PanelCategoryStack
{
    PanelCategoryStack    *next;     // 1 type, name 318, 0.
    PanelCategoryStack    *prev;     // 2 type, name 318, 1.
    char                  idname[64];// 3 type, name 0, 2419.
};

// 115 DNA structure 319, 15
struct uiList
{
    uiList        *next;            // 1 type, name 319, 0.
    uiList        *prev;            // 2 type, name 319, 1.
    void          *type;            // Note: using void* on undefined DNA type: uiListType
    char          list_id[64];      // 4 type, name 0, 2420.
    int           layout_type;      // 5 type, name 4, 2421.
    int           flag;             // 6 type, name 4, 18.
    int           list_scroll;      // 7 type, name 4, 2422.
    int           list_grip;        // 8 type, name 4, 2423.
    int           list_last_len;    // 9 type, name 4, 2424.
    int           list_last_activei;// 10 type, name 4, 2425.
    char          filter_byname[64];// 11 type, name 0, 2426.
    int           filter_flag;      // 12 type, name 4, 2427.
    int           filter_sort_flag; // 13 type, name 4, 2428.
    IDProperty    *properties;      // 14 type, name 21, 30.
    void          *dyn_data;        // Note: using void* on undefined DNA type: uiListDyn
};

// 116 DNA structure 322, 5
struct uiPreview
{
    uiPreview    *next;         // 1 type, name 322, 0.
    uiPreview    *prev;         // 2 type, name 322, 1.
    char         preview_id[64];// 3 type, name 0, 2430.
    short        height;        // 4 type, name 2, 788.
    short        pad1[3];       // 5 type, name 2, 2431.
};

// 117 DNA structure 327, 14
struct FileGlobal
{
    char        subvstr[4];            // 1 type, name 0, 2458.
    short       subversion;            // 2 type, name 2, 2459.
    short       pads;                  // 3 type, name 2, 2460.
    short       minversion;            // 4 type, name 2, 2461.
    short       minsubversion;         // 5 type, name 2, 2462.
    short       displaymode;           // 6 type, name 2, 1461.
    short       winpos;                // 7 type, name 2, 2463.
    bScreen     *curscreen;            // 8 type, name 312, 2464.
    Scene       *curscene;             // 9 type, name 41, 2465.
    int         fileflags;             // 10 type, name 4, 2466.
    int         globalf;               // 11 type, name 4, 2467.
    uint64_t    build_commit_timestamp;// 12 type, name 10, 2468.
    char        build_hash[16];        // 13 type, name 0, 2469.
    char        filename[1024];        // 14 type, name 0, 1989.
};

// 118 DNA structure 328, 3
struct StripElem
{
    char    name[256];  // 1 type, name 0, 2470.
    int     orig_width; // 2 type, name 4, 2471.
    int     orig_height;// 3 type, name 4, 2472.
};

// 119 DNA structure 329, 4
struct StripCrop
{
    int    top;   // 1 type, name 4, 1925.
    int    bottom;// 2 type, name 4, 2473.
    int    left;  // 3 type, name 4, 1931.
    int    right; // 4 type, name 4, 2474.
};

// 120 DNA structure 330, 2
struct StripTransform
{
    int    xofs;// 1 type, name 4, 2475.
    int    yofs;// 2 type, name 4, 2476.
};

// 121 DNA structure 331, 5
struct StripColorBalance
{
    float    lift[3]; // 1 type, name 7, 2477.
    float    gamma[3];// 2 type, name 7, 2478.
    float    gain[3]; // 3 type, name 7, 2479.
    int      flag;    // 4 type, name 4, 18.
    int      pad;     // 5 type, name 4, 66.
};

// 122 DNA structure 332, 7
struct StripProxy
{
    char     dir[768];        // 1 type, name 0, 2480.
    char     file[256];       // 2 type, name 0, 1890.
    void     *anim;           // Note: using void* on undefined DNA type: anim
    short    tc;              // 4 type, name 2, 2481.
    short    quality;         // 5 type, name 2, 1413.
    short    build_size_flags;// 6 type, name 2, 2482.
    short    build_tc_flags;  // 7 type, name 2, 2483.
};

// 123 DNA structure 336, 5
struct MetaStack
{
    MetaStack    *next;        // 1 type, name 336, 0.
    MetaStack    *prev;        // 2 type, name 336, 1.
    ListBase     *oldbasep;    // 3 type, name 14, 2522.
    Sequence     *parseq;      // 4 type, name 334, 2523.
    int          disp_range[2];// 5 type, name 4, 2524.
};

// 124 DNA structure 337, 4
struct WipeVars
{
    float    edgeWidth;// 1 type, name 7, 2534.
    float    angle;    // 2 type, name 7, 781.
    short    forward;  // 3 type, name 2, 2535.
    short    wipetype; // 4 type, name 2, 2536.
};

// 125 DNA structure 338, 6
struct GlowVars
{
    float    fMini;   // 1 type, name 7, 2537.
    float    fClamp;  // 2 type, name 7, 2538.
    float    fBoost;  // 3 type, name 7, 2539.
    float    dDist;   // 4 type, name 7, 2540.
    int      dQuality;// 5 type, name 4, 2541.
    int      bNoComp; // 6 type, name 4, 2542.
};

// 126 DNA structure 339, 8
struct TransformVars
{
    float    ScalexIni;    // 1 type, name 7, 2543.
    float    ScaleyIni;    // 2 type, name 7, 2544.
    float    xIni;         // 3 type, name 7, 2545.
    float    yIni;         // 4 type, name 7, 2546.
    float    rotIni;       // 5 type, name 7, 2547.
    int      percent;      // 6 type, name 4, 778.
    int      interpolation;// 7 type, name 4, 2548.
    int      uniform_scale;// 8 type, name 4, 2549.
};

// 127 DNA structure 340, 2
struct SolidColorVars
{
    float    col[3];// 1 type, name 7, 1540.
    float    pad;   // 2 type, name 7, 66.
};

// 128 DNA structure 341, 5
struct SpeedControlVars
{
    float    *frameMap;     // 1 type, name 7, 2550.
    float    globalSpeed;   // 2 type, name 7, 2551.
    int      flags;         // 3 type, name 4, 90.
    int      length;        // 4 type, name 4, 738.
    int      lastValidFrame;// 5 type, name 4, 2552.
};

// 129 DNA structure 342, 2
struct GaussianBlurVars
{
    float    size_x;// 1 type, name 7, 2553.
    float    size_y;// 2 type, name 7, 2554.
};

// 130 DNA structure 343, 9
struct SequenceModifierData
{
    SequenceModifierData    *next;          // 1 type, name 343, 0.
    SequenceModifierData    *prev;          // 2 type, name 343, 1.
    int                     type;           // 3 type, name 4, 16.
    int                     flag;           // 4 type, name 4, 18.
    char                    name[64];       // 5 type, name 0, 19.
    int                     mask_input_type;// 6 type, name 4, 2555.
    int                     pad;            // 7 type, name 4, 66.
    Sequence                *mask_sequence; // 8 type, name 334, 2556.
    Mask                    *mask_id;       // 9 type, name 275, 2557.
};

// 131 DNA structure 273, 6
struct SequencerScopes
{
    void     *reference_ibuf;   // Note: using void* on undefined DNA type: ImBuf
    void     *zebra_ibuf;       // Note: using void* on undefined DNA type: ImBuf
    void     *waveform_ibuf;    // Note: using void* on undefined DNA type: ImBuf
    void     *sep_waveform_ibuf;// Note: using void* on undefined DNA type: ImBuf
    void     *vector_ibuf;      // Note: using void* on undefined DNA type: ImBuf
    void     *histogram_ibuf;   // Note: using void* on undefined DNA type: ImBuf
};

// 132 DNA structure 349, 6
struct Effect
{
    Effect    *next;  // 1 type, name 349, 0.
    Effect    *prev;  // 2 type, name 349, 1.
    short     type;   // 3 type, name 2, 16.
    short     flag;   // 4 type, name 2, 18.
    short     buttype;// 5 type, name 2, 2567.
    short     rt;     // 6 type, name 2, 56.
};

// 133 DNA structure 350, 8
struct BuildEff
{
    BuildEff    *next;  // 1 type, name 350, 0.
    BuildEff    *prev;  // 2 type, name 350, 1.
    short       type;   // 3 type, name 2, 16.
    short       flag;   // 4 type, name 2, 18.
    short       buttype;// 5 type, name 2, 2567.
    short       rt;     // 6 type, name 2, 56.
    float       len;    // 7 type, name 7, 22.
    float       sfra;   // 8 type, name 7, 123.
};

// 134 DNA structure 351, 45
struct PartEff
{
    PartEff     *next;           // 1 type, name 351, 0.
    PartEff     *prev;           // 2 type, name 351, 1.
    short       type;            // 3 type, name 2, 16.
    short       flag;            // 4 type, name 2, 18.
    short       buttype;         // 5 type, name 2, 2567.
    short       stype;           // 6 type, name 2, 236.
    short       vertgroup;       // 7 type, name 2, 1207.
    short       userjit;         // 8 type, name 2, 2568.
    float       sta;             // 9 type, name 7, 2569.
    float       end;             // 10 type, name 7, 2570.
    float       lifetime;        // 11 type, name 7, 794.
    int         totpart;         // 12 type, name 4, 2571.
    int         totkey;          // 13 type, name 4, 82.
    int         seed;            // 14 type, name 4, 740.
    float       normfac;         // 15 type, name 7, 2572.
    float       obfac;           // 16 type, name 7, 2573.
    float       randfac;         // 17 type, name 7, 2574.
    float       texfac;          // 18 type, name 7, 2575.
    float       randlife;        // 19 type, name 7, 2576.
    float       force[3];        // 20 type, name 7, 2577.
    float       damp;            // 21 type, name 7, 791.
    float       nabla;           // 22 type, name 7, 316.
    float       vectsize;        // 23 type, name 7, 2578.
    float       maxlen;          // 24 type, name 7, 2579.
    float       pad;             // 25 type, name 7, 66.
    float       defvec[3];       // 26 type, name 7, 2580.
    float       mult[4];         // 27 type, name 7, 2581.
    float       life[4];         // 28 type, name 7, 2582.
    short       child[4];        // 29 type, name 2, 2583.
    short       mat[4];          // 30 type, name 2, 2584.
    short       texmap;          // 31 type, name 2, 2585.
    short       curmult;         // 32 type, name 2, 2586.
    short       staticstep;      // 33 type, name 2, 2587.
    short       omat;            // 34 type, name 2, 2588.
    short       timetex;         // 35 type, name 2, 2589.
    short       speedtex;        // 36 type, name 2, 2590.
    short       flag2;           // 37 type, name 2, 549.
    short       flag2neg;        // 38 type, name 2, 2591.
    short       disp;            // 39 type, name 2, 546.
    short       vertgroup_v;     // 40 type, name 2, 2592.
    char        vgroupname[64];  // 41 type, name 0, 2593.
    char        vgroupname_v[64];// 42 type, name 0, 2594.
    float       imat[4][4];      // 43 type, name 7, 234.
    void        *keys;           // Note: using void* on undefined DNA type: Particle
    Group       *group;          // 45 type, name 66, 503.
};

// 135 DNA structure 353, 16
struct WaveEff
{
    WaveEff    *next;   // 1 type, name 353, 0.
    WaveEff    *prev;   // 2 type, name 353, 1.
    short      type;    // 3 type, name 2, 16.
    short      flag;    // 4 type, name 2, 18.
    short      buttype; // 5 type, name 2, 2567.
    short      stype;   // 6 type, name 2, 236.
    float      startx;  // 7 type, name 7, 786.
    float      starty;  // 8 type, name 7, 787.
    float      height;  // 9 type, name 7, 788.
    float      width;   // 10 type, name 7, 603.
    float      narrow;  // 11 type, name 7, 789.
    float      speed;   // 12 type, name 7, 790.
    float      minfac;  // 13 type, name 7, 2596.
    float      damp;    // 14 type, name 7, 791.
    float      timeoffs;// 15 type, name 7, 793.
    float      lifetime;// 16 type, name 7, 794.
};

// 136 DNA structure 266, 5
struct TreeStoreElem
{
    short    type;// 1 type, name 2, 16.
    short    nr;  // 2 type, name 2, 2597.
    short    flag;// 3 type, name 2, 18.
    short    used;// 4 type, name 2, 2598.
    ID       *id; // 5 type, name 22, 1962.
};

// 137 DNA structure 354, 3
struct TreeStore
{
    int              totelem; // 1 type, name 4, 70.
    int              usedelem;// 2 type, name 4, 2599.
    TreeStoreElem    *data;   // 3 type, name 266, 2.
};

// 138 DNA structure 355, 7
struct bProperty
{
    bProperty    *next;   // 1 type, name 355, 0.
    bProperty    *prev;   // 2 type, name 355, 1.
    char         name[64];// 3 type, name 0, 19.
    short        type;    // 4 type, name 2, 16.
    short        flag;    // 5 type, name 2, 18.
    int          data;    // 6 type, name 4, 21.
    void         *poin;   // 7 type, name 11, 2600.
};

// 139 DNA structure 356, 5
struct bNearSensor
{
    char     name[64]; // 1 type, name 0, 19.
    float    dist;     // 2 type, name 7, 341.
    float    resetdist;// 3 type, name 7, 2601.
    int      lastval;  // 4 type, name 4, 2602.
    int      pad;      // 5 type, name 4, 66.
};

// 140 DNA structure 357, 6
struct bMouseSensor
{
    short    type;        // 1 type, name 2, 16.
    short    flag;        // 2 type, name 2, 18.
    short    pad1;        // 3 type, name 2, 68.
    short    mode;        // 4 type, name 2, 334.
    char     propname[64];// 5 type, name 0, 2603.
    char     matname[64]; // 6 type, name 0, 2604.
};

// 141 DNA structure 358, 4
struct bTouchSensor
{
    char        name[64];// 1 type, name 0, 19.
    Material    *ma;     // 2 type, name 65, 2605.
    float       dist;    // 3 type, name 7, 341.
    float       pad;     // 4 type, name 7, 66.
};

// 142 DNA structure 359, 6
struct bKeyboardSensor
{
    short    key;           // 1 type, name 2, 2606.
    short    qual;          // 2 type, name 2, 2607.
    short    type;          // 3 type, name 2, 16.
    short    qual2;         // 4 type, name 2, 2608.
    char     targetName[64];// 5 type, name 0, 2609.
    char     toggleName[64];// 6 type, name 0, 2610.
};

// 143 DNA structure 360, 5
struct bPropertySensor
{
    int     type;        // 1 type, name 4, 16.
    int     pad;         // 2 type, name 4, 66.
    char    name[64];    // 3 type, name 0, 19.
    char    value[64];   // 4 type, name 0, 2611.
    char    maxvalue[64];// 5 type, name 0, 2612.
};

// 144 DNA structure 361, 3
struct bActuatorSensor
{
    int     type;    // 1 type, name 4, 16.
    int     pad;     // 2 type, name 4, 66.
    char    name[64];// 3 type, name 0, 19.
};

// 145 DNA structure 362, 4
struct bDelaySensor
{
    short    delay;   // 1 type, name 2, 2613.
    short    duration;// 2 type, name 2, 2614.
    short    flag;    // 3 type, name 2, 18.
    short    pad;     // 4 type, name 2, 66.
};

// 146 DNA structure 363, 6
struct bCollisionSensor
{
    char     name[64];        // 1 type, name 0, 19.
    char     materialName[64];// 2 type, name 0, 2615.
    short    damptimer;       // 3 type, name 2, 2616.
    short    damp;            // 4 type, name 2, 791.
    short    mode;            // 5 type, name 2, 334.
    short    pad2;            // 6 type, name 2, 29.
};

// 147 DNA structure 364, 5
struct bRadarSensor
{
    char     name[64];// 1 type, name 0, 19.
    float    angle;   // 2 type, name 7, 781.
    float    range;   // 3 type, name 7, 1298.
    short    flag;    // 4 type, name 2, 18.
    short    axis;    // 5 type, name 2, 752.
};

// 148 DNA structure 365, 3
struct bRandomSensor
{
    char    name[64];// 1 type, name 0, 19.
    int     seed;    // 2 type, name 4, 740.
    int     delay;   // 3 type, name 4, 2613.
};

// 149 DNA structure 366, 7
struct bRaySensor
{
    char     name[64];    // 1 type, name 0, 19.
    float    range;       // 2 type, name 7, 1298.
    char     propname[64];// 3 type, name 0, 2603.
    char     matname[64]; // 4 type, name 0, 2604.
    short    mode;        // 5 type, name 2, 334.
    short    pad1;        // 6 type, name 2, 68.
    int      axisflag;    // 7 type, name 4, 2617.
};

// 150 DNA structure 367, 4
struct bArmatureSensor
{
    char     posechannel[64];// 1 type, name 0, 2618.
    char     constraint[64]; // 2 type, name 0, 2619.
    int      type;           // 3 type, name 4, 16.
    float    value;          // 4 type, name 7, 756.
};

// 151 DNA structure 368, 3
struct bMessageSensor
{
    Object    *fromObject;// 1 type, name 29, 2620.
    char      subject[64];// 2 type, name 0, 2621.
    char      body[64];   // 3 type, name 0, 2622.
};

// 152 DNA structure 369, 18
struct bSensor
{
    bSensor        *next;   // 1 type, name 369, 0.
    bSensor        *prev;   // 2 type, name 369, 1.
    short          type;    // 3 type, name 2, 16.
    short          otype;   // 4 type, name 2, 2623.
    short          flag;    // 5 type, name 2, 18.
    short          pulse;   // 6 type, name 2, 2624.
    short          freq;    // 7 type, name 2, 2625.
    short          totlinks;// 8 type, name 2, 2626.
    short          pad1;    // 9 type, name 2, 68.
    short          pad2;    // 10 type, name 2, 29.
    char           name[64];// 11 type, name 0, 19.
    void           *data;   // 12 type, name 11, 2.
    bController    **links; // 13 type, name 370, 2627.
    Object         *ob;     // 14 type, name 29, 44.
    short          invert;  // 15 type, name 2, 1587.
    short          level;   // 16 type, name 2, 701.
    short          tap;     // 17 type, name 2, 2628.
    short          pad;     // 18 type, name 2, 66.
};

// 153 DNA structure 371, 11
struct bJoystickSensor
{
    char     name[64];   // 1 type, name 0, 19.
    char     type;       // 2 type, name 0, 16.
    char     joyindex;   // 3 type, name 0, 2629.
    short    flag;       // 4 type, name 2, 18.
    short    axis;       // 5 type, name 2, 752.
    short    axis_single;// 6 type, name 2, 2630.
    int      axisf;      // 7 type, name 4, 2631.
    int      button;     // 8 type, name 4, 2632.
    int      hat;        // 9 type, name 4, 2633.
    int      hatf;       // 10 type, name 4, 2634.
    int      precision;  // 11 type, name 4, 2635.
};

// 154 DNA structure 372, 1
struct bExpressionCont
{
    char    str[128];// 1 type, name 0, 2636.
};

// 155 DNA structure 373, 4
struct bPythonCont
{
    Text    *text;     // 1 type, name 38, 1924.
    char    module[64];// 2 type, name 0, 2264.
    int     mode;      // 3 type, name 4, 334.
    int     flag;      // 4 type, name 4, 18.
};

// 156 DNA structure 370, 18
struct bController
{
    bController    *next;     // 1 type, name 370, 0.
    bController    *prev;     // 2 type, name 370, 1.
    bController    *mynew;    // 3 type, name 370, 2637.
    short          type;      // 4 type, name 2, 16.
    short          flag;      // 5 type, name 2, 18.
    short          inputs;    // 6 type, name 2, 2638.
    short          totlinks;  // 7 type, name 2, 2626.
    short          otype;     // 8 type, name 2, 2623.
    short          totslinks; // 9 type, name 2, 2639.
    short          pad2;      // 10 type, name 2, 29.
    short          pad3;      // 11 type, name 2, 500.
    char           name[64];  // 12 type, name 0, 19.
    void           *data;     // 13 type, name 11, 2.
    bActuator      **links;   // 14 type, name 374, 2627.
    bSensor        **slinks;  // 15 type, name 369, 2640.
    short          val;       // 16 type, name 2, 14.
    short          valo;      // 17 type, name 2, 2641.
    int            state_mask;// 18 type, name 4, 2642.
};

// 157 DNA structure 375, 3
struct bAddObjectActuator
{
    int       time;// 1 type, name 4, 768.
    int       pad; // 2 type, name 4, 66.
    Object    *ob; // 3 type, name 29, 44.
};

// 158 DNA structure 376, 15
struct bActionActuator
{
    bAction    *act;         // 1 type, name 186, 2643.
    short      type;         // 2 type, name 2, 16.
    short      flag;         // 3 type, name 2, 18.
    float      sta;          // 4 type, name 7, 2569.
    float      end;          // 5 type, name 7, 2570.
    char       name[64];     // 6 type, name 0, 19.
    char       frameProp[64];// 7 type, name 0, 2644.
    short      blendin;      // 8 type, name 2, 2645.
    short      priority;     // 9 type, name 2, 2646.
    short      layer;        // 10 type, name 2, 128.
    short      end_reset;    // 11 type, name 2, 2647.
    short      strideaxis;   // 12 type, name 2, 2648.
    short      blend_mode;   // 13 type, name 2, 2520.
    float      stridelength; // 14 type, name 7, 2649.
    float      layer_weight; // 15 type, name 7, 2650.
};

// 159 DNA structure 377, 8
struct Sound3D
{
    float    min_gain;          // 1 type, name 7, 2651.
    float    max_gain;          // 2 type, name 7, 2652.
    float    reference_distance;// 3 type, name 7, 2653.
    float    max_distance;      // 4 type, name 7, 2654.
    float    rolloff_factor;    // 5 type, name 7, 2655.
    float    cone_inner_angle;  // 6 type, name 7, 2656.
    float    cone_outer_angle;  // 7 type, name 7, 2657.
    float    cone_outer_gain;   // 8 type, name 7, 2658.
};

// 160 DNA structure 379, 14
struct bEditObjectActuator
{
    int       time;          // 1 type, name 4, 768.
    short     type;          // 2 type, name 2, 16.
    short     flag;          // 3 type, name 2, 18.
    Object    *ob;           // 4 type, name 29, 44.
    Mesh      *me;           // 5 type, name 79, 2663.
    char      name[64];      // 6 type, name 0, 19.
    float     linVelocity[3];// 7 type, name 7, 2664.
    float     angVelocity[3];// 8 type, name 7, 2665.
    float     mass;          // 9 type, name 7, 1036.
    short     localflag;     // 10 type, name 2, 2666.
    short     dyn_operation; // 11 type, name 2, 2667.
    short     upflag;        // 12 type, name 2, 1026.
    short     trackflag;     // 13 type, name 2, 1025.
    int       pad;           // 14 type, name 4, 66.
};

// 161 DNA structure 380, 5
struct bSceneActuator
{
    short     type;   // 1 type, name 2, 16.
    short     pad1;   // 2 type, name 2, 68.
    int       pad;    // 3 type, name 4, 66.
    Scene     *scene; // 4 type, name 41, 119.
    Object    *camera;// 5 type, name 29, 1572.
};

// 162 DNA structure 381, 5
struct bPropertyActuator
{
    int       pad;      // 1 type, name 4, 66.
    int       type;     // 2 type, name 4, 16.
    char      name[64]; // 3 type, name 0, 19.
    char      value[64];// 4 type, name 0, 2611.
    Object    *ob;      // 5 type, name 29, 44.
};

// 163 DNA structure 382, 13
struct bObjectActuator
{
    short     flag;              // 1 type, name 2, 18.
    short     type;              // 2 type, name 2, 16.
    short     otype;             // 3 type, name 2, 2623.
    short     damping;           // 4 type, name 2, 1037.
    float     forceloc[3];       // 5 type, name 7, 2668.
    float     forcerot[3];       // 6 type, name 7, 2669.
    float     pad[3];            // 7 type, name 7, 333.
    float     pad1[3];           // 8 type, name 7, 2431.
    float     dloc[3];           // 9 type, name 7, 1008.
    float     drot[3];           // 10 type, name 7, 1012.
    float     linearvelocity[3]; // 11 type, name 7, 2670.
    float     angularvelocity[3];// 12 type, name 7, 2671.
    Object    *reference;        // 13 type, name 29, 2672.
};

// 164 DNA structure 383, 10
struct bIpoActuator
{
    short    flag;         // 1 type, name 2, 18.
    short    type;         // 2 type, name 2, 16.
    float    sta;          // 3 type, name 7, 2569.
    float    end;          // 4 type, name 7, 2570.
    char     name[64];     // 5 type, name 0, 19.
    char     frameProp[64];// 6 type, name 0, 2644.
    short    pad1;         // 7 type, name 2, 68.
    short    pad2;         // 8 type, name 2, 29.
    short    pad3;         // 9 type, name 2, 500.
    short    pad4;         // 10 type, name 2, 1468.
};

// 165 DNA structure 384, 8
struct bCameraActuator
{
    Object    *ob;    // 1 type, name 29, 44.
    float     height; // 2 type, name 7, 788.
    float     min;    // 3 type, name 7, 2673.
    float     max;    // 4 type, name 7, 2674.
    float     damping;// 5 type, name 7, 1037.
    short     pad1;   // 6 type, name 2, 68.
    short     axis;   // 7 type, name 2, 752.
    float     pad2;   // 8 type, name 7, 29.
};

// 166 DNA structure 385, 12
struct bConstraintActuator
{
    short    type;       // 1 type, name 2, 16.
    short    mode;       // 2 type, name 2, 334.
    short    flag;       // 3 type, name 2, 18.
    short    damp;       // 4 type, name 2, 791.
    short    time;       // 5 type, name 2, 768.
    short    rotdamp;    // 6 type, name 2, 2675.
    int      pad;        // 7 type, name 4, 66.
    float    minloc[3];  // 8 type, name 7, 2676.
    float    maxloc[3];  // 9 type, name 7, 2677.
    float    minrot[3];  // 10 type, name 7, 2678.
    float    maxrot[3];  // 11 type, name 7, 2679.
    char     matprop[64];// 12 type, name 0, 2680.
};

// 167 DNA structure 386, 9
struct bGroupActuator
{
    short    flag;    // 1 type, name 2, 18.
    short    type;    // 2 type, name 2, 16.
    int      sta;     // 3 type, name 4, 2569.
    int      end;     // 4 type, name 4, 2570.
    char     name[64];// 5 type, name 0, 19.
    short    pad[3];  // 6 type, name 2, 333.
    short    cur;     // 7 type, name 2, 63.
    short    butsta;  // 8 type, name 2, 2681.
    short    butend;  // 9 type, name 2, 2682.
};

// 168 DNA structure 387, 7
struct bRandomActuator
{
    int      seed;        // 1 type, name 4, 740.
    int      distribution;// 2 type, name 4, 2683.
    int      int_arg_1;   // 3 type, name 4, 2684.
    int      int_arg_2;   // 4 type, name 4, 2685.
    float    float_arg_1; // 5 type, name 7, 2686.
    float    float_arg_2; // 6 type, name 7, 2687.
    char     propname[64];// 7 type, name 0, 2603.
};

// 169 DNA structure 388, 7
struct bMessageActuator
{
    char      toPropName[64];// 1 type, name 0, 2688.
    Object    *toObject;     // 2 type, name 29, 2689.
    char      subject[64];   // 3 type, name 0, 2621.
    short     bodyType;      // 4 type, name 2, 2690.
    short     pad1;          // 5 type, name 2, 68.
    int       pad2;          // 6 type, name 4, 29.
    char      body[64];      // 7 type, name 0, 2622.
};

// 170 DNA structure 389, 6
struct bGameActuator
{
    short    flag;           // 1 type, name 2, 18.
    short    type;           // 2 type, name 2, 16.
    int      sta;            // 3 type, name 4, 2569.
    int      end;            // 4 type, name 4, 2570.
    char     filename[64];   // 5 type, name 0, 2691.
    char     loadaniname[64];// 6 type, name 0, 2692.
};

// 171 DNA structure 390, 1
struct bVisibilityActuator
{
    int    flag;// 1 type, name 4, 18.
};

// 172 DNA structure 391, 6
struct bTwoDFilterActuator
{
    char     pad[4];   // 1 type, name 0, 568.
    short    type;     // 2 type, name 2, 16.
    short    flag;     // 3 type, name 2, 18.
    int      int_arg;  // 4 type, name 4, 2693.
    float    float_arg;// 5 type, name 7, 2694.
    Text     *text;    // 6 type, name 38, 1924.
};

// 173 DNA structure 392, 4
struct bParentActuator
{
    char      pad[2];// 1 type, name 0, 573.
    short     flag;  // 2 type, name 2, 18.
    int       type;  // 3 type, name 4, 16.
    Object    *ob;   // 4 type, name 29, 44.
};

// 174 DNA structure 393, 2
struct bStateActuator
{
    int    type;// 1 type, name 4, 16.
    int    mask;// 2 type, name 4, 1830.
};

// 175 DNA structure 394, 8
struct bArmatureActuator
{
    char      posechannel[64];// 1 type, name 0, 2618.
    char      constraint[64]; // 2 type, name 0, 2619.
    int       type;           // 3 type, name 4, 16.
    float     weight;         // 4 type, name 7, 557.
    float     influence;      // 5 type, name 7, 2695.
    float     pad;            // 6 type, name 7, 66.
    Object    *target;        // 7 type, name 29, 856.
    Object    *subtarget;     // 8 type, name 29, 2696.
};

// 176 DNA structure 395, 11
struct bSteeringActuator
{
    char      pad[5];      // 1 type, name 0, 2697.
    char      flag;        // 2 type, name 0, 18.
    short     facingaxis;  // 3 type, name 2, 2698.
    int       type;        // 4 type, name 4, 16.
    float     dist;        // 5 type, name 7, 341.
    float     velocity;    // 6 type, name 7, 2699.
    float     acceleration;// 7 type, name 7, 2700.
    float     turnspeed;   // 8 type, name 7, 2701.
    int       updateTime;  // 9 type, name 4, 2702.
    Object    *target;     // 10 type, name 29, 856.
    Object    *navmesh;    // 11 type, name 29, 2703.
};

// 177 DNA structure 396, 7
struct bMouseActuator
{
    short    type;          // 1 type, name 2, 16.
    short    flag;          // 2 type, name 2, 18.
    int      object_axis[2];// 3 type, name 4, 2704.
    float    threshold[2];  // 4 type, name 7, 2705.
    float    sensitivity[2];// 5 type, name 7, 2706.
    float    limit_x[2];    // 6 type, name 7, 2707.
    float    limit_y[2];    // 7 type, name 7, 2708.
};

// 178 DNA structure 374, 10
struct bActuator
{
    bActuator    *next;   // 1 type, name 374, 0.
    bActuator    *prev;   // 2 type, name 374, 1.
    bActuator    *mynew;  // 3 type, name 374, 2637.
    short        type;    // 4 type, name 2, 16.
    short        flag;    // 5 type, name 2, 18.
    short        otype;   // 6 type, name 2, 2623.
    short        go;      // 7 type, name 2, 2709.
    char         name[64];// 8 type, name 0, 19.
    void         *data;   // 9 type, name 11, 2.
    Object       *ob;     // 10 type, name 29, 44.
};

// 179 DNA structure 397, 6
struct GroupObject
{
    GroupObject    *next;   // 1 type, name 397, 0.
    GroupObject    *prev;   // 2 type, name 397, 1.
    Object         *ob;     // 3 type, name 29, 44.
    void           *lampren;// 4 type, name 11, 2716.
    short          recalc;  // 5 type, name 2, 241.
    char           pad[6];  // 6 type, name 0, 736.
};

// 180 DNA structure 401, 2
struct bMotionPathVert
{
    float    co[3];// 1 type, name 7, 690.
    int      flag; // 2 type, name 4, 18.
};

// 181 DNA structure 190, 5
struct bMotionPath
{
    bMotionPathVert    *points;    // 1 type, name 401, 2754.
    int                length;     // 2 type, name 4, 738.
    int                start_frame;// 3 type, name 4, 2755.
    int                end_frame;  // 4 type, name 4, 2756.
    int                flag;       // 5 type, name 4, 18.
};

// 182 DNA structure 189, 16
struct bAnimVizSettings
{
    int      ghost_sf;     // 1 type, name 4, 2757.
    int      ghost_ef;     // 2 type, name 4, 2758.
    int      ghost_bc;     // 3 type, name 4, 2759.
    int      ghost_ac;     // 4 type, name 4, 2760.
    short    ghost_type;   // 5 type, name 2, 2761.
    short    ghost_step;   // 6 type, name 2, 2762.
    short    ghost_flag;   // 7 type, name 2, 2763.
    short    recalc;       // 8 type, name 2, 241.
    short    path_type;    // 9 type, name 2, 2764.
    short    path_step;    // 10 type, name 2, 2765.
    short    path_viewflag;// 11 type, name 2, 2766.
    short    path_bakeflag;// 12 type, name 2, 2767.
    int      path_sf;      // 13 type, name 4, 2768.
    int      path_ef;      // 14 type, name 4, 2769.
    int      path_bc;      // 15 type, name 4, 2770.
    int      path_ac;      // 16 type, name 4, 2771.
};

// 183 DNA structure 403, 1
struct bIKParam
{
    int    iksolver;// 1 type, name 4, 2802.
};

// 184 DNA structure 404, 12
struct bItasc
{
    int      iksolver; // 1 type, name 4, 2802.
    float    precision;// 2 type, name 7, 2635.
    short    numiter;  // 3 type, name 2, 2806.
    short    numstep;  // 4 type, name 2, 2807.
    float    minstep;  // 5 type, name 7, 2808.
    float    maxstep;  // 6 type, name 7, 2809.
    short    solver;   // 7 type, name 2, 2810.
    short    flag;     // 8 type, name 2, 18.
    float    feedback; // 9 type, name 7, 2811.
    float    maxvel;   // 10 type, name 7, 2812.
    float    dampmax;  // 11 type, name 7, 2813.
    float    dampeps;  // 12 type, name 7, 2814.
};

// 185 DNA structure 408, 5
struct bConstraintChannel
{
    bConstraintChannel    *next;   // 1 type, name 408, 0.
    bConstraintChannel    *prev;   // 2 type, name 408, 1.
    Ipo                   *ipo;    // 3 type, name 33, 80.
    short                 flag;    // 4 type, name 2, 18.
    char                  name[30];// 5 type, name 0, 2829.
};

// 186 DNA structure 409, 14
struct bConstraint
{
    bConstraint    *next;    // 1 type, name 409, 0.
    bConstraint    *prev;    // 2 type, name 409, 1.
    void           *data;    // 3 type, name 11, 2.
    short          type;     // 4 type, name 2, 16.
    short          flag;     // 5 type, name 2, 18.
    char           ownspace; // 6 type, name 0, 2830.
    char           tarspace; // 7 type, name 0, 2831.
    char           name[64]; // 8 type, name 0, 19.
    short          pad;      // 9 type, name 2, 66.
    float          enforce;  // 10 type, name 7, 2832.
    float          headtail; // 11 type, name 7, 2833.
    Ipo            *ipo;     // 12 type, name 33, 80.
    float          lin_error;// 13 type, name 7, 2834.
    float          rot_error;// 14 type, name 7, 2835.
};

// 187 DNA structure 410, 9
struct bConstraintTarget
{
    bConstraintTarget    *next;        // 1 type, name 410, 0.
    bConstraintTarget    *prev;        // 2 type, name 410, 1.
    Object               *tar;         // 3 type, name 29, 2836.
    char                 subtarget[64];// 4 type, name 0, 798.
    float                matrix[4][4]; // 5 type, name 7, 2837.
    short                space;        // 6 type, name 2, 2838.
    short                flag;         // 7 type, name 2, 18.
    short                type;         // 8 type, name 2, 16.
    short                rotOrder;     // 9 type, name 2, 2839.
};

// 188 DNA structure 412, 15
struct bKinematicConstraint
{
    Object    *tar;             // 1 type, name 29, 2836.
    short     iterations;       // 2 type, name 2, 2842.
    short     flag;             // 3 type, name 2, 18.
    short     rootbone;         // 4 type, name 2, 2843.
    short     max_rootbone;     // 5 type, name 2, 2844.
    char      subtarget[64];    // 6 type, name 0, 798.
    Object    *poletar;         // 7 type, name 29, 2845.
    char      polesubtarget[64];// 8 type, name 0, 2846.
    float     poleangle;        // 9 type, name 7, 2847.
    float     weight;           // 10 type, name 7, 557.
    float     orientweight;     // 11 type, name 7, 2848.
    float     grabtarget[3];    // 12 type, name 7, 2849.
    short     type;             // 13 type, name 2, 16.
    short     mode;             // 14 type, name 2, 334.
    float     dist;             // 15 type, name 7, 341.
};

// 189 DNA structure 413, 6
struct bSplineIKConstraint
{
    Object    *tar;       // 1 type, name 29, 2836.
    float     *points;    // 2 type, name 7, 2754.
    short     numpoints;  // 3 type, name 2, 2850.
    short     chainlen;   // 4 type, name 2, 2851.
    short     flag;       // 5 type, name 2, 18.
    short     xzScaleMode;// 6 type, name 2, 2852.
};

// 190 DNA structure 414, 6
struct bTrackToConstraint
{
    Object    *tar;         // 1 type, name 29, 2836.
    int       reserved1;    // 2 type, name 4, 2853.
    int       reserved2;    // 3 type, name 4, 2854.
    int       flags;        // 4 type, name 4, 90.
    int       pad;          // 5 type, name 4, 66.
    char      subtarget[64];// 6 type, name 0, 798.
};

// 191 DNA structure 415, 4
struct bRotateLikeConstraint
{
    Object    *tar;         // 1 type, name 29, 2836.
    int       flag;         // 2 type, name 4, 18.
    int       reserved1;    // 3 type, name 4, 2853.
    char      subtarget[64];// 4 type, name 0, 798.
};

// 192 DNA structure 416, 4
struct bLocateLikeConstraint
{
    Object    *tar;         // 1 type, name 29, 2836.
    int       flag;         // 2 type, name 4, 18.
    int       reserved1;    // 3 type, name 4, 2853.
    char      subtarget[64];// 4 type, name 0, 798.
};

// 193 DNA structure 417, 4
struct bSizeLikeConstraint
{
    Object    *tar;         // 1 type, name 29, 2836.
    int       flag;         // 2 type, name 4, 18.
    int       reserved1;    // 3 type, name 4, 2853.
    char      subtarget[64];// 4 type, name 0, 798.
};

// 194 DNA structure 418, 2
struct bSameVolumeConstraint
{
    int      flag;  // 1 type, name 4, 18.
    float    volume;// 2 type, name 7, 1165.
};

// 195 DNA structure 419, 2
struct bTransLikeConstraint
{
    Object    *tar;         // 1 type, name 29, 2836.
    char      subtarget[64];// 2 type, name 0, 798.
};

// 196 DNA structure 420, 10
struct bMinMaxConstraint
{
    Object    *tar;         // 1 type, name 29, 2836.
    int       minmaxflag;   // 2 type, name 4, 2855.
    float     offset;       // 3 type, name 7, 122.
    int       flag;         // 4 type, name 4, 18.
    short     sticky;       // 5 type, name 2, 1921.
    short     stuck;        // 6 type, name 2, 2856.
    short     pad1;         // 7 type, name 2, 68.
    short     pad2;         // 8 type, name 2, 29.
    float     cache[3];     // 9 type, name 7, 2857.
    char      subtarget[64];// 10 type, name 0, 798.
};

// 197 DNA structure 421, 10
struct bActionConstraint
{
    Object     *tar;         // 1 type, name 29, 2836.
    short      type;         // 2 type, name 2, 16.
    short      local;        // 3 type, name 2, 1215.
    int        start;        // 4 type, name 4, 737.
    int        end;          // 5 type, name 4, 2570.
    float      min;          // 6 type, name 7, 2673.
    float      max;          // 7 type, name 7, 2674.
    int        flag;         // 8 type, name 4, 18.
    bAction    *act;         // 9 type, name 186, 2643.
    char       subtarget[64];// 10 type, name 0, 798.
};

// 198 DNA structure 422, 4
struct bLockTrackConstraint
{
    Object    *tar;         // 1 type, name 29, 2836.
    int       trackflag;    // 2 type, name 4, 1025.
    int       lockflag;     // 3 type, name 4, 2858.
    char      subtarget[64];// 4 type, name 0, 798.
};

// 199 DNA structure 423, 4
struct bDampTrackConstraint
{
    Object    *tar;         // 1 type, name 29, 2836.
    int       trackflag;    // 2 type, name 4, 1025.
    int       pad;          // 3 type, name 4, 66.
    char      subtarget[64];// 4 type, name 0, 798.
};

// 200 DNA structure 424, 6
struct bFollowPathConstraint
{
    Object    *tar;      // 1 type, name 29, 2836.
    float     offset;    // 2 type, name 7, 122.
    float     offset_fac;// 3 type, name 7, 868.
    int       followflag;// 4 type, name 4, 2859.
    short     trackflag; // 5 type, name 2, 1025.
    short     upflag;    // 6 type, name 2, 1026.
};

// 201 DNA structure 425, 10
struct bStretchToConstraint
{
    Object    *tar;         // 1 type, name 29, 2836.
    int       flag;         // 2 type, name 4, 18.
    int       volmode;      // 3 type, name 4, 2860.
    int       plane;        // 4 type, name 4, 2861.
    float     orglength;    // 5 type, name 7, 2862.
    float     bulge;        // 6 type, name 7, 2863.
    float     bulge_min;    // 7 type, name 7, 2864.
    float     bulge_max;    // 8 type, name 7, 2865.
    float     bulge_smooth; // 9 type, name 7, 2866.
    char      subtarget[64];// 10 type, name 0, 798.
};

// 202 DNA structure 426, 16
struct bRigidBodyJointConstraint
{
    Object    *tar;       // 1 type, name 29, 2836.
    Object    *child;     // 2 type, name 29, 2778.
    int       type;       // 3 type, name 4, 16.
    float     pivX;       // 4 type, name 7, 2867.
    float     pivY;       // 5 type, name 7, 2868.
    float     pivZ;       // 6 type, name 7, 2869.
    float     axX;        // 7 type, name 7, 2870.
    float     axY;        // 8 type, name 7, 2871.
    float     axZ;        // 9 type, name 7, 2872.
    float     minLimit[6];// 10 type, name 7, 2873.
    float     maxLimit[6];// 11 type, name 7, 2874.
    float     extraFz;    // 12 type, name 7, 2875.
    short     flag;       // 13 type, name 2, 18.
    short     pad;        // 14 type, name 2, 66.
    short     pad1;       // 15 type, name 2, 68.
    short     pad2;       // 16 type, name 2, 29.
};

// 203 DNA structure 427, 3
struct bClampToConstraint
{
    Object    *tar; // 1 type, name 29, 2836.
    int       flag; // 2 type, name 4, 18.
    int       flag2;// 3 type, name 4, 549.
};

// 204 DNA structure 428, 5
struct bChildOfConstraint
{
    Object    *tar;         // 1 type, name 29, 2836.
    int       flag;         // 2 type, name 4, 18.
    int       pad;          // 3 type, name 4, 66.
    float     invmat[4][4]; // 4 type, name 7, 2876.
    char      subtarget[64];// 5 type, name 0, 798.
};

// 205 DNA structure 429, 18
struct bTransformConstraint
{
    Object    *tar;             // 1 type, name 29, 2836.
    char      subtarget[64];    // 2 type, name 0, 798.
    short     from;             // 3 type, name 2, 2877.
    short     to;               // 4 type, name 2, 2878.
    char      map[3];           // 5 type, name 0, 2879.
    char      expo;             // 6 type, name 0, 2880.
    float     from_min[3];      // 7 type, name 7, 2881.
    float     from_max[3];      // 8 type, name 7, 2882.
    float     to_min[3];        // 9 type, name 7, 2883.
    float     to_max[3];        // 10 type, name 7, 2884.
    float     from_min_rot[3];  // 11 type, name 7, 2885.
    float     from_max_rot[3];  // 12 type, name 7, 2886.
    float     to_min_rot[3];    // 13 type, name 7, 2887.
    float     to_max_rot[3];    // 14 type, name 7, 2888.
    float     from_min_scale[3];// 15 type, name 7, 2889.
    float     from_max_scale[3];// 16 type, name 7, 2890.
    float     to_min_scale[3];  // 17 type, name 7, 2891.
    float     to_max_scale[3];  // 18 type, name 7, 2892.
};

// 206 DNA structure 430, 5
struct bPivotConstraint
{
    Object    *tar;         // 1 type, name 29, 2836.
    char      subtarget[64];// 2 type, name 0, 798.
    float     offset[3];    // 3 type, name 7, 746.
    short     rotAxis;      // 4 type, name 2, 2893.
    short     flag;         // 5 type, name 2, 18.
};

// 207 DNA structure 431, 8
struct bLocLimitConstraint
{
    float    xmin; // 1 type, name 7, 8.
    float    xmax; // 2 type, name 7, 9.
    float    ymin; // 3 type, name 7, 10.
    float    ymax; // 4 type, name 7, 11.
    float    zmin; // 5 type, name 7, 2894.
    float    zmax; // 6 type, name 7, 2895.
    short    flag; // 7 type, name 2, 18.
    short    flag2;// 8 type, name 2, 549.
};

// 208 DNA structure 432, 8
struct bRotLimitConstraint
{
    float    xmin; // 1 type, name 7, 8.
    float    xmax; // 2 type, name 7, 9.
    float    ymin; // 3 type, name 7, 10.
    float    ymax; // 4 type, name 7, 11.
    float    zmin; // 5 type, name 7, 2894.
    float    zmax; // 6 type, name 7, 2895.
    short    flag; // 7 type, name 2, 18.
    short    flag2;// 8 type, name 2, 549.
};

// 209 DNA structure 433, 8
struct bSizeLimitConstraint
{
    float    xmin; // 1 type, name 7, 8.
    float    xmax; // 2 type, name 7, 9.
    float    ymin; // 3 type, name 7, 10.
    float    ymax; // 4 type, name 7, 11.
    float    zmin; // 5 type, name 7, 2894.
    float    zmax; // 6 type, name 7, 2895.
    short    flag; // 7 type, name 2, 18.
    short    flag2;// 8 type, name 2, 549.
};

// 210 DNA structure 434, 7
struct bDistLimitConstraint
{
    Object    *tar;         // 1 type, name 29, 2836.
    char      subtarget[64];// 2 type, name 0, 798.
    float     dist;         // 3 type, name 7, 341.
    float     soft;         // 4 type, name 7, 349.
    short     flag;         // 5 type, name 2, 18.
    short     mode;         // 6 type, name 2, 334.
    int       pad;          // 7 type, name 4, 66.
};

// 211 DNA structure 435, 7
struct bShrinkwrapConstraint
{
    Object    *target;      // 1 type, name 29, 856.
    float     dist;         // 2 type, name 7, 341.
    short     shrinkType;   // 3 type, name 2, 860.
    char      projAxis;     // 4 type, name 0, 863.
    char      projAxisSpace;// 5 type, name 0, 2896.
    float     projLimit;    // 6 type, name 7, 862.
    char      pad[4];       // 7 type, name 0, 568.
};

// 212 DNA structure 436, 7
struct bFollowTrackConstraint
{
    MovieClip    *clip;       // 1 type, name 249, 1749.
    char         track[64];   // 2 type, name 0, 2897.
    int          flag;        // 3 type, name 4, 18.
    int          frame_method;// 4 type, name 4, 2898.
    char         object[64];  // 5 type, name 0, 2899.
    Object       *camera;     // 6 type, name 29, 1572.
    Object       *depth_ob;   // 7 type, name 29, 2900.
};

// 213 DNA structure 437, 3
struct bCameraSolverConstraint
{
    MovieClip    *clip;// 1 type, name 249, 1749.
    int          flag; // 2 type, name 4, 18.
    int          pad;  // 3 type, name 4, 66.
};

// 214 DNA structure 438, 6
struct bObjectSolverConstraint
{
    MovieClip    *clip;       // 1 type, name 249, 1749.
    int          flag;        // 2 type, name 4, 18.
    int          pad;         // 3 type, name 4, 66.
    char         object[64];  // 4 type, name 0, 2899.
    float        invmat[4][4];// 5 type, name 7, 2876.
    Object       *camera;     // 6 type, name 29, 1572.
};

// 215 DNA structure 439, 10
struct bActionModifier
{
    bActionModifier    *next;      // 1 type, name 439, 0.
    bActionModifier    *prev;      // 2 type, name 439, 1.
    short              type;       // 3 type, name 2, 16.
    short              flag;       // 4 type, name 2, 18.
    char               channel[32];// 5 type, name 0, 2901.
    float              noisesize;  // 6 type, name 7, 279.
    float              turbul;     // 7 type, name 7, 280.
    short              channels;   // 8 type, name 2, 2815.
    short              no_rot_axis;// 9 type, name 2, 2902.
    Object             *ob;        // 10 type, name 29, 44.
};

// 216 DNA structure 441, 11
struct bNodeStack
{
    float    vec[4];    // 1 type, name 7, 569.
    float    min;       // 2 type, name 7, 2673.
    float    max;       // 3 type, name 7, 2674.
    void     *data;     // 4 type, name 11, 2.
    short    hasinput;  // 5 type, name 2, 2912.
    short    hasoutput; // 6 type, name 2, 2913.
    short    datatype;  // 7 type, name 2, 2914.
    short    sockettype;// 8 type, name 2, 2915.
    short    is_copy;   // 9 type, name 2, 2916.
    short    external;  // 10 type, name 2, 2917.
    short    pad[2];    // 11 type, name 2, 573.
};

// 217 DNA structure 288, 1
struct bNodeInstanceKey
{
    int    value;// 1 type, name 4, 756.
};

// 218 DNA structure 444, 8
struct bNodeLink
{
    bNodeLink      *next;    // 1 type, name 444, 0.
    bNodeLink      *prev;    // 2 type, name 444, 1.
    bNode          *fromnode;// 3 type, name 445, 2963.
    bNode          *tonode;  // 4 type, name 445, 2964.
    bNodeSocket    *fromsock;// 5 type, name 442, 2965.
    bNodeSocket    *tosock;  // 6 type, name 442, 2966.
    int            flag;     // 7 type, name 4, 18.
    int            pad;      // 8 type, name 4, 66.
};

// 219 DNA structure 454, 4
struct bNodeSocketValueInt
{
    int    subtype;// 1 type, name 4, 17.
    int    value;  // 2 type, name 4, 756.
    int    min;    // 3 type, name 4, 2673.
    int    max;    // 4 type, name 4, 2674.
};

// 220 DNA structure 455, 4
struct bNodeSocketValueFloat
{
    int      subtype;// 1 type, name 4, 17.
    float    value;  // 2 type, name 7, 756.
    float    min;    // 3 type, name 7, 2673.
    float    max;    // 4 type, name 7, 2674.
};

// 221 DNA structure 456, 2
struct bNodeSocketValueBoolean
{
    char    value; // 1 type, name 0, 756.
    char    pad[3];// 2 type, name 0, 333.
};

// 222 DNA structure 457, 4
struct bNodeSocketValueVector
{
    int      subtype; // 1 type, name 4, 17.
    float    value[3];// 2 type, name 7, 2989.
    float    min;     // 3 type, name 7, 2673.
    float    max;     // 4 type, name 7, 2674.
};

// 223 DNA structure 458, 1
struct bNodeSocketValueRGBA
{
    float    value[4];// 1 type, name 7, 2990.
};

// 224 DNA structure 459, 3
struct bNodeSocketValueString
{
    int     subtype;    // 1 type, name 4, 17.
    int     pad;        // 2 type, name 4, 66.
    char    value[1024];// 3 type, name 0, 2991.
};

// 225 DNA structure 460, 2
struct NodeFrame
{
    short    flag;      // 1 type, name 2, 18.
    short    label_size;// 2 type, name 2, 2992.
};

// 226 DNA structure 461, 6
struct NodeImageAnim
{
    int      frames;// 1 type, name 4, 121.
    int      sfra;  // 2 type, name 4, 123.
    int      nr;    // 3 type, name 4, 2597.
    char     cyclic;// 4 type, name 0, 2993.
    char     movie; // 5 type, name 0, 2994.
    short    pad;   // 6 type, name 2, 66.
};

// 227 DNA structure 462, 6
struct ColorCorrectionData
{
    float    saturation;// 1 type, name 7, 283.
    float    contrast;  // 2 type, name 7, 282.
    float    gamma;     // 3 type, name 7, 2995.
    float    gain;      // 4 type, name 7, 2996.
    float    lift;      // 5 type, name 7, 2997.
    int      pad;       // 6 type, name 4, 66.
};

// 228 DNA structure 464, 5
struct NodeBokehImage
{
    float    angle;       // 1 type, name 7, 781.
    int      flaps;       // 2 type, name 4, 3004.
    float    rounding;    // 3 type, name 7, 3005.
    float    catadioptric;// 4 type, name 7, 3006.
    float    lensshift;   // 5 type, name 7, 3007.
};

// 229 DNA structure 465, 6
struct NodeBoxMask
{
    float    x;       // 1 type, name 7, 5.
    float    y;       // 2 type, name 7, 6.
    float    rotation;// 3 type, name 7, 3008.
    float    height;  // 4 type, name 7, 788.
    float    width;   // 5 type, name 7, 603.
    int      pad;     // 6 type, name 4, 66.
};

// 230 DNA structure 466, 6
struct NodeEllipseMask
{
    float    x;       // 1 type, name 7, 5.
    float    y;       // 2 type, name 7, 6.
    float    rotation;// 3 type, name 7, 3008.
    float    height;  // 4 type, name 7, 788.
    float    width;   // 5 type, name 7, 603.
    int      pad;     // 6 type, name 4, 66.
};

// 231 DNA structure 467, 2
struct NodeImageLayer
{
    int    pass_index;// 1 type, name 4, 3009.
    int    pass_flag; // 2 type, name 4, 3010.
};

// 232 DNA structure 468, 16
struct NodeBlurData
{
    short    sizex;          // 1 type, name 2, 2410.
    short    sizey;          // 2 type, name 2, 2411.
    short    samples;        // 3 type, name 2, 1408.
    short    maxspeed;       // 4 type, name 2, 3011.
    short    minspeed;       // 5 type, name 2, 3012.
    short    relative;       // 6 type, name 2, 69.
    short    aspect;         // 7 type, name 2, 1963.
    short    curved;         // 8 type, name 2, 3013.
    float    fac;            // 9 type, name 7, 783.
    float    percentx;       // 10 type, name 7, 3014.
    float    percenty;       // 11 type, name 7, 3015.
    short    filtertype;     // 12 type, name 2, 356.
    char     bokeh;          // 13 type, name 0, 3016.
    char     gamma;          // 14 type, name 0, 2995.
    int      image_in_width; // 15 type, name 4, 3017.
    int      image_in_height;// 16 type, name 4, 3018.
};

// 233 DNA structure 469, 9
struct NodeDBlurData
{
    float    center_x;// 1 type, name 7, 3019.
    float    center_y;// 2 type, name 7, 3020.
    float    distance;// 3 type, name 7, 984.
    float    angle;   // 4 type, name 7, 781.
    float    spin;    // 5 type, name 7, 3021.
    float    zoom;    // 6 type, name 7, 1880.
    short    iter;    // 7 type, name 2, 779.
    char     wrap;    // 8 type, name 0, 3022.
    char     pad;     // 9 type, name 0, 66.
};

// 234 DNA structure 470, 4
struct NodeBilateralBlurData
{
    float    sigma_color;// 1 type, name 7, 3023.
    float    sigma_space;// 2 type, name 7, 3024.
    short    iter;       // 3 type, name 2, 779.
    short    pad;        // 4 type, name 2, 66.
};

// 235 DNA structure 471, 3
struct NodeHueSat
{
    float    hue;// 1 type, name 7, 3025.
    float    sat;// 2 type, name 7, 2497.
    float    val;// 3 type, name 7, 14.
};

// 236 DNA structure 475, 9
struct NodeChroma
{
    float    t1;       // 1 type, name 7, 3032.
    float    t2;       // 2 type, name 7, 3033.
    float    t3;       // 3 type, name 7, 3034.
    float    fsize;    // 4 type, name 7, 614.
    float    fstrength;// 5 type, name 7, 3035.
    float    falpha;   // 6 type, name 7, 3036.
    float    key[4];   // 7 type, name 7, 3037.
    short    algorithm;// 8 type, name 2, 3038.
    short    channel;  // 9 type, name 2, 3039.
};

// 237 DNA structure 476, 8
struct NodeTwoXYs
{
    short    x1;    // 1 type, name 2, 3040.
    short    x2;    // 2 type, name 2, 3041.
    short    y1;    // 3 type, name 2, 3042.
    short    y2;    // 4 type, name 2, 3043.
    float    fac_x1;// 5 type, name 7, 3044.
    float    fac_x2;// 6 type, name 7, 3045.
    float    fac_y1;// 7 type, name 7, 3046.
    float    fac_y2;// 8 type, name 7, 3047.
};

// 238 DNA structure 477, 2
struct NodeTwoFloats
{
    float    x;// 1 type, name 7, 5.
    float    y;// 2 type, name 7, 6.
};

// 239 DNA structure 478, 2
struct NodeGeometry
{
    char    uvname[64]; // 1 type, name 0, 168.
    char    colname[64];// 2 type, name 0, 3048.
};

// 240 DNA structure 479, 1
struct NodeVertexCol
{
    char    name[64];// 1 type, name 0, 19.
};

// 241 DNA structure 480, 12
struct NodeDefocus
{
    char     bktype;  // 1 type, name 0, 3049.
    char     pad_c1;  // 2 type, name 0, 3050.
    char     preview; // 3 type, name 0, 1854.
    char     gamco;   // 4 type, name 0, 3051.
    short    samples; // 5 type, name 2, 1408.
    short    no_zbuf; // 6 type, name 2, 3052.
    float    fstop;   // 7 type, name 7, 3053.
    float    maxblur; // 8 type, name 7, 3054.
    float    bthresh; // 9 type, name 7, 3055.
    float    scale;   // 10 type, name 7, 933.
    float    rotation;// 11 type, name 7, 3008.
    float    pad_f1;  // 12 type, name 7, 3056.
};

// 242 DNA structure 481, 2
struct NodeScriptDict
{
    void    *dict;// 1 type, name 11, 3057.
    void    *node;// 2 type, name 11, 3058.
};

// 243 DNA structure 482, 13
struct NodeGlare
{
    char     quality;  // 1 type, name 0, 1413.
    char     type;     // 2 type, name 0, 16.
    char     iter;     // 3 type, name 0, 779.
    char     angle;    // 4 type, name 0, 781.
    char     pad_c1;   // 5 type, name 0, 3050.
    char     size;     // 6 type, name 0, 102.
    char     pad[2];   // 7 type, name 0, 573.
    float    colmod;   // 8 type, name 7, 3059.
    float    mix;      // 9 type, name 7, 3060.
    float    threshold;// 10 type, name 7, 932.
    float    fade;     // 11 type, name 7, 3061.
    float    angle_ofs;// 12 type, name 7, 3062.
    float    pad_f1;   // 13 type, name 7, 3056.
};

// 244 DNA structure 483, 8
struct NodeTonemap
{
    float    key;   // 1 type, name 7, 2606.
    float    offset;// 2 type, name 7, 122.
    float    gamma; // 3 type, name 7, 2995.
    float    f;     // 4 type, name 7, 696.
    float    m;     // 5 type, name 7, 3063.
    float    a;     // 6 type, name 7, 225.
    float    c;     // 7 type, name 7, 3064.
    int      type;  // 8 type, name 4, 16.
};

// 245 DNA structure 484, 4
struct NodeLensDist
{
    short    jit; // 1 type, name 2, 3065.
    short    proj;// 2 type, name 2, 3066.
    short    fit; // 3 type, name 2, 3067.
    short    pad; // 4 type, name 2, 66.
};

// 246 DNA structure 485, 6
struct NodeColorBalance
{
    float    slope[3]; // 1 type, name 7, 3068.
    float    offset[3];// 2 type, name 7, 746.
    float    power[3]; // 3 type, name 7, 3069.
    float    lift[3];  // 4 type, name 7, 2477.
    float    gamma[3]; // 5 type, name 7, 2478.
    float    gain[3];  // 6 type, name 7, 2479.
};

// 247 DNA structure 486, 6
struct NodeColorspill
{
    short    limchan; // 1 type, name 2, 3070.
    short    unspill; // 2 type, name 2, 3071.
    float    limscale;// 3 type, name 7, 3072.
    float    uspillr; // 4 type, name 7, 3073.
    float    uspillg; // 5 type, name 7, 3074.
    float    uspillb; // 6 type, name 7, 3075.
};

// 248 DNA structure 487, 2
struct NodeDilateErode
{
    char    falloff;// 1 type, name 0, 792.
    char    pad[7]; // 2 type, name 0, 118.
};

// 249 DNA structure 488, 2
struct NodeMask
{
    int    size_x;// 1 type, name 4, 2553.
    int    size_y;// 2 type, name 4, 2554.
};

// 250 DNA structure 501, 1
struct NodeShaderAttribute
{
    char    name[64];// 1 type, name 0, 19.
};

// 251 DNA structure 502, 4
struct NodeShaderVectTransform
{
    int    type;        // 1 type, name 4, 16.
    int    convert_from;// 2 type, name 4, 3092.
    int    convert_to;  // 3 type, name 4, 3093.
    int    pad;         // 4 type, name 4, 66.
};

// 252 DNA structure 503, 1
struct TexNodeOutput
{
    char    name[64];// 1 type, name 0, 19.
};

// 253 DNA structure 504, 1
struct NodeKeyingScreenData
{
    char    tracking_object[64];// 1 type, name 0, 3094.
};

// 254 DNA structure 505, 12
struct NodeKeyingData
{
    float    screen_balance;       // 1 type, name 7, 3095.
    float    despill_factor;       // 2 type, name 7, 3096.
    float    despill_balance;      // 3 type, name 7, 3097.
    int      edge_kernel_radius;   // 4 type, name 4, 3098.
    float    edge_kernel_tolerance;// 5 type, name 7, 3099.
    float    clip_black;           // 6 type, name 7, 3100.
    float    clip_white;           // 7 type, name 7, 3101.
    int      dilate_distance;      // 8 type, name 4, 3102.
    int      feather_distance;     // 9 type, name 4, 3103.
    int      feather_falloff;      // 10 type, name 4, 3104.
    int      blur_pre;             // 11 type, name 4, 3105.
    int      blur_post;            // 12 type, name 4, 3106.
};

// 255 DNA structure 506, 2
struct NodeTrackPosData
{
    char    tracking_object[64];// 1 type, name 0, 3094.
    char    track_name[64];     // 2 type, name 0, 3107.
};

// 256 DNA structure 507, 3
struct NodeTranslateData
{
    char    wrap_axis;// 1 type, name 0, 3108.
    char    relative; // 2 type, name 0, 69.
    char    pad[6];   // 3 type, name 0, 736.
};

// 257 DNA structure 508, 2
struct NodePlaneTrackDeformData
{
    char    tracking_object[64]; // 1 type, name 0, 3094.
    char    plane_track_name[64];// 2 type, name 0, 3109.
};

// 258 DNA structure 509, 5
struct NodeShaderScript
{
    int     mode;             // 1 type, name 4, 334.
    int     flag;             // 2 type, name 4, 18.
    char    filepath[1024];   // 3 type, name 0, 35.
    char    bytecode_hash[64];// 4 type, name 0, 3110.
    char    *bytecode;        // 5 type, name 0, 3111.
};

// 259 DNA structure 510, 3
struct NodeShaderTangent
{
    int     direction_type;// 1 type, name 4, 3112.
    int     axis;          // 2 type, name 4, 752.
    char    uv_map[64];    // 3 type, name 0, 3113.
};

// 260 DNA structure 511, 2
struct NodeShaderNormalMap
{
    int     space;     // 1 type, name 4, 2838.
    char    uv_map[64];// 2 type, name 0, 3113.
};

// 261 DNA structure 512, 1
struct NodeShaderUVMap
{
    char    uv_map[64];// 1 type, name 0, 3113.
};

// 262 DNA structure 513, 2
struct NodeSunBeams
{
    float    source[2]; // 1 type, name 7, 3114.
    float    ray_length;// 2 type, name 7, 3115.
};

// 263 DNA structure 514, 4
struct CurveMapPoint
{
    float    x;     // 1 type, name 7, 5.
    float    y;     // 2 type, name 7, 6.
    short    flag;  // 3 type, name 2, 18.
    short    shorty;// 4 type, name 2, 3116.
};

// 264 DNA structure 515, 12
struct CurveMap
{
    short            totpoint;         // 1 type, name 2, 1143.
    short            flag;             // 2 type, name 2, 18.
    float            range;            // 3 type, name 7, 1298.
    float            mintable;         // 4 type, name 7, 3117.
    float            maxtable;         // 5 type, name 7, 3118.
    float            ext_in[2];        // 6 type, name 7, 3119.
    float            ext_out[2];       // 7 type, name 7, 3120.
    CurveMapPoint    *curve;           // 8 type, name 514, 3121.
    CurveMapPoint    *table;           // 9 type, name 514, 3122.
    CurveMapPoint    *premultable;     // 10 type, name 514, 3123.
    float            premul_ext_in[2]; // 11 type, name 7, 3124.
    float            premul_ext_out[2];// 12 type, name 7, 3125.
};

// 265 DNA structure 283, 13
struct Histogram
{
    int      channels;      // 1 type, name 4, 2815.
    int      x_resolution;  // 2 type, name 4, 3135.
    float    data_luma[256];// 3 type, name 7, 3136.
    float    data_r[256];   // 4 type, name 7, 3137.
    float    data_g[256];   // 5 type, name 7, 3138.
    float    data_b[256];   // 6 type, name 7, 3139.
    float    data_a[256];   // 7 type, name 7, 3140.
    float    xmax;          // 8 type, name 7, 9.
    float    ymax;          // 9 type, name 7, 11.
    short    mode;          // 10 type, name 2, 334.
    short    flag;          // 11 type, name 2, 18.
    int      height;        // 12 type, name 4, 788.
    float    co[2][2];      // 13 type, name 7, 3141.
};

// 266 DNA structure 219, 8
struct ColorManagedViewSettings
{
    int             flag;              // 1 type, name 4, 18.
    int             pad;               // 2 type, name 4, 66.
    char            look[64];          // 3 type, name 0, 3158.
    char            view_transform[64];// 4 type, name 0, 3159.
    float           exposure;          // 5 type, name 7, 1296.
    float           gamma;             // 6 type, name 7, 2995.
    CurveMapping    *curve_mapping;    // 7 type, name 55, 3160.
    void            *pad2;             // 8 type, name 11, 1603.
};

// 267 DNA structure 220, 1
struct ColorManagedDisplaySettings
{
    char    display_device[64];// 1 type, name 0, 3161.
};

// 268 DNA structure 47, 1
struct ColorManagedColorspaceSettings
{
    char    name[64];// 1 type, name 0, 19.
};

// 269 DNA structure 516, 4
struct BrushClone
{
    Image    *image;   // 1 type, name 43, 772.
    float    offset[2];// 2 type, name 7, 3162.
    float    alpha;    // 3 type, name 7, 429.
    float    pad;      // 4 type, name 7, 66.
};

// 270 DNA structure 518, 4
struct PaletteColor
{
    PaletteColor    *next; // 1 type, name 518, 0.
    PaletteColor    *prev; // 2 type, name 518, 1.
    float           rgb[3];// 3 type, name 7, 1608.
    float           value; // 4 type, name 7, 756.
};

// 271 DNA structure 520, 10
struct CustomDataLayer
{
    int     type;        // 1 type, name 4, 16.
    int     offset;      // 2 type, name 4, 122.
    int     flag;        // 3 type, name 4, 18.
    int     active;      // 4 type, name 4, 1095.
    int     active_rnd;  // 5 type, name 4, 3213.
    int     active_clone;// 6 type, name 4, 3214.
    int     active_mask; // 7 type, name 4, 3215.
    int     uid;         // 8 type, name 4, 71.
    char    name[64];    // 9 type, name 0, 19.
    void    *data;       // 10 type, name 11, 2.
};

// 272 DNA structure 521, 1
struct CustomDataExternal
{
    char    filename[1024];// 1 type, name 0, 1989.
};

// 273 DNA structure 94, 7
struct CustomData
{
    CustomDataLayer       *layers;    // 1 type, name 520, 3216.
    int                   typemap[41];// 2 type, name 4, 3217.
    int                   totlayer;   // 3 type, name 4, 3218.
    int                   maxlayer;   // 4 type, name 4, 3219.
    int                   totsize;    // 5 type, name 4, 3220.
    void                  *pool;      // Note: using void* on undefined DNA type: BLI_mempool
    CustomDataExternal    *external;  // 7 type, name 521, 3222.
};

// 274 DNA structure 522, 5
struct HairKey
{
    float    co[3];   // 1 type, name 7, 690.
    float    time;    // 2 type, name 7, 768.
    float    weight;  // 3 type, name 7, 557.
    short    editflag;// 4 type, name 2, 672.
    short    pad;     // 5 type, name 2, 66.
};

// 275 DNA structure 523, 5
struct ParticleKey
{
    float    co[3]; // 1 type, name 7, 690.
    float    vel[3];// 2 type, name 7, 1240.
    float    rot[4];// 3 type, name 7, 3223.
    float    ave[3];// 4 type, name 7, 3224.
    float    time;  // 5 type, name 7, 768.
};

// 276 DNA structure 526, 3
struct ParticleSpring
{
    float    rest_length;      // 1 type, name 7, 3227.
    int      particle_index[2];// 2 type, name 4, 3228.
    int      delete_flag;      // 3 type, name 4, 3229.
};

// 277 DNA structure 527, 7
struct ChildParticle
{
    int      num;    // 1 type, name 4, 3230.
    int      parent; // 2 type, name 4, 3231.
    int      pa[4];  // 3 type, name 4, 3232.
    float    w[4];   // 4 type, name 7, 3233.
    float    fuv[4]; // 5 type, name 7, 3234.
    float    foffset;// 6 type, name 7, 3235.
    float    rt;     // 7 type, name 7, 56.
};

// 278 DNA structure 528, 8
struct ParticleTarget
{
    ParticleTarget    *next;   // 1 type, name 528, 0.
    ParticleTarget    *prev;   // 2 type, name 528, 1.
    Object            *ob;     // 3 type, name 29, 44.
    int               psys;    // 4 type, name 4, 249.
    short             flag;    // 5 type, name 2, 18.
    short             mode;    // 6 type, name 2, 334.
    float             time;    // 7 type, name 7, 768.
    float             duration;// 8 type, name 7, 2614.
};

// 279 DNA structure 529, 7
struct ParticleDupliWeight
{
    ParticleDupliWeight    *next;// 1 type, name 529, 0.
    ParticleDupliWeight    *prev;// 2 type, name 529, 1.
    Object                 *ob;  // 3 type, name 29, 44.
    short                  count;// 4 type, name 2, 751.
    short                  flag; // 5 type, name 2, 18.
    short                  index;// 6 type, name 2, 413.
    short                  rt;   // 7 type, name 2, 56.
};

// 280 DNA structure 531, 17
struct SPHFluidSettings
{
    float    radius;             // 1 type, name 7, 245.
    float    spring_k;           // 2 type, name 7, 3244.
    float    rest_length;        // 3 type, name 7, 3227.
    float    plasticity_constant;// 4 type, name 7, 3245.
    float    yield_ratio;        // 5 type, name 7, 3246.
    float    plasticity_balance; // 6 type, name 7, 3247.
    float    yield_balance;      // 7 type, name 7, 3248.
    float    viscosity_omega;    // 8 type, name 7, 3249.
    float    viscosity_beta;     // 9 type, name 7, 3250.
    float    stiffness_k;        // 10 type, name 7, 3251.
    float    stiffness_knear;    // 11 type, name 7, 3252.
    float    rest_density;       // 12 type, name 7, 3253.
    float    buoyancy;           // 13 type, name 7, 3254.
    int      flag;               // 14 type, name 4, 18.
    int      spring_frames;      // 15 type, name 4, 3255.
    short    solver;             // 16 type, name 2, 2810.
    short    pad[3];             // 17 type, name 2, 333.
};

// 281 DNA structure 139, 42
struct ClothSimSettings
{
    void               *cache;           // Note: using void* on undefined DNA type: LinkNode
    float              mingoal;          // 2 type, name 7, 1204.
    float              Cdis;             // 3 type, name 7, 3382.
    float              Cvi;              // 4 type, name 7, 3383.
    float              gravity[3];       // 5 type, name 7, 1718.
    float              dt;               // 6 type, name 7, 1053.
    float              mass;             // 7 type, name 7, 1036.
    float              structural;       // 8 type, name 7, 3384.
    float              shear;            // 9 type, name 7, 613.
    float              bending;          // 10 type, name 7, 3385.
    float              max_bend;         // 11 type, name 7, 3386.
    float              max_struct;       // 12 type, name 7, 3387.
    float              max_shear;        // 13 type, name 7, 3388.
    float              max_sewing;       // 14 type, name 7, 3389.
    float              avg_spring_len;   // 15 type, name 7, 3390.
    float              timescale;        // 16 type, name 7, 3391.
    float              maxgoal;          // 17 type, name 7, 1205.
    float              eff_force_scale;  // 18 type, name 7, 3392.
    float              eff_wind_scale;   // 19 type, name 7, 3393.
    float              sim_time_old;     // 20 type, name 7, 3394.
    float              defgoal;          // 21 type, name 7, 1206.
    float              goalspring;       // 22 type, name 7, 1202.
    float              goalfrict;        // 23 type, name 7, 1203.
    float              velocity_smooth;  // 24 type, name 7, 3395.
    float              collider_friction;// 25 type, name 7, 3396.
    float              vel_damping;      // 26 type, name 7, 3397.
    float              shrink_min;       // 27 type, name 7, 3398.
    float              shrink_max;       // 28 type, name 7, 3399.
    int                stepsPerFrame;    // 29 type, name 4, 3400.
    int                flags;            // 30 type, name 4, 90.
    int                preroll;          // 31 type, name 4, 3401.
    int                maxspringlen;     // 32 type, name 4, 3402.
    short              solver_type;      // 33 type, name 2, 3403.
    short              vgroup_bend;      // 34 type, name 2, 3404.
    short              vgroup_mass;      // 35 type, name 2, 3405.
    short              vgroup_struct;    // 36 type, name 2, 3406.
    short              vgroup_shrink;    // 37 type, name 2, 3407.
    short              shapekey_rest;    // 38 type, name 2, 3408.
    short              presets;          // 39 type, name 2, 3409.
    short              reset;            // 40 type, name 2, 3410.
    char               pad[4];           // 41 type, name 0, 568.
    EffectorWeights    *effector_weights;// 42 type, name 200, 1235.
};

// 282 DNA structure 140, 14
struct ClothCollSettings
{
    void        *collision_list;// Note: using void* on undefined DNA type: LinkNode
    float       epsilon;        // 2 type, name 7, 3412.
    float       self_friction;  // 3 type, name 7, 3413.
    float       friction;       // 4 type, name 7, 504.
    float       selfepsilon;    // 5 type, name 7, 3414.
    float       repel_force;    // 6 type, name 7, 3415.
    float       distance_repel; // 7 type, name 7, 3416.
    int         flags;          // 8 type, name 4, 90.
    short       self_loop_count;// 9 type, name 2, 3417.
    short       loop_count;     // 10 type, name 2, 3418.
    Group       *group;         // 11 type, name 66, 503.
    short       vgroup_selfcol; // 12 type, name 2, 3419.
    short       pad;            // 13 type, name 2, 66.
    int         pad2;           // 14 type, name 4, 29.
};

// 283 DNA structure 540, 6
struct bGPDspoint
{
    float    x;       // 1 type, name 7, 5.
    float    y;       // 2 type, name 7, 6.
    float    z;       // 3 type, name 7, 7.
    float    pressure;// 4 type, name 7, 3210.
    float    time;    // 5 type, name 7, 768.
    int      flag;    // 6 type, name 4, 18.
};

// 284 DNA structure 541, 8
struct bGPDstroke
{
    bGPDstroke    *next;    // 1 type, name 541, 0.
    bGPDstroke    *prev;    // 2 type, name 541, 1.
    bGPDspoint    *points;  // 3 type, name 540, 2754.
    void          *pad;     // 4 type, name 11, 1364.
    int           totpoints;// 5 type, name 4, 247.
    short         thickness;// 6 type, name 2, 3420.
    short         flag;     // 7 type, name 2, 18.
    double        inittime; // 8 type, name 8, 3421.
};

// 285 DNA structure 552, 18
struct wmKeyMapItem
{
    wmKeyMapItem    *next;            // 1 type, name 552, 0.
    wmKeyMapItem    *prev;            // 2 type, name 552, 1.
    char            idname[64];       // 3 type, name 0, 2419.
    IDProperty      *properties;      // 4 type, name 21, 30.
    char            propvalue_str[64];// 5 type, name 0, 3484.
    short           propvalue;        // 6 type, name 2, 3485.
    short           type;             // 7 type, name 2, 16.
    short           val;              // 8 type, name 2, 14.
    short           shift;            // 9 type, name 2, 3486.
    short           ctrl;             // 10 type, name 2, 3487.
    short           alt;              // 11 type, name 2, 3488.
    short           oskey;            // 12 type, name 2, 3489.
    short           keymodifier;      // 13 type, name 2, 3490.
    short           flag;             // 14 type, name 2, 18.
    short           maptype;          // 15 type, name 2, 3491.
    short           id;               // 16 type, name 2, 31.
    short           pad;              // 17 type, name 2, 66.
    void            *ptr;             // Note: using void* on undefined DNA type: PointerRNA
};

// 286 DNA structure 554, 4
struct wmKeyMapDiffItem
{
    wmKeyMapDiffItem    *next;       // 1 type, name 554, 0.
    wmKeyMapDiffItem    *prev;       // 2 type, name 554, 1.
    wmKeyMapItem        *remove_item;// 3 type, name 552, 3493.
    wmKeyMapItem        *add_item;   // 4 type, name 552, 3494.
};

// 287 DNA structure 557, 11
struct FModifier
{
    FModifier    *next;    // 1 type, name 557, 0.
    FModifier    *prev;    // 2 type, name 557, 1.
    void         *data;    // 3 type, name 11, 2.
    char         name[64]; // 4 type, name 0, 19.
    short        type;     // 5 type, name 2, 16.
    short        flag;     // 6 type, name 2, 18.
    float        influence;// 7 type, name 7, 2695.
    float        sfra;     // 8 type, name 7, 123.
    float        efra;     // 9 type, name 7, 1213.
    float        blendin;  // 10 type, name 7, 2645.
    float        blendout; // 11 type, name 7, 2909.
};

// 288 DNA structure 558, 5
struct FMod_Generator
{
    float    *coefficients;// 1 type, name 7, 3509.
    int      arraysize;    // 2 type, name 4, 3510.
    int      poly_order;   // 3 type, name 4, 3511.
    int      mode;         // 4 type, name 4, 334.
    int      flag;         // 5 type, name 4, 18.
};

// 289 DNA structure 559, 6
struct FMod_FunctionGenerator
{
    float    amplitude;       // 1 type, name 7, 566.
    float    phase_multiplier;// 2 type, name 7, 3512.
    float    phase_offset;    // 3 type, name 7, 3513.
    float    value_offset;    // 4 type, name 7, 3514.
    int      type;            // 5 type, name 4, 16.
    int      flag;            // 6 type, name 4, 18.
};

// 290 DNA structure 560, 5
struct FCM_EnvelopeData
{
    float    min; // 1 type, name 7, 2673.
    float    max; // 2 type, name 7, 2674.
    float    time;// 3 type, name 7, 768.
    short    f1;  // 4 type, name 2, 560.
    short    f2;  // 5 type, name 2, 561.
};

// 291 DNA structure 561, 5
struct FMod_Envelope
{
    FCM_EnvelopeData    *data;  // 1 type, name 560, 2.
    int                 totvert;// 2 type, name 4, 53.
    float               midval; // 3 type, name 7, 3515.
    float               min;    // 4 type, name 7, 2673.
    float               max;    // 5 type, name 7, 2674.
};

// 292 DNA structure 562, 4
struct FMod_Cycles
{
    short    before_mode;  // 1 type, name 2, 3516.
    short    after_mode;   // 2 type, name 2, 3517.
    short    before_cycles;// 3 type, name 2, 3518.
    short    after_cycles; // 4 type, name 2, 3519.
};

// 293 DNA structure 563, 2
struct FMod_Python
{
    Text          *script;// 1 type, name 38, 1957.
    IDProperty    *prop;  // 2 type, name 21, 2265.
};

// 294 DNA structure 565, 6
struct FMod_Noise
{
    float    size;        // 1 type, name 7, 102.
    float    strength;    // 2 type, name 7, 734.
    float    phase;       // 3 type, name 7, 3521.
    float    offset;      // 4 type, name 7, 122.
    short    depth;       // 5 type, name 2, 240.
    short    modification;// 6 type, name 2, 3522.
};

// 295 DNA structure 566, 5
struct FMod_Stepped
{
    float    step_size;  // 1 type, name 7, 3523.
    float    offset;     // 2 type, name 7, 122.
    float    start_frame;// 3 type, name 7, 2755.
    float    end_frame;  // 4 type, name 7, 2756.
    int      flag;       // 5 type, name 4, 18.
};

// 296 DNA structure 567, 6
struct DriverTarget
{
    ID       *id;           // 1 type, name 22, 1962.
    char     *rna_path;     // 2 type, name 0, 3524.
    char     pchan_name[32];// 3 type, name 0, 3525.
    short    transChan;     // 4 type, name 2, 3526.
    short    flag;          // 5 type, name 2, 18.
    int      idtype;        // 6 type, name 4, 3527.
};

// 297 DNA structure 570, 3
struct FPoint
{
    float    vec[2];// 1 type, name 7, 3533.
    int      flag;  // 2 type, name 4, 18.
    int      pad;   // 3 type, name 4, 66.
};

// 298 DNA structure 572, 2
struct AnimMapPair
{
    char    from[128];// 1 type, name 0, 3537.
    char    to[128];  // 2 type, name 0, 3538.
};

// 299 DNA structure 576, 11
struct KS_Path
{
    KS_Path    *next;      // 1 type, name 576, 0.
    KS_Path    *prev;      // 2 type, name 576, 1.
    ID         *id;        // 3 type, name 22, 1962.
    char       group[64];  // 4 type, name 0, 3547.
    int        idtype;     // 5 type, name 4, 3527.
    short      groupmode;  // 6 type, name 2, 3548.
    short      pad;        // 7 type, name 2, 66.
    char       *rna_path;  // 8 type, name 0, 3524.
    int        array_index;// 9 type, name 4, 3535.
    short      flag;       // 10 type, name 2, 18.
    short      keyingflag; // 11 type, name 2, 3549.
};

// 300 DNA structure 578, 5
struct AnimOverride
{
    AnimOverride    *next;      // 1 type, name 578, 0.
    AnimOverride    *prev;      // 2 type, name 578, 1.
    char            *rna_path;  // 3 type, name 0, 3524.
    int             array_index;// 4 type, name 4, 3535.
    float           value;      // 5 type, name 7, 756.
};

// 301 DNA structure 580, 5
struct BoidRule
{
    BoidRule    *next;   // 1 type, name 580, 0.
    BoidRule    *prev;   // 2 type, name 580, 1.
    int         type;    // 3 type, name 4, 16.
    int         flag;    // 4 type, name 4, 18.
    char        name[32];// 5 type, name 0, 1532.
};

// 302 DNA structure 525, 4
struct BoidData
{
    float    health;  // 1 type, name 7, 3571.
    float    acc[3];  // 2 type, name 7, 3311.
    short    state_id;// 3 type, name 2, 3572.
    short    mode;    // 4 type, name 2, 334.
};

// 303 DNA structure 126, 26
struct SmokeFlowSettings
{
    SmokeModifierData    *smd;            // 1 type, name 124, 3594.
    void                 *dm;             // Note: using void* on undefined DNA type: DerivedMesh
    ParticleSystem       *psys;           // 3 type, name 152, 842.
    Tex                  *noise_texture;  // 4 type, name 49, 3646.
    float                *verts_old;      // 5 type, name 7, 3647.
    int                  numverts;        // 6 type, name 4, 816.
    float                vel_multi;       // 7 type, name 7, 3648.
    float                vel_normal;      // 8 type, name 7, 3649.
    float                vel_random;      // 9 type, name 7, 3650.
    float                density;         // 10 type, name 7, 392.
    float                color[3];        // 11 type, name 7, 2936.
    float                fuel_amount;     // 12 type, name 7, 3651.
    float                temp;            // 13 type, name 7, 2388.
    float                volume_density;  // 14 type, name 7, 3652.
    float                surface_distance;// 15 type, name 7, 3653.
    float                particle_size;   // 16 type, name 7, 3654.
    int                  subframes;       // 17 type, name 4, 3267.
    float                texture_size;    // 18 type, name 7, 3655.
    float                texture_offset;  // 19 type, name 7, 3656.
    int                  pad;             // 20 type, name 4, 66.
    char                 uvlayer_name[64];// 21 type, name 0, 727.
    short                vgroup_density;  // 22 type, name 2, 3657.
    short                type;            // 23 type, name 2, 16.
    short                source;          // 24 type, name 2, 137.
    short                texture_type;    // 25 type, name 2, 3658.
    int                  flags;           // 26 type, name 4, 90.
};

// 304 DNA structure 127, 6
struct SmokeCollSettings
{
    SmokeModifierData    *smd;      // 1 type, name 124, 3594.
    void                 *dm;       // Note: using void* on undefined DNA type: DerivedMesh
    float                *verts_old;// 3 type, name 7, 3647.
    int                  numverts;  // 4 type, name 4, 816.
    short                type;      // 5 type, name 2, 16.
    short                pad;       // 6 type, name 2, 66.
};

// 305 DNA structure 252, 3
struct MovieClipUser
{
    int      framenr;    // 1 type, name 4, 120.
    short    render_size;// 2 type, name 2, 1877.
    short    render_flag;// 3 type, name 2, 3666.
};

// 306 DNA structure 590, 5
struct MovieClipProxy
{
    char     dir[768];       // 1 type, name 0, 2480.
    short    tc;             // 2 type, name 2, 2481.
    short    quality;        // 3 type, name 2, 1413.
    short    build_size_flag;// 4 type, name 2, 3667.
    short    build_tc_flag;  // 5 type, name 2, 3668.
};

// 307 DNA structure 595, 3
struct MovieReconstructedCamera
{
    int      framenr;  // 1 type, name 4, 120.
    float    error;    // 2 type, name 7, 3686.
    float    mat[4][4];// 3 type, name 7, 326.
};

// 308 DNA structure 596, 14
struct MovieTrackingCamera
{
    void     *intrinsics;     // 1 type, name 11, 3687.
    short    distortion_model;// 2 type, name 2, 3688.
    short    pad;             // 3 type, name 2, 66.
    float    sensor_width;    // 4 type, name 7, 3689.
    float    pixel_aspect;    // 5 type, name 7, 3690.
    float    focal;           // 6 type, name 7, 3691.
    short    units;           // 7 type, name 2, 3692.
    short    pad1;            // 8 type, name 2, 68.
    float    principal[2];    // 9 type, name 7, 3693.
    float    k1;              // 10 type, name 7, 3694.
    float    k2;              // 11 type, name 7, 3695.
    float    k3;              // 12 type, name 7, 3696.
    float    division_k1;     // 13 type, name 7, 3697.
    float    division_k2;     // 14 type, name 7, 3698.
};

// 309 DNA structure 593, 6
struct MovieTrackingMarker
{
    float    pos[2];               // 1 type, name 7, 3699.
    float    pattern_corners[4][2];// 2 type, name 7, 3700.
    float    search_min[2];        // 3 type, name 7, 3701.
    float    search_max[2];        // 4 type, name 7, 3702.
    int      framenr;              // 5 type, name 4, 120.
    int      flag;                 // 6 type, name 4, 18.
};

// 310 DNA structure 594, 26
struct MovieTrackingTrack
{
    MovieTrackingTrack     *next;              // 1 type, name 594, 0.
    MovieTrackingTrack     *prev;              // 2 type, name 594, 1.
    char                   name[64];           // 3 type, name 0, 19.
    float                  pat_min[2];         // 4 type, name 7, 3703.
    float                  pat_max[2];         // 5 type, name 7, 3704.
    float                  search_min[2];      // 6 type, name 7, 3701.
    float                  search_max[2];      // 7 type, name 7, 3702.
    float                  offset[2];          // 8 type, name 7, 3162.
    int                    markersnr;          // 9 type, name 4, 3705.
    int                    last_marker;        // 10 type, name 4, 3706.
    MovieTrackingMarker    *markers;           // 11 type, name 593, 3707.
    float                  bundle_pos[3];      // 12 type, name 7, 3708.
    float                  error;              // 13 type, name 7, 3686.
    int                    flag;               // 14 type, name 4, 18.
    int                    pat_flag;           // 15 type, name 4, 3709.
    int                    search_flag;        // 16 type, name 4, 3710.
    float                  color[3];           // 17 type, name 7, 2936.
    short                  frames_limit;       // 18 type, name 2, 3711.
    short                  margin;             // 19 type, name 2, 1041.
    short                  pattern_match;      // 20 type, name 2, 3712.
    short                  motion_model;       // 21 type, name 2, 3713.
    int                    algorithm_flag;     // 22 type, name 4, 3714.
    float                  minimum_correlation;// 23 type, name 7, 3715.
    bGPdata                *gpd;               // 24 type, name 188, 998.
    float                  weight;             // 25 type, name 7, 557.
    float                  pad;                // 26 type, name 7, 66.
};

// 311 DNA structure 597, 3
struct MovieTrackingPlaneMarker
{
    float    corners[4][2];// 1 type, name 7, 3716.
    int      framenr;      // 2 type, name 4, 120.
    int      flag;         // 3 type, name 4, 18.
};

// 312 DNA structure 598, 12
struct MovieTrackingPlaneTrack
{
    MovieTrackingPlaneTrack     *next;         // 1 type, name 598, 0.
    MovieTrackingPlaneTrack     *prev;         // 2 type, name 598, 1.
    char                        name[64];      // 3 type, name 0, 19.
    MovieTrackingTrack          **point_tracks;// 4 type, name 594, 3717.
    int                         point_tracksnr;// 5 type, name 4, 3718.
    int                         pad;           // 6 type, name 4, 66.
    MovieTrackingPlaneMarker    *markers;      // 7 type, name 597, 3707.
    int                         markersnr;     // 8 type, name 4, 3705.
    int                         flag;          // 9 type, name 4, 18.
    Image                       *image;        // 10 type, name 43, 772.
    float                       image_opacity; // 11 type, name 7, 3719.
    int                         last_marker;   // 12 type, name 4, 3706.
};

// 313 DNA structure 599, 24
struct MovieTrackingSettings
{
    int      flag;                       // 1 type, name 4, 18.
    short    default_motion_model;       // 2 type, name 2, 3720.
    short    default_algorithm_flag;     // 3 type, name 2, 3721.
    float    default_minimum_correlation;// 4 type, name 7, 3722.
    short    default_pattern_size;       // 5 type, name 2, 3723.
    short    default_search_size;        // 6 type, name 2, 3724.
    short    default_frames_limit;       // 7 type, name 2, 3725.
    short    default_margin;             // 8 type, name 2, 3726.
    short    default_pattern_match;      // 9 type, name 2, 3727.
    short    default_flag;               // 10 type, name 2, 3728.
    float    default_weight;             // 11 type, name 7, 905.
    short    motion_flag;                // 12 type, name 2, 3729.
    short    speed;                      // 13 type, name 2, 790.
    int      keyframe1;                  // 14 type, name 4, 3730.
    int      keyframe2;                  // 15 type, name 4, 3731.
    int      reconstruction_flag;        // 16 type, name 4, 3732.
    short    refine_camera_intrinsics;   // 17 type, name 2, 3733.
    short    pad2;                       // 18 type, name 2, 29.
    float    dist;                       // 19 type, name 7, 341.
    int      clean_frames;               // 20 type, name 4, 3734.
    int      clean_action;               // 21 type, name 4, 3735.
    float    clean_error;                // 22 type, name 7, 3736.
    float    object_distance;            // 23 type, name 7, 3737.
    int      pad3;                       // 24 type, name 4, 500.
};

// 314 DNA structure 600, 11
struct MovieTrackingStabilization
{
    int                   flag;      // 1 type, name 4, 18.
    int                   tot_track; // 2 type, name 4, 3738.
    int                   act_track; // 3 type, name 4, 3739.
    float                 maxscale;  // 4 type, name 7, 3740.
    MovieTrackingTrack    *rot_track;// 5 type, name 594, 3741.
    float                 locinf;    // 6 type, name 7, 3742.
    float                 scaleinf;  // 7 type, name 7, 3743.
    float                 rotinf;    // 8 type, name 7, 3744.
    int                   filter;    // 9 type, name 4, 441.
    int                   ok;        // 10 type, name 4, 126.
    float                 scale;     // 11 type, name 7, 933.
};

// 315 DNA structure 601, 5
struct MovieTrackingReconstruction
{
    int                         flag;       // 1 type, name 4, 18.
    float                       error;      // 2 type, name 7, 3686.
    int                         last_camera;// 3 type, name 4, 3745.
    int                         camnr;      // 4 type, name 4, 3746.
    MovieReconstructedCamera    *cameras;   // 5 type, name 595, 3747.
};

// 316 DNA structure 603, 1
struct MovieTrackingStats
{
    char    message[256];// 1 type, name 0, 3751.
};

// 317 DNA structure 604, 9
struct MovieTrackingDopesheetChannel
{
    MovieTrackingDopesheetChannel    *next;       // 1 type, name 604, 0.
    MovieTrackingDopesheetChannel    *prev;       // 2 type, name 604, 1.
    MovieTrackingTrack               *track;      // 3 type, name 594, 991.
    int                              pad;         // 4 type, name 4, 66.
    char                             name[64];    // 5 type, name 0, 19.
    int                              tot_segment; // 6 type, name 4, 3752.
    int                              *segments;   // 7 type, name 4, 3753.
    int                              max_segment; // 8 type, name 4, 3754.
    int                              total_frames;// 9 type, name 4, 3755.
};

// 318 DNA structure 605, 6
struct MovieTrackingDopesheetCoverageSegment
{
    MovieTrackingDopesheetCoverageSegment    *next;      // 1 type, name 605, 0.
    MovieTrackingDopesheetCoverageSegment    *prev;      // 2 type, name 605, 1.
    int                                      coverage;   // 3 type, name 4, 3756.
    int                                      start_frame;// 4 type, name 4, 2755.
    int                                      end_frame;  // 5 type, name 4, 2756.
    int                                      pad;        // 6 type, name 4, 66.
};

// 319 DNA structure 172, 24
struct DynamicPaintBrushSettings
{
    DynamicPaintModifierData    *pmd;             // 1 type, name 170, 3801.
    void                        *dm;              // Note: using void* on undefined DNA type: DerivedMesh
    ParticleSystem              *psys;            // 3 type, name 152, 842.
    Material                    *mat;             // 4 type, name 65, 543.
    int                         flags;            // 5 type, name 4, 90.
    int                         collision;        // 6 type, name 4, 3805.
    float                       r;                // 7 type, name 7, 183.
    float                       g;                // 8 type, name 7, 184.
    float                       b;                // 9 type, name 7, 185.
    float                       alpha;            // 10 type, name 7, 429.
    float                       wetness;          // 11 type, name 7, 3806.
    float                       particle_radius;  // 12 type, name 7, 3807.
    float                       particle_smooth;  // 13 type, name 7, 3808.
    float                       paint_distance;   // 14 type, name 7, 3809.
    ColorBand                   *paint_ramp;      // 15 type, name 51, 3810.
    ColorBand                   *vel_ramp;        // 16 type, name 51, 3811.
    short                       proximity_falloff;// 17 type, name 2, 3812.
    short                       wave_type;        // 18 type, name 2, 3091.
    short                       ray_dir;          // 19 type, name 2, 3813.
    short                       pad;              // 20 type, name 2, 66.
    float                       wave_factor;      // 21 type, name 7, 3814.
    float                       wave_clamp;       // 22 type, name 7, 3815.
    float                       max_velocity;     // 23 type, name 7, 3816.
    float                       smudge_strength;  // 24 type, name 7, 3817.
};

// 320 DNA structure 609, 7
struct MaskParent
{
    int      id_type;                  // 1 type, name 4, 3821.
    int      type;                     // 2 type, name 4, 16.
    ID       *id;                      // 3 type, name 22, 1962.
    char     parent[64];               // 4 type, name 0, 3822.
    char     sub_parent[64];           // 5 type, name 0, 3823.
    float    parent_orig[2];           // 6 type, name 7, 3824.
    float    parent_corners_orig[4][2];// 7 type, name 7, 3825.
};

// 321 DNA structure 610, 3
struct MaskSplinePointUW
{
    float    u;   // 1 type, name 7, 3826.
    float    w;   // 2 type, name 7, 586.
    int      flag;// 3 type, name 4, 18.
};

// 322 DNA structure 613, 7
struct MaskLayerShape
{
    MaskLayerShape    *next;   // 1 type, name 613, 0.
    MaskLayerShape    *prev;   // 2 type, name 613, 1.
    float             *data;   // 3 type, name 7, 2.
    int               tot_vert;// 4 type, name 4, 3834.
    int               frame;   // 5 type, name 4, 1142.
    char              flag;    // 6 type, name 0, 18.
    char              pad[7];  // 7 type, name 0, 118.
};

// 323 DNA structure 195, 19
struct RigidBodyOb
{
    void     *physics_object; // 1 type, name 11, 3847.
    void     *physics_shape;  // 2 type, name 11, 3848.
    short    type;            // 3 type, name 2, 16.
    short    shape;           // 4 type, name 2, 1103.
    int      flag;            // 5 type, name 4, 18.
    int      col_groups;      // 6 type, name 4, 3849.
    short    mesh_source;     // 7 type, name 2, 3850.
    short    pad;             // 8 type, name 2, 66.
    float    mass;            // 9 type, name 7, 1036.
    float    friction;        // 10 type, name 7, 504.
    float    restitution;     // 11 type, name 7, 3851.
    float    margin;          // 12 type, name 7, 1041.
    float    lin_damping;     // 13 type, name 7, 3852.
    float    ang_damping;     // 14 type, name 7, 3853.
    float    lin_sleep_thresh;// 15 type, name 7, 3854.
    float    ang_sleep_thresh;// 16 type, name 7, 3855.
    float    orn[4];          // 17 type, name 7, 3856.
    float    pos[3];          // 18 type, name 7, 3857.
    float    pad1;            // 19 type, name 7, 68.
};

// 324 DNA structure 196, 30
struct RigidBodyCon
{
    Object    *ob1;                     // 1 type, name 29, 3858.
    Object    *ob2;                     // 2 type, name 29, 3859.
    short     type;                     // 3 type, name 2, 16.
    short     num_solver_iterations;    // 4 type, name 2, 3845.
    int       flag;                     // 5 type, name 4, 18.
    float     breaking_threshold;       // 6 type, name 7, 3860.
    float     pad;                      // 7 type, name 7, 66.
    float     limit_lin_x_lower;        // 8 type, name 7, 3861.
    float     limit_lin_x_upper;        // 9 type, name 7, 3862.
    float     limit_lin_y_lower;        // 10 type, name 7, 3863.
    float     limit_lin_y_upper;        // 11 type, name 7, 3864.
    float     limit_lin_z_lower;        // 12 type, name 7, 3865.
    float     limit_lin_z_upper;        // 13 type, name 7, 3866.
    float     limit_ang_x_lower;        // 14 type, name 7, 3867.
    float     limit_ang_x_upper;        // 15 type, name 7, 3868.
    float     limit_ang_y_lower;        // 16 type, name 7, 3869.
    float     limit_ang_y_upper;        // 17 type, name 7, 3870.
    float     limit_ang_z_lower;        // 18 type, name 7, 3871.
    float     limit_ang_z_upper;        // 19 type, name 7, 3872.
    float     spring_stiffness_x;       // 20 type, name 7, 3873.
    float     spring_stiffness_y;       // 21 type, name 7, 3874.
    float     spring_stiffness_z;       // 22 type, name 7, 3875.
    float     spring_damping_x;         // 23 type, name 7, 3876.
    float     spring_damping_y;         // 24 type, name 7, 3877.
    float     spring_damping_z;         // 25 type, name 7, 3878.
    float     motor_lin_target_velocity;// 26 type, name 7, 3879.
    float     motor_ang_target_velocity;// 27 type, name 7, 3880.
    float     motor_lin_max_impulse;    // 28 type, name 7, 3881.
    float     motor_ang_max_impulse;    // 29 type, name 7, 3882.
    void      *physics_constraint;      // 30 type, name 11, 3883.
};

// 325 DNA structure 615, 14
struct FreestyleLineSet
{
    FreestyleLineSet      *next;             // 1 type, name 615, 0.
    FreestyleLineSet      *prev;             // 2 type, name 615, 1.
    char                  name[64];          // 3 type, name 0, 19.
    int                   flags;             // 4 type, name 4, 90.
    int                   selection;         // 5 type, name 4, 3884.
    short                 qi;                // 6 type, name 2, 3885.
    short                 pad1;              // 7 type, name 2, 68.
    int                   qi_start;          // 8 type, name 4, 3886.
    int                   qi_end;            // 9 type, name 4, 3887.
    int                   edge_types;        // 10 type, name 4, 3888.
    int                   exclude_edge_types;// 11 type, name 4, 3889.
    int                   pad2;              // 12 type, name 4, 29.
    Group                 *group;            // 13 type, name 66, 503.
    FreestyleLineStyle    *linestyle;        // 14 type, name 616, 3890.
};

// 326 DNA structure 617, 5
struct FreestyleModuleConfig
{
    FreestyleModuleConfig    *next;       // 1 type, name 617, 0.
    FreestyleModuleConfig    *prev;       // 2 type, name 617, 1.
    Text                     *script;     // 3 type, name 38, 1957.
    short                    is_displayed;// 4 type, name 2, 3891.
    short                    pad[3];      // 5 type, name 2, 333.
};

// 327 DNA structure 618, 7
struct LineStyleModifier
{
    LineStyleModifier    *next;    // 1 type, name 618, 0.
    LineStyleModifier    *prev;    // 2 type, name 618, 1.
    char                 name[64]; // 3 type, name 0, 19.
    int                  type;     // 4 type, name 4, 16.
    float                influence;// 5 type, name 7, 2695.
    int                  flags;    // 6 type, name 4, 90.
    int                  blend;    // 7 type, name 4, 1754.
};

// 328 DNA structure 20, 4
struct IDPropertyData
{
    void        *pointer;// 1 type, name 11, 12.
    ListBase    group;   // 2 type, name 14, 13.
    int         val;     // 3 type, name 4, 14.
    int         val2;    // 4 type, name 4, 15.
};

// 329 DNA structure 23, 7
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

// 330 DNA structure 39, 20
struct Camera
{
    ID          id;            // 1 type, name 22, 31.
    AnimData    *adt;          // 2 type, name 36, 75.
    char        type;          // 3 type, name 0, 16.
    char        dtx;           // 4 type, name 0, 104.
    short       flag;          // 5 type, name 2, 18.
    float       passepartalpha;// 6 type, name 7, 105.
    float       clipsta;       // 7 type, name 7, 106.
    float       clipend;       // 8 type, name 7, 107.
    float       lens;          // 9 type, name 7, 108.
    float       ortho_scale;   // 10 type, name 7, 109.
    float       drawsize;      // 11 type, name 7, 110.
    float       sensor_x;      // 12 type, name 7, 111.
    float       sensor_y;      // 13 type, name 7, 112.
    float       shiftx;        // 14 type, name 7, 113.
    float       shifty;        // 15 type, name 7, 114.
    float       YF_dofdist;    // 16 type, name 7, 115.
    Ipo         *ipo;          // 17 type, name 33, 80.
    Object      *dof_ob;       // 18 type, name 29, 116.
    char        sensor_fit;    // 19 type, name 0, 117.
    char        pad[7];        // 20 type, name 0, 118.
};

// 331 DNA structure 51, 7
struct ColorBand
{
    short     tot;        // 1 type, name 2, 226.
    short     cur;        // 2 type, name 2, 63.
    char      ipotype;    // 3 type, name 0, 227.
    char      ipotype_hue;// 4 type, name 0, 228.
    char      color_mode; // 5 type, name 0, 229.
    char      pad[1];     // 6 type, name 0, 230.
    CBData    data[32];   // 7 type, name 50, 231.
};

// 332 DNA structure 61, 75
struct Lamp
{
    ID              id;                     // 1 type, name 22, 31.
    AnimData        *adt;                   // 2 type, name 36, 75.
    short           type;                   // 3 type, name 2, 16.
    short           flag;                   // 4 type, name 2, 18.
    int             mode;                   // 5 type, name 4, 334.
    short           colormodel;             // 6 type, name 2, 177.
    short           totex;                  // 7 type, name 2, 335.
    float           r;                      // 8 type, name 7, 183.
    float           g;                      // 9 type, name 7, 184.
    float           b;                      // 10 type, name 7, 185.
    float           k;                      // 11 type, name 7, 186.
    float           shdwr;                  // 12 type, name 7, 336.
    float           shdwg;                  // 13 type, name 7, 337.
    float           shdwb;                  // 14 type, name 7, 338.
    float           shdwpad;                // 15 type, name 7, 339.
    float           energy;                 // 16 type, name 7, 340.
    float           dist;                   // 17 type, name 7, 341.
    float           spotsize;               // 18 type, name 7, 342.
    float           spotblend;              // 19 type, name 7, 343.
    float           haint;                  // 20 type, name 7, 344.
    float           att1;                   // 21 type, name 7, 345.
    float           att2;                   // 22 type, name 7, 346.
    CurveMapping    *curfalloff;            // 23 type, name 55, 347.
    short           falloff_type;           // 24 type, name 2, 243.
    short           pad2;                   // 25 type, name 2, 29.
    float           clipsta;                // 26 type, name 7, 106.
    float           clipend;                // 27 type, name 7, 107.
    float           bias;                   // 28 type, name 7, 348.
    float           soft;                   // 29 type, name 7, 349.
    float           compressthresh;         // 30 type, name 7, 350.
    float           bleedbias;              // 31 type, name 7, 351.
    float           pad5;                   // 32 type, name 7, 352.
    short           bufsize;                // 33 type, name 2, 353.
    short           samp;                   // 34 type, name 2, 354.
    short           buffers;                // 35 type, name 2, 355.
    short           filtertype;             // 36 type, name 2, 356.
    char            bufflag;                // 37 type, name 0, 357.
    char            buftype;                // 38 type, name 0, 358.
    short           ray_samp;               // 39 type, name 2, 359.
    short           ray_sampy;              // 40 type, name 2, 360.
    short           ray_sampz;              // 41 type, name 2, 361.
    short           ray_samp_type;          // 42 type, name 2, 362.
    short           area_shape;             // 43 type, name 2, 363.
    float           area_size;              // 44 type, name 7, 364.
    float           area_sizey;             // 45 type, name 7, 365.
    float           area_sizez;             // 46 type, name 7, 366.
    float           adapt_thresh;           // 47 type, name 7, 367.
    short           ray_samp_method;        // 48 type, name 2, 368.
    short           shadowmap_type;         // 49 type, name 2, 369.
    short           texact;                 // 50 type, name 2, 370.
    short           shadhalostep;           // 51 type, name 2, 371.
    short           sun_effect_type;        // 52 type, name 2, 372.
    short           skyblendtype;           // 53 type, name 2, 373.
    float           horizon_brightness;     // 54 type, name 7, 374.
    float           spread;                 // 55 type, name 7, 375.
    float           sun_brightness;         // 56 type, name 7, 376.
    float           sun_size;               // 57 type, name 7, 377.
    float           backscattered_light;    // 58 type, name 7, 378.
    float           sun_intensity;          // 59 type, name 7, 379.
    float           atm_turbidity;          // 60 type, name 7, 380.
    float           atm_inscattering_factor;// 61 type, name 7, 381.
    float           atm_extinction_factor;  // 62 type, name 7, 382.
    float           atm_distance_factor;    // 63 type, name 7, 383.
    float           skyblendfac;            // 64 type, name 7, 384.
    float           sky_exposure;           // 65 type, name 7, 385.
    float           shadow_frustum_size;    // 66 type, name 7, 386.
    short           sky_colorspace;         // 67 type, name 2, 387.
    char            pad4[2];                // 68 type, name 0, 388.
    Ipo             *ipo;                   // 69 type, name 33, 80.
    MTex            *mtex[18];              // 70 type, name 48, 389.
    short           pr_texture;             // 71 type, name 2, 390.
    short           use_nodes;              // 72 type, name 2, 323.
    char            pad6[4];                // 73 type, name 0, 391.
    PreviewImage    *preview;               // 74 type, name 26, 147.
    bNodeTree       *nodetree;              // 75 type, name 58, 318.
};

// 333 DNA structure 67, 5
struct VFont
{
    ID            id;         // 1 type, name 22, 31.
    char          name[1024]; // 2 type, name 0, 34.
    void          *data;      // Note: using void* on undefined DNA type: VFontData
    PackedFile    *packedfile;// 4 type, name 25, 37.
    PackedFile    *temp_pf;   // 5 type, name 25, 532.
};

// 334 DNA structure 75, 4
struct EditNurb
{
    ListBase    nurbs;    // 1 type, name 14, 588.
    void        *keyindex;// Note: using void* on undefined DNA type: GHash
    int         shapenr;  // 3 type, name 4, 590.
    char        pad[4];   // 4 type, name 0, 568.
};

// 335 DNA structure 104, 1
struct MultiresColFace
{
    MultiresCol    col[4];// 1 type, name 103, 676.
};

// 336 DNA structure 114, 6
struct MappingInfoModifierData
{
    ModifierData    modifier;        // 1 type, name 113, 724.
    Tex             *texture;        // 2 type, name 49, 725.
    Object          *map_object;     // 3 type, name 29, 726.
    char            uvlayer_name[64];// 4 type, name 0, 727.
    int             uvlayer_tmp;     // 5 type, name 4, 728.
    int             texmapping;      // 6 type, name 4, 729.
};

// 337 DNA structure 115, 7
struct SubsurfModifierData
{
    ModifierData    modifier;    // 1 type, name 113, 724.
    short           subdivType;  // 2 type, name 2, 730.
    short           levels;      // 3 type, name 2, 711.
    short           renderLevels;// 4 type, name 2, 731.
    short           flags;       // 5 type, name 2, 90.
    void            *emCache;    // 6 type, name 11, 732.
    void            *mCache;     // 7 type, name 11, 733.
};

// 338 DNA structure 116, 5
struct LatticeModifierData
{
    ModifierData    modifier;// 1 type, name 113, 724.
    Object          *object; // 2 type, name 29, 166.
    char            name[64];// 3 type, name 0, 19.
    float           strength;// 4 type, name 7, 734.
    char            pad[4];  // 5 type, name 0, 568.
};

// 339 DNA structure 117, 5
struct CurveModifierData
{
    ModifierData    modifier;// 1 type, name 113, 724.
    Object          *object; // 2 type, name 29, 166.
    char            name[64];// 3 type, name 0, 19.
    short           defaxis; // 4 type, name 2, 735.
    char            pad[6];  // 5 type, name 0, 736.
};

// 340 DNA structure 118, 6
struct BuildModifierData
{
    ModifierData    modifier; // 1 type, name 113, 724.
    float           start;    // 2 type, name 7, 737.
    float           length;   // 3 type, name 7, 738.
    short           flag;     // 4 type, name 2, 18.
    short           randomize;// 5 type, name 2, 739.
    int             seed;     // 6 type, name 4, 740.
};

// 341 DNA structure 119, 5
struct MaskModifierData
{
    ModifierData    modifier;  // 1 type, name 113, 724.
    Object          *ob_arm;   // 2 type, name 29, 741.
    char            vgroup[64];// 3 type, name 0, 72.
    int             mode;      // 4 type, name 4, 334.
    int             flag;      // 5 type, name 4, 18.
};

// 342 DNA structure 120, 13
struct ArrayModifierData
{
    ModifierData    modifier;   // 1 type, name 113, 724.
    Object          *start_cap; // 2 type, name 29, 742.
    Object          *end_cap;   // 3 type, name 29, 743.
    Object          *curve_ob;  // 4 type, name 29, 744.
    Object          *offset_ob; // 5 type, name 29, 745.
    float           offset[3];  // 6 type, name 7, 746.
    float           scale[3];   // 7 type, name 7, 747.
    float           length;     // 8 type, name 7, 738.
    float           merge_dist; // 9 type, name 7, 748.
    int             fit_type;   // 10 type, name 4, 749.
    int             offset_type;// 11 type, name 4, 750.
    int             flags;      // 12 type, name 4, 90.
    int             count;      // 13 type, name 4, 751.
};

// 343 DNA structure 121, 5
struct MirrorModifierData
{
    ModifierData    modifier;  // 1 type, name 113, 724.
    short           axis;      // 2 type, name 2, 752.
    short           flag;      // 3 type, name 2, 18.
    float           tolerance; // 4 type, name 7, 753.
    Object          *mirror_ob;// 5 type, name 29, 754.
};

// 344 DNA structure 122, 3
struct EdgeSplitModifierData
{
    ModifierData    modifier;   // 1 type, name 113, 724.
    float           split_angle;// 2 type, name 7, 755.
    int             flags;      // 3 type, name 4, 90.
};

// 345 DNA structure 123, 13
struct BevelModifierData
{
    ModifierData    modifier;       // 1 type, name 113, 724.
    float           value;          // 2 type, name 7, 756.
    int             res;            // 3 type, name 4, 757.
    short           flags;          // 4 type, name 2, 90.
    short           val_flags;      // 5 type, name 2, 758.
    short           lim_flags;      // 6 type, name 2, 759.
    short           e_flags;        // 7 type, name 2, 760.
    short           mat;            // 8 type, name 2, 761.
    short           pad;            // 9 type, name 2, 66.
    int             pad2;           // 10 type, name 4, 29.
    float           profile;        // 11 type, name 7, 762.
    float           bevel_angle;    // 12 type, name 7, 763.
    char            defgrp_name[64];// 13 type, name 0, 764.
};

// 346 DNA structure 124, 6
struct SmokeModifierData
{
    ModifierData           modifier;// 1 type, name 113, 724.
    SmokeDomainSettings    *domain; // 2 type, name 125, 765.
    SmokeFlowSettings      *flow;   // 3 type, name 126, 766.
    SmokeCollSettings      *coll;   // 4 type, name 127, 767.
    float                  time;    // 5 type, name 7, 768.
    int                    type;    // 6 type, name 4, 16.
};

// 347 DNA structure 128, 11
struct DisplaceModifierData
{
    ModifierData    modifier;        // 1 type, name 113, 724.
    Tex             *texture;        // 2 type, name 49, 725.
    Object          *map_object;     // 3 type, name 29, 726.
    char            uvlayer_name[64];// 4 type, name 0, 727.
    int             uvlayer_tmp;     // 5 type, name 4, 728.
    int             texmapping;      // 6 type, name 4, 729.
    float           strength;        // 7 type, name 7, 734.
    int             direction;       // 8 type, name 4, 769.
    char            defgrp_name[64]; // 9 type, name 0, 764.
    float           midlevel;        // 10 type, name 7, 770.
    int             pad;             // 11 type, name 4, 66.
};

// 348 DNA structure 129, 12
struct UVProjectModifierData
{
    ModifierData    modifier;        // 1 type, name 113, 724.
    Object          *projectors[10]; // 2 type, name 29, 771.
    Image           *image;          // 3 type, name 43, 772.
    int             flags;           // 4 type, name 4, 90.
    int             num_projectors;  // 5 type, name 4, 773.
    float           aspectx;         // 6 type, name 7, 774.
    float           aspecty;         // 7 type, name 7, 775.
    float           scalex;          // 8 type, name 7, 776.
    float           scaley;          // 9 type, name 7, 777.
    char            uvlayer_name[64];// 10 type, name 0, 727.
    int             uvlayer_tmp;     // 11 type, name 4, 728.
    int             pad;             // 12 type, name 4, 66.
};

// 349 DNA structure 130, 11
struct DecimateModifierData
{
    ModifierData    modifier;       // 1 type, name 113, 724.
    float           percent;        // 2 type, name 7, 778.
    short           iter;           // 3 type, name 2, 779.
    char            delimit;        // 4 type, name 0, 780.
    char            pad;            // 5 type, name 0, 66.
    float           angle;          // 6 type, name 7, 781.
    char            defgrp_name[64];// 7 type, name 0, 764.
    short           flag;           // 8 type, name 2, 18.
    short           mode;           // 9 type, name 2, 334.
    int             face_count;     // 10 type, name 4, 782.
    int             pad2;           // 11 type, name 4, 29.
};

// 350 DNA structure 131, 5
struct SmoothModifierData
{
    ModifierData    modifier;       // 1 type, name 113, 724.
    float           fac;            // 2 type, name 7, 783.
    char            defgrp_name[64];// 3 type, name 0, 764.
    short           flag;           // 4 type, name 2, 18.
    short           repeat;         // 5 type, name 2, 784.
};

// 351 DNA structure 132, 8
struct CastModifierData
{
    ModifierData    modifier;       // 1 type, name 113, 724.
    Object          *object;        // 2 type, name 29, 166.
    float           fac;            // 3 type, name 7, 783.
    float           radius;         // 4 type, name 7, 245.
    float           size;           // 5 type, name 7, 102.
    char            defgrp_name[64];// 6 type, name 0, 764.
    short           flag;           // 7 type, name 2, 18.
    short           type;           // 8 type, name 2, 16.
};

// 352 DNA structure 133, 21
struct WaveModifierData
{
    ModifierData    modifier;        // 1 type, name 113, 724.
    Tex             *texture;        // 2 type, name 49, 725.
    Object          *map_object;     // 3 type, name 29, 726.
    char            uvlayer_name[64];// 4 type, name 0, 727.
    int             uvlayer_tmp;     // 5 type, name 4, 728.
    int             texmapping;      // 6 type, name 4, 729.
    Object          *objectcenter;   // 7 type, name 29, 785.
    char            defgrp_name[64]; // 8 type, name 0, 764.
    short           flag;            // 9 type, name 2, 18.
    short           pad;             // 10 type, name 2, 66.
    float           startx;          // 11 type, name 7, 786.
    float           starty;          // 12 type, name 7, 787.
    float           height;          // 13 type, name 7, 788.
    float           width;           // 14 type, name 7, 603.
    float           narrow;          // 15 type, name 7, 789.
    float           speed;           // 16 type, name 7, 790.
    float           damp;            // 17 type, name 7, 791.
    float           falloff;         // 18 type, name 7, 792.
    float           timeoffs;        // 19 type, name 7, 793.
    float           lifetime;        // 20 type, name 7, 794.
    float           pad1;            // 21 type, name 7, 68.
};

// 353 DNA structure 134, 7
struct ArmatureModifierData
{
    ModifierData    modifier;       // 1 type, name 113, 724.
    short           deformflag;     // 2 type, name 2, 795.
    short           multi;          // 3 type, name 2, 796.
    int             pad2;           // 4 type, name 4, 29.
    Object          *object;        // 5 type, name 29, 166.
    float           *prevCos;       // 6 type, name 7, 797.
    char            defgrp_name[64];// 7 type, name 0, 764.
};

// 354 DNA structure 135, 10
struct HookModifierData
{
    ModifierData    modifier;       // 1 type, name 113, 724.
    Object          *object;        // 2 type, name 29, 166.
    char            subtarget[64];  // 3 type, name 0, 798.
    float           parentinv[4][4];// 4 type, name 7, 799.
    float           cent[3];        // 5 type, name 7, 800.
    float           falloff;        // 6 type, name 7, 792.
    int             *indexar;       // 7 type, name 4, 801.
    int             totindex;       // 8 type, name 4, 802.
    float           force;          // 9 type, name 7, 803.
    char            name[64];       // 10 type, name 0, 19.
};

// 355 DNA structure 136, 1
struct SoftbodyModifierData
{
    ModifierData    modifier;// 1 type, name 113, 724.
};

// 356 DNA structure 142, 13
struct CollisionModifierData
{
    ModifierData    modifier;     // 1 type, name 113, 724.
    MVert           *x;           // 2 type, name 89, 809.
    MVert           *xnew;        // 3 type, name 89, 810.
    MVert           *xold;        // 4 type, name 89, 811.
    MVert           *current_xnew;// 5 type, name 89, 812.
    MVert           *current_x;   // 6 type, name 89, 813.
    MVert           *current_v;   // 7 type, name 89, 814.
    MFace           *mfaces;      // 8 type, name 86, 815.
    int             numverts;     // 9 type, name 4, 816.
    int             numfaces;     // 10 type, name 4, 817.
    float           time_x;       // 11 type, name 7, 818.
    float           time_xnew;    // 12 type, name 7, 819.
    void            *bvhtree;     // Note: using void* on undefined DNA type: BVHTree
};

// 357 DNA structure 144, 7
struct SurfaceModifierData
{
    ModifierData       modifier;// 1 type, name 113, 724.
    MVert              *x;      // 2 type, name 89, 809.
    MVert              *v;      // 3 type, name 89, 821.
    void               *dm;     // Note: using void* on undefined DNA type: DerivedMesh
    void               *bvhtree;// Note: using void* on undefined DNA type: BVHTreeFromMesh
    int                cfra;    // 6 type, name 4, 823.
    int                numverts;// 7 type, name 4, 816.
};

// 358 DNA structure 147, 4
struct BooleanModifierData
{
    ModifierData    modifier; // 1 type, name 113, 724.
    Object          *object;  // 2 type, name 29, 166.
    int             operation;// 3 type, name 4, 824.
    int             pad;      // 4 type, name 4, 66.
};

// 359 DNA structure 150, 22
struct MeshDeformModifierData
{
    ModifierData     modifier;       // 1 type, name 113, 724.
    Object           *object;        // 2 type, name 29, 166.
    char             defgrp_name[64];// 3 type, name 0, 764.
    short            gridsize;       // 4 type, name 2, 827.
    short            flag;           // 5 type, name 2, 18.
    short            pad[2];         // 6 type, name 2, 573.
    MDefInfluence    *bindinfluences;// 7 type, name 148, 828.
    int              *bindoffsets;   // 8 type, name 4, 829.
    float            *bindcagecos;   // 9 type, name 7, 830.
    int              totvert;        // 10 type, name 4, 53.
    int              totcagevert;    // 11 type, name 4, 831.
    MDefCell         *dyngrid;       // 12 type, name 149, 832.
    MDefInfluence    *dyninfluences; // 13 type, name 148, 833.
    int              *dynverts;      // 14 type, name 4, 834.
    int              dyngridsize;    // 15 type, name 4, 835.
    int              totinfluence;   // 16 type, name 4, 826.
    float            dyncellmin[3];  // 17 type, name 7, 836.
    float            dyncellwidth;   // 18 type, name 7, 837.
    float            bindmat[4][4];  // 19 type, name 7, 838.
    float            *bindweights;   // 20 type, name 7, 839.
    float            *bindcos;       // 21 type, name 7, 840.
    void             (*bindfunc)();  // 22 type, name 11, 841.
};

// 360 DNA structure 151, 8
struct ParticleSystemModifierData
{
    ModifierData      modifier; // 1 type, name 113, 724.
    ParticleSystem    *psys;    // 2 type, name 152, 842.
    void              *dm;      // Note: using void* on undefined DNA type: DerivedMesh
    int               totdmvert;// 4 type, name 4, 843.
    int               totdmedge;// 5 type, name 4, 844.
    int               totdmface;// 6 type, name 4, 845.
    short             flag;     // 7 type, name 2, 18.
    short             pad;      // 8 type, name 2, 66.
};

// 361 DNA structure 153, 8
struct ParticleInstanceModifierData
{
    ModifierData    modifier;       // 1 type, name 113, 724.
    Object          *ob;            // 2 type, name 29, 44.
    short           psys;           // 3 type, name 2, 249.
    short           flag;           // 4 type, name 2, 18.
    short           axis;           // 5 type, name 2, 752.
    short           pad;            // 6 type, name 2, 66.
    float           position;       // 7 type, name 7, 846.
    float           random_position;// 8 type, name 7, 847.
};

// 362 DNA structure 154, 6
struct ExplodeModifierData
{
    ModifierData    modifier;  // 1 type, name 113, 724.
    int             *facepa;   // 2 type, name 4, 848.
    short           flag;      // 3 type, name 2, 18.
    short           vgroup;    // 4 type, name 2, 849.
    float           protect;   // 5 type, name 7, 850.
    char            uvname[64];// 6 type, name 0, 168.
};

// 363 DNA structure 155, 8
struct MultiresModifierData
{
    ModifierData    modifier; // 1 type, name 113, 724.
    char            lvl;      // 2 type, name 0, 851.
    char            sculptlvl;// 3 type, name 0, 852.
    char            renderlvl;// 4 type, name 0, 717.
    char            totlvl;   // 5 type, name 0, 853.
    char            simple;   // 6 type, name 0, 854.
    char            flags;    // 7 type, name 0, 90.
    char            pad[2];   // 8 type, name 0, 573.
};

// 364 DNA structure 156, 3
struct FluidsimModifierData
{
    ModifierData        modifier;    // 1 type, name 113, 724.
    FluidsimSettings    *fss;        // 2 type, name 157, 855.
    PointCache          *point_cache;// 3 type, name 141, 807.
};

// 365 DNA structure 158, 12
struct ShrinkwrapModifierData
{
    ModifierData    modifier;       // 1 type, name 113, 724.
    Object          *target;        // 2 type, name 29, 856.
    Object          *auxTarget;     // 3 type, name 29, 857.
    char            vgroup_name[64];// 4 type, name 0, 858.
    float           keepDist;       // 5 type, name 7, 859.
    short           shrinkType;     // 6 type, name 2, 860.
    char            shrinkOpts;     // 7 type, name 0, 861.
    char            pad1;           // 8 type, name 0, 68.
    float           projLimit;      // 9 type, name 7, 862.
    char            projAxis;       // 10 type, name 0, 863.
    char            subsurfLevels;  // 11 type, name 0, 864.
    char            pad[2];         // 12 type, name 0, 573.
};

// 366 DNA structure 159, 8
struct SimpleDeformModifierData
{
    ModifierData    modifier;       // 1 type, name 113, 724.
    Object          *origin;        // 2 type, name 29, 865.
    char            vgroup_name[64];// 3 type, name 0, 858.
    float           factor;         // 4 type, name 7, 866.
    float           limit[2];       // 5 type, name 7, 867.
    char            mode;           // 6 type, name 0, 334.
    char            axis;           // 7 type, name 0, 752.
    char            pad[2];         // 8 type, name 0, 573.
};

// 367 DNA structure 160, 1
struct ShapeKeyModifierData
{
    ModifierData    modifier;// 1 type, name 113, 724.
};

// 368 DNA structure 161, 13
struct SolidifyModifierData
{
    ModifierData    modifier;       // 1 type, name 113, 724.
    char            defgrp_name[64];// 2 type, name 0, 764.
    float           offset;         // 3 type, name 7, 122.
    float           offset_fac;     // 4 type, name 7, 868.
    float           offset_fac_vg;  // 5 type, name 7, 869.
    float           offset_clamp;   // 6 type, name 7, 870.
    float           pad;            // 7 type, name 7, 66.
    float           crease_inner;   // 8 type, name 7, 871.
    float           crease_outer;   // 9 type, name 7, 872.
    float           crease_rim;     // 10 type, name 7, 873.
    int             flag;           // 11 type, name 4, 18.
    short           mat_ofs;        // 12 type, name 2, 874.
    short           mat_ofs_rim;    // 13 type, name 2, 875.
};

// 369 DNA structure 162, 10
struct ScrewModifierData
{
    ModifierData    modifier;    // 1 type, name 113, 724.
    Object          *ob_axis;    // 2 type, name 29, 876.
    int             steps;       // 3 type, name 4, 877.
    int             render_steps;// 4 type, name 4, 878.
    int             iter;        // 5 type, name 4, 779.
    float           screw_ofs;   // 6 type, name 7, 879.
    float           angle;       // 7 type, name 7, 781.
    char            axis;        // 8 type, name 0, 752.
    char            pad;         // 9 type, name 0, 66.
    short           flag;        // 10 type, name 2, 18.
};

// 370 DNA structure 163, 29
struct OceanModifierData
{
    ModifierData    modifier;         // 1 type, name 113, 724.
    void            *ocean;           // Note: using void* on undefined DNA type: Ocean
    void            *oceancache;      // Note: using void* on undefined DNA type: OceanCache
    int             resolution;       // 4 type, name 4, 882.
    int             spatial_size;     // 5 type, name 4, 883.
    float           wind_velocity;    // 6 type, name 7, 884.
    float           damp;             // 7 type, name 7, 791.
    float           smallest_wave;    // 8 type, name 7, 885.
    float           depth;            // 9 type, name 7, 240.
    float           wave_alignment;   // 10 type, name 7, 886.
    float           wave_direction;   // 11 type, name 7, 887.
    float           wave_scale;       // 12 type, name 7, 888.
    float           chop_amount;      // 13 type, name 7, 889.
    float           foam_coverage;    // 14 type, name 7, 890.
    float           time;             // 15 type, name 7, 768.
    int             bakestart;        // 16 type, name 4, 891.
    int             bakeend;          // 17 type, name 4, 892.
    char            cachepath[1024];  // 18 type, name 0, 893.
    char            foamlayername[64];// 19 type, name 0, 894.
    char            cached;           // 20 type, name 0, 895.
    char            geometry_mode;    // 21 type, name 0, 896.
    char            flag;             // 22 type, name 0, 18.
    char            refresh;          // 23 type, name 0, 897.
    short           repeat_x;         // 24 type, name 2, 898.
    short           repeat_y;         // 25 type, name 2, 899.
    int             seed;             // 26 type, name 4, 740.
    float           size;             // 27 type, name 7, 102.
    float           foam_fade;        // 28 type, name 7, 900.
    int             pad;              // 29 type, name 4, 66.
};

// 371 DNA structure 166, 15
struct WarpModifierData
{
    ModifierData    modifier;        // 1 type, name 113, 724.
    Tex             *texture;        // 2 type, name 49, 725.
    Object          *map_object;     // 3 type, name 29, 726.
    char            uvlayer_name[64];// 4 type, name 0, 727.
    int             uvlayer_tmp;     // 5 type, name 4, 728.
    int             texmapping;      // 6 type, name 4, 729.
    Object          *object_from;    // 7 type, name 29, 901.
    Object          *object_to;      // 8 type, name 29, 902.
    CurveMapping    *curfalloff;     // 9 type, name 55, 347.
    char            defgrp_name[64]; // 10 type, name 0, 764.
    float           strength;        // 11 type, name 7, 734.
    float           falloff_radius;  // 12 type, name 7, 903.
    char            flag;            // 13 type, name 0, 18.
    char            falloff_type;    // 14 type, name 0, 243.
    char            pad[6];          // 15 type, name 0, 736.
};

// 372 DNA structure 167, 16
struct WeightVGEditModifierData
{
    ModifierData    modifier;                 // 1 type, name 113, 724.
    char            defgrp_name[64];          // 2 type, name 0, 764.
    short           edit_flags;               // 3 type, name 2, 904.
    short           falloff_type;             // 4 type, name 2, 243.
    float           default_weight;           // 5 type, name 7, 905.
    CurveMapping    *cmap_curve;              // 6 type, name 55, 906.
    float           add_threshold;            // 7 type, name 7, 907.
    float           rem_threshold;            // 8 type, name 7, 908.
    float           mask_constant;            // 9 type, name 7, 909.
    char            mask_defgrp_name[64];     // 10 type, name 0, 910.
    int             mask_tex_use_channel;     // 11 type, name 4, 911.
    Tex             *mask_texture;            // 12 type, name 49, 912.
    Object          *mask_tex_map_obj;        // 13 type, name 29, 913.
    int             mask_tex_mapping;         // 14 type, name 4, 914.
    char            mask_tex_uvlayer_name[64];// 15 type, name 0, 915.
    int             pad_i1;                   // 16 type, name 4, 916.
};

// 373 DNA structure 168, 16
struct WeightVGMixModifierData
{
    ModifierData    modifier;                 // 1 type, name 113, 724.
    char            defgrp_name_a[64];        // 2 type, name 0, 917.
    char            defgrp_name_b[64];        // 3 type, name 0, 918.
    float           default_weight_a;         // 4 type, name 7, 919.
    float           default_weight_b;         // 5 type, name 7, 920.
    char            mix_mode;                 // 6 type, name 0, 921.
    char            mix_set;                  // 7 type, name 0, 922.
    char            pad_c1[6];                // 8 type, name 0, 923.
    float           mask_constant;            // 9 type, name 7, 909.
    char            mask_defgrp_name[64];     // 10 type, name 0, 910.
    int             mask_tex_use_channel;     // 11 type, name 4, 911.
    Tex             *mask_texture;            // 12 type, name 49, 912.
    Object          *mask_tex_map_obj;        // 13 type, name 29, 913.
    int             mask_tex_mapping;         // 14 type, name 4, 914.
    char            mask_tex_uvlayer_name[64];// 15 type, name 0, 915.
    int             pad_i1;                   // 16 type, name 4, 916.
};

// 374 DNA structure 169, 16
struct WeightVGProximityModifierData
{
    ModifierData    modifier;                 // 1 type, name 113, 724.
    char            defgrp_name[64];          // 2 type, name 0, 764.
    int             proximity_mode;           // 3 type, name 4, 924.
    int             proximity_flags;          // 4 type, name 4, 925.
    Object          *proximity_ob_target;     // 5 type, name 29, 926.
    float           mask_constant;            // 6 type, name 7, 909.
    char            mask_defgrp_name[64];     // 7 type, name 0, 910.
    int             mask_tex_use_channel;     // 8 type, name 4, 911.
    Tex             *mask_texture;            // 9 type, name 49, 912.
    Object          *mask_tex_map_obj;        // 10 type, name 29, 913.
    int             mask_tex_mapping;         // 11 type, name 4, 914.
    char            mask_tex_uvlayer_name[64];// 12 type, name 0, 915.
    float           min_dist;                 // 13 type, name 7, 927.
    float           max_dist;                 // 14 type, name 7, 928.
    short           falloff_type;             // 15 type, name 2, 243.
    short           pad_s1;                   // 16 type, name 2, 929.
};

// 375 DNA structure 170, 5
struct DynamicPaintModifierData
{
    ModifierData                  modifier;// 1 type, name 113, 724.
    DynamicPaintCanvasSettings    *canvas; // 2 type, name 171, 930.
    DynamicPaintBrushSettings     *brush;  // 3 type, name 172, 931.
    int                           type;    // 4 type, name 4, 16.
    int                           pad;     // 5 type, name 4, 66.
};

// 376 DNA structure 173, 8
struct RemeshModifierData
{
    ModifierData    modifier;   // 1 type, name 113, 724.
    float           threshold;  // 2 type, name 7, 932.
    float           scale;      // 3 type, name 7, 933.
    float           hermite_num;// 4 type, name 7, 934.
    char            depth;      // 5 type, name 0, 240.
    char            flag;       // 6 type, name 0, 18.
    char            mode;       // 7 type, name 0, 334.
    char            pad;        // 8 type, name 0, 66.
};

// 377 DNA structure 174, 5
struct SkinModifierData
{
    ModifierData    modifier;        // 1 type, name 113, 724.
    float           branch_smoothing;// 2 type, name 7, 935.
    char            flag;            // 3 type, name 0, 18.
    char            symmetry_axes;   // 4 type, name 0, 936.
    char            pad[2];          // 5 type, name 0, 573.
};

// 378 DNA structure 175, 5
struct TriangulateModifierData
{
    ModifierData    modifier;   // 1 type, name 113, 724.
    int             flag;       // 2 type, name 4, 18.
    int             quad_method;// 3 type, name 4, 937.
    int             ngon_method;// 4 type, name 4, 938.
    int             pad;        // 5 type, name 4, 66.
};

// 379 DNA structure 176, 7
struct LaplacianSmoothModifierData
{
    ModifierData    modifier;       // 1 type, name 113, 724.
    float           lambda;         // 2 type, name 7, 939.
    float           lambda_border;  // 3 type, name 7, 940.
    float           pad1;           // 4 type, name 7, 68.
    char            defgrp_name[64];// 5 type, name 0, 764.
    short           flag;           // 6 type, name 2, 18.
    short           repeat;         // 7 type, name 2, 784.
};

// 380 DNA structure 177, 11
struct UVWarpModifierData
{
    ModifierData    modifier;        // 1 type, name 113, 724.
    char            axis_u;          // 2 type, name 0, 941.
    char            axis_v;          // 3 type, name 0, 942.
    char            pad[6];          // 4 type, name 0, 736.
    float           center[2];       // 5 type, name 7, 943.
    Object          *object_src;     // 6 type, name 29, 944.
    char            bone_src[64];    // 7 type, name 0, 945.
    Object          *object_dst;     // 8 type, name 29, 946.
    char            bone_dst[64];    // 9 type, name 0, 947.
    char            vgroup_name[64]; // 10 type, name 0, 858.
    char            uvlayer_name[64];// 11 type, name 0, 727.
};

// 381 DNA structure 178, 18
struct MeshCacheModifierData
{
    ModifierData    modifier;      // 1 type, name 113, 724.
    char            flag;          // 2 type, name 0, 18.
    char            type;          // 3 type, name 0, 16.
    char            time_mode;     // 4 type, name 0, 948.
    char            play_mode;     // 5 type, name 0, 949.
    char            forward_axis;  // 6 type, name 0, 950.
    char            up_axis;       // 7 type, name 0, 951.
    char            flip_axis;     // 8 type, name 0, 952.
    char            interp;        // 9 type, name 0, 953.
    float           factor;        // 10 type, name 7, 866.
    char            deform_mode;   // 11 type, name 0, 954.
    char            pad[7];        // 12 type, name 0, 118.
    float           frame_start;   // 13 type, name 7, 955.
    float           frame_scale;   // 14 type, name 7, 956.
    float           eval_frame;    // 15 type, name 7, 957.
    float           eval_time;     // 16 type, name 7, 958.
    float           eval_factor;   // 17 type, name 7, 959.
    char            filepath[1024];// 18 type, name 0, 35.
};

// 382 DNA structure 179, 8
struct LaplacianDeformModifierData
{
    ModifierData    modifier;           // 1 type, name 113, 724.
    char            anchor_grp_name[64];// 2 type, name 0, 960.
    int             total_verts;        // 3 type, name 4, 961.
    int             repeat;             // 4 type, name 4, 784.
    float           *vertexco;          // 5 type, name 7, 962.
    void            *cache_system;      // 6 type, name 11, 963.
    short           flag;               // 7 type, name 2, 18.
    short           pad[3];             // 8 type, name 2, 333.
};

// 383 DNA structure 180, 9
struct WireframeModifierData
{
    ModifierData    modifier;       // 1 type, name 113, 724.
    char            defgrp_name[64];// 2 type, name 0, 764.
    float           offset;         // 3 type, name 7, 122.
    float           offset_fac;     // 4 type, name 7, 868.
    float           offset_fac_vg;  // 5 type, name 7, 869.
    float           crease_weight;  // 6 type, name 7, 964.
    short           flag;           // 7 type, name 2, 18.
    short           mat_ofs;        // 8 type, name 2, 874.
    short           pad[2];         // 9 type, name 2, 573.
};

// 384 DNA structure 182, 27
struct Lattice
{
    ID             id;        // 1 type, name 22, 31.
    AnimData       *adt;      // 2 type, name 36, 75.
    short          pntsu;     // 3 type, name 2, 571.
    short          pntsv;     // 4 type, name 2, 572.
    short          pntsw;     // 5 type, name 2, 966.
    short          flag;      // 6 type, name 2, 18.
    short          opntsu;    // 7 type, name 2, 967.
    short          opntsv;    // 8 type, name 2, 968.
    short          opntsw;    // 9 type, name 2, 969.
    short          pad2;      // 10 type, name 2, 29.
    char           typeu;     // 11 type, name 0, 970.
    char           typev;     // 12 type, name 0, 971.
    char           typew;     // 13 type, name 0, 972.
    char           pad3;      // 14 type, name 0, 500.
    int            actbp;     // 15 type, name 4, 973.
    float          fu;        // 16 type, name 7, 974.
    float          fv;        // 17 type, name 7, 975.
    float          fw;        // 18 type, name 7, 976.
    float          du;        // 19 type, name 7, 977.
    float          dv;        // 20 type, name 7, 978.
    float          dw;        // 21 type, name 7, 979.
    BPoint         *def;      // 22 type, name 31, 980.
    Ipo            *ipo;      // 23 type, name 33, 80.
    Key            *key;      // 24 type, name 35, 596.
    MDeformVert    *dvert;    // 25 type, name 91, 652.
    char           vgroup[64];// 26 type, name 0, 72.
    EditLatt       *editlatt; // 27 type, name 181, 981.
};

// 385 DNA structure 202, 9
struct PTCacheMem
{
    PTCacheMem    *next;     // 1 type, name 202, 0.
    PTCacheMem    *prev;     // 2 type, name 202, 1.
    int           frame;     // 3 type, name 4, 1142.
    int           totpoint;  // 4 type, name 4, 1143.
    int           data_types;// 5 type, name 4, 1144.
    int           flag;      // 6 type, name 4, 18.
    void          *data[8];  // 7 type, name 11, 1145.
    void          *cur[8];   // 8 type, name 11, 1146.
    ListBase      extradata; // 9 type, name 14, 1147.
};

// 386 DNA structure 141, 23
struct PointCache
{
    PointCache     *next;         // 1 type, name 141, 0.
    PointCache     *prev;         // 2 type, name 141, 1.
    int            flag;          // 3 type, name 4, 18.
    int            step;          // 4 type, name 4, 1148.
    int            simframe;      // 5 type, name 4, 1149.
    int            startframe;    // 6 type, name 4, 1150.
    int            endframe;      // 7 type, name 4, 1151.
    int            editframe;     // 8 type, name 4, 1152.
    int            last_exact;    // 9 type, name 4, 1153.
    int            last_valid;    // 10 type, name 4, 1154.
    int            pad;           // 11 type, name 4, 66.
    int            totpoint;      // 12 type, name 4, 1143.
    int            index;         // 13 type, name 4, 413.
    short          compression;   // 14 type, name 2, 1155.
    short          rt;            // 15 type, name 2, 56.
    char           name[64];      // 16 type, name 0, 19.
    char           prev_name[64]; // 17 type, name 0, 1156.
    char           info[64];      // 18 type, name 0, 1157.
    char           path[1024];    // 19 type, name 0, 1158.
    char           *cached_frames;// 20 type, name 0, 1159.
    ListBase       mem_cache;     // 21 type, name 14, 1160.
    void           *edit;         // Note: using void* on undefined DNA type: PTCacheEdit
    void           (*free_edit)();// 23 type, name 11, 1162.
};

// 387 DNA structure 193, 54
struct SoftBody
{
    int                totpoint;            // 1 type, name 4, 1143.
    int                totspring;           // 2 type, name 4, 1191.
    void               *bpoint;             // Note: using void* on undefined DNA type: BodyPoint
    void               *bspring;            // Note: using void* on undefined DNA type: BodySpring
    char               pad;                 // 5 type, name 0, 66.
    char               msg_lock;            // 6 type, name 0, 1194.
    short              msg_value;           // 7 type, name 2, 1195.
    float              nodemass;            // 8 type, name 7, 1196.
    char               namedVG_Mass[64];    // 9 type, name 0, 1197.
    float              grav;                // 10 type, name 7, 1198.
    float              mediafrict;          // 11 type, name 7, 1199.
    float              rklimit;             // 12 type, name 7, 1200.
    float              physics_speed;       // 13 type, name 7, 1201.
    float              goalspring;          // 14 type, name 7, 1202.
    float              goalfrict;           // 15 type, name 7, 1203.
    float              mingoal;             // 16 type, name 7, 1204.
    float              maxgoal;             // 17 type, name 7, 1205.
    float              defgoal;             // 18 type, name 7, 1206.
    short              vertgroup;           // 19 type, name 2, 1207.
    char               namedVG_Softgoal[64];// 20 type, name 0, 1208.
    short              fuzzyness;           // 21 type, name 2, 1209.
    float              inspring;            // 22 type, name 7, 1210.
    float              infrict;             // 23 type, name 7, 1211.
    char               namedVG_Spring_K[64];// 24 type, name 0, 1212.
    int                sfra;                // 25 type, name 4, 123.
    int                efra;                // 26 type, name 4, 1213.
    int                interval;            // 27 type, name 4, 1214.
    short              local;               // 28 type, name 2, 1215.
    short              solverflags;         // 29 type, name 2, 1216.
    SBVertex           **keys;              // 30 type, name 204, 1217.
    int                totpointkey;         // 31 type, name 4, 1218.
    int                totkey;              // 32 type, name 4, 82.
    float              secondspring;        // 33 type, name 7, 1219.
    float              colball;             // 34 type, name 7, 1220.
    float              balldamp;            // 35 type, name 7, 1221.
    float              ballstiff;           // 36 type, name 7, 1222.
    short              sbc_mode;            // 37 type, name 2, 1223.
    short              aeroedge;            // 38 type, name 2, 1224.
    short              minloops;            // 39 type, name 2, 1225.
    short              maxloops;            // 40 type, name 2, 1226.
    short              choke;               // 41 type, name 2, 1227.
    short              solver_ID;           // 42 type, name 2, 1228.
    short              plastic;             // 43 type, name 2, 1229.
    short              springpreload;       // 44 type, name 2, 1230.
    void               *scratch;            // Note: using void* on undefined DNA type: SBScratch
    float              shearstiff;          // 46 type, name 7, 1232.
    float              inpush;              // 47 type, name 7, 1233.
    PointCache         *pointcache;         // 48 type, name 141, 1234.
    ListBase           ptcaches;            // 49 type, name 14, 808.
    EffectorWeights    *effector_weights;   // 50 type, name 200, 1235.
    float              lcom[3];             // 51 type, name 7, 1236.
    float              lrot[3][3];          // 52 type, name 7, 1237.
    float              lscale[3][3];        // 53 type, name 7, 1238.
    int                last_frame;          // 54 type, name 4, 1239.
};

// 388 DNA structure 232, 13
struct ImagePaintSettings
{
    Paint    paint;              // 1 type, name 229, 1578.
    short    flag;               // 2 type, name 2, 18.
    short    missing_data;       // 3 type, name 2, 1579.
    short    seam_bleed;         // 4 type, name 2, 1580.
    short    normal_angle;       // 5 type, name 2, 1581.
    short    screen_grab_size[2];// 6 type, name 2, 1582.
    int      mode;               // 7 type, name 4, 334.
    void     *paintcursor;       // 8 type, name 11, 1583.
    Image    *stencil;           // 9 type, name 43, 1584.
    Image    *clone;             // 10 type, name 43, 1585.
    Image    *canvas;            // 11 type, name 43, 930.
    float    stencil_col[3];     // 12 type, name 7, 1586.
    float    pad1;               // 13 type, name 7, 68.
};

// 389 DNA structure 234, 14
struct ParticleEditSettings
{
    short                flag;        // 1 type, name 2, 18.
    short                totrekey;    // 2 type, name 2, 1588.
    short                totaddkey;   // 3 type, name 2, 1589.
    short                brushtype;   // 4 type, name 2, 1590.
    ParticleBrushData    brush[7];    // 5 type, name 233, 1591.
    void                 *paintcursor;// 6 type, name 11, 1583.
    float                emitterdist; // 7 type, name 7, 1592.
    float                rt;          // 8 type, name 7, 56.
    int                  selectmode;  // 9 type, name 4, 1593.
    int                  edittype;    // 10 type, name 4, 1594.
    int                  draw_step;   // 11 type, name 4, 1595.
    int                  fade_frames; // 12 type, name 4, 1596.
    Scene                *scene;      // 13 type, name 41, 119.
    Object               *object;     // 14 type, name 29, 166.
};

// 390 DNA structure 235, 9
struct Sculpt
{
    Paint     paint;               // 1 type, name 229, 1578.
    int       flags;               // 2 type, name 4, 90.
    int       radial_symm[3];      // 3 type, name 4, 1597.
    float     detail_size;         // 4 type, name 7, 1598.
    int       symmetrize_direction;// 5 type, name 4, 1599.
    float     gravity_factor;      // 6 type, name 7, 1600.
    float     constant_detail;     // 7 type, name 7, 1601.
    Object    *gravity_object;     // 8 type, name 29, 1602.
    void      *pad2;               // 9 type, name 11, 1603.
};

// 391 DNA structure 236, 1
struct UvSculpt
{
    Paint    paint;// 1 type, name 229, 1578.
};

// 392 DNA structure 237, 7
struct VPaint
{
    Paint          paint;       // 1 type, name 229, 1578.
    short          flag;        // 2 type, name 2, 18.
    short          pad;         // 3 type, name 2, 66.
    int            tot;         // 4 type, name 4, 226.
    int            *vpaint_prev;// 5 type, name 4, 1604.
    MDeformVert    *wpaint_prev;// 6 type, name 91, 1605.
    void           *paintcursor;// 7 type, name 11, 1583.
};

// 393 DNA structure 259, 6
struct SpaceLink
{
    SpaceLink    *next;          // 1 type, name 259, 0.
    SpaceLink    *prev;          // 2 type, name 259, 1.
    ListBase     regionbase;     // 3 type, name 14, 1792.
    int          spacetype;      // 4 type, name 4, 1793.
    float        blockscale;     // 5 type, name 7, 1794.
    short        blockhandler[8];// 6 type, name 2, 1795.
};

// 394 DNA structure 262, 8
struct SpaceInfo
{
    SpaceLink    *next;          // 1 type, name 259, 0.
    SpaceLink    *prev;          // 2 type, name 259, 1.
    ListBase     regionbase;     // 3 type, name 14, 1792.
    int          spacetype;      // 4 type, name 4, 1793.
    float        blockscale;     // 5 type, name 7, 1794.
    short        blockhandler[8];// 6 type, name 2, 1795.
    char         rpt_mask;       // 7 type, name 0, 1848.
    char         pad[7];         // 8 type, name 0, 118.
};

// 395 DNA structure 277, 16
struct SpaceFile
{
    SpaceLink           *next;              // 1 type, name 259, 0.
    SpaceLink           *prev;              // 2 type, name 259, 1.
    ListBase            regionbase;         // 3 type, name 14, 1792.
    int                 spacetype;          // 4 type, name 4, 1793.
    int                 scroll_offset;      // 5 type, name 4, 1901.
    FileSelectParams    *params;            // 6 type, name 276, 1902.
    void                *files;             // Note: using void* on undefined DNA type: FileList
    ListBase            *folders_prev;      // 8 type, name 14, 1904.
    ListBase            *folders_next;      // 9 type, name 14, 1905.
    wmOperator          *op;                // 10 type, name 279, 1906.
    void                *smoothscroll_timer;// Note: using void* on undefined DNA type: wmTimer
    void                *layout;            // Note: using void* on undefined DNA type: FileLayout
    short               recentnr;           // 13 type, name 2, 1909.
    short               bookmarknr;         // 14 type, name 2, 1910.
    short               systemnr;           // 15 type, name 2, 1911.
    short               pad2;               // 16 type, name 2, 29.
};

// 396 DNA structure 285, 10
struct Script
{
    ID      id;                 // 1 type, name 22, 31.
    void    *py_draw;           // 2 type, name 11, 1949.
    void    *py_event;          // 3 type, name 11, 1950.
    void    *py_button;         // 4 type, name 11, 1951.
    void    *py_browsercallback;// 5 type, name 11, 1952.
    void    *py_globaldict;     // 6 type, name 11, 1953.
    int     flags;              // 7 type, name 4, 90.
    int     lastspace;          // 8 type, name 4, 1954.
    char    scriptname[1024];   // 9 type, name 0, 1955.
    char    scriptarg[256];     // 10 type, name 0, 1956.
};

// 397 DNA structure 286, 10
struct SpaceScript
{
    SpaceLink    *next;     // 1 type, name 259, 0.
    SpaceLink    *prev;     // 2 type, name 259, 1.
    ListBase     regionbase;// 3 type, name 14, 1792.
    int          spacetype; // 4 type, name 4, 1793.
    float        blockscale;// 5 type, name 7, 1794.
    Script       *script;   // 6 type, name 285, 1957.
    short        flags;     // 7 type, name 2, 90.
    short        menunr;    // 8 type, name 2, 1927.
    int          pad1;      // 9 type, name 4, 68.
    void         *but_refs; // 10 type, name 11, 1958.
};

// 398 DNA structure 287, 7
struct bNodeTreePath
{
    bNodeTreePath       *next;         // 1 type, name 287, 0.
    bNodeTreePath       *prev;         // 2 type, name 287, 1.
    bNodeTree           *nodetree;     // 3 type, name 58, 318.
    bNodeInstanceKey    parent_key;    // 4 type, name 288, 1959.
    int                 pad;           // 5 type, name 4, 66.
    float               view_center[2];// 6 type, name 7, 1960.
    char                node_name[64]; // 7 type, name 0, 1961.
};

// 399 DNA structure 290, 10
struct SpaceLogic
{
    SpaceLink    *next;          // 1 type, name 259, 0.
    SpaceLink    *prev;          // 2 type, name 259, 1.
    ListBase     regionbase;     // 3 type, name 14, 1792.
    int          spacetype;      // 4 type, name 4, 1793.
    float        blockscale;     // 5 type, name 7, 1794.
    short        blockhandler[8];// 6 type, name 2, 1795.
    short        flag;           // 7 type, name 2, 18.
    short        scaflag;        // 8 type, name 2, 1029.
    int          pad;            // 9 type, name 4, 66.
    bGPdata      *gpd;           // 10 type, name 188, 998.
};

// 400 DNA structure 293, 7
struct SpaceUserPref
{
    SpaceLink    *next;      // 1 type, name 259, 0.
    SpaceLink    *prev;      // 2 type, name 259, 1.
    ListBase     regionbase; // 3 type, name 14, 1792.
    int          spacetype;  // 4 type, name 4, 1793.
    char         pad[3];     // 5 type, name 0, 333.
    char         filter_type;// 6 type, name 0, 1979.
    char         filter[64]; // 7 type, name 0, 1980.
};

// 401 DNA structure 313, 6
struct ScrVert
{
    ScrVert    *next;   // 1 type, name 313, 0.
    ScrVert    *prev;   // 2 type, name 313, 1.
    ScrVert    *newv;   // 3 type, name 313, 2400.
    vec2s      vec;     // 4 type, name 15, 2401.
    short      flag;    // 5 type, name 2, 18.
    short      editflag;// 6 type, name 2, 672.
};

// 402 DNA structure 333, 13
struct Strip
{
    Strip                             *next;              // 1 type, name 333, 0.
    Strip                             *prev;              // 2 type, name 333, 1.
    int                               us;                 // 3 type, name 4, 27.
    int                               done;               // 4 type, name 4, 2484.
    int                               startstill;         // 5 type, name 4, 2485.
    int                               endstill;           // 6 type, name 4, 2486.
    StripElem                         *stripdata;         // 7 type, name 328, 2487.
    char                              dir[768];           // 8 type, name 0, 2480.
    StripProxy                        *proxy;             // 9 type, name 332, 992.
    StripCrop                         *crop;              // 10 type, name 329, 2488.
    StripTransform                    *transform;         // 11 type, name 330, 2489.
    StripColorBalance                 *color_balance;     // 12 type, name 331, 2490.
    ColorManagedColorspaceSettings    colorspace_settings;// 13 type, name 47, 159.
};

// 403 DNA structure 347, 3
struct BrightContrastModifierData
{
    SequenceModifierData    modifier;// 1 type, name 343, 724.
    float                   bright;  // 2 type, name 7, 281.
    float                   contrast;// 3 type, name 7, 282.
};

// 404 DNA structure 348, 1
struct SequencerMaskModifierData
{
    SequenceModifierData    modifier;// 1 type, name 343, 724.
};

// 405 DNA structure 378, 13
struct bSoundActuator
{
    short      flag;   // 1 type, name 2, 18.
    short      sndnr;  // 2 type, name 2, 2659.
    int        pad1;   // 3 type, name 4, 68.
    int        pad2;   // 4 type, name 4, 29.
    short      pad3[2];// 5 type, name 2, 2660.
    float      volume; // 6 type, name 7, 1165.
    float      pitch;  // 7 type, name 7, 2514.
    bSound     *sound; // 8 type, name 335, 2512.
    Sound3D    sound3D;// 9 type, name 377, 2661.
    short      type;   // 10 type, name 2, 16.
    short      pad4;   // 11 type, name 2, 1468.
    short      pad5;   // 12 type, name 2, 352.
    short      pad6[1];// 13 type, name 2, 2662.
};

// 406 DNA structure 335, 18
struct bSound
{
    ID            id;              // 1 type, name 22, 31.
    char          name[1024];      // 2 type, name 0, 34.
    PackedFile    *packedfile;     // 3 type, name 25, 37.
    void          *handle;         // 4 type, name 11, 2710.
    PackedFile    *newpackedfile;  // 5 type, name 25, 2711.
    Ipo           *ipo;            // 6 type, name 33, 80.
    float         volume;          // 7 type, name 7, 1165.
    float         attenuation;     // 8 type, name 7, 2712.
    float         pitch;           // 9 type, name 7, 2514.
    float         min_gain;        // 10 type, name 7, 2651.
    float         max_gain;        // 11 type, name 7, 2652.
    float         distance;        // 12 type, name 7, 984.
    int           flags;           // 13 type, name 4, 90.
    int           pad;             // 14 type, name 4, 66.
    void          *cache;          // 15 type, name 11, 130.
    void          *waveform;       // 16 type, name 11, 2713.
    void          *playback_handle;// 17 type, name 11, 2714.
    void          *mutex;          // 18 type, name 11, 2715.
};

// 407 DNA structure 398, 28
struct Bone
{
    Bone          *next;         // 1 type, name 398, 0.
    Bone          *prev;         // 2 type, name 398, 1.
    IDProperty    *prop;         // 3 type, name 21, 2265.
    Bone          *parent;       // 4 type, name 398, 36.
    ListBase      childbase;     // 5 type, name 14, 2719.
    char          name[64];      // 6 type, name 0, 19.
    float         roll;          // 7 type, name 7, 2720.
    float         head[3];       // 8 type, name 7, 2721.
    float         tail[3];       // 9 type, name 7, 2722.
    float         bone_mat[3][3];// 10 type, name 7, 2723.
    int           flag;          // 11 type, name 4, 18.
    float         arm_head[3];   // 12 type, name 7, 2724.
    float         arm_tail[3];   // 13 type, name 7, 2725.
    float         arm_mat[4][4]; // 14 type, name 7, 2726.
    float         arm_roll;      // 15 type, name 7, 2727.
    float         dist;          // 16 type, name 7, 341.
    float         weight;        // 17 type, name 7, 557.
    float         xwidth;        // 18 type, name 7, 2728.
    float         length;        // 19 type, name 7, 738.
    float         zwidth;        // 20 type, name 7, 2729.
    float         ease1;         // 21 type, name 7, 2730.
    float         ease2;         // 22 type, name 7, 2731.
    float         rad_head;      // 23 type, name 7, 2732.
    float         rad_tail;      // 24 type, name 7, 2733.
    float         size[3];       // 25 type, name 7, 174.
    int           layer;         // 26 type, name 4, 128.
    short         segments;      // 27 type, name 2, 2734.
    short         pad[1];        // 28 type, name 2, 230.
};

// 408 DNA structure 268, 8
struct bDopeSheet
{
    ID          *source;      // 1 type, name 22, 983.
    ListBase    chanbase;     // 2 type, name 14, 2795.
    Group       *filter_grp;  // 3 type, name 66, 2822.
    char        searchstr[64];// 4 type, name 0, 2823.
    int         filterflag;   // 5 type, name 4, 2824.
    int         flag;         // 6 type, name 4, 18.
    int         renameIndex;  // 7 type, name 4, 2825.
    int         pad;          // 8 type, name 4, 66.
};

// 409 DNA structure 407, 8
struct bActionChannel
{
    bActionChannel    *next;             // 1 type, name 407, 0.
    bActionChannel    *prev;             // 2 type, name 407, 1.
    bActionGroup      *grp;              // 3 type, name 405, 2828.
    Ipo               *ipo;              // 4 type, name 33, 80.
    ListBase          constraintChannels;// 5 type, name 14, 1001.
    int               flag;              // 6 type, name 4, 18.
    char              name[64];          // 7 type, name 0, 19.
    int               temp;              // 8 type, name 4, 2388.
};

// 410 DNA structure 411, 7
struct bPythonConstraint
{
    Text          *text;        // 1 type, name 38, 1924.
    IDProperty    *prop;        // 2 type, name 21, 2265.
    int           flag;         // 3 type, name 4, 18.
    int           tarnum;       // 4 type, name 4, 2840.
    ListBase      targets;      // 5 type, name 14, 2841.
    Object        *tar;         // 6 type, name 29, 2836.
    char          subtarget[64];// 7 type, name 0, 798.
};

// 411 DNA structure 440, 22
struct bActionStrip
{
    bActionStrip    *next;            // 1 type, name 440, 0.
    bActionStrip    *prev;            // 2 type, name 440, 1.
    short           flag;             // 3 type, name 2, 18.
    short           mode;             // 4 type, name 2, 334.
    short           stride_axis;      // 5 type, name 2, 2903.
    short           curmod;           // 6 type, name 2, 2904.
    Ipo             *ipo;             // 7 type, name 33, 80.
    bAction         *act;             // 8 type, name 186, 2643.
    Object          *object;          // 9 type, name 29, 166.
    float           start;            // 10 type, name 7, 737.
    float           end;              // 11 type, name 7, 2570.
    float           actstart;         // 12 type, name 7, 2905.
    float           actend;           // 13 type, name 7, 2906.
    float           actoffs;          // 14 type, name 7, 2907.
    float           stridelen;        // 15 type, name 7, 2908.
    float           repeat;           // 16 type, name 7, 784.
    float           scale;            // 17 type, name 7, 933.
    float           blendin;          // 18 type, name 7, 2645.
    float           blendout;         // 19 type, name 7, 2909.
    char            stridechannel[32];// 20 type, name 0, 2910.
    char            offs_bone[32];    // 21 type, name 0, 2911.
    ListBase        modifiers;        // 22 type, name 14, 1004.
};

// 412 DNA structure 442, 25
struct bNodeSocket
{
    bNodeSocket        *next;         // 1 type, name 442, 0.
    bNodeSocket        *prev;         // 2 type, name 442, 1.
    bNodeSocket        *new_sock;     // 3 type, name 442, 2918.
    IDProperty         *prop;         // 4 type, name 21, 2265.
    char               identifier[64];// 5 type, name 0, 2919.
    char               name[64];      // 6 type, name 0, 19.
    void               *storage;      // 7 type, name 11, 2920.
    short              type;          // 8 type, name 2, 16.
    short              flag;          // 9 type, name 2, 18.
    short              limit;         // 10 type, name 2, 2921.
    short              in_out;        // 11 type, name 2, 2922.
    void               *typeinfo;     // Note: using void* on undefined DNA type: bNodeSocketType
    char               idname[64];    // 13 type, name 0, 2419.
    float              locx;          // 14 type, name 7, 2924.
    float              locy;          // 15 type, name 7, 2925.
    void               *default_value;// 16 type, name 11, 2926.
    short              stack_index;   // 17 type, name 2, 2927.
    short              stack_type;    // 18 type, name 2, 2928.
    int                pad;           // 19 type, name 4, 66.
    void               *cache;        // 20 type, name 11, 130.
    int                own_index;     // 21 type, name 4, 2929.
    int                to_index;      // 22 type, name 4, 2930.
    bNodeSocket        *groupsock;    // 23 type, name 442, 2931.
    bNodeLink          *link;         // 24 type, name 444, 2932.
    bNodeStack         ns;            // 25 type, name 441, 2933.
};

// 413 DNA structure 448, 3
struct bNodeInstanceHashEntry
{
    bNodeInstanceKey    key;// 1 type, name 288, 2606.
    short               tag;// 2 type, name 2, 2958.
    short               pad;// 3 type, name 2, 66.
};

// 414 DNA structure 282, 18
struct Scopes
{
    int          ok;             // 1 type, name 4, 126.
    int          sample_full;    // 2 type, name 4, 3142.
    int          sample_lines;   // 3 type, name 4, 3143.
    float        accuracy;       // 4 type, name 7, 3144.
    int          wavefrm_mode;   // 5 type, name 4, 3145.
    float        wavefrm_alpha;  // 6 type, name 7, 3146.
    float        wavefrm_yfac;   // 7 type, name 7, 3147.
    int          wavefrm_height; // 8 type, name 4, 3148.
    float        vecscope_alpha; // 9 type, name 7, 3149.
    int          vecscope_height;// 10 type, name 4, 3150.
    float        minmax[3][2];   // 11 type, name 7, 3151.
    Histogram    hist;           // 12 type, name 283, 3152.
    float        *waveform_1;    // 13 type, name 7, 3153.
    float        *waveform_2;    // 14 type, name 7, 3154.
    float        *waveform_3;    // 15 type, name 7, 3155.
    float        *vecscope;      // 16 type, name 7, 3156.
    int          waveform_tot;   // 17 type, name 4, 3157.
    int          pad;            // 18 type, name 4, 66.
};

// 415 DNA structure 519, 2
struct PaintCurvePoint
{
    BezTriple    bez;     // 1 type, name 32, 3209.
    float        pressure;// 2 type, name 7, 3210.
};

// 416 DNA structure 517, 4
struct PaintCurve
{
    ID                 id;        // 1 type, name 22, 31.
    PaintCurvePoint    *points;   // 2 type, name 519, 2754.
    int                tot_points;// 3 type, name 4, 3211.
    int                add_index; // 4 type, name 4, 3212.
};

// 417 DNA structure 524, 5
struct BoidParticle
{
    Object      *ground;   // 1 type, name 29, 3225.
    BoidData    data;      // 2 type, name 525, 21.
    float       gravity[3];// 3 type, name 7, 1718.
    float       wander[3]; // 4 type, name 7, 3226.
    float       rt;        // 5 type, name 7, 56.
};

// 418 DNA structure 542, 6
struct bGPDframe
{
    bGPDframe    *next;   // 1 type, name 542, 0.
    bGPDframe    *prev;   // 2 type, name 542, 1.
    ListBase     strokes; // 3 type, name 14, 3422.
    int          framenum;// 4 type, name 4, 3423.
    short        flag;    // 5 type, name 2, 18.
    short        key_type;// 6 type, name 2, 3424.
};

// 419 DNA structure 543, 13
struct bGPDlayer
{
    bGPDlayer    *next;         // 1 type, name 543, 0.
    bGPDlayer    *prev;         // 2 type, name 543, 1.
    ListBase     frames;        // 3 type, name 14, 121.
    bGPDframe    *actframe;     // 4 type, name 542, 3425.
    short        flag;          // 5 type, name 2, 18.
    short        thickness;     // 6 type, name 2, 3420.
    short        gstep;         // 7 type, name 2, 3426.
    short        gstep_next;    // 8 type, name 2, 3427.
    float        gcolor_prev[3];// 9 type, name 7, 3428.
    float        gcolor_next[3];// 10 type, name 7, 3429.
    float        color[4];      // 11 type, name 7, 3430.
    float        fill[4];       // 12 type, name 7, 3431.
    char         info[128];     // 13 type, name 0, 3432.
};

// 420 DNA structure 544, 6
struct ReportList
{
    ListBase    list;        // 1 type, name 14, 3436.
    int         printlevel;  // 2 type, name 4, 3437.
    int         storelevel;  // 3 type, name 4, 3438.
    int         flag;        // 4 type, name 4, 18.
    int         pad;         // 5 type, name 4, 66.
    void        *reporttimer;// Note: using void* on undefined DNA type: wmTimer
};

// 421 DNA structure 547, 7
struct wmKeyConfig
{
    wmKeyConfig    *next;       // 1 type, name 547, 0.
    wmKeyConfig    *prev;       // 2 type, name 547, 1.
    char           idname[64];  // 3 type, name 0, 2419.
    char           basename[64];// 4 type, name 0, 3502.
    ListBase       keymaps;     // 5 type, name 14, 2313.
    int            actkeymap;   // 6 type, name 4, 3503.
    int            flag;        // 7 type, name 4, 18.
};

// 422 DNA structure 279, 14
struct wmOperator
{
    wmOperator        *next;       // 1 type, name 279, 0.
    wmOperator        *prev;       // 2 type, name 279, 1.
    char              idname[64];  // 3 type, name 0, 2419.
    IDProperty        *properties; // 4 type, name 21, 30.
    void              *type;       // Note: using void* on undefined DNA type: wmOperatorType
    void              *customdata; // 6 type, name 11, 3504.
    void              *py_instance;// 7 type, name 11, 3505.
    void              *ptr;        // Note: using void* on undefined DNA type: PointerRNA
    ReportList        *reports;    // 9 type, name 544, 3506.
    ListBase          macro;       // 10 type, name 14, 3507.
    wmOperator        *opm;        // 11 type, name 279, 3508.
    void              *layout;     // Note: using void* on undefined DNA type: uiLayout
    short             flag;        // 13 type, name 2, 18.
    short             pad[3];      // 14 type, name 2, 333.
};

// 423 DNA structure 564, 3
struct FMod_Limits
{
    rctf    rect;// 1 type, name 19, 3520.
    int     flag;// 2 type, name 4, 18.
    int     pad; // 3 type, name 4, 66.
};

// 424 DNA structure 568, 7
struct DriverVar
{
    DriverVar       *next;      // 1 type, name 568, 0.
    DriverVar       *prev;      // 2 type, name 568, 1.
    char            name[64];   // 3 type, name 0, 19.
    DriverTarget    targets[8]; // 4 type, name 567, 3528.
    short           num_targets;// 5 type, name 2, 3529.
    short           type;       // 6 type, name 2, 16.
    float           curval;     // 7 type, name 7, 60.
};

// 425 DNA structure 569, 7
struct ChannelDriver
{
    ListBase    variables;      // 1 type, name 14, 3530.
    char        expression[256];// 2 type, name 0, 3531.
    void        *expr_comp;     // 3 type, name 11, 3532.
    float       curval;         // 4 type, name 7, 60.
    float       influence;      // 5 type, name 7, 2695.
    int         type;           // 6 type, name 4, 16.
    int         flag;           // 7 type, name 4, 18.
};

// 426 DNA structure 571, 17
struct FCurve
{
    FCurve           *next;           // 1 type, name 571, 0.
    FCurve           *prev;           // 2 type, name 571, 1.
    bActionGroup     *grp;            // 3 type, name 405, 2828.
    ChannelDriver    *driver;         // 4 type, name 569, 61.
    ListBase         modifiers;       // 5 type, name 14, 1004.
    BezTriple        *bezt;           // 6 type, name 32, 49.
    FPoint           *fpt;            // 7 type, name 570, 3534.
    int              totvert;         // 8 type, name 4, 53.
    float            curval;          // 9 type, name 7, 60.
    short            flag;            // 10 type, name 2, 18.
    short            extend;          // 11 type, name 2, 268.
    int              array_index;     // 12 type, name 4, 3535.
    char             *rna_path;       // 13 type, name 0, 3524.
    int              color_mode;      // 14 type, name 4, 229.
    float            color[3];        // 15 type, name 7, 2936.
    float            prev_norm_factor;// 16 type, name 7, 3536.
    float            pad;             // 17 type, name 7, 66.
};

// 427 DNA structure 573, 4
struct AnimMapper
{
    AnimMapper    *next;   // 1 type, name 573, 0.
    AnimMapper    *prev;   // 2 type, name 573, 1.
    bAction       *target; // 3 type, name 186, 856.
    ListBase      mappings;// 4 type, name 14, 3539.
};

// 428 DNA structure 575, 6
struct NlaTrack
{
    NlaTrack    *next;   // 1 type, name 575, 0.
    NlaTrack    *prev;   // 2 type, name 575, 1.
    ListBase    strips;  // 3 type, name 14, 3540.
    int         flag;    // 4 type, name 4, 18.
    int         index;   // 5 type, name 4, 413.
    char        name[64];// 6 type, name 0, 19.
};

// 429 DNA structure 577, 10
struct KeyingSet
{
    KeyingSet    *next;           // 1 type, name 577, 0.
    KeyingSet    *prev;           // 2 type, name 577, 1.
    ListBase     paths;           // 3 type, name 14, 3550.
    char         idname[64];      // 4 type, name 0, 2419.
    char         name[64];        // 5 type, name 0, 19.
    char         description[240];// 6 type, name 0, 3551.
    char         typeinfo[64];    // 7 type, name 0, 3552.
    short        flag;            // 8 type, name 2, 18.
    short        keyingflag;      // 9 type, name 2, 3549.
    int          active_path;     // 10 type, name 4, 3553.
};

// 430 DNA structure 579, 2
struct IdAdtTemplate
{
    ID          id;  // 1 type, name 22, 31.
    AnimData    *adt;// 2 type, name 36, 75.
};

// 431 DNA structure 581, 6
struct BoidRuleGoalAvoid
{
    BoidRule    rule;       // 1 type, name 580, 3562.
    Object      *ob;        // 2 type, name 29, 44.
    int         options;    // 3 type, name 4, 3563.
    float       fear_factor;// 4 type, name 7, 3564.
    int         signal_id;  // 5 type, name 4, 3565.
    int         channels;   // 6 type, name 4, 2815.
};

// 432 DNA structure 582, 3
struct BoidRuleAvoidCollision
{
    BoidRule    rule;      // 1 type, name 580, 3562.
    int         options;   // 2 type, name 4, 3563.
    float       look_ahead;// 3 type, name 7, 3566.
};

// 433 DNA structure 583, 8
struct BoidRuleFollowLeader
{
    BoidRule    rule;      // 1 type, name 580, 3562.
    Object      *ob;       // 2 type, name 29, 44.
    float       loc[3];    // 3 type, name 7, 324.
    float       oloc[3];   // 4 type, name 7, 3567.
    float       cfra;      // 5 type, name 7, 823.
    float       distance;  // 6 type, name 7, 984.
    int         options;   // 7 type, name 4, 3563.
    int         queue_size;// 8 type, name 4, 3568.
};

// 434 DNA structure 584, 5
struct BoidRuleAverageSpeed
{
    BoidRule    rule;  // 1 type, name 580, 3562.
    float       wander;// 2 type, name 7, 3569.
    float       level; // 3 type, name 7, 701.
    float       speed; // 4 type, name 7, 790.
    float       rt;    // 5 type, name 7, 56.
};

// 435 DNA structure 585, 3
struct BoidRuleFight
{
    BoidRule    rule;         // 1 type, name 580, 3562.
    float       distance;     // 2 type, name 7, 984.
    float       flee_distance;// 3 type, name 7, 3570.
};

// 436 DNA structure 533, 23
struct BoidSettings
{
    int         options;            // 1 type, name 4, 3563.
    int         last_state_id;      // 2 type, name 4, 3578.
    float       landing_smoothness; // 3 type, name 7, 3579.
    float       height;             // 4 type, name 7, 788.
    float       banking;            // 5 type, name 7, 3580.
    float       pitch;              // 6 type, name 7, 2514.
    float       health;             // 7 type, name 7, 3571.
    float       aggression;         // 8 type, name 7, 3581.
    float       strength;           // 9 type, name 7, 734.
    float       accuracy;           // 10 type, name 7, 3144.
    float       range;              // 11 type, name 7, 1298.
    float       air_min_speed;      // 12 type, name 7, 3582.
    float       air_max_speed;      // 13 type, name 7, 3583.
    float       air_max_acc;        // 14 type, name 7, 3584.
    float       air_max_ave;        // 15 type, name 7, 3585.
    float       air_personal_space; // 16 type, name 7, 3586.
    float       land_jump_speed;    // 17 type, name 7, 3587.
    float       land_max_speed;     // 18 type, name 7, 3588.
    float       land_max_acc;       // 19 type, name 7, 3589.
    float       land_max_ave;       // 20 type, name 7, 3590.
    float       land_personal_space;// 21 type, name 7, 3591.
    float       land_stick_force;   // 22 type, name 7, 3592.
    ListBase    states;             // 23 type, name 14, 3593.
};

// 437 DNA structure 125, 62
struct SmokeDomainSettings
{
    SmokeModifierData    *smd;                // 1 type, name 124, 3594.
    void                 *fluid;              // Note: using void* on undefined DNA type: FLUID_3D
    void                 *fluid_mutex;        // 3 type, name 11, 3595.
    Group                *fluid_group;        // 4 type, name 66, 3596.
    Group                *eff_group;          // 5 type, name 66, 3344.
    Group                *coll_group;         // 6 type, name 66, 3597.
    void                 *wt;                 // Note: using void* on undefined DNA type: WTURBULENCE
    void                 *tex;                // Note: using void* on undefined DNA type: GPUTexture
    void                 *tex_wt;             // Note: using void* on undefined DNA type: GPUTexture
    void                 *tex_shadow;         // Note: using void* on undefined DNA type: GPUTexture
    void                 *tex_flame;          // Note: using void* on undefined DNA type: GPUTexture
    float                *shadow;             // 12 type, name 7, 3602.
    float                p0[3];               // 13 type, name 7, 3603.
    float                p1[3];               // 14 type, name 7, 3604.
    float                dp0[3];              // 15 type, name 7, 3605.
    float                cell_size[3];        // 16 type, name 7, 3606.
    float                global_size[3];      // 17 type, name 7, 3607.
    float                prev_loc[3];         // 18 type, name 7, 3608.
    int                  shift[3];            // 19 type, name 4, 3609.
    float                shift_f[3];          // 20 type, name 7, 3610.
    float                obj_shift_f[3];      // 21 type, name 7, 3611.
    float                imat[4][4];          // 22 type, name 7, 234.
    float                obmat[4][4];         // 23 type, name 7, 1018.
    int                  base_res[3];         // 24 type, name 4, 3612.
    int                  res_min[3];          // 25 type, name 4, 3613.
    int                  res_max[3];          // 26 type, name 4, 3614.
    int                  res[3];              // 27 type, name 4, 3615.
    int                  total_cells;         // 28 type, name 4, 3616.
    float                dx;                  // 29 type, name 7, 3617.
    float                scale;               // 30 type, name 7, 933.
    int                  adapt_margin;        // 31 type, name 4, 3618.
    int                  adapt_res;           // 32 type, name 4, 3619.
    float                adapt_threshold;     // 33 type, name 7, 3620.
    float                alpha;               // 34 type, name 7, 429.
    float                beta;                // 35 type, name 7, 3621.
    int                  amplify;             // 36 type, name 4, 3622.
    int                  maxres;              // 37 type, name 4, 3623.
    int                  flags;               // 38 type, name 4, 90.
    int                  viewsettings;        // 39 type, name 4, 3624.
    short                noise;               // 40 type, name 2, 3625.
    short                diss_percent;        // 41 type, name 2, 3626.
    int                  diss_speed;          // 42 type, name 4, 3627.
    float                strength;            // 43 type, name 7, 734.
    int                  res_wt[3];           // 44 type, name 4, 3628.
    float                dx_wt;               // 45 type, name 7, 3629.
    int                  cache_comp;          // 46 type, name 4, 3630.
    int                  cache_high_comp;     // 47 type, name 4, 3631.
    PointCache           *point_cache[2];     // 48 type, name 141, 3632.
    ListBase             ptcaches[2];         // 49 type, name 14, 3633.
    EffectorWeights      *effector_weights;   // 50 type, name 200, 1235.
    int                  border_collisions;   // 51 type, name 4, 3634.
    float                time_scale;          // 52 type, name 7, 3635.
    float                vorticity;           // 53 type, name 7, 3636.
    int                  active_fields;       // 54 type, name 4, 3637.
    float                active_color[3];     // 55 type, name 7, 3638.
    int                  highres_sampling;    // 56 type, name 4, 3639.
    float                burning_rate;        // 57 type, name 7, 3640.
    float                flame_smoke;         // 58 type, name 7, 3641.
    float                flame_vorticity;     // 59 type, name 7, 3642.
    float                flame_ignition;      // 60 type, name 7, 3643.
    float                flame_max_temp;      // 61 type, name 7, 3644.
    float                flame_smoke_color[3];// 62 type, name 7, 3645.
};

// 438 DNA structure 589, 15
struct Speaker
{
    ID          id;                // 1 type, name 22, 31.
    AnimData    *adt;              // 2 type, name 36, 75.
    bSound      *sound;            // 3 type, name 335, 2512.
    float       volume_max;        // 4 type, name 7, 3659.
    float       volume_min;        // 5 type, name 7, 3660.
    float       distance_max;      // 6 type, name 7, 3661.
    float       distance_reference;// 7 type, name 7, 3662.
    float       attenuation;       // 8 type, name 7, 2712.
    float       cone_angle_outer;  // 9 type, name 7, 3663.
    float       cone_angle_inner;  // 10 type, name 7, 3664.
    float       cone_volume_outer; // 11 type, name 7, 3665.
    float       volume;            // 12 type, name 7, 1165.
    float       pitch;             // 13 type, name 7, 2514.
    short       flag;              // 14 type, name 2, 18.
    short       pad1[3];           // 15 type, name 2, 2431.
};

// 439 DNA structure 295, 15
struct MovieClipScopes
{
    short                  ok;                  // 1 type, name 2, 126.
    short                  use_track_mask;      // 2 type, name 2, 3674.
    int                    track_preview_height;// 3 type, name 4, 3675.
    int                    frame_width;         // 4 type, name 4, 3676.
    int                    frame_height;        // 5 type, name 4, 3677.
    MovieTrackingMarker    undist_marker;       // 6 type, name 593, 3678.
    void                   *track_search;       // Note: using void* on undefined DNA type: ImBuf
    void                   *track_preview;      // Note: using void* on undefined DNA type: ImBuf
    float                  track_pos[2];        // 9 type, name 7, 3681.
    short                  track_disabled;      // 10 type, name 2, 3682.
    short                  track_locked;        // 11 type, name 2, 3683.
    int                    framenr;             // 12 type, name 4, 120.
    MovieTrackingTrack     *track;              // 13 type, name 594, 991.
    MovieTrackingMarker    *marker;             // 14 type, name 593, 3684.
    float                  slide_scale[2];      // 15 type, name 7, 3685.
};

// 440 DNA structure 607, 50
struct DynamicPaintSurface
{
    DynamicPaintSurface           *next;                  // 1 type, name 607, 0.
    DynamicPaintSurface           *prev;                  // 2 type, name 607, 1.
    DynamicPaintCanvasSettings    *canvas;                // 3 type, name 171, 930.
    void                          *data;                  // Note: using void* on undefined DNA type: PaintSurfaceData
    Group                         *brush_group;           // 5 type, name 66, 3769.
    EffectorWeights               *effector_weights;      // 6 type, name 200, 1235.
    PointCache                    *pointcache;            // 7 type, name 141, 1234.
    ListBase                      ptcaches;               // 8 type, name 14, 808.
    int                           current_frame;          // 9 type, name 4, 3770.
    char                          name[64];               // 10 type, name 0, 19.
    short                         format;                 // 11 type, name 2, 3027.
    short                         type;                   // 12 type, name 2, 16.
    short                         disp_type;              // 13 type, name 2, 3771.
    short                         image_fileformat;       // 14 type, name 2, 3772.
    short                         effect_ui;              // 15 type, name 2, 3773.
    short                         preview_id;             // 16 type, name 2, 3774.
    short                         init_color_type;        // 17 type, name 2, 3775.
    short                         pad_s;                  // 18 type, name 2, 3776.
    int                           flags;                  // 19 type, name 4, 90.
    int                           effect;                 // 20 type, name 4, 1002.
    int                           image_resolution;       // 21 type, name 4, 3777.
    int                           substeps;               // 22 type, name 4, 3778.
    int                           start_frame;            // 23 type, name 4, 2755.
    int                           end_frame;              // 24 type, name 4, 2756.
    int                           pad;                    // 25 type, name 4, 66.
    float                         init_color[4];          // 26 type, name 7, 3779.
    Tex                           *init_texture;          // 27 type, name 49, 3780.
    char                          init_layername[64];     // 28 type, name 0, 3781.
    int                           dry_speed;              // 29 type, name 4, 3782.
    int                           diss_speed;             // 30 type, name 4, 3627.
    float                         color_dry_threshold;    // 31 type, name 7, 3783.
    float                         depth_clamp;            // 32 type, name 7, 3784.
    float                         disp_factor;            // 33 type, name 7, 3785.
    float                         spread_speed;           // 34 type, name 7, 3786.
    float                         color_spread_speed;     // 35 type, name 7, 3787.
    float                         shrink_speed;           // 36 type, name 7, 3788.
    float                         drip_vel;               // 37 type, name 7, 3789.
    float                         drip_acc;               // 38 type, name 7, 3790.
    float                         influence_scale;        // 39 type, name 7, 3791.
    float                         radius_scale;           // 40 type, name 7, 3792.
    float                         wave_damping;           // 41 type, name 7, 3793.
    float                         wave_speed;             // 42 type, name 7, 3794.
    float                         wave_timescale;         // 43 type, name 7, 3795.
    float                         wave_spring;            // 44 type, name 7, 3796.
    float                         wave_smoothness;        // 45 type, name 7, 3797.
    int                           pad2;                   // 46 type, name 4, 29.
    char                          uvlayer_name[64];       // 47 type, name 0, 727.
    char                          image_output_path[1024];// 48 type, name 0, 3798.
    char                          output_name[64];        // 49 type, name 0, 3799.
    char                          output_name2[64];       // 50 type, name 0, 3800.
};

// 441 DNA structure 171, 7
struct DynamicPaintCanvasSettings
{
    DynamicPaintModifierData    *pmd;      // 1 type, name 170, 3801.
    void                        *dm;       // Note: using void* on undefined DNA type: DerivedMesh
    ListBase                    surfaces;  // 3 type, name 14, 3802.
    short                       active_sur;// 4 type, name 2, 3803.
    short                       flags;     // 5 type, name 2, 90.
    int                         pad;       // 6 type, name 4, 66.
    char                        error[64]; // 7 type, name 0, 3804.
};

// 442 DNA structure 612, 9
struct MaskSpline
{
    MaskSpline         *next;         // 1 type, name 612, 0.
    MaskSpline         *prev;         // 2 type, name 612, 1.
    short              flag;          // 3 type, name 2, 18.
    char               offset_mode;   // 4 type, name 0, 3830.
    char               weight_interp; // 5 type, name 0, 3831.
    int                tot_point;     // 6 type, name 4, 3832.
    MaskSplinePoint    *points;       // 7 type, name 611, 2754.
    MaskParent         parent;        // 8 type, name 609, 3231.
    MaskSplinePoint    *points_deform;// 9 type, name 611, 3833.
};

// 443 DNA structure 250, 14
struct RigidBodyWorld
{
    EffectorWeights    *effector_weights;    // 1 type, name 200, 1235.
    Group              *group;               // 2 type, name 66, 503.
    Object             **objects;            // 3 type, name 29, 3840.
    Group              *constraints;         // 4 type, name 66, 3841.
    int                pad;                  // 5 type, name 4, 66.
    float              ltime;                // 6 type, name 7, 3842.
    PointCache         *pointcache;          // 7 type, name 141, 1234.
    ListBase           ptcaches;             // 8 type, name 14, 808.
    int                numbodies;            // 9 type, name 4, 3843.
    short              steps_per_second;     // 10 type, name 2, 3844.
    short              num_solver_iterations;// 11 type, name 2, 3845.
    int                flag;                 // 12 type, name 4, 18.
    float              time_scale;           // 13 type, name 7, 3635.
    void               *physics_world;       // 14 type, name 11, 3846.
};

// 444 DNA structure 619, 2
struct LineStyleColorModifier_AlongStroke
{
    LineStyleModifier    modifier;   // 1 type, name 618, 724.
    ColorBand            *color_ramp;// 2 type, name 51, 3898.
};

// 445 DNA structure 620, 4
struct LineStyleAlphaModifier_AlongStroke
{
    LineStyleModifier    modifier;// 1 type, name 618, 724.
    CurveMapping         *curve;  // 2 type, name 55, 3121.
    int                  flags;   // 3 type, name 4, 90.
    int                  pad;     // 4 type, name 4, 66.
};

// 446 DNA structure 621, 6
struct LineStyleThicknessModifier_AlongStroke
{
    LineStyleModifier    modifier; // 1 type, name 618, 724.
    CurveMapping         *curve;   // 2 type, name 55, 3121.
    int                  flags;    // 3 type, name 4, 90.
    float                value_min;// 4 type, name 7, 3899.
    float                value_max;// 5 type, name 7, 3900.
    int                  pad;      // 6 type, name 4, 66.
};

// 447 DNA structure 622, 4
struct LineStyleColorModifier_DistanceFromCamera
{
    LineStyleModifier    modifier;   // 1 type, name 618, 724.
    ColorBand            *color_ramp;// 2 type, name 51, 3898.
    float                range_min;  // 3 type, name 7, 3901.
    float                range_max;  // 4 type, name 7, 3902.
};

// 448 DNA structure 623, 6
struct LineStyleAlphaModifier_DistanceFromCamera
{
    LineStyleModifier    modifier; // 1 type, name 618, 724.
    CurveMapping         *curve;   // 2 type, name 55, 3121.
    int                  flags;    // 3 type, name 4, 90.
    float                range_min;// 4 type, name 7, 3901.
    float                range_max;// 5 type, name 7, 3902.
    int                  pad;      // 6 type, name 4, 66.
};

// 449 DNA structure 624, 8
struct LineStyleThicknessModifier_DistanceFromCamera
{
    LineStyleModifier    modifier; // 1 type, name 618, 724.
    CurveMapping         *curve;   // 2 type, name 55, 3121.
    int                  flags;    // 3 type, name 4, 90.
    float                range_min;// 4 type, name 7, 3901.
    float                range_max;// 5 type, name 7, 3902.
    float                value_min;// 6 type, name 7, 3899.
    float                value_max;// 7 type, name 7, 3900.
    int                  pad;      // 8 type, name 4, 66.
};

// 450 DNA structure 625, 5
struct LineStyleColorModifier_DistanceFromObject
{
    LineStyleModifier    modifier;   // 1 type, name 618, 724.
    Object               *target;    // 2 type, name 29, 856.
    ColorBand            *color_ramp;// 3 type, name 51, 3898.
    float                range_min;  // 4 type, name 7, 3901.
    float                range_max;  // 5 type, name 7, 3902.
};

// 451 DNA structure 626, 7
struct LineStyleAlphaModifier_DistanceFromObject
{
    LineStyleModifier    modifier; // 1 type, name 618, 724.
    Object               *target;  // 2 type, name 29, 856.
    CurveMapping         *curve;   // 3 type, name 55, 3121.
    int                  flags;    // 4 type, name 4, 90.
    float                range_min;// 5 type, name 7, 3901.
    float                range_max;// 6 type, name 7, 3902.
    int                  pad;      // 7 type, name 4, 66.
};

// 452 DNA structure 627, 9
struct LineStyleThicknessModifier_DistanceFromObject
{
    LineStyleModifier    modifier; // 1 type, name 618, 724.
    Object               *target;  // 2 type, name 29, 856.
    CurveMapping         *curve;   // 3 type, name 55, 3121.
    int                  flags;    // 4 type, name 4, 90.
    float                range_min;// 5 type, name 7, 3901.
    float                range_max;// 6 type, name 7, 3902.
    float                value_min;// 7 type, name 7, 3899.
    float                value_max;// 8 type, name 7, 3900.
    int                  pad;      // 9 type, name 4, 66.
};

// 453 DNA structure 628, 4
struct LineStyleColorModifier_Material
{
    LineStyleModifier    modifier;   // 1 type, name 618, 724.
    ColorBand            *color_ramp;// 2 type, name 51, 3898.
    int                  flags;      // 3 type, name 4, 90.
    int                  mat_attr;   // 4 type, name 4, 3903.
};

// 454 DNA structure 629, 4
struct LineStyleAlphaModifier_Material
{
    LineStyleModifier    modifier;// 1 type, name 618, 724.
    CurveMapping         *curve;  // 2 type, name 55, 3121.
    int                  flags;   // 3 type, name 4, 90.
    int                  mat_attr;// 4 type, name 4, 3903.
};

// 455 DNA structure 630, 6
struct LineStyleThicknessModifier_Material
{
    LineStyleModifier    modifier; // 1 type, name 618, 724.
    CurveMapping         *curve;   // 2 type, name 55, 3121.
    int                  flags;    // 3 type, name 4, 90.
    float                value_min;// 4 type, name 7, 3899.
    float                value_max;// 5 type, name 7, 3900.
    int                  mat_attr; // 6 type, name 4, 3903.
};

// 456 DNA structure 631, 3
struct LineStyleGeometryModifier_Sampling
{
    LineStyleModifier    modifier;// 1 type, name 618, 724.
    float                sampling;// 2 type, name 7, 3904.
    int                  pad;     // 3 type, name 4, 66.
};

// 457 DNA structure 632, 3
struct LineStyleGeometryModifier_BezierCurve
{
    LineStyleModifier    modifier;// 1 type, name 618, 724.
    float                error;   // 2 type, name 7, 3686.
    int                  pad;     // 3 type, name 4, 66.
};

// 458 DNA structure 633, 5
struct LineStyleGeometryModifier_SinusDisplacement
{
    LineStyleModifier    modifier;  // 1 type, name 618, 724.
    float                wavelength;// 2 type, name 7, 3905.
    float                amplitude; // 3 type, name 7, 566.
    float                phase;     // 4 type, name 7, 3521.
    int                  pad;       // 5 type, name 4, 66.
};

// 459 DNA structure 634, 5
struct LineStyleGeometryModifier_SpatialNoise
{
    LineStyleModifier    modifier; // 1 type, name 618, 724.
    float                amplitude;// 2 type, name 7, 566.
    float                scale;    // 3 type, name 7, 933.
    int                  octaves;  // 4 type, name 4, 3906.
    int                  flags;    // 5 type, name 4, 90.
};

// 460 DNA structure 635, 7
struct LineStyleGeometryModifier_PerlinNoise1D
{
    LineStyleModifier    modifier; // 1 type, name 618, 724.
    float                frequency;// 2 type, name 7, 3907.
    float                amplitude;// 3 type, name 7, 566.
    float                angle;    // 4 type, name 7, 781.
    int                  octaves;  // 5 type, name 4, 3906.
    int                  seed;     // 6 type, name 4, 740.
    int                  pad1;     // 7 type, name 4, 68.
};

// 461 DNA structure 636, 7
struct LineStyleGeometryModifier_PerlinNoise2D
{
    LineStyleModifier    modifier; // 1 type, name 618, 724.
    float                frequency;// 2 type, name 7, 3907.
    float                amplitude;// 3 type, name 7, 566.
    float                angle;    // 4 type, name 7, 781.
    int                  octaves;  // 5 type, name 4, 3906.
    int                  seed;     // 6 type, name 4, 740.
    int                  pad1;     // 7 type, name 4, 68.
};

// 462 DNA structure 637, 3
struct LineStyleGeometryModifier_BackboneStretcher
{
    LineStyleModifier    modifier;       // 1 type, name 618, 724.
    float                backbone_length;// 2 type, name 7, 3908.
    int                  pad;            // 3 type, name 4, 66.
};

// 463 DNA structure 638, 3
struct LineStyleGeometryModifier_TipRemover
{
    LineStyleModifier    modifier;  // 1 type, name 618, 724.
    float                tip_length;// 2 type, name 7, 3909.
    int                  pad;       // 3 type, name 4, 66.
};

// 464 DNA structure 639, 3
struct LineStyleGeometryModifier_Polygonalization
{
    LineStyleModifier    modifier;// 1 type, name 618, 724.
    float                error;   // 2 type, name 7, 3686.
    int                  pad;     // 3 type, name 4, 66.
};

// 465 DNA structure 640, 3
struct LineStyleGeometryModifier_GuidingLines
{
    LineStyleModifier    modifier;// 1 type, name 618, 724.
    float                offset;  // 2 type, name 7, 122.
    int                  pad;     // 3 type, name 4, 66.
};

// 466 DNA structure 641, 7
struct LineStyleGeometryModifier_Blueprint
{
    LineStyleModifier    modifier;       // 1 type, name 618, 724.
    int                  flags;          // 2 type, name 4, 90.
    int                  rounds;         // 3 type, name 4, 3910.
    float                backbone_length;// 4 type, name 7, 3908.
    int                  random_radius;  // 5 type, name 4, 3911.
    int                  random_center;  // 6 type, name 4, 3912.
    int                  random_backbone;// 7 type, name 4, 3913.
};

// 467 DNA structure 642, 5
struct LineStyleGeometryModifier_2DOffset
{
    LineStyleModifier    modifier;// 1 type, name 618, 724.
    float                start;   // 2 type, name 7, 737.
    float                end;     // 3 type, name 7, 2570.
    float                x;       // 4 type, name 7, 5.
    float                y;       // 5 type, name 7, 6.
};

// 468 DNA structure 643, 9
struct LineStyleGeometryModifier_2DTransform
{
    LineStyleModifier    modifier;// 1 type, name 618, 724.
    int                  pivot;   // 2 type, name 4, 3914.
    float                scale_x; // 3 type, name 7, 3915.
    float                scale_y; // 4 type, name 7, 3916.
    float                angle;   // 5 type, name 7, 781.
    float                pivot_u; // 6 type, name 7, 3917.
    float                pivot_x; // 7 type, name 7, 3918.
    float                pivot_y; // 8 type, name 7, 3919.
    int                  pad;     // 9 type, name 4, 66.
};

// 469 DNA structure 644, 5
struct LineStyleThicknessModifier_Calligraphy
{
    LineStyleModifier    modifier;     // 1 type, name 618, 724.
    float                min_thickness;// 2 type, name 7, 3920.
    float                max_thickness;// 3 type, name 7, 3921.
    float                orientation;  // 4 type, name 7, 3922.
    int                  pad;          // 5 type, name 4, 66.
};

// 470 DNA structure 21, 10
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

// 471 DNA structure 30, 21
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

// 472 DNA structure 35, 15
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
    short       type;       // 10 type, name 2, 16.
    short       totkey;     // 11 type, name 2, 82.
    short       slurph;     // 12 type, name 2, 83.
    short       flag;       // 13 type, name 2, 18.
    float       ctime;      // 14 type, name 7, 84.
    int         uidgen;     // 15 type, name 4, 85.
};

// 473 DNA structure 38, 14
struct Text
{
    ID          id;       // 1 type, name 22, 31.
    char        *name;    // 2 type, name 0, 89.
    int         flags;    // 3 type, name 4, 90.
    int         nlines;   // 4 type, name 4, 91.
    ListBase    lines;    // 5 type, name 14, 92.
    TextLine    *curl;    // 6 type, name 37, 93.
    TextLine    *sell;    // 7 type, name 37, 94.
    int         curc;     // 8 type, name 4, 95.
    int         selc;     // 9 type, name 4, 96.
    char        *undo_buf;// 10 type, name 0, 97.
    int         undo_pos; // 11 type, name 4, 98.
    int         undo_len; // 12 type, name 4, 99.
    void        *compiled;// 13 type, name 11, 100.
    double      mtime;    // 14 type, name 8, 101.
};

// 474 DNA structure 49, 63
struct Tex
{
    ID              id;           // 1 type, name 22, 31.
    AnimData        *adt;         // 2 type, name 36, 75.
    float           noisesize;    // 3 type, name 7, 279.
    float           turbul;       // 4 type, name 7, 280.
    float           bright;       // 5 type, name 7, 281.
    float           contrast;     // 6 type, name 7, 282.
    float           saturation;   // 7 type, name 7, 283.
    float           rfac;         // 8 type, name 7, 284.
    float           gfac;         // 9 type, name 7, 285.
    float           bfac;         // 10 type, name 7, 286.
    float           filtersize;   // 11 type, name 7, 287.
    float           pad2;         // 12 type, name 7, 29.
    float           mg_H;         // 13 type, name 7, 288.
    float           mg_lacunarity;// 14 type, name 7, 289.
    float           mg_octaves;   // 15 type, name 7, 290.
    float           mg_offset;    // 16 type, name 7, 291.
    float           mg_gain;      // 17 type, name 7, 292.
    float           dist_amount;  // 18 type, name 7, 293.
    float           ns_outscale;  // 19 type, name 7, 294.
    float           vn_w1;        // 20 type, name 7, 295.
    float           vn_w2;        // 21 type, name 7, 296.
    float           vn_w3;        // 22 type, name 7, 297.
    float           vn_w4;        // 23 type, name 7, 298.
    float           vn_mexp;      // 24 type, name 7, 299.
    short           vn_distm;     // 25 type, name 2, 300.
    short           vn_coltype;   // 26 type, name 2, 301.
    short           noisedepth;   // 27 type, name 2, 302.
    short           noisetype;    // 28 type, name 2, 303.
    short           noisebasis;   // 29 type, name 2, 304.
    short           noisebasis2;  // 30 type, name 2, 305.
    short           imaflag;      // 31 type, name 2, 306.
    short           flag;         // 32 type, name 2, 18.
    short           type;         // 33 type, name 2, 16.
    short           stype;        // 34 type, name 2, 236.
    float           cropxmin;     // 35 type, name 7, 307.
    float           cropymin;     // 36 type, name 7, 308.
    float           cropxmax;     // 37 type, name 7, 309.
    float           cropymax;     // 38 type, name 7, 310.
    int             texfilter;    // 39 type, name 4, 311.
    int             afmax;        // 40 type, name 4, 312.
    short           xrepeat;      // 41 type, name 2, 313.
    short           yrepeat;      // 42 type, name 2, 314.
    short           extend;       // 43 type, name 2, 268.
    short           fie_ima;      // 44 type, name 2, 124.
    int             len;          // 45 type, name 4, 22.
    int             frames;       // 46 type, name 4, 121.
    int             offset;       // 47 type, name 4, 122.
    int             sfra;         // 48 type, name 4, 123.
    float           checkerdist;  // 49 type, name 7, 315.
    float           nabla;        // 50 type, name 7, 316.
    float           pad1;         // 51 type, name 7, 68.
    ImageUser       iuser;        // 52 type, name 40, 317.
    bNodeTree       *nodetree;    // 53 type, name 58, 318.
    Ipo             *ipo;         // 54 type, name 33, 80.
    Image           *ima;         // 55 type, name 43, 232.
    ColorBand       *coba;        // 56 type, name 51, 263.
    EnvMap          *env;         // 57 type, name 52, 319.
    PreviewImage    *preview;     // 58 type, name 26, 147.
    PointDensity    *pd;          // 59 type, name 54, 320.
    VoxelData       *vd;          // 60 type, name 56, 321.
    OceanTex        *ot;          // 61 type, name 57, 322.
    char            use_nodes;    // 62 type, name 0, 323.
    char            pad[7];       // 63 type, name 0, 118.
};

// 475 DNA structure 60, 9
struct ColorMapping
{
    ColorBand    coba;          // 1 type, name 51, 329.
    float        bright;        // 2 type, name 7, 281.
    float        contrast;      // 3 type, name 7, 282.
    float        saturation;    // 4 type, name 7, 283.
    int          flag;          // 5 type, name 4, 18.
    float        blend_color[3];// 6 type, name 7, 330.
    float        blend_factor;  // 7 type, name 7, 331.
    int          blend_type;    // 8 type, name 4, 332.
    int          pad[3];        // 9 type, name 4, 333.
};

// 476 DNA structure 137, 7
struct ClothModifierData
{
    ModifierData         modifier;    // 1 type, name 113, 724.
    Scene                *scene;      // 2 type, name 41, 119.
    void                 *clothObject;// Note: using void* on undefined DNA type: Cloth
    ClothSimSettings     *sim_parms;  // 4 type, name 139, 805.
    ClothCollSettings    *coll_parms; // 5 type, name 140, 806.
    PointCache           *point_cache;// 6 type, name 141, 807.
    ListBase             ptcaches;    // 7 type, name 14, 808.
};

// 477 DNA structure 209, 77
struct World
{
    ID              id;                  // 1 type, name 22, 31.
    AnimData        *adt;                // 2 type, name 36, 75.
    short           colormodel;          // 3 type, name 2, 177.
    short           totex;               // 4 type, name 2, 335.
    short           texact;              // 5 type, name 2, 370.
    short           mistype;             // 6 type, name 2, 1289.
    float           horr;                // 7 type, name 7, 1290.
    float           horg;                // 8 type, name 7, 1291.
    float           horb;                // 9 type, name 7, 1292.
    float           zenr;                // 10 type, name 7, 1293.
    float           zeng;                // 11 type, name 7, 1294.
    float           zenb;                // 12 type, name 7, 1295.
    float           ambr;                // 13 type, name 7, 421.
    float           ambg;                // 14 type, name 7, 423.
    float           ambb;                // 15 type, name 7, 422.
    float           exposure;            // 16 type, name 7, 1296.
    float           exp;                 // 17 type, name 7, 1297.
    float           range;               // 18 type, name 7, 1298.
    float           linfac;              // 19 type, name 7, 1299.
    float           logfac;              // 20 type, name 7, 1300.
    float           gravity;             // 21 type, name 7, 1301.
    float           activityBoxRadius;   // 22 type, name 7, 1302.
    short           skytype;             // 23 type, name 2, 1303.
    short           mode;                // 24 type, name 2, 334.
    short           occlusionRes;        // 25 type, name 2, 1304.
    short           physicsEngine;       // 26 type, name 2, 1305.
    short           ticrate;             // 27 type, name 2, 1306.
    short           maxlogicstep;        // 28 type, name 2, 1307.
    short           physubstep;          // 29 type, name 2, 1308.
    short           maxphystep;          // 30 type, name 2, 1309.
    float           misi;                // 31 type, name 7, 1310.
    float           miststa;             // 32 type, name 7, 1311.
    float           mistdist;            // 33 type, name 7, 1312.
    float           misthi;              // 34 type, name 7, 1313.
    float           starr;               // 35 type, name 7, 1314.
    float           starg;               // 36 type, name 7, 1315.
    float           starb;               // 37 type, name 7, 1316.
    float           stark;               // 38 type, name 7, 1317.
    float           starsize;            // 39 type, name 7, 1318.
    float           starmindist;         // 40 type, name 7, 1319.
    float           stardist;            // 41 type, name 7, 1320.
    float           starcolnoise;        // 42 type, name 7, 1321.
    short           dofsta;              // 43 type, name 2, 1322.
    short           dofend;              // 44 type, name 2, 1323.
    short           dofmin;              // 45 type, name 2, 1324.
    short           dofmax;              // 46 type, name 2, 1325.
    float           aodist;              // 47 type, name 7, 1326.
    float           aodistfac;           // 48 type, name 7, 1327.
    float           aoenergy;            // 49 type, name 7, 1328.
    float           aobias;              // 50 type, name 7, 1329.
    short           aomode;              // 51 type, name 2, 1330.
    short           aosamp;              // 52 type, name 2, 1331.
    short           aomix;               // 53 type, name 2, 1332.
    short           aocolor;             // 54 type, name 2, 1333.
    float           ao_adapt_thresh;     // 55 type, name 7, 1334.
    float           ao_adapt_speed_fac;  // 56 type, name 7, 1335.
    float           ao_approx_error;     // 57 type, name 7, 1336.
    float           ao_approx_correction;// 58 type, name 7, 1337.
    float           ao_indirect_energy;  // 59 type, name 7, 1338.
    float           ao_env_energy;       // 60 type, name 7, 1339.
    float           ao_pad2;             // 61 type, name 7, 1340.
    short           ao_indirect_bounces; // 62 type, name 2, 1341.
    short           ao_pad;              // 63 type, name 2, 1342.
    short           ao_samp_method;      // 64 type, name 2, 1343.
    short           ao_gather_method;    // 65 type, name 2, 1344.
    short           ao_approx_passes;    // 66 type, name 2, 1345.
    short           flag;                // 67 type, name 2, 18.
    float           *aosphere;           // 68 type, name 7, 1346.
    float           *aotables;           // 69 type, name 7, 1347.
    Ipo             *ipo;                // 70 type, name 33, 80.
    MTex            *mtex[18];           // 71 type, name 48, 389.
    short           pr_texture;          // 72 type, name 2, 390.
    short           use_nodes;           // 73 type, name 2, 323.
    short           pad[2];              // 74 type, name 2, 573.
    PreviewImage    *preview;            // 75 type, name 26, 147.
    bNodeTree       *nodetree;           // 76 type, name 58, 318.
    ListBase        gpumaterial;         // 77 type, name 14, 531.
};

// 478 DNA structure 218, 16
struct ImageFormatData
{
    char                           imtype;          // 1 type, name 0, 1411.
    char                           depth;           // 2 type, name 0, 240.
    char                           planes;          // 3 type, name 0, 1412.
    char                           flag;            // 4 type, name 0, 18.
    char                           quality;         // 5 type, name 0, 1413.
    char                           compress;        // 6 type, name 0, 1414.
    char                           exr_codec;       // 7 type, name 0, 1415.
    char                           cineon_flag;     // 8 type, name 0, 1416.
    short                          cineon_white;    // 9 type, name 2, 1417.
    short                          cineon_black;    // 10 type, name 2, 1418.
    float                          cineon_gamma;    // 11 type, name 7, 1419.
    char                           jp2_flag;        // 12 type, name 0, 1420.
    char                           jp2_codec;       // 13 type, name 0, 1421.
    char                           pad[6];          // 14 type, name 0, 736.
    ColorManagedViewSettings       view_settings;   // 15 type, name 219, 1422.
    ColorManagedDisplaySettings    display_settings;// 16 type, name 220, 1423.
};

// 479 DNA structure 251, 14
struct BGpic
{
    BGpic            *next; // 1 type, name 251, 0.
    BGpic            *prev; // 2 type, name 251, 1.
    Image            *ima;  // 3 type, name 43, 232.
    ImageUser        iuser; // 4 type, name 40, 317.
    MovieClip        *clip; // 5 type, name 249, 1749.
    MovieClipUser    cuser; // 6 type, name 252, 1753.
    float            xof;   // 7 type, name 7, 618.
    float            yof;   // 8 type, name 7, 619.
    float            size;  // 9 type, name 7, 102.
    float            blend; // 10 type, name 7, 1754.
    short            view;  // 11 type, name 2, 1755.
    short            flag;  // 12 type, name 2, 18.
    short            source;// 13 type, name 2, 137.
    short            pad;   // 14 type, name 2, 66.
};

// 480 DNA structure 304, 178
struct ThemeSpace
{
    char                back[4];                       // 1 type, name 0, 2034.
    char                title[4];                      // 2 type, name 0, 2068.
    char                text[4];                       // 3 type, name 0, 2021.
    char                text_hi[4];                    // 4 type, name 0, 2069.
    char                header[4];                     // 5 type, name 0, 2033.
    char                header_title[4];               // 6 type, name 0, 2070.
    char                header_text[4];                // 7 type, name 0, 2071.
    char                header_text_hi[4];             // 8 type, name 0, 2072.
    char                tab_active[4];                 // 9 type, name 0, 2073.
    char                tab_inactive[4];               // 10 type, name 0, 2074.
    char                tab_back[4];                   // 11 type, name 0, 2075.
    char                tab_outline[4];                // 12 type, name 0, 2076.
    char                button[4];                     // 13 type, name 0, 2077.
    char                button_title[4];               // 14 type, name 0, 2078.
    char                button_text[4];                // 15 type, name 0, 2079.
    char                button_text_hi[4];             // 16 type, name 0, 2080.
    char                list[4];                       // 17 type, name 0, 2081.
    char                list_title[4];                 // 18 type, name 0, 2082.
    char                list_text[4];                  // 19 type, name 0, 2083.
    char                list_text_hi[4];               // 20 type, name 0, 2084.
    uiPanelColors       panelcolors;                   // 21 type, name 301, 2085.
    uiGradientColors    gradients;                     // 22 type, name 302, 2086.
    char                shade1[4];                     // 23 type, name 0, 2087.
    char                shade2[4];                     // 24 type, name 0, 2088.
    char                hilite[4];                     // 25 type, name 0, 2089.
    char                grid[4];                       // 26 type, name 0, 2090.
    char                view_overlay[4];               // 27 type, name 0, 2091.
    char                wire[4];                       // 28 type, name 0, 2092.
    char                wire_edit[4];                  // 29 type, name 0, 2093.
    char                select[4];                     // 30 type, name 0, 2094.
    char                lamp[4];                       // 31 type, name 0, 2095.
    char                speaker[4];                    // 32 type, name 0, 2096.
    char                empty[4];                      // 33 type, name 0, 2097.
    char                camera[4];                     // 34 type, name 0, 2098.
    char                active[4];                     // 35 type, name 0, 2099.
    char                group[4];                      // 36 type, name 0, 2100.
    char                group_active[4];               // 37 type, name 0, 2101.
    char                transform[4];                  // 38 type, name 0, 2102.
    char                vertex[4];                     // 39 type, name 0, 2103.
    char                vertex_select[4];              // 40 type, name 0, 2104.
    char                vertex_unreferenced[4];        // 41 type, name 0, 2105.
    char                edge[4];                       // 42 type, name 0, 2106.
    char                edge_select[4];                // 43 type, name 0, 2107.
    char                edge_seam[4];                  // 44 type, name 0, 2108.
    char                edge_sharp[4];                 // 45 type, name 0, 2109.
    char                edge_facesel[4];               // 46 type, name 0, 2110.
    char                edge_crease[4];                // 47 type, name 0, 2111.
    char                face[4];                       // 48 type, name 0, 2112.
    char                face_select[4];                // 49 type, name 0, 2113.
    char                face_dot[4];                   // 50 type, name 0, 2114.
    char                extra_edge_len[4];             // 51 type, name 0, 2115.
    char                extra_edge_angle[4];           // 52 type, name 0, 2116.
    char                extra_face_angle[4];           // 53 type, name 0, 2117.
    char                extra_face_area[4];            // 54 type, name 0, 2118.
    char                normal[4];                     // 55 type, name 0, 2119.
    char                vertex_normal[4];              // 56 type, name 0, 2120.
    char                loop_normal[4];                // 57 type, name 0, 2121.
    char                bone_solid[4];                 // 58 type, name 0, 2122.
    char                bone_pose[4];                  // 59 type, name 0, 2123.
    char                bone_pose_active[4];           // 60 type, name 0, 2124.
    char                strip[4];                      // 61 type, name 0, 2125.
    char                strip_select[4];               // 62 type, name 0, 2126.
    char                cframe[4];                     // 63 type, name 0, 2127.
    char                freestyle_edge_mark[4];        // 64 type, name 0, 2128.
    char                freestyle_face_mark[4];        // 65 type, name 0, 2129.
    char                nurb_uline[4];                 // 66 type, name 0, 2130.
    char                nurb_vline[4];                 // 67 type, name 0, 2131.
    char                act_spline[4];                 // 68 type, name 0, 2132.
    char                nurb_sel_uline[4];             // 69 type, name 0, 2133.
    char                nurb_sel_vline[4];             // 70 type, name 0, 2134.
    char                lastsel_point[4];              // 71 type, name 0, 2135.
    char                handle_free[4];                // 72 type, name 0, 2136.
    char                handle_auto[4];                // 73 type, name 0, 2137.
    char                handle_vect[4];                // 74 type, name 0, 2138.
    char                handle_align[4];               // 75 type, name 0, 2139.
    char                handle_auto_clamped[4];        // 76 type, name 0, 2140.
    char                handle_sel_free[4];            // 77 type, name 0, 2141.
    char                handle_sel_auto[4];            // 78 type, name 0, 2142.
    char                handle_sel_vect[4];            // 79 type, name 0, 2143.
    char                handle_sel_align[4];           // 80 type, name 0, 2144.
    char                handle_sel_auto_clamped[4];    // 81 type, name 0, 2145.
    char                ds_channel[4];                 // 82 type, name 0, 2146.
    char                ds_subchannel[4];              // 83 type, name 0, 2147.
    char                keytype_keyframe[4];           // 84 type, name 0, 2148.
    char                keytype_extreme[4];            // 85 type, name 0, 2149.
    char                keytype_breakdown[4];          // 86 type, name 0, 2150.
    char                keytype_jitter[4];             // 87 type, name 0, 2151.
    char                keytype_keyframe_select[4];    // 88 type, name 0, 2152.
    char                keytype_extreme_select[4];     // 89 type, name 0, 2153.
    char                keytype_breakdown_select[4];   // 90 type, name 0, 2154.
    char                keytype_jitter_select[4];      // 91 type, name 0, 2155.
    char                keyborder[4];                  // 92 type, name 0, 2156.
    char                keyborder_select[4];           // 93 type, name 0, 2157.
    char                console_output[4];             // 94 type, name 0, 2158.
    char                console_input[4];              // 95 type, name 0, 2159.
    char                console_info[4];               // 96 type, name 0, 2160.
    char                console_error[4];              // 97 type, name 0, 2161.
    char                console_cursor[4];             // 98 type, name 0, 2162.
    char                console_select[4];             // 99 type, name 0, 2163.
    char                vertex_size;                   // 100 type, name 0, 2164.
    char                outline_width;                 // 101 type, name 0, 2165.
    char                facedot_size;                  // 102 type, name 0, 2166.
    char                noodle_curving;                // 103 type, name 0, 2167.
    char                syntaxl[4];                    // 104 type, name 0, 2168.
    char                syntaxs[4];                    // 105 type, name 0, 2169.
    char                syntaxb[4];                    // 106 type, name 0, 2170.
    char                syntaxn[4];                    // 107 type, name 0, 2171.
    char                syntaxv[4];                    // 108 type, name 0, 2172.
    char                syntaxc[4];                    // 109 type, name 0, 2173.
    char                syntaxd[4];                    // 110 type, name 0, 2174.
    char                syntaxr[4];                    // 111 type, name 0, 2175.
    char                nodeclass_output[4];           // 112 type, name 0, 2176.
    char                nodeclass_filter[4];           // 113 type, name 0, 2177.
    char                nodeclass_vector[4];           // 114 type, name 0, 2178.
    char                nodeclass_texture[4];          // 115 type, name 0, 2179.
    char                nodeclass_shader[4];           // 116 type, name 0, 2180.
    char                nodeclass_script[4];           // 117 type, name 0, 2181.
    char                nodeclass_pattern[4];          // 118 type, name 0, 2182.
    char                nodeclass_layout[4];           // 119 type, name 0, 2183.
    char                movie[4];                      // 120 type, name 0, 2184.
    char                movieclip[4];                  // 121 type, name 0, 2185.
    char                mask[4];                       // 122 type, name 0, 2186.
    char                image[4];                      // 123 type, name 0, 2187.
    char                scene[4];                      // 124 type, name 0, 2188.
    char                audio[4];                      // 125 type, name 0, 2189.
    char                effect[4];                     // 126 type, name 0, 2190.
    char                transition[4];                 // 127 type, name 0, 2191.
    char                meta[4];                       // 128 type, name 0, 2192.
    char                editmesh_active[4];            // 129 type, name 0, 2193.
    char                handle_vertex[4];              // 130 type, name 0, 2194.
    char                handle_vertex_select[4];       // 131 type, name 0, 2195.
    char                pad2[4];                       // 132 type, name 0, 2196.
    char                handle_vertex_size;            // 133 type, name 0, 2197.
    char                marker_outline[4];             // 134 type, name 0, 2198.
    char                marker[4];                     // 135 type, name 0, 2199.
    char                act_marker[4];                 // 136 type, name 0, 2200.
    char                sel_marker[4];                 // 137 type, name 0, 2201.
    char                dis_marker[4];                 // 138 type, name 0, 2202.
    char                lock_marker[4];                // 139 type, name 0, 2203.
    char                bundle_solid[4];               // 140 type, name 0, 2204.
    char                path_before[4];                // 141 type, name 0, 2205.
    char                path_after[4];                 // 142 type, name 0, 2206.
    char                camera_path[4];                // 143 type, name 0, 2207.
    char                hpad[3];                       // 144 type, name 0, 2208.
    char                preview_back[4];               // 145 type, name 0, 2209.
    char                preview_stitch_face[4];        // 146 type, name 0, 2210.
    char                preview_stitch_edge[4];        // 147 type, name 0, 2211.
    char                preview_stitch_vert[4];        // 148 type, name 0, 2212.
    char                preview_stitch_stitchable[4];  // 149 type, name 0, 2213.
    char                preview_stitch_unstitchable[4];// 150 type, name 0, 2214.
    char                preview_stitch_active[4];      // 151 type, name 0, 2215.
    char                uv_shadow[4];                  // 152 type, name 0, 2216.
    char                uv_others[4];                  // 153 type, name 0, 2217.
    char                match[4];                      // 154 type, name 0, 2218.
    char                selected_highlight[4];         // 155 type, name 0, 2219.
    char                skin_root[4];                  // 156 type, name 0, 2220.
    char                anim_active[4];                // 157 type, name 0, 2221.
    char                anim_non_active[4];            // 158 type, name 0, 2222.
    char                nla_tweaking[4];               // 159 type, name 0, 2223.
    char                nla_tweakdupli[4];             // 160 type, name 0, 2224.
    char                nla_transition[4];             // 161 type, name 0, 2225.
    char                nla_transition_sel[4];         // 162 type, name 0, 2226.
    char                nla_meta[4];                   // 163 type, name 0, 2227.
    char                nla_meta_sel[4];               // 164 type, name 0, 2228.
    char                nla_sound[4];                  // 165 type, name 0, 2229.
    char                nla_sound_sel[4];              // 166 type, name 0, 2230.
    char                info_selected[4];              // 167 type, name 0, 2231.
    char                info_selected_text[4];         // 168 type, name 0, 2232.
    char                info_error[4];                 // 169 type, name 0, 2233.
    char                info_error_text[4];            // 170 type, name 0, 2234.
    char                info_warning[4];               // 171 type, name 0, 2235.
    char                info_warning_text[4];          // 172 type, name 0, 2236.
    char                info_info[4];                  // 173 type, name 0, 2237.
    char                info_info_text[4];             // 174 type, name 0, 2238.
    char                info_debug[4];                 // 175 type, name 0, 2239.
    char                info_debug_text[4];            // 176 type, name 0, 2240.
    char                paint_curve_pivot[4];          // 177 type, name 0, 2241.
    char                paint_curve_handle[4];         // 178 type, name 0, 2242.
};

// 481 DNA structure 334, 52
struct Sequence
{
    Sequence     *next;          // 1 type, name 334, 0.
    Sequence     *prev;          // 2 type, name 334, 1.
    void         *tmp;           // 3 type, name 11, 2491.
    void         *lib;           // 4 type, name 11, 25.
    char         name[64];       // 5 type, name 0, 19.
    int          flag;           // 6 type, name 4, 18.
    int          type;           // 7 type, name 4, 16.
    int          len;            // 8 type, name 4, 22.
    int          start;          // 9 type, name 4, 737.
    int          startofs;       // 10 type, name 4, 2492.
    int          endofs;         // 11 type, name 4, 2493.
    int          startstill;     // 12 type, name 4, 2485.
    int          endstill;       // 13 type, name 4, 2486.
    int          machine;        // 14 type, name 4, 2494.
    int          depth;          // 15 type, name 4, 240.
    int          startdisp;      // 16 type, name 4, 2495.
    int          enddisp;        // 17 type, name 4, 2496.
    float        sat;            // 18 type, name 7, 2497.
    float        mul;            // 19 type, name 7, 2498.
    float        handsize;       // 20 type, name 7, 2499.
    short        anim_preseek;   // 21 type, name 2, 2500.
    short        streamindex;    // 22 type, name 2, 2501.
    int          multicam_source;// 23 type, name 4, 2502.
    int          clip_flag;      // 24 type, name 4, 2503.
    Strip        *strip;         // 25 type, name 333, 2504.
    Ipo          *ipo;           // 26 type, name 33, 80.
    Scene        *scene;         // 27 type, name 41, 119.
    Object       *scene_camera;  // 28 type, name 29, 2505.
    MovieClip    *clip;          // 29 type, name 249, 1749.
    Mask         *mask;          // 30 type, name 275, 1884.
    void         *anim;          // Note: using void* on undefined DNA type: anim
    float        effect_fader;   // 32 type, name 7, 2506.
    float        speed_fader;    // 33 type, name 7, 2507.
    Sequence     *seq1;          // 34 type, name 334, 2508.
    Sequence     *seq2;          // 35 type, name 334, 2509.
    Sequence     *seq3;          // 36 type, name 334, 2510.
    ListBase     seqbase;        // 37 type, name 14, 2511.
    bSound       *sound;         // 38 type, name 335, 2512.
    void         *scene_sound;   // 39 type, name 11, 2513.
    float        volume;         // 40 type, name 7, 1165.
    float        pitch;          // 41 type, name 7, 2514.
    float        pan;            // 42 type, name 7, 2515.
    float        strobe;         // 43 type, name 7, 2516.
    void         *effectdata;    // 44 type, name 11, 2517.
    int          anim_startofs;  // 45 type, name 4, 2518.
    int          anim_endofs;    // 46 type, name 4, 2519.
    int          blend_mode;     // 47 type, name 4, 2520.
    float        blend_opacity;  // 48 type, name 7, 2521.
    int          sfra;           // 49 type, name 4, 123.
    char         alpha_mode;     // 50 type, name 0, 160.
    char         pad[3];         // 51 type, name 0, 333.
    ListBase     modifiers;      // 52 type, name 14, 1004.
};

// 482 DNA structure 344, 3
struct ColorBalanceModifierData
{
    SequenceModifierData    modifier;      // 1 type, name 343, 724.
    StripColorBalance       color_balance; // 2 type, name 331, 2558.
    float                   color_multiply;// 3 type, name 7, 2559.
};

// 483 DNA structure 66, 4
struct Group
{
    ID          id;          // 1 type, name 22, 31.
    ListBase    gobject;     // 2 type, name 14, 2717.
    int         layer;       // 3 type, name 4, 128.
    float       dupli_ofs[3];// 4 type, name 7, 2718.
};

// 484 DNA structure 405, 7
struct bActionGroup
{
    bActionGroup      *next;    // 1 type, name 405, 0.
    bActionGroup      *prev;    // 2 type, name 405, 1.
    ListBase          channels; // 3 type, name 14, 2815.
    int               flag;     // 4 type, name 4, 18.
    int               customCol;// 5 type, name 4, 2816.
    char              name[64]; // 6 type, name 0, 19.
    ThemeWireColor    cs;       // 7 type, name 305, 2817.
};

// 485 DNA structure 449, 5
struct bNodePreview
{
    bNodeInstanceHashEntry    hash_entry;// 1 type, name 448, 2959.
    char                      *rect;     // 2 type, name 0, 2960.
    short                     xsize;     // 3 type, name 2, 2961.
    short                     ysize;     // 4 type, name 2, 2962.
    int                       pad;       // 5 type, name 4, 66.
};

// 486 DNA structure 530, 19
struct ParticleData
{
    ParticleKey     state;      // 1 type, name 523, 1083.
    ParticleKey     prev_state; // 2 type, name 523, 3236.
    HairKey         *hair;      // 3 type, name 522, 3237.
    ParticleKey     *keys;      // 4 type, name 523, 2595.
    BoidParticle    *boid;      // 5 type, name 524, 3238.
    int             totkey;     // 6 type, name 4, 82.
    float           time;       // 7 type, name 7, 768.
    float           lifetime;   // 8 type, name 7, 794.
    float           dietime;    // 9 type, name 7, 3239.
    int             num;        // 10 type, name 4, 3230.
    int             num_dmcache;// 11 type, name 4, 3240.
    float           fuv[4];     // 12 type, name 7, 3234.
    float           foffset;    // 13 type, name 7, 3235.
    float           size;       // 14 type, name 7, 102.
    float           sphdensity; // 15 type, name 7, 3241.
    int             pad;        // 16 type, name 4, 66.
    int             hair_index; // 17 type, name 4, 3242.
    short           flag;       // 18 type, name 2, 18.
    short           alive;      // 19 type, name 2, 3243.
};

// 487 DNA structure 532, 132
struct ParticleSettings
{
    ID                  id;                 // 1 type, name 22, 31.
    AnimData            *adt;               // 2 type, name 36, 75.
    BoidSettings        *boids;             // 3 type, name 533, 3256.
    SPHFluidSettings    *fluid;             // 4 type, name 531, 3257.
    EffectorWeights     *effector_weights;  // 5 type, name 200, 1235.
    int                 flag;               // 6 type, name 4, 18.
    int                 rt;                 // 7 type, name 4, 56.
    short               type;               // 8 type, name 2, 16.
    short               from;               // 9 type, name 2, 2877.
    short               distr;              // 10 type, name 2, 3258.
    short               texact;             // 11 type, name 2, 370.
    short               phystype;           // 12 type, name 2, 3259.
    short               rotmode;            // 13 type, name 2, 1050.
    short               avemode;            // 14 type, name 2, 3260.
    short               reactevent;         // 15 type, name 2, 3261.
    int                 draw;               // 16 type, name 4, 3262.
    int                 pad1;               // 17 type, name 4, 68.
    short               draw_as;            // 18 type, name 2, 3263.
    short               draw_size;          // 19 type, name 2, 3264.
    short               childtype;          // 20 type, name 2, 3265.
    short               pad2;               // 21 type, name 2, 29.
    short               ren_as;             // 22 type, name 2, 3266.
    short               subframes;          // 23 type, name 2, 3267.
    short               draw_col;           // 24 type, name 2, 3268.
    short               draw_step;          // 25 type, name 2, 1595.
    short               ren_step;           // 26 type, name 2, 3269.
    short               hair_step;          // 27 type, name 2, 3270.
    short               keys_step;          // 28 type, name 2, 3271.
    short               adapt_angle;        // 29 type, name 2, 3272.
    short               adapt_pix;          // 30 type, name 2, 3273.
    short               disp;               // 31 type, name 2, 546.
    short               omat;               // 32 type, name 2, 2588.
    short               interpolation;      // 33 type, name 2, 2548.
    short               integrator;         // 34 type, name 2, 3274.
    short               rotfrom;            // 35 type, name 2, 3275.
    short               kink;               // 36 type, name 2, 1105.
    short               kink_axis;          // 37 type, name 2, 1106.
    short               bb_align;           // 38 type, name 2, 3276.
    short               bb_uv_split;        // 39 type, name 2, 3277.
    short               bb_anim;            // 40 type, name 2, 3278.
    short               bb_split_offset;    // 41 type, name 2, 3279.
    float               bb_tilt;            // 42 type, name 7, 3280.
    float               bb_rand_tilt;       // 43 type, name 7, 3281.
    float               bb_offset[2];       // 44 type, name 7, 3282.
    float               bb_size[2];         // 45 type, name 7, 3283.
    float               bb_vel_head;        // 46 type, name 7, 3284.
    float               bb_vel_tail;        // 47 type, name 7, 3285.
    float               color_vec_max;      // 48 type, name 7, 3286.
    short               simplify_flag;      // 49 type, name 2, 1510.
    short               simplify_refsize;   // 50 type, name 2, 3287.
    float               simplify_rate;      // 51 type, name 7, 3288.
    float               simplify_transition;// 52 type, name 7, 3289.
    float               simplify_viewport;  // 53 type, name 7, 3290.
    float               sta;                // 54 type, name 7, 2569.
    float               end;                // 55 type, name 7, 2570.
    float               lifetime;           // 56 type, name 7, 794.
    float               randlife;           // 57 type, name 7, 2576.
    float               timetweak;          // 58 type, name 7, 3291.
    float               courant_target;     // 59 type, name 7, 3292.
    float               jitfac;             // 60 type, name 7, 3293.
    float               eff_hair;           // 61 type, name 7, 3294.
    float               grid_rand;          // 62 type, name 7, 3295.
    float               ps_offset[1];       // 63 type, name 7, 3296.
    int                 totpart;            // 64 type, name 4, 2571.
    int                 userjit;            // 65 type, name 4, 2568.
    int                 grid_res;           // 66 type, name 4, 3297.
    int                 effector_amount;    // 67 type, name 4, 3298.
    short               time_flag;          // 68 type, name 2, 3299.
    short               time_pad[3];        // 69 type, name 2, 3300.
    float               normfac;            // 70 type, name 7, 2572.
    float               obfac;              // 71 type, name 7, 2573.
    float               randfac;            // 72 type, name 7, 2574.
    float               partfac;            // 73 type, name 7, 3301.
    float               tanfac;             // 74 type, name 7, 3302.
    float               tanphase;           // 75 type, name 7, 3303.
    float               reactfac;           // 76 type, name 7, 3304.
    float               ob_vel[3];          // 77 type, name 7, 3305.
    float               avefac;             // 78 type, name 7, 3306.
    float               phasefac;           // 79 type, name 7, 3307.
    float               randrotfac;         // 80 type, name 7, 3308.
    float               randphasefac;       // 81 type, name 7, 3309.
    float               mass;               // 82 type, name 7, 1036.
    float               size;               // 83 type, name 7, 102.
    float               randsize;           // 84 type, name 7, 3310.
    float               acc[3];             // 85 type, name 7, 3311.
    float               dragfac;            // 86 type, name 7, 3312.
    float               brownfac;           // 87 type, name 7, 3313.
    float               dampfac;            // 88 type, name 7, 212.
    float               randlength;         // 89 type, name 7, 3314.
    int                 child_nbr;          // 90 type, name 4, 3315.
    int                 ren_child_nbr;      // 91 type, name 4, 3316.
    float               parents;            // 92 type, name 7, 3317.
    float               childsize;          // 93 type, name 7, 3318.
    float               childrandsize;      // 94 type, name 7, 3319.
    float               childrad;           // 95 type, name 7, 3320.
    float               childflat;          // 96 type, name 7, 3321.
    float               clumpfac;           // 97 type, name 7, 211.
    float               clumppow;           // 98 type, name 7, 3322.
    float               kink_amp;           // 99 type, name 7, 1132.
    float               kink_freq;          // 100 type, name 7, 1130.
    float               kink_shape;         // 101 type, name 7, 1131.
    float               kink_flat;          // 102 type, name 7, 3323.
    float               kink_amp_clump;     // 103 type, name 7, 3324.
    float               rough1;             // 104 type, name 7, 3325.
    float               rough1_size;        // 105 type, name 7, 3326.
    float               rough2;             // 106 type, name 7, 3327.
    float               rough2_size;        // 107 type, name 7, 3328.
    float               rough2_thres;       // 108 type, name 7, 3329.
    float               rough_end;          // 109 type, name 7, 3330.
    float               rough_end_shape;    // 110 type, name 7, 3331.
    float               clength;            // 111 type, name 7, 3332.
    float               clength_thres;      // 112 type, name 7, 3333.
    float               parting_fac;        // 113 type, name 7, 3334.
    float               parting_min;        // 114 type, name 7, 3335.
    float               parting_max;        // 115 type, name 7, 3336.
    float               branch_thres;       // 116 type, name 7, 3337.
    float               draw_line[2];       // 117 type, name 7, 3338.
    float               path_start;         // 118 type, name 7, 3339.
    float               path_end;           // 119 type, name 7, 3340.
    int                 trail_count;        // 120 type, name 4, 3341.
    int                 keyed_loops;        // 121 type, name 4, 3342.
    MTex                *mtex[18];          // 122 type, name 48, 389.
    Group               *dup_group;         // 123 type, name 66, 1074.
    ListBase            dupliweights;       // 124 type, name 14, 3343.
    Group               *eff_group;         // 125 type, name 66, 3344.
    Object              *dup_ob;            // 126 type, name 29, 3345.
    Object              *bb_ob;             // 127 type, name 29, 3346.
    Ipo                 *ipo;               // 128 type, name 33, 80.
    PartDeflect         *pd;                // 129 type, name 192, 320.
    PartDeflect         *pd2;               // 130 type, name 192, 3347.
    short               use_modifier_stack; // 131 type, name 2, 3348.
    short               pad[3];             // 132 type, name 2, 333.
};

// 488 DNA structure 188, 7
struct bGPdata
{
    ID          id;           // 1 type, name 22, 31.
    AnimData    *adt;         // 2 type, name 36, 75.
    ListBase    layers;       // 3 type, name 14, 1476.
    int         flag;         // 4 type, name 4, 18.
    short       sbuffer_size; // 5 type, name 2, 3433.
    short       sbuffer_sflag;// 6 type, name 2, 3434.
    void        *sbuffer;     // 7 type, name 11, 3435.
};

// 489 DNA structure 555, 11
struct wmKeyMap
{
    wmKeyMap    *next;       // 1 type, name 555, 0.
    wmKeyMap    *prev;       // 2 type, name 555, 1.
    ListBase    items;       // 3 type, name 14, 3495.
    ListBase    diff_items;  // 4 type, name 14, 3496.
    char        idname[64];  // 5 type, name 0, 2419.
    short       spaceid;     // 6 type, name 2, 3497.
    short       regionid;    // 7 type, name 2, 3498.
    short       flag;        // 8 type, name 2, 18.
    short       kmi_id;      // 9 type, name 2, 3499.
    int         (*poll)();   // 10 type, name 4, 3500.
    void        *modal_items;// 11 type, name 11, 3501.
};

// 490 DNA structure 606, 7
struct MovieTrackingDopesheet
{
    int         ok;               // 1 type, name 4, 126.
    short       sort_method;      // 2 type, name 2, 3757.
    short       flag;             // 3 type, name 2, 18.
    ListBase    coverage_segments;// 4 type, name 14, 3758.
    ListBase    channels;         // 5 type, name 14, 2815.
    int         tot_channel;      // 6 type, name 4, 3759.
    int         pad;              // 7 type, name 4, 66.
};

// 491 DNA structure 275, 9
struct Mask
{
    ID          id;         // 1 type, name 22, 31.
    AnimData    *adt;       // 2 type, name 36, 75.
    ListBase    masklayers; // 3 type, name 14, 3818.
    int         masklay_act;// 4 type, name 4, 3819.
    int         masklay_tot;// 5 type, name 4, 3820.
    int         sfra;       // 6 type, name 4, 123.
    int         efra;       // 7 type, name 4, 1213.
    int         flag;       // 8 type, name 4, 18.
    int         pad;        // 9 type, name 4, 66.
};

// 492 DNA structure 611, 5
struct MaskSplinePoint
{
    BezTriple            bezt;  // 1 type, name 32, 3827.
    int                  pad;   // 2 type, name 4, 66.
    int                  tot_uw;// 3 type, name 4, 3828.
    MaskSplinePointUW    *uw;   // 4 type, name 610, 3829.
    MaskParent           parent;// 5 type, name 609, 3231.
};

// 493 DNA structure 614, 14
struct MaskLayer
{
    MaskLayer          *next;         // 1 type, name 614, 0.
    MaskLayer          *prev;         // 2 type, name 614, 1.
    char               name[64];      // 3 type, name 0, 19.
    ListBase           splines;       // 4 type, name 14, 3835.
    ListBase           splines_shapes;// 5 type, name 14, 3836.
    MaskSpline         *act_spline;   // 6 type, name 612, 3837.
    MaskSplinePoint    *act_point;    // 7 type, name 611, 3838.
    float              alpha;         // 8 type, name 7, 429.
    char               blend;         // 9 type, name 0, 1754.
    char               blend_flag;    // 10 type, name 0, 3839.
    char               falloff;       // 11 type, name 0, 792.
    char               pad[7];        // 12 type, name 0, 118.
    char               flag;          // 13 type, name 0, 18.
    char               restrictflag;  // 14 type, name 0, 1066.
};

// 494 DNA structure 217, 8
struct FreestyleConfig
{
    ListBase    modules;             // 1 type, name 14, 3892.
    int         mode;                // 2 type, name 4, 334.
    int         raycasting_algorithm;// 3 type, name 4, 3893.
    int         flags;               // 4 type, name 4, 90.
    float       sphere_radius;       // 5 type, name 7, 3894.
    float       dkr_epsilon;         // 6 type, name 7, 3895.
    float       crease_angle;        // 7 type, name 7, 3896.
    ListBase    linesets;            // 8 type, name 14, 3897.
};

// 495 DNA structure 33, 7
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

// 496 DNA structure 43, 40
struct Image
{
    ID                                id;                 // 1 type, name 22, 31.
    char                              name[1024];         // 2 type, name 0, 34.
    void                              *cache;             // Note: using void* on undefined DNA type: MovieCache
    void                              *gputexture;        // Note: using void* on undefined DNA type: GPUTexture
    void                              *anim;              // Note: using void* on undefined DNA type: anim
    void                              *rr;                // Note: using void* on undefined DNA type: RenderResult
    void                              *renders[8];        // Note: using void* on undefined DNA type: RenderResult
    short                             render_slot;        // 8 type, name 2, 135.
    short                             last_render_slot;   // 9 type, name 2, 136.
    short                             ok;                 // 10 type, name 2, 126.
    short                             flag;               // 11 type, name 2, 18.
    short                             source;             // 12 type, name 2, 137.
    short                             type;               // 13 type, name 2, 16.
    int                               lastframe;          // 14 type, name 4, 138.
    short                             tpageflag;          // 15 type, name 2, 139.
    short                             totbind;            // 16 type, name 2, 140.
    short                             xrep;               // 17 type, name 2, 141.
    short                             yrep;               // 18 type, name 2, 142.
    short                             twsta;              // 19 type, name 2, 143.
    short                             twend;              // 20 type, name 2, 144.
    int                               bindcode;           // 21 type, name 4, 145.
    int                               *repbind;           // 22 type, name 4, 146.
    PackedFile                        *packedfile;        // 23 type, name 25, 37.
    PreviewImage                      *preview;           // 24 type, name 26, 147.
    float                             lastupdate;         // 25 type, name 7, 148.
    int                               lastused;           // 26 type, name 4, 149.
    short                             animspeed;          // 27 type, name 2, 150.
    short                             pad2;               // 28 type, name 2, 29.
    int                               gen_x;              // 29 type, name 4, 151.
    int                               gen_y;              // 30 type, name 4, 152.
    char                              gen_type;           // 31 type, name 0, 153.
    char                              gen_flag;           // 32 type, name 0, 154.
    short                             gen_depth;          // 33 type, name 2, 155.
    float                             gen_color[4];       // 34 type, name 7, 156.
    float                             aspx;               // 35 type, name 7, 157.
    float                             aspy;               // 36 type, name 7, 158.
    ColorManagedColorspaceSettings    colorspace_settings;// 37 type, name 47, 159.
    char                              alpha_mode;         // 38 type, name 0, 160.
    char                              pad[7];             // 39 type, name 0, 118.
    RenderSlot                        render_slots[8];    // 40 type, name 42, 161.
};

// 497 DNA structure 71, 19
struct MetaBall
{
    ID          id;        // 1 type, name 22, 31.
    AnimData    *adt;      // 2 type, name 36, 75.
    ListBase    elems;     // 3 type, name 14, 545.
    ListBase    disp;      // 4 type, name 14, 546.
    ListBase    *editelems;// 5 type, name 14, 547.
    Ipo         *ipo;      // 6 type, name 33, 80.
    Material    **mat;     // 7 type, name 65, 548.
    char        flag;      // 8 type, name 0, 18.
    char        flag2;     // 9 type, name 0, 549.
    short       totcol;    // 10 type, name 2, 550.
    short       texflag;   // 11 type, name 2, 176.
    short       pad;       // 12 type, name 2, 66.
    float       loc[3];    // 13 type, name 7, 324.
    float       size[3];   // 14 type, name 7, 174.
    float       rot[3];    // 15 type, name 7, 325.
    float       wiresize;  // 16 type, name 7, 551.
    float       rendersize;// 17 type, name 7, 552.
    float       thresh;    // 18 type, name 7, 553.
    MetaElem    *lastelem; // 19 type, name 69, 554.
};

// 498 DNA structure 77, 70
struct Curve
{
    ID          id;             // 1 type, name 22, 31.
    AnimData    *adt;           // 2 type, name 36, 75.
    BoundBox    *bb;            // 3 type, name 70, 533.
    ListBase    nurb;           // 4 type, name 14, 591.
    EditNurb    *editnurb;      // 5 type, name 75, 592.
    Object      *bevobj;        // 6 type, name 29, 593.
    Object      *taperobj;      // 7 type, name 29, 594.
    Object      *textoncurve;   // 8 type, name 29, 595.
    Ipo         *ipo;           // 9 type, name 33, 80.
    Key         *key;           // 10 type, name 35, 596.
    Material    **mat;          // 11 type, name 65, 548.
    float       loc[3];         // 12 type, name 7, 324.
    float       size[3];        // 13 type, name 7, 174.
    float       rot[3];         // 14 type, name 7, 325.
    short       type;           // 15 type, name 2, 16.
    short       texflag;        // 16 type, name 2, 176.
    short       drawflag;       // 17 type, name 2, 597.
    short       twist_mode;     // 18 type, name 2, 598.
    float       twist_smooth;   // 19 type, name 7, 599.
    float       smallcaps_scale;// 20 type, name 7, 600.
    int         pathlen;        // 21 type, name 4, 601.
    short       bevresol;       // 22 type, name 2, 602.
    short       totcol;         // 23 type, name 2, 550.
    int         flag;           // 24 type, name 4, 18.
    float       width;          // 25 type, name 7, 603.
    float       ext1;           // 26 type, name 7, 604.
    float       ext2;           // 27 type, name 7, 605.
    short       resolu;         // 28 type, name 2, 574.
    short       resolv;         // 29 type, name 2, 575.
    short       resolu_ren;     // 30 type, name 2, 606.
    short       resolv_ren;     // 31 type, name 2, 607.
    int         actnu;          // 32 type, name 4, 608.
    int         actvert;        // 33 type, name 4, 609.
    char        pad[4];         // 34 type, name 0, 568.
    short       lines;          // 35 type, name 2, 92.
    char        spacemode;      // 36 type, name 0, 610.
    char        pad1;           // 37 type, name 0, 68.
    float       spacing;        // 38 type, name 7, 611.
    float       linedist;       // 39 type, name 7, 612.
    float       shear;          // 40 type, name 7, 613.
    float       fsize;          // 41 type, name 7, 614.
    float       wordspace;      // 42 type, name 7, 615.
    float       ulpos;          // 43 type, name 7, 616.
    float       ulheight;       // 44 type, name 7, 617.
    float       xof;            // 45 type, name 7, 618.
    float       yof;            // 46 type, name 7, 619.
    float       linewidth;      // 47 type, name 7, 620.
    int         pos;            // 48 type, name 4, 67.
    int         selstart;       // 49 type, name 4, 621.
    int         selend;         // 50 type, name 4, 622.
    int         len_wchar;      // 51 type, name 4, 623.
    int         len;            // 52 type, name 4, 22.
    char        *str;           // 53 type, name 0, 624.
    void        *editfont;      // Note: using void* on undefined DNA type: EditFont
    char        family[64];     // 55 type, name 0, 626.
    VFont       *vfont;         // 56 type, name 67, 627.
    VFont       *vfontb;        // 57 type, name 67, 628.
    VFont       *vfonti;        // 58 type, name 67, 629.
    VFont       *vfontbi;       // 59 type, name 67, 630.
    TextBox     *tb;            // 60 type, name 74, 631.
    int         totbox;         // 61 type, name 4, 632.
    int         actbox;         // 62 type, name 4, 633.
    CharInfo    *strinfo;       // 63 type, name 73, 634.
    CharInfo    curinfo;        // 64 type, name 73, 635.
    float       ctime;          // 65 type, name 7, 84.
    float       bevfac1;        // 66 type, name 7, 636.
    float       bevfac2;        // 67 type, name 7, 637.
    char        bevfac1_mapping;// 68 type, name 0, 638.
    char        bevfac2_mapping;// 69 type, name 0, 639.
    char        pad2[2];        // 70 type, name 0, 640.
};

// 499 DNA structure 95, 14
struct Multires
{
    ListBase      levels;       // 1 type, name 14, 711.
    MVert         *verts;       // 2 type, name 89, 710.
    char          level_count;  // 3 type, name 0, 712.
    char          current;      // 4 type, name 0, 713.
    char          newlvl;       // 5 type, name 0, 714.
    char          edgelvl;      // 6 type, name 0, 715.
    char          pinlvl;       // 7 type, name 0, 716.
    char          renderlvl;    // 8 type, name 0, 717.
    char          use_col;      // 9 type, name 0, 718.
    char          flag;         // 10 type, name 0, 18.
    CustomData    vdata;        // 11 type, name 94, 656.
    CustomData    fdata;        // 12 type, name 94, 658.
    short         *edge_flags;  // 13 type, name 2, 719.
    char          *edge_creases;// 14 type, name 0, 720.
};

// 500 DNA structure 216, 14
struct SceneRenderLayer
{
    SceneRenderLayer    *next;               // 1 type, name 216, 0.
    SceneRenderLayer    *prev;               // 2 type, name 216, 1.
    char                name[64];            // 3 type, name 0, 19.
    Material            *mat_override;       // 4 type, name 65, 1401.
    Group               *light_override;     // 5 type, name 66, 1402.
    int                 lay;                 // 6 type, name 4, 1021.
    int                 lay_zmask;           // 7 type, name 4, 1403.
    int                 lay_exclude;         // 8 type, name 4, 1404.
    int                 layflag;             // 9 type, name 4, 1405.
    int                 passflag;            // 10 type, name 4, 1406.
    int                 pass_xor;            // 11 type, name 4, 1407.
    int                 samples;             // 12 type, name 4, 1408.
    float               pass_alpha_threshold;// 13 type, name 7, 1409.
    FreestyleConfig     freestyleConfig;     // 14 type, name 217, 1410.
};

// 501 DNA structure 221, 13
struct BakeData
{
    ImageFormatData    im_format;        // 1 type, name 218, 1424.
    char               filepath[1024];   // 2 type, name 0, 35.
    short              width;            // 3 type, name 2, 603.
    short              height;           // 4 type, name 2, 788.
    short              margin;           // 5 type, name 2, 1041.
    short              flag;             // 6 type, name 2, 18.
    float              cage_extrusion;   // 7 type, name 7, 1425.
    float              pad2;             // 8 type, name 7, 29.
    char               normal_swizzle[3];// 9 type, name 0, 1426.
    char               normal_space;     // 10 type, name 0, 1427.
    char               save_mode;        // 11 type, name 0, 1428.
    char               pad[3];           // 12 type, name 0, 333.
    char               cage[64];         // 13 type, name 0, 1429.
};

// 502 DNA structure 227, 36
struct GameData
{
    GameFraming    framing;              // 1 type, name 225, 1554.
    short          playerflag;           // 2 type, name 2, 1555.
    short          xplay;                // 3 type, name 2, 1445.
    short          yplay;                // 4 type, name 2, 1446.
    short          freqplay;             // 5 type, name 2, 1447.
    short          depth;                // 6 type, name 2, 240.
    short          attrib;               // 7 type, name 2, 1448.
    short          rt1;                  // 8 type, name 2, 1556.
    short          rt2;                  // 9 type, name 2, 1557.
    short          aasamples;            // 10 type, name 2, 1558.
    short          pad4[3];              // 11 type, name 2, 529.
    GameDome       dome;                 // 12 type, name 224, 1559.
    short          stereoflag;           // 13 type, name 2, 1560.
    short          stereomode;           // 14 type, name 2, 1450.
    float          eyeseparation;        // 15 type, name 7, 1561.
    RecastData     recastData;           // 16 type, name 226, 1562.
    float          gravity;              // 17 type, name 7, 1301.
    float          activityBoxRadius;    // 18 type, name 7, 1302.
    int            flag;                 // 19 type, name 4, 18.
    short          mode;                 // 20 type, name 2, 334.
    short          matmode;              // 21 type, name 2, 1563.
    short          occlusionRes;         // 22 type, name 2, 1304.
    short          physicsEngine;        // 23 type, name 2, 1305.
    short          exitkey;              // 24 type, name 2, 1564.
    short          vsync;                // 25 type, name 2, 1565.
    short          ticrate;              // 26 type, name 2, 1306.
    short          maxlogicstep;         // 27 type, name 2, 1307.
    short          physubstep;           // 28 type, name 2, 1308.
    short          maxphystep;           // 29 type, name 2, 1309.
    short          obstacleSimulation;   // 30 type, name 2, 1566.
    short          raster_storage;       // 31 type, name 2, 1567.
    float          levelHeight;          // 32 type, name 7, 1568.
    float          deactivationtime;     // 33 type, name 7, 1569.
    float          lineardeactthreshold; // 34 type, name 7, 1570.
    float          angulardeactthreshold;// 35 type, name 7, 1571.
    float          pad2;                 // 36 type, name 7, 29.
};

// 503 DNA structure 284, 33
struct SpaceText
{
    SpaceLink    *next;          // 1 type, name 259, 0.
    SpaceLink    *prev;          // 2 type, name 259, 1.
    ListBase     regionbase;     // 3 type, name 14, 1792.
    int          spacetype;      // 4 type, name 4, 1793.
    float        blockscale;     // 5 type, name 7, 1794.
    short        blockhandler[8];// 6 type, name 2, 1795.
    Text         *text;          // 7 type, name 38, 1924.
    int          top;            // 8 type, name 4, 1925.
    int          viewlines;      // 9 type, name 4, 1926.
    short        flags;          // 10 type, name 2, 90.
    short        menunr;         // 11 type, name 2, 1927.
    short        lheight;        // 12 type, name 2, 1928.
    char         cwidth;         // 13 type, name 0, 1929.
    char         linenrs_tot;    // 14 type, name 0, 1930.
    int          left;           // 15 type, name 4, 1931.
    int          showlinenrs;    // 16 type, name 4, 1932.
    int          tabnumber;      // 17 type, name 4, 1933.
    short        showsyntax;     // 18 type, name 2, 1934.
    short        line_hlight;    // 19 type, name 2, 1935.
    short        overwrite;      // 20 type, name 2, 1936.
    short        live_edit;      // 21 type, name 2, 1937.
    float        pix_per_line;   // 22 type, name 7, 1938.
    rcti         txtscroll;      // 23 type, name 18, 1939.
    rcti         txtbar;         // 24 type, name 18, 1940.
    int          wordwrap;       // 25 type, name 4, 1941.
    int          doplugins;      // 26 type, name 4, 1942.
    char         findstr[256];   // 27 type, name 0, 1943.
    char         replacestr[256];// 28 type, name 0, 1944.
    short        margin_column;  // 29 type, name 2, 1945.
    short        lheight_dpi;    // 30 type, name 2, 1946.
    char         pad[4];         // 31 type, name 0, 568.
    void         *drawcache;     // 32 type, name 11, 1947.
    float        scroll_accum[2];// 33 type, name 7, 1948.
};

// 504 DNA structure 292, 14
struct SpaceConsole
{
    SpaceLink    *next;          // 1 type, name 259, 0.
    SpaceLink    *prev;          // 2 type, name 259, 1.
    ListBase     regionbase;     // 3 type, name 14, 1792.
    int          spacetype;      // 4 type, name 4, 1793.
    float        blockscale;     // 5 type, name 7, 1794.
    short        blockhandler[8];// 6 type, name 2, 1795.
    int          lheight;        // 7 type, name 4, 1928.
    int          pad;            // 8 type, name 4, 66.
    ListBase     scrollback;     // 9 type, name 14, 1973.
    ListBase     history;        // 10 type, name 14, 1974.
    char         prompt[256];    // 11 type, name 0, 1975.
    char         language[32];   // 12 type, name 0, 1976.
    int          sel_start;      // 13 type, name 4, 1977.
    int          sel_end;        // 14 type, name 4, 1978.
};

// 505 DNA structure 246, 11
struct Editing
{
    ListBase    *seqbasep;         // 1 type, name 14, 2525.
    ListBase    seqbase;           // 2 type, name 14, 2511.
    ListBase    metastack;         // 3 type, name 14, 2526.
    Sequence    *act_seq;          // 4 type, name 334, 2527.
    char        act_imagedir[1024];// 5 type, name 0, 2528.
    char        act_sounddir[1024];// 6 type, name 0, 2529.
    int         over_ofs;          // 7 type, name 4, 2530.
    int         over_cfra;         // 8 type, name 4, 2531.
    int         over_flag;         // 9 type, name 4, 2532.
    int         pad;               // 10 type, name 4, 66.
    rctf        over_border;       // 11 type, name 19, 2533.
};

// 506 DNA structure 399, 27
struct bArmature
{
    ID          id;             // 1 type, name 22, 31.
    AnimData    *adt;           // 2 type, name 36, 75.
    ListBase    bonebase;       // 3 type, name 14, 2735.
    ListBase    chainbase;      // 4 type, name 14, 2736.
    ListBase    *edbo;          // 5 type, name 14, 2737.
    Bone        *act_bone;      // 6 type, name 398, 2738.
    void        *act_edbone;    // Note: using void* on undefined DNA type: EditBone
    void        *sketch;        // 8 type, name 11, 2740.
    int         flag;           // 9 type, name 4, 18.
    int         drawtype;       // 10 type, name 4, 1805.
    int         gevertdeformer; // 11 type, name 4, 2741.
    int         pad;            // 12 type, name 4, 66.
    short       deformflag;     // 13 type, name 2, 795.
    short       pathflag;       // 14 type, name 2, 1858.
    int         layer_used;     // 15 type, name 4, 2742.
    int         layer;          // 16 type, name 4, 128.
    int         layer_protected;// 17 type, name 4, 2743.
    short       ghostep;        // 18 type, name 2, 2744.
    short       ghostsize;      // 19 type, name 2, 2745.
    short       ghosttype;      // 20 type, name 2, 2746.
    short       pathsize;       // 21 type, name 2, 2747.
    int         ghostsf;        // 22 type, name 4, 2748.
    int         ghostef;        // 23 type, name 4, 2749.
    int         pathsf;         // 24 type, name 4, 2750.
    int         pathef;         // 25 type, name 4, 2751.
    int         pathbc;         // 26 type, name 4, 2752.
    int         pathac;         // 27 type, name 4, 2753.
};

// 507 DNA structure 402, 40
struct bPoseChannel
{
    bPoseChannel    *next;         // 1 type, name 402, 0.
    bPoseChannel    *prev;         // 2 type, name 402, 1.
    IDProperty      *prop;         // 3 type, name 21, 2265.
    ListBase        constraints;   // 4 type, name 14, 1069.
    char            name[64];      // 5 type, name 0, 19.
    short           flag;          // 6 type, name 2, 18.
    short           ikflag;        // 7 type, name 2, 2772.
    short           protectflag;   // 8 type, name 2, 1024.
    short           agrp_index;    // 9 type, name 2, 2773.
    char            constflag;     // 10 type, name 0, 2774.
    char            selectflag;    // 11 type, name 0, 2775.
    char            pad0[6];       // 12 type, name 0, 2776.
    Bone            *bone;         // 13 type, name 398, 2777.
    bPoseChannel    *parent;       // 14 type, name 402, 36.
    bPoseChannel    *child;        // 15 type, name 402, 2778.
    ListBase        iktree;        // 16 type, name 14, 2779.
    ListBase        siktree;       // 17 type, name 14, 2780.
    bMotionPath     *mpath;        // 18 type, name 190, 1000.
    Object          *custom;       // 19 type, name 29, 2781.
    bPoseChannel    *custom_tx;    // 20 type, name 402, 2782.
    float           loc[3];        // 21 type, name 7, 324.
    float           size[3];       // 22 type, name 7, 174.
    float           eul[3];        // 23 type, name 7, 2783.
    float           quat[4];       // 24 type, name 7, 536.
    float           rotAxis[3];    // 25 type, name 7, 1014.
    float           rotAngle;      // 26 type, name 7, 1016.
    short           rotmode;       // 27 type, name 2, 1050.
    short           pad;           // 28 type, name 2, 66.
    float           chan_mat[4][4];// 29 type, name 7, 2784.
    float           pose_mat[4][4];// 30 type, name 7, 2785.
    float           constinv[4][4];// 31 type, name 7, 1019.
    float           pose_head[3];  // 32 type, name 7, 2786.
    float           pose_tail[3];  // 33 type, name 7, 2787.
    float           limitmin[3];   // 34 type, name 7, 2788.
    float           limitmax[3];   // 35 type, name 7, 2789.
    float           stiffness[3];  // 36 type, name 7, 2790.
    float           ikstretch;     // 37 type, name 7, 2791.
    float           ikrotweight;   // 38 type, name 7, 2792.
    float           iklinweight;   // 39 type, name 7, 2793.
    void            *temp;         // 40 type, name 11, 2794.
};

// 508 DNA structure 187, 16
struct bPose
{
    ListBase            chanbase;          // 1 type, name 14, 2795.
    void                *chanhash;         // Note: using void* on undefined DNA type: GHash
    short               flag;              // 3 type, name 2, 18.
    short               pad;               // 4 type, name 2, 66.
    int                 proxy_layer;       // 5 type, name 4, 2797.
    int                 pad1;              // 6 type, name 4, 68.
    float               ctime;             // 7 type, name 7, 84.
    float               stride_offset[3];  // 8 type, name 7, 2798.
    float               cyclic_offset[3];  // 9 type, name 7, 2799.
    ListBase            agroups;           // 10 type, name 14, 2800.
    int                 active_group;      // 11 type, name 4, 2801.
    int                 iksolver;          // 12 type, name 4, 2802.
    void                *ikdata;           // 13 type, name 11, 2803.
    void                *ikparam;          // 14 type, name 11, 2804.
    bAnimVizSettings    avs;               // 15 type, name 189, 999.
    char                proxy_act_bone[64];// 16 type, name 0, 2805.
};

// 509 DNA structure 472, 4
struct NodeImageFile
{
    char               name[1024];// 1 type, name 0, 34.
    ImageFormatData    im_format; // 2 type, name 218, 1424.
    int                sfra;      // 3 type, name 4, 123.
    int                efra;      // 4 type, name 4, 1213.
};

// 510 DNA structure 473, 6
struct NodeImageMultiFile
{
    char               base_path[1024];// 1 type, name 0, 3026.
    ImageFormatData    format;         // 2 type, name 218, 3027.
    int                sfra;           // 3 type, name 4, 123.
    int                efra;           // 4 type, name 4, 1213.
    int                active_input;   // 5 type, name 4, 3028.
    int                pad;            // 6 type, name 4, 66.
};

// 511 DNA structure 474, 7
struct NodeImageMultiFileSocket
{
    short              use_render_format;// 1 type, name 2, 3029.
    short              use_node_format;  // 2 type, name 2, 3030.
    int                pad1;             // 3 type, name 4, 68.
    char               path[1024];       // 4 type, name 0, 1158.
    ImageFormatData    format;           // 5 type, name 218, 3027.
    char               layer[30];        // 6 type, name 0, 3031.
    char               pad2[2];          // 7 type, name 0, 640.
};

// 512 DNA structure 55, 11
struct CurveMapping
{
    int         flag;             // 1 type, name 4, 18.
    int         cur;              // 2 type, name 4, 63.
    int         preset;           // 3 type, name 4, 3126.
    int         changed_timestamp;// 4 type, name 4, 3127.
    rctf        curr;             // 5 type, name 19, 3128.
    rctf        clipr;            // 6 type, name 19, 3129.
    CurveMap    cm[4];            // 7 type, name 515, 3130.
    float       black[3];         // 8 type, name 7, 3131.
    float       white[3];         // 9 type, name 7, 3132.
    float       bwmul[3];         // 10 type, name 7, 3133.
    float       sample[3];        // 11 type, name 7, 3134.
};

// 513 DNA structure 231, 5
struct Palette
{
    ID          id;          // 1 type, name 22, 31.
    ListBase    colors;      // 2 type, name 14, 3206.
    ListBase    deleted;     // 3 type, name 14, 3207.
    int         active_color;// 4 type, name 4, 3208.
    int         pad;         // 5 type, name 4, 66.
};

// 514 DNA structure 574, 25
struct NlaStrip
{
    NlaStrip      *next;          // 1 type, name 574, 0.
    NlaStrip      *prev;          // 2 type, name 574, 1.
    ListBase      strips;         // 3 type, name 14, 3540.
    bAction       *act;           // 4 type, name 186, 2643.
    AnimMapper    *remap;         // 5 type, name 573, 3541.
    ListBase      fcurves;        // 6 type, name 14, 3542.
    ListBase      modifiers;      // 7 type, name 14, 1004.
    char          name[64];       // 8 type, name 0, 19.
    float         influence;      // 9 type, name 7, 2695.
    float         strip_time;     // 10 type, name 7, 3543.
    float         start;          // 11 type, name 7, 737.
    float         end;            // 12 type, name 7, 2570.
    float         actstart;       // 13 type, name 7, 2905.
    float         actend;         // 14 type, name 7, 2906.
    float         repeat;         // 15 type, name 7, 784.
    float         scale;          // 16 type, name 7, 933.
    float         blendin;        // 17 type, name 7, 2645.
    float         blendout;       // 18 type, name 7, 2909.
    short         blendmode;      // 19 type, name 2, 3544.
    short         extendmode;     // 20 type, name 2, 3545.
    short         pad1;           // 21 type, name 2, 68.
    short         type;           // 22 type, name 2, 16.
    void          *speaker_handle;// 23 type, name 11, 3546.
    int           flag;           // 24 type, name 4, 18.
    int           pad2;           // 25 type, name 4, 29.
};

// 515 DNA structure 36, 12
struct AnimData
{
    bAction       *action;       // 1 type, name 186, 995.
    bAction       *tmpact;       // 2 type, name 186, 3554.
    AnimMapper    *remap;        // 3 type, name 573, 3541.
    ListBase      nla_tracks;    // 4 type, name 14, 3555.
    NlaStrip      *actstrip;     // 5 type, name 574, 3556.
    ListBase      drivers;       // 6 type, name 14, 3557.
    ListBase      overrides;     // 7 type, name 14, 3558.
    int           flag;          // 8 type, name 4, 18.
    int           recalc;        // 9 type, name 4, 241.
    short         act_blendmode; // 10 type, name 2, 3559.
    short         act_extendmode;// 11 type, name 2, 3560.
    float         act_influence; // 12 type, name 7, 3561.
};

// 516 DNA structure 586, 14
struct BoidState
{
    BoidState    *next;         // 1 type, name 586, 0.
    BoidState    *prev;         // 2 type, name 586, 1.
    ListBase     rules;         // 3 type, name 14, 3573.
    ListBase     conditions;    // 4 type, name 14, 3574.
    ListBase     actions;       // 5 type, name 14, 3575.
    char         name[32];      // 6 type, name 0, 1532.
    int          id;            // 7 type, name 4, 31.
    int          flag;          // 8 type, name 4, 18.
    int          ruleset_type;  // 9 type, name 4, 3576.
    float        rule_fuzziness;// 10 type, name 7, 3577.
    int          signal_id;     // 11 type, name 4, 3565.
    int          channels;      // 12 type, name 4, 2815.
    float        volume;        // 13 type, name 7, 1165.
    float        falloff;       // 14 type, name 7, 792.
};

// 517 DNA structure 602, 10
struct MovieTrackingObject
{
    MovieTrackingObject            *next;         // 1 type, name 602, 0.
    MovieTrackingObject            *prev;         // 2 type, name 602, 1.
    char                           name[64];      // 3 type, name 0, 19.
    int                            flag;          // 4 type, name 4, 18.
    float                          scale;         // 5 type, name 7, 933.
    ListBase                       tracks;        // 6 type, name 14, 3748.
    ListBase                       plane_tracks;  // 7 type, name 14, 3749.
    MovieTrackingReconstruction    reconstruction;// 8 type, name 601, 3750.
    int                            keyframe1;     // 9 type, name 4, 3730.
    int                            keyframe2;     // 10 type, name 4, 3731.
};

// 518 DNA structure 65, 137
struct Material
{
    ID                id;               // 1 type, name 22, 31.
    AnimData          *adt;             // 2 type, name 36, 75.
    short             material_type;    // 3 type, name 2, 414.
    short             flag;             // 4 type, name 2, 18.
    float             r;                // 5 type, name 7, 183.
    float             g;                // 6 type, name 7, 184.
    float             b;                // 7 type, name 7, 185.
    float             specr;            // 8 type, name 7, 415.
    float             specg;            // 9 type, name 7, 416.
    float             specb;            // 10 type, name 7, 417.
    float             mirr;             // 11 type, name 7, 418.
    float             mirg;             // 12 type, name 7, 419.
    float             mirb;             // 13 type, name 7, 420.
    float             ambr;             // 14 type, name 7, 421.
    float             ambb;             // 15 type, name 7, 422.
    float             ambg;             // 16 type, name 7, 423.
    float             amb;              // 17 type, name 7, 424.
    float             emit;             // 18 type, name 7, 425.
    float             ang;              // 19 type, name 7, 426.
    float             spectra;          // 20 type, name 7, 427.
    float             ray_mirror;       // 21 type, name 7, 428.
    float             alpha;            // 22 type, name 7, 429.
    float             ref;              // 23 type, name 7, 430.
    float             spec;             // 24 type, name 7, 431.
    float             zoffs;            // 25 type, name 7, 432.
    float             add;              // 26 type, name 7, 433.
    float             translucency;     // 27 type, name 7, 434.
    VolumeSettings    vol;              // 28 type, name 62, 435.
    GameSettings      game;             // 29 type, name 63, 436.
    float             fresnel_mir;      // 30 type, name 7, 437.
    float             fresnel_mir_i;    // 31 type, name 7, 438.
    float             fresnel_tra;      // 32 type, name 7, 439.
    float             fresnel_tra_i;    // 33 type, name 7, 440.
    float             filter;           // 34 type, name 7, 441.
    float             tx_limit;         // 35 type, name 7, 442.
    float             tx_falloff;       // 36 type, name 7, 443.
    short             ray_depth;        // 37 type, name 2, 444.
    short             ray_depth_tra;    // 38 type, name 2, 445.
    short             har;              // 39 type, name 2, 446.
    char              seed1;            // 40 type, name 0, 447.
    char              seed2;            // 41 type, name 0, 448.
    float             gloss_mir;        // 42 type, name 7, 449.
    float             gloss_tra;        // 43 type, name 7, 450.
    short             samp_gloss_mir;   // 44 type, name 2, 451.
    short             samp_gloss_tra;   // 45 type, name 2, 452.
    float             adapt_thresh_mir; // 46 type, name 7, 453.
    float             adapt_thresh_tra; // 47 type, name 7, 454.
    float             aniso_gloss_mir;  // 48 type, name 7, 455.
    float             dist_mir;         // 49 type, name 7, 456.
    short             fadeto_mir;       // 50 type, name 2, 457.
    short             shade_flag;       // 51 type, name 2, 458.
    int               mode;             // 52 type, name 4, 334.
    int               mode_l;           // 53 type, name 4, 459.
    int               mode2;            // 54 type, name 4, 460.
    int               mode2_l;          // 55 type, name 4, 461.
    short             flarec;           // 56 type, name 2, 462.
    short             starc;            // 57 type, name 2, 463.
    short             linec;            // 58 type, name 2, 464.
    short             ringc;            // 59 type, name 2, 465.
    float             hasize;           // 60 type, name 7, 466.
    float             flaresize;        // 61 type, name 7, 467.
    float             subsize;          // 62 type, name 7, 468.
    float             flareboost;       // 63 type, name 7, 469.
    float             strand_sta;       // 64 type, name 7, 470.
    float             strand_end;       // 65 type, name 7, 471.
    float             strand_ease;      // 66 type, name 7, 472.
    float             strand_surfnor;   // 67 type, name 7, 473.
    float             strand_min;       // 68 type, name 7, 474.
    float             strand_widthfade; // 69 type, name 7, 475.
    char              strand_uvname[64];// 70 type, name 0, 476.
    float             sbias;            // 71 type, name 7, 477.
    float             lbias;            // 72 type, name 7, 478.
    float             shad_alpha;       // 73 type, name 7, 479.
    int               septex;           // 74 type, name 4, 480.
    char              rgbsel;           // 75 type, name 0, 481.
    char              texact;           // 76 type, name 0, 370.
    char              pr_type;          // 77 type, name 0, 482.
    char              use_nodes;        // 78 type, name 0, 323.
    short             pr_lamp;          // 79 type, name 2, 483.
    short             pr_texture;       // 80 type, name 2, 390.
    short             ml_flag;          // 81 type, name 2, 484.
    char              mapflag;          // 82 type, name 0, 485.
    char              pad;              // 83 type, name 0, 66.
    short             diff_shader;      // 84 type, name 2, 486.
    short             spec_shader;      // 85 type, name 2, 487.
    float             roughness;        // 86 type, name 7, 488.
    float             refrac;           // 87 type, name 7, 489.
    float             param[4];         // 88 type, name 7, 490.
    float             rms;              // 89 type, name 7, 491.
    float             darkness;         // 90 type, name 7, 492.
    short             texco;            // 91 type, name 2, 162.
    short             mapto;            // 92 type, name 2, 163.
    ColorBand         *ramp_col;        // 93 type, name 51, 493.
    ColorBand         *ramp_spec;       // 94 type, name 51, 494.
    char              rampin_col;       // 95 type, name 0, 495.
    char              rampin_spec;      // 96 type, name 0, 496.
    char              rampblend_col;    // 97 type, name 0, 497.
    char              rampblend_spec;   // 98 type, name 0, 498.
    short             ramp_show;        // 99 type, name 2, 499.
    short             pad3;             // 100 type, name 2, 500.
    float             rampfac_col;      // 101 type, name 7, 501.
    float             rampfac_spec;     // 102 type, name 7, 502.
    MTex              *mtex[18];        // 103 type, name 48, 389.
    bNodeTree         *nodetree;        // 104 type, name 58, 318.
    Ipo               *ipo;             // 105 type, name 33, 80.
    Group             *group;           // 106 type, name 66, 503.
    PreviewImage      *preview;         // 107 type, name 26, 147.
    float             friction;         // 108 type, name 7, 504.
    float             fh;               // 109 type, name 7, 505.
    float             reflect;          // 110 type, name 7, 506.
    float             fhdist;           // 111 type, name 7, 507.
    float             xyfrict;          // 112 type, name 7, 508.
    short             dynamode;         // 113 type, name 2, 509.
    short             pad2;             // 114 type, name 2, 29.
    float             sss_radius[3];    // 115 type, name 7, 510.
    float             sss_col[3];       // 116 type, name 7, 511.
    float             sss_error;        // 117 type, name 7, 512.
    float             sss_scale;        // 118 type, name 7, 513.
    float             sss_ior;          // 119 type, name 7, 514.
    float             sss_colfac;       // 120 type, name 7, 515.
    float             sss_texfac;       // 121 type, name 7, 516.
    float             sss_front;        // 122 type, name 7, 517.
    float             sss_back;         // 123 type, name 7, 518.
    short             sss_flag;         // 124 type, name 2, 519.
    short             sss_preset;       // 125 type, name 2, 520.
    int               mapto_textured;   // 126 type, name 4, 521.
    short             shadowonly_flag;  // 127 type, name 2, 522.
    short             index;            // 128 type, name 2, 413.
    float             line_col[4];      // 129 type, name 7, 523.
    short             line_priority;    // 130 type, name 2, 524.
    short             vcol_alpha;       // 131 type, name 2, 525.
    short             paint_active_slot;// 132 type, name 2, 526.
    short             paint_clone_slot; // 133 type, name 2, 527.
    short             tot_slots;        // 134 type, name 2, 528.
    short             pad4[3];          // 135 type, name 2, 529.
    TexPaintSlot      *texpaintslot;    // 136 type, name 64, 530.
    ListBase          gpumaterial;      // 137 type, name 14, 531.
};

// 519 DNA structure 298, 18
struct uiStyle
{
    uiStyle        *next;         // 1 type, name 298, 0.
    uiStyle        *prev;         // 2 type, name 298, 1.
    char           name[64];      // 3 type, name 0, 19.
    uiFontStyle    paneltitle;    // 4 type, name 297, 2003.
    uiFontStyle    grouplabel;    // 5 type, name 297, 2004.
    uiFontStyle    widgetlabel;   // 6 type, name 297, 2005.
    uiFontStyle    widget;        // 7 type, name 297, 2006.
    float          panelzoom;     // 8 type, name 7, 2007.
    short          minlabelchars; // 9 type, name 2, 2008.
    short          minwidgetchars;// 10 type, name 2, 2009.
    short          columnspace;   // 11 type, name 2, 2010.
    short          templatespace; // 12 type, name 2, 2011.
    short          boxspace;      // 13 type, name 2, 2012.
    short          buttonspacex;  // 14 type, name 2, 2013.
    short          buttonspacey;  // 15 type, name 2, 2014.
    short          panelspace;    // 16 type, name 2, 2015.
    short          panelouter;    // 17 type, name 2, 2016.
    short          pad;           // 18 type, name 2, 66.
};

// 520 DNA structure 463, 6
struct NodeColorCorrection
{
    ColorCorrectionData    master;       // 1 type, name 462, 2998.
    ColorCorrectionData    shadows;      // 2 type, name 462, 2999.
    ColorCorrectionData    midtones;     // 3 type, name 462, 3000.
    ColorCorrectionData    highlights;   // 4 type, name 462, 3001.
    float                  startmidtones;// 5 type, name 7, 3002.
    float                  endmidtones;  // 6 type, name 7, 3003.
};

// 521 DNA structure 489, 2
struct NodeTexBase
{
    TexMapping      tex_mapping;  // 1 type, name 59, 3076.
    ColorMapping    color_mapping;// 2 type, name 60, 3077.
};

// 522 DNA structure 230, 57
struct Brush
{
    ID              id;                       // 1 type, name 22, 31.
    BrushClone      clone;                    // 2 type, name 516, 3163.
    CurveMapping    *curve;                   // 3 type, name 55, 3121.
    MTex            mtex;                     // 4 type, name 48, 3164.
    MTex            mask_mtex;                // 5 type, name 48, 3165.
    Brush           *toggle_brush;            // 6 type, name 230, 3166.
    void            *icon_imbuf;              // Note: using void* on undefined DNA type: ImBuf
    PreviewImage    *preview;                 // 8 type, name 26, 147.
    ColorBand       *gradient;                // 9 type, name 51, 3168.
    PaintCurve      *paint_curve;             // 10 type, name 517, 3169.
    char            icon_filepath[1024];      // 11 type, name 0, 3170.
    float           normal_weight;            // 12 type, name 7, 3171.
    short           blend;                    // 13 type, name 2, 1754.
    short           ob_mode;                  // 14 type, name 2, 3172.
    float           weight;                   // 15 type, name 7, 557.
    int             size;                     // 16 type, name 4, 102.
    int             flag;                     // 17 type, name 4, 18.
    int             mask_pressure;            // 18 type, name 4, 3173.
    float           jitter;                   // 19 type, name 7, 3174.
    int             jitter_absolute;          // 20 type, name 4, 3175.
    int             overlay_flags;            // 21 type, name 4, 3176.
    int             spacing;                  // 22 type, name 4, 611.
    int             smooth_stroke_radius;     // 23 type, name 4, 3177.
    float           smooth_stroke_factor;     // 24 type, name 7, 3178.
    float           rate;                     // 25 type, name 7, 3179.
    float           rgb[3];                   // 26 type, name 7, 1608.
    float           alpha;                    // 27 type, name 7, 429.
    float           secondary_rgb[3];         // 28 type, name 7, 1609.
    int             sculpt_plane;             // 29 type, name 4, 3180.
    float           plane_offset;             // 30 type, name 7, 3181.
    float           pad;                      // 31 type, name 7, 66.
    int             gradient_spacing;         // 32 type, name 4, 3182.
    int             gradient_stroke_mode;     // 33 type, name 4, 3183.
    int             gradient_fill_mode;       // 34 type, name 4, 3184.
    char            sculpt_tool;              // 35 type, name 0, 3185.
    char            vertexpaint_tool;         // 36 type, name 0, 3186.
    char            imagepaint_tool;          // 37 type, name 0, 3187.
    char            mask_tool;                // 38 type, name 0, 3188.
    float           autosmooth_factor;        // 39 type, name 7, 3189.
    float           crease_pinch_factor;      // 40 type, name 7, 3190.
    float           plane_trim;               // 41 type, name 7, 3191.
    float           height;                   // 42 type, name 7, 788.
    float           texture_sample_bias;      // 43 type, name 7, 3192.
    int             texture_overlay_alpha;    // 44 type, name 4, 3193.
    int             mask_overlay_alpha;       // 45 type, name 4, 3194.
    int             cursor_overlay_alpha;     // 46 type, name 4, 3195.
    float           unprojected_radius;       // 47 type, name 7, 1607.
    float           sharp_threshold;          // 48 type, name 7, 3196.
    int             blur_kernel_radius;       // 49 type, name 4, 3197.
    int             blur_mode;                // 50 type, name 4, 3198.
    float           fill_threshold;           // 51 type, name 7, 3199.
    float           add_col[3];               // 52 type, name 7, 3200.
    float           sub_col[3];               // 53 type, name 7, 3201.
    float           stencil_pos[2];           // 54 type, name 7, 3202.
    float           stencil_dimension[2];     // 55 type, name 7, 3203.
    float           mask_stencil_pos[2];      // 56 type, name 7, 3204.
    float           mask_stencil_dimension[2];// 57 type, name 7, 3205.
};

// 523 DNA structure 152, 51
struct ParticleSystem
{
    ParticleSystem        *next;               // 1 type, name 152, 0.
    ParticleSystem        *prev;               // 2 type, name 152, 1.
    ParticleSettings      *part;               // 3 type, name 532, 3349.
    ParticleData          *particles;          // 4 type, name 530, 3350.
    ChildParticle         *child;              // 5 type, name 527, 2778.
    void                  *edit;               // Note: using void* on undefined DNA type: PTCacheEdit
    void                  (*free_edit)();      // 7 type, name 11, 1162.
    void                  **pathcache;         // Note: using void* on undefined DNA type: ParticleCacheKey
    void                  **childcache;        // Note: using void* on undefined DNA type: ParticleCacheKey
    ListBase              pathcachebufs;       // 10 type, name 14, 3353.
    ListBase              childcachebufs;      // 11 type, name 14, 3354.
    ClothModifierData     *clmd;               // 12 type, name 137, 3355.
    void                  *hair_in_dm;         // Note: using void* on undefined DNA type: DerivedMesh
    void                  *hair_out_dm;        // Note: using void* on undefined DNA type: DerivedMesh
    Object                *target_ob;          // 15 type, name 29, 3358.
    void                  *lattice_deform_data;// Note: using void* on undefined DNA type: LatticeDeformData
    Object                *parent;             // 17 type, name 29, 36.
    ListBase              targets;             // 18 type, name 14, 2841.
    char                  name[64];            // 19 type, name 0, 19.
    float                 imat[4][4];          // 20 type, name 7, 234.
    float                 cfra;                // 21 type, name 7, 823.
    float                 tree_frame;          // 22 type, name 7, 3360.
    float                 bvhtree_frame;       // 23 type, name 7, 3361.
    int                   seed;                // 24 type, name 4, 740.
    int                   child_seed;          // 25 type, name 4, 3362.
    int                   flag;                // 26 type, name 4, 18.
    int                   totpart;             // 27 type, name 4, 2571.
    int                   totunexist;          // 28 type, name 4, 3363.
    int                   totchild;            // 29 type, name 4, 3364.
    int                   totcached;           // 30 type, name 4, 3365.
    int                   totchildcache;       // 31 type, name 4, 3366.
    short                 recalc;              // 32 type, name 2, 241.
    short                 target_psys;         // 33 type, name 2, 3367.
    short                 totkeyed;            // 34 type, name 2, 3368.
    short                 bakespace;           // 35 type, name 2, 3369.
    char                  bb_uvname[3][64];    // 36 type, name 0, 3370.
    short                 vgroup[12];          // 37 type, name 2, 3371.
    short                 vg_neg;              // 38 type, name 2, 3372.
    short                 rt3;                 // 39 type, name 2, 3373.
    void                  *renderdata;         // Note: using void* on undefined DNA type: ParticleRenderData
    PointCache            *pointcache;         // 41 type, name 141, 1234.
    ListBase              ptcaches;            // 42 type, name 14, 808.
    ListBase              *effectors;          // 43 type, name 14, 3375.
    ParticleSpring        *fluid_springs;      // 44 type, name 526, 3376.
    int                   tot_fluidsprings;    // 45 type, name 4, 3377.
    int                   alloc_fluidsprings;  // 46 type, name 4, 3378.
    void                  *tree;               // Note: using void* on undefined DNA type: KDTree
    void                  *bvhtree;            // Note: using void* on undefined DNA type: BVHTree
    void                  *pdd;                // Note: using void* on undefined DNA type: ParticleDrawData
    float                 dt_frac;             // 50 type, name 7, 3381.
    float                 _pad;                // 51 type, name 7, 271.
};

// 524 DNA structure 260, 26
struct View2D
{
    rctf                 tot;          // 1 type, name 19, 226.
    rctf                 cur;          // 2 type, name 19, 63.
    rcti                 vert;         // 3 type, name 18, 1828.
    rcti                 hor;          // 4 type, name 18, 1829.
    rcti                 mask;         // 5 type, name 18, 1830.
    float                min[2];       // 6 type, name 7, 1831.
    float                max[2];       // 7 type, name 7, 1832.
    float                minzoom;      // 8 type, name 7, 1833.
    float                maxzoom;      // 9 type, name 7, 1834.
    short                scroll;       // 10 type, name 2, 1835.
    short                scroll_ui;    // 11 type, name 2, 1836.
    short                keeptot;      // 12 type, name 2, 1837.
    short                keepzoom;     // 13 type, name 2, 1838.
    short                keepofs;      // 14 type, name 2, 1839.
    short                flag;         // 15 type, name 2, 18.
    short                align;        // 16 type, name 2, 1840.
    short                winx;         // 17 type, name 2, 1841.
    short                winy;         // 18 type, name 2, 1842.
    short                oldwinx;      // 19 type, name 2, 1843.
    short                oldwiny;      // 20 type, name 2, 1844.
    short                around;       // 21 type, name 2, 1808.
    float                *tab_offset;  // 22 type, name 7, 1845.
    int                  tab_num;      // 23 type, name 4, 1846.
    int                  tab_cur;      // 24 type, name 4, 1847.
    void                 *sms;         // Note: using void* on undefined DNA type: SmoothView2DStore
    void                 *smooth_timer;// Note: using void* on undefined DNA type: wmTimer
};

// 525 DNA structure 294, 29
struct SpaceClip
{
    SpaceLink          *next;           // 1 type, name 259, 0.
    SpaceLink          *prev;           // 2 type, name 259, 1.
    ListBase           regionbase;      // 3 type, name 14, 1792.
    int                spacetype;       // 4 type, name 4, 1793.
    float              xof;             // 5 type, name 7, 618.
    float              yof;             // 6 type, name 7, 619.
    float              xlockof;         // 7 type, name 7, 1981.
    float              ylockof;         // 8 type, name 7, 1982.
    float              zoom;            // 9 type, name 7, 1880.
    MovieClipUser      user;            // 10 type, name 252, 1983.
    MovieClip          *clip;           // 11 type, name 249, 1749.
    MovieClipScopes    scopes;          // 12 type, name 295, 1883.
    int                flag;            // 13 type, name 4, 18.
    short              mode;            // 14 type, name 2, 334.
    short              view;            // 15 type, name 2, 1755.
    int                path_length;     // 16 type, name 4, 1984.
    float              loc[2];          // 17 type, name 7, 1985.
    float              scale;           // 18 type, name 7, 933.
    float              angle;           // 19 type, name 7, 781.
    int                pad;             // 20 type, name 4, 66.
    float              stabmat[4][4];   // 21 type, name 7, 1986.
    float              unistabmat[4][4];// 22 type, name 7, 1987.
    int                postproc_flag;   // 23 type, name 4, 1988.
    short              gpencil_src;     // 24 type, name 2, 1651.
    short              pad2;            // 25 type, name 2, 29.
    int                around;          // 26 type, name 4, 1808.
    int                pad4;            // 27 type, name 4, 1468.
    float              cursor[2];       // 28 type, name 7, 1914.
    MaskSpaceInfo      mask_info;       // 29 type, name 274, 1923.
};

// 526 DNA structure 312, 23
struct bScreen
{
    ID          id;                 // 1 type, name 22, 31.
    ListBase    vertbase;           // 2 type, name 14, 2383.
    ListBase    edgebase;           // 3 type, name 14, 2384.
    ListBase    areabase;           // 4 type, name 14, 2385.
    ListBase    regionbase;         // 5 type, name 14, 1792.
    Scene       *scene;             // 6 type, name 41, 119.
    Scene       *newscene;          // 7 type, name 41, 2386.
    int         redraws_flag;       // 8 type, name 4, 2387.
    int         pad1;               // 9 type, name 4, 68.
    short       state;              // 10 type, name 2, 1083.
    short       temp;               // 11 type, name 2, 2388.
    short       winid;              // 12 type, name 2, 2389.
    short       do_draw;            // 13 type, name 2, 2390.
    short       do_refresh;         // 14 type, name 2, 2391.
    short       do_draw_gesture;    // 15 type, name 2, 2392.
    short       do_draw_paintcursor;// 16 type, name 2, 2393.
    short       do_draw_drag;       // 17 type, name 2, 2394.
    short       swap;               // 18 type, name 2, 2395.
    short       mainwin;            // 19 type, name 2, 2396.
    short       subwinactive;       // 20 type, name 2, 2397.
    short       pad;                // 21 type, name 2, 66.
    void        *animtimer;         // Note: using void* on undefined DNA type: wmTimer
    void        *context;           // 23 type, name 11, 2399.
};

// 527 DNA structure 323, 23
struct ScrArea
{
    ScrArea      *next;            // 1 type, name 323, 0.
    ScrArea      *prev;            // 2 type, name 323, 1.
    ScrVert      *v1;              // 3 type, name 313, 2402.
    ScrVert      *v2;              // 4 type, name 313, 2403.
    ScrVert      *v3;              // 5 type, name 313, 2432.
    ScrVert      *v4;              // 6 type, name 313, 2433.
    bScreen      *full;            // 7 type, name 312, 2434.
    rcti         totrct;           // 8 type, name 18, 51.
    char         spacetype;        // 9 type, name 0, 1793.
    char         butspacetype;     // 10 type, name 0, 2435.
    short        winx;             // 11 type, name 2, 1841.
    short        winy;             // 12 type, name 2, 1842.
    short        headertype;       // 13 type, name 2, 2436.
    short        do_refresh;       // 14 type, name 2, 2391.
    short        flag;             // 15 type, name 2, 18.
    short        region_active_win;// 16 type, name 2, 2437.
    char         temp;             // 17 type, name 0, 2388.
    char         pad;              // 18 type, name 0, 66.
    void         *type;            // Note: using void* on undefined DNA type: SpaceType
    ListBase     spacedata;        // 20 type, name 14, 2438.
    ListBase     regionbase;       // 21 type, name 14, 1792.
    ListBase     handlers;         // 22 type, name 14, 2439.
    ListBase     actionzones;      // 23 type, name 14, 2440.
};

// 528 DNA structure 345, 2
struct CurvesModifierData
{
    SequenceModifierData    modifier;     // 1 type, name 343, 724.
    CurveMapping            curve_mapping;// 2 type, name 55, 2560.
};

// 529 DNA structure 346, 2
struct HueCorrectModifierData
{
    SequenceModifierData    modifier;     // 1 type, name 343, 724.
    CurveMapping            curve_mapping;// 2 type, name 55, 2560.
};

// 530 DNA structure 186, 9
struct bAction
{
    ID          id;           // 1 type, name 22, 31.
    ListBase    curves;       // 2 type, name 14, 2818.
    ListBase    chanbase;     // 3 type, name 14, 2795.
    ListBase    groups;       // 4 type, name 14, 2819.
    ListBase    markers;      // 5 type, name 14, 1735.
    int         flag;         // 6 type, name 4, 18.
    int         active_marker;// 7 type, name 4, 2820.
    int         idroot;       // 8 type, name 4, 2821.
    int         pad;          // 9 type, name 4, 66.
};

// 531 DNA structure 490, 5
struct NodeTexSky
{
    NodeTexBase    base;            // 1 type, name 489, 1722.
    int            sky_model;       // 2 type, name 4, 3078.
    float          sun_direction[3];// 3 type, name 7, 3079.
    float          turbidity;       // 4 type, name 7, 3080.
    float          ground_albedo;   // 5 type, name 7, 3081.
};

// 532 DNA structure 492, 1
struct NodeTexChecker
{
    NodeTexBase    base;// 1 type, name 489, 1722.
};

// 533 DNA structure 493, 5
struct NodeTexBrick
{
    NodeTexBase    base;       // 1 type, name 489, 1722.
    int            offset_freq;// 2 type, name 4, 3085.
    int            squash_freq;// 3 type, name 4, 3086.
    float          offset;     // 4 type, name 7, 122.
    float          squash;     // 5 type, name 7, 3087.
};

// 534 DNA structure 495, 3
struct NodeTexGradient
{
    NodeTexBase    base;         // 1 type, name 489, 1722.
    int            gradient_type;// 2 type, name 4, 3088.
    int            pad;          // 3 type, name 4, 66.
};

// 535 DNA structure 496, 1
struct NodeTexNoise
{
    NodeTexBase    base;// 1 type, name 489, 1722.
};

// 536 DNA structure 497, 3
struct NodeTexVoronoi
{
    NodeTexBase    base;    // 1 type, name 489, 1722.
    int            coloring;// 2 type, name 4, 3089.
    int            pad;     // 3 type, name 4, 66.
};

// 537 DNA structure 498, 3
struct NodeTexMusgrave
{
    NodeTexBase    base;         // 1 type, name 489, 1722.
    int            musgrave_type;// 2 type, name 4, 3090.
    int            pad;          // 3 type, name 4, 66.
};

// 538 DNA structure 499, 3
struct NodeTexWave
{
    NodeTexBase    base;     // 1 type, name 489, 1722.
    int            wave_type;// 2 type, name 4, 3091.
    int            pad;      // 3 type, name 4, 66.
};

// 539 DNA structure 500, 3
struct NodeTexMagic
{
    NodeTexBase    base; // 1 type, name 489, 1722.
    int            depth;// 2 type, name 4, 240.
    int            pad;  // 3 type, name 4, 66.
};

// 540 DNA structure 546, 33
struct wmWindow
{
    wmWindow       *next;         // 1 type, name 546, 0.
    wmWindow       *prev;         // 2 type, name 546, 1.
    void           *ghostwin;     // 3 type, name 11, 3460.
    bScreen        *screen;       // 4 type, name 312, 3461.
    bScreen        *newscreen;    // 5 type, name 312, 3462.
    char           screenname[64];// 6 type, name 0, 3463.
    short          posx;          // 7 type, name 2, 3464.
    short          posy;          // 8 type, name 2, 3465.
    short          sizex;         // 9 type, name 2, 2410.
    short          sizey;         // 10 type, name 2, 2411.
    short          windowstate;   // 11 type, name 2, 3466.
    short          monitor;       // 12 type, name 2, 3467.
    short          active;        // 13 type, name 2, 1095.
    short          cursor;        // 14 type, name 2, 1972.
    short          lastcursor;    // 15 type, name 2, 3468.
    short          modalcursor;   // 16 type, name 2, 3469.
    short          grabcursor;    // 17 type, name 2, 3470.
    short          addmousemove;  // 18 type, name 2, 3471.
    int            winid;         // 19 type, name 4, 2389.
    short          lock_pie_event;// 20 type, name 2, 3472.
    short          last_pie_event;// 21 type, name 2, 3473.
    void           *eventstate;   // Note: using void* on undefined DNA type: wmEvent
    void           *curswin;      // Note: using void* on undefined DNA type: wmSubWindow
    void           *tweak;        // Note: using void* on undefined DNA type: wmGesture
    void           *ime_data;     // Note: using void* on undefined DNA type: wmIMEData
    int            drawmethod;    // 26 type, name 4, 3478.
    int            drawfail;      // 27 type, name 4, 3479.
    void           *drawdata;     // 28 type, name 11, 3480.
    ListBase       queue;         // 29 type, name 14, 3447.
    ListBase       handlers;      // 30 type, name 14, 2439.
    ListBase       modalhandlers; // 31 type, name 14, 3481.
    ListBase       subwindows;    // 32 type, name 14, 3482.
    ListBase       gesture;       // 33 type, name 14, 3483.
};

// 541 DNA structure 616, 45
struct FreestyleLineStyle
{
    ID           id;                 // 1 type, name 22, 31.
    AnimData     *adt;               // 2 type, name 36, 75.
    float        r;                  // 3 type, name 7, 183.
    float        g;                  // 4 type, name 7, 184.
    float        b;                  // 5 type, name 7, 185.
    float        alpha;              // 6 type, name 7, 429.
    float        thickness;          // 7 type, name 7, 3420.
    int          thickness_position; // 8 type, name 4, 3923.
    float        thickness_ratio;    // 9 type, name 7, 3924.
    int          flag;               // 10 type, name 4, 18.
    int          caps;               // 11 type, name 4, 3925.
    int          chaining;           // 12 type, name 4, 3926.
    int          rounds;             // 13 type, name 4, 3910.
    float        split_length;       // 14 type, name 7, 3927.
    float        min_angle;          // 15 type, name 7, 3928.
    float        max_angle;          // 16 type, name 7, 3929.
    float        min_length;         // 17 type, name 7, 3930.
    float        max_length;         // 18 type, name 7, 3931.
    int          chain_count;        // 19 type, name 4, 3932.
    short        split_dash1;        // 20 type, name 2, 3933.
    short        split_gap1;         // 21 type, name 2, 3934.
    short        split_dash2;        // 22 type, name 2, 3935.
    short        split_gap2;         // 23 type, name 2, 3936.
    short        split_dash3;        // 24 type, name 2, 3937.
    short        split_gap3;         // 25 type, name 2, 3938.
    int          sort_key;           // 26 type, name 4, 3939.
    int          integration_type;   // 27 type, name 4, 3940.
    float        texstep;            // 28 type, name 7, 3941.
    short        texact;             // 29 type, name 2, 370.
    short        pr_texture;         // 30 type, name 2, 390.
    short        use_nodes;          // 31 type, name 2, 323.
    short        pad[3];             // 32 type, name 2, 333.
    short        dash1;              // 33 type, name 2, 3942.
    short        gap1;               // 34 type, name 2, 3943.
    short        dash2;              // 35 type, name 2, 3944.
    short        gap2;               // 36 type, name 2, 3945.
    short        dash3;              // 37 type, name 2, 3946.
    short        gap3;               // 38 type, name 2, 3947.
    int          panel;              // 39 type, name 4, 2059.
    MTex         *mtex[18];          // 40 type, name 48, 389.
    bNodeTree    *nodetree;          // 41 type, name 58, 318.
    ListBase     color_modifiers;    // 42 type, name 14, 3948.
    ListBase     alpha_modifiers;    // 43 type, name 14, 3949.
    ListBase     thickness_modifiers;// 44 type, name 14, 3950.
    ListBase     geometry_modifiers; // 45 type, name 14, 3951.
};

// 542 DNA structure 79, 49
struct Mesh
{
    ID             id;          // 1 type, name 22, 31.
    AnimData       *adt;        // 2 type, name 36, 75.
    BoundBox       *bb;         // 3 type, name 70, 533.
    Ipo            *ipo;        // 4 type, name 33, 80.
    Key            *key;        // 5 type, name 35, 596.
    Material       **mat;       // 6 type, name 65, 548.
    MSelect        *mselect;    // 7 type, name 80, 641.
    MPoly          *mpoly;      // 8 type, name 81, 642.
    MTexPoly       *mtpoly;     // 9 type, name 82, 643.
    MLoop          *mloop;      // 10 type, name 83, 644.
    MLoopUV        *mloopuv;    // 11 type, name 84, 645.
    MLoopCol       *mloopcol;   // 12 type, name 85, 646.
    MFace          *mface;      // 13 type, name 86, 647.
    MTFace         *mtface;     // 14 type, name 87, 648.
    TFace          *tface;      // 15 type, name 88, 649.
    MVert          *mvert;      // 16 type, name 89, 650.
    MEdge          *medge;      // 17 type, name 90, 651.
    MDeformVert    *dvert;      // 18 type, name 91, 652.
    MCol           *mcol;       // 19 type, name 92, 653.
    Mesh           *texcomesh;  // 20 type, name 79, 654.
    void           *edit_btmesh;// Note: using void* on undefined DNA type: BMEditMesh
    CustomData     vdata;       // 22 type, name 94, 656.
    CustomData     edata;       // 23 type, name 94, 657.
    CustomData     fdata;       // 24 type, name 94, 658.
    CustomData     pdata;       // 25 type, name 94, 659.
    CustomData     ldata;       // 26 type, name 94, 660.
    int            totvert;     // 27 type, name 4, 53.
    int            totedge;     // 28 type, name 4, 661.
    int            totface;     // 29 type, name 4, 662.
    int            totselect;   // 30 type, name 4, 663.
    int            totpoly;     // 31 type, name 4, 664.
    int            totloop;     // 32 type, name 4, 665.
    int            act_face;    // 33 type, name 4, 666.
    float          loc[3];      // 34 type, name 7, 324.
    float          size[3];     // 35 type, name 7, 174.
    float          rot[3];      // 36 type, name 7, 325.
    int            drawflag;    // 37 type, name 4, 597.
    short          texflag;     // 38 type, name 2, 176.
    short          flag;        // 39 type, name 2, 18.
    float          smoothresh;  // 40 type, name 7, 667.
    int            pad2;        // 41 type, name 4, 29.
    char           cd_flag;     // 42 type, name 0, 668.
    char           pad;         // 43 type, name 0, 66.
    char           subdiv;      // 44 type, name 0, 669.
    char           subdivr;     // 45 type, name 0, 670.
    char           subsurftype; // 46 type, name 0, 671.
    char           editflag;    // 47 type, name 0, 672.
    short          totcol;      // 48 type, name 2, 550.
    Multires       *mr;         // 49 type, name 95, 673.
};

// 543 DNA structure 242, 76
struct ToolSettings
{
    VPaint                  *vpaint;                                // 1 type, name 237, 1637.
    VPaint                  *wpaint;                                // 2 type, name 237, 1638.
    Sculpt                  *sculpt;                                // 3 type, name 235, 985.
    UvSculpt                *uvsculpt;                              // 4 type, name 236, 1639.
    float                   vgroup_weight;                          // 5 type, name 7, 1640.
    float                   doublimit;                              // 6 type, name 7, 1641.
    float                   normalsize;                             // 7 type, name 7, 1642.
    short                   automerge;                              // 8 type, name 2, 1643.
    short                   selectmode;                             // 9 type, name 2, 1593.
    char                    unwrapper;                              // 10 type, name 0, 1644.
    char                    uvcalc_flag;                            // 11 type, name 0, 1645.
    char                    uv_flag;                                // 12 type, name 0, 1646.
    char                    uv_selectmode;                          // 13 type, name 0, 1647.
    float                   uvcalc_margin;                          // 14 type, name 7, 1648.
    short                   autoik_chainlen;                        // 15 type, name 2, 1649.
    char                    gpencil_flags;                          // 16 type, name 0, 1650.
    char                    gpencil_src;                            // 17 type, name 0, 1651.
    char                    pad[4];                                 // 18 type, name 0, 568.
    ImagePaintSettings      imapaint;                               // 19 type, name 232, 1652.
    ParticleEditSettings    particle;                               // 20 type, name 234, 1653.
    float                   proportional_size;                      // 21 type, name 7, 1654.
    float                   select_thresh;                          // 22 type, name 7, 1655.
    short                   autokey_mode;                           // 23 type, name 2, 1656.
    short                   autokey_flag;                           // 24 type, name 2, 1657.
    char                    multires_subdiv_type;                   // 25 type, name 0, 1658.
    char                    pad3[1];                                // 26 type, name 0, 1659.
    short                   skgen_resolution;                       // 27 type, name 2, 1660.
    float                   skgen_threshold_internal;               // 28 type, name 7, 1661.
    float                   skgen_threshold_external;               // 29 type, name 7, 1662.
    float                   skgen_length_ratio;                     // 30 type, name 7, 1663.
    float                   skgen_length_limit;                     // 31 type, name 7, 1664.
    float                   skgen_angle_limit;                      // 32 type, name 7, 1665.
    float                   skgen_correlation_limit;                // 33 type, name 7, 1666.
    float                   skgen_symmetry_limit;                   // 34 type, name 7, 1667.
    float                   skgen_retarget_angle_weight;            // 35 type, name 7, 1668.
    float                   skgen_retarget_length_weight;           // 36 type, name 7, 1669.
    float                   skgen_retarget_distance_weight;         // 37 type, name 7, 1670.
    short                   skgen_options;                          // 38 type, name 2, 1671.
    char                    skgen_postpro;                          // 39 type, name 0, 1672.
    char                    skgen_postpro_passes;                   // 40 type, name 0, 1673.
    char                    skgen_subdivisions[3];                  // 41 type, name 0, 1674.
    char                    skgen_multi_level;                      // 42 type, name 0, 1675.
    Object                  *skgen_template;                        // 43 type, name 29, 1676.
    char                    bone_sketching;                         // 44 type, name 0, 1677.
    char                    bone_sketching_convert;                 // 45 type, name 0, 1678.
    char                    skgen_subdivision_number;               // 46 type, name 0, 1679.
    char                    skgen_retarget_options;                 // 47 type, name 0, 1680.
    char                    skgen_retarget_roll;                    // 48 type, name 0, 1681.
    char                    skgen_side_string[8];                   // 49 type, name 0, 1682.
    char                    skgen_num_string[8];                    // 50 type, name 0, 1683.
    char                    edge_mode;                              // 51 type, name 0, 1684.
    char                    edge_mode_live_unwrap;                  // 52 type, name 0, 1685.
    char                    snap_mode;                              // 53 type, name 0, 1686.
    char                    snap_node_mode;                         // 54 type, name 0, 1687.
    char                    snap_uv_mode;                           // 55 type, name 0, 1688.
    short                   snap_flag;                              // 56 type, name 2, 1689.
    short                   snap_target;                            // 57 type, name 2, 1690.
    short                   proportional;                           // 58 type, name 2, 1691.
    short                   prop_mode;                              // 59 type, name 2, 1692.
    char                    proportional_objects;                   // 60 type, name 0, 1693.
    char                    proportional_mask;                      // 61 type, name 0, 1694.
    char                    auto_normalize;                         // 62 type, name 0, 1695.
    char                    multipaint;                             // 63 type, name 0, 1696.
    char                    weightuser;                             // 64 type, name 0, 1697.
    char                    vgroupsubset;                           // 65 type, name 0, 1698.
    int                     use_uv_sculpt;                          // 66 type, name 4, 1699.
    int                     uv_sculpt_settings;                     // 67 type, name 4, 1700.
    int                     uv_sculpt_tool;                         // 68 type, name 4, 1701.
    int                     uv_relax_method;                        // 69 type, name 4, 1702.
    short                   sculpt_paint_settings;                  // 70 type, name 2, 1703.
    short                   pad5;                                   // 71 type, name 2, 352.
    int                     sculpt_paint_unified_size;              // 72 type, name 4, 1704.
    float                   sculpt_paint_unified_unprojected_radius;// 73 type, name 7, 1705.
    float                   sculpt_paint_unified_alpha;             // 74 type, name 7, 1706.
    UnifiedPaintSettings    unified_paint_settings;                 // 75 type, name 239, 1707.
    MeshStatVis             statvis;                                // 76 type, name 241, 1708.
};

// 544 DNA structure 258, 54
struct View3D
{
    SpaceLink    *next;               // 1 type, name 259, 0.
    SpaceLink    *prev;               // 2 type, name 259, 1.
    ListBase     regionbase;          // 3 type, name 14, 1792.
    int          spacetype;           // 4 type, name 4, 1793.
    float        blockscale;          // 5 type, name 7, 1794.
    short        blockhandler[8];     // 6 type, name 2, 1795.
    float        viewquat[4];         // 7 type, name 7, 1773.
    float        dist;                // 8 type, name 7, 341.
    float        bundle_size;         // 9 type, name 7, 1796.
    char         bundle_drawtype;     // 10 type, name 0, 1797.
    char         pad[3];              // 11 type, name 0, 333.
    int          lay_prev;            // 12 type, name 4, 1798.
    int          lay_used;            // 13 type, name 4, 1799.
    short        persp;               // 14 type, name 2, 1779.
    short        view;                // 15 type, name 2, 1755.
    Object       *camera;             // 16 type, name 29, 1572.
    Object       *ob_centre;          // 17 type, name 29, 1800.
    rctf         render_border;       // 18 type, name 19, 1801.
    ListBase     bgpicbase;           // 19 type, name 14, 1802.
    BGpic        *bgpic;              // 20 type, name 251, 1803.
    View3D       *localvd;            // 21 type, name 258, 1766.
    char         ob_centre_bone[64];  // 22 type, name 0, 1804.
    int          lay;                 // 23 type, name 4, 1021.
    int          layact;              // 24 type, name 4, 1729.
    short        drawtype;            // 25 type, name 2, 1805.
    short        ob_centre_cursor;    // 26 type, name 2, 1806.
    short        scenelock;           // 27 type, name 2, 1807.
    short        around;              // 28 type, name 2, 1808.
    short        flag;                // 29 type, name 2, 18.
    short        flag2;               // 30 type, name 2, 549.
    float        lens;                // 31 type, name 7, 108.
    float        grid;                // 32 type, name 7, 1809.
    float        near;                // 33 type, name 7, 1810.
    float        far;                 // 34 type, name 7, 1811.
    float        ofs[3];              // 35 type, name 7, 173.
    float        cursor[3];           // 36 type, name 7, 1725.
    short        matcap_icon;         // 37 type, name 2, 1812.
    short        gridlines;           // 38 type, name 2, 1813.
    short        gridsubdiv;          // 39 type, name 2, 1814.
    char         gridflag;            // 40 type, name 0, 1815.
    char         twtype;              // 41 type, name 0, 1816.
    char         twmode;              // 42 type, name 0, 1817.
    char         twflag;              // 43 type, name 0, 1818.
    short        flag3;               // 44 type, name 2, 1819.
    ListBase     afterdraw_transp;    // 45 type, name 14, 1820.
    ListBase     afterdraw_xray;      // 46 type, name 14, 1821.
    ListBase     afterdraw_xraytransp;// 47 type, name 14, 1822.
    char         zbuf;                // 48 type, name 0, 1823.
    char         transp;              // 49 type, name 0, 677.
    char         xray;                // 50 type, name 0, 1824.
    char         pad3[5];             // 51 type, name 0, 1825.
    void         *properties_storage; // 52 type, name 11, 1826.
    Material     *defmaterial;        // 53 type, name 65, 1827.
    bGPdata      *gpd;                // 54 type, name 188, 998.
};

// 545 DNA structure 281, 27
struct SpaceImage
{
    SpaceLink        *next;           // 1 type, name 259, 0.
    SpaceLink        *prev;           // 2 type, name 259, 1.
    ListBase         regionbase;      // 3 type, name 14, 1792.
    int              spacetype;       // 4 type, name 4, 1793.
    int              flag;            // 5 type, name 4, 18.
    Image            *image;          // 6 type, name 43, 772.
    ImageUser        iuser;           // 7 type, name 40, 317.
    CurveMapping     *cumap;          // 8 type, name 55, 1912.
    Scopes           scopes;          // 9 type, name 282, 1883.
    Histogram        sample_line_hist;// 10 type, name 283, 1913.
    bGPdata          *gpd;            // 11 type, name 188, 998.
    float            cursor[2];       // 12 type, name 7, 1914.
    float            xof;             // 13 type, name 7, 618.
    float            yof;             // 14 type, name 7, 619.
    float            zoom;            // 15 type, name 7, 1880.
    float            centx;           // 16 type, name 7, 1915.
    float            centy;           // 17 type, name 7, 1916.
    char             mode;            // 18 type, name 0, 334.
    char             pin;             // 19 type, name 0, 1917.
    short            pad;             // 20 type, name 2, 66.
    short            curtile;         // 21 type, name 2, 1918.
    short            lock;            // 22 type, name 2, 1919.
    char             dt_uv;           // 23 type, name 0, 1920.
    char             sticky;          // 24 type, name 0, 1921.
    char             dt_uvstretch;    // 25 type, name 0, 1922.
    char             around;          // 26 type, name 0, 1808.
    MaskSpaceInfo    mask_info;       // 27 type, name 274, 1923.
};

// 546 DNA structure 445, 47
struct bNode
{
    bNode         *next;         // 1 type, name 445, 0.
    bNode         *prev;         // 2 type, name 445, 1.
    bNode         *new_node;     // 3 type, name 445, 2934.
    IDProperty    *prop;         // 4 type, name 21, 2265.
    void          *typeinfo;     // Note: using void* on undefined DNA type: bNodeType
    char          idname[64];    // 6 type, name 0, 2419.
    char          name[64];      // 7 type, name 0, 19.
    int           flag;          // 8 type, name 4, 18.
    short         type;          // 9 type, name 2, 16.
    short         pad;           // 10 type, name 2, 66.
    short         done;          // 11 type, name 2, 2484.
    short         level;         // 12 type, name 2, 701.
    short         lasty;         // 13 type, name 2, 2935.
    short         menunr;        // 14 type, name 2, 1927.
    short         stack_index;   // 15 type, name 2, 2927.
    short         nr;            // 16 type, name 2, 2597.
    float         color[3];      // 17 type, name 7, 2936.
    ListBase      inputs;        // 18 type, name 14, 2638.
    ListBase      outputs;       // 19 type, name 14, 2937.
    bNode         *parent;       // 20 type, name 445, 36.
    ID            *id;           // 21 type, name 22, 1962.
    void          *storage;      // 22 type, name 11, 2920.
    bNode         *original;     // 23 type, name 445, 2938.
    ListBase      internal_links;// 24 type, name 14, 2939.
    float         locx;          // 25 type, name 7, 2924.
    float         locy;          // 26 type, name 7, 2925.
    float         width;         // 27 type, name 7, 603.
    float         height;        // 28 type, name 7, 788.
    float         miniwidth;     // 29 type, name 7, 2940.
    float         offsetx;       // 30 type, name 7, 2941.
    float         offsety;       // 31 type, name 7, 2942.
    int           update;        // 32 type, name 4, 2943.
    char          label[64];     // 33 type, name 0, 2944.
    short         custom1;       // 34 type, name 2, 2945.
    short         custom2;       // 35 type, name 2, 2946.
    float         custom3;       // 36 type, name 7, 2947.
    float         custom4;       // 37 type, name 7, 2948.
    short         need_exec;     // 38 type, name 2, 2949.
    short         exec;          // 39 type, name 2, 2950.
    void          *threaddata;   // 40 type, name 11, 2951.
    rctf          totr;          // 41 type, name 19, 2952.
    rctf          butr;          // 42 type, name 19, 2953.
    rctf          prvr;          // 43 type, name 19, 2954.
    short         preview_xsize; // 44 type, name 2, 2955.
    short         preview_ysize; // 45 type, name 2, 2956.
    int           pad2;          // 46 type, name 4, 29.
    void          *block;        // Note: using void* on undefined DNA type: uiBlock
};

// 547 DNA structure 491, 6
struct NodeTexImage
{
    NodeTexBase    base;            // 1 type, name 489, 1722.
    ImageUser      iuser;           // 2 type, name 40, 317.
    int            color_space;     // 3 type, name 4, 3082.
    int            projection;      // 4 type, name 4, 3083.
    float          projection_blend;// 5 type, name 7, 3084.
    int            interpolation;   // 6 type, name 4, 2548.
};

// 548 DNA structure 494, 4
struct NodeTexEnvironment
{
    NodeTexBase    base;       // 1 type, name 489, 1722.
    ImageUser      iuser;      // 2 type, name 40, 317.
    int            color_space;// 3 type, name 4, 3082.
    int            projection; // 4 type, name 4, 3083.
};

// 549 DNA structure 263, 22
struct SpaceButs
{
    SpaceLink    *next;               // 1 type, name 259, 0.
    SpaceLink    *prev;               // 2 type, name 259, 1.
    ListBase     regionbase;          // 3 type, name 14, 1792.
    int          spacetype;           // 4 type, name 4, 1793.
    float        blockscale;          // 5 type, name 7, 1794.
    short        blockhandler[8];     // 6 type, name 2, 1795.
    View2D       v2d;                 // 7 type, name 260, 1849.
    short        mainb;               // 8 type, name 2, 1850.
    short        mainbo;              // 9 type, name 2, 1851.
    short        mainbuser;           // 10 type, name 2, 1852.
    short        re_align;            // 11 type, name 2, 1853.
    short        align;               // 12 type, name 2, 1840.
    short        preview;             // 13 type, name 2, 1854.
    short        texture_context;     // 14 type, name 2, 1855.
    short        texture_context_prev;// 15 type, name 2, 1856.
    char         flag;                // 16 type, name 0, 18.
    char         pad[7];              // 17 type, name 0, 118.
    void         *path;               // 18 type, name 11, 1857.
    int          pathflag;            // 19 type, name 4, 1858.
    int          dataicon;            // 20 type, name 4, 1859.
    ID           *pinid;              // 21 type, name 22, 1860.
    void         *texuser;            // 22 type, name 11, 1861.
};

// 550 DNA structure 269, 11
struct SpaceNla
{
    SpaceLink     *next;          // 1 type, name 259, 0.
    SpaceLink     *prev;          // 2 type, name 259, 1.
    ListBase      regionbase;     // 3 type, name 14, 1792.
    int           spacetype;      // 4 type, name 4, 1793.
    float         blockscale;     // 5 type, name 7, 1794.
    short         blockhandler[8];// 6 type, name 2, 1795.
    short         autosnap;       // 7 type, name 2, 1872.
    short         flag;           // 8 type, name 2, 18.
    int           pad;            // 9 type, name 4, 66.
    bDopeSheet    *ads;           // 10 type, name 268, 1870.
    View2D        v2d;            // 11 type, name 260, 1849.
};

// 551 DNA structure 58, 36
struct bNodeTree
{
    ID                   id;               // 1 type, name 22, 31.
    AnimData             *adt;             // 2 type, name 36, 75.
    void                 *typeinfo;        // Note: using void* on undefined DNA type: bNodeTreeType
    char                 idname[64];       // 4 type, name 0, 2419.
    void                 *interface_type;  // Note: using void* on undefined DNA type: StructRNA
    bGPdata              *gpd;             // 6 type, name 188, 998.
    float                view_center[2];   // 7 type, name 7, 1960.
    ListBase             nodes;            // 8 type, name 14, 2968.
    ListBase             links;            // 9 type, name 14, 2969.
    int                  type;             // 10 type, name 4, 16.
    int                  init;             // 11 type, name 4, 2970.
    int                  cur_index;        // 12 type, name 4, 2971.
    int                  flag;             // 13 type, name 4, 18.
    int                  update;           // 14 type, name 4, 2943.
    short                is_updating;      // 15 type, name 2, 2972.
    short                done;             // 16 type, name 2, 2484.
    int                  pad2;             // 17 type, name 4, 29.
    int                  nodetype;         // 18 type, name 4, 2973.
    short                edit_quality;     // 19 type, name 2, 2974.
    short                render_quality;   // 20 type, name 2, 2975.
    int                  chunksize;        // 21 type, name 4, 2976.
    rctf                 viewer_border;    // 22 type, name 19, 2977.
    ListBase             inputs;           // 23 type, name 14, 2638.
    ListBase             outputs;          // 24 type, name 14, 2937.
    void                 *previews;        // Note: using void* on undefined DNA type: bNodeInstanceHash
    bNodeInstanceKey     active_viewer_key;// 26 type, name 288, 2979.
    int                  pad;              // 27 type, name 4, 66.
    void                 *execdata;        // Note: using void* on undefined DNA type: bNodeTreeExec
    void                 (*progress)();    // 29 type, name 11, 2981.
    void                 (*stats_draw)();  // 30 type, name 11, 2982.
    int                  (*test_break)();  // 31 type, name 4, 2983.
    void                 (*update_draw)(); // 32 type, name 11, 2984.
    void                 *tbh;             // 33 type, name 11, 2985.
    void                 *prh;             // 34 type, name 11, 2986.
    void                 *sdh;             // 35 type, name 11, 2987.
    void                 *udh;             // 36 type, name 11, 2988.
};

// 552 DNA structure 267, 14
struct SpaceIpo
{
    SpaceLink     *next;          // 1 type, name 259, 0.
    SpaceLink     *prev;          // 2 type, name 259, 1.
    ListBase      regionbase;     // 3 type, name 14, 1792.
    int           spacetype;      // 4 type, name 4, 1793.
    float         blockscale;     // 5 type, name 7, 1794.
    short         blockhandler[8];// 6 type, name 2, 1795.
    View2D        v2d;            // 7 type, name 260, 1849.
    bDopeSheet    *ads;           // 8 type, name 268, 1870.
    ListBase      ghostCurves;    // 9 type, name 14, 1871.
    short         mode;           // 10 type, name 2, 334.
    short         autosnap;       // 11 type, name 2, 1872.
    int           flag;           // 12 type, name 4, 18.
    float         cursorVal;      // 13 type, name 7, 1873.
    int           around;         // 14 type, name 4, 1808.
};

// 553 DNA structure 271, 9
struct SpaceTime
{
    SpaceLink    *next;        // 1 type, name 259, 0.
    SpaceLink    *prev;        // 2 type, name 259, 1.
    ListBase     regionbase;   // 3 type, name 14, 1792.
    int          spacetype;    // 4 type, name 4, 1793.
    float        blockscale;   // 5 type, name 7, 1794.
    View2D       v2d;          // 6 type, name 260, 1849.
    ListBase     caches;       // 7 type, name 14, 1875.
    int          cache_display;// 8 type, name 4, 1876.
    int          flag;         // 9 type, name 4, 18.
};

// 554 DNA structure 272, 21
struct SpaceSeq
{
    SpaceLink          *next;          // 1 type, name 259, 0.
    SpaceLink          *prev;          // 2 type, name 259, 1.
    ListBase           regionbase;     // 3 type, name 14, 1792.
    int                spacetype;      // 4 type, name 4, 1793.
    float              blockscale;     // 5 type, name 7, 1794.
    short              blockhandler[8];// 6 type, name 2, 1795.
    View2D             v2d;            // 7 type, name 260, 1849.
    float              xof;            // 8 type, name 7, 618.
    float              yof;            // 9 type, name 7, 619.
    short              mainb;          // 10 type, name 2, 1850.
    short              render_size;    // 11 type, name 2, 1877.
    short              chanshown;      // 12 type, name 2, 1878.
    short              zebra;          // 13 type, name 2, 1879.
    int                flag;           // 14 type, name 4, 18.
    float              zoom;           // 15 type, name 7, 1880.
    int                view;           // 16 type, name 4, 1755.
    int                overlay_type;   // 17 type, name 4, 1881.
    int                draw_flag;      // 18 type, name 4, 1882.
    int                pad;            // 19 type, name 4, 66.
    bGPdata            *gpd;           // 20 type, name 188, 998.
    SequencerScopes    scopes;         // 21 type, name 273, 1883.
};

// 555 DNA structure 264, 16
struct SpaceOops
{
    SpaceLink        *next;            // 1 type, name 259, 0.
    SpaceLink        *prev;            // 2 type, name 259, 1.
    ListBase         regionbase;       // 3 type, name 14, 1792.
    int              spacetype;        // 4 type, name 4, 1793.
    float            blockscale;       // 5 type, name 7, 1794.
    short            blockhandler[8];  // 6 type, name 2, 1795.
    View2D           v2d;              // 7 type, name 260, 1849.
    ListBase         tree;             // 8 type, name 14, 1862.
    void             *treestore;       // Note: using void* on undefined DNA type: BLI_mempool
    char             search_string[32];// 10 type, name 0, 1864.
    TreeStoreElem    search_tse;       // 11 type, name 266, 1865.
    short            flag;             // 12 type, name 2, 18.
    short            outlinevis;       // 13 type, name 2, 1866.
    short            storeflag;        // 14 type, name 2, 1867.
    short            search_flags;     // 15 type, name 2, 1868.
    void             *treehash;        // 16 type, name 11, 1869.
};

// 556 DNA structure 289, 29
struct SpaceNode
{
    SpaceLink    *next;          // 1 type, name 259, 0.
    SpaceLink    *prev;          // 2 type, name 259, 1.
    ListBase     regionbase;     // 3 type, name 14, 1792.
    int          spacetype;      // 4 type, name 4, 1793.
    float        blockscale;     // 5 type, name 7, 1794.
    short        blockhandler[8];// 6 type, name 2, 1795.
    View2D       v2d;            // 7 type, name 260, 1849.
    ID           *id;            // 8 type, name 22, 1962.
    ID           *from;          // 9 type, name 22, 81.
    short        flag;           // 10 type, name 2, 18.
    short        pad1;           // 11 type, name 2, 68.
    float        aspect;         // 12 type, name 7, 1963.
    float        pad2;           // 13 type, name 7, 29.
    float        xof;            // 14 type, name 7, 618.
    float        yof;            // 15 type, name 7, 619.
    float        zoom;           // 16 type, name 7, 1880.
    float        cursor[2];      // 17 type, name 7, 1914.
    ListBase     treepath;       // 18 type, name 14, 1964.
    bNodeTree    *nodetree;      // 19 type, name 58, 318.
    bNodeTree    *edittree;      // 20 type, name 58, 1965.
    char         tree_idname[64];// 21 type, name 0, 1966.
    int          treetype;       // 22 type, name 4, 1967.
    int          pad3;           // 23 type, name 4, 500.
    short        texfrom;        // 24 type, name 2, 1968.
    short        shaderfrom;     // 25 type, name 2, 1969.
    short        recalc;         // 26 type, name 2, 241.
    short        pad4;           // 27 type, name 2, 1468.
    ListBase     linkdrag;       // 28 type, name 14, 1970.
    bGPdata      *gpd;           // 29 type, name 188, 998.
};

// 557 DNA structure 406, 13
struct SpaceAction
{
    SpaceLink     *next;          // 1 type, name 259, 0.
    SpaceLink     *prev;          // 2 type, name 259, 1.
    ListBase      regionbase;     // 3 type, name 14, 1792.
    int           spacetype;      // 4 type, name 4, 1793.
    float         blockscale;     // 5 type, name 7, 1794.
    short         blockhandler[8];// 6 type, name 2, 1795.
    View2D        v2d;            // 7 type, name 260, 1849.
    bAction       *action;        // 8 type, name 186, 995.
    bDopeSheet    ads;            // 9 type, name 268, 2826.
    char          mode;           // 10 type, name 0, 334.
    char          autosnap;       // 11 type, name 0, 1872.
    short         flag;           // 12 type, name 2, 18.
    float         timeslide;      // 13 type, name 7, 2827.
};

// 558 DNA structure 592, 13
struct MovieTracking
{
    MovieTrackingSettings          settings;        // 1 type, name 599, 3760.
    MovieTrackingCamera            camera;          // 2 type, name 596, 3761.
    ListBase                       tracks;          // 3 type, name 14, 3748.
    ListBase                       plane_tracks;    // 4 type, name 14, 3749.
    MovieTrackingReconstruction    reconstruction;  // 5 type, name 601, 3750.
    MovieTrackingStabilization     stabilization;   // 6 type, name 600, 3762.
    MovieTrackingTrack             *act_track;      // 7 type, name 594, 3763.
    MovieTrackingPlaneTrack        *act_plane_track;// 8 type, name 598, 3764.
    ListBase                       objects;         // 9 type, name 14, 3765.
    int                            objectnr;        // 10 type, name 4, 3766.
    int                            tot_object;      // 11 type, name 4, 3767.
    MovieTrackingStats             *stats;          // 12 type, name 603, 1733.
    MovieTrackingDopesheet         dopesheet;       // 13 type, name 606, 3768.
};

// 559 DNA structure 311, 115
struct UserDef
{
    int               versionfile;                // 1 type, name 4, 2274.
    int               subversionfile;             // 2 type, name 4, 2275.
    int               flag;                       // 3 type, name 4, 18.
    int               dupflag;                    // 4 type, name 4, 2276.
    int               savetime;                   // 5 type, name 4, 2277.
    char              tempdir[768];               // 6 type, name 0, 2278.
    char              fontdir[768];               // 7 type, name 0, 2279.
    char              renderdir[1024];            // 8 type, name 0, 2280.
    char              render_cachedir[768];       // 9 type, name 0, 2281.
    char              textudir[768];              // 10 type, name 0, 2282.
    char              pythondir[768];             // 11 type, name 0, 2283.
    char              sounddir[768];              // 12 type, name 0, 2284.
    char              i18ndir[768];               // 13 type, name 0, 2285.
    char              image_editor[1024];         // 14 type, name 0, 2286.
    char              anim_player[1024];          // 15 type, name 0, 2287.
    int               anim_player_preset;         // 16 type, name 4, 2288.
    short             v2d_min_gridsize;           // 17 type, name 2, 2289.
    short             timecode_style;             // 18 type, name 2, 2290.
    short             versions;                   // 19 type, name 2, 2291.
    short             dbl_click_time;             // 20 type, name 2, 2292.
    short             gameflags;                  // 21 type, name 2, 2293.
    short             wheellinescroll;            // 22 type, name 2, 2294.
    int               uiflag;                     // 23 type, name 4, 2295.
    int               uiflag2;                    // 24 type, name 4, 2296.
    int               language;                   // 25 type, name 4, 2297.
    short             userpref;                   // 26 type, name 2, 2298.
    short             viewzoom;                   // 27 type, name 2, 2299.
    int               mixbufsize;                 // 28 type, name 4, 2300.
    int               audiodevice;                // 29 type, name 4, 2301.
    int               audiorate;                  // 30 type, name 4, 2302.
    int               audioformat;                // 31 type, name 4, 2303.
    int               audiochannels;              // 32 type, name 4, 2304.
    int               scrollback;                 // 33 type, name 4, 1973.
    int               dpi;                        // 34 type, name 4, 2305.
    short             encoding;                   // 35 type, name 2, 2306.
    short             transopts;                  // 36 type, name 2, 2307.
    short             menuthreshold1;             // 37 type, name 2, 2308.
    short             menuthreshold2;             // 38 type, name 2, 2309.
    ListBase          themes;                     // 39 type, name 14, 2310.
    ListBase          uifonts;                    // 40 type, name 14, 2311.
    ListBase          uistyles;                   // 41 type, name 14, 2312.
    ListBase          keymaps;                    // 42 type, name 14, 2313.
    ListBase          user_keymaps;               // 43 type, name 14, 2314.
    ListBase          addons;                     // 44 type, name 14, 2315.
    ListBase          autoexec_paths;             // 45 type, name 14, 2316.
    char              keyconfigstr[64];           // 46 type, name 0, 2317.
    short             undosteps;                  // 47 type, name 2, 2318.
    short             undomemory;                 // 48 type, name 2, 2319.
    short             gp_manhattendist;           // 49 type, name 2, 2320.
    short             gp_euclideandist;           // 50 type, name 2, 2321.
    short             gp_eraser;                  // 51 type, name 2, 2322.
    short             gp_settings;                // 52 type, name 2, 2323.
    short             tb_leftmouse;               // 53 type, name 2, 2324.
    short             tb_rightmouse;              // 54 type, name 2, 2325.
    SolidLight        light[3];                   // 55 type, name 309, 2326.
    short             tw_hotspot;                 // 56 type, name 2, 2327.
    short             tw_flag;                    // 57 type, name 2, 2328.
    short             tw_handlesize;              // 58 type, name 2, 2329.
    short             tw_size;                    // 59 type, name 2, 2330.
    short             textimeout;                 // 60 type, name 2, 2331.
    short             texcollectrate;             // 61 type, name 2, 2332.
    short             wmdrawmethod;               // 62 type, name 2, 2333.
    short             dragthreshold;              // 63 type, name 2, 2334.
    int               memcachelimit;              // 64 type, name 4, 2335.
    int               prefetchframes;             // 65 type, name 4, 2336.
    short             frameserverport;            // 66 type, name 2, 2337.
    short             pad_rot_angle;              // 67 type, name 2, 2338.
    short             obcenter_dia;               // 68 type, name 2, 2339.
    short             rvisize;                    // 69 type, name 2, 2340.
    short             rvibright;                  // 70 type, name 2, 2341.
    short             recent_files;               // 71 type, name 2, 2342.
    short             smooth_viewtx;              // 72 type, name 2, 2343.
    short             glreslimit;                 // 73 type, name 2, 2344.
    short             curssize;                   // 74 type, name 2, 2345.
    short             color_picker_type;          // 75 type, name 2, 2346.
    char              ipo_new;                    // 76 type, name 0, 2347.
    char              keyhandles_new;             // 77 type, name 0, 2348.
    char              gpu_select_method;          // 78 type, name 0, 2349.
    char              pad1;                       // 79 type, name 0, 68.
    short             scrcastfps;                 // 80 type, name 2, 2350.
    short             scrcastwait;                // 81 type, name 2, 2351.
    short             widget_unit;                // 82 type, name 2, 2352.
    short             anisotropic_filter;         // 83 type, name 2, 2353.
    short             use_16bit_textures;         // 84 type, name 2, 2354.
    short             use_gpu_mipmap;             // 85 type, name 2, 2355.
    float             ndof_sensitivity;           // 86 type, name 7, 2356.
    float             ndof_orbit_sensitivity;     // 87 type, name 7, 2357.
    int               ndof_flag;                  // 88 type, name 4, 2358.
    short             ogl_multisamples;           // 89 type, name 2, 2359.
    short             image_draw_method;          // 90 type, name 2, 2360.
    float             glalphaclip;                // 91 type, name 7, 2361.
    short             autokey_mode;               // 92 type, name 2, 1656.
    short             autokey_flag;               // 93 type, name 2, 1657.
    short             text_render;                // 94 type, name 2, 2362.
    short             pad9;                       // 95 type, name 2, 2363.
    ColorBand         coba_weight;                // 96 type, name 51, 2364.
    float             sculpt_paint_overlay_col[3];// 97 type, name 7, 2365.
    float             gpencil_new_layer_col[4];   // 98 type, name 7, 2366.
    short             tweak_threshold;            // 99 type, name 2, 2367.
    char              navigation_mode;            // 100 type, name 0, 2368.
    char              pad;                        // 101 type, name 0, 66.
    char              author[80];                 // 102 type, name 0, 2369.
    char              font_path_ui[1024];         // 103 type, name 0, 2370.
    int               compute_device_type;        // 104 type, name 4, 2371.
    int               compute_device_id;          // 105 type, name 4, 2372.
    float             fcu_inactive_alpha;         // 106 type, name 7, 2373.
    float             pixelsize;                  // 107 type, name 7, 2374.
    int               virtual_pixel;              // 108 type, name 4, 2375.
    short             pie_interaction_type;       // 109 type, name 2, 2376.
    short             pie_initial_timeout;        // 110 type, name 2, 2377.
    short             pie_animation_timeout;      // 111 type, name 2, 2378.
    short             pie_menu_confirm;           // 112 type, name 2, 2379.
    short             pie_menu_radius;            // 113 type, name 2, 2380.
    short             pie_menu_threshold;         // 114 type, name 2, 2381.
    WalkNavigation    walk_navigation;            // 115 type, name 310, 2382.
};

// 560 DNA structure 545, 21
struct wmWindowManager
{
    ID             id;                 // 1 type, name 22, 31.
    wmWindow       *windrawable;       // 2 type, name 546, 3440.
    wmWindow       *winactive;         // 3 type, name 546, 3441.
    ListBase       windows;            // 4 type, name 14, 3442.
    int            initialized;        // 5 type, name 4, 3443.
    short          file_saved;         // 6 type, name 2, 3444.
    short          op_undo_depth;      // 7 type, name 2, 3445.
    ListBase       operators;          // 8 type, name 14, 3446.
    ListBase       queue;              // 9 type, name 14, 3447.
    ReportList     reports;            // 10 type, name 544, 3448.
    ListBase       jobs;               // 11 type, name 14, 3449.
    ListBase       paintcursors;       // 12 type, name 14, 3450.
    ListBase       drags;              // 13 type, name 14, 3451.
    ListBase       keyconfigs;         // 14 type, name 14, 3452.
    wmKeyConfig    *defaultconf;       // 15 type, name 547, 3453.
    wmKeyConfig    *addonconf;         // 16 type, name 547, 3454.
    wmKeyConfig    *userconf;          // 17 type, name 547, 3455.
    ListBase       timers;             // 18 type, name 14, 3456.
    void           *autosavetimer;     // Note: using void* on undefined DNA type: wmTimer
    char           is_interface_locked;// 20 type, name 0, 3458.
    char           par[7];             // 21 type, name 0, 3459.
};

// 561 DNA structure 222, 119
struct RenderData
{
    ImageFormatData           im_format;               // 1 type, name 218, 1424.
    AviCodecData              *avicodecdata;           // 2 type, name 211, 1430.
    QuicktimeCodecData        *qtcodecdata;            // 3 type, name 212, 1431.
    QuicktimeCodecSettings    qtcodecsettings;         // 4 type, name 213, 1432.
    FFMpegCodecData           ffcodecdata;             // 5 type, name 214, 1433.
    int                       cfra;                    // 6 type, name 4, 823.
    int                       sfra;                    // 7 type, name 4, 123.
    int                       efra;                    // 8 type, name 4, 1213.
    float                     subframe;                // 9 type, name 7, 1434.
    int                       psfra;                   // 10 type, name 4, 1435.
    int                       pefra;                   // 11 type, name 4, 1436.
    int                       images;                  // 12 type, name 4, 1437.
    int                       framapto;                // 13 type, name 4, 1438.
    short                     flag;                    // 14 type, name 2, 18.
    short                     threads;                 // 15 type, name 2, 1242.
    float                     framelen;                // 16 type, name 7, 1439.
    float                     blurfac;                 // 17 type, name 7, 1440.
    float                     edgeR;                   // 18 type, name 7, 1441.
    float                     edgeG;                   // 19 type, name 7, 1442.
    float                     edgeB;                   // 20 type, name 7, 1443.
    short                     fullscreen;              // 21 type, name 2, 1444.
    short                     xplay;                   // 22 type, name 2, 1445.
    short                     yplay;                   // 23 type, name 2, 1446.
    short                     freqplay;                // 24 type, name 2, 1447.
    short                     depth;                   // 25 type, name 2, 240.
    short                     attrib;                  // 26 type, name 2, 1448.
    int                       frame_step;              // 27 type, name 4, 1449.
    short                     stereomode;              // 28 type, name 2, 1450.
    short                     dimensionspreset;        // 29 type, name 2, 1451.
    short                     filtertype;              // 30 type, name 2, 356.
    short                     size;                    // 31 type, name 2, 102.
    short                     maximsize;               // 32 type, name 2, 1452.
    short                     pad6;                    // 33 type, name 2, 1453.
    int                       xsch;                    // 34 type, name 4, 1454.
    int                       ysch;                    // 35 type, name 4, 1455.
    short                     xparts;                  // 36 type, name 2, 1456.
    short                     yparts;                  // 37 type, name 2, 1457.
    int                       tilex;                   // 38 type, name 4, 1458.
    int                       tiley;                   // 39 type, name 4, 1459.
    short                     planes;                  // 40 type, name 2, 1412.
    short                     imtype;                  // 41 type, name 2, 1411.
    short                     subimtype;               // 42 type, name 2, 1460.
    short                     quality;                 // 43 type, name 2, 1413.
    short                     displaymode;             // 44 type, name 2, 1461.
    char                      use_lock_interface;      // 45 type, name 0, 1462.
    char                      pad7;                    // 46 type, name 0, 1463.
    int                       scemode;                 // 47 type, name 4, 1464.
    int                       mode;                    // 48 type, name 4, 334.
    int                       raytrace_options;        // 49 type, name 4, 1465.
    short                     raytrace_structure;      // 50 type, name 2, 1466.
    short                     pad1;                    // 51 type, name 2, 68.
    short                     ocres;                   // 52 type, name 2, 1467.
    short                     pad4;                    // 53 type, name 2, 1468.
    short                     alphamode;               // 54 type, name 2, 1469.
    short                     osa;                     // 55 type, name 2, 1470.
    short                     frs_sec;                 // 56 type, name 2, 1471.
    short                     edgeint;                 // 57 type, name 2, 1472.
    rctf                      safety;                  // 58 type, name 19, 1473.
    rctf                      border;                  // 59 type, name 19, 1474.
    rcti                      disprect;                // 60 type, name 18, 1475.
    ListBase                  layers;                  // 61 type, name 14, 1476.
    short                     actlay;                  // 62 type, name 2, 1477.
    short                     mblur_samples;           // 63 type, name 2, 1478.
    float                     xasp;                    // 64 type, name 7, 1479.
    float                     yasp;                    // 65 type, name 7, 1480.
    float                     frs_sec_base;            // 66 type, name 7, 1481.
    float                     gauss;                   // 67 type, name 7, 1482.
    int                       color_mgt_flag;          // 68 type, name 4, 1483.
    float                     postgamma;               // 69 type, name 7, 1484.
    float                     posthue;                 // 70 type, name 7, 1485.
    float                     postsat;                 // 71 type, name 7, 1486.
    float                     dither_intensity;        // 72 type, name 7, 1487.
    short                     bake_osa;                // 73 type, name 2, 1488.
    short                     bake_filter;             // 74 type, name 2, 1489.
    short                     bake_mode;               // 75 type, name 2, 1490.
    short                     bake_flag;               // 76 type, name 2, 1491.
    short                     bake_normal_space;       // 77 type, name 2, 1492.
    short                     bake_quad_split;         // 78 type, name 2, 1493.
    float                     bake_maxdist;            // 79 type, name 7, 1494.
    float                     bake_biasdist;           // 80 type, name 7, 1495.
    short                     bake_samples;            // 81 type, name 2, 1496.
    short                     bake_pad;                // 82 type, name 2, 1497.
    float                     bake_user_scale;         // 83 type, name 7, 1498.
    float                     bake_pad1;               // 84 type, name 7, 1499.
    char                      pic[1024];               // 85 type, name 0, 1500.
    int                       stamp;                   // 86 type, name 4, 1501.
    short                     stamp_font_id;           // 87 type, name 2, 1502.
    short                     pad3;                    // 88 type, name 2, 500.
    char                      stamp_udata[768];        // 89 type, name 0, 1503.
    float                     fg_stamp[4];             // 90 type, name 7, 1504.
    float                     bg_stamp[4];             // 91 type, name 7, 1505.
    char                      seq_prev_type;           // 92 type, name 0, 1506.
    char                      seq_rend_type;           // 93 type, name 0, 1507.
    char                      seq_flag;                // 94 type, name 0, 1508.
    char                      pad5[5];                 // 95 type, name 0, 1509.
    int                       simplify_flag;           // 96 type, name 4, 1510.
    short                     simplify_subsurf;        // 97 type, name 2, 1511.
    short                     simplify_shadowsamples;  // 98 type, name 2, 1512.
    float                     simplify_particles;      // 99 type, name 7, 1513.
    float                     simplify_aosss;          // 100 type, name 7, 1514.
    short                     cineonwhite;             // 101 type, name 2, 1515.
    short                     cineonblack;             // 102 type, name 2, 1516.
    float                     cineongamma;             // 103 type, name 7, 1517.
    short                     jp2_preset;              // 104 type, name 2, 1518.
    short                     jp2_depth;               // 105 type, name 2, 1519.
    int                       rpad3;                   // 106 type, name 4, 1520.
    short                     domeres;                 // 107 type, name 2, 1521.
    short                     domemode;                // 108 type, name 2, 1522.
    short                     domeangle;               // 109 type, name 2, 1523.
    short                     dometilt;                // 110 type, name 2, 1524.
    float                     domeresbuf;              // 111 type, name 7, 1525.
    float                     pad2;                    // 112 type, name 7, 29.
    Text                      *dometext;               // 113 type, name 38, 1526.
    int                       line_thickness_mode;     // 114 type, name 4, 1527.
    float                     unit_line_thickness;     // 115 type, name 7, 1528.
    char                      engine[32];              // 116 type, name 0, 1529.
    BakeData                  bake;                    // 117 type, name 221, 1530.
    int                       preview_start_resolution;// 118 type, name 4, 1531.
    int                       pad;                     // 119 type, name 4, 66.
};

// 562 DNA structure 249, 19
struct MovieClip
{
    ID                                id;                 // 1 type, name 22, 31.
    AnimData                          *adt;               // 2 type, name 36, 75.
    char                              name[1024];         // 3 type, name 0, 34.
    int                               source;             // 4 type, name 4, 137.
    int                               lastframe;          // 5 type, name 4, 138.
    int                               lastsize[2];        // 6 type, name 4, 3669.
    float                             aspx;               // 7 type, name 7, 157.
    float                             aspy;               // 8 type, name 7, 158.
    void                              *anim;              // Note: using void* on undefined DNA type: anim
    void                              *cache;             // Note: using void* on undefined DNA type: MovieClipCache
    bGPdata                           *gpd;               // 11 type, name 188, 998.
    MovieTracking                     tracking;           // 12 type, name 592, 3670.
    void                              *tracking_context;  // 13 type, name 11, 3671.
    MovieClipProxy                    proxy;              // 14 type, name 590, 3672.
    int                               flag;               // 15 type, name 4, 18.
    int                               len;                // 16 type, name 4, 22.
    int                               start_frame;        // 17 type, name 4, 2755.
    int                               frame_offset;       // 18 type, name 4, 3673.
    ColorManagedColorspaceSettings    colorspace_settings;// 19 type, name 47, 159.
};

// 563 DNA structure 325, 31
struct ARegion
{
    ARegion        *next;                 // 1 type, name 325, 0.
    ARegion        *prev;                 // 2 type, name 325, 1.
    View2D         v2d;                   // 3 type, name 260, 1849.
    rcti           winrct;                // 4 type, name 18, 2441.
    rcti           drawrct;               // 5 type, name 18, 2442.
    short          winx;                  // 6 type, name 2, 1841.
    short          winy;                  // 7 type, name 2, 1842.
    short          swinid;                // 8 type, name 2, 2443.
    short          regiontype;            // 9 type, name 2, 2444.
    short          alignment;             // 10 type, name 2, 2445.
    short          flag;                  // 11 type, name 2, 18.
    float          fsize;                 // 12 type, name 7, 614.
    short          sizex;                 // 13 type, name 2, 2410.
    short          sizey;                 // 14 type, name 2, 2411.
    short          do_draw;               // 15 type, name 2, 2390.
    short          do_draw_overlay;       // 16 type, name 2, 2446.
    short          swap;                  // 17 type, name 2, 2395.
    short          overlap;               // 18 type, name 2, 2447.
    short          flagfullscreen;        // 19 type, name 2, 2448.
    short          pad;                   // 20 type, name 2, 66.
    void           *type;                 // Note: using void* on undefined DNA type: ARegionType
    ListBase       uiblocks;              // 22 type, name 14, 2449.
    ListBase       panels;                // 23 type, name 14, 2450.
    ListBase       panels_category_active;// 24 type, name 14, 2451.
    ListBase       ui_lists;              // 25 type, name 14, 2452.
    ListBase       ui_previews;           // 26 type, name 14, 2453.
    ListBase       handlers;              // 27 type, name 14, 2439.
    ListBase       panels_category;       // 28 type, name 14, 2454.
    void           *regiontimer;          // Note: using void* on undefined DNA type: wmTimer
    char           *headerstr;            // 30 type, name 0, 2456.
    void           *regiondata;           // 31 type, name 11, 2457.
};

// 564 DNA structure 29, 134
struct Object
{
    ID                  id;                    // 1 type, name 22, 31.
    AnimData            *adt;                  // 2 type, name 36, 75.
    void                *sculpt;               // Note: using void* on undefined DNA type: SculptSession
    short               type;                  // 4 type, name 2, 16.
    short               partype;               // 5 type, name 2, 986.
    int                 par1;                  // 6 type, name 4, 987.
    int                 par2;                  // 7 type, name 4, 988.
    int                 par3;                  // 8 type, name 4, 989.
    char                parsubstr[64];         // 9 type, name 0, 990.
    Object              *parent;               // 10 type, name 29, 36.
    Object              *track;                // 11 type, name 29, 991.
    Object              *proxy;                // 12 type, name 29, 992.
    Object              *proxy_group;          // 13 type, name 29, 993.
    Object              *proxy_from;           // 14 type, name 29, 994.
    Ipo                 *ipo;                  // 15 type, name 33, 80.
    BoundBox            *bb;                   // 16 type, name 70, 533.
    bAction             *action;               // 17 type, name 186, 995.
    bAction             *poselib;              // 18 type, name 186, 996.
    bPose               *pose;                 // 19 type, name 187, 997.
    void                *data;                 // 20 type, name 11, 2.
    bGPdata             *gpd;                  // 21 type, name 188, 998.
    bAnimVizSettings    avs;                   // 22 type, name 189, 999.
    bMotionPath         *mpath;                // 23 type, name 190, 1000.
    ListBase            constraintChannels;    // 24 type, name 14, 1001.
    ListBase            effect;                // 25 type, name 14, 1002.
    ListBase            defbase;               // 26 type, name 14, 1003.
    ListBase            modifiers;             // 27 type, name 14, 1004.
    int                 mode;                  // 28 type, name 4, 334.
    int                 restore_mode;          // 29 type, name 4, 1005.
    Material            **mat;                 // 30 type, name 65, 548.
    char                *matbits;              // 31 type, name 0, 1006.
    int                 totcol;                // 32 type, name 4, 550.
    int                 actcol;                // 33 type, name 4, 1007.
    float               loc[3];                // 34 type, name 7, 324.
    float               dloc[3];               // 35 type, name 7, 1008.
    float               orig[3];               // 36 type, name 7, 1009.
    float               size[3];               // 37 type, name 7, 174.
    float               dsize[3];              // 38 type, name 7, 1010.
    float               dscale[3];             // 39 type, name 7, 1011.
    float               rot[3];                // 40 type, name 7, 325.
    float               drot[3];               // 41 type, name 7, 1012.
    float               quat[4];               // 42 type, name 7, 536.
    float               dquat[4];              // 43 type, name 7, 1013.
    float               rotAxis[3];            // 44 type, name 7, 1014.
    float               drotAxis[3];           // 45 type, name 7, 1015.
    float               rotAngle;              // 46 type, name 7, 1016.
    float               drotAngle;             // 47 type, name 7, 1017.
    float               obmat[4][4];           // 48 type, name 7, 1018.
    float               parentinv[4][4];       // 49 type, name 7, 799.
    float               constinv[4][4];        // 50 type, name 7, 1019.
    float               imat[4][4];            // 51 type, name 7, 234.
    float               imat_ren[4][4];        // 52 type, name 7, 1020.
    int                 lay;                   // 53 type, name 4, 1021.
    short               flag;                  // 54 type, name 2, 18.
    short               colbits;               // 55 type, name 2, 1022.
    short               transflag;             // 56 type, name 2, 1023.
    short               protectflag;           // 57 type, name 2, 1024.
    short               trackflag;             // 58 type, name 2, 1025.
    short               upflag;                // 59 type, name 2, 1026.
    short               nlaflag;               // 60 type, name 2, 1027.
    short               ipoflag;               // 61 type, name 2, 1028.
    short               scaflag;               // 62 type, name 2, 1029.
    char                scavisflag;            // 63 type, name 0, 1030.
    char                depsflag;              // 64 type, name 0, 1031.
    int                 dupon;                 // 65 type, name 4, 1032.
    int                 dupoff;                // 66 type, name 4, 1033.
    int                 dupsta;                // 67 type, name 4, 1034.
    int                 dupend;                // 68 type, name 4, 1035.
    int                 pad;                   // 69 type, name 4, 66.
    float               mass;                  // 70 type, name 7, 1036.
    float               damping;               // 71 type, name 7, 1037.
    float               inertia;               // 72 type, name 7, 1038.
    float               formfactor;            // 73 type, name 7, 1039.
    float               rdamping;              // 74 type, name 7, 1040.
    float               margin;                // 75 type, name 7, 1041.
    float               max_vel;               // 76 type, name 7, 1042.
    float               min_vel;               // 77 type, name 7, 1043.
    float               obstacleRad;           // 78 type, name 7, 1044.
    float               step_height;           // 79 type, name 7, 1045.
    float               jump_speed;            // 80 type, name 7, 1046.
    float               fall_speed;            // 81 type, name 7, 1047.
    short               col_group;             // 82 type, name 2, 1048.
    short               col_mask;              // 83 type, name 2, 1049.
    short               rotmode;               // 84 type, name 2, 1050.
    char                boundtype;             // 85 type, name 0, 1051.
    char                collision_boundtype;   // 86 type, name 0, 1052.
    short               dtx;                   // 87 type, name 2, 104.
    char                dt;                    // 88 type, name 0, 1053.
    char                empty_drawtype;        // 89 type, name 0, 1054.
    float               empty_drawsize;        // 90 type, name 7, 1055.
    float               dupfacesca;            // 91 type, name 7, 1056.
    ListBase            prop;                  // 92 type, name 14, 1057.
    ListBase            sensors;               // 93 type, name 14, 1058.
    ListBase            controllers;           // 94 type, name 14, 1059.
    ListBase            actuators;             // 95 type, name 14, 1060.
    float               sf;                    // 96 type, name 7, 1061.
    short               index;                 // 97 type, name 2, 413.
    short               actdef;                // 98 type, name 2, 1062.
    float               col[4];                // 99 type, name 7, 676.
    int                 gameflag;              // 100 type, name 4, 1063.
    int                 gameflag2;             // 101 type, name 4, 1064.
    BulletSoftBody      *bsoft;                // 102 type, name 191, 1065.
    char                restrictflag;          // 103 type, name 0, 1066.
    char                recalc;                // 104 type, name 0, 241.
    short               softflag;              // 105 type, name 2, 1067.
    float               anisotropicFriction[3];// 106 type, name 7, 1068.
    ListBase            constraints;           // 107 type, name 14, 1069.
    ListBase            nlastrips;             // 108 type, name 14, 1070.
    ListBase            hooks;                 // 109 type, name 14, 1071.
    ListBase            particlesystem;        // 110 type, name 14, 1072.
    PartDeflect         *pd;                   // 111 type, name 192, 320.
    SoftBody            *soft;                 // 112 type, name 193, 1073.
    Group               *dup_group;            // 113 type, name 66, 1074.
    char                body_type;             // 114 type, name 0, 1075.
    char                shapeflag;             // 115 type, name 0, 1076.
    short               shapenr;               // 116 type, name 2, 590.
    float               smoothresh;            // 117 type, name 7, 667.
    FluidsimSettings    *fluidsimSettings;     // 118 type, name 157, 1077.
    void                *curve_cache;          // Note: using void* on undefined DNA type: CurveCache
    void                *derivedDeform;        // Note: using void* on undefined DNA type: DerivedMesh
    void                *derivedFinal;         // Note: using void* on undefined DNA type: DerivedMesh
    uint64_t            lastDataMask;          // 122 type, name 10, 1081.
    uint64_t            customdata_mask;       // 123 type, name 10, 1082.
    int                 state;                 // 124 type, name 4, 1083.
    int                 init_state;            // 125 type, name 4, 1084.
    ListBase            gpulamp;               // 126 type, name 14, 1085.
    ListBase            pc_ids;                // 127 type, name 14, 1086.
    ListBase            *duplilist;            // 128 type, name 14, 1087.
    RigidBodyOb         *rigidbody_object;     // 129 type, name 195, 1088.
    RigidBodyCon        *rigidbody_constraint; // 130 type, name 196, 1089.
    float               ima_ofs[2];            // 131 type, name 7, 1090.
    ImageUser           *iuser;                // 132 type, name 40, 1091.
    ListBase            lodlevels;             // 133 type, name 14, 1092.
    LodLevel            *currentlod;           // 134 type, name 184, 1093.
};

// 565 DNA structure 303, 29
struct ThemeUI
{
    uiWidgetColors         wcol_regular;     // 1 type, name 299, 2040.
    uiWidgetColors         wcol_tool;        // 2 type, name 299, 2041.
    uiWidgetColors         wcol_text;        // 3 type, name 299, 2042.
    uiWidgetColors         wcol_radio;       // 4 type, name 299, 2043.
    uiWidgetColors         wcol_option;      // 5 type, name 299, 2044.
    uiWidgetColors         wcol_toggle;      // 6 type, name 299, 2045.
    uiWidgetColors         wcol_num;         // 7 type, name 299, 2046.
    uiWidgetColors         wcol_numslider;   // 8 type, name 299, 2047.
    uiWidgetColors         wcol_menu;        // 9 type, name 299, 2048.
    uiWidgetColors         wcol_pulldown;    // 10 type, name 299, 2049.
    uiWidgetColors         wcol_menu_back;   // 11 type, name 299, 2050.
    uiWidgetColors         wcol_menu_item;   // 12 type, name 299, 2051.
    uiWidgetColors         wcol_tooltip;     // 13 type, name 299, 2052.
    uiWidgetColors         wcol_box;         // 14 type, name 299, 2053.
    uiWidgetColors         wcol_scroll;      // 15 type, name 299, 2054.
    uiWidgetColors         wcol_progress;    // 16 type, name 299, 2055.
    uiWidgetColors         wcol_list_item;   // 17 type, name 299, 2056.
    uiWidgetColors         wcol_pie_menu;    // 18 type, name 299, 2057.
    uiWidgetStateColors    wcol_state;       // 19 type, name 300, 2058.
    uiPanelColors          panel;            // 20 type, name 301, 2059.
    char                   widget_emboss[4]; // 21 type, name 0, 2060.
    float                  menu_shadow_fac;  // 22 type, name 7, 2061.
    short                  menu_shadow_width;// 23 type, name 2, 2062.
    short                  pad[3];           // 24 type, name 2, 333.
    char                   iconfile[256];    // 25 type, name 0, 2063.
    float                  icon_alpha;       // 26 type, name 7, 2064.
    char                   xaxis[4];         // 27 type, name 0, 2065.
    char                   yaxis[4];         // 28 type, name 0, 2066.
    char                   zaxis[4];         // 29 type, name 0, 2067.
};

// 566 DNA structure 41, 48
struct Scene
{
    ID                                id;                           // 1 type, name 22, 31.
    AnimData                          *adt;                         // 2 type, name 36, 75.
    Object                            *camera;                      // 3 type, name 29, 1572.
    World                             *world;                       // 4 type, name 209, 1720.
    Scene                             *set;                         // 5 type, name 41, 1721.
    ListBase                          base;                         // 6 type, name 14, 1722.
    Base                              *basact;                      // 7 type, name 210, 1723.
    Object                            *obedit;                      // 8 type, name 29, 1724.
    float                             cursor[3];                    // 9 type, name 7, 1725.
    float                             twcent[3];                    // 10 type, name 7, 1726.
    float                             twmin[3];                     // 11 type, name 7, 1727.
    float                             twmax[3];                     // 12 type, name 7, 1728.
    int                               lay;                          // 13 type, name 4, 1021.
    int                               layact;                       // 14 type, name 4, 1729.
    int                               lay_updated;                  // 15 type, name 4, 1730.
    short                             flag;                         // 16 type, name 2, 18.
    char                              use_nodes;                    // 17 type, name 0, 323.
    char                              pad[1];                       // 18 type, name 0, 230.
    bNodeTree                         *nodetree;                    // 19 type, name 58, 318.
    Editing                           *ed;                          // 20 type, name 246, 1731.
    ToolSettings                      *toolsettings;                // 21 type, name 242, 1732.
    void                              *stats;                       // Note: using void* on undefined DNA type: SceneStats
    RenderData                        r;                            // 23 type, name 222, 183.
    AudioData                         audio;                        // 24 type, name 215, 1734.
    ListBase                          markers;                      // 25 type, name 14, 1735.
    ListBase                          transform_spaces;             // 26 type, name 14, 1736.
    void                              *sound_scene;                 // 27 type, name 11, 1737.
    void                              *sound_scene_handle;          // 28 type, name 11, 1738.
    void                              *sound_scrub_handle;          // 29 type, name 11, 1739.
    void                              *speaker_handles;             // 30 type, name 11, 1740.
    void                              *fps_info;                    // 31 type, name 11, 1741.
    void                              *theDag;                      // Note: using void* on undefined DNA type: DagForest
    short                             dagflags;                     // 33 type, name 2, 1743.
    short                             recalc;                       // 34 type, name 2, 241.
    int                               active_keyingset;             // 35 type, name 4, 1744.
    ListBase                          keyingsets;                   // 36 type, name 14, 1745.
    GameFraming                       framing;                      // 37 type, name 225, 1554.
    GameData                          gm;                           // 38 type, name 227, 1746.
    UnitSettings                      unit;                         // 39 type, name 244, 1747.
    bGPdata                           *gpd;                         // 40 type, name 188, 998.
    PhysicsSettings                   physics_settings;             // 41 type, name 245, 1748.
    MovieClip                         *clip;                        // 42 type, name 249, 1749.
    uint64_t                          customdata_mask;              // 43 type, name 10, 1082.
    uint64_t                          customdata_mask_modal;        // 44 type, name 10, 1750.
    ColorManagedViewSettings          view_settings;                // 45 type, name 219, 1422.
    ColorManagedDisplaySettings       display_settings;             // 46 type, name 220, 1423.
    ColorManagedColorspaceSettings    sequencer_colorspace_settings;// 47 type, name 47, 1751.
    RigidBodyWorld                    *rigidbody_world;             // 48 type, name 250, 1752.
};

// 567 DNA structure 306, 24
struct bTheme
{
    bTheme            *next;            // 1 type, name 306, 0.
    bTheme            *prev;            // 2 type, name 306, 1.
    char              name[32];         // 3 type, name 0, 1532.
    ThemeUI           tui;              // 4 type, name 303, 2244.
    ThemeSpace        tbuts;            // 5 type, name 304, 2245.
    ThemeSpace        tv3d;             // 6 type, name 304, 2246.
    ThemeSpace        tfile;            // 7 type, name 304, 2247.
    ThemeSpace        tipo;             // 8 type, name 304, 2248.
    ThemeSpace        tinfo;            // 9 type, name 304, 2249.
    ThemeSpace        tact;             // 10 type, name 304, 2250.
    ThemeSpace        tnla;             // 11 type, name 304, 2251.
    ThemeSpace        tseq;             // 12 type, name 304, 2252.
    ThemeSpace        tima;             // 13 type, name 304, 2253.
    ThemeSpace        text;             // 14 type, name 304, 2254.
    ThemeSpace        toops;            // 15 type, name 304, 2255.
    ThemeSpace        ttime;            // 16 type, name 304, 2256.
    ThemeSpace        tnode;            // 17 type, name 304, 2257.
    ThemeSpace        tlogic;           // 18 type, name 304, 2258.
    ThemeSpace        tuserpref;        // 19 type, name 304, 2259.
    ThemeSpace        tconsole;         // 20 type, name 304, 2260.
    ThemeSpace        tclip;            // 21 type, name 304, 2261.
    ThemeWireColor    tarm[20];         // 22 type, name 305, 2262.
    int               active_theme_area;// 23 type, name 4, 2263.
    int               pad;              // 24 type, name 4, 66.
};

/** @}*/
}
#endif//_Blender_h_
