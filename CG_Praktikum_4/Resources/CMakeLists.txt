set(dir Resources)
set(${dir}_glsl vertexShader.glsl fragmentShader.glsl gbufferWrite.fs.glsl gbufferRead.fs.glsl gbufferRead.vs.glsl)

foreach(var ${${dir}_glsl})
    set(sources ${sources} ${dir}/${var})
endforeach(var)
include_directories(${dir})
