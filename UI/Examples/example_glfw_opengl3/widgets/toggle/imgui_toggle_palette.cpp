#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif // IMGUI_DEFINE_MATH_OPERATORS

#include "imgui_toggle_palette.h"
#include "imgui_toggle_math.h"

#include "imgui.h"


using namespace ImGuiToggleMath;

void ImGui::UnionPalette(ImGuiTogglePalette* target, const ImGuiTogglePalette* candidate, const ImVec4 colors[], bool v)
{

    target->Knob = ImVec4(0.149f, 0.502f, 0.922f, 1.0f); //colors[ImGuiCol_Text];
    target->KnobHover = ImVec4(0.149f, 0.502f, 0.922f, 0.8f);
    target->Frame = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    target->FrameHover = colors[!v ? ImGuiCol_FrameBgHovered : ImGuiCol_ButtonHovered];
    target->FrameBorder = ImVec4(0.882f, 0.882f, 0.882f, 1.0f);
    target->FrameShadow = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
    target->KnobBorder = colors[ImGuiCol_Border];
    target->KnobShadow = colors[ImGuiCol_BorderShadow];
    target->A11yGlyph = colors[!v ? ImGuiCol_FrameBg : ImGuiCol_Text];

    // if the user didn't provide a candidate, just provide the theme colored palette.
    if (candidate == nullptr)
    {
        return;
    }

    // if the user did provide a candidate, populate all non-zero colors
#define GET_PALETTE_POPULATE_NONZERO(member) \
    do { \
        if (IsNonZero(candidate->member)) \
        { \
            target->member = candidate->member; \
        } \
    } while (0)

    GET_PALETTE_POPULATE_NONZERO(Knob);
    GET_PALETTE_POPULATE_NONZERO(KnobHover);
    GET_PALETTE_POPULATE_NONZERO(Frame);
    GET_PALETTE_POPULATE_NONZERO(FrameHover);
    GET_PALETTE_POPULATE_NONZERO(FrameBorder);
    GET_PALETTE_POPULATE_NONZERO(FrameShadow);
    GET_PALETTE_POPULATE_NONZERO(KnobBorder);
    GET_PALETTE_POPULATE_NONZERO(KnobShadow);
    GET_PALETTE_POPULATE_NONZERO(A11yGlyph);

#undef GET_PALETTE_POPULATE_NONZERO
}

void ImGui::BlendPalettes(ImGuiTogglePalette* result, const ImGuiTogglePalette& a, const ImGuiTogglePalette& b, float blend_amount)
{
    // a quick out for if we are at either end of the blend.
    if (ImApproximately(blend_amount, 0.0f))
    {
        *result = a;
        return;
    }
    else if (ImApproximately(blend_amount, 1.0f))
    {
        *result = b;
        return;
    }


#define BLEND_PALETTES_LERP(member) \
    do { \
        result->member = ImLerp(a.member, b.member, blend_amount); \
    } while (0)

    BLEND_PALETTES_LERP(Knob);
    BLEND_PALETTES_LERP(KnobHover);
    BLEND_PALETTES_LERP(Frame);
    BLEND_PALETTES_LERP(FrameHover);
    BLEND_PALETTES_LERP(FrameBorder);
    BLEND_PALETTES_LERP(FrameShadow);
    BLEND_PALETTES_LERP(KnobBorder);
    BLEND_PALETTES_LERP(KnobShadow);
    BLEND_PALETTES_LERP(A11yGlyph);

#undef BLEND_PALETTES_LERP
}