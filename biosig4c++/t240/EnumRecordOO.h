/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "FEF-IntermediateDraft"
 * 	found in "../annexb-snacc-122001.asn1"
 */

#ifndef	_EnumRecordOO_H_
#define	_EnumRecordOO_H_


#include <asn_application.h>

/* Including external dependencies */
#include "OID-Type.h"
#include <ANY.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* EnumRecordOO */
typedef struct EnumRecordOO {
	OID_Type_t	 record_type_code;
	ANY_t	 record_data;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} EnumRecordOO_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_EnumRecordOO;

#ifdef __cplusplus
}
#endif

#endif	/* _EnumRecordOO_H_ */
