#pragma once
#ifdef GUID_ANDROID
#include <thread>
#include <jni.h>
#endif
#include <spdlog/fmt/fmt.h>
#include <array>

namespace sakura
{
    // Class to represent a GUID/UUID. Each instance acts as a wrapper around a
    // 16 byte value that can be passed around by value. It also supports
    // conversion to string (via the stream operator <<) and conversion from a
    // string via constructor.
    class RuntimeCoreAPI Guid
    {
    public:
        explicit Guid(const std::array<unsigned char, 16> &bytes);
        explicit Guid(std::array<unsigned char, 16> &&bytes);

        explicit Guid(std::string_view fromString);
        Guid();
        
        Guid(const Guid &other) = default;
        Guid &operator=(const Guid &other) = default;
        Guid(Guid &&other) = default;
        Guid &operator=(Guid &&other) = default;

        bool operator==(const Guid &other) const;
        bool operator!=(const Guid &other) const;

        std::string str() const;
        operator std::string() const;
        const std::array<unsigned char, 16>& bytes() const;
        void swap(Guid &other);
        bool isValid() const;

    private:
        void zeroify();

        // actual data
        std::array<unsigned char, 16> _bytes;

        // make the << operator a friend so it can access _bytes
        friend std::ostream &operator<<(std::ostream &s, const Guid &guid);
        friend bool operator<(const Guid &lhs, const Guid &rhs);
    };

    RuntimeCoreAPI Guid newGuid();

    #ifdef GUID_ANDROID
    struct AndroidGuidInfo
    {
        static AndroidGuidInfo fromJniEnv(JNIEnv *env);

        JNIEnv *env;
        jclass uuidClass;
        jmethodID newGuidMethod;
        jmethodID mostSignificantBitsMethod;
        jmethodID leastSignificantBitsMethod;
        std::thread::id initThreadId;
    };

    extern AndroidGuidInfo androidInfo;

    void initJni(JNIEnv *env);

    // overloading for multi-threaded calls
    Guid newGuid(JNIEnv *env);
    #endif

    namespace details
    {
        template <typename...> struct hash;

        template<typename T> 
        struct hash<T> : public std::hash<T>
        {
            using std::hash<T>::hash;
        };


        template <typename T, typename... Rest>
        struct hash<T, Rest...>
        {
            inline std::size_t operator()(const T& v, const Rest&... rest) {
                std::size_t seed = hash<Rest...>{}(rest...);
                seed ^= hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                return seed;
            }
        };
    }

    inline sakura::Guid new_guid() noexcept
    {
        return newGuid();
    }
}

namespace std
{
	// Template specialization for std::swap<Guid>() --
	// See guid.cpp for the function definition
	template <>
	void swap(sakura::Guid &guid0, sakura::Guid &guid1) noexcept;

	// Specialization for std::hash<Guid> -- this implementation
	// uses std::hash<std::string> on the stringification of the guid
	// to calculate the hash
	template <>
	struct hash<sakura::Guid>
	{
		std::size_t operator()(sakura::Guid const &guid) const
		{
			const uint64_t* p = reinterpret_cast<const uint64_t*>(guid.bytes().data());
			return sakura::details::hash<uint64_t, uint64_t>{}(p[0], p[1]);
		}
	};
}

namespace fmt
{
	template<> struct formatter<sakura::Guid> : fmt::formatter<std::string>
	{
		template<typename FormatContext>
		auto format(sakura::Guid _guid, FormatContext& ctx)
		{
            std::string result = _guid.str().c_str();
            return formatter<std::string>::format(result, ctx);
		}
	};
}
