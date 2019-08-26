set(SOURCES
    QBDITest.cpp
    API/AllocTest.cpp
    API/MemoryAccessTest.cpp
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
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/../src"
    "${CMAKE_CURRENT_SOURCE_DIR}/../deps/llvm/${QBDI_PLATFORM}-${QBDI_ARCH}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../deps/llvm/${QBDI_PLATFORM}-${QBDI_ARCH}/lib/Target/${QBDI_LLVM_ARCH}"
    "${CMAKE_CURRENT_SOURCE_DIR}/../deps/gtest/${QBDI_PLATFORM}-${QBDI_ARCH}/include"
)

find_library(GTEST gtest PATHS
    "${CMAKE_CURRENT_SOURCE_DIR}/../deps/gtest/${QBDI_PLATFORM}-${QBDI_ARCH}/lib"
    NO_DEFAULT_PATH)


if (${GTEST} STREQUAL "GTEST-NOTFOUND")
    target_link_libraries(QBDITest
        QBDI_static
        "${CMAKE_CURRENT_SOURCE_DIR}/../deps/gtest/${PLATFORM}/lib/libgtest.a"
    )
else()
    target_link_libraries(QBDITest
        QBDI_static
        ${GTEST}
    )
endif()


set_property(TARGET QBDITest PROPERTY CXX_STANDARD 11)
set_property(TARGET QBDITest PROPERTY CXX_STANDARD_REQUIRED ON)
