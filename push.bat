@echo off
set commitMsg=%~1
if "%commitMsg%"=="" set commitMsg=Mise a jour du jeu

echo [GIT] Ajout des fichiers...
git add .

echo [GIT] Enregistrement du commit : "%commitMsg%"
git commit -m "%commitMsg%"

echo [GIT] Envoi vers GitHub...
git push

echo [GIT] Termine avec succes !
