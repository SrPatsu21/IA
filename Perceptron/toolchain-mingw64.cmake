set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_VERSION 1) # optional

# Target architecture: 64-bit
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)

# This tells CMake to produce .exe instead of Linux binaries
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)
