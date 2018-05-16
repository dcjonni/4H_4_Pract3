#include <18F4620.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#fuses HS, NOFCMEN, NOIESO, XT, PUT, NOBROWNOUT, NOWDT
#fuses NOPBADEN, STVREN, NOLVP, NODEBUG,
#use fast_io(B)
#use fast_io(C)
#use fast_io(D)
#use delay(clock=16000000)
#use RS232(stream=uart, baud=9600, xmit=pin_c6, rcv=pin_c7, bits=8, parity=N, stop=1)// falta documentar


bool analizaNumero(char vector[], int anchoValor);
bool numerosDentroDelRango(int16 numeroEntero);
int16 convertirAEntero(char vector[], int anchoValor);
int rutinaErrorLed();


void main(void){
   set_tris_a(0xC0);
   set_tris_b(0x00);
   set_tris_d(0x00);
   output_b(0x00);
   output_d(0x00);
   setup_oscillator(OSC_16MHZ);
   printf("Hola Mundo\n\t");
   char value, operacion;
   char valor_1[10];
   char valor_2[10];
   int anchoValor_1=0, anchoValor_2=0;
   int empiezaTrama=0, finalizaTrama, recibiTrama;
   unsigned int16 numero_1=0, numero_2=0;
   int32 resultado;
   while(true){
      if(kbhit()){
         value=getch();
         printf("\n\r entro");
         if(value=='>'){
            printf("\n\r entro al if");
            empiezaTrama=1;
         }
         if((value!='>')&&(value!=',')&&(empiezaTrama==1)){
            printf("\n\r entro al segundo if");
            valor_1[anchoValor_1]=value;
            for(int i=0;i<=anchoValor_1;i++){
            printf("%d\n",valor_1[i]);
            }
            anchoValor_1++;
         }
         if((value==',')&&(empiezaTrama!=2)){
            printf("\n\r entro al tercer if");
            empiezaTrama=2;
         }
         if((empiezaTrama==2)&&(value!='>')&&(value!=',')){
            printf("\n\r entro al cuarto if \t\n");
            valor_2[anchoValor_2]=value;
            for(int i=0;i<=anchoValor_2;i++){
            printf("%d\n",valor_2[i]);
            }
            anchoValor_2++;
            finalizaTrama=1;
         }
         if((finalizaTrama==1)&&(value==',')){
            printf("\n\r entro al quinto if \t\n");
            empiezaTrama=0;
         }
         if((empiezaTrama==0)&&(finalizaTrama==1)&&(value!=',')&&(value!='>')&&(value!='<')){
            printf("\n\r entro al sexto if \t\n");
            operacion=value;
            printf("\n\r la operacion %d\n",operacion);
         }
         if((finalizaTrama==1)&&(value=='<')){
            recibiTrama=1;
            finalizaTrama=0;
            if(recibiTrama==1){
               printf("\n\r La trama esta completa \n");
               recibiTrama=0;
               if(analizaNumero(valor_1, anchoValor_1) && analizaNumero(valor_2, anchoValor_2)){
                  printf("¡Si son!");
                  numero_1 = convertirAEntero(valor_1, anchoValor_1);
                  numero_2 = convertirAEntero(valor_2, anchoValor_2);
                  //printf("%ld\n", numero_1);
                  //printf("%ld\n", numero_2);
                  anchoValor_1=0;
                  anchoValor_2=0;
                  if(numerosDentroDelRango(numero_1) && numerosDentroDelRango(numero_2)){
                  printf("\n\r estan dentro");
                     switch (operacion){
                        case '+':
                           resultado=(int16)numero_1 + (int16)numero_2;
                           output_b(resultado);
                           output_d(resultado>>8);
                           printf("\n\rresultado %Ld \n\r",resultado);
                           operacion=0;
                           numero_1=0;
                           numero_2=0;
                        break;
                        case '-':
                           resultado=(int16)numero_1 - (int16)numero_2;
                           printf("\n\rresultado %LX \n\r",resultado);
                           output_b(resultado);
                           output_d(resultado>>8);
                           operacion=0;
                           numero_1=0;
                           numero_2=0;
                        break;
                        case '*':
                           resultado=(int16)numero_1 * (int16)numero_2;
                           printf("\n\rresultado %LX \n\r",resultado);
                           output_b(resultado);
                           output_d(resultado>>8);
                           operacion=0;
                           numero_1=0;
                           numero_2=0;
                        break;
                        case '/':
                           if(numero_2!=0){
                              resultado=(int16)numero_1 / (int16)numero_2;
                              printf("\n\rresultado %LX \n\r",resultado);
                              output_b(resultado);
                              output_d(resultado>>8);
                           }else{
                              printf("¡ERROR!");
                              //rutinaErrorLed();
                           }
                           operacion=0;
                           numero_1=0;
                           numero_2=0;
                        break;
                     }
                  }else{
                     printf("\n\r ¡La trama es incorrecta! favor de ingresar valores entre 0 y 255");
                     rutinaErrorLed();
                     anchoValor_1=0;
                     anchoValor_2=0;
                  }
               }else{
                  printf("\n\r ¡La trama es incorrecta! favor de solo ingresar numeros");
                  rutinaErrorLed();
                  anchoValor_1=0;
                  anchoValor_2=0;
               }
            }else{
               rutinaErrorLed();
               printf("\n\r ¡La trama es incompleta!");
               anchoValor_1=0;
               anchoValor_2=0;
            }
         }
      }
   } 
}

bool analizaNumero(char vector[], int anchoValor){

   bool valor=true;
   for(int i=0;i<anchoValor;i++){
       if(vector[i]<'0' || vector[i]>'9')
         valor=false;   
   }
   return valor;
}

int16 convertirAEntero(char vector[], int anchoValor){
   
   int16 numeroEntero= 0;
   numeroEntero = atol(vector);
      //for(int i=0; i<anchoValor; i++)
         //numeroEntero = numeroEntero*10 + (vector[i]-48);
   return numeroEntero;
}

bool numerosDentroDelRango(int16 numeroEntero){
   
   if((numeroEntero>=0)&&(numeroEntero<=255)){
      return true;
      }
      return false;
}

int rutinaErrorLed(){
   
   int16 contador=1;
   for(int i=1;i=16;i++){
      output_b(contador);
      output_d(contador>>8);
      delay_ms(250);
      contador = contador*2;
   }
   return 1;
}
