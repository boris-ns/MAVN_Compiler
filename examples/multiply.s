.globl main

.data
m1: .word 6
m2: .word 5
m3: .word 0

.text
main:
	la $t4, m1
	lw $t4, 0($t4)
	la $t3, m2
	lw $t3, 0($t3)
	li $t2, 1
	li $t1, 0
lab:
	add $t1, $t1, $t4
	sub $t0, $t2, $t3
	addi $t2, $t2, 1
	bltz $t0, lab
	la $t0, m3
	sw $t1, 0($t0)
	nop
