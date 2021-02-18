uniform float offset_x;
uniform float offset_y;

void main() {
    gl_Position = gl_ProjectionMatrix * gl_Vertex * gl_ModelViewMatrix;
    gl_TexCoord[0] = gl_MultiTexCoord0 * gl_TextureMatrix[0] ;
    gl_TexCoord[0].x = gl_TexCoord[0].x + offset_x;
    gl_TexCoord[0].y = gl_TexCoord[0].y + offset_y;
    gl_FrontColor = gl_Color;
}