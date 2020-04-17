/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-13 22:58:31
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-04-17 12:06:58
 */
#pragma once
#include "confinfo.h" 

#if defined(CONFINFO_PLATFORM_LINUX) || defined(CONFINFO_PLATFORM_CYGWIN) || defined(CONFINFO_PLATFORM_MACOS)
#include <dlfcn.h>
using NativeLibHandle = void*;
#elif defined(CONFINFO_PLATFORM_WIN32) 
#include <windows.h>
using NativeLibHandle = HMODULE;
#endif
#ifdef SAKURA_TARGET_PLATFORM_OSX
#include <boost/container/pmr/string.hpp>
#include <boost/container/pmr/vector.hpp>
#include <boost/container/pmr/map.hpp>
namespace Sakura::SPA
{
    namespace pmr
    {
        using boost::container::string;
        using boost::container::vector;
        using boost::container::map;
    }
}
#else
#include <memory_resource>
#endif
namespace Sakura::SPA
{      
    /**
     * @description: Provides cross-platform low-level 
     * access to shared library 
     * @author: SaeruHikari
     */
    class SharedLibrary
    {
    public:
        SharedLibrary() = default;
        /**
         * @description: Create & load a SharedLibrary on the disk.  
         * @author: SaeruHikari
         */
        SharedLibrary(const char* path)
        {load(path);}
        SharedLibrary(const std::string& path)
        {load(path);}
        SharedLibrary(const pmr::string& path)
        {load(path);}
        SharedLibrary(std::string_view path)
        {load(path);}
        ~SharedLibrary() = default;

        SharedLibrary(const SharedLibrary& rhs) = delete;
        const SharedLibrary& operator=(const SharedLibrary& rhs) = delete;

        /**
        * @description: Load a shared lib. Unload old lib if called
        * on a obj with loaded lib.
        * @param const char* path
        * @return: result of the operation
        * @author: SaeruHikari
        */
        bool load(const char* path)
        {
            if(isLoaded() && !unload())
                return false;
            return loadImpl(path);
        }
        /**
        * @overload 
        * @see load(const char* path) 
        */
        inline bool load(const std::string& path)
        {return load(path.c_str());}
        /**
        * @overload 
        * @see load(const char* path) 
        */
        inline bool load(const pmr::string& path)
        {return load(path.c_str());}
        /**
        * @overload 
        * @see load(const char* path) 
        */
        inline bool load(std::string_view path)
        {return load(path.data());}

        /**
         * @description: checks if the library is loaded. 
         * @author: SaeruHikari
         */
        bool isLoaded() const
        {return _handle != nullptr;}
        /**
         * @description: unload the shared library.
         * @author: SaeruHikari
         */
        bool unload()
        {return isLoaded() && unloadImpl();}
       
        /**
         * @description: Checks if the library has the symbol
         * specified by @a symbolName
         * @param symbolName 
         * @author: SaeruHikari
         */
        bool hasSymbol(const char* symbolName)
        {
            pmr::string error = _lastError;
            getImpl(symbolName);
            bool has = _lastError.empty();
            _lastError = error;
            return has;
        }
        /**
         * @overload 
         * @see hasSymbol(const char* symbolName) 
         */
        bool hasSymbol(const std::string& symbolName)
        {
            return hasSymbol(symbolName.c_str());
        }

        /**
         * @description: Returns the symbol specified by @a symbolName 
         * need to ensuring the symbol type.
         * @param symbolName 
         * @author: SaeruHikari
         */
        template<typename SymT>
        SymT& get(const char* symbolName)
        {return *(reinterpret_cast<SymT*>(reinterpret_cast<uintptr_t>(getImpl(symbolName))));}
        /**
        * @overload
        * @see get(const char* symbolName)
        */
        template<typename SymT>
        SymT& get(const std::string& symbolName)
        {return get<SymT>(symbolName.c_str());}
        /**
        * @overload
        * @see get(const char* symbolName)
        */
        template<typename SymT>
        SymT& get(const pmr::string& symbolName)
        {return get<SymT>(symbolName.c_str());}
        /**
        * @overload
        * @see get(const char* symbolName)
        */
        template<typename SymT>
        SymT& get(std::string_view symbolName)
        {return get<SymT>(symbolName.data());}

        /**
         * @description:  Get the address of a symbol.
         *  Returns nullptr if the library doesn't have the symbol.
         * @param symbolName
         * @return The address
         * @author: SaeruHikari
         */
        void* getRawAddress(const char* symbolName)
        { return getImpl(symbolName); }
        /**
         * @overload
         * @see getRawAddress(const char* symbolName)
         */
        void* getRawAddress(const std::string& symbolName)
        { return getRawAddress(symbolName.c_str()); }

        /**
         * @description: Checks if the last call raise an error. 
         * @author: SaeruHikari
         */
        bool hasError() const
        {return !_lastError.empty();}

        /**
         * @description: Get the last error string.
         * @author: SaeruHikari
         */
        pmr::string errorString() const
        { return _lastError; }

        /**
         * @description: Get the native handle of lib on the system.
         * @author: SaeruHikari
         */
        NativeLibHandle handle() const
        {return _handle;}
    private:
        pmr::string _lastError;
        NativeLibHandle _handle = nullptr;
        // Linux implementation
#if defined(CONFINFO_PLATFORM_LINUX) || defined(CONFINFO_PLATFORM_CYGWIN) || defined(CONFINFO_PLATFORM_MACOS)
    bool loadImpl(const char* path)
    {
        _lastError.clear();
        _handle = dlopen(path, RTLD_LAZY | RTLD_LOCAL);
        if(!_handle)
        {
            _lastError = dlerror();
            return false;
        }
        return true;
    }

    bool unloadImpl()
    {
        _lastError.clear();
        // dlclose returns 0 on success
        if(dlclose(_handle) != 0)
        {
            _lastError = dlerror();
            return false;
        }
        _handle = nullptr;
        return true;
    }

    void* getImpl(const char* symbolName)
    {
        _lastError.clear();
        dlerror();
        void* symbol = dlsym(_handle, symbolName);
        const char* error = dlerror();
        if(error)
        {
            // An error occured
            _lastError = error;
            return nullptr;
        }
        return symbol;
    }
#elif defined(CONFINFO_PLATFORM_WIN32) // Windows implementation

    // Return a string explaining the last error
    std::string getWindowsError()
    {
        DWORD lastError = GetLastError();
        TCHAR buffer[256];
        if(lastError != 0)
        {
            FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
                          nullptr,
                          lastError,
                          MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
                          buffer,
                          256-1,
                          nullptr);

            return std::string(buffer);
        }
        return std::string();
    }

    bool loadImpl(const char* path)
    {
        _lastError.clear();
        _handle = LoadLibrary(path);
        if(!_handle)
        {
            _lastError = getWindowsError();
            return false;
        }
        return true;
    }

    bool unloadImpl()
    {
        _lastError.clear();
        if(!FreeLibrary(_handle))
        {
            _lastError = getWindowsError();
            return false;
        }
        _handle = nullptr;
        return true;
    }

    void* getImpl(const char* symbolName)
    {
        _lastError.clear();
        void* addr = (void*)GetProcAddress(_handle, symbolName);
        if(!addr)
        {
            _lastError = getWindowsError();
            return nullptr;
        }
        return addr;
    }
#endif
    };

    
}