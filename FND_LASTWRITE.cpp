// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

unsigned int check_first(std::string list_of_file, std::string file_name)
{//depracated

	std::ifstream myfile;
	std::string appo;
	char buffer[256];
	
	memset(buffer, 0, sizeof buffer);

	myfile.open(list_of_file, std::ios_base::in);

	if ( !myfile.is_open() )
	{
		
		std::cout << "File "<< list_of_file << " not found"<< std::endl;
		return 0;
	}

	myfile.getline(buffer, sizeof buffer);
	appo=buffer;
	for_each (file_name.begin(), file_name.end(), tolower);
	for_each (appo.begin(), appo.end(), tolower);

	return (file_name==appo? 1 : 0);
}
 

std::stringstream printTime(SYSTEMTIME stime)
{
	 std::stringstream ss;
	 ss << '\t';
	 ss << stime.wYear << '-';
	 ss << stime.wMonth << '-';
	 ss << stime.wDay << '\t';
	 ss << stime.wHour << ':';
	 ss << stime.wMinute << ':';
	 ss << stime.wSecond << '\t';
   
	 return ss;
	
}

char* convertTChar2Char(char* _out_,  const TCHAR* _in_, size_t bufLength)
{
	    size_t appo;
		wcstombs_s(&appo, _out_, bufLength,   _in_ , bufLength );
		return ( appo > 0 ? _out_: 0 );
}


WIN32_FIND_DATA listDir(   std::string directory)
{
   WIN32_FIND_DATA ffd;
   WIN32_FIND_DATA retVal;
   LARGE_INTEGER filesize;
   TCHAR szDir[MAX_PATH];
   size_t length_of_arg;
   HANDLE hFind = INVALID_HANDLE_VALUE;
   DWORD dwError=0;
   char pathLocal[MAX_PATH+1];
	   

   memset (szDir, 0, MAX_PATH);
   memset (&retVal, 0, sizeof WIN32_FIND_DATA);
   // If the directory is not specified as a command-line argument,
   // print usage.


   // Check that the input path plus 3 is not longer than MAX_PATH.
   // Three characters are for the "\*" plus NULL appended below.
    std::copy(directory.begin(), directory.end(), szDir);
 
   _tprintf(TEXT("\nTarget directory is %s\n\n"), szDir);

   // Prepare string for use with FindFile functions.  First, copy the
   // string to a buffer, then append '\*' to the directory name.
    StringCchCat(szDir, MAX_PATH, TEXT("\\*"));
	

   // Find the first file in the directory. is the '.'

	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind) 
	{
      std::cout << "Invalid handle"<< convertTChar2Char(pathLocal, ffd.cFileName, MAX_PATH) << std::endl;
      return retVal;
	}
	SYSTEMTIME st;
       
	FileTimeToSystemTime(&ffd.ftLastWriteTime, &st);
	retVal=ffd;
	std::cout << "Local Dir: "<< convertTChar2Char(pathLocal, ffd.cFileName, MAX_PATH) <<printTime(st).str() << std::endl;
   
   
   // List all the files in the directory with some info about them.

   while (FindNextFile(hFind, &ffd) != 0)
   {

		char path[MAX_PATH+1];
	    size_t stica;
		wcstombs_s(&stica, path, MAX_PATH,   ffd.cFileName , MAX_PATH );


      if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {
		 if ( std::string(path).find("..")==std::string::npos )
		 { 
			SYSTEMTIME st;
       
			FileTimeToSystemTime(&ffd.ftLastWriteTime, &st);
		 
			std::cout << "<DIR>\n"<< directory+"\\"+path <<printTime(st).str()  << std::endl;
		 	listDir(directory+"\\"+path);
		 }

      }
      else
      {
         filesize.LowPart = ffd.nFileSizeLow;
         filesize.HighPart = ffd.nFileSizeHigh;
		 SYSTEMTIME st;
       
		 FileTimeToSystemTime(&ffd.ftLastWriteTime, &st);
		 std::cout << path << '\t' <<printTime(st).str()  <<std::endl;
		
		 retVal =	retVal.ftLastWriteTime.dwHighDateTime <= ffd.ftLastWriteTime.dwHighDateTime &&
					retVal.ftLastWriteTime.dwLowDateTime  <= ffd.ftLastWriteTime.dwLowDateTime   ?
					ffd : retVal;		
      }
   }
   
 
   dwError = GetLastError();
   if (dwError != ERROR_NO_MORE_FILES) 
   {
      std::cout <<("FindFirstFile") << std::endl;
   }
   _tprintf(TEXT("\nEnd Exploring Dir %s\n\n\n\n"), szDir);

   FindClose(hFind);
   return retVal;

}

std::string printHelp()
{
	std::string ret("");

	ret +="\nFND_FIRST <clue_file> <path>\n\n";
	ret +="\tlook for the oldest file inside a directory and its subdirectory\n";
	ret +="\tcheck if the oldes is the <clue_file> in this cfase return 1\n";
	ret +="\t otherwise return 0\n\n";
	ret +="Without parameters print this message\n\n";

	return ret;
}



int main(int argc, char* argv[])
{
	int ret = 0;
	WIN32_FIND_DATA retVal;
	char lastWrite[MAX_PATH+1];
    size_t stica;
	
	memset (lastWrite, 0, sizeof lastWrite);

	if ( argc != 3 )
	{
		std::cout << std::endl <<	printHelp() << std::endl;
	}
	else 
	{
		retVal=listDir( argv[2]);

		wcstombs_s(&stica, lastWrite, MAX_PATH,   retVal.cFileName , MAX_PATH );


		ret = std::string(lastWrite)==std::string(argv[1])  ;
		std::cout <<  lastWrite  <<"\t" <<argv[1] << std::endl;
		std::cout << (bool) ret << std::endl;
		if ( ret )
		{
			std::ofstream vai;

			vai.open("vai.vai", std::ios_base::out);
			if (vai.is_open())
			{
				vai << argv[1] <<" aggiornato " << std::endl;
			}
			vai.close();
		}
	}
	return 	ret;
}

