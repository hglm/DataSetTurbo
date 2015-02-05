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

// This file has been adapted from cpuinfo.c from the xf86-video-fbturbo driver
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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "dstCpuInfo.h"

#if defined(__linux__) || defined(ANDROID) || defined(__ANDROID__)

#define MAXBUFSIZE 16384

static const char *cpuinfo_match_prefix(const char *s, const char *prefix)
{
    const char *result;
    if (strncmp(s, prefix, strlen(prefix)) != 0)
        return NULL;
    result = strchr(s, ':');
    if (!result)
        return NULL;
    result++;
    while (*result && (*result == ' ' || *result == '\t'))
        result++;
    return result;
}

static bool is_space_or_eol(char x)
{
    return (x == 0 || x == ' ' || x == '\t' || x == '\n');
}

static bool find_feature(const char *buffer, const char *feature)
{
    const char *s = buffer;
    while (*s) {
        const char *t = strstr(s, feature);
        if (!t)
            return false;
        if (!is_space_or_eol(*(t + strlen(feature)))) {
            s++;
            continue;
        }
        if (t == buffer)
            return true;
        if (t > buffer && is_space_or_eol(*(t - 1)))
            return true;
        s++;
    }
    return false;
}

static bool parse_proc_cpuinfo(dstCpuInfo *cpuinfo)
{
    char *buffer = (char *)malloc(MAXBUFSIZE);
    FILE *fd;
    const char *val;

    if (!buffer)
        return false;

    fd = fopen("/proc/cpuinfo", "r");
    if (!fd) {
        free(buffer);
        return false;
    }

    cpuinfo->type = DST_CPU_TYPE_UNKNOWN;
    while (fgets(buffer, MAXBUFSIZE, fd)) {
        if (!strchr(buffer, '\n') && !feof(fd)) {
            fclose(fd);
            free(buffer);
            return false;
        }

        if ((val = cpuinfo_match_prefix(buffer, "vendor_id")))
            if (find_feature(val, "AuthenticAMD") ||
            find_feature(val, "GenuineIntel")) {
                cpuinfo->type = DST_CPU_TYPE_X86;
		continue;
            }

        if ((val = cpuinfo_match_prefix(buffer, "Processor")))
	    if (find_feature(val, "ARMv6")
            || find_feature(val, "ARMv7")
            || find_feature(val, "ARMv8")) {
		cpuinfo->type = DST_CPU_TYPE_ARM;
                continue;
            }

        if ((val = cpuinfo_match_prefix(buffer, "Features"))) {
            cpuinfo->has_arm_edsp = find_feature(val, "edsp");
            cpuinfo->has_arm_vfp  = find_feature(val, "vfp");
            cpuinfo->has_arm_vfpv4  = find_feature(val, "vfpv4");
            cpuinfo->has_arm_neon = find_feature(val, "neon");
            cpuinfo->has_arm_wmmx = find_feature(val, "iwmmxt");
        }
        else if ((val = cpuinfo_match_prefix(buffer, "flags"))) {
            cpuinfo->has_sse2 = find_feature(val, "sse2");
            cpuinfo->has_sse3 = find_feature(val, "pni");
            cpuinfo->has_ssse3 = find_feature(val, "ssse3");
            cpuinfo->has_sse4a = find_feature(val, "sse4a");
            cpuinfo->has_sse41 = find_feature(val, "sse41");
            cpuinfo->has_sse42 = find_feature(val, "sse42");
            cpuinfo->has_misalignsse = find_feature(val, "misalignsse");
            cpuinfo->has_avx = find_feature(val, "avx");
            cpuinfo->has_fma4 = find_feature(val, "fma4");
            cpuinfo->has_fma3 = find_feature(val, "fma");
        }
        else if ((val = cpuinfo_match_prefix(buffer, "CPU implementer"))) {
            if (sscanf(val, "%i", &cpuinfo->arm_implementer) != 1) {
                fclose(fd);
                free(buffer);
                return false;
            }
        }
        else if ((val = cpuinfo_match_prefix(buffer, "CPU architecture"))) {
            if (sscanf(val, "%i", &cpuinfo->arm_architecture) != 1) {
                fclose(fd);
                free(buffer);
                return false;
            }
        }
        else if ((val = cpuinfo_match_prefix(buffer, "CPU variant"))) {
            if (sscanf(val, "%i", &cpuinfo->arm_variant) != 1) {
                fclose(fd);
                free(buffer);
                return false;
            }
        }
        else if ((val = cpuinfo_match_prefix(buffer, "CPU part"))) {
            if (sscanf(val, "%i", &cpuinfo->arm_part) != 1) {
                fclose(fd);
                free(buffer);
                return false;
            }
        }
        else if ((val = cpuinfo_match_prefix(buffer, "CPU revision"))) {
            if (sscanf(val, "%x", &cpuinfo->arm_revision) != 1) {
                fclose(fd);
                free(buffer);
                return false;
            }
        }
    }
    fclose(fd);
    free(buffer);
    return true;
}

#else

static int parse_proc_cpuinfo(dstCpuInfo *cpuinfo)
{
    return false;
}

#endif

dstCpuInfo *dstCpuInfoInit()
{
    dstCpuInfo *cpuinfo = (dstCpuInfo *)calloc(sizeof(dstCpuInfo), 1);
    if (!cpuinfo)
        return NULL;

    if (!parse_proc_cpuinfo(cpuinfo)) {
        cpuinfo->processor_name = strdup("Unknown");
        return cpuinfo;
    }

    if (cpuinfo->type != DST_CPU_TYPE_ARM) {
	if (cpuinfo->type == DST_CPU_TYPE_X86)
            cpuinfo->processor_name = strdup("Unknown x86");
        else
            cpuinfo->processor_name = strdup("Unknown Non-ARM/x86");
        return cpuinfo;
    }

    if (cpuinfo->arm_implementer == 0x41 && cpuinfo->arm_part == 0xC0F) {
        cpuinfo->processor_name = strdup("ARM Cortex-A15");
    } else if (cpuinfo->arm_implementer == 0x41 && cpuinfo->arm_part == 0xC09) {
        if (cpuinfo->has_arm_neon)
            cpuinfo->processor_name = strdup("ARM Cortex-A9");
        else
            cpuinfo->processor_name = strdup("ARM Cortex-A9 without NEON (Tegra2?)");
    } else if (cpuinfo->arm_implementer == 0x41 && cpuinfo->arm_part == 0xC08) {
        if (cpuinfo->arm_variant > 1)
            cpuinfo->processor_name = strdup("Late ARM Cortex-A8 (NEON can bypass L1 cache)");
        else
            cpuinfo->processor_name = strdup("Early ARM Cortex-A8");
    } else if (cpuinfo->arm_implementer == 0x41 && cpuinfo->arm_part == 0xC07) {
        cpuinfo->processor_name = strdup("ARM Cortex-A7");
    } else if (cpuinfo->arm_implementer == 0x41 && cpuinfo->arm_part == 0xC05) {
        cpuinfo->processor_name = strdup("ARM Cortex-A5");
    } else if (cpuinfo->arm_implementer == 0x41 && cpuinfo->arm_part == 0xB76) {
        cpuinfo->processor_name = strdup("ARM1176");
    } else if (cpuinfo->arm_implementer == 0x56 && cpuinfo->arm_part == 0x581) {
        cpuinfo->processor_name = strdup("Marvell PJ4");
    } else {
        cpuinfo->processor_name = strdup("Unknown");
    }

    return cpuinfo;
}

void dstCpuInfoClose(dstCpuInfo *cpuinfo)
{
    free(cpuinfo->processor_name);
    free(cpuinfo);
}

