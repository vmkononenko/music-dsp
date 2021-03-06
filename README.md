# About

Music-DSP is a C++ library with accompanying toolkit for music information retrieval.

It provides an API for the basic algorithms like
  * fast Fourier transform (FFT)
  * filters
  * window functions

and more sophisticated algorithms like
  * signal envelope detection
  * audio beat detection
  * chord recognition

The library has been started as a fun way to understand music theory concepts.<br>
At some point code base has shaped into a project I consider worth sharing with the public.

Welcome to the project and find your way to contribute and benefit from it!

# Building

While [setting up development environment](https://github.com/vmkononenko/music-dsp/wiki/Setting-Up-Development-Environment) is covered in wiki, use quick steps below as a quick start:

1. Download source code
   ```
   git clone https://github.com/vmkononenko/music-dsp
   ```
2. Create build directory
   ```
   mkdir music-dsp-build
   ```
3. Build
   ```
   cd music-dsp-build
   cmake ../music-dsp
   make
   ```
This is sufficient to build the project with the default configuration. If you want to fine tune it for your specific needs [parameters guide](https://github.com/vmkononenko/music-dsp/wiki/Fine-Tuning-Music-DSP-Lib-with-config.h) will help.

# Using

## Native console client
If `-DWITH_CLIENT=y` (on by default) has been specified during the build, native built-in CLI client will be provided along with the shared lib.
Run `bin/lmclient -h` for the quick help. Check [the wiki](https://github.com/vmkononenko/music-dsp/wiki/Lmclient-%E2%80%92-the-Power-of-Console-Audio-Analysis) to discover advanced features.

## Linking
The code is licensed under LGPL v3.0, meaning that:
  * the library can be used (linked to) in the proprietary projects *as-is*
  * any modifications to it must be published under the same license.

## API
While proper API docs is yet to come you can find how to use it from:
  * client source code at `client/src/lmclient.cpp`
  * `libmusic/include` files

# Contributing
## Submit changes
If you have a fix or enhancement ready PR is always appreciated.<br>
Before doing big features or refactorings though it's best to contact me to make sure your changes will nicely fit.
## Reports issues
Use project [issue tracker](https://github.com/vmkononenko/music-dsp/issues) for reporting issues.
