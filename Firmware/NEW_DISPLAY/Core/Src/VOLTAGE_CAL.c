#include "main.h"
#include "VOLTAGE_CAL.h"

/* Platform-specific ADC read function (user must implement) */
extern uint16_t read_adc(void);

/**
 * @brief Reads and averages multiple ADC samples.
 * @param samples Number of ADC samples to average.
 * @return The averaged ADC reading, or 0 if an error occurs.
 */
uint16_t ADC_Sample_Average(uint16_t samples) 
{
  if (samples == 0) 
  {
    return 0; /* Prevent division by zero */
  }
  
  uint32_t sum = 0;
  for (uint16_t i = 0; i < samples; i++) 
  {
    sum += read_adc(); /* Platform-specific ADC read */
  }
  
  return (uint16_t)(sum / samples);
}

/**
 * @brief Calculates the input DC voltage from averaged ADC readings.
 * @param ADC_RESOLUTION The ADC resolution (ADC_8B_RESOLUTION, ADC_10B_RESOLUTION, or ADC_12B_RESOLUTION).
 * @return The calculated input voltage in volts, or -1.0 if an error occurs (e.g., invalid resolution or ADC count).
 */
float DC_Voltage_Calibration(uint16_t ADC_RESOLUTION , uint16_t ADC_READ_COUNT) 
{
  static float Temp = 0;
  /* initialize the variable */
  uint16_t RESOLUTION_COUNT = 0;
    float ADC_VOLTAGE = 0;
    float INPUT_VOLTAGE = 0;

    /* Set resolution count */
    switch (ADC_RESOLUTION) 
    {
    case ADC_12B_RESOLUTION:RESOLUTION_COUNT = 4095;break;
    case ADC_10B_RESOLUTION:RESOLUTION_COUNT = 1023;break;
    case ADC_8B_RESOLUTION:RESOLUTION_COUNT = 255;  break;
    default:return -1.0;                                                        /* Invalid resolution */
    }

    /* Get averaged ADC reading */
    if (ADC_READ_COUNT == 0 && ADC_SAMPLES > 0){return -1.0;}

    /* Validate ADC read count */
    if (ADC_READ_COUNT > RESOLUTION_COUNT){return -1.0;}

    /* Calculate ADC voltage */
    float Resolution_val = (float)ADC_READ_COUNT / RESOLUTION_COUNT;
    ADC_VOLTAGE = ((Resolution_val * REFERENCE_VOLTAGE) - Temp);

    /* Reconstruct input voltage */
    if (R2 == 0) {return -1.0;}
    
    /* Calculate the voltage*/
    float voltage_devider = R1 + R2;
    INPUT_VOLTAGE = ADC_VOLTAGE *  (voltage_devider / R2);

    /* return the calculated value */
    return INPUT_VOLTAGE;
}

















