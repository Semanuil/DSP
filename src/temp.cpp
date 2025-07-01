#include <stdio.h>              // Standardna biblioteka za ulaz/izlaz
#include "freertos/FreeRTOS.h"  // FreeRTOS osnovne definicije
#include "freertos/task.h"      // FreeRTOS task funkcije
#include "driver/adc.h"         // Biblioteka za rad sa ADC-om

// ADC konfiguracija
#define ADC_CHANNEL ADC1_CHANNEL_6 // Koristi se kanal 6 (GPIO34) za očitavanje
#define SAMPLE_RATE 1000 // Uzorkovanje signala 1000 puta u sekundi (1000 Hz)

// Glavna funkcija
void app_main() {
    // Podešavanje rezolucije ADC-a na 12 bita
    adc1_config_width(ADC_WIDTH_BIT_12);

    // Podešavanje slabljenja (attenuation) za izabrani kanal
    adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN_DB_11);

    while (1) {
        // Očitavanje vrednosti sa definisanog pina
        int adc_raw = adc1_get_raw(ADC_CHANNEL);

        // Slanje očitane vrednosti na serijski port u formatu sa tri decimale
        printf("%.3f\n", adc_raw);

        // Pauza između uzoraka – određuje brzinu uzorkovanja
        vTaskDelay(pdMS_TO_TICKS(1000 / SAMPLE_RATE));
    }
}