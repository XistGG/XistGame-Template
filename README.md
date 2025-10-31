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

## CHANGE THIS to be your desired project root directory
$DevRoot = "C:/Dev"
```

## PROCEDURE: Setup Variables (above) then:

- Clone https://github.com/XistGG/XistGame-Template
- Rename Clone directory to `$NewGameName`
- Disassociate your new game from the template Git repo
- Rename directories and files based on your name
- Replace content in source code based on your name
- Add CoreRedirect based on your name

#### Powershell Procedure:  *(you **must** have configured the variables above)*

```powershell
# Make sure we are in your Dev workspace root
cd $DevRoot

# Clone XistGame-Template into $NewGameName
git clone https://github.com/XistGG/XistGame-Template $NewGameName
```

If you want to switch branches, now is the time!

When you are happy with the branch you are on:

```powershell
cd $DevRoot/$NewGameName

# This refactor might take a while depending on your hardware
./Scripts/Refactor.ps1 -NewGameName $NewGameName
```

#### Cleanup files we do not want in the new project

```powershell
cd $DevRoot/$NewGameName

# Don't want Scripts dir anymore
rmdir -force -recurse Scripts

# Disassociate our new game project from the template Git repo.
# NUKE the .git repo for XistGame since we're making a new game with a new repo.
Remove-Item -Force -Recurse $DevRoot/$NewGameName/.git
```

### Optional: Initialize New Git repo

If you want to initialize a new Git repo for your new game, follow the instructions here:

https://github.com/XistGG/UE5-Git-Init

After you have created **AT LEAST** `.gitignore` and `.gitattributes`,
run these terminal commands one at a time:

```powershell
git init

git lfs install

git add .gitignore .gitattributes

git commit -m "Initialize UE5 Git Repo"
```

If you get errors related to `git lfs`, you may need to
[install Git LFS](https://www.google.com/search?q=install+Git+LFS)

## PROJECT SPECIFIC: Configure Files: `Config/*.ini`

After you do this, look in the `Config` directory and update the configs for your game.

#### Verify auto-generated CoreRedirect

Notice the first few lines in `Config/DefaultEngine.ini`, which will look something like this:

```ini
[CoreRedirects]
+PackageRedirects=(OldName="/Script/XistGame", NewName="/Script/MyGame", MatchSubstring=true)
```

- Verify that the `NewName="/Script/MyGame"` part has the correct name of your game.
- If you skipped the rename search/replace command, make sure you manually add a `PackageRedirect`
  from `OldName="/Script/XistGame"` to `NewName="/Script/$NewGameName"`, like the example above.

## Optional, Recommended Follow-up

- Open the `.uproject` in Rider
  - Or for other IDEs, generate project files and open the generated files
- Build Config=`Development` Target=`Editor`
- Run the Development Editor
- Right click in the `Content` directory and choose `Resave All...`
  - Resave all binary assets to use new game names

After you do that, you can remove the `[CoreRedirects]` section from `DefaultEngine.ini`
since the binary content will now be updated to use your new names.
