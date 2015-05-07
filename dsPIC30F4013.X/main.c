#include "Arduino.h"

/***** Configuration bits *****/
#pragma config FOSFPR = FRC_PLL16
#pragma config FCKSMEN = CSW_FSCM_OFF
#pragma config WDT = WDT_OFF
#pragma config FPWRT = PWRT_64
#pragma config BODENV = BORV20
#pragma config BOREN = PBOR_ON
#pragma config MCLRE = MCLR_EN
#pragma config GWRP = GWRP_OFF
#pragma config GCP = CODE_PROT_OFF
#pragma config ICS = ICS_PGD

int main()
{
  init();

  setup();

  while(1)
  {
    loop();
  }
}

