@ cuadeq.s
@ Author: Javier Mondragon Martin del Campo
@ Institution: ITESM
@ ID: A01365137
@ Description: Calculates 6x^2+9x+2

.data

question1: .asciz "x: "
operation: .asciz "6*%d^2 + 9*%d + 2 = %d\n"
err: .asciz "Error: Please try again following the instructions as it shown"

format: .asciz "%d"

x: .word 0

.text
.global main
.extern printf
.extern scanf

main:
push {ip, lr}

@ Ask for x
ldr r0, =question1
bl printf

ldr r0, =format
ldr r1, =x
bl scanf

@ Calculate

ldr r1, =x
ldr r1, [r1]
mul r2, r1, r1
mov r4, #6
mul r2, r4, r2
mov r4, #9
mul r3, r4, r1
add r3, r2, r3
add r3, r3, #2

@ Print result

mov r2, r1
ldr r0, =operation
bl printf

pop {ip, pc}

