#include "execDir.hpp"
#include <cstdio>
#include <vector>
#include <algorithm>

#if _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

std::string getExecutableDirectory()
{
	static std::string dir;

	if (dir.empty())
	{
		std::vector<char> buffer;

		HMODULE hCurModule = GetModuleHandleA(NULL);

		DWORD size = 8;
		do
		{
			size *= 2;
			buffer.resize(size, 0);
			size = GetModuleFileNameA(hCurModule, buffer.data(), size);
		} while (size == buffer.size());

		if (size == 0) return std::string{};

		std::replace(buffer.begin(), buffer.end(), '\\', '/');
		for (; size > 1; size--)
			if (buffer[size] == '/') break;

		dir = std::string(buffer.data(), size);
	}

	return dir;
}

#elif __APPLE__
#ifndef __OBJC__
#error This file must be compiled as an Objective-C++ source file!
#endif
#import <Foundation/Foundation.h>

// Taken directly from the app template on Mac
std::string getExecutableDirectory()
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    
    std::string rpath;
    NSBundle* bundle = [NSBundle mainBundle];
    
    if (bundle == nil) {
#ifdef DEBUG
        NSLog(@"bundle is nil... thus no executable dir can be found.");
#endif
    } else {
        NSString* path = [bundle executablePath];
        rpath = [path UTF8String];
    }
    
    [pool drain];
    
    return rpath;
}

std::vector<std::string> getAllFilesInDir(std::string dir)
{
    NSDirectoryEnumerator* enumerator = [[NSFileManager defaultManager] enumeratorAtPath:[NSString stringWithUTF8String:dir.c_str()]];
    
    std::vector<std::string> list;
    NSString* filename;
    while ((filename = [enumerator nextObject]))
        list.emplace_back(filename.UTF8String);
    
    return list;
}

#elif __linux__
extern "C"
{
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
}

std::string getExecutableDirectory()
{
    static std::string dir;

    if (dir.empty())
    {
        std::vector<char> buffer;

        ssize_t size = 8;
        do
        {
            size *= 2;
            buffer.resize(size, 0);
            size = readlink("/proc/self/exe", buffer.data(), size);
        } while (size == buffer.size());

        if (size < 0) return std::string{};

        for (; size > 1; size--)
            if (buffer[size] == '/') break;

        dir = std::string(buffer.data(), size);
    }

    return dir;
}

std::vector<std::string> getAllFilesInDir(std::string dir)
{
    DIR* cur = opendir(dir.data());
    if (!cur) return std::vector<std::string>{};
    
    struct dirent* curEnt;
    std::vector<std::string> list;
    
    while ((curEnt = readdir(cur)))
    {
        std::string name(curEnt->d_name);
        
        auto pos = name.find_last_of("\\/");
        if (pos == std::string::npos)
            pos = decltype(pos)(0);
        else pos++;
        
        list.emplace_back(name.substr(pos));
    }
        
    closedir(cur);
    
    return list;
}
#endif
