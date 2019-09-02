/// @file EsLogViewer.h
/// @brief This file contains functions to get log from the shm
///
// @author excelsecu
///
// ============================================================================

#ifndef _ES_LOG_VIEWER_H_
#define _ES_LOG_VIEWER_H_

#include <EsTypeDef.h>
#include <EsCommonInner/ILogger.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
    
    // @brief connect to logger as logviewer
    //
    // @return 0 succeeded, otherwise failed
    //
    // @note one process can only conect once
    u4 CALL_TYPE EsLogViewerConnect();
    
    // @brief disconnect to logger as logviewer
    //
    // @return 0 succeeded, otherwise failed
    //
    // @note one process can only conect once
    u4 CALL_TYPE EsLogViewerDisconnect();
    
    // @brief set heartbeats for keepping alive
    //
    // @note LogViewer must call this function periodically,
    //       and the interval should be less than 10ms
    void CALL_TYPE EsLogViewerKeepAlive();
    
    // @brief get log from logger
    // @param [in] pblkLogItems buffer for log
    // @param [in] u4Count count of the pblkLogItems to get
    //
    // @return the count of the logitems got, if 0, failed or no any log
    u4 CALL_TYPE EsLogViewerGet(BlkLogItem* pblkLogItems, u4 u4Count);
    
    // @brief pop some elements at the head of the queue and discard
    // @param u4Count count of the elements to discard, if elements sum < u4Number, than discard
    //                all, if u4Count=0, than discard all elements
    //
    // @returns 0 succeeded, otherwise failed
    u4 CALL_TYPE EsLogViewerDiscard(u4 u4Count);
    
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _ES_LOG_VIEWER_H_
