#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D noiseTexture;
uniform float time;

const float offset = 1.0 / 800.0;

// Palette NES completa (0x00-0x3F), 64 colori
const vec3 nesPalette[64] = vec3[](
    vec3( 84, 84, 84)/255.0, vec3(  0, 30,116)/255.0, vec3(  8, 16,144)/255.0, vec3( 48,  0,136)/255.0,
    vec3( 68,  0,100)/255.0, vec3( 92,  0, 48)/255.0, vec3( 84,  4,  0)/255.0, vec3( 60, 24,  0)/255.0,
    vec3( 32, 42,  0)/255.0, vec3(  8, 58,  0)/255.0, vec3(  0, 64,  0)/255.0, vec3(  0, 60,  0)/255.0,
    vec3(  0, 50, 60)/255.0, vec3(  0,  0,  0)/255.0, vec3(  0,  0,  0)/255.0, vec3(  0,  0,  0)/255.0,

    vec3(152,150,152)/255.0, vec3(  8, 76,196)/255.0, vec3( 48, 50,236)/255.0, vec3( 92, 30,228)/255.0,
    vec3(136, 20,176)/255.0, vec3(160, 20,100)/255.0, vec3(152, 34, 32)/255.0, vec3(120, 60,  0)/255.0,
    vec3( 84, 90,  0)/255.0, vec3( 40,114,  0)/255.0, vec3(  8,124,  0)/255.0, vec3(  0,118, 40)/255.0,
    vec3(  0,102,120)/255.0, vec3(  0,  0,  0)/255.0, vec3(  0,  0,  0)/255.0, vec3(  0,  0,  0)/255.0,

    vec3(236,238,236)/255.0, vec3( 76,154,236)/255.0, vec3(120,124,236)/255.0, vec3(176, 98,236)/255.0,
    vec3(228, 84,236)/255.0, vec3(236, 88,180)/255.0, vec3(236,106,100)/255.0, vec3(212,136, 32)/255.0,
    vec3(160,170,  0)/255.0, vec3(116,196,  0)/255.0, vec3( 76,208, 32)/255.0, vec3( 56,204,108)/255.0,
    vec3( 56,180,204)/255.0, vec3( 60, 60, 60)/255.0, vec3(  0,  0,  0)/255.0, vec3(  0,  0,  0)/255.0,

    vec3(236,238,236)/255.0, vec3(168,204,236)/255.0, vec3(188,188,236)/255.0, vec3(212,178,236)/255.0,
    vec3(236,174,236)/255.0, vec3(236,174,212)/255.0, vec3(236,180,176)/255.0, vec3(228,196,144)/255.0,
    vec3(204,210,120)/255.0, vec3(180,222,120)/255.0, vec3(168,226,144)/255.0, vec3(152,226,180)/255.0,
    vec3(160,214,228)/255.0, vec3(160,162,160)/255.0, vec3(  0,  0,  0)/255.0, vec3(  0,  0,  0)/255.0
);

vec2 offsets[9] = vec2[](
    vec2(-offset,  offset), // top-left
    vec2( 0.0f,    offset), // top-center
    vec2( offset,  offset), // top-right
    vec2(-offset,  0.0f),   // center-left
    vec2( 0.0f,    0.0f),   // center-center
    vec2( offset,  0.0f),   // center-right
    vec2(-offset, -offset), // bottom-left
    vec2( 0.0f,   -offset), // bottom-center
    vec2( offset, -offset)  // bottom-right    
);

float kernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
);

int sgn ( float t ) {
    if ( t > 0 ) { return 1; }
    return -1;
}

vec4 blend ( vec4 a, vec4 b, float time ) {
    return a * ( 1-time ) + b * time;
}

// trova il colore della palette NES piu' vicino nello spazio RGB
vec3 nearestNesColor ( vec3 color ) {
    float bestDist = 1e9;
    vec3 best = nesPalette[0];
    for ( int i = 0; i < 64; i++ ) {
        vec3 diff = color - nesPalette[i];
        float dist = dot(diff, diff);
        if ( dist < bestDist ) {
            bestDist = dist;
            best = nesPalette[i];
        }
    }
    return best;
}

void main() {
// aut frame
    float transizione = ( time -1.5 ) / 1.5;
    if ( transizione < 0 ) {
        vec2 TC = TexCoords;
        TC.x = 2*(TexCoords.x-0.5);
        TC.y = -2*(TexCoords.y-0.5);
        TC.x *= 1.1;
        TC.y *= 1.1;

        if ( abs(TC.x) > 1 || abs(TC.y) > 1 ) 
        { FragColor = vec4(0); return; }

        TC.x = (TC.x+1) * 0.5;
        TC.y = (TC.y+1) * 0.5;

        FragColor = blend ( texture( screenTexture, TC ), vec4(0), - transizione );
        return;
    }
    transizione -= 1;
    if ( transizione < 0 ) { transizione = 0; }
    if ( transizione > 1 ) { transizione = 1; }

// bend coordiantes
    vec2 TC = TexCoords;
    TC.x = 2*(TexCoords.x-0.5);
    TC.y = -2*(TexCoords.y-0.5);
    TC.x *= 1.1;
    TC.y *= 1.1;

    TC.x = ( TC.x + TC.x * pow(abs(TC.y/4) * transizione,2) );
    TC.y = ( TC.y + TC.y * pow(abs(TC.x/4) * transizione,2) );
    if ( abs(TC.x) > 1 || abs(TC.y) > 1 ) 
    { FragColor = vec4(0); return; }

    TC.x = (TC.x+1) * 0.5;
    TC.y = (TC.y+1) * 0.5;

// applicate blur
    vec4 sampleTex[9];
    for(int i = 0; i < 9; i++) {
// scanlines
        if ( int( ( TC.y + offsets[i].y )* 1000 + time*4) % 9 < 1 )
        { sampleTex[i] = vec4(0); } 
        else 
        { sampleTex[i] = texture(screenTexture, TC + offsets[i] * transizione); }
    }

    FragColor = vec4(0.0);
    for(int i = 0; i < 9; i++)
        FragColor += sampleTex[i] * kernel[i];
// no blur
// FragColor = texture( screenTexture, TC );

// posterizzazione con palette NES
    vec3 nesColor = nearestNesColor( FragColor.rgb );
    FragColor = vec4( nesColor, FragColor.a );

    FragColor = blend ( texture( screenTexture, TC ), FragColor, transizione );
// add scann error
    float start = TC.y + time/3;
    if ( int( start * 1000 ) % 4000 < 100 ) {
        float coeeficient = float( int( start * 1000 ) % 4000 ) / 100;
        if ( coeeficient > 0.85 ) 
        { FragColor = FragColor - vec4(0.09) * ( 1.5 - coeeficient * 2 ); } 
        else 
        { FragColor = FragColor - vec4(0.09) * coeeficient; }
    }

    FragColor += texture(noiseTexture, vec2 (TC.x, TC.y+time*0.01)) * 0.01;
}