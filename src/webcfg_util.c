/*
 * Copyright 2019 Comcast Cable Communications Management, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <base64.h>

#include "webcfg_log.h"
#include "webcfg.h"
#include "webcfg_util.h"

/*----------------------------------------------------------------------------*/
/*                                   Macros                                   */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                               Data Structures                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                            File Scoped Variables                           */
/*----------------------------------------------------------------------------*/
/* none */

/*----------------------------------------------------------------------------*/
/*                             Function Prototypes                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                             External Functions                             */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                             Internal functions                             */
/*----------------------------------------------------------------------------*/

uint16_t generateTransactionId()
{
    FILE *fp;
	uint16_t random_key,sz;
	fp = fopen("/dev/urandom", "r");
	if (fp == NULL){
    		return 0;
    	}    	
	sz = fread(&random_key, sizeof(random_key), 1, fp);
	if (!sz)
	{	
		fclose(fp);
		WebcfgError("fread failed.\n");
		return 0;
	}
	WebcfgDebug("generateTransactionId\n %d",random_key);
	fclose(fp);		
	return(random_key);
}

int writebase64ToDBFile(char *base64_file_path, char *data)
{
	FILE *fp;
	fp = fopen(base64_file_path , "w+");
	if (fp == NULL)
	{
		WebcfgError("Failed to open base64_file in db %s\n", base64_file_path);
		return 0;
	}
	if(data !=NULL)
	{
		fwrite(data, strlen(data), 1, fp);
		fclose(fp);
		return 1;
	}
	else
	{
		WebcfgError("WriteToJson failed, Data is NULL\n");
		fclose(fp);
		return 0;
	}
}

int writeToDBFile(char *db_file_path, char *data, size_t size)
{
	FILE *fp;
	fp = fopen(db_file_path , "w+");
	if (fp == NULL)
	{
		WebcfgError("Failed to open file in db %s\n", db_file_path );
		return 0;
	}
	if(data !=NULL)
	{
		fwrite(data, size, 1, fp);
		fclose(fp);
		return 1;
	}
	else
	{
		WebcfgError("WriteToJson failed, Data is NULL\n");
		fclose(fp);
		return 0;
	}
}

int writeToFileData(char *db_file_path, char *data, size_t size)
{
	FILE *fp;
	fp = fopen(db_file_path , "w+");
	if (fp == NULL)
	{
		WebcfgError("Failed to open file in db %s\n", db_file_path );
		return 0;
	}
	if(data !=NULL)
	{
		fwrite(data, size, 1, fp);
		fclose(fp);
		return 1;
	}
	else
	{
		WebcfgError("WriteToJson failed, Data is NULL\n");
		fclose(fp);
		return 0;
	}
}

void webcfgStrncpy(char *destStr, const char *srcStr, size_t destSize)
{
    strncpy(destStr, srcStr, destSize-1);
    destStr[destSize-1] = '\0';
}

void getCurrent_Time(struct timespec *timer)
{
	clock_gettime(CLOCK_REALTIME, timer);
}

long timeVal_Diff(struct timespec *starttime, struct timespec *finishtime)
{
	long msec;
	msec=(finishtime->tv_sec-starttime->tv_sec)*1000;
	msec+=(finishtime->tv_nsec-starttime->tv_nsec)/1000000;
	return msec;
}

int readFromFile(char *filename, char **data, int *len)
{
	FILE *fp;
	size_t sz;
	int ch_count = 0;
	fp = fopen(filename, "r+");
	if (fp == NULL)
	{
		WebcfgError("Failed to open file %s\n", filename);
		return 0;
	}
	fseek(fp, 0, SEEK_END);
	ch_count = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	*data = (char *) malloc(sizeof(char) * (ch_count + 1));
	sz = fread(*data, 1, ch_count-1,fp);
	if (!sz) 
	{	
		fclose(fp);
		WebcfgError("fread failed.\n");
		WEBCFG_FREE(*data);
		return WEBCFG_FAILURE;
	}
	*len = ch_count;
	(*data)[ch_count] ='\0';
	fclose(fp);
	return 1;
}
