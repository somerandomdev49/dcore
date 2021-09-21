#include <string>
#include <vector>
#include <unordered_map>

class ConfigData
{
    std::unordered_map<std::string, std::vector<std::string>> Data_;
public:
    std::string GetSingle(const std::string &name, const std::string &defaultValue = {});
    std::vector<std::string> GetMultiple(const std::string &name, const std::vector<std::string> &defaultValue = {});
};

class Component {};

class Composer
{
public:
    void AddComponent(Component *component);
};

class Controller
{
    virtual void OnCompose(Composer *selfComposer);
    virtual void OnBeginPlay();
    virtual void OnTick(float deltaTime);
    virtual void OnEndPlay();
};

class Asset {};

class Asset_AudioClip : public Asset {};

class Asset_Mesh : public Asset {};
class Component_Mesh : public Component { public: Component_Mesh(Asset_Mesh*); };

class Asset_Skeleton : public Asset {};
class Component_Skeleton : public Component { public: Component_Skeleton(Asset_Skeleton*); };

class Asset_SkeletalMesh : public Asset {};
class Component_SkeletalMesh : public Component { public: Component_SkeletalMesh(Asset_SkeletalMesh*); };

class AssetManager
{
public:
    Asset_SkeletalMesh *LoadSkeletalMesh(const std::string &name);
    Asset_AudioClip *LoadAudioClip(const std::string &name);
};

class AudioEmitter
{
public:
    void Play(Asset_AudioClip *clip);
};

class Animator
{
public:
    Animator(Component_SkeletalMesh *skeletalMeshComponent);
    void Play(const std::string &name);
};

template<typename T>
class AutoPointer
{
public:
    AutoPointer() : Pointer_(nullptr) {}
    AutoPointer(T *ptr) : Pointer_(ptr) {}
    ~AutoPointer() { delete ptr; }
    T &operator*() { return *Pointer_; }
    const T &operator*() const { return *Pointer_; }
    T *operator->() { return Pointer_; }
    T *Raw() { return Pointer_; }
private:
    T *Pointer_ = nullptr;
};

class ActorController : public Controller
{
    Asset_SkeletalMesh *SkeletalMeshAsset_;
    Component_SkeletalMesh *SkeletalMeshComponent_;
    AutoPointer<Animator> Animator_;
public:
    ActorController(AssetManager &assets, ConfigData &params)
    {
        SkeletalMeshAsset_ = assets.LoadSkeletalMesh(params.GetSingle("ActorController.Rig", "_DefaultActorMesh"));
    }

    virtual void OnCompose(Composer *selfComposer)
    {
        selfComposer->AddComponent((SkeletalMeshComponent_ = new Component_SkeletalMesh(SkeletalMeshAsset_)));
        Animator_ = AutoPointer<Animator>(new Animator(SkeletalMeshComponent_));
    }

    virtual void OnBeginPlay() { }
    virtual void OnTick(float deltaTime) { }

    virtual void OnEndPlay()
    {

    }

    Animator *GetAnimator() { return Animator_.Raw(); };

};

class StaticNpcController : public ActorController
{
    std::vector<Asset_AudioClip*> clips;
public:
    StaticNpcController(AssetManager &assets, ConfigData &params)
        : ActorController(assets, params)
    {
        for(const auto &p : params.GetMultiple("StaticNpcController.Lines"))
            clips.push_back(assets.LoadAudioClip(p));
    }

    virtual void OnCompose(Composer *selfComposer)
    {
        ActorController::OnCompose(selfComposer);
    }
};
