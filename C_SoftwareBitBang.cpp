

#include "C_SoftwareBitBang.h"





    /* Check if the channel is free for transmission:
    If receiving 10 bits no 1s are detected
    there is no active transmission */

    boolean can_start(uint8_t input_pin, uint8_t output_pin) {
      uint8_t i;
	  pinModeFast(input_pin, INPUT);
	 
      for(i = 0; i < 9; i++) {
		  if (digitalReadFast(input_pin)) {
			  
			  return false;
		  }
		  
		
        delayMicroseconds(SWBB_BIT_WIDTH);
      }
      if(digitalReadFast(input_pin)) return false;
      pinModeFast(output_pin, OUTPUT);
      return true;
    }


    /* Every byte is prepended with 2 synchronization padding bits. The first
       is a longer than standard logic 1 followed by a standard logic 0.
       __________ ___________________________
      | SyncPad  | Byte                      |
      |______    |___       ___     _____    |
      | |    |   |   |     |   |   |     |   |
      | | 1  | 0 | 1 | 0 0 | 1 | 0 | 1 1 | 0 |
      |_|____|___|___|_____|___|___|_____|___|
        |
       ACCEPTANCE

    The reception tecnique is based on finding a logic 1 as long as the
    first padding bit within a certain threshold, synchronizing to its
    falling edge and checking if it is followed by a logic 0. If this
    pattern is recognised, reception starts, if not, interference,
    synchronization loss or simply absence of communication is
    detected at byte level. */

    void send_byte(uint8_t b, uint8_t input_pin, uint8_t output_pin) {
      uint8_t mask;
	  
	  digitalWriteFast(output_pin, HIGH);
      delayMicroseconds(SWBB_BIT_SPACER);
      digitalWriteFast(output_pin, LOW);
      delayMicroseconds(SWBB_BIT_WIDTH);
      for(mask = 0x01; mask; mask <<= 1) {
        digitalWriteFast(output_pin, b & mask);
        delayMicroseconds(SWBB_BIT_WIDTH);
      }
    }


    /* Syncronize with transmitter:
     This function is used only in byte syncronization.
     READ_DELAY has to be tuned to correctly send and
     receive transmissions because this variable shifts
     in which portion of the bit, the reading will be
     executed by the next read_byte function */

    uint8_t syncronization_bit(uint8_t pin) {
      delayMicroseconds((SWBB_BIT_WIDTH / 2) - SWBB_READ_DELAY);
      uint8_t bit_value = digitalReadFast(pin);
      delayMicroseconds(SWBB_BIT_WIDTH / 2);
      return bit_value;
    }


    /* Read a byte from the pin */

    uint8_t read_byte(uint8_t pin) {
      
	  uint8_t i;
	  uint8_t byte_value = 0b00000000;
      /* Delay until the center of the first bit */
      delayMicroseconds(SWBB_BIT_WIDTH / 2);
      for(i = 0; i < 7; i++) {
        /* Read in the center of the n one */
        byte_value += digitalReadFast(pin) << i;
        /* Delay until the center of the next one */
        delayMicroseconds(SWBB_BIT_WIDTH);
      }
      /* Read in the center of the last one */
      byte_value += digitalReadFast(pin) << 7;
      /* Delay until the end of the bit */
      delayMicroseconds(SWBB_BIT_WIDTH / 2);
      return byte_value;
    }


    /* Check if a byte is coming from the pin:
     This function is looking for padding bits before a byte.
     If value is 1 for more than ACCEPTANCE and after
     that comes a 0 probably a byte is coming:
      ________
     |  Init  |
     |--------|
     |_____   |
     |  |  |  |
     |1 |  |0 |
     |__|__|__|
        |
      ACCEPTANCE */

    uint16_t receive_byte(uint8_t input_pin, uint8_t output_pin) {
      /* Initialize the pin and set it to LOW to reduce interference */
      pullDownFast(input_pin);

      if(output_pin != input_pin && output_pin != NOT_ASSIGNED)
        pullDownFast(output_pin);

      uint32_t time = micros();
      /* Do nothing until the pin goes LOW or passed more time than SWBB_BIT_SPACER duration */
      while(digitalReadFast(input_pin) && (uint32_t)(micros() - time) <= SWBB_BIT_SPACER);
      /* Save how much time passed */
      time = micros() - time;
      /* is for sure equal or less than SWBB_BIT_SPACER, and if is more than ACCEPTANCE
         (a minimum HIGH duration) and what is coming after is a LOW bit
         probably a byte is coming so try to receive it. */
      if(time >= SWBB_ACCEPTANCE && !syncronization_bit(input_pin))
        return (uint8_t)read_byte(input_pin);
      return FAIL;
    }


    /* Receive byte response */

    uint16_t receive_response(uint8_t input_pin, uint8_t output_pin) {

      if(output_pin != input_pin && output_pin != NOT_ASSIGNED)
        digitalWriteFast(output_pin, LOW);

      uint16_t response = FAIL;
      uint32_t time = micros();
      while(response == FAIL && (uint32_t)((micros() - SWBB_BIT_SPACER) - SWBB_BIT_WIDTH) <= time)
        response = receive_byte(input_pin, output_pin);
      return response;
    }


    /* Send byte response to package transmitter */

    void send_response(uint8_t response, uint8_t input_pin, uint8_t output_pin) {
      pinModeFast(output_pin, OUTPUT);
      send_byte(response, input_pin, output_pin);
      pullDownFast(output_pin);
    }


