#ifndef _ESBLEKEYDEFINES_H_
#define _ESBLEKEYDEFINES_H_

// Defines for the ESBLEKey peripheral


#define TI_SIMPLEPROFILE_SERVICE_UUID                       0xFFF2
#define TI_SIMPLEPROFILE_CHAR1_UUID                         0xFFF1
#define TI_SIMPLEPROFILE_CHAR2_UUID                         0xFFF2
#define TI_SIMPLEPROFILE_CHAR3_UUID                         0xFFF3
#define TI_SIMPLEPROFILE_CHAR4_UUID                         0xFFF4
#define TI_SIMPLEPROFILE_CHAR5_UUID                         0xFFF5
#define TI_SIMPLEPROFILE_CHAR6_UUID                         0xFFF6
#define TI_SIMPLEPROFILE_CHAR7_UUID                         0xFFF7
#define TI_SIMPLEPROFILE_CHAR8_UUID                         0xFFF8

#define NOTIFY_FLAG_CHAR4   (1<<4)
#define NOTIFY_FLAG_CHAR5   (1<<5)
#define NOTIFY_FLAG_CHAR6   (1<<6)
#define NOTIFY_FLAG_CHAR7   (1<<7)
#define NOTIFY_FLAG_CHAR8   (1<<8)


#define NOTIFY_FLAG_TRANSTXCHAR   (1<<9)
#define NOTIFY_FLAG_CONFIGTXCHAR   (1<<10)
#define FLAG_GOT_AUTH_STATUS   (1<<11)
#define FLAG_DISCOVER_SERVICE  (1<<12)

//GAP
#define UUIDSTR_GAP_SERVICE @"1800"

//Device Info service
#define UUIDSTR_DEVICE_INFO_SERVICE             @"180A"
#define UUIDSTR_MANUFACTURE_NAME_CHAR           @"2A29"
#define UUIDSTR_MODEL_NUMBER_CHAR               @"2A24"
#define UUIDSTR_SERIAL_NUMBER_CHAR              @"2A25"
#define UUIDSTR_HARDWARE_REVISION_CHAR          @"2A27"
#define UUIDSTR_FIRMWARE_REVISION_CHAR          @"2A26"
#define UUIDSTR_SOFTWARE_REVISION_CHAR          @"2A28"
#define UUIDSTR_SYSTEM_ID_CHAR                  @"2A23"
#define UUIDSTR_IEEE_11073_20601_CHAR           @"2A2A"

#define UUIDSTR_ISSC_PROPRIETARY_SERVICE        @"49535343-FE7D-4AE5-8FA9-9FAFD205E455"
#define UUIDSTR_CONNECTION_PARAMETER_CHAR       @"49535343-6DAA-4D02-ABF6-19569ACA69FE"
#define UUIDSTR_AIR_PATCH_CHAR                  @"49535343-ACA3-481C-91EC-D85E28A60318"
#define UUIDSTR_ISSC_TRANS_TX                   @"49535343-1E4D-4BD9-BA61-23C647249616"
#define UUIDSTR_ISSC_TRANS_RX                   @"49535343-8841-43F4-A8D4-ECBE34729BB3"
#define UUIDSTR_ISSC_MP                         @"49535343-ACA3-481C-91EC-D85E28A60318"

// Es Service
#define UUIDSTR_ES_PROPRIETARY_SERVICE        @"FFC1"
#define UUIDSTR_ES_TRANS_TX                   @"FFC3"
#define UUIDSTR_ES_TRANS_RX                   @"FFC1"
#define UUIDSTR_ES_CONFIG_TX                  @"FFC4"
#define UUIDSTR_ES_CONFIG_RX                  @"FFC2"

// Es Service_long
#define UUIDSTR_ES_PROPRIETARY_SERVICE_LONG        @"3046FFC1-dac4-457f-afda-f2ad97f48a43"
#define UUIDSTR_ES_TRANS_TX_LONG                   @"3046FFC3-dac4-457f-afda-f2ad97f48a43"
#define UUIDSTR_ES_TRANS_RX_LONG                   @"3046FFC1-dac4-457f-afda-f2ad97f48a43"
#define UUIDSTR_ES_CONFIG_TX_LONG                  @"3046FFC4-dac4-457f-afda-f2ad97f48a43"
#define UUIDSTR_ES_CONFIG_RX_LONG                  @"3046FFC2-dac4-457f-afda-f2ad97f48a43"

//CBCentralManagerOptionRestoreIdentifierKey
#define ISSC_RestoreIdentifierKey               @"ISSC_RestoreIdentifierKey"

#endif

