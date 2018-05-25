.globl main

.data
m1: .word 6
m2: .word 5
m3: .word 0

.text
:
lab:
main:
	la $t0, m1
	lw $t0, 0($t0)
	la $t0, m2
	lw $t0, 0($t0)
	li $t0, 1
	li $t0, 0
