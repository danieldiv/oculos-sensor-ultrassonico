![C++](https://img.shields.io/badge/Linguagem-C++-green.svg)
![Arduino](https://img.shields.io/badge/IDE-Arduino-informational)
![ISO](https://img.shields.io/badge/ISO-Linux-blueviolet)
![ISO](https://img.shields.io/badge/ISO-Windows-blueviolet)

# Sistema de Detecção de Obstáculos Baseado em Sensores Ultrassônicos para Auxílio à Mobilidade de Deficientes Visuais

> Este trabalho descreve o desenvolvimento de um protótipo de óculos inteligentes para auxiliar a mobilidade de pessoas com deficiência visual. O projeto visa criar uma solução de baixo custo e fácil implementação, com potencial para melhorar a autonomia e segurança de deficientes visuais. O sistema utiliza um sensor ultrassônico HC-SR04 para detectar obstáculos, com um alcance de até 4 metros, configurado para emitir alertas sonoros a uma distância de até 1,5 metros. O sensor é controlado por um microcontrolador Arduino Uno, que aciona um buzzer quando obstáculos são detectados, fornecendo feedback imediato ao usuário. Durante os testes, o protótipo demonstrou precisão na detecção de obstáculos a curta distância e tempo de resposta adequado, embora tenha apresentado limitações na detecção de superfícies transparentes e problemas com o conforto durante o uso prolongado.

## Link para simulação

https://wokwi.com/projects/398616470820568065

## Link para o artigo

## Fluxo geral do Sistema

1. O computador envia informações ao arduino sobre o qual a velocidade o motor DC deverá funcionar.
2. O arduino envia para o motor DC a velocidade que ele irá funcionar, e esse valor é setado.
3. O motor DC estando com um disco acoplado, gira esse disco.
4. O disco girando é captado pelo sensor infravermelho, enviando informações sobre o RPM.
5. O arduino recebe informações do sensor infravermelho, interpreta e extrai o RPM atual.
6. O arduino então retorna a informação para o computador para que o usuário possa visualizar o RPM do motor DC.

## Componentes Utilizados

- Arduino Uno
- Sensor de Radiofrequencia
- Buzzer Ativo 6V
- Bateria 9 V
- Óculos para Suporte

## Resultados

O circuito do protótipo foi inicialmente montado e testado em uma plataforma de simulação online. Wokwi. Nesta plataforma validamos a lógica de operação do sistema e otimizamos o layout dos componentes. Após a simulação, o circuito foi implementado fisicamente nos óculos inteligentes. A estrutura final integra um sensor ultrassônico HC-SR04, um microcontrolador Arduino Uno, um buzzer e uma bateria de 9V, além de um Schmitt Trigger para garantir a estabilidade do sinal e minimizar ruídos indesejados.

Durante os testes experimentais, avaliamos a eficácia do sistema em diversos cenários para medir a precisão na detecção de obstáculos, o tempo de resposta do sistema, o conforto do dispositivo e o desempenho geral da bateria. Os resultados obtidos são discutidos detalhadamente nas seções seguintes.

### Precisão na Detecção de Obstáculos

Durante os testes, o protótipo demonstrou capacidade de detectar obstáculos a uma distância de até 1,5 metros. A detecção foi observada com base no feedback sonoro emitido pelo buzzer, que funcionou conforme esperado quando o objeto estava dentro do raio de ação do sensor. No entanto, notamos que superfícies transparentes ou reflexivas, como vidro, apresentaram dificuldades na detecção, um desafio comum em sistemas baseados em ultrassom.

### Tempo de Resposta do Sistema}

O tempo de resposta foi avaliado de forma empírica, com base na observação da rapidez com que o sistema emitia o alerta sonoro após a detecção de um obstáculo. Em condições normais, o protótipo respondeu de forma rápida e eficaz, permitindo ao usuário ajustar sua trajetória antes de colidir com o objeto. Embora não tenhamos dados quantitativos precisos, o tempo de resposta foi considerado adequado para o propósito do projeto.

### Conforto e Usabilidade

O protótipo foi avaliado por sua ergonomia e usabilidade. Observou-se que o peso total do dispositivo, aproximadamente 116 gramas, causava algum desconforto em usos prolongados. Além disso, o volume do buzzer, embora eficaz em ambientes silenciosos, foi considerado excessivo e incômodo em locais mais tranquilos, afetando não apenas o usuário, mas também as pessoas ao redor.

### Limitações Identificadas

O sistema apresentou limitações na detecção de obstáculos transparentes e pequenos, como superfícies de vidro, onde as ondas ultrassônicas não retornam adequadamente. Além disso, a ausência de um controle manual para desligar o dispositivo resultou em um consumo de bateria contínuo, o que poderia ser evitado com a inclusão de um botão de desligamento. O peso dos óculos e o volume do buzzer também foram identificados como pontos a serem melhorados.

## Documentação do Código

### Limites de acionamento

```cpp
const float UPPER_THRESHOLD = 170;
const float LOWER_THRESHOLD = 150;
```

### Calculo da distancia em centimetros

Inicialmente o pino `TRIG` do sensor ultrasson é colocado em baixo, é aguardado um delay inicial de **2 microsegundos** e em seguida o pino `TRIG` é colocado em alto, apos **10 microsegundos** o pino `TRIG` é colocado em baixo novamente, e então é calculado a distância pelo valor do pino `ECHO`.

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

### Acionamento do Schmitt Trigger

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

### Intervalo de Apito do Buzzer

A função tem o objetivo de modificar o intervalo em que o buzzer ira apitar, quanto mais proximo estiver do objeto, mais rapido sera o apito.

```cpp
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
```
