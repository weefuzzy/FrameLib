
#include "FrameLib_Subframe.h"
#include "FrameLib_MaxClass.h"

extern "C" int C74_EXPORT main(void)
{
    FrameLib_MaxClass_Expand<FrameLib_Subframe>::makeClass(CLASS_BOX, "fl.subframe~");
}
