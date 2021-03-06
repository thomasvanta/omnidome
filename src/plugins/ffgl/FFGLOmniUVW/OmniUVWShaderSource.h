#ifndef OMNI_UVW_SHADER_CODE_H_
#define OMNI_UVW_SHADER_CODE_H_
const char* omni_uvw_shader_source_code = 
"#ifndef ISF\n "
"uniform float map_mode;\n "
"uniform sampler2D image;\n "
"uniform sampler2D uvw_map;\n "
"uniform float map_yaw;\n "
"uniform float map_pitch;\n "
"uniform float map_roll;\n "
"uniform bool map_flip_vertical;\n "
"uniform bool map_flip_horizontal;\n "
"#endif\n "
"#define MAP_EQUIRECTANGULAR 1.0\n "
"#define MAP_FISHEYE 2.0\n "
"#define MAP_CUBE 3.0\n "
"vec3 uvw;\n "
"const float PI = 3.14159265358979323846264;\n "
"/// Convert degrees to radians\n "
"float deg2rad(in float deg)\n "
"{\n "
"  return deg * PI / 180.0;\n "
"}\n "
"/// Calculates the rotation matrix of a rotation around Z axis with an angle in radians\n "
"mat3 rotateAroundZ( in float angle )\n "
"{\n "
"  float s = sin(angle);\n "
"  float c = cos(angle);\n "
"  return mat3(  c, -s,0.0,\n "
"                s,  c,0.0,\n "
"              0.0,0.0,1.0);\n "
"}\n "
"/// Calculates the rotation matrix of a rotation around X axis with an angle in radians\n "
"mat3 rotateAroundX( in float angle )\n "
"{\n "
"  float s = sin(angle);\n "
"  float c = cos(angle);\n "
"  return mat3(1.0,0.0,0.0,\n "
"              0.0,  c, -s,\n "
"              0.0,  s,  c);\n "
"}\n "
"/// Calculates the rotation matrix of a rotation around Y axis with an angle in radians\n "
"mat3 rotateAroundY( in float angle )\n "
"{\n "
"  float s = sin(angle);\n "
"  float c = cos(angle);\n "
"  return mat3(  c,0.0,  s,\n "
"              0.0,1.0,0.0,\n "
"               -s,0.0,  c);\n "
"}\n "
"/// Calculate rotation by given yaw and pitch angles (in degrees!)\n "
"mat3 rotationMatrix(in float yaw, in float pitch, in float roll)\n "
"{\n "
"  return rotateAroundZ(deg2rad(yaw)) *\n "
"         rotateAroundY(deg2rad(-pitch)) *\n "
"         rotateAroundX(deg2rad(roll));\n "
"}\n "
"vec3 map_uvw()\n "
"{\n "
"  return rotationMatrix(map_yaw,map_pitch,map_roll) * uvw;\n "
"}\n "
"#ifdef MAP_EQUIRECTANGULAR\n "
"float map_equirectangular(out vec2 texCoords)\n "
"{\n "
"  vec3 uvw = map_uvw();\n "
"  texCoords.s = fract(atan(uvw.y,uvw.x) / (2.0*PI));\n "
"  texCoords.t = fract(acos(uvw.z) / PI);\n "
"  return 1.0;\n "
"}\n "
"#endif\n "
"#ifdef MAP_FISHEYE\n "
"float map_fisheye(out vec2 texCoords)\n "
"{\n "
"  vec3 n = map_uvw();\n "
"  float phi = atan(sqrt(n.x*n.x + n.y*n.y),n.z);\n "
"  float r =  phi / PI * 2.0;\n "
"  if ((r > 1.0) || (r <= 0.0)) return -1.0;\n "
"  float theta = atan(n.x,n.y);\n "
"  texCoords.s = fract(0.5 * (1.0 + r* cos(theta)));\n "
"  texCoords.t = fract(0.5 * (1.0 + r * sin(theta)));\n "
"  return 1.0;\n "
"}\n "
"#endif\n "
"#ifdef MAP_CUBE\n "
"float map_cube(out vec2 texCoords)\n "
"{\n "
"  vec3 n = map_uvw();\n "
"  float sc, tc, ma;\n "
"  float eps =  -0.015;\n "
"  float _off = 0.0;\n "
"  //n = n.yzx;\n "
"  if ((abs(n.x) >= abs(n.y)) && (abs(n.x) >= abs(n.z)))\n "
"  {\n "
"    sc = (n.x > 0.0) ? -n.y : n.y;\n "
"    tc = n.z;\n "
"    ma = n.x;\n "
"    _off += (n.x < 0.0) ? 0.0/6.0 : 2.0/6.0; // EAST / WEST\n "
"  } else\n "
"  if ((abs(n.y) >= abs(n.z)))\n "
"  {\n "
"    sc = (n.y < 0.0) ? -n.x : n.x;\n "
"    tc = n.z;\n "
"    ma = n.y;\n "
"    _off += (n.y < 0.0) ? 1.0/6.0 : 3.0/6.0; // NORTH / SOUTH\n "
"  } else\n "
"  {\n "
"    sc = (n.z > 0.0) ? n.y : n.y;\n "
"    tc = (n.z > 0.0) ? n.x : -n.x;\n "
"    ma = n.z;\n "
"    _off = (n.z < 0.0) ? 4.0/6.0 : 5.0 / 6.0;  // TOP / BOTTOM\n "
"  }\n "
"  texCoords = vec2(sc/(12.0 - eps)/abs(ma) + 0.5/6.0 + _off,0.5+tc/(2.0 - eps)/abs(ma)) ;\n "
"  return 1.0;\n "
"}\n "
"#endif\n "
"float mapping(out vec2 texCoords)\n "
"{\n "
"#ifdef MAP_EQUIRECTANGULAR\n "
"  if (map_mode < MAP_EQUIRECTANGULAR)\n "
"  {\n "
"    return map_equirectangular(texCoords);\n "
"  }\n "
"#endif\n "
"#ifdef MAP_FISHEYE\n "
"  if (map_mode < MAP_FISHEYE)\n "
"  {\n "
"    return map_fisheye(texCoords);\n "
"  }\n "
"#endif\n "
"#ifdef MAP_CUBE\n "
"  if (map_mode < MAP_CUBE)\n "
"  {\n "
"    return map_cube(texCoords);\n "
"  }\n "
"#endif\n "
"  return -1.0;\n "
"}\n "
"void main()\n "
"{\n "
"  gl_FragColor = vec4(0.0,0.0,0.0,1.0);\n "
"    vec2 s = vec2(1.0,1.0);\n "
"    vec2 uv = gl_TexCoord[0].st;\n "
"    vec2 uvw_upper = vec2(uv.x,(uv.y + 2.0)/3.0);\n "
"    vec2 uvw_lower = vec2(uv.x,(uv.y + 1.0)/3.0);\n "
"    vec2 blendmask = vec2(uv.x,(uv.y) /3.0);\n "
"    vec3 lower_n = texture2D(uvw_map,uvw_lower).xyz;\n "
"    vec3 upper_n = texture2D(uvw_map,uvw_upper).xyz;\n "
"    vec3 n = (upper_n +  lower_n  / 255.0 ) * 2.0 - 1.0;\n "
"    // + 1.0 / 255.0 - texture2D(uvw_map,uvw_lower).xyz/127.0 - 0.5;\n "
"  if (length(n) <= 0.01)\n "
"  {\n "
"    return;\n "
"  }\n "
"  uvw = normalize(n);\n "
"  vec2 texCoords;\n "
"  if (mapping(texCoords) < 0.0)\n "
"  {\n "
"    return;\n "
"  }\n "
"  if (map_flip_horizontal)\n "
"  {\n "
"    texCoords.s = 1.0 - texCoords.s;\n "
"  }\n "
"  if (map_flip_vertical)\n "
"  {\n "
"    texCoords.t = 1.0 - texCoords.t;\n "
"  }\n "
"  float alpha =  texture2D(uvw_map,blendmask).r;\n "
"  gl_FragColor = vec4(texture2D(image, texCoords).rgb*alpha,1.0);\n "
"}\n "
;
#endif /* OMNI_UVW_SHADER_CODE_H_ */
