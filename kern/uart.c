#include "kern/uart.h"
#include "kern/sysdef.h"

// Taken from
//      https://github.com/bigmagic123/d1-nezha-baremeta/blob/main/src/1.startup/main.c

void pz_sys_uart_init(void)
{
    vaddr_t addr;
    uint32_t val;

    /* Config GPIOB8 and GPIOB9 to txd0 and rxd0 */
    addr = 0x02000030 + 0x04;
    val = pz_read32(addr);
    val &= ~(0xf << ((8 & 0x7) << 2));
    val |= ((0x6 & 0xf) << ((8 & 0x7) << 2));
    pz_write32(addr, val);

    val = pz_read32(addr);
    val &= ~(0xf << ((9 & 0x7) << 2));
    val |= ((0x6 & 0xf) << ((9 & 0x7) << 2));
    pz_write32(addr, val);

    /* Open the clock gate for uart0 */
    addr = 0x0200190c;
    val = pz_read32(addr);
    val |= 1 << 0;
    pz_write32(addr, val);

    /* Deassert uart0 reset */
    addr = 0x0200190c;
    val = pz_read32(addr);
    val |= 1 << 16;
    pz_write32(addr, val);

    /* Config uart0 to 115200-8-1-0 */
    addr = 0x02500000;
    pz_write32(addr + 0x04, 0x0);
    pz_write32(addr + 0x08, 0xf7);
    pz_write32(addr + 0x10, 0x0);
    val = pz_read32(addr + 0x0c);
    val |= (1 << 7);
    pz_write32(addr + 0x0c, val);
    pz_write32(addr + 0x00, 0xd & 0xff);
    pz_write32(addr + 0x04, (0xd >> 8) & 0xff);
    val = pz_read32(addr + 0x0c);
    val &= ~(1 << 7);
    pz_write32(addr + 0x0c, val);
    val = pz_read32(addr + 0x0c);
    val &= ~0x1f;
    val |= (0x3 << 0) | (0 << 2) | (0x0 << 3);
    pz_write32(addr + 0x0c, val);
}

void pz_sys_uart_putc(char c)
{
    vaddr_t addr = 0x02500000;

    while ((pz_read32(addr + 0x7c) & (0x1 << 1)) == 0)
        ;
    pz_write32(addr + 0x00, c);

    if (c == '\n') {
        pz_sys_uart_putc('\r');
    }
}

void pz_sys_uart_puts(const char *s)
{
    while (*s != '\0') {
        pz_sys_uart_putc(*s);
        s++;
    }
}
