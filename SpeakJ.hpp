// SpeakJ.hpp --- Japanese voice synthesis by katahiromz
// This file is public domain software (PDS).
//////////////////////////////////////////////////////////////////////////////
#ifndef SPEAKJ_HPP_
#define SPEAKJ_HPP_     1   /* Version 1 */

#if defined(_MSC_VER) && (_MSC_VER > 1000)
    #pragma once
#endif

//////////////////////////////////////////////////////////////////////////////

#if (defined(SPEAKJ_USE_WIN32) + defined(SPEAKJ_USE_OPENAL) == 0)
    #ifdef _WIN32
        #define SPEAKJ_USE_WIN32
    #else
        #define SPEAKJ_USE_OPENAL
    #endif
#endif

//////////////////////////////////////////////////////////////////////////////

#ifdef SPEAKJ_USE_WIN32
    #ifndef _INC_WINDOWS
        #define NOMINMAX
        #include <windows.h>
    #endif
    #include <tchar.h>
#endif

#ifdef SPEAKJ_USE_OPENAL
    #include "alwrap.hpp"
#endif

//#include <iostream>
#include <vector>
#include <cstring>
#include <cassert>

//////////////////////////////////////////////////////////////////////////////

#ifdef SPEAKJ_USE_WIN32
    #ifndef __GNUC__
        #pragma comment(lib, "winmm.lib")
    #endif
#endif

//////////////////////////////////////////////////////////////////////////////
// SJ_TEXT

#ifndef SJ_TEXT
    #ifdef UNICODE
        #define SJ_TEXT(text)   L##text
    #else
        #define SJ_TEXT(text)   text
    #endif
#endif

//////////////////////////////////////////////////////////////////////////////
// SpeakJ basic types

#ifndef SJ_Char
    #ifdef UNICODE
        typedef wchar_t         SJ_Char;
    #else
        typedef char            SJ_Char;
    #endif
    #define SJ_Char             SJ_Char
#endif

#ifndef SJ_String
    #include <string>
    #ifdef UNICODE
        typedef std::wstring    SJ_String;
    #else
        typedef std::string     SJ_String;
    #endif
    #define SJ_String           SJ_String
#endif

#ifndef SJ_Thread
    #include <thread>
    typedef std::thread         SJ_Thread;
    #define SJ_Thread           SJ_Thread
#endif

#ifndef SJ_Mutex
    #include <mutex>
    typedef std::mutex          SJ_Mutex;
    #define SJ_Mutex            SJ_Mutex
#endif

typedef unsigned int            SJ_UInt;

//////////////////////////////////////////////////////////////////////////////
// SpeakJ

class SpeakJ
{
public:
    struct PhonemeEntry
    {
        const SJ_Char *text;
        const SJ_Char *phoneme;
    };
    struct LexicalEntry
    {
        const SJ_Char *from;
        const SJ_Char *to;
    };
    struct PlayItem
    {
        SJ_String   text;
        SJ_String   phoneme;
    };

    SpeakJ() : m_playing(0), m_count(0), m_bits(0)
#ifdef _WIN32
    , m_hResInst(NULL)
#endif
    {
#ifdef SPEAKJ_USE_OPENAL
        GetAlWrap()->Load("OpenAL32.dll");
#endif
#if defined(_WIN32) || defined(SPEAKJ_USE_WIN32)
        SJ_Char szPath[MAX_PATH];
        ::GetModuleFileName(NULL, szPath, MAX_PATH);
        SJ_Char *pch = _tcsrchr(szPath, SJ_TEXT('\\'));
        *pch = 0;
        SetPath(szPath);
#else
        SetPath();
#endif
    }

    SpeakJ(const SJ_Char *pszDir)
        : m_playing(0), m_count(0), m_bits(0)
#ifdef _WIN32
    , m_hResInst(NULL)
#endif
    {
#ifdef SPEAKJ_USE_OPENAL
        GetAlWrap()->Load("OpenAL32.dll");
#endif
        assert(pszDir);
        SetPath(pszDir);
    }

#ifdef _WIN32
    SpeakJ(HINSTANCE hResourceInst)
        : m_playing(0), m_count(0), m_bits(0), m_hResInst(hResourceInst)
    {
        assert(hResourceInst);
    }
#endif

    SJ_UInt
    PlayText(const SJ_String& text, bool sync = true, bool exclusive = true)
    {
        if (sync)
            exclusive = true;
        if (exclusive)
            Mutex().lock();
        ::InterlockedIncrement(&m_playing);

        SJ_String strText = LexicalReplace(text);
        const SJ_Char *pch = strText.c_str();

        m_vecPlayItems.clear();
        while (*pch)
        {
            const PhonemeEntry *pEntry = FindEntry(pch);
            if (pEntry)
            {
                PlayItem item;
                item.text = pEntry->text;
                item.phoneme = pEntry->phoneme;
                m_vecPlayItems.push_back(item);
                pch += int(_tcslen(pEntry->text));
            }
            else
            {
                ++pch;  // skip if pEntry == NULL
            }
        }

        const SJ_UInt id = m_count;
        SJ_Thread   the_thread(ThreadProc, this, exclusive, id);
        if (sync)
        {
            the_thread.join();
        }
        else
        {
            the_thread.detach();
        }

        m_count = (m_count + 1) % (sizeof(SJ_UInt) * 8);
        return id;
    }

    void Stop(SJ_UInt id)
    {
        m_bits |= (1 << id);
    }

    void StopAll()
    {
        m_playing = 0;
        m_bits = ~0;
#ifdef SPEAKJ_USE_WIN32
        ::PlaySound(NULL, NULL, SND_PURGE);
#endif
#ifdef SPEAKJ_USE_OPENAL
        GetAlWrap()->...
#endif
    }

    bool IsPlaying() const
    {
        return m_playing > 0;
    }

protected:
    LONG                            m_playing;
    SJ_UInt                         m_count;
    SJ_UInt                         m_bits;
#ifdef _WIN32
    HMODULE                         m_hResInst;
#endif
    SJ_String                       m_strPath;
    std::vector<PlayItem>           m_vecPlayItems;

    static SJ_Mutex& Mutex()
    {
        static SJ_Mutex *s_pMutex = NULL;
        if (s_pMutex == NULL)
        {
            s_pMutex = new SJ_Mutex;
        }
        return *s_pMutex;
    }

    static inline void ThreadProc(SpeakJ *this_, bool exclusive, SJ_UInt id)
    {
        HMODULE hResInst = this_->m_hResInst;
        SJ_String strPath = this_->m_strPath;
        std::vector<PlayItem> vecPlayItems = this_->m_vecPlayItems;

        for (size_t i = 0; i < vecPlayItems.size(); ++i)
        {
            if ((this_->m_bits >> id) & 1)
                break;

            const PlayItem& item = vecPlayItems[i];
#ifdef SPEAKJ_USE_WIN32
            #if _DEBUG
                OutputDebugString(item.text.c_str());
                OutputDebugString(TEXT(", "));
                OutputDebugString(item.phoneme.c_str());
                OutputDebugString(TEXT("\r\n"));
            #endif
            DWORD dwFlags = SND_NODEFAULT | SND_SYNC;
            if (hResInst)
            {
                dwFlags |= SND_RESOURCE;
                ::PlaySound(item.phoneme.c_str(), hResInst, dwFlags);
            }
            else
            {
                
                SJ_String strWavePath(strPath);
                strWavePath += item.phoneme;
                strWavePath += SJ_TEXT(".wav");

                dwFlags |= SND_FILENAME;
                ::PlaySound(strWavePath.c_str(), NULL, dwFlags);
            }
#else
        ...
#endif
        }

        if (exclusive)
            Mutex().unlock();
        if (this_->m_playing > 0)
        {
            if (::InterlockedDecrement(&this_->m_playing) == 0)
            {
                this_->m_bits = 0;
            }
        }
    }

    const PhonemeEntry *FindEntry(const TCHAR *pch) const
    {
        static const PhonemeEntry s_Entries[] =
        {
#undef DEFINE_ENTRY
#define DEFINE_ENTRY(text,phoneme)  { SJ_TEXT(text), SJ_TEXT(phoneme) },
#ifdef _WIN32
    #include "phonemes-sjis.h"   // Shift_JIS
#else
    #include "phonemes-utf8.h"   // UTF-8
#endif
        };
        const PhonemeEntry *ret = NULL;
        int max_len = 0;
        const size_t count = sizeof(s_Entries) / sizeof(s_Entries[0]);
        for (size_t i = 0; i < count; ++i)
        {
            const PhonemeEntry *pEntry = &s_Entries[i];
            int len = int(_tcslen(pEntry->text));
            if (len < max_len)
                continue;

            if (memcmp(pEntry->text, pch, len * sizeof(TCHAR)) != 0)
                continue;

            ret = pEntry;
            max_len = len;
        }
        return ret;
    }

    void SetPath(const SJ_Char *pszDir = SJ_TEXT("."))
    {
        if (pszDir == NULL || *pszDir == 0)
            m_strPath = SJ_TEXT(".");
        else
            m_strPath = pszDir;

        // add backslash
#ifdef _WIN32
        SJ_Char ch = m_strPath[m_strPath.size() - 1];
        if (ch != SJ_TEXT('\\') && ch != SJ_TEXT('/'))
        {
            m_strPath += SJ_TEXT('\\');
        }
#else
        if (m_strPath.back() != SJ_TEXT('/'))
        {
            m_strPath += SJ_TEXT('/');
        }
#endif
    }

    SJ_String LexicalReplace(const SJ_String& text) const
    {
        static LexicalEntry s_Entries[] =
        {
#undef DEFINE_ENTRY
#define DEFINE_ENTRY(from,to)  { SJ_TEXT(from), SJ_TEXT(to) },
#ifdef _WIN32
    #include "lexical-sjis.h"   // Shift_JIS
#else
    #include "lexical-utf8.h"   // UTF-8
#endif
        };
        const size_t count = sizeof(s_Entries) / sizeof(s_Entries[0]);
        SJ_String strText(text);
        for (size_t i = 0; i < count; ++i)
        {
            while (ReplaceText(strText, s_Entries[i].from, s_Entries[i].to))
            {
                ;
            }
        }
        return strText;
    }

    bool ReplaceText(SJ_String& strText,
                     const SJ_String& from, const SJ_String& to) const
    {
        size_t i = 0;
        bool flag = false;
        for (;;)
        {
            i = strText.find(from, i);
            if (i == SJ_String::npos)
                break;
            flag = true;
            strText.replace(i, from.size(), to);
            i += to.size();
        }
        return flag;
    }
}; // class SpeakJ

//////////////////////////////////////////////////////////////////////////////

#endif  // ndef SPEAKJ_HPP_
