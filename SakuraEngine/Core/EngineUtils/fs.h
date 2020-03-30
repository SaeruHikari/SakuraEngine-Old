#pragma once
#include "../CoreMinimal/SDefination.h"
#include "../Containers/SString.h"
#include "sstream.h"
using namespace Sakura;

namespace Sakura::fs
{
	bool exists(const char* path);

	bool isdir(const char* path);

	// Returns modify time of the file.
	int64 mtime(const char* path);

	// Returns size of the file.
	int64 fsize(const char* path);

	// p: -p switch
	bool mkdir(const char* path, bool p = false);

	// rf: -rf switch
	bool remove(const char* path, bool rf = false);

	bool rename(const char* from, const char* to);

	template<typename S>
	sinline bool exists(S&& path)
	{
		return fs::exists(path);
	}

	template<typename S>
	sinline bool isdir(S&& path) 
	{
		return fs::isdir(path.c_str());
	}

	template<typename S>
	sinline int64 mtime(S&& path) 
	{
		return fs::mtime(path.c_str());
	}

	template<typename S>
	sinline int64 fsize(S&& path) 
	{
		return fs::fsize(path.c_str());
	}

	template<typename S>
	sinline bool mkdir(S&& path, bool p = false) 
	{
		return fs::mkdir(path.c_str(), p);
	}

	template<typename S>
	sinline bool remove(S&& path, bool rf = false) 
	{
		return fs::remove(path.c_str(), rf);
	}

	template<typename S>
	sinline bool rename(S&& from, S&& to) 
	{
		return fs::rename(from.c_str(), to);
	}


	// open mode:
	//   'r': read         open if exists
	//   'a': append       created if not exists
	//   'w': write        created if not exists, truncated if exists
	//   'm': modify       like 'w', but not truncated if exists
	class file
	{
	public:
		static const int seek_beg = 0;
		static const int seek_cur = 1;
		static const int seek_end = 2;

		file() : _p(0) { }

		file(const char* path, char mode) : _p(0) {
			this->open(path, mode);
		}

		file(file&& f) {
			_p = f._p;
			f._p = 0;
		}

		~file();

		file(const file& x) = delete;
		void operator=(const file& x) = delete;
		void operator=(file&& x) = delete;

		operator bool() const;

		const Sakura::sstring& path() const;

		int64 size() const
		{
			return fs::fsize(this->path());
		}

		bool exists() const 
		{
			return fs::exists(this->path());
		}

		bool open(const char* path, char mode);

		void close();

		void seek(int64 off, int whence = seek_beg);

		size_t read(void* buf, size_t size);

		Sakura::sstring read(size_t size);

		size_t write(const void* buf, size_t size);

		size_t write(const char* s) {
			return this->write(s, strlen(s));
		}

		template<typename S>
		size_t write(const S& s) {
			return this->write(s.data(), s.size());
		}

		size_t write(char c) {
			return this->write(&c, 1);
		}

	private:
		void* _p;
	};

	// open mode:
	//   'a': append       created if not exists
	//   'w': write        created if not exists, truncated if exists
	class fstream {
	public:
		explicit fstream(size_t cap = 8192)
			: _s(cap) {
		}

		explicit fstream(const char* path, char mode, size_t cap = 8192)
			: _s(cap), _f(path, mode == 'w' ? 'w' : 'a') {
		}

		fstream(fstream&& fs)
			: _s(std::move(fs._s)), _f(std::move(fs._f)) 
		{
		}

		~fstream()
		{
			this->close();
		}

		operator bool() const {
			return _f;
		}

		bool open(const char* path, char mode)
		{
			return _f.open(path, mode == 'w' ? 'w' : 'a');
		}

		void flush() {
			if (!_s.empty()) 
			{
				_f.write(_s.data(), _s.size());
				_s.clear();
			}
		}

		void close() {
			if (_f)
			{
				this->flush();
				_f.close();
			}
		}

		// n <= cap - szie         ->   append
		// cap - size < n <= cap   ->   flush and append
		// n > cap                 ->   flush and write
		fstream& append(const void* s, size_t n) 
		{
			if (_s.capacity() < _s.size() + n) this->flush();
			n <= _s.capacity() ? ((void)_s.append(s, n)) : ((void)_f.write(s, n));
			return *this;
		}

		fstream& operator<<(const char* s) {
			return this->append(s, strlen(s));
		}

		fstream& operator<<(const std::pmr::string& s) {
			return this->append(s.data(), s.size());
		}

		fstream& operator<<(const std::string& s) {
			return this->append(s.data(), s.size());
		}

		fstream& operator<<(std::string_view s) {
			return this->append(s.data(), s.size());
		}

		fstream& operator<<(const sstream& s) {
			return this->append(s.data(), s.size());
		}

		template<typename T>
		fstream& operator<<(const T& v) {
			if (_s.capacity() < _s.size() + 24) this->flush();
			_s << v;
			return *this;
		}

	private:
		sstream _s;
		fs::file _f;

		DISALLOW_COPY_AND_ASSIGN(fstream);
	};
}