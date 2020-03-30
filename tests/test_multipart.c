 /**
  * Copyright 2019 Comcast Cable Communications Management, LLC
  *
  * Licensed under the Apache License, Version 2.0 (the "License");
  * you may not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  *     http://www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
 */
#include <stdint.h>
#include <errno.h>
#include <stdio.h>
#include <CUnit/Basic.h>
#include "../src/webcfgparam.h"
#include "../src/webcfg.h"
#include "../src/multipart.h"
#include "../src/helpers.h"
#include "../src/webcfg_db.h"
#include <msgpack.h>
#include <curl/curl.h>
#include <base64.h>
#include "../src/webcfg_generic.h"
#define FILE_URL "/tmp/webcfg_url"

#define UNUSED(x) (void )(x)

char *url = NULL;
char *interface = NULL;

bool _getConfigURL(int index, char **url)
{
	UNUSED(index);
	UNUSED(url);
	return 0;
}

int _getConfigVersion(int index, char **version)
{
	UNUSED(index);
	UNUSED(version);
	return 0;
}
int _setRequestTimeStamp(int index)
{
	UNUSED(index);
	return 0;
}
bool _getRequestTimeStamp(int index,char **RequestTimeStamp)
{
	UNUSED(index);
	UNUSED(RequestTimeStamp);
	return 0;
}
int _setSyncCheckOK(int index, bool status)
{
	UNUSED(index);
	UNUSED(status);
	return 0;
}

char* get_global_deviceMAC()
{
	char *tmp = strdup("b42xxxxxxxxx");
	return tmp;
}
void setValues(const param_t paramVal[], const unsigned int paramCount, const int setType, char *transactionId, money_trace_spans *timeSpan, WDMP_STATUS *retStatus, int *ccspStatus)
{
	UNUSED(paramVal);
	UNUSED(paramCount);
	UNUSED(setType);
	UNUSED(transactionId);
	UNUSED(timeSpan);
	UNUSED(retStatus);
	UNUSED(ccspStatus);
	return;
}

int _setConfigVersion(int index, char *version)
{
	UNUSED(index);
	UNUSED(version);
	return 0;
}

int getForceSync(char** pString, char **transactionId)
{
	UNUSED(pString);
	UNUSED(transactionId);
	return 0;
}
int setForceSync(char* pString, char *transactionId,int *session_status)
{
	UNUSED(pString);
	UNUSED(transactionId);
	UNUSED(session_status);
	return 0;
}

void getDeviceMac()
{
	return;
}

char * getParameterValue(char *paramName)
{
	UNUSED(paramName);
	return NULL;
}

char * getSerialNumber()
{
	return NULL;
}

char * getDeviceBootTime()
{
	return NULL;
}

char * getProductClass()
{
	return NULL;
}

char * getModelName()
{
	return NULL;
}

char * getFirmwareVersion()
{
	return NULL;
}

void sendNotification(char *payload, char *source, char *destination)
{
	UNUSED(payload);
	UNUSED(source);
	UNUSED(destination);
	return;
}

char *get_global_systemReadyTime()
{
	return NULL;
}

int Get_Webconfig_URL( char **pString)
{
	char *webConfigURL =NULL;
	loadInitURLFromFile(&webConfigURL);
	*pString = webConfigURL;
	return 0;
}

int Set_Webconfig_URL( char *pString)
{
	UNUSED(pString);
	return 0;
}

void test_multipart()
{
	int configRet = 0;
	char *doc = NULL;

	if(url == NULL)
	{
		printf("\nProvide config URL as argument\n");
		return;
	}
	processWebconfgSync(doc);
	if(configRet == 0)
	{
		printf("config ret success\n");
	}	
	else
	{
		printf("webcfg_http_request failed\n");
	}
}

void add_suites( CU_pSuite *suite )
{
    *suite = CU_add_suite( "tests", NULL, NULL );
    CU_add_test( *suite, "Full", test_multipart);
}

/*----------------------------------------------------------------------------*/
/*                             External Functions                             */
/*----------------------------------------------------------------------------*/
int main( int argc, char *argv[] )
{
	unsigned rv = 1;
	CU_pSuite suite = NULL;
	// int len=0;
	printf("argc %d \n", argc );
	if(argv[1] !=NULL)
	{
		url = strdup(argv[1]);
	}
	// Read url from file
	//readFromFile(FILE_URL, &url, &len );
	if(url !=NULL && strlen(url)==0)
	{
		printf("<url> is NULL.. add url in /tmp/webcfg_url file\n");
		return 0;
	}
	printf("url fetched %s\n", url);
	if(argv[2] !=NULL)
	{
		interface = strdup(argv[2]);
	}
	if( CUE_SUCCESS == CU_initialize_registry() ) {
	add_suites( &suite );

	if( NULL != suite ) {
	    CU_basic_set_mode( CU_BRM_VERBOSE );
	    CU_basic_run_tests();
	    printf( "\n" );
	    CU_basic_show_failures( CU_get_failure_list() );
	    printf( "\n\n" );
	    rv = CU_get_number_of_tests_failed();
	}

	CU_cleanup_registry();

	}
	return rv;
}

