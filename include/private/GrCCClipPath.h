/*
 * Copyright 2018 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrCCClipPath_DEFINED
#define GrCCClipPath_DEFINED

#include "GrTextureProxy.h"
#include "SkPath.h"

struct GrCCPerFlushResourceSpecs;
class GrCCAtlas;
class GrCCPerFlushResources;
class GrOnFlushResourceProvider;
class GrProxyProvider;

/**
 * These are keyed by SkPath generation ID, and store which device-space paths are accessed and
 * where by clip FPs in a given opList. A single GrCCClipPath can be referenced by multiple FPs. At
 * flush time their coverage count masks are packed into atlas(es) alongside normal DrawPathOps.
 */
class GrCCClipPath {
public:
    GrCCClipPath() = default;
    GrCCClipPath(const GrCCClipPath&) = delete;

    ~GrCCClipPath() {
        // Ensure no clip FPs exist with a dangling pointer back into this class.
        SkASSERT(!fAtlasLazyProxy || fAtlasLazyProxy->isUnique_debugOnly());
        // Ensure no lazy proxy callbacks exist with a dangling pointer back into this class.
        SkASSERT(fHasAtlasTransform);
    }

    bool isInitialized() const { return fAtlasLazyProxy != nullptr; }
    void init(GrProxyProvider* proxyProvider,
              const SkPath& deviceSpacePath, const SkIRect& accessRect,
              int rtWidth, int rtHeight);

    void addAccess(const SkIRect& accessRect) {
        SkASSERT(this->isInitialized());
        fAccessRect.join(accessRect);
    }
    GrTextureProxy* atlasLazyProxy() const {
        SkASSERT(this->isInitialized());
        return fAtlasLazyProxy.get();
    }
    const SkPath& deviceSpacePath() const {
        SkASSERT(this->isInitialized());
        return fDeviceSpacePath;
    }
    const SkIRect& pathDevIBounds() const {
        SkASSERT(this->isInitialized());
        return fPathDevIBounds;
    }

    void accountForOwnPath(GrCCPerFlushResourceSpecs*) const;
    void renderPathInAtlas(GrCCPerFlushResources*, GrOnFlushResourceProvider*);

    const SkVector& atlasScale() const { SkASSERT(fHasAtlasTransform); return fAtlasScale; }
    const SkVector& atlasTranslate() const { SkASSERT(fHasAtlasTransform); return fAtlasTranslate; }

private:
    sk_sp<GrTextureProxy> fAtlasLazyProxy;
    SkPath fDeviceSpacePath;
    SkIRect fPathDevIBounds;
    SkIRect fAccessRect;

    const GrCCAtlas* fAtlas = nullptr;
    int16_t fAtlasOffsetX;
    int16_t fAtlasOffsetY;
    SkDEBUGCODE(bool fHasAtlas = false);

    SkVector fAtlasScale;
    SkVector fAtlasTranslate;
    SkDEBUGCODE(bool fHasAtlasTransform = false);
};

#endif
