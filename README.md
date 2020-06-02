# bbm
Building blocks manager

## Init
Dans `.vscode/settings.json`, définir les variables d'environnement nécessaires au build (mingw et Qt).
```
    "terminal.integrated.env.windows": {
        "PATH": "E:\\App\\Qt\\Tools\\mingw810_64\\bin;${env:PATH};E:\\App\\Qt\\5.15.0\\mingw81_64\\bin"
    }
```

## Taches VSCODE
`clean`: Suppresion du répertoire du build
`cmake`: Génération du makefile (en mode release)
`cmake-dbg`: Génération du makefile en mode debug
`make`: Compilation
`test`: Compilation & execution des tests automatiques
`windeployqt`: copier les dépendances Qt au niveau de l'application bbm-ui
