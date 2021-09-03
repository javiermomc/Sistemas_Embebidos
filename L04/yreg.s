@ yreg.s
@ Author: Javier Mondragon Martin del Campo
@ Institution: ITESM
@ ID: A01365137
@ Description: Registers multiple values on y variable.

.data

y: .word 0

.text
.global main

main:

ldr r0, =y

@0xAAAAAAAA
ldr r1, =0xAAAAAAAA
str r1, [r0]

@0xBBBBBBBB
ldrh r2, =0xBBBBBBBB
mov r3, r2
lsl r2, #16
add r2, r2, r3
str r2, [r0]

@0xCCCCCCCC

mov r1, r0
sub r1, r1, #4
ldr r2, =0xCCCCCCCC
str r2, [r1, #4]

bx lr
