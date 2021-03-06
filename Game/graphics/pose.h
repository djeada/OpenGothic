#pragma once

#include <Tempest/Matrix4x4>
#include <vector>
#include <mutex>
#include <memory>

#include "game/constants.h"
#include "animation.h"

class Skeleton;
class Serialize;
class AnimationSolver;
class Npc;

class Pose final {
  public:
    Pose()=default;

    enum Flags {
      NoFlags       = 0,
      NoTranslation = 1, // usefull for mobsi
      };

    enum StartHint {
      NoHint     = 0x0,
      Force      = 0x1,
      NoInterupt = 0x2,
      };

    void               save(Serialize& fout);
    void               load(Serialize& fin, const AnimationSolver &solver);

    void               setFlags(Flags f);
    BodyState          bodyState() const;
    void               setSkeleton(const Skeleton *sk);
    bool               startAnim(const AnimationSolver &solver, const Animation::Sequence* sq, BodyState bs,
                                 StartHint hint, uint64_t tickCount);
    bool               stopAnim(const char* name);
    void               stopItemStateAnim();
    void               interrupt();
    void               stopAllAnim();
    bool               update(AnimationSolver &solver, int comb, uint64_t tickCount);

    ZMath::float3      animMoveSpeed(uint64_t tickCount, uint64_t dt) const;
    void               processSfx(Npc &npc, uint64_t tickCount);
    void               processPfx(Npc &npc, uint64_t tickCount);
    void               processEvents(uint64_t& barrier, uint64_t now, Animation::EvCount &ev) const;
    bool               isDefParWindow(uint64_t tickCount) const;
    bool               isDefWindow(uint64_t tickCount) const;
    bool               isDefence(uint64_t tickCount) const;
    bool               isJumpBack() const;
    bool               isJumpAnim() const;
    bool               isFlyAnim() const;
    bool               isStanding() const;
    bool               isItem() const;
    bool               isPrehit(uint64_t now) const;
    bool               isIdle() const;
    bool               isInAnim(const char *sq) const;
    bool               isInAnim(const Animation::Sequence* sq) const;
    bool               hasAnim() const;
    uint64_t           animationTotalTime() const;

    auto               continueCombo(const AnimationSolver &solver,const Animation::Sequence *sq,uint64_t tickCount) -> const Animation::Sequence*;
    uint32_t           comboLength() const;

    float              translateY() const { return trY; }
    Tempest::Matrix4x4 cameraBone() const;
    auto               bone(size_t id) const -> const Tempest::Matrix4x4&;

    void               setRotation(const AnimationSolver &solver, Npc &npc, WeaponState fightMode, int dir);
    bool               setAnimItem(const AnimationSolver &solver, Npc &npc, const char* scheme);

    std::vector<Tempest::Matrix4x4> tr;

  private:
    struct Layer final {
      const Animation::Sequence* seq     = nullptr;
      uint64_t                   sAnim   = 0;
      int32_t                    comb    = 0;
      BodyState                  bs      = BS_NONE;
      };

    auto mkBaseTranslation(const Animation::Sequence *s) -> Tempest::Matrix4x4;
    void mkSkeleton(const Animation::Sequence &s);
    void mkSkeleton(const Tempest::Matrix4x4 &mt);
    void mkSkeleton(const Tempest::Matrix4x4 &mt, size_t parent);
    void zeroSkeleton();

    bool updateFrame(const Animation::Sequence &s, uint64_t barrier, uint64_t sTime, uint64_t now);

    auto getNext(AnimationSolver& solver, const Animation::Sequence* sq) -> const Animation::Sequence*;

    void addLayer(const Animation::Sequence* seq, BodyState bs, uint64_t tickCount);
    void onRemoveLayer(Layer& l);

    template<class T,class F>
    void removeIf(T& t,F f);

    std::vector<Tempest::Matrix4x4> base;

    const Skeleton*                 skeleton=nullptr;
    std::vector<Layer>              lay;
    const Animation::Sequence*      rotation=nullptr;
    const Animation::Sequence*      itemUse=nullptr;
    float                           trY=0;
    Flags                           flag=NoFlags;
    uint64_t                        lastUpdate=0;
    uint16_t                        comboLen=0;
  };
