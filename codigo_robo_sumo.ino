// Declaração de variáveis
long tempo;
float dist1, dist2;
int trigger1 = 2;  // Pino trigger do primeiro sensor de distância
int echo1 = 7;     // Pino echo do primeiro sensor de distância
int trigger2 = 4;  // Pino trigger do segundo sensor de distância
int echo2 = 8;     // Pino echo do segundo sensor de distância

// Pinos de controle da ponte H para os motores
int motorA1 = 9;  // Pino IN1 da ponte H para o motor A
int motorA2 = 10;  // Pino IN2 da ponte H para o motor A
int motorB1 = 11;  // Pino IN3 da ponte H para o motor B
int motorB2 = 3;   // Pino IN4 da ponte H para o motor B

// Pinos de controle de velocidade (PWM)
int ENA = 5;       // Pino ENA para controle de velocidade do motor A
int ENB = 6;       // Pino ENB para controle de velocidade do motor B

// Pino do sensor infravermelho (linha)
int sensorLinha = A1; // Pino analógico do sensor de linha

// Variáveis auxiliares
int linha; // Estado do sensor de linha: Preto (0) ou Branco (1)

// Funções auxiliares
float medirDistancia(int trigger, int echo) {
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  long duracao = pulseIn(echo, HIGH);
  return (duracao * 0.034) / 2; // Distância em cm
}

void pararMotores() {
  analogWrite(ENA, 0); // Desliga motor A (controlado por ENA)
  analogWrite(ENB, 0); // Desliga motor B (controlado por ENB)
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);
}

void avancar() {
  analogWrite(ENA, 255); // Motor A avança (velocidade máxima)
  analogWrite(ENB, 255); // Motor B avança (velocidade máxima)
  digitalWrite(motorA1, LOW); // Motor A avança
  digitalWrite(motorA2, HIGH);  // Motor A avança
  digitalWrite(motorB1, HIGH); // Motor B avança
  digitalWrite(motorB2, LOW);  // Motor B avança
  Serial.println("Avançando em direção ao adversário.");
}

void recuar() {
  analogWrite(ENA, 255); // Motor A avança (velocidade máxima)
  analogWrite(ENB, 230); // Motor B avança (velocidade máxima)
  digitalWrite(motorA1, HIGH); // Motor A avança
  digitalWrite(motorA2, LOW);  // Motor A avança
  digitalWrite(motorB1, LOW); // Motor B avança
  digitalWrite(motorB2, HIGH);  // Motor B avança
  delay(1500);
  Serial.println("Avançando em direção ao adversário.");
}

//void recuar() {
  //analogWrite(ENA, 180); // Motor A recua (velocidade média)
  //analogWrite(ENB, 180); // Motor B recua (velocidade média)
  //digitalWrite(motorA1, LOW); // Motor A recua
  //digitalWrite(motorA2, HIGH); // Motor A recua
  //digitalWrite(motorB1, LOW); // Motor B recua
  //digitalWrite(motorB2, HIGH); // Motor B recua
  //Serial.println("Recuando.");
//}

void girarEsquerda() {
  analogWrite(ENA, 255); // Motor A gira (velocidade máxima)
  analogWrite(ENB, 200); // Motor B gira (velocidade moderada)
  digitalWrite(motorA1, HIGH); // Motor A gira
  digitalWrite(motorA2, LOW);  // Motor A gira
  digitalWrite(motorB1, HIGH);  // Motor B gira
  digitalWrite(motorB2, LOW);  // Motor B gira
  delay(1000); // Ajuste para realizar um giro de 180 graus (tempo aproximado)
  Serial.println("Girando para a esquerda.");
}

void setup() {
  Serial.begin(9600);

  pinMode(trigger1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trigger2, OUTPUT);
  pinMode(echo2, INPUT);

  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);

  pinMode(ENA, OUTPUT); // Define o pino ENA como saída
  pinMode(ENB, OUTPUT); // Define o pino ENB como saída

  pinMode(sensorLinha, INPUT);
}

void loop() {
  // Medição das distâncias
  dist1 = medirDistancia(trigger1, echo1);
  dist2 = medirDistancia(trigger2, echo2);

  // Leitura do sensor de linha
  int valorSensorLinha = analogRead(sensorLinha);
  if (valorSensorLinha > 1000) {
    linha = 0; // Preto
  } else if (valorSensorLinha < 100) {
    linha = 1; // Branco
  } else {
    linha = -1; // Nenhuma cor definida
  }

  // Exibe os status dos sensores
  Serial.print("dist1 - ");
  Serial.println(dist1);
  Serial.print("dist2 - ");
  Serial.println(dist2);
  Serial.print("linha (valor) - ");
  Serial.println(valorSensorLinha);
  Serial.print("linha (estado) - ");
  if (linha == 0) {
    Serial.println("Preto");
  } else if (linha == 1) {
    Serial.println("Branco");
  } else {
    Serial.println("Indefinido");
  }

  // Lógica de movimentação com base nos sensores
  while (linha == 0 && (dist1 < 20 || dist2 < 20)) {
    // Parâmetro 1: Adversário detectado e linha preta detectada
    Serial.println("Adversário detectado e linha preta detectada.");
    avancar();
    dist1 = medirDistancia(trigger1, echo1);
    dist2 = medirDistancia(trigger2, echo2);
  }

  // Lógica de movimentação com base nos sensores
  while (linha == 0 && (dist1 > 20 && dist2 > 20)) {
    // Parâmetro 2 Busca por adversário
    Serial.println("Procurando adversário enquanto permanece na linha preta.");
    pararMotores();
    delay(1500); // Pausa antes de girar
    girarEsquerda();
    
    dist1 = medirDistancia(trigger1, echo1);
    dist2 = medirDistancia(trigger2, echo2);
  }

  if (linha == 1) {
    // Parâmetro 3: Linha branca detectada! Evitando saída da arena
    Serial.println("Linha branca detectada! Evitando saída da arena.");
    pararMotores();
    delay(500); // Pausa antes de girar
    recuar();
  }  else {
    pararMotores();
  }
  
  delay(100); // Pequeno atraso para evitar leituras excessivas
}