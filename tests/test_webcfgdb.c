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

#define UNUSED(x) (void )(x)

char *url = NULL;
char *interface = NULL;
char device_mac[32] = {'\0'};

uint32_t get_global_root()
{
	return 0;
}

void test_initDB()
{
	int ret = 0;
	printf("-------------- case 1 ------------------------\n");
	ret = initDB("/tmp/webconfig_db.bin");
	CU_ASSERT_EQUAL(ret, WEBCFG_SUCCESS);
	printf("-------------- case 2 ------------------------\n");
	ret = initDB("/tmp/webconfig_db.bin");
	CU_ASSERT_EQUAL(ret, WEBCFG_SUCCESS);
	printf("-------------- case 3 ------------------------\n");
	ret = initDB("/tmp/webcfg.bin");
	CU_ASSERT_EQUAL(ret, WEBCFG_FAILURE);
	printf("-------------- case 4------------------------\n");
	system("touch /tmp/webcfg1.bin");
	ret = initDB("/tmp/webcfg1.bin");
	CU_ASSERT_EQUAL(ret, WEBCFG_SUCCESS);
	printf("--------------------------------------\n");
}

void add_suites( CU_pSuite *suite )
{
    *suite = CU_add_suite( "tests", NULL, NULL );
    CU_add_test( *suite, "initDB\n", test_initDB);
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

