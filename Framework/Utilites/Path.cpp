#include "Framework.h"
#include "Path.h"

// string���� ������ wstring���� �ٲ㼭 boolüũ
// ������ �����ϴ��� üũ
bool Path::ExistFile(string path)
{
	return ExistFile(String::ToWstring(path));
}

// ������ �����ϴ��� üũ
bool Path::ExistFile(wstring path)
{
	DWORD fileValue = GetFileAttributes(path.c_str());
	// ���н� -1 ��ȯ(uint���̹Ƿ� uint�� �ִ밪), �� �̿ܿ��� ����
	return fileValue < 0xFFFFFFFF;
}

// ���丮�� �����ϴ��� üũ
bool Path::ExistDirectory(string path)
{
	return ExistDirectory(String::ToWstring(path));
}

bool Path::ExistDirectory(wstring path)
{
	DWORD attribute = GetFileAttributes(path.c_str());

	// ���丮�϶��� true
	BOOL temp = (attribute != INVALID_FILE_ATTRIBUTES &&
		(attribute & FILE_ATTRIBUTE_DIRECTORY));


	return temp == TRUE;
}

// ���ڿ� ��ġ��
string Path::Combine(string path1, string path2)
{
	return path1 + path2;
}

wstring Path::Combine(wstring path1, wstring path2)
{
	return path1 + path2;
}

// vector�� ���� ��� ��ġ��
string Path::Combine(vector<string> paths)
{
	string temp = "";
	for (string path : paths)
		temp += path;

	return temp;
}

wstring Path::Combine(vector<wstring> paths)
{
	wstring temp = L"";
	for (wstring path : paths)
		temp += path;

	return temp;
}

// ���丮 �̸� ��ȯ
string Path::GetDirectoryName(string path)
{
	// path�� ������ / ���� �߶� ���ϸ� ����
	String::Replace(&path, "\\", "/");
	size_t index = path.find_last_of('/');

	return path.substr(0, index + 1);
}

wstring Path::GetDirectoryName(wstring path)
{
	String::Replace(&path, L"\\", L"/");
	size_t index = path.find_last_of('/');

	return path.substr(0, index + 1);
}

// ���ϸ� ���ϴ� �Լ�
string Path::GetExtension(string path)
{
	// .���� ���ڿ��� �߶� ��ȯ
	String::Replace(&path, "\\", "/");
	size_t index = path.find_last_of('.');

	return path.substr(index + 1, path.length());;
}

wstring Path::GetExtension(wstring path)
{
	String::Replace(&path, L"\\", L"/");
	size_t index = path.find_last_of('.');

	return path.substr(index + 1, path.length());;
}

// ���ϸ� ��ȯ
string Path::GetFileName(string path)
{
	// ������ /���� ���ϸ� ��ȯ
	String::Replace(&path, "\\", "/");
	size_t index = path.find_last_of('/');

	return path.substr(index + 1, path.length());
}

wstring Path::GetFileName(wstring path)
{
	String::Replace(&path, L"\\", L"/");
	size_t index = path.find_last_of('/');

	return path.substr(index + 1, path.length());
}

// Ȯ���� ������ ���ϸ� ��ȯ�Լ�
string Path::GetFileNameWithoutExtension(string path)
{
	// ���ϸ� ���ϰ� .�������� ���ڿ� ��ȯ
	string fileName = GetFileName(path);

	size_t index = fileName.find_last_of('.');
	return fileName.substr(0, index);
}

wstring Path::GetFileNameWithoutExtension(wstring path)
{
	wstring fileName = GetFileName(path);

	size_t index = fileName.find_last_of('.');
	return fileName.substr(0, index);
}

const WCHAR* Path::ImageFilter = L"Image\0*.png;*.bmp;*.jpg";
const WCHAR* Path::ShaderFilter = L"HLSL file\0*.hlsl";
const WCHAR* Path::TextFilter = L"Text file\0*.txt";
const WCHAR* Path::TileMapDataFilter = L"TileMapData file\0*.tilemap";
const WCHAR* Path::SpriteInfoFilter = L"SpriteInfoData file\0*.sif";
const WCHAR* Path::SpriteFilter = L"Sprite\0*.png;*.bmp;*.jpg;*.json";

// ������ ���� ��ȭâ
void Path::OpenFileDialog(wstring file, const WCHAR* filter, wstring folder, function<void(wstring)> func, HWND hwnd)
{
	WCHAR name[255];
	wcscpy_s(name, file.c_str());

	wstring tempFolder = folder;

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = filter;	// � ������ �ҷ����� ����
	ofn.lpstrFile = name;
	ofn.lpstrFileTitle = L"�ҷ�����";
	ofn.nMaxFile = 255;
	ofn.lpstrInitialDir = tempFolder.c_str();	// inital directory ����
	ofn.Flags = OFN_NOCHANGEDIR;	// directory ����Ұ� flag

	if (GetOpenFileName(&ofn) == TRUE)
	{
		if (func != NULL)
		{
			wstring loadName = name;
			String::Replace(&loadName, L"\\", L"/");

			// wstring�� ���ڷ� �޴� �Լ� ��ü ȣ��
			func(loadName);
		}
	}
}

// �����ϱ� �Լ�
void Path::SaveFileDialog(wstring file, const WCHAR* filter, wstring folder, function<void(wstring)> func, HWND hwnd)
{
	WCHAR name[255];
	wcscpy_s(name, file.c_str());

	wstring tempFolder = folder;
	
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = name;
	ofn.lpstrFileTitle = L"�����ϱ�";
	ofn.nMaxFile = 255;
	ofn.lpstrInitialDir = tempFolder.c_str();
	ofn.Flags = OFN_NOCHANGEDIR;

	if (GetSaveFileName(&ofn) == TRUE)
	{
		if (func != NULL)
		{
			wstring saveName = name;
			String::Replace(&saveName, L"\\", L"/");

			func(saveName);
		}
	}


}

void Path::SaveFileDialogWithoutExtension(wstring file, const WCHAR * filter, wstring folder, function<void(wstring)> func, wstring ext)
{
	WCHAR name[255];
	wcscpy_s(name, file.c_str());

	wstring tempFolder = folder;

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = handle;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = name;
	ofn.lpstrFileTitle = L"�����ϱ�";
	ofn.nMaxFile = 255;
	ofn.lpstrInitialDir = tempFolder.c_str();
	ofn.Flags = OFN_NOCHANGEDIR;

	if (GetSaveFileName(&ofn) == TRUE)
	{
		if (func != NULL)
		{
			wstring saveName = name;
			String::Replace(&saveName, L"\\", L"/");
			wstring fileName = GetFileNameWithoutExtension(saveName);
			wstring directory = GetDirectoryName(saveName);

			saveName = directory + fileName + L"." + ext;
			func(saveName);
		}
	}


}

// �ش� path�� �ִ� ���ϵ��� files�� �־��ִ� �Լ�
void Path::GetFiles(vector<string>* files, string path, string filter, bool bFindSubFolder)
{
	vector<wstring> wFiles;
	wstring wPath = String::ToWstring(path);
	wstring wFilter = String::ToWstring(filter);

	GetFiles(&wFiles, wPath, wFilter, bFindSubFolder);

	for (wstring str : wFiles)
		files->push_back(String::ToString(str));
}

//path : ../Temp/
//filter : *.txt
void Path::GetFiles(vector<wstring>* files, wstring path, wstring filter, bool bFindSubFolder)
{

	wstring file = path + filter;

	WIN32_FIND_DATA findData;
	HANDLE handle = FindFirstFile(file.c_str(), &findData);
	if (handle != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (findData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				// findData�� directory�� �ش� ���� �ȿ� �ִ� ���ϵ��� ã��
				if (bFindSubFolder == true && findData.cFileName[0] != '.')
				{
					wstring folder = path + wstring(findData.cFileName) + L"/";
					GetFiles(files, folder, filter, bFindSubFolder);

				}
			}
			else //�ش� ��ο� �Ӽ��� ������ �ƴҶ�. �� �����϶�
			{
				wstring fileName = path + wstring(findData.cFileName);
				files->push_back(fileName);
			}
		} while (FindNextFile(handle, &findData));

		FindClose(handle);
	}
}

void Path::CreateFolder(string path)
{
	CreateFolder(String::ToWstring(path));
}

void Path::CreateFolder(wstring path)
{
	// ������ �������� ������ Directory ����
	if (ExistDirectory(path) == false)
		CreateDirectory(path.c_str(), NULL);
}

// �����ϰ� ���� �����ȿ� ������ �ִ� ��� ���α��� ����
void Path::CreateFolders(string path)
{
	CreateFolders(String::ToWstring(path));
}

// �����ϰ� ���� �����ȿ� ������ �ִ� ��� ���α��� ����
void Path::CreateFolders(wstring path)
{
	String::Replace(&path, L"\\", L"/");

	vector<wstring> folders;
	String::SplitString(&folders, path, L"/");

	wstring temp = L"";
	for (wstring folder : folders)
	{
		temp += folder + L"/";

		CreateFolder(temp);
	}
}
