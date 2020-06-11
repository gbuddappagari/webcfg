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
#include <getopt.h>
#include <CUnit/Basic.h>
#include "../src/webcfg_param.h"
#include "../src/webcfg.h"
#include "../src/webcfg_multipart.h"
#include "../src/webcfg_helpers.h"
#include "../src/webcfg_db.h"
#include "../src/webcfg_notify.h"
#include <msgpack.h>
#include <curl/curl.h>
#include <base64.h>
#include "../src/webcfg_generic.h"
#include "../src/webcfg_event.h"
#define FILE_URL "/tmp/webcfg_url"

#define UNUSED(x) (void )(x)

char *url = NULL;
char webpa_token[4096]={'\0'};
char device_mac[32] = {'\0'};
WDMP_STATUS globalStatus = WDMP_FAILURE;
char* get_deviceMAC()
{
	return device_mac;
}
void setValues(const param_t paramVal[], const unsigned int paramCount, const int setType, char *transactionId, money_trace_spans *timeSpan, WDMP_STATUS *retStatus, int *ccspStatus)
{
	UNUSED(paramVal);
	UNUSED(paramCount);
	UNUSED(setType);
	UNUSED(transactionId);
	UNUSED(timeSpan);
	*retStatus = globalStatus;
	UNUSED(ccspStatus);
	return;
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

char * getParameterValue(char *paramName)
{
	UNUSED(paramName);
	return NULL;
}

char * getSerialNumber()
{
	char *sNum = strdup("1234");
	return sNum;
}

char * getDeviceBootTime()
{
	char *bTime = strdup("152200345");
	return bTime;
}

char * getProductClass()
{
	char *pClass = strdup("Product");
	return pClass;
}

char * getModelName()
{
	char *mName = strdup("Model");
	return mName;
}

char * getFirmwareVersion()
{
	char *fName = strdup("Firmware.bin");
	return fName;
}

void sendNotification(char *payload, char *source, char *destination)
{
	WEBCFG_FREE(payload);
	WEBCFG_FREE(source);
	UNUSED(destination);
	return;
}

char *get_global_systemReadyTime()
{
	char *sTime = strdup("158000123");
	return sTime;
}

int Get_Webconfig_URL( char *pString)
{
	strcpy(pString,url);
    printf("The value of pString is %s\n",pString);
	return 0;
}

int Set_Webconfig_URL( char *pString)
{
	printf("Set_Webconfig_URL pString %s\n", pString);
	url = strdup(pString);
	return 0;
}

int registerWebcfgEvent(WebConfigEventCallback webcfgEventCB)
{
	UNUSED(webcfgEventCB);
	return 0;
}

void createNewAuthToken(char *newToken, size_t len, char *hw_mac, char* hw_serial_number)
{
	UNUSED(newToken);
	UNUSED(len);
	UNUSED(hw_mac);
	UNUSED(hw_serial_number);
}

void getAuthToken()
{
}

char* get_global_auth_token()
{
    return webpa_token;
}

char* get_global_serialNum()
{
    return strdup("E1234B");
}

int parse_commandline_arguments(int argc, char **argv)
{
	if(argc < 5)
	{
		fprintf(stderr,"Usage: %s --url=<url> --mac=<mac> --token-path=<token-path> --interface=<interface>\n\n", argv[0]);
		return 0;
	}

	static const struct option long_options[] = {
        {"url",      required_argument, 0, 'u'},
        {"mac",      required_argument, 0, 'm'},
		{"token-path",      required_argument, 0, 't'},
		{"interface",      required_argument, 0, 'i'},
        {0, 0, 0, 0}
    };
	int c;
	optind = 1;
	char buf[128] = {'\0'};
	while (1)
	{
		/* getopt_long stores the option index here. */
		int option_index = 0;
		c = getopt_long (argc, argv, "u:m:i:t",
		long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1)
			break;

		switch (c)
		{
			case 'u':
				url = strdup(optarg);
			break;
			case 'm':
				strncpy(device_mac, optarg, sizeof(device_mac)-1);
			break;
			case 't':
			{
				int len = 0;
				char *token = NULL;
				readFromFile(optarg, &token, &len);
				strncpy(webpa_token, token, sizeof(webpa_token)-1);
				WEBCFG_FREE(token);
			}
			break;
			case 'i':
				snprintf(buf,sizeof(buf),"WEBCONFIG_INTERFACE=%s ",optarg);
				writeToDBFile(DEVICE_PROPS_FILE, buf, strlen(buf));
			break;
			case '?':
			/* getopt_long already printed an error message. */
			break;

			default:
				printf("Enter Valid arguments..\n");
			return -1;
		}
	}
	return 1;
}
void test_multipart()
{
	unsigned long status = 0;
	globalStatus = WDMP_SUCCESS;
	initWebConfigNotifyTask();
	processWebcfgEvents();
	initEventHandlingTask();
	processWebconfgSync(status);
}

void test_multipart1()
{
	unsigned long status = 0;
	globalStatus = WDMP_FAILURE;
	initWebConfigNotifyTask();
	processWebcfgEvents();
	initEventHandlingTask();
	processWebconfgSync(status);
}
void err_multipart()
{
	unsigned long status = 0;
	url = "";
	initWebConfigNotifyTask();
	processWebcfgEvents();
	initEventHandlingTask();
	processWebconfgSync(status);
}

void add_suites( CU_pSuite *suite )
{
	*suite = CU_add_suite( "tests", NULL, NULL );
	CU_add_test( *suite, "WebConfig sync with successful SET\n", test_multipart);
	CU_add_test( *suite, "WebConfig sync with failed SET\n", test_multipart1);
	CU_add_test( *suite, "WebConfig sync\n", err_multipart);
}

/*----------------------------------------------------------------------------*/
/*                             External Functions                             */
/*----------------------------------------------------------------------------*/
int main( int argc, char *argv[] )
{
	if(!parse_commandline_arguments(argc, argv))
	{
		return 0;
	}
	unsigned rv = 1;
	CU_pSuite suite = NULL;
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

