/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-13 22:00:21
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-13 22:29:48
 */
#ifndef _WIN32
#include "../fs.h"
#include "../../CoreMinimal/SDefination.h"
#include <assert.h>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace Sakura;

namespace Sakura::fs
{
    bool exists(const char* path)
    {
        struct stat attr;
        return ::lstat(path, &attr) == 0;
    }

    bool isdir(const char* path) 
    {
        struct stat attr;
        if (::lstat(path, &attr) != 0) return false;
        return S_ISDIR(attr.st_mode);
    }

    int64 mtime(const char* path) 
    {
        struct stat attr;
        if (::lstat(path, &attr) != 0) return -1;
        return attr.st_mtime;
    }

    int64 fsize(const char* path) 
    {
        struct stat attr;
        if (::lstat(path, &attr) != 0) return -1;
        return attr.st_size;
    }

    // p = false  ->  mkdir
    // p = true   ->  mkdir -p
    bool mkdir(const char* path, bool p)
    {
        if (!p) return ::mkdir(path, 0755) == 0;

        const char* s = strrchr((char*)path, '/');
        if (s == 0) return ::mkdir(path, 0755) == 0;

        pmr::string parent(path, s - path);
        
        if (fs::exists(parent.c_str())) {
            return ::mkdir(path, 0755) == 0;
        } else {
            return fs::mkdir(parent.c_str(), true) && ::mkdir(path, 0755) == 0;
        }
    }

    // rf = false  ->  rm or rmdir
    // rf = true   ->  rm -rf
    bool remove(const char* path, bool rf) 
    {
        if (!fs::exists(path)) return true;

        if (!rf) {
            if (fs::isdir(path)) 
                return rmdir(path) == 0;
            return unlink(path) == 0;
        } else {
            pmr::string cmd;
            cmd.resize(strlen(path) + 9);
            cmd.append("rm -rf \"").append(path);
            cmd += '"';
            return system(cmd.c_str()) != -1;
        }
    }

    bool rename(const char* from, const char* to)
    {
        return ::rename(from, to) == 0;
    }

    #define nullfd -1

namespace xx 
    {
        int open(const char* path, char mode) 
        {
            if (mode == 'r') {
                return ::open(path, O_RDONLY);
            } else if (mode == 'a') {
                return ::open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
            } else if (mode == 'w') {
                return ::open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            } else if (mode == 'm') {
                return ::open(path, O_WRONLY | O_CREAT, 0644);
            } else {
                return nullfd;
            }
        }
    } // xx

    struct fctx 
    {
        int fd;
        pmr::string path;
    };

    Sakura::fs::file::~file() 
    {
        if (!_p) return;
        this->close();
        delete (fctx*) _p;
        _p = 0;
    }

    Sakura::fs::file::operator bool() const 
    {
        fctx* p = (fctx*) _p;
        return p && p->fd != nullfd;
    }

    const pmr::string& file::path() const 
    {
        fctx* p = (fctx*) _p;
        return p->path;
    }

    bool file::open(const char* path, char mode) {
        this->close();
        fctx* p = (fctx*) _p;
        if (!p) _p = p = new fctx;
        p->path = path;
        return (p->fd = xx::open(path, mode)) != nullfd;
    }

    void file::close() {
        fctx* p = (fctx*) _p;
        if (!p || p->fd == nullfd) return;
        ::close(p->fd);
        p->fd = nullfd;
    }

    void file::seek(int64 off, int whence) {
        static int seekfrom[3] = { SEEK_SET, SEEK_CUR, SEEK_END };
        whence = seekfrom[whence];
        ::lseek(((fctx*)_p)->fd, off, whence);
    }

    size_t file::read(void* s, size_t n) {
        int64 r = ::read(((fctx*)_p)->fd, s, n);
        return r < 0 ? 0 : (size_t)r;
    }

    pmr::string file::read(size_t n) {
        pmr::string s;
        s.resize(n + 1);
        this->read((void*)s.data(), n);
        return s;
    }

    size_t file::write(const void* s, size_t n) {
        int64 r = ::write(((fctx*)_p)->fd, s, n);
        return r < 0 ? 0 : (size_t)r;
    }

    #undef nullfd
}
#endif//_WIN32