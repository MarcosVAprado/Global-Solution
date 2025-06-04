#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Definição dos pinos do sensor ultrassônico
#define TRIGGER_PIN 7  // Pino de controle do sinal de disparo
#define ECHO_PIN 6     // Pino que recebe o sinal de eco

// Definição dos pinos dos LEDs indicadores
#define LED_VERDE 2     // Indica nível seguro
#define LED_AMARELO 3   // Indica nível médio de alerta
#define LED_VERMELHO 4  // Indica nível crítico

// Definição do pino do buzzer
#define BUZZER 5  // Emitirá som quando nível crítico for atingido

// Inicialização do display LCD (endereço I2C 0x27)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Definição das alturas em centímetros para os níveis de água
const int ALTURA_BAIXA = 50;   // Nível seguro
const int ALTURA_MEDIA = 175;  // Nível de alerta

void setup() {
    // Configuração dos pinos como entrada ou saída
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(LED_VERDE, OUTPUT);
    pinMode(LED_AMARELO, OUTPUT);
    pinMode(LED_VERMELHO, OUTPUT);
    pinMode(BUZZER, OUTPUT);

    // Inicialização do LCD
    lcd.begin(16, 2);
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Sistema iniciado");
}

void loop() {
    long duration;
    int distance;

    // Emitindo pulso de disparo para medir distância
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);

    // Captura do tempo de resposta do sinal de eco
    duration = pulseIn(ECHO_PIN, HIGH);

    // Cálculo da distância com base no tempo de resposta
    distance = duration * 0.034 / 2;

    // Atualizando display LCD com valor medido
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Altura: ");
    lcd.print(distance);
    lcd.print(" cm");

    // Lógica dos LEDs e buzzer baseada na distância medida
    if (distance < ALTURA_BAIXA) {
        // Nível seguro (LED verde aceso)
        digitalWrite(LED_VERDE, HIGH);
        digitalWrite(LED_AMARELO, LOW);
        digitalWrite(LED_VERMELHO, LOW);
        digitalWrite(BUZZER, LOW);
        lcd.setCursor(0, 1);
        lcd.print("Nivel: Baixo");
    } else if (distance < ALTURA_MEDIA) {
        // Nível médio (LED amarelo aceso)
        digitalWrite(LED_VERDE, LOW);
        digitalWrite(LED_AMARELO, HIGH);
        digitalWrite(LED_VERMELHO, LOW);
        digitalWrite(BUZZER, LOW);
        lcd.setCursor(0, 1);
        lcd.print("Nivel: Medio");
    } else {
        // Nível crítico (LED vermelho aceso + alerta sonoro)
        digitalWrite(LED_VERDE, LOW);
        digitalWrite(LED_AMARELO, LOW);
        digitalWrite(LED_VERMELHO, HIGH);
        digitalWrite(BUZZER, HIGH);
        lcd.setCursor(0, 1);
        lcd.print("Nivel: Critico!");

        // Ativa um som de alerta com um tom agudo
        tone(BUZZER, 2000);
        delay(50);
        noTone(BUZZER);  // Para o som após um curto intervalo
    }

    delay(50); // Pequeno intervalo antes da próxima leitura
}
