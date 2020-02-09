/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-09 20:06:42
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-09 22:14:36
 */
#include "../path.h"

namespace Sakura::path
{
    spmr_string clean(sstring_view s)
    {
        if(s.empty()) 
            return spmr_string(1, '.');
        spmr_string r(s.data(), s.size());
        size_t n = s.size();

        bool rooted = s[0] == '/';
        size_t p = rooted ? 1 : 0;
        size_t dotdot = rooted ? 1 : 0;

         for (size_t i = p; i < n;) {
            if (s[i] == '/' || (s[i] == '.' && (i+1 == n || s[i+1] == '/'))) {
                // empty or . element
                ++i;
            } else if (s[i] == '.' && s[i+1] == '.' && (i+2 == n || s[i+2] == '/')) {
            // .. element: remove to last /
                i += 2;

            if (p > dotdot) {
                // backtrack
                for (--p; p > dotdot && r[p] != '/'; --p);

            } else if (!rooted) {
                // cannot backtrack, but not rooted, so append .. element
                if (p > 0) r[p++] = '/';
                r[p++] = '.';
                r[p++] = '.';
                dotdot = p;
            }

            } else {
                // real path element, add slash if needed
                if ((rooted && p != 1) || (!rooted && p != 0)) {
                    r[p++] = '/';
                }

                // copy element until the next /
                for (; i < n && s[i] != '/'; i++) {
                    r[p++] = s[i];
                }
            }
        }

        if (p == 0) 
            return spmr_string(1, '.');
        return r.substr(0, p);
    }    

}