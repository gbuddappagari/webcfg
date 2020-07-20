/*
 * Copyright 2020 Comcast Cable Communications Management, LLC
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

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include "webcfg.h"
#include "webcfg_log.h"
#include "webcfg_util.h"
/*----------------------------------------------------------------------------*/
/*                                   Macros                                   */
/*----------------------------------------------------------------------------*/
//TODO load fron webcfg.properties file
#define WEBCONFIG_SUPPORTED_DOCS_BIT "0001000000000001|0010000000000001"
/*----------------------------------------------------------------------------*/
/*                               Data Structures                              */
/*----------------------------------------------------------------------------*/
/* none */

/*----------------------------------------------------------------------------*/
/*                            File Scoped Variables                           */
/*----------------------------------------------------------------------------*/
char *bitMap[MAX_GROUP_SIZE][MAX_SUBDOCS_SIZE] = {{"0001","portforwarding","wan", "lan","macbinding","hotspot","bridge"},{"0010","privatessid","homessid","radio"},{"0011","moca"},{"0100","xdns"},{"0101","advsecurity"},{"0110","mesh"},{"0111","aker"},{"1000","telemetry"},{"1001","trafficreport","statusreport"},{"1010","radioreport","interfacereport"}};
/*----------------------------------------------------------------------------*/
/*                             Function Prototypes                            */
/*----------------------------------------------------------------------------*/
int getSubdocGroupId(char *subDoc, char **groupId);
void getSubdDocBitForGroupId(char *groupId, char **subDocBit);
/*----------------------------------------------------------------------------*/
/*                             External Functions                             */
/*----------------------------------------------------------------------------*/
WEBCFG_STATUS isSubDocSupported(char *subDoc)
{
	int pos = 0;
	char *groupId = NULL, *subDocBit = NULL;
	long long docBit = 0;
	pos = getSubdocGroupId(subDoc, &groupId);
	WebcfgDebug("%s is at %d position in %s group\n",subDoc,pos, groupId);
	getSubdDocBitForGroupId(groupId, &subDocBit);
	if(subDocBit != NULL)
	{
		WebcfgDebug("subDocBit: %s\n",subDocBit);
		sscanf(subDocBit,"%lld",&docBit);
		WebcfgDebug("docBit: %lld\n",docBit);
	
		if(docBit & (1 << (pos -1)))
		{
			WebcfgInfo("%s is supported\n",subDoc);
			WEBCFG_FREE(groupId);
			WEBCFG_FREE(subDocBit);
			return WEBCFG_SUCCESS;
		}
	}
	WEBCFG_FREE(groupId);
	if(subDocBit != NULL)
	{
		WEBCFG_FREE(subDocBit);
	}
	WebcfgInfo("%s is not supported\n",subDoc);
	return WEBCFG_FAILURE;
}
/*----------------------------------------------------------------------------*/
/*                             Internal functions                             */
/*----------------------------------------------------------------------------*/
int getSubdocGroupId(char *subDoc, char **groupId)
{
	int position = 0, i = 0, j = 0;
	WebcfgDebug("subDoc: %s\n",subDoc);
	for(i=0; i<MAX_GROUP_SIZE; i++)
	{
		for(j=0;j<MAX_SUBDOCS_SIZE; j++)
		{
			if(bitMap[i][j] == NULL)
			{
				break;
			}
			if(strcmp(bitMap[i][j],subDoc) == 0)
			{
				WebcfgDebug("bitMap[%d][%d]: %s\n",i, j, bitMap[i][j]);
				*groupId = strdup(bitMap[i][0]);
				position = j;
				return position;
			}
		}
	}
	return -1;
}

void getSubdDocBitForGroupId(char *groupId, char **subDocBit)
{
	char *tmpStr=  NULL, *numStr = NULL;
	char bitmap[16] = {'\0'};
	char group[4] = {'\0'};
	char subDoc[12] = {'\0'};
	
	tmpStr = strdup(WEBCONFIG_SUPPORTED_DOCS_BIT);
	while(tmpStr != NULL)
	{
		numStr = strsep(&tmpStr, "|");
		webcfgStrncpy(bitmap, numStr, sizeof(bitmap)+1);
		webcfgStrncpy(group, numStr, sizeof(group)+1);
		webcfgStrncpy(subDoc, numStr+sizeof(group),sizeof(subDoc)+1);
		if(strcmp(group,groupId) == 0)
		{
			WebcfgDebug("bitmap: %s group: %s subDoc: %s\n",bitmap, group,subDoc);
			*subDocBit= strdup(subDoc);
			break;
		}
	}
}

