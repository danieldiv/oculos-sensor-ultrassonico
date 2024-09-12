![C++](https://img.shields.io/badge/Linguagem-C++-green.svg)
![Arduino](https://img.shields.io/badge/IDE-Arduino-informational)
![ISO](https://img.shields.io/badge/ISO-Linux-blueviolet)
![ISO](https://img.shields.io/badge/ISO-Windows-blueviolet)

# oculos-sensor-ultrassonico

> O projeto tem o objetivo de acionar um buzzer(apito sonoro) quando atingir uma determinada distancia de um objeto.

- Link para simulação

https://wokwi.com/projects/398616470820568065

- Limites de acionamento

```cpp
const float UPPER_THRESHOLD = 170;
const float LOWER_THRESHOLD = 150;
```

## Calculo da distancia em centimetros

Inicialmente o `TRIG` do sensor ultrasson é colocado em baixo, é aguardado um delay inicial de **2 microsegundos** e em seguida o `TRIG` é colocado em alto, apos **10 microsegundos** o pino `TRIG` é colocado em baixo novamente, e então é calculado a distância pelo valor do pino `ECHO`.

> O valor de `0,034` é referente a velocidade do som no ar. A divisão por **2** é por causa do tempo de ida e volta.

```cpp
float readDistanceCM() {
	digitalWrite(TRIG_PIN, LOW);
	delayMicroseconds(2);
	digitalWrite(TRIG_PIN, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIG_PIN, LOW);
	int duracao = pulseIn(ECHO_PIN, HIGH);
	return duracao * 0.034 / 2;
}
```

## Acionamento do Schmitt Trigger

A variavel `estado_duplo` tem o objetivo de acionar ou desligar o buzzer, quando for maior do que `UPPER_THRESHOLD` o buzzer é desligado, e quando for menor do que `LOWER_THRESHOLD` o buzzer é acionado. Assim que o buzzer é acionado, ele só volta a desligar quando atingir o limite de `UPPER_THRESHOLD`.

```cpp
void schmitt_triggers_baixo(const float distancia) {
  if (distancia < LOWER_THRESHOLD) {
    estado_duplo = true;
  }  else if(distancia > UPPER_THRESHOLD) {
    estado_duplo = false;
  }
  digitalWrite(LED_BUILTIN, estado_duplo);
}
```
