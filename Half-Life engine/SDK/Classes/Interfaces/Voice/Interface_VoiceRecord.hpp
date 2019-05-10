//========= Copyright © 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#ifndef IVOICERECORD_H
#define IVOICERECORD_H
#pragma once


// This is the voice recording interface. It provides 16-bit signed mono data from 
// a mic at some sample rate.
abstract_class IVoiceRecord : public Base
{
public:
	// Use this to delete the object.
/*! off=4(1) */	virtual void		Release()=0;

	// Start/stop capturing.
/*! off=8(2) */	virtual bool		RecordStart() = 0;
/*! off=12(3) */	virtual void		RecordStop() = 0;

	// Idle processing.
/*! off=16(4) */	virtual void		Idle()=0;

	// Get the most recent N samples. If nSamplesWanted is less than the number of
	// available samples, it discards the first samples and gives you the last ones.
/*! off=20(5) */	virtual int			GetRecordedData(short *pOut, int nSamplesWanted)=0;
};


#endif // IVOICERECORD_H
