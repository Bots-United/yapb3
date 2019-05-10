//======== (C) Copyright 1999, 2000 Valve, L.L.C. All rights reserved. ========
//
// The copyright to the contents herein is the property of Valve, L.L.C.
// The contents may be used and/or copied only with the written permission of
// Valve, L.L.C., or in accordance with the terms and conditions stipulated in
// the agreement/contract under which the contents have been supplied.
//
// Purpose: 
//
// $Workfile:     $
// $Date:         $
// $NoKeywords: $
//=============================================================================
#if !defined( IENGINE_H )
#define IENGINE_H
#ifdef _WIN32
#pragma once
#endif

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
abstract_class IEngine : public Base
{
	public:
		enum QuitType_t
		{
			QUIT_NOTQUITTING,
			QUIT_TODESKTOP,
			QUIT_RESTART
		};

/*! off=4(1) */		virtual	bool	Load( bool dedicated, const char *rootdir, const char *lpCmdLine) = 0;
/*! off=8(2) */		virtual void	Unload( void ) = 0;
/*! off=12(3) */		virtual	void	SetState( int iState ) = 0;
/*! off=16(4) */		virtual int		GetState( void ) = 0;
/*! off=20(5) */		virtual	void	SetSubState( int iSubState ) = 0;
/*! off=24(6) */		virtual int		GetSubState( void ) = 0;
/*! off=28(7) */		virtual int		Frame( void ) = 0;

/*! off=32(8) */		virtual double	GetFrameTime( void ) = 0;
/*! off=36(9) */		virtual double	GetCurTime( void ) = 0;

/*! off=40(10) */		virtual void	TrapKey_Event( int key, bool down ) = 0;
/*! off=44(11) */		virtual void	TrapMouse_Event( int buttons, bool down ) = 0;

/*! off=48(12) */		virtual void	StartTrapMode( void ) = 0;
/*! off=52(13) */		virtual bool	IsTrapping( void ) = 0;
/*! off=56(14) */		virtual bool	CheckDoneTrapping( int& buttons, int& key ) = 0;

/*! off=60(15) */		virtual QuitType_t		GetQuitting( void ) = 0;
/*! off=64(16) */		virtual void	SetQuitting( QuitType_t quittype ) = 0;
};

class Engine : public IEngine
{/*
	public:
		int				m_nQuitting;

		int				m_nDLLState;

		double			m_fCurTime;
		double			m_fFrameTime;
		double			m_fOldTime;


		bool			m_bTrapMode;
		bool			m_bDoneTrapping;
		int				m_nTrapKey;
		int				m_nTrapButtons;*/
};

#endif // IENGINE_H