#pragma once

#include "AreaObj.h"

class WarpArea : public AreaObj {
public:
    WarpArea(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void movement();
    virtual const char* getManagerName() const;

    s32 hasEnteredArea;
    s32 AlexSus;
    s32 WarpID;
    s32 mode;
    TVec3f pos;
};
