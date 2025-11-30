#!/usr/bin/env pwsh
#
# XistGame-Template/Scripts/Refactor.ps1
#
# @see https://github.com/XistGG/XistGame-Template
#
# Once you run this to refactor the template, you do not need to keep this
# script around in your new project repository. Feel free to delete it.
#
# It only needs to continue to live in the XistGame-Template repository.
#

[CmdletBinding()]
param(
	[Parameter()] $NewGameName,
	[Parameter()] $DevRoot = $null,
	[Parameter()] $OldGameName = "XistGame"
)

################################################################################
##  Check Powershell >= 7

$MinPSVersion = 7

if ($MinPSVersion -gt $PSVersionTable.PSVersion.Major) {
    Write-Error "This system is using PowerShell version $($PSVersionTable.PSVersion.Major), which is not adequate to run Refactor.ps1"
    throw "Powershell $($MinPSVersion)+ is required"
}

################################################################################
##  Check Input, compute defaults as needed

if ("$NewGameName" -eq "")
{
	throw "Usage: Refactor.ps1 -NewGameName Supergame"
}

# If no -DevRoot was specified, infer it based on where this script is located
if ($DevRoot -eq $null)
{
	$DevRoot = Get-Item -Path (Join-Path $PSScriptRoot ".." "..")

	if (-not $DevRoot -or -not $DevRoot.Exists -or -not $DevRoot.PSIsContainer) {
		 throw "Unexpected directory structure"
	}

	$DevRoot = $DevRoot.FullName
}

if (-not ($NewGameName -match "^[a-zA-Z0-9]+$"))
{
	throw "NewGameName [$NewGameName] is an invalid format"
}

if (-not (Test-Path -Path $DevRoot -PathType Container))
{
	throw "DevRoot [$DevRoot] is not a valid directory"
}

################################################################################
##  Check Input, compute defaults as needed

$gameDir = Join-Path $DevRoot $NewGameName

Push-Location $gameDir 2> $null

if (-not ((Get-Location).Path -eq $gameDir)) {
	throw "Cannot cd to $gameDir"
}

try
{
	# Get list of dirs that need to be renamed
    $Dirs = Get-ChildItem -Recurse -Include "*${OldGameName}*" -Dir `
    	| %{ $_.FullName }

	# Rename Dirs
	$Dirs | %{ $NewDir = $_ -replace $OldGameName, $NewGameName; `
		Rename-Item $_ $NewDir }

	# Get list of files that need to be renamed and contents replaced
	$OldFiles = Get-ChildItem -Recurse -Include "*${OldGameName}*" -File `
    	| %{ $_.FullName }

	# All ini files even if they don't require rename
	$KeepFiles = Get-ChildItem -Path Config -Recurse -Include "*.ini" -File `
    	| %{ $_.FullName }

	# Replace content in files (save to new filenames if needed).
	# Two operations so the read completes before the write begins;
	# supports files that do not change names.
	$AllFiles = @()  # keep list initialization
	$AllFiles = $AllFiles + $KeepFiles + $OldFiles
	$AllFiles | %{ `
		$NewFile = $_ -replace $OldGameName, $NewGameName; `
		$NewContent = (Get-Content $_) `
			-creplace $OldGameName.ToUpper(), $NewGameName.ToUpper() `
			-creplace $OldGameName, $NewGameName; `
		$NewContent > $NewFile; `
	}

	# Delete $OldFiles only. Do not delete $KeepFiles
	$OldFiles | Remove-Item

	# Fix names in Config/DefaultEngine.ini CoreRedirects

	$ClassRedirect = "+ClassRedirects=(OldName=`"/Script/$OldGameName.$OldGameName...`", NewName=`"/Script/$NewGameName.$NewGameName`", MatchWildcard=true)"
	$PackageRedirect = "+PackageRedirects=(OldName=`"/Script/$OldGameName`", NewName=`"/Script/$NewGameName`")"

	# This searches for a specific marker that should only exist in our DefaultEngine.ini template
	$newline = [Environment]::NewLine
	$reSearch = "(?s)^(\[CoreRedirects\][\n\r]+);; XIST_GAME_ADD_REDIRECTS_HERE[^\n\r]*"
	$reReplace = "`$1$ClassRedirect$newline$PackageRedirect"

	# Generate the redirect for the marker (if it exists) in Config/DefaultEngine.ini
	(Get-Content -Raw "Config/DefaultEngine.ini") -replace $reSearch, $reReplace `
		| Set-Content "Config/DefaultEngine.ini.$NewGameName"

	# Replace the template Config/DefaultEngine.ini with the game-specific Config/DefaultEngine.ini.$NewGameName
	Move-Item -Force "Config/DefaultEngine.ini.$NewGameName" Config/DefaultEngine.ini

	# Replace the XistGame-Template README with the new refactored game name README
	Move-Item -Force README.Template.md README.md
}
finally
{
	Pop-Location
}
