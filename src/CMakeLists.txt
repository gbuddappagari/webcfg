#   Copyright 2016 Comcast Cable Communications Management, LLC
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.

set(PROJ_WEBCFG webcfg)
set(HEADERS webcfg.h webcfg_param.h webcfg_pack.h webcfg_multipart.h webcfg_auth.h webcfg_notify.h webcfg_generic.h webcfg_db.h webcfg_log.h webcfg_blob.h webcfg_event.h)
set(SOURCES webcfg_helpers.c webcfg.c webcfg_param.c webcfg_pack.c webcfg_multipart.c webcfg_auth.c webcfg_notify.c webcfg_db.c webcfg_generic.c webcfg_blob.c webcfg_event.c)

add_library(${PROJ_WEBCFG} STATIC ${HEADERS} ${SOURCES})
add_library(${PROJ_WEBCFG}.shared SHARED ${HEADERS} ${SOURCES})
set_target_properties(${PROJ_WEBCFG}.shared PROPERTIES OUTPUT_NAME ${PROJ_WEBCFG})
set_property(TARGET ${PROJ_WEBCFG} PROPERTY C_STANDARD 99)
set_property(TARGET ${PROJ_WEBCFG}.shared PROPERTY C_STANDARD 99)

install (TARGETS ${PROJ_WEBCFG} DESTINATION lib${LIB_SUFFIX})
install (TARGETS ${PROJ_WEBCFG}.shared DESTINATION lib${LIB_SUFFIX})
install (FILES ${HEADERS} DESTINATION include/${PROJ_WEBCFG})
