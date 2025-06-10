//
// Description:
// ------------
// - SGC ATU interface to Kenwood transceiver. Transceiver has 2 bidirectional lines, TS and TT. TS is pulled low for tuning start, ATU answers by pulling
//   TT low until tuning is achieved, then TT is set high, TX respond by putting TS high
// - If transceiver pulls TT low, ATU answers by pulling TS low and reset ATU. TS is then set high, TX answers by setting TT high. This is also how an
//   external tuner is detected.
// - LED is used to indicate tuner status. On when Tuned, Blinking while tuning
// - TS and TT are set to open drain. In open drain mode, input is still active. Led is set to open drain, but could be push-pull, with circuit/code modified accordingly.
// - An external pull-up is used for the tuned status, internal pull-up is insufficient in terms of uA provided, vs mA with a resistor. Filtration is required on input, as RFI will affect function.
//
// References:
// -----------
// - CNLohr ch32fun: https://github.com/cnlohr
// - Wagiminator: https://github.com/wagiminator (for the template used, I like it more than ch32fun)
// - WCH Nanjing Qinheng Microelectronics: http://wch.cn
//
// Compilation Instructions:
// -------------------------
// - Make sure GCC toolchain (gcc-riscv64-unknown-elf, newlib) and Python3 with
//   rvprog (via pip) are installed. In addition, Linux requires access rights to
//   WCH-LinkE programmer.
// - Connect the WCH-LinkE programmer to the MCU board.
// - Run 'make flash'.
//


// ===================================================================================
// Libraries, Definitions and Macros
// ===================================================================================
#include <system.h>                               // system functions
#include <gpio.h>                                 // GPIO functions
#define TS PC1
#define TUNED PA2
#define TT PC4
#define RESET PD6
#define PIN_LED PC2

// ===================================================================================
// Main Function
// ===================================================================================
int main(void) {
  uint8_t count;
  // Setup pins for input and output
  PIN_output_OD(TS);
  PIN_input(TUNED);
  PIN_output_OD(TT);
  PIN_output(RESET);
  PIN_output_OD(PIN_LED);
  PIN_low(PIN_LED);
  PIN_high(TT);
  PIN_high(TS);
  PIN_low(RESET);
  DLY_ms(50);

// Loop
  while(1) {

// Check if TT is is pulled low. If so, set TS low, reset SGC tuner to put it in standby. This is also how Kenwood detect external tuners.
    if(!PIN_read(TT)) {
      DLY_ms(10);
      PIN_low(TS);
      PIN_high(RESET);
      DLY_ms(100);
      PIN_high(TS);
      PIN_low(RESET);
      DLY_ms(50);   
      }  
// Check if TS is pulled low. If so, set TT low to initiate tuning. Wait until either SGC tuned signal is set to gnd or kenwood transceiver stop tuning by setting TS high again.
// Led is flashing while tuning is on-going.
    if(!PIN_read(TS)) {
      DLY_ms(10);
      count=0;  
      PIN_low(TT);
      DLY_ms(100);
      while(!PIN_read(TS)&&PIN_read(TUNED)) {
      DLY_ms(1);
      count++;
      if (count == 80) {
        PIN_toggle(PIN_LED);
        count = 0;
        }
      }
      PIN_high(TT);
      DLY_ms(50);
      }
// Set led status to reflect SGC tuner status
      if (PIN_read(TUNED)) {
      PIN_high(PIN_LED);
      }
      else {
      PIN_low(PIN_LED);
      }
//    PIN_write(PIN_LED,PIN_read(TUNED)); didn't try this, should work. Just got too lazy to reflash.
  }
}


