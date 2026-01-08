/*
 * Solar Panel Protection System & Simulation
 * Desenvolvido para projeto final SENAI - Automação Industrial
 * Autor: Gabriel Lucas Barbosa
 *
 * Funcionalidades:
 * 1. Controle de Lona de Proteção:
 * - Cobre o painel (Avança motor) à noite (Sem luz).
 * - Descobre o painel (Recua motor) de dia (Com luz).
 * - SEGURANÇA: Recolhe imediatamente se Vento > 30 m/s.
 * 2. Simulação de Geração:
 * - LED indica operação (Aceso com luz, apagado sem luz).
 */

#define PIN_LDR A0          // Sensor de Luz
#define PIN_ANEMOMETRO A1   // Sensor de Vento (Simulado por Potenciômetro ou sinal analógico)
#define PIN_LED_SOLAR 13    // LED indicador de geração

// Driver Motor (Ex: Ponte H L298N)
#define PIN_MOTOR_A_1 5     // Avanço
#define PIN_MOTOR_A_2 6     // Recuo
#define PIN_MOTOR_B_1 9     // Sincronizado
#define PIN_MOTOR_B_2 10    // Sincronizado

// Parâmetros de Calibração
const int LIMITE_LUZ = 400;       // Limite entre dia/noite (0-1023)
const float FATOR_VENTO = 0.048;  // Conversão leitura ADC p/ m/s (ajustar conforme sensor)
const int VENTO_MAX = 30;         // Limite de segurança (m/s)

// Estados do Sistema
enum EstadoLona { RECOLHIDA, ESTENDIDA, EM_MOVIMENTO };
EstadoLona estadoAtual = RECOLHIDA;

void setup() {
  pinMode(PIN_LED_SOLAR, OUTPUT);
  pinMode(PIN_MOTOR_A_1, OUTPUT);
  pinMode(PIN_MOTOR_A_2, OUTPUT);
  pinMode(PIN_MOTOR_B_1, OUTPUT);
  pinMode(PIN_MOTOR_B_2, OUTPUT);
  
  Serial.begin(9600);
}

void moverLona(bool estender) {
  // Para antes de mudar direção
  pararMotores();
  delay(500); 

  if (estender) {
    // Avança Lona (Cobre)
    digitalWrite(PIN_MOTOR_A_1, HIGH);
    digitalWrite(PIN_MOTOR_A_2, LOW);
    digitalWrite(PIN_MOTOR_B_1, HIGH);
    digitalWrite(PIN_MOTOR_B_2, LOW);
    Serial.println("STATUS: Estendendo Proteção...");
  } else {
    // Recua Lona (Descobre)
    digitalWrite(PIN_MOTOR_A_1, LOW);
    digitalWrite(PIN_MOTOR_A_2, HIGH);
    digitalWrite(PIN_MOTOR_B_1, LOW);
    digitalWrite(PIN_MOTOR_B_2, HIGH);
    Serial.println("STATUS: Recolhendo Proteção...");
  }
  
  // Simula tempo de curso do motor (5 segundos)
  delay(5000); 
  pararMotores();
  estadoAtual = estender ? ESTENDIDA : RECOLHIDA;
}

void pararMotores() {
  digitalWrite(PIN_MOTOR_A_1, LOW);
  digitalWrite(PIN_MOTOR_A_2, LOW);
  digitalWrite(PIN_MOTOR_B_1, LOW);
  digitalWrite(PIN_MOTOR_B_2, LOW);
}

void loop() {
  // 1. Leitura de Sensores
  int leituraLuz = analogRead(PIN_LDR);
  int leituraVento = analogRead(PIN_ANEMOMETRO);
  float velocidadeVento = leituraVento * FATOR_VENTO; // Converte para m/s

  // 2. Lógica de Simulação Solar (LED)
  if (leituraLuz > LIMITE_LUZ) {
    digitalWrite(PIN_LED_SOLAR, HIGH); // Gerando energia
  } else {
    digitalWrite(PIN_LED_SOLAR, LOW);  // Sem geração
  }

  // 3. Lógica de Proteção e Controle
  // Prioridade 1: Segurança (Vento Forte)
  if (velocidadeVento > VENTO_MAX) {
    if (estadoAtual != RECOLHIDA) {
      Serial.println("ALERTA: Vento Forte! Recolhendo emergencialmente.");
      moverLona(false); // Recolhe
    }
  } 
  // Prioridade 2: Automação Noturna (Sem luz = Cobre)
  else if (leituraLuz < LIMITE_LUZ) {
    if (estadoAtual != ESTENDIDA) {
      Serial.println("EVENTO: Anoiteceu. Ativando proteção.");
      moverLona(true); // Estende
    }
  }
  // Prioridade 3: Automação Diurna (Com luz = Descobre)
  else {
    if (estadoAtual != RECOLHIDA) {
      Serial.println("EVENTO: Amanheceu. Liberando painel.");
      moverLona(false); // Recolhe
    }
  }

  delay(1000); // Intervalo de verificação
}
