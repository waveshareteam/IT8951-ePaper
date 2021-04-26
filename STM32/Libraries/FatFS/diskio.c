/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

#include "diskio.h"
#include "sdcard.h"

/*-----------------------------------------------------------------------*/
/* Correspondence between physical drive number and physical drive.      */

#define BlockSize            512 /* Block Size in Bytes */
#define SD_Mode				0		//0 dma,1 interrupt

static SD_CardInfo SDCardInfo;

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */

DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{
    SD_Error Status;

	if(drv==0)
	{
    	Status = SD_Init();
    	
    	if(Status != SD_OK)
    	{
        	return STA_NOINIT; //其他错误：初始化失败
    	}
    	else
    	{
	  		Status = SD_GetCardInfo(&SDCardInfo); //读sd卡信息

	   		if (Status != SD_OK)
	    	{
	     		return  STA_NOINIT;//RES_NOTRDY;  //报NOT READY错误
	        }
	              // Select Card 
	     	Status = SD_SelectDeselect((u32) (SDCardInfo.RCA << 16));

	     	if (Status != SD_OK)
	     	{
	        	return  STA_NOINIT;//RES_NOTRDY;  //报NOT READY错误
	     	}

	   		switch(SD_Mode)
	   		{
	    		case 0:  //dma方式
		    		Status = SD_EnableWideBusOperation(SDIO_BusWide_4b);
		       		if (Status != SD_OK)
		        	{  
		         		return RES_NOTRDY;  //报NOT READY错误
		        	}

		       		Status = SD_SetDeviceMode(SD_DMA_MODE);
		      		if (Status != SD_OK)
		      		{
		         		return RES_NOTRDY;  //报NOT READY错误
		      		}
		      		break;

	    		case 1:  //中断方式
		       		Status = SD_EnableWideBusOperation(SDIO_BusWide_4b);
		       		if (Status != SD_OK)
		        	{  
		         		return RES_NOTRDY;  //报NOT READY错误
		        	}
		        	
		       		Status = SD_SetDeviceMode(SD_INTERRUPT_MODE);  
		      		if (Status != SD_OK)
		      		{
		         		return RES_NOTRDY;  //报NOT READY错误
		      		}
		       		break;
	       		
	     		default :
	     			return RES_NOTRDY;
	   		}

	 		return 0;           //初始化成功
	 	}
    }
    else//仅支持磁盘0的操作
    {
		return STA_NOINIT;  
    }
}

/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{
	 if(drv==0)
	 {
	     return 0;               //初始化成功
	 }
	 else    //仅支持磁盘0的操作
	 {
		return STA_NOINIT;  
	 }

} 

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..255) */
)
{
	SD_Error Status; 

	if (!count) return RES_PARERR;  //count不能等于0，否则返回参数错误

	if(drv==0)
	{		
 		switch(SD_Mode)
	  	{
	   		case 0:  //dma方式
	      		if(count==1)// 1个sector的读操作      
	      		{      
	    			Status = SD_ReadBlock(sector << 9,(u32 *)(&buff[0]),BlockSize);//sector<<9 扇区地址转为字节地址 一个扇区512字节                                    
	      		}                                                
	      		else                    //多个sector的读操作     
	      		{    
	       			Status = SD_ReadMultiBlocks(sector << 9,(u32 *)(&buff[0]),BlockSize,count);                                      
	      		}
	     		break;
	     		
	  		case 1:  //中断方式
	      		if(count==1)            // 1个sector的读操作      
	      		{      
	    			Status = SD_ReadBlock(sector<<9,(u32 *)(&buff[0]),BlockSize);                                              
	      		}                                                
	      		else                    //多个sector的读操作     
	      		{    
	       			Status = SD_ReadMultiBlocks(sector<<9 ,(u32 *)(&buff[0]),BlockSize,count);                                     
	      		}  	  
	      		break;
	      		
	    	default:
	    		Status=SD_ERROR;
		}
		
	    //处理返回值，将sdcard.c的返回值转成ff.c的返回值
	    if(Status == SD_OK)
	        return RES_OK;
	    else
	        return RES_ERROR;
	}
	else//仅支持磁盘0的操作
    {
		return RES_ERROR;  
    }
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..255) */
)
{
   SD_Error Status;
  
    if (!count)  return RES_PARERR;  //count不能等于0，否则返回参数错误

	if(drv==0)
	{
 		switch(SD_Mode)
  		{
   			case 0:  //dma方式
	      		if(count==1)            // 1个sector的写操作      
	      		{      
	          		Status = SD_WriteBlock(sector << 9,(u32 *)(&buff[0]),BlockSize);//sector<<9 扇区地址转为字节地址 一个扇区512字节                                             
	      		}                                                
	     	 	else                    //多个sector的写操作     
	      		{    
	          		Status =SD_WriteMultiBlocks(sector << 9,(u32 *)(&buff[0]),BlockSize,count);                                          
	      		}  	  
				break;

   			case 1:  //中断方式
				if(count==1)           // 1个sector的写操作      
      			{      
    				Status = SD_WriteBlock(sector << 9 ,(u32 *)(&buff[0]),BlockSize);                                            
      			}                                                
      			else                    //多个sector的写操作     
      			{    
       				Status = SD_WriteMultiBlocks(sector << 9 ,(u32 *)(&buff[0]),BlockSize,count);                                     
      			}  
				break;

			default :
				Status=SD_ERROR;
		}
                                        
	    //处理返回值，将sdcard.c的返回值转成ff.c的返回值
	    if(Status == SD_OK)
	        return RES_OK;
	    else
	        return RES_ERROR;
	}
	else//仅支持磁盘0的操作
    {
		return RES_ERROR;  
    }
}
#endif /* _READONLY */

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	u32 x, y, z;
    DRESULT res;
	
    if (drv==0)
    {
    	//FATFS目前版本仅需处理CTRL_SYNC，GET_SECTOR_COUNT，GET_BLOCK_SIZ三个命令
    	switch(ctrl)
    	{
     		case CTRL_SYNC:
				if(SD_GetTransferState()==SD_NO_TRANSFER)
         		{
             		res = RES_OK;
         		}
         		else
         		{
             		res = RES_ERROR;
         		}
         		break;
        
     		case GET_BLOCK_SIZE:
				*(WORD*)buff = BlockSize;
         		res = RES_OK;
         		break;

     		case GET_SECTOR_COUNT:     //读卡容量
		        ////formula of the capacity///////////////
		        //
		        //  memory capacity = BLOCKNR * BLOCK_LEN
		        // 
		        // BLOCKNR = (C_SIZE + 1)* MULT
		        //
		        //           C_SIZE_MULT+2
		        // MULT = 2
		        //
		        //               READ_BL_LEN
		        // BLOCK_LEN = 2
		    	//////////////////////////////////////////
     			if (SD_GetCardInfo(&SDCardInfo)==SD_OK)//读sd卡信息
         		{
          			x=SDCardInfo.SD_csd.DeviceSize+1; //C_SIZE + 1
    				y=SDCardInfo.SD_csd.DeviceSizeMul+2; //C_SIZE_MULT+2
    				z=SDCardInfo.SD_csd.RdBlockLen+y;
       				*(DWORD*)buff =x<<z; 
					res = RES_OK;
				}
				else
				{
					res = RES_ERROR ;
				}
				break;

     		default:
				res = RES_PARERR;
    	}
    	return res;
    }
    else	//仅支持磁盘0的操作
    {
		return RES_ERROR;  
    }
}


/*-----------------------------------------------------------------------*/
/* User defined to give a current time to fatfs module          */
/* 31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31)     */                                                                                                                                                                                                                                          
/* 15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2)           */ 
/*-----------------------------------------------------------------------*/                                                                                                                                                                                                                                               
DWORD get_fattime (void)
{
#if 0
    struct tm t;
    DWORD date;
    
    t = Time_GetCalendarTime();
    t.tm_year -= 1980;  //年份改为1980年起
    t.tm_mon++;          //0-11月改为1-12月
    t.tm_sec /= 2;       //将秒数改为0-29
    
    date = 0;
    date = (t.tm_year << 25) | (t.tm_mon<<21) | (t.tm_mday<<16)|\
            (t.tm_hour<<11) | (t.tm_min<<5) | (t.tm_sec);

    return date;
#else
	return 0;
#endif
}


