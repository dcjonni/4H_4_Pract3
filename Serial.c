#include <18F4620.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#fuses HS, NOFCMEN, NOIESO, XT, PUT, NOBROWNOUT, NOWDT
#fuses NOPBADEN, STVREN, NOLVP, NODEBUG,
#use fast_io(B)
#use fast_io(C)
#use fast_io(D)
#use delay(clock=16000000)
#use RS232(stream=uart, baud=9600, xmit=pin_c6, rcv=pin_c7, bits=8, parity=N, stop=1)// falta documentar


bool analizaNumero(char trama[], int anchoTrama);
int16 convertirAEntero(char trama[]);
bool numerosDentroDelRango(int16 numeroEntero);
void limpiarTrama(char trama[], int anchoTrama);
void rutinaErrorLed();


void main(void){
   set_tris_a(0xC0);
   set_tris_b(0x00);
   set_tris_d(0x00);
   output_b(0x00);
   output_d(0x00);
   setup_oscillator(OSC_16MHZ);
   printf("Favor de ingresar los dos numeros y selecionar operacion '+' '-' '*' '/' \n\r");
   char value, operacion;
   char tramaPrimerNumero[10];
   char tramaSegundoNumero[10];
   int anchoTramaPrimerNumero=0, anchoTramaSegundoNumero=0;
   int empiezaTrama=0, finalizaTrama, recibiTrama;
   unsigned int16 numero_1=0, numero_2=0;
   int32 resultado;
   while(true){
      if(kbhit()){
         value=getch();
         if(value=='>'){
            empiezaTrama=1;
         }
         if((value!='>')&&(value!=',')&&(empiezaTrama==1)){
            tramaPrimerNumero[anchoTramaPrimerNumero]=value;
            anchoTramaPrimerNumero++;
         }
         if((value==',')&&(empiezaTrama!=2)){
            empiezaTrama=2;
         }
         if((empiezaTrama==2)&&(value!='>')&&(value!=',')){
            tramaSegundoNumero[anchoTramaSegundoNumero]=value;
            anchoTramaSegundoNumero++;
            finalizaTrama=1;
         }
         if((finalizaTrama==1)&&(value==',')){
            empiezaTrama=0;
         }
         if((empiezaTrama==0)&&(finalizaTrama==1)&&(value!=',')&&(value!='>')&&(value!='<')){
            operacion=value;
         }
         if((finalizaTrama==1)&&(value=='<')){
            recibiTrama=1;
            finalizaTrama=0;
            if(recibiTrama==1){
               recibiTrama=0;
               if(analizaNumero(tramaPrimerNumero, anchoTramaPrimerNumero) && analizaNumero(tramaSegundoNumero, anchoTramaSegundoNumero)){
                  numero_1 = convertirAEntero(tramaPrimerNumero);
                  numero_2 = convertirAEntero(tramaSegundoNumero);
                  limpiarTrama(tramaPrimerNumero, anchoTramaPrimerNumero);
                  limpiarTrama(tramaSegundoNumero, anchoTramaSegundoNumero);
                  anchoTramaPrimerNumero=0;
                  anchoTramaSegundoNumero=0;
                  if(numerosDentroDelRango(numero_1) && numerosDentroDelRango(numero_2)){
                     switch (operacion){
                        case '+':
                           resultado=numero_1 + numero_2;
                           printf("\n\rEl resultado es: %Ld \n\r",resultado);
                           output_b(resultado);
                           output_d(resultado>>8);
                           operacion=0;
                           numero_1=0;
                           numero_2=0;
                        break;
                        case '-':
                           resultado=numero_1 - numero_2;
                           printf("\n\rEl resultado es: %Ld \n\r",resultado);
                           output_b(resultado);
                           output_d(resultado>>8);
                           operacion=0;
                           numero_1=0;
                           numero_2=0;
                        break;
                        case '*':
                           resultado=numero_1 * numero_2;
                           printf("\n\rEl resultado es: %Ld \n\r",resultado);
                           output_b(resultado);
                           output_d(resultado>>8);
                           operacion=0;
                           numero_1=0;
                           numero_2=0;
                        break;
                        case '/':
                           if(numero_2!=0){
                              resultado=numero_1 / numero_2;
                              printf("\n\rEl resultado es: %Ld \n\r",resultado);
                              output_b(resultado);
                              output_d(resultado>>8);
                           }else{
                              printf("\n\r¡ERROR! en division\n\r");
                              //rutinaErrorLed();
                           }
                           operacion=0;
                           numero_1=0;
                           numero_2=0;
                        break;
                     }
                  }else{
                     printf("\n\r¡La trama es incorrecta! favor de ingresar valores entre 0 y 255\n\r");
                     //rutinaErrorLed();
                     limpiarTrama(tramaPrimerNumero, anchoTramaPrimerNumero);
                     limpiarTrama(tramaSegundoNumero, anchoTramaSegundoNumero);
                     anchoTramaPrimerNumero=0;
                     anchoTramaSegundoNumero=0;
                  }
               }else{
                  printf("\n\r¡La trama es incorrecta! favor de solo ingresar numeros\n\r");
                  //rutinaErrorLed();
                  limpiarTrama(tramaPrimerNumero, anchoTramaPrimerNumero);
                  limpiarTrama(tramaSegundoNumero, anchoTramaSegundoNumero);
                  anchoTramaPrimerNumero=0;
                  anchoTramaSegundoNumero=0;
               }
            }else{
               printf("\n\r¡La trama es incompleta!\n\r");
               //rutinaErrorLed();
               limpiarTrama(tramaPrimerNumero, anchoTramaPrimerNumero);
               limpiarTrama(tramaSegundoNumero, anchoTramaSegundoNumero);
               anchoTramaPrimerNumero=0;
               anchoTramaSegundoNumero=0;
            }
         }
      }
   } 
}

bool analizaNumero(char trama[], int anchoTrama){

   bool valor=true;
   for(int i=0;i<anchoTrama;i++){
       if(trama[i]<'0' || trama[i]>'9')
         valor=false;   
   }
   return valor;
}

int16 convertirAEntero(char trama[]){
   
   int16 numeroEntero= 0;
   numeroEntero = atol(trama);
   return numeroEntero;
}

bool numerosDentroDelRango(int16 numeroEntero){
   
   if((numeroEntero>=0)&&(numeroEntero<=255)){
      return true;
      }
      return false;
}

void rutinaErrorLed(){
   
   int16 contador=1;
   for(int i=1;i=16;i++){
      output_b(contador);
      output_d(contador>>8);
      delay_ms(250);
      contador = contador*2;
   }
   return;
}

void limpiarTrama(char trama[], int anchoTrama){
   
   memset(trama, '\0', anchoTrama);
   return;
}
