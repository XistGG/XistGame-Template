# XistGame

You have successfully initialized your XistGame project.

Make certain to finish the initial setup.
After you do so, remove these steps from the README.

## Initial Setup Steps

Now that you have successfully refactored your XistGame project,
you need to take the following one-time setup steps.

### 1) Configure Files: `Config/*.ini`

Look in the [Config](./Config/) directory and update the configs for your game.

#### 2) Verify auto-generated CoreRedirect

Notice the first few lines in
[Config/DefaultEngine.ini](./Config/DefaultEngine.ini)
, which will look something like the following
if your `$NewGameName` is `MyGame`:

```ini
[CoreRedirects]
+ClassRedirects=(OldName="/Script/XistGame.XistGame...", NewName="/Script/MyGame.MyGame", MatchWildcard=true)
+PackageRedirects=(OldName="/Script/XistGame", NewName="/Script/MyGame")
```

You will need to keep these redirects until you have resaved all of the `Content` assets in UEditor.

### 3) Build XistGame in your IDE

- Open [XistGame.uproject](./XistGame.uproject) in Rider
  - Or for other IDEs, generate project files and open the generated files
- Build Config=`Development` Target=`Editor`

### 4) Resave All Content Assets in UEditor

- Start XistGame Editor from your IDE
- In UEditor, right-click in the `Content` directory and choose `Resave All...`
  - Resave all binary assets to use new game names
- After successful Resave, remove the `[CoreRedirects]` section from `DefaultEngine.ini`

While this isn't strictly necessary, this will make your Editor run faster,
so is **highly** recommended.

CoreRedirects slow down the Editor, especially the `MatchWildcard` type we use here.

For best performance, you will want to resave the assets with the redirects
in place, and then remove the Redirects.

Ideally, perform this Resave **before** you commit your initial binaries to Git.
