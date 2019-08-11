
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <avr/io.h>
#include "SIMUSART.h"
#include "MyUSART.h"

char lat[12] = "12.852803";
char lng[12] = "80.226723";

char batlvl[5] = "100";

void debugPrint(char *msg, char *cmd,char *val){
	sprintf(debugMsg,"%s %s %s\n",msg,cmd,val);
	USART_putstring(debugMsg);
}

bool execCmd(char cmd[],char key[],int tout){
	SIMUSART_putstring(cmd);
	for(int i=0;i<tout;i++){
		if(strstr(buff,key)){
			debugPrint("Command Successful ",cmd,buff);
			return true;
		}
		_delay_ms(1000);
	}
	debugPrint("Error during execution ",cmd,buff);
	
	return false;
}

bool wait4resp(char key[],int tout){
	for(int i=0;i<tout;i++){
		if(strstr(buff,key)){
			return true;
		}
		_delay_ms(1000);
	}
	return false;
}

void GSM_Begin()
{
	while (1)
	{
		SIMUSART_putstring("AT+GSN\r");
		_delay_ms(500);
		if(wait4resp("OK",2))
		{
			char * token = strtok(buff,"\r\n");
			token = strtok(NULL,"\r\n");
			sprintf(imei,"%15s",token);
			debugPrint("Retrieved IMEI",imei,buff);
			break;
		}
		else
		{
			debugPrint("Error Retrieving IMEI","AT+GSN\r",buff);
		}
		_delay_ms(5000);
	}
}

int netcheck(){
	if(!execCmd("AT+COPS?\r","OK",10)) //Check if sim module is registered to network
	return 0;
	
	execCmd("AT+CREG?\r","OK",10); // Check Signal
	
	char cmd[75];
	sprintf(cmd,"AT+CGDCONT=1,\"IP\",\"%s\"\r",apn);
	execCmd(cmd,"OK",10); //Initialize context
	execCmd("AT+CGATT=1\r","OK",10); //attach GPRS connection
	execCmd("AT+CGACT=1,1\r","OK",10); //enable context
	
	return 1;
}

void updateLocation(){
	SIMUSART_putstring("AT+CGPSSTATUS?\r");
	_delay_ms(500);
	
	if(wait4resp("2D Fix",4) || wait4resp("3D Fix",4))
	{
		SIMUSART_putstring("AT+CGPSINF=0\r");
		_delay_ms(2000);
		char * toks = strtok(buff,",");
		toks = strtok(NULL,","); //Ignore this token.

		toks = strtok(NULL,",");//extracting longitude(corrected).
		sprintf(lng,"%9s",toks);

		toks = strtok(NULL,",");//extracting latitude
		sprintf(lat,"%9s",toks);
		debugPrint("GPS location used\r");
	}
	else
	{
		SIMUSART_putstring("AT+CIPGSMLOC=1,1\r");
		_delay_ms(4000);
		if(wait4resp("OK",10))
		{
			char * toks = strtok(buff,",");
			toks = strtok(NULL,","); //Ignore this token.

			toks = strtok(NULL,",");//extracting longitude(corrected).

			sprintf(lng,"%9s",toks);
			toks = strtok(NULL,",");//extracting latitude

			sprintf(lat,"%9s",toks);
			debugPrint("GSM location used\r");

		}
		else
		{
			debugPrint("Error getting location details. Using the hardcoded lat,lng\r");
		}
	}
	
	execCmd("AT+CGNSPWR=0\r","OK",10);
}

void updateBatteryStatus(){
	SIMUSART_putstring("AT+CBC\r");
	_delay_ms(200);
	if(wait4resp("OK",2)){
		char * toks = strtok(buff,",");
		
		toks = strtok(NULL,",");
		sprintf(batlvl,"%3s",toks);
		if(!(batlvl[0]=='1')){
			
			batlvl[0]='0';
		}
		debugPrint("Battery Status updated\n");
	}
	else{
		debugPrint("Error updating Battery Status\n");
	}
}

void prep4Send(char svals[],uint8_t mode)
{
	execCmd("AT+CFUN=1\r","OK",5);
	execCmd("AT+CGNSPWR=1\r","OK",10);
	_delay_ms(10000);
	
	updateBatteryStatus();
	
	while(netcheck()!=1){
		debugPrint("Error during Network Initialization");
		_delay_ms(2000);
	}
	showStatusLED(4);
	
	char cmd[75];
	sprintf(cmd,"\"AT+SAPBR=3,1,\"APN\",\"%s\"\r\"",apn);
	debugPrint(cmd);
	START:
	while(1){
		if(!execCmd("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r","OK",10)){
			execCmd("AT+SAPBR=0,1\r","OK",5);
		}else{
			break;
		}
		_delay_ms(1000);
	}
	
	execCmd(cmd,"OK",10);
	
	if(!execCmd("AT+SAPBR=1,1\r","OK",10)){
		execCmd("AT+SAPBR=0,1\r","OK",5);
		goto START;
	}
	updateLocation();//update lat,lng with latest location from GPS or GSM
	while(1)
	{
		if(!execCmd("AT+HTTPINIT\r","OK",10)){
			execCmd("AT+HTTPTERM\r","OK",5);
		}
		else
		break;
	}
	
	// 	USART_putstring("AT+HTTPSSL=1\r");
	// 	if(!wait4resp("OK",10)){
	// 		USART_putstring("Error\r");
	// 	}
	
	execCmd("AT+HTTPPARA=\"CID\",1\r","OK",10);
	execCmd("AT+HTTPPARA=\"URL\",\"<your URL>\"\r","OK",10);	
	execCmd("AT+HTTPPARA=\"CONTENT\",\"application/json\"\r","OK",10);
	
	char sendStr[100];
	sprintf(sendStr,"{\"data\":\"%s,%s,%s,%s,%s\"}\r",imei,lat,lng,svals,batlvl);
	
	showStatusLED(1);
	
	int len = strlen(sendStr);
	char l[25];
	sprintf(l,"AT+HTTPDATA=%d,100000\r",len);
	
	if(execCmd(l,"DOWNLOAD",15)){
		execCmd(sendStr,"OK",10);
	}else{
		debugPrint("Error when downloading data to server\r");
	}

	int data2Read = 0 ;
	if(!execCmd("AT+HTTPACTION=1\r",",200,",20)){
		if(wait4resp(",400,",30)){
			debugPrint("400 Error\r");
		}
	}else{
		char * toks1 = strtok(buff,",");
		toks1 = strtok(NULL,",");
		toks1 = strtok(NULL,",");
		data2Read = atoi(toks1);
	}
	
	execCmd("AT+HTTPTERM\r","OK",5);
	execCmd("AT+SAPBR=0,1\r","OK",5);
	execCmd("AT+CFUN=0\r","OK",5);
}