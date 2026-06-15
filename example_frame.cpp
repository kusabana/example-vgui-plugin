#include "example_frame.h"

#include "convar.h"
#include "tier1/strtools.h"

#include <vgui/ISurface.h>
#include <vgui/IScheme.h>
#include <vgui_controls/Button.h>
#include <vgui_controls/Label.h>

/* Must be last include file in .cpp file */
#include "tier0/memdbgon.h"

CExampleFrame *g_pExampleFrame = NULL;

CExampleFrame::CExampleFrame(vgui::VPANEL parent)
    : BaseClass(NULL, "ExampleFrame") {
  SetParent(parent);

  SetProportional(false);

  SetTitle("Example VGUI Plugin", true);
  SetSizeable(true);
  SetMoveable(true);
  SetCloseButtonVisible(true);

  SetScheme(vgui::scheme()->LoadSchemeFromFile("resource/SourceScheme.res",
                                               "SourceScheme"));

  m_pLabel = new vgui::Label(this, "HelloLabel", "Hello from a client plugin!");

  m_pButton = new vgui::Button(this, "PrintButton", "Print to console");
  m_pButton->SetCommand("print_hello");

  int sw = 0, sh = 0;
  vgui::surface()->GetScreenSize(sw, sh);
  const int w = 320, h = 140;
  SetBounds((sw - w) / 2, (sh - h) / 2, w, h);

  SetVisible(false);
  InvalidateLayout(true, true);
}

void CExampleFrame::PerformLayout() {
  BaseClass::PerformLayout();

  int x, y, w, h;
  GetClientArea(x, y, w, h);

  m_pLabel->SetBounds(x + 12, y + 8, w - 24, 24);
  m_pButton->SetBounds(x + 12, y + 40, 160, 24);
}

void CExampleFrame::Install(vgui::VPANEL parent) {
  if (g_pExampleFrame)
    return;

  g_pExampleFrame = new CExampleFrame(parent);
  Assert(g_pExampleFrame);
}

void CExampleFrame::OnCommand(const char *command) {
  if (command && !V_stricmp(command, "print_hello")) {
    Msg("button clicked!\n");
    return;
  }

  BaseClass::OnCommand(command);
}

// ---------------------------------------------------------------------------
// Console command to toggle the window
// ---------------------------------------------------------------------------
static void ToggleExampleFrame() {
  if (!g_pExampleFrame)
    return;

  bool show = !g_pExampleFrame->IsVisible();
  g_pExampleFrame->SetVisible(show);
  if (show) {
    g_pExampleFrame->MoveToFront();
    g_pExampleFrame->RequestFocus();
    g_pExampleFrame->Activate();
  }
}

static ConCommand example_vgui("example_vgui", ToggleExampleFrame,
                               "Show/hide the example VGUI window.",
                               FCVAR_DONTRECORD);
