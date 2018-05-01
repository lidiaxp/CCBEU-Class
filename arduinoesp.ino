#include <SoftwareSerial.h>
 
//RX pino 2, TX pino 3
SoftwareSerial esp8266(2, 3);
 
#define DEBUG true

String ssid = "NomeDaRede";      
String pass = "SenhaDaRede";  

const int led = 6;
const int botao = 7;
 
void setup(){
  Serial.begin(9600);
  esp8266.begin(112500);
  conectar(ssid, pass);
  pinMode(led, OUTPUT); 
  pinMode(botao, INPUT);
}
 
void loop(){
  if(digitalRead(botao) == 1){
    count++;
  }
  
  // Verifica se o ESP8266 esta enviando dados
  if (esp8266.available()){
    if (esp8266.find("+IPD,")){
      delay(300);
      int connectionId = esp8266.read() - 48;
 
      String webpage = "<head><meta http-equiv=""refresh"" content=""3"">";
      webpage += "</head><h1><u>ESP8266 - Web Server</u></h1>";
      webpage += "Click <a href=\"/H\">here</a> Ligar a led<br>";
      webpage += "Click <a href=\"/L\">here</a> Desligar a led <br>";
      webpage += "O botão foi clicado: ";
      webpage += count;
      webpage += " vezes.";
      webpage += "Click <a href=\"/R\">here</a> Resetar o contador<br>";
 
      String cipSend = "AT+CIPSEND=";
      cipSend += connectionId;
      cipSend += ",";
      cipSend += webpage.length();
      cipSend += "rn";
 
      enviarDados(cipSend, 1000, DEBUG);
      enviarDados(webpage, 1000, DEBUG);

      if(esp.8266.endsWith("H")){ 
         digitalWrite(led, HIGH);   
      }

      if(esp.8266.endsWith("L")){ 
         digitalWrite(led, LOW);   
      }

       if(esp.8266.endsWith("R")){ 
        count = 0;
      }
 
      String closeCommand = "AT+CIPCLOSE=";
      closeCommand += connectionId; // append connection id
      closeCommand += "rn";
 
      enviarDados(closeCommand, 3000, DEBUG);
    }
  }
}
 
String enviarDados(String command, const int timeout, boolean debug){
  // Envio dos comandos AT para o modulo
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while ( (time + timeout) > millis()){
    while (esp8266.available()) {
      // The esp has data so display its output to the serial window
      char c = esp8266.read(); // read the next character.
      response += c;
    }
  }
  if (debug){
    Serial.print(response);
  }
  return response;
}

void conectar(String ssid, String senha){
  enviarDados("AT+RSTrn", 2000, DEBUG); // rst
  
  // Conecta a rede wireless
  enviarDados("AT+CWJAP=" + ssid + "," + senha + "rn", 2000, DEBUG);
  delay(3000);
  enviarDados("AT+CWMODE=1rn", 1000, DEBUG);
  
  // Mostra o endereco IP
  enviarDados("AT+CIFSRrn", 1000, DEBUG);
  
  // Configura para multiplas conexoes
  enviarDados("AT+CIPMUX=1rn", 1000, DEBUG);
  
  // Inicia o web server na porta 80
  enviarDados("AT+CIPSERVER=1,80rn", 1000, DEBUG);
}
