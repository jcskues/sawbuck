// Copyright 2012 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Utility functions for use with disassembler callbacks.

#ifndef SYZYGY_CORE_DISASSEMBLER_UTIL_H_
#define SYZYGY_CORE_DISASSEMBLER_UTIL_H_

#include "base/basictypes.h"
#include "distorm.h"  // NOLINT

namespace core {

// Wrapper for the distorm_decompose function to patch a bug in distorm. The
// access size for I_FNSTCW and I_FLDCW dst operand is 0 instead of 16.
// TODO(sebmarchand): Remove this code once this is fixed in distorm.
// @param ci Structure containing some information about the code to decompose
//     (code origin, code data, code length, decoding mode and features).
// @param result Array of type _DecodeInst which will be used by this function
//     in order to return the disassembled instructions.
// @param max_instructions The maximum number of entries in the result array
//     that you pass to this function, so it won't exceed its bound.
// @param used_instructions_count Number of the instruction that successfully
//     were disassembled and written to the result array.
// @returns DECRES_SUCCESS on success (no more to disassemble), DECRES_INPUTERR
//     on input error (null code buffer, invalid decoding mode, etc...),
//     DECRES_MEMORYERR when there are not enough entries to use in the result
//     array, BUT YOU STILL have to check for usedInstructionsCount!
_DecodeResult DistormDecompose(_CodeInfo* ci,
                               _DInst result[],
                               unsigned int max_instructions,
                               unsigned int* used_instructions_count);

// Decodes exactly one instruction from the given buffer.
// @param address the address of the instruction, as an absolute address
//     consistent with the image's base address. If this is not provided a
//     fake address of 0x10000000 will be used.
// @param buffer the buffer containing the data to decode.
// @param length the length of the buffer.
// @returns true if an instruction was decoded, false otherwise.
bool DecodeOneInstruction(
    uint32 address, const uint8* buffer, size_t length, _DInst* instruction);
bool DecodeOneInstruction(
    const uint8* buffer, size_t length, _DInst* instruction);

// Determines if the given instruction is a recognized no-op. We only recognize
// those instructions that we see generated by the MSVS toolchain.
// @param instruction the instruction to evaluate.
// @returns true if @p instruction is a recognized no-op, false otherwise.
bool IsNop(const _DInst& instruction);

// Determines if the given instruction is a call.
// @param instruction the instruction to evaluate.
// @returns true if @p instruction is a call, false otherwise.
bool IsCall(const _DInst& instruction);

// Determines if the given instruction is a control-flow instruction.
// @param instruction the instruction to evaluate.
// @returns true if @p instruction is a control-flow instruction, false
//     otherwise.
bool IsControlFlow(const _DInst& instruction);

// Determines if the given instruction is an implicit control-flow instruction.
// @param instruction the instruction to evaluate.
// @returns true if @p instruction is an implicit control-flow instruction
//     (we can't explicitly compute the target due to the addressing mode)
//     false otherwise.
bool IsImplicitControlFlow(const _DInst& instruction);

// Determines if the given instruction is an interrupt instruction.
// @param instruction the instruction to evaluate.
// @returns true if @p instruction is an interrupt instruction, false otherwise.
bool IsInterrupt(const _DInst& instruction);

// Determines if the given instruction is the debug interrupt instruction.
// @param instruction the instruction to evaluate.
// @returns true if @p instruction is the debug interrupt instruction, false
//     otherwise.
bool IsDebugInterrupt(const _DInst& instruction);

}  // namespace core

#endif  // SYZYGY_CORE_DISASSEMBLER_UTIL_H_
