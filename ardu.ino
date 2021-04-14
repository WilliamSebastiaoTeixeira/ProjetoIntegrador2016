//Robô autônomo de limpeza.
//Grupo: Pedro Alves, William Sebastião, Lucas dos Santos, Hemily da Costa.


#include <LiquidCrystal.h> //Declaração da biblioteca para a utilização do Display LCD 16X2.
LiquidCrystal lcd(31, 33, 35, 37, 39, 41);//Declaração das portas utilizadas.

#include <Keypad.h> //Declaração da biblioteca para a utilização do Teclado Matricial 4X4.
const byte ROWS = 4;//Quantidade de linhas.
const byte COLS = 4;//Quantidade de colunas.
char keys[ROWS][COLS] = { //Mapa do teclado matricial.
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {30, 32, 36, 38};//Declaração das portas utilizadas.
byte colPins[COLS] = {42, 44, 48, 50};//Declaração das portas utilizadas.
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

#include <AFMotor.h> //Declaração da biblioteca para a utilização do Shield de controle dos motores.
AF_DCMotor motor1(1);//Declaração de motores Utilizados.
AF_DCMotor motor2(2);//Declaração de motores Utilizados.
AF_DCMotor motor4(4);//Declaração de motores Utilizados.

#define trigPin_1 25 //Declaração do pino Trig do sensor Ultrassônico "1".
#define echoPin_1 24 //Declaração do pino Echo do sensor Ultrassônico "1".
#define trigPin_2 23 //Declaração do pino Trig do sensor Ultrassônico "2".
#define echoPin_2 22 //Declaração do pino Echo do sensor Ultrassônico "2".

int contador_1 = 1;  //Criação de uma váriavel "contador_1" para mudança de menus.

void menu_1(); //Declaração da função para mostrar o menu "1".
void menu_2(); //Declaração da função para mostrar o menu "2".
void menu_3(); //Declaração da função para mostrar o menu "3".

void autonomo(); //Declaração da Função do modo autônomo.
void ultrassonicos(); //Declaração da Função do modo de teste de sensores.
void motores(); //Declaração da Função do modo de teste de motores.


//Declaração de funções para utilização dos motores, Junto com a bibliteca de controle de motor do Shield.
void vassoura_frente();
void vassoura_tras();
void vassoura_parada();

void para_frente();
void para_tras();
void para_esquerda();
void para_direita();
void parado();

void setup() {

  //Definindo a quantidade de linhas e colunas do Display lcd.
  lcd.begin(16, 2);

  //Defindo portas de saída e entrada dos sensores "1" e "2".
  pinMode(trigPin_1, OUTPUT);
  pinMode(echoPin_1, INPUT);
  pinMode(trigPin_2, OUTPUT);
  pinMode(echoPin_2, INPUT);
}

void loop() {

  char key = kpd.getKey(); //Variável correspondente as teclas do teclado matricial.

  //Criação de função para a utilização de botões para navegar na interface.
  if (key == 'A') {//Decrementa um valor da varíavel "contador_1" a cada vez que a tecla "A" for pressionada.
    contador_1--;
  }
  else if (key == 'B') {//Incrementa um valor da varíavel "contador_1" a cada vez que a tecla "B" for pressionada.
    contador_1++;
  }

  //Criação de função para implementar uma rolagem infinita na interface.
  if (contador_1 > 3) {//Se "contador_1" maior que 3 volta para 1.
    contador_1 = 1;
  }
  else if (contador_1 < 1) {//Se "contador_1" menor que 1 volta para 3.
    contador_1 = 3;
  }


  //Criação da lógica da interface de menus.

  if (contador_1 == 1) { //Se "Contador" igual a "1", "2" ou "3", mostra as informações dos "menu_1", "menu_2" ou "menu_3".
    menu_1();

    boolean contador_2 = 0; //Inicia uma variável booleana "contador_2".
    if (key == 'D') { //Incrementa um valor da varíavel "contador_2" a cada vez que a tecla "D" for pressionada.
      contador_2 = 1;
    }

    while (contador_2 == 1) { //Enquanto "contador_2" for igual a "1" ficará repetindo as ações do loop while.
      autonomo(); //Chama o modo autônomo para desvio de obstaculos.
      vassoura_frente();//Ativa a vassoura.

      key = kpd.getKey(); //Variável correspondente as teclas do teclado matricial.
      if (key == 'C') { //Decrementa um valor da varíavel "contador_2" a cada vez que a tecla "C" for pressionada.
        contador_2 = 0;
      }

      if ( contador_2 == 0 ) {//Se "contador_2" for igual a "0", destiva a vassoura e para os motores.
        parado();
        vassoura_parada();
      }
    }
  }

  if (contador_1 == 2) {//Se "Contador" igual a "1", "2" ou "3", mostra as informações dos "menu_1", "menu_2" ou "menu_3".
    menu_2();

    boolean contador_2 = 0; //Inicia uma variável booleana "contador_2".
    if (key == 'D') {//Incrementa um valor da varíavel "contador_2" a cada vez que a tecla "D" for pressionada.
      contador_2 = 1;
    }

    while (contador_2 == 1) { //Enquanto "contador_2" for igual a "1" ficará repetindo as ações do loop while.
      ultrassonicos();//Ativa o modo de teste dos sensores Ultrassônicos.
      key = kpd.getKey(); //Variável correspondente as teclas do teclado matricial.
      if (key == 'C') {//Decrementa um valor da varíavel "contador_2" a cada vez que a tecla "C" for pressionada.
        contador_2 = 0;
      }
    }
  }

  if (contador_1 == 3) {//Se "Contador" igual a "1", "2" ou "3", mostra as informações dos "menu_1", "menu_2" ou "menu_3".
    menu_3();
    boolean contador_2 = 0; //Inicia uma variável booleana "contador_2".
    if (key == 'D') {//Incrementa um valor da varíavel "contador_2" a cada vez que a tecla "D" for pressionada.
      contador_2 = 1;
    }

    while (contador_2 == 1) {//Enquanto "contador_2" for igual a "1" ficará repetindo as ações do loop while.
      motores();//Chama as informações correspondentes as teclas de teste dos motores.
      key = kpd.getKey(); //Variável correspondente as teclas do teclado matricial.

      switch (key) { //Se a tecla for igual a "2", "4", "5", "6" e "8", ativa as funções correspondentes. No caso "para_frente", "para_esquerda", "vassoura_frente", "para_direita" e "para_tras". Caso nenhuma tecla for pressionada ativa as funções "vassoura_parada" e "parado".
        case '2':
          para_frente();
          delay(1000);
          break;

        case '4':
          para_esquerda();
          delay(1000);
          break;

        case '5':
          vassoura_frente();
          delay(1000);
          break;

        case '6':
          para_direita();
          delay(1000);
          break;

        case '8':
          para_tras();
          delay(1000);
          break;

        default:
          parado();
          vassoura_parada();
          break;
      }

      if (key == 'C') { //Decrementa um valor da varíavel "contador_2" a cada vez que a tecla "C" for pressionada.
        contador_2 = 0;
      }
    }
  }
}



void menu_1() { //Mostra as informações correspondente ao "menu_1".
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">AUTONOMO");
  lcd.setCursor(0, 1);
  lcd.print("sensores");
  delay(100);
}

void menu_2() {//Mostra as informações correspondente ao "menu_2".
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">SENSORES");
  lcd.setCursor(0, 1);
  lcd.print("motores");
  delay(100);
}

void menu_3() {//Mostra as informações correspondente ao "menu_3".
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">MOTORES");
  lcd.setCursor(0, 1);
  lcd.print("autonomo");
  delay(100);
}

void ultrassonicos() { // função de teste de sensores ultrassônicos.

  //Função para utilização de sensor ultrassônico direito.
  int duration, distance;
  digitalWrite (trigPin_1, HIGH);
  delayMicroseconds (10);
  digitalWrite (trigPin_1, LOW);
  duration = pulseIn (echoPin_1, HIGH);
  distance = (duration / 2) / 29.1; // distance_1 = ultrassônico direito.

  //Função para utilização de sensor ultrassônico esquerdo.
  int duration2, distance2;
  digitalWrite (trigPin_2, HIGH);
  delayMicroseconds (10);
  digitalWrite (trigPin_2, LOW);
  duration2 = pulseIn (echoPin_2, HIGH);
  distance2 = (duration2 / 2) / 29.1; // distance2 = ultrassônico esquerdo.

  //Mostra as informações no Display LCD.
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Direito  = ");
  lcd.print(distance);
  lcd.print("cm");
  lcd.setCursor(0, 1);
  lcd.print("Esquerdo = ");
  lcd.print(distance2);
  lcd.print("cm");
  delay(100);
}

void autonomo() { //Modo autonomo, desvio de obstáculos.

  //Mostra informações no Display LCD.
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("<Modo autonomo>");

  //Função para utilização de sensor ultrassônico direito.
  int duration, distance;
  digitalWrite (trigPin_1, HIGH);
  delayMicroseconds (10);
  digitalWrite (trigPin_1, LOW);
  duration = pulseIn (echoPin_1, HIGH);
  distance = (duration / 2) / 29.1; // distance = ultrassônico direito.


  //Função para utilização de sensor ultrassônico esquerdo.
  int duration2, distance2;
  digitalWrite (trigPin_2, HIGH);
  delayMicroseconds (10);
  digitalWrite (trigPin_2, LOW);
  duration2 = pulseIn (echoPin_2, HIGH);
  distance2 = (duration2 / 2) / 29.1; // distance2 = ultrassônico esquerdo.


  //Lógica para desvio de obstáculos.

  int sensores = 0;

  if (distance < 20 || distance2 < 20) {//Lê a informações dos Sensores. Se a distância for menor que 20 centimetros em um dos dois Incrementa "1" a variável "sensores".
    sensores = 1;
  }

  int esquerda = 0;
  int direita = 0;

  if (sensores == 1) { //Executa uma função de teste, comparando os sensores direito e esquerdo.
    if ( distance > distance2 ) {
      esquerda = 1;
    }
    else {
      esquerda = 0;
    }
    if (distance2 > distance) {
      direita = 1;
    }
    else {
      direita = 0;
    }
  }

  //Decide se vai girar para a esquerda, direita ou ficar parado. Fazendo as ações com um delay de  milissegundos.
  if ( direita > 0) {
    para_esquerda();
    delay(150);
  }
  else if ( esquerda > 0) {
    para_direita();
    delay(150);
  }
  else {
    para_frente();
  }
  delay(100);
}

void motores() { //Mostra informações de comando para a função de teste de motores.
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Funcoes:");
  lcd.setCursor(1, 1);
  lcd.print("2, 4, 5, 6, 8");
  delay(100);
}



//Liga a vassoura, para frente, para trás ou parada.
void vassoura_frente() {
  motor4.setSpeed(200);
  motor4.run(FORWARD);
}

void vassoura_tras() {
  motor4.setSpeed(200);
  motor4.run(BACKWARD);
}

void vassoura_parada() {
  motor4.setSpeed(200);
  motor4.run(RELEASE);
}



// Gira os dois motores em sentidos que façam o robô ir para frente ou para trás, girar para direita ou esquerda e ficar parado.
void para_frente() {
  motor1.setSpeed(125);
  motor1.run(FORWARD);

  motor2.setSpeed(125);
  motor2.run(FORWARD);
}

void para_tras() {
  motor1.setSpeed(125);
  motor1.run(BACKWARD);

  motor2.setSpeed(125);
  motor2.run(BACKWARD);
}

void para_esquerda() {
  motor1.setSpeed(125);
  motor1.run(BACKWARD);

  motor2.setSpeed(125);
  motor2.run(FORWARD);
}

void para_direita() {
  motor1.setSpeed(125);
  motor1.run(FORWARD);

  motor2.setSpeed(125);
  motor2.run(BACKWARD);
}

void parado() {
  motor1.setSpeed(125);
  motor1.run(RELEASE);

  motor2.setSpeed(125);
  motor2.run(RELEASE);
}
