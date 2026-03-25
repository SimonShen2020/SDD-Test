// Module: Plugin/Huarui
// Responsibility boundary: Wrap Huarui SDK and expose plugin-side implementations.
// Dependency red line: Keep SDK usage in plugin layer; do not leak SDK into Core.

namespace camera
{
    namespace plugin_huarui
    {

        int camera_huarui_stub_symbol()
        {
            return 0;
        }

    } // namespace plugin_huarui
} // namespace camera
