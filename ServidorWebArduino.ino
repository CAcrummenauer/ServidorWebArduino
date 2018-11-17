#include <SPI.h>
#include <Ethernet.h>

String readString;
#define LUZ 8

byte mac[] = {0xA4, 0x28, 0x72, 0xCA, 0x55, 0x2F};
IPAddress ip(172, 20, 8, 100);
IPAddress gateway(172, 20, 8, 1);
IPAddress subRede(255, 255, 248, 0);

EthernetServer server(80);

void setup() {
  pinMode(LUZ, OUTPUT);
  Ethernet.begin(mac, ip, gateway, subRede);
  server.begin();
  digitalWrite(LUZ, LOW);
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char caractere = client.read();
        if (readString.length() < 100) {
          readString += caractere;
        }
        if (caractere == 'n') {
          if (readString.indexOf("?ligar=1") > 0) {
            digitalWrite(LUZ, HIGH);
          } else {
            if (readString.indexOf("?ligar=0") > 0) {
              digitalWrite(LUZ, LOW);
            }
          }
          readString = "";
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("  <head>");
          client.println("    <title>CSI - Arduino</title>");
          client.println("    <meta charset='utf-8'/>");
          client.println("  </head>");
          client.println("  <body>");
          client.println("    <form>");
          if (digitalRead(LUZ) == HIGH) {
            client.println("    <h1>A luz está ligada!<br></h1>");
            client.println("    <input type='hidden' name='ligar' value='0'/>");
            client.println("    <input type='submit' value='Desligar luz'/>");
          } else {
            client.println("    <h1>A luz está desligada!<br></h1>");
            client.println("    <input type='hidden' name='ligar' value='1'/>");
            client.println("    <input type='submit' value='Ligar luz'/>");
          }
          client.println("    </form>");
          client.println("    <br>");
          client.println("  </body>");
          client.println("</html>");
          delay(1);
          client.stop();
        }
      }
    }
  }
}
