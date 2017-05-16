#define uint8_t unsigned char
#define uint16_t unsigned short

#define PD_ODR      (*(volatile uint8_t *)0x500F)	//Port D data output latch register
#define PD_DDR      (*(volatile uint8_t *)0x5011)	//Port D data direction register
#define PD_CR1      (*(volatile uint8_t *)0x5012)	//Port D control register 1
#define PD_CR2      (*(volatile uint8_t *)0x5013)	//Port D control register 2

#define PE_ODR      (*(volatile uint8_t *)0x5014)	//Port D data output latch register
#define PE_DDR      (*(volatile uint8_t *)0x5016)	//Port D data direction register
#define PE_CR1      (*(volatile uint8_t *)0x5017)	//Port D control register 1
#define PE_CR2      (*(volatile uint8_t *)0x5018)	//Port D control register 2

#define UART2_SR    (*(volatile uint8_t *)0x5240) 	//UART2 status register 0xC0
#define UART2_DR    (*(volatile uint8_t *)0x5241) 	//UART2 data register 0xXX
#define UART2_BRR1  (*(volatile uint8_t *)0x5242) 	//UART2 baud rate register 1 0x00
#define UART2_BRR2  (*(volatile uint8_t *)0x5243) 	//UART2 baud rate register 2 0x00
#define UART2_CR1   (*(volatile uint8_t *)0x5244) 	//UART2 control register 1 0x00
#define UART2_CR2   (*(volatile uint8_t *)0x5245) 	//UART2 control register 2 0x00
#define UART2_CR3   (*(volatile uint8_t *)0x5246) 	//UART2 control register 3 0x00
#define UART2_CR4   (*(volatile uint8_t *)0x5247) 	//UART2 control register 4 0x00
#define UART2_CR5   (*(volatile uint8_t *)0x5248) 	//UART2 control register 5 0x00
#define UART2_CR6   (*(volatile uint8_t *)0x5249) 	//UART2 control register 6 0x00
#define UART2_GTR   (*(volatile uint8_t *)0x524A) 	//UART2 guard time register 0x00
#define UART2_PSCR  (*(volatile uint8_t *)0x524B) 	//UART2 prescaler register 0x00

#define TIM2_CR1    (*(volatile uint8_t *)0x5300)	//TIM2 control register 1
#define TIM2_IER    (*(volatile uint8_t *)0x5301)	//TIM2 Interrupt enable register 0x00
#define TIM2_SR1    (*(volatile uint8_t *)0x5302)	//TIM2 status register 1
#define TIM2_SR2    (*(volatile uint8_t *)0x5303)	//TIM2 status register 2
#define TIM2_EGR    (*(volatile uint8_t *)0x5304)	//TIM2 event generation register
#define TIM2_CCMR1  (*(volatile uint8_t *)0x5305)	//TIM2 capture/compare mode register 1
#define TIM2_CCMR2  (*(volatile uint8_t *)0x5306)	//TIM2 capture/compare mode register 2
#define TIM2_CCMR3  (*(volatile uint8_t *)0x5307)	//TIM2 capture/compare mode register 3
#define TIM2_CCER1  (*(volatile uint8_t *)0x5308)	//TIM2 capture/compare enable register 1
#define TIM2_CCER2  (*(volatile uint8_t *)0x5309)	//TIM2 capture/compare enable register 2
#define TIM2_CNTRH  (*(volatile uint8_t *)0x530A)	//TIM2 counter high
#define TIM2_CNTRL  (*(volatile uint8_t *)0x530B)	//TIM2 counter low
#define TIM2_PSCR   (*(volatile uint8_t *)0x530C)	//TIM2 prescaler register
#define TIM2_ARRH   (*(volatile uint8_t *)0x530D)	//TIM2 auto-reload register high 0xFF
#define TIM2_ARRL   (*(volatile uint8_t *)0x530E)	//TIM2 auto-reload register low
#define TIM2_CCR1H  (*(volatile uint8_t *)0x530F)	//TIM2 capture/compare register 1 high
#define TIM2_CCR1L  (*(volatile uint8_t *)0x5310)	//TIM2 capture/compare register 1 low
#define TIM2_CCR2H  (*(volatile uint8_t *)0x5311)	//TIM2 capture/compare register 2 high
#define TIM2_CCR2L  (*(volatile uint8_t *)0x5312)	//TIM2 capture/compare register 2 low
#define TIM2_CCR3H  (*(volatile uint8_t *)0x5313)	//TIM2 capture/compare register 3 high
#define TIM2_CCR3L  (*(volatile uint8_t *)0x5314)	//TIM2 capture/compare register 3 low

#define CLK_ICKR    (*(volatile uint8_t *)0x50C0)    //Internal clock control register
#define CLK_CKDIVR  (*(volatile uint8_t *)0x50C6)    //Clock divider register
#define CCR         (*(volatile uint8_t *)0x7F0A)    //Condition code register

#define FLASH_IAPSR (*(volatile uint8_t *)0x505F)    //Flash in-application programming status register
#define FLASH_DUKR  (*(volatile uint8_t *)0x5064)    //Data EEPROM unprotection register

const char hw[] = {"helloworld"};
const char test[] =
{
    //wake up
    0xFE,0xFE,0xFE,0xFE,
    //start
    0x68,
    //dest address
    0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,
    //start
    0x68,
    //control code
    0b00000001,
    //data length
    0x00,
    //data
    //no data
    //checksum
    0x8A,
    //end
    0x16
};

char * flash_data = (char *)0x4000;

static uint8_t uart_rx_head = 0;

static uint8_t uart_tx_head = 0;
static uint8_t uart_tx_size = 0;
static uint8_t uart_tx_buffer[0xff] = {0};

void uart_rx(void) __interrupt 21
{
    uint8_t uart_rx_buffer = UART2_DR;
    flash_data[uart_rx_head++] = uart_rx_buffer;

    if(uart_rx_buffer == 0x68)
        PE_ODR &= 0b11011111;
    if(uart_rx_buffer == 0x16)
        PE_ODR |= 0b00100000;
}

void uart_tx(void) __interrupt 20
{
    // clear TC bit
    UART2_SR &= 0b10111111;
    // have data in the buffer need to send
    if(uart_tx_size != uart_tx_head)
    {
        // stop receiver
        UART2_CR2 &= 0b11111011;
        // start data transmission
        UART2_DR = uart_tx_buffer[uart_tx_head++];
    }
    else
    {
        // transmission complete, stop TX interrupt
        UART2_CR2 &= 0b01111111;
        // restart receiver
        UART2_CR2 |= 0b00000100;
    }
}

void uart_send(uint8_t * data, uint8_t size)
{
    uint8_t i;
    // stop TX interrupt, update the uart TX buffer
    UART2_CR2 &= 0b01111111;
    for(i=0; i<size; i++)
        uart_tx_buffer[uart_tx_head + i] = data[i];
    uart_tx_size += size;
    // restart TX interrupt
    UART2_CR2 |= 0b10000000;
}

void delay()
{
    unsigned short k=65535;
    while(k--);
}

void main(void)
{
    /*/ set 16MHz
    // enable internal clock
    CLK_ICKR = 0b00000001;
    // no Clock divider
    CLK_CKDIVR = 0b00000000;
    //*/

    // UART init
    // Set even parity
    UART2_CR1 &= 0b11111101;
    // Parity control enabled, 1 Start bit, 9 Data bits, 1 Stop bit
    UART2_CR1 |= 0b00010100;
    // RI,TX,RX enable
    UART2_CR2 |= 0b00101100;
    // set baud to 1200 in 16MHz
    //UART2_BRR1 = 0x41;
    //UART2_BRR2 = 0x35;
    // set baud to 1200 in 2MHz
    UART2_BRR1 = 0x68;
    UART2_BRR2 = 0x03;
    //*/

    // globle interrupt enable
    __asm__("rim");

    // Port D init
    // set PD5,PD4 to output
    PD_DDR |= 0b00110000;
    // set all Port D to open drain
    //PD_CR1 = 0b00000000;
    // set PD4 to Push-pull
    PD_CR1 |= 0b00010000;
    // set PD5 to open drain
    PD_CR1 &= 0b11011111;
    //*/

    // TIM 2 init
    // enable TIM2 ch1(PD4)
    TIM2_CCER1 |= 0b00000011;
    // PWM mode 2
    // Output compare 1 preload enable
    TIM2_CCMR1 |= 0b01111000;
    // no PSCR
    TIM2_PSCR = 0;
    // 421.05 = 16MHz / 38kHz
    //TIM2_ARRH = 421/256;
    //TIM2_ARRL = 421%256;
    // 210.5 = 421 / 2
    //TIM2_CCR1H = 0;
    //TIM2_CCR1L = 210;
    // 52.63 = 2MHz / 38kHz
    TIM2_ARRH = 0;
    TIM2_ARRL = 52;
    // 26.5 = 53 / 2
    TIM2_CCR1H = 0;
    TIM2_CCR1L = 27;
    //Auto-reload preload enable & Counter enable
    TIM2_CR1 |= 0b10000001;
    //*/

    // Init FLASH
    while((FLASH_IAPSR & 0b00001000) == 0)
    {
        // unlock EEPROM write access
        FLASH_DUKR = 0xAE;
        FLASH_DUKR = 0x56;
    }

    // output test
    PE_DDR |= 0b00100000;
    PE_CR1 &= 0b11011111;
    PE_ODR |= 0b00100000;
    //*/

    // read test
    delay();
    uart_send(test, sizeof(test));

    while(1)
    {
//        delay();
//        uart_send(hw, sizeof(hw));
    }
}

