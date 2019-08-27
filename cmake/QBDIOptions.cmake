
if(__add_qbdi_options)
	return()
endif()
set(__add_qbdi_options ON)

# Enable compilation using ccache
option(QBDI_CCACHE "Enable CCACHE" ON)

# FORCE_DISABLE_AVX - default is OFF
if(QBDI_ARCH_X86_64 OR QBDI_ARCH_X86)
    option(QBDI_DISABLE_AVX "Force disable AVX support in case dynamic support detection is buggy" OFF)
else()
    # AVX only available on X86 and X86-64
    set(QBDI_DISABLE_AVX OFF)
endif()

# ASAN option
option(QBDI_ASAN "Enable AddressSanitizer (ASAN) for debugging (May be slow down)" OFF)

# Enable the logging level debug
option(QBDI_LOG_DEBUG "Enable Debug log level" OFF)

# test
option(QBDI_TEST "Compile tests" ON)

# benchmark
option(QBDI_BENCHMARK "Compile benchmark" ON)

# example
option(QBDI_EXAMPLES "Compile examples" OFF)

# tools

if (NOT (QBDI_PLATFORM_WINDOWS OR QBDI_PLATFORM_IOS))
    # QBDIPreload (not available on windows)
    option(QBDI_TOOLS_QBDIPRELOAD "Compile QBDIPRELOAD (not available on windows)" ON)

    # Validator (compare execution between QBDIPreload and ptrace)
    option(QBDI_TOOLS_VALIDATOR "Compile the validator (need TOOLS_QBDIPRELOAD)" OFF)

    # PYQBDI (need a python 32bit for 32bit architecture)
    if(QBDI_BITS_64)
        option(QBDI_TOOLS_PYQBDI "Compile python binding (need TOOLS_QBDIPRELOAD)" ON)
    else()
        option(QBDI_TOOLS_PYQBDI "Compile python binding (need TOOLS_QBDIPRELOAD)" OFF)
    endif()
else()
    set(QBDI_TOOLS_QBDIPRELOAD OFF)
    set(QBDI_TOOLS_VALIDATOR OFF)
    set(QBDI_TOOLS_PYQBDI OFF)
endif()

# binding QBDI for frida
option(QBDI_TOOLS_FRIDAQBDI "Install frida-qbdi" ON)

# package
option(QBDI_PACKAGE_LIBNCURSE6 "create package with libncurse6 (default libncurse5)" OFF)


message("== QBDI Options ==")
message("QBDI_CCACHE:           ${QBDI_CCACHE}")
if (QBDI_ARCH_X86_64 OR QBDI_ARCH_X86)
    message("QBDI_DISABLE_AVX:      ${QBDI_DISABLE_AVX}")
endif()
message("QBDI_ASAN:             ${QBDI_ASAN}")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(QBDI_LOG_DEBUG ON)
    message("QBDI_LOG_DEBUG:        ${QBDI_LOG_DEBUG} (by CMAKE_BUILD_TYPE=Debug)")
else()
    message("QBDI_LOG_DEBUG:        ${QBDI_LOG_DEBUG}")
endif()
message("QBDI_TEST:             ${QBDI_TEST}")
message("QBDI_BENCHMARK:        ${QBDI_BENCHMARK}")
message("QBDI_EXAMPLES:         ${QBDI_EXAMPLES}")
if (NOT (QBDI_PLATFORM_WINDOWS OR QBDI_PLATFORM_IOS))
    message("QBDI_TOOLS_QBDIPRELOAD: ${QBDI_TOOLS_QBDIPRELOAD}")
    message("QBDI_TOOLS_VALIDATOR:  ${QBDI_TOOLS_VALIDATOR}")
    message("QBDI_TOOLS_PYQBDI:     ${QBDI_TOOLS_PYQBDI}")
endif()
message("QBDI_TOOLS_FRIDAQBDI:  ${QBDI_TOOLS_FRIDAQBDI}")

message("")

if (QBDI_LOG_DEBUG)
    set(QBDI_ENABLE_LOG_DEBUG 1)
endif()



