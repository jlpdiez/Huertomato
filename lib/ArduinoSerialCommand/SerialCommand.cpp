/******************************************************************************* 
SerialCommand - An Arduino library to tokenize and parse commands received over
a serial port. 
Copyright (C) 2011-2013 Steven Cogswell  <steven.cogswell@gmail.com>
http://awtfy.com

See SerialCommand.h for version history. 

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
***********************************************************************************/

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "SerialCommand.h"


#include <string.h>
#ifndef SERIALCOMMAND_HARDWAREONLY
#include <SoftwareSerial.h>
#endif

// Constructor makes sure some things are set. 
SerialCommand::SerialCommand()
{
	selectedSerialPort = SERIAL0;
	HardSerial = &Serial;
	strncpy(delim," ",MAXDELIMETER);  // strtok_r needs a null-terminated string
	term='\r';   // return character, default terminator for commands
	numCommand=0;    // Number of callback handlers installed
	clearBuffer(); 
}

//Constrictor for the user selected serial
#ifndef SERIALCOMMAND_HARDWAREONLY
SerialCommand::SerialCommand(serialPortID SerialPort, SoftwareSerial *_SoftSer /*= 0*/)
#else
SerialCommand::SerialCommand(serialPortID SerialPort)
#endif

{
	selectedSerialPort = SerialPort;
	strncpy(delim," ",MAXDELIMETER);  // strtok_r needs a null-terminated string
	term='\r';   // return character, default terminator for commands
	numCommand=0;    // Number of callback handlers installed
	clearBuffer();
	switch (SerialPort)
	{
#ifndef SERIALCOMMAND_HARDWAREONLY
		case SW_SERIAL:
			SoftSerial = _SoftSer;
		break;
#endif

#if defined(UBRR1H)
		case SERIAL1:
			HardSerial = &Serial1;
		break;
#endif

#if defined(UBRR2H)
		case SERIAL2:
			HardSerial = &Serial2;
		break;
#endif

#if defined(UBRR3H)
		case SERIAL3:
			HardSerial = &Serial3;
		break;
#endif

		case SERIAL0:
		default:
			HardSerial = &Serial;
		break;

	};
}


#ifndef SERIALCOMMAND_HARDWAREONLY
// Constructor to use a SoftwareSerial object
SerialCommand::SerialCommand(SoftwareSerial &_SoftSer)
{
	selectedSerialPort = SW_SERIAL;
	SoftSerial = &_SoftSer;
	strncpy(delim," ",MAXDELIMETER);  // strtok_r needs a null-terminated string
	term='\r';   // return character, default terminator for commands
	numCommand=0;    // Number of callback handlers installed
	clearBuffer(); 
}
#endif


//
// Initialize the command buffer being processed to all null characters
//
void SerialCommand::clearBuffer()
{
	for (int i=0; i<SERIALCOMMANDBUFFER; i++) 
	{
		buffer[i]='\0';
	}
	bufPos=0; 
}

// Retrieve the next token ("word" or "argument") from the Command buffer.  
// returns a NULL if no more tokens exist.   
char *SerialCommand::next() 
{
	char *nextToken;
	nextToken = strtok_r(NULL, delim, &last); 
	return nextToken; 
}

// This checks the Serial stream for characters, and assembles them into a buffer.  
// When the terminator character (default '\r') is seen, it starts parsing the 
// buffer for a prefix command, and calls handlers setup by addCommand() member
void SerialCommand::readSerial() 
{
	// If we're using the Hardware port, check it.   Otherwise check the user-created SoftwareSerial Port
	#ifdef SERIALCOMMAND_HARDWAREONLY
	while (HardSerial->available() > 0) 
	#else
	while ((selectedSerialPort!=SW_SERIAL && HardSerial->available() > 0) || (selectedSerialPort==SW_SERIAL && SoftSerial->available() > 0) )
	#endif
	{
		int i; 
		boolean matched; 
		#ifndef SERIALCOMMAND_HARDWAREONLY
		if (selectedSerialPort!=SW_SERIAL) {
		#endif
			// Hardware serial port
			inChar=HardSerial->read();   // Read single available character, there may be more waiting
		#ifndef SERIALCOMMAND_HARDWAREONLY
		} else {
			// SoftwareSerial port
			inChar = SoftSerial->read();   // Read single available character, there may be more waiting
		}
		#endif
		#ifdef SERIALCOMMANDDEBUG
		HardSerial.print(inChar);   // Echo back to serial stream
		#endif
		if (inChar==term) {     // Check for the terminator (default '\r') meaning end of command
			#ifdef SERIALCOMMANDDEBUG
			HardSerial.print("Received: "); 
			HardSerial.println(buffer);
			#endif
			bufPos=0;           // Reset to start of buffer
			token = strtok_r(buffer,delim,&last);   // Search for command at start of buffer
			if (token == NULL) return; 
			matched=false; 
			for (i=0; i<numCommand; i++) {
				#ifdef SERIALCOMMANDDEBUG
				HardSerial.print("Comparing ["); 
				HardSerial.print(token); 
				HardSerial.print("] to [");
				HardSerial.print(CommandList[i].command);
				HardSerial.println("]");
				#endif
				// Compare the found command against the list of known commands for a match
				if (strncmp(token,CommandList[i].command,SERIALCOMMANDBUFFER) == 0) 
				{
					#ifdef SERIALCOMMANDDEBUG
					HardSerial.print("Matched Command: "); 
					HardSerial.println(token);
					#endif
					// Execute the stored handler function for the command
					(*CommandList[i].function)(); 
					clearBuffer(); 
					matched=true; 
					break; 
				}
			}
			if (matched==false) {
				(*defaultHandler)(); 
				clearBuffer(); 
			}

		}
		if (isprint(inChar))   // Only printable characters into the buffer
		{
			buffer[bufPos++]=inChar;   // Put character into buffer
			buffer[bufPos]='\0';  // Null terminate
			if (bufPos > SERIALCOMMANDBUFFER-1) bufPos=0; // wrap buffer around if full  
		}
	}
}

// Adds a "command" and a handler function to the list of available commands.  
// This is used for matching a found token in the buffer, and gives the pointer
// to the handler function to deal with it. 
void SerialCommand::addCommand(const char *command, void (*function)())
{
	if (numCommand < MAXSERIALCOMMANDS) {
		#ifdef SERIALCOMMANDDEBUG
		HardSerial.print(numCommand); 
		HardSerial.print("-"); 
		HardSerial.print("Adding command for "); 
		HardSerial.println(command); 
		#endif
		
		strncpy(CommandList[numCommand].command,command,SERIALCOMMANDBUFFER); 
		CommandList[numCommand].function = function; 
		numCommand++; 
	} else {
		// In this case, you tried to push more commands into the buffer than it is compiled to hold.  
		// Not much we can do since there is no real visible error assertion, we just ignore adding
		// the command
		#ifdef SERIALCOMMANDDEBUG
		HardSerial.println("Too many handlers - recompile changing MAXSERIALCOMMANDS"); 
		#endif 
	}
}

// This sets up a handler to be called in the event that the receveived command string
// isn't in the list of things with handlers.
void SerialCommand::addDefaultHandler(void (*function)())
{
	defaultHandler = function;
}