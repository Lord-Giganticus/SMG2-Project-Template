//This is modified compared to the release on Shibbo's repo.
//For some reason CW didn't like when I included this file in more than one file.
//Creating a CPP for this fixed it. CW is weird!

#include "System/Resource/FileLoader.h"
#include "Singleton.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRDvdRipper.h"
#include "Util.h"
#include "Util/FileUtil.h"
#include "Util/MemoryUtil.h"

namespace Syati
{
    JKRArchive* loadArchive(const char *pArchivePath)
    {
        if (MR::isFileExist(pArchivePath, false)) {
        void* rawData = MR::loadToMainRAM(pArchivePath, 0, MR::getStationedHeapNapa(), 1);
        OSReport("Archive %s loaded\n", pArchivePath);

        FileLoader* ldr = SingletonHolder<FileLoader>::sInstance;
        JKRArchive* arch = ldr->createAndAddArchive(rawData, 0, pArchivePath);

        return arch;
        }
        else
        return 0;
    }

    void* loadResourceFromArchive(const char *pArchivePath, const char *pFile)
    {
        FileLoader* ldr = SingletonHolder<FileLoader>::sInstance;
        JKRArchive* archive = ldr->mArchiveHolder->getArchive(pArchivePath);

        if (archive)
        {
            void* data = archive->getResource(pFile);

            if (data)
            {
                OSReport("File %s loaded\n", pFile);
                return data;
            }
            else
                OSReport("File %s isn't exist!\n", pFile);
        }

        return 0;
    }
};
