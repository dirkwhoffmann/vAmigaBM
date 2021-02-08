// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Application.h"
// #include <unistd.h>

// TextureManager textures;

int main(int argc, const char * argv[]) {

    Application app;

    // REMOVE ASAP
    for (int i = 0; i < argc; i++) {
        printf("%d: %s\n", i, argv[i]);
    }
    
    // Initiate the launch sequence
    try {
        app.check();
        app.init();
        app.configure("");
        
    } catch (Exception &e) {
        std::cout << "vAmiga is not able to launch." << std::endl;
        std::cout << "Error: " << e.what() << std::endl;
        exit(1);
    }
    
    // Enter the event loop
    app.run();
    return 0;
}
