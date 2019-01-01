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
	.file	"nrf_fprintf_format.c"
	.text
.Ltext0:
	.section	.text.buffer_add,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	buffer_add, %function
buffer_add:
.LFB147:
	.file 1 "C:\\Users\\paulr\\OneDrive\\Documents\\DARPA\\darpasubt\\darpasubt\\firmware\\dwm1001_dev_source_code\\nRF5_SDK_14.2.0\\external\\fprintf\\nrf_fprintf_format.c"
	.loc 1 73 0
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI0:
	sub	sp, sp, #12
.LCFI1:
	str	r0, [sp, #4]
	mov	r3, r1
	strb	r3, [sp, #3]
	.loc 1 74 0
	ldr	r3, [sp, #4]
	ldr	r2, [r3]
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #8]
	adds	r0, r3, #1
	ldr	r1, [sp, #4]
	str	r0, [r1, #8]
	add	r3, r3, r2
	ldrb	r2, [sp, #3]
	strb	r2, [r3]
	.loc 1 76 0
	ldr	r3, [sp, #4]
	ldr	r2, [r3, #8]
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #4]
	cmp	r2, r3
	bcc	.L3
	.loc 1 78 0
	ldr	r0, [sp, #4]
	bl	nrf_fprintf_buffer_flush
.L3:
	.loc 1 80 0
	nop
	add	sp, sp, #12
.LCFI2:
	@ sp needed
	ldr	pc, [sp], #4
.LFE147:
	.size	buffer_add, .-buffer_add
	.section	.text.string_print,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	string_print, %function
string_print:
.LFB148:
	.loc 1 86 0
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI3:
	sub	sp, sp, #28
.LCFI4:
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	str	r2, [sp, #4]
	str	r3, [sp]
	.loc 1 87 0
	movs	r3, #0
	str	r3, [sp, #20]
	.loc 1 90 0
	ldr	r3, [sp]
	and	r3, r3, #1
	cmp	r3, #0
	beq	.L5
	.loc 1 92 0
	b	.L6
.L7:
	.loc 1 94 0
	ldr	r3, [sp, #8]
	adds	r3, r3, #1
	str	r3, [sp, #8]
	.loc 1 95 0
	ldr	r3, [sp, #20]
	adds	r3, r3, #1
	str	r3, [sp, #20]
	.loc 1 96 0
	ldrb	r3, [sp, #19]	@ zero_extendqisi2
	mov	r1, r3
	ldr	r0, [sp, #12]
	bl	buffer_add
.L6:
	.loc 1 92 0
	ldr	r3, [sp, #8]
	ldrb	r3, [r3]
	strb	r3, [sp, #19]
	ldrb	r3, [sp, #19]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L7
	.loc 1 99 0
	b	.L8
.L10:
	.loc 1 101 0
	ldr	r3, [sp, #4]
	subs	r3, r3, #1
	str	r3, [sp, #4]
	.loc 1 102 0
	movs	r1, #32
	ldr	r0, [sp, #12]
	bl	buffer_add
.L8:
	.loc 1 99 0
	ldr	r2, [sp, #4]
	ldr	r3, [sp, #20]
	cmp	r2, r3
	bls	.L17
	.loc 1 99 0 is_stmt 0 discriminator 1
	ldr	r3, [sp, #4]
	cmp	r3, #0
	bne	.L10
	.loc 1 125 0 is_stmt 1
	b	.L17
.L5:
	.loc 1 107 0
	ldr	r3, [sp, #8]
	cmp	r3, #0
	beq	.L12
	.loc 1 109 0
	ldr	r0, [sp, #8]
	bl	strlen
	str	r0, [sp, #20]
	.loc 1 112 0
	b	.L12
.L14:
	.loc 1 114 0
	ldr	r3, [sp, #4]
	subs	r3, r3, #1
	str	r3, [sp, #4]
	.loc 1 115 0
	movs	r1, #32
	ldr	r0, [sp, #12]
	bl	buffer_add
.L12:
	.loc 1 112 0
	ldr	r2, [sp, #4]
	ldr	r3, [sp, #20]
	cmp	r2, r3
	bls	.L15
	.loc 1 112 0 is_stmt 0 discriminator 1
	ldr	r3, [sp, #4]
	cmp	r3, #0
	bne	.L14
	.loc 1 118 0 is_stmt 1
	b	.L15
.L16:
	.loc 1 120 0
	ldr	r3, [sp, #8]
	adds	r3, r3, #1
	str	r3, [sp, #8]
	.loc 1 121 0
	ldr	r3, [sp, #20]
	adds	r3, r3, #1
	str	r3, [sp, #20]
	.loc 1 122 0
	ldrb	r3, [sp, #19]	@ zero_extendqisi2
	mov	r1, r3
	ldr	r0, [sp, #12]
	bl	buffer_add
.L15:
	.loc 1 118 0
	ldr	r3, [sp, #8]
	ldrb	r3, [r3]
	strb	r3, [sp, #19]
	ldrb	r3, [sp, #19]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L16
.L17:
	.loc 1 125 0
	nop
	add	sp, sp, #28
.LCFI5:
	@ sp needed
	ldr	pc, [sp], #4
.LFE148:
	.size	string_print, .-string_print
	.section	.text.unsigned_print,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	unsigned_print, %function
unsigned_print:
.LFB149:
	.loc 1 133 0
	@ args = 8, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI6:
	sub	sp, sp, #36
.LCFI7:
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	str	r2, [sp, #4]
	str	r3, [sp]
	.loc 1 141 0
	ldr	r3, [sp, #8]
	str	r3, [sp, #28]
	.loc 1 145 0
	movs	r3, #1
	str	r3, [sp, #24]
	.loc 1 146 0
	b	.L19
.L20:
	.loc 1 148 0
	ldr	r2, [sp, #28]
	ldr	r3, [sp, #4]
	udiv	r3, r2, r3
	str	r3, [sp, #28]
	.loc 1 149 0
	ldr	r3, [sp, #24]
	adds	r3, r3, #1
	str	r3, [sp, #24]
.L19:
	.loc 1 146 0
	ldr	r2, [sp, #28]
	ldr	r3, [sp, #4]
	cmp	r2, r3
	bcs	.L20
	.loc 1 151 0
	ldr	r2, [sp]
	ldr	r3, [sp, #24]
	cmp	r2, r3
	bls	.L21
	.loc 1 153 0
	ldr	r3, [sp]
	str	r3, [sp, #24]
.L21:
	.loc 1 158 0
	ldr	r3, [sp, #44]
	and	r3, r3, #1
	cmp	r3, #0
	bne	.L22
	.loc 1 160 0
	ldr	r3, [sp, #40]
	cmp	r3, #0
	beq	.L22
	.loc 1 162 0
	ldr	r3, [sp, #44]
	and	r3, r3, #2
	cmp	r3, #0
	beq	.L23
	.loc 1 162 0 is_stmt 0 discriminator 1
	ldr	r3, [sp]
	cmp	r3, #0
	bne	.L23
	.loc 1 165 0 is_stmt 1
	movs	r3, #48
	strb	r3, [sp, #23]
	b	.L24
.L23:
	.loc 1 169 0
	movs	r3, #32
	strb	r3, [sp, #23]
.L24:
	.loc 1 171 0
	b	.L25
.L26:
	.loc 1 173 0
	ldr	r3, [sp, #40]
	subs	r3, r3, #1
	str	r3, [sp, #40]
	.loc 1 174 0
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	mov	r1, r3
	ldr	r0, [sp, #12]
	bl	buffer_add
.L25:
	.loc 1 171 0
	ldr	r3, [sp, #40]
	cmp	r3, #0
	beq	.L22
	.loc 1 171 0 is_stmt 0 discriminator 1
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #40]
	cmp	r2, r3
	bcc	.L26
.L22:
	.loc 1 179 0 is_stmt 1
	movs	r3, #1
	str	r3, [sp, #28]
.L30:
	.loc 1 191 0
	ldr	r3, [sp]
	cmp	r3, #1
	bls	.L27
	.loc 1 193 0
	ldr	r3, [sp]
	subs	r3, r3, #1
	str	r3, [sp]
	b	.L28
.L27:
	.loc 1 197 0
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #28]
	udiv	r3, r2, r3
	str	r3, [sp, #16]
	.loc 1 199 0
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #4]
	cmp	r2, r3
	bcs	.L28
	.loc 1 201 0
	b	.L29
.L28:
	.loc 1 204 0
	ldr	r3, [sp, #28]
	ldr	r2, [sp, #4]
	mul	r3, r2, r3
	str	r3, [sp, #28]
	.loc 1 191 0
	b	.L30
.L29:
	.loc 1 211 0 discriminator 1
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #28]
	udiv	r3, r2, r3
	str	r3, [sp, #16]
	.loc 1 212 0 discriminator 1
	ldr	r3, [sp, #16]
	ldr	r2, [sp, #28]
	mul	r3, r2, r3
	ldr	r2, [sp, #8]
	subs	r3, r2, r3
	str	r3, [sp, #8]
	.loc 1 213 0 discriminator 1
	ldr	r2, .L35
	ldr	r3, [sp, #16]
	add	r3, r3, r2
	ldrb	r3, [r3]	@ zero_extendqisi2
	mov	r1, r3
	ldr	r0, [sp, #12]
	bl	buffer_add
	.loc 1 214 0 discriminator 1
	ldr	r2, [sp, #28]
	ldr	r3, [sp, #4]
	udiv	r3, r2, r3
	str	r3, [sp, #28]
	.loc 1 215 0 discriminator 1
	ldr	r3, [sp, #28]
	cmp	r3, #0
	bne	.L29
	.loc 1 219 0
	ldr	r3, [sp, #44]
	and	r3, r3, #1
	cmp	r3, #0
	beq	.L34
	.loc 1 221 0
	ldr	r3, [sp, #40]
	cmp	r3, #0
	beq	.L34
	.loc 1 223 0
	b	.L32
.L33:
	.loc 1 225 0
	ldr	r3, [sp, #40]
	subs	r3, r3, #1
	str	r3, [sp, #40]
	.loc 1 226 0
	movs	r1, #32
	ldr	r0, [sp, #12]
	bl	buffer_add
.L32:
	.loc 1 223 0
	ldr	r3, [sp, #40]
	cmp	r3, #0
	beq	.L34
	.loc 1 223 0 is_stmt 0 discriminator 1
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #40]
	cmp	r2, r3
	bcc	.L33
.L34:
	.loc 1 230 0 is_stmt 1
	nop
	add	sp, sp, #36
.LCFI8:
	@ sp needed
	ldr	pc, [sp], #4
.L36:
	.align	2
.L35:
	.word	_aV2C.7643
.LFE149:
	.size	unsigned_print, .-unsigned_print
	.section	.text.int_print,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	int_print, %function
int_print:
.LFB150:
	.loc 1 238 0
	@ args = 8, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI9:
	sub	sp, sp, #36
.LCFI10:
	str	r0, [sp, #20]
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	str	r3, [sp, #8]
	.loc 1 242 0
	ldr	r3, [sp, #16]
	cmp	r3, #0
	it	lt
	rsblt	r3, r3, #0
	str	r3, [sp, #24]
	.loc 1 247 0
	movs	r3, #1
	str	r3, [sp, #28]
	.loc 1 248 0
	b	.L38
.L39:
	.loc 1 250 0
	ldr	r3, [sp, #12]
	ldr	r2, [sp, #24]
	sdiv	r3, r2, r3
	str	r3, [sp, #24]
	.loc 1 251 0
	ldr	r3, [sp, #28]
	adds	r3, r3, #1
	str	r3, [sp, #28]
.L38:
	.loc 1 248 0
	ldr	r3, [sp, #12]
	ldr	r2, [sp, #24]
	cmp	r2, r3
	bge	.L39
	.loc 1 253 0
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #28]
	cmp	r2, r3
	bls	.L40
	.loc 1 255 0
	ldr	r3, [sp, #8]
	str	r3, [sp, #28]
.L40:
	.loc 1 257 0
	ldr	r3, [sp, #40]
	cmp	r3, #0
	beq	.L41
	.loc 1 257 0 is_stmt 0 discriminator 1
	ldr	r3, [sp, #16]
	cmp	r3, #0
	blt	.L42
	.loc 1 258 0 is_stmt 1 discriminator 2
	ldr	r3, [sp, #44]
	and	r3, r3, #4
	.loc 1 257 0 discriminator 2
	cmp	r3, #0
	beq	.L41
.L42:
	.loc 1 260 0
	ldr	r3, [sp, #40]
	subs	r3, r3, #1
	str	r3, [sp, #40]
.L41:
	.loc 1 265 0
	ldr	r3, [sp, #44]
	and	r3, r3, #2
	cmp	r3, #0
	beq	.L43
	.loc 1 265 0 is_stmt 0 discriminator 2
	ldr	r3, [sp, #8]
	cmp	r3, #0
	beq	.L44
.L43:
	.loc 1 266 0 is_stmt 1 discriminator 3
	ldr	r3, [sp, #44]
	and	r3, r3, #1
	.loc 1 265 0 discriminator 3
	cmp	r3, #0
	bne	.L44
	.loc 1 268 0
	ldr	r3, [sp, #40]
	cmp	r3, #0
	beq	.L44
	.loc 1 270 0
	b	.L45
.L46:
	.loc 1 272 0
	ldr	r3, [sp, #40]
	subs	r3, r3, #1
	str	r3, [sp, #40]
	.loc 1 273 0
	movs	r1, #32
	ldr	r0, [sp, #20]
	bl	buffer_add
.L45:
	.loc 1 270 0
	ldr	r3, [sp, #40]
	cmp	r3, #0
	beq	.L44
	.loc 1 270 0 is_stmt 0 discriminator 1
	ldr	r2, [sp, #28]
	ldr	r3, [sp, #40]
	cmp	r2, r3
	bcc	.L46
.L44:
	.loc 1 280 0 is_stmt 1
	ldr	r3, [sp, #16]
	cmp	r3, #0
	bge	.L47
	.loc 1 282 0
	ldr	r3, [sp, #16]
	negs	r3, r3
	str	r3, [sp, #16]
	.loc 1 283 0
	movs	r1, #45
	ldr	r0, [sp, #20]
	bl	buffer_add
	b	.L48
.L47:
	.loc 1 285 0
	ldr	r3, [sp, #44]
	and	r3, r3, #4
	cmp	r3, #0
	beq	.L48
	.loc 1 287 0
	movs	r1, #43
	ldr	r0, [sp, #20]
	bl	buffer_add
.L48:
	.loc 1 296 0
	ldr	r3, [sp, #44]
	and	r3, r3, #2
	cmp	r3, #0
	beq	.L49
	.loc 1 297 0 discriminator 1
	ldr	r3, [sp, #44]
	and	r3, r3, #1
	.loc 1 296 0 discriminator 1
	cmp	r3, #0
	bne	.L49
	.loc 1 297 0
	ldr	r3, [sp, #8]
	cmp	r3, #0
	bne	.L49
	.loc 1 299 0
	ldr	r3, [sp, #40]
	cmp	r3, #0
	beq	.L49
	.loc 1 301 0
	b	.L50
.L51:
	.loc 1 303 0
	ldr	r3, [sp, #40]
	subs	r3, r3, #1
	str	r3, [sp, #40]
	.loc 1 304 0
	movs	r1, #48
	ldr	r0, [sp, #20]
	bl	buffer_add
.L50:
	.loc 1 301 0
	ldr	r3, [sp, #40]
	cmp	r3, #0
	beq	.L49
	.loc 1 301 0 is_stmt 0 discriminator 1
	ldr	r2, [sp, #28]
	ldr	r3, [sp, #40]
	cmp	r2, r3
	bcc	.L51
.L49:
	.loc 1 311 0 is_stmt 1
	ldr	r1, [sp, #16]
	ldr	r3, [sp, #44]
	str	r3, [sp, #4]
	ldr	r3, [sp, #40]
	str	r3, [sp]
	ldr	r3, [sp, #8]
	ldr	r2, [sp, #12]
	ldr	r0, [sp, #20]
	bl	unsigned_print
	.loc 1 312 0
	nop
	add	sp, sp, #36
.LCFI11:
	@ sp needed
	ldr	pc, [sp], #4
.LFE150:
	.size	int_print, .-int_print
	.section	.text.nrf_fprintf_fmt,"ax",%progbits
	.align	1
	.global	nrf_fprintf_fmt
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	nrf_fprintf_fmt, %function
nrf_fprintf_fmt:
.LFB151:
	.loc 1 317 0
	@ args = 0, pretend = 0, frame = 48
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI12:
	sub	sp, sp, #60
.LCFI13:
	str	r0, [sp, #20]
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	.loc 1 324 0
	ldr	r3, [sp, #16]
	cmp	r3, #0
	beq	.L84
.L53:
	.loc 1 337 0
	ldr	r3, [sp, #16]
	ldrb	r3, [r3]
	strb	r3, [sp, #55]
	.loc 1 338 0
	ldr	r3, [sp, #16]
	adds	r3, r3, #1
	str	r3, [sp, #16]
	.loc 1 340 0
	ldrb	r3, [sp, #55]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L85
	.loc 1 344 0
	ldrb	r3, [sp, #55]	@ zero_extendqisi2
	cmp	r3, #37
	bne	.L57
	.loc 1 349 0
	movs	r3, #0
	str	r3, [sp, #40]
	.loc 1 350 0
	movs	r3, #1
	str	r3, [sp, #48]
.L63:
	.loc 1 354 0
	ldr	r3, [sp, #16]
	ldrb	r3, [r3]
	strb	r3, [sp, #55]
	.loc 1 355 0
	ldrb	r3, [sp, #55]	@ zero_extendqisi2
	cmp	r3, #45
	beq	.L59
	cmp	r3, #48
	beq	.L60
	cmp	r3, #43
	beq	.L61
	b	.L83
.L59:
	.loc 1 358 0
	ldr	r3, [sp, #40]
	orr	r3, r3, #1
	str	r3, [sp, #40]
	.loc 1 359 0
	ldr	r3, [sp, #16]
	adds	r3, r3, #1
	str	r3, [sp, #16]
	.loc 1 360 0
	b	.L62
.L60:
	.loc 1 362 0
	ldr	r3, [sp, #40]
	orr	r3, r3, #2
	str	r3, [sp, #40]
	.loc 1 363 0
	ldr	r3, [sp, #16]
	adds	r3, r3, #1
	str	r3, [sp, #16]
	.loc 1 364 0
	b	.L62
.L61:
	.loc 1 366 0
	ldr	r3, [sp, #40]
	orr	r3, r3, #4
	str	r3, [sp, #40]
	.loc 1 367 0
	ldr	r3, [sp, #16]
	adds	r3, r3, #1
	str	r3, [sp, #16]
	.loc 1 368 0
	b	.L62
.L83:
	.loc 1 370 0
	movs	r3, #0
	str	r3, [sp, #48]
	.loc 1 371 0
	nop
.L62:
	.loc 1 373 0
	ldr	r3, [sp, #48]
	cmp	r3, #0
	bne	.L63
	.loc 1 378 0
	movs	r3, #0
	str	r3, [sp, #36]
.L66:
	.loc 1 381 0
	ldrb	r3, [sp, #55]	@ zero_extendqisi2
	cmp	r3, #42
	bne	.L64
	.loc 1 384 0
	ldr	r3, [sp, #12]
	ldr	r3, [r3]
	adds	r1, r3, #4
	ldr	r2, [sp, #12]
	str	r1, [r2]
	ldr	r3, [r3]
	ldr	r2, [sp, #36]
	add	r3, r3, r2
	str	r3, [sp, #36]
	.loc 1 386 0
	ldr	r3, [sp, #16]
	adds	r3, r3, #1
	str	r3, [sp, #16]
	.loc 1 387 0
	b	.L65
.L64:
	.loc 1 389 0
	ldr	r3, [sp, #16]
	ldrb	r3, [r3]
	strb	r3, [sp, #55]
	.loc 1 390 0
	ldrb	r3, [sp, #55]	@ zero_extendqisi2
	cmp	r3, #47
	bls	.L65
	.loc 1 390 0 is_stmt 0 discriminator 1
	ldrb	r3, [sp, #55]	@ zero_extendqisi2
	cmp	r3, #57
	bhi	.L65
	.loc 1 394 0 is_stmt 1
	ldr	r3, [sp, #16]
	adds	r3, r3, #1
	str	r3, [sp, #16]
	.loc 1 395 0
	ldr	r2, [sp, #36]
	mov	r3, r2
	lsls	r3, r3, #2
	add	r3, r3, r2
	lsls	r3, r3, #1
	mov	r2, r3
	ldrb	r3, [sp, #55]	@ zero_extendqisi2
	add	r3, r3, r2
	subs	r3, r3, #48
	str	r3, [sp, #36]
	.loc 1 381 0
	b	.L66
.L65:
	.loc 1 401 0
	movs	r3, #0
	str	r3, [sp, #44]
	.loc 1 402 0
	ldr	r3, [sp, #16]
	ldrb	r3, [r3]
	strb	r3, [sp, #55]
	.loc 1 403 0
	ldrb	r3, [sp, #55]	@ zero_extendqisi2
	cmp	r3, #46
	bne	.L67
	.loc 1 405 0
	ldr	r3, [sp, #16]
	adds	r3, r3, #1
	str	r3, [sp, #16]
.L68:
	.loc 1 408 0
	ldr	r3, [sp, #16]
	ldrb	r3, [r3]
	strb	r3, [sp, #55]
	.loc 1 409 0
	ldrb	r3, [sp, #55]	@ zero_extendqisi2
	cmp	r3, #47
	bls	.L67
	.loc 1 409 0 is_stmt 0 discriminator 1
	ldrb	r3, [sp, #55]	@ zero_extendqisi2
	cmp	r3, #57
	bhi	.L67
	.loc 1 413 0 is_stmt 1
	ldr	r3, [sp, #16]
	adds	r3, r3, #1
	str	r3, [sp, #16]
	.loc 1 414 0
	ldr	r2, [sp, #44]
	mov	r3, r2
	lsls	r3, r3, #2
	add	r3, r3, r2
	lsls	r3, r3, #1
	mov	r2, r3
	ldrb	r3, [sp, #55]	@ zero_extendqisi2
	add	r3, r3, r2
	subs	r3, r3, #48
	str	r3, [sp, #44]
	.loc 1 408 0
	b	.L68
.L67:
	.loc 1 420 0
	ldr	r3, [sp, #16]
	ldrb	r3, [r3]
	strb	r3, [sp, #55]
.L71:
	.loc 1 423 0
	ldrb	r3, [sp, #55]	@ zero_extendqisi2
	cmp	r3, #108
	beq	.L69
	.loc 1 423 0 is_stmt 0 discriminator 1
	ldrb	r3, [sp, #55]	@ zero_extendqisi2
	cmp	r3, #104
	bne	.L70
.L69:
	.loc 1 425 0 is_stmt 1
	ldr	r3, [sp, #16]
	adds	r3, r3, #1
	str	r3, [sp, #16]
	.loc 1 426 0
	ldr	r3, [sp, #16]
	ldrb	r3, [r3]
	strb	r3, [sp, #55]
	.loc 1 423 0
	b	.L71
.L70:
	.loc 1 437 0
	ldrb	r3, [sp, #55]	@ zero_extendqisi2
	subs	r3, r3, #37
	cmp	r3, #83
	bhi	.L86
	adr	r2, .L74
	ldr	pc, [r2, r3, lsl #2]
	.p2align 2
.L74:
	.word	.L73+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L75+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L76+1
	.word	.L77+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L77+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L86+1
	.word	.L78+1
	.word	.L86+1
	.word	.L86+1
	.word	.L79+1
	.word	.L86+1
	.word	.L80+1
	.word	.L86+1
	.word	.L86+1
	.word	.L75+1
	.p2align 1
.L76:
.LBB2:
	.loc 1 442 0
	ldr	r3, [sp, #12]
	ldr	r3, [r3]
	adds	r1, r3, #4
	ldr	r2, [sp, #12]
	str	r1, [r2]
	ldr	r3, [r3]
	str	r3, [sp, #48]
	.loc 1 443 0
	ldr	r3, [sp, #48]
	strb	r3, [sp, #31]
	.loc 1 444 0
	ldrb	r3, [sp, #31]	@ zero_extendqisi2
	mov	r1, r3
	ldr	r0, [sp, #20]
	bl	buffer_add
	.loc 1 445 0
	b	.L81
.L77:
.LBE2:
	.loc 1 449 0
	ldr	r3, [sp, #12]
	ldr	r3, [r3]
	adds	r1, r3, #4
	ldr	r2, [sp, #12]
	str	r1, [r2]
	ldr	r3, [r3]
	str	r3, [sp, #48]
	.loc 1 450 0
	ldr	r3, [sp, #40]
	str	r3, [sp, #4]
	ldr	r3, [sp, #36]
	str	r3, [sp]
	ldr	r3, [sp, #44]
	movs	r2, #10
	ldr	r1, [sp, #48]
	ldr	r0, [sp, #20]
	bl	int_print
	.loc 1 456 0
	b	.L81
.L80:
	.loc 1 458 0
	ldr	r3, [sp, #12]
	ldr	r3, [r3]
	adds	r1, r3, #4
	ldr	r2, [sp, #12]
	str	r1, [r2]
	ldr	r3, [r3]
	str	r3, [sp, #48]
	.loc 1 459 0
	ldr	r1, [sp, #48]
	ldr	r3, [sp, #40]
	str	r3, [sp, #4]
	ldr	r3, [sp, #36]
	str	r3, [sp]
	ldr	r3, [sp, #44]
	movs	r2, #10
	ldr	r0, [sp, #20]
	bl	unsigned_print
	.loc 1 465 0
	b	.L81
.L75:
	.loc 1 468 0
	ldr	r3, [sp, #12]
	ldr	r3, [r3]
	adds	r1, r3, #4
	ldr	r2, [sp, #12]
	str	r1, [r2]
	ldr	r3, [r3]
	str	r3, [sp, #48]
	.loc 1 469 0
	ldr	r1, [sp, #48]
	ldr	r3, [sp, #40]
	str	r3, [sp, #4]
	ldr	r3, [sp, #36]
	str	r3, [sp]
	ldr	r3, [sp, #44]
	movs	r2, #16
	ldr	r0, [sp, #20]
	bl	unsigned_print
	.loc 1 475 0
	b	.L81
.L79:
.LBB3:
	.loc 1 478 0
	ldr	r3, [sp, #12]
	ldr	r3, [r3]
	adds	r1, r3, #4
	ldr	r2, [sp, #12]
	str	r1, [r2]
	ldr	r3, [r3]
	str	r3, [sp, #32]
	.loc 1 479 0
	ldr	r3, [sp, #40]
	ldr	r2, [sp, #36]
	ldr	r1, [sp, #32]
	ldr	r0, [sp, #20]
	bl	string_print
	.loc 1 480 0
	b	.L81
.L78:
.LBE3:
	.loc 1 483 0
	ldr	r3, [sp, #12]
	ldr	r3, [r3]
	adds	r1, r3, #4
	ldr	r2, [sp, #12]
	str	r1, [r2]
	ldr	r3, [r3]
	str	r3, [sp, #48]
	.loc 1 484 0
	movs	r1, #48
	ldr	r0, [sp, #20]
	bl	buffer_add
	.loc 1 485 0
	movs	r1, #120
	ldr	r0, [sp, #20]
	bl	buffer_add
	.loc 1 486 0
	ldr	r1, [sp, #48]
	movs	r3, #0
	str	r3, [sp, #4]
	movs	r3, #8
	str	r3, [sp]
	movs	r3, #8
	movs	r2, #16
	ldr	r0, [sp, #20]
	bl	unsigned_print
	.loc 1 487 0
	b	.L81
.L73:
	.loc 1 489 0
	movs	r1, #37
	ldr	r0, [sp, #20]
	bl	buffer_add
	.loc 1 490 0
	b	.L81
.L86:
	.loc 1 492 0
	nop
.L81:
	.loc 1 495 0
	ldr	r3, [sp, #16]
	adds	r3, r3, #1
	str	r3, [sp, #16]
	b	.L82
.L57:
	.loc 1 499 0
	ldrb	r3, [sp, #55]	@ zero_extendqisi2
	mov	r1, r3
	ldr	r0, [sp, #20]
	bl	buffer_add
.L82:
	.loc 1 501 0
	ldr	r3, [sp, #16]
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L53
	b	.L56
.L85:
	.loc 1 342 0
	nop
.L56:
	.loc 1 503 0
	ldr	r3, [sp, #20]
	ldrb	r3, [r3, #12]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L52
	.loc 1 505 0
	ldr	r0, [sp, #20]
	bl	nrf_fprintf_buffer_flush
	b	.L52
.L84:
	.loc 1 326 0
	nop
.L52:
	.loc 1 507 0
	add	sp, sp, #60
.LCFI14:
	@ sp needed
	ldr	pc, [sp], #4
.LFE151:
	.size	nrf_fprintf_fmt, .-nrf_fprintf_fmt
	.section	.rodata._aV2C.7643,"a",%progbits
	.align	2
	.type	_aV2C.7643, %object
	.size	_aV2C.7643, 16
_aV2C.7643:
	.byte	48
	.byte	49
	.byte	50
	.byte	51
	.byte	52
	.byte	53
	.byte	54
	.byte	55
	.byte	56
	.byte	57
	.byte	65
	.byte	66
	.byte	67
	.byte	68
	.byte	69
	.byte	70
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
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI1-.LCFI0
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.4byte	.LCFI2-.LCFI1
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE0:
.LSFDE2:
	.4byte	.LEFDE2-.LASFDE2
.LASFDE2:
	.4byte	.Lframe0
	.4byte	.LFB148
	.4byte	.LFE148-.LFB148
	.byte	0x4
	.4byte	.LCFI3-.LFB148
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI4-.LCFI3
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.4byte	.LCFI5-.LCFI4
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE2:
.LSFDE4:
	.4byte	.LEFDE4-.LASFDE4
.LASFDE4:
	.4byte	.Lframe0
	.4byte	.LFB149
	.4byte	.LFE149-.LFB149
	.byte	0x4
	.4byte	.LCFI6-.LFB149
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI7-.LCFI6
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.4byte	.LCFI8-.LCFI7
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE4:
.LSFDE6:
	.4byte	.LEFDE6-.LASFDE6
.LASFDE6:
	.4byte	.Lframe0
	.4byte	.LFB150
	.4byte	.LFE150-.LFB150
	.byte	0x4
	.4byte	.LCFI9-.LFB150
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI10-.LCFI9
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.4byte	.LCFI11-.LCFI10
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE6:
.LSFDE8:
	.4byte	.LEFDE8-.LASFDE8
.LASFDE8:
	.4byte	.Lframe0
	.4byte	.LFB151
	.4byte	.LFE151-.LFB151
	.byte	0x4
	.4byte	.LCFI12-.LFB151
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI13-.LCFI12
	.byte	0xe
	.uleb128 0x40
	.byte	0x4
	.4byte	.LCFI14-.LCFI13
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE8:
	.text
.Letext0:
	.file 2 "C:/Program Files/SEGGER/SEGGER Embedded Studio for ARM 4.12/include/stdint.h"
	.file 3 "C:/Program Files/SEGGER/SEGGER Embedded Studio for ARM 4.12/include/__crossworks.h"
	.file 4 "<built-in>"
	.file 5 "C:/Program Files/SEGGER/SEGGER Embedded Studio for ARM 4.12/include/string.h"
	.file 6 "C:/Users/paulr/AppData/Local/SEGGER/SEGGER Embedded Studio/v3/packages/CMSIS_5/CMSIS/Core/Include/core_cm4.h"
	.file 7 "C:/Users/paulr/AppData/Local/SEGGER/SEGGER Embedded Studio/v3/packages/nRF/CMSIS/Device/Include/system_nrf52.h"
	.file 8 "../../../nRF5_SDK_14.2.0/components/libraries/util/app_util.h"
	.file 9 "C:/Program Files/SEGGER/SEGGER Embedded Studio for ARM 4.12/include/stdarg.h"
	.file 10 "C:/Program Files/SEGGER/SEGGER Embedded Studio for ARM 4.12/include/stdio.h"
	.file 11 "C:\\Users\\paulr\\OneDrive\\Documents\\DARPA\\darpasubt\\darpasubt\\firmware\\dwm1001_dev_source_code\\nRF5_SDK_14.2.0\\external\\fprintf\\nrf_fprintf.h"
	.section	.debug_info,"",%progbits
.Ldebug_info0:
	.4byte	0x99b
	.2byte	0x4
	.4byte	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF125
	.byte	0xc
	.4byte	.LASF126
	.4byte	.LASF127
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
	.uleb128 0x3
	.4byte	.LASF11
	.byte	0x3
	.byte	0x4c
	.4byte	0x9e
	.uleb128 0x7
	.4byte	.LASF11
	.byte	0x4
	.byte	0x4
	.byte	0
	.4byte	0xb5
	.uleb128 0x8
	.4byte	.LASF128
	.4byte	0xb5
	.byte	0
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.uleb128 0x7
	.4byte	.LASF12
	.byte	0x8
	.byte	0x3
	.byte	0x7e
	.4byte	0xdc
	.uleb128 0xa
	.4byte	.LASF13
	.byte	0x3
	.byte	0x7f
	.4byte	0x6c
	.byte	0
	.uleb128 0xa
	.4byte	.LASF14
	.byte	0x3
	.byte	0x80
	.4byte	0xdc
	.byte	0x4
	.byte	0
	.uleb128 0x2
	.byte	0x4
	.byte	0x5
	.4byte	.LASF15
	.uleb128 0xb
	.4byte	0x6c
	.4byte	0xfc
	.uleb128 0xc
	.4byte	0xfc
	.uleb128 0xc
	.4byte	0x7e
	.uleb128 0xc
	.4byte	0x113
	.byte	0
	.uleb128 0xd
	.byte	0x4
	.4byte	0x107
	.uleb128 0x4
	.4byte	0xfc
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.4byte	.LASF16
	.uleb128 0x4
	.4byte	0x107
	.uleb128 0xd
	.byte	0x4
	.4byte	0xb7
	.uleb128 0xb
	.4byte	0x6c
	.4byte	0x137
	.uleb128 0xc
	.4byte	0x137
	.uleb128 0xc
	.4byte	0x13d
	.uleb128 0xc
	.4byte	0x7e
	.uleb128 0xc
	.4byte	0x113
	.byte	0
	.uleb128 0xd
	.byte	0x4
	.4byte	0x7e
	.uleb128 0xd
	.byte	0x4
	.4byte	0x10e
	.uleb128 0xe
	.byte	0x58
	.byte	0x3
	.byte	0x86
	.4byte	0x2cc
	.uleb128 0xa
	.4byte	.LASF17
	.byte	0x3
	.byte	0x88
	.4byte	0x13d
	.byte	0
	.uleb128 0xa
	.4byte	.LASF18
	.byte	0x3
	.byte	0x89
	.4byte	0x13d
	.byte	0x4
	.uleb128 0xa
	.4byte	.LASF19
	.byte	0x3
	.byte	0x8a
	.4byte	0x13d
	.byte	0x8
	.uleb128 0xa
	.4byte	.LASF20
	.byte	0x3
	.byte	0x8c
	.4byte	0x13d
	.byte	0xc
	.uleb128 0xa
	.4byte	.LASF21
	.byte	0x3
	.byte	0x8d
	.4byte	0x13d
	.byte	0x10
	.uleb128 0xa
	.4byte	.LASF22
	.byte	0x3
	.byte	0x8e
	.4byte	0x13d
	.byte	0x14
	.uleb128 0xa
	.4byte	.LASF23
	.byte	0x3
	.byte	0x8f
	.4byte	0x13d
	.byte	0x18
	.uleb128 0xa
	.4byte	.LASF24
	.byte	0x3
	.byte	0x90
	.4byte	0x13d
	.byte	0x1c
	.uleb128 0xa
	.4byte	.LASF25
	.byte	0x3
	.byte	0x91
	.4byte	0x13d
	.byte	0x20
	.uleb128 0xa
	.4byte	.LASF26
	.byte	0x3
	.byte	0x92
	.4byte	0x13d
	.byte	0x24
	.uleb128 0xa
	.4byte	.LASF27
	.byte	0x3
	.byte	0x94
	.4byte	0x107
	.byte	0x28
	.uleb128 0xa
	.4byte	.LASF28
	.byte	0x3
	.byte	0x95
	.4byte	0x107
	.byte	0x29
	.uleb128 0xa
	.4byte	.LASF29
	.byte	0x3
	.byte	0x96
	.4byte	0x107
	.byte	0x2a
	.uleb128 0xa
	.4byte	.LASF30
	.byte	0x3
	.byte	0x97
	.4byte	0x107
	.byte	0x2b
	.uleb128 0xa
	.4byte	.LASF31
	.byte	0x3
	.byte	0x98
	.4byte	0x107
	.byte	0x2c
	.uleb128 0xa
	.4byte	.LASF32
	.byte	0x3
	.byte	0x99
	.4byte	0x107
	.byte	0x2d
	.uleb128 0xa
	.4byte	.LASF33
	.byte	0x3
	.byte	0x9a
	.4byte	0x107
	.byte	0x2e
	.uleb128 0xa
	.4byte	.LASF34
	.byte	0x3
	.byte	0x9b
	.4byte	0x107
	.byte	0x2f
	.uleb128 0xa
	.4byte	.LASF35
	.byte	0x3
	.byte	0x9c
	.4byte	0x107
	.byte	0x30
	.uleb128 0xa
	.4byte	.LASF36
	.byte	0x3
	.byte	0x9d
	.4byte	0x107
	.byte	0x31
	.uleb128 0xa
	.4byte	.LASF37
	.byte	0x3
	.byte	0x9e
	.4byte	0x107
	.byte	0x32
	.uleb128 0xa
	.4byte	.LASF38
	.byte	0x3
	.byte	0x9f
	.4byte	0x107
	.byte	0x33
	.uleb128 0xa
	.4byte	.LASF39
	.byte	0x3
	.byte	0xa0
	.4byte	0x107
	.byte	0x34
	.uleb128 0xa
	.4byte	.LASF40
	.byte	0x3
	.byte	0xa1
	.4byte	0x107
	.byte	0x35
	.uleb128 0xa
	.4byte	.LASF41
	.byte	0x3
	.byte	0xa6
	.4byte	0x13d
	.byte	0x38
	.uleb128 0xa
	.4byte	.LASF42
	.byte	0x3
	.byte	0xa7
	.4byte	0x13d
	.byte	0x3c
	.uleb128 0xa
	.4byte	.LASF43
	.byte	0x3
	.byte	0xa8
	.4byte	0x13d
	.byte	0x40
	.uleb128 0xa
	.4byte	.LASF44
	.byte	0x3
	.byte	0xa9
	.4byte	0x13d
	.byte	0x44
	.uleb128 0xa
	.4byte	.LASF45
	.byte	0x3
	.byte	0xaa
	.4byte	0x13d
	.byte	0x48
	.uleb128 0xa
	.4byte	.LASF46
	.byte	0x3
	.byte	0xab
	.4byte	0x13d
	.byte	0x4c
	.uleb128 0xa
	.4byte	.LASF47
	.byte	0x3
	.byte	0xac
	.4byte	0x13d
	.byte	0x50
	.uleb128 0xa
	.4byte	.LASF48
	.byte	0x3
	.byte	0xad
	.4byte	0x13d
	.byte	0x54
	.byte	0
	.uleb128 0x3
	.4byte	.LASF49
	.byte	0x3
	.byte	0xae
	.4byte	0x143
	.uleb128 0x4
	.4byte	0x2cc
	.uleb128 0xe
	.byte	0x20
	.byte	0x3
	.byte	0xc4
	.4byte	0x345
	.uleb128 0xa
	.4byte	.LASF50
	.byte	0x3
	.byte	0xc6
	.4byte	0x359
	.byte	0
	.uleb128 0xa
	.4byte	.LASF51
	.byte	0x3
	.byte	0xc7
	.4byte	0x36e
	.byte	0x4
	.uleb128 0xa
	.4byte	.LASF52
	.byte	0x3
	.byte	0xc8
	.4byte	0x36e
	.byte	0x8
	.uleb128 0xa
	.4byte	.LASF53
	.byte	0x3
	.byte	0xcb
	.4byte	0x388
	.byte	0xc
	.uleb128 0xa
	.4byte	.LASF54
	.byte	0x3
	.byte	0xcc
	.4byte	0x39d
	.byte	0x10
	.uleb128 0xa
	.4byte	.LASF55
	.byte	0x3
	.byte	0xcd
	.4byte	0x39d
	.byte	0x14
	.uleb128 0xa
	.4byte	.LASF56
	.byte	0x3
	.byte	0xd0
	.4byte	0x3a3
	.byte	0x18
	.uleb128 0xa
	.4byte	.LASF57
	.byte	0x3
	.byte	0xd1
	.4byte	0x3a9
	.byte	0x1c
	.byte	0
	.uleb128 0xb
	.4byte	0x6c
	.4byte	0x359
	.uleb128 0xc
	.4byte	0x6c
	.uleb128 0xc
	.4byte	0x6c
	.byte	0
	.uleb128 0xd
	.byte	0x4
	.4byte	0x345
	.uleb128 0xb
	.4byte	0x6c
	.4byte	0x36e
	.uleb128 0xc
	.4byte	0x6c
	.byte	0
	.uleb128 0xd
	.byte	0x4
	.4byte	0x35f
	.uleb128 0xb
	.4byte	0x6c
	.4byte	0x388
	.uleb128 0xc
	.4byte	0xdc
	.uleb128 0xc
	.4byte	0x6c
	.byte	0
	.uleb128 0xd
	.byte	0x4
	.4byte	0x374
	.uleb128 0xb
	.4byte	0xdc
	.4byte	0x39d
	.uleb128 0xc
	.4byte	0xdc
	.byte	0
	.uleb128 0xd
	.byte	0x4
	.4byte	0x38e
	.uleb128 0xd
	.byte	0x4
	.4byte	0xe3
	.uleb128 0xd
	.byte	0x4
	.4byte	0x119
	.uleb128 0x3
	.4byte	.LASF58
	.byte	0x3
	.byte	0xd2
	.4byte	0x2dc
	.uleb128 0x4
	.4byte	0x3af
	.uleb128 0xe
	.byte	0xc
	.byte	0x3
	.byte	0xd4
	.4byte	0x3ec
	.uleb128 0xa
	.4byte	.LASF59
	.byte	0x3
	.byte	0xd5
	.4byte	0x13d
	.byte	0
	.uleb128 0xa
	.4byte	.LASF60
	.byte	0x3
	.byte	0xd6
	.4byte	0x3ec
	.byte	0x4
	.uleb128 0xa
	.4byte	.LASF61
	.byte	0x3
	.byte	0xd7
	.4byte	0x3f2
	.byte	0x8
	.byte	0
	.uleb128 0xd
	.byte	0x4
	.4byte	0x2d7
	.uleb128 0xd
	.byte	0x4
	.4byte	0x3ba
	.uleb128 0x3
	.4byte	.LASF62
	.byte	0x3
	.byte	0xd8
	.4byte	0x3bf
	.uleb128 0x4
	.4byte	0x3f8
	.uleb128 0x7
	.4byte	.LASF63
	.byte	0x14
	.byte	0x3
	.byte	0xdc
	.4byte	0x421
	.uleb128 0xa
	.4byte	.LASF64
	.byte	0x3
	.byte	0xdd
	.4byte	0x421
	.byte	0
	.byte	0
	.uleb128 0xf
	.4byte	0x431
	.4byte	0x431
	.uleb128 0x10
	.4byte	0x7e
	.byte	0x4
	.byte	0
	.uleb128 0xd
	.byte	0x4
	.4byte	0x403
	.uleb128 0x11
	.4byte	.LASF65
	.byte	0x3
	.2byte	0x106
	.4byte	0x408
	.uleb128 0x11
	.4byte	.LASF66
	.byte	0x3
	.2byte	0x10d
	.4byte	0x403
	.uleb128 0x11
	.4byte	.LASF67
	.byte	0x3
	.2byte	0x110
	.4byte	0x3ba
	.uleb128 0x11
	.4byte	.LASF68
	.byte	0x3
	.2byte	0x111
	.4byte	0x3ba
	.uleb128 0xf
	.4byte	0x3e
	.4byte	0x477
	.uleb128 0x10
	.4byte	0x7e
	.byte	0x7f
	.byte	0
	.uleb128 0x4
	.4byte	0x467
	.uleb128 0x11
	.4byte	.LASF69
	.byte	0x3
	.2byte	0x113
	.4byte	0x477
	.uleb128 0xf
	.4byte	0x10e
	.4byte	0x493
	.uleb128 0x12
	.byte	0
	.uleb128 0x4
	.4byte	0x488
	.uleb128 0x11
	.4byte	.LASF70
	.byte	0x3
	.2byte	0x115
	.4byte	0x493
	.uleb128 0x11
	.4byte	.LASF71
	.byte	0x3
	.2byte	0x116
	.4byte	0x493
	.uleb128 0x11
	.4byte	.LASF72
	.byte	0x3
	.2byte	0x117
	.4byte	0x493
	.uleb128 0x11
	.4byte	.LASF73
	.byte	0x3
	.2byte	0x118
	.4byte	0x493
	.uleb128 0x11
	.4byte	.LASF74
	.byte	0x3
	.2byte	0x11a
	.4byte	0x493
	.uleb128 0x11
	.4byte	.LASF75
	.byte	0x3
	.2byte	0x11b
	.4byte	0x493
	.uleb128 0x11
	.4byte	.LASF76
	.byte	0x3
	.2byte	0x11c
	.4byte	0x493
	.uleb128 0x11
	.4byte	.LASF77
	.byte	0x3
	.2byte	0x11d
	.4byte	0x493
	.uleb128 0x11
	.4byte	.LASF78
	.byte	0x3
	.2byte	0x11e
	.4byte	0x493
	.uleb128 0x11
	.4byte	.LASF79
	.byte	0x3
	.2byte	0x11f
	.4byte	0x493
	.uleb128 0xb
	.4byte	0x6c
	.4byte	0x51f
	.uleb128 0xc
	.4byte	0x51f
	.byte	0
	.uleb128 0xd
	.byte	0x4
	.4byte	0x52a
	.uleb128 0x13
	.4byte	.LASF95
	.uleb128 0x4
	.4byte	0x525
	.uleb128 0x11
	.4byte	.LASF80
	.byte	0x3
	.2byte	0x135
	.4byte	0x53b
	.uleb128 0xd
	.byte	0x4
	.4byte	0x510
	.uleb128 0xb
	.4byte	0x6c
	.4byte	0x550
	.uleb128 0xc
	.4byte	0x550
	.byte	0
	.uleb128 0xd
	.byte	0x4
	.4byte	0x525
	.uleb128 0x11
	.4byte	.LASF81
	.byte	0x3
	.2byte	0x136
	.4byte	0x562
	.uleb128 0xd
	.byte	0x4
	.4byte	0x541
	.uleb128 0x14
	.4byte	.LASF82
	.byte	0x3
	.2byte	0x14d
	.4byte	0x574
	.uleb128 0xd
	.byte	0x4
	.4byte	0x57a
	.uleb128 0xb
	.4byte	0x13d
	.4byte	0x589
	.uleb128 0xc
	.4byte	0x6c
	.byte	0
	.uleb128 0x15
	.4byte	.LASF83
	.byte	0x8
	.byte	0x3
	.2byte	0x14f
	.4byte	0x5b1
	.uleb128 0x16
	.4byte	.LASF84
	.byte	0x3
	.2byte	0x151
	.4byte	0x568
	.byte	0
	.uleb128 0x16
	.4byte	.LASF85
	.byte	0x3
	.2byte	0x152
	.4byte	0x5b1
	.byte	0x4
	.byte	0
	.uleb128 0xd
	.byte	0x4
	.4byte	0x589
	.uleb128 0x14
	.4byte	.LASF86
	.byte	0x3
	.2byte	0x153
	.4byte	0x589
	.uleb128 0x11
	.4byte	.LASF87
	.byte	0x3
	.2byte	0x157
	.4byte	0x5cf
	.uleb128 0xd
	.byte	0x4
	.4byte	0x5b7
	.uleb128 0x3
	.4byte	.LASF88
	.byte	0x5
	.byte	0x37
	.4byte	0x7e
	.uleb128 0x4
	.4byte	0x5d5
	.uleb128 0x11
	.4byte	.LASF89
	.byte	0x6
	.2byte	0x804
	.4byte	0x67
	.uleb128 0x17
	.4byte	.LASF90
	.byte	0x7
	.byte	0x21
	.4byte	0x73
	.uleb128 0x17
	.4byte	.LASF91
	.byte	0x8
	.byte	0x4b
	.4byte	0x73
	.uleb128 0x17
	.4byte	.LASF92
	.byte	0x8
	.byte	0x4c
	.4byte	0x73
	.uleb128 0x3
	.4byte	.LASF93
	.byte	0x9
	.byte	0x45
	.4byte	0x93
	.uleb128 0x14
	.4byte	.LASF94
	.byte	0xa
	.2byte	0x317
	.4byte	0x629
	.uleb128 0x13
	.4byte	.LASF96
	.uleb128 0x11
	.4byte	.LASF97
	.byte	0xa
	.2byte	0x31b
	.4byte	0x63a
	.uleb128 0xd
	.byte	0x4
	.4byte	0x61d
	.uleb128 0x11
	.4byte	.LASF98
	.byte	0xa
	.2byte	0x31c
	.4byte	0x63a
	.uleb128 0x11
	.4byte	.LASF99
	.byte	0xa
	.2byte	0x31d
	.4byte	0x63a
	.uleb128 0x3
	.4byte	.LASF100
	.byte	0xb
	.byte	0x32
	.4byte	0x663
	.uleb128 0xd
	.byte	0x4
	.4byte	0x669
	.uleb128 0x18
	.4byte	0x67e
	.uleb128 0xc
	.4byte	0x67e
	.uleb128 0xc
	.4byte	0x13d
	.uleb128 0xc
	.4byte	0x5d5
	.byte	0
	.uleb128 0xd
	.byte	0x4
	.4byte	0x689
	.uleb128 0x4
	.4byte	0x67e
	.uleb128 0x19
	.uleb128 0x7
	.4byte	.LASF101
	.byte	0x18
	.byte	0xb
	.byte	0x37
	.4byte	0x6df
	.uleb128 0xa
	.4byte	.LASF102
	.byte	0xb
	.byte	0x39
	.4byte	0x102
	.byte	0
	.uleb128 0xa
	.4byte	.LASF103
	.byte	0xb
	.byte	0x3a
	.4byte	0x5e0
	.byte	0x4
	.uleb128 0xa
	.4byte	.LASF104
	.byte	0xb
	.byte	0x3b
	.4byte	0x5d5
	.byte	0x8
	.uleb128 0xa
	.4byte	.LASF105
	.byte	0xb
	.byte	0x3c
	.4byte	0x6df
	.byte	0xc
	.uleb128 0xa
	.4byte	.LASF106
	.byte	0xb
	.byte	0x3e
	.4byte	0x684
	.byte	0x10
	.uleb128 0xa
	.4byte	.LASF107
	.byte	0xb
	.byte	0x40
	.4byte	0x658
	.byte	0x14
	.byte	0
	.uleb128 0x2
	.byte	0x1
	.byte	0x2
	.4byte	.LASF108
	.uleb128 0x3
	.4byte	.LASF109
	.byte	0xb
	.byte	0x41
	.4byte	0x68a
	.uleb128 0x1a
	.4byte	.LASF129
	.byte	0x1
	.2byte	0x13a
	.4byte	.LFB151
	.4byte	.LFE151-.LFB151
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x7b1
	.uleb128 0x1b
	.4byte	.LASF110
	.byte	0x1
	.2byte	0x13a
	.4byte	0x7b7
	.uleb128 0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0x1b
	.4byte	.LASF111
	.byte	0x1
	.2byte	0x13b
	.4byte	0x13d
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x1b
	.4byte	.LASF112
	.byte	0x1
	.2byte	0x13c
	.4byte	0x7bc
	.uleb128 0x2
	.byte	0x91
	.sleb128 -52
	.uleb128 0x1c
	.ascii	"c\000"
	.byte	0x1
	.2byte	0x149
	.4byte	0x107
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0x1c
	.ascii	"v\000"
	.byte	0x1
	.2byte	0x14a
	.4byte	0x5c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x1d
	.4byte	.LASF113
	.byte	0x1
	.2byte	0x14b
	.4byte	0x73
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x1d
	.4byte	.LASF114
	.byte	0x1
	.2byte	0x14c
	.4byte	0x73
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x1d
	.4byte	.LASF115
	.byte	0x1
	.2byte	0x14d
	.4byte	0x73
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x1e
	.4byte	.LBB2
	.4byte	.LBE2-.LBB2
	.4byte	0x797
	.uleb128 0x1c
	.ascii	"c0\000"
	.byte	0x1
	.2byte	0x1b9
	.4byte	0x107
	.uleb128 0x2
	.byte	0x91
	.sleb128 -33
	.byte	0
	.uleb128 0x1f
	.4byte	.LBB3
	.4byte	.LBE3-.LBB3
	.uleb128 0x1c
	.ascii	"p_s\000"
	.byte	0x1
	.2byte	0x1de
	.4byte	0x13d
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.byte	0
	.byte	0
	.uleb128 0xd
	.byte	0x4
	.4byte	0x6e6
	.uleb128 0x4
	.4byte	0x7b1
	.uleb128 0xd
	.byte	0x4
	.4byte	0x612
	.uleb128 0x20
	.4byte	.LASF119
	.byte	0x1
	.byte	0xe8
	.4byte	.LFB150
	.4byte	.LFE150-.LFB150
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x846
	.uleb128 0x21
	.4byte	.LASF110
	.byte	0x1
	.byte	0xe8
	.4byte	0x7b7
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x22
	.ascii	"v\000"
	.byte	0x1
	.byte	0xe9
	.4byte	0x5c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x21
	.4byte	.LASF116
	.byte	0x1
	.byte	0xea
	.4byte	0x73
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x21
	.4byte	.LASF113
	.byte	0x1
	.byte	0xeb
	.4byte	0x73
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x21
	.4byte	.LASF115
	.byte	0x1
	.byte	0xec
	.4byte	0x73
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x21
	.4byte	.LASF114
	.byte	0x1
	.byte	0xed
	.4byte	0x73
	.uleb128 0x2
	.byte	0x91
	.sleb128 4
	.uleb128 0x23
	.4byte	.LASF117
	.byte	0x1
	.byte	0xef
	.4byte	0x73
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x23
	.4byte	.LASF118
	.byte	0x1
	.byte	0xf0
	.4byte	0x5c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.byte	0
	.uleb128 0x20
	.4byte	.LASF120
	.byte	0x1
	.byte	0x7f
	.4byte	.LFB149
	.4byte	.LFE149-.LFB149
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x8f5
	.uleb128 0x21
	.4byte	.LASF110
	.byte	0x1
	.byte	0x7f
	.4byte	0x7b7
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x22
	.ascii	"v\000"
	.byte	0x1
	.byte	0x80
	.4byte	0x73
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x21
	.4byte	.LASF116
	.byte	0x1
	.byte	0x81
	.4byte	0x73
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0x21
	.4byte	.LASF113
	.byte	0x1
	.byte	0x82
	.4byte	0x73
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x21
	.4byte	.LASF115
	.byte	0x1
	.byte	0x83
	.4byte	0x73
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x21
	.4byte	.LASF114
	.byte	0x1
	.byte	0x84
	.4byte	0x73
	.uleb128 0x2
	.byte	0x91
	.sleb128 4
	.uleb128 0x23
	.4byte	.LASF121
	.byte	0x1
	.byte	0x86
	.4byte	0x905
	.uleb128 0x5
	.byte	0x3
	.4byte	_aV2C.7643
	.uleb128 0x24
	.ascii	"Div\000"
	.byte	0x1
	.byte	0x88
	.4byte	0x73
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x23
	.4byte	.LASF122
	.byte	0x1
	.byte	0x89
	.4byte	0x73
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x23
	.4byte	.LASF117
	.byte	0x1
	.byte	0x8a
	.4byte	0x73
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x24
	.ascii	"c\000"
	.byte	0x1
	.byte	0x8b
	.4byte	0x107
	.uleb128 0x2
	.byte	0x91
	.sleb128 -17
	.byte	0
	.uleb128 0xf
	.4byte	0x10e
	.4byte	0x905
	.uleb128 0x10
	.4byte	0x7e
	.byte	0xf
	.byte	0
	.uleb128 0x4
	.4byte	0x8f5
	.uleb128 0x20
	.4byte	.LASF123
	.byte	0x1
	.byte	0x52
	.4byte	.LFB148
	.4byte	.LFE148-.LFB148
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x972
	.uleb128 0x21
	.4byte	.LASF110
	.byte	0x1
	.byte	0x52
	.4byte	0x7b7
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x21
	.4byte	.LASF124
	.byte	0x1
	.byte	0x53
	.4byte	0x13d
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x21
	.4byte	.LASF115
	.byte	0x1
	.byte	0x54
	.4byte	0x73
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x21
	.4byte	.LASF114
	.byte	0x1
	.byte	0x55
	.4byte	0x73
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x23
	.4byte	.LASF117
	.byte	0x1
	.byte	0x57
	.4byte	0x73
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x24
	.ascii	"c\000"
	.byte	0x1
	.byte	0x58
	.4byte	0x107
	.uleb128 0x2
	.byte	0x91
	.sleb128 -13
	.byte	0
	.uleb128 0x25
	.4byte	.LASF130
	.byte	0x1
	.byte	0x48
	.4byte	.LFB147
	.4byte	.LFE147-.LFB147
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x21
	.4byte	.LASF110
	.byte	0x1
	.byte	0x48
	.4byte	0x7b7
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x22
	.ascii	"c\000"
	.byte	0x1
	.byte	0x48
	.4byte	0x107
	.uleb128 0x2
	.byte	0x91
	.sleb128 -13
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
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.uleb128 0x34
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xa
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
	.uleb128 0xb
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
	.uleb128 0xc
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xe
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
	.uleb128 0xf
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x11
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
	.uleb128 0x12
	.uleb128 0x21
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x13
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x14
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
	.uleb128 0x15
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
	.uleb128 0x16
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
	.uleb128 0x17
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
	.uleb128 0x18
	.uleb128 0x15
	.byte	0x1
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x19
	.uleb128 0x26
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x1a
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
	.uleb128 0x1b
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
	.uleb128 0x1c
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
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
	.uleb128 0x1d
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
	.uleb128 0x1e
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1f
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
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
	.uleb128 0x21
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
	.uleb128 0x22
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
	.uleb128 0x23
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
	.uleb128 0x24
	.uleb128 0x34
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
	.uleb128 0x25
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
	.uleb128 0x2116
	.uleb128 0x19
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_pubnames,"",%progbits
	.4byte	0x63
	.2byte	0x2
	.4byte	.Ldebug_info0
	.4byte	0x99f
	.4byte	0x6f1
	.ascii	"nrf_fprintf_fmt\000"
	.4byte	0x7c2
	.ascii	"int_print\000"
	.4byte	0x846
	.ascii	"unsigned_print\000"
	.4byte	0x90a
	.ascii	"string_print\000"
	.4byte	0x972
	.ascii	"buffer_add\000"
	.4byte	0
	.section	.debug_pubtypes,"",%progbits
	.4byte	0x220
	.2byte	0x2
	.4byte	.Ldebug_info0
	.4byte	0x99f
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
	.4byte	0x9e
	.ascii	"__va_list\000"
	.4byte	0x93
	.ascii	"__va_list\000"
	.4byte	0xdc
	.ascii	"long int\000"
	.4byte	0xb7
	.ascii	"__mbstate_s\000"
	.4byte	0x107
	.ascii	"char\000"
	.4byte	0x2cc
	.ascii	"__RAL_locale_data_t\000"
	.4byte	0x3af
	.ascii	"__RAL_locale_codeset_t\000"
	.4byte	0x3f8
	.ascii	"__RAL_locale_t\000"
	.4byte	0x408
	.ascii	"__locale_s\000"
	.4byte	0x568
	.ascii	"__RAL_error_decoder_fn_t\000"
	.4byte	0x589
	.ascii	"__RAL_error_decoder_s\000"
	.4byte	0x5b7
	.ascii	"__RAL_error_decoder_t\000"
	.4byte	0x5d5
	.ascii	"size_t\000"
	.4byte	0x612
	.ascii	"va_list\000"
	.4byte	0x61d
	.ascii	"FILE\000"
	.4byte	0x658
	.ascii	"nrf_fprintf_fwrite\000"
	.4byte	0x6df
	.ascii	"_Bool\000"
	.4byte	0x68a
	.ascii	"nrf_fprintf_ctx\000"
	.4byte	0x6e6
	.ascii	"nrf_fprintf_ctx_t\000"
	.4byte	0
	.section	.debug_aranges,"",%progbits
	.4byte	0x3c
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
	.4byte	.LFB149
	.4byte	.LFE149-.LFB149
	.4byte	.LFB150
	.4byte	.LFE150-.LFB150
	.4byte	.LFB151
	.4byte	.LFE151-.LFB151
	.4byte	0
	.4byte	0
	.section	.debug_ranges,"",%progbits
.Ldebug_ranges0:
	.4byte	.LFB147
	.4byte	.LFE147
	.4byte	.LFB148
	.4byte	.LFE148
	.4byte	.LFB149
	.4byte	.LFE149
	.4byte	.LFB150
	.4byte	.LFE150
	.4byte	.LFB151
	.4byte	.LFE151
	.4byte	0
	.4byte	0
	.section	.debug_line,"",%progbits
.Ldebug_line0:
	.section	.debug_str,"MS",%progbits,1
.LASF116:
	.ascii	"Base\000"
.LASF106:
	.ascii	"p_user_ctx\000"
.LASF113:
	.ascii	"NumDigits\000"
.LASF76:
	.ascii	"__RAL_data_utf8_space\000"
.LASF48:
	.ascii	"date_time_format\000"
.LASF88:
	.ascii	"size_t\000"
.LASF67:
	.ascii	"__RAL_codeset_ascii\000"
.LASF82:
	.ascii	"__RAL_error_decoder_fn_t\000"
.LASF105:
	.ascii	"auto_flush\000"
.LASF29:
	.ascii	"p_cs_precedes\000"
.LASF93:
	.ascii	"va_list\000"
.LASF78:
	.ascii	"__RAL_data_utf8_minus\000"
.LASF114:
	.ascii	"FormatFlags\000"
.LASF124:
	.ascii	"p_str\000"
.LASF10:
	.ascii	"long long unsigned int\000"
.LASF63:
	.ascii	"__locale_s\000"
.LASF81:
	.ascii	"__user_get_time_of_day\000"
.LASF89:
	.ascii	"ITM_RxBuffer\000"
.LASF46:
	.ascii	"date_format\000"
.LASF85:
	.ascii	"next\000"
.LASF44:
	.ascii	"abbrev_month_names\000"
.LASF9:
	.ascii	"long long int\000"
.LASF0:
	.ascii	"signed char\000"
.LASF65:
	.ascii	"__RAL_global_locale\000"
.LASF61:
	.ascii	"codeset\000"
.LASF54:
	.ascii	"__towupper\000"
.LASF15:
	.ascii	"long int\000"
.LASF49:
	.ascii	"__RAL_locale_data_t\000"
.LASF75:
	.ascii	"__RAL_data_utf8_comma\000"
.LASF20:
	.ascii	"int_curr_symbol\000"
.LASF129:
	.ascii	"nrf_fprintf_fmt\000"
.LASF11:
	.ascii	"__va_list\000"
.LASF25:
	.ascii	"positive_sign\000"
.LASF35:
	.ascii	"int_p_cs_precedes\000"
.LASF14:
	.ascii	"__wchar\000"
.LASF18:
	.ascii	"thousands_sep\000"
.LASF24:
	.ascii	"mon_grouping\000"
.LASF53:
	.ascii	"__iswctype\000"
.LASF34:
	.ascii	"n_sign_posn\000"
.LASF127:
	.ascii	"C:\\Users\\paulr\\OneDrive\\Documents\\DARPA\\darpa"
	.ascii	"subt\\darpasubt\\firmware\\dwm1001_dev_source_code\\"
	.ascii	"examples\\ds_twr_resp\\SES\000"
.LASF97:
	.ascii	"stdin\000"
.LASF8:
	.ascii	"unsigned int\000"
.LASF37:
	.ascii	"int_p_sep_by_space\000"
.LASF22:
	.ascii	"mon_decimal_point\000"
.LASF45:
	.ascii	"am_pm_indicator\000"
.LASF74:
	.ascii	"__RAL_data_utf8_period\000"
.LASF19:
	.ascii	"grouping\000"
.LASF112:
	.ascii	"p_args\000"
.LASF55:
	.ascii	"__towlower\000"
.LASF79:
	.ascii	"__RAL_data_empty_string\000"
.LASF95:
	.ascii	"timeval\000"
.LASF64:
	.ascii	"__category\000"
.LASF92:
	.ascii	"__StackLimit\000"
.LASF51:
	.ascii	"__toupper\000"
.LASF32:
	.ascii	"n_sep_by_space\000"
.LASF60:
	.ascii	"data\000"
.LASF26:
	.ascii	"negative_sign\000"
.LASF5:
	.ascii	"short unsigned int\000"
.LASF59:
	.ascii	"name\000"
.LASF102:
	.ascii	"p_io_buffer\000"
.LASF41:
	.ascii	"day_names\000"
.LASF4:
	.ascii	"uint16_t\000"
.LASF36:
	.ascii	"int_n_cs_precedes\000"
.LASF98:
	.ascii	"stdout\000"
.LASF115:
	.ascii	"FieldWidth\000"
.LASF62:
	.ascii	"__RAL_locale_t\000"
.LASF47:
	.ascii	"time_format\000"
.LASF109:
	.ascii	"nrf_fprintf_ctx_t\000"
.LASF33:
	.ascii	"p_sign_posn\000"
.LASF118:
	.ascii	"Number\000"
.LASF69:
	.ascii	"__RAL_ascii_ctype_map\000"
.LASF111:
	.ascii	"p_fmt\000"
.LASF84:
	.ascii	"decode\000"
.LASF57:
	.ascii	"__mbtowc\000"
.LASF42:
	.ascii	"abbrev_day_names\000"
.LASF31:
	.ascii	"n_cs_precedes\000"
.LASF52:
	.ascii	"__tolower\000"
.LASF119:
	.ascii	"int_print\000"
.LASF103:
	.ascii	"io_buffer_size\000"
.LASF80:
	.ascii	"__user_set_time_of_day\000"
.LASF90:
	.ascii	"SystemCoreClock\000"
.LASF13:
	.ascii	"__state\000"
.LASF91:
	.ascii	"__StackTop\000"
.LASF121:
	.ascii	"_aV2C\000"
.LASF108:
	.ascii	"_Bool\000"
.LASF6:
	.ascii	"int32_t\000"
.LASF1:
	.ascii	"unsigned char\000"
.LASF123:
	.ascii	"string_print\000"
.LASF70:
	.ascii	"__RAL_c_locale_day_names\000"
.LASF28:
	.ascii	"frac_digits\000"
.LASF2:
	.ascii	"short int\000"
.LASF87:
	.ascii	"__RAL_error_decoder_head\000"
.LASF94:
	.ascii	"FILE\000"
.LASF101:
	.ascii	"nrf_fprintf_ctx\000"
.LASF23:
	.ascii	"mon_thousands_sep\000"
.LASF125:
	.ascii	"GNU C99 7.3.1 20180622 (release) [ARM/embedded-7-br"
	.ascii	"anch revision 261907] -fmessage-length=0 -mcpu=cort"
	.ascii	"ex-m4 -mlittle-endian -mfloat-abi=hard -mfpu=fpv4-s"
	.ascii	"p-d16 -mthumb -mtp=soft -munaligned-access -std=gnu"
	.ascii	"99 -g2 -gpubnames -fomit-frame-pointer -fno-dwarf2-"
	.ascii	"cfi-asm -fno-builtin -ffunction-sections -fdata-sec"
	.ascii	"tions -fshort-enums -fno-common\000"
.LASF7:
	.ascii	"uint32_t\000"
.LASF120:
	.ascii	"unsigned_print\000"
.LASF39:
	.ascii	"int_p_sign_posn\000"
.LASF21:
	.ascii	"currency_symbol\000"
.LASF16:
	.ascii	"char\000"
.LASF30:
	.ascii	"p_sep_by_space\000"
.LASF104:
	.ascii	"io_buffer_cnt\000"
.LASF66:
	.ascii	"__RAL_c_locale\000"
.LASF58:
	.ascii	"__RAL_locale_codeset_t\000"
.LASF128:
	.ascii	"__ap\000"
.LASF40:
	.ascii	"int_n_sign_posn\000"
.LASF122:
	.ascii	"Value\000"
.LASF99:
	.ascii	"stderr\000"
.LASF100:
	.ascii	"nrf_fprintf_fwrite\000"
.LASF72:
	.ascii	"__RAL_c_locale_month_names\000"
.LASF50:
	.ascii	"__isctype\000"
.LASF126:
	.ascii	"C:\\Users\\paulr\\OneDrive\\Documents\\DARPA\\darpa"
	.ascii	"subt\\darpasubt\\firmware\\dwm1001_dev_source_code\\"
	.ascii	"nRF5_SDK_14.2.0\\external\\fprintf\\nrf_fprintf_for"
	.ascii	"mat.c\000"
.LASF83:
	.ascii	"__RAL_error_decoder_s\000"
.LASF86:
	.ascii	"__RAL_error_decoder_t\000"
.LASF12:
	.ascii	"__mbstate_s\000"
.LASF3:
	.ascii	"uint8_t\000"
.LASF68:
	.ascii	"__RAL_codeset_utf8\000"
.LASF27:
	.ascii	"int_frac_digits\000"
.LASF43:
	.ascii	"month_names\000"
.LASF107:
	.ascii	"fwrite\000"
.LASF38:
	.ascii	"int_n_sep_by_space\000"
.LASF96:
	.ascii	"__RAL_FILE\000"
.LASF117:
	.ascii	"Width\000"
.LASF77:
	.ascii	"__RAL_data_utf8_plus\000"
.LASF73:
	.ascii	"__RAL_c_locale_abbrev_month_names\000"
.LASF56:
	.ascii	"__wctomb\000"
.LASF17:
	.ascii	"decimal_point\000"
.LASF110:
	.ascii	"p_ctx\000"
.LASF71:
	.ascii	"__RAL_c_locale_abbrev_day_names\000"
.LASF130:
	.ascii	"buffer_add\000"
	.ident	"GCC: (GNU) 7.3.1 20180622 (release) [ARM/embedded-7-branch revision 261907]"
