// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "config.h"
#include "DMSFile.h"
#include "AmigaFile.h"

extern "C" {
unsigned short extractDMS(FILE *fi, FILE *fo);
}

bool
DMSFile::isCompatiblePath(const string &path)
{
    string suffix = util::extractSuffix(path);
    return suffix == "dms" || suffix == "DMS";
}

bool
DMSFile::isCompatibleStream(std::istream &stream)
{
    u8 signature[] = { 'D', 'M', 'S', '!' };
                                                                                            
    return util::matchingStreamHeader(stream, signature, sizeof(signature));
}

isize
DMSFile::readFromStream(std::istream &stream)
{
    FILE *fpi, *fpo;
    char *pi, *po;
    size_t si, so;
    
    isize result = AmigaFile::readFromStream(stream);
        
    // We use a third-party tool called xdms to convert the DMS file into an
    // ADF file. Originally, xdms is a command line utility that is designed
    // to work with the file system. To ease the integration of this tool, we
    // utilize memory streams for getting data in and out.

    msg("Setting up input stream\n");
    
    // Setup input stream
    fpi = open_memstream(&pi, &si);
    for (isize i = 0; i < size; i++) putc(data[i], fpi);
    fclose(fpi);
    
    msg("Setting up ouptput stream\n");

    // Setup output stream
    fpi = fmemopen(pi, si, "r");
    fpo = open_memstream(&po, &so);
    
    // Convert the DMS into an ADF
    isize error = extractDMS(fpi, fpo);
    printf("error = %zd\n", error);
    fclose(fpi);
    fclose(fpo);
    
    msg("Create ADF\n");

    // Create ADF
    fpo = fmemopen(po, so, "r");
    /*
    FILE *tmp = fopen("/tmp/tmp.adf", "w");
    int c;
    while ((c = getc(fpo)) != -1) {
        putc(c, tmp);
    }
    fclose(tmp);
    */
    
    adf = AmigaFile::make <ADFFile> (fpo);
    fclose(fpo);
    
    if (!adf) throw VAError(ERROR_UNKNOWN);
    return result;
}
