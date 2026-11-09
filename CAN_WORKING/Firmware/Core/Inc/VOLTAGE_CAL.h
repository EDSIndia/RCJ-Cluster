/******************** (C) COPYRIGHT 2022 EDS INDIA *****************************
* File Name     : VOLTAGE_CAL.h         				       *
* Author        : SAHIL ALI                                                    *
* Date          : 05-02-2022                                                   *
* Description   : This file contains all define and extern varibales use in    *
                  voltage calibration                                          *
* Revision	: Rev0    						       *
********************************************************************************/

#ifndef DC_VOLTAGE_H
#define DC_VOLTAGE_H

/* ADC resolution constants */
#define ADC_8B_RESOLUTION  8
#define ADC_10B_RESOLUTION 10
#define ADC_12B_RESOLUTION 12

/* Voltage divider and reference voltage constants */
#define REFERENCE_VOLTAGE 3.3                                                   /* ADC reference voltage in volts (e.g., 3.3V) */
#define R1 150000                                                                /* Voltage divider resistor R1 in ohms */
#define R2 5600                                                                /* Voltage divider resistor R2 in ohms */

#define OFFSET 0.9

/* Sampling configuration */
#define ADC_SAMPLES 10                                                          /* Number of ADC samples to average */

/**
 * @brief Calculates the input DC voltage from averaged ADC readings.
 * @param ADC_RESOLUTION The ADC resolution (ADC_8B_RESOLUTION, ADC_10B_RESOLUTION, or ADC_12B_RESOLUTION).
 * @return The calculated input voltage in volts, or -1.0 if an error occurs (e.g., invalid resolution or ADC count).
 */
float DC_Voltage_Calibration(uint16_t ADC_RESOLUTION , uint16_t ADC_READ_COUNT);

/**
 * @brief Reads and averages multiple ADC samples.
 * @param samples Number of ADC samples to average.
 * @return The averaged ADC reading, or 0 if an error occurs.
 */
uint16_t ADC_Sample_Average(uint16_t samples);

#endif /* DC_VOLTAGE_H */