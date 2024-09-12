#define ECHO_PIN 2
#define TRIG_PIN 3
#define BUZZER_PIN 12

const float UPPER_THRESHOLD = 170;
const float LOWER_THRESHOLD = 150;

bool estado_simples = false;
bool estado_duplo = false;

bool tocando = false;
int count_ativo = 0;
int max_count_dist = -1;

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

float readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  int duracao = pulseIn(ECHO_PIN, HIGH);
  return duracao * 0.034 / 2;
}

void single_threshold(const float distancia) {
  if (distancia > UPPER_THRESHOLD) {
    estado_simples = true;
  } else {
    estado_simples = false;
  }
  Serial.print(estado_simples);
  Serial.print(" | ");

//  digitalWrite(LED_BUILTIN, estado_simples);
}

void schmitt_triggers_cima(const float distancia) {
  if (distancia > UPPER_THRESHOLD) {
    estado_duplo = true;
  }  else if(distancia < LOWER_THRESHOLD) {
    estado_duplo = false;
  }
  Serial.print(estado_duplo);
  Serial.print(" | ");
}

void schmitt_triggers_baixo(const float distancia) {
  if (distancia < LOWER_THRESHOLD) {
    estado_duplo = true;
  }  else if(distancia > UPPER_THRESHOLD) {
    estado_duplo = false;
  }
//  Serial.println(estado_duplo);

  digitalWrite(LED_BUILTIN, estado_duplo);
}

void loop() {
  float dist = readDistanceCM();

//  digitalWrite(LED_BUILTIN, estado);

  Serial.print(dist);
  Serial.println(" cm | ");
//  single_threshold(dist);
//  schmitt_triggers_cima(dist);
  schmitt_triggers_baixo(dist);

  max_count_dist = map(dist, 0, UPPER_THRESHOLD, 1, 10);
  Serial.println(max_count_dist);
  

  delay(50);
  
  if (estado_duplo && !tocando) {
    count_ativo += 1;
    if (count_ativo >= max_count_dist){
      tocando = true;  
    }
  } else if(estado_duplo && tocando){
    count_ativo -= 1;
    if (count_ativo <= 0){
      tocando = false;
    }
  } else{
    count_ativo = 0;
    tocando = false;
  }
    
  if (tocando) {
  tone(BUZZER_PIN, 500);
  } else {
    noTone(BUZZER_PIN);
  }
  
}
