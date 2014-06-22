set(dir Resources)
set(${dir}_glsl vertexShader.glsl fragmentShader.glsl)

foreach(var ${${dir}_glsl})
    set(sources ${sources} ${dir}/${var})
endforeach(var)
include_directories(${dir})
