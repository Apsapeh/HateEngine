# Заметки по движку

## Работа с ресурсами: звук, модели, карты, текстуры
Ресурсы будут храниться в специальном формате библиотки rres (https://github.com/raysan5/rres)\
Для архивации в rres будет написана отдельная программа с поддержкой конфигурационных файлов в .json формате
```json
# Пример конфигурационного файла
{
  "password": "some-passowrd",
  "files" : [
    {"files": ["file1", "file2"], "output_file":  "rres-file1_and_file2"},
    {"files": ["file3", "file4"], "output_file":  "rres-file3_and_file4"}
  ]  
}
```

## Системы координат
OGL = {x, y, z} - эталон\
GLM = {-x, -y, -z}\
ReactPhysics3D = {z, y, x}