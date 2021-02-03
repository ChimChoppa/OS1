#include <pch.h>
#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

//Меню
int showMenu();

// функции вывода списка дисков
void getLogicalDrives();
void getLogicalDriveStrings();

// функции вывода информации о диске и размере свободного пространства
void getVolumeInfo(std::string diskName);

// Функции создания и удаления каталогов
void createDirectory();
void removeDirectory();

// Функции создания, копирования и перемещения файлов между каталогами
void createFile();
void copyFile();
void moveFile();
void moveFileEx();

// Функции получения информации и изменения атрибутов файлов
void getFAttributes();
void setFAttributes();
void getFileInfo();
void getFileTime();
void setFileTime();

int showMenu()
{
	int res; // ввод пользователя
	cout << "Меню:" << endl
		<< "1. Список дисков " << endl
		<< "2. Информация о диске " << endl
		<< "3. Создать каталог " << endl
		<< "4. Удалить каталог " << endl
		<< "5. Создать файл " << endl
		<< "6. Копировать файл " << endl
		<< "7. Переместить файл " << endl
		<< "8. Переместить файл с флагом " << endl
		<< "9. Получить атрибуты файла " << endl
		<< "10. Установить атрибуты файла " << endl
		<< "11. Информация о файле " << endl
		<< "12. Вывод временных характеристик файла " << endl
		<< "13. Изменить временные характеристики файла " << endl
		<< "0. Выход " << endl
		<< ">>";
	cin >> res;
	return res;
}

// Первый способ вывода дисков
void getLogicalDrives() {

	int temp;
	DWORD logDrives = GetLogicalDrives();

	cout << "Доступные диски:" << endl;
	// проходимся по битам, если текущий единица, то i-й диск есть в системе
	for (int i = 0; i < 26; ++i)
	{
		temp = (logDrives >> i) & 1;
		if (temp)
		{
			cout << (char)(65 + i) << ":\\   ";
		}

	}
	cout << endl;
}

// Второй способ вывода дисков
void getLogicalDriveStrings() {
	wchar_t disks[256];
	wchar_t* disk;
	DWORD bufSize = 256;
	GetLogicalDriveStrings(bufSize, disks);
	disk = disks;
	cout << "Доступные диски:" << endl;
	while (*disk)
	{
		wprintf(L"%s   ", disk);
		disk += wcslen(disk) + 1;
	}
	cout << endl;
}

// Вывод информации о диске и размере свободного пространства
void getVolumeInfo(string diskName) {

	char VolumeNameBuffer[100];
	char FileSystemNameBuffer[100];
	unsigned long VolumeSerialNumber;
	DWORD VolumeFlags;

	if (GetVolumeInformationA(
		(LPCSTR)diskName.c_str(),
		VolumeNameBuffer,
		100,
		&VolumeSerialNumber,
		NULL,
		&VolumeFlags,
		FileSystemNameBuffer,
		100
	))
	{
		cout << "Имя диска -   " << VolumeNameBuffer << endl <<
			"Серийный номер диска -   " << VolumeSerialNumber << endl <<
			"Файловая система -   " << FileSystemNameBuffer << endl;

		cout << "Флаги файловой системы: " << endl;
		if (VolumeFlags & FS_CASE_IS_PRESERVED)
			cout << "  При записи на диск сохраняется регистр букв в имени файла" << endl;
		if (VolumeFlags & FS_FILE_COMPRESSION)
			cout << "  Файловая система поддерживает сжатие файлов" << endl;
		if (VolumeFlags & FS_VOL_IS_COMPRESSED)
			cout << "  Том был подвергнут сжатию" << endl;
		if (VolumeFlags & FILE_NAMED_STREAMS)
			cout << "  Файловая система поддерживает именованные потоки" << endl;
		if (VolumeFlags & FILE_SUPPORTS_ENCRYPTION)
			cout << "  Файловая система поддерживает Encrypted File System (EFS)" << endl;
		if (VolumeFlags & FILE_SUPPORTS_OBJECT_IDS)
			cout << "  Файловая система поддерживает идентификаторы объектов" << endl;
	}
	else
		cout << " Не удалось получить информацию. Код ошибки: " << GetLastError() << endl;

}

// Создание каталога
void createDirectory()
{
	string nameCreateDir;

	cout << "Введите конечный путь к создаваемому каталогу(пример c://newDir):" << endl;
	cin >> nameCreateDir;


	if (CreateDirectoryA((LPCSTR)nameCreateDir.c_str(), NULL))
		cout << "Каталог " << nameCreateDir << " создан." << endl;
	else
		cout << " Произошла ошибка. Каталог не был создан. Код ошибки: " << GetLastError() << endl;
	cout << endl;
}

// Удаление каталога
void removeDirectory()
{
	string nameRemoveDir;

	cout << "Введите конечный путь к удаляемому каталогу(пример c://newDir):" << endl;
	cin >> nameRemoveDir;


	if (RemoveDirectoryA((LPCSTR)nameRemoveDir.c_str()))
		cout << "Каталог " << nameRemoveDir << " успешно удален." << endl;
	else
		cout << " Произошла ошибка. Каталог не был удален. Код ошибки: " << GetLastError() << endl;
	cout << endl;
}

// Создание файла
void createFile()
{
	HANDLE handleFile;
	string fileCreateDir;

	cout << " Введите конечный путь к создаваемому файлу(пример c://newDir//newFile.txt): " << endl;
	cin >> fileCreateDir;

	handleFile = CreateFileA((LPCSTR)fileCreateDir.c_str(), GENERIC_READ |
		GENERIC_WRITE, 0, NULL, CREATE_NEW, 0, NULL);
	if (handleFile)
		cout << " Файл " << fileCreateDir << " был создан." << endl;
	else
		cout << " Произошла ошибка. Файл не был создан. Код ошибки: " << GetLastError() << endl;

	if (handleFile) {
		CloseHandle(handleFile);
	}

	cout << endl;
}

// Копирование файла
void copyFile()
{
	string firstName, copyName;

	cout << " Введите конечный путь к копируемому файлу(пример c://newDir//oldFile.txt): " << endl;
	cin >> firstName;

	cout << " Введите конечный путь к новому файлу(пример c://newDir//newFile.txt): " << endl;
	cin >> copyName;

	if (CopyFileA((LPCSTR)firstName.c_str(), (LPCSTR)copyName.c_str(), 0))
		cout << " Файл " << firstName << " успешно скопирован." << endl;
	else {
		if (GetLastError() == 183)
			cout << " Файл с таким названием уже существует." << endl;
		else
			cout << " Произошла ошибка. Копирование не выполнено. Код ошибки: " << GetLastError() << endl;
	}

}

// Перемещение файла
void moveFile() {
	string firstPath, newPath;

	cout << " Введите конечный путь к перемещаемому файлу(пример c://newDir//oldFile.txt): " << endl;
	cin >> firstPath;

	cout << " Введите конечный путь к новому файлу(пример c://newDir//newFile.txt): " << endl;
	cin >> newPath;

	if (MoveFileA((LPCSTR)firstPath.c_str(), (LPCSTR)newPath.c_str()))
		cout << " Файл " << firstPath << " был перемещен." << endl;
	else {
		if (GetLastError() == 183)
			cout << " Файл с таким названием уже существует." << endl;
		else
			cout << " Произошла ошибка. Перемещение не выполнено. Код ошибки: " << GetLastError() << endl;
	}
	cout << endl;
}

// Перемещение файла со всем содержимым
void moveFileEx() {
	string firstPath, newPath;

	cout << " Введите конечный путь к перемещаемому файлу(пример c://newDir//oldFile.txt): " << endl;
	cin >> firstPath;

	cout << " Введите конечный путь к новому файлу(пример c://newDir//newFile.txt): " << endl;
	cin >> newPath;


	if (MoveFileExA((LPCSTR)firstPath.c_str(), (LPCSTR)newPath.c_str(), MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING))
		cout << " Файл " << firstPath << " был перемещен." << endl;
	else
		cout << " Произошла ошибка. Перемещение не выполнено. Код ошибки: " << GetLastError() << endl;
	cout << endl;
}

// Получение атрибутов файла
void getFAttributes() {
	string path;
	long FileAttributes;

	cout << " Введите конечный путь к проверяемому файлу/каталогу(пример c://newDir//newFile.txt): " << endl;
	cin >> path;

	FileAttributes = GetFileAttributesA((LPCSTR)path.c_str());

	if (FileAttributes == -1)
		cout << " Файл " << path << " не найден." << endl;
	else {
		if (FileAttributes & FILE_ATTRIBUTE_ARCHIVE)
			cout << " Файл " << path << " - архивный." << endl;
		if (FileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			cout << " Файл " << path << " - директория." << endl;
		if (FileAttributes & FILE_ATTRIBUTE_READONLY)
			cout << " Файл " << path << " - только для чтения." << endl;
		if (FileAttributes & FILE_ATTRIBUTE_SYSTEM)
			cout << " Файл " << path << " - системный." << endl;
		if (FileAttributes & FILE_ATTRIBUTE_COMPRESSED)
			cout << " Файл " << path << " - сжатый." << endl;
		if (FileAttributes & FILE_ATTRIBUTE_HIDDEN)
			cout << " Файл " << path << " - скрытый." << endl;
		if (FileAttributes & FILE_ATTRIBUTE_NORMAL)
			cout << " Файл " << path << " не имеет таких атрибутов." << endl;
		if (FileAttributes == 0)
			cout << " Произошла ошибка. Код ошибки: " << GetLastError() << endl;
	}
	cout << endl;
}


// Установка атрибутов файла
void setFAttributes() {
	string path;
	int key;
	long FileAttributes;

	cout << " Введите конечный путь к файлу/каталогу, которому нужно установить атрибуты(пример c://newDir//newFile.txt): " << endl;
	cin >> path;

	cout << "1 - Сбросить все атрибуты " << endl
		<< "2 - Сделать скрытым файлом" << endl
		<< "3 - Сделать архивным файлом" << endl
		<< "4 - Сделать файлом 'только для чтения'" << endl
		<< "5 - Сделать системным файлом" << endl
		<< ">> " << endl;
	cin >> key;

	FileAttributes = GetFileAttributesA((LPCSTR)path.c_str());

	switch (key) {
	case 1:
		FileAttributes = FILE_ATTRIBUTE_NORMAL;
		break;
	case 2:
		FileAttributes |= FILE_ATTRIBUTE_HIDDEN;
		break;
	case 3:
		FileAttributes |= FILE_ATTRIBUTE_ARCHIVE;
		break;
	case 4:
		FileAttributes |= FILE_ATTRIBUTE_READONLY;
		break;
	case 5:
		FileAttributes |= FILE_ATTRIBUTE_SYSTEM;
		break;
	default:
		cout << "Ключ был введен неверно." << endl;
		break;
	}

	if (key > 0 && key < 6) {
		if (SetFileAttributesA((LPCSTR)path.c_str(), FileAttributes))
			cout << " Атрибуты успешно установлены." << endl;
		else
			cout << " Произошла ошибка при установке атрибутов. Код ошибки: " << GetLastError() << endl;
	}
	cout << endl;
}

// Получение информации о файле
void getFileInfo() {
	long handleInfo, FileAttributes;
	BY_HANDLE_FILE_INFORMATION infoFile;
	string path;
	HANDLE handleFile;

	cout << " Введите конечный путь к файлу/каталогу, о котором нужно получить информацию(пример c://newDir//newFile.txt): " << endl;
	cin >> path;

	handleFile = CreateFileA((LPCSTR)path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	handleInfo = GetFileInformationByHandle(handleFile, &infoFile);
	if (!handleInfo)
		cout << " Произошла ошибка. Код ошибки: " << GetLastError() << endl;
	else {
		FileAttributes = infoFile.dwFileAttributes;
		if (FileAttributes == -1)
			cout << " Файл " << path << " не найден." << endl;
		else {

			cout << "Информация о файле по дескриптору:" << endl;

			if (FileAttributes & FILE_ATTRIBUTE_ARCHIVE)
				cout << " Файл " << path << " - архивный." << endl;
			if (FileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				cout << " Файл " << path << " - директория." << endl;
			if (FileAttributes & FILE_ATTRIBUTE_READONLY)
				cout << " Файл " << path << " - только для чтения." << endl;
			if (FileAttributes & FILE_ATTRIBUTE_SYSTEM)
				cout << " Файл " << path << " - системный." << endl;
			if (FileAttributes & FILE_ATTRIBUTE_COMPRESSED)
				cout << " Файл " << path << " - сжатый." << endl;
			if (FileAttributes & FILE_ATTRIBUTE_HIDDEN)
				cout << " Файл " << path << " - скрытый." << endl;
			if (FileAttributes & FILE_ATTRIBUTE_NORMAL)
				cout << " Файл " << path << " не имеет таких атрибутов.";
			if (FileAttributes == 0)
				cout << " Произошла ошибка. Код ошибки: " << GetLastError() << endl;
		}


		cout << " Старшая часть уникального идентификатора связи с файлом: " << infoFile.nFileIndexHigh << endl;
		cout << " Порядковый номер тома, который содержит файл: " << infoFile.dwVolumeSerialNumber << endl;
		cout << " Старшая часть размера файла: " << infoFile.nFileSizeHigh << endl;
		cout << " Младшая часть размера файла: " << infoFile.nFileSizeLow << endl;
		cout << " Число связей с этим файлом: " << infoFile.nNumberOfLinks << endl;
		cout << " Младшая часть уникального идентификатора связи с файлом: " << infoFile.nFileIndexLow << endl;
	}
	handleInfo = CloseHandle(handleFile);
	cout << endl;
}

// Получение времени создания файла
void getFileTime() {

	HANDLE handleFile;
	string path;

	FILETIME createTime, accessFile, lastWFile;

	SYSTEMTIME UTCtime, localTime;

	cout << " Введите конечный путь к проверяемому файлу/каталогу(пример c://newDir//newFile.txt): " << endl;
	cin >> path;

	handleFile = CreateFileA((LPCSTR)path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING | CREATE_NEW, 0, NULL);
	if (!handleFile)
		cout << " Произошла ошибка. Код ошибки: " << GetLastError() << endl;
	else {
		GetFileTime(handleFile, &createTime, &accessFile, &lastWFile);

		FileTimeToSystemTime(&createTime, &UTCtime);

		SystemTimeToTzSpecificLocalTime(NULL, &UTCtime, &localTime);
		cout << " Дата и время создания файла: " << endl
			<< localTime.wDay << "." << localTime.wMonth << "." << localTime.wYear << endl
			<< localTime.wHour << ":" << localTime.wMinute << endl;

		FileTimeToSystemTime(&accessFile, &UTCtime);

		SystemTimeToTzSpecificLocalTime(NULL, &UTCtime, &localTime);
		cout << " Время последнего доступа к файлу: " << endl
			<< localTime.wDay << "." << localTime.wMonth << "." << localTime.wYear << endl
			<< localTime.wHour << ":" << localTime.wMinute << endl;

		FileTimeToSystemTime(&lastWFile, &UTCtime);

		SystemTimeToTzSpecificLocalTime(NULL, &UTCtime, &localTime);
		cout << " Время последней записи в файл: " << endl
			<< localTime.wDay << "." << localTime.wMonth << "." << localTime.wYear << endl
			<< localTime.wHour << ":" << localTime.wMinute << endl;
	}
	CloseHandle(handleFile);
	cout << endl;
}

// Установка времени создания файла
void setFileTime() {

	HANDLE handleFile;
	string path;

	FILETIME createTime, accessFile, lastWFile;

	SYSTEMTIME UTCtime, localTime;


	cout << " Введите конечный путь к файлу/каталогу, которому нужно установить временные атрибуты(пример c://newDir//newFile.txt): " << endl;
	cin >> path;

	handleFile = CreateFileA((LPCSTR)path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING | FILE_WRITE_ATTRIBUTES, 0, NULL);
	if (!handleFile)
		cout << " Произошла ошибка. Код ошибки: " << GetLastError() << endl;
	else {

		GetFileTime(handleFile, &createTime, &accessFile, &lastWFile);
		GetSystemTimeAsFileTime(&accessFile);

		SetFileTime(handleFile, &createTime, &accessFile, &lastWFile);
		FileTimeToSystemTime(&accessFile, &UTCtime);
		SystemTimeToTzSpecificLocalTime(NULL, &UTCtime, &localTime);

		cout << "Текущее время: " << endl
			<< localTime.wDay << "." << localTime.wMonth << "." << localTime.wYear << endl
			<< localTime.wHour << ":" << localTime.wMinute << endl
			<< "установлено временем последнего доступа к файлу" << endl;
	}
	CloseHandle(handleFile);
	cout << endl;
}

int main()
{
	setlocale(LC_ALL, "Rus");

	int menu;

	string diskName;

	DWORD sectorsPerCluster;
	DWORD bytesPerSector;
	DWORD numberOfFreeClusters;
	DWORD totalNumberOfClusters;


	do
	{
		system("cls");
		menu = showMenu();
		switch (menu)
		{
		case 1: // Вывод списка дисков двумя способами
			system("cls");
			cout << "Первый способ" << endl;
			getLogicalDrives();

			cout << "Второй способ" << endl;
			getLogicalDriveStrings();

			system("pause");
			break;
		case 2:

			system("cls");
			//Выбор диска для работы
			getLogicalDrives();
			cout << "Введите имя диска для работы (одним символом) " << endl;
			cin >> diskName;
			diskName += "://";

			// Вывод типа устройства
			switch (GetDriveTypeA((LPCSTR)diskName.c_str())) {
			case 0:
				cout << "Тип устройства определить не удалось" << endl;
				break;
			case 1:
				cout << "Корневой директории не существует" << endl;
				break;
			case 2:
				cout << "Этот диск является сменным диском (например дискетта или любой сменный диск)" << endl;
				break;
			case 3:
				cout << "Этот диск является жестким диском" << endl;
				break;
			case 4:
				cout << "Этот диск является сетевым диском" << endl;
				break;
			case 5:
				cout << "Этот диск является CD-ROM" << endl;
				break;
			case 6:
				cout << "Этот диск является RAM-диском" << endl;
				break;
			}

			//Вывод информации о диске
			cout << "Информация о диске: " << endl;
			getVolumeInfo(diskName);

			//Вывод информации о памяти диска
			cout << "Информация о памяти диска: " << endl;

			if (GetDiskFreeSpaceA((LPCSTR)diskName.c_str(),
				&sectorsPerCluster,
				&bytesPerSector,
				&numberOfFreeClusters,
				&totalNumberOfClusters)) {
				cout << "  Число секторов в кластере: " << sectorsPerCluster << endl;
				cout << "  Число байт в секторе: " << bytesPerSector << endl;
				cout << "  Число свободных кластеров: " << numberOfFreeClusters << endl;
				cout << "  Общее число кластеров: " << totalNumberOfClusters << endl;
			}
			else
				cout << " Не удалось получить информацию. Код ошибки: " << GetLastError() << endl;


			system("pause");
			break;

		case 3:
			system("cls");
			createDirectory();

			system("pause");
			break;
		case 4:
			system("cls");
			removeDirectory();

			system("pause");
			break;
		case 5:
			system("cls");
			createFile();


			system("pause");
			break;
		case 6:
			system("cls");
			copyFile();

			system("pause");
			break;
		case 7:
			system("cls");
			moveFile();

			system("pause");
			break;
		case 8:
			system("cls");
			moveFileEx();

			system("pause");
			break;
		case 9:
			system("cls");
			getFAttributes();

			system("pause");
			break;
		case 10:
			system("cls");
			setFAttributes();

			system("pause");
			break;
		case 11:
			system("cls");
			getFileInfo();

			system("pause");
			break;
		case 12:
			system("cls");
			getFileTime();

			system("pause");
			break;
		case 13:
			system("cls");
			setFileTime();

			system("pause");
			break;
		case 0:
			system("cls");
			break;
		default:
			cout << "Вы выбрали несуществующий пункт меню, повторите ввод!" << endl;
			system("pause");
			break;
		}


	} while (menu != 0);

	return 0;
}
