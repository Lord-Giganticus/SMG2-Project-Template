#pragma once

#include "syati.h"

namespace Syati {
	JKRArchive* loadArchive(const char *pArchivePath);
    void* loadResourceFromArchive(const char *pArchivePath, const char *pFile);
};
