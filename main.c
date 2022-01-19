/* ------------------------------------------------------------ */
/*                     Minimal application                      */
/*                      for NUCLEO-G0B1RE                       */
/* ------------------------------------------------------------ */
typedef unsigned long uint32_t;

/**
  \brief   Data Synchronization Barrier
  \details Acts as a special kind of Data Memory Barrier.
           It completes when all explicit memory accesses before this instruction complete.
 */
__attribute__((always_inline)) static inline void __DSB(void)
{
  __asm volatile ("dsb 0xF":::"memory");
}


/* Memory and peripheral start addresses (common to all STM32 MCUs) */
#define FLASH_BASE      0x08000000
#define SRAM_BASE       0x20000000
#define PERIPH_BASE     0x40000000

/* Work out end of RAM address as initial stack pointer
 * (specific of a given STM32 MCU) */
#define SRAM_SIZE       128*1024 //STM32G0B1RE has 144 - 16 (parity)  KB of RAM
#define SRAM_END        (SRAM_BASE + SRAM_SIZE)

/* RCC peripheral addresses applicable to GPIOA
 * (specific of a given STM32 MCU) */
#define RCC_BASE        (PERIPH_BASE + 0x23800)
#define RCC_AHB1ENR     ((volatile uint32_t*)(RCC_BASE + 0x30))

/* GPIOA peripheral addresses
 * (specific of a given STM32 MCU) */
#define GPIOA_BASE      (PERIPH_BASE + 0x20000)
#define GPIOA_MODER     ((volatile uint32_t*)(GPIOA_BASE + 0x00))
#define GPIOA_ODR       ((volatile uint32_t*)(GPIOA_BASE + 0x14))

/* Function headers */
void __initialize_data(uint32_t*, uint32_t*, uint32_t*);
void _start (void);
int main(void);
void delay(uint32_t count);

/* Minimal vector table */
uint32_t *vector_table[] __attribute__((section(".isr_vector"))) = {
    (uint32_t*)SRAM_END,    // initial stack pointer (MSP)
    (uint32_t*)_start       // _start as Reset_Handler
};

/* Variables defined in linkerscript */
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;

volatile uint32_t dataVar = 0x3f;

/* Data initialization */
inline void __initialize_data(uint32_t* flash_begin, uint32_t* data_begin, uint32_t* data_end) {
    uint32_t *p = data_begin;
    while(p < data_end)
        *p++ = *flash_begin++;
}

/* Entry point */
void __attribute__((noreturn,weak)) _start (void) {
    __initialize_data(&_sidata, &_sdata, &_edata);
    asm volatile("":::"memory"); // <- Did I put this instruction at the right spot?
    main();

    for(;;);
}

/* Main function */
int main() {
    /* Enable clock on GPIOA peripheral */
    *RCC_AHB1ENR = 0x1;
    __DSB();

    /* Configure the PA5 as output pull-up */
    *GPIOA_MODER |= 0x400;  // Sets MODER[11:10] = 0x1

    while(dataVar == 0x3f) {    // Always true
        *GPIOA_ODR = 0x20;
        delay(200000);
        *GPIOA_ODR = 0x0;
        delay(200000);
    }
}

void delay(uint32_t count) {
    while(count--){
        asm volatile("");
    }
}
