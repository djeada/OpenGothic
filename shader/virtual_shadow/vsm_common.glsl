#ifndef VSM_COMMON_GLSL
#define VSM_COMMON_GLSL

// const int VSM_MAX_PAGES     = 1024;
const int VSM_PAGE_SIZE     = 128;
const int VSM_PAGE_TBL_SIZE = 64;
const int VSM_CLIPMAP_SIZE  = VSM_PAGE_SIZE * VSM_PAGE_TBL_SIZE;

uint packVsmPageInfo(ivec3 at) {
  return (at.x & 0xFF) | ((at.y & 0xFF) << 8) | ((at.z & 0xFF) << 16);
  }

ivec3 unpackVsmPageInfo(uint p) {
  ivec3 r;
  r.x = int(p      ) & 0xFF;
  r.y = int(p >>  8) & 0xFF;
  r.z = int(p >> 16) & 0xFF;
  return r;
  }

uint packVsmPageId(ivec2 at) {
  return (at.x + at.y*32);
  }

ivec2 unpackVsmPageId(uint pageId) {
  return ivec2(pageId%32, pageId/32);
  }

bool vsmPageClip(ivec2 fragCoord, const uint page) {
  ivec4 pg = ivec4(unpackVsmPageId(page) * VSM_PAGE_SIZE, 0, 0);
  pg.zw = pg.xy + ivec2(VSM_PAGE_SIZE);

  ivec2 f = fragCoord;
  if(pg.x <= f.x && f.x<pg.z &&
     pg.y <= f.y && f.y<pg.w)
    return true;
  return false;
  }

#endif
