/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Fabien Caylus
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

/*
 * Sementic versionning 2.0.0 implementation in C++11
 * Project hosted at: https://github.com/Tranqyll/Version
 */

#ifndef VERSION_H
#define VERSION_H

#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <ostream>

/**
 * @brief Simple header-only class that implements a Version object as specified in Semantic Versionning 2.0.0 (http://semver.org/).
 * You can create a version object from a string, which is usually formatted as MAJOR.MINOR.PATCH[-prereleaseinfo][+metadata].
 * @note This class requires c++11.
 */
class Version {

public:
    /**
     * @brief Default constructor
     */
    Version() {}
    /**
     * @brief Copy constructor
     */
    Version(const Version& v)
        : _major(v.major()), _minor(v.minor()), _patch(v.patch()),
          _prerelease(v.prerelease()), _metadata(v.metadata())
    {}

    /**
     * @brief Construct a Version object from a string (call parse() function)
     * @param version string to parse
     */
    Version(const std::string& version)
    {
        parse(version);
    }

    /**
     * @brief Parse a version string
     * @param version
     * @return true if version is a valid version string
     */
    bool parse(const std::string& version)
    {
        // Checks if the string contains only [A-Za-z0-9-.+]
        if(!std::all_of(version.begin(), version.end(), [](unsigned char c) {
                        return (bool)std::isalnum(c)
                                || c == static_cast<unsigned char>('+')
                                || c == static_cast<unsigned char>('-')
                                || c == static_cast<unsigned char>('.');}))
        {
            return false;
        }

        // Version must be X.Y.Z[-prerelease][+metadata], so it must contains at least 2 dots
        if(std::count(version.begin(), version.end(), '.') < 2)
            return false;

        // Get MAJOR and MINOR numbers
        const std::string::size_type firstDotId = version.find_first_of('.');
        const int major = std::stoi(version.substr(0, firstDotId));
        const std::string::size_type secondDotId = version.find_first_of('.', firstDotId+1);
        const int minor = std::stoi(version.substr(firstDotId+1, secondDotId-firstDotId-1));

        // After the PATCH number, there can be a '-' (for prerelase) or a '+' (for metadata) or nothing
        const std::string::size_type hyphenId = version.find_first_of('-');
        const std::string::size_type plusId = version.find_first_of('+');

        int patch = 0;
        std::string prerelease;
        std::string metadata;

        if(plusId == std::string::npos && hyphenId == std::string::npos)
        {
            // There is no prerelease and metadata
            patch = std::stoi(version.substr(secondDotId+1));
        }
        else if(plusId == std::string::npos)
        {
            // There is only a prerelease
            patch = std::stoi(version.substr(secondDotId+1, hyphenId-secondDotId-1));
            prerelease = version.substr(hyphenId+1);
        }
        else if(hyphenId == std::string::npos || plusId < hyphenId)
        {
            // There is only metadata
            patch = std::stoi(version.substr(secondDotId+1, plusId-secondDotId-1));
            metadata = version.substr(plusId+1);
        }
        else if(hyphenId < plusId)
        {
            // There are both
            patch = std::stoi(version.substr(secondDotId+1, hyphenId-secondDotId-1));
            prerelease = version.substr(hyphenId+1, plusId-hyphenId-1);
            metadata = version.substr(plusId+1);
        }

        if(major < 0 || minor < 0 || patch < 0)
            return false;

        _major = major;
        _minor = minor;
        _patch = patch;
        _prerelease = prerelease;
        _metadata = metadata;

        return true;
    }

    /**
     * @brief Check if this version is compatible with 'other'.
     * Compatible means the MAJOR numbers are equals and the minor version is higher (or equal) than the 'minVersion' minor version.
     * @param minVersion
     * @return true if versions are compatible
     */
    bool compatible(const Version& minVersion)
    {
        return _major == minVersion.major() && _minor >= minVersion.minor();
    }
    /**
     * @brief Overloaded function.
     * @param str
     * @return
     */
    bool compatible(const std::string& str) { return compatible(Version(str)); }

    //
    // Getters
    //

    /**
     * @brief major
     * @return Major version
     */
    int major() const { return _major; }
    /**
     * @brief minor
     * @return Minor version
     */
    int minor() const { return _minor; }
    /**
     * @brief patch
     * @return Patch version
     */
    int patch() const { return _patch; }
    /**
     * @brief prerelease
     * @return Prerelease string (empty string if not specified)
     */
    std::string prerelease() const { return _prerelease; }
    /**
     * @brief prereleaseIdentifiers
     * @return A list of all prerelease identifiers (empty list if no prerelease is specified)
     */
    std::vector<std::string> prereleaseIdentifiers() const { return split(_prerelease); }
    /**
     * @brief metadata
     * @return Metadata string (empty string if not specified)
     */
    std::string metadata() const { return _metadata; }

    /**
     * @brief toString
     * @return Convert the version object to string (useful for printing)
     */
    std::string toString() const
    {
        std::string str = std::to_string(_major) + "." + std::to_string(_minor) + "." + std::to_string(_patch);
        if(!_prerelease.empty())
            str += "-" + _prerelease;
        if(!_metadata.empty())
            str += "+" + _metadata;
        return str;
    }

    //
    // Setters
    //

    /**
     * @brief setMajor
     * @param major
     */
    void setMajor(const int& major)
    {
        if(major >= 0)
            _major = major;
    }
    /**
     * @brief setMinor
     * @param minor
     */
    void setMinor(const int& minor)
    {
        if(minor >= 0)
            _minor = minor;
    }
    /**
     * @brief setPatch
     * @param patch
     */
    void setPatch(const int& patch)
    {
        if(patch >= 0)
            _patch = patch;
    }
    /**
     * @brief setPrerelease
     * @param str
     */
    void setPrerelease(const std::string& str)
    {
        _prerelease = str;
    }
    /**
     * @brief setMetadata
     * @param str
     */
    void setMetadata(const std::string& str)
    {
        _metadata = str;
    }


private:

    int _major = 0;
    int _minor = 0;
    int _patch = 0;
    std::string _prerelease;
    std::string _metadata;

    // Split string by '.'
    std::vector<std::string> split(const std::string &in) const
    {
        char sep = '.';
        std::string::size_type b = 0;
        std::vector<std::string> result;

        while ((b = in.find_first_not_of(sep, b)) != std::string::npos)
        {
            std::string::size_type e = in.find_first_of(sep, b);
            result.push_back(in.substr(b, e-b));
            b = e;
        }
        return result;
    }
};

//
// Operators
//

inline bool operator==(const Version& v1, const Version& v2)
{
    return v1.major() == v2.major()
            && v1.minor() == v2.minor()
            && v1.patch() == v2.patch()
            && v1.prerelease() == v2.prerelease();
}
inline bool operator!=(const Version& v1, const Version& v2) { return !(v1 == v2); }

inline bool operator<(const Version& v1, const Version& v2)
{
    if(v1.major() == v2.major())
    {
        if(v1.minor() == v2.minor())
        {
            if(v1.patch() == v2.patch())
            {
                if(!v1.prerelease().empty() && v2.prerelease().empty())
                    return true;
                else if(v1.prerelease().empty() && !v2.prerelease().empty())
                    return false;
                else if(v1.prerelease().empty() && v2.prerelease().empty()) // Versions Equal
                    return false;
                else
                {
                    // Both prerelease strings are not empty
                    std::vector<std::string> pr1 = v1.prereleaseIdentifiers();
                    std::vector<std::string> pr2 = v2.prereleaseIdentifiers();
					// Add parenthesis around std::min in case windows.h is included and NOMINMAX not defined
                    const size_t maxI = (std::min)(pr1.size(), pr2.size());
                    for(size_t i=0; i < maxI; ++i)
                    {
                        // Checks if the prerelease is only digits
                        const bool allDigit1 = std::all_of(pr1[i].begin(), pr1[i].end(), ::isdigit);
                        const bool allDigit2 = std::all_of(pr2[i].begin(), pr2[i].end(), ::isdigit);

                        if(allDigit1 && allDigit2)
                            return std::stoi(pr1[i]) < std::stoi(pr2[i]);
                        else if(allDigit1 && !allDigit2)
                            return true;
                        else if(!allDigit1 && allDigit2)
                            return false;

                        // Here, both prerelease are litterals
                        const int comp = pr1[i].compare(pr2[i]);
                        if(comp < 0)
                            return true;
                        else if(comp > 0)
                            return false;
                    }

                    return maxI == pr2.size();
                }
            }
            return v1.patch() < v2.patch();
        }
        return v1.minor() < v2.minor();
    }
    return v1.major() < v2.major();
}
inline bool operator>(const Version& v1, const Version& v2) { return v2 < v1; }
inline bool operator<=(const Version& v1, const Version& v2) { return !(v1 > v2); }
inline bool operator>=(const Version& v1, const Version& v2) { return !(v1 < v2); }

inline std::ostream& operator<<(std::ostream& os, const Version& v)
{
    os << v.toString();
    return os;
}


#endif // VERSION_H
