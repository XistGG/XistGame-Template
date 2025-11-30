# XistGame -- UE 5.7 Empty Game Template

Requires one of:
[UE 5.8](https://github.com/XistGG/XistGame-Template/tree/ue-5.8) |
[UE 5.7](https://github.com/XistGG/XistGame-Template/tree/ue-5.7)

This is a very simple TopDown game template for quick prototyping and testing.

Follow the procedure below to fully customize your own new game based on this template.

**NOTICE: DO NOT BLINDLY CUT/PASTE POWERSHELL INTO YOUR TERMINAL**

Never trust someone else to think for you. Make sure you read each of these lines
and you understand what it does before you execute it.

## SETUP: Configure Variables for your Project

#### Powershell Procedure:

```powershell
## CHANGE THIS to your game name
$NewGameName = "MyGame"
```
```powershell
## CHANGE THIS to be your desired project root directory
$DevRoot = "C:/Dev"
```

## PROCEDURE: Setup Variables (above) then:

- Clone https://github.com/XistGG/XistGame-Template
- Rename Clone directory to `$NewGameName`

#### Powershell Procedure:  *(you **must** have configured the variables above)*

```powershell
# Make sure we are in your Dev workspace root
cd $DevRoot

# Clone XistGame-Template into $NewGameName
git clone https://github.com/XistGG/XistGame-Template $NewGameName
```

**If you want to switch branches, now is the time!**

When you are happy with the branch you are on:

- Rename directories and files based on your name
- Replace content in source code based on your name
- Add CoreRedirect based on your name

```powershell
# Make sure we are in your Dev workspace root
cd $DevRoot/$NewGameName

# NOTICE: IF YOU WANT TO SWITCH BRANCHES, DO SO NOW. For example:
# git checkout ue-5.7
```
```powershell
# This refactor might take a while depending on your hardware
./Scripts/Refactor.ps1 -NewGameName $NewGameName
```

#### Cleanup files we do not want in the new project

- Remove the [Scripts](./Scripts/) directory *(we ran the 1 script above, and we will not need it again)*
- Disassociate your new game from the template Git repo

```powershell
# Make sure we are in your Dev workspace root
cd $DevRoot/$NewGameName

# Don't want Scripts dir anymore
Remove-Item -Force -Recurse Scripts

# Disassociate our new game project from the template Git repo.
# NUKE the .git repo for XistGame since we're making a new game with a new repo.
Remove-Item -Force -Recurse .git
```

### RECOMMENDED: Initialize Your New Game Git repo

For now, you can just keep the Git config from the Template repo
unless you need to modify them specifically for your project needs.

```powershell
# Make sure we are in your Dev workspace root
cd $DevRoot/$NewGameName

git init

git lfs install

git add .gitignore .gitattributes

git commit -m "Initialize UE5 Git Repo"
```

If you get errors related to `git lfs`, you may need to
[install Git LFS](https://www.google.com/search?q=install+Git+LFS)

For more info regarding setting up a new UE5 Git repository, see
https://github.com/XistGG/UE5-Git-Init

### FINALLY: Open Your Game in Rider

- Open `$DevRoot/$NewGameName/$NewGameName.uproject` in Rider
- Read your new Game README.md (it is copied from [README.Template.md](./README.Template.md))
  - Follow the setup instructions there
