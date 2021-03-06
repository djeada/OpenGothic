#include "visualfx.h"

#include "world/world.h"

VisualFx::VisualFx(Daedalus::GEngineClasses::CFx_Base &&src):fx(std::move(src)) {
  }

const Daedalus::GEngineClasses::C_ParticleFXEmitKey& VisualFx::key(SpellFxKey type) const {
  return keys[int(type)];
  }

Daedalus::GEngineClasses::C_ParticleFXEmitKey& VisualFx::key(SpellFxKey type) {
  return keys[int(type)];
  }

void VisualFx::emitSound(World& wrld, const Tempest::Vec3& p, SpellFxKey type) const {
  auto& k   = key(type);
  wrld.emitSoundEffect(k.sfxID.c_str(),p.x,p.y,p.z,0,true);
  }
