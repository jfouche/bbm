# bbm
Building blocks manager

## Init
Les outils préconnisés, et expliqués dans ce documents sont :
- Qt
- VSCode
    - CMake Tools plugin
    - C/C++ plugin
- cppcheck

Dépendance : Qt 

Dans `.vscode/settings.json`, définir les variables nécessaires au build.
* Pour permettre l'intellisense
```
{
    ...
    "QT_DIR":"E:\\App\\Qt\\5.15.0\\mingw81_64",
    "MINGW_DIR": "E:\\App\\Qt\\Tools\\mingw810_64",
}
```
* Pour permettre `windeployqt`
```
{
    ...
    "terminal.integrated.env.windows": {
        "PATH": "${env:PATH};E:\\App\\Qt\\5.15.0\\mingw81_64\\bin"
    }
}
```
* Pour ajouter Qt 
```
    "cmake.configureSettings": { 
        "CMAKE_PREFIX_PATH": "E:\\App\\Qt\\5.15.0\\mingw81_64"
    }
```

## Taches VSCODE
* `clean`: Suppresion du répertoire du build
* `windeployqt`: copier les dépendances Qt au niveau de l'application bbm-ui et du test test-bbm

