#include "pch.h"
#include "langdetect-uap.h"

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::UI::Popups;
using namespace concurrency;
using namespace std;

UAP_DIR* uap_opendir(StorageFolder^ folder) {
	UAP_DIR* udir = new UAP_DIR();
	udir->file_ptr = 0;

	// ask WinRT to get a vector of all of the files in this folder.
	// we store this vector in the DIR object. each file will have a corresponding dirent struct.
	// it stores the StorageFile object for the file, which will allow us to read it. 
	auto f = create_task(folder->GetFilesAsync()).then([=](IVectorView<StorageFile^>^ filesInFolder) {
		udir->files = new uap_dirent*[filesInFolder->Size];
		udir->numberOfFiles = filesInFolder->Size;
		for (auto it = filesInFolder->First(); it->HasCurrent; it->MoveNext()) {
			StorageFile^ storage_file = it->Current;
			struct uap_dirent* file = new uap_dirent();		
			file->d_name = storage_file;
			file->d_type = 0x8000;
			udir->files[udir->file_ptr++] = file;
		}

		udir->file_ptr = 0;
	});

	// we have to wait for this task to complete. file IO will be handled synchronously, but this will not block the UI thread
	f.wait();
	return udir;
}

// free up any memory we allocated
void uap_closedir(UAP_DIR* dir) {
	for (int i = 0; i < dir->numberOfFiles; i++) {
		delete dir->files[i]->d_name;
		delete dir->files[i];
	}
	delete dir->files;
	delete dir;
}

// return the directory entry pointed at by the file pointer, if there are still files left to be processed
struct uap_dirent* uap_readdir(UAP_DIR* dir) {
	if (dir->file_ptr < dir->numberOfFiles) {
		return dir->files[dir->file_ptr++];
	}
	else {
		return NULL;
	}
}

void uap_chdir(StorageFolder^ dir) {
	// this intentionally does nothing. 
}

// creates a FILE struct which will be used by the language processor
// it contains the StorageFile object which is necessary for reading
UAP_FILE* uap_fopen(StorageFile^ name, char* flags) {
	UAP_FILE* file = new UAP_FILE();
	file->uapfile = name;
	file->line_ptr = 0;
	return file;
}

// places the next line of the file into the buffer
int uap_fgets(char* buffer, int bufsize, UAP_FILE* fp) {
	StorageFile^ uap_file = fp->uapfile;

	if (fp->lines == nullptr) {
		// we haven't read this file yet, so read all lines, place them into a vector, and then store it in the FILE object for later use
		auto t = create_task(FileIO::ReadLinesAsync(uap_file, Windows::Storage::Streams::UnicodeEncoding::Utf8)).then([=](IVector<String^>^ lines) {
			fp->lines = lines;
		});
		t.wait();
	}
	
	if (fp->line_ptr < fp->lines->Size) {
		// if we aren't at the end of the file, keep reading
		String^ current_line = fp->lines->GetAt(fp->line_ptr++);
		wstr_to_utf8(current_line, buffer);
		return 1;
	}
	
	else { // EOF
		return 0;
	}
}

// the FILE object itself is the only unmanaged object that we have to worry about.
void uap_fclose(UAP_FILE* fp) {
	delete fp;
}

void uap_display_dialog(char* text) {
	String^ str = utf8_to_wstr(text);
	MessageDialog^ msg = ref new MessageDialog(str);
	msg->ShowAsync();

}