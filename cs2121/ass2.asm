;*************************************************************************************************
; The following wirings are to be made
; PB4 <-> Mot
; R0 - R3 <-> PA0 - PA3
; C0 - C3 <-> PA4 - PA7
; PC0 - PC3 <-> D0 - D7
; PC4 - PC7 <-> D4 - D7
; BE - RS <-> PE5 - PE2
; PB0 <-> PD0
; PB1 <-> PD1
; LED0 - LED3 <-> PD4 - PD7
; AIn <-> PB4
; ASD <-> Speaker (PIN 1)
;*************************************************************************************************

.include "m64def.inc"
.def open = r9			; 1 if open button is held down, 0 if not
.def emergency = r10	; 1 if emergency switch pressed
.def endpointer = r11	; "end of array" during bubble sort
.def pointer = r12		; counter of the progression through the queue
.def num = r13			; number pressed to add to queue
.def seconds = r14		; number of seconds passed during phase
.def size = r15			; size of queue
.def temp = r16
.def temp2 = r17
.def row = r18			
.def col = r19
.def mask = r20
.def dutyCycle = r21	; dutyCycle of the PWM motor control
.def data = r22			; stores the char to print to the LCD
.def floor = r23		; the current floor
.def del_lo = r24
.def del_hi = r25
.def phase = r26 		; stores the phases of the elevator
.def direction = r27 	; stores direction elevator is travelling in

.equ PORTDDIR = 0xF0		; set PD4 - PD7 as output
.equ PORTADIR = 0xF0		; set rows to input, cols to output
.equ INITCOLMASK = 0xEF
.equ INITROWMASK = 0x01
.equ ROWMASK = 0x0F

.equ up = 1
.equ down = -1

.equ stationary = 0b00010000
.equ closing = 0b00100000
.equ moving = 0b01000000
.equ opening = 0b10000000

;LCD protocol control bits
.equ LCD_RS = 3+4
.equ LCD_RW = 1+4
.equ LCD_E = 2+4
;LCD functions
.equ LCD_FUNC_SET = 0b00110000
.equ LCD_DISP_OFF = 0b00001000
.equ LCD_DISP_CLR = 0b00000001
.equ LCD_DISP_ON = 0b00001100
.equ LCD_ENTRY_SET = 0b00000100
.equ LCD_ADDR_SET = 0b10000000
;LCD function bits and constants
.equ LCD_BF = 7
.equ LCD_N = 3
.equ LCD_F = 2
.equ LCD_ID = 1
.equ LCD_S = 0
.equ LCD_C = 1
.equ LCD_B = 0
.equ LCD_LINE1 = 0
.equ LCD_LINE2 = 0x40

.MACRO Clear							; the macro clears a word (2 bytes) in a memory
ldi r28, low(@0) 						; the parameter @0 is the memory address for that word
ldi r29, high(@0) 						; load the memory address to Y 
clr temp	
st y+, temp 							; clear the two bytes at @0 in SRAM
st y, temp 
.ENDMACRO

.MACRO ClearQueue						; removes all elements of a queue in parameter 0
ldi r28, low(@0)
ldi r29, high(@0)
ldi temp, 10
clr temp2
clr size
clearloop:
	st y+, temp
	st y+, temp
	inc temp2
	cpi temp2, 9
	brne clearloop
.ENDMACRO

;setting up the interrupt vector
jmp RESET
jmp EXT_INT0 ; IRQ0 Handler
jmp EXT_INT1 ; IRQ1 Handler
jmp Default ; IRQ2 Handler
jmp Default ; IRQ3 Handler
jmp Default ; IRQ4 Handler
jmp Default ; IRQ5 Handler
jmp Default ; IRQ6 Handler
jmp Default ; IRQ7 Handler
jmp Default ; Timer2 Compare Handler
jmp Timer2OVF ; Timer2 Overflow Handler
jmp Default ; Timer1 Capture Handler
jmp Default ; Timer1 CompareA Handler
jmp Default ; Timer1 CompareB Handler
jmp Default ; Timer1 Overflow Handler
jmp Default ; Timer0 Compare Handler
jmp Default  ; Timer0 Overflow Handler

.org 0x100
.dseg
TempCounter:.byte 2 					; temporary counter. Used to determine if one second has passed
Queue: .byte 10							; queue will be of maximum length 9, one extra byte for a buffer
.cseg

Default: reti

RESET: ldi temp, high(RAMEND) ; Initialize stack pointer
	out SPH, temp
	ldi temp, low(RAMEND)
	out SPL, temp
	ldi temp, PORTADIR
	out DDRA, temp						; set rows to input, cols to output for keypad
	ldi temp, PORTDDIR
	out DDRD, temp						; set PD4 - PD7 as output for LEDs
	ldi temp, 2 << ISC10 | 2 << ISC00 	;setting the interrupts for falling edge
	sts EICRA, temp                       ;storing them into EICRA 
	in temp, EIMSK                        ;taking the values inside the EIMSK  
	ori temp, 1<<INT0 | 1 << INT1       ; oring the values with INT0 and INT1  
	out EIMSK, temp                       ; enabling interrput0 and interrupt1

	ldi temp, 0b00000010
	out TCCR2, temp 						; prescaling value=8
	ldi temp, 1<<TOIE2 						; =278 microseconds 
	out TIMSK, temp 						; T/C0 interrupt enable 
	sei 		

	ldi temp, 0b00010000
	out DDRB, temp 				; Bit 4 will function as OC0.
	ldi dutyCycle, 0 			; the value controls the PWM duty cycle
	out OCR0, dutyCycle
	ldi temp, (1<< WGM20)|(1<<COM21)|(1<<CS20) ; Set the Timer0 to Phase Correct PWM mode. 
	out TCCR0, temp
	clr floor						; clear out the registers				
	ldi phase, stationary			; elevator starts off stationary
	out PORTD, phase				
	ldi direction, up				; initial direction is up
	clr size						; size of queue starts off as 0
	clr num							; no keypad input
	clr seconds						; no seconds in this phase have passed
	clr emergency					; not in a state of emergency
	clr open						; open button is not pressed
	ClearQueue Queue				; clear the queue
	rcall printqueue				; print the current status of the elevator to the LCD
	jmp main

EXT_INT0:							; close button is pressed
	push temp						; push conflict registers
	in temp, SREG
	push temp
	cpi phase, closing				; close button only works when the lift is stationary
	breq End_INT0
	cpi phase, moving
	breq End_INT0
	clr seconds
	cpi phase, opening
	breq End_INT0
	ldi phase, closing				; change the phase to closing
	ldi temp, 144
	out OCR0, temp
	End_INT0:
		pop temp
		out SREG, temp
		pop temp
		reti
 
EXT_INT1:							; emergency button pressed
	push temp						; push conflict registers
	in temp, SREG
	push temp
	ldi temp, 1						; if emergency button already pressed
	cp temp, emergency				; do nothing
	breq end_INT1	
	sei								; re-enable global interrupts since TIMER2OVF
									; is required		
	ldi temp, 1						
	mov emergency, temp
	rcall printemergency			; print emergency message
	ClearQueue Queue				; clear out the queue and add 0 to the queue
	clr temp
	mov num, temp
	rcall addqueue
	ldi phase, closing				; close the doors when the emergency is signalled
	ldi temp, 144
	out OCR0, temp
	emergencyloop:
		rcall printemergency		; wait for the elevator to reach the ground floor
		cpi floor, 0
		brne emergencyloop
		cpi phase, stationary		; the door needs to open, and then when it is stationary
		breq emergencyclose			; again for 3 seconds, close the doors
		jmp emergencyloop
	emergencyclose:
		ldi phase, closing			; close the doors again
		ldi temp, 144
		out OCR0, temp
	emergencyloop2:					; infinite loop
		rcall printemergency
		rjmp emergencyloop2
	end_INT1:
		pop temp
		out SREG, temp
		pop temp
		reti

Timer2OVF: 									; interrupt subroutine to Timer0
	push temp
	in temp, SREG							; store the status register in temp
	push temp 								; prologue starts.
	push r31
	push r30
	push r29								; save all conflict registers in the prologue. 
	push r28 
	push r25
	push r24 								; prologue ends.
	ldi r28, low(TempCounter) 				; load the address of the temporary
	ldi r29, high(TempCounter) 				; counter.
	ld r24, y+ 								; load the value of the temporary counter.
	ld r25, y
	adiw r24, 1								; increase the temporary counter by one.

	cpi r24, low(3597) 						; check if (r25:r24)=899 
	ldi temp, high(3597) 					; 3597= 106/278 ( times a second)
	cpc r25, temp
	brne NotSecond	
	Clear TempCounter
	cpi phase, stationary					; check the phase of the elevator
	breq doStationary						; respond appropriately
	cpi phase, closing
	breq changeMoving
	cpi phase, moving
	breq doMoving
	cpi phase, opening
	breq changeStationary

	NotSecond:								; a second has not passed
		st y, r25 							; store the value of the temporary counter.
		st -y, r24
		ldi temp, 1
		cp emergency, temp					; if the emergency lights are on, flicker the lights
		breq emergencylights
		jmp outphase
	emergencylights:
		mov temp, r24						; flicker the lights based on the temp counter
		out PORTD, temp
		jmp EndIF
	doStationary:
		ldi temp, 0
		cp size, temp						; empty queue, stay stationary
		breq EndIF
		ldi temp, 1							; check if the open button is still being pressed
		cp open, temp
		breq EndIF
		inc seconds							; increment the seconds in this phase
		ldi temp, 3
		cp seconds, temp					; when 3 seconds have passed, move to next phase
		breq changeClosing
		jmp outphase
	changeClosing:							; change between stationary to closing
		clr seconds							; reset the seconds
		ldi phase, closing
		ldi temp, 144						; motor spins at 40 rps while moving
		out OCR0, temp
		jmp outphase
	changeMoving:							; change between closing and moving
		ldi temp, 0
		cp size, temp						; empty queue, stay stationary but keep doors closed
		breq changeStationary
		clr seconds
		ldi phase, moving					; motor spins at 20 rps while moving
		ldi temp, 96
		out OCR0, temp
		jmp outphase
	doMoving:								; in transit between floors
		inc seconds							; increment the seconds for this phase
		ldi temp, 2							; after 2 seconds, change floors
		cp seconds, temp
		breq changeFloors
		jmp outphase
	changeFloors:
		clr seconds							; reset the seconds for this phase
		add floor, direction				; change the floor based on the direction
		rcall printqueue					; update the LCD
		ldi r30, low(Queue) 				; load the address of the queue
		ldi r31, high(Queue) 				
		ld temp, z							; check if we've reached the floor
		cp floor, temp
		breq changeOpening
		jmp outphase
	changeOpening:		
		ldi phase, opening					; change between moving and opening
		ldi temp, 216						; motor spins at 70 rps while opening
		out OCR0, temp
		jmp outphase
	changeStationary:						; change between opening and stationary
		clr seconds							; reset the seconds for this 
		ldi phase, stationary
		clr temp
		out OCR0, temp						; turn off the motor
		ldi r30, low(Queue) 				; load the address of the queue
		ldi r31, high(Queue) 				
		ld temp, z							; if the lift is stationary and reached
		cp floor, temp						; the next floor, remove the floor number
		breq arrived						; from the queue
		jmp outphase
	arrived:
		rcall popqueue						; remove first element from queue
	outphase:
		out PORTD, phase					; out the phase of the elevator to the LEDs
	EndIF:
		pop r24 							; epilogue starts; 
		pop r25								; restore all conflict registers from the stack. 
		pop r28
		pop r29
		pop r30
		pop r31
		pop temp
		out SREG, temp
		pop temp
		reti

;*********************************************************************************************
;	lcd function
;*********************************************************************************************
	
;Function lcd_write_com: Write a command to the LCD. The data reg stores the value to be written.

lcd_write_com:
	out PORTC, data ; set the data port's value up
	clr temp
	out PORTE, temp ; RS = 0, RW = 0 for a command write
	nop ; delay to meet timing (Set up time)
	sbi PORTE, LCD_E ; turn on the enable pin
	nop ; delay to meet timing (Enable pulse width)
	nop
	nop
	cbi PORTE, LCD_E ; turn off the enable pin
	nop ; delay to meet timing (Enable cycle time)
	nop
	nop
	ret
;Function lcd_write_data: Write a character to the LCD. The data reg stores the value to be written.
lcd_write_data:
	out PORTC, data ; set the data port's value up
	ldi temp, 1 << LCD_RS
	out PORTE, temp ; RS = 1, RW = 0 for a data write
	nop ; delay to meet timing (Set up time)
	sbi PORTE, LCD_E ; turn on the enable pin
	nop ; delay to meet timing (Enable pulse width)
	nop
	nop
	cbi PORTE, LCD_E ; turn off the enable pin
	nop ; delay to meet timing (Enable cycle time)
	nop
	nop
	ret
;Function lcd_wait_busy: Read the LCD busy flag until it reads as not busy.
lcd_wait_busy:
	clr temp
	out DDRC, temp ; Make PORTC be an input port for now
	out PORTC, temp
	ldi temp, 1 << LCD_RW
	out PORTE, temp ; RS = 0, RW = 1 for a command port read
busy_loop:
	nop ; delay to meet timing (Set up time / Enable cycle time)
	sbi PORTE, LCD_E ; turn on the enable pin
	nop ; delay to meet timing (Data delay time)
	nop
	nop
	in temp, PINC ; read value from LCD
	cbi PORTE, LCD_E ; turn off the enable pin
	sbrc temp, LCD_BF ; if the busy flag is set
	rjmp busy_loop ; repeat command read
	clr temp ; else
	out PORTE, temp ; turn off read mode,
	ser temp
	out DDRC, temp ; make PORTC an output port again
	ret ; and return
	; Function delay: Pass a number in registers r18:r19 to indicate how many microseconds
	; must be delayed. Actual delay will be slightly greater (~1.08us*r18:r19).
	; r18:r19 are altered in this function.
	; Code is omitted
	;Function lcd_init Initialisation function for LCD.
lcd_init:
	ser temp
	out DDRC, temp ; PORTC, the data port is usually all otuputs
	out DDRE, temp ; PORTE, the control port is always all outputs
	ldi del_lo, low(15000)
	ldi del_hi, high(15000)
	rcall delay ; delay for > 15ms
	; Function set command with N = 1 and F = 0
	ldi data, LCD_FUNC_SET | (1 << LCD_N)
	rcall lcd_write_com ; 1st Function set command with 2 lines and 5*7 font
	ldi del_lo, low(4100)
	ldi del_hi, high(4100)
	rcall delay ; delay for > 4.1ms
	rcall lcd_write_com ; 2nd Function set command with 2 lines and 5*7 font
	ldi del_lo, low(100)
	ldi del_hi, high(100)
	rcall delay ; delay for > 100us
	rcall lcd_write_com ; 3rd Function set command with 2 lines and 5*7 font
	rcall lcd_write_com ; Final Function set command with 2 lines and 5*7 font
	rcall lcd_wait_busy ; Wait until the LCD is ready
	ldi data, LCD_DISP_OFF
	rcall lcd_write_com ; Turn Display off
	rcall lcd_wait_busy ; Wait until the LCD is ready
	ldi data, LCD_DISP_CLR
	rcall lcd_write_com ; Clear Display
	rcall lcd_wait_busy ; Wait until the LCD is ready
	; Entry set command with I/D = 1 and S = 0
	ldi data, LCD_ENTRY_SET | (1 << LCD_ID)
	rcall lcd_write_com ; Set Entry mode: Increment = yes and Shift = no
	rcall lcd_wait_busy ; Wait until the LCD is ready
	; Display on command with C = 0 and B = 1
	ldi data, LCD_DISP_ON | (1 << LCD_C)
	rcall lcd_write_com ; Trun Display on with a cursor that doesn't blink
	ret

delay:
	dloop:
		subi del_lo, 1
		sbci del_hi, 0
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
	brne dloop
	ret
 	 	
;*********************************************************************************************
;	main function
;*********************************************************************************************
main:
	ldi mask, INITCOLMASK ; initial column mask
	clr col ; initial column
colloop:
	out PORTA, mask ; set column to mask value
	; (sets column 0 off)
	ldi temp, 0xFF ; implement a delay so the
	; hardware can stabilize
delayKey:
	dec temp
	brne delayKey
	in temp, PINA ; read PORTA
	andi temp, ROWMASK ; read only the row bits
	cpi temp, 0xF ; check if any rows are grounded
	breq nextcol ; if not go to the next column
	ldi mask, INITROWMASK ; initialise row check
	clr row ; initial row
rowloop:
	mov temp2, temp
	and temp2, mask ; check masked bit
	brne skipconv 	; if the result is non-zero,
					; we need to look again
	rcall convert 	; if bit is clear, convert the bitcode
	jmp main 		; and start again
skipconv:
	inc row ; else move to the next row
	lsl mask ; shift the mask to the next bit
	jmp rowloop
nextcol:
	cpi col, 3	; check if we’re on the last column
	breq main 	; if so, no buttons were pushed,
				; so start again.

	sec 		; else shift the column mask:

				; We must set the carry bit
	rol mask	; and then rotate left by a bit,
				; shifting the carry into
				; bit zero. We need this to make
				; sure all the rows have
				; pull-up resistors
	inc col 	; increment column value
	jmp colloop ; and check the next column
				; convert function converts the row and column given to a
				; binary number and also outputs the value to PORTC.
				; Inputs come from registers row and col and output is in
				; temp.
convert:
	cpi col, 3 ; if column is 3 we have a letter
	breq letters
	cpi row, 3 ; if row is 3 we have a symbol or 0
	breq symbols
	mov temp, row ; otherwise we have a number (1-9)
	lsl temp ; temp = row * 2
	add temp, row ; temp = row * 3
	add temp, col ; add the column address
	; to get the offset from 1
	inc temp ; add 1. Value of switch is
	; row*3 + col + 1.
	mov num, temp
	rcall addqueue
letters:
	jmp convert_end
symbols:
	cpi col, 0 ; check if we have a star
	breq star
	cpi col, 1 ; or if we have zero
	breq zero
	; # is the open button
	cpi phase, moving
	breq convert_end
	cpi phase, stationary
	breq convert_end
	ldi phase, opening
	ldi temp, 216
	out OCR0, temp
	inc open
	jmp convert_end
star:
	jmp convert_end

zero:
	clr num
	rcall addqueue	
	jmp convert_end
convert_end:
	loop:
		in temp, PINA 		; read PORTA
		andi temp, ROWMASK	; loop until PORTA no longer takes input
		cpi temp, 0x0F
		brne loop
	clr open
	rcall printqueue
	ret ; return to caller

addqueue:					; add a number to the queue
	push temp				; push conflict registers
	in temp, SREG
	push temp
	push r29
	push r28
	ldi r28, low(Queue)
	ldi r29, high(Queue)
	ldi temp, 9 			; queue can't have more than 9 elements
	cp size, temp		
	breq endadd
	cp floor, num			; you can't add the floor you're on now to the queue
	breq endadd
	clr pointer		
	cp pointer, size 		; we have an empty queue
	breq addNum

	cpi direction, up		; if you're going up, num > floor
	breq checkGreater
	cp floor, num			; if you're going down, num < floor
	brlt endadd
	rjmp addloop

	checkGreater:
		cp floor, num
		brge endadd

	addloop:			; traverse through the queue
		ld data, y		; check if the number you're adding already exists
		cp data, num
		breq endadd
		inc pointer
		adiw y, 2 		;increment y pointer by 2
		cp pointer, size
		brne addloop
	addNum:	
		inc size		; increment the size
		st y, num		; store the number in data memory at the end of the queue
		ldi temp, 1		; if the size of the queue is 1, we can change the direction
		cp size, temp	; of the lift
		breq changedirection
		rcall sortqueue
		rjmp endadd
	changedirection:
		cp floor, num
		brlt dirup
		ldi direction, down
		rjmp endadd
		dirup:
			ldi direction, up
	endadd:
		pop r28
		pop r29
		pop temp
		out SREG, temp
		pop temp
		ret

sortqueue:				; a bubble sort to keep the queue ordered
	push temp
	in temp, SREG
	push temp
	push r31
	push r30
	push r29
	push r28
	push size
	mov endpointer, size	; endpointer is the last index we want to check
	clr pointer
	inc pointer
	cpi direction, down		; sort ascending if direction is up
	breq sortdec			; sort descending if direction is down
	sortinc:
		incOuterLoop:		; check if the last index is equal to where we start
			clr pointer
			inc pointer
			cp pointer, endpointer
			breq endsort
			dec pointer		
			dec endpointer
			ldi r30, low(Queue)
			ldi r31, high(Queue)
			ldi r28, low(Queue)
			ldi r29, high(Queue)
			adiw z, 2
		incInnerLoop:				; run through the indices from 0 - endpointer
			cp pointer, endpointer	; swap if queue[i] > queue[i+1]
			breq incOuterloop
			inc pointer
			ld temp, y
			ld temp2, z
			cp temp, temp2
			brge incSwap
			adiw y, 2
			adiw z, 2
			rjmp incInnerLoop
		incSwap:
			st y, temp2
			st z, temp
			adiw y, 2
			adiw z, 2
			rjmp incInnerLoop
		
	sortdec:
		decOuterLoop:				; check if the last index is equal to where we start
			clr pointer		
			inc pointer
			cp pointer, endpointer
			breq endsort
			dec pointer
			dec endpointer
			ldi r30, low(Queue)
			ldi r31, high(Queue)
			ldi r28, low(Queue)
			ldi r29, high(Queue)
			adiw z, 2
		decInnerLoop:				; run through the indices from 0 - endpointer
			cp pointer, endpointer	; swap if queue[i] > queue[i+1]
			breq decOuterloop	
			inc pointer
			ld temp, y
			ld temp2, z
			cp temp, temp2
			brlt decSwap
			adiw y, 2
			adiw z, 2
			rjmp decInnerLoop
		decSwap:
			st y, temp2
			st z, temp
			adiw y, 2
			adiw z, 2
			rjmp decInnerLoop
	
	endsort:
		pop size
		pop r28
		pop r29
		pop r30
		pop r31
		pop temp
		out SREG, temp
		pop temp
		ret	

popqueue:					; remove the first element of the queue
	push temp
	in temp, SREG
	push temp
	push r31
	push r30
	push r29
	push r28
	ldi r30, low(Queue)
	ldi r31, high(Queue)
	ldi r28, low(Queue)
	ldi r29, high(Queue)
	adiw z, 2
	clr pointer
	cp pointer, size
	breq endpop
	poploop:				; we have to move everything down one index
		ld temp, z			; so that queue[i] = queue[i+1]
		st y, temp
		adiw y, 2 			;increment y pointer by 2
		adiw z, 2 			;increment z pointer by 2
		inc pointer
		cp pointer, size
		brne poploop
		dec size
	endpop:
		rcall printqueue
		pop r28
		pop r29
		pop r30
		pop r31
		pop temp
		out SREG, temp
		pop temp
		ret

printqueue:						; print the queue in an orderly fashion
	push temp					; most of this code is pretty superflous
	in temp, SREG
	push temp
	push r29
	push r28
	ldi r28, low(Queue)
	ldi r29, high(Queue)
	rcall lcd_init
	ldi data, 'F'
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	ldi data, 'l'
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	ldi data, 'o'
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	ldi data, 'o'
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	ldi data, 'r'
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	ldi data, ':'
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	ldi data, ' '
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	mov data, floor
	ldi temp, '0'
	add data, temp
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	ldi data, ' '
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	cpi direction, up
	breq printup
	ldi data, 'D'
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	ldi data, 'O'
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	ldi data, 'W'
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	ldi data, 'N'
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	jmp print
	printup:
		ldi data, 'U'
		rcall lcd_wait_busy
	   	rcall lcd_write_data            ; write the character to the screen
		ldi data, 'P'
		rcall lcd_wait_busy
	   	rcall lcd_write_data            ; write the character to the screen	
	print:
		clr pointer
		cp pointer, size
		breq endprint
		ldi data, LCD_ADDR_SET | LCD_LINE2
		rcall lcd_wait_busy
	    rcall lcd_write_com           ; write the character to the screen
		ldi data, 'N'
		rcall lcd_wait_busy
	    rcall lcd_write_data            ; write the character to the screen
		ldi data, 'e'
		rcall lcd_wait_busy
	    rcall lcd_write_data            ; write the character to the screen
		ldi data, 'x'
		rcall lcd_wait_busy
	    rcall lcd_write_data            ; write the character to the screen
		ldi data, 't'
		rcall lcd_wait_busy
	    rcall lcd_write_data            ; write the character to the screen
		ldi data, ':'
		rcall lcd_wait_busy
	    rcall lcd_write_data            ; write the character to the screen
		ldi data, ' '
		rcall lcd_wait_busy
	    rcall lcd_write_data            ; write the character to the screen
		printloop:
			ld data, y
			ldi temp, '0'
			add data, temp
			rcall lcd_wait_busy
	    	rcall lcd_write_data            ; write the character to the screen
			adiw y, 2 						;increment y pointer by 2
			inc pointer
			cp pointer, size
			brne printloop
		endprint:
			pop r28
			pop r29
			pop temp
			out SREG, temp
			pop temp
			ret

printemergency:						; print the emergency message to 
	push temp						; the LCD
	in temp, SREG
	push temp
	rcall lcd_init
	ldi data, '\"'
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	ldi data, 'E'
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	ldi data, 'm'
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	ldi data, 'e'
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	ldi data, 'r'
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	ldi data, 'g'
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	ldi data, 'e'
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	ldi data, 'n'
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	ldi data, 'c'
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	ldi data, 'y'
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen


	ldi data, LCD_ADDR_SET | LCD_LINE2
	rcall lcd_wait_busy
	rcall lcd_write_com           ; write the character to the screen
	ldi data, 'C'
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	ldi data, 'a'
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	ldi data, 'l'
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	ldi data, 'l'
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	ldi data, ' '
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	ldi data, '0'
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	ldi data, '0'
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	ldi data, '0'
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	ldi data, '\"'
	rcall lcd_wait_busy
    rcall lcd_write_data            ; write the character to the screen
	pop temp
	out SREG, temp
	pop temp
	ret
