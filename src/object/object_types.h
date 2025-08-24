#pragma once

// clang-format off
enum ObjectType {
    // Object->Node
    ObjectTypeNode,
        // Object->Node->CanvasItem
        ObjectTypeCanvasItem,
            // Object->Node->CanvasItem->Control
            ObjectTypeControl,
                // Object->Node->CanvasItem->Control->Viewport
                ObjectTypeViewport,
        // Object->Node->Node3D
        ObjectTypeNode3D,
        // Object->Node->Window
        ObjectTypeWindow
};
// clang-format on
