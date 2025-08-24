# This script calculate the values for a_factor, b_factor and p1, p2 for SI5351 XTAL generator
# for an example frequency of 10.695 MHz
# gen-tiny (c)2025 EA7EE 
# GPL v2

PLLRATIO=36
CFACTOR=1048575
f_xtal = 25000000
freq = 10695000

# Set VCOs of PLLA and PLLB to 650 MHz
a = PLLRATIO;     # Division factor 650/25 MHz !!!!
b = 0;            # Numerator, sets b/c=0
c = CFACTOR;      # Max. resolution, but irrelevant in this case (b=0)

#Formula for splitting up the numbers to register data, see AN619
p1 = 128 * a +  (128 * b // c) - 512
p2 = 128 * b - c *  (128 * b // c)
a2 = (p1 & 0x00030000) >> 16
a3 = (p1 & 0x0000FF00) >> 8
a4 = (p1 & 0x000000FF)
a5 = 0xF0 | ((p2 & 0x000F0000) >> 16)
a6 = (p2 & 0x0000FF00) >> 8
a7 = (p2 & 0x000000FF)

print('--REGISTER ARRAY.--')
print('--PLL_A REGISTERS--')
print(hex(a2))
print(hex(a3))
print(hex(a4))
print(hex(a5))
print(hex(a6))
print(hex(a7))
print('--PLL_B REGISTERS--')
print(hex(a2))
print(hex(a3))
print(hex(a4))
print(hex(a5))
print(hex(a6))
print(hex(a7))

c = CFACTOR; 
fdiv = (f_xtal * PLLRATIO) / freq
# division factor fvco/freq (will be integer part of a+b/c)

a = int(fdiv)
# equiv. to fractional part b/c
rm = fdiv - a
b = int(rm * c)
p1  = 128 * a +  (128 * b // c) - 512
p2 = 128 * b - c *  (128 * b // c)

print(f"FACTOR_A = {a}")
print(f"FACTOR_B = {b}")

a2 = (p1 & 0x00030000) >> 16
a3 = (p1 & 0x0000FF00) >> 8
a4 = (p1 & 0x000000FF)
a5 = 0xF0 | ((p2 & 0x000F0000) >> 16)
a6 = (p2 & 0x0000FF00) >> 8
a7 = (p2 & 0x000000FF)

print('---REGISTER ARRAY---')
print('---MS0 REGISTERS----')
print(hex(a2))
print(hex(a3))
print(hex(a4))
print(hex(a5))
print(hex(a6))
print(hex(a7)) 