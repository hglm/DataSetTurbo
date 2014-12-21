/*

Copyright (c) 2014 Harm Hanemaaijer <fgenfb@yahoo.com>

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

*/

// This file has been adapted from cpuinfo.h from the xf86-video-fbturbo driver
// (http://www.github.com/ssvb/xf86-video-fbturbo.git).
/*
 * Copyright © 2013 Siarhei Siamashka <siarhei.siamashka@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef CPUINFO_H
#define CPUINFO_H

enum {
	DST_CPU_TYPE_UNKNOWN,
	DST_CPU_TYPE_X86,
	DST_CPU_TYPE_ARM
};

/* The information about CPU features */
typedef struct {
    int type;
    // x86 features
    bool has_sse2;
    bool has_sse3;
    bool has_ssse3;
    bool has_sse4a;
    bool has_misalignsse;
    bool has_sse41;
    bool has_sse42;
    bool has_avx;
    bool has_fma4;
    bool has_fma3;
    // ARM features.
    /* The values originating from MIDR register */
    int arm_implementer;
    int arm_architecture;
    int arm_variant;
    int arm_part;
    int arm_revision;
    /* CPU features */
    int has_arm_edsp;
    int has_arm_vfp;
    int has_arm_neon;
    int has_arm_wmmx;
    /* The user-friendly CPU description string (usable for logs, etc.) */
    char *processor_name;
} dstCpuInfo;

dstCpuInfo *dstCpuInfoInit();
void dstCpuInfoClose(dstCpuInfo *cpuinfo);

#endif

