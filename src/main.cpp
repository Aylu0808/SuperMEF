#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <TimerOne.h>


/*Este codigo es la Maquina de estado finitos que se va a usar para los pulsadores
  En este caso se esta implementando con un pulsador que incrementa una variable y la muestra en un lcd
  Este codigo esta probado y funciona
  A este codigo le falta correccion de un profesor
  cambio 7:38
*/
#define FALSE 0
#define TRUE 1

#define incremento 10
#define inicio 12
#define infra //no se en que pin hay algo declarado

LiquidCrystal_I2C lcd(0x3F, 16, 2);

volatile int t = 0;
volatile int tlcd = 0;


volatile int estadoLcd = 0;
volatile int estados[] = {0,0,0}; //Incremento, inicio, infras, lcd

volatile bool flagRetencion[] = {FALSE, FALSE, FALSE}; //Incremento, inicio, infras
volatile bool flagPulso[] = {FALSE, FALSE, FALSE}; //Incremento, inicio, infras

volatile int numViajes = 0;
volatile int contadorViajes = 0;

void contando();
void cantViajes();
void infras();

/// RETENCION DE PULSADORES  //////////////////////////////////////
void state1(){
  flagPulso[0] = FALSE; //Incremento
  flagPulso[1] = FALSE; //Inicio
  flagPulso[2] = FALSE; //Infras

  if(digitalRead(incremento) == HIGH)
    estados[0] = 1;
  if(digitalRead(inicio) == HIGH)
    estados[1] = 1;
  if(digitalRead(infra) == HIGH)
    estados[2] = 1;

  if(digitalRead(incremento) == LOW){
    t = 0;
    estados[0] = 2;
  }
  if(digitalRead(inicio) == LOW){
    t = 0;
    estados[1] = 2;
  }
  if(digitalRead(infra) == LOW){
    t = 0;
    estados[2] = 2;
  }
}
void state2(){
  if(flagRetencion[0] == FALSE)
    estados[0] = 2;
  if(flagRetencion[0] == TRUE)
    estados[0] = 3;

  if(flagRetencion[1] == FALSE)
    estados[1] = 2;
  if(flagRetencion[1] == TRUE)
    estados[1] = 3;

  if(flagRetencion[2] == FALSE)
    estados[2] = 2;
  if(flagRetencion[2] == TRUE)
    estados[2] = 3;
}
void state3(){
  if(digitalRead(incremento) == LOW){ //EN EL CODIGO ANTERIOR ESTO NO FUNCIONABA ASI (FUNCIONO SIN EL IF Y EL ELSE)
    flagPulso[0] = TRUE;
    estados[0] = 1;
  }
  else{
    flagPulso[0] = FALSE;
    estados[0] = 1;
  }

  if(digitalRead(inicio) == LOW){ //EN EL CODIGO ANTERIOR ESTO NO FUNCIONABA ASI (FUNCIONO SIN EL IF Y EL ELSE)
    flagPulso[1] = TRUE;
    estados[1] = 1;
  }
  else{
    flagPulso[1] = FALSE;
    estados[1] = 1;
  }

  if(digitalRead(infras) == LOW){ //EN EL CODIGO ANTERIOR ESTO NO FUNCIONABA ASI (FUNCIONO SIN EL IF Y EL ELSE)
    flagPulso[2] = TRUE;
    estados[2] = 1;
  }
  else{
    flagPulso[2] = FALSE;
    estados[2] = 1;
  }
}

/////// CONDICIONES DE LCD ///////////////////////////////////////////
void lcd1(){
  if(digitalRead(inicio) == HIGH){
    estados[1] = 1;
    if(flagPulso[1] == FALSE)
      estadoLcd = 1;
  }
  else{
    tlcd = 0;
    estadoLcd = 2;
  }
}
void lcd2(){
  if(tlcd <= 0)
    estadoLcd = 3;
  else
    estadoLcd = 2;
}
void lcd3(){
  if(contadorViajes != numViajes){
    flagJuego = FALSE;
    estadoLcd = 3;
  }
  else
    flagJuego = TRUE;
    estadoLcd = 4;
}
void lcd4(){
  if(finMensajeFinal == FALSE) //esta flag se usaria para demostrar que se termino de imprimir el mensaje final
    estadoLcd = 4;
  else
    estadoLcd = 1;
}
/////// SALIDAS DE LCD /////////////////////////////////////////////////
// void outLcd0(){//este solo esta para usar en el defaul
//   lcd.setCursor(0,0);
//   lcd.print("default");
// }
void outLcd1(){
  lcd.setCursor(0,0);
  lcd.print("Cant de viajes: ");
  lcd.print(numViajes);
}
void outLcd2(){
  lcd.setCursor(0,0);
  lcd.print("El juego inicia");
  lcd.setCursor(0, 1);
  lcd.print("     en: ");
  lcd.print(5 - tlcd);
}
void outLcd3(){
  lcd.setCursor(0, 0);
  lcd.print("    A JUGAR!    ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
}
void outLcd4(){
  lcd.setCursor(0, 0);
  lcd.print("  Felicidades!  ");
  lcd.setCursor(0, 1);
  lcd.print(" Fin del juego! ");
}
///////////////////////////////////////////////////////////////////////
void setup(){
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Esto funciona");
  delay(1000);
  lcd.clear();

  pinMode(incremento, INPUT);
  pinMode(inicio, INPUT);
  pinMode(infra, INPUT);

  Timer1.initialize(1000);//1ms
  Timer1.attachInterrupt(contando);
}

void loop(){

  if(aceptacion == 0)
    cantViajes();

  switch(estados[0]){ //Incremento
    case 1: state1(); break;
    case 2: state2(); break;
    case 3: state3(); break;
  }
  switch(estados[1]){ //Inicio
    case 1: state1(); break;
    case 2: state2(); break;
    case 3: state3(); break;
  }
  switch(estados[2]){ //Infras
    case 1: state1(); break;
    case 2: state2(); break;
    case 3: state3(); break;
  }

  switch(estadoLcd){
    case 1: lcd1(); outLcd1(); break;
    case 2: lcd2(); outLcd2(); break;
    case 3: lcd3(); outLcd3(); break;
    case 4: lcd4(); outLcd4(); break; 
    //default: lcd.print("Ni idea"); //outLcd0();  Me parece que si le pongo un defaul se va a querer imprimir eso siempre
  }

  if(flagJuego == 0)
    infras();
}

void contando(){
  t++;
  tlcd++;
  
  if(esatados[0] == 2){
    if(t >= 300)
      flagRetencion[0] = TRUE;
    else
      flagRetencion[0] = FALSE;
  }
  if(esatados[1] == 2){
    if(t >= 300)
      flagRetencion[1] = TRUE;
    else
      flagRetencion[1] = FALSE;
  }
  if(esatados[2] == 2){
    if(t >= 300)
      flagRetencion[2] = TRUE;
    else
      flagRetencion[2] = FALSE;
  }
}
void cantViajes(){
  estadoLcd = 1;
  estados[0] = 1;
  estados[1] = 1;

  if(flagPulso[0] == TRUE)
    numViajes++;
  if(flagPulso[1] == TRUE){
    estadoLcd = 2;
    aceptacion = 1;
  }
}
void infras{
  estados[2] = 1;

  if(flagPulso[2] == TRUE)
    contadorViajes++;  
}