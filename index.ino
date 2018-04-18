#include <SPI.h>
#include <WiFi.h>

//Definindo características da rede
char ssid[] = "NomeDaRede";      
char pass[] = "SenhaDaRede";   
int keyIndex = 0; 

//Definindo onde está a led e botao
const int led = D6;
const int botao = D7;
int count = 0; //contador

int status = WL_IDLE_STATUS;
WiFiServer server(80);

//Para ip estático
//IPAddress ip(192, 168, 0, 175);
//IPAddress gateway(192, 168, 0, 1);
//IPAddress subnet(255, 255, 255, 0);

void setup() {
  Serial.begin(115200);      
  pinMode(led, OUTPUT); 
  pinMode(botao, INPUT);  
  conectar();                          
}

void loop() {
  if(digitalRead(botao) == 1){
    count++;
  }
  
  WiFiClient client = server.available();   

  if (client) {                             
    Serial.println("new client");           
    String currentLine = "";                
    while (client.connected()) {            
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        if (c == '\n') {                    
          if (currentLine.length() == 0) {
            //Montando a página web
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.print("Click <a href=\"/H\">here</a> Ligar a led<br>");
            client.print("Click <a href=\"/L\">here</a> Desligar a led <br>");
            client.print("O botão foi clicado: ");
            client.print(count);
            client.print(" vezes.");
            client.print("Click <a href=\"/R\">here</a> Resetar o contador<br>");

            client.println();
            break;
          } else {    
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      
        }

        //Configurando as ações da página
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(led, HIGH);               
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(led, LOW);                
        }
        if (currentLine.endsWith("GET /R")) {
          count = 0;                
        }
      }
    }
    client.stop();
    Serial.println("cliente desconectado");
  }
}

//Método para mostrar o Status do WiFi
void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  Serial.print("Para acessar essa página acesse http://");
  Serial.println(ip);
}

//Método para se conectar à internet
void conectar(){
   if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("Não tem Shield de internet");
    while (true);      
  }

  String fv = WiFi.firmwareVersion();
  if (fv != "1.1.0") {
    Serial.println("Atualize o firmware");
  }

  
  while (status != WL_CONNECTED) {
    Serial.print("Se conectando a rede: ");
    Serial.println(ssid);                   

    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  server.begin();                           
  printWifiStatus(); 
}
