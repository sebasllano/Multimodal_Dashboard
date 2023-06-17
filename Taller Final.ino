/*
  Receptor para interacción con teachable machine
*/


#include <ArduinoJson.h>    // Biblioteca para el formato JSON
//#include <ESP8266WiFi.h>    //Biblioteca para el manejo de WIFI del esp8266
#include <PubSubClient.h>   //Biblioteca para el manejo del protocolo MQTT
#include <WiFi.h> 



const char* ssid = "iPhone Sebastián Llano";  //Escribe la identificación de tu red
const char* password = "sebas1694"; //Escribe el password de tu red
const char* mqtt_server = "157.230.214.127"; // Escribe la dirección de tu PC asignada por tu red

WiFiClient espClient;               // Crea una instancia tipo WifiCliente llamado espClient
PubSubClient client(espClient);     //Gestiona la Biblioteca de publicación-subscripción para el cliente creado



long lastMsg = 0;                   // Declaración de variable para conteo de tiempo de transmisión de mensaje
char msg[50];                       // Decalaración de buffer para guardar el mensaje recibido
String sr2 = "";                    //Declaración de la variable sr2 para guardar la cadena de caracteres recibidos
String inputString = "";            //Declaración de variable auxiliares para datos recibidos
static char buffer2[6];             //Declaración de variables para almacenar caracteres
char rec[50];                       //Declaración de varible auxiliar para la recepción de mensajes

void setup() {
  pinMode(18,OUTPUT);
  pinMode(19, OUTPUT);
  digitalWrite(19,LOW);
  digitalWrite(18,LOW);

  Serial.begin(115200);                    //Inicializa la comunicación serial a una velocidad de 115200
  setup_wifi();                            //Inicializa la Biblioteca WIFI
  client.setServer(mqtt_server, 1883);     //Inicializa el servidor con las dirección descrita en el encabezado y el puerto que usa
  client.setCallback(callback);            //Inicializa la función que se activa cuando llegan mensajes

}

void setup_wifi() {

  delay(10);                              // espera 10 milisegundos
  Serial.println();                       // Imprime un espacio de linea
  Serial.print("Conectando a ");          // Imprime la leyenda "Conectando a"
  Serial.println(ssid);                   // Imprime la identificación de la red a al que se conecta
  WiFi.begin(ssid, password);             // Realiza la conexión a la red configurada usando el password configurado en el encabezado

  while (WiFi.status() != WL_CONNECTED) { // Espera mientras se establece la conexión
    delay(500);                           // Espera 500 milisegundos
    Serial.print(".");                    // Imprime en la terminal "." hasta que se logre la conexión
  }

  Serial.println("");                     //Imprime en la terminal un espacio
  Serial.println("WiFi connecteda");       //Imprime en la terminal un espacio
  Serial.println("dirección IP: ");       //Imprime en la terminal "dirección IP: "
  Serial.println(WiFi.localIP());         //Imprime en la terminal "dirección IP asignada al microcontrolador
}


void reconnect() {                                   //función para la reconexión

  while (!client.connected()) {                      //Espera hasta que se establezca la reconexión
    Serial.print("Esperando Conexión MQTT...");

    if (client.connect("ESP8266Cliente")) {                //Se conecta como cliente con el nombre ESP8266Cliente
      Serial.println("connectada");                        //Imprime en la terminal conectado con el cliente
      client.subscribe("Taller");              //se suscribe al tópico invernadero/actuadores
    } else {                                               // si no logra conectarse
      Serial.print("falla, rc=");                            //Imprime en la terminal "falla, rc="
      Serial.print(client.state());                          //Imprime en la terminal el tipo de falla de conexión (un múmero)
      Serial.println("Intento de nuevo en 5 segundos");      //Imprime en la terminal "Intento de nuevo en 5 segundos"
      delay(5000);                                           //Espera 5 segundos
    }
  }
}


void loop() {

  if (!client.connected()) {                                  // Pregunta si el cliente  no se ha conectado
    reconnect();                                                 //Ejecuta la función de reconexión
  }
  client.loop();                                               //En caso de conexión exitosa ejecuta el bucle del cliente
  

}



void callback(char* topic, byte* payload, unsigned int length) {


  sr2 = "";                                                      //Limpio la variable donde se guardan los mensajes recibidos
  for (int i = 0; i < length; i++) {                            //Recorre el mensaje recibido de acuerdo a su longitud
    Serial.print((char)payload[i]);                             //Imprime en la terminal serial el mensaje recibido
    rec[i] = payload[i];                                       // guardo lo recibido en una variable auxiliar
    sr2 += rec[i];                                            // Concatena los mensajes enviados en la varible sr2
    inputString = sr2;                                        // guardo todo el mensaje en la variable inputString
  }

  char msg2[inputString.length()];                             // Se crea una variable con el tamaño del valor de longitud del mensaje
  Serial.println(msg2);                                        // Se imprime el mensaje recibido
  inputString.toCharArray(msg2, inputString.length() + 1);     // Se convierte en una cadena de caracteres
  DynamicJsonBuffer jsonBuffer;                                // Se crea una buffer dinamico donde guardaremos el JSON recibido
  JsonObject &root = jsonBuffer.parseObject(msg2);             // Se Interpreta el mensaje recibido como un JSON y se guara en la variable root
  String gesto = root["gesto"];

  

  if (inputString.equals("Abrir")) {
    digitalWrite(19,HIGH);
    digitalWrite(18,LOW);
    delay(3000);
    digitalWrite(19,LOW);
    digitalWrite(18,LOW);


  }



  if (inputString.equals("Cerrar")) {
    digitalWrite(18, HIGH);
    digitalWrite(19,LOW);
    delay(3000);
    digitalWrite(19,LOW);
    digitalWrite(18,LOW);
  }




}



