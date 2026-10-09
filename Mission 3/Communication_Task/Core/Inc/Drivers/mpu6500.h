/**
 ******************************************************************************
 * @file    mpu6500.h
 * @brief   Header file for MPU-6500 6-Axis MotionTracking IMU Driver.
 *          Supports flexible I2C channel and pin selection (Hardware I2C with
 *          custom GPIO mapping or Bit-Banged Software I2C).
 * @author  Antigravity
 ******************************************************************************
 */

#ifndef __MPU6500_H
#define __MPU6500_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"
#include <stdbool.h>
#include <stdint.h>

/* Exported Constants --------------------------------------------------------*/

/**
 * @defgroup MPU6500_I2C_Addresses MPU6500 Default I2C Addresses (7-bit)
 * @{
 */
#define MPU6500_I2C_ADDR_AD0_LOW        (0x68U) /*!< AD0 Pin connected to GND */
#define MPU6500_I2C_ADDR_AD0_HIGH       (0x69U) /*!< AD0 Pin connected to VCC */
/** @} */

/**
 * @defgroup MPU6500_Device_IDs Expected WHO_AM_I Values
 * @{
 */
#define MPU6500_WHO_AM_I_VAL            (0x70U) /*!< Standard MPU6500 ID */
#define MPU9250_WHO_AM_I_VAL            (0x71U) /*!< MPU9250 (contains MPU6500 core) */
#define MPU6555_WHO_AM_I_VAL            (0x75U) /*!< MPU6555 ID */
/** @} */

/**
 * @defgroup MPU6500_Registers MPU-6500 Register Map
 * @{
 */
#define MPU6500_REG_SELF_TEST_X_GYRO    (0x00U)
#define MPU6500_REG_SELF_TEST_Y_GYRO    (0x01U)
#define MPU6500_REG_SELF_TEST_Z_GYRO    (0x02U)
#define MPU6500_REG_SELF_TEST_X_ACCEL   (0x0DU)
#define MPU6500_REG_SELF_TEST_Y_ACCEL   (0x0EU)
#define MPU6500_REG_SELF_TEST_Z_ACCEL   (0x0FU)
#define MPU6500_REG_XG_OFFSET_H         (0x13U)
#define MPU6500_REG_XG_OFFSET_L         (0x14U)
#define MPU6500_REG_YG_OFFSET_H         (0x15U)
#define MPU6500_REG_YG_OFFSET_L         (0x16U)
#define MPU6500_REG_ZG_OFFSET_H         (0x17U)
#define MPU6500_REG_ZG_OFFSET_L         (0x18U)
#define MPU6500_REG_SMPLRT_DIV          (0x19U)
#define MPU6500_REG_CONFIG              (0x1AU)
#define MPU6500_REG_GYRO_CONFIG         (0x1BU)
#define MPU6500_REG_ACCEL_CONFIG        (0x1CU)
#define MPU6500_REG_ACCEL_CONFIG2       (0x1DU)
#define MPU6500_REG_LP_ACCEL_ODR        (0x1EU)
#define MPU6500_REG_WOM_THR             (0x1FU)
#define MPU6500_REG_FIFO_EN             (0x23U)
#define MPU6500_REG_I2C_MST_CTRL        (0x24U)
#define MPU6500_REG_INT_PIN_CFG         (0x37U)
#define MPU6500_REG_INT_ENABLE          (0x38U)
#define MPU6500_REG_INT_STATUS          (0x3AU)
#define MPU6500_REG_ACCEL_XOUT_H        (0x3BU)
#define MPU6500_REG_ACCEL_XOUT_L        (0x3CU)
#define MPU6500_REG_ACCEL_YOUT_H        (0x3DU)
#define MPU6500_REG_ACCEL_YOUT_L        (0x3EU)
#define MPU6500_REG_ACCEL_ZOUT_H        (0x3FU)
#define MPU6500_REG_ACCEL_ZOUT_L        (0x40U)
#define MPU6500_REG_TEMP_OUT_H          (0x41U)
#define MPU6500_REG_TEMP_OUT_L          (0x42U)
#define MPU6500_REG_GYRO_XOUT_H         (0x43U)
#define MPU6500_REG_GYRO_XOUT_L         (0x44U)
#define MPU6500_REG_GYRO_YOUT_H         (0x45U)
#define MPU6500_REG_GYRO_YOUT_L         (0x46U)
#define MPU6500_REG_GYRO_ZOUT_H         (0x47U)
#define MPU6500_REG_GYRO_ZOUT_L         (0x48U)
#define MPU6500_REG_SIGNAL_PATH_RESET   (0x68U)
#define MPU6500_REG_ACCEL_INTEL_CTRL    (0x69U)
#define MPU6500_REG_USER_CTRL           (0x6AU)
#define MPU6500_REG_PWR_MGMT_1          (0x6BU)
#define MPU6500_REG_PWR_MGMT_2          (0x6CU)
#define MPU6500_REG_FIFO_COUNTH         (0x72U)
#define MPU6500_REG_FIFO_COUNTL         (0x73U)
#define MPU6500_REG_FIFO_R_W            (0x74U)
#define MPU6500_REG_WHO_AM_I            (0x75U)
#define MPU6500_REG_XA_OFFSET_H         (0x77U)
#define MPU6500_REG_XA_OFFSET_L         (0x78U)
#define MPU6500_REG_YA_OFFSET_H         (0x7AU)
#define MPU6500_REG_YA_OFFSET_L         (0x7BU)
#define MPU6500_REG_ZA_OFFSET_H         (0x7DU)
#define MPU6500_REG_ZA_OFFSET_L         (0x7EU)
/** @} */

/* Exported Types ------------------------------------------------------------*/

/**
 * @brief Communication Interface Type
 */
typedef enum {
    MPU6500_BUS_HARDWARE_I2C = 0,   /*!< STM32 Hardware I2C Peripheral */
    MPU6500_BUS_SOFTWARE_I2C = 1,   /*!< Bit-banged Software I2C on any GPIO pins */
    MPU6500_BUS_EXISTING_HANDLE = 2 /*!< Pre-configured HAL I2C Handle */
} MPU6500_BusType_e;

/**
 * @brief Status and Error Return Codes
 */
typedef enum {
    MPU6500_OK       = 0x00,
    MPU6500_ERROR    = 0x01,
    MPU6500_TIMEOUT  = 0x02,
    MPU6500_NOT_FOUND = 0x03,
    MPU6500_INVALID_PARAM = 0x04
} MPU6500_Status_e;

/**
 * @brief Accelerometer Full-Scale Range
 */
typedef enum {
    MPU6500_ACCEL_FS_2G  = 0x00,    /*!< ±2g  (16384 LSB/g) */
    MPU6500_ACCEL_FS_4G  = 0x08,    /*!< ±4g  (8192 LSB/g)  */
    MPU6500_ACCEL_FS_8G  = 0x10,    /*!< ±8g  (4096 LSB/g)  */
    MPU6500_ACCEL_FS_16G = 0x18     /*!< ±16g (2048 LSB/g)  */
} MPU6500_AccelScale_e;

/**
 * @brief Gyroscope Full-Scale Range
 */
typedef enum {
    MPU6500_GYRO_FS_250DPS  = 0x00, /*!< ±250 deg/s  (131 LSB/dps)  */
    MPU6500_GYRO_FS_500DPS  = 0x08, /*!< ±500 deg/s  (65.5 LSB/dps) */
    MPU6500_GYRO_FS_1000DPS = 0x10, /*!< ±1000 deg/s (32.8 LSB/dps) */
    MPU6500_GYRO_FS_2000DPS = 0x18  /*!< ±2000 deg/s (16.4 LSB/dps) */
} MPU6500_GyroScale_e;

/**
 * @brief Digital Low Pass Filter (DLPF) Bandwidth
 */
typedef enum {
    MPU6500_DLPF_250HZ = 0x00,      /*!< Accel BW 218Hz, Gyro BW 250Hz, Delay ~0.97ms */
    MPU6500_DLPF_184HZ = 0x01,      /*!< Accel BW 218Hz, Gyro BW 184Hz, Delay ~2.9ms  */
    MPU6500_DLPF_92HZ  = 0x02,      /*!< Accel BW 99Hz,  Gyro BW 92Hz,  Delay ~3.9ms  */
    MPU6500_DLPF_41HZ  = 0x03,      /*!< Accel BW 44Hz,  Gyro BW 41Hz,  Delay ~5.9ms  */
    MPU6500_DLPF_20HZ  = 0x04,      /*!< Accel BW 21Hz,  Gyro BW 20Hz,  Delay ~9.9ms  */
    MPU6500_DLPF_10HZ  = 0x05,      /*!< Accel BW 10Hz,  Gyro BW 10Hz,  Delay ~17.85ms*/
    MPU6500_DLPF_5HZ   = 0x06       /*!< Accel BW 5Hz,   Gyro BW 5Hz,   Delay ~33.48ms*/
} MPU6500_DLPF_e;

/**
 * @brief Power Management Clock Source
 */
typedef enum {
    MPU6500_CLOCK_INTERNAL_20MHZ = 0x00, /*!< Internal 20MHz oscillator */
    MPU6500_CLOCK_AUTO_SELECT    = 0x01, /*!< Auto selects best available clock (PLL) */
    MPU6500_CLOCK_STOP           = 0x07  /*!< Stops clock and keeps timing generator in reset */
} MPU6500_ClockSource_e;

/**
 * @brief Low Power Accelerometer Output Data Rate (ODR)
 */
typedef enum {
    MPU6500_LP_ACCEL_0_24HZ = 0x00,
    MPU6500_LP_ACCEL_0_49HZ = 0x01,
    MPU6500_LP_ACCEL_0_98HZ = 0x02,
    MPU6500_LP_ACCEL_1_95HZ = 0x03,
    MPU6500_LP_ACCEL_3_91HZ = 0x04,
    MPU6500_LP_ACCEL_7_81HZ = 0x05,
    MPU6500_LP_ACCEL_15_63HZ = 0x06,
    MPU6500_LP_ACCEL_31_25HZ = 0x07,
    MPU6500_LP_ACCEL_62_50HZ = 0x08,
    MPU6500_LP_ACCEL_125HZ   = 0x09,
    MPU6500_LP_ACCEL_250HZ   = 0x0A,
    MPU6500_LP_ACCEL_500HZ   = 0x0B
} MPU6500_LowPowerAccelODR_e;

/**
 * @brief Interrupt Pin & Signal Configuration
 */
typedef struct {
    bool active_low;          /*!< false: Active High, true: Active Low */
    bool open_drain;          /*!< false: Push-pull, true: Open-drain */
    bool latch_until_cleared; /*!< false: 50us pulse, true: Latch until INT_STATUS read */
    bool clear_on_any_read;   /*!< false: Clear only on INT_STATUS read, true: Clear on any read */
    bool enable_data_ready;   /*!< Enable Data Ready Interrupt */
    bool enable_wom;          /*!< Enable Wake on Motion Interrupt */
    bool enable_fifo_overflow;/*!< Enable FIFO Overflow Interrupt */
} MPU6500_IntConfig_t;

/**
 * @brief Raw Sensor Readings (16-bit ADC values)
 */
typedef struct {
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;
    int16_t temp_raw;
    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
} MPU6500_RawData_t;

/**
 * @brief Scaled Engineering Units Data
 */
typedef struct {
    /* Accelerometer data (g or m/s²) */
    float accel_x_g;          /*!< X acceleration in 'g' */
    float accel_y_g;          /*!< Y acceleration in 'g' */
    float accel_z_g;          /*!< Z acceleration in 'g' */
    float accel_x_ms2;        /*!< X acceleration in m/s² */
    float accel_y_ms2;        /*!< Y acceleration in m/s² */
    float accel_z_ms2;        /*!< Z acceleration in m/s² */

    /* Gyroscope data (deg/s or rad/s) */
    float gyro_x_dps;         /*!< X angular rate in deg/s */
    float gyro_y_dps;         /*!< Y angular rate in deg/s */
    float gyro_z_dps;         /*!< Z angular rate in deg/s */
    float gyro_x_rads;        /*!< X angular rate in rad/s */
    float gyro_y_rads;        /*!< Y angular rate in rad/s */
    float gyro_z_rads;        /*!< Z angular rate in rad/s */

    /* Temperature */
    float temp_c;             /*!< Temperature in °Celsius */
} MPU6500_ScaledData_t;

/**
 * @brief Combined Raw and Scaled Data Container
 */
typedef struct {
    MPU6500_RawData_t    raw;
    MPU6500_ScaledData_t scaled;
} MPU6500_AllData_t;

/**
 * @brief Calibration Offsets
 */
typedef struct {
    float accel_offset_x;
    float accel_offset_y;
    float accel_offset_z;
    float gyro_offset_x;
    float gyro_offset_y;
    float gyro_offset_z;
} MPU6500_Offsets_t;

/**
 * @brief Hardware / Software Pin Definition Struct
 */
typedef struct {
    MPU6500_BusType_e   bus_type;      /*!< Bus mode (HW I2C, SW Bit-bang, or Handle) */
    I2C_HandleTypeDef   *hi2c;         /*!< Pointer to HAL I2C Handle (used in HW mode) */
    I2C_TypeDef         *i2c_instance; /*!< HW I2C Peripheral (I2C1, I2C2, I2C3) */
    GPIO_TypeDef        *scl_port;     /*!< GPIO Port for SCL (e.g. GPIOB) */
    uint16_t            scl_pin;       /*!< GPIO Pin for SCL (e.g. GPIO_PIN_6, GPIO_PIN_8) */
    GPIO_TypeDef        *sda_port;     /*!< GPIO Port for SDA (e.g. GPIOB) */
    uint16_t            sda_pin;       /*!< GPIO Pin for SDA (e.g. GPIO_PIN_7, GPIO_PIN_9) */
    uint8_t             alternate_func;/*!< GPIO AF (e.g. GPIO_AF4_I2C1) or 0 for auto */
    uint32_t            clock_speed;   /*!< I2C Clock Speed in Hz (e.g. 100000 or 400000) */
    uint8_t             i2c_addr;      /*!< 7-bit I2C Address (0x68 or 0x69) */
} MPU6500_PinConfig_t;

/**
 * @brief MPU-6500 Device Handle
 */
typedef struct {
    MPU6500_PinConfig_t   pins;          /*!< Communication and pin configuration */
    I2C_HandleTypeDef     internal_hi2c; /*!< Internal HAL I2C handle if initialized dynamically */
    MPU6500_AccelScale_e  accel_scale;   /*!< Current Accel scale */
    MPU6500_GyroScale_e   gyro_scale;    /*!< Current Gyro scale */
    MPU6500_DLPF_e        dlpf;          /*!< Current DLPF filter setting */
    float                 accel_lsb_to_g;/*!< Conversion factor LSB -> g */
    float                 gyro_lsb_to_dps;/*!< Conversion factor LSB -> dps */
    MPU6500_Offsets_t     offsets;       /*!< Calibration offsets */
    bool                  is_initialized;/*!< Initialization flag */
} MPU6500_t;

/* Exported Functions --------------------------------------------------------*/

/**
 * @brief  Initialize MPU-6500 with dynamic Hardware I2C and custom GPIO pin selection.
 * @param  dev: Pointer to MPU6500 handle.
 * @param  i2c_instance: I2C Peripheral instance (e.g., I2C1, I2C2, I2C3).
 * @param  scl_port: GPIO Port for SCL (e.g. GPIOB).
 * @param  scl_pin: GPIO Pin for SCL (e.g. GPIO_PIN_6 or GPIO_PIN_8).
 * @param  sda_port: GPIO Port for SDA (e.g. GPIOB).
 * @param  sda_pin: GPIO Pin for SDA (e.g. GPIO_PIN_7 or GPIO_PIN_9).
 * @param  clock_speed: Bus speed in Hz (e.g. 100000 for standard, 400000 for fast mode).
 * @param  i2c_addr: 7-bit I2C address (MPU6500_I2C_ADDR_AD0_LOW or MPU6500_I2C_ADDR_AD0_HIGH).
 * @retval MPU6500_Status_e: MPU6500_OK on success, error code otherwise.
 */
MPU6500_Status_e MPU6500_InitHardwareI2C(MPU6500_t *dev,
                                         I2C_TypeDef *i2c_instance,
                                         GPIO_TypeDef *scl_port,
                                         uint16_t scl_pin,
                                         GPIO_TypeDef *sda_port,
                                         uint16_t sda_pin,
                                         uint32_t clock_speed,
                                         uint8_t i2c_addr);

/**
 * @brief  Initialize MPU-6500 using an already initialized HAL I2C Handle.
 * @param  dev: Pointer to MPU6500 handle.
 * @param  hi2c: Pointer to STM32 HAL I2C Handle.
 * @param  i2c_addr: 7-bit I2C address (0x68 or 0x69).
 * @retval MPU6500_Status_e: MPU6500_OK on success, error code otherwise.
 */
MPU6500_Status_e MPU6500_InitWithHandle(MPU6500_t *dev,
                                        I2C_HandleTypeDef *hi2c,
                                        uint8_t i2c_addr);

/**
 * @brief  Initialize MPU-6500 using Software Bit-Banged I2C on ANY arbitrary GPIO pins.
 * @param  dev: Pointer to MPU6500 handle.
 * @param  scl_port: GPIO Port for SCL.
 * @param  scl_pin: GPIO Pin for SCL.
 * @param  sda_port: GPIO Port for SDA.
 * @param  sda_pin: GPIO Pin for SDA.
 * @param  i2c_addr: 7-bit I2C address (0x68 or 0x69).
 * @retval MPU6500_Status_e: MPU6500_OK on success, error code otherwise.
 */
MPU6500_Status_e MPU6500_InitSoftwareI2C(MPU6500_t *dev,
                                         GPIO_TypeDef *scl_port,
                                         uint16_t scl_pin,
                                         GPIO_TypeDef *sda_port,
                                         uint16_t sda_pin,
                                         uint8_t i2c_addr);

/**
 * @brief  Complete default sensor startup sequence (Wake up, set clock, set default scales & DLPF).
 * @param  dev: Pointer to MPU6500 handle.
 * @retval MPU6500_Status_e: MPU6500_OK on success.
 */
MPU6500_Status_e MPU6500_Init(MPU6500_t *dev);

/**
 * @brief  Software Reset the MPU-6500 sensor.
 * @param  dev: Pointer to MPU6500 handle.
 * @retval MPU6500_Status_e.
 */
MPU6500_Status_e MPU6500_Reset(MPU6500_t *dev);

/**
 * @brief  Check if device responds and WHO_AM_I matches MPU6500 ID.
 * @param  dev: Pointer to MPU6500 handle.
 * @retval bool: true if connected, false otherwise.
 */
bool MPU6500_IsConnected(MPU6500_t *dev);

/**
 * @brief  Read the WHO_AM_I register value directly.
 * @param  dev: Pointer to MPU6500 handle.
 * @param  who_am_i: Pointer to store the register value.
 * @retval MPU6500_Status_e.
 */
MPU6500_Status_e MPU6500_ReadWhoAmI(MPU6500_t *dev, uint8_t *who_am_i);

/* Sensor Configuration APIs -------------------------------------------------*/

/**
 * @brief  Set Accelerometer Full Scale Range.
 * @param  dev: Pointer to MPU6500 handle.
 * @param  scale: ±2g, ±4g, ±8g, or ±16g.
 * @retval MPU6500_Status_e.
 */
MPU6500_Status_e MPU6500_SetAccelFullScale(MPU6500_t *dev, MPU6500_AccelScale_e scale);

/**
 * @brief  Set Gyroscope Full Scale Range.
 * @param  dev: Pointer to MPU6500 handle.
 * @param  scale: ±250, ±500, ±1000, or ±2000 dps.
 * @retval MPU6500_Status_e.
 */
MPU6500_Status_e MPU6500_SetGyroFullScale(MPU6500_t *dev, MPU6500_GyroScale_e scale);

/**
 * @brief  Set Sample Rate Divider (Sample Rate = Internal_Sample_Rate / (1 + div)).
 * @param  dev: Pointer to MPU6500 handle.
 * @param  div: 0 to 255.
 * @retval MPU6500_Status_e.
 */
MPU6500_Status_e MPU6500_SetSampleRateDivider(MPU6500_t *dev, uint8_t div);

/**
 * @brief  Set Digital Low Pass Filter (DLPF) Bandwidth.
 * @param  dev: Pointer to MPU6500 handle.
 * @param  dlpf: DLPF frequency enum.
 * @retval MPU6500_Status_e.
 */
MPU6500_Status_e MPU6500_SetDLPFBandwidth(MPU6500_t *dev, MPU6500_DLPF_e dlpf);

/**
 * @brief  Configure Power Clock Source.
 * @param  dev: Pointer to MPU6500 handle.
 * @param  clock_src: Clock source selection.
 * @retval MPU6500_Status_e.
 */
MPU6500_Status_e MPU6500_SetClockSource(MPU6500_t *dev, MPU6500_ClockSource_e clock_src);

/**
 * @brief  Enable or Disable Sleep Mode.
 * @param  dev: Pointer to MPU6500 handle.
 * @param  enable: true = Sleep, false = Active.
 * @retval MPU6500_Status_e.
 */
MPU6500_Status_e MPU6500_SetSleepMode(MPU6500_t *dev, bool enable);

/**
 * @brief  Configure Interrupt Output Pin and Interrupt Sources.
 * @param  dev: Pointer to MPU6500 handle.
 * @param  int_cfg: Pointer to interrupt configuration struct.
 * @retval MPU6500_Status_e.
 */
MPU6500_Status_e MPU6500_ConfigureInterrupt(MPU6500_t *dev, const MPU6500_IntConfig_t *int_cfg);

/**
 * @brief  Read the Interrupt Status Register (Clears latch if configured).
 * @param  dev: Pointer to MPU6500 handle.
 * @param  status: Pointer to store interrupt status byte.
 * @retval MPU6500_Status_e.
 */
MPU6500_Status_e MPU6500_GetInterruptStatus(MPU6500_t *dev, uint8_t *status);

/* Data Acquisition APIs -----------------------------------------------------*/

/**
 * @brief  Read Raw Accelerometer data (X, Y, Z).
 * @param  dev: Pointer to MPU6500 handle.
 * @param  ax, ay, az: Output pointers for raw 16-bit values.
 * @retval MPU6500_Status_e.
 */
MPU6500_Status_e MPU6500_ReadRawAccel(MPU6500_t *dev, int16_t *ax, int16_t *ay, int16_t *az);

/**
 * @brief  Read Raw Gyroscope data (X, Y, Z).
 * @param  dev: Pointer to MPU6500 handle.
 * @param  gx, gy, gz: Output pointers for raw 16-bit values.
 * @retval MPU6500_Status_e.
 */
MPU6500_Status_e MPU6500_ReadRawGyro(MPU6500_t *dev, int16_t *gx, int16_t *gy, int16_t *gz);

/**
 * @brief  Read Raw Temperature data.
 * @param  dev: Pointer to MPU6500 handle.
 * @param  temp_raw: Output pointer for raw temperature value.
 * @retval MPU6500_Status_e.
 */
MPU6500_Status_e MPU6500_ReadRawTemp(MPU6500_t *dev, int16_t *temp_raw);

/**
 * @brief  Perform an atomic 14-byte burst read of all raw data registers.
 * @param  dev: Pointer to MPU6500 handle.
 * @param  raw: Output pointer to MPU6500_RawData_t struct.
 * @retval MPU6500_Status_e.
 */
MPU6500_Status_e MPU6500_ReadAllRaw(MPU6500_t *dev, MPU6500_RawData_t *raw);

/**
 * @brief  Read Scaled Accelerometer data in 'g' (gravity units).
 * @param  dev: Pointer to MPU6500 handle.
 * @param  ax, ay, az: Output pointers for acceleration in g.
 * @retval MPU6500_Status_e.
 */
MPU6500_Status_e MPU6500_ReadAccel(MPU6500_t *dev, float *ax, float *ay, float *az);

/**
 * @brief  Read Scaled Gyroscope data in degrees per second (°/s).
 * @param  dev: Pointer to MPU6500 handle.
 * @param  gx, gy, gz: Output pointers for angular rate in dps.
 * @retval MPU6500_Status_e.
 */
MPU6500_Status_e MPU6500_ReadGyro(MPU6500_t *dev, float *gx, float *gy, float *gz);

/**
 * @brief  Read Scaled Temperature in degrees Celsius (°C).
 * @param  dev: Pointer to MPU6500 handle.
 * @param  temp_c: Output pointer for temperature in °C.
 * @retval MPU6500_Status_e.
 */
MPU6500_Status_e MPU6500_ReadTemp(MPU6500_t *dev, float *temp_c);

/**
 * @brief  Read and calculate all sensor channels in engineering units (g, m/s², dps, rad/s, °C).
 * @param  dev: Pointer to MPU6500 handle.
 * @param  data: Output pointer to MPU6500_ScaledData_t.
 * @retval MPU6500_Status_e.
 */
MPU6500_Status_e MPU6500_ReadAll(MPU6500_t *dev, MPU6500_ScaledData_t *data);

/**
 * @brief  Read and return both raw 16-bit values and calibrated/scaled physical quantities.
 * @param  dev: Pointer to MPU6500 handle.
 * @param  data: Output pointer to MPU6500_AllData_t.
 * @retval MPU6500_Status_e.
 */
MPU6500_Status_e MPU6500_ReadAllData(MPU6500_t *dev, MPU6500_AllData_t *data);

/* Calibration & Offset APIs -------------------------------------------------*/

/**
 * @brief  Calibrate Gyroscope by taking average over N stationary samples and storing offsets.
 * @param  dev: Pointer to MPU6500 handle.
 * @param  num_samples: Number of samples to average (e.g. 500 - 1000).
 * @retval MPU6500_Status_e.
 */
MPU6500_Status_e MPU6500_CalibrateGyro(MPU6500_t *dev, uint16_t num_samples);

/**
 * @brief  Calibrate Accelerometer assuming Z-axis is pointing UP (1g) on flat surface.
 * @param  dev: Pointer to MPU6500 handle.
 * @param  num_samples: Number of samples to average (e.g. 500 - 1000).
 * @retval MPU6500_Status_e.
 */
MPU6500_Status_e MPU6500_CalibrateAccel(MPU6500_t *dev, uint16_t num_samples);

/**
 * @brief  Set user defined offsets manually.
 * @param  dev: Pointer to MPU6500 handle.
 * @param  offsets: Pointer to offsets struct.
 */
void MPU6500_SetOffsets(MPU6500_t *dev, const MPU6500_Offsets_t *offsets);

/**
 * @brief  Get current calibration offsets.
 * @param  dev: Pointer to MPU6500 handle.
 * @param  offsets: Pointer to store offsets.
 */
void MPU6500_GetOffsets(const MPU6500_t *dev, MPU6500_Offsets_t *offsets);

/* FIFO and Advanced APIs ----------------------------------------------------*/

/**
 * @brief  Enable or Disable FIFO buffer channels.
 * @param  dev: Pointer to MPU6500 handle.
 * @param  fifo_flags: Bitwise OR of flags (e.g. 0x78 for Accel + Gyro).
 * @retval MPU6500_Status_e.
 */
MPU6500_Status_e MPU6500_FIFO_Enable(MPU6500_t *dev, uint8_t fifo_flags);

/**
 * @brief  Get current byte count in FIFO buffer.
 * @param  dev: Pointer to MPU6500 handle.
 * @param  count: Pointer to store byte count.
 * @retval MPU6500_Status_e.
 */
MPU6500_Status_e MPU6500_FIFO_GetCount(MPU6500_t *dev, uint16_t *count);

/**
 * @brief  Read data block from FIFO.
 * @param  dev: Pointer to MPU6500 handle.
 * @param  buffer: Buffer to store FIFO data.
 * @param  length: Number of bytes to read.
 * @retval MPU6500_Status_e.
 */
MPU6500_Status_e MPU6500_FIFO_Read(MPU6500_t *dev, uint8_t *buffer, uint16_t length);

/**
 * @brief  Reset the FIFO buffer.
 * @param  dev: Pointer to MPU6500 handle.
 * @retval MPU6500_Status_e.
 */
MPU6500_Status_e MPU6500_FIFO_Reset(MPU6500_t *dev);

/**
 * @brief  Configure Wake on Motion (WOM) interrupt.
 * @param  dev: Pointer to MPU6500 handle.
 * @param  threshold_mg: Motion threshold in mg (e.g. 4mg/LSB).
 * @param  odr: Low power accelerometer ODR.
 * @retval MPU6500_Status_e.
 */
MPU6500_Status_e MPU6500_EnableWakeOnMotion(MPU6500_t *dev, uint8_t threshold_mg, MPU6500_LowPowerAccelODR_e odr);

/* Low-level Register Read/Write Helpers -------------------------------------*/

/**
 * @brief  Write single 8-bit register.
 */
MPU6500_Status_e MPU6500_WriteRegister(MPU6500_t *dev, uint8_t reg_addr, uint8_t data);

/**
 * @brief  Read single 8-bit register.
 */
MPU6500_Status_e MPU6500_ReadRegister(MPU6500_t *dev, uint8_t reg_addr, uint8_t *data);

/**
 * @brief  Read multiple sequential registers.
 */
MPU6500_Status_e MPU6500_ReadRegisters(MPU6500_t *dev, uint8_t reg_addr, uint8_t *data, uint16_t length);

#ifdef __cplusplus
}
#endif

#endif /* __MPU6500_H */
