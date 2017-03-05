// ListStrErrorWin32.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include "FileEntity.h"

void TestMethod01()
{
	vector<string> messages;
	for (int i = 0; i < 256; i++) {
		errno = i;
		unique_ptr<char> messageBuffer(new char[256]);
		strerror_s(messageBuffer.get(), (size_t)256, errno);
		string item = "Number : " + to_string(i);
		item.append(" ");
		item.append(string(messageBuffer.get()));
		messages.push_back(item);
	}
	
	FileEntity f;
	f.SetFullPath("D:\\workspace\\TestMethod01.txt");
	f.WritePrepare(false);
	if (f.IsWritePrepared()) {
		f.SetFileLines(messages);
		f.WriteFile();
	}
}

void TestMethod02()
{
	vector<string> messages;
	for (int i = 0; i < 256; i++) {
		errno = i;
		LPVOID msg = NULL;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			errno,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&msg,
			0,
			NULL
		);
		if (msg == NULL) {
			continue;
		}
		wstring witem = wstring((const wchar_t *)msg);
		if (witem.at(witem.size() - 2) == '\r' && witem.at(witem.size() - 1) == '\n') {
			witem = witem.substr(0, witem.size() - 3);
		}
		
		size_t wsize = witem.length() * 2 + 1;
		size_t isize = 0;
		unique_ptr<char> mbitem(new char[wsize]);
		locale::global(locale(""));
		wcstombs_s(&isize, mbitem.get(), wsize, witem.c_str(), _TRUNCATE);
		locale::global(locale("C"));
		string item = "Number : " + to_string(i);
		item.append(" ");
		item.append(string(mbitem.get()));
		messages.push_back(item);
	}

	FileEntity f;
	f.SetFullPath("D:\\workspace\\TestMethod02.txt");
	f.WritePrepare(false);
	if (f.IsWritePrepared()) {
		f.SetFileLines(messages);
		f.WriteFile();
	}
}

int main()
{
	TestMethod01();
	TestMethod02();
    return 0;
}

