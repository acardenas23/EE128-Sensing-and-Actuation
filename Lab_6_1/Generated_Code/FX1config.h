/**
 * \file
 * \brief Configuration header file for FXOS8700CQ accelerometer
 *
 * This header file is used to configure settings of the FXOS8700CQ module.
 */

#ifndef __FX1_CONFIG_H
#define __FX1_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(FX1_CONFIG_I2C_DEVICE_ADDRESS)
  #define FX1_CONFIG_I2C_DEVICE_ADDRESS  (0x1D)
    /*!< I2C 7bit device address */
    /*       SDA0  SDA1
       0x1E  low   low
       0x1D  high  low
       0x1C  low   high
       0x1F  high  high
    */
#endif

#if !defined(FX1_CONFIG_TEMP_OFFSET)
  #define FX1_CONFIG_TEMP_OFFSET  (24)
    /*!< die temperature offset for real temperature calculation */
#endif


#if !defined(FX1_CONFIG_PARSE_COMMAND_ENABLED)
  #define FX1_CONFIG_PARSE_COMMAND_ENABLED  0
    /*!< 1: shell support enabled, 0: otherwise */
#endif

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* __FX1_CONFIG_H */
