#define MaxAddr_W 0x94
#define MaxAddr_R 0x95

char lux[5] = "0.00";

float lux_trigger; //Threshold setting for Lux
float rec_lux; //Recorded value from sensor
uint8_t lux_breach;

void checkLuxBreach(){
	I2C_Start_Wait(MaxAddr_W);
	I2C_Write(0x3);
	I2C_Write(0x4);
	
	I2C_Repeated_Start(MaxAddr_R);
	
	char exp = I2C_Read_Ack();
	char mant = I2C_Read_Nack();
	I2C_Stop();
	
	int exponent = (int) (( exp & 0xF0) >> 4);
	int mantissa = (int) ((exp & 0x0F) << 4) | (mant & 0x0F);
	
	rec_lux =  pow(2, exponent) * mantissa * 0.045;
	
	if(rec_lux>lux_trigger){
		lux_breach = 1;
	}else{
		lux_breach = 0;
	}
}