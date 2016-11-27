
/*
  msg_t i2cMasterTransmitTimeout(I2CDriver *i2cp,
                                 i2caddr_t addr,
                                 const uint8_t *txbuf, size_t txbytes,
                                 uint8_t *rxbuf, size_t rxbytes,
                                 systime_t timeout);
  msg_t i2cMasterReceiveTimeout(I2CDriver *i2cp,
                                i2caddr_t addr,
                                uint8_t *rxbuf, size_t rxbytes,
                                systime_t timeout);
    
*/





static bool MPU6050_Writes(uint8_t slaveAddr, uint8_t regAddr, uint8_t *data, uint8_t number_bytes);
static bool MPU6050_Reads(uint8_t slaveAddr, uint8_t regAddr, uint8_t *data, uint8_t number_bytes);

static bool MPU6050_Write(uint8_t slaveAddr, uint8_t regAddr, uint8_t data);
static bool MPU6050_Read(uint8_t slaveAddr, uint8_t regAddr, uint8_t *data);

static bool MPU6050_WriteMask(uint8_t slaveAddr, uint8_t regAddr, uint8_t data, uint8_t mask, uint8_t offset);
static bool MPU6050_ReadMask(uint8_t slaveAddr, uint8_t regAddr, uint8_t *data, uint8_t mask, uint8_t offset);
