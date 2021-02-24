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

uniform vec4 myColor;
void main()
{
    // lookup the pixel in the texture
    vec4 pixel1 = texture2D(texture1, gl_TexCoord[0].xy);
    vec4 pixel2 = texture2D(texture2, gl_TexCoord[0].xy);

    // multiply it by the color
    gl_FragColor = gl_Color * pixel1 * pixel2;
}
