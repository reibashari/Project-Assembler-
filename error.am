;file ps.as
.entry LOOP
.entry LENGTH
.extern L3
STR: .string "abcdef"
;

.extern W
MAIN: mov S1.1, W, r2
 add STR,STR
LOOP: jmp W
STR: hlt
 prn #-5, STR
 sub r1, r4
 inc K
mov S1.2, r3
 bne L3
STR: hlt
STR: .string "abcdef"
LENGTH: .data 6,-9,15
K: .data 22
S1: .struct 8, "ab"
