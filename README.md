## What is vAmiga Bare Metal? 

vAmiga Bare Metal (vAmigaBM) is a fork of vAmiga wich emulates a Commodore Amiga 500, 1000, or 2000 on the Mac. vAmiga is as pretty stable emulator by now and close to a 1.0 release. The goal of vAmigaBM is to bring vAmiga to a variety of other platforms. The emulator is based on SFML (Simple and Fast Multimedia Library) to limit OS specific code to a minimum. As a result, vAmiga BM is portable with little effort to almost any hardware platform supporting this framework.

## Building from source

To build vAmigaBM from source, please follow the instructions below: 

#### Download

The easiest way to download the source code is to clone the GitHub project with the following command:

    git clone https://github.com/dirkwhoffmann/vAmigaBM.git

#### Build

    cd vAmigaBM
    make

  If no errors occur, a subdirectory vAmiga has been created which contains the executable and a number of resource files.
  
    cd vAmiga

## Configure

On startup, vAmigaBM reads in a config file named `startup.ini`. This file is the central place to configure the emulator. Before starting vAmigaBM the first time, two important settings must be made. To do so, open the configuration file in the editor of your choice and search for the following two items:

    memory load rom "..."
    
    dfn set searchpath "..."
    
These two settings tell vAmiga the file name of the Kickstart Rom and the location of ADF files. Please note that vAmiga does not come with any an original Commodore ROM. All original ROMs are the intellectual property of the company Cloanto and cannot be distributed freely.

## Run

You are done. Type 'vAmiga' and have fun with your virtual Amiga.

<div class ="center">
<img src="https://dirkwhoffmann.github.io/vAmiga/images/vAmigaBM.png" width="600">
</div>

## Frequently asked questions

- How can I insert a disk? 

  Click the corresponding drive icon in the status bar. A file requester will come up that lets you select a file from the specified search folder.

- How can I configure vAmigaBM?

  You can configure vAmigaBM by either opening the debug console or editing `startup.ini`. Please see the comments in the `startup.ini` file to learn about the various configuration options.


