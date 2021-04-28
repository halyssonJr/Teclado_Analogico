// Bilioteca desejada
#include <Keyboard.h>

// Definição de pinos
#define CTRL_F1  A0
#define CTRL_F3  A1
#define CTRL_F4  A2
#define CTRL_F5  A3
#define CTRL_F6  A4
#define CTRL_F8  A5
#define SHIFT_F1 A8
#define SHIFT_F2 A9
#define SHIFT_F3 A10



// Definição de configuração de hardware
#define NUMERO_PINOS 9
#define LIMIAR_PRESSIONADO 100 // Regular de acordo com o desejado 
#define CONTROLE_TECLADO 3
#define LED_HABILITA 4

//#define DEBUG
#define APP


// Prototipo de funções

void controleTeclado();


// Estrutura de dados 
typedef struct{
  
  unsigned char pino; 
  char codigoChave;
  bool pressionado;
  unsigned int valorAnalogico; 
  bool ultimoEstado;
}teclado;

// Declara a esturuta de dados para as teclas desejadas
teclado tecla[]{
  {CTRL_F1,KEY_F1,false,0,false}, 
  {CTRL_F3,KEY_F3,false,0,false},  
  {CTRL_F4,KEY_F4,false,0,false},  
  {CTRL_F5,KEY_F5,false,0,false},  
  {CTRL_F6,KEY_F6,false,0,false},  
  {CTRL_F8,KEY_F8,false,0,false},  
  
  {SHIFT_F1,KEY_F1,false,0,false}, 
  {SHIFT_F2,KEY_F2,false,0,false}, 
  {SHIFT_F3,KEY_F3,false,0,false}    
};

// Variavel Global
unsigned char habilita_teclado = 0;


// *** Setup ***
void setup() {
  
  for(int indice ; indice < NUMERO_PINOS; indice++){ // Configura os LDR's com entrada
  pinMode (tecla[indice].pino, INPUT); // Define os pinos como entrada analogica
 
 }
  
 pinMode(CONTROLE_TECLADO, INPUT_PULLUP); // Configura o botão com a topologia resistor pullup
 pinMode(LED_HABILITA, OUTPUT); // Configura LED como saída
 
 digitalWrite(CONTROLE_TECLADO,1); // Mantem o nível lógico igual a 1
 digitalWrite(LED_HABILITA,0);   // Desliga o LED
 
 Keyboard.begin(); // Habilita o Arduino Leonardo para função HID - Teclado
  
 #ifdef DEBUG
  Serial.begin(9600); // Inicia a comunicação serial
 #endif

  attachInterrupt(digitalPinToInterrupt(CONTROLE_TECLADO),controleTeclado, LOW); // Método para chamar o serviço de interrupção da placa Atmel32u
}


// *** Loop ***

void loop() {
 
 
 if(habilita_teclado){ // Caso habilite o teclado esse bloco será executado
  
  #ifdef APP 
  for(int indice = 0; indice <NUMERO_PINOS; indice++){
   tecla[indice].valorAnalogico = analogRead(tecla[indice].pino);

   if(tecla[indice].valorAnalogico < LIMIAR_PRESSIONADO){tecla[indice].pressionado = true;}
        
    delay(1);
    
        if(indice >= NUMERO_PINOS-4){ // Mapea os LDRs relacionados ao CTRL
            if(tecla[indice].pressionado && !tecla[indice].ultimoEstado) {
                Keyboard.press(KEY_LEFT_CTRL);//Pressiona o Ctrl
                Keyboard.press(tecla[indice].codigoChave);  
                //Serial.println(indice);
                //Serial.println("CTRL");
              tecla[indice].ultimoEstado = true;
              
            }
        }
    
        else{// Mapea os LDRs relacionados ao CTRL
             if(tecla[indice].pressionado && !tecla[indice].ultimoEstado) {
                Keyboard.press(KEY_LEFT_SHIFT);//Pressiona o Shift
                Keyboard.press(tecla[indice].codigoChave);   // Pressiona o código de teclado selecionado
                 //Serial.println(indice);
                //Serial.println("SHIFT");
              tecla[indice].ultimoEstado = true;
              
            }
        }
        
        if (!tecla[indice].pressionado && tecla[indice].ultimoEstado){
          Keyboard.releaseAll();
          tecla[indice].ultimoEstado = false;
        }
   
   if(tecla[indice].valorAnalogico > LIMIAR_PRESSIONADO){tecla[indice].pressionado = false;}
  
 }// Fim FOR  
  delay(1);
  
  #endif 
  
  #ifdef DEBUG
   
   for(int indice = 0; indice <NUMERO_PINOS; indice++){
     Serial.print(tecla[indice].pressionado);
     //Serial.print(tecla[indice].valorAnalogico);
     
     Serial.print("\t");
   }// fim FOR
   
   Serial.print("\n");
   delay(10);
  
  #endif
 } // fimIF
 
  else{
    #ifdef DEBUG
    Serial.println("*** TECLADO DESABILITADO ***");
    delay(1000);
    #endif 
  }


}


void controleTeclado(){ // Função para habilitar a leitura dos LDR's
  
  habilita_teclado =! habilita_teclado;
  
  Serial.println( "*** TECLADO HABILITADO *** ");
  
  if(habilita_teclado){
    digitalWrite(LED_HABILITA,1);
  }
  
  else {
    digitalWrite(LED_HABILITA,0);
  }

 }// fim controleTeclado
