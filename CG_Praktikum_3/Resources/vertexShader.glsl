attribute highp vec4 posAttr;
attribute lowp vec4 colAttr;
varying lowp vec4 col;
uniform highp mat4 matrix;
void main() {
   col = vec4(0, 0, 1, 0.5);
   gl_Position = matrix * posAttr;
}
