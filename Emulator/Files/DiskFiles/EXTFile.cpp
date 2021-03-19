// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "config.h"
#include "EXTFile.h"

#include "IO.h"

namespace va {

const u8 EXTFile::extAdfHeaders[2][8] = {

    { 'U', 'A', 'E', '-', '-', 'A', 'D', 'F' },
    { 'U', 'A', 'E', '-', '1', 'A', 'D', 'F' }
};
    
bool
EXTFile::isCompatiblePath(const string &path)
{
    return true;
}

bool
EXTFile::isCompatibleStream(std::istream &stream)
{
    isize length = util::streamLength(stream);
    
    isize len = isizeof(extAdfHeaders[0]);
    isize cnt = isizeof(extAdfHeaders) / len;

    if (length < len) return false;
    
    for (isize i = 0; i < cnt; i++) {
        if (util::matchingStreamHeader(stream, extAdfHeaders[i], len)) return true;
    }
    return false;
}

}
