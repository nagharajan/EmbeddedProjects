
float min_temp_trigger;
float max_temp_trigger;

float min_hum_trigger;
float max_hum_trigger;

float rec_temp;
float rec_hum;

uint8_t temp_breach;
uint8_t humd_breach;

char temp[5] = "25.0";
char hum[5] = "50.0";

#define HDC_Addr_W 0x80
#define HDC_Addr_R 0x81

void checkTempBreach(){
	I2C_Start(HDC_Addr_W);
		I2C_Write(0x0F);
		I2C_Write(0x01);
		I2C_Stop();
		
		_delay_ms(2);
		I2C_Start(HDC_Addr_W);
		I2C_Write(0x00);
		I2C_Write(0x01);
		I2C_Write(0x02);
		I2C_Write(0x03);
		I2C_Repeated_Start(HDC_Addr_R);
		
		char t_lsb = I2C_Read_Ack();
		char t_msb = I2C_Read_Ack();
		char h_lsb = I2C_Read_Ack();
		char h_msb = I2C_Read_Nack();
		I2C_Stop();
	
		rec_temp = (((int) (t_msb))*165/pow(2,8))-40;
		rec_hum = ((int) (h_msb))*100/pow(2,8);
	
	if(rec_temp<min_temp_trigger || rec_temp>max_temp_trigger){
		temp_breach = 4;
	}else{
		temp_breach = 0;
	}
	
	if(rec_hum<min_hum_trigger || rec_hum>max_hum_trigger){
		humd_breach = 8;
	}else{
		humd_breach = 0;
	}
}