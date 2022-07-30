#include <ESP8266WiFi.h>
#include <DHT.h>
 
//Habilita pino D4 para envio de dados
#define DHTPIN D4 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11
 
//Define pino e tipo de DHT a ser utilizado
DHT dht(DHTPIN, DHTTYPE);
 
//Credenciais ThingSpeak
const char* host = "api.thingspeak.com";
String ApiKey = "cole aqui sua chave de escrita";
 
//Caminho para leitura dos dados
String path_one = "/update?key=" +ApiKey + "&field1=";
String path_two = "&field2=";
 
//Login e senha do wi-fi principal
char* ssid = "LoginPrincipal"; //coloque aqui o nome da rede principal que se deseja conectar
char* pass = "SenhaPrincipal"; //coloque aqui a senha da rede principal que se deseja conectar
 
//Define contador igual a zero
int count = 0;
 
 
void setup()
{
 
//Inicia DHT11
  Serial.println("Inicia teste no DHT11!");
  dht.begin();
  Serial.begin(115200);
  Serial.println("Estabelecendo conexão...");
  connect();
   
//Caso o Wi-Fi seja desconectado:
    if(WiFi.status()!=WL_CONNECTED)
      {
        Serial.print("A conexão não foi estabelecida, por favor apertar botão reset ou aguardar...\n");
        return;
      }     
}
void loop() 
{
 
// A leitura da temperatura e umidade pode levar 250ms!
  // O atraso do sensor pode chegar a 2 segundos.
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  // Testa se retorno é valido, caso contrário algo está errado.
  if (isnan(t) || isnan(h)) 
 {
    Serial.println("Falha na leitura do DHT");
 } 
  else
{
 
    Serial.print("Umidade: ");
    Serial.print(h);
    Serial.print(" %t");
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println(" *C");
 }
//Conexão ao endereço IP específico
WiFiClient client;
const int httpPort = 80;
 
//Caso não se conecte ao ThingSpeak
if (!client.connect(host, httpPort))
  {
    Serial.println("Falha na conexão, por favor aguarde estabilização da rede");
    if (ssid=="LoginPrincipal") //coloque aqui o nome da rede principal que se deseja conectar
    {
      ssid = "LoginAuxiliar"; //coloque aqui o nome da rede auxiliar que se deseja conectar
      pass = "SenhaAuxiliar"; //coloque aqui a senha da rede auxiliar que se deseja conectar
      Serial.println("Estabelecendo conexão na rede auxiliar");
      connect();
 
      //Tenta estabelecer nova conexão a rede principal
      if(WiFi.status()!=WL_CONNECTED)
      return;
    }
    else
    {
      ssid = "LoginPrincipal"; //coloque aqui o nome da rede principal que se deseja conectar
      pass = "SenhaPrincipal"; //coloque aqui a senha da rede principal que se deseja conectar
      Serial.println("Estabelecendo conexão na rede principal");
      connect();
 
      //Tenta estabelecer nova conexão a rede auxiliar
      if(WiFi.status()!=WL_CONNECTED)
      return;
    }
    return;
  }
 
//Envia os dados de leitura do sensor DHT 11 ao ThingSpeak
         client.print(String("GET ") + path_one + t + path_two + h + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" + 
         "Connection: keep-alive\r\n\r\n");
         delay(1500);
 }
void connect()
{
  //Quando conectado contador volta a zero
  count = 0;
  WiFi.begin(ssid, pass);
 
  //Enquanto Wi-Fi não estiver conectado e o limite do tempo de conexão não exceder
  while (WiFi.status()!= WL_CONNECTED && count<= 130)
  {
    delay(100);
    Serial.print(".");
    count = count + 1;
  }
  if ((WiFi.status() == WL_CONNECTED))
  {
    Serial.println("");
    Serial.print("Conectado em: ");
    Serial.println(ssid);
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());
   
   if (ssid=="LoginPrincipal") //coloque aqui o nome da rede principal que se deseja conectar
   {
      Serial.println("Rede principal conectada");
      delay (100);
     
    }
   if (ssid=="LoginAuxiliar") //coloque aqui o nome da rede auxiliar que se deseja conectar
   {
      Serial.println("Rede auxiliar conectada");
      delay(100);
   }
  }
}