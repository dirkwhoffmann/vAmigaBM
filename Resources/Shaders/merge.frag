// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec2 textureSize;

void main()
{
    vec2 unit = vec2(1.0, 1.0) / textureSize;
    vec2 coord = gl_TexCoord[0].xy / unit;
    
    vec4 color;
    if (mod(coord.y, 2.0) < 1.0) {
        color = texture2D(texture1, gl_TexCoord[0].xy);
    } else {
        color = texture2D(texture2, gl_TexCoord[0].xy);
    }
    
    gl_FragColor = gl_Color * color;
}
