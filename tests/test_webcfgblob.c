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
#include "../src/webcfg_util.h"
#include "../src/webcfg_blob.h"
#include <msgpack.h>
#include <base64.h>
#include <curl/curl.h>

#define UNUSED(x) (void )(x)

static char *decodeBlobData(char *data)
{
	int size = 0;
	char *decodedData = NULL;
    size = b64_get_decoded_buffer_size(strlen(data));
    decodedData = (char *) malloc(sizeof(char) * size);
    if(decodedData)
    {
		memset( decodedData, 0, sizeof(char) *  size );
		size = b64_decode( (const uint8_t *)data, strlen(data), (uint8_t *)decodedData );
	}
	return decodedData;
}

char * base64blobencoder(char * blob_data, size_t blob_size )
{
        char* b64buffer =  NULL;
        size_t encodeSize = -1;
        printf("Data is %s\n", blob_data);
        printf("-----------Start of Base64 Encode ------------\n");
        encodeSize = b64_get_encoded_buffer_size(blob_size);
        printf("encodeSize is %zu\n", encodeSize);
        b64buffer = malloc(encodeSize + 1);
        if(b64buffer != NULL)
        {
            memset( b64buffer, 0, sizeof( encodeSize )+1 );

            b64_encode((uint8_t *)blob_data, blob_size, (uint8_t *)b64buffer);
            b64buffer[encodeSize] = '\0' ;
        }
        return b64buffer;
}

//char * webcfg_appendeddoc(char * subdoc_name, uint32_t version, char * blob_data, size_t blob_size, uint16_t *trans_id)
void test_webcfgAppendeddoc()
{
	void *docData = NULL;
	char *blobData = NULL;
	uint16_t transId = -1;
	docData = webcfg_appendeddoc("subdoc1",13546,NULL, 0, &transId);
	CU_ASSERT_PTR_NOT_NULL(docData);
	blobData = decodeBlobData(docData);
	CU_ASSERT_PTR_NOT_NULL(blobData);
	printf("blobData: %s\n",blobData);
	docData = webcfg_appendeddoc("subdoc2",23546,blobData, strlen(blobData)+1, &transId);
	CU_ASSERT_PTR_NOT_NULL(docData);
	blobData = decodeBlobData(docData);
	CU_ASSERT_PTR_NOT_NULL(blobData);
	printf("blobData: %s\n",blobData);
	docData = webcfg_appendeddoc("subdoc3",43546,blobData, strlen(blobData)+1, &transId);
	CU_ASSERT_PTR_NOT_NULL(docData);
}

void err_webcfg_pack_appenddoc()
{
	webcfg_pack_appenddoc(NULL,NULL);
}
void add_suites( CU_pSuite *suite )
{
    *suite = CU_add_suite( "tests", NULL, NULL );
    CU_add_test( *suite, "webcfgAppendeddoc\n", test_webcfgAppendeddoc);
	CU_add_test( *suite, "webcfg_pack_appenddoc\n", err_webcfg_pack_appenddoc);
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

