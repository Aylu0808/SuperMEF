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

volatile int retencionIncremento = 0;
volatile int retencionInicio = 0;
volatile int estadoLcd = 0;
volatile int estadoInfras = 0;

volatile bool flagRetencionIncremento = FALSE;
volatile bool flagPulsoIncremento = FALSE;
volatile bool flagRetencionInicio = FALSE;
volatile bool flagPulsoInicio = FALSE;
volatile bool flagRetencionInfras = FALSE;
volatile bool flagPulsoInfras = FALSE;

volatile int numViajes = 0;
volatile int contadorViajes = 0;

void contando();
void cantViajes();
void infras();

/// RETENCION DE PULSADOR INCREMENTO ///////////////////////////////
void state1Incremento(){
  flagPulsoIncremento = FALSE;

  if(digitalRead(incremento) == HIGH)
    retencionIncremento = 1;

  if(digitalRead(incremento) == LOW){
    t = 0;
    retencionIncremento = 2;
  }
}
void state2Incremento(){
  if(flagRetencionIncremento == FALSE)
    retencionIncremento = 2;
  if(flagRetencionIncremento == TRUE)
    retencionIncremento = 3;
}
void state3Incremento(){
  if(digitalRead(incremento) == LOW){ //EN EL CODIGO ANTERIOR ESTO NO FUNCIONABA ASI (FUNCIONO SIN EL IF Y EL ELSE)
    flagPulsoIncremento = TRUE;
    retencionIncremento = 1;
  }
  else{
    flagPulsoIncremento = FALSE;
    retencionIncremento = 1;
  }
}
/// RETENCION DE PULSADOR INICIO /////////////////////////////////
void state1Inicio(){
  flagPulsoInicio = FALSE;

  if(digitalRead(inicio) == HIGH)
    retencionInicio = 1;

  if(digitalRead(inicio) == LOW){
    t = 0;
    retencionInicio = 2;
  }
}
void state2Inicio(){
  if(flagRetencionInicio == FALSE)
    retencionInicio = 2;
  if(flagRetencionInicio == TRUE)
    retencionInicio = 3;
}
void state3Inicio(){
  if(digitalRead(inicio) == LOW){ //EN EL CODIGO ANTERIOR ESTO NO FUNCIONABA ASI (FUNCIONO SIN EL IF Y EL ELSE)
    flagPulsoInicio = TRUE;
    retencionInicio = 1;
  }
  else{
    flagPulsoInicio = FALSE;
    retencionInicio = 1;
  }
}
/// RETENCION DE PULSADOR INFRA /////////////////////////////////
void state1Infra(){
  flagPulsoInfras = FALSE;

  if(digitalRead(infra) == HIGH)
    estadoInfras = 1;

  if(digitalRead(infra) == LOW){
    t = 0;
    estadoInfras = 2;
  }
}
void state2Infra(){
  if(flagRetencionInfras == FALSE)
    estadoInfras = 2;
  if(flagRetencionInfras == TRUE)
    estadoInfras = 3;
}
void state3Infra(){
  if(digitalRead(infra) == LOW){ //EN EL CODIGO ANTERIOR ESTO NO FUNCIONABA ASI (FUNCIONO SIN EL IF Y EL ELSE)
    flagPulsoInfras = TRUE;
    estadoInfras = 1;
  }
  else{
    flagPulsoInicio = FALSE;
    estadoInfras = 1;
  }
}
/////// CONDICIONES DE LCD ///////////////////////////////////////////
void lcd1(){
  if(digitalRead(inicio) == HIGH){
    retencionInicio = 1;
    if(flagPulsoInicio == FALSE)
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

  switch(retencionIncremento){
    case 1: state1Incremento(); break;
    case 2: state2Incremento(); break;
    case 3: state3Incremento(); break;
  }
  switch(retencionInicio){
    case 1: state1Inicio(); break;
    case 2: state2Inicio(); break;
    case 3: state3Inicio(); break;
  }
  switch(estadoInfras){
    case 1: state1Infras(); break;
    case 2: state2Infras(); break;
    case 3: state3Infras(); break;
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
  
  if(retencionIncremento == 2){
    if(t >= 300)
      flagRetencionIncremento = TRUE;
    else
      flagRetencionIncremento = FALSE;
  }
  if(retencionInicio == 2){
    if(t >= 300)
      flagRetencionInicio = TRUE;
    else
      flagRetencionInicio = FALSE;
  }
  if(estadoInfras == 2){
    if(t >= 300)
      flagRetencionInfras = TRUE;
    else
      flagRetencionInfras = FALSE;
  }
}
void cantViajes(){
  estadoLcd = 1;
  retencionIncremento = 1;
  retencionInicio = 1;

  if(flagPulsoIncremento == TRUE)
    numViajes++;
  if(flagPulsoInicio == TRUE){
    estadoLcd = 2;
    aceptacion = 1;
  }
}
void infras{
  estadoInfras = 1;

  if(flagPulsoInfras == TRUE)
    contadorViajes++;  
}