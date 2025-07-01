#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_log.h"


// Definisanje ADC kanala; GPIO34 na ESP32 je povezan na ADC1_CHANNEL_6
#define ADC_CHANNEL ADC1_CHANNEL_6 
// Frekvencija uzorkovanja signala u Hz (10 uzoraka u sekundi)
#define SAMPLE_RATE 10             

// Red filtra — mora odgovarati onom iz MATLAB simulacije
#define FILTER_ORDER 3 

// Koeficijenti niza (b[]) iz MATLAB dizajna — ulazni uzorci
float b[] = {0.018099, 0.054297, 0.054297, 0.018099};
// težinski koeficijenti za izlazne uzorke (feedback)
float a[] = {1.000000, -1.760042, 1.182893, -0.278060};

// Bafer za ulazne uzorke (x[0] je najnoviji, x[1], x[2]... su stariji)
float x[FILTER_ORDER + 1] = {0};  // Inicijalizacija ulaznog niza na nule
// Bafer za izlazne uzorke (y[0] je najnoviji izlaz, y[1], y[2]... su prethodni)
float y[FILTER_ORDER + 1] = {0};  // Inicijalizacija izlaznog niza na nule

// Funkcija koja prima novi uzorak i vraća filtriranu vrednost
float filter_sample(float new_sample) {
    
    // Pomeranje prethodnih uzoraka unazad za jedno mesto 
    for (int i = FILTER_ORDER; i > 0; i--) {
        x[i] = x[i - 1];  // Pomeranje ulaznih uzoraka udesno
        y[i] = y[i - 1];  // Pomeranje izlaznih uzoraka udesno
    }

    // Dodavanje novog ulaznog uzorka na početak niza
    x[0] = new_sample;
    // Inicijalizacija nove izlazne vrednosti
    y[0] = 0;

    // Filtrirana vrednost (y[0]) se računa kao zbir ulaznih 
    // uzoraka (x[]) množenih sa b[] koeficijentima
    // i prethodnih izlaza (y[]) množenih sa a[] koeficijentima (bez a[0]).
    for (int i = 0; i <= FILTER_ORDER; i++) {
        // Sabiranje vrednosti trenutnog i prethodnih ulaza
        y[0] += b[i] * x[i]; 

        if (i > 0) {
            // Oduzimanje vrednosti prethodnih izlaza (bez a[0])
            y[0] -= a[i] * y[i]; 
        }
    }

    // Vraćanje novog filtriranog izlaza
    return y[0];
}

void app_main() {
    // Konfigurisanje ADC na 12 bita (vrednosti od 0 do 4095)
    adc1_config_width(ADC_WIDTH_BIT_12);

    // Postavljanje atenuacije za izabrani ADC kanal (11 dB ~ 3.3V)
    adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN_DB_11);

    // Beskonačna petlja - očitavanje vrednosti se izvršava neprekidno
    while (1) {
        // Čitanje sirove ADC vrednosti sa definisanog kanala (0–4095)
        int adc_raw = adc1_get_raw(ADC_CHANNEL);

        // Primena digitalnog filtra na očitanu vrednost
        float filtered_value = filter_sample(adc_raw);

        // Ispis filtrirane vrednosti na serijski terminal
        printf("%.3f\n", filtered_value);

        // Kašnjenje od 10 milisekundi (učestalost uzorkovanja ~ 100 Hz)
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
