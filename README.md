# oculos-sensor-ultrassonico

- Execução

> O projeto foi desenvolvido utilizando o Arduino IDE.

- Link para simulação

https://wokwi.com/projects/398616470820568065

- Limites de acionamento

```cpp
const float UPPER_THRESHOLD = 170;
const float LOWER_THRESHOLD = 150;
```

- Calculo da distancia em centimetros

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
