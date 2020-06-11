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
#ifndef __WEBCFG_UTIL_H__
#define __WEBCFG_UTIL_H__

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>

/*----------------------------------------------------------------------------*/
/*                                   Macros                                   */
/*----------------------------------------------------------------------------*/
/* none */

/*----------------------------------------------------------------------------*/
/*                               Data Structures                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                             External Functions                             */
/*----------------------------------------------------------------------------*/

uint16_t generateTransactionId();
int readFromFile(char *filename, char **data, int *len);
int writeToFileData(char *db_file_path, char *data, size_t size);
int writeToDBFile(char * db_file_path, char * data, size_t size);
void webcfgStrncpy(char *destStr, const char *srcStr, size_t destSize);
void getCurrent_Time(struct timespec *timer);
long timeVal_Diff(struct timespec *starttime, struct timespec *finishtime);
int writebase64ToDBFile(char *base64_file_path, char *data);
#endif
