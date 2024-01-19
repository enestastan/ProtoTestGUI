# set(CMAKE_C_COMPILER "/opt/homebrew/bin/gcc-13")
# set(CMAKE_CXX_COMPILER "/opt/homebrew/bin/g++-13")

# /usr/bin/gcc is an alias for Apple Clang run "gcc -v" to reveal the truth
# If you need to really use gcc brew install g++ and give path /usr/local bin/gcc

# when doing native development, specifying toolchain components is not necessary.
# the CMAKE_CROSSCOMPILING variable can be used to detect this, with some caveats.
# refer to the cmake documentation of CMAKE_CROSSCOMPILING for further details.

set(CMAKE_LIBRARY_ARCHITECTURE "arm64" CACHE STRING "Native architecture.")