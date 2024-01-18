
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(CMAKE_SYSROOT "/")  # customize this.
# set(CMAKE_STAGING_PREFIX "/")  # preferably do NOT set this.

set(tools "/usr")
set(CMAKE_C_COMPILER "${tools}/bin/arm-linux-gnueabi-gcc")
set(CMAKE_CXX_COMPILER "${tools}/bin/arm-linux-gnueabi-g++")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
