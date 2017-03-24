/*
 * Constants.h
 *
 *  Created on: Mar 24, 2017
 *      Author: coltonp
 */

#ifndef SOURCES_CONSTANTS_H_
#define SOURCES_CONSTANTS_H_

// Unique Q IDS
#define DD_QUEUE 9
#define  TASK_CREATOR_QUEUE 10
#define  TASK_DELETOR_QUEUE 11
#define  ACTIVE_LIST_QUEUE 12
#define  OVERDUE_LIST_QUEUE 13

// these strings are sent as Data
typedef unsigned char * UCHAR_PTR;
const UCHAR_PTR TaskCreatedString = (UCHAR_PTR) "TASK CREATED\n";
const UCHAR_PTR TaskCreatedFailedString = (UCHAR_PTR) "TASK CREATED FAILED\n";
const UCHAR_PTR TaskDeletedString = (UCHAR_PTR) "TASK DELETED\n";
const UCHAR_PTR TaskDeletedFailedString = (UCHAR_PTR) "TASK DELETED FAILED\n";
const UCHAR_PTR ActiveListPassedString = (UCHAR_PTR) "ACTIVE LIST PASS\n";
const UCHAR_PTR ActiveListFailedString = (UCHAR_PTR) "ACTIVE LIST FAILED\n";
const UCHAR_PTR OverdueListPassedString = (UCHAR_PTR) "OVERDUE LIST PASS\n";
const UCHAR_PTR OverdueListFailedString = (UCHAR_PTR) "OVERDUE LIST FAILED\n";


#endif /* SOURCES_CONSTANTS_H_ */
