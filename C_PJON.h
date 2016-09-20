
 /*-O//\         __     __
   |-gfo\       |__| | |  | |\ |
   |!y°o:\      |  __| |__| | \| v4.2
   |y"s§+`\     Giovanni Blu Mitolo 2012-2016
  /so+:-..`\    gioscarab@gmail.com
  |+/:ngr-*.`\
  |5/:%&-a3f.:;\    PJON is a multi-master, multi-media, device communications bus
  \+//u/+g%{osv,,\   system framework able to connect up to 255 arduino boards over
    \=+&/osw+olds.\\   one or two wires up to 5.95kB/s.
       \:/+-.-°-:+oss\
        | |       \oy\\
        > <
  _____-| |-________________________________________________________________________

Copyright 2012-2016 Giovanni Blu Mitolo gioscarab@gmail.com

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#ifndef PJON_h
  #define PJON_h
  
#include <Arduino.h>
 
#include <digitalWriteFast.h>

#include <C_SoftwareBitBang.h>

  

  /* Communication modes */
  #define SIMPLEX     150
  #define HALF_DUPLEX 151

  /* Protocol symbols */
  #define ACK         6
  #define ACQUIRE_ID  63
  #define BUSY        666
  #define NAK         21

  /* Reserved addresses */
  #ifndef BROADCAST
    #define BROADCAST 0
  #endif
  #ifndef NOT_ASSIGNED
    #define NOT_ASSIGNED 255
  #endif

  #if BROADCAST == NOT_ASSIGNED
    #error BROADCAST and NOT_ASSIGNED point the same address
  #endif

  /* Internal constants */
  #define FAIL       0x100
  #define TO_BE_SENT 74

  /* HEADER CONFIGURATION:
  Thanks to the header byte the transmitter is able to instruct
  the receiver to handle communication as requested. */

  /* Packet header bits */
  #define MODE_BIT        1 // 1 - Shared | 0 - Local
  #define SENDER_INFO_BIT 2 // 1 - Sender device id + Sender bus id if shared | 0 - No info inclusion
  #define ACK_REQUEST_BIT 4 // 1 - Request synchronous acknowledge | 0 - Do not request acknowledge

  /* [0, 1, 1]: Local bus  | Sender info included    | Acknowledge requested - DEFAULT
     [0, 0, 1]: Local bus  | No sender info included | Acknowledge requested
     [0, 0, 0]: Local bus  | No sender info included | No acknowledge
     [1, 0, 0]: Shared bus | No sender info included | No acknowledge
     [1, 1, 0]: Shared bus | Sender info included    | No acknowledge
     [1, 1, 1]: Shared bus | Sender info included    | Acknowledge requested  */


 
 

  /* Errors */
  #define CONNECTION_LOST     101
  #define PACKETS_BUFFER_FULL 102
  #define MEMORY_FULL         103
  #define CONTENT_TOO_LONG    104
  #define ID_ACQUISITION_FAIL 105

  /* Constraints:

  Max attempts before throwing CONNECTON_LOST error */
  #ifndef MAX_ATTEMPTS
    #define MAX_ATTEMPTS      125
  #endif

  /* Packets buffer length, if full PACKETS_BUFFER_FULL error is thrown */
  #ifndef MAX_PACKETS
    #define MAX_PACKETS       10
  #endif

  /* Max packet length, higher if necessary (and you have free memory) */
  #ifndef PACKET_MAX_LENGTH
    #define PACKET_MAX_LENGTH 50
  #endif

  /* Maximum random delay on startup in milliseconds */
  #define INITIAL_MAX_DELAY   1000
  /* Maximum randon delay on collision */
  #define COLLISION_MAX_DELAY 48
  /* Maximum id scan time (5 seconds) */
  #define MAX_ID_SCAN_TIME    5000000


typedef struct {
	uint8_t header;
	uint8_t receiver_id;
	uint8_t receiver_bus_id[4];
	uint8_t sender_id;
	uint8_t sender_bus_id[4];
}PacketInfo;

typedef void(*error)(uint8_t code, uint8_t data);
static void dummy_receiver_handler(uint8_t *payload, uint8_t length, const PacketInfo *packet_info) {};
static void dummy_error_handler(uint8_t code, uint8_t data) {};
typedef void(*receiver)(uint8_t *payload, uint8_t length, const PacketInfo *packet_info);


 void PJON(uint8_t device_id);
 uint16_t dispatch(uint8_t id, uint8_t *b_id, const char *packet, uint8_t length, uint32_t timing);
 uint16_t send_string(uint8_t id, char *string, uint8_t length, uint8_t header);
 void acquire_id();
 void begin();
 boolean bus_id_equality(const uint8_t *name_one, const uint8_t *name_two);
 uint8_t compute_crc_8(char input_byte, uint8_t crc);
 void copy_bus_id(uint8_t dest[], const uint8_t src[]) ;
 uint8_t device_id();
 uint8_t get_packets_count(uint8_t device_id);
 void get_packet_info(const uint8_t *packet, PacketInfo *packet_info);
 void include_sender_info(bool state) ;
 uint16_t receive(uint32_t duration);
  uint16_t receive_();
  void remove_(uint16_t id);
  void remove__all_packets(uint8_t device_id );
  uint16_t reply(const char *packet, uint8_t length);
  uint16_t send(uint8_t id, const char *packet, uint8_t length);
  uint16_t send_repeatedly(uint8_t id, const char *packet, uint8_t length, uint32_t timing);
  uint16_t send_string(uint8_t id, char *string, uint8_t length, uint8_t header);
  void set_acknowledge(boolean state);
  void set_communication_mode(uint8_t mode);
  void set_default();
  void set_error(error e);
   void set_id(uint8_t id);
   void set_packet_auto_deletion(boolean state);
    void set_pin(uint8_t pin);
     void set_pins(uint8_t input_pin, uint8_t output_pin);
     void set_receiver(receiver r);
     void set_router(boolean state) ;
     void set_shared_network(boolean state) ;
     uint8_t update();

    



     

#endif
