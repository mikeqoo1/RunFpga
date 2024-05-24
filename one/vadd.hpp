/*
 * Copyright 2021 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _KRNL_VADD_H_
#define _KRNL_VADD_H_

// Includes
#include <iostream>
#include <ap_int.h>
#include <hls_stream.h>

const int size = 4096;

extern "C"
{
    void krnl_vadd(uint32_t *input1, uint32_t *input2, uint32_t *output, int vSize);
}

#endif