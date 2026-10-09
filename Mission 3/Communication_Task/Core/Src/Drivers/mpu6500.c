/**
 ******************************************************************************
 * @file    mpu6500.c
 * @brief   Source file for MPU-6500 6-Axis MotionTracking IMU Driver.
 *          Supports customizable I2C channels and GPIO pins (Hardware & Software).
 * @author  Antigravity
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "Drivers/mpu6500.h"
#include <string.h>

/* Private Constants & Macros ------------------------------------------------*/
#define MPU6500_TIMEOUT_DEFAULT_MS     (100U)
#define CONST_GRAVITY_EARTH            (9.80665f)
#define DEG_TO_RAD                     (0.017453292519943295f)

/* Private Function Prototypes -----------------------------------------------*/
static void GPIO_Clock_Enable(GPIO_TypeDef *port);
static void I2C_Clock_Enable(I2C_TypeDef *instance);
static uint8_t Auto_Detect_AF(I2C_TypeDef *instance);

/* Software Bit-Bang I2C Functions */
static void SW_I2C_Delay(void);
static void SW_I2C_Start(const MPU6500_PinConfig_t *pins);
static void SW_I2C_Stop(const MPU6500_PinConfig_t *pins);
static bool SW_I2C_WriteByte(const MPU6500_PinConfig_t *pins, uint8_t byte);
static uint8_t SW_I2C_ReadByte(const MPU6500_PinConfig_t *pins, bool ack);

/* Exported Functions --------------------------------------------------------*/

/**
 * @brief  Initialize MPU-6500 with dynamic Hardware I2C and custom GPIO pin selection.
 */
MPU6500_Status_e MPU6500_InitHardwareI2C(MPU6500_t *dev,
                                         I2C_TypeDef *i2c_instance,
                                         GPIO_TypeDef *scl_port,
                                         uint16_t scl_pin,
                                         GPIO_TypeDef *sda_port,
                                         uint16_t sda_pin,
                                         uint32_t clock_speed,
                                         uint8_t i2c_addr)
{
    if (dev == NULL || i2c_instance == NULL || scl_port == NULL || sda_port == NULL) {
        return MPU6500_INVALID_PARAM;
    }

    memset(dev, 0, sizeof(MPU6500_t));

    dev->pins.bus_type     = MPU6500_BUS_HARDWARE_I2C;
    dev->pins.i2c_instance = i2c_instance;
    dev->pins.scl_port     = scl_port;
    dev->pins.scl_pin      = scl_pin;
    dev->pins.sda_port     = sda_port;
    dev->pins.sda_pin      = sda_pin;
    dev->pins.clock_speed  = (clock_speed == 0) ? 100000U : clock_speed;
    dev->pins.i2c_addr     = (i2c_addr == 0) ? MPU6500_I2C_ADDR_AD0_LOW : i2c_addr;
    dev->pins.alternate_func = Auto_Detect_AF(i2c_instance);

    /* 1. Enable GPIO Clocks */
    GPIO_Clock_Enable(scl_port);
    GPIO_Clock_Enable(sda_port);

    /* 2. Configure SCL & SDA GPIO Pins in Alternate Function Open-Drain mode with Pull-Ups */
    GPIO_InitTypeDef gpio_init = {0};
    gpio_init.Mode      = GPIO_MODE_AF_OD;
    gpio_init.Pull      = GPIO_PULLUP;
    gpio_init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_init.Alternate = dev->pins.alternate_func;

    gpio_init.Pin       = scl_pin;
    HAL_GPIO_Init(scl_port, &gpio_init);

    gpio_init.Pin       = sda_pin;
    HAL_GPIO_Init(sda_port, &gpio_init);

    /* 3. Enable I2C Peripheral Clock */
    I2C_Clock_Enable(i2c_instance);

    /* 4. Configure HAL I2C Handle */
    dev->internal_hi2c.Instance             = i2c_instance;
    dev->internal_hi2c.Init.ClockSpeed      = dev->pins.clock_speed;
    dev->internal_hi2c.Init.DutyCycle       = I2C_DUTYCYCLE_2;
    dev->internal_hi2c.Init.OwnAddress1     = 0;
    dev->internal_hi2c.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
    dev->internal_hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    dev->internal_hi2c.Init.OwnAddress2     = 0;
    dev->internal_hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    dev->internal_hi2c.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;

    if (HAL_I2C_Init(&dev->internal_hi2c) != HAL_OK) {
        return MPU6500_ERROR;
    }

    dev->pins.hi2c = &dev->internal_hi2c;

    /* 5. Run standard sensor init sequence */
    return MPU6500_Init(dev);
}

/**
 * @brief  Initialize MPU-6500 using an already initialized HAL I2C Handle.
 */
MPU6500_Status_e MPU6500_InitWithHandle(MPU6500_t *dev,
                                        I2C_HandleTypeDef *hi2c,
                                        uint8_t i2c_addr)
{
    if (dev == NULL || hi2c == NULL) {
        return MPU6500_INVALID_PARAM;
    }

    memset(dev, 0, sizeof(MPU6500_t));

    dev->pins.bus_type = MPU6500_BUS_EXISTING_HANDLE;
    dev->pins.hi2c     = hi2c;
    dev->pins.i2c_addr = (i2c_addr == 0) ? MPU6500_I2C_ADDR_AD0_LOW : i2c_addr;

    return MPU6500_Init(dev);
}

/**
 * @brief  Initialize MPU-6500 using Software Bit-Banged I2C on ANY arbitrary GPIO pins.
 */
MPU6500_Status_e MPU6500_InitSoftwareI2C(MPU6500_t *dev,
                                         GPIO_TypeDef *scl_port,
                                         uint16_t scl_pin,
                                         GPIO_TypeDef *sda_port,
                                         uint16_t sda_pin,
                                         uint8_t i2c_addr)
{
    if (dev == NULL || scl_port == NULL || sda_port == NULL) {
        return MPU6500_INVALID_PARAM;
    }

    memset(dev, 0, sizeof(MPU6500_t));

    dev->pins.bus_type = MPU6500_BUS_SOFTWARE_I2C;
    dev->pins.scl_port = scl_port;
    dev->pins.scl_pin  = scl_pin;
    dev->pins.sda_port = sda_port;
    dev->pins.sda_pin  = sda_pin;
    dev->pins.i2c_addr = (i2c_addr == 0) ? MPU6500_I2C_ADDR_AD0_LOW : i2c_addr;

    GPIO_Clock_Enable(scl_port);
    GPIO_Clock_Enable(sda_port);

    GPIO_InitTypeDef gpio = {0};
    gpio.Mode  = GPIO_MODE_OUTPUT_OD;
    gpio.Pull  = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    gpio.Pin   = scl_pin;
    HAL_GPIO_Init(scl_port, &gpio);
    HAL_GPIO_WritePin(scl_port, scl_pin, GPIO_PIN_SET);

    gpio.Pin   = sda_pin;
    HAL_GPIO_Init(sda_port, &gpio);
    HAL_GPIO_WritePin(sda_port, sda_pin, GPIO_PIN_SET);

    return MPU6500_Init(dev);
}

/**
 * @brief  Complete default sensor startup sequence.
 */
MPU6500_Status_e MPU6500_Init(MPU6500_t *dev)
{
    if (dev == NULL) {
        return MPU6500_INVALID_PARAM;
    }

    HAL_Delay(50);

    /* 1. Verify WHO_AM_I (Try configured address first, then try alternate address 0x68/0x69) */
    uint8_t who = 0;
    MPU6500_Status_e who_status = MPU6500_ReadWhoAmI(dev, &who);
    
    if (who_status != MPU6500_OK || (who == 0x00 || who == 0xFF)) {
        /* Try alternate address */
        uint8_t alt_addr = (dev->pins.i2c_addr == MPU6500_I2C_ADDR_AD0_LOW) ? 
                            MPU6500_I2C_ADDR_AD0_HIGH : MPU6500_I2C_ADDR_AD0_LOW;
        dev->pins.i2c_addr = alt_addr;
        who_status = MPU6500_ReadWhoAmI(dev, &who);
    }

    if (who_status != MPU6500_OK || who == 0x00 || who == 0xFF) {
        return MPU6500_NOT_FOUND;
    }

    /* 2. Reset Device */
    if (MPU6500_Reset(dev) != MPU6500_OK) {
        return MPU6500_ERROR;
    }
    HAL_Delay(100);

    /* 3. Wake up device and select Auto PLL clock source */
    if (MPU6500_SetClockSource(dev, MPU6500_CLOCK_AUTO_SELECT) != MPU6500_OK) {
        return MPU6500_ERROR;
    }

    /* 4. Set Default Scales: Accel ±4g, Gyro ±500dps */
    if (MPU6500_SetAccelFullScale(dev, MPU6500_ACCEL_FS_4G) != MPU6500_OK) {
        return MPU6500_ERROR;
    }
    if (MPU6500_SetGyroFullScale(dev, MPU6500_GYRO_FS_500DPS) != MPU6500_OK) {
        return MPU6500_ERROR;
    }

    /* 5. Set DLPF (Digital Low Pass Filter) to 41Hz */
    if (MPU6500_SetDLPFBandwidth(dev, MPU6500_DLPF_41HZ) != MPU6500_OK) {
        return MPU6500_ERROR;
    }

    /* 6. Set Sample Rate to 200Hz (1kHz / (1 + 4)) */
    if (MPU6500_SetSampleRateDivider(dev, 4) != MPU6500_OK) {
        return MPU6500_ERROR;
    }

    /* 7. Configure Interrupt Pin: Push-pull, Active High, Data Ready Interrupt Enabled */
    MPU6500_IntConfig_t int_cfg = {
        .active_low          = false,
        .open_drain          = false,
        .latch_until_cleared = false,
        .clear_on_any_read   = true,
        .enable_data_ready   = true,
        .enable_wom          = false,
        .enable_fifo_overflow= false
    };
    MPU6500_ConfigureInterrupt(dev, &int_cfg);

    dev->is_initialized = true;
    return MPU6500_OK;
}

/**
 * @brief  Software Reset the MPU-6500.
 */
MPU6500_Status_e MPU6500_Reset(MPU6500_t *dev)
{
    /* Bit 7 of PWR_MGMT_1 triggers device reset */
    MPU6500_Status_e status = MPU6500_WriteRegister(dev, MPU6500_REG_PWR_MGMT_1, 0x80);
    if (status == MPU6500_OK) {
        HAL_Delay(100);
    }
    return status;
}

/**
 * @brief  Check if device responds and WHO_AM_I is valid.
 */
bool MPU6500_IsConnected(MPU6500_t *dev)
{
    uint8_t who = 0;
    if (MPU6500_ReadWhoAmI(dev, &who) == MPU6500_OK) {
        return (who == MPU6500_WHO_AM_I_VAL || who == MPU9250_WHO_AM_I_VAL || who == MPU6555_WHO_AM_I_VAL || who == 0x68);
    }
    return false;
}

/**
 * @brief  Read the WHO_AM_I register value directly.
 */
MPU6500_Status_e MPU6500_ReadWhoAmI(MPU6500_t *dev, uint8_t *who_am_i)
{
    return MPU6500_ReadRegister(dev, MPU6500_REG_WHO_AM_I, who_am_i);
}

/**
 * @brief  Set Accelerometer Full Scale Range.
 */
MPU6500_Status_e MPU6500_SetAccelFullScale(MPU6500_t *dev, MPU6500_AccelScale_e scale)
{
    MPU6500_Status_e status = MPU6500_WriteRegister(dev, MPU6500_REG_ACCEL_CONFIG, (uint8_t)scale);
    if (status == MPU6500_OK) {
        dev->accel_scale = scale;
        switch (scale) {
            case MPU6500_ACCEL_FS_2G:  dev->accel_lsb_to_g = 1.0f / 16384.0f; break;
            case MPU6500_ACCEL_FS_4G:  dev->accel_lsb_to_g = 1.0f / 8192.0f;  break;
            case MPU6500_ACCEL_FS_8G:  dev->accel_lsb_to_g = 1.0f / 4096.0f;  break;
            case MPU6500_ACCEL_FS_16G: dev->accel_lsb_to_g = 1.0f / 2048.0f;  break;
            default: dev->accel_lsb_to_g = 1.0f / 8192.0f; break;
        }
    }
    return status;
}

/**
 * @brief  Set Gyroscope Full Scale Range.
 */
MPU6500_Status_e MPU6500_SetGyroFullScale(MPU6500_t *dev, MPU6500_GyroScale_e scale)
{
    MPU6500_Status_e status = MPU6500_WriteRegister(dev, MPU6500_REG_GYRO_CONFIG, (uint8_t)scale);
    if (status == MPU6500_OK) {
        dev->gyro_scale = scale;
        switch (scale) {
            case MPU6500_GYRO_FS_250DPS:  dev->gyro_lsb_to_dps = 1.0f / 131.0f; break;
            case MPU6500_GYRO_FS_500DPS:  dev->gyro_lsb_to_dps = 1.0f / 65.5f;  break;
            case MPU6500_GYRO_FS_1000DPS: dev->gyro_lsb_to_dps = 1.0f / 32.8f;  break;
            case MPU6500_GYRO_FS_2000DPS: dev->gyro_lsb_to_dps = 1.0f / 16.4f;  break;
            default: dev->gyro_lsb_to_dps = 1.0f / 65.5f; break;
        }
    }
    return status;
}

/**
 * @brief  Set Sample Rate Divider.
 */
MPU6500_Status_e MPU6500_SetSampleRateDivider(MPU6500_t *dev, uint8_t div)
{
    return MPU6500_WriteRegister(dev, MPU6500_REG_SMPLRT_DIV, div);
}

/**
 * @brief  Set Digital Low Pass Filter (DLPF) Bandwidth.
 */
MPU6500_Status_e MPU6500_SetDLPFBandwidth(MPU6500_t *dev, MPU6500_DLPF_e dlpf)
{
    MPU6500_Status_e status = MPU6500_WriteRegister(dev, MPU6500_REG_CONFIG, (uint8_t)dlpf);
    if (status == MPU6500_OK) {
        dev->dlpf = dlpf;
    }
    return status;
}

/**
 * @brief  Configure Power Clock Source.
 */
MPU6500_Status_e MPU6500_SetClockSource(MPU6500_t *dev, MPU6500_ClockSource_e clock_src)
{
    uint8_t pwr_mgmt = 0;
    MPU6500_ReadRegister(dev, MPU6500_REG_PWR_MGMT_1, &pwr_mgmt);
    pwr_mgmt = (pwr_mgmt & ~0x07) | ((uint8_t)clock_src & 0x07);
    pwr_mgmt &= ~(1 << 6); /* Clear sleep mode */
    return MPU6500_WriteRegister(dev, MPU6500_REG_PWR_MGMT_1, pwr_mgmt);
}

/**
 * @brief  Enable or Disable Sleep Mode.
 */
MPU6500_Status_e MPU6500_SetSleepMode(MPU6500_t *dev, bool enable)
{
    uint8_t pwr_mgmt = 0;
    MPU6500_ReadRegister(dev, MPU6500_REG_PWR_MGMT_1, &pwr_mgmt);
    if (enable) {
        pwr_mgmt |= (1 << 6);
    } else {
        pwr_mgmt &= ~(1 << 6);
    }
    return MPU6500_WriteRegister(dev, MPU6500_REG_PWR_MGMT_1, pwr_mgmt);
}

/**
 * @brief  Configure Interrupt Output Pin and Interrupt Sources.
 */
MPU6500_Status_e MPU6500_ConfigureInterrupt(MPU6500_t *dev, const MPU6500_IntConfig_t *int_cfg)
{
    if (dev == NULL || int_cfg == NULL) {
        return MPU6500_INVALID_PARAM;
    }

    uint8_t pin_cfg = 0;
    if (int_cfg->active_low)          pin_cfg |= (1 << 7);
    if (int_cfg->open_drain)          pin_cfg |= (1 << 6);
    if (int_cfg->latch_until_cleared) pin_cfg |= (1 << 5);
    if (int_cfg->clear_on_any_read)   pin_cfg |= (1 << 4);

    MPU6500_Status_e status = MPU6500_WriteRegister(dev, MPU6500_REG_INT_PIN_CFG, pin_cfg);
    if (status != MPU6500_OK) return status;

    uint8_t enable_val = 0;
    if (int_cfg->enable_wom)           enable_val |= (1 << 6);
    if (int_cfg->enable_fifo_overflow) enable_val |= (1 << 4);
    if (int_cfg->enable_data_ready)    enable_val |= (1 << 0);

    return MPU6500_WriteRegister(dev, MPU6500_REG_INT_ENABLE, enable_val);
}

/**
 * @brief  Read the Interrupt Status Register.
 */
MPU6500_Status_e MPU6500_GetInterruptStatus(MPU6500_t *dev, uint8_t *status)
{
    return MPU6500_ReadRegister(dev, MPU6500_REG_INT_STATUS, status);
}

/**
 * @brief  Read Raw Accelerometer data (X, Y, Z).
 */
MPU6500_Status_e MPU6500_ReadRawAccel(MPU6500_t *dev, int16_t *ax, int16_t *ay, int16_t *az)
{
    uint8_t buf[6];
    MPU6500_Status_e status = MPU6500_ReadRegisters(dev, MPU6500_REG_ACCEL_XOUT_H, buf, 6);
    if (status == MPU6500_OK) {
        if (ax) *ax = (int16_t)(((uint16_t)buf[0] << 8) | buf[1]);
        if (ay) *ay = (int16_t)(((uint16_t)buf[2] << 8) | buf[3]);
        if (az) *az = (int16_t)(((uint16_t)buf[4] << 8) | buf[5]);
    }
    return status;
}

/**
 * @brief  Read Raw Gyroscope data (X, Y, Z).
 */
MPU6500_Status_e MPU6500_ReadRawGyro(MPU6500_t *dev, int16_t *gx, int16_t *gy, int16_t *gz)
{
    uint8_t buf[6];
    MPU6500_Status_e status = MPU6500_ReadRegisters(dev, MPU6500_REG_GYRO_XOUT_H, buf, 6);
    if (status == MPU6500_OK) {
        if (gx) *gx = (int16_t)(((uint16_t)buf[0] << 8) | buf[1]);
        if (gy) *gy = (int16_t)(((uint16_t)buf[2] << 8) | buf[3]);
        if (gz) *gz = (int16_t)(((uint16_t)buf[4] << 8) | buf[5]);
    }
    return status;
}

/**
 * @brief  Read Raw Temperature data.
 */
MPU6500_Status_e MPU6500_ReadRawTemp(MPU6500_t *dev, int16_t *temp_raw)
{
    uint8_t buf[2];
    MPU6500_Status_e status = MPU6500_ReadRegisters(dev, MPU6500_REG_TEMP_OUT_H, buf, 2);
    if (status == MPU6500_OK && temp_raw) {
        *temp_raw = (int16_t)(((uint16_t)buf[0] << 8) | buf[1]);
    }
    return status;
}

/**
 * @brief  Perform an atomic 14-byte burst read of all raw data registers.
 */
MPU6500_Status_e MPU6500_ReadAllRaw(MPU6500_t *dev, MPU6500_RawData_t *raw)
{
    if (dev == NULL || raw == NULL) {
        return MPU6500_INVALID_PARAM;
    }

    uint8_t buf[14];
    MPU6500_Status_e status = MPU6500_ReadRegisters(dev, MPU6500_REG_ACCEL_XOUT_H, buf, 14);
    if (status == MPU6500_OK) {
        raw->accel_x  = (int16_t)(((uint16_t)buf[0]  << 8) | buf[1]);
        raw->accel_y  = (int16_t)(((uint16_t)buf[2]  << 8) | buf[3]);
        raw->accel_z  = (int16_t)(((uint16_t)buf[4]  << 8) | buf[5]);
        raw->temp_raw = (int16_t)(((uint16_t)buf[6]  << 8) | buf[7]);
        raw->gyro_x   = (int16_t)(((uint16_t)buf[8]  << 8) | buf[9]);
        raw->gyro_y   = (int16_t)(((uint16_t)buf[10] << 8) | buf[11]);
        raw->gyro_z   = (int16_t)(((uint16_t)buf[12] << 8) | buf[13]);
    }
    return status;
}

/**
 * @brief  Read Scaled Accelerometer data in 'g'.
 */
MPU6500_Status_e MPU6500_ReadAccel(MPU6500_t *dev, float *ax, float *ay, float *az)
{
    int16_t raw_x, raw_y, raw_z;
    MPU6500_Status_e status = MPU6500_ReadRawAccel(dev, &raw_x, &raw_y, &raw_z);
    if (status == MPU6500_OK) {
        if (ax) *ax = ((float)raw_x * dev->accel_lsb_to_g) - dev->offsets.accel_offset_x;
        if (ay) *ay = ((float)raw_y * dev->accel_lsb_to_g) - dev->offsets.accel_offset_y;
        if (az) *az = ((float)raw_z * dev->accel_lsb_to_g) - dev->offsets.accel_offset_z;
    }
    return status;
}

/**
 * @brief  Read Scaled Gyroscope data in degrees per second (°/s).
 */
MPU6500_Status_e MPU6500_ReadGyro(MPU6500_t *dev, float *gx, float *gy, float *gz)
{
    int16_t raw_x, raw_y, raw_z;
    MPU6500_Status_e status = MPU6500_ReadRawGyro(dev, &raw_x, &raw_y, &raw_z);
    if (status == MPU6500_OK) {
        if (gx) *gx = ((float)raw_x * dev->gyro_lsb_to_dps) - dev->offsets.gyro_offset_x;
        if (gy) *gy = ((float)raw_y * dev->gyro_lsb_to_dps) - dev->offsets.gyro_offset_y;
        if (gz) *gz = ((float)raw_z * dev->gyro_lsb_to_dps) - dev->offsets.gyro_offset_z;
    }
    return status;
}

/**
 * @brief  Read Scaled Temperature in degrees Celsius (°C).
 */
MPU6500_Status_e MPU6500_ReadTemp(MPU6500_t *dev, float *temp_c)
{
    int16_t raw_temp;
    MPU6500_Status_e status = MPU6500_ReadRawTemp(dev, &raw_temp);
    if (status == MPU6500_OK && temp_c) {
        /* Formula from MPU-6500 Register Map Spec: Temp_C = (TEMP_OUT / 333.87) + 21.0 */
        *temp_c = ((float)raw_temp / 333.87f) + 21.0f;
    }
    return status;
}

/**
 * @brief  Read and calculate all sensor channels in engineering units.
 */
MPU6500_Status_e MPU6500_ReadAll(MPU6500_t *dev, MPU6500_ScaledData_t *data)
{
    if (dev == NULL || data == NULL) {
        return MPU6500_INVALID_PARAM;
    }

    MPU6500_RawData_t raw;
    MPU6500_Status_e status = MPU6500_ReadAllRaw(dev, &raw);
    if (status != MPU6500_OK) {
        return status;
    }

    /* Accel in g */
    data->accel_x_g = ((float)raw.accel_x * dev->accel_lsb_to_g) - dev->offsets.accel_offset_x;
    data->accel_y_g = ((float)raw.accel_y * dev->accel_lsb_to_g) - dev->offsets.accel_offset_y;
    data->accel_z_g = ((float)raw.accel_z * dev->accel_lsb_to_g) - dev->offsets.accel_offset_z;

    /* Accel in m/s² */
    data->accel_x_ms2 = data->accel_x_g * CONST_GRAVITY_EARTH;
    data->accel_y_ms2 = data->accel_y_g * CONST_GRAVITY_EARTH;
    data->accel_z_ms2 = data->accel_z_g * CONST_GRAVITY_EARTH;

    /* Gyro in dps */
    data->gyro_x_dps = ((float)raw.gyro_x * dev->gyro_lsb_to_dps) - dev->offsets.gyro_offset_x;
    data->gyro_y_dps = ((float)raw.gyro_y * dev->gyro_lsb_to_dps) - dev->offsets.gyro_offset_y;
    data->gyro_z_dps = ((float)raw.gyro_z * dev->gyro_lsb_to_dps) - dev->offsets.gyro_offset_z;

    /* Gyro in rad/s */
    data->gyro_x_rads = data->gyro_x_dps * DEG_TO_RAD;
    data->gyro_y_rads = data->gyro_y_dps * DEG_TO_RAD;
    data->gyro_z_rads = data->gyro_z_dps * DEG_TO_RAD;

    /* Temperature in °C */
    data->temp_c = ((float)raw.temp_raw / 333.87f) + 21.0f;

    return MPU6500_OK;
}

/**
 * @brief  Read and return both raw 16-bit values and calibrated/scaled physical quantities.
 */
MPU6500_Status_e MPU6500_ReadAllData(MPU6500_t *dev, MPU6500_AllData_t *data)
{
    if (dev == NULL || data == NULL) {
        return MPU6500_INVALID_PARAM;
    }

    MPU6500_Status_e status = MPU6500_ReadAllRaw(dev, &data->raw);
    if (status != MPU6500_OK) {
        return status;
    }

    /* Accel in g */
    data->scaled.accel_x_g = ((float)data->raw.accel_x * dev->accel_lsb_to_g) - dev->offsets.accel_offset_x;
    data->scaled.accel_y_g = ((float)data->raw.accel_y * dev->accel_lsb_to_g) - dev->offsets.accel_offset_y;
    data->scaled.accel_z_g = ((float)data->raw.accel_z * dev->accel_lsb_to_g) - dev->offsets.accel_offset_z;

    /* Accel in m/s² */
    data->scaled.accel_x_ms2 = data->scaled.accel_x_g * CONST_GRAVITY_EARTH;
    data->scaled.accel_y_ms2 = data->scaled.accel_y_g * CONST_GRAVITY_EARTH;
    data->scaled.accel_z_ms2 = data->scaled.accel_z_g * CONST_GRAVITY_EARTH;

    /* Gyro in dps */
    data->scaled.gyro_x_dps = ((float)data->raw.gyro_x * dev->gyro_lsb_to_dps) - dev->offsets.gyro_offset_x;
    data->scaled.gyro_y_dps = ((float)data->raw.gyro_y * dev->gyro_lsb_to_dps) - dev->offsets.gyro_offset_y;
    data->scaled.gyro_z_dps = ((float)data->raw.gyro_z * dev->gyro_lsb_to_dps) - dev->offsets.gyro_offset_z;

    /* Gyro in rad/s */
    data->scaled.gyro_x_rads = data->scaled.gyro_x_dps * DEG_TO_RAD;
    data->scaled.gyro_y_rads = data->scaled.gyro_y_dps * DEG_TO_RAD;
    data->scaled.gyro_z_rads = data->scaled.gyro_z_dps * DEG_TO_RAD;

    /* Temperature in °C */
    data->scaled.temp_c = ((float)data->raw.temp_raw / 333.87f) + 21.0f;

    return MPU6500_OK;
}

/**
 * @brief  Calibrate Gyroscope by taking average over N stationary samples.
 */
MPU6500_Status_e MPU6500_CalibrateGyro(MPU6500_t *dev, uint16_t num_samples)
{
    if (dev == NULL || num_samples == 0) return MPU6500_INVALID_PARAM;

    float sum_x = 0.0f, sum_y = 0.0f, sum_z = 0.0f;
    int16_t gx, gy, gz;

    dev->offsets.gyro_offset_x = 0.0f;
    dev->offsets.gyro_offset_y = 0.0f;
    dev->offsets.gyro_offset_z = 0.0f;

    for (uint16_t i = 0; i < num_samples; i++) {
        if (MPU6500_ReadRawGyro(dev, &gx, &gy, &gz) != MPU6500_OK) {
            return MPU6500_ERROR;
        }
        sum_x += (float)gx * dev->gyro_lsb_to_dps;
        sum_y += (float)gy * dev->gyro_lsb_to_dps;
        sum_z += (float)gz * dev->gyro_lsb_to_dps;
        HAL_Delay(3);
    }

    dev->offsets.gyro_offset_x = sum_x / (float)num_samples;
    dev->offsets.gyro_offset_y = sum_y / (float)num_samples;
    dev->offsets.gyro_offset_z = sum_z / (float)num_samples;

    return MPU6500_OK;
}

/**
 * @brief  Calibrate Accelerometer assuming Z-axis points UP (+1g) on flat surface.
 */
MPU6500_Status_e MPU6500_CalibrateAccel(MPU6500_t *dev, uint16_t num_samples)
{
    if (dev == NULL || num_samples == 0) return MPU6500_INVALID_PARAM;

    float sum_x = 0.0f, sum_y = 0.0f, sum_z = 0.0f;
    int16_t ax, ay, az;

    dev->offsets.accel_offset_x = 0.0f;
    dev->offsets.accel_offset_y = 0.0f;
    dev->offsets.accel_offset_z = 0.0f;

    for (uint16_t i = 0; i < num_samples; i++) {
        if (MPU6500_ReadRawAccel(dev, &ax, &ay, &az) != MPU6500_OK) {
            return MPU6500_ERROR;
        }
        sum_x += (float)ax * dev->accel_lsb_to_g;
        sum_y += (float)ay * dev->accel_lsb_to_g;
        sum_z += (float)az * dev->accel_lsb_to_g;
        HAL_Delay(3);
    }

    dev->offsets.accel_offset_x = sum_x / (float)num_samples;
    dev->offsets.accel_offset_y = sum_y / (float)num_samples;
    dev->offsets.accel_offset_z = (sum_z / (float)num_samples) - 1.0f; /* Account for 1g gravity */

    return MPU6500_OK;
}

/**
 * @brief  Set user defined offsets manually.
 */
void MPU6500_SetOffsets(MPU6500_t *dev, const MPU6500_Offsets_t *offsets)
{
    if (dev != NULL && offsets != NULL) {
        dev->offsets = *offsets;
    }
}

/**
 * @brief  Get current calibration offsets.
 */
void MPU6500_GetOffsets(const MPU6500_t *dev, MPU6500_Offsets_t *offsets)
{
    if (dev != NULL && offsets != NULL) {
        *offsets = dev->offsets;
    }
}

/**
 * @brief  Enable or Disable FIFO buffer channels.
 */
MPU6500_Status_e MPU6500_FIFO_Enable(MPU6500_t *dev, uint8_t fifo_flags)
{
    /* Enable FIFO operations in USER_CTRL */
    uint8_t user_ctrl = 0;
    MPU6500_ReadRegister(dev, MPU6500_REG_USER_CTRL, &user_ctrl);
    user_ctrl |= (1 << 6); /* FIFO_EN */
    MPU6500_WriteRegister(dev, MPU6500_REG_USER_CTRL, user_ctrl);

    /* Configure channels in FIFO_EN register */
    return MPU6500_WriteRegister(dev, MPU6500_REG_FIFO_EN, fifo_flags);
}

/**
 * @brief  Get current byte count in FIFO buffer.
 */
MPU6500_Status_e MPU6500_FIFO_GetCount(MPU6500_t *dev, uint16_t *count)
{
    uint8_t buf[2];
    MPU6500_Status_e status = MPU6500_ReadRegisters(dev, MPU6500_REG_FIFO_COUNTH, buf, 2);
    if (status == MPU6500_OK && count) {
        *count = ((uint16_t)buf[0] << 8) | buf[1];
    }
    return status;
}

/**
 * @brief  Read data block from FIFO.
 */
MPU6500_Status_e MPU6500_FIFO_Read(MPU6500_t *dev, uint8_t *buffer, uint16_t length)
{
    return MPU6500_ReadRegisters(dev, MPU6500_REG_FIFO_R_W, buffer, length);
}

/**
 * @brief  Reset the FIFO buffer.
 */
MPU6500_Status_e MPU6500_FIFO_Reset(MPU6500_t *dev)
{
    uint8_t user_ctrl = 0;
    MPU6500_ReadRegister(dev, MPU6500_REG_USER_CTRL, &user_ctrl);
    user_ctrl |= (1 << 2); /* FIFO_RESET */
    return MPU6500_WriteRegister(dev, MPU6500_REG_USER_CTRL, user_ctrl);
}

/**
 * @brief  Configure Wake on Motion (WOM) interrupt.
 */
MPU6500_Status_e MPU6500_EnableWakeOnMotion(MPU6500_t *dev, uint8_t threshold_mg, MPU6500_LowPowerAccelODR_e odr)
{
    /* Set threshold */
    MPU6500_WriteRegister(dev, MPU6500_REG_WOM_THR, threshold_mg);
    /* Set Low Power ODR */
    MPU6500_WriteRegister(dev, MPU6500_REG_LP_ACCEL_ODR, (uint8_t)odr);
    /* Enable Accelerometer Intelligence */
    MPU6500_WriteRegister(dev, MPU6500_REG_ACCEL_INTEL_CTRL, 0xC0);
    /* Enable WOM interrupt */
    return MPU6500_WriteRegister(dev, MPU6500_REG_INT_ENABLE, 0x40);
}

/* Low-level Communication Functions -----------------------------------------*/

/**
 * @brief  Write single 8-bit register.
 */
MPU6500_Status_e MPU6500_WriteRegister(MPU6500_t *dev, uint8_t reg_addr, uint8_t data)
{
    if (dev == NULL) return MPU6500_INVALID_PARAM;

    if (dev->pins.bus_type == MPU6500_BUS_HARDWARE_I2C || dev->pins.bus_type == MPU6500_BUS_EXISTING_HANDLE) {
        uint8_t i2c_addr_8bit = (dev->pins.i2c_addr << 1);
        if (HAL_I2C_Mem_Write(dev->pins.hi2c, i2c_addr_8bit, reg_addr, I2C_MEMADD_SIZE_8BIT, &data, 1, MPU6500_TIMEOUT_DEFAULT_MS) == HAL_OK) {
            return MPU6500_OK;
        }
        return MPU6500_ERROR;
    } else {
        /* Software I2C */
        uint8_t dev_addr_write = (dev->pins.i2c_addr << 1);
        SW_I2C_Start(&dev->pins);
        if (!SW_I2C_WriteByte(&dev->pins, dev_addr_write)) { SW_I2C_Stop(&dev->pins); return MPU6500_ERROR; }
        if (!SW_I2C_WriteByte(&dev->pins, reg_addr))       { SW_I2C_Stop(&dev->pins); return MPU6500_ERROR; }
        if (!SW_I2C_WriteByte(&dev->pins, data))           { SW_I2C_Stop(&dev->pins); return MPU6500_ERROR; }
        SW_I2C_Stop(&dev->pins);
        return MPU6500_OK;
    }
}

/**
 * @brief  Read single 8-bit register.
 */
MPU6500_Status_e MPU6500_ReadRegister(MPU6500_t *dev, uint8_t reg_addr, uint8_t *data)
{
    return MPU6500_ReadRegisters(dev, reg_addr, data, 1);
}

/**
 * @brief  Read multiple sequential registers.
 */
MPU6500_Status_e MPU6500_ReadRegisters(MPU6500_t *dev, uint8_t reg_addr, uint8_t *data, uint16_t length)
{
    if (dev == NULL || data == NULL || length == 0) return MPU6500_INVALID_PARAM;

    if (dev->pins.bus_type == MPU6500_BUS_HARDWARE_I2C || dev->pins.bus_type == MPU6500_BUS_EXISTING_HANDLE) {
        uint8_t i2c_addr_8bit = (dev->pins.i2c_addr << 1);
        if (HAL_I2C_Mem_Read(dev->pins.hi2c, i2c_addr_8bit, reg_addr, I2C_MEMADD_SIZE_8BIT, data, length, MPU6500_TIMEOUT_DEFAULT_MS) == HAL_OK) {
            return MPU6500_OK;
        }
        return MPU6500_ERROR;
    } else {
        /* Software I2C */
        uint8_t dev_addr_write = (dev->pins.i2c_addr << 1);
        uint8_t dev_addr_read  = (dev->pins.i2c_addr << 1) | 0x01;

        SW_I2C_Start(&dev->pins);
        if (!SW_I2C_WriteByte(&dev->pins, dev_addr_write)) { SW_I2C_Stop(&dev->pins); return MPU6500_ERROR; }
        if (!SW_I2C_WriteByte(&dev->pins, reg_addr))       { SW_I2C_Stop(&dev->pins); return MPU6500_ERROR; }

        SW_I2C_Start(&dev->pins); /* Repeated Start */
        if (!SW_I2C_WriteByte(&dev->pins, dev_addr_read))  { SW_I2C_Stop(&dev->pins); return MPU6500_ERROR; }

        for (uint16_t i = 0; i < length; i++) {
            bool ack = (i < (length - 1)); /* Send ACK for all except last byte */
            data[i] = SW_I2C_ReadByte(&dev->pins, ack);
        }
        SW_I2C_Stop(&dev->pins);
        return MPU6500_OK;
    }
}

/* Helper Functions ----------------------------------------------------------*/

static void GPIO_Clock_Enable(GPIO_TypeDef *port)
{
    if (port == GPIOA)      __HAL_RCC_GPIOA_CLK_ENABLE();
    else if (port == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
    else if (port == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();
#ifdef GPIOD
    else if (port == GPIOD) __HAL_RCC_GPIOD_CLK_ENABLE();
#endif
#ifdef GPIOE
    else if (port == GPIOE) __HAL_RCC_GPIOE_CLK_ENABLE();
#endif
#ifdef GPIOH
    else if (port == GPIOH) __HAL_RCC_GPIOH_CLK_ENABLE();
#endif
}

static void I2C_Clock_Enable(I2C_TypeDef *instance)
{
#ifdef I2C1
    if (instance == I2C1) __HAL_RCC_I2C1_CLK_ENABLE();
#endif
#ifdef I2C2
    if (instance == I2C2) __HAL_RCC_I2C2_CLK_ENABLE();
#endif
#ifdef I2C3
    if (instance == I2C3) __HAL_RCC_I2C3_CLK_ENABLE();
#endif
}

static uint8_t Auto_Detect_AF(I2C_TypeDef *instance)
{
#ifdef I2C1
    if (instance == I2C1) return GPIO_AF4_I2C1;
#endif
#ifdef I2C2
    if (instance == I2C2) return GPIO_AF4_I2C2;
#endif
#ifdef I2C3
    if (instance == I2C3) return GPIO_AF4_I2C3;
#endif
    return 0x04; /* Default AF4 for I2C on STM32F4 */
}

/* Software Bit-Banged I2C Implementation ------------------------------------*/

static void SW_I2C_Delay(void)
{
    /* Small delay loop (~2.5 - 5us depending on CPU freq for ~100-200kHz I2C clock) */
    for (volatile uint32_t i = 0; i < 25; i++) {
        __NOP();
    }
}

static void SW_I2C_Start(const MPU6500_PinConfig_t *pins)
{
    HAL_GPIO_WritePin(pins->sda_port, pins->sda_pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(pins->scl_port, pins->scl_pin, GPIO_PIN_SET);
    SW_I2C_Delay();
    HAL_GPIO_WritePin(pins->sda_port, pins->sda_pin, GPIO_PIN_RESET);
    SW_I2C_Delay();
    HAL_GPIO_WritePin(pins->scl_port, pins->scl_pin, GPIO_PIN_RESET);
    SW_I2C_Delay();
}

static void SW_I2C_Stop(const MPU6500_PinConfig_t *pins)
{
    HAL_GPIO_WritePin(pins->sda_port, pins->sda_pin, GPIO_PIN_RESET);
    SW_I2C_Delay();
    HAL_GPIO_WritePin(pins->scl_port, pins->scl_pin, GPIO_PIN_SET);
    SW_I2C_Delay();
    HAL_GPIO_WritePin(pins->sda_port, pins->sda_pin, GPIO_PIN_SET);
    SW_I2C_Delay();
}

static bool SW_I2C_WriteByte(const MPU6500_PinConfig_t *pins, uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++) {
        if (byte & 0x80) {
            HAL_GPIO_WritePin(pins->sda_port, pins->sda_pin, GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(pins->sda_port, pins->sda_pin, GPIO_PIN_RESET);
        }
        byte <<= 1;
        SW_I2C_Delay();
        HAL_GPIO_WritePin(pins->scl_port, pins->scl_pin, GPIO_PIN_SET);
        SW_I2C_Delay();
        HAL_GPIO_WritePin(pins->scl_port, pins->scl_pin, GPIO_PIN_RESET);
    }

    /* Read ACK */
    HAL_GPIO_WritePin(pins->sda_port, pins->sda_pin, GPIO_PIN_SET); /* Release SDA line */
    SW_I2C_Delay();
    HAL_GPIO_WritePin(pins->scl_port, pins->scl_pin, GPIO_PIN_SET);
    SW_I2C_Delay();

    GPIO_PinState ack = HAL_GPIO_ReadPin(pins->sda_port, pins->sda_pin);
    HAL_GPIO_WritePin(pins->scl_port, pins->scl_pin, GPIO_PIN_RESET);
    SW_I2C_Delay();

    return (ack == GPIO_PIN_RESET); /* ACK is LOW */
}

static uint8_t SW_I2C_ReadByte(const MPU6500_PinConfig_t *pins, bool ack)
{
    uint8_t byte = 0;
    HAL_GPIO_WritePin(pins->sda_port, pins->sda_pin, GPIO_PIN_SET); /* Release line */

    for (uint8_t i = 0; i < 8; i++) {
        byte <<= 1;
        HAL_GPIO_WritePin(pins->scl_port, pins->scl_pin, GPIO_PIN_SET);
        SW_I2C_Delay();
        if (HAL_GPIO_ReadPin(pins->sda_port, pins->sda_pin) == GPIO_PIN_SET) {
            byte |= 0x01;
        }
        HAL_GPIO_WritePin(pins->scl_port, pins->scl_pin, GPIO_PIN_RESET);
        SW_I2C_Delay();
    }

    /* Send ACK or NACK */
    if (ack) {
        HAL_GPIO_WritePin(pins->sda_port, pins->sda_pin, GPIO_PIN_RESET); /* Send ACK */
    } else {
        HAL_GPIO_WritePin(pins->sda_port, pins->sda_pin, GPIO_PIN_SET);   /* Send NACK */
    }
    SW_I2C_Delay();
    HAL_GPIO_WritePin(pins->scl_port, pins->scl_pin, GPIO_PIN_SET);
    SW_I2C_Delay();
    HAL_GPIO_WritePin(pins->scl_port, pins->scl_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(pins->sda_port, pins->sda_pin, GPIO_PIN_SET);
    SW_I2C_Delay();

    return byte;
}
