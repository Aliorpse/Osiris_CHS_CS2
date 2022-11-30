#pragma once

#include "../JsonForward.h"
#include <Interfaces/ClientInterfaces.h>
#include <Interfaces/OtherInterfaces.h>
#include <Platform/IsPlatform.h>
#include <Config/ResetConfigurator.h>
#include "Visuals/ColorCorrection.h"
#include "Visuals/SkyboxChanger.h"

namespace csgo { enum class FrameStage; }
class GameEvent;
struct ImDrawList;
class EngineInterfaces;

class Visuals {
public:
    Visuals(const Memory& memory, OtherInterfaces interfaces, ClientInterfaces clientInterfaces, EngineInterfaces engineInterfaces, const helpers::PatternFinder& clientPatternFinder, const helpers::PatternFinder& enginePatternFinder)
        : memory{ memory }, interfaces{ interfaces }, clientInterfaces{ clientInterfaces }, engineInterfaces{ engineInterfaces }, skyboxChanger{ createSkyboxChanger(interfaces.getCvar(), enginePatternFinder) }
    {
#if IS_WIN32()
        disablePostProcessingPtr = reinterpret_cast<bool*>(clientPatternFinder("\x83\xEC\x4C\x80\x3D").add(5).deref().get());
#elif IS_LINUX()
        disablePostProcessingPtr = reinterpret_cast<bool*>(clientPatternFinder("\x80\x3D?????\x89\xB5").add(2).relativeToAbsolute().get());
#endif
    }

    bool isThirdpersonOn() noexcept;
    bool isZoomOn() noexcept;
    bool isSmokeWireframe() noexcept;
    bool isDeagleSpinnerOn() noexcept;
    bool shouldRemoveFog() noexcept;
    bool shouldRemoveScopeOverlay() noexcept;
    bool shouldRemoveSmoke() noexcept;
    float viewModelFov() noexcept;
    float fov() noexcept;
    float farZ() noexcept;

    void performColorCorrection() noexcept;
    void inverseRagdollGravity() noexcept;
    void colorWorld() noexcept;
    void modifySmoke(csgo::FrameStage stage) noexcept;
    void thirdperson() noexcept;
    void removeVisualRecoil(csgo::FrameStage stage) noexcept;
    void removeBlur(csgo::FrameStage stage) noexcept;
    void updateBrightness() noexcept;
    void removeGrass(csgo::FrameStage stage) noexcept;
    void remove3dSky() noexcept;
    void removeShadows() noexcept;
    void applyZoom(csgo::FrameStage stage) noexcept;
    void applyScreenEffects() noexcept;
    void hitEffect(const GameEvent* event = nullptr) noexcept;
    void hitMarker(const GameEvent* event, ImDrawList* drawList = nullptr) noexcept;
    void disablePostProcessing(csgo::FrameStage stage) noexcept;
    void reduceFlashEffect() noexcept;
    bool removeHands(const char* modelName) noexcept;
    bool removeSleeves(const char* modelName) noexcept;
    bool removeWeapons(const char* modelName) noexcept;
    void skybox(csgo::FrameStage stage) noexcept;
    void bulletTracer(const GameEvent& event) noexcept;
    void drawMolotovHull(ImDrawList* drawList) noexcept;

    void updateEventListeners(bool forceRemove = false) noexcept;
    void updateInput() noexcept;

    // GUI
    void menuBarItem() noexcept;
    void tabItem() noexcept;
    void drawGUI(bool contentOnly) noexcept;

    // Config
    json toJson() noexcept;
    void fromJson(const json& j) noexcept;
    void resetConfig() noexcept;

    template <typename Configurator>
    void configure(Configurator& configurator)
    {
        configurator("Color correction", colorCorrection);
        configurator("Disable post-processing", disablePostProcessing_).def(false);
        configurator("Inverse ragdoll gravity", inverseRagdollGravity_).def(false);
        configurator("No fog", noFog).def(false);
        configurator("No 3d sky", no3dSky).def(false);
        configurator("No aim punch", noAimPunch).def(false);
        configurator("No view punch", noViewPunch).def(false);
        configurator("No hands", noHands).def(false);
        configurator("No sleeves", noSleeves).def(false);
        configurator("No weapons", noWeapons).def(false);
        configurator("No smoke", noSmoke).def(false);
        configurator("No blur", noBlur).def(false);
        configurator("No scope overlay", noScopeOverlay).def(false);
        configurator("No grass", noGrass).def(false);
        configurator("No shadows", noShadows).def(false);
        configurator("Wireframe smoke", wireframeSmoke).def(false);
        configurator("Zoom", zoom).def(false);
    }

private:
    const Memory& memory;
    OtherInterfaces interfaces;
    ClientInterfaces clientInterfaces;
    EngineInterfaces engineInterfaces;
    bool* disablePostProcessingPtr;
    ColorCorrection colorCorrection;
    SkyboxChanger skyboxChanger;

    bool disablePostProcessing_;
    bool inverseRagdollGravity_;
    bool noFog;
    bool no3dSky;
    bool noAimPunch;
    bool noViewPunch;
    bool noHands;
    bool noSleeves;
    bool noWeapons;
    bool noSmoke;
    bool noBlur;
    bool noScopeOverlay;
    bool noGrass;
    bool noShadows;
    bool wireframeSmoke;
    bool zoom;
};
