#ifndef TYPES_HPP_INCLUDED
#define TYPES_HPP_INCLUDED

//main
#define CHK_REP 101
#define CHK_FORCE 102
#define TXT_STATUS 103

//plugin
#define CHK_DBG 201
//You can either access it like buttons->R_DPAD or use Value as a compiled data.
//Used in GetKeys() to return input to emulator as a DWORD
typedef union {
	DWORD Value;
	struct {
		unsigned R_DPAD       : 1;
		unsigned L_DPAD       : 1;
		unsigned D_DPAD       : 1;
		unsigned U_DPAD       : 1;
		unsigned START_BUTTON : 1;
		unsigned Z_TRIG       : 1;
		unsigned B_BUTTON     : 1;
		unsigned A_BUTTON     : 1;

		unsigned R_CBUTTON    : 1;
		unsigned L_CBUTTON    : 1;
		unsigned D_CBUTTON    : 1;
		unsigned U_CBUTTON    : 1;
		unsigned R_TRIG       : 1;
		unsigned L_TRIG       : 1;
		unsigned Reserved1    : 1;
		unsigned Reserved2    : 1;

		signed   Y_AXIS       : 8;

		signed   X_AXIS       : 8;
	};
} BUTTONS;

typedef struct {
	BOOL Present;
	BOOL RawData;
	int  Plugin;
} CONTROL;

typedef struct {
	WORD Version;        /* Should be set to 0x0101 */
	WORD Type;           /* Set to PLUGIN_TYPE_AUDIO */
	char Name[100];      /* Name of the DLL */

	/* If DLL supports memory these memory options then set them to TRUE or FALSE
	   if it does not support it */
	BOOL NormalMemory;   /* a normal BYTE array */
	BOOL MemoryBswaped;  /* a normal BYTE array where the memory has been pre
	                          bswap on a dword (32 bits) boundry */
} PLUGIN_INFO;
#endif // TYPES_HPP_INCLUDED
