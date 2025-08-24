/*
 *  EA7EE, File with 30 register to write to SI5351 at start-up 
 *
 */

#ifndef SI5351A_REVB_REG_CONFIG_HEADER
#define SI5351A_REVB_REG_CONFIG_HEADER

#define SI5351A_REVB_REG_CONFIG_NUM_REGS				30

typedef struct
{
	unsigned char address; /* 8-bit register address */
	unsigned char value; /* 8-bit register data */

} si5351a_revb_register_t;

si5351a_revb_register_t const si5351a_revb_registers[SI5351A_REVB_REG_CONFIG_NUM_REGS] =
{
	{ 0xB7, 0xD2 },   // Set crystal load capacitor to 10pF (default)
	{ 0x03, 0x02 },  // Enable CLK0
	{ 0x10, 0x0F },  // Set PLLA to CLK0, 8 mA output
	{ 0x11, 0x2F },  // Set PLLB to CLK1, 8 mA output
	{ 0x12, 0x2F },  // Set PLLB to CLK2, 8 mA output
	{ 0xB1, 0xA0 },  // Reset PLLA and PLLB
//Write data to PLLA registers
	{ 0x1A, 0xFF },
	{ 0x1B, 0xFF },
	{ 0x1C, 0x00 },
	{ 0x1D, 0x10 },
	{ 0x1E, 0x00 },
	{ 0x1F, 0xF0 },
	{ 0x20, 0x00 },
	{ 0x21, 0x00 },
//Write data to PLLB registers 	
	{ 0x22, 0xFF },
	{ 0x23, 0xFF },
	{ 0x24, 0x00 },
	{ 0x25, 0x10 },
	{ 0x26, 0x00 },
	{ 0x27, 0xF0 },
	{ 0x28, 0x00 },
	{ 0x29, 0x00 },
//Write data to MS0 registers for OUTPUT0	
	{ 0x2A, 0xFF },
	{ 0x2B, 0xFF },
	{ 0x2C, 0x00 },
	{ 0x2D, 0x13 },
	{ 0x2E, 0x7A },
	{ 0x2F, 0xFC },
	{ 0x30, 0xE7 },
	{ 0x31, 0xFA }
};

#endif