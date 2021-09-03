@ add.s
@ Author: Javier Mondragon Martin del Campo
@ Institution: ITESM
@ ID: A01365137
@ Description: Simple calculator in assembly

.data

question1: .asciz "Give me the first operand:"
question2: .asciz "Give me the second operand:"
operation: .asciz "%d + %d = %d\n"
err: .asciz "Error: Please try again following the instructions as it shown"

format: .asciz "%d"

a: .word 0
b: .word 0

.text
.global main
.extern printf
.extern scanf

main:
push {ip, lr}

@ Ask for first operand
ldr r0, =question1
bl printf

ldr r0, =format
ldr r1, =a
bl scanf

@ Ask for second operand

ldr r0, =question2
bl printf

ldr r0, =format
ldr r1, =b
bl scanf

@ Calculate

ldr r0, =operation
ldr r0, [r0]
mov r2, r0
ldr r1, =a
ldr r1, [r1]
ldr r2, =b
ldr r2, [r2]

add r3, r1, r2

@ Print result

ldr r0, =operation
bl printf

pop {ip, pc}

