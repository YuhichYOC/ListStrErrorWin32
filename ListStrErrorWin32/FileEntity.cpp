#include "stdafx.h"

#include "FileEntity.h"

int FileEntity::CountFileSize() {
	ifile.seekg(0, istream::end);
	streamoff endPos = ifile.tellg();
	ifile.seekg(0, istream::beg);
	streamoff startPos = ifile.tellg();
	return (int)(endPos - startPos);
}

void FileEntity::FetchOnce() {
	if (readAsBinary) {
		unique_ptr<char> readBuffer(new char[fileSize]);
		ifile.read(readBuffer.get(), fileSize);
		fileContents = move(readBuffer);
	}
	else {
		if (readAsMB) {
			string readingLine;
			while (getline(ifile, readingLine)) {
				fileLines.push_back(readingLine);
			}
		}
		else {
			wstring readingLine;
			while (getline(wifile, readingLine)) {
				wfileLines.push_back(readingLine);
			}
		}
	}
}

void FileEntity::SetDirectory(string arg) {
	string newFullPath = arg + '/' + fileName;
	fullPath = newFullPath;
	directory.assign(arg);
}

string FileEntity::GetDirectory() {
	return directory;
}

void FileEntity::SetFileName(string arg) {
	string newFullPath = directory + '/' + arg;
	fullPath = newFullPath;
	fileName.assign(arg);
}

string FileEntity::GetFileName() {
	return fileName;
}

void FileEntity::SetFullPath(string arg) {
	string newDirectory = arg.substr(0, arg.find_last_of('/'));
	directory = newDirectory;
	string newFileName = arg.substr(arg.find_last_of('/') + 1);
	fileName = newFileName;
	fullPath.assign(arg);
}

string FileEntity::GetFullPath() {
	if (fullPath.length() != 0) {
		return fullPath;
	}
	else {
		return directory + '/' + fileName;
	}
}

void FileEntity::ReadPrepare(bool asBinary) {
	if (writePrepared) {
		readPrepared = false;
		return;
	}
	string readFullPath;
	if (fullPath.length() != 0) {
		readFullPath.assign(fullPath);
	}
	else {
		readFullPath.assign(directory);
		readFullPath.append("\\");
		readFullPath.append(fileName);
	}
	if (asBinary) {
		ifile = ifstream(readFullPath, ios::in | ios::binary);
		readAsBinary = true;
		readAsMB = false;
		writeAsBinary = true;
		writeAsMB = false;
	}
	else {
		ifile = ifstream(readFullPath, ios::in);
		readAsBinary = false;
		readAsMB = true;
		writeAsBinary = false;
		writeAsMB = true;
	}
	if (ifile) {
		fileSize = CountFileSize();
		readPrepared = true;
	}
	else {
		readPrepared = false;
	}
}

void FileEntity::ReadPrepareW() {
	if (writePrepared) {
		readPrepared = false;
		return;
	}
	string readFullPath;
	if (fullPath.length() != 0) {
		readFullPath.assign(fullPath);
	}
	else {
		readFullPath.assign(directory);
		readFullPath.append("\\");
		readFullPath.append(fileName);
	}
	wifile = wifstream(readFullPath, ios::in);
	readAsBinary = false;
	readAsMB = false;
	writeAsBinary = false;
	writeAsMB = false;
	if (ifile) {
		readPrepared = true;
	}
	else {
		readPrepared = false;
	}
}

bool FileEntity::IsReadPrepared() {
	return readPrepared;
}

int FileEntity::GetFileSize() {
	return fileSize;
}

void FileEntity::ReadFile() {
	readSuccess = false;

	FetchOnce();

	ifile.close();

	readSuccess = true;
}

void FileEntity::ReadFileW() {
	readSuccess = false;

	FetchOnce();

	wifile.close();

	readSuccess = true;
}

bool FileEntity::IsReadSuccess() {
	return readSuccess;
}

unique_ptr<char> FileEntity::GetFileContents() {
	unique_ptr<char> ret(new char[fileSize]);
	for (int i = 0; i < fileSize; i++) {
		ret.get()[i] = fileContents.get()[i];
	}
	return ret;
}

unique_ptr<char> FileEntity::GetFileContents(int arg1start,
	int arg2length) {
	unique_ptr<char> ret(new char[arg2length]);
	for (int i = 0; i < arg2length; i++) {
		ret.get()[i] = fileContents.get()[arg1start + i];
	}
	return ret;
}

vector<string> FileEntity::GetFileLines() {
	return fileLines;
}

vector<wstring> FileEntity::GetFileLinesW() {
	return wfileLines;
}

void FileEntity::SetFileContents(char * arg, int size) {
	fileContents.reset(arg);
	fileSize = size;
}

void FileEntity::SetFileLines(vector<string> arg) {
	fileLines = arg;
}

void FileEntity::SetFileLinesW(vector<wstring> arg) {
	wfileLines = arg;
}

void FileEntity::AddFileLines(string arg) {
	fileLines.push_back(arg);
}

void FileEntity::AddFileLinesW(wstring arg) {
	wfileLines.push_back(arg);
}

void FileEntity::WritePrepare(bool asBinary) {
	if (readPrepared) {
		writePrepared = false;
		return;
	}
	string writeFullPath;
	if (fullPath.length() != 0) {
		writeFullPath.assign(fullPath);
	}
	else {
		writeFullPath.assign(directory);
		writeFullPath.append("\\");
		writeFullPath.append(fileName);
	}
	if (asBinary) {
		ofile = ofstream(writeFullPath, ios::out | ios::binary);
		readAsBinary = true;
		readAsMB = false;
		writeAsBinary = true;
		writeAsMB = false;
	}
	else {
		ofile = ofstream(writeFullPath, ios::out);
		readAsBinary = false;
		readAsMB = true;
		writeAsBinary = false;
		writeAsMB = true;
	}
	if (ofile) {
		writePrepared = true;
	}
	else {
		writePrepared = false;
	}
}

void FileEntity::WritePrepareW() {
	if (readPrepared) {
		writePrepared = false;
		return;
	}
	string writeFullPath;
	if (fullPath.length() != 0) {
		writeFullPath.assign(fullPath);
	}
	else {
		writeFullPath.assign(directory);
		writeFullPath.append("\\");
		writeFullPath.append(fileName);
	}
	wofile = wofstream(writeFullPath, ios::out);
	readAsBinary = false;
	readAsMB = false;
	writeAsBinary = false;
	writeAsMB = false;
	if (ofile) {
		writePrepared = true;
	}
	else {
		writePrepared = false;
	}
}

void FileEntity::OverWritePrepare(bool asBinary) {
	if (readPrepared) {
		writePrepared = false;
		return;
	}
	string writeFullPath;
	if (fullPath.length() != 0) {
		writeFullPath.assign(fullPath);
	}
	else {
		writeFullPath.assign(directory);
		writeFullPath.append("\\");
		writeFullPath.append(fileName);
	}
	if (asBinary) {
		ofile = ofstream(writeFullPath, ios::out | ios::binary | ios::app);
		readAsBinary = true;
		readAsMB = false;
		writeAsBinary = true;
		writeAsMB = false;
	}
	else {
		ofile = ofstream(writeFullPath, ios::out | ios::app);
		readAsBinary = false;
		readAsMB = true;
		writeAsBinary = false;
		writeAsMB = true;
	}
	if (ofile) {
		writePrepared = true;
	}
	else {
		writePrepared = false;
	}
}

void FileEntity::OverWritePrepareW() {
	if (readPrepared) {
		writePrepared = false;
		return;
	}
	string writeFullPath;
	if (fullPath.length() != 0) {
		writeFullPath.assign(fullPath);
	}
	else {
		writeFullPath.assign(directory);
		writeFullPath.append("\\");
		writeFullPath.append(fileName);
	}
	wofile = wofstream(writeFullPath, ios::out | ios::app);
	readAsBinary = false;
	readAsMB = false;
	writeAsBinary = false;
	writeAsMB = false;
	if (ofile) {
		writePrepared = true;
	}
	else {
		writePrepared = false;
	}
}

bool FileEntity::IsWritePrepared() {
	return writePrepared;
}

void FileEntity::WriteFile() {
	writeSuccess = false;

	if (writeAsBinary) {
		ofile.write(fileContents.get(), fileSize);
	}
	else {
		int count = (int)fileLines.size();
		for (int i = 0; i < count; i++) {
			ofile.write(fileLines.at(i).c_str(), fileLines.at(i).size());
			ofile.write("\n", 1);
		}
	}
	ofile.close();

	writeSuccess = true;
}

void FileEntity::WriteFileW(string localeword) {
	writeSuccess = false;

	if (localeword.compare("none") == 0) {
		ios_base::sync_with_stdio(false);
		locale newlocale("");
		wofile.imbue(newlocale);
	}
	else if (localeword.size() != 0) {
		ios_base::sync_with_stdio(false);
		locale newlocale(localeword);
		wofile.imbue(newlocale);
	}
	int count = (int)fileLines.size();
	for (int i = 0; i < count; i++) {
		wofile.write(wfileLines.at(i).c_str(), wfileLines.at(i).size());
		wofile.write(L"\n", 1);
	}
	wofile.close();

	writeSuccess = true;
}

bool FileEntity::IsWriteSuccess() {
	return writeSuccess;
}

bool FileEntity::FindFile() {
	string path;
	if (fullPath.length() != 0) {
		path = fullPath;
	}
	else {
		path = directory + "\\" + fileName;
	}
	ifstream f(path);
	return f.good();
}

void FileEntity::DeleteExistingFile() {
	deleteSuccess = false;

	string path;
	if (fullPath.length() != 0) {
		path = fullPath;
	}
	else {
		path = directory + "\\" + fileName;
	}
	if (FindFile()) {
		if (remove(path.c_str())) {
			deleteSuccess = true;
		}
	}
}

bool FileEntity::IsDeleteSuccess() {
	return deleteSuccess;
}

void FileEntity::CopyFile(string fileName) {
	ifstream from(GetFullPath());
	if (!from) {
		return;
	}
	ofstream to(fileName.c_str());
	if (!to) {
		return;
	}
	to << from.rdbuf() << flush;
	if (!from) {
		return;
	}
	if (!to) {
		return;
	}
	copySuccess = true;
}

bool FileEntity::IsCopySuccess() {
	return copySuccess;
}

FileEntity::FileEntity() {
	directory = string();
	fileName = string();
	fullPath = string();
	fileSize = 0;
	readAsBinary = false;
	readAsMB = false;
	readPrepared = false;
	writeAsBinary = false;
	writeAsMB = false;
	writePrepared = false;
	readSuccess = false;
	writeSuccess = false;
	deleteSuccess = false;
	copySuccess = false;
	disposed = false;
}

FileEntity::~FileEntity() {
}