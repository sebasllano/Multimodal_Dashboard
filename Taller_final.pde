import ddf.minim.*;
import controlP5.*;
import mqtt.*;
PImage fondo;

Minim minim;
AudioPlayer abrirPlayer; // Reproductor de sonido para el botón "Abrir"
AudioPlayer cerrarPlayer; // Reproductor de sonido para el botón "Cerrar"

ControlP5 cp5;
boolean pantallaAbierta = false;

MQTTClient client;
String gesto = "";

void setup() {
  
  fondo = loadImage("fondos.png");
 
  minim = new Minim(this);

  cp5 = new ControlP5(this);
  size(600, 350);

  client = new MQTTClient(this);
  client.connect("mqtt://157.230.214.127:1883", "SS");

  // Botón para abrir la pantalla
  cp5.addButton("abrir")
    .setPosition(143, 152)
    .setSize(57, 19)
    .setLabel("Abrir casco")
    .setColorBackground(color(43,93,72));

  // Botón para cerrar la pantalla
  cp5.addButton("cerrar")
    .setPosition(401, 152)
    .setSize(57, 19)
    .setLabel("Cerrar casco")
    .setColorBackground(color(43,93,72));

  // Cargar archivos de sonido
  abrirPlayer = minim.loadFile("CascoAbierto.mp3");
  cerrarPlayer = minim.loadFile("CascoCerrado.mp3");
}

void draw() {
  
    background(0); // Cambia el fondo a un color sólido si prefieres no utilizar el fondo de la imagen
  image(fondo, 0, 0);

}

void clientConnected() {
  println("client connected");
  client.subscribe("Taller");
  client.publish("Taller");
}

void messageReceived(String topic, byte[] payload) {
  if (topic.equals("Taller")) {
    gesto = new String(payload);
    if (gesto.equals("Abrir") || gesto.equals("Cerrar")) {

    }
  }
}

void connectionLost() {
  println("connection lost");
}

void abrir() {
  client.publish("Taller", "Abrir");
  reproducirAbrir();
}

void cerrar() {
  client.publish("Taller", "Cerrar");
  reproducirCerrar();
}

void stop() {
  if (abrirPlayer != null) {
    abrirPlayer.close();
  }
  if (cerrarPlayer != null) {
    cerrarPlayer.close();
  }
  minim.stop();
  super.stop();
}

void reproducirAbrir() {
  if (abrirPlayer != null && abrirPlayer.isPlaying()) {
    abrirPlayer.close();
  }
  abrirPlayer.rewind(); // Reiniciar reproducción desde el principio
  abrirPlayer.play();
}

void reproducirCerrar() {
  if (cerrarPlayer != null && cerrarPlayer.isPlaying()) {
    cerrarPlayer.close();
  }
  cerrarPlayer.rewind(); // Reiniciar reproducción desde el principio
  cerrarPlayer.play();
}
