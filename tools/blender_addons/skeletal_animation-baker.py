bl_info = {
    "name": "HateEngine Skeletal Animation Baker",
    "blender": (2, 90, 0),
    "category": "3D View",
}

import bpy

# Глобальная переменная для хранения последнего выбранного объекта
last_selected_obj = None

selected_animations = {}

# Панель для HateEngine Animation Baker
class HateEngineAnimationBakerPanel(bpy.types.Panel):
    """Creates a Panel in the 3D Viewport for HateEngine Animation Baking"""
    bl_label = "HE Animation Baker"
    bl_idname = "he_anim_baker"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category = 'HE Anim Baker'

    def draw(self, context):
        layout = self.layout

        # Проверяем, есть ли выбранный объект
        obj = context.object

        # Если объект выбран, отображаем его имя
        if obj:
            row = layout.row()
            row.label(text=f"Selected Object: {obj.name}")

        if obj.animation_data:
            row = layout.row()
            row.label(text="Current Action: " + obj.animation_data.action.name)

            # Если объект имеет действия (Actions), выводим их в выпадающий список
            # if obj.animation_data:
            #     row = layout.row()
            #     row.prop(context.scene, "action_list", text="Selected Actions")
            # else:
            #     row = layout.row()
            #     row.label(text="Empty")
            #if selected_animations.get(obj.name) != None:
            row = layout.row()
            row.prop(context.scene, "action_list", text="Selected Actions")

            #row = layout.row()
            #row.prop(context.scene, "start_frame_selected", text="Start Frame")
            #row = layout.row()
            #row.prop(context.scene, "end_frame_selected", text="End Frame")

            row = layout.row()
            row.operator("wm.unselect_action_operator", text="Unselect Action")

            row = layout.row()
            row.prop(context.scene, "all_actions_list", text="All Actions")

            

            # Поля для выбора начального и конечного кадров
            row = layout.row()
            row.prop(context.scene, "start_frame", text="Start Frame")
            row = layout.row()
            row.prop(context.scene, "end_frame", text="End Frame")

            row = layout.row()
            row.operator("wm.select_action_operator", text="Select Action")

            row = layout.row()

            # Кнопки для выпечки анимаций
            row = layout.row()
            row.operator("wm.bake_current_animation_operator", text="Bake Current Action")
            row = layout.row()
            row.operator("wm.bake_selected_animation_operator", text="Bake Selected Action")
            row = layout.row()
            row.operator("wm.bake_all_animations_operator", text="Bake All")

            
        else:
            row = layout.row()
            row.label(text="No object selected")

        # Кнопка для экспорта текстового файла
        #row = layout.row()
        #row.operator("wm.export_hea_operator", text="Export HEA")

        


class SelectActionOperator(bpy.types.Operator):
    """Bake Current Animation Operator"""
    bl_idname = "wm.select_action_operator"
    bl_label = "Select Action"

    def execute(self, context):
        selected_action = context.scene.all_actions_list

        

        if selected_action != "NONE":
            if selected_animations.get(context.object) == None:
                selected_animations[context.object] = []
            
            if selected_action not in selected_animations[context.object]:
                selected_animations[context.object].append(selected_action)

            actions = [(action, action, "") for action in get_object_actions(context.object)]
            if actions:
                bpy.types.Scene.action_list = bpy.props.EnumProperty(items=actions)
            else:
                bpy.types.Scene.action_list = bpy.props.EnumProperty(items=[("NONE", "No Actions", "")])

        return {'FINISHED'}
        

class UnselectActionOperator(bpy.types.Operator):
    """Bake Current Animation Operator"""
    bl_idname = "wm.unselect_action_operator"
    bl_label = "Unselect Action"

    def execute(self, context):
        selected_action = context.scene.action_list

        if selected_action != "NONE":
            if selected_animations.get(context.object) != None:
                if selected_action in selected_animations[context.object]:
                    selected_animations[context.object].remove(selected_action)
                    actions = [(action, action, "") for action in get_object_actions(context.object)]
                    if actions:
                        bpy.types.Scene.action_list = bpy.props.EnumProperty(items=actions)
                    else:
                        bpy.types.Scene.action_list = bpy.props.EnumProperty(items=[("NONE", "No Actions", "")])

        return {'FINISHED'}


# Оператор для выпекания текущей анимации
class BakeCurrentAnimationOperator(bpy.types.Operator):
    """Bake Current Animation Operator"""
    bl_idname = "wm.bake_current_animation_operator"
    bl_label = "Bake Current Animation"

    def execute(self, context):
        obj = context.object

        bake(obj.animation_data.action.name)
        return {'FINISHED'}

        # selected_action = context.scene.action_list
        # if selected_action != "NONE":
        #     # Ваша логика для выпекания текущей анимации
        #     self.report({'INFO'}, f"Baking current action: {selected_action}")
        # else:
        #     self.report({'WARNING'}, "No action selected")
        # return {'FINISHED'}

class BakeSelectedAnimationOperator(bpy.types.Operator):
    """Bake Selected Animation Operator"""
    bl_idname = "wm.bake_selected_animation_operator"
    bl_label = "Bake Selected Animation"

    def execute(self, context):
        selected_action = context.scene.all_actions_list
        bake(selected_action)

        
        return {'FINISHED'}

        pass

# Оператор для выпекания всех анимаций
class BakeAllAnimationsOperator(bpy.types.Operator):
    """Bake All Animations Operator"""
    bl_idname = "wm.bake_all_animations_operator"
    bl_label = "Bake All Animations"

    def execute(self, context):
        obj = context.object

        for action in get_object_actions(obj):
            bake(action)

        return {'FINISHED'}


# Оператор экспорта иерархии коллекций в .hea
class ExportHEAOperator(bpy.types.Operator):
    """Bake All Animations Operator"""
    bl_idname = "wm.export_hea_operator"
    bl_label = "Export HEA"

    def execute(self, context):
        root_collection = bpy.data.collections["HateEngineAnimations"]

        for collection in root_collection.children:
            print("Exporting: " + collection.name)

        return {'FINISHED'}

# Получаем список доступных анимаций у объекта
def get_object_actions(obj):
    actions = []

    actions_raw = selected_animations.get(obj)

    if actions_raw:
        #print("Доступные действия в Dope Sheet:")
        for action in actions_raw:
            actions.append(action)
            #print(action)
        #print("Нет доступных действий.")

    return actions

def get_all_actions():
    actions = []


    actions_raw = bpy.data.actions

    if actions_raw:
        #print("Доступные действия в Dope Sheet:")
        for action in actions_raw:
            actions.append(action.name)
            #print(action.name)

    return actions


# Обработчик для отслеживания изменений в сцене
def depsgraph_update(scene, depsgraph):
    global last_selected_obj

    actions = [(action, action, "") for action in get_all_actions()]
    if actions:
        bpy.types.Scene.all_actions_list = bpy.props.EnumProperty(items=actions, update=update_end_frame_all_actions)
    else:
        bpy.types.Scene.all_actions_list = bpy.props.EnumProperty(items=[("NONE", "No Actions", "")], update=update_end_frame_all_actions)

    # Проверяем, изменилось ли выделение объекта
    current_obj = bpy.context.active_object

    if selected_animations.get(current_obj) == None:
        selected_animations[current_obj] = []
        

    #if current_obj != last_selected_obj:
    last_selected_obj = current_obj

    # Обновляем список экшенов (Actions) для нового объекта
    if current_obj and current_obj.animation_data:
        actions = [(action, action, "") for action in get_object_actions(current_obj)]
        if actions:
            bpy.types.Scene.action_list = bpy.props.EnumProperty(items=actions)
        else:
            bpy.types.Scene.action_list = bpy.props.EnumProperty(items=[("NONE", "No Actions", "")])
    else:
        bpy.types.Scene.action_list = bpy.props.EnumProperty(items=[("NONE", "No Actions", "")])

    # Обновляем интерфейс
    for area in bpy.context.screen.areas:
        if area.type == 'VIEW_3D':
            area.tag_redraw()


# Обработчик для обновления конечного кадра при выборе действия
def update_end_frame_all_actions(self, context):
    selected_action = context.scene.all_actions_list
    #print("SELECTED: " + selected_action)
    if selected_action != "NONE":
        action = bpy.data.actions.get(selected_action)
        #print("ACTION: " + action)
        if action:
            # Обновляем конечный кадр в зависимости от длины выбранного действия
            context.scene.start_frame = int(action.frame_range[0])
            context.scene.end_frame = int(action.frame_range[1])




# Регистрация свойств и классов
def register():
    bpy.utils.register_class(HateEngineAnimationBakerPanel)
    bpy.utils.register_class(SelectActionOperator)
    bpy.utils.register_class(UnselectActionOperator)
    bpy.utils.register_class(BakeCurrentAnimationOperator)
    bpy.utils.register_class(BakeSelectedAnimationOperator)
    bpy.utils.register_class(BakeAllAnimationsOperator)
    bpy.utils.register_class(ExportHEAOperator)

    # Добавляем свойства для начального и конечного кадров
    bpy.types.Scene.start_frame = bpy.props.IntProperty(name="Start Frame", default=1, min=1)
    bpy.types.Scene.end_frame = bpy.props.IntProperty(name="End Frame", default=1, min=1)
    
    # Регистрируем пустой выпадающий список действий (будет обновляться при выборе объекта)
    bpy.types.Scene.action_list = bpy.props.EnumProperty(items=[("NONE", "No Actions", "")])
    bpy.types.Scene.all_actions_list = bpy.props.EnumProperty(items=[("NONE", "No Actions", "")], update=update_end_frame_all_actions)

    # Регистрируем обработчик обновления графа зависимостей (depsgraph)
    bpy.app.handlers.depsgraph_update_post.append(depsgraph_update)


def unregister():
    bpy.utils.unregister_class(HateEngineAnimationBakerPanel)
    bpy.utils.unregister_class(SelectActionOperator)
    bpy.utils.unregister_class(UnselectActionOperator)
    bpy.utils.unregister_class(BakeCurrentAnimationOperator)
    bpy.utils.unregister_class(BakeSelectedAnimationOperator)
    bpy.utils.unregister_class(BakeAllAnimationsOperator)
    bpy.utils.unregister_class(ExportHEAOperator)

    # Удаляем свойства
    del bpy.types.Scene.start_frame
    del bpy.types.Scene.end_frame
    del bpy.types.Scene.action_list
    del bpy.types.Scene.all_actions_list

    # Убираем обработчик при удалении аддона
    bpy.app.handlers.depsgraph_update_post.remove(depsgraph_update)



def get_mesh_objects(rig):
    """Получить связанные меши для данной арматуры."""
    mesh_objects = []
    
    # Перебираем все объекты в сцене
    for obj in bpy.data.objects:
        # Проверяем, является ли объект мешем и привязан ли он к арматуре
        if obj.type == 'MESH':
            # Проверяем наличие модификатора "Armature"
            for modifier in obj.modifiers:
                if modifier.type == 'ARMATURE' and modifier.object == rig:
                    mesh_objects.append(obj.name)

    return mesh_objects

def bake(action_name: str):
    # Укажите здесь название объекта с анимацией и количество кадров
    frame_start = 1
    frame_end = 1

    action = bpy.data.actions.get(action_name)
    if action:
        frame_start = int(action.frame_range[0])
        frame_end = int(action.frame_range[1])

    bpy.context.object.animation_data.action = bpy.data.actions[action_name]
    

    # HateEngineAnimations          1
    #    - <Object Name>            2
    #        - <Action Name>        3
    #            - [Frames]      4
    #                - [meshes]

    # 1
    collection_name = f"HateEngineAnimations"
    if collection_name in bpy.data.collections:
        animations_root_collection = bpy.data.collections[collection_name]
    else:
        animations_root_collection = bpy.data.collections.new(collection_name)
        bpy.context.scene.collection.children.link(animations_root_collection)

    rig = bpy.context.active_object

    # 2
    collection_name = f"OBJECT[{rig.name}]"
    if collection_name in bpy.data.collections:
        rig_anim_collection = bpy.data.collections[collection_name]
    else:
        rig_anim_collection = bpy.data.collections.new(collection_name)
        animations_root_collection.children.link(rig_anim_collection)

    # 3
    collection_name = f"ACTION[{action_name}] - {rig.name}"
    if collection_name in bpy.data.collections:
        action_anim_collection = bpy.data.collections[collection_name]
    else:
        action_anim_collection = bpy.data.collections.new(collection_name)
        rig_anim_collection.children.link(action_anim_collection)

    for object_name in get_mesh_objects(rig):
        print("OBJECT_NAME: " + object_name)
        # Получаем ссылку на объект
        try:
            armature = bpy.data.objects[object_name]
        except KeyError:
            print(f"Объект с именем '{object_name}' не найден.")
            exit()

        # Создаем коллекцию для хранения всех покадровых мешей
        #4
        # collection_name = f"MESH - {rig.name}/{action_name}/{object_name}"
        # if collection_name in bpy.data.collections:
        #     collection = bpy.data.collections[collection_name]
        # else:
        #     collection = bpy.data.collections.new(collection_name)
        #     action_anim_collection.children.link(collection)

        # Переходим в объектный режим и выбираем объект
        bpy.ops.object.mode_set(mode='OBJECT')
        bpy.ops.object.select_all(action='DESELECT')
        armature.select_set(True)
        bpy.context.view_layer.objects.active = armature

        # Создаем копии объекта для каждого кадра
        duplicated_objects = []
        for frame in range(frame_start, frame_end + 1):

            

            # Переходим на нужный кадр
            bpy.context.scene.frame_set(frame)

            # Обновляем вид, чтобы отразить изменения
            bpy.context.view_layer.update()

            # Дублируем объект
            bpy.ops.object.duplicate_move()

            # Получаем ссылку на дубликат объекта
            duplicated_object = bpy.context.view_layer.objects.active

            if not duplicated_object:
                print(f"Не удалось дублировать объект на кадре {frame}.")
                continue
            
            # Переименовываем объект, чтобы он соответствовал кадру
            #duplicated_object.name = f"{rig.name}_{action_name}_{object_name}_frame_{frame}"
            duplicated_object.name = f"[{action_name}][{frame}] - {rig.name}/{object_name}"

            # Добавляем дубликат в список
            duplicated_objects.append(duplicated_object)

        counter = frame_start
        # Применяем модификаторы ко всем дубликатам
        for duplicated_object in duplicated_objects:
            bpy.context.view_layer.objects.active = duplicated_object
            bpy.context.scene.frame_set(counter)
            bpy.context.view_layer.update()

            collection_name = f"FRAME[{counter}] - {rig.name}/{action_name}"
            if collection_name in bpy.data.collections:
                collection = bpy.data.collections[collection_name]
            else:
                collection = bpy.data.collections.new(collection_name)
                action_anim_collection.children.link(collection)

            counter += 1
            print(duplicated_object.modifiers)
            # Применяем модификаторы, чтобы зафиксировать анимацию на этом кадре
            for modifier in duplicated_object.modifiers:
                print(modifier.name)
                bpy.ops.object.modifier_apply(modifier=modifier.name)

            # Удаляем связь с арматурой у нового объекта (если есть)
            if duplicated_object.find_armature():
                bpy.ops.object.parent_clear(type='CLEAR_KEEP_TRANSFORM')

            # Проверяем, находится ли объект уже в коллекции
            if duplicated_object.name not in collection.objects:
                # Перемещаем новый объект в коллекцию
                collection.objects.link(duplicated_object)

            # Удаляем объект из активной коллекции, если он не должен там быть
            if duplicated_object.name in bpy.context.collection.objects:
                bpy.context.collection.objects.unlink(duplicated_object)
            print(duplicated_object)

    print("Преобразование завершено!")



if __name__ == "__main__":
    register()
