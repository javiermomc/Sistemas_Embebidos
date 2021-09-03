@ add.s
@ Author: Javier Mondragon Martin del Campo
@ Institution: ITESM
@ ID: A01365137
@ Description: Simple calculator in assembly

.data

question1: .asciz "Give me the first operand:"
question2: .asciz "Give me the operation to be performed (+, -, *, /):"
question3: .asciz "Give me the second operand:"
operation: .asciz "%d %c %d = "
result: .asciz "%d\n"
err: .asciz "Error: Please try again following the instructions as it shown"

format: .asciz "%d"
cformat: .asciz "%s"


a: .word 0
b: .word 0
op: .word 0

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

@ Ask for operation
ldr r0, =question2
bl printf

ldr r0, =cformat
ldr r1, =op
bl scanf

@ Ask for second operand

ldr r0, =question3
bl printf

ldr r0, =format
ldr r1, =b
bl scanf

@ Calculate

ldr r0, =op
ldr r0, [r0]
mov r2, r0
ldr r1, =a
ldr r1, [r1]
ldr r3, =b
ldr r3, [r3]

@ Addition

cmp r0, #43
bne sub
add r4, r1, r3
b endCalc

@ Substraction
sub:
cmp r0, #45
bne mul
sub r4, r1, r3
b endCalc

@ Multiplication
mul:
cmp r0, #42
bne div
mul r4, r1, r3
b endCalc

@ Divition
div:
cmp r0, #47
bne error
sdiv r4, r1, r3
b endCalc

endCalc:

@ Print result

ldr r0, =operation
bl printf

ldr r0, =result
mov r1, r4
bl printf

pop {ip, pc}

error:

ldr r0, =err
bl printf
pop {ip, pc}
