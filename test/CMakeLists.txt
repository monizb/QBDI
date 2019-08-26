

include(ExternalProject)

ExternalProject_Add(QBDITest_Catch2
    GIT_REPOSITORY      "https://github.com/catchorg/Catch2.git"
    GIT_PROGRESS        "true"
    GIT_TAG             "v2.9.2"
    CONFIGURE_COMMAND   ""
    BUILD_COMMAND       ""
    INSTALL_COMMAND     "")

ExternalProject_get_property(QBDITest_Catch2 SOURCE_DIR)
set(QBDITest_Catch2_SOURCE_DIR "${SOURCE_DIR}")

set(SOURCES
    QBDITest.cpp
    API/AllocTest.cpp
    API/RangeTest.cpp
    API/VMTest.cpp
    ExecBlock/ExecBlockTest.cpp
    ExecBlock/ExecBlockManagerTest.cpp
    TestSetup/LLVMTestEnv.cpp
    Patch/ComparedExecutor_${QBDI_ARCH}.cpp
    Patch/Instr_${QBDI_BASE_ARCH}Test.cpp
    Patch/Patch_${QBDI_BASE_ARCH}Test.cpp
    Miscs/StringTest.cpp
    TestSetup/InMemoryAssembler.cpp
    TestSetup/ShellcodeTester.cpp
)

if (QBDI_ARCH_X86_64 OR QBDI_ARCH_X86)
    set(SOURCES ${SOURCES} "API/MemoryAccessTest.cpp")
endif()

if(QBDI_PLATFORM_WINDOWS AND QBDI_ARCH_X86_64)
    set(SOURCES ${SOURCES}
        Patch/WIN64_RunRealExec.asm
    )
elseif (QBDI_PLATFORM_WINDOWS AND QBDI_ARCH_X86)
    set(SOURCES ${SOURCES}
        Patch/WIN86_RunRealExec.asm
    )
    set_source_files_properties("Patch/WIN86_RunRealExec.asm" PROPERTIES COMPILE_FLAGS "/safeseh")
endif()

add_executable(QBDITest ${SOURCES})
add_signature(QBDITest)

target_include_directories(QBDITest PRIVATE
    "${CMAKE_BINARY_DIR}/include"
    "${CMAKE_BINARY_DIR}/include/QBDI"
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/include/QBDI"
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/../src"
    "${CMAKE_CURRENT_SOURCE_DIR}/../deps/llvm/${QBDI_PLATFORM}-${QBDI_ARCH}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../deps/llvm/${QBDI_PLATFORM}-${QBDI_ARCH}/lib/Target/${QBDI_LLVM_ARCH}"
    "${QBDITest_Catch2_SOURCE_DIR}/single_include"
)

add_dependencies(QBDITest QBDITest_Catch2)
target_link_libraries(QBDITest QBDI_static)

set_property(TARGET QBDITest PROPERTY CXX_STANDARD 11)
set_property(TARGET QBDITest PROPERTY CXX_STANDARD_REQUIRED ON)
