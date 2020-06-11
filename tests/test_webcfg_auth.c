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
#include <unistd.h>
#include <sys/stat.h>
#include <CUnit/Basic.h>
#include "../src/webcfg_auth.h"
#include <curl/curl.h>

#define UNUSED(x) (void )(x)
char device_mac[32] = {'\0'};
char *sNum= NULL;
/*-------------------------------------------------------------*/
char* get_deviceMAC()
{
	return device_mac;
}

char * getSerialNumber()
{
        return sNum;
}

/*---------------------------------------------------------------*/
void err_getAuthToken()
{
	getAuthToken();
	strncpy(device_mac, "14xxxxxxxxxx", sizeof(device_mac)-1);
	sNum = strdup("");
	getAuthToken();
	sNum = NULL;
	getAuthToken();
}

void test_getAuthToken()
{
	char buffer[128] = {'\0'};
	writeToFileData(WEBPA_READ_HEADER,"printf abcdfqerhbhj", strlen("printf abcdfqerhbhj")+1);
	chmod(WEBPA_READ_HEADER, 0777);
	sNum = strdup("E123b2337");
	getAuthToken();
	writeToFileData(WEBPA_READ_HEADER,"printf ERROR", strlen("printf ERROR")+1);
	writeToFileData(WEBPA_CREATE_HEADER,"printf SUCCESS", strlen("printf SUCCESS")+1);
	chmod(WEBPA_CREATE_HEADER, 0777);
	getAuthToken();
	sprintf(buffer,"rm -rf %s %s",WEBPA_READ_HEADER, WEBPA_CREATE_HEADER);
	system(buffer);	
}

void err_createNewAuthToken()
{
	char buffer[128] = {'\0'};
	sprintf(buffer,"printf ERROR");
	writeToFileData(WEBPA_READ_HEADER,buffer, strlen(buffer)+1);
	chmod(WEBPA_READ_HEADER, 0777);
	getAuthToken();
	sprintf(buffer,"rm -rf %s",WEBPA_READ_HEADER);
	system(buffer);
}

void add_suites( CU_pSuite *suite )
{
    *suite = CU_add_suite( "tests", NULL, NULL );
	CU_add_test( *suite, "Error getAuthToken\n", err_getAuthToken);
    CU_add_test( *suite, "getAuthToken\n", test_getAuthToken);
	CU_add_test( *suite, "Error createNewAuthToken\n", err_createNewAuthToken);
}

/*----------------------------------------------------------------------------*/
/*                             External Functions                             */
/*----------------------------------------------------------------------------*/
int main( )
{
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

