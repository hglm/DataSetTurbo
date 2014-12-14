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

// Optimized assembly memset() for ARMv6 and ARMV7 platforms. Both NEON and non-NEON versions are
// provided.
//
// The following compile-time definitions are recognized:
// DST_CONFIG_THUMB	Generate for Thumb2 instruction set. This is supported by most
//                      Linux platforms (based on Debian armhf).

#if __ARM_ARCH__ >= 6

#ifdef DST_CONFIG_THUMB
#define W(instr) instr.w
#define THUMB(instr...)	instr
#define ARM(instr...)
#else
#define W(instr) instr
#define THUMB(instr...)
#define ARM(instr...) instr
#endif

/*
 * In practice, because the way NEON is configured on most systems,
 * specifying alignment hints for NEON instructions doesn't seem
 * to improve performance, or even degrade performance in some cases.
 * However, actually having the address aligned to an element
 * boundary or greater is beneficial.
 */
#define NEON_ALIGN(n)
/* #define NEON_ALIGN(n) :n */

/* Prevent the stack from becoming executable */
#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif

.text
.syntax unified
.arch armv7a
.fpu neon

.macro asm_function function_name
    .global \function_name
.func \function_name
.type \function_name, function
ARM(    .p2align 5      )
THUMB(  .p2align 2      )
\function_name:
.endm


/*
 * Macro for memset replacement.
 * write_align must be 0, 8, or 32.
 * use_neon must be 0 or 1.
 *
 * size argument (r2) is a signed integer.
 */

.macro memset_variant write_align, use_neon
.if \use_neon == 1
	.fpu neon
.endif
	ands	r3, r0, #3
	mov	ip, r0
	bne	7f

	/* Destination is word aligned. */
1:	orr	r1, r1, r1, lsl #8
.if \use_neon == 1
	cmp	r2, #16
.else
	cmp	r2, #8
.endif
	orr	r1, r1, r1, lsl #16
.if \use_neon == 1
	blt	13f
        vmov	d0, r1, r1
	vmov	d1, r1, r1
.else
	blt	5f
	mov	r3, r1
.endif

	cmp	r2, #64
	push 	{r4}
.if \use_neon == 1
	blt	10f
.else
	ble	10f
.endif
.if \write_align > 0
	ands	r4, r0, #(\write_align - 1)
.if \use_neon == 1
#ifndef DST_CONFIG_THUMB
	add	r3, r4, #7
#endif
.endif
	/* Let r4 be equal to the number of bytes to align.  */
	rsb	r4, r4, #\write_align
	/*
	 * At this point r4 contains the number of bytes to align
	 * if eq is not set. The eq flag is set if there are no bytes
	 * to align.
	 */
.if \write_align == 8
	subne	r2, r2, r4
	strne	r1, [r0], #4
.elseif \write_align == 32
	beq	2f
	tst     r4, #4
	sub	r2, r2, r4
	strne	r1, [r0], #4
.if \use_neon == 1
#ifdef DST_CONFIG_THUMB
	tst     r4, #8
	vst1ne.64 {d0}, [r0 NEON_ALIGN(64)]!
	cmp	r4, #16
	vst1ge.64 {d0, d1}, [r0 NEON_ALIGN(128)]!
#else
	bic	r4, r3, #7
	lsr	r4, r4, #1
	add	pc, pc, r4
	nop
	vst1.64 {d0}, [r0 NEON_ALIGN(64)]!
	vst1.64 {d0}, [r0 NEON_ALIGN(64)]!
	vst1.64 {d0}, [r0 NEON_ALIGN(64)]!
	vst1.64 {d0}, [r0 NEON_ALIGN(64)]!
#endif
.else
	tst     r4, #8
	stmiane r0!, {r1, r3}
	cmp	r4, #16
	stmiage r0!, {r1, r3}
	stmiage r0!, {r1, r3}
.endif
.endif	/* \write_align == 32 */
	cmp	r2, #64
	blt	4f
.endif	/* \write_align > 0 */

2:
.if \use_neon == 1
        /*
         * When NEON is enabled, \write_align is
         * equal to 32 so specify 256-bit alignment in the
         * NEON store instructions.
         */
	subs	r2, r2, #64
        vmov	q1, q0
3:	vst1.64 {d0-d3}, [r0 NEON_ALIGN(256)]!
	subs	r2, r2, #64
        vst1.64 {d0-d3}, [r0 NEON_ALIGN(256)]!
        bge     3b
	adds	r2, r2, #64
.else
	mov	r4, r1
	subs	r2, r2, #64
	push	{r5}
	mov	r5, r1

3:	stmia	r0!, {r1, r3, r4, r5}
	subs	r2, r2, #64		/* Thumb16 */
	stmia	r0!, {r1, r3, r4, r5}
	stmia	r0!, {r1, r3, r4, r5}
	stmia	r0!, {r1, r3, r4, r5}
	bge	3b
	adds	r2, r2, #64		/* Thumb16 */

	pop	{r5}
.endif
	/* Early exit if there are 0 bytes left. */
/* THUMB(	cbz	r2, 9f	) */
THUMB(	cmp	r2, #0	)
THUMB(	beq	9f	)
ARM(	teq	r2, #0	)
ARM(	beq	9f	)
	/*
	 * Handle 8-64 bytes (or 16-63 bytes in case of NEON).
	 * In case of NEON, destination must be 8-byte aligned.
	 */
4:
.if \use_neon == 1
#ifdef DST_CONFIG_THUMB
	vmov	q1, q0
	cmp	r2, #32
	vst1ge.64 {d0-d3}, [r0 NEON_ALIGN(64)]!
	tst	r2, #16
	vst1ne.64 {d0, d1}, [r0 NEON_ALIGN(64)]!
	tst	r2, #8
	vst1ne.64 {d0}, [r0 NEON_ALIGN(64)]!
	and	r2, r2, #7
#else
	bic	r4, r2, #15
	subs	r2, r2, r4
	rsb	r4, r4, #64
	/*
	 * When using NEON, the vst instruction
	 * (storing 16 bytes) is always 32-bit.
	 */
	lsr	r4, r4, #2
	add	pc, pc, r4
	nop
	vst1.64 {d0, d1}, [r0 NEON_ALIGN(64)]!
	vst1.64 {d0, d1}, [r0 NEON_ALIGN(64)]!
	vst1.64 {d0, d1}, [r0 NEON_ALIGN(64)]!
	vst1.64 {d0, d1}, [r0 NEON_ALIGN(64)]!
	cmp	r2, #8
	strge	r1, [r0], #4
	strge	r1, [r0], #4
	subge	r2, r2, #8
#endif
.else	/* use_neon == 0 */
	bic	r4, r2, #7
	subs	r2, r2, r4
	rsb	r4, r4, #64
	/*
	 * The stmia instruction (storing 8 bytes) is 32-bit for ARM,
	 * 16-bit for Thumb2.
	 */
THUMB(	lsrs	r4, r4, #2	)
ARM(	lsr	r4, r4, #1	)
	add	pc, pc, r4
	nop
	stmia	r0!, {r1, r3}
	stmia	r0!, {r1, r3}
	stmia	r0!, {r1, r3}
	stmia	r0!, {r1, r3}
	stmia	r0!, {r1, r3}
	stmia	r0!, {r1, r3}
	stmia	r0!, {r1, r3}
	stmia	r0!, {r1, r3}
.endif
14:	pop	{r4}

5:	cmp	r2, #4
	strge	r1, [r0], #4
	/* Early exit for multiple of 4 size. */
	ands	r2, r2, #3
	moveq	r0, ip
	bxeq	lr

	/*
	 * At this point there are 1, 2 or 3 bytes,
	 * and the destination is aligned.
	 */
6:	cmp	r2, #2
	strhge	r1, [r0], #2
	strbne	r1, [r0]
	mov	r0, ip
	bx	lr

.if \use_neon == 1
	/* 0-15 bytes left, word aligned. */
13:	cmp	r2, #8
	strge	r1, [r0]
	strge	r1, [r0, #4]
	addge	r0, r0, #8
	subge	r2, r2, #8
	b	5b
.endif

	/* Unaligned case. */
7:	cmp	r2, #4
	blt	8f
#ifdef DST_CONFIG_THUMB
.if \use_neon == 1
	/*
	 * When Thumb2 is enabled with NEON, use the optimized
	 * unaligned NEON code path for small sizes.
	 */
	cmp 	r2, #64
	blt	11f
.endif
#endif
	/* Align the destination. */
	cmp	r3, #2
	sub	r2, r2, #4
	strble	r1, [r0]
	strble	r1, [r0, #1]
	addle	r0, r0, #2
	add	r2, r2, r3
	strbne	r1, [r0], #1
	b	1b

	/* 0 to 3 bytes left. */
8:	cmp	r2, #2
	strbge  r1, [r0]
	strbge  r1, [r0, #1]
	addge	r0, r0, #2
	tst	r2, #1
	strbne  r1, [r0]
	mov	r0, ip
	bx	lr

9:	pop	{r4}
	mov	r0, ip
	bx	lr

	/*
	 * Word aligned 8 <= size <= 64
	 * (16 <= size <= 63 in case of NEON).
	 */
10:
	/* Align the destination to an 8 byte boundary. */
	tst     r0, #4
	strne	r1, [r0], #4
	subne	r2, r2, #4
.if \use_neon == 1
	cmp	r2, #16
	poplt	{r4}
	blt	13b
.else
	cmp	r2, #8
	blt	14b
.endif
	b	4b

#ifdef DST_CONFIG_THUMB
.if \use_neon == 1
	/*
	 * Handle 4 <= size <= 63 bytes, unaligned.
	 * Use unaligned NEON instructions with Thumb2.
	 */
11:
	orr	r1, r1, r1, lsl #8
	tst	r2, #8
	orr	r1, r1, r1, lsl #16
        vmov	d0, r1, r1
	vst1ne.8 {d0}, [r0]!
	vmov	d1, r1, r1
	tst	r2, #16
	vst1ne.8 {d0, d1}, [r0]!
	vmov	q1, q0
	cmp	r2, #32
	and	r2, r2, #7
	vst1ge.8 {d0-d3}, [r0]!
	cmp	r2, #4
	/* The following store is unaligned. */
	strge	r1, [r0], #4
	subge	r2, r2, #4
	b	8b
.endif
#endif
.endm

// Use or do not use ARM NEON SIMD instructions. Most ARM CPUs except old ones like
// Tegra2 or the Raspberry Pi support NEON. NEON is significantly faster for memset.

asm_function dstMemsetARMv6
	// Memset write alignment of 8 is clearly faster than 32 on modern ARM
	// platforms.
	memset_variant 8, 0
.endfunc
asm_function dstMemsetARMv7NEON
	memset_variant 32, 1
.endfunc

#endif

