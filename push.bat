@echo off
set commitMsg=%~1
if "%commitMsg%"=="" set commitMsg=Amelioration de l'interface graphique et ajout de nouvelles fonctionnalités

echo [GIT] Ajout des fichiers...
git add .

echo [GIT] Enregistrement du commit : "%commitMsg%"
git commit -m "%commitMsg%"

echo [GIT] Envoi vers GitHub...
git push

echo [GIT] Termine avec succes !
