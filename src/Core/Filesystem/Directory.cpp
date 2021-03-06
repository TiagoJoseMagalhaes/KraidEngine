#include "Directory.h"

#include <Core/Windows.h>
#include <Core/Utils/Log.h>
#include "Filesystem.h"

namespace Kraid
{

    Directory::Directory(const wchar_t* path)
    {
        this->directory_handle = CreateFileW(path,
            FILE_LIST_DIRECTORY | GENERIC_READ | GENERIC_WRITE ,
            FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE,
            NULL,
            OPEN_EXISTING,
            FILE_FLAG_BACKUP_SEMANTICS,
            NULL);
        if(this->directory_handle == INVALID_HANDLE_VALUE)
        {
            PRINT_WINERROR();
        }
    }

    Directory::~Directory()
    {
        CloseHandle(this->directory_handle);
        this->directory_handle = nullptr;
        this->changes_obtained_once = false;
    }

    std::vector<std::wstring> Directory::GetChangedFiles()
    {
        #define CHANGED_FILE_ENTRY_COUNT 1024
        uint32 entries_read;
        FILE_NOTIFY_INFORMATION* changed_entries = (FILE_NOTIFY_INFORMATION*)malloc(CHANGED_FILE_ENTRY_COUNT * sizeof(FILE_NOTIFY_INFORMATION));
        if (changed_entries == nullptr) return {};
        //NOTE(Tiago): there is a weird behavior where a write causes two change notifications to be launched for the same file, this seems to be related to metadata. The issue is that in teh first time that a change is detected, the second notification is not received and as such we have to have a way to handle that special corner case.
        bool result = ReadDirectoryChangesW(this->directory_handle, (void*)changed_entries, CHANGED_FILE_ENTRY_COUNT, FALSE, FILE_NOTIFY_CHANGE_LAST_WRITE, (DWORD*)&entries_read, NULL, NULL);
        if(this->changes_obtained_once)
        {
            result = ReadDirectoryChangesW(this->directory_handle, (void*)changed_entries, CHANGED_FILE_ENTRY_COUNT, FALSE, FILE_NOTIFY_CHANGE_LAST_WRITE, (DWORD*)&entries_read, NULL, NULL);
        }
        this->changes_obtained_once = true;
        if(result == false)
        {
            PRINT_WINERROR();
            return {};
        }

        std::vector<std::wstring> changed_files;
        FILE_NOTIFY_INFORMATION* iterator = changed_entries;
        do
        {
            if (iterator->FileNameLength <= 0) continue;

            wchar_t* filename = (wchar_t*)malloc(iterator->FileNameLength + sizeof(wchar_t));
            if (filename != nullptr)
            {
                memset(filename, 0, iterator->FileNameLength + sizeof(wchar_t));
                memcpy(filename, iterator->FileName, iterator->FileNameLength);
                changed_files.emplace_back(filename);
                free(filename);
            }

            iterator += iterator->NextEntryOffset;
        } while (iterator->NextEntryOffset != 0);
        free(changed_entries);

        return changed_files;
    }

    DirectoryWatcher::DirectoryWatcher(const wchar_t* directory)
    {
        this->watched_dir = {directory};
        this->wait_event_handle = FindFirstChangeNotificationW(directory, false, FILE_NOTIFY_CHANGE_LAST_WRITE);
        if(this->wait_event_handle == INVALID_HANDLE_VALUE)
        {
            PRINT_WINERROR();
            return;
        }

        this->watch_thread = Thread([this](void* args) -> DWORD
        {
            while(this->watched_dir.directory_handle != nullptr)
            {
                uint32 wait_status = WaitForMultipleObjects(1, &this->wait_event_handle, TRUE, INFINITE);
                if(wait_status == WAIT_OBJECT_0)
                {
                    auto changed_files = this->watched_dir.GetChangedFiles();
                    for(auto& file: changed_files)
                    {
                        this->file_callback_mutex.Lock();
                        auto callbacks = this->file_change_callbacks[file];
                        for(auto& callback: callbacks)
                        {
                            if(callback)
                            {
                                callback();
                            }
                            else
                            {
                                LWARNING(L"Attempt to invoke null callback for file change");
                            }
                        }
                        this->file_callback_mutex.Unlock();
                    }
                }
            }
            return 0;
        }); 
    }

    bool DirectoryWatcher::operator==(const DirectoryWatcher &other)
    {
        return this->wait_event_handle == other.wait_event_handle;
    }

    void DirectoryWatcher::RegisterFileChangeCallback(const std::wstring& filename, const std::function<void(void)>& callback)
    {
        if(callback == nullptr)
        {
            LWARNING(L"Attempt to register NULL callback, aborted");
            return;
        }

        this->file_callback_mutex.Lock();
        this->file_change_callbacks[filename].push_back(callback);
        this->file_callback_mutex.Unlock();
    }

    //TODO(Tiago):associative containers in C++ are dogs ass, remember to implement your own hash 
    DirectoryWatcher& DirectoryWatcher::GetDirectoryWatcher(const std::wstring& directory)
    {
        std::wstring directory_path = Kraid::GetAbsoluteFilepath(directory);
        if(!DirectoryWatcher::directory_watcher_instances.contains(directory_path))
        {
            //NOTE(Tiago):keeps the first check fast, that way if the instance is already there we can quickly get it, otherwise we lock to make sure that no other thread is already inserting the instance.
            watcher_instance_mutex.Lock();
            if(!DirectoryWatcher::directory_watcher_instances.contains(directory_path))
            {
                DirectoryWatcher::directory_watcher_instances.emplace(directory_path, directory_path.c_str());
            }
            watcher_instance_mutex.Unlock();
        }
        return DirectoryWatcher::directory_watcher_instances[directory_path];
    }

}
