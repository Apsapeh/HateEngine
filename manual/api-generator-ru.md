## Описание

Генератор API написан с libclang на python, для запуска желательно иметь **uv**, он сам подтянет нужные зависимости. Пока генератор написан не до конца, в планах добавить нормальный cli интерфейс, вывод в json, поддержку генерации API для других языков.


## Как использовать генератор при разработке

Генератор работает через Doxygen коммментарии, чтоб пометить, что для какой-то сущности нужно сгенерировать API, в Doxygen комментарий необходимо добавить тег ***@api***

<br/>

### Функции:
###### Поддерживаемые теги:
- *@api* - указывает, что для этой функции нужно сгенерировать API

##### Пример:
```c
// some_header.h

/**
 * @brief Возвращает сумму 'a' и 'b'
 *
 * @api
 */
i32 add(i32 a, i32 b);
```

##### Пример использования в игре:
```c
// game.c

#include <HateEngineAPI.h>

void _process(double delta) {
    printf("%d\n", add((i32)delta, (i32)delta));
}
```

<br/>


### Структуры:
###### Поддерживаемые теги:
- *@api \<mode\>*  - указывает, что для этой структуры нужно сгенерировать API и режим генерации
- *@api_config \<json\>* - Параметры генерации, нужно для *@api server*. Параметром указывается JSON

###### Режимы генерации:
- \**пусто*\* - Генерирует непрозрачный указател на структуру (`typedef struct MyStruct MyStruct;`)
- *forward* - Генерирует прозрачную структуру с полями (`typedef struct Vec2 {float x; float f;} Vec2;`)
- *server* - Генерирует непорозрачную структуру и по полям генерирует функции с конфигурируемым префиксом

###### JSON конфигурация для ***server***:
- *fn_prefix* - Префикс, который добавится к генерируемым из полей функциям
- *init_method* - Имя метода, который будет вызываться для инициализации функций сервера

##### Пример (пусто):
```c
// some_header.h

/**
 * @api
 */
struct Mesh {   // Можно typedef struct Mesh {...} Mesh; или typedef struct {...} Mesh;
    vec_float points;
    vec_float normals;
    vec_int indecies;
    ......
};

/**
 * @api
 */
Mesh* mesh_new(void);
```

##### Пример использования в игре (пусто):
```c
// game.c

#include <HateEngineAPI.h>

// Просто создаст typedef struct Mesh Mesh;

void _process(double delta) {
    Mesh* mesh = mesh_new();
}
```


##### Пример (forward):
```c
// some_header.h

/**
 * @api
 */
typedef struct Vec2 {
    float x;
    float y;
} Vec2;

```

##### Пример использования в игре (forward):
```c
// game.c

#include <HateEngineAPI.h>

void _process(double delta) {
    Vec2 vec = (Vec2){1.0f, 1.0f};
    printf("%f, %f", vec.x, vec.y);
}
```


##### Пример (server):
```c
// some_header.h

/**
 * @api
 * @api_config {
 *     "fn_prefix": "window_server_",
 *     "init_method": "___hate_engine_runtime_init_window_server"
 * }
 */
typedef struct {
    /**
     * @warning Можно указывать комментарии к методам
     */
    Error (*create_window)(c_str title);
} WindowServerBackend;

/**
 * @warning Обязательно нужно создать функцию с такой сигнатурой:
 *     Error (aka const char*) {fn_prefix}_backend_get_function({struct}*, c_str (aka const char*), void (**)(void))
 *
 * @api
 */
Error window_server_backend_get_function(
        WindowServerBackend* backend, c_str name, void (**function)(void)
);

```

##### Пример использования в игре (server):
```c
// game.c

#include <HateEngineAPI.h>

void _process(double delta) {
    window_server_create_window((c_str)"New window");
}
```


<br/>


### Псевдонимы типов (typedef):

###### Поддерживаемые теги:
- *@api* - указывает, что для этой типа нужно сгенерировать API

**Поддерживает непрозрачные структуры typedef struct \<name\> \<name\>;** 

##### Пример:
```c
// some_header.h

/**
 * @brief
 *
 * @api
 */
typedef uint64_t UID;

```

##### Пример использования в игре:
```c
// game.c

#include <HateEngineAPI.h>

void _process(double delta) {
    UID uid = 0;
    ++uid;
}
```


<br/>


### Define перечисления (API Enum):
Тут другой принцип, в обычном многострочном комментарии указыватся структура вида *API ENUM json*

###### Поля JSON:
- *name* - Имя перечисления
- *type* - Тип перечисления
- *values* - Массив значений
    - Первое значение - Имя значения
    - Второе значение - Значение

##### Пример:
```c
// some_header.h

/*
API ENUM {
        "name": "WindowServerWindowVSync",
        "type": "char",
        "values": [
                ["Disabled", "'d'"],
                ["Enabled", "'e'"],
                ["EnabledAsync", "'a'"]
        ]
}
*/

// Просто создаст #define в SNAKE_UPPERCASE
```

##### Пример использования в игре:
```c
// game.c

#include <HateEngineAPI.h>

void _process(double delta) {
    set_vsync(WINDOW_SERVER_WINDOW_V_SYNC_ENABLED);
}
```