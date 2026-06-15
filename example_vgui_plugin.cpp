#include <stdio.h>

#include "engine/iserverplugin.h"
#include "ienginevgui.h"
#include "interface.h"
#include "tier0/platform.h"
#include "tier1/tier1.h"
#include "tier2/tier2.h"
#include "tier3/tier3.h"
#include "vgui/ISurface.h"
#include "vgui/IVGui.h"
#include "vgui_controls/AnimationController.h"
#include "vgui_controls/Panel.h"

#include "example_frame.h"

/* Must be last include file in .cpp file */
#include "tier0/memdbgon.h"

IEngineVGui *engine_vgui = NULL;

class CVGuiAnimationDriver : public vgui::Panel {
  typedef vgui::Panel BaseClass;

public:
  CVGuiAnimationDriver(vgui::VPANEL parent)
      : BaseClass(NULL, "VGuiAnimationDriver") {
    SetParent(parent);
    SetVisible(false);
    SetPaintEnabled(false);
    SetPaintBackgroundEnabled(false);
    vgui::ivgui()->AddTickSignal(GetVPanel(), 0);
  }

  void OnTick() override {
    vgui::GetAnimationController()->UpdateAnimations(Plat_FloatTime());
  }
};

static CVGuiAnimationDriver *g_pAnimDriver = NULL;

class CExamplePlugin : public IServerPluginCallbacks {
public:
  bool Load(CreateInterfaceFn interfaceFactory,
            CreateInterfaceFn gameServerFactory) override;
  void Unload(void) override;
  void Pause(void) override {}
  void UnPause(void) override {}
  const char *GetPluginDescription(void) override { return "Example VGUI Plugin"; }
  void LevelInit(char const *pMapName) override {}
  void ServerActivate(edict_t *pEdictList, int edictCount, int clientMax) override {}
  void GameFrame(bool simulating) override {}
  void LevelShutdown(void) override {}
  void ClientActive(edict_t *pEntity) override {}
  void ClientDisconnect(edict_t *pEntity) override {}
  void ClientPutInServer(edict_t *pEntity, char const *playername) override {}
  void SetCommandClient(int index) override {}
  void ClientSettingsChanged(edict_t *pEdict) override {}
  PLUGIN_RESULT ClientConnect(bool *bAllowConnect, edict_t *pEntity,
                              const char *pszName, const char *pszAddress,
                              char *reject, int maxrejectlen) override {
    return PLUGIN_CONTINUE;
  }
  PLUGIN_RESULT ClientCommand(edict_t *pEntity, const CCommand &args) override {
    return PLUGIN_CONTINUE;
  }
  PLUGIN_RESULT NetworkIDValidated(const char *pszUserName,
                                   const char *pszNetworkID) override {
    return PLUGIN_CONTINUE;
  }
  void OnQueryCvarValueFinished(QueryCvarCookie_t iCookie,
                                edict_t *pPlayerEntity,
                                EQueryCvarValueStatus eStatus,
                                const char *pCvarName,
                                const char *pCvarValue) override {}
  void OnEdictAllocated(edict_t *edict) override {}
  void OnEdictFreed(const edict_t *edict) override {}
};

CExamplePlugin g_ExamplePlugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CExamplePlugin, IServerPluginCallbacks,
                                  INTERFACEVERSION_ISERVERPLUGINCALLBACKS,
                                  g_ExamplePlugin);

bool CExamplePlugin::Load(CreateInterfaceFn interfaceFactory,
                          CreateInterfaceFn gameServerFactory) {
  ConnectTier1Libraries(&interfaceFactory, 1);
  ConnectTier2Libraries(&interfaceFactory, 1);
  ConnectTier3Libraries(&interfaceFactory, 1);

  engine_vgui = (IEngineVGui *)interfaceFactory(VENGINE_VGUI_VERSION, NULL);

  if (!engine_vgui || !vgui::surface())
    return false;

  ConVar_Register(0);

  // Parent the frame to the engine's embedded VGUI panel
  vgui::VPANEL root = vgui::surface()->GetEmbeddedPanel();
  CExampleFrame::Install(root);

  g_pAnimDriver = new CVGuiAnimationDriver(root);

  return true;
}

void CExamplePlugin::Unload(void) {
  delete g_pAnimDriver;
  g_pAnimDriver = NULL;

  ConVar_Unregister();
  DisconnectTier3Libraries();
  DisconnectTier2Libraries();
  DisconnectTier1Libraries();
}
