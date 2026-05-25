# BUZZER - buzzer.c Description

To connect the buzzer module, we used the diagrams below. 
  
Buzzer code description:
-	RCC->AHB1ENR |= (0x1 << 1); //ENABLE GPIOB CLOCK
Session 6.3.10 - RCC AHB1 peripheral clock enable register (RCC_AHB1ENR)
RCC = Reset and Clock Control 
Select bit 1 from AHB1ENR that is GPIOB, then use OR bitwise compares to turn specifically this bit on (it turns ON the bit without affecting other bits), finally set the bit to 1.
1.	 0x1 → one active bit. 0x1 = 1 (decimal) = 0000 0001 (binary).
 
2.	1 → move it to position bit 1. Then shift by left means you go from 0000 0001 to 0000 0010. Result: (0x1 << 1) = 0x2.
3.	|= → set that bit in the register.
 
 
-	GPIOB->MODER &= ~(0x03 << 20);  // Clear bits [21:20]
Session 7.4.1 - GPIO port mode register (GPIOx_MODER) (x = A..H)
MODER = Mode register
1.	0x03 = 0000 0011 in binary → two bits set (11), shift left by 20 (target bit 21 and 20). 
 
2.	~ → apply bitwise NOT, this inverts all the bits.
 
3.	&= → apply AND (the result is 1 only if both bits are 1). So, bits 21 and 20 are forced to 0 and all other bits remain unchanged.
 
 
 
-	GPIOB->MODER |=  (0x01 << 20);  // Set PB10 as OUTPUT 
1.	0x1 → one active bit and shift to position 20.
 
2.	Set PB10 bits [21:20] to output mode.
 
3.	|= → set that bit in the register, bit 20 is set to 1, and bit 21 stays whatever it was before. If any bit of the comparison is 1 then set to 1.
 
-	GPIOB->OTYPER &= ~ (0x1 << 10); //OUTPUT TYPE: 0 = PUSH-PULL, 1 = OPEN-DRAIN
OTYPER = controls the output type of pins, each pin uses 1 bit only
1.	0x1 → one active bit and shift to position 10.
 
2.	~ → apply bitwise NOT, this inverts all the bits.
 
3.	&= → apply AND (the result is 1 only if both bits are 1). So, bit 10 is forced to 0 and all other bits remain unchanged.

4.	Bit 10 = 0 → Push-Pull mode. To ensure the bit becomes 0, regardless of its previous value.
 

