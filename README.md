# Solar Panel Smart Protection System

Sistema embarcado desenvolvido em C++ (Arduino) para automação e proteção de painéis solares em plataformas offshore. O projeto visa aumentar a durabilidade do equipamento protegendo-o automaticamente contra intempéries e controlando a exposição.

## ⚙️ Funcionalidades
* **Proteção Ativa:** Monitoramento constante da velocidade do vento via anemômetro.
* **Safety Lock:** Recolhimento automático e imediato da lona de proteção caso o vento ultrapasse 30 m/s.
* **Ciclo Dia/Noite:** Algoritmo baseado em LDR para estender a proteção durante a noite e recolher durante o dia, maximizando a eficiência operacional.
* **Simulação de Status:** Feedback visual (LEDs) simulando o estado de geração de energia das células fotovoltaicas.

## 🛠 Hardware Simulado
* Microcontrolador: Arduino Uno / Nano (ATmega328P)
* Atuadores: 2x Motores DC (Ponte H) para tracionar a lona mecânica.
* Sensores: Anemômetro (Analógico) e LDR (Luminosidade).
