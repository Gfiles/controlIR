#include <IRremote.hpp>
#include <TinyIRSender.hpp>

const int BUFFER_SIZE = 10;
char buf[BUFFER_SIZE];

void setup() {
  Serial.begin(9600);

  uint8_t tSendPin = 2;
  IrSender.begin(tSendPin, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN); // Specify send pin and enable feedback LED at default feedback LED pin
  // You can change send pin later with IrSender.setSendPin();

  Serial.println("Ready");
}

String valA, strpin, stradr, strcom, strsum1, strsum2, strsum3, strsum4, strsum5, strsum6, strsum7, strsum8;

void loop() {
  if (Serial.available() > 0) {
    Serial.readBytesUntil('\n', buf, BUFFER_SIZE);
    valA = String(buf);

    Serial.println(valA);
    /*Serial.println(inString.length());

    for(int i=0;i<inString.length();i++){
      Serial.println(valA.charAt(i));      
    }*/

    if(valA.charAt(0) == 's' || valA.charAt(0) == 'S'){
      Serial.println("Samsung");
      //converter digitos 2 e 3 para o pin
      strsum1 =  valA.charAt(1);
      strsum2 = valA.charAt(2);
      strpin = strsum1 + strsum2;
      int pin = strpin.toInt();
      Serial.print("PIN: ");
      Serial.println(pin);  
      IrSender.setSendPin(pin);//seleciona a porta para envio do sinal
      //converter digitos 4 5 e 6 para o address
      strsum3 = valA.charAt(3);
      strsum4 = valA.charAt(4);
      strsum5 = valA.charAt(5);
      stradr = strsum3 + strsum4 + strsum5;
      uint16_t long adr = strtol(stradr.c_str(), NULL, 16);
      Serial.print("ADDRESS: 0x");
      Serial.println(adr,16);
      //converte digitos 7 8 e 9 para o commmand
      strsum6 = valA.charAt(6);
      strsum7 = valA.charAt(7);
      strsum8 = valA.charAt(8);
      strcom = strsum6 + strsum7 + strsum8;
      uint16_t long com = strtol(strcom.c_str(), NULL, 16);
      Serial.print("COMMAND: 0x");
      Serial.println(com, 16);
      //envia o sinal Samsung
      IrSender.sendSamsung(adr, com, 1);
    }else
    if(valA.charAt(0) == 'n' || valA.charAt(0) == 'N'){
      Serial.println("NEC");
      //converter digitos 2 e 3 para o pin
      strsum1 =  valA.charAt(1);
      strsum2 = valA.charAt(2);
      strpin = strsum1 + strsum2;
      int pin = strpin.toInt();
      Serial.print("PIN: ");
      Serial.println(pin);
      //converter digitos 4 5 e 6 para o address
      strsum3 = valA.charAt(3);
      strsum4 = valA.charAt(4);
      strsum5 = valA.charAt(5);
      stradr = strsum3 + strsum4 + strsum5;
      uint16_t long adr = strtol(stradr.c_str(), NULL, 16);
      Serial.print("ADDRESS: 0x");
      Serial.println(adr, 16);
      //converte digitos 7 8 e 9 para o commmand
      strsum6 = valA.charAt(6);
      strsum7 = valA.charAt(7);
      strsum8 = valA.charAt(8);
      strcom = strsum6 + strsum7 + strsum8;
      uint16_t long com = strtol(strcom.c_str(), NULL, 16);
      Serial.print("COMMAND: 0x");
      Serial.println(com,16);
      //envia o sinal NEC
      sendNEC(pin, adr, com, 1);
    }else Serial.println("Protocolo invalido");
  }
}
