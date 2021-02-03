#include <pch.h>
#include <iostream>
#include <tchar.h>
#include <Windows.h>


# pragma comment(lib, "winmm.lib")

#define BLOCK 4096 // размер кластера диска

using namespace std;

//Прототипы функций для работы

HANDLE createF(bool flagRW);
void copyFile(HANDLE inFileHandle, HANDLE outFileHandle, DWORD block, int operationsCount);


int callback = 0;

VOID CALLBACK FileIOCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped) {

	++callback;

}

int main()
{
	setlocale(LC_ALL, "Russian");

	HANDLE inFileHandle,
		outFileHandle;


	int operationsCount = 0,
		blockCount = 0;

	DWORD block = 0,
		startTime = 0,
		endTime = 0;


	bool restart = true;

	do {

		cout << "Введите конечный путь к исходному файлу (пример c://newDir//testFile.docx)" << endl;
		inFileHandle = createF(true);

		cout << "Введите конечный путь к файлу-результату (пример c://newDir//newTestFile.docx)" << endl;
		outFileHandle = createF(false);


		cout << "Введите множитель для размера блока (1 блок = 4096 байт) " << endl
			<< ">>" << endl;
		cin >> blockCount;
		cout << "Выберите число перекрывающихся операций ввода/вывода (1, 2, 4, 8, 12, 16): " << endl
			<< ">>" << endl;
		cin >> operationsCount;


		block = BLOCK * blockCount;

		startTime = timeGetTime();

		copyFile(inFileHandle, outFileHandle, block, operationsCount);

		endTime = timeGetTime();

		CloseHandle(inFileHandle);

		CloseHandle(outFileHandle);

		cout << "\nВремя выполнения в миллисекундах: " << endTime - startTime << endl;

		cout << "Повторить выполнение программы? " << endl
			<< "1 - повторить, 0 - выйти из программы" << endl;

		cin >> restart;

		system("cls");

	} while (restart);

	return 0;

}

HANDLE createF(bool flagRW) {

	HANDLE FileHandle;

	char fileName[MAX_PATH + 1];


	cin >> fileName;

	FileHandle = CreateFileA(fileName, flagRW ? GENERIC_READ : GENERIC_WRITE, 0,
		NULL, flagRW ? OPEN_EXISTING : CREATE_ALWAYS, FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL);

	return FileHandle;

}

void copyFile(HANDLE inFileHandle, HANDLE outFileHandle, DWORD block, int operationsCount) {

	int fileSize = GetFileSize(inFileHandle, NULL);
	int oldFileSize = fileSize, countOperations = 0;

	CHAR* buffer = new CHAR[fileSize + (fileSize % block > 0 ? 1 : 0) * block];
	CHAR* outbuffer = new CHAR[fileSize + (fileSize % block > 0 ? 1 : 0) * block];

	OVERLAPPED* inover = new OVERLAPPED[operationsCount];
	OVERLAPPED* outover = new OVERLAPPED[operationsCount];


	for (int i = 0; i < operationsCount; i++)
	{

		inover[i].Offset = 0;

		inover[i].OffsetHigh = 0;

		inover[i].hEvent = NULL;

		outover[i].Offset = 0;

		outover[i].OffsetHigh = 0;

		outover[i].hEvent = NULL;

	}

	outbuffer = buffer;


	bool flagIn = false,
		flagOut = false;

	do
	{

		callback = 0;

		countOperations = 0;

		for (int i = 0; i < operationsCount; i++)
		{
			if (fileSize > 0)
			{
				++countOperations;
				if (i || flagIn)
				{

					for (int j = 0; j < operationsCount; j++)
					{
						inover[j].Offset += block;
					}
					buffer += block;
				}

				flagIn = true;

				ReadFileEx(inFileHandle, buffer, block, &inover[i], FileIOCompletionRoutine);

				fileSize -= block;

			}

		}

		while (callback < countOperations)
		{

			SleepEx(-1, TRUE);

		}

		callback = 0;

		for (int i = 0; i < countOperations; i++)
		{

			if (i || flagOut)
			{
				for (int j = 0; j < operationsCount; j++)
				{
					outover[j].Offset += block;
				}

				outbuffer += block;
			}

			flagOut = true;

			WriteFileEx(outFileHandle, outbuffer, block, &outover[i], FileIOCompletionRoutine);


		}

		while (callback < countOperations)
		{

			SleepEx(-1, TRUE);

		}

	} while (fileSize > 0);

	SetFilePointer(outFileHandle, oldFileSize, NULL, FILE_BEGIN);

	SetEndOfFile(outFileHandle);

}
