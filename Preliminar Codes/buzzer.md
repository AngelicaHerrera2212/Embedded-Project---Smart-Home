# ACTIVE BUZZER - buzzer.c Description

To connect the buzzer module, we used the diagrams below. 

  <img width="222" height="241" alt="image" src="https://github.com/user-attachments/assets/4b4cb6b4-0311-447d-b2db-aa21af2b76e0" />

  <img width="209" height="163" alt="image" src="https://github.com/user-attachments/assets/3ba1d86b-6673-4f7d-ad88-1f66a9307a9a" />

  <img width="87" height="194" alt="Screenshot 2026-05-28 012722" src="https://github.com/user-attachments/assets/7d1b531a-0131-49ff-8a3f-228e881e7e2d" />


Buzzer code description:
-	RCC->AHB1ENR |= (0x1 << 1); //ENABLE GPIOB CLOCK
Session 6.3.10 - RCC AHB1 peripheral clock enable register (RCC_AHB1ENR)
RCC = Reset and Clock Control 
Select bit 1 from AHB1ENR that is GPIOB, then use OR bitwise compares to turn specifically this bit on (it turns ON the bit without affecting other bits), finally set the bit to 1.
1.	 0x1 → one active bit. 0x1 = 1 (decimal) = 0000 0001 (binary).
 <img width="240" height="256" alt="image" src="https://github.com/user-attachments/assets/fafa264a-c88d-414a-9474-0829ab76bf29" />

2.	1 → move it to position bit 1. Then shift by left means you go from 0000 0001 to 0000 0010. Result: (0x1 << 1) = 0x2.
3.	|= → set that bit in the register.
- <img width="242" height="113" alt="image" src="https://github.com/user-attachments/assets/280c4083-9d41-4356-a392-15225a712fa7" />
- <img width="500" height="378" alt="image" src="https://github.com/user-attachments/assets/229b0688-d1ef-431f-9236-7c8dd23ecb67" />

-	GPIOB->MODER &= ~(0x03 << 20);  // Clear bits [21:20]
Session 7.4.1 - GPIO port mode register (GPIOx_MODER) (x = A..H)
MODER = Mode register
1.	0x03 = 0000 0011 in binary → two bits set (11), shift left by 20 (target bit 21 and 20). 
 <img width="259" height="131" alt="image" src="https://github.com/user-attachments/assets/ea3ac239-0f01-45c4-99eb-bfe65dd56ee0" />

2.	~ → apply bitwise NOT, this inverts all the bits.
 <img width="259" height="131" alt="image" src="https://github.com/user-attachments/assets/ff4a2153-10f8-44f0-9477-e538237221ca" />

3.	&= → apply AND (the result is 1 only if both bits are 1). So, bits 21 and 20 are forced to 0 and all other bits remain unchanged.
 <img width="272" height="130" alt="image" src="https://github.com/user-attachments/assets/d2f7736d-2825-4b8c-a0ae-536629650ad6" />
<img width="272" height="180" alt="image" src="https://github.com/user-attachments/assets/4e5511f1-d97e-4e72-8bfa-399374d11aa4" />


 <img width="540" height="325" alt="image" src="https://github.com/user-attachments/assets/bfcf7859-b6b8-43c2-a731-1fa82636ad9b" />

-	GPIOB->MODER |=  (0x01 << 20);  // Set PB10 as OUTPUT 
1.	0x1 → one active bit and shift to position 20.
 <img width="242" height="175" alt="image" src="https://github.com/user-attachments/assets/3f9cac0e-318c-4543-bfe7-f2698d1edd0c" />

2.	Set PB10 bits [21:20] to output mode.
 <img width="236" height="183" alt="image" src="https://github.com/user-attachments/assets/23e9d80e-e7ea-45ec-b875-27cabd951c7e" />

3.	|= → set that bit in the register, bit 20 is set to 1, and bit 21 stays whatever it was before. If any bit of the comparison is 1 then set to 1.

<img width="222" height="058" alt="image" src="https://github.com/user-attachments/assets/b7b28f96-8740-4262-bc16-3d7d9c86a42a" />

GPIOB->OTYPER &= ~ (0x1 << 10); //OUTPUT TYPE: 0 = PUSH-PULL, 1 = OPEN-DRAIN
OTYPER = controls the output type of pins, each pin uses 1 bit only

1.	0x1 → one active bit and shift to position 10.
   <img width="236" height="094" alt="image" src="https://github.com/user-attachments/assets/46577c72-bea9-48ff-a997-f3e2e80e35cb" />

2.	~ → apply bitwise NOT, this inverts all the bits.
 <img width="236" height="102" alt="image" src="https://github.com/user-attachments/assets/20376a29-b815-4892-b8cd-2d23318aaae3" />

3.	&= → apply AND (the result is 1 only if both bits are 1). So, bit 10 is forced to 0 and all other bits remain unchanged.

4.	Bit 10 = 0 → Push-Pull mode. To ensure the bit becomes 0, regardless of its previous value.
 
<img width="536" height="260" alt="image" src="https://github.com/user-attachments/assets/df773575-a45b-4307-a989-7d176d8f7309" />

- GPIOB->PUPDR &= ~(0x03 << 20); //NO PULL-UP / PULL-DOWN

The instruction disables both pull-up and pull-down resistors on pin PB10, leaving it in a floating state (unless externally driven).

<img width="536" height="304" alt="image" src="https://github.com/user-attachments/assets/a74ccdb4-9824-4b86-803d-2b4e3d1d9356" />

- GPIOB->ODR &= ~(0x1 << 10); //INITIAL STATE = OFF

This instruction drives pin PB10 low.

1. GPIOB->ODR, This is the Output Data Register. Each bit corresponds to one GPIO pin’s output value.

2. (0x1 << 10), 0x1 = binary 0001. Shift left by 10 → targets bit 10. This corresponds to pin PB10

3. ~(0x1 << 10), Inverts the mask → all bits = 1 except bit 10 = 0.

4. &=, Bitwise AND clears bit 10 while leaving all other bits unchanged.

<img width="521" height="253" alt="image" src="https://github.com/user-attachments/assets/f1b52a03-2ef1-421a-a302-23357ee4af02" />

- Funtion void Buzzer_On(void) //*TURN BUZZER ON

GPIOB->ODR |= (0x1 << 10); 

The instruction (0x1 << 10) → selects bit 10 → pin PB10. Then |= (bitwise OR) → sets that bit to 1. Result: PB10 = HIGH (1)

- Function void Buzzer_Off(void) //TURN BUZZER OFF

GPIOB->ODR &= ~(0x1 << 10);

The instruction (0x1 << 10) → selects PB10. Then ~ → inverts → clears that bit. After that, &= → forces bit to 0. Result: PB10 = LOW (0).

- The other functions are set of sounds that depends on these explained before.
