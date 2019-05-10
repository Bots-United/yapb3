//
// Copyright © 2003-2010, by YaPB Development Team. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// Server.hpp
//
// Structure: Server_t
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_SERVER_INCLUDED
#define HALF_LIFE_ENGINE_SDK_STRUCTURES_SERVER_INCLUDED

#define	MAX_MASTERS	8				// max recipients for heartbeat packets
#define	MAX_SIGNON_BUFFERS	8

enum server_state_t
{
	ss_dead,	// no map loaded (No server)
	ss_loading,	// spawning level edicts (Spawning)
	ss_active	// actively running
};
// some qc commands are only valid before the server has finished
// initializing (precache commands, static sounds / objects, etc)

#define	MAX_MSGLEN		1024		// max length of a reliable message
#define	MAX_DATAGRAM	4000		// max length of unreliable message
#define	MAX_SIGNON		32768u		// max length of signon message

//
// per-level limits
//
#define	MAX_EDICTS		900			// FIXME: ouch! ouch! ouch!
#define	MAX_LIGHTSTYLES	64
#define	MAX_MODELS		512			// these are sent over the net as bytes
#define	MAX_SOUNDS		512			// so they cannot be blindly increased
#define	MAX_SOUNDS_HASH_TABLE		1024
#define MAX_EVENTS		256
#define MAX_GENERIC		512
#define MAX_CONSISTENCY_INFOS		512
#define MAX_MOD_KNOWN	1024

struct sizebuf_t
{
	DECLARE_TYPED_ENUMERATION (unsigned char/*short*/, OverflowedFlag_t)
	{
		OverflowedFlag_AllowOverflow = BIT (0u),	// if false, do a Sys_Error()
		OverflowedFlag_Overflowed    = BIT (1u)		// set to true if the buffer size failed
	};

/*!off=0(0)*/	unsigned char    *name;				// Example: "Client Datagram".
/*!off=4(1)*/	OverflowedFlag_t  overflowedFlags;
/*!off=8(2)*/	unsigned char    *data;
/*!off=12(3)*/	unsigned int      maxsize;
/*!off=16(4)*/	unsigned int      cursize;
};	// sizeof (sizebuf_t) == 20 (5)

struct Server_t
{
/*!off=0(0)*/	BOOL	active;				// false when server is going down (false if only a net client)

/*!off=4(1)*/	BOOL	paused;				// are we paused?
	BOOL	loadgame;			// handle connections specially

/*!!off=W=16(4)/L=12(3)!!(dbl_2136E50)*/	double		time;
/*!!off=W=24(6)/L=20(5)!!*/	double		UNKNOWN0_time;	// (Set in start of SV_Frame() function from 'time'. UNUSED!!!)

/*!!off=W=32(8)/L=28(7)!!*/	int			lastcheck;			// used by PF_checkclient
/*!!off=W=40(10)/L=32(8)!!*/	double		lastchecktime;		// for monster ai

/*!!off=48(12)/L=40(10) (diff=8(2))!!*/	char		name[64u];			// map name
/*!!off=112(28)*/	char		prevMapName[64u];
/*!!off=176(44)*/	char		startspot[64u];
/*!!off=240(60)*/	char		modelname[Constants::MaximumPathNameLength];		// maps/<name>.bsp, for model_precache[0]
/*!!off=304(76)/L=(74)*/	Model_t 	*worldmodel;	// cl_entitites[0].model
/*!!off=308(77)*/	TypeDefinitions::CRC32_t worldmapCRC;	// For detecting that client has a hacked local copy of map, the client will be dropped if this occurs.
/*!!off=312(78)*/	TypeDefinitions::CRC32_t clientSideDllCRC;	// The dll that this server is expecting clients to be using.
/*!!off=316(79)*/	unsigned int UNKNOWN3[3u];	/// UNUSED!!!
/*!!off=328(82)*/	Resource_t resources[MAX_RESOURCES];
/*!!off=174408(43602)*/	unsigned int resourcesNumber;
	struct ConsistencyInfo_t	// Used by pfnForceUnmodified() function.
	{
		const char             *filename;
		unsigned int            UNKNOWN[3u];	/// UNUSED!!!
		Constants::ForceType_t  type;
		Math::Extent3D          boundingBox;
	};
/*!!off=174412(43603)*/	ConsistencyInfo_t consistencyInfos[MAX_CONSISTENCY_INFOS];
/*!!off=196940(49235)*/	unsigned int consistencyInfosNumber;
/*!!off=196944(49236)*/	char		*model_precache[MAX_MODELS];	// NULL terminated
/*!!off=198992(49748)*/	Model_t		*models[MAX_MODELS];
/*!!off=201040(50260)*/	Resource_t::Flag_t model_precache_flags[MAX_MODELS];
	struct PrecachedEvent_t
	{
		unsigned short  index;
		const char     *name;
		unsigned int    fileSize;
		unsigned char  *fileBuffer;
	};
/*!!off=201552(50388)*/	PrecachedEvent_t event_precache[MAX_EVENTS];
/*!!off=205648(51412)*/	char		*sound_precache[MAX_SOUNDS];	// NULL terminated
/*!!off=207696(51924)*/	unsigned short soundPrecacheHashTable[MAX_SOUNDS_HASH_TABLE];
/*!!off=209744(52436)*/	BOOL         isSoundPrecacheHashTableBuilt;
/*!!off=209748(52437)*/	char        *generic_precache[MAX_GENERIC];
/*!!off=211796(52949)*/	char         genericResourcesPathNames[MAX_GENERIC][64u];
/*!!off=244564(61141)*/	unsigned int genericResourcesPathNamesNumber;
/*!!off=244568(61142)*/	char		*lightstyles[MAX_LIGHTSTYLES];

/*!!off=244824(61206)*/	int			num_edicts;			// increases towards MAX_EDICTS
/*!!off=244828(61207)*/	int			max_edicts;			// increases towards MAX_CLIENTS (3 * (MAX_CLIENTS or maxclients) + (MAX_EDICTS or "-num_edicts") + 12 * (MAX_CLIENTS or maxclients) - 15)
/*!!off=244832(61208)*/	Classes::Edict		*edicts;			// Can be array indexed now, 'Classes::Edict' is fixed

/*!!off=244836(61209)*/	EntityState_t		*baselines;

// instanced baselines container
/*!!off=244840(61210)*/	struct InstanceBaselines_t
	{
		unsigned int   count;
		EntityState_t *instance_baselines;
	} *instance_baselines/*[64]*/;	// parallel array to sv.edicts, storing entity baselines

/*!!off=244844(61211)*/	server_state_t	state;			// precache commands are only valid during load (some actions are only valid during load)

	// added to every client's unreliable buffer each frame, then cleared
/*!off=244848(61212)*/	sizebuf_t	datagram;
/*!off=244868(61217)*/	unsigned char		datagram_buf[MAX_DATAGRAM];

	// added to every client's reliable buffer each frame, then cleared
/*!off=248868(62217)*/	sizebuf_t	reliable_datagram;	// copied to all clients at end of frame
/*!off=248888(62222)*/	unsigned char		reliable_datagram_buf[MAX_DATAGRAM];

	// the multicast buffer is used to send a message to a set of clients
/*!off=252888(63222)*/	sizebuf_t	multicast;	// Longest multicast message
/*!off=252908(63227)*/	unsigned char		multicast_buf[MAX_MSGLEN];

/*!off=253932(63483)*/	sizebuf_t	spectator;
/*!off=253952(63488)*/	unsigned char		spectator_buf[MAX_MSGLEN];
/*!NOT EXISTS
	// the master buffer is used for building log packets
	sizebuf_t	master;
	unsigned char		master_buf[MAX_DATAGRAM];
*/
	// the signon buffer will be sent to each client as they connect
	// includes the entity baselines, the static entities, etc
	// large levels will have >MAX_DATAGRAM sized signons, so
	// multiple signon messages are kept
/*!off=254976(63744)*/	sizebuf_t	signon;
/*!off=254996(63749)*/	unsigned char		signon_buf[MAX_SIGNON];
};	// sizeof (Server_t) == 287768 (71942)

#define	NUM_SPAWN_PARMS			16

#define	MAX_PACKET_ENTITIES	256	// doesn't count nails
/*struct packet_entities_t
{
	int		num_entities;
	EntityState_t	entities[MAX_PACKET_ENTITIES];
};*/
class PackedEntities
{
public:
	PackedEntities() : entities(NULL) {}

/*!Off=0(0)/6120(1530)*/	int				num_entities;
//NOT EXISTS?!?!?!?!?!?!?!?	int				max_entities;

/*!Off=4(1)/6124(1531)*/	unsigned int UNKNOWN0[8u];

	// FIXME: Can we use the packed ent dict on the client too?
	// Client only fields:
/*!Off=36(9)/6156(1539)*/	EntityState_t	*entities;
};	// sizeof (PackedEntities) == 44 (11)

struct client_frame_t
{
	// received from client

	// reply

	// Time world sample was sent to client.
/*!Off=0(0)*/	double				senttime;
	// Realtime when client ack'd the update.
/*!Off=8(2)*/	float				raw_ping;
/*!Off=12(3)*/	ClientData_t        clientData;
/*!Off=488(122)*/	WeaponData_t weaponData[Constants::MaximumWeapons * 2u/* WHY 64 ??????????????????????????????????????*/];

	// State of entities this frame from the POV of the client.
/*!Off=6120(1530)*/	PackedEntities		entities;
};	// sizeof (client_frame_t) == 6160 (1540)

#define SIGNED_GUID_LEN 64	// Hashed CD Key (32 hex alphabetic chars + 0 terminator)

#define MAX_BACK_BUFFERS 4

#define	UPDATE_BACKUP	64	// copies of EntityState_t to keep buffered (must be power of two)
#define	UPDATE_MASK		(UPDATE_BACKUP-1)

#define FLOW_OUTGOING 0
#define FLOW_INCOMING 1
#define MAX_FLOWS 2

// Message data
struct flowstats_t
{
	// Size of message sent/received
/*!Off=8432(2108)(from Client_t=(2116))*/	int		size;
/*!Off=8436(2109)(from Client_t=(2117))*/	unsigned int UNKNOWN0;
	// Time that message was sent/received
/*!Off=8440(2110)(from Client_t=(2118))*/	double	time;
};	// sizeof (flowstats_t) == 16 (4)

#define	MAX_LATENT	32

struct flow_t
{
	// Data for last MAX_LATENT messages
/*!Off=8432(2108)(from Client_t=(2116))*/	flowstats_t stats[MAX_LATENT];
	// Current message position
/*!Off=8944(2236)(from Client_t=(2244))*/	int			current;
	// Time when we should recompute k/sec data
/*!Off=8952(2238)(from Client_t=(2246))*/	double		nextcompute;
	// Average data
/*!Off=8960(2240)(from Client_t=(2248))*/	float		kbytespersec;
/*!Off=8964(2241)(from Client_t=(2249))*/	float		avgkbytespersec;
};	// sizeof (flow_t) == 536 (134)

#define	MAX_OSPATH		260			// max length of a filesystem pathname

// Size of fragmentation buffer internal buffers
#define FRAGMENT_SIZE 1400

#define	FRAG_NORMAL_STREAM	0
#define FRAG_FILE_STREAM	1

// Generic fragment structure
struct fragbuf_t
{
	// Next buffer in chain
/*!Off=0(0)*/	fragbuf_t	*next;
	// Id of this buffer
/*!Off=4(1)*/	int					bufferid;
	// Message buffer where raw data is stored
/*!Off=8(2)*/	sizebuf_t			frag_message;
	// The actual data sits here
/*!Off=28(7)*/	unsigned char				frag_message_buf[FRAGMENT_SIZE];
	// Is this a file buffer?
/*!Off=1428(357)*/	BOOL			isfile;
	// Is this file buffer from memory (custom decal, etc.).
/*!Off=1432(358)*/	BOOL			isbuffer;
	// Name of the file to save out on remote host
/*!Off=1440(360)*/	char				filename[MAX_OSPATH];
	// Offset in file from which to read data
/*!Off=1700(425)*/	int					foffset;
	// Size of data to read at that offset
/*!Off=1704(426)*/	int					size;
//////EXISTS?!?!?!?!?!?!??!?!?!?!
};	// sizeof (fragbuf_t) == 1708 (427)

// Waiting list of fragbuf chains
struct fragbufwaiting_t
{
	// Next chain in waiting list
	fragbufwaiting_t		*next;
	// Number of buffers in this chain
	int							fragbufcount;
	// The actual buffers
	fragbuf_t					*fragbufs;
};	// sizeof (fragbufwaiting_t) == 12 (3)

struct netadr_t
{
	enum type_t
	{
		NA_UNUSED,
		NA_LOOPBACK,
		NA_BROADCAST,
		NA_IP,
		NA_IPX,
		NA_BROADCAST_IPX
	};

	type_t         type;
	unsigned char  ip[4];
	unsigned char  ipx[10];
	unsigned short port;
};	// sizeof (netadr_t) = 20 (5)

// 0 == regular, 1 == file stream
#define MAX_STREAMS			2

// NETWORKING INFO

// This is the packet payload without any header bytes (which are attached for actual sending)
#define	NET_MAX_PAYLOAD	3990

// Network Connection Channel
struct netchan_t
{
	enum netsrc_t
	{
		NS_CLIENT,
		NS_SERVER,
		NS_MULTICAST
	};


/*!Off=0(0)(from Client_t=32(8))*/	netsrc_t    sock;	// NS_SERVER or NS_CLIENT, depending on channel.

/*!Off=4(1)(from Client_t=36(9))*/	netadr_t	remote_address;	// Address this channel is talking to.

/*!Off=24(6)(from Client_t=56(14))*/	unsigned int UNKNOWN0_index;

/*!off=28(7)(from Client_t=60(15))*/	float		last_received;	// For timeouts. Time last message was received.
/*!Off=32(8)(from Client_t=64(16))*/	float       connect_time;	// Time when channel was connected. (stored from real time) (also known as client connect/play time)

//*!Off=36(9)(from Client_t=68(17))*/	unsigned int UNKNOWN1;	// DUNNO WHY IT'S NOT EXISTS AND DIFFERENCE BETWEEN 'rate' AND 'connect_time' is 8(2) INSTEAD OF 4(1) !!!!!!

	// Bandwidth choke
	// Bytes per second
/*!Off=40(10)(from Client_t=72(18))*/	double		rate;
/*!Off=48(12)(from Client_t=80(20))*/	double		cleartime;	// If realtime > cleartime, free to send next packet

	// Sequencing variables
	//
/*!Off=56(14)(from Client_t=88(22))*/	int			incoming_sequence;				// Increasing count of sequence numbers
/*!Off=60(15)(from Client_t=92(23))*/	int			incoming_acknowledged;			// # of last outgoing message that has been ack'd.
/*!Off=64(16)(from Client_t=96(24))*/	int			incoming_reliable_acknowledged;	// Toggles T/F as reliable messages are received.
/*!Off=68(17)(from Client_t=100(25))*/	int			incoming_reliable_sequence;		// single bit, maintained local
/*!Off=72(18)(from Client_t=104(26))*/	int			outgoing_sequence;				// Message we are sending to remote
/*!Off=76(19)(from Client_t=108(27))*/	int			reliable_sequence;				// Whether the message contains reliable payload, single bit
/*!Off=80(20)(from Client_t=112(28))*/	int			last_reliable_sequence;			// Outgoing sequence number of last send that had reliable data

/*!Off=84(21)(from Client_t=(29))*/	Client_t *clientForFragmentSizeFunction;
typedef const int (*FragmentSizeFunction_t) (Client_t *const client);
/*!Off=88(22)(from Client_t=(30))*/	FragmentSizeFunction_t fragmentSizeFunctionForClient;	// fragmentSizeFunctionForClient(clientForFragmentSizeFunction)

	// Staging and holding areas
/*!Off=92(23)(from Client_t=(31))*/	sizebuf_t	message;
/*!Off=112(28)(from Client_t=(36))*/	unsigned char		message_buf[NET_MAX_PAYLOAD];

	// Reliable message buffer. We keep adding to it until reliable is acknowledged. Then we clear it.
/*!Off=4104(1026)(from Client_t=(1034))*/	int			reliable_length;
/*!Off=4108(1027)(from Client_t=(1035))*/	unsigned char		reliable_buf[NET_MAX_PAYLOAD];	// unacked reliable message

	// Waiting list of buffered fragments to go onto queue.
	// Multiple outgoing buffers can be queued in succession
/*!Off=8100(2025)(from Client_t=(2033))*/	fragbufwaiting_t *waitlist[MAX_STREAMS];

/*!Off=8108(2027)(from Client_t=(2035))*/	int				reliable_fragment[MAX_STREAMS];	// Is reliable waiting buf a fragment?
/*!Off=8116(2029)(from Client_t=(2037))*/	unsigned int	reliable_fragid[MAX_STREAMS];	// Buffer id for each waiting fragment

/*!Off=8124(2031)(from Client_t=(2039))*/	fragbuf_t	*fragbufs[MAX_STREAMS];		// The current fragment being set
/*!Off=8132(2033)(from Client_t=(2041))*/	int			fragbufcount[MAX_STREAMS];	// The total number of fragments in this stream

/*!Off=8140(2035)(from Client_t=(2043))*/	short		frag_startpos[MAX_STREAMS];	// Position in outgoing buffer where frag data starts
/*!Off=8144(2036)(from Client_t=(2044))*/	short		frag_length[MAX_STREAMS];	// Length of frag data in the buffer

/*!Off=8148(2037)(from Client_t=(2045))*/	fragbuf_t	*incomingbufs[MAX_STREAMS];	// Incoming fragments are stored here
/*!Off=8156(2039)(from Client_t=(2047))*/	BOOL	incomingready[MAX_STREAMS];		// Set to true when incoming data is ready

	// Only referenced by the FRAG_FILE_STREAM component
	// Name of file being downloaded
/*!Off=8164(2041)(from Client_t=(2049))*/	char		incomingfilename[MAX_OSPATH];

/*!Off=8424(2106)(from Client_t=(2114))*/	unsigned char *tempbuffer;
/*!Off=8428(2107)(from Client_t=(2115))*/	unsigned int   tempbufferSize;

/*!Off=8432(2108)(from Client_t=(2116))*/	flow_t flow[MAX_FLOWS];	// Incoming and outgoing flow metrics
};	// sizeof (netchan_t) == 9504 (2376)

struct Client_t
{
/*!off=0(0)*/	BOOL		active;				// false = client is free
/*!off=4(1)*/	BOOL		spawned;			// false = don't send datagrams
/*!off=8(2)*/	BOOL        UNKNOWN0;	/// (@info: Assigned to FALSE in SV_DropClient() function.) (Maybe "int lastmsg;" ??)
/*!off=12(3)*/	BOOL        connected;          // On server, getting data.

/*!off=16(4)*/	BOOL         UNKNOWN1;	/// (@info: Used in SV_RequestMissingResources()) (Maybe "int resources;" ??)
/*!off=20(5)*/	BOOL         UNKNOWN2;	/// (@info: Used in PF_MessageEnd_I(), assigned in SV_New_f(), SV_ActivateServer(), SV_DropClient() functions.) Do sending game DLL messages? (Maybe "BOOL ready;" ??)
/*!off=24(6)*/	unsigned int UNKNOWN3[2u];	/// (Maybe "int consistency;" ??)

//===== NETWORK ============
/*!off=32(8)*/	netchan_t		netchan;				// The client's net connection.
/*!Off=9536(2384)*/	int				chokecount;			// Number of packets choked at the server because the client - server network channel is backlogged with too much data.
/*!Off=9540(2385)*/	int				delta_sequence;		// -1 = no compression. This is where the server is creating the compressed info from.

/*!Off=9544(2386)*/	BOOL		fakeclient;	// JAC: This client is a fake player controlled by the game DLL
/*!Off=9548(2387)*/	BOOL		hltv_proxy;	// this is spectator proxy (hltv) (value of "*hltv" in userinfo)
/*!Off=9552(2388)*/	UserCommand_t		lastcmd;			// for filling in big drops and partial predictions

/*!Off=9608(2402)*/	double last_cmdtime;
/*!Off=9616(2404)*/	double last_movetime;
/*!Off=9624(2406)*/	double next_movetime;
/*!Off=9632(2408)*/	float				latency;	// (Assigns in SV_ComputeLatency() from SV_CalcClientTime() (Calculated in SV_CalcLatency() - HL2))
/*!Off=9636(2409)*/	float				lossage;			// loss percentage (Calculated in SV_CalcPacketLoss() - HL2)
/*!Off=9640(2410)*/	unsigned int UNKNOWN5[2u];	/// UNUSED!!! (Maybe "double UNKNOWN5;" ??)
/*!Off=9648(2412)*/	double				next_checkpingtime;	// time to send all players pings to client
/*!Off=9656(2414)*/	double timebase;	// client timebase

	// The datagram is written to after every frame, but only cleared when it is sent out to the client. Overflow is tolerated.
/*!Off=9664(2416)*/	sizebuf_t		datagram;
/*!Off=9684(2421)*/	unsigned char			datagram_buf[MAX_DATAGRAM];

/*!Off=13688(3422)*/	double				connection_started;	// Or time of disconnect for zombies
/*!Off=13696(3424)*/	double				next_messagetime;	// Time when we should send next world state update (datagram)
/*!Off=13704(3426)*/	double next_messageinterval;	// default time to wait for next message (inverted value of "cl_updaterate" in userinfo)
/*!Off=13712(3428)*/	BOOL				send_message;	// Set on frames a datagram arrived on
/*!Off=13716(3429)*/	BOOL				skip_message;	// Defer message sending for one frame

/*!Off=13720(3430)*/	client_frame_t	*frames;	// updates can be delta'd from here

/*!Off=13724(3431)*/	event_state_t events;	// Per edict events

	// Identity information.
/*!Off=19356(4839)*/	Classes::Edict			*edict;				// EDICT_NUM(clientnum+1)
/*!Off=19360(4840)*/	const Classes::Edict	*pViewEntity;		// View Entity (camera or the client itself)

/*!Off=19364(4841)*/	int				userid;							// identifying number on server
	struct UNKNOWN7_ConnectionInformation_t
	{
/*!Off=19368(4842)*/		unsigned int authentication_method;	// 1 ... 3, 1 = for regular clients ("STEAM_*"), 2 = "VALVE_*", 3 = "HLTV", else = "UNKNOWN"
/*!Off=19372(4843)*/		unsigned int UNKNOWN1;	/// UNUSED!!!
/*!Off=19376(4844)*/		Classes::Steam::ID steamID;	// Created on user connection.
/*!Off=19384(4846)*/		unsigned char ip[4u];	// Default is [1, 0, 0, 127]
/*!Off=19388(4847)*/		unsigned int UNKNOWN2;	/// UNUSED!!!
	};	// sizeof (UNKNOWN7_ConnectionInformation_t) == 24 (6)
	UNKNOWN7_ConnectionInformation_t UNKNOWN7_connectionInformation;	// User ID information.
/*!Off=19392(4848)*/	char			userinfo[Constants::MaximumInfoStringLength];		// infostring: name, etc (received from client)
/*!Off=19648(4912)*/	BOOL		sendinfo;			// at end of frame, send info to all this prevents malicious multiple broadcasts
/*!Off=19652(4913)*/	float sendinfoRealTime;
/*!Off=19656(4914)*/	char            hashedcdkey[SIGNED_GUID_LEN]; // MD5 hash is 32 hex #'s, plus trailing 0
/*!Off=19720(4930)*/	char			name[Constants::MaximumClientNameLength];			// for printing to other people. Extracted from userinfo
/*!Off=19752(4938)*/	int			topcolor;	// value of "topcolor" in userinfo
/*!Off=19756(4939)*/	int			bottomcolor;	// value of "bottomcolor" in userinfo
/*!Off=19760(4940)*/	unsigned int UNKNOWN8;	/// UNUSED!!!
/*!Off=19764(4941)*/	Resource_t UNKNOWN9_resource;
/*!Off=19900(4975)*/	Resource_t UNKNOWN10_resource;	// <mapname.res> from client (server downloading)
/*!Off=20036(5009)*/	unsigned int UNKNOWN11;	/// UNUSED!!!
/*!Off=20040(5010)*/	BOOL isCustomResourcePropagationCompleted;
/*!Off=20044(5011)*/	Customization_t customization;	// player customization linked list
/*!Off=20208(5052)*/	TypeDefinitions::CRC32_t worldmapCRC;
/*!Off=20212(5053)*/	BOOL local_weapons;	// enable weapon predicting (value of "cl_lw" in userinfo)
/*!Off=20216(5054)*/	BOOL lag_compensation;	// enable lag compensation (value of "cl_lc" in userinfo)
/*!Off=20220(5055)*/	char physInfo[Constants::MaximumPhysInfoStringLength];	// Physics info string (set on server (transmit to client))

	// ------------------------------------------------------------ //
	// Voice stuff.
	// ------------------------------------------------------------ //
/*!Off=20476(5119)*/	BOOL			m_bLoopback;		// Does this client want to hear his own voice? (value of "voice_loopback" console variable)
/*!Off=20480(5120)*/	unsigned long	m_VoiceStreams[Constants::MaximumClients / 32u + 1u];	// Which other clients does this guy's voice stream go to?
};	// sizeof (Client_t) == [STEAM: 20488 (5122), NOSTEAM: 22552 (5638)]

// a client can leave the server in one of four ways:
// dropping properly by quiting or disconnecting
// timing out if no valid messages are received for timeout.value seconds
// getting kicked off by the server operator
// a program error, like an overflowed reliable buffer

//=============================================================================

struct ServerStatic_t
{
/*!Off=0(0)*/	BOOL    dll_initialized;    // Have we loaded the game dll.
/*!Off=4(1)*/	Client_t *clients;	// array of up to [maxclients] client slots.
/*!Off=8(2)*/	int			maxclients;         // Current max #
/*!Off=12(3)*/	int			maxclientslimit;    // Max allowed on server.
/*!Off=16(4)*/	int			spawncount;			// Number of servers spawned since start, used to check late spawns (e.g., when d/l'ing lots of data)
/*!Off=20(5)*/	int			serverflags;		// episode completion information (UNUSED, ALWAYS 0)
/*!Off=24(6)*/	BOOL logToFile;
/*!Off=28(7)*/	BOOL logToConsole;
/*!Off=32(8)*/	sizebuf_t log;
/*!Off=52(13)*/	unsigned char *logFile;
/*!Off=56(14)*/	double gatherStatisticsRealTime1;
/*!Off=64(16)*/	double gatherStatisticsRealTime2;
	struct UNKNOWN3_t
	{
/*!Off=72(18)*/		unsigned int UNKNOWN0_count;
/*!Off=76(19)*/		unsigned int UNKNOWN1_count;
/*!Off=80(20)*/		unsigned int UNKNOWN2_count;
/*!Off=84(21)*/		float        UNKNOWN3;
/*!Off=88(22)*/		float        UNKNOWN4;
/*!Off=92(23)*/		unsigned int UNKNOWN5_count;
/*!Off=96(24)*/		unsigned int UNKNOWN6_count;
/*!Off=100(25)*/		float        UNKNOWN7;
/*!Off=104(26)*/		float        UNKNOWN8;
/*!Off=108(27)*/		unsigned int UNKNOWN9_count;
/*!Off=112(28)*/		float        UNKNOWN10;
/*!Off=116(29)*/		float        UNKNOWN11;
/*!Off=120(30)*/		float        UNKNOWN12;
/*!Off=124(31)*/		float        UNKNOWN13;
	};	// sizeof (UNKNOWN3_t) == 56 (14)
/*!Off=72(18)*/	UNKNOWN3_t UNKNOWN3;	// Something for SV_GatherStatistics() function....
/*!Off=128(32)*/	BOOL UNKNOWN4;
};	// sizeof (ServerStatic_t) == 132 (33) (WARNING: THIS IS NOT 132 SIZE, IT 136!!! I DON'T KNOW WHY...)

#endif	// ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_SERVER_INCLUDED