#pragma  once

#include "syati.h"

class JKRArchive;

namespace Syati
{
JKRArchive* loadArchive(const char *pArchivePath);
void* loadResourceFromArchive(const char *pArchivePath, const char *pFile);

};