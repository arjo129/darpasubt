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
	.file	"nrf_drv_clock.c"
	.text
.Ltext0:
	.section	.text.nrf_clock_int_enable,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	nrf_clock_int_enable, %function
nrf_clock_int_enable:
.LFB147:
	.file 1 "../../../nRF5_SDK_14.2.0/components/drivers_nrf/hal/nrf_clock.h"
	.loc 1 287 0
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #8
.LCFI0:
	str	r0, [sp, #4]
	.loc 1 288 0
	mov	r2, #1073741824
	ldr	r3, [sp, #4]
	str	r3, [r2, #772]
	.loc 1 289 0
	nop
	add	sp, sp, #8
.LCFI1:
	@ sp needed
	bx	lr
.LFE147:
	.size	nrf_clock_int_enable, .-nrf_clock_int_enable
	.section	.text.nrf_clock_int_disable,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	nrf_clock_int_disable, %function
nrf_clock_int_disable:
.LFB148:
	.loc 1 292 0
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #8
.LCFI2:
	str	r0, [sp, #4]
	.loc 1 293 0
	mov	r2, #1073741824
	ldr	r3, [sp, #4]
	str	r3, [r2, #776]
	.loc 1 294 0
	nop
	add	sp, sp, #8
.LCFI3:
	@ sp needed
	bx	lr
.LFE148:
	.size	nrf_clock_int_disable, .-nrf_clock_int_disable
	.section	.text.nrf_clock_task_trigger,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	nrf_clock_task_trigger, %function
nrf_clock_task_trigger:
.LFB151:
	.loc 1 307 0
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #8
.LCFI4:
	mov	r3, r0
	strb	r3, [sp, #7]
	.loc 1 308 0
	ldrb	r3, [sp, #7]	@ zero_extendqisi2
	add	r3, r3, #1073741824
	movs	r2, #1
	str	r2, [r3]
	.loc 1 309 0
	nop
	add	sp, sp, #8
.LCFI5:
	@ sp needed
	bx	lr
.LFE151:
	.size	nrf_clock_task_trigger, .-nrf_clock_task_trigger
	.section	.text.nrf_clock_event_clear,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	nrf_clock_event_clear, %function
nrf_clock_event_clear:
.LFB153:
	.loc 1 317 0
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #16
.LCFI6:
	mov	r3, r0
	strh	r3, [sp, #6]	@ movhi
	.loc 1 318 0
	ldrh	r3, [sp, #6]
	add	r3, r3, #1073741824
	movs	r2, #0
	str	r2, [r3]
	.loc 1 320 0
	ldrh	r3, [sp, #6]
	add	r3, r3, #1073741824
	ldr	r3, [r3]
	str	r3, [sp, #12]
	.loc 1 321 0
	ldr	r3, [sp, #12]
	.loc 1 323 0
	nop
	add	sp, sp, #16
.LCFI7:
	@ sp needed
	bx	lr
.LFE153:
	.size	nrf_clock_event_clear, .-nrf_clock_event_clear
	.section	.text.nrf_clock_event_check,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	nrf_clock_event_check, %function
nrf_clock_event_check:
.LFB154:
	.loc 1 326 0
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #8
.LCFI8:
	mov	r3, r0
	strh	r3, [sp, #6]	@ movhi
	.loc 1 327 0
	ldrh	r3, [sp, #6]
	add	r3, r3, #1073741824
	ldr	r3, [r3]
	cmp	r3, #0
	ite	ne
	movne	r3, #1
	moveq	r3, #0
	uxtb	r3, r3
	.loc 1 328 0
	mov	r0, r3
	add	sp, sp, #8
.LCFI9:
	@ sp needed
	bx	lr
.LFE154:
	.size	nrf_clock_event_check, .-nrf_clock_event_check
	.section	.text.nrf_clock_lf_src_set,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	nrf_clock_lf_src_set, %function
nrf_clock_lf_src_set:
.LFB155:
	.loc 1 331 0
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #8
.LCFI10:
	mov	r3, r0
	strb	r3, [sp, #7]
	.loc 1 333 0
	ldrb	r3, [sp, #7]	@ zero_extendqisi2
	.loc 1 332 0
	mov	r2, #1073741824
	.loc 1 333 0
	and	r3, r3, #3
	.loc 1 332 0
	str	r3, [r2, #1304]
	.loc 1 334 0
	nop
	add	sp, sp, #8
.LCFI11:
	@ sp needed
	bx	lr
.LFE155:
	.size	nrf_clock_lf_src_set, .-nrf_clock_lf_src_set
	.section	.text.nrf_clock_lf_is_running,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	nrf_clock_lf_is_running, %function
nrf_clock_lf_is_running:
.LFB159:
	.loc 1 355 0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	.loc 1 356 0
	mov	r3, #1073741824
	ldr	r3, [r3, #1048]
	.loc 1 357 0
	lsrs	r3, r3, #16
	and	r3, r3, #1
	cmp	r3, #0
	ite	ne
	movne	r3, #1
	moveq	r3, #0
	uxtb	r3, r3
	.loc 1 358 0
	mov	r0, r3
	bx	lr
.LFE159:
	.size	nrf_clock_lf_is_running, .-nrf_clock_lf_is_running
	.section	.text.nrf_clock_hf_is_running,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	nrf_clock_hf_is_running, %function
nrf_clock_hf_is_running:
.LFB162:
	.loc 1 374 0
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #8
.LCFI12:
	mov	r3, r0
	strb	r3, [sp, #7]
	.loc 1 375 0
	mov	r3, #1073741824
	ldr	r3, [r3, #1036]
	and	r2, r3, #65537
	.loc 1 376 0
	ldrb	r3, [sp, #7]	@ zero_extendqisi2
	orr	r3, r3, #65536
	.loc 1 375 0
	cmp	r2, r3
	ite	eq
	moveq	r3, #1
	movne	r3, #0
	uxtb	r3, r3
	.loc 1 377 0
	mov	r0, r3
	add	sp, sp, #8
.LCFI13:
	@ sp needed
	bx	lr
.LFE162:
	.size	nrf_clock_hf_is_running, .-nrf_clock_hf_is_running
	.section	.text.nrf_drv_common_irq_enable_check,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	nrf_drv_common_irq_enable_check, %function
nrf_drv_common_irq_enable_check:
.LFB165:
	.file 2 "../../../nRF5_SDK_14.2.0/components/drivers_nrf/common/nrf_drv_common.h"
	.loc 2 296 0
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #8
.LCFI14:
	mov	r3, r0
	strb	r3, [sp, #7]
	.loc 2 297 0
	ldr	r2, .L14
	ldrsb	r3, [sp, #7]
	lsrs	r3, r3, #5
	ldr	r2, [r2, r3, lsl #2]
	.loc 2 298 0
	ldrb	r3, [sp, #7]	@ zero_extendqisi2
	and	r3, r3, #31
	movs	r1, #1
	lsl	r3, r1, r3
	.loc 2 297 0
	ands	r3, r3, r2
	cmp	r3, #0
	ite	ne
	movne	r3, #1
	moveq	r3, #0
	uxtb	r3, r3
	.loc 2 299 0
	mov	r0, r3
	add	sp, sp, #8
.LCFI15:
	@ sp needed
	bx	lr
.L15:
	.align	2
.L14:
	.word	-536813312
.LFE165:
	.size	nrf_drv_common_irq_enable_check, .-nrf_drv_common_irq_enable_check
	.section	.text.nrf_drv_common_power_clock_irq_init,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	nrf_drv_common_power_clock_irq_init, %function
nrf_drv_common_power_clock_irq_init:
.LFB170:
	.loc 2 324 0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
.LCFI16:
	.loc 2 325 0
	movs	r0, #0
	bl	nrf_drv_common_irq_enable_check
	mov	r3, r0
	eor	r3, r3, #1
	uxtb	r3, r3
	cmp	r3, #0
	beq	.L18
	.loc 2 327 0
	movs	r1, #7
	movs	r0, #0
	bl	nrf_drv_common_irq_enable
.L18:
	.loc 2 341 0
	nop
	pop	{r3, pc}
.LFE170:
	.size	nrf_drv_common_power_clock_irq_init, .-nrf_drv_common_power_clock_irq_init
	.section	.bss.m_clock_cb,"aw",%nobits
	.align	2
	.type	m_clock_cb, %object
	.size	m_clock_cb, 20
m_clock_cb:
	.space	20
	.section	.text.lfclk_start,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	lfclk_start, %function
lfclk_start:
.LFB174:
	.file 3 "C:\\Users\\paulr\\OneDrive\\Documents\\DARPA\\darpasubt\\darpasubt\\firmware\\dwm1001_dev_source_code\\nRF5_SDK_14.2.0\\components\\drivers_nrf\\clock\\nrf_drv_clock.c"
	.loc 3 114 0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
.LCFI17:
	.loc 3 115 0
	mov	r0, #260
	bl	nrf_clock_event_clear
	.loc 3 116 0
	movs	r0, #2
	bl	nrf_clock_int_enable
	.loc 3 117 0
	movs	r0, #8
	bl	nrf_clock_task_trigger
	.loc 3 118 0
	nop
	pop	{r3, pc}
.LFE174:
	.size	lfclk_start, .-lfclk_start
	.section	.text.lfclk_stop,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	lfclk_stop, %function
lfclk_stop:
.LFB175:
	.loc 3 123 0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
.LCFI18:
	.loc 3 135 0
	movs	r0, #12
	bl	nrf_clock_task_trigger
	.loc 3 136 0
	nop
.L21:
	.loc 3 136 0 is_stmt 0 discriminator 1
	bl	nrf_clock_lf_is_running
	mov	r3, r0
	cmp	r3, #0
	bne	.L21
	.loc 3 138 0 is_stmt 1
	ldr	r3, .L22
	movs	r2, #0
	strb	r2, [r3, #2]
	.loc 3 139 0
	nop
	pop	{r3, pc}
.L23:
	.align	2
.L22:
	.word	m_clock_cb
.LFE175:
	.size	lfclk_stop, .-lfclk_stop
	.section	.text.hfclk_start,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	hfclk_start, %function
hfclk_start:
.LFB176:
	.loc 3 142 0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
.LCFI19:
	.loc 3 151 0
	mov	r0, #256
	bl	nrf_clock_event_clear
	.loc 3 152 0
	movs	r0, #1
	bl	nrf_clock_int_enable
	.loc 3 153 0
	movs	r0, #0
	bl	nrf_clock_task_trigger
	.loc 3 154 0
	nop
	pop	{r3, pc}
.LFE176:
	.size	hfclk_start, .-hfclk_start
	.section	.text.hfclk_stop,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	hfclk_stop, %function
hfclk_stop:
.LFB177:
	.loc 3 157 0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
.LCFI20:
	.loc 3 166 0
	movs	r0, #4
	bl	nrf_clock_task_trigger
	.loc 3 167 0
	nop
.L26:
	.loc 3 167 0 is_stmt 0 discriminator 1
	movs	r0, #1
	bl	nrf_clock_hf_is_running
	mov	r3, r0
	cmp	r3, #0
	bne	.L26
	.loc 3 169 0 is_stmt 1
	ldr	r3, .L27
	movs	r2, #0
	strb	r2, [r3, #1]
	.loc 3 170 0
	nop
	pop	{r3, pc}
.L28:
	.align	2
.L27:
	.word	m_clock_cb
.LFE177:
	.size	hfclk_stop, .-hfclk_stop
	.section	.text.nrf_drv_clock_init_check,"ax",%progbits
	.align	1
	.global	nrf_drv_clock_init_check
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	nrf_drv_clock_init_check, %function
nrf_drv_clock_init_check:
.LFB178:
	.loc 3 173 0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	.loc 3 174 0
	ldr	r3, .L31
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 3 175 0
	mov	r0, r3
	bx	lr
.L32:
	.align	2
.L31:
	.word	m_clock_cb
.LFE178:
	.size	nrf_drv_clock_init_check, .-nrf_drv_clock_init_check
	.section	.text.nrf_drv_clock_init,"ax",%progbits
	.align	1
	.global	nrf_drv_clock_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	nrf_drv_clock_init, %function
nrf_drv_clock_init:
.LFB179:
	.loc 3 178 0
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI21:
	sub	sp, sp, #12
.LCFI22:
	.loc 3 179 0
	movs	r3, #0
	str	r3, [sp, #4]
	.loc 3 180 0
	ldr	r3, .L38
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L34
	.loc 3 182 0
	movs	r3, #133
	str	r3, [sp, #4]
	b	.L37
.L34:
	.loc 3 186 0
	ldr	r3, .L38
	movs	r2, #0
	str	r2, [r3, #8]
	.loc 3 187 0
	ldr	r3, .L38
	movs	r2, #0
	str	r2, [r3, #4]
	.loc 3 188 0
	ldr	r3, .L38
	movs	r2, #0
	str	r2, [r3, #16]
	.loc 3 189 0
	ldr	r3, .L38
	movs	r2, #0
	str	r2, [r3, #12]
	.loc 3 190 0
	bl	nrf_drv_common_power_clock_irq_init
	.loc 3 195 0
	movs	r0, #1
	bl	nrf_clock_lf_src_set
	.loc 3 202 0
	ldr	r3, .L38
	movs	r2, #1
	strb	r2, [r3]
.L37:
	.loc 3 207 0
	ldr	r3, [sp, #4]
	.loc 3 208 0
	mov	r0, r3
	add	sp, sp, #12
.LCFI23:
	@ sp needed
	ldr	pc, [sp], #4
.L39:
	.align	2
.L38:
	.word	m_clock_cb
.LFE179:
	.size	nrf_drv_clock_init, .-nrf_drv_clock_init
	.section	.text.nrf_drv_clock_uninit,"ax",%progbits
	.align	1
	.global	nrf_drv_clock_uninit
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	nrf_drv_clock_uninit, %function
nrf_drv_clock_uninit:
.LFB180:
	.loc 3 211 0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
.LCFI24:
	.loc 3 213 0
	bl	nrf_drv_common_clock_irq_disable
	.loc 3 214 0
	mov	r0, #-1
	bl	nrf_clock_int_disable
	.loc 3 216 0
	bl	lfclk_stop
	.loc 3 217 0
	bl	hfclk_stop
	.loc 3 218 0
	ldr	r3, .L41
	movs	r2, #0
	strb	r2, [r3]
	.loc 3 220 0
	nop
	pop	{r3, pc}
.L42:
	.align	2
.L41:
	.word	m_clock_cb
.LFE180:
	.size	nrf_drv_clock_uninit, .-nrf_drv_clock_uninit
	.section	.text.item_enqueue,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	item_enqueue, %function
item_enqueue:
.LFB181:
	.loc 3 224 0
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #16
.LCFI25:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 3 225 0
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	str	r3, [sp, #12]
	.loc 3 226 0
	b	.L44
.L47:
	.loc 3 228 0
	ldr	r2, [sp, #12]
	ldr	r3, [sp]
	cmp	r2, r3
	beq	.L48
	.loc 3 232 0
	ldr	r3, [sp, #12]
	ldr	r3, [r3]
	str	r3, [sp, #12]
.L44:
	.loc 3 226 0
	ldr	r3, [sp, #12]
	cmp	r3, #0
	bne	.L47
	.loc 3 235 0
	ldr	r3, [sp, #4]
	ldr	r2, [r3]
	ldr	r3, [sp]
	str	r2, [r3]
	.loc 3 236 0
	ldr	r3, [sp, #4]
	ldr	r2, [sp]
	str	r2, [r3]
	b	.L43
.L48:
	.loc 3 230 0
	nop
.L43:
	.loc 3 237 0
	add	sp, sp, #16
.LCFI26:
	@ sp needed
	bx	lr
.LFE181:
	.size	item_enqueue, .-item_enqueue
	.section	.text.item_dequeue,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	item_dequeue, %function
item_dequeue:
.LFB182:
	.loc 3 240 0
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #16
.LCFI27:
	str	r0, [sp, #4]
	.loc 3 241 0
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	str	r3, [sp, #12]
	.loc 3 242 0
	ldr	r3, [sp, #12]
	cmp	r3, #0
	beq	.L50
	.loc 3 244 0
	ldr	r3, [sp, #12]
	ldr	r2, [r3]
	ldr	r3, [sp, #4]
	str	r2, [r3]
.L50:
	.loc 3 246 0
	ldr	r3, [sp, #12]
	.loc 3 247 0
	mov	r0, r3
	add	sp, sp, #16
.LCFI28:
	@ sp needed
	bx	lr
.LFE182:
	.size	item_dequeue, .-item_dequeue
	.section	.text.nrf_drv_clock_lfclk_request,"ax",%progbits
	.align	1
	.global	nrf_drv_clock_lfclk_request
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	nrf_drv_clock_lfclk_request, %function
nrf_drv_clock_lfclk_request:
.LFB183:
	.loc 3 250 0
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI29:
	sub	sp, sp, #12
.LCFI30:
	str	r0, [sp, #4]
	.loc 3 253 0
	ldr	r3, .L59
	ldrb	r3, [r3, #2]
	uxtb	r3, r3
	cmp	r3, #0
	beq	.L53
	.loc 3 255 0
	ldr	r3, [sp, #4]
	cmp	r3, #0
	beq	.L54
	.loc 3 257 0
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #4]
	movs	r0, #1
	blx	r3
.LVL0:
.L54:
	.loc 3 259 0
	movs	r0, #0
	bl	app_util_critical_region_enter
	.loc 3 260 0
	ldr	r3, .L59
	ldr	r3, [r3, #12]
	adds	r3, r3, #1
	ldr	r2, .L59
	str	r3, [r2, #12]
	.loc 3 261 0
	movs	r0, #0
	bl	app_util_critical_region_exit
	.loc 3 280 0
	b	.L58
.L53:
	.loc 3 265 0
	movs	r0, #0
	bl	app_util_critical_region_enter
	.loc 3 266 0
	ldr	r3, [sp, #4]
	cmp	r3, #0
	beq	.L56
	.loc 3 268 0
	ldr	r1, [sp, #4]
	ldr	r0, .L59+4
	bl	item_enqueue
.L56:
	.loc 3 271 0
	ldr	r3, .L59
	ldr	r3, [r3, #12]
	cmp	r3, #0
	bne	.L57
	.loc 3 273 0
	bl	lfclk_start
.L57:
	.loc 3 275 0
	ldr	r3, .L59
	ldr	r3, [r3, #12]
	adds	r3, r3, #1
	ldr	r2, .L59
	str	r3, [r2, #12]
	.loc 3 276 0
	movs	r0, #0
	bl	app_util_critical_region_exit
.L58:
	.loc 3 280 0
	nop
	add	sp, sp, #12
.LCFI31:
	@ sp needed
	ldr	pc, [sp], #4
.L60:
	.align	2
.L59:
	.word	m_clock_cb
	.word	m_clock_cb+16
.LFE183:
	.size	nrf_drv_clock_lfclk_request, .-nrf_drv_clock_lfclk_request
	.section	.text.nrf_drv_clock_lfclk_release,"ax",%progbits
	.align	1
	.global	nrf_drv_clock_lfclk_release
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	nrf_drv_clock_lfclk_release, %function
nrf_drv_clock_lfclk_release:
.LFB184:
	.loc 3 283 0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
.LCFI32:
	.loc 3 287 0
	movs	r0, #0
	bl	app_util_critical_region_enter
	.loc 3 288 0
	ldr	r3, .L63
	ldr	r3, [r3, #12]
	subs	r3, r3, #1
	ldr	r2, .L63
	str	r3, [r2, #12]
	.loc 3 289 0
	ldr	r3, .L63
	ldr	r3, [r3, #12]
	cmp	r3, #0
	bne	.L62
	.loc 3 291 0
	bl	lfclk_stop
.L62:
	.loc 3 293 0
	movs	r0, #0
	bl	app_util_critical_region_exit
	.loc 3 294 0
	nop
	pop	{r3, pc}
.L64:
	.align	2
.L63:
	.word	m_clock_cb
.LFE184:
	.size	nrf_drv_clock_lfclk_release, .-nrf_drv_clock_lfclk_release
	.section	.text.nrf_drv_clock_lfclk_is_running,"ax",%progbits
	.align	1
	.global	nrf_drv_clock_lfclk_is_running
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	nrf_drv_clock_lfclk_is_running, %function
nrf_drv_clock_lfclk_is_running:
.LFB185:
	.loc 3 297 0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
.LCFI33:
	.loc 3 307 0
	bl	nrf_clock_lf_is_running
	mov	r3, r0
	.loc 3 308 0
	mov	r0, r3
	pop	{r3, pc}
.LFE185:
	.size	nrf_drv_clock_lfclk_is_running, .-nrf_drv_clock_lfclk_is_running
	.section	.text.nrf_drv_clock_hfclk_request,"ax",%progbits
	.align	1
	.global	nrf_drv_clock_hfclk_request
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	nrf_drv_clock_hfclk_request, %function
nrf_drv_clock_hfclk_request:
.LFB186:
	.loc 3 311 0
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI34:
	sub	sp, sp, #12
.LCFI35:
	str	r0, [sp, #4]
	.loc 3 314 0
	ldr	r3, .L74
	ldrb	r3, [r3, #1]
	uxtb	r3, r3
	cmp	r3, #0
	beq	.L68
	.loc 3 316 0
	ldr	r3, [sp, #4]
	cmp	r3, #0
	beq	.L69
	.loc 3 318 0
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #4]
	movs	r0, #0
	blx	r3
.LVL1:
.L69:
	.loc 3 320 0
	movs	r0, #0
	bl	app_util_critical_region_enter
	.loc 3 321 0
	ldr	r3, .L74
	ldr	r3, [r3, #4]
	adds	r3, r3, #1
	ldr	r2, .L74
	str	r3, [r2, #4]
	.loc 3 322 0
	movs	r0, #0
	bl	app_util_critical_region_exit
	.loc 3 341 0
	b	.L73
.L68:
	.loc 3 326 0
	movs	r0, #0
	bl	app_util_critical_region_enter
	.loc 3 327 0
	ldr	r3, [sp, #4]
	cmp	r3, #0
	beq	.L71
	.loc 3 329 0
	ldr	r1, [sp, #4]
	ldr	r0, .L74+4
	bl	item_enqueue
.L71:
	.loc 3 332 0
	ldr	r3, .L74
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne	.L72
	.loc 3 334 0
	bl	hfclk_start
.L72:
	.loc 3 336 0
	ldr	r3, .L74
	ldr	r3, [r3, #4]
	adds	r3, r3, #1
	ldr	r2, .L74
	str	r3, [r2, #4]
	.loc 3 337 0
	movs	r0, #0
	bl	app_util_critical_region_exit
.L73:
	.loc 3 341 0
	nop
	add	sp, sp, #12
.LCFI36:
	@ sp needed
	ldr	pc, [sp], #4
.L75:
	.align	2
.L74:
	.word	m_clock_cb
	.word	m_clock_cb+8
.LFE186:
	.size	nrf_drv_clock_hfclk_request, .-nrf_drv_clock_hfclk_request
	.section	.text.nrf_drv_clock_hfclk_release,"ax",%progbits
	.align	1
	.global	nrf_drv_clock_hfclk_release
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	nrf_drv_clock_hfclk_release, %function
nrf_drv_clock_hfclk_release:
.LFB187:
	.loc 3 344 0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
.LCFI37:
	.loc 3 348 0
	movs	r0, #0
	bl	app_util_critical_region_enter
	.loc 3 349 0
	ldr	r3, .L78
	ldr	r3, [r3, #4]
	subs	r3, r3, #1
	ldr	r2, .L78
	str	r3, [r2, #4]
	.loc 3 350 0
	ldr	r3, .L78
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne	.L77
	.loc 3 352 0
	bl	hfclk_stop
.L77:
	.loc 3 354 0
	movs	r0, #0
	bl	app_util_critical_region_exit
	.loc 3 355 0
	nop
	pop	{r3, pc}
.L79:
	.align	2
.L78:
	.word	m_clock_cb
.LFE187:
	.size	nrf_drv_clock_hfclk_release, .-nrf_drv_clock_hfclk_release
	.section	.text.nrf_drv_clock_hfclk_is_running,"ax",%progbits
	.align	1
	.global	nrf_drv_clock_hfclk_is_running
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	nrf_drv_clock_hfclk_is_running, %function
nrf_drv_clock_hfclk_is_running:
.LFB188:
	.loc 3 358 0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
.LCFI38:
	.loc 3 370 0
	movs	r0, #1
	bl	nrf_clock_hf_is_running
	mov	r3, r0
	.loc 3 371 0
	mov	r0, r3
	pop	{r3, pc}
.LFE188:
	.size	nrf_drv_clock_hfclk_is_running, .-nrf_drv_clock_hfclk_is_running
	.section	.text.nrf_drv_clock_calibration_start,"ax",%progbits
	.align	1
	.global	nrf_drv_clock_calibration_start
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	nrf_drv_clock_calibration_start, %function
nrf_drv_clock_calibration_start:
.LFB189:
	.loc 3 396 0
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #16
.LCFI39:
	mov	r3, r0
	str	r1, [sp]
	strb	r3, [sp, #7]
	.loc 3 397 0
	movs	r3, #0
	str	r3, [sp, #12]
	.loc 3 429 0
	movs	r3, #15
	str	r3, [sp, #12]
	.loc 3 431 0
	ldr	r3, [sp, #12]
	.loc 3 433 0
	mov	r0, r3
	add	sp, sp, #16
.LCFI40:
	@ sp needed
	bx	lr
.LFE189:
	.size	nrf_drv_clock_calibration_start, .-nrf_drv_clock_calibration_start
	.section	.text.nrf_drv_clock_calibration_abort,"ax",%progbits
	.align	1
	.global	nrf_drv_clock_calibration_abort
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	nrf_drv_clock_calibration_abort, %function
nrf_drv_clock_calibration_abort:
.LFB190:
	.loc 3 436 0
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #8
.LCFI41:
	.loc 3 437 0
	movs	r3, #0
	str	r3, [sp, #4]
	.loc 3 464 0
	movs	r3, #15
	str	r3, [sp, #4]
	.loc 3 466 0
	ldr	r3, [sp, #4]
	.loc 3 468 0
	mov	r0, r3
	add	sp, sp, #8
.LCFI42:
	@ sp needed
	bx	lr
.LFE190:
	.size	nrf_drv_clock_calibration_abort, .-nrf_drv_clock_calibration_abort
	.section	.text.nrf_drv_clock_is_calibrating,"ax",%progbits
	.align	1
	.global	nrf_drv_clock_is_calibrating
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	nrf_drv_clock_is_calibrating, %function
nrf_drv_clock_is_calibrating:
.LFB191:
	.loc 3 471 0
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #16
.LCFI43:
	str	r0, [sp, #4]
	.loc 3 472 0
	movs	r3, #0
	str	r3, [sp, #12]
	.loc 3 479 0
	movs	r3, #15
	str	r3, [sp, #12]
	.loc 3 481 0
	ldr	r3, [sp, #12]
	.loc 3 483 0
	mov	r0, r3
	add	sp, sp, #16
.LCFI44:
	@ sp needed
	bx	lr
.LFE191:
	.size	nrf_drv_clock_is_calibrating, .-nrf_drv_clock_is_calibrating
	.section	.text.clock_clk_started_notify,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	clock_clk_started_notify, %function
clock_clk_started_notify:
.LFB192:
	.loc 3 486 0
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI45:
	sub	sp, sp, #20
.LCFI46:
	mov	r3, r0
	strb	r3, [sp, #7]
	.loc 3 488 0
	ldrb	r3, [sp, #7]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L89
	.loc 3 490 0
	ldr	r3, .L96
	str	r3, [sp, #12]
	b	.L93
.L89:
	.loc 3 494 0
	ldr	r3, .L96+4
	str	r3, [sp, #12]
.L93:
.LBB2:
	.loc 3 499 0
	ldr	r0, [sp, #12]
	bl	item_dequeue
	str	r0, [sp, #8]
	.loc 3 500 0
	ldr	r3, [sp, #8]
	cmp	r3, #0
	beq	.L95
	.loc 3 505 0
	ldr	r3, [sp, #8]
	ldr	r3, [r3, #4]
	ldrb	r2, [sp, #7]	@ zero_extendqisi2
	mov	r0, r2
	blx	r3
.LVL2:
.LBE2:
	.loc 3 498 0
	b	.L93
.L95:
.LBB3:
	.loc 3 502 0
	nop
.LBE3:
	.loc 3 507 0
	nop
	add	sp, sp, #20
.LCFI47:
	@ sp needed
	ldr	pc, [sp], #4
.L97:
	.align	2
.L96:
	.word	m_clock_cb+8
	.word	m_clock_cb+16
.LFE192:
	.size	clock_clk_started_notify, .-clock_clk_started_notify
	.section	.text.POWER_CLOCK_IRQHandler,"ax",%progbits
	.align	1
	.global	POWER_CLOCK_IRQHandler
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	POWER_CLOCK_IRQHandler, %function
POWER_CLOCK_IRQHandler:
.LFB193:
	.loc 3 514 0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
.LCFI48:
	.loc 3 515 0
	mov	r0, #256
	bl	nrf_clock_event_check
	mov	r3, r0
	cmp	r3, #0
	beq	.L99
	.loc 3 517 0
	mov	r0, #256
	bl	nrf_clock_event_clear
	.loc 3 519 0
	movs	r0, #1
	bl	nrf_clock_int_disable
	.loc 3 520 0
	ldr	r3, .L102
	movs	r2, #1
	strb	r2, [r3, #1]
	.loc 3 521 0
	movs	r0, #0
	bl	clock_clk_started_notify
.L99:
	.loc 3 523 0
	mov	r0, #260
	bl	nrf_clock_event_check
	mov	r3, r0
	cmp	r3, #0
	beq	.L101
	.loc 3 525 0
	mov	r0, #260
	bl	nrf_clock_event_clear
	.loc 3 527 0
	movs	r0, #2
	bl	nrf_clock_int_disable
	.loc 3 528 0
	ldr	r3, .L102
	movs	r2, #1
	strb	r2, [r3, #2]
	.loc 3 529 0
	movs	r0, #1
	bl	clock_clk_started_notify
.L101:
	.loc 3 555 0
	nop
	pop	{r3, pc}
.L103:
	.align	2
.L102:
	.word	m_clock_cb
.LFE193:
	.size	POWER_CLOCK_IRQHandler, .-POWER_CLOCK_IRQHandler
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
	.4byte	.LFB147
	.4byte	.LFE147-.LFB147
	.byte	0x4
	.4byte	.LCFI0-.LFB147
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
	.4byte	.LFB148
	.4byte	.LFE148-.LFB148
	.byte	0x4
	.4byte	.LCFI2-.LFB148
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
	.4byte	.LFB151
	.4byte	.LFE151-.LFB151
	.byte	0x4
	.4byte	.LCFI4-.LFB151
	.byte	0xe
	.uleb128 0x8
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
	.4byte	.LFB153
	.4byte	.LFE153-.LFB153
	.byte	0x4
	.4byte	.LCFI6-.LFB153
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.4byte	.LCFI7-.LCFI6
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE6:
.LSFDE8:
	.4byte	.LEFDE8-.LASFDE8
.LASFDE8:
	.4byte	.Lframe0
	.4byte	.LFB154
	.4byte	.LFE154-.LFB154
	.byte	0x4
	.4byte	.LCFI8-.LFB154
	.byte	0xe
	.uleb128 0x8
	.byte	0x4
	.4byte	.LCFI9-.LCFI8
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE8:
.LSFDE10:
	.4byte	.LEFDE10-.LASFDE10
.LASFDE10:
	.4byte	.Lframe0
	.4byte	.LFB155
	.4byte	.LFE155-.LFB155
	.byte	0x4
	.4byte	.LCFI10-.LFB155
	.byte	0xe
	.uleb128 0x8
	.byte	0x4
	.4byte	.LCFI11-.LCFI10
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE10:
.LSFDE12:
	.4byte	.LEFDE12-.LASFDE12
.LASFDE12:
	.4byte	.Lframe0
	.4byte	.LFB159
	.4byte	.LFE159-.LFB159
	.align	2
.LEFDE12:
.LSFDE14:
	.4byte	.LEFDE14-.LASFDE14
.LASFDE14:
	.4byte	.Lframe0
	.4byte	.LFB162
	.4byte	.LFE162-.LFB162
	.byte	0x4
	.4byte	.LCFI12-.LFB162
	.byte	0xe
	.uleb128 0x8
	.byte	0x4
	.4byte	.LCFI13-.LCFI12
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE14:
.LSFDE16:
	.4byte	.LEFDE16-.LASFDE16
.LASFDE16:
	.4byte	.Lframe0
	.4byte	.LFB165
	.4byte	.LFE165-.LFB165
	.byte	0x4
	.4byte	.LCFI14-.LFB165
	.byte	0xe
	.uleb128 0x8
	.byte	0x4
	.4byte	.LCFI15-.LCFI14
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE16:
.LSFDE18:
	.4byte	.LEFDE18-.LASFDE18
.LASFDE18:
	.4byte	.Lframe0
	.4byte	.LFB170
	.4byte	.LFE170-.LFB170
	.byte	0x4
	.4byte	.LCFI16-.LFB170
	.byte	0xe
	.uleb128 0x8
	.byte	0x83
	.uleb128 0x2
	.byte	0x8e
	.uleb128 0x1
	.align	2
.LEFDE18:
.LSFDE20:
	.4byte	.LEFDE20-.LASFDE20
.LASFDE20:
	.4byte	.Lframe0
	.4byte	.LFB174
	.4byte	.LFE174-.LFB174
	.byte	0x4
	.4byte	.LCFI17-.LFB174
	.byte	0xe
	.uleb128 0x8
	.byte	0x83
	.uleb128 0x2
	.byte	0x8e
	.uleb128 0x1
	.align	2
.LEFDE20:
.LSFDE22:
	.4byte	.LEFDE22-.LASFDE22
.LASFDE22:
	.4byte	.Lframe0
	.4byte	.LFB175
	.4byte	.LFE175-.LFB175
	.byte	0x4
	.4byte	.LCFI18-.LFB175
	.byte	0xe
	.uleb128 0x8
	.byte	0x83
	.uleb128 0x2
	.byte	0x8e
	.uleb128 0x1
	.align	2
.LEFDE22:
.LSFDE24:
	.4byte	.LEFDE24-.LASFDE24
.LASFDE24:
	.4byte	.Lframe0
	.4byte	.LFB176
	.4byte	.LFE176-.LFB176
	.byte	0x4
	.4byte	.LCFI19-.LFB176
	.byte	0xe
	.uleb128 0x8
	.byte	0x83
	.uleb128 0x2
	.byte	0x8e
	.uleb128 0x1
	.align	2
.LEFDE24:
.LSFDE26:
	.4byte	.LEFDE26-.LASFDE26
.LASFDE26:
	.4byte	.Lframe0
	.4byte	.LFB177
	.4byte	.LFE177-.LFB177
	.byte	0x4
	.4byte	.LCFI20-.LFB177
	.byte	0xe
	.uleb128 0x8
	.byte	0x83
	.uleb128 0x2
	.byte	0x8e
	.uleb128 0x1
	.align	2
.LEFDE26:
.LSFDE28:
	.4byte	.LEFDE28-.LASFDE28
.LASFDE28:
	.4byte	.Lframe0
	.4byte	.LFB178
	.4byte	.LFE178-.LFB178
	.align	2
.LEFDE28:
.LSFDE30:
	.4byte	.LEFDE30-.LASFDE30
.LASFDE30:
	.4byte	.Lframe0
	.4byte	.LFB179
	.4byte	.LFE179-.LFB179
	.byte	0x4
	.4byte	.LCFI21-.LFB179
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI22-.LCFI21
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.4byte	.LCFI23-.LCFI22
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE30:
.LSFDE32:
	.4byte	.LEFDE32-.LASFDE32
.LASFDE32:
	.4byte	.Lframe0
	.4byte	.LFB180
	.4byte	.LFE180-.LFB180
	.byte	0x4
	.4byte	.LCFI24-.LFB180
	.byte	0xe
	.uleb128 0x8
	.byte	0x83
	.uleb128 0x2
	.byte	0x8e
	.uleb128 0x1
	.align	2
.LEFDE32:
.LSFDE34:
	.4byte	.LEFDE34-.LASFDE34
.LASFDE34:
	.4byte	.Lframe0
	.4byte	.LFB181
	.4byte	.LFE181-.LFB181
	.byte	0x4
	.4byte	.LCFI25-.LFB181
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.4byte	.LCFI26-.LCFI25
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE34:
.LSFDE36:
	.4byte	.LEFDE36-.LASFDE36
.LASFDE36:
	.4byte	.Lframe0
	.4byte	.LFB182
	.4byte	.LFE182-.LFB182
	.byte	0x4
	.4byte	.LCFI27-.LFB182
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.4byte	.LCFI28-.LCFI27
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE36:
.LSFDE38:
	.4byte	.LEFDE38-.LASFDE38
.LASFDE38:
	.4byte	.Lframe0
	.4byte	.LFB183
	.4byte	.LFE183-.LFB183
	.byte	0x4
	.4byte	.LCFI29-.LFB183
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI30-.LCFI29
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.4byte	.LCFI31-.LCFI30
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE38:
.LSFDE40:
	.4byte	.LEFDE40-.LASFDE40
.LASFDE40:
	.4byte	.Lframe0
	.4byte	.LFB184
	.4byte	.LFE184-.LFB184
	.byte	0x4
	.4byte	.LCFI32-.LFB184
	.byte	0xe
	.uleb128 0x8
	.byte	0x83
	.uleb128 0x2
	.byte	0x8e
	.uleb128 0x1
	.align	2
.LEFDE40:
.LSFDE42:
	.4byte	.LEFDE42-.LASFDE42
.LASFDE42:
	.4byte	.Lframe0
	.4byte	.LFB185
	.4byte	.LFE185-.LFB185
	.byte	0x4
	.4byte	.LCFI33-.LFB185
	.byte	0xe
	.uleb128 0x8
	.byte	0x83
	.uleb128 0x2
	.byte	0x8e
	.uleb128 0x1
	.align	2
.LEFDE42:
.LSFDE44:
	.4byte	.LEFDE44-.LASFDE44
.LASFDE44:
	.4byte	.Lframe0
	.4byte	.LFB186
	.4byte	.LFE186-.LFB186
	.byte	0x4
	.4byte	.LCFI34-.LFB186
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI35-.LCFI34
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.4byte	.LCFI36-.LCFI35
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE44:
.LSFDE46:
	.4byte	.LEFDE46-.LASFDE46
.LASFDE46:
	.4byte	.Lframe0
	.4byte	.LFB187
	.4byte	.LFE187-.LFB187
	.byte	0x4
	.4byte	.LCFI37-.LFB187
	.byte	0xe
	.uleb128 0x8
	.byte	0x83
	.uleb128 0x2
	.byte	0x8e
	.uleb128 0x1
	.align	2
.LEFDE46:
.LSFDE48:
	.4byte	.LEFDE48-.LASFDE48
.LASFDE48:
	.4byte	.Lframe0
	.4byte	.LFB188
	.4byte	.LFE188-.LFB188
	.byte	0x4
	.4byte	.LCFI38-.LFB188
	.byte	0xe
	.uleb128 0x8
	.byte	0x83
	.uleb128 0x2
	.byte	0x8e
	.uleb128 0x1
	.align	2
.LEFDE48:
.LSFDE50:
	.4byte	.LEFDE50-.LASFDE50
.LASFDE50:
	.4byte	.Lframe0
	.4byte	.LFB189
	.4byte	.LFE189-.LFB189
	.byte	0x4
	.4byte	.LCFI39-.LFB189
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.4byte	.LCFI40-.LCFI39
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE50:
.LSFDE52:
	.4byte	.LEFDE52-.LASFDE52
.LASFDE52:
	.4byte	.Lframe0
	.4byte	.LFB190
	.4byte	.LFE190-.LFB190
	.byte	0x4
	.4byte	.LCFI41-.LFB190
	.byte	0xe
	.uleb128 0x8
	.byte	0x4
	.4byte	.LCFI42-.LCFI41
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE52:
.LSFDE54:
	.4byte	.LEFDE54-.LASFDE54
.LASFDE54:
	.4byte	.Lframe0
	.4byte	.LFB191
	.4byte	.LFE191-.LFB191
	.byte	0x4
	.4byte	.LCFI43-.LFB191
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.4byte	.LCFI44-.LCFI43
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE54:
.LSFDE56:
	.4byte	.LEFDE56-.LASFDE56
.LASFDE56:
	.4byte	.Lframe0
	.4byte	.LFB192
	.4byte	.LFE192-.LFB192
	.byte	0x4
	.4byte	.LCFI45-.LFB192
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI46-.LCFI45
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.4byte	.LCFI47-.LCFI46
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE56:
.LSFDE58:
	.4byte	.LEFDE58-.LASFDE58
.LASFDE58:
	.4byte	.Lframe0
	.4byte	.LFB193
	.4byte	.LFE193-.LFB193
	.byte	0x4
	.4byte	.LCFI48-.LFB193
	.byte	0xe
	.uleb128 0x8
	.byte	0x83
	.uleb128 0x2
	.byte	0x8e
	.uleb128 0x1
	.align	2
.LEFDE58:
	.text
.Letext0:
	.file 4 "C:/Program Files/SEGGER/SEGGER Embedded Studio for ARM 4.12/include/stdint.h"
	.file 5 "C:/Program Files/SEGGER/SEGGER Embedded Studio for ARM 4.12/include/__crossworks.h"
	.file 6 "../../../nRF5_SDK_14.2.0/components/libraries/util/sdk_errors.h"
	.file 7 "C:/Users/paulr/AppData/Local/SEGGER/SEGGER Embedded Studio/v3/packages/nRF/CMSIS/Device/Include/nrf52.h"
	.file 8 "C:/Users/paulr/AppData/Local/SEGGER/SEGGER Embedded Studio/v3/packages/CMSIS_5/CMSIS/Core/Include/core_cm4.h"
	.file 9 "C:/Users/paulr/AppData/Local/SEGGER/SEGGER Embedded Studio/v3/packages/nRF/CMSIS/Device/Include/system_nrf52.h"
	.file 10 "../../../nRF5_SDK_14.2.0/components/libraries/util/app_util.h"
	.file 11 "C:/Program Files/SEGGER/SEGGER Embedded Studio for ARM 4.12/include/stdio.h"
	.file 12 "C:\\Users\\paulr\\OneDrive\\Documents\\DARPA\\darpasubt\\darpasubt\\firmware\\dwm1001_dev_source_code\\nRF5_SDK_14.2.0\\components\\drivers_nrf\\clock\\nrf_drv_clock.h"
	.file 13 "../../../nRF5_SDK_14.2.0/components/libraries/experimental_log/src/nrf_log_internal.h"
	.section	.debug_info,"",%progbits
.Ldebug_info0:
	.4byte	0x11a0
	.2byte	0x4
	.4byte	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF278
	.byte	0xc
	.4byte	.LASF279
	.4byte	.LASF280
	.4byte	.Ldebug_ranges0+0x18
	.4byte	0
	.4byte	.Ldebug_line0
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.4byte	.LASF0
	.uleb128 0x3
	.4byte	.LASF3
	.byte	0x4
	.byte	0x30
	.4byte	0x3c
	.uleb128 0x4
	.4byte	0x2c
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.4byte	.LASF1
	.uleb128 0x5
	.4byte	0x3c
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.4byte	.LASF2
	.uleb128 0x3
	.4byte	.LASF4
	.byte	0x4
	.byte	0x36
	.4byte	0x5a
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.4byte	.LASF5
	.uleb128 0x3
	.4byte	.LASF6
	.byte	0x4
	.byte	0x37
	.4byte	0x71
	.uleb128 0x4
	.4byte	0x61
	.uleb128 0x6
	.byte	0x4
	.byte	0x5
	.ascii	"int\000"
	.uleb128 0x3
	.4byte	.LASF7
	.byte	0x4
	.byte	0x38
	.4byte	0x8d
	.uleb128 0x4
	.4byte	0x78
	.uleb128 0x5
	.4byte	0x83
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
	.4byte	.LASF61
	.byte	0x8
	.byte	0x5
	.byte	0x7e
	.4byte	0xc7
	.uleb128 0x8
	.4byte	.LASF11
	.byte	0x5
	.byte	0x7f
	.4byte	0x71
	.byte	0
	.uleb128 0x8
	.4byte	.LASF12
	.byte	0x5
	.byte	0x80
	.4byte	0xc7
	.byte	0x4
	.byte	0
	.uleb128 0x2
	.byte	0x4
	.byte	0x5
	.4byte	.LASF13
	.uleb128 0x9
	.4byte	0x71
	.4byte	0xe7
	.uleb128 0xa
	.4byte	0xe7
	.uleb128 0xa
	.4byte	0x8d
	.uleb128 0xa
	.4byte	0xf9
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.4byte	0xed
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.4byte	.LASF14
	.uleb128 0x5
	.4byte	0xed
	.uleb128 0xb
	.byte	0x4
	.4byte	0xa2
	.uleb128 0x9
	.4byte	0x71
	.4byte	0x11d
	.uleb128 0xa
	.4byte	0x11d
	.uleb128 0xa
	.4byte	0x123
	.uleb128 0xa
	.4byte	0x8d
	.uleb128 0xa
	.4byte	0xf9
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.4byte	0x8d
	.uleb128 0xb
	.byte	0x4
	.4byte	0xf4
	.uleb128 0xc
	.byte	0x58
	.byte	0x5
	.byte	0x86
	.4byte	0x2b2
	.uleb128 0x8
	.4byte	.LASF15
	.byte	0x5
	.byte	0x88
	.4byte	0x123
	.byte	0
	.uleb128 0x8
	.4byte	.LASF16
	.byte	0x5
	.byte	0x89
	.4byte	0x123
	.byte	0x4
	.uleb128 0x8
	.4byte	.LASF17
	.byte	0x5
	.byte	0x8a
	.4byte	0x123
	.byte	0x8
	.uleb128 0x8
	.4byte	.LASF18
	.byte	0x5
	.byte	0x8c
	.4byte	0x123
	.byte	0xc
	.uleb128 0x8
	.4byte	.LASF19
	.byte	0x5
	.byte	0x8d
	.4byte	0x123
	.byte	0x10
	.uleb128 0x8
	.4byte	.LASF20
	.byte	0x5
	.byte	0x8e
	.4byte	0x123
	.byte	0x14
	.uleb128 0x8
	.4byte	.LASF21
	.byte	0x5
	.byte	0x8f
	.4byte	0x123
	.byte	0x18
	.uleb128 0x8
	.4byte	.LASF22
	.byte	0x5
	.byte	0x90
	.4byte	0x123
	.byte	0x1c
	.uleb128 0x8
	.4byte	.LASF23
	.byte	0x5
	.byte	0x91
	.4byte	0x123
	.byte	0x20
	.uleb128 0x8
	.4byte	.LASF24
	.byte	0x5
	.byte	0x92
	.4byte	0x123
	.byte	0x24
	.uleb128 0x8
	.4byte	.LASF25
	.byte	0x5
	.byte	0x94
	.4byte	0xed
	.byte	0x28
	.uleb128 0x8
	.4byte	.LASF26
	.byte	0x5
	.byte	0x95
	.4byte	0xed
	.byte	0x29
	.uleb128 0x8
	.4byte	.LASF27
	.byte	0x5
	.byte	0x96
	.4byte	0xed
	.byte	0x2a
	.uleb128 0x8
	.4byte	.LASF28
	.byte	0x5
	.byte	0x97
	.4byte	0xed
	.byte	0x2b
	.uleb128 0x8
	.4byte	.LASF29
	.byte	0x5
	.byte	0x98
	.4byte	0xed
	.byte	0x2c
	.uleb128 0x8
	.4byte	.LASF30
	.byte	0x5
	.byte	0x99
	.4byte	0xed
	.byte	0x2d
	.uleb128 0x8
	.4byte	.LASF31
	.byte	0x5
	.byte	0x9a
	.4byte	0xed
	.byte	0x2e
	.uleb128 0x8
	.4byte	.LASF32
	.byte	0x5
	.byte	0x9b
	.4byte	0xed
	.byte	0x2f
	.uleb128 0x8
	.4byte	.LASF33
	.byte	0x5
	.byte	0x9c
	.4byte	0xed
	.byte	0x30
	.uleb128 0x8
	.4byte	.LASF34
	.byte	0x5
	.byte	0x9d
	.4byte	0xed
	.byte	0x31
	.uleb128 0x8
	.4byte	.LASF35
	.byte	0x5
	.byte	0x9e
	.4byte	0xed
	.byte	0x32
	.uleb128 0x8
	.4byte	.LASF36
	.byte	0x5
	.byte	0x9f
	.4byte	0xed
	.byte	0x33
	.uleb128 0x8
	.4byte	.LASF37
	.byte	0x5
	.byte	0xa0
	.4byte	0xed
	.byte	0x34
	.uleb128 0x8
	.4byte	.LASF38
	.byte	0x5
	.byte	0xa1
	.4byte	0xed
	.byte	0x35
	.uleb128 0x8
	.4byte	.LASF39
	.byte	0x5
	.byte	0xa6
	.4byte	0x123
	.byte	0x38
	.uleb128 0x8
	.4byte	.LASF40
	.byte	0x5
	.byte	0xa7
	.4byte	0x123
	.byte	0x3c
	.uleb128 0x8
	.4byte	.LASF41
	.byte	0x5
	.byte	0xa8
	.4byte	0x123
	.byte	0x40
	.uleb128 0x8
	.4byte	.LASF42
	.byte	0x5
	.byte	0xa9
	.4byte	0x123
	.byte	0x44
	.uleb128 0x8
	.4byte	.LASF43
	.byte	0x5
	.byte	0xaa
	.4byte	0x123
	.byte	0x48
	.uleb128 0x8
	.4byte	.LASF44
	.byte	0x5
	.byte	0xab
	.4byte	0x123
	.byte	0x4c
	.uleb128 0x8
	.4byte	.LASF45
	.byte	0x5
	.byte	0xac
	.4byte	0x123
	.byte	0x50
	.uleb128 0x8
	.4byte	.LASF46
	.byte	0x5
	.byte	0xad
	.4byte	0x123
	.byte	0x54
	.byte	0
	.uleb128 0x3
	.4byte	.LASF47
	.byte	0x5
	.byte	0xae
	.4byte	0x129
	.uleb128 0x5
	.4byte	0x2b2
	.uleb128 0xc
	.byte	0x20
	.byte	0x5
	.byte	0xc4
	.4byte	0x32b
	.uleb128 0x8
	.4byte	.LASF48
	.byte	0x5
	.byte	0xc6
	.4byte	0x33f
	.byte	0
	.uleb128 0x8
	.4byte	.LASF49
	.byte	0x5
	.byte	0xc7
	.4byte	0x354
	.byte	0x4
	.uleb128 0x8
	.4byte	.LASF50
	.byte	0x5
	.byte	0xc8
	.4byte	0x354
	.byte	0x8
	.uleb128 0x8
	.4byte	.LASF51
	.byte	0x5
	.byte	0xcb
	.4byte	0x36e
	.byte	0xc
	.uleb128 0x8
	.4byte	.LASF52
	.byte	0x5
	.byte	0xcc
	.4byte	0x383
	.byte	0x10
	.uleb128 0x8
	.4byte	.LASF53
	.byte	0x5
	.byte	0xcd
	.4byte	0x383
	.byte	0x14
	.uleb128 0x8
	.4byte	.LASF54
	.byte	0x5
	.byte	0xd0
	.4byte	0x389
	.byte	0x18
	.uleb128 0x8
	.4byte	.LASF55
	.byte	0x5
	.byte	0xd1
	.4byte	0x38f
	.byte	0x1c
	.byte	0
	.uleb128 0x9
	.4byte	0x71
	.4byte	0x33f
	.uleb128 0xa
	.4byte	0x71
	.uleb128 0xa
	.4byte	0x71
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.4byte	0x32b
	.uleb128 0x9
	.4byte	0x71
	.4byte	0x354
	.uleb128 0xa
	.4byte	0x71
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.4byte	0x345
	.uleb128 0x9
	.4byte	0x71
	.4byte	0x36e
	.uleb128 0xa
	.4byte	0xc7
	.uleb128 0xa
	.4byte	0x71
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.4byte	0x35a
	.uleb128 0x9
	.4byte	0xc7
	.4byte	0x383
	.uleb128 0xa
	.4byte	0xc7
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.4byte	0x374
	.uleb128 0xb
	.byte	0x4
	.4byte	0xce
	.uleb128 0xb
	.byte	0x4
	.4byte	0xff
	.uleb128 0x3
	.4byte	.LASF56
	.byte	0x5
	.byte	0xd2
	.4byte	0x2c2
	.uleb128 0x5
	.4byte	0x395
	.uleb128 0xc
	.byte	0xc
	.byte	0x5
	.byte	0xd4
	.4byte	0x3d2
	.uleb128 0x8
	.4byte	.LASF57
	.byte	0x5
	.byte	0xd5
	.4byte	0x123
	.byte	0
	.uleb128 0x8
	.4byte	.LASF58
	.byte	0x5
	.byte	0xd6
	.4byte	0x3d2
	.byte	0x4
	.uleb128 0x8
	.4byte	.LASF59
	.byte	0x5
	.byte	0xd7
	.4byte	0x3d8
	.byte	0x8
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.4byte	0x2bd
	.uleb128 0xb
	.byte	0x4
	.4byte	0x3a0
	.uleb128 0x3
	.4byte	.LASF60
	.byte	0x5
	.byte	0xd8
	.4byte	0x3a5
	.uleb128 0x5
	.4byte	0x3de
	.uleb128 0x7
	.4byte	.LASF62
	.byte	0x14
	.byte	0x5
	.byte	0xdc
	.4byte	0x407
	.uleb128 0x8
	.4byte	.LASF63
	.byte	0x5
	.byte	0xdd
	.4byte	0x407
	.byte	0
	.byte	0
	.uleb128 0xd
	.4byte	0x417
	.4byte	0x417
	.uleb128 0xe
	.4byte	0x8d
	.byte	0x4
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.4byte	0x3e9
	.uleb128 0xf
	.4byte	.LASF64
	.byte	0x5
	.2byte	0x106
	.4byte	0x3ee
	.uleb128 0xf
	.4byte	.LASF65
	.byte	0x5
	.2byte	0x10d
	.4byte	0x3e9
	.uleb128 0xf
	.4byte	.LASF66
	.byte	0x5
	.2byte	0x110
	.4byte	0x3a0
	.uleb128 0xf
	.4byte	.LASF67
	.byte	0x5
	.2byte	0x111
	.4byte	0x3a0
	.uleb128 0xd
	.4byte	0x43
	.4byte	0x45d
	.uleb128 0xe
	.4byte	0x8d
	.byte	0x7f
	.byte	0
	.uleb128 0x5
	.4byte	0x44d
	.uleb128 0xf
	.4byte	.LASF68
	.byte	0x5
	.2byte	0x113
	.4byte	0x45d
	.uleb128 0xd
	.4byte	0xf4
	.4byte	0x479
	.uleb128 0x10
	.byte	0
	.uleb128 0x5
	.4byte	0x46e
	.uleb128 0xf
	.4byte	.LASF69
	.byte	0x5
	.2byte	0x115
	.4byte	0x479
	.uleb128 0xf
	.4byte	.LASF70
	.byte	0x5
	.2byte	0x116
	.4byte	0x479
	.uleb128 0xf
	.4byte	.LASF71
	.byte	0x5
	.2byte	0x117
	.4byte	0x479
	.uleb128 0xf
	.4byte	.LASF72
	.byte	0x5
	.2byte	0x118
	.4byte	0x479
	.uleb128 0xf
	.4byte	.LASF73
	.byte	0x5
	.2byte	0x11a
	.4byte	0x479
	.uleb128 0xf
	.4byte	.LASF74
	.byte	0x5
	.2byte	0x11b
	.4byte	0x479
	.uleb128 0xf
	.4byte	.LASF75
	.byte	0x5
	.2byte	0x11c
	.4byte	0x479
	.uleb128 0xf
	.4byte	.LASF76
	.byte	0x5
	.2byte	0x11d
	.4byte	0x479
	.uleb128 0xf
	.4byte	.LASF77
	.byte	0x5
	.2byte	0x11e
	.4byte	0x479
	.uleb128 0xf
	.4byte	.LASF78
	.byte	0x5
	.2byte	0x11f
	.4byte	0x479
	.uleb128 0x9
	.4byte	0x71
	.4byte	0x505
	.uleb128 0xa
	.4byte	0x505
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.4byte	0x510
	.uleb128 0x11
	.4byte	.LASF179
	.uleb128 0x5
	.4byte	0x50b
	.uleb128 0xf
	.4byte	.LASF79
	.byte	0x5
	.2byte	0x135
	.4byte	0x521
	.uleb128 0xb
	.byte	0x4
	.4byte	0x4f6
	.uleb128 0x9
	.4byte	0x71
	.4byte	0x536
	.uleb128 0xa
	.4byte	0x536
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.4byte	0x50b
	.uleb128 0xf
	.4byte	.LASF80
	.byte	0x5
	.2byte	0x136
	.4byte	0x548
	.uleb128 0xb
	.byte	0x4
	.4byte	0x527
	.uleb128 0x12
	.4byte	.LASF81
	.byte	0x5
	.2byte	0x14d
	.4byte	0x55a
	.uleb128 0xb
	.byte	0x4
	.4byte	0x560
	.uleb128 0x9
	.4byte	0x123
	.4byte	0x56f
	.uleb128 0xa
	.4byte	0x71
	.byte	0
	.uleb128 0x13
	.4byte	.LASF82
	.byte	0x8
	.byte	0x5
	.2byte	0x14f
	.4byte	0x597
	.uleb128 0x14
	.4byte	.LASF83
	.byte	0x5
	.2byte	0x151
	.4byte	0x54e
	.byte	0
	.uleb128 0x14
	.4byte	.LASF84
	.byte	0x5
	.2byte	0x152
	.4byte	0x597
	.byte	0x4
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.4byte	0x56f
	.uleb128 0x12
	.4byte	.LASF85
	.byte	0x5
	.2byte	0x153
	.4byte	0x56f
	.uleb128 0xf
	.4byte	.LASF86
	.byte	0x5
	.2byte	0x157
	.4byte	0x5b5
	.uleb128 0xb
	.byte	0x4
	.4byte	0x59d
	.uleb128 0x3
	.4byte	.LASF87
	.byte	0x6
	.byte	0x9d
	.4byte	0x78
	.uleb128 0x15
	.byte	0x5
	.byte	0x1
	.4byte	0x25
	.byte	0x7
	.byte	0x41
	.4byte	0x6ee
	.uleb128 0x16
	.4byte	.LASF88
	.sleb128 -15
	.uleb128 0x16
	.4byte	.LASF89
	.sleb128 -14
	.uleb128 0x16
	.4byte	.LASF90
	.sleb128 -13
	.uleb128 0x16
	.4byte	.LASF91
	.sleb128 -12
	.uleb128 0x16
	.4byte	.LASF92
	.sleb128 -11
	.uleb128 0x16
	.4byte	.LASF93
	.sleb128 -10
	.uleb128 0x16
	.4byte	.LASF94
	.sleb128 -5
	.uleb128 0x16
	.4byte	.LASF95
	.sleb128 -4
	.uleb128 0x16
	.4byte	.LASF96
	.sleb128 -2
	.uleb128 0x16
	.4byte	.LASF97
	.sleb128 -1
	.uleb128 0x17
	.4byte	.LASF98
	.byte	0
	.uleb128 0x17
	.4byte	.LASF99
	.byte	0x1
	.uleb128 0x17
	.4byte	.LASF100
	.byte	0x2
	.uleb128 0x17
	.4byte	.LASF101
	.byte	0x3
	.uleb128 0x17
	.4byte	.LASF102
	.byte	0x4
	.uleb128 0x17
	.4byte	.LASF103
	.byte	0x5
	.uleb128 0x17
	.4byte	.LASF104
	.byte	0x6
	.uleb128 0x17
	.4byte	.LASF105
	.byte	0x7
	.uleb128 0x17
	.4byte	.LASF106
	.byte	0x8
	.uleb128 0x17
	.4byte	.LASF107
	.byte	0x9
	.uleb128 0x17
	.4byte	.LASF108
	.byte	0xa
	.uleb128 0x17
	.4byte	.LASF109
	.byte	0xb
	.uleb128 0x17
	.4byte	.LASF110
	.byte	0xc
	.uleb128 0x17
	.4byte	.LASF111
	.byte	0xd
	.uleb128 0x17
	.4byte	.LASF112
	.byte	0xe
	.uleb128 0x17
	.4byte	.LASF113
	.byte	0xf
	.uleb128 0x17
	.4byte	.LASF114
	.byte	0x10
	.uleb128 0x17
	.4byte	.LASF115
	.byte	0x11
	.uleb128 0x17
	.4byte	.LASF116
	.byte	0x12
	.uleb128 0x17
	.4byte	.LASF117
	.byte	0x13
	.uleb128 0x17
	.4byte	.LASF118
	.byte	0x14
	.uleb128 0x17
	.4byte	.LASF119
	.byte	0x15
	.uleb128 0x17
	.4byte	.LASF120
	.byte	0x16
	.uleb128 0x17
	.4byte	.LASF121
	.byte	0x17
	.uleb128 0x17
	.4byte	.LASF122
	.byte	0x18
	.uleb128 0x17
	.4byte	.LASF123
	.byte	0x19
	.uleb128 0x17
	.4byte	.LASF124
	.byte	0x1a
	.uleb128 0x17
	.4byte	.LASF125
	.byte	0x1b
	.uleb128 0x17
	.4byte	.LASF126
	.byte	0x1c
	.uleb128 0x17
	.4byte	.LASF127
	.byte	0x1d
	.uleb128 0x17
	.4byte	.LASF128
	.byte	0x20
	.uleb128 0x17
	.4byte	.LASF129
	.byte	0x21
	.uleb128 0x17
	.4byte	.LASF130
	.byte	0x22
	.uleb128 0x17
	.4byte	.LASF131
	.byte	0x23
	.uleb128 0x17
	.4byte	.LASF132
	.byte	0x24
	.uleb128 0x17
	.4byte	.LASF133
	.byte	0x25
	.uleb128 0x17
	.4byte	.LASF134
	.byte	0x26
	.byte	0
	.uleb128 0x3
	.4byte	.LASF135
	.byte	0x7
	.byte	0x75
	.4byte	0x5c6
	.uleb128 0x18
	.2byte	0xe04
	.byte	0x8
	.2byte	0x196
	.4byte	0x7b5
	.uleb128 0x14
	.4byte	.LASF136
	.byte	0x8
	.2byte	0x198
	.4byte	0x7c5
	.byte	0
	.uleb128 0x14
	.4byte	.LASF137
	.byte	0x8
	.2byte	0x199
	.4byte	0x7ca
	.byte	0x20
	.uleb128 0x14
	.4byte	.LASF138
	.byte	0x8
	.2byte	0x19a
	.4byte	0x7c5
	.byte	0x80
	.uleb128 0x14
	.4byte	.LASF139
	.byte	0x8
	.2byte	0x19b
	.4byte	0x7ca
	.byte	0xa0
	.uleb128 0x19
	.4byte	.LASF140
	.byte	0x8
	.2byte	0x19c
	.4byte	0x7c5
	.2byte	0x100
	.uleb128 0x19
	.4byte	.LASF141
	.byte	0x8
	.2byte	0x19d
	.4byte	0x7ca
	.2byte	0x120
	.uleb128 0x19
	.4byte	.LASF142
	.byte	0x8
	.2byte	0x19e
	.4byte	0x7c5
	.2byte	0x180
	.uleb128 0x19
	.4byte	.LASF143
	.byte	0x8
	.2byte	0x19f
	.4byte	0x7ca
	.2byte	0x1a0
	.uleb128 0x19
	.4byte	.LASF144
	.byte	0x8
	.2byte	0x1a0
	.4byte	0x7c5
	.2byte	0x200
	.uleb128 0x19
	.4byte	.LASF145
	.byte	0x8
	.2byte	0x1a1
	.4byte	0x7da
	.2byte	0x220
	.uleb128 0x1a
	.ascii	"IP\000"
	.byte	0x8
	.2byte	0x1a2
	.4byte	0x7fa
	.2byte	0x300
	.uleb128 0x19
	.4byte	.LASF146
	.byte	0x8
	.2byte	0x1a3
	.4byte	0x7ff
	.2byte	0x3f0
	.uleb128 0x19
	.4byte	.LASF147
	.byte	0x8
	.2byte	0x1a4
	.4byte	0x83
	.2byte	0xe00
	.byte	0
	.uleb128 0xd
	.4byte	0x83
	.4byte	0x7c5
	.uleb128 0xe
	.4byte	0x8d
	.byte	0x7
	.byte	0
	.uleb128 0x4
	.4byte	0x7b5
	.uleb128 0xd
	.4byte	0x78
	.4byte	0x7da
	.uleb128 0xe
	.4byte	0x8d
	.byte	0x17
	.byte	0
	.uleb128 0xd
	.4byte	0x78
	.4byte	0x7ea
	.uleb128 0xe
	.4byte	0x8d
	.byte	0x37
	.byte	0
	.uleb128 0xd
	.4byte	0x37
	.4byte	0x7fa
	.uleb128 0xe
	.4byte	0x8d
	.byte	0xef
	.byte	0
	.uleb128 0x4
	.4byte	0x7ea
	.uleb128 0xd
	.4byte	0x78
	.4byte	0x810
	.uleb128 0x1b
	.4byte	0x8d
	.2byte	0x283
	.byte	0
	.uleb128 0x12
	.4byte	.LASF148
	.byte	0x8
	.2byte	0x1a5
	.4byte	0x6f9
	.uleb128 0xf
	.4byte	.LASF149
	.byte	0x8
	.2byte	0x804
	.4byte	0x6c
	.uleb128 0x1c
	.4byte	.LASF150
	.byte	0x9
	.byte	0x21
	.4byte	0x78
	.uleb128 0xd
	.4byte	0x88
	.4byte	0x843
	.uleb128 0xe
	.4byte	0x8d
	.byte	0x7
	.byte	0
	.uleb128 0x5
	.4byte	0x833
	.uleb128 0x4
	.4byte	0x843
	.uleb128 0x18
	.2byte	0x560
	.byte	0x7
	.2byte	0x24e
	.4byte	0x9e6
	.uleb128 0x14
	.4byte	.LASF151
	.byte	0x7
	.2byte	0x24f
	.4byte	0x83
	.byte	0
	.uleb128 0x14
	.4byte	.LASF152
	.byte	0x7
	.2byte	0x250
	.4byte	0x83
	.byte	0x4
	.uleb128 0x14
	.4byte	.LASF153
	.byte	0x7
	.2byte	0x251
	.4byte	0x83
	.byte	0x8
	.uleb128 0x14
	.4byte	.LASF154
	.byte	0x7
	.2byte	0x252
	.4byte	0x83
	.byte	0xc
	.uleb128 0x14
	.4byte	.LASF155
	.byte	0x7
	.2byte	0x253
	.4byte	0x83
	.byte	0x10
	.uleb128 0x14
	.4byte	.LASF156
	.byte	0x7
	.2byte	0x254
	.4byte	0x83
	.byte	0x14
	.uleb128 0x14
	.4byte	.LASF157
	.byte	0x7
	.2byte	0x255
	.4byte	0x83
	.byte	0x18
	.uleb128 0x14
	.4byte	.LASF137
	.byte	0x7
	.2byte	0x256
	.4byte	0x9fb
	.byte	0x1c
	.uleb128 0x19
	.4byte	.LASF158
	.byte	0x7
	.2byte	0x257
	.4byte	0x83
	.2byte	0x100
	.uleb128 0x19
	.4byte	.LASF159
	.byte	0x7
	.2byte	0x258
	.4byte	0x83
	.2byte	0x104
	.uleb128 0x19
	.4byte	.LASF160
	.byte	0x7
	.2byte	0x259
	.4byte	0x88
	.2byte	0x108
	.uleb128 0x19
	.4byte	.LASF161
	.byte	0x7
	.2byte	0x25a
	.4byte	0x83
	.2byte	0x10c
	.uleb128 0x19
	.4byte	.LASF162
	.byte	0x7
	.2byte	0x25b
	.4byte	0x83
	.2byte	0x110
	.uleb128 0x19
	.4byte	.LASF141
	.byte	0x7
	.2byte	0x25c
	.4byte	0xa15
	.2byte	0x114
	.uleb128 0x19
	.4byte	.LASF163
	.byte	0x7
	.2byte	0x25d
	.4byte	0x83
	.2byte	0x304
	.uleb128 0x19
	.4byte	.LASF164
	.byte	0x7
	.2byte	0x25e
	.4byte	0x83
	.2byte	0x308
	.uleb128 0x19
	.4byte	.LASF143
	.byte	0x7
	.2byte	0x25f
	.4byte	0xa2f
	.2byte	0x30c
	.uleb128 0x19
	.4byte	.LASF165
	.byte	0x7
	.2byte	0x260
	.4byte	0x88
	.2byte	0x408
	.uleb128 0x19
	.4byte	.LASF166
	.byte	0x7
	.2byte	0x261
	.4byte	0x88
	.2byte	0x40c
	.uleb128 0x19
	.4byte	.LASF145
	.byte	0x7
	.2byte	0x262
	.4byte	0x88
	.2byte	0x410
	.uleb128 0x19
	.4byte	.LASF167
	.byte	0x7
	.2byte	0x263
	.4byte	0x88
	.2byte	0x414
	.uleb128 0x19
	.4byte	.LASF168
	.byte	0x7
	.2byte	0x264
	.4byte	0x88
	.2byte	0x418
	.uleb128 0x19
	.4byte	.LASF169
	.byte	0x7
	.2byte	0x265
	.4byte	0x88
	.2byte	0x41c
	.uleb128 0x19
	.4byte	.LASF146
	.byte	0x7
	.2byte	0x266
	.4byte	0xa49
	.2byte	0x420
	.uleb128 0x19
	.4byte	.LASF170
	.byte	0x7
	.2byte	0x267
	.4byte	0x83
	.2byte	0x518
	.uleb128 0x19
	.4byte	.LASF171
	.byte	0x7
	.2byte	0x268
	.4byte	0xa63
	.2byte	0x51c
	.uleb128 0x19
	.4byte	.LASF172
	.byte	0x7
	.2byte	0x269
	.4byte	0x83
	.2byte	0x538
	.uleb128 0x19
	.4byte	.LASF173
	.byte	0x7
	.2byte	0x26a
	.4byte	0x848
	.2byte	0x53c
	.uleb128 0x19
	.4byte	.LASF174
	.byte	0x7
	.2byte	0x26b
	.4byte	0x83
	.2byte	0x55c
	.byte	0
	.uleb128 0xd
	.4byte	0x88
	.4byte	0x9f6
	.uleb128 0xe
	.4byte	0x8d
	.byte	0x38
	.byte	0
	.uleb128 0x5
	.4byte	0x9e6
	.uleb128 0x4
	.4byte	0x9f6
	.uleb128 0xd
	.4byte	0x88
	.4byte	0xa10
	.uleb128 0xe
	.4byte	0x8d
	.byte	0x7b
	.byte	0
	.uleb128 0x5
	.4byte	0xa00
	.uleb128 0x4
	.4byte	0xa10
	.uleb128 0xd
	.4byte	0x88
	.4byte	0xa2a
	.uleb128 0xe
	.4byte	0x8d
	.byte	0x3e
	.byte	0
	.uleb128 0x5
	.4byte	0xa1a
	.uleb128 0x4
	.4byte	0xa2a
	.uleb128 0xd
	.4byte	0x88
	.4byte	0xa44
	.uleb128 0xe
	.4byte	0x8d
	.byte	0x3d
	.byte	0
	.uleb128 0x5
	.4byte	0xa34
	.uleb128 0x4
	.4byte	0xa44
	.uleb128 0xd
	.4byte	0x88
	.4byte	0xa5e
	.uleb128 0xe
	.4byte	0x8d
	.byte	0x6
	.byte	0
	.uleb128 0x5
	.4byte	0xa4e
	.uleb128 0x4
	.4byte	0xa5e
	.uleb128 0x12
	.4byte	.LASF175
	.byte	0x7
	.2byte	0x26c
	.4byte	0x84d
	.uleb128 0x1c
	.4byte	.LASF176
	.byte	0xa
	.byte	0x4b
	.4byte	0x78
	.uleb128 0x1c
	.4byte	.LASF177
	.byte	0xa
	.byte	0x4c
	.4byte	0x78
	.uleb128 0x12
	.4byte	.LASF178
	.byte	0xb
	.2byte	0x317
	.4byte	0xa96
	.uleb128 0x11
	.4byte	.LASF180
	.uleb128 0xf
	.4byte	.LASF181
	.byte	0xb
	.2byte	0x31b
	.4byte	0xaa7
	.uleb128 0xb
	.byte	0x4
	.4byte	0xa8a
	.uleb128 0xf
	.4byte	.LASF182
	.byte	0xb
	.2byte	0x31c
	.4byte	0xaa7
	.uleb128 0xf
	.4byte	.LASF183
	.byte	0xb
	.2byte	0x31d
	.4byte	0xaa7
	.uleb128 0x15
	.byte	0x7
	.byte	0x1
	.4byte	0x3c
	.byte	0x1
	.byte	0x43
	.4byte	0xae5
	.uleb128 0x17
	.4byte	.LASF184
	.byte	0
	.uleb128 0x17
	.4byte	.LASF185
	.byte	0x1
	.uleb128 0x17
	.4byte	.LASF186
	.byte	0x2
	.byte	0
	.uleb128 0x3
	.4byte	.LASF187
	.byte	0x1
	.byte	0x47
	.4byte	0xac5
	.uleb128 0x15
	.byte	0x7
	.byte	0x1
	.4byte	0x3c
	.byte	0x1
	.byte	0x4d
	.4byte	0xb0a
	.uleb128 0x17
	.4byte	.LASF188
	.byte	0
	.uleb128 0x17
	.4byte	.LASF189
	.byte	0x1
	.byte	0
	.uleb128 0x3
	.4byte	.LASF190
	.byte	0x1
	.byte	0x50
	.4byte	0xaf0
	.uleb128 0x15
	.byte	0x7
	.byte	0x1
	.4byte	0x3c
	.byte	0x1
	.byte	0x60
	.4byte	0xb3b
	.uleb128 0x17
	.4byte	.LASF191
	.byte	0x1
	.uleb128 0x17
	.4byte	.LASF192
	.byte	0x2
	.uleb128 0x17
	.4byte	.LASF193
	.byte	0x8
	.uleb128 0x17
	.4byte	.LASF194
	.byte	0x10
	.byte	0
	.uleb128 0x15
	.byte	0x7
	.byte	0x1
	.4byte	0x3c
	.byte	0x1
	.byte	0x6e
	.4byte	0xb73
	.uleb128 0x17
	.4byte	.LASF195
	.byte	0
	.uleb128 0x17
	.4byte	.LASF196
	.byte	0x4
	.uleb128 0x17
	.4byte	.LASF197
	.byte	0x8
	.uleb128 0x17
	.4byte	.LASF198
	.byte	0xc
	.uleb128 0x17
	.4byte	.LASF199
	.byte	0x10
	.uleb128 0x17
	.4byte	.LASF200
	.byte	0x14
	.uleb128 0x17
	.4byte	.LASF201
	.byte	0x18
	.byte	0
	.uleb128 0x3
	.4byte	.LASF202
	.byte	0x1
	.byte	0x76
	.4byte	0xb3b
	.uleb128 0x15
	.byte	0x7
	.byte	0x2
	.4byte	0x5a
	.byte	0x1
	.byte	0x7c
	.4byte	0xba8
	.uleb128 0x1d
	.4byte	.LASF203
	.2byte	0x100
	.uleb128 0x1d
	.4byte	.LASF204
	.2byte	0x104
	.uleb128 0x1d
	.4byte	.LASF205
	.2byte	0x10c
	.uleb128 0x1d
	.4byte	.LASF206
	.2byte	0x110
	.byte	0
	.uleb128 0x3
	.4byte	.LASF207
	.byte	0x1
	.byte	0x81
	.4byte	0xb7e
	.uleb128 0x15
	.byte	0x7
	.byte	0x1
	.4byte	0x3c
	.byte	0xc
	.byte	0x49
	.4byte	0xbd9
	.uleb128 0x17
	.4byte	.LASF208
	.byte	0
	.uleb128 0x17
	.4byte	.LASF209
	.byte	0x1
	.uleb128 0x17
	.4byte	.LASF210
	.byte	0x2
	.uleb128 0x17
	.4byte	.LASF211
	.byte	0x3
	.byte	0
	.uleb128 0x3
	.4byte	.LASF212
	.byte	0xc
	.byte	0x4e
	.4byte	0xbb3
	.uleb128 0x3
	.4byte	.LASF213
	.byte	0xc
	.byte	0x55
	.4byte	0xbef
	.uleb128 0xb
	.byte	0x4
	.4byte	0xbf5
	.uleb128 0x1e
	.4byte	0xc00
	.uleb128 0xa
	.4byte	0xbd9
	.byte	0
	.uleb128 0x3
	.4byte	.LASF214
	.byte	0xc
	.byte	0x58
	.4byte	0xc10
	.uleb128 0x4
	.4byte	0xc00
	.uleb128 0x7
	.4byte	.LASF215
	.byte	0x8
	.byte	0xc
	.byte	0x5a
	.4byte	0xc35
	.uleb128 0x8
	.4byte	.LASF216
	.byte	0xc
	.byte	0x5c
	.4byte	0xc35
	.byte	0
	.uleb128 0x8
	.4byte	.LASF217
	.byte	0xc
	.byte	0x5d
	.4byte	0xbe4
	.byte	0x4
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.4byte	0xc00
	.uleb128 0x1f
	.byte	0xc
	.byte	0xd
	.2byte	0x12b
	.4byte	0xc79
	.uleb128 0x14
	.4byte	.LASF218
	.byte	0xd
	.2byte	0x12d
	.4byte	0x4f
	.byte	0
	.uleb128 0x14
	.4byte	.LASF219
	.byte	0xd
	.2byte	0x12e
	.4byte	0x4f
	.byte	0x2
	.uleb128 0x14
	.4byte	.LASF220
	.byte	0xd
	.2byte	0x12f
	.4byte	0x78
	.byte	0x4
	.uleb128 0x14
	.4byte	.LASF221
	.byte	0xd
	.2byte	0x130
	.4byte	0x78
	.byte	0x8
	.byte	0
	.uleb128 0x12
	.4byte	.LASF222
	.byte	0xd
	.2byte	0x131
	.4byte	0xc3b
	.uleb128 0xf
	.4byte	.LASF223
	.byte	0xd
	.2byte	0x13b
	.4byte	0xc79
	.uleb128 0xc
	.byte	0x14
	.byte	0x3
	.byte	0x5c
	.4byte	0xcee
	.uleb128 0x8
	.4byte	.LASF224
	.byte	0x3
	.byte	0x5e
	.4byte	0xcee
	.byte	0
	.uleb128 0x8
	.4byte	.LASF225
	.byte	0x3
	.byte	0x5f
	.4byte	0xcf5
	.byte	0x1
	.uleb128 0x8
	.4byte	.LASF226
	.byte	0x3
	.byte	0x60
	.4byte	0xcf5
	.byte	0x2
	.uleb128 0x8
	.4byte	.LASF227
	.byte	0x3
	.byte	0x61
	.4byte	0x83
	.byte	0x4
	.uleb128 0x8
	.4byte	.LASF228
	.byte	0x3
	.byte	0x62
	.4byte	0xcfa
	.byte	0x8
	.uleb128 0x8
	.4byte	.LASF229
	.byte	0x3
	.byte	0x63
	.4byte	0x83
	.byte	0xc
	.uleb128 0x8
	.4byte	.LASF230
	.byte	0x3
	.byte	0x64
	.4byte	0xcfa
	.byte	0x10
	.byte	0
	.uleb128 0x2
	.byte	0x1
	.byte	0x2
	.4byte	.LASF231
	.uleb128 0x4
	.4byte	0xcee
	.uleb128 0xb
	.byte	0x4
	.4byte	0xc0b
	.uleb128 0x3
	.4byte	.LASF232
	.byte	0x3
	.byte	0x6a
	.4byte	0xc91
	.uleb128 0x20
	.4byte	.LASF233
	.byte	0x3
	.byte	0x6c
	.4byte	0xd00
	.uleb128 0x5
	.byte	0x3
	.4byte	m_clock_cb
	.uleb128 0x21
	.4byte	.LASF245
	.byte	0x3
	.2byte	0x200
	.4byte	.LFB193
	.4byte	.LFE193-.LFB193
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x22
	.4byte	.LASF253
	.byte	0x3
	.2byte	0x1e5
	.4byte	.LFB192
	.4byte	.LFE192-.LFB192
	.uleb128 0x1
	.byte	0x9c
	.4byte	0xd78
	.uleb128 0x23
	.4byte	.LASF236
	.byte	0x3
	.2byte	0x1e5
	.4byte	0xbd9
	.uleb128 0x2
	.byte	0x91
	.sleb128 -17
	.uleb128 0x24
	.4byte	.LASF234
	.byte	0x3
	.2byte	0x1e7
	.4byte	0xd78
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x25
	.4byte	.Ldebug_ranges0+0
	.uleb128 0x24
	.4byte	.LASF235
	.byte	0x3
	.2byte	0x1f3
	.4byte	0xc35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.byte	0
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.4byte	0xc35
	.uleb128 0x26
	.4byte	.LASF239
	.byte	0x3
	.2byte	0x1d6
	.4byte	0x5bb
	.4byte	.LFB191
	.4byte	.LFE191-.LFB191
	.uleb128 0x1
	.byte	0x9c
	.4byte	0xdc0
	.uleb128 0x23
	.4byte	.LASF237
	.byte	0x3
	.2byte	0x1d6
	.4byte	0xdc0
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x24
	.4byte	.LASF238
	.byte	0x3
	.2byte	0x1d8
	.4byte	0x5bb
	.uleb128 0x2
	.byte	0x91
	.sleb128 -4
	.uleb128 0x27
	.4byte	.LASF241
	.4byte	0xdd6
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.4byte	0xcee
	.uleb128 0xd
	.4byte	0xf4
	.4byte	0xdd6
	.uleb128 0xe
	.4byte	0x8d
	.byte	0x1c
	.byte	0
	.uleb128 0x5
	.4byte	0xdc6
	.uleb128 0x26
	.4byte	.LASF240
	.byte	0x3
	.2byte	0x1b3
	.4byte	0x5bb
	.4byte	.LFB190
	.4byte	.LFE190-.LFB190
	.uleb128 0x1
	.byte	0x9c
	.4byte	0xe0e
	.uleb128 0x24
	.4byte	.LASF238
	.byte	0x3
	.2byte	0x1b5
	.4byte	0x5bb
	.uleb128 0x2
	.byte	0x91
	.sleb128 -4
	.uleb128 0x27
	.4byte	.LASF241
	.4byte	0xe1e
	.byte	0
	.uleb128 0xd
	.4byte	0xf4
	.4byte	0xe1e
	.uleb128 0xe
	.4byte	0x8d
	.byte	0x1f
	.byte	0
	.uleb128 0x5
	.4byte	0xe0e
	.uleb128 0x26
	.4byte	.LASF242
	.byte	0x3
	.2byte	0x18b
	.4byte	0x5bb
	.4byte	.LFB189
	.4byte	.LFE189-.LFB189
	.uleb128 0x1
	.byte	0x9c
	.4byte	0xe74
	.uleb128 0x23
	.4byte	.LASF243
	.byte	0x3
	.2byte	0x18b
	.4byte	0x2c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0x23
	.4byte	.LASF244
	.byte	0x3
	.2byte	0x18b
	.4byte	0xbe4
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x24
	.4byte	.LASF238
	.byte	0x3
	.2byte	0x18d
	.4byte	0x5bb
	.uleb128 0x2
	.byte	0x91
	.sleb128 -4
	.uleb128 0x27
	.4byte	.LASF241
	.4byte	0xe1e
	.byte	0
	.uleb128 0x28
	.4byte	.LASF248
	.byte	0x3
	.2byte	0x165
	.4byte	0xcee
	.4byte	.LFB188
	.4byte	.LFE188-.LFB188
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x21
	.4byte	.LASF246
	.byte	0x3
	.2byte	0x157
	.4byte	.LFB187
	.4byte	.LFE187-.LFB187
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x29
	.4byte	.LASF251
	.byte	0x3
	.2byte	0x136
	.4byte	.LFB186
	.4byte	.LFE186-.LFB186
	.uleb128 0x1
	.byte	0x9c
	.4byte	0xec2
	.uleb128 0x23
	.4byte	.LASF247
	.byte	0x3
	.2byte	0x136
	.4byte	0xc35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.byte	0
	.uleb128 0x28
	.4byte	.LASF249
	.byte	0x3
	.2byte	0x128
	.4byte	0xcee
	.4byte	.LFB185
	.4byte	.LFE185-.LFB185
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x21
	.4byte	.LASF250
	.byte	0x3
	.2byte	0x11a
	.4byte	.LFB184
	.4byte	.LFE184-.LFB184
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x2a
	.4byte	.LASF252
	.byte	0x3
	.byte	0xf9
	.4byte	.LFB183
	.4byte	.LFE183-.LFB183
	.uleb128 0x1
	.byte	0x9c
	.4byte	0xf0e
	.uleb128 0x2b
	.4byte	.LASF247
	.byte	0x3
	.byte	0xf9
	.4byte	0xc35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.byte	0
	.uleb128 0x2c
	.4byte	.LASF263
	.byte	0x3
	.byte	0xef
	.4byte	0xc35
	.4byte	.LFB182
	.4byte	.LFE182-.LFB182
	.uleb128 0x1
	.byte	0x9c
	.4byte	0xf44
	.uleb128 0x2b
	.4byte	.LASF234
	.byte	0x3
	.byte	0xef
	.4byte	0xd78
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x20
	.4byte	.LASF235
	.byte	0x3
	.byte	0xf1
	.4byte	0xc35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -4
	.byte	0
	.uleb128 0x2d
	.4byte	.LASF254
	.byte	0x3
	.byte	0xde
	.4byte	.LFB181
	.4byte	.LFE181-.LFB181
	.uleb128 0x1
	.byte	0x9c
	.4byte	0xf84
	.uleb128 0x2b
	.4byte	.LASF234
	.byte	0x3
	.byte	0xde
	.4byte	0xd78
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x2b
	.4byte	.LASF235
	.byte	0x3
	.byte	0xdf
	.4byte	0xc35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x20
	.4byte	.LASF216
	.byte	0x3
	.byte	0xe1
	.4byte	0xc35
	.uleb128 0x2
	.byte	0x91
	.sleb128 -4
	.byte	0
	.uleb128 0x2e
	.4byte	.LASF255
	.byte	0x3
	.byte	0xd2
	.4byte	.LFB180
	.4byte	.LFE180-.LFB180
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x2f
	.4byte	.LASF256
	.byte	0x3
	.byte	0xb1
	.4byte	0x5bb
	.4byte	.LFB179
	.4byte	.LFE179-.LFB179
	.uleb128 0x1
	.byte	0x9c
	.4byte	0xfc6
	.uleb128 0x20
	.4byte	.LASF238
	.byte	0x3
	.byte	0xb3
	.4byte	0x5bb
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x27
	.4byte	.LASF241
	.4byte	0xfd6
	.byte	0
	.uleb128 0xd
	.4byte	0xf4
	.4byte	0xfd6
	.uleb128 0xe
	.4byte	0x8d
	.byte	0x12
	.byte	0
	.uleb128 0x5
	.4byte	0xfc6
	.uleb128 0x30
	.4byte	.LASF257
	.byte	0x3
	.byte	0xac
	.4byte	0xcee
	.4byte	.LFB178
	.4byte	.LFE178-.LFB178
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x31
	.4byte	.LASF258
	.byte	0x3
	.byte	0x9c
	.4byte	.LFB177
	.4byte	.LFE177-.LFB177
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x31
	.4byte	.LASF259
	.byte	0x3
	.byte	0x8d
	.4byte	.LFB176
	.4byte	.LFE176-.LFB176
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x31
	.4byte	.LASF260
	.byte	0x3
	.byte	0x7a
	.4byte	.LFB175
	.4byte	.LFE175-.LFB175
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x31
	.4byte	.LASF261
	.byte	0x3
	.byte	0x71
	.4byte	.LFB174
	.4byte	.LFE174-.LFB174
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x32
	.4byte	.LASF262
	.byte	0x2
	.2byte	0x143
	.4byte	.LFB170
	.4byte	.LFE170-.LFB170
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x33
	.4byte	.LASF264
	.byte	0x2
	.2byte	0x127
	.4byte	0xcee
	.4byte	.LFB165
	.4byte	.LFE165-.LFB165
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1070
	.uleb128 0x23
	.4byte	.LASF265
	.byte	0x2
	.2byte	0x127
	.4byte	0x6ee
	.uleb128 0x2
	.byte	0x91
	.sleb128 -1
	.byte	0
	.uleb128 0x33
	.4byte	.LASF266
	.byte	0x1
	.2byte	0x175
	.4byte	0xcee
	.4byte	.LFB162
	.4byte	.LFE162-.LFB162
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x109a
	.uleb128 0x23
	.4byte	.LASF267
	.byte	0x1
	.2byte	0x175
	.4byte	0xb0a
	.uleb128 0x2
	.byte	0x91
	.sleb128 -1
	.byte	0
	.uleb128 0x34
	.4byte	.LASF281
	.byte	0x1
	.2byte	0x162
	.4byte	0xcee
	.4byte	.LFB159
	.4byte	.LFE159-.LFB159
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x35
	.4byte	.LASF268
	.byte	0x1
	.2byte	0x14a
	.4byte	.LFB155
	.4byte	.LFE155-.LFB155
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x10d6
	.uleb128 0x23
	.4byte	.LASF269
	.byte	0x1
	.2byte	0x14a
	.4byte	0xae5
	.uleb128 0x2
	.byte	0x91
	.sleb128 -1
	.byte	0
	.uleb128 0x33
	.4byte	.LASF270
	.byte	0x1
	.2byte	0x145
	.4byte	0xcee
	.4byte	.LFB154
	.4byte	.LFE154-.LFB154
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1100
	.uleb128 0x23
	.4byte	.LASF271
	.byte	0x1
	.2byte	0x145
	.4byte	0xba8
	.uleb128 0x2
	.byte	0x91
	.sleb128 -2
	.byte	0
	.uleb128 0x35
	.4byte	.LASF272
	.byte	0x1
	.2byte	0x13c
	.4byte	.LFB153
	.4byte	.LFE153-.LFB153
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1135
	.uleb128 0x23
	.4byte	.LASF271
	.byte	0x1
	.2byte	0x13c
	.4byte	0xba8
	.uleb128 0x2
	.byte	0x91
	.sleb128 -10
	.uleb128 0x24
	.4byte	.LASF273
	.byte	0x1
	.2byte	0x140
	.4byte	0x83
	.uleb128 0x2
	.byte	0x91
	.sleb128 -4
	.byte	0
	.uleb128 0x35
	.4byte	.LASF274
	.byte	0x1
	.2byte	0x132
	.4byte	.LFB151
	.4byte	.LFE151-.LFB151
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x115b
	.uleb128 0x23
	.4byte	.LASF275
	.byte	0x1
	.2byte	0x132
	.4byte	0xb73
	.uleb128 0x2
	.byte	0x91
	.sleb128 -1
	.byte	0
	.uleb128 0x35
	.4byte	.LASF276
	.byte	0x1
	.2byte	0x123
	.4byte	.LFB148
	.4byte	.LFE148-.LFB148
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1181
	.uleb128 0x23
	.4byte	.LASF277
	.byte	0x1
	.2byte	0x123
	.4byte	0x78
	.uleb128 0x2
	.byte	0x91
	.sleb128 -4
	.byte	0
	.uleb128 0x36
	.4byte	.LASF282
	.byte	0x1
	.2byte	0x11e
	.4byte	.LFB147
	.4byte	.LFE147-.LFB147
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x23
	.4byte	.LASF277
	.byte	0x1
	.2byte	0x11e
	.4byte	0x78
	.uleb128 0x2
	.byte	0x91
	.sleb128 -4
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
	.uleb128 0x35
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x26
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
	.uleb128 0x8
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
	.uleb128 0x9
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
	.uleb128 0xa
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xc
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
	.uleb128 0xd
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xf
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
	.uleb128 0x10
	.uleb128 0x21
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x13
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x12
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
	.uleb128 0x13
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
	.uleb128 0x14
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
	.uleb128 0x15
	.uleb128 0x4
	.byte	0x1
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x28
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1c
	.uleb128 0xd
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0x28
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1c
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x18
	.uleb128 0x13
	.byte	0x1
	.uleb128 0xb
	.uleb128 0x5
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x19
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
	.uleb128 0x5
	.byte	0
	.byte	0
	.uleb128 0x1a
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0x5
	.byte	0
	.byte	0
	.uleb128 0x1b
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0x5
	.byte	0
	.byte	0
	.uleb128 0x1c
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
	.uleb128 0x1d
	.uleb128 0x28
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1c
	.uleb128 0x5
	.byte	0
	.byte	0
	.uleb128 0x1e
	.uleb128 0x15
	.byte	0x1
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1f
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
	.uleb128 0x20
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
	.uleb128 0x21
	.uleb128 0x2e
	.byte	0
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
	.uleb128 0x23
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
	.uleb128 0x24
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
	.uleb128 0x25
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x55
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x26
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
	.uleb128 0x27
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x34
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x28
	.uleb128 0x2e
	.byte	0
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
	.byte	0
	.byte	0
	.uleb128 0x29
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
	.uleb128 0x2a
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
	.uleb128 0x2b
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
	.uleb128 0x2c
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
	.uleb128 0x2d
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
	.uleb128 0x2e
	.uleb128 0x2e
	.byte	0
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
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x2f
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
	.uleb128 0x30
	.uleb128 0x2e
	.byte	0
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
	.byte	0
	.byte	0
	.uleb128 0x31
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
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
	.byte	0
	.byte	0
	.uleb128 0x32
	.uleb128 0x2e
	.byte	0
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
	.byte	0
	.byte	0
	.uleb128 0x33
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
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
	.uleb128 0x34
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
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
	.uleb128 0x35
	.uleb128 0x2e
	.byte	0x1
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
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x36
	.uleb128 0x2e
	.byte	0x1
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
	.uleb128 0x2117
	.uleb128 0x19
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_pubnames,"",%progbits
	.4byte	0x928
	.2byte	0x2
	.4byte	.Ldebug_info0
	.4byte	0x11a4
	.4byte	0x5d3
	.ascii	"Reset_IRQn\000"
	.4byte	0x5d9
	.ascii	"NonMaskableInt_IRQn\000"
	.4byte	0x5df
	.ascii	"HardFault_IRQn\000"
	.4byte	0x5e5
	.ascii	"MemoryManagement_IRQn\000"
	.4byte	0x5eb
	.ascii	"BusFault_IRQn\000"
	.4byte	0x5f1
	.ascii	"UsageFault_IRQn\000"
	.4byte	0x5f7
	.ascii	"SVCall_IRQn\000"
	.4byte	0x5fd
	.ascii	"DebugMonitor_IRQn\000"
	.4byte	0x603
	.ascii	"PendSV_IRQn\000"
	.4byte	0x609
	.ascii	"SysTick_IRQn\000"
	.4byte	0x60f
	.ascii	"POWER_CLOCK_IRQn\000"
	.4byte	0x615
	.ascii	"RADIO_IRQn\000"
	.4byte	0x61b
	.ascii	"UARTE0_UART0_IRQn\000"
	.4byte	0x621
	.ascii	"SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQn\000"
	.4byte	0x627
	.ascii	"SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQn\000"
	.4byte	0x62d
	.ascii	"NFCT_IRQn\000"
	.4byte	0x633
	.ascii	"GPIOTE_IRQn\000"
	.4byte	0x639
	.ascii	"SAADC_IRQn\000"
	.4byte	0x63f
	.ascii	"TIMER0_IRQn\000"
	.4byte	0x645
	.ascii	"TIMER1_IRQn\000"
	.4byte	0x64b
	.ascii	"TIMER2_IRQn\000"
	.4byte	0x651
	.ascii	"RTC0_IRQn\000"
	.4byte	0x657
	.ascii	"TEMP_IRQn\000"
	.4byte	0x65d
	.ascii	"RNG_IRQn\000"
	.4byte	0x663
	.ascii	"ECB_IRQn\000"
	.4byte	0x669
	.ascii	"CCM_AAR_IRQn\000"
	.4byte	0x66f
	.ascii	"WDT_IRQn\000"
	.4byte	0x675
	.ascii	"RTC1_IRQn\000"
	.4byte	0x67b
	.ascii	"QDEC_IRQn\000"
	.4byte	0x681
	.ascii	"COMP_LPCOMP_IRQn\000"
	.4byte	0x687
	.ascii	"SWI0_EGU0_IRQn\000"
	.4byte	0x68d
	.ascii	"SWI1_EGU1_IRQn\000"
	.4byte	0x693
	.ascii	"SWI2_EGU2_IRQn\000"
	.4byte	0x699
	.ascii	"SWI3_EGU3_IRQn\000"
	.4byte	0x69f
	.ascii	"SWI4_EGU4_IRQn\000"
	.4byte	0x6a5
	.ascii	"SWI5_EGU5_IRQn\000"
	.4byte	0x6ab
	.ascii	"TIMER3_IRQn\000"
	.4byte	0x6b1
	.ascii	"TIMER4_IRQn\000"
	.4byte	0x6b7
	.ascii	"PWM0_IRQn\000"
	.4byte	0x6bd
	.ascii	"PDM_IRQn\000"
	.4byte	0x6c3
	.ascii	"MWU_IRQn\000"
	.4byte	0x6c9
	.ascii	"PWM1_IRQn\000"
	.4byte	0x6cf
	.ascii	"PWM2_IRQn\000"
	.4byte	0x6d5
	.ascii	"SPIM2_SPIS2_SPI2_IRQn\000"
	.4byte	0x6db
	.ascii	"RTC2_IRQn\000"
	.4byte	0x6e1
	.ascii	"I2S_IRQn\000"
	.4byte	0x6e7
	.ascii	"FPU_IRQn\000"
	.4byte	0xafd
	.ascii	"NRF_CLOCK_HFCLK_LOW_ACCURACY\000"
	.4byte	0xb03
	.ascii	"NRF_CLOCK_HFCLK_HIGH_ACCURACY\000"
	.4byte	0xb22
	.ascii	"NRF_CLOCK_INT_HF_STARTED_MASK\000"
	.4byte	0xb28
	.ascii	"NRF_CLOCK_INT_LF_STARTED_MASK\000"
	.4byte	0xb2e
	.ascii	"NRF_CLOCK_INT_DONE_MASK\000"
	.4byte	0xb34
	.ascii	"NRF_CLOCK_INT_CTTO_MASK\000"
	.4byte	0xb48
	.ascii	"NRF_CLOCK_TASK_HFCLKSTART\000"
	.4byte	0xb4e
	.ascii	"NRF_CLOCK_TASK_HFCLKSTOP\000"
	.4byte	0xb54
	.ascii	"NRF_CLOCK_TASK_LFCLKSTART\000"
	.4byte	0xb5a
	.ascii	"NRF_CLOCK_TASK_LFCLKSTOP\000"
	.4byte	0xb60
	.ascii	"NRF_CLOCK_TASK_CAL\000"
	.4byte	0xb66
	.ascii	"NRF_CLOCK_TASK_CTSTART\000"
	.4byte	0xb6c
	.ascii	"NRF_CLOCK_TASK_CTSTOP\000"
	.4byte	0xb8b
	.ascii	"NRF_CLOCK_EVENT_HFCLKSTARTED\000"
	.4byte	0xb92
	.ascii	"NRF_CLOCK_EVENT_LFCLKSTARTED\000"
	.4byte	0xb99
	.ascii	"NRF_CLOCK_EVENT_DONE\000"
	.4byte	0xba0
	.ascii	"NRF_CLOCK_EVENT_CTTO\000"
	.4byte	0xbc0
	.ascii	"NRF_DRV_CLOCK_EVT_HFCLK_STARTED\000"
	.4byte	0xbc6
	.ascii	"NRF_DRV_CLOCK_EVT_LFCLK_STARTED\000"
	.4byte	0xbcc
	.ascii	"NRF_DRV_CLOCK_EVT_CAL_DONE\000"
	.4byte	0xbd2
	.ascii	"NRF_DRV_CLOCK_EVT_CAL_ABORTED\000"
	.4byte	0xd0b
	.ascii	"m_clock_cb\000"
	.4byte	0xd0b
	.ascii	"m_clock_cb\000"
	.4byte	0xd1c
	.ascii	"POWER_CLOCK_IRQHandler\000"
	.4byte	0xd2e
	.ascii	"clock_clk_started_notify\000"
	.4byte	0xd7e
	.ascii	"nrf_drv_clock_is_calibrating\000"
	.4byte	0xddb
	.ascii	"nrf_drv_clock_calibration_abort\000"
	.4byte	0xe23
	.ascii	"nrf_drv_clock_calibration_start\000"
	.4byte	0xe74
	.ascii	"nrf_drv_clock_hfclk_is_running\000"
	.4byte	0xe8a
	.ascii	"nrf_drv_clock_hfclk_release\000"
	.4byte	0xe9c
	.ascii	"nrf_drv_clock_hfclk_request\000"
	.4byte	0xec2
	.ascii	"nrf_drv_clock_lfclk_is_running\000"
	.4byte	0xed8
	.ascii	"nrf_drv_clock_lfclk_release\000"
	.4byte	0xeea
	.ascii	"nrf_drv_clock_lfclk_request\000"
	.4byte	0xf0e
	.ascii	"item_dequeue\000"
	.4byte	0xf44
	.ascii	"item_enqueue\000"
	.4byte	0xf84
	.ascii	"nrf_drv_clock_uninit\000"
	.4byte	0xf95
	.ascii	"nrf_drv_clock_init\000"
	.4byte	0xfdb
	.ascii	"nrf_drv_clock_init_check\000"
	.4byte	0xff0
	.ascii	"hfclk_stop\000"
	.4byte	0x1001
	.ascii	"hfclk_start\000"
	.4byte	0x1012
	.ascii	"lfclk_stop\000"
	.4byte	0x1023
	.ascii	"lfclk_start\000"
	.4byte	0x1034
	.ascii	"nrf_drv_common_power_clock_irq_init\000"
	.4byte	0x1046
	.ascii	"nrf_drv_common_irq_enable_check\000"
	.4byte	0x1070
	.ascii	"nrf_clock_hf_is_running\000"
	.4byte	0x109a
	.ascii	"nrf_clock_lf_is_running\000"
	.4byte	0x10b0
	.ascii	"nrf_clock_lf_src_set\000"
	.4byte	0x10d6
	.ascii	"nrf_clock_event_check\000"
	.4byte	0x1100
	.ascii	"nrf_clock_event_clear\000"
	.4byte	0x1135
	.ascii	"nrf_clock_task_trigger\000"
	.4byte	0x115b
	.ascii	"nrf_clock_int_disable\000"
	.4byte	0x1181
	.ascii	"nrf_clock_int_enable\000"
	.4byte	0
	.section	.debug_pubtypes,"",%progbits
	.4byte	0x2fb
	.2byte	0x2
	.4byte	.Ldebug_info0
	.4byte	0x11a4
	.4byte	0x25
	.ascii	"signed char\000"
	.4byte	0x3c
	.ascii	"unsigned char\000"
	.4byte	0x2c
	.ascii	"uint8_t\000"
	.4byte	0x48
	.ascii	"short int\000"
	.4byte	0x5a
	.ascii	"short unsigned int\000"
	.4byte	0x4f
	.ascii	"uint16_t\000"
	.4byte	0x71
	.ascii	"int\000"
	.4byte	0x61
	.ascii	"int32_t\000"
	.4byte	0x8d
	.ascii	"unsigned int\000"
	.4byte	0x78
	.ascii	"uint32_t\000"
	.4byte	0x94
	.ascii	"long long int\000"
	.4byte	0x9b
	.ascii	"long long unsigned int\000"
	.4byte	0xc7
	.ascii	"long int\000"
	.4byte	0xa2
	.ascii	"__mbstate_s\000"
	.4byte	0xed
	.ascii	"char\000"
	.4byte	0x2b2
	.ascii	"__RAL_locale_data_t\000"
	.4byte	0x395
	.ascii	"__RAL_locale_codeset_t\000"
	.4byte	0x3de
	.ascii	"__RAL_locale_t\000"
	.4byte	0x3ee
	.ascii	"__locale_s\000"
	.4byte	0x54e
	.ascii	"__RAL_error_decoder_fn_t\000"
	.4byte	0x56f
	.ascii	"__RAL_error_decoder_s\000"
	.4byte	0x59d
	.ascii	"__RAL_error_decoder_t\000"
	.4byte	0x5bb
	.ascii	"ret_code_t\000"
	.4byte	0x6ee
	.ascii	"IRQn_Type\000"
	.4byte	0x810
	.ascii	"NVIC_Type\000"
	.4byte	0xa68
	.ascii	"NRF_CLOCK_Type\000"
	.4byte	0xa8a
	.ascii	"FILE\000"
	.4byte	0xae5
	.ascii	"nrf_clock_lfclk_t\000"
	.4byte	0xb0a
	.ascii	"nrf_clock_hfclk_t\000"
	.4byte	0xb73
	.ascii	"nrf_clock_task_t\000"
	.4byte	0xba8
	.ascii	"nrf_clock_event_t\000"
	.4byte	0xbd9
	.ascii	"nrf_drv_clock_evt_type_t\000"
	.4byte	0xbe4
	.ascii	"nrf_drv_clock_event_handler_t\000"
	.4byte	0xc00
	.ascii	"nrf_drv_clock_handler_item_t\000"
	.4byte	0xc10
	.ascii	"nrf_drv_clock_handler_item_s\000"
	.4byte	0xc79
	.ascii	"nrf_log_module_dynamic_data_t\000"
	.4byte	0xcee
	.ascii	"_Bool\000"
	.4byte	0xd00
	.ascii	"nrf_drv_clock_cb_t\000"
	.4byte	0
	.section	.debug_aranges,"",%progbits
	.4byte	0x104
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x4
	.byte	0
	.2byte	0
	.2byte	0
	.4byte	.LFB147
	.4byte	.LFE147-.LFB147
	.4byte	.LFB148
	.4byte	.LFE148-.LFB148
	.4byte	.LFB151
	.4byte	.LFE151-.LFB151
	.4byte	.LFB153
	.4byte	.LFE153-.LFB153
	.4byte	.LFB154
	.4byte	.LFE154-.LFB154
	.4byte	.LFB155
	.4byte	.LFE155-.LFB155
	.4byte	.LFB159
	.4byte	.LFE159-.LFB159
	.4byte	.LFB162
	.4byte	.LFE162-.LFB162
	.4byte	.LFB165
	.4byte	.LFE165-.LFB165
	.4byte	.LFB170
	.4byte	.LFE170-.LFB170
	.4byte	.LFB174
	.4byte	.LFE174-.LFB174
	.4byte	.LFB175
	.4byte	.LFE175-.LFB175
	.4byte	.LFB176
	.4byte	.LFE176-.LFB176
	.4byte	.LFB177
	.4byte	.LFE177-.LFB177
	.4byte	.LFB178
	.4byte	.LFE178-.LFB178
	.4byte	.LFB179
	.4byte	.LFE179-.LFB179
	.4byte	.LFB180
	.4byte	.LFE180-.LFB180
	.4byte	.LFB181
	.4byte	.LFE181-.LFB181
	.4byte	.LFB182
	.4byte	.LFE182-.LFB182
	.4byte	.LFB183
	.4byte	.LFE183-.LFB183
	.4byte	.LFB184
	.4byte	.LFE184-.LFB184
	.4byte	.LFB185
	.4byte	.LFE185-.LFB185
	.4byte	.LFB186
	.4byte	.LFE186-.LFB186
	.4byte	.LFB187
	.4byte	.LFE187-.LFB187
	.4byte	.LFB188
	.4byte	.LFE188-.LFB188
	.4byte	.LFB189
	.4byte	.LFE189-.LFB189
	.4byte	.LFB190
	.4byte	.LFE190-.LFB190
	.4byte	.LFB191
	.4byte	.LFE191-.LFB191
	.4byte	.LFB192
	.4byte	.LFE192-.LFB192
	.4byte	.LFB193
	.4byte	.LFE193-.LFB193
	.4byte	0
	.4byte	0
	.section	.debug_ranges,"",%progbits
.Ldebug_ranges0:
	.4byte	.LBB2
	.4byte	.LBE2
	.4byte	.LBB3
	.4byte	.LBE3
	.4byte	0
	.4byte	0
	.4byte	.LFB147
	.4byte	.LFE147
	.4byte	.LFB148
	.4byte	.LFE148
	.4byte	.LFB151
	.4byte	.LFE151
	.4byte	.LFB153
	.4byte	.LFE153
	.4byte	.LFB154
	.4byte	.LFE154
	.4byte	.LFB155
	.4byte	.LFE155
	.4byte	.LFB159
	.4byte	.LFE159
	.4byte	.LFB162
	.4byte	.LFE162
	.4byte	.LFB165
	.4byte	.LFE165
	.4byte	.LFB170
	.4byte	.LFE170
	.4byte	.LFB174
	.4byte	.LFE174
	.4byte	.LFB175
	.4byte	.LFE175
	.4byte	.LFB176
	.4byte	.LFE176
	.4byte	.LFB177
	.4byte	.LFE177
	.4byte	.LFB178
	.4byte	.LFE178
	.4byte	.LFB179
	.4byte	.LFE179
	.4byte	.LFB180
	.4byte	.LFE180
	.4byte	.LFB181
	.4byte	.LFE181
	.4byte	.LFB182
	.4byte	.LFE182
	.4byte	.LFB183
	.4byte	.LFE183
	.4byte	.LFB184
	.4byte	.LFE184
	.4byte	.LFB185
	.4byte	.LFE185
	.4byte	.LFB186
	.4byte	.LFE186
	.4byte	.LFB187
	.4byte	.LFE187
	.4byte	.LFB188
	.4byte	.LFE188
	.4byte	.LFB189
	.4byte	.LFE189
	.4byte	.LFB190
	.4byte	.LFE190
	.4byte	.LFB191
	.4byte	.LFE191
	.4byte	.LFB192
	.4byte	.LFE192
	.4byte	.LFB193
	.4byte	.LFE193
	.4byte	0
	.4byte	0
	.section	.debug_line,"",%progbits
.Ldebug_line0:
	.section	.debug_str,"MS",%progbits,1
.LASF139:
	.ascii	"RSERVED1\000"
.LASF248:
	.ascii	"nrf_drv_clock_hfclk_is_running\000"
.LASF244:
	.ascii	"handler\000"
.LASF62:
	.ascii	"__locale_s\000"
.LASF249:
	.ascii	"nrf_drv_clock_lfclk_is_running\000"
.LASF236:
	.ascii	"evt_type\000"
.LASF52:
	.ascii	"__towupper\000"
.LASF56:
	.ascii	"__RAL_locale_codeset_t\000"
.LASF254:
	.ascii	"item_enqueue\000"
.LASF195:
	.ascii	"NRF_CLOCK_TASK_HFCLKSTART\000"
.LASF169:
	.ascii	"LFCLKSRCCOPY\000"
.LASF113:
	.ascii	"CCM_AAR_IRQn\000"
.LASF6:
	.ascii	"int32_t\000"
.LASF247:
	.ascii	"p_handler_item\000"
.LASF278:
	.ascii	"GNU C99 7.3.1 20180622 (release) [ARM/embedded-7-br"
	.ascii	"anch revision 261907] -fmessage-length=0 -mcpu=cort"
	.ascii	"ex-m4 -mlittle-endian -mfloat-abi=hard -mfpu=fpv4-s"
	.ascii	"p-d16 -mthumb -mtp=soft -munaligned-access -std=gnu"
	.ascii	"99 -g2 -gpubnames -fomit-frame-pointer -fno-dwarf2-"
	.ascii	"cfi-asm -fno-builtin -ffunction-sections -fdata-sec"
	.ascii	"tions -fshort-enums -fno-common\000"
.LASF241:
	.ascii	"__func__\000"
.LASF45:
	.ascii	"time_format\000"
.LASF73:
	.ascii	"__RAL_data_utf8_period\000"
.LASF240:
	.ascii	"nrf_drv_clock_calibration_abort\000"
.LASF133:
	.ascii	"I2S_IRQn\000"
.LASF102:
	.ascii	"SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQn\000"
.LASF205:
	.ascii	"NRF_CLOCK_EVENT_DONE\000"
.LASF41:
	.ascii	"month_names\000"
.LASF190:
	.ascii	"nrf_clock_hfclk_t\000"
.LASF74:
	.ascii	"__RAL_data_utf8_comma\000"
.LASF18:
	.ascii	"int_curr_symbol\000"
.LASF44:
	.ascii	"date_format\000"
.LASF164:
	.ascii	"INTENCLR\000"
.LASF157:
	.ascii	"TASKS_CTSTOP\000"
.LASF29:
	.ascii	"n_cs_precedes\000"
.LASF211:
	.ascii	"NRF_DRV_CLOCK_EVT_CAL_ABORTED\000"
.LASF50:
	.ascii	"__tolower\000"
.LASF177:
	.ascii	"__StackLimit\000"
.LASF142:
	.ascii	"ICPR\000"
.LASF25:
	.ascii	"int_frac_digits\000"
.LASF108:
	.ascii	"TIMER2_IRQn\000"
.LASF23:
	.ascii	"positive_sign\000"
.LASF223:
	.ascii	"m_nrf_log_clock_logs_data_dynamic\000"
.LASF159:
	.ascii	"EVENTS_LFCLKSTARTED\000"
.LASF243:
	.ascii	"interval\000"
.LASF175:
	.ascii	"NRF_CLOCK_Type\000"
.LASF228:
	.ascii	"p_hf_head\000"
.LASF161:
	.ascii	"EVENTS_DONE\000"
.LASF227:
	.ascii	"hfclk_requests\000"
.LASF101:
	.ascii	"SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQn\000"
.LASF20:
	.ascii	"mon_decimal_point\000"
.LASF257:
	.ascii	"nrf_drv_clock_init_check\000"
.LASF13:
	.ascii	"long int\000"
.LASF216:
	.ascii	"p_next\000"
.LASF132:
	.ascii	"RTC2_IRQn\000"
.LASF193:
	.ascii	"NRF_CLOCK_INT_DONE_MASK\000"
.LASF82:
	.ascii	"__RAL_error_decoder_s\000"
.LASF85:
	.ascii	"__RAL_error_decoder_t\000"
.LASF64:
	.ascii	"__RAL_global_locale\000"
.LASF224:
	.ascii	"module_initialized\000"
.LASF81:
	.ascii	"__RAL_error_decoder_fn_t\000"
.LASF222:
	.ascii	"nrf_log_module_dynamic_data_t\000"
.LASF105:
	.ascii	"SAADC_IRQn\000"
.LASF34:
	.ascii	"int_n_cs_precedes\000"
.LASF265:
	.ascii	"IRQn\000"
.LASF210:
	.ascii	"NRF_DRV_CLOCK_EVT_CAL_DONE\000"
.LASF282:
	.ascii	"nrf_clock_int_enable\000"
.LASF97:
	.ascii	"SysTick_IRQn\000"
.LASF55:
	.ascii	"__mbtowc\000"
.LASF0:
	.ascii	"signed char\000"
.LASF168:
	.ascii	"LFCLKSTAT\000"
.LASF3:
	.ascii	"uint8_t\000"
.LASF103:
	.ascii	"NFCT_IRQn\000"
.LASF126:
	.ascii	"PWM0_IRQn\000"
.LASF54:
	.ascii	"__wctomb\000"
.LASF167:
	.ascii	"LFCLKRUN\000"
.LASF131:
	.ascii	"SPIM2_SPIS2_SPI2_IRQn\000"
.LASF137:
	.ascii	"RESERVED0\000"
.LASF160:
	.ascii	"RESERVED1\000"
.LASF1:
	.ascii	"unsigned char\000"
.LASF143:
	.ascii	"RESERVED3\000"
.LASF145:
	.ascii	"RESERVED4\000"
.LASF146:
	.ascii	"RESERVED5\000"
.LASF171:
	.ascii	"RESERVED6\000"
.LASF173:
	.ascii	"RESERVED7\000"
.LASF30:
	.ascii	"n_sep_by_space\000"
.LASF144:
	.ascii	"IABR\000"
.LASF279:
	.ascii	"C:\\Users\\paulr\\OneDrive\\Documents\\DARPA\\darpa"
	.ascii	"subt\\darpasubt\\firmware\\dwm1001_dev_source_code\\"
	.ascii	"nRF5_SDK_14.2.0\\components\\drivers_nrf\\clock\\nr"
	.ascii	"f_drv_clock.c\000"
.LASF75:
	.ascii	"__RAL_data_utf8_space\000"
.LASF235:
	.ascii	"p_item\000"
.LASF231:
	.ascii	"_Bool\000"
.LASF219:
	.ascii	"order_idx\000"
.LASF141:
	.ascii	"RESERVED2\000"
.LASF151:
	.ascii	"TASKS_HFCLKSTART\000"
.LASF93:
	.ascii	"UsageFault_IRQn\000"
.LASF238:
	.ascii	"err_code\000"
.LASF14:
	.ascii	"char\000"
.LASF172:
	.ascii	"CTIV\000"
.LASF208:
	.ascii	"NRF_DRV_CLOCK_EVT_HFCLK_STARTED\000"
.LASF187:
	.ascii	"nrf_clock_lfclk_t\000"
.LASF212:
	.ascii	"nrf_drv_clock_evt_type_t\000"
.LASF32:
	.ascii	"n_sign_posn\000"
.LASF179:
	.ascii	"timeval\000"
.LASF280:
	.ascii	"C:\\Users\\paulr\\OneDrive\\Documents\\DARPA\\darpa"
	.ascii	"subt\\darpasubt\\firmware\\dwm1001_dev_source_code\\"
	.ascii	"examples\\ds_twr_resp\\SES\000"
.LASF269:
	.ascii	"source\000"
.LASF72:
	.ascii	"__RAL_c_locale_abbrev_month_names\000"
.LASF197:
	.ascii	"NRF_CLOCK_TASK_LFCLKSTART\000"
.LASF21:
	.ascii	"mon_thousands_sep\000"
.LASF53:
	.ascii	"__towlower\000"
.LASF166:
	.ascii	"HFCLKSTAT\000"
.LASF181:
	.ascii	"stdin\000"
.LASF24:
	.ascii	"negative_sign\000"
.LASF140:
	.ascii	"ISPR\000"
.LASF33:
	.ascii	"int_p_cs_precedes\000"
.LASF83:
	.ascii	"decode\000"
.LASF182:
	.ascii	"stdout\000"
.LASF276:
	.ascii	"nrf_clock_int_disable\000"
.LASF123:
	.ascii	"SWI5_EGU5_IRQn\000"
.LASF147:
	.ascii	"STIR\000"
.LASF87:
	.ascii	"ret_code_t\000"
.LASF275:
	.ascii	"task\000"
.LASF234:
	.ascii	"p_head\000"
.LASF165:
	.ascii	"HFCLKRUN\000"
.LASF104:
	.ascii	"GPIOTE_IRQn\000"
.LASF66:
	.ascii	"__RAL_codeset_ascii\000"
.LASF155:
	.ascii	"TASKS_CAL\000"
.LASF60:
	.ascii	"__RAL_locale_t\000"
.LASF163:
	.ascii	"INTENSET\000"
.LASF42:
	.ascii	"abbrev_month_names\000"
.LASF258:
	.ascii	"hfclk_stop\000"
.LASF198:
	.ascii	"NRF_CLOCK_TASK_LFCLKSTOP\000"
.LASF59:
	.ascii	"codeset\000"
.LASF268:
	.ascii	"nrf_clock_lf_src_set\000"
.LASF262:
	.ascii	"nrf_drv_common_power_clock_irq_init\000"
.LASF118:
	.ascii	"SWI0_EGU0_IRQn\000"
.LASF158:
	.ascii	"EVENTS_HFCLKSTARTED\000"
.LASF250:
	.ascii	"nrf_drv_clock_lfclk_release\000"
.LASF246:
	.ascii	"nrf_drv_clock_hfclk_release\000"
.LASF89:
	.ascii	"NonMaskableInt_IRQn\000"
.LASF12:
	.ascii	"__wchar\000"
.LASF86:
	.ascii	"__RAL_error_decoder_head\000"
.LASF199:
	.ascii	"NRF_CLOCK_TASK_CAL\000"
.LASF47:
	.ascii	"__RAL_locale_data_t\000"
.LASF91:
	.ascii	"MemoryManagement_IRQn\000"
.LASF71:
	.ascii	"__RAL_c_locale_month_names\000"
.LASF150:
	.ascii	"SystemCoreClock\000"
.LASF122:
	.ascii	"SWI4_EGU4_IRQn\000"
.LASF202:
	.ascii	"nrf_clock_task_t\000"
.LASF135:
	.ascii	"IRQn_Type\000"
.LASF176:
	.ascii	"__StackTop\000"
.LASF191:
	.ascii	"NRF_CLOCK_INT_HF_STARTED_MASK\000"
.LASF130:
	.ascii	"PWM2_IRQn\000"
.LASF170:
	.ascii	"LFCLKSRC\000"
.LASF43:
	.ascii	"am_pm_indicator\000"
.LASF28:
	.ascii	"p_sep_by_space\000"
.LASF114:
	.ascii	"WDT_IRQn\000"
.LASF213:
	.ascii	"nrf_drv_clock_event_handler_t\000"
.LASF218:
	.ascii	"module_id\000"
.LASF36:
	.ascii	"int_n_sep_by_space\000"
.LASF92:
	.ascii	"BusFault_IRQn\000"
.LASF79:
	.ascii	"__user_set_time_of_day\000"
.LASF111:
	.ascii	"RNG_IRQn\000"
.LASF272:
	.ascii	"nrf_clock_event_clear\000"
.LASF107:
	.ascii	"TIMER1_IRQn\000"
.LASF9:
	.ascii	"long long int\000"
.LASF61:
	.ascii	"__mbstate_s\000"
.LASF196:
	.ascii	"NRF_CLOCK_TASK_HFCLKSTOP\000"
.LASF99:
	.ascii	"RADIO_IRQn\000"
.LASF94:
	.ascii	"SVCall_IRQn\000"
.LASF267:
	.ascii	"clk_src\000"
.LASF252:
	.ascii	"nrf_drv_clock_lfclk_request\000"
.LASF63:
	.ascii	"__category\000"
.LASF251:
	.ascii	"nrf_drv_clock_hfclk_request\000"
.LASF270:
	.ascii	"nrf_clock_event_check\000"
.LASF115:
	.ascii	"RTC1_IRQn\000"
.LASF134:
	.ascii	"FPU_IRQn\000"
.LASF46:
	.ascii	"date_time_format\000"
.LASF153:
	.ascii	"TASKS_LFCLKSTART\000"
.LASF233:
	.ascii	"m_clock_cb\000"
.LASF271:
	.ascii	"event\000"
.LASF242:
	.ascii	"nrf_drv_clock_calibration_start\000"
.LASF106:
	.ascii	"TIMER0_IRQn\000"
.LASF8:
	.ascii	"unsigned int\000"
.LASF253:
	.ascii	"clock_clk_started_notify\000"
.LASF31:
	.ascii	"p_sign_posn\000"
.LASF266:
	.ascii	"nrf_clock_hf_is_running\000"
.LASF207:
	.ascii	"nrf_clock_event_t\000"
.LASF80:
	.ascii	"__user_get_time_of_day\000"
.LASF185:
	.ascii	"NRF_CLOCK_LFCLK_Xtal\000"
.LASF189:
	.ascii	"NRF_CLOCK_HFCLK_HIGH_ACCURACY\000"
.LASF67:
	.ascii	"__RAL_codeset_utf8\000"
.LASF65:
	.ascii	"__RAL_c_locale\000"
.LASF112:
	.ascii	"ECB_IRQn\000"
.LASF259:
	.ascii	"hfclk_start\000"
.LASF116:
	.ascii	"QDEC_IRQn\000"
.LASF245:
	.ascii	"POWER_CLOCK_IRQHandler\000"
.LASF15:
	.ascii	"decimal_point\000"
.LASF221:
	.ascii	"filter_lvls\000"
.LASF180:
	.ascii	"__RAL_FILE\000"
.LASF255:
	.ascii	"nrf_drv_clock_uninit\000"
.LASF230:
	.ascii	"p_lf_head\000"
.LASF174:
	.ascii	"TRACECONFIG\000"
.LASF178:
	.ascii	"FILE\000"
.LASF215:
	.ascii	"nrf_drv_clock_handler_item_s\000"
.LASF214:
	.ascii	"nrf_drv_clock_handler_item_t\000"
.LASF260:
	.ascii	"lfclk_stop\000"
.LASF125:
	.ascii	"TIMER4_IRQn\000"
.LASF76:
	.ascii	"__RAL_data_utf8_plus\000"
.LASF274:
	.ascii	"nrf_clock_task_trigger\000"
.LASF148:
	.ascii	"NVIC_Type\000"
.LASF48:
	.ascii	"__isctype\000"
.LASF10:
	.ascii	"long long unsigned int\000"
.LASF35:
	.ascii	"int_p_sep_by_space\000"
.LASF4:
	.ascii	"uint16_t\000"
.LASF263:
	.ascii	"item_dequeue\000"
.LASF88:
	.ascii	"Reset_IRQn\000"
.LASF232:
	.ascii	"nrf_drv_clock_cb_t\000"
.LASF124:
	.ascii	"TIMER3_IRQn\000"
.LASF184:
	.ascii	"NRF_CLOCK_LFCLK_RC\000"
.LASF127:
	.ascii	"PDM_IRQn\000"
.LASF138:
	.ascii	"ICER\000"
.LASF98:
	.ascii	"POWER_CLOCK_IRQn\000"
.LASF239:
	.ascii	"nrf_drv_clock_is_calibrating\000"
.LASF226:
	.ascii	"lfclk_on\000"
.LASF209:
	.ascii	"NRF_DRV_CLOCK_EVT_LFCLK_STARTED\000"
.LASF40:
	.ascii	"abbrev_day_names\000"
.LASF264:
	.ascii	"nrf_drv_common_irq_enable_check\000"
.LASF206:
	.ascii	"NRF_CLOCK_EVENT_CTTO\000"
.LASF68:
	.ascii	"__RAL_ascii_ctype_map\000"
.LASF273:
	.ascii	"dummy\000"
.LASF16:
	.ascii	"thousands_sep\000"
.LASF100:
	.ascii	"UARTE0_UART0_IRQn\000"
.LASF95:
	.ascii	"DebugMonitor_IRQn\000"
.LASF49:
	.ascii	"__toupper\000"
.LASF204:
	.ascii	"NRF_CLOCK_EVENT_LFCLKSTARTED\000"
.LASF57:
	.ascii	"name\000"
.LASF117:
	.ascii	"COMP_LPCOMP_IRQn\000"
.LASF154:
	.ascii	"TASKS_LFCLKSTOP\000"
.LASF26:
	.ascii	"frac_digits\000"
.LASF19:
	.ascii	"currency_symbol\000"
.LASF183:
	.ascii	"stderr\000"
.LASF2:
	.ascii	"short int\000"
.LASF129:
	.ascii	"PWM1_IRQn\000"
.LASF11:
	.ascii	"__state\000"
.LASF121:
	.ascii	"SWI3_EGU3_IRQn\000"
.LASF217:
	.ascii	"event_handler\000"
.LASF22:
	.ascii	"mon_grouping\000"
.LASF162:
	.ascii	"EVENTS_CTTO\000"
.LASF192:
	.ascii	"NRF_CLOCK_INT_LF_STARTED_MASK\000"
.LASF201:
	.ascii	"NRF_CLOCK_TASK_CTSTOP\000"
.LASF261:
	.ascii	"lfclk_start\000"
.LASF39:
	.ascii	"day_names\000"
.LASF156:
	.ascii	"TASKS_CTSTART\000"
.LASF220:
	.ascii	"filter\000"
.LASF77:
	.ascii	"__RAL_data_utf8_minus\000"
.LASF37:
	.ascii	"int_p_sign_posn\000"
.LASF194:
	.ascii	"NRF_CLOCK_INT_CTTO_MASK\000"
.LASF149:
	.ascii	"ITM_RxBuffer\000"
.LASF51:
	.ascii	"__iswctype\000"
.LASF69:
	.ascii	"__RAL_c_locale_day_names\000"
.LASF186:
	.ascii	"NRF_CLOCK_LFCLK_Synth\000"
.LASF225:
	.ascii	"hfclk_on\000"
.LASF277:
	.ascii	"int_mask\000"
.LASF281:
	.ascii	"nrf_clock_lf_is_running\000"
.LASF256:
	.ascii	"nrf_drv_clock_init\000"
.LASF7:
	.ascii	"uint32_t\000"
.LASF38:
	.ascii	"int_n_sign_posn\000"
.LASF229:
	.ascii	"lfclk_requests\000"
.LASF237:
	.ascii	"p_is_calibrating\000"
.LASF120:
	.ascii	"SWI2_EGU2_IRQn\000"
.LASF78:
	.ascii	"__RAL_data_empty_string\000"
.LASF109:
	.ascii	"RTC0_IRQn\000"
.LASF152:
	.ascii	"TASKS_HFCLKSTOP\000"
.LASF27:
	.ascii	"p_cs_precedes\000"
.LASF5:
	.ascii	"short unsigned int\000"
.LASF110:
	.ascii	"TEMP_IRQn\000"
.LASF96:
	.ascii	"PendSV_IRQn\000"
.LASF188:
	.ascii	"NRF_CLOCK_HFCLK_LOW_ACCURACY\000"
.LASF136:
	.ascii	"ISER\000"
.LASF17:
	.ascii	"grouping\000"
.LASF90:
	.ascii	"HardFault_IRQn\000"
.LASF84:
	.ascii	"next\000"
.LASF58:
	.ascii	"data\000"
.LASF128:
	.ascii	"MWU_IRQn\000"
.LASF200:
	.ascii	"NRF_CLOCK_TASK_CTSTART\000"
.LASF119:
	.ascii	"SWI1_EGU1_IRQn\000"
.LASF203:
	.ascii	"NRF_CLOCK_EVENT_HFCLKSTARTED\000"
.LASF70:
	.ascii	"__RAL_c_locale_abbrev_day_names\000"
	.ident	"GCC: (GNU) 7.3.1 20180622 (release) [ARM/embedded-7-branch revision 261907]"
