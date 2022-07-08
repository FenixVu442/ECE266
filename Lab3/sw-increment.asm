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
                ADD     r1, #1              ; increment c2 by one
                STRB    r1, [r0, #0]        ; store back c2
                CMP     r1, #10             ; compare c2 and 10
                BNE     return              ; if c2 != 10, jump to return
                MOV     r1, #0              ; reset c2 to zero
                STRB    r1, [r0, #0]        ; store back again

                ; YOUR CODE: update c1, s2, and s1

return          BX      LR                  ; return
                .endasmfunc

                .end
