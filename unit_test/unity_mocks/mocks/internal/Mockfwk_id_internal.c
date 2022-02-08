/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "cmock.h"
#include "internal/Mockfwk_id_internal.h"

static const char* CMockString___fwk_id_str = "__fwk_id_str";
static const char* CMockString_id = "id";

typedef struct _CMOCK___fwk_id_str_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  char ExpectAnyArgsBool;
  struct __fwk_id_fmt ReturnVal;
  union __fwk_id Expected_id;

} CMOCK___fwk_id_str_CALL_INSTANCE;

static struct Mockfwk_id_internalInstance
{
  CMOCK_MEM_INDEX_TYPE __fwk_id_str_CallInstance;
} Mock;

extern jmp_buf AbortFrame;

void Mockfwk_id_internal_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_MEM_INDEX_TYPE call_instance;
  call_instance = Mock.__fwk_id_str_CallInstance;
  if (CMOCK_GUTS_NONE != call_instance)
  {
    UNITY_SET_DETAIL(CMockString___fwk_id_str);
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledLess);
  }
}

void Mockfwk_id_internal_Init(void)
{
  Mockfwk_id_internal_Destroy();
}

void Mockfwk_id_internal_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
}

struct __fwk_id_fmt __fwk_id_str(union __fwk_id id)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK___fwk_id_str_CALL_INSTANCE* cmock_call_instance;
  UNITY_SET_DETAIL(CMockString___fwk_id_str);
  cmock_call_instance = (CMOCK___fwk_id_str_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.__fwk_id_str_CallInstance);
  Mock.__fwk_id_str_CallInstance = CMock_Guts_MemNext(Mock.__fwk_id_str_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringCalledMore);
  cmock_line = cmock_call_instance->LineNumber;
  if (!cmock_call_instance->ExpectAnyArgsBool)
  {
  {
    UNITY_SET_DETAILS(CMockString___fwk_id_str,CMockString_id);
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_id), (void*)(&id), sizeof(union __fwk_id), cmock_line, CMockStringMismatch);
  }
  }
  UNITY_CLR_DETAILS();
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters___fwk_id_str(CMOCK___fwk_id_str_CALL_INSTANCE* cmock_call_instance, union __fwk_id id);
void CMockExpectParameters___fwk_id_str(CMOCK___fwk_id_str_CALL_INSTANCE* cmock_call_instance, union __fwk_id id)
{
  memcpy((void*)(&cmock_call_instance->Expected_id), (void*)(&id),
         sizeof(union __fwk_id[sizeof(id) == sizeof(union __fwk_id) ? 1 : -1])); /* add union __fwk_id to :treat_as_array if this causes an error */
}

void __fwk_id_str_CMockExpectAnyArgsAndReturn(UNITY_LINE_TYPE cmock_line, struct __fwk_id_fmt cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK___fwk_id_str_CALL_INSTANCE));
  CMOCK___fwk_id_str_CALL_INSTANCE* cmock_call_instance = (CMOCK___fwk_id_str_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.__fwk_id_str_CallInstance = CMock_Guts_MemChain(Mock.__fwk_id_str_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ExpectAnyArgsBool = (char)0;
  cmock_call_instance->ReturnVal = cmock_to_return;
  cmock_call_instance->ExpectAnyArgsBool = (char)1;
}

void __fwk_id_str_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, union __fwk_id id, struct __fwk_id_fmt cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK___fwk_id_str_CALL_INSTANCE));
  CMOCK___fwk_id_str_CALL_INSTANCE* cmock_call_instance = (CMOCK___fwk_id_str_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.__fwk_id_str_CallInstance = CMock_Guts_MemChain(Mock.__fwk_id_str_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ExpectAnyArgsBool = (char)0;
  CMockExpectParameters___fwk_id_str(cmock_call_instance, id);
  memcpy((void*)(&cmock_call_instance->ReturnVal), (void*)(&cmock_to_return),
         sizeof(struct __fwk_id_fmt[sizeof(cmock_to_return) == sizeof(struct __fwk_id_fmt) ? 1 : -1])); /* add struct __fwk_id_fmt to :treat_as_array if this causes an error */
}

