//Remember to rebuild with CMake if this file changes
#ifdef GLES2
	#ifdef GL_FRAGMENT_PRECISION_HIGH
		precision highp float;
		precision highp int;
	#else
		precision mediump float;
		precision mediump int;
	#endif
#else
	#define lowp
	#define mediump
	#define highp
#endif

uniform mat4 u_m; // model matrix
uniform mat4 u_view; // view matrix
uniform mat4 u_viewProjection; 
uniform vec4 u_scene_ambient;
uniform vec3 u_eyeWorldPos;

struct Material
{
	vec4 color;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform Material u_material;

struct BaseLight                                                                    
{                
	vec4 diffuse;
	vec4 specular;
};

struct DirectionalLight
{
    BaseLight base;
    vec3 direction;
};

struct Attenuation
{
    float constant;
    float linear;
    float quadratic;
};                                                                
                                                                                    
struct PointLight                                                                           
{                                                                                           
    BaseLight base;                                                                         
    vec3 position;                                                                          
    Attenuation atten;                                                                      
};                                                                                          

uniform int u_numPointLights;
uniform DirectionalLight u_directionalLight;                                                 
uniform PointLight u_pointLights[MAX_POINT_LIGHTS];

#ifdef COMPILEVS
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
// Vertex shader specific
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
	attribute vec4 a_position;
	attribute vec2 a_texcoord;
	attribute vec3 a_normal;
	attribute vec4 a_color;
	varying vec4 v_color;
	varying vec3 v_normal;
	varying vec2 v_texcoord;
	#if defined(INSTANCED)
	    attribute vec4 a_mMatrixCol0;
	    attribute vec4 a_mMatrixCol1;
	    attribute vec4 a_mMatrixCol2;
		mat4 GetModelMatrix()
		{
		    const vec4 lastColumn = vec4(0.0, 0.0, 0.0, 1.0);
		    return mat4(a_mMatrixCol0, a_mMatrixCol1, a_mMatrixCol2, lastColumn);
		}
	#else
		mat4 GetModelMatrix()
		{
		    return u_m;
		}
	#endif

	vec4 GetWorldPos()
	{
	    #if !defined(INSTANCED)
	    	return GetModelMatrix() * a_position;
	    #else
			//instancing model matrix is a transpose, so the matrix multiply order must be swapped
	       return a_position * GetModelMatrix();
	   	#endif
	}

	vec4 GetWorldNormal()
	{
	    #if !defined(INSTANCED)
	    	return normalize(GetModelMatrix() * vec4(a_normal, 0.0));
	    #else
			//instancing model matrix is a transpose, so the matrix multiply order must be swapped
	       return normalize(vec4(a_normal, 0.0) * GetModelMatrix());
	   	#endif
	}

	vec4 GetClipPos(vec4 vertexWorldPos)
	{
	    return u_viewProjection * vertexWorldPos;
	}

	vec4 CalcPointLight(int index, vec4 vertexWorldPos, vec4 normalWorldPos)
	{                            
		vec3 lightPositionWorldPos = u_pointLights[index].position;

		// Vector that goes from the vertex to the camera, in camera space.
		// In camera space, the camera is at the origin (0,0,0).
		vec3 vertexPositionCameraSpace = (u_view * vertexWorldPos).xyz;
		vec3 eyeDirectionCameraSpace = vec3(0.0) - vertexPositionCameraSpace;

		// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity
		vec3 lightPositionCameraSpace = (u_view * vec4(lightPositionWorldPos,1)).xyz;
		vec3 lightDirectionCameraSpace = lightPositionCameraSpace + eyeDirectionCameraSpace;

		// Normal of the the vertex, in camera space
		vec3 normalCameraSpace = (u_view * normalWorldPos).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not

	    // Distance to the light
		float distance = length(lightPositionWorldPos - vertexWorldPos);

		// Normal of the computed fragment, in camera space
		vec3 n = normalize(normalCameraSpace);

		// Direction of the light (from the fragment to the light)
		vec3 l = normalize(lightDirectionCameraSpace);

		// Cosine of the angle between the normal and the light direction, 
		// clamped above 0
		//  - light is at the vertical of the triangle -> 1
		//  - light is perpendicular to the triangle -> 0
		//  - light is behind the triangle -> 0
		float cosTheta = clamp( dot( n,l ), 0.0, 1.0 );

		// Eye vector (towards the camera)
		vec3 E = normalize(eyeDirectionCameraSpace);

		// Direction in which the triangle reflects the light
		vec3 R = reflect(-l, n);

		// Cosine of the angle between the Eye vector and the Reflect vector,
		// clamped to 0
		//  - Looking into the reflection -> 1
		//  - Looking elsewhere -> < 1
		float cosAlpha = clamp( dot( E,R ), 0.0, 1.0 );

		float specularFactor = pow(cosAlpha, u_material.shininess);

		vec4 ambientColor = u_scene_ambient * u_material.ambient;
		vec4 diffuseColor = (light.diffuse * u_material.diffuse) * cosTheta;
		vec4 specularColor = (light.specular * u_material.specular) * specularFactor;	

	    vec4 color = ambientColor + diffuseColor + specularColor;

	    float attenuation =  u_pointLights[index].atten.constant +                               
	                         u_pointLights[index].atten.linear * distance +                      
	                         u_pointLights[index].atten.quadratic * distance * distance;               
	                                                                                            
	    return color / attenuation;                                                             
	}

#if !defined(HAS_USER_VERTEX_SHADER)
	void main()
	{
	#ifdef SHOW_TEXTURE
		gl_Position = a_position;
		v_texcoord = vec2(a_texcoord.x, 1.0 - a_texcoord.y);
	#elif defined(BLUR) || defined(BLEND)
		gl_Position = a_position;
		v_texcoord = a_texcoord;
	#elif defined(STENCIL)
		gl_Position = GetClipPos(GetWorldPos());	
	#else
		#ifdef DIFFUSE
			vec4 vertexWorldPos = GetWorldPos();
		    vec4 normalWorldPos = GetWorldNormal();
		    //vec4 totalLight = CalcDirectionalLight(normal);
		    vec4 totalLight(0.0);
		    for (int i = 0 ; i < u_numPointLights ; i++) 
		    {
		        totalLight += CalcPointLight(i, vertexWorldPos, normalWorldPos);                                            
		    }                                                                                       
		    v_color = a_color * totalLight;
		#else
			v_color = u_material.color * a_color;
		#endif
	    v_texcoord = a_texcoord;
		gl_Position = GetClipPos(vertexWorldPos);
	#endif
	}
#endif	

#else
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
// Fragment shader specific
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
	uniform sampler2D u_texture0;
	uniform sampler2D u_texture1;

	varying vec4 v_color;
	varying vec3 v_normal;
	varying vec2 v_texcoord;

	#ifdef BLEND
		uniform int u_blendMode;
		vec4 Blend()
		{
			vec4 dst = texture2D(u_texture0, v_texcoord); // rendered scene
			vec4 src = texture2D(u_texture1, v_texcoord); // for example the glow map 

			if ( u_blendMode == 0 )
			{
				// Additive blending (strong result, high overexposure)
				return min(src + dst, 1.0);
			}
			else if ( u_blendMode == 1 )
			{
				// Screen blending (mild result, medium overexposure)
				vec4 color = clamp((src + dst) - (src * dst), 0.0, 1.0);
				color.w = 1.0;
				return color;
			}
			else if ( u_blendMode == 2 )
			{
				// Softlight blending (light result, no overexposure)
				// Due to the nature of soft lighting, we need to bump the black region of the glowmap
				// to 0.5, otherwise the blended result will be dark (black soft lighting will darken
				// the image).
				src = (src * 0.5) + 0.5;

				float x = (src.x <= 0.5) ? (dst.x - (1.0 - 2.0 * src.x) * dst.x * (1.0 - dst.x)) : (((src.x > 0.5) && (dst.x <= 0.25)) ? (dst.x + (2.0 * src.x - 1.0) * (4.0 * dst.x * (4.0 * dst.x + 1.0) * (dst.x - 1.0) + 7.0 * dst.x)) : (dst.x + (2.0 * src.x - 1.0) * (sqrt(dst.x) - dst.x)));
				float y = (src.y <= 0.5) ? (dst.y - (1.0 - 2.0 * src.y) * dst.y * (1.0 - dst.y)) : (((src.y > 0.5) && (dst.y <= 0.25)) ? (dst.y + (2.0 * src.y - 1.0) * (4.0 * dst.y * (4.0 * dst.y + 1.0) * (dst.y - 1.0) + 7.0 * dst.y)) : (dst.y + (2.0 * src.y - 1.0) * (sqrt(dst.y) - dst.y)));
				float z = (src.z <= 0.5) ? (dst.z - (1.0 - 2.0 * src.z) * dst.z * (1.0 - dst.z)) : (((src.z > 0.5) && (dst.z <= 0.25)) ? (dst.z + (2.0 * src.z - 1.0) * (4.0 * dst.z * (4.0 * dst.z + 1.0) * (dst.z - 1.0) + 7.0 * dst.z)) : (dst.z + (2.0 * src.z - 1.0) * (sqrt(dst.z) - dst.z)));
				
				return vec4(x, y, z, 1.0);
			}
			else
			{
				return src;
			}
		}
	#elif defined(BLUR)
		uniform vec2 u_texelSize;
		uniform int u_orientation;
		uniform int u_blurAmount;
		uniform float u_blurScale;
		uniform float u_blurStrength;

		/// Gets the Gaussian value in the first dimension.
		/// <param name=x>Distance from origin on the x-axis.</param>
		/// <param name=deviation>Standard deviation.</param>
		/// <returns>The gaussian value on the x-axis.</returns>
		float Gaussian(float x, float deviation)
		{
			return (1.0 / sqrt(2.0 * 3.141592 * deviation)) * exp(-((x * x) / (2.0 * deviation)));	
		}

		vec4 Blur()
		{
			float halfBlur = float(u_blurAmount) * 0.5;
			vec4 colour = vec4(0.0);
			vec4 texColour = vec4(0.0);
			
			// Gaussian deviation
			float deviation = halfBlur * 0.35;
			deviation *= deviation;
			float strength = 1.0 - u_blurStrength;
			
			//if ( u_orientation == 0 )
			{
				// Horizontal blur
				for (int i = 0; i < 100; ++i)
				{
					if ( i >= u_blurAmount )
						break;
					
					float offset = float(i) - halfBlur;
					texColour = texture2D(u_texture0, v_texcoord + vec2(offset * u_texelSize.x * u_blurScale, 0.0)) * Gaussian(offset * strength, deviation);
					colour += texColour;
				}
			}
			//else
			{
				// Vertical blur
				for (int i = 0; i < 100; ++i)
				{
					if ( i >= u_blurAmount )
						break;
					
					float offset = float(i) - halfBlur;
					texColour = texture2D(u_texture0, v_texcoord + vec2(0.0, offset * u_texelSize.y * u_blurScale)) * Gaussian(offset * strength, deviation);
					colour += texColour;
				}
			}
			
			// Apply colour
			vec4 color = clamp(colour, 0.0, 1.0);
			color.w = 1.0;
			return color;
		}

	#endif

#if !defined(HAS_USER_FRAGMENT_SHADER)

	void main()
	{
	#if defined(TEXT)
		gl_FragColor = v_color * vec4(1.0, 1.0, 1.0, texture2D(u_texture0, v_texcoord).a);
	#elif defined(BLEND)
		gl_FragColor = Blend();
	#elif defined(BLUR)
		gl_FragColor = Blur();
	#elif defined(SHOW_TEXTURE)
		gl_FragColor = texture2D(u_texture0, v_texcoord);
	#elif defined(STENCIL)
		gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	#else
		gl_FragColor = v_color * texture2D(u_texture0, v_texcoord);
	#endif	    
	}	

#endif	

#endif



