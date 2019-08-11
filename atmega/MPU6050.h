uint8_t shock_breach;

char gval[5] = "9.80";

char epm_gval[5];
float shock_trigger = 3.0; // GVal Threshold-

float rec_gval;

void MPU6050_Init()										/* Gyro initialization function */
{
	_delay_ms(150);										/* Power up time >100ms */
	I2C_Start_Wait(0xD0);								/* Start with device write address */
	I2C_Write(SMPLRT_DIV);								/* Write to sample rate register */
	I2C_Write(0x07);									/* 1KHz sample rate */
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(PWR_MGMT_1);								/* Write to power management register */
	I2C_Write(0x01);									/* X axis gyroscope reference frequency */
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(CONFIG);									/* Write to Configuration register */
	I2C_Write(0x00);									/* Fs = 8KHz */
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(GYRO_CONFIG);								/* Write to Gyro configuration register */
	I2C_Write(0x18);									/* Full scale range +/- 2000 degree/C */
	I2C_Stop();
	
	I2C_Start_Wait(0xD0);
	I2C_Write(INT_ENABLE);								/* Write to interrupt enable register */
	I2C_Write(0x01);
	I2C_Stop();
}

void MPU_Start_Loc()
{
	I2C_Start_Wait(0xD0);								/* I2C start with device write address */
	I2C_Write(ACCEL_XOUT_H);							/* Write start location address from where to read */
	I2C_Repeated_Start(0xD1);							/* I2C start with device read address */
}

void checkShockBreach(){
	float Xa,Ya,Za,Xg,Yg,Zg,t=0;
	//Get Values from Accelerator
	MPU_Start_Loc();									/* Read Gyro values */
	Xa = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack())/16384.0;
	Ya = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack())/16384.0;
	Za = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack())/16384.0;
	t = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Xg = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Yg = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Zg = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Nack());
	I2C_Stop();
	
	rec_gval = sqrt((Xa*Xa)+(Ya*Ya)+(Za*Za))*9.8;
	
	if(rec_gval<shock_trigger){
		shock_breach = 2;
	}else{
		shock_breach = 0;
	}
}