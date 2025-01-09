# XistGame -- UE 5.5 Empty Game Template

This is a very simple TopDown game template for quick prototyping and testing.

Clone this repository and then rename it whatever you want.

To rename the project after cloning:

## Setup -- MAKE SURE YOU DO THIS FIRST

```powershell
# Set this to your new game name
$NewGameName = "MyGame"
```

## Rename Procedure -- DO THIS SECOND

```powershell
$OldGameName = "XistGame"  # keep this the same

# Clone XistGame-Template into $NewGameName
git clone https://github.com/XistGG/XistGame-Template $NewGameName

cd $NewGameName

# NUKE the .git repo for XistGame since we're making a new game with a new repo
Remove-Item -force -recurse .git

# Get list of dirs that need to be renamed
$Dirs = Get-ChildItem -Path $NewGameDir -Recurse -Include "*${OldGameName}*" -Dir `
	| %{ $_.FullName }

# Rename Dirs
$Dirs | %{ $NewDir = $_ -replace $OldGameName, $NewGameName; Rename-Item $_ $NewDir }

# Get list of files that need to be renamed and contents replaced
# Explicitly DO NOT match "XistGameMode.*", but do match other XistGame files
$Files = Get-ChildItem -Path $NewGameDir -Recurse -Include "*${OldGameName}*" -File `
	| %{ if ($_.BaseName -ne "XistGameMode") { $_.FullName } }

# Replace content in files (save to new filenames)
$Files | %{ $NewFile = $_ -replace $OldGameName, $NewGameName; `
	(Get-Content $_) -replace $OldGameName, $NewGameName `
		| Set-Content $NewFile }

# Delete old files
$Files | Remove-Item
```

## Manual Work

After you do this, look in the `Config` directory and update the configs for your game.

In particular you will want to make a manual edit to the top of `Config/DefaultEngine.ini`

```ini
[CoreRedirects]
+PackageRedirects=(OldName="/Script/XistGame", NewName="/Script/MyGame", MatchSubstring=true)
```

(Change `MyGame` to whatever your game name is).

## Follow-up

After you complete the steps above, open Rider, build the project, start the UEditor,
then "Resave All" of the `Content` assets.  This will save them all using your new
project name.

After you do that, you can remove the `[CoreRedirects]` section from `DefaultEngine.ini`
