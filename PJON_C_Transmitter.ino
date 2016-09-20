
#include <C_PJON.h>



    

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW); // Initialize LED 13 to be off
  Serial.begin(9600);
  
  PJON(45); //set bus name
  set_pin(12); //set bus pin
  
  send_repeatedly(44, "B", 1, 1000000); // send repeatedelly "B" each 1 sec
  
  begin();
  
  set_receiver(receiver_function);
  
};

void receiver_function(uint8_t *payload, uint8_t length, const PacketInfo &packet_info) {
  if(payload[0] == 'B') {
    Serial.println("BLINK");
    digitalWrite(13, HIGH);
    delay(30);
    digitalWrite(13, LOW);
    
  }
}

void loop() {
  update();
  receive_();  
};
