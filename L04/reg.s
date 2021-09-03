.data
y: .word 0 @ Final regiser

.text
.global main

main:
ldr r0, =y
ldr r1, =0xAAAAAAAA
str r1, [r0]
