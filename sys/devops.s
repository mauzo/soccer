	.file	"devops.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
	.section	.text.open,"ax",@progbits
.global	open
	.type	open, @function
open:
	push r15
	push r16
	push r17
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 5 */
.L__stack_usage = 5
	cpi r24,lo8(3)
	brsh .L11
	mov r18,r24
	ldi r19,0
	ldi r25,lo8(6)
	mul r24,r25
	movw r30,r0
	clr __zero_reg__
	movw r26,r30
	subi r26,lo8(-(Devices))
	sbci r27,hi8(-(Devices))
	movw r20,r26
	subi r20,-2
	sbci r21,-1
	movw r30,r20
	lpm r28,Z+
	lpm r29,Z+
	sbiw r28,0
	brne .L28
.L15:
	ldi r24,lo8(-3)
	ldi r25,lo8(-1)
	rjmp .L1
.L11:
	ldi r24,lo8(-2)
	ldi r25,lo8(-1)
	rjmp .L1
.L14:
	ldi r24,lo8(-7)
	ldi r25,lo8(-1)
	rjmp .L1
.L28:
	movw r30,r26
	lpm r20,Z+
	lpm r21,Z+
	movw r30,r20
	lpm r20,Z+
	lpm r21,Z+
	or r20,r21
	breq .L15
	mov r31,r22
	andi r31,lo8(1)
	mov r15,r31
	sbrs r22,0
	rjmp .L4
	ld r25,Y
	andi r25,lo8(31)
	brne .L14
.L4:
	mov r16,r22
	andi r16,lo8(2)
	sbrs r22,1
	rjmp .L5
	ldd r25,Y+1
	andi r25,lo8(31)
	brne .L14
.L5:
	in r17,__SREG__
/* #APP */
 ;  67 "/home/mauzo/src/soccer/sys/devops.c" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	ldi r20,lo8(6)
	mul r24,r20
	movw r24,r0
	clr __zero_reg__
	mul r20,r18
	movw r30,r0
	mul r20,r19
	add r31,r0
	clr __zero_reg__
	subi r30,lo8(-(Devices))
	sbci r31,hi8(-(Devices))
	lpm r0,Z+
	lpm r31,Z
	mov r30,r0
	lpm r0,Z+
	lpm r31,Z
	mov r30,r0
	subi r24,lo8(-(Devices))
	sbci r25,hi8(-(Devices))
	icall
	sbiw r24,0
	brne .L7
	tst r15
	breq .L8
	lds r19,Currtask
	andi r19,lo8(31)
	ld r18,Y
	andi r18,lo8(-32)
	or r18,r19
	st Y,r18
.L8:
	tst r16
	breq .L7
	lds r19,Currtask
	andi r19,lo8(31)
	ldd r18,Y+1
	andi r18,lo8(-32)
	or r18,r19
	std Y+1,r18
.L7:
	out __SREG__,r17
.L1:
/* epilogue start */
	pop r29
	pop r28
	pop r17
	pop r16
	pop r15
	ret
	.size	open, .-open
	.section	.text.poll,"ax",@progbits
.global	poll
	.type	poll, @function
poll:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	cpi r24,lo8(3)
	brsh .L30
	ldi r25,lo8(6)
	mul r24,r25
	movw r30,r0
	clr __zero_reg__
	subi r30,lo8(-(Devices+2))
	sbci r31,hi8(-(Devices+2))
	lpm r0,Z+
	lpm r31,Z
	mov r30,r0
	sbiw r30,0
	breq .L30
	cpi r22,lo8(1)
	breq .L34
	cpi r22,lo8(2)
	breq .L35
	ret
.L36:
/* #APP */
 ;  92 "/home/mauzo/src/soccer/sys/devops.c" 1
	nop
 ;  0 "" 2
/* #NOAPP */
.L35:
	ldd r24,Z+1
	sbrc r24,5
	rjmp .L36
	ret
.L37:
/* #APP */
 ;  97 "/home/mauzo/src/soccer/sys/devops.c" 1
	nop
 ;  0 "" 2
/* #NOAPP */
.L34:
	ld r24,Z
	sbrc r24,6
	rjmp .L37
.L30:
/* epilogue start */
	ret
	.size	poll, .-poll
	.section	.text.read_queue,"ax",@progbits
.global	read_queue
	.type	read_queue, @function
read_queue:
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	push r16
	push r17
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 10 */
.L__stack_usage = 10
	mov r15,r24
	mov r14,r22
	mov r13,r23
	mov r12,r20
	mov r11,r21
	ldi r24,lo8(2)
	cp r24,r15
	brlo .L53
	mov r28,r15
	ldi r29,0
	ldi r26,lo8(6)
	mul r15,r26
	movw r30,r0
	clr __zero_reg__
	movw r20,r30
	subi r20,lo8(-(Devices))
	sbci r21,hi8(-(Devices))
	movw r24,r20
	adiw r24,2
	movw r30,r24
	lpm r16,Z+
	lpm r17,Z+
	cp r16,__zero_reg__
	cpc r17,__zero_reg__
	brne .L61
.L57:
	ldi r24,lo8(-3)
	rjmp .L64
.L49:
	adiw r30,2
	ori r18,lo8(1)
	rjmp .L63
.L56:
	ldi r24,lo8(-6)
	ldi r25,lo8(-1)
	rjmp .L50
.L53:
	ldi r24,lo8(-2)
.L64:
	ldi r25,lo8(-1)
	rjmp .L45
.L55:
	ldi r24,lo8(-1)
	rjmp .L64
.L61:
	movw r30,r20
	lpm r24,Z+
	lpm r25,Z+
	movw r30,r24
	adiw r30,2
	lpm r24,Z+
	lpm r25,Z+
	or r24,r25
	breq .L57
	movw r26,r16
	ld r19,X
	andi r19,lo8(31)
	lds r24,Currtask
	ldi r25,0
	cp r19,r24
	cpc __zero_reg__,r25
	brne .L55
	sbrs r18,0
	rjmp .L48
	ldi r22,lo8(1)
	mov r24,r15
	call poll
.L48:
	in r10,__SREG__
/* #APP */
 ;  147 "/home/mauzo/src/soccer/sys/devops.c" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	movw r30,r16
	ld r25,Z
	mov r18,r25
	swap r18
	lsr r18
	andi r18,lo8(7)
	sbrs r25,5
	rjmp .L49
	sbrc r18,1
	rjmp .L56
	adiw r30,6
	ori r18,lo8(2)
.L63:
	swap r18
	lsl r18
	andi r18,lo8(-32)
	andi r25,lo8(31)
	or r25,r18
	movw r26,r16
	st X,r25
	std Z+2,r12
	std Z+3,r11
	st Z,r14
	std Z+1,r13
	ldi r25,lo8(6)
	mul r15,r25
	movw r18,r0
	clr __zero_reg__
	mul r25,r28
	movw r30,r0
	mul r25,r29
	add r31,r0
	clr __zero_reg__
	subi r30,lo8(-(Devices))
	sbci r31,hi8(-(Devices))
	lpm r0,Z+
	lpm r31,Z
	mov r30,r0
	adiw r30,2
	lpm r0,Z+
	lpm r31,Z
	mov r30,r0
	movw r24,r18
	subi r24,lo8(-(Devices))
	sbci r25,hi8(-(Devices))
	icall
.L50:
	out __SREG__,r10
.L45:
/* epilogue start */
	pop r29
	pop r28
	pop r17
	pop r16
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	ret
	.size	read_queue, .-read_queue
	.section	.text.read_poll,"ax",@progbits
.global	read_poll
	.type	read_poll, @function
read_poll:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	movw r18,r22
	cpi r24,lo8(3)
	brsh .L69
	ldi r25,lo8(6)
	mul r24,r25
	movw r30,r0
	clr __zero_reg__
	subi r30,lo8(-(Devices+2))
	sbci r31,hi8(-(Devices+2))
	lpm r0,Z+
	lpm r31,Z
	mov r30,r0
	sbiw r30,0
	breq .L70
	ld r20,Z
	andi r20,lo8(31)
	lds r24,Currtask
	ldi r25,0
	cp r20,r24
	cpc __zero_reg__,r25
	brne .L71
	in r22,__SREG__
/* #APP */
 ;  167 "/home/mauzo/src/soccer/sys/devops.c" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	ldd r20,Z+2
	ldd r21,Z+3
	cp r18,r20
	cpc r19,r21
	brlo .L67
	ldd r24,Z+4
	ldd r25,Z+5
	add r24,r20
	adc r25,r21
	cp r18,r24
	cpc r19,r25
	brlo .L72
.L67:
	ldd r24,Z+6
	ldd r25,Z+7
	cp r18,r24
	cpc r19,r25
	brlo .L74
	ldd __tmp_reg__,Z+8
	ldd r31,Z+9
	mov r30,__tmp_reg__
	add r24,r30
	adc r25,r31
	cp r18,r24
	cpc r19,r25
	brsh .L74
.L72:
	ldi r24,lo8(-6)
	ldi r25,lo8(-1)
.L68:
	out __SREG__,r22
	sbiw r24,0
	brne .L65
	cp r18,r20
	cpc r19,r21
	brsh .L65
	movw r24,r20
	sub r24,r18
	sbc r25,r19
	ret
.L74:
	ldi r25,0
	ldi r24,0
	rjmp .L68
.L69:
	ldi r24,lo8(-2)
	ldi r25,lo8(-1)
	ret
.L70:
	ldi r24,lo8(-3)
	ldi r25,lo8(-1)
	ret
.L71:
	ldi r24,lo8(-1)
	ldi r25,lo8(-1)
.L65:
/* epilogue start */
	ret
	.size	read_poll, .-read_poll
	.section	.text.read_setlen,"ax",@progbits
.global	read_setlen
	.type	read_setlen, @function
read_setlen:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	movw r18,r22
	movw r22,r20
	cpi r24,lo8(3)
	brsh .L79
	ldi r25,lo8(6)
	mul r24,r25
	movw r30,r0
	clr __zero_reg__
	subi r30,lo8(-(Devices+2))
	sbci r31,hi8(-(Devices+2))
	lpm r0,Z+
	lpm r31,Z
	mov r30,r0
	sbiw r30,0
	breq .L80
	in r20,__SREG__
/* #APP */
 ;  192 "/home/mauzo/src/soccer/sys/devops.c" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	ldd r24,Z+6
	ldd r25,Z+7
	cp r24,r18
	cpc r25,r19
	brne .L77
	std Z+9,r23
	std Z+8,r22
.L83:
	ldi r25,0
	ldi r24,0
.L78:
	out __SREG__,r20
	ret
.L77:
	ldd r24,Z+2
	ldd r25,Z+3
	cp r24,r18
	cpc r25,r19
	brlo .L82
	add r18,r22
	adc r19,r23
	cp r24,r18
	cpc r25,r19
	brsh .L82
	sub r18,r24
	sbc r19,r25
	std Z+5,r19
	std Z+4,r18
	rjmp .L83
.L82:
	ldi r24,lo8(-4)
	ldi r25,lo8(-1)
	rjmp .L78
.L79:
	ldi r24,lo8(-2)
	ldi r25,lo8(-1)
	ret
.L80:
	ldi r24,lo8(-3)
	ldi r25,lo8(-1)
/* epilogue start */
	ret
	.size	read_setlen, .-read_setlen
	.section	.text.write_queue,"ax",@progbits
.global	write_queue
	.type	write_queue, @function
write_queue:
	push r7
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	push r16
	push r17
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 13 */
.L__stack_usage = 13
	mov r12,r22
	mov r11,r23
	mov r10,r20
	mov r9,r21
	cpi r24,lo8(3)
	brlo .+2
	rjmp .L94
	mov r20,r24
	ldi r21,0
	ldi r25,lo8(6)
	mul r24,r25
	movw r30,r0
	clr __zero_reg__
	movw r26,r30
	subi r26,lo8(-(Devices))
	sbci r27,hi8(-(Devices))
	movw r22,r26
	subi r22,-2
	sbci r23,-1
	movw r30,r22
	lpm r28,Z+
	lpm r29,Z+
	sbiw r28,0
	brne .L99
.L97:
	ldi r24,lo8(-3)
	ldi r25,lo8(-1)
	rjmp .L84
.L88:
	std Y+10,r12
	std Y+11,r11
	std Y+12,r10
	std Y+13,r9
	andi r24,lo8(31)
	tst r7
	breq .L90
	ori r25,lo8(2)
.L101:
	swap r25
	lsl r25
	andi r25,lo8(-32)
	or r24,r25
	std Y+1,r24
	ldd r24,Y+1
	ori r24,lo8(32)
	std Y+1,r24
	movw r30,r14
	lpm r24,Z+
	lpm r25,Z+
	movw r30,r24
	adiw r30,4
	lpm r0,Z+
	lpm r31,Z
	mov r30,r0
	movw r24,r16
	icall
	out __SREG__,r8
	cpi r24,-6
	ldi r31,-1
	cpc r25,r31
	brne .+2
	rjmp .L89
.L84:
/* epilogue start */
	pop r29
	pop r28
	pop r17
	pop r16
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop r7
	ret
.L90:
	andi r25,lo8(5)
	rjmp .L101
.L94:
	ldi r24,lo8(-2)
	ldi r25,lo8(-1)
	rjmp .L84
.L96:
	ldi r24,lo8(-1)
	ldi r25,lo8(-1)
	rjmp .L84
.L99:
	movw r30,r26
	lpm r22,Z+
	lpm r23,Z+
	movw r30,r22
	adiw r30,4
	lpm r22,Z+
	lpm r23,Z+
	or r22,r23
	brne .+2
	rjmp .L97
	ldd r25,Y+1
	andi r25,lo8(31)
	lds r22,Currtask
	ldi r23,0
	cp r25,r22
	cpc __zero_reg__,r23
	breq .L87
	sbrs r18,3
	rjmp .L96
.L87:
	ldi r25,lo8(6)
	mul r24,r25
	movw r16,r0
	clr __zero_reg__
	subi r16,lo8(-(Devices))
	sbci r17,hi8(-(Devices))
	mov r31,r18
	andi r31,lo8(4)
	mov r7,r31
	mul r25,r20
	movw r22,r0
	mul r25,r21
	add r23,r0
	clr __zero_reg__
	movw r24,r22
	subi r24,lo8(-(Devices))
	sbci r25,hi8(-(Devices))
	movw r14,r24
	andi r18,lo8(1)
	mov r13,r18
.L92:
	in r8,__SREG__
/* #APP */
 ;  241 "/home/mauzo/src/soccer/sys/devops.c" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	ldd r24,Y+1
	mov r25,r24
	swap r25
	lsr r25
	andi r25,lo8(7)
	sbrs r24,5
	rjmp .L88
	out __SREG__,r8
.L89:
	cpse r13,__zero_reg__
	rjmp .L92
	ldi r24,lo8(-6)
	ldi r25,lo8(-1)
	rjmp .L84
	.size	write_queue, .-write_queue
	.section	.text.write_poll,"ax",@progbits
.global	write_poll
	.type	write_poll, @function
write_poll:
	push r10
	push r11
	push r13
	push r14
	push r15
	push r16
	push r17
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 9 */
.L__stack_usage = 9
	movw r14,r22
	ldi r28,lo8(-2)
	ldi r29,lo8(-1)
	cpi r24,lo8(3)
	brlo .+2
	rjmp .L102
	ldi r25,lo8(6)
	mul r24,r25
	movw r30,r0
	clr __zero_reg__
	subi r30,lo8(-(Devices+2))
	sbci r31,hi8(-(Devices+2))
	lpm r10,Z+
	lpm r11,Z+
	ldi r28,lo8(-3)
	ldi r29,lo8(-1)
	cp r10,__zero_reg__
	cpc r11,__zero_reg__
	breq .L102
	movw r30,r10
	ldd r21,Z+1
	andi r21,lo8(31)
	lds r24,Currtask
	ldi r25,0
	cp r21,r24
	cpc __zero_reg__,r25
	breq .L110
	ldi r28,lo8(-1)
	ldi r29,lo8(-1)
	sbrs r20,3
	rjmp .L102
.L110:
	ldi r29,0
	ldi r28,0
	andi r20,lo8(1)
	mov r13,r20
.L107:
	in r20,__SREG__
/* #APP */
 ;  265 "/home/mauzo/src/soccer/sys/devops.c" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	movw r30,r10
	ldd r16,Z+10
	ldd r17,Z+11
	cp r14,r16
	cpc r15,r17
	brlo .L105
	ldd r24,Z+12
	ldd r25,Z+13
	add r24,r16
	adc r25,r17
	cp r14,r24
	cpc r15,r25
	brsh .L105
	mov r16,r19
	mov r17,r18
	ldi r28,lo8(-6)
	ldi r29,lo8(-1)
.L105:
	out __SREG__,r20
	cpi r28,-6
	ldi r31,-1
	cpc r29,r31
	brne .L106
	tst r13
	breq .L102
	call cons_flash
	mov r19,r16
	mov r18,r17
	rjmp .L107
.L106:
	sbiw r28,0
	brne .L102
	cp r14,r16
	cpc r15,r17
	brsh .L102
	movw r28,r16
	sub r28,r14
	sbc r29,r15
.L102:
	movw r24,r28
/* epilogue start */
	pop r29
	pop r28
	pop r17
	pop r16
	pop r15
	pop r14
	pop r13
	pop r11
	pop r10
	ret
	.size	write_poll, .-write_poll
	.ident	"GCC: (GNU) 9.1.0"
