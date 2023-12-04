	.arch msp430g2553
	.p2align 1,0

	.data
led_init:
	.word 1
LEDS:
	.word 1
blinkState:
	.word 1
sillyBlink:
	.word 1
waveState:
	.word 1
wavePeriod:
	.word 1

	.text
	.global led_init, LEDS, blinkState, sillyBlink, waveState, wavePeriod
	.extern P1OUT

move_blink:
	sub #2, r1		; 1 variable
	and LEDS, P1OUT 	; P1OUT &= LEDS
	mov r12, r1		; r1 = r12
	add r1, blinkState 	; blinkState += r1
	mov #0, sillyBlink	; sillyBlink = 0
	mov #0, waveState	; waveState = 0
	mov #64, wavePeriod	; wavePeriod = 64
	cmp #4, blinkState	; if 4 <= blink
	jnc go_back		; goto go_back
	cmp blinkState, #0	; if 0 > blinkState
	jc go_forward		; goto go_forward

go_back:
	move #0, blinkState	; blinkState = 0
	pop r0
go_forward:
	mov #3, blinkState 	; blinkState = 3
	pop r0
