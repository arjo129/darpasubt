	.cpu cortex-m4
	.eabi_attribute 27, 1
	.eabi_attribute 28, 1
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"nrf_balloc.c"
	.text
.Ltext0:
	.section	.text.nrf_balloc_idx2block,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	nrf_balloc_idx2block, %function
nrf_balloc_idx2block:
.LFB148:
	.file 1 "C:\\Users\\paulr\\OneDrive\\Documents\\DARPA\\darpasubt\\darpasubt\\firmware\\dwm1001_dev_source_code\\nRF5_SDK_14.2.0\\components\\libraries\\balloc\\nrf_balloc.c"
	.loc 1 174 0
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #8
.LCFI0:
	str	r0, [sp, #4]
	mov	r3, r1
	strb	r3, [sp, #3]
	.loc 1 176 0
	ldr	r3, [sp, #4]
	ldr	r2, [r3, #12]
	ldrb	r3, [sp, #3]	@ zero_extendqisi2
	ldr	r1, [sp, #4]
	ldrh	r1, [r1, #16]
	mul	r3, r1, r3
	add	r3, r3, r2
	.loc 1 177 0
	mov	r0, r3
	add	sp, sp, #8
.LCFI1:
	@ sp needed
	bx	lr
.LFE148:
	.size	nrf_balloc_idx2block, .-nrf_balloc_idx2block
	.section	.text.nrf_balloc_block2idx,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	nrf_balloc_block2idx, %function
nrf_balloc_block2idx:
.LFB149:
	.loc 1 187 0
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #8
.LCFI2:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 189 0
	ldr	r3, [sp]
	ldr	r2, [sp, #4]
	ldr	r2, [r2, #12]
	subs	r3, r3, r2
	ldr	r2, [sp, #4]
	ldrh	r2, [r2, #16]
	udiv	r3, r3, r2
	uxtb	r3, r3
	.loc 1 190 0
	mov	r0, r3
	add	sp, sp, #8
.LCFI3:
	@ sp needed
	bx	lr
.LFE149:
	.size	nrf_balloc_block2idx, .-nrf_balloc_block2idx
	.section	.text.nrf_balloc_init,"ax",%progbits
	.align	1
	.global	nrf_balloc_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	nrf_balloc_init, %function
nrf_balloc_init:
.LFB150:
	.loc 1 193 0
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #16
.LCFI4:
	str	r0, [sp, #4]
	.loc 1 196 0
	ldr	r3, [sp, #4]
	cmp	r3, #0
	bne	.L6
	.loc 1 196 0 is_stmt 0 discriminator 1
	movs	r3, #14
	b	.L7
.L6:
	.loc 1 204 0 is_stmt 1
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #8]
	mov	r2, r3
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #4]
	subs	r3, r2, r3
	strb	r3, [sp, #15]
	.loc 1 223 0
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	ldr	r2, [sp, #4]
	ldr	r2, [r2, #4]
	str	r2, [r3]
	.loc 1 224 0
	b	.L8
.L9:
	.loc 1 226 0
	ldr	r3, [sp, #4]
	ldr	r2, [r3]
	ldr	r3, [r2]
	adds	r1, r3, #1
	str	r1, [r2]
	ldrb	r2, [sp, #15]
	strb	r2, [r3]
.L8:
	.loc 1 224 0
	ldrb	r3, [sp, #15]	@ zero_extendqisi2
	subs	r2, r3, #1
	strb	r2, [sp, #15]
	cmp	r3, #0
	bne	.L9
	.loc 1 229 0
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	movs	r2, #0
	strb	r2, [r3, #4]
	.loc 1 231 0
	movs	r3, #0
.L7:
	.loc 1 232 0
	mov	r0, r3
	add	sp, sp, #16
.LCFI5:
	@ sp needed
	bx	lr
.LFE150:
	.size	nrf_balloc_init, .-nrf_balloc_init
	.section	.text.nrf_balloc_alloc,"ax",%progbits
	.align	1
	.global	nrf_balloc_alloc
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	nrf_balloc_alloc, %function
nrf_balloc_alloc:
.LFB151:
	.loc 1 235 0
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI6:
	sub	sp, sp, #20
.LCFI7:
	str	r0, [sp, #4]
	.loc 1 238 0
	movs	r3, #0
	str	r3, [sp, #12]
	.loc 1 240 0
	movs	r0, #0
	bl	app_util_critical_region_enter
	.loc 1 242 0
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	ldr	r2, [r3]
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #4]
	cmp	r2, r3
	bls	.L11
.LBB2:
	.loc 1 245 0
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	ldr	r2, [r3]
	subs	r2, r2, #1
	str	r2, [r3]
	ldr	r3, [r3]
	ldrb	r3, [r3]	@ zero_extendqisi2
	mov	r1, r3
	ldr	r0, [sp, #4]
	bl	nrf_balloc_idx2block
	str	r0, [sp, #12]
	.loc 1 248 0
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #8]
	mov	r2, r3
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	ldr	r3, [r3]
	subs	r3, r2, r3
	strb	r3, [sp, #11]
	.loc 1 249 0
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	ldrb	r3, [r3, #4]	@ zero_extendqisi2
	ldrb	r2, [sp, #11]	@ zero_extendqisi2
	cmp	r2, r3
	bls	.L11
	.loc 1 251 0
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	ldrb	r2, [sp, #11]
	strb	r2, [r3, #4]
.L11:
.LBE2:
	.loc 1 255 0
	movs	r0, #0
	bl	app_util_critical_region_exit
	.loc 1 267 0
	ldr	r3, [sp, #12]
	.loc 1 268 0
	mov	r0, r3
	add	sp, sp, #20
.LCFI8:
	@ sp needed
	ldr	pc, [sp], #4
.LFE151:
	.size	nrf_balloc_alloc, .-nrf_balloc_alloc
	.section	.text.nrf_balloc_free,"ax",%progbits
	.align	1
	.global	nrf_balloc_free
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	nrf_balloc_free, %function
nrf_balloc_free:
.LFB152:
	.loc 1 271 0
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
.LCFI9:
	sub	sp, sp, #16
.LCFI10:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 304 0
	ldr	r3, [sp]
	str	r3, [sp, #12]
	.loc 1 307 0
	movs	r0, #0
	bl	app_util_critical_region_enter
	.loc 1 338 0
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	ldr	r4, [r3]
	adds	r2, r4, #1
	str	r2, [r3]
	ldr	r1, [sp, #12]
	ldr	r0, [sp, #4]
	bl	nrf_balloc_block2idx
	mov	r3, r0
	strb	r3, [r4]
	.loc 1 340 0
	movs	r0, #0
	bl	app_util_critical_region_exit
	.loc 1 341 0
	nop
	add	sp, sp, #16
.LCFI11:
	@ sp needed
	pop	{r4, pc}
.LFE152:
	.size	nrf_balloc_free, .-nrf_balloc_free
	.section	.debug_frame,"",%progbits
.Lframe0:
	.4byte	.LECIE0-.LSCIE0
.LSCIE0:
	.4byte	0xffffffff
	.byte	0x3
	.ascii	"\000"
	.uleb128 0x1
	.sleb128 -4
	.uleb128 0xe
	.byte	0xc
	.uleb128 0xd
	.uleb128 0
	.align	2
.LECIE0:
.LSFDE0:
	.4byte	.LEFDE0-.LASFDE0
.LASFDE0:
	.4byte	.Lframe0
	.4byte	.LFB148
	.4byte	.LFE148-.LFB148
	.byte	0x4
	.4byte	.LCFI0-.LFB148
	.byte	0xe
	.uleb128 0x8
	.byte	0x4
	.4byte	.LCFI1-.LCFI0
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE0:
.LSFDE2:
	.4byte	.LEFDE2-.LASFDE2
.LASFDE2:
	.4byte	.Lframe0
	.4byte	.LFB149
	.4byte	.LFE149-.LFB149
	.byte	0x4
	.4byte	.LCFI2-.LFB149
	.byte	0xe
	.uleb128 0x8
	.byte	0x4
	.4byte	.LCFI3-.LCFI2
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE2:
.LSFDE4:
	.4byte	.LEFDE4-.LASFDE4
.LASFDE4:
	.4byte	.Lframe0
	.4byte	.LFB150
	.4byte	.LFE150-.LFB150
	.byte	0x4
	.4byte	.LCFI4-.LFB150
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.4byte	.LCFI5-.LCFI4
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE4:
.LSFDE6:
	.4byte	.LEFDE6-.LASFDE6
.LASFDE6:
	.4byte	.Lframe0
	.4byte	.LFB151
	.4byte	.LFE151-.LFB151
	.byte	0x4
	.4byte	.LCFI6-.LFB151
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI7-.LCFI6
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.4byte	.LCFI8-.LCFI7
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE6:
.LSFDE8:
	.4byte	.LEFDE8-.LASFDE8
.LASFDE8:
	.4byte	.Lframe0
	.4byte	.LFB152
	.4byte	.LFE152-.LFB152
	.byte	0x4
	.4byte	.LCFI9-.LFB152
	.byte	0xe
	.uleb128 0x8
	.byte	0x84
	.uleb128 0x2
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI10-.LCFI9
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.4byte	.LCFI11-.LCFI10
	.byte	0xe
	.uleb128 0x8
	.align	2
.LEFDE8:
	.text
.Letext0:
	.file 2 "C:/Program Files/SEGGER/SEGGER Embedded Studio for ARM 4.12/include/stdint.h"
	.file 3 "C:/Program Files/SEGGER/SEGGER Embedded Studio for ARM 4.12/include/__crossworks.h"
	.file 4 "C:/Program Files/SEGGER/SEGGER Embedded Studio for ARM 4.12/include/string.h"
	.file 5 "../../../nRF5_SDK_14.2.0/components/libraries/util/sdk_errors.h"
	.file 6 "C:/Users/paulr/AppData/Local/SEGGER/SEGGER Embedded Studio/v3/packages/CMSIS_5/CMSIS/Core/Include/core_cm4.h"
	.file 7 "C:/Users/paulr/AppData/Local/SEGGER/SEGGER Embedded Studio/v3/packages/nRF/CMSIS/Device/Include/system_nrf52.h"
	.file 8 "../../../nRF5_SDK_14.2.0/components/libraries/util/app_util.h"
	.file 9 "C:/Program Files/SEGGER/SEGGER Embedded Studio for ARM 4.12/include/stdio.h"
	.file 10 "C:\\Users\\paulr\\OneDrive\\Documents\\DARPA\\darpasubt\\darpasubt\\firmware\\dwm1001_dev_source_code\\nRF5_SDK_14.2.0\\components\\libraries\\balloc\\nrf_balloc.h"
	.file 11 "../../../nRF5_SDK_14.2.0/components/libraries/experimental_log/src/nrf_log_internal.h"
	.section	.debug_info,"",%progbits
.Ldebug_info0:
	.4byte	0x849
	.2byte	0x4
	.4byte	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF121
	.byte	0xc
	.4byte	.LASF122
	.4byte	.LASF123
	.4byte	.Ldebug_ranges0+0
	.4byte	0
	.4byte	.Ldebug_line0
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.4byte	.LASF0
	.uleb128 0x3
	.4byte	.LASF3
	.byte	0x2
	.byte	0x30
	.4byte	0x37
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.4byte	.LASF1
	.uleb128 0x4
	.4byte	0x37
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.4byte	.LASF2
	.uleb128 0x3
	.4byte	.LASF4
	.byte	0x2
	.byte	0x36
	.4byte	0x55
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.4byte	.LASF5
	.uleb128 0x3
	.4byte	.LASF6
	.byte	0x2
	.byte	0x37
	.4byte	0x6c
	.uleb128 0x5
	.4byte	0x5c
	.uleb128 0x6
	.byte	0x4
	.byte	0x5
	.ascii	"int\000"
	.uleb128 0x3
	.4byte	.LASF7
	.byte	0x2
	.byte	0x38
	.4byte	0x7e
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.4byte	.LASF8
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.4byte	.LASF9
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.4byte	.LASF10
	.uleb128 0x7
	.byte	0x4
	.uleb128 0x8
	.4byte	.LASF61
	.byte	0x8
	.byte	0x3
	.byte	0x7e
	.4byte	0xba
	.uleb128 0x9
	.4byte	.LASF11
	.byte	0x3
	.byte	0x7f
	.4byte	0x6c
	.byte	0
	.uleb128 0x9
	.4byte	.LASF12
	.byte	0x3
	.byte	0x80
	.4byte	0xba
	.byte	0x4
	.byte	0
	.uleb128 0x2
	.byte	0x4
	.byte	0x5
	.4byte	.LASF13
	.uleb128 0xa
	.4byte	0x6c
	.4byte	0xda
	.uleb128 0xb
	.4byte	0xda
	.uleb128 0xb
	.4byte	0x7e
	.uleb128 0xb
	.4byte	0xec
	.byte	0
	.uleb128 0xc
	.byte	0x4
	.4byte	0xe0
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.4byte	.LASF14
	.uleb128 0x4
	.4byte	0xe0
	.uleb128 0xc
	.byte	0x4
	.4byte	0x95
	.uleb128 0xa
	.4byte	0x6c
	.4byte	0x110
	.uleb128 0xb
	.4byte	0x110
	.uleb128 0xb
	.4byte	0x116
	.uleb128 0xb
	.4byte	0x7e
	.uleb128 0xb
	.4byte	0xec
	.byte	0
	.uleb128 0xc
	.byte	0x4
	.4byte	0x7e
	.uleb128 0xc
	.byte	0x4
	.4byte	0xe7
	.uleb128 0xd
	.byte	0x58
	.byte	0x3
	.byte	0x86
	.4byte	0x2a5
	.uleb128 0x9
	.4byte	.LASF15
	.byte	0x3
	.byte	0x88
	.4byte	0x116
	.byte	0
	.uleb128 0x9
	.4byte	.LASF16
	.byte	0x3
	.byte	0x89
	.4byte	0x116
	.byte	0x4
	.uleb128 0x9
	.4byte	.LASF17
	.byte	0x3
	.byte	0x8a
	.4byte	0x116
	.byte	0x8
	.uleb128 0x9
	.4byte	.LASF18
	.byte	0x3
	.byte	0x8c
	.4byte	0x116
	.byte	0xc
	.uleb128 0x9
	.4byte	.LASF19
	.byte	0x3
	.byte	0x8d
	.4byte	0x116
	.byte	0x10
	.uleb128 0x9
	.4byte	.LASF20
	.byte	0x3
	.byte	0x8e
	.4byte	0x116
	.byte	0x14
	.uleb128 0x9
	.4byte	.LASF21
	.byte	0x3
	.byte	0x8f
	.4byte	0x116
	.byte	0x18
	.uleb128 0x9
	.4byte	.LASF22
	.byte	0x3
	.byte	0x90
	.4byte	0x116
	.byte	0x1c
	.uleb128 0x9
	.4byte	.LASF23
	.byte	0x3
	.byte	0x91
	.4byte	0x116
	.byte	0x20
	.uleb128 0x9
	.4byte	.LASF24
	.byte	0x3
	.byte	0x92
	.4byte	0x116
	.byte	0x24
	.uleb128 0x9
	.4byte	.LASF25
	.byte	0x3
	.byte	0x94
	.4byte	0xe0
	.byte	0x28
	.uleb128 0x9
	.4byte	.LASF26
	.byte	0x3
	.byte	0x95
	.4byte	0xe0
	.byte	0x29
	.uleb128 0x9
	.4byte	.LASF27
	.byte	0x3
	.byte	0x96
	.4byte	0xe0
	.byte	0x2a
	.uleb128 0x9
	.4byte	.LASF28
	.byte	0x3
	.byte	0x97
	.4byte	0xe0
	.byte	0x2b
	.uleb128 0x9
	.4byte	.LASF29
	.byte	0x3
	.byte	0x98
	.4byte	0xe0
	.byte	0x2c
	.uleb128 0x9
	.4byte	.LASF30
	.byte	0x3
	.byte	0x99
	.4byte	0xe0
	.byte	0x2d
	.uleb128 0x9
	.4byte	.LASF31
	.byte	0x3
	.byte	0x9a
	.4byte	0xe0
	.byte	0x2e
	.uleb128 0x9
	.4byte	.LASF32
	.byte	0x3
	.byte	0x9b
	.4byte	0xe0
	.byte	0x2f
	.uleb128 0x9
	.4byte	.LASF33
	.byte	0x3
	.byte	0x9c
	.4byte	0xe0
	.byte	0x30
	.uleb128 0x9
	.4byte	.LASF34
	.byte	0x3
	.byte	0x9d
	.4byte	0xe0
	.byte	0x31
	.uleb128 0x9
	.4byte	.LASF35
	.byte	0x3
	.byte	0x9e
	.4byte	0xe0
	.byte	0x32
	.uleb128 0x9
	.4byte	.LASF36
	.byte	0x3
	.byte	0x9f
	.4byte	0xe0
	.byte	0x33
	.uleb128 0x9
	.4byte	.LASF37
	.byte	0x3
	.byte	0xa0
	.4byte	0xe0
	.byte	0x34
	.uleb128 0x9
	.4byte	.LASF38
	.byte	0x3
	.byte	0xa1
	.4byte	0xe0
	.byte	0x35
	.uleb128 0x9
	.4byte	.LASF39
	.byte	0x3
	.byte	0xa6
	.4byte	0x116
	.byte	0x38
	.uleb128 0x9
	.4byte	.LASF40
	.byte	0x3
	.byte	0xa7
	.4byte	0x116
	.byte	0x3c
	.uleb128 0x9
	.4byte	.LASF41
	.byte	0x3
	.byte	0xa8
	.4byte	0x116
	.byte	0x40
	.uleb128 0x9
	.4byte	.LASF42
	.byte	0x3
	.byte	0xa9
	.4byte	0x116
	.byte	0x44
	.uleb128 0x9
	.4byte	.LASF43
	.byte	0x3
	.byte	0xaa
	.4byte	0x116
	.byte	0x48
	.uleb128 0x9
	.4byte	.LASF44
	.byte	0x3
	.byte	0xab
	.4byte	0x116
	.byte	0x4c
	.uleb128 0x9
	.4byte	.LASF45
	.byte	0x3
	.byte	0xac
	.4byte	0x116
	.byte	0x50
	.uleb128 0x9
	.4byte	.LASF46
	.byte	0x3
	.byte	0xad
	.4byte	0x116
	.byte	0x54
	.byte	0
	.uleb128 0x3
	.4byte	.LASF47
	.byte	0x3
	.byte	0xae
	.4byte	0x11c
	.uleb128 0x4
	.4byte	0x2a5
	.uleb128 0xd
	.byte	0x20
	.byte	0x3
	.byte	0xc4
	.4byte	0x31e
	.uleb128 0x9
	.4byte	.LASF48
	.byte	0x3
	.byte	0xc6
	.4byte	0x332
	.byte	0
	.uleb128 0x9
	.4byte	.LASF49
	.byte	0x3
	.byte	0xc7
	.4byte	0x347
	.byte	0x4
	.uleb128 0x9
	.4byte	.LASF50
	.byte	0x3
	.byte	0xc8
	.4byte	0x347
	.byte	0x8
	.uleb128 0x9
	.4byte	.LASF51
	.byte	0x3
	.byte	0xcb
	.4byte	0x361
	.byte	0xc
	.uleb128 0x9
	.4byte	.LASF52
	.byte	0x3
	.byte	0xcc
	.4byte	0x376
	.byte	0x10
	.uleb128 0x9
	.4byte	.LASF53
	.byte	0x3
	.byte	0xcd
	.4byte	0x376
	.byte	0x14
	.uleb128 0x9
	.4byte	.LASF54
	.byte	0x3
	.byte	0xd0
	.4byte	0x37c
	.byte	0x18
	.uleb128 0x9
	.4byte	.LASF55
	.byte	0x3
	.byte	0xd1
	.4byte	0x382
	.byte	0x1c
	.byte	0
	.uleb128 0xa
	.4byte	0x6c
	.4byte	0x332
	.uleb128 0xb
	.4byte	0x6c
	.uleb128 0xb
	.4byte	0x6c
	.byte	0
	.uleb128 0xc
	.byte	0x4
	.4byte	0x31e
	.uleb128 0xa
	.4byte	0x6c
	.4byte	0x347
	.uleb128 0xb
	.4byte	0x6c
	.byte	0
	.uleb128 0xc
	.byte	0x4
	.4byte	0x338
	.uleb128 0xa
	.4byte	0x6c
	.4byte	0x361
	.uleb128 0xb
	.4byte	0xba
	.uleb128 0xb
	.4byte	0x6c
	.byte	0
	.uleb128 0xc
	.byte	0x4
	.4byte	0x34d
	.uleb128 0xa
	.4byte	0xba
	.4byte	0x376
	.uleb128 0xb
	.4byte	0xba
	.byte	0
	.uleb128 0xc
	.byte	0x4
	.4byte	0x367
	.uleb128 0xc
	.byte	0x4
	.4byte	0xc1
	.uleb128 0xc
	.byte	0x4
	.4byte	0xf2
	.uleb128 0x3
	.4byte	.LASF56
	.byte	0x3
	.byte	0xd2
	.4byte	0x2b5
	.uleb128 0x4
	.4byte	0x388
	.uleb128 0xd
	.byte	0xc
	.byte	0x3
	.byte	0xd4
	.4byte	0x3c5
	.uleb128 0x9
	.4byte	.LASF57
	.byte	0x3
	.byte	0xd5
	.4byte	0x116
	.byte	0
	.uleb128 0x9
	.4byte	.LASF58
	.byte	0x3
	.byte	0xd6
	.4byte	0x3c5
	.byte	0x4
	.uleb128 0x9
	.4byte	.LASF59
	.byte	0x3
	.byte	0xd7
	.4byte	0x3cb
	.byte	0x8
	.byte	0
	.uleb128 0xc
	.byte	0x4
	.4byte	0x2b0
	.uleb128 0xc
	.byte	0x4
	.4byte	0x393
	.uleb128 0x3
	.4byte	.LASF60
	.byte	0x3
	.byte	0xd8
	.4byte	0x398
	.uleb128 0x4
	.4byte	0x3d1
	.uleb128 0x8
	.4byte	.LASF62
	.byte	0x14
	.byte	0x3
	.byte	0xdc
	.4byte	0x3fa
	.uleb128 0x9
	.4byte	.LASF63
	.byte	0x3
	.byte	0xdd
	.4byte	0x3fa
	.byte	0
	.byte	0
	.uleb128 0xe
	.4byte	0x40a
	.4byte	0x40a
	.uleb128 0xf
	.4byte	0x7e
	.byte	0x4
	.byte	0
	.uleb128 0xc
	.byte	0x4
	.4byte	0x3dc
	.uleb128 0x10
	.4byte	.LASF64
	.byte	0x3
	.2byte	0x106
	.4byte	0x3e1
	.uleb128 0x10
	.4byte	.LASF65
	.byte	0x3
	.2byte	0x10d
	.4byte	0x3dc
	.uleb128 0x10
	.4byte	.LASF66
	.byte	0x3
	.2byte	0x110
	.4byte	0x393
	.uleb128 0x10
	.4byte	.LASF67
	.byte	0x3
	.2byte	0x111
	.4byte	0x393
	.uleb128 0xe
	.4byte	0x3e
	.4byte	0x450
	.uleb128 0xf
	.4byte	0x7e
	.byte	0x7f
	.byte	0
	.uleb128 0x4
	.4byte	0x440
	.uleb128 0x10
	.4byte	.LASF68
	.byte	0x3
	.2byte	0x113
	.4byte	0x450
	.uleb128 0xe
	.4byte	0xe7
	.4byte	0x46c
	.uleb128 0x11
	.byte	0
	.uleb128 0x4
	.4byte	0x461
	.uleb128 0x10
	.4byte	.LASF69
	.byte	0x3
	.2byte	0x115
	.4byte	0x46c
	.uleb128 0x10
	.4byte	.LASF70
	.byte	0x3
	.2byte	0x116
	.4byte	0x46c
	.uleb128 0x10
	.4byte	.LASF71
	.byte	0x3
	.2byte	0x117
	.4byte	0x46c
	.uleb128 0x10
	.4byte	.LASF72
	.byte	0x3
	.2byte	0x118
	.4byte	0x46c
	.uleb128 0x10
	.4byte	.LASF73
	.byte	0x3
	.2byte	0x11a
	.4byte	0x46c
	.uleb128 0x10
	.4byte	.LASF74
	.byte	0x3
	.2byte	0x11b
	.4byte	0x46c
	.uleb128 0x10
	.4byte	.LASF75
	.byte	0x3
	.2byte	0x11c
	.4byte	0x46c
	.uleb128 0x10
	.4byte	.LASF76
	.byte	0x3
	.2byte	0x11d
	.4byte	0x46c
	.uleb128 0x10
	.4byte	.LASF77
	.byte	0x3
	.2byte	0x11e
	.4byte	0x46c
	.uleb128 0x10
	.4byte	.LASF78
	.byte	0x3
	.2byte	0x11f
	.4byte	0x46c
	.uleb128 0xa
	.4byte	0x6c
	.4byte	0x4f8
	.uleb128 0xb
	.4byte	0x4f8
	.byte	0
	.uleb128 0xc
	.byte	0x4
	.4byte	0x503
	.uleb128 0x12
	.4byte	.LASF94
	.uleb128 0x4
	.4byte	0x4fe
	.uleb128 0x10
	.4byte	.LASF79
	.byte	0x3
	.2byte	0x135
	.4byte	0x514
	.uleb128 0xc
	.byte	0x4
	.4byte	0x4e9
	.uleb128 0xa
	.4byte	0x6c
	.4byte	0x529
	.uleb128 0xb
	.4byte	0x529
	.byte	0
	.uleb128 0xc
	.byte	0x4
	.4byte	0x4fe
	.uleb128 0x10
	.4byte	.LASF80
	.byte	0x3
	.2byte	0x136
	.4byte	0x53b
	.uleb128 0xc
	.byte	0x4
	.4byte	0x51a
	.uleb128 0x13
	.4byte	.LASF81
	.byte	0x3
	.2byte	0x14d
	.4byte	0x54d
	.uleb128 0xc
	.byte	0x4
	.4byte	0x553
	.uleb128 0xa
	.4byte	0x116
	.4byte	0x562
	.uleb128 0xb
	.4byte	0x6c
	.byte	0
	.uleb128 0x14
	.4byte	.LASF82
	.byte	0x8
	.byte	0x3
	.2byte	0x14f
	.4byte	0x58a
	.uleb128 0x15
	.4byte	.LASF83
	.byte	0x3
	.2byte	0x151
	.4byte	0x541
	.byte	0
	.uleb128 0x15
	.4byte	.LASF84
	.byte	0x3
	.2byte	0x152
	.4byte	0x58a
	.byte	0x4
	.byte	0
	.uleb128 0xc
	.byte	0x4
	.4byte	0x562
	.uleb128 0x13
	.4byte	.LASF85
	.byte	0x3
	.2byte	0x153
	.4byte	0x562
	.uleb128 0x10
	.4byte	.LASF86
	.byte	0x3
	.2byte	0x157
	.4byte	0x5a8
	.uleb128 0xc
	.byte	0x4
	.4byte	0x590
	.uleb128 0x3
	.4byte	.LASF87
	.byte	0x4
	.byte	0x37
	.4byte	0x7e
	.uleb128 0x3
	.4byte	.LASF88
	.byte	0x5
	.byte	0x9d
	.4byte	0x73
	.uleb128 0x10
	.4byte	.LASF89
	.byte	0x6
	.2byte	0x804
	.4byte	0x67
	.uleb128 0x16
	.4byte	.LASF90
	.byte	0x7
	.byte	0x21
	.4byte	0x73
	.uleb128 0x16
	.4byte	.LASF91
	.byte	0x8
	.byte	0x4b
	.4byte	0x73
	.uleb128 0x16
	.4byte	.LASF92
	.byte	0x8
	.byte	0x4c
	.4byte	0x73
	.uleb128 0xc
	.byte	0x4
	.4byte	0x2c
	.uleb128 0x13
	.4byte	.LASF93
	.byte	0x9
	.2byte	0x317
	.4byte	0x603
	.uleb128 0x12
	.4byte	.LASF95
	.uleb128 0x10
	.4byte	.LASF96
	.byte	0x9
	.2byte	0x31b
	.4byte	0x614
	.uleb128 0xc
	.byte	0x4
	.4byte	0x5f7
	.uleb128 0x10
	.4byte	.LASF97
	.byte	0x9
	.2byte	0x31c
	.4byte	0x614
	.uleb128 0x10
	.4byte	.LASF98
	.byte	0x9
	.2byte	0x31d
	.4byte	0x614
	.uleb128 0xd
	.byte	0x8
	.byte	0xa
	.byte	0x5b
	.4byte	0x653
	.uleb128 0x9
	.4byte	.LASF99
	.byte	0xa
	.byte	0x5d
	.4byte	0x5f1
	.byte	0
	.uleb128 0x9
	.4byte	.LASF100
	.byte	0xa
	.byte	0x5e
	.4byte	0x2c
	.byte	0x4
	.byte	0
	.uleb128 0x3
	.4byte	.LASF101
	.byte	0xa
	.byte	0x5f
	.4byte	0x632
	.uleb128 0xd
	.byte	0x14
	.byte	0xa
	.byte	0x62
	.4byte	0x6a3
	.uleb128 0x9
	.4byte	.LASF102
	.byte	0xa
	.byte	0x64
	.4byte	0x6a3
	.byte	0
	.uleb128 0x9
	.4byte	.LASF103
	.byte	0xa
	.byte	0x65
	.4byte	0x5f1
	.byte	0x4
	.uleb128 0x9
	.4byte	.LASF104
	.byte	0xa
	.byte	0x69
	.4byte	0x5f1
	.byte	0x8
	.uleb128 0x9
	.4byte	.LASF105
	.byte	0xa
	.byte	0x6a
	.4byte	0x93
	.byte	0xc
	.uleb128 0x9
	.4byte	.LASF106
	.byte	0xa
	.byte	0x76
	.4byte	0x4a
	.byte	0x10
	.byte	0
	.uleb128 0xc
	.byte	0x4
	.4byte	0x653
	.uleb128 0x3
	.4byte	.LASF107
	.byte	0xa
	.byte	0x7b
	.4byte	0x65e
	.uleb128 0x4
	.4byte	0x6a9
	.uleb128 0x17
	.byte	0xc
	.byte	0xb
	.2byte	0x12b
	.4byte	0x6f7
	.uleb128 0x15
	.4byte	.LASF108
	.byte	0xb
	.2byte	0x12d
	.4byte	0x4a
	.byte	0
	.uleb128 0x15
	.4byte	.LASF109
	.byte	0xb
	.2byte	0x12e
	.4byte	0x4a
	.byte	0x2
	.uleb128 0x15
	.4byte	.LASF110
	.byte	0xb
	.2byte	0x12f
	.4byte	0x73
	.byte	0x4
	.uleb128 0x15
	.4byte	.LASF111
	.byte	0xb
	.2byte	0x130
	.4byte	0x73
	.byte	0x8
	.byte	0
	.uleb128 0x13
	.4byte	.LASF112
	.byte	0xb
	.2byte	0x131
	.4byte	0x6b9
	.uleb128 0x10
	.4byte	.LASF113
	.byte	0xb
	.2byte	0x13b
	.4byte	0x6f7
	.uleb128 0x18
	.4byte	.LASF124
	.byte	0x1
	.2byte	0x10e
	.4byte	.LFB152
	.4byte	.LFE152-.LFB152
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x753
	.uleb128 0x19
	.4byte	.LASF114
	.byte	0x1
	.2byte	0x10e
	.4byte	0x753
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x19
	.4byte	.LASF115
	.byte	0x1
	.2byte	0x10e
	.4byte	0x93
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x1a
	.4byte	.LASF116
	.byte	0x1
	.2byte	0x130
	.4byte	0x93
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.byte	0
	.uleb128 0xc
	.byte	0x4
	.4byte	0x6b4
	.uleb128 0x1b
	.4byte	.LASF118
	.byte	0x1
	.byte	0xea
	.4byte	0x93
	.4byte	.LFB151
	.4byte	.LFE151-.LFB151
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x7a7
	.uleb128 0x1c
	.4byte	.LASF114
	.byte	0x1
	.byte	0xea
	.4byte	0x753
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x1d
	.4byte	.LASF116
	.byte	0x1
	.byte	0xee
	.4byte	0x93
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x1e
	.4byte	.LBB2
	.4byte	.LBE2-.LBB2
	.uleb128 0x1d
	.4byte	.LASF117
	.byte	0x1
	.byte	0xf8
	.4byte	0x2c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -13
	.byte	0
	.byte	0
	.uleb128 0x1f
	.4byte	.LASF119
	.byte	0x1
	.byte	0xc0
	.4byte	0x5b9
	.4byte	.LFB150
	.4byte	.LFE150-.LFB150
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x7dd
	.uleb128 0x1c
	.4byte	.LASF114
	.byte	0x1
	.byte	0xc0
	.4byte	0x753
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x1d
	.4byte	.LASF120
	.byte	0x1
	.byte	0xc2
	.4byte	0x2c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -1
	.byte	0
	.uleb128 0x20
	.4byte	.LASF125
	.byte	0x1
	.byte	0xba
	.4byte	0x2c
	.4byte	.LFB149
	.4byte	.LFE149-.LFB149
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x813
	.uleb128 0x1c
	.4byte	.LASF114
	.byte	0x1
	.byte	0xba
	.4byte	0x753
	.uleb128 0x2
	.byte	0x91
	.sleb128 -4
	.uleb128 0x1c
	.4byte	.LASF116
	.byte	0x1
	.byte	0xba
	.4byte	0x813
	.uleb128 0x2
	.byte	0x91
	.sleb128 -8
	.byte	0
	.uleb128 0xc
	.byte	0x4
	.4byte	0x819
	.uleb128 0x21
	.uleb128 0x22
	.4byte	.LASF126
	.byte	0x1
	.byte	0xad
	.4byte	0x93
	.4byte	.LFB148
	.4byte	.LFE148-.LFB148
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x1c
	.4byte	.LASF114
	.byte	0x1
	.byte	0xad
	.4byte	0x753
	.uleb128 0x2
	.byte	0x91
	.sleb128 -4
	.uleb128 0x23
	.ascii	"idx\000"
	.byte	0x1
	.byte	0xad
	.4byte	0x2c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -5
	.byte	0
	.byte	0
	.section	.debug_abbrev,"",%progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x55
	.uleb128 0x17
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x17
	.uleb128 0x2134
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x35
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x15
	.byte	0x1
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x13
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x21
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x13
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0x13
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x18
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x19
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x1a
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x1b
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1c
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x1d
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x1e
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x1f
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x20
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x21
	.uleb128 0x26
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x22
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x23
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_pubnames,"",%progbits
	.4byte	0x7d
	.2byte	0x2
	.4byte	.Ldebug_info0
	.4byte	0x84d
	.4byte	0x70f
	.ascii	"nrf_balloc_free\000"
	.4byte	0x759
	.ascii	"nrf_balloc_alloc\000"
	.4byte	0x7a7
	.ascii	"nrf_balloc_init\000"
	.4byte	0x7dd
	.ascii	"nrf_balloc_block2idx\000"
	.4byte	0x81a
	.ascii	"nrf_balloc_idx2block\000"
	.4byte	0
	.section	.debug_pubtypes,"",%progbits
	.4byte	0x203
	.2byte	0x2
	.4byte	.Ldebug_info0
	.4byte	0x84d
	.4byte	0x25
	.ascii	"signed char\000"
	.4byte	0x37
	.ascii	"unsigned char\000"
	.4byte	0x2c
	.ascii	"uint8_t\000"
	.4byte	0x43
	.ascii	"short int\000"
	.4byte	0x55
	.ascii	"short unsigned int\000"
	.4byte	0x4a
	.ascii	"uint16_t\000"
	.4byte	0x6c
	.ascii	"int\000"
	.4byte	0x5c
	.ascii	"int32_t\000"
	.4byte	0x7e
	.ascii	"unsigned int\000"
	.4byte	0x73
	.ascii	"uint32_t\000"
	.4byte	0x85
	.ascii	"long long int\000"
	.4byte	0x8c
	.ascii	"long long unsigned int\000"
	.4byte	0xba
	.ascii	"long int\000"
	.4byte	0x95
	.ascii	"__mbstate_s\000"
	.4byte	0xe0
	.ascii	"char\000"
	.4byte	0x2a5
	.ascii	"__RAL_locale_data_t\000"
	.4byte	0x388
	.ascii	"__RAL_locale_codeset_t\000"
	.4byte	0x3d1
	.ascii	"__RAL_locale_t\000"
	.4byte	0x3e1
	.ascii	"__locale_s\000"
	.4byte	0x541
	.ascii	"__RAL_error_decoder_fn_t\000"
	.4byte	0x562
	.ascii	"__RAL_error_decoder_s\000"
	.4byte	0x590
	.ascii	"__RAL_error_decoder_t\000"
	.4byte	0x5ae
	.ascii	"size_t\000"
	.4byte	0x5b9
	.ascii	"ret_code_t\000"
	.4byte	0x5f7
	.ascii	"FILE\000"
	.4byte	0x653
	.ascii	"nrf_balloc_cb_t\000"
	.4byte	0x6a9
	.ascii	"nrf_balloc_t\000"
	.4byte	0x6f7
	.ascii	"nrf_log_module_dynamic_data_t\000"
	.4byte	0
	.section	.debug_aranges,"",%progbits
	.4byte	0x3c
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x4
	.byte	0
	.2byte	0
	.2byte	0
	.4byte	.LFB148
	.4byte	.LFE148-.LFB148
	.4byte	.LFB149
	.4byte	.LFE149-.LFB149
	.4byte	.LFB150
	.4byte	.LFE150-.LFB150
	.4byte	.LFB151
	.4byte	.LFE151-.LFB151
	.4byte	.LFB152
	.4byte	.LFE152-.LFB152
	.4byte	0
	.4byte	0
	.section	.debug_ranges,"",%progbits
.Ldebug_ranges0:
	.4byte	.LFB148
	.4byte	.LFE148
	.4byte	.LFB149
	.4byte	.LFE149
	.4byte	.LFB150
	.4byte	.LFE150
	.4byte	.LFB151
	.4byte	.LFE151
	.4byte	.LFB152
	.4byte	.LFE152
	.4byte	0
	.4byte	0
	.section	.debug_line,"",%progbits
.Ldebug_line0:
	.section	.debug_str,"MS",%progbits,1
.LASF19:
	.ascii	"currency_symbol\000"
.LASF75:
	.ascii	"__RAL_data_utf8_space\000"
.LASF46:
	.ascii	"date_time_format\000"
.LASF87:
	.ascii	"size_t\000"
.LASF119:
	.ascii	"nrf_balloc_init\000"
.LASF66:
	.ascii	"__RAL_codeset_ascii\000"
.LASF81:
	.ascii	"__RAL_error_decoder_fn_t\000"
.LASF27:
	.ascii	"p_cs_precedes\000"
.LASF103:
	.ascii	"p_stack_base\000"
.LASF124:
	.ascii	"nrf_balloc_free\000"
.LASF109:
	.ascii	"order_idx\000"
.LASF111:
	.ascii	"filter_lvls\000"
.LASF62:
	.ascii	"__locale_s\000"
.LASF80:
	.ascii	"__user_get_time_of_day\000"
.LASF89:
	.ascii	"ITM_RxBuffer\000"
.LASF44:
	.ascii	"date_format\000"
.LASF84:
	.ascii	"next\000"
.LASF42:
	.ascii	"abbrev_month_names\000"
.LASF9:
	.ascii	"long long int\000"
.LASF0:
	.ascii	"signed char\000"
.LASF64:
	.ascii	"__RAL_global_locale\000"
.LASF108:
	.ascii	"module_id\000"
.LASF59:
	.ascii	"codeset\000"
.LASF10:
	.ascii	"long long unsigned int\000"
.LASF52:
	.ascii	"__towupper\000"
.LASF102:
	.ascii	"p_cb\000"
.LASF13:
	.ascii	"long int\000"
.LASF47:
	.ascii	"__RAL_locale_data_t\000"
.LASF74:
	.ascii	"__RAL_data_utf8_comma\000"
.LASF18:
	.ascii	"int_curr_symbol\000"
.LASF4:
	.ascii	"uint16_t\000"
.LASF23:
	.ascii	"positive_sign\000"
.LASF33:
	.ascii	"int_p_cs_precedes\000"
.LASF113:
	.ascii	"m_nrf_log_balloc_logs_data_dynamic\000"
.LASF12:
	.ascii	"__wchar\000"
.LASF126:
	.ascii	"nrf_balloc_idx2block\000"
.LASF16:
	.ascii	"thousands_sep\000"
.LASF22:
	.ascii	"mon_grouping\000"
.LASF51:
	.ascii	"__iswctype\000"
.LASF32:
	.ascii	"n_sign_posn\000"
.LASF123:
	.ascii	"C:\\Users\\paulr\\OneDrive\\Documents\\DARPA\\darpa"
	.ascii	"subt\\darpasubt\\firmware\\dwm1001_dev_source_code\\"
	.ascii	"examples\\ds_twr_resp\\SES\000"
.LASF96:
	.ascii	"stdin\000"
.LASF8:
	.ascii	"unsigned int\000"
.LASF106:
	.ascii	"block_size\000"
.LASF35:
	.ascii	"int_p_sep_by_space\000"
.LASF20:
	.ascii	"mon_decimal_point\000"
.LASF43:
	.ascii	"am_pm_indicator\000"
.LASF73:
	.ascii	"__RAL_data_utf8_period\000"
.LASF17:
	.ascii	"grouping\000"
.LASF70:
	.ascii	"__RAL_c_locale_abbrev_day_names\000"
.LASF53:
	.ascii	"__towlower\000"
.LASF78:
	.ascii	"__RAL_data_empty_string\000"
.LASF94:
	.ascii	"timeval\000"
.LASF63:
	.ascii	"__category\000"
.LASF92:
	.ascii	"__StackLimit\000"
.LASF49:
	.ascii	"__toupper\000"
.LASF30:
	.ascii	"n_sep_by_space\000"
.LASF58:
	.ascii	"data\000"
.LASF24:
	.ascii	"negative_sign\000"
.LASF5:
	.ascii	"short unsigned int\000"
.LASF99:
	.ascii	"p_stack_pointer\000"
.LASF57:
	.ascii	"name\000"
.LASF39:
	.ascii	"day_names\000"
.LASF34:
	.ascii	"int_n_cs_precedes\000"
.LASF97:
	.ascii	"stdout\000"
.LASF114:
	.ascii	"p_pool\000"
.LASF60:
	.ascii	"__RAL_locale_t\000"
.LASF45:
	.ascii	"time_format\000"
.LASF117:
	.ascii	"utilization\000"
.LASF31:
	.ascii	"p_sign_posn\000"
.LASF88:
	.ascii	"ret_code_t\000"
.LASF68:
	.ascii	"__RAL_ascii_ctype_map\000"
.LASF112:
	.ascii	"nrf_log_module_dynamic_data_t\000"
.LASF83:
	.ascii	"decode\000"
.LASF100:
	.ascii	"max_utilization\000"
.LASF55:
	.ascii	"__mbtowc\000"
.LASF125:
	.ascii	"nrf_balloc_block2idx\000"
.LASF104:
	.ascii	"p_stack_limit\000"
.LASF101:
	.ascii	"nrf_balloc_cb_t\000"
.LASF29:
	.ascii	"n_cs_precedes\000"
.LASF50:
	.ascii	"__tolower\000"
.LASF105:
	.ascii	"p_memory_begin\000"
.LASF79:
	.ascii	"__user_set_time_of_day\000"
.LASF120:
	.ascii	"pool_size\000"
.LASF90:
	.ascii	"SystemCoreClock\000"
.LASF11:
	.ascii	"__state\000"
.LASF91:
	.ascii	"__StackTop\000"
.LASF6:
	.ascii	"int32_t\000"
.LASF1:
	.ascii	"unsigned char\000"
.LASF107:
	.ascii	"nrf_balloc_t\000"
.LASF69:
	.ascii	"__RAL_c_locale_day_names\000"
.LASF77:
	.ascii	"__RAL_data_utf8_minus\000"
.LASF26:
	.ascii	"frac_digits\000"
.LASF2:
	.ascii	"short int\000"
.LASF115:
	.ascii	"p_element\000"
.LASF86:
	.ascii	"__RAL_error_decoder_head\000"
.LASF122:
	.ascii	"C:\\Users\\paulr\\OneDrive\\Documents\\DARPA\\darpa"
	.ascii	"subt\\darpasubt\\firmware\\dwm1001_dev_source_code\\"
	.ascii	"nRF5_SDK_14.2.0\\components\\libraries\\balloc\\nrf"
	.ascii	"_balloc.c\000"
.LASF93:
	.ascii	"FILE\000"
.LASF21:
	.ascii	"mon_thousands_sep\000"
.LASF121:
	.ascii	"GNU C99 7.3.1 20180622 (release) [ARM/embedded-7-br"
	.ascii	"anch revision 261907] -fmessage-length=0 -mcpu=cort"
	.ascii	"ex-m4 -mlittle-endian -mfloat-abi=hard -mfpu=fpv4-s"
	.ascii	"p-d16 -mthumb -mtp=soft -munaligned-access -std=gnu"
	.ascii	"99 -g2 -gpubnames -fomit-frame-pointer -fno-dwarf2-"
	.ascii	"cfi-asm -fno-builtin -ffunction-sections -fdata-sec"
	.ascii	"tions -fshort-enums -fno-common\000"
.LASF7:
	.ascii	"uint32_t\000"
.LASF37:
	.ascii	"int_p_sign_posn\000"
.LASF14:
	.ascii	"char\000"
.LASF28:
	.ascii	"p_sep_by_space\000"
.LASF116:
	.ascii	"p_block\000"
.LASF65:
	.ascii	"__RAL_c_locale\000"
.LASF56:
	.ascii	"__RAL_locale_codeset_t\000"
.LASF110:
	.ascii	"filter\000"
.LASF38:
	.ascii	"int_n_sign_posn\000"
.LASF98:
	.ascii	"stderr\000"
.LASF71:
	.ascii	"__RAL_c_locale_month_names\000"
.LASF48:
	.ascii	"__isctype\000"
.LASF82:
	.ascii	"__RAL_error_decoder_s\000"
.LASF85:
	.ascii	"__RAL_error_decoder_t\000"
.LASF61:
	.ascii	"__mbstate_s\000"
.LASF3:
	.ascii	"uint8_t\000"
.LASF67:
	.ascii	"__RAL_codeset_utf8\000"
.LASF25:
	.ascii	"int_frac_digits\000"
.LASF41:
	.ascii	"month_names\000"
.LASF36:
	.ascii	"int_n_sep_by_space\000"
.LASF95:
	.ascii	"__RAL_FILE\000"
.LASF40:
	.ascii	"abbrev_day_names\000"
.LASF76:
	.ascii	"__RAL_data_utf8_plus\000"
.LASF72:
	.ascii	"__RAL_c_locale_abbrev_month_names\000"
.LASF118:
	.ascii	"nrf_balloc_alloc\000"
.LASF54:
	.ascii	"__wctomb\000"
.LASF15:
	.ascii	"decimal_point\000"
	.ident	"GCC: (GNU) 7.3.1 20180622 (release) [ARM/embedded-7-branch revision 261907]"
