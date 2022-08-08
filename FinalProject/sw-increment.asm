; Assembly file for increment() function
; Lab 3, ECE 266, Spring 2020
; Created by Zhao Zhang

; Function prototype: stopWatchIncrement(displayState_t *pDisplayState);
; This function increment the number shown on the stop watch display. It should be called once every one centisecond.
                .text
                .global stopWatchIncrement	; make this symbol global
stopWatchIncrement  .asmfunc

                ; Update c2, continue to c1 if c2 is reset to zero
                LDRB    r1, [r0, #0]        ; load c2 to r1, note c2's offset is zero which can be omitted
                LDRB    r2, [r0, #1] ; load c1 to r2
                LDRB    r3, [r0, #2] ; load s2 to r3
                LDRB    r4, [r0, #3] ; load s1 to r4

                ADD     r1, #1              ; increment c2 by one
                STRB    r1, [r0, #0]        ; store back c2
                CMP     r1, #10             ; compare c2 and 10
                BNE     return              ; if c2 != 10, jump to return

                MOV     r1, #0              ; reset c2 to zero
                STRB    r1, [r0, #0]        ; store back again
                ADD		r2, #1				; increment c1 by one
                STRB    r2, [r0, #1]        ; store back c1
				CMP     r2, #10             ; compare c1 and 10
                BNE     return              ; if c2 != 10, jump to return

                MOV     r2, #0              ; reset c2 to zero
                STRB    r2, [r0, #1]        ; store back again
                ADD		r3, #1				; increment s2 by one
                STRB    r3, [r0, #2]        ; store back s2
				CMP     r3, #10             ; compare s2 and 10
                BNE     return              ; if s2 != 10, jump to return

                MOV     r3, #0              ; reset s2 to zero
                STRB    r3, [r0, #2]        ; store back again
                ADD		r4, #1				; increment s1 by one
                STRB    r4, [r0, #3]        ; store back s1
				CMP     r4, #6              ; compare s1 and 6
                BNE     return              ; if s1 != 10, jump to return

                MOV     r4, #0              ; reset s2 to zero
                STRB    r4, [r0, #3]        ; store back again

return          BX      LR                  ; return
                .endasmfunc

                .end
