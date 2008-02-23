#include "global.h"
#include "XmlFile.h"
#include "RageUtil.h"
#include "ProfileManager.h"
#include "RageLog.h"
#include "RageTimer.h"
#include "SongManager.h"
#include "ProductInfo.h"

// This is how I chose to find the Crash Log size.
// -- Matt1360
int GetNumCrashLogs()
{
	// Give ourselves a variable.
	CStringArray aLogs;
	
	// Get them all.
	GetDirListing( "/stats/crashlog-*.txt" , aLogs );
	
	return aLogs.size();
}

int GetNumMachineEdits()
{
	CStringArray aEdits;
	CString sDir = PROFILEMAN->GetProfileDir(PROFILE_SLOT_MACHINE) + EDIT_SUBDIR;
	
	GetDirListing( sDir , aEdits );
	
	return aEdits.size();
}

int GetIP()
{
	return 0;
}

int GetRevision()
{
	// Create the XML Handler, and clear it, for practice.
	XNode *xml = new XNode;
	xml->Clear();
	xml->m_sName = "patch";
	int rev;
	
	// Check for the file existing
	if( !IsAFile( "/Data/patch/patch.xml" ) )
	{
		LOG->Trace( "There is no patch file (patch.xml)" );
		return 1;
	}
	
	// Make sure you can read it
	if( !xml->LoadFromFile( "/Data/patch/patch.xml" ) )
	{
		LOG->Trace( "patch.xml unloadable" );
		return 1;
	}
	
	// Check the node <Revision>x</Revision>
	if( !xml->GetChild( "Revision" ) )
	{
		LOG->Trace( "Revision node missing! (patch.xml)" );
		return 1;
	}
	
	// Return as an integer
	xml->GetChild("Revision")->GetValue(rev);
	return rev;
}

int GetNumMachineScores()
{
	// Create the XML Handler and clear it, for practice
	XNode *xml = new XNode;
	xml->Clear();
	
	// Check for the file existing
	if( !IsAFile( "/Data/MachineProfile/Stats.xml" ) )
	{
		LOG->Trace( "There is no stats file!" );
		return 1;
	}
	
	// Make sure you can read it
	if( !xml->LoadFromFile( "/Data/MachineProfile/Stats.xml" ) )
	{
		LOG->Trace( "Stats.xml unloadable!" );
		return 1;
	}
	
	const XNode *pData = xml->GetChild( "SongScores" );
	
	if( pData == NULL )
	{
		LOG->Warn( "Error loading scores: <SongScores> node missing" );
		return 0;
	}
	
	int iScoreCount = 0;
	
	// Named here, for LoadFromFile() renames it to "Stats"
	xml->m_sName = "SongScores";
	
	// For each pData Child, or the Child in SongScores...
	FOREACH_CONST_Child( pData , p )
		iScoreCount++;
	
	return iScoreCount;
}

/*
 * [ScreenArcadeDiagnostics]
 *
 * All ITG2AC Functions here
 * Mostly...Implemented
 *
 * Work Log!
 *
 * Work started 2/9/08, after 10 PM - 2:30 AM
 *  ProductName, Revision, Uptime, Crashlogs,
 *  Machine edits, done!
 *
 * Work, 2/10/08 7 PM - 9:30 PM
 *  Did work on GetNumMachineScores() ... That sucked
 *  Somewhat complete...Can't do IO Errors, an ITG-IO
 *  exclusive, it seems.
 *
 * Total Hours: ~6
 * 
 * this doesn't belong in LuaManager.cpp --infamouspat
 */
#include "LuaFunctions.h"
LuaFunction_NoArgs( GetProductName	, CString( PRODUCT_NAME ) ); // Return the product's name from ProductInfo.h [ScreenArcadeDiagnostics]
LuaFunction_NoArgs( GetRevision	, GetRevision() ); // Return current Revision ( ProductInfo.h ) [ScreenArcadeDiagnostics]
// do we really need it by the millisecond?
LuaFunction_NoArgs( GetUptime		, SecondsToMMSSMsMsMs( RageTimer::GetTimeSinceStart() ) ); // Uptime calling [ScreenArcadeDiagnostics]
LuaFunction_NoArgs( GetIP		, GetIP() ); // Calling the IP [ScreenArcadeDiagnostics]
LuaFunction_NoArgs( GetNumCrashLogs	, GetNumCrashLogs() ); // Count the crashlogs [ScreenArcadeDiagnostics]
LuaFunction_NoArgs( GetNumMachineEdits	, GetNumMachineEdits() ); // Count the machine edits [ScreenArcadeDiagnostics]
LuaFunction_NoArgs( GetNumIOErrors	, 0 ); // Call the number of I/O Errors [ScreenArcadeDiagnostics]
LuaFunction_NoArgs( GetNumMachineScores, GetNumMachineScores() ); // Call the machine score count [ScreenArcadeDiagnostics]

/*
 * (c) 2004 Glenn Maynard
 * (c) 2008 Matt1360, Vyhd
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
