#include <18F4620.h>
#include <stdio.h>
#include <stdbool.h>
#fuses HS, NOFCMEN, NOIESO, XT, PUT, NOBROWNOUT, NOWDT
#fuses NOPBADEN, STVREN, NOLVP, NODEBUG,
#use delay(clock=16000000)
#use RS232(stream=uart, baud=9600, xmit=pin_c6, rcv=pin_c7, bits=8, parity=N, stop=1)// falta documentar
#use fast_io(B)
#use fast_io(C)
#use fast_io(D)


char valor_1[10];
char valor_2[10];

bool analizaNumero(char vector[], int anchoValor);
int convertirAEntero(char vector[], int anchoValor);
void rutinaErrorLed();
//void cleanVector(char vector[], int anchoValor);


void main(void){
   set_tris_a(0xC0);
   set_tris_b(0x00);
   set_tris_d(0x00);
   output_b(0x00);
   output_d(0x00);
   setup_oscillator(OSC_16MHZ);
   printf("Hola Mundo\n\t");
   char value, operacion;
   int anchoValor_1=0, anchoValor_2=0;
   int empiezaTrama=0, finalizaTrama, recibiTrama, numero_1;
   //int16 resultado;
   while(true){
      if(kbhit()){
         value=getch();
         printf("\n entro");
         if(value=='>'){
            printf("\n entro al if");
            empiezaTrama=1;
         }
         if((value!='>')&&(value!=',')&&(empiezaTrama==1)){
            printf("\n entro al segundo if");
            valor_1[anchoValor_1]=value;
            for(int i=0;i<=anchoValor_1;i++){
            printf("%d\n",valor_1[i]);
            }
            anchoValor_1++;
            //numero_1 = numero_1*10 + (value - '0');
            //printf("%d\n"numero_1);
            
         }
         if((value==',')&&(empiezaTrama!=2)){
            printf("\n entro al tercer if");
            empiezaTrama=2;
         }
         if((empiezaTrama==2)&&(value!='>')&&(value!=',')){
            printf("entro al cuarto if \t\n");
            valor_2[anchoValor_2]=value;
            for(int i=0;i<=anchoValor_2;i++){
            printf("%d\n",valor_2[i]);
            }
            anchoValor_2++;
            finalizaTrama=1;
         }
         if((finalizaTrama==1)&&(value==',')){
            printf("entro al quinto if \t\n");
            empiezaTrama=0;
         }
         if((empiezaTrama==0)&&(finalizaTrama==1)&&(value!=',')&&(value!='>')&&(value!='<')){
            printf("entro al sexto if \t\n");
            operacion=value;
            printf("la operacion %d\n",operacion);
         }
         if((finalizaTrama==1)&&(value=='<')){
            recibiTrama=1;
            finalizaTrama=0;
            if(recibiTrama==1){
               printf("La trama esta completa \n");
               recibiTrama=0;
               if(analizaNumero(valor_1, anchoValor_1) && analizaNumero(valor_2, anchoValor_2)){
                  printf("¡Si son!");
                  numero_1 = convertirAEntero(valor_1, anchoValor_1);
                  anchoValor_1=0;
                  anchoValor_2=0;
                  printf("%d\n"numero_1);
                  /*numero_2 = convertirAEntero(valor_2[]);
                  if((numerosDentroDelRango(numero_1)==true)&&(numerosDentroDelRango(numero_2)==true)){
                   //switch (operacion){
                      /*case '+':
                           resultado=(int16)numero_1 + (int16)numero_2;
                           operacion=0;
                           numero_1=0;
                           numero_2=0;
                        break;
                        case '-':
                           resultado=(int16)numero_1 - (int16)numero_2;
                           operacion=0;
                           numero_1=0;
                           numero_2=0;
                        break;
                        case '*':
                           resultado=(int16)numero_1 * (int16)numero_2;
                           operacion=0;
                           numero_1=0;
                           numero_2=0;
                        break;
                        case '/':
                           if(numero_2!=0){
                              resultado=(int16)numero_1 / (int16)numero_2;
                           }else{
                              printf("¡ERROR!")
                              rutinaErrorLed();
                           }
                           operacion=0;
                           numero_1=0;
                           numero_2=0;
                        break;
                     }
                  }else{
                     printf("¡La trama es incorrecta! favor de ingresar valores entre 0 y 255");
                     rutinaErrorLed();
                  }*/
               }else{
                  printf("¡La trama es incorrecta! favor de solo ingresar numeros");
                  rutinaErrorLed();
               }
            }else{
               printf("¡La trama es incompleta!");
               rutinaErrorLed();
            }
         }
      }
      
      //printf("El resultado es ");
      //output_b(resultado);
      //output_d(resultado>>8);
   } 
   //cleanVector(valor_1, anchoValor_1);
   //cleanVector(valor_2, anchoValor_2);
}

bool analizaNumero(char vector[], int anchoValor){

   bool valor=true;
   for(int i=0;i<anchoValor;i++){
       if(vector[i]<'0' || vector[i]>'9')
         valor=false;   
   }
   return valor;
}

/*int convertirAEntero(char vector[], int anchoValor){
   int numero;
   for(int i=0;i>=anchoValor;i++){
   numero = (vector[i] - 48);
   }
   return numero;
}

 /* bool numerosDentroDelRango(int numeroEntero){
         if((numeroEntero>=0)&&(numeroEntero<=255)){
            return true;
            }
            return false;
      }*/

void rutinaErrorLed(){
   int16 contador=1;
   for(int i=1;i=16;i++){
      output_b(contador);
      output_d(contador>>8);
      delay_ms(250);
      contador = contador*2;
   }
}

/*void cleanVector(char vector[], int anchoValor){
   for(int i=0;i<anchoValor;i++){
      vector[i]=" ";
   }
}*/
