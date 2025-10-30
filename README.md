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
# keep this the same, these are the names we're replacing
$OldGameName = "XistGame"
```

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
- Disassociate your new game from the template Git repo
- Rename directories and files based on your name
- Replace content in source code based on your name
- Add CoreRedirect based on your name

#### Powershell Procedure:  *(you **must** have configured the variables above)*

```powershell
cd $DevRoot
```

```powershell
# Clone XistGame-Template into $NewGameName
git clone https://github.com/XistGG/XistGame-Template $NewGameName
```

```powershell
cd $NewGameName
```

If you want to switch branches, now is the time!
We're about to nuke the old template `.git` repo from the new game directory.

```powershell
# NUKE the .git repo for XistGame since we're making a new game with a new repo
Remove-Item -force -recurse .git
```

```powershell
# Get list of dirs that need to be renamed
$Dirs = Get-ChildItem -Path $NewGameDir -Recurse -Include "*${OldGameName}*" -Dir `
	| %{ $_.FullName }
```

```powershell
# Rename Dirs
$Dirs | %{ $NewDir = $_ -replace $OldGameName, $NewGameName; Rename-Item $_ $NewDir }
```

```powershell
# Get list of files that need to be renamed and contents replaced
$Files = Get-ChildItem -Path $NewGameDir -Recurse -Include "*${OldGameName}*" -File `
	| %{ $_.FullName }
```

```powershell
# Replace content in files (save to new filenames)
$Files | %{ $NewFile = $_ -replace $OldGameName, $NewGameName; `
	(Get-Content $_) -replace $OldGameName, $NewGameName `
		| Set-Content $NewFile }
```

```powershell
# Delete old files
$Files | Remove-Item
```

```powershell
# Fix names in Config/DefaultEngine.ini CoreRedirects

# This searches for a specific marker that should only exist in our DefaultEngine.ini template
$reSearch = "(?s)^(\[CoreRedirects\][\n\r]+);\+PackageRedirects\=XIST_GAME_PACKAGE_REDIRECT[^\n\r]*"
$reReplace = "`$1+PackageRedirects=(OldName=`"/Script/$OldGameName`", NewName=`"/Script/$NewGameName`", MatchSubstring=true)"

# Generate the redirect for the marker (if it exists) in Config/DefaultEngine.ini
(Get-Content -Raw "Config/DefaultEngine.ini") -replace $reSearch, $reReplace `
	| Set-Content "Config/DefaultEngine.ini.$NewGameName"
```

```powershell
# Replace the template Config/DefaultEngine.ini with the game-specific Config/DefaultEngine.ini.$NewGameName
Move-Item -force "Config/DefaultEngine.ini.$NewGameName" "Config/DefaultEngine.ini"
```

### Optional: Initialize Git repo

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
