# bbm
Building blocks manager

## Init
Dans `.vscode/settings.json`, définir les variables d'environnement nécessaires au build (mingw et Qt).

    "terminal.integrated.env.windows": {
        "PATH": "E:\\App\\Qt\\Tools\\mingw810_64\\bin;${env:PATH};E:\\App\\Qt\\5.15.0\\mingw81_64\\bin"
    },
    "cmake.configureSettings": { 
        "CMAKE_PREFIX_PATH": "E:\\App\\Qt\\5.15.0\\mingw81_64"
    }

## Taches VSCODE
* `clean`: Suppresion du répertoire du build
* `windeployqt`: copier les dépendances Qt au niveau de l'application bbm-ui et du test test-bbm

