#pragma once

#include <vgui_controls/Frame.h>

namespace vgui {
class Label;
class Button;
} // namespace vgui

class CExampleFrame : public vgui::Frame {
public:
  DECLARE_CLASS_SIMPLE(CExampleFrame, vgui::Frame);

  CExampleFrame(vgui::VPANEL parent);

  static void Install(vgui::VPANEL parent);

  void PerformLayout() override;
  void OnCommand(const char *command) override;
  void OnTick() override;

private:
  vgui::Label *m_pLabel;
  vgui::Button *m_pButton;
};

extern CExampleFrame *g_pExampleFrame;
