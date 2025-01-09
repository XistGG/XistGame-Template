# XistGame UE 5.5 Empty Game Template

This is a very simple TopDown game template for quick prototyping and testing.

Clone this repository and then rename it whatever you want.

To rename the project after cloning:

```powershell
# Set this to the dir where you cloned this repo
$NewGameDir = "C:/Path/To/Clone"

# Set this to your new game name
$NewGameName = "MyGame"

$OldGameName = "XistGame"  # keep this the same

# NUKE the .git repo for XistGame since we're making a new game with a new repo
rmdir -force -recurse $NewGameDir/.git

# Get list of dirs that need to be renamed
$Dirs = Get-ChildItem -Path $NewGameDir -Recurse -Include "*${OldGameName}*" -Dir | %{ $_.FullName }

# Get list of files that need to be renamed and contents replaced
$Files = Get-ChildItem -Path $NewGameDir -Recurse -Include "*${OldGameName}*" -File | %{ $_.FullName }

# Replace content in files (save to new filenames)
$Files | %{ $NewFile = $_ -replace $OldGameName, $NewGameName; (Get-Content $_) -replace $OldGameName, $NewGameName | Set-Content $NewFile }

# Delete old files
$Files | Remove-Item

# Rename Dirs
$Dirs | %{ $NewDir = $_ -replace $OldGameName, $NewGameName; Rename-Item $_ $NewDir }
```

