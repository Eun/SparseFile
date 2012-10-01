#define _WIN32_WINNT 0x0500
#include <stdio.h>
#include <windows.h>
#include <winioctl.h>
HANDLE CreateSparseFile(LPCTSTR lpSparseFileName)
{
    // Use CreateFile as you would normally - Create file with whatever flags 
    //and File Share attributes that works for you
    DWORD dwTemp;

    HANDLE hSparseFile = CreateFile(lpSparseFileName, 
                                    GENERIC_READ|GENERIC_WRITE, 
                                    FILE_SHARE_READ|FILE_SHARE_WRITE, 
                                    NULL, 
                                    CREATE_ALWAYS, 
                                    FILE_ATTRIBUTE_NORMAL, 
                                    NULL);

    if (hSparseFile == INVALID_HANDLE_VALUE) 
        return hSparseFile;

    DeviceIoControl(hSparseFile, 
                    FSCTL_SET_SPARSE, 
                    NULL, 
                    0, 
                    NULL, 
                    0, 
                    &dwTemp, 
                    NULL);
    return hSparseFile;
}
DWORD SetSparseRange(HANDLE hSparseFile, LONGLONG start, ULONGLONG size)
{
    // Specify the starting and the ending address (not the size) of the 
    // sparse zero block
    FILE_ZERO_DATA_INFORMATION fzdi;
    fzdi.FileOffset.QuadPart = start;
    fzdi.BeyondFinalZero.QuadPart = start + size; 

    // Mark the range as sparse zero block
    DWORD dwTemp;
    SetLastError(0);
    BOOL bStatus = DeviceIoControl(hSparseFile, 
                                 FSCTL_SET_ZERO_DATA, 
                                 &fzdi, 
                                 sizeof(fzdi), 
                         NULL, 
                                 0, 
                                 &dwTemp, 
                                 NULL);
    if (bStatus) return 0; //Sucess
    else
    {
        DWORD e = GetLastError();
        return(e); //return the error value
    }
}

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		printf("usage: sparsefile.exe <file> <size>\n  file: file to create\n  size: in byte max 17591112302592\n");
		return 0;
	}
	BOOL bStatus;
	__int64 size = _atoi64(argv[2]);
	HANDLE file = CreateSparseFile(argv[1]);
	LARGE_INTEGER x;
	x.QuadPart = size;
	bStatus = SetFilePointerEx(file, x, 0, FILE_BEGIN);
    bStatus = SetEndOfFile(file);
	DWORD LastErr = GetLastError();
	SetSparseRange(file, 0, size);
	CloseHandle(file);
	return 0;
}