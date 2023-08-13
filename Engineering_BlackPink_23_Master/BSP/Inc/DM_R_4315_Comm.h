#ifndef __DM_R_4315_COMM_H__
#define __DM_R_4315_COMM_H__

/**Include Header Files**/
#include "sysconfig.h"
#include "DM_R_4315.h"

#include "Motion_Motor.h"

/**Macro Definition**/
#define DMR4315_HUART    					huart8
#define DMR4315Receive_Size				40
#define DataHeader_Size						4
#define CRCLength_Size						2
#define DataLength_Offset					4
#define CommandID_Offset					3


/**Struct Definition**/

	
	
	
	

/**Extern Variables**/
	
	
/**Function Delaration**/
void BSP_Init_DMR4315Comm(void);
void DMR4315Comm_CallBack(UART_HandleTypeDef *huart);
void DMR4315_dataHandle(uint8_t len);



#endif
