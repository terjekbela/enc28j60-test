////////////////////////////////////////////////////////////////////////////////
// enc28j60 minimal test example
////////////////////////////////////////////////////////////////////////////////

#include <UIPEthernet.h>

byte netMAC[] = {0xde,0xad,0xbe,0xef,0x00,0x01 };
byte netIP[]  = { 192, 168, 15, 19 };

EthernetServer netServer(80);

void setup() {
  Ethernet.begin(netMAC, netIP);
  netServer.begin();
}

void loop() {
  boolean currentBlank;
  String currentLine;
  String currentURL;
  char c;
  EthernetClient client = netServer.available();
  if (client) {
    currentBlank = true;
    currentLine  = "";
    currentURL   = "";
    while (client.connected()) {
      if (client.available()) {
        c = client.read();
        if (c == '\n' && currentBlank) {
          htmlContent(client, currentURL);
          break;
        }
        if (c == '\n' or c == '\r') {
          currentBlank = true;
          if (currentLine.indexOf('GET') > 0) {
            currentURL = currentLine;
            currentURL.replace("GET", "");
            currentURL.replace("HTTP/1.1", "");
            currentURL.replace(" ", "");
            currentLine = "";
          }
        } else if (c != '\r') {
          currentBlank = false;
          currentLine.concat(c);
        }
      }
    }
    delay(50);
    client.stop();
  }
  delay(10);
}

void htmlContent(EthernetClient ec, String url) {
  ec.println(F("HTTP/1.1 200 OK"));
  ec.println(F("Content-Type: text/html"));
  ec.println(F("Connection: close"));
  ec.println();
  ec.println(F("<!DOCTYPE html>"));
  ec.println(F("<html>"));
  ec.println(F("<head>"));
  ec.println(F("<meta charset=\"utf-8\"/>"));
  ec.print(F("<title>enc28j60 test</title>"));
  ec.println(F("</head>"));
  ec.println(F("<body style=\"display:grid;place-items:center;\">"));
  ec.println(F("<div style=\"width:200px; margin-top:200px;\">It works!</div>"));
  ec.println(F("</body>"));
  ec.println(F("</html>"));
}
