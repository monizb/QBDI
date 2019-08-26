/*
 * This file is part of QBDI.
 *
 * Copyright 2017 Quarkslab
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "TestSetup/LLVMTestEnv.h"
#include "Platform.h"

LLVMTestEnv::LLVMTestEnv(std::string cpu, std::vector<std::string> mattrs) :
        cpu(cpu), mattrs(mattrs) {
    std::string error;
    std::string featuresStr;

    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllDisassemblers();

    // Build features string
    if (mattrs.size()) {
        llvm::SubtargetFeatures features;
        for (unsigned i = 0; i != mattrs.size(); ++i) {
            features.AddFeature(mattrs[i]);
        }
        featuresStr = features.getString();
    }

#ifdef QBDI_PLATFORM_IOS
    // FIXME
    cpu = "swift";
#endif

    // lookup target
    tripleName = llvm::Triple::normalize(
        llvm::sys::getDefaultTargetTriple()
    );
    llvm::Triple process_triple(tripleName);
    processTarget = llvm::TargetRegistry::lookupTarget(tripleName, error);
    REQUIRE(nullptr != processTarget);
    // Allocate all LLVM classes
    MRI = std::unique_ptr<llvm::MCRegisterInfo>(
        processTarget->createMCRegInfo(tripleName)
    );
    REQUIRE(MRI != nullptr);
    MAI = std::unique_ptr<llvm::MCAsmInfo>(
        processTarget->createMCAsmInfo(*MRI, tripleName)
    );
    REQUIRE(MAI != nullptr);
    MOFI = std::unique_ptr<llvm::MCObjectFileInfo>(new llvm::MCObjectFileInfo());
    REQUIRE(MOFI != nullptr);
    MCTX = std::unique_ptr<llvm::MCContext>(new llvm::MCContext(MAI.get(), MRI.get(), MOFI.get()));
    REQUIRE(MCTX != nullptr);
    MCII = std::unique_ptr<llvm::MCInstrInfo>(processTarget->createMCInstrInfo());
    REQUIRE(MCII != nullptr);
    MSTI = std::unique_ptr<llvm::MCSubtargetInfo>(
      processTarget->createMCSubtargetInfo(tripleName, cpu, featuresStr)
    );
    REQUIRE(MSTI != nullptr);
    auto MAB = std::unique_ptr<llvm::MCAsmBackend>(
        processTarget->createMCAsmBackend(*MSTI, *MRI, llvm::MCTargetOptions())
    );
    MCE = std::unique_ptr<llvm::MCCodeEmitter>(
       processTarget->createMCCodeEmitter(*MCII, *MRI, *MCTX)
    );
    REQUIRE(MAB != nullptr);
    assembly = std::unique_ptr<QBDI::Assembly>(
        new QBDI::Assembly(*MCTX, std::move(MAB), *MCII, *processTarget, *MSTI)
    );
    REQUIRE(assembly != nullptr);
}
