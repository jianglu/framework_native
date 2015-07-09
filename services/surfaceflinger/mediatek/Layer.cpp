#include "Layer.h"

#include <cutils/xlog.h>
#include <ui/GraphicBufferExtra.h>
#include <ui/gralloc_extra.h>

namespace android {


status_t Layer::adjustTexCoord(const sp<const DisplayDevice>& hw, Mesh::VertexArray<vec2> texCoords) const {

    if (!hasS3DBuffer()) {
        return NO_ERROR;
    }

    switch (hw->mS3DPhase) {
        case DisplayDevice::eComposingS3DSBSLeft:     // use bottom half
            {
                texCoords[0].x = texCoords[0].x * 0.5;
                texCoords[1].x = texCoords[1].x * 0.5;
                texCoords[2].x = texCoords[2].x * 0.5;
                texCoords[3].x = texCoords[3].x * 0.5;
                break;
            }
        case DisplayDevice::eComposingS3DSBSRight:      // use top half
            {
                texCoords[0].x = 0.5 + texCoords[0].x * 0.5;
                texCoords[1].x = 0.5 + texCoords[1].x * 0.5;
                texCoords[2].x = 0.5 + texCoords[2].x * 0.5;
                texCoords[3].x = 0.5 + texCoords[3].x * 0.5;
                break;
            }
        case DisplayDevice::eComposingS3DTABTop:     // use bottom half
            {
                texCoords[0].y = texCoords[0].y * 0.5;
                texCoords[1].y = texCoords[1].y * 0.5;
                texCoords[2].y = texCoords[2].y * 0.5;
                texCoords[3].y = texCoords[3].y * 0.5;
                break;
            }
        case DisplayDevice::eComposingS3DTABBottom:      // use top half
            {
                texCoords[0].y = 0.5 + texCoords[0].y * 0.5;
                texCoords[1].y = 0.5 + texCoords[1].y * 0.5;
                texCoords[2].y = 0.5 + texCoords[2].y * 0.5;
                texCoords[3].y = 0.5 + texCoords[3].y * 0.5;
                break;
            }
        default:
            break;
    }
    return NO_ERROR;
}

bool Layer::hasS3DBuffer() const {

    if (mActiveBuffer == NULL) {
        return false;
    }

    ANativeWindowBuffer* nativeBuffer = mActiveBuffer->getNativeBuffer();
    buffer_handle_t handle = nativeBuffer->handle;
    gralloc_extra_ion_sf_info_t extInfo;

    int err = 0;
    err |= gralloc_extra_query(handle, GRALLOC_EXTRA_GET_IOCTL_ION_SF_INFO, &extInfo);

    int bit_S3D = (extInfo.status & GRALLOC_EXTRA_MASK_S3D);
         
    return (bit_S3D == GRALLOC_EXTRA_BIT_S3D_SBS) || (bit_S3D == GRALLOC_EXTRA_BIT_S3D_TAB);
}

// ---------------------------------------------------------------------------
}; // namespace android
