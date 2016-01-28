#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <map>

#ifdef WINDOWS

#include <windows.h>

#endif

using namespace std;

stringstream ss;
string id;
string subkattis;
string kattisDir;
#ifdef WINDOWS
string sevenZipDir;
#endif

#ifdef WINDOWS
string pathSeparator = "\\";
#else
string pathSeparator = "/";
#endif

#define ssstr ss.str ( ).c_str ( )

void exec( const char* s ){

	#ifdef WINDOWS

	WinExec(s, SW_HIDE);

	#else

	system ( s );

	#endif
}

void renameFile ( string filename, string targetName ) {

	#ifdef WINDOWS

	ss
	<< "move "
	<< filename
	<< " "
	<< targetName;

	#else

	ss
	<< "mv "
	<< filename
	<< " "
	<< targetName;

	#endif

	#ifdef DEBUGGING

	cout
	<< ss.str()
	<< endl;

	#endif

	system ( ssstr );
	ss.str("");
}

bool setKattisDir ( ) {

	#ifdef WINDOWS

	kattisDir = getenv ( "kattis" );
	if ( kattisDir != "" ) return false;
	return true;

	#else

	kattisDir = ".";
	return true;

	#endif
}

void createProblemDir ( ) {

	#ifdef WINDOWS

	ss
	<< "mkdir "
	<< kattisDir
	<< pathSeparator
	<< id;

	#else

	ss
	<< "mkdir "
	<< kattisDir
	<< pathSeparator
	<< id;

	#endif

	#ifdef DEBUGGING

	cout
	<< ss.str()
	<< endl;

	#endif

	system ( ssstr );
	ss.str("");
	
}

bool getSubkattis ( vector < string >* args ) {
	if ( args -> size ( ) > 2 ) {
		subkattis = (*args)[2];
		return true;
	}
	else{
		subkattis = "open";
		return false;
	}
} 

void getSampleData ( ) {

	string url;

	ss
	<< "https://"
	<< subkattis
	<< ".kattis.com/problems/"
	<< id
	<< "/file/statement/samples.zip";

	url = ss.str ( );
	ss.str ("");

	#ifdef WINDOWS

	ss
	<< "powershell -command \" & { iwr '"
	<< url
	<< "' -OutFile %kattis%\\"
	<< id
	<< "\\sampledata.zip } \"";

	#else

	ss
	<< "wget "
	<< url
	<< " -O "
	<< kattisDir
	<< "/"
	<< id
	<< "/sampledata.zip --no-check-certificate";

	#endif

	#ifdef DEBUGGING

	cout
	<< ss.str ( )
	<< endl;

	#endif

	system ( ssstr );
	ss.str("");
}

void unzipSampleData ( ) {

	#ifdef WINDOWS

	ss
	<< "%7z% e "

	<< kattisDir
	<< pathSeparator
	<< id
	<< pathSeparator
	<< "sampledata.zip"

	<< " -o"
	<< kattisDir
	<< pathSeparator
	<< id
	<< pathSeparator

	<< " -y";

	#else

	ss
	<< "unzip "
	<< kattisDir
	<< pathSeparator
	<< id
	<< pathSeparator
	<< "sampledata.zip -d "
	<< kattisDir
	<< pathSeparator
	<< id
	<< pathSeparator;

	#endif

	#ifdef DEBUGGING

	cout
	<< ss.str()
	<< endl;

	#endif

	system ( ssstr );
	ss.str("");
} 

void deleteSampleDataZipFile ( ) {

	#ifdef WINDOWS

	ss
	<< "del "
	<< kattisDir
	<< pathSeparator
	<< id
	<< pathSeparator
	<< "sampledata.zip";

	#else

	ss
	<< "rm "
	<< kattisDir
	<< pathSeparator
	<< id
	<< pathSeparator
	<< "sampledata.zip";

	#endif

	#ifdef DEBUGGING

	cout
	<< ss.str()
	<< endl;

	#endif
	system ( ssstr );
	ss.str("");
}
  
vector < string > ListDirectoryContents ( const char * sDir ) {

	vector < string > r;

	#ifdef WINDOWS

	WIN32_FIND_DATA fdFile;
	HANDLE hFind = NULL;

	char sPath [ 2048 ];

	sprintf ( sPath, "%s\\*.*", sDir );

	if ( ( hFind = FindFirstFile ( sPath, &fdFile ) ) == INVALID_HANDLE_VALUE ) {
		printf ( "Path not found: [%s]\n", sDir );
		return r;
	}

	do {
		if (
			strcmp ( fdFile.cFileName, "." ) != 0 &&
			strcmp ( fdFile.cFileName, ".." ) != 0
			) {

			sprintf ( sPath, "%s\\%s", sDir, fdFile.cFileName);

			r.push_back ( sPath );
		}
	} while ( FindNextFile ( hFind, & fdFile ) );

	FindClose(hFind);

	#else

	ss
	<< "ls -d -1 $PWD/"
	<< id
	<< "/* > "
	<< kattisDir
	<< pathSeparator
	<< id
	<< pathSeparator
	<< "temp";

	system ( ssstr );
	ss.str ("");

	ss
	<< kattisDir
	<< pathSeparator
	<< id
	<< pathSeparator
	<< "temp";

	ifstream temp ( ssstr );
	ss.str ("");

	if ( temp.is_open ( ) ) {
		string line;
		while ( getline ( temp, line ) )
			r.push_back ( line );
		temp.close ( );
	}

	ss
	<< "rm "
	<< kattisDir
	<< pathSeparator
	<< id
	<< pathSeparator
	<< "temp";

	system ( ssstr );
	ss.str ("");

	#endif

	return r;
}

void renameSampleFiles ( ) {
	vector < string > directoryContents = ListDirectoryContents ( string ( kattisDir + pathSeparator + id ).c_str() );

	#ifdef DEBUGGING

	cout << "Sample files are being renamed. We found the following files in the problem directory:\n";

	for ( int i = 0; i < directoryContents.size(); i++ ) cout << directoryContents[i] << endl;

	#endif

	int numFoundSampleInFiles = 0;
	int numFoundSampleOutFiles = 0;

	for ( int i = 0; i < directoryContents.size(); i++ ){
		bool shouldBeRenamed = false;
		if (
			directoryContents [ i ] [ directoryContents [ i ].size ( ) - 1 ] == 's' &&
			directoryContents [ i ] [ directoryContents [ i ].size ( ) - 2 ] == 'n' &&
			directoryContents [ i ] [ directoryContents [ i ].size ( ) - 3 ] == 'a'
			) {

			numFoundSampleOutFiles ++;

			#ifdef DEBUGGING

			cout << "Found an .ans file:\n";
			cout << "\t" << directoryContents[i] << endl;
			cout << "I will rename this.\n";

			#endif

			string newName;
			int lastPathSeparator;

			for ( int j = 0; j < directoryContents [ i ].size() - 1; j++ ){
				if ( directoryContents [ i ].substr ( j, 1 ) == pathSeparator ) {
					lastPathSeparator = j;
				}
			}

			#ifdef DEBUGGING

			cout << "Last path separator found here:\n";
			cout << "\t" << directoryContents [ i ] << endl;
			cout << "\t";
			for ( int j = 0; j < lastPathSeparator; j++ ) cout << " ";
			cout << "^\n";

			#endif

			newName = directoryContents [ i ].substr ( 0, lastPathSeparator + 1) ;

			#ifdef DEBUGGING

			cout << "Added relevant part of old name to new name:\n"
			<< newName << endl;

			#endif

			ss
			<< id
			<< numFoundSampleOutFiles
			<< ".o";

			newName +=  ss.str();
			ss.str("");

			#ifdef DEBUGGING

			cout << "I'd like to rename this file to:\n";
			cout << newName << endl;
			cout << "and you can't do anything about that.\n";

			#endif

			renameFile ( directoryContents [ i ], newName);

		} else if (
			directoryContents [ i ] [ directoryContents [ i ].size ( ) - 1 ] == 'n' &&
			directoryContents [ i ] [ directoryContents [ i ].size ( ) - 2 ] == 'i'
			) {

			numFoundSampleInFiles ++;

			#ifdef DEBUGGING

			cout << "Found an .in file:\n";
			cout << "\t" << directoryContents[i] << endl;
			cout << "I will rename this.\n";

			#endif

			string newName;
			int lastPathSeparator;

			for ( int j = 0; j < directoryContents [ i ].size() - 1; j++ ){
				if ( directoryContents [ i ].substr ( j, 1 ) == pathSeparator ) {
					lastPathSeparator = j;
				}
			}

			#ifdef DEBUGGING

			cout << "Last path separator found here:\n";
			cout << "\t" << directoryContents [ i ] << endl;
			cout << "\t";
			for ( int j = 0; j < lastPathSeparator; j++ ) cout << " ";
			cout << "^\n";

			#endif

			newName = directoryContents [ i ].substr ( 0, lastPathSeparator + 1) ;

			#ifdef DEBUGGING

			cout << "Added relevant part of old name to new name:\n"
			<< newName << endl;

			#endif

			ss
			<< id
			<< numFoundSampleInFiles
			<< ".i";

			newName +=  ss.str();
			ss.str("");

			#ifdef DEBUGGING

			cout << "I'd like to rename this file to:\n";
			cout << newName << endl;
			cout << "and there's nothing you can do about that.\n";

			#endif

			renameFile ( directoryContents [ i ], newName);
		}
	}
}

void copyTemplates ( ) {

	#ifdef WINDOWS

	ss
	<< "copy "
	<< kattisDir
	<< pathSeparator
	<< "template"
	<< pathSeparator
	<< "* "
	<< kattisDir
	<< pathSeparator
	<< id
	<< pathSeparator;

	#else

	ss
	<< "cp -i "
	<< kattisDir
	<< pathSeparator
	<< "template"
	<< pathSeparator
	<< "* "
	<< kattisDir
	<< pathSeparator
	<< id
	<< pathSeparator;

	#endif

	system ( ssstr );
	ss.str("");

	#ifdef WINDOWS

	ss
	<< "copy "
	<< kattisDir
	<< pathSeparator
	<< "template.cpp "
	<< kattisDir
	<< pathSeparator
	<< id
	<< pathSeparator
	<< id
	<< ".cpp /-Y";

	#else

	ss
	<< "cp -i "
	<< kattisDir
	<< pathSeparator
	<< "template.cpp "
	<< kattisDir
	<< pathSeparator
	<< id
	<< pathSeparator
	<< id
	<< ".cpp";


	#endif

	system ( ssstr );
	ss.str("");
}

	#ifdef WINDOWS

void openTemplateFile ( ) {

	ss
	<< kattisDir
	<< pathSeparator
	<< id
	<< pathSeparator
	<< id
	<< ".cpp";

	system ( ssstr );
	ss.str ( "" );
	
}

	#else

void openTemplateFile ( string cmd ) {

	ss
	<< cmd
	<< " "
	<< kattisDir
	<< pathSeparator
	<< id
	<< pathSeparator
	<< id
	<< ".cpp";

	system ( ssstr );
	ss.str ( "" );

}

	#endif

int main ( int argc, const char * argv [ ] ){

	vector < string > args ( argv, argv + argc );

	if ( args.size ( ) < 2){
		cout << "Please supply a problem id.\n";
		return 0;
	}

	id = args [ 1 ];

	if ( id [ id.size ( ) - 1 ] == '/' )
		id.erase ( id.size ( ) - 1 );
	
	if ( 3 <= args.size ( ) )
		subkattis = args [2];

	setKattisDir ( );

	createProblemDir ( );

	getSubkattis ( &args ) ;

	getSampleData ( );

	unzipSampleData ( );

	deleteSampleDataZipFile ( );

	renameSampleFiles ( );

	copyTemplates ( );

	#ifdef WINDOWS

	openTemplateFile ( );

	#else

	if ( 4 <= args.size ( ) )
		openTemplateFile ( args [ 3 ] );

	#endif

	return 0;
}