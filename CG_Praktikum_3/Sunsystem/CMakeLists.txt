set(dir Sunsystem)
set(${dir}_cpp sunsystem.cpp fileLoader.cpp)
set(${dir}_hpp tree.hpp sunsystem.hpp fileLoader.hpp)

foreach(var ${${dir}_cpp} ${${dir}_hpp})
    set(sources ${sources} ${dir}/${var})
endforeach(var)
include_directories(${dir})
